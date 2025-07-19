#include <Servo.h>
#include <NewPing.h>

#define TRIG_PIN 4
#define ECHO_PIN 8
#define MAX_DISTANCE 200
#define WALL_THRESHOLD 30
#define STOP_DISTANCE 27

#define SPEED 100
#define MOTOR_ENA 11  // Motor kanan (PWM)
#define MOTOR_IN1 12  // Motor kanan
#define MOTOR_IN2 13
#define MOTOR_ENB 5   // Motor kiri (PWM)
#define MOTOR_IN3 6   // Motor kiri
#define MOTOR_IN4 7

#define FLAME_LEFT_PIN   A4
#define FLAME_FRONT_PIN  A5
#define FLAME_RIGHT_PIN  A3

#define PUMP_RELAY_PIN   A2 // Gunakan NO relay, aktif saat HIGH (active-HIGH relay)

#define API_TERPANTAU      1014  // Tidak ada api jika semua > ini
#define API_MENDEKATI       950  // Ada api di kejauhan jika < 950
#define API_DEPAN_DEKAT     744  // Api sangat dekat jika < 744
#define SENSOR_TOLERANCE    30   // Toleransi perbedaan sensor kiri-kanan untuk lurus

const int SERVO_KANAN = 40; //Derajat servo
const int SERVO_KIRI  = 180;
const int SERVO_DEPAN = 110;

Servo myServo;
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

bool keKanan = true;
bool scanning = false;
bool alreadyBacked = false;
bool fireDetected = false;
bool fireFighting = false;
int servoPos = 110;
int originalServoPos = 110;

enum Mode {
  WALL_FOLLOWING,
  FIRE_DETECTED,
  FIRE_FIGHTING
};

Mode currentMode = WALL_FOLLOWING;

void setup() {
  Serial.begin(9600);
  Serial.println("=== ROBOT PEMADAM API WALL FOLLOWER ===");
  Serial.println("Inisialisasi sistem...");

  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(MOTOR_IN3, OUTPUT);
  pinMode(MOTOR_IN4, OUTPUT);
  pinMode(MOTOR_ENA, OUTPUT);
  pinMode(MOTOR_ENB, OUTPUT);

  pinMode(FLAME_LEFT_PIN, INPUT);
  pinMode(FLAME_FRONT_PIN, INPUT);
  pinMode(FLAME_RIGHT_PIN, INPUT);
  pinMode(PUMP_RELAY_PIN, OUTPUT);

  myServo.attach(9);
  myServo.write(originalServoPos);
  delay(500);

  stopMotors();
  digitalWrite(PUMP_RELAY_PIN, LOW); // Pompa mati

  Serial.println("Sistem siap! Mulai navigasi wall follower...");
  Serial.println("=====================================");
}

void loop() {
  int flameLeft  = analogRead(FLAME_LEFT_PIN);
  int flameFront = analogRead(FLAME_FRONT_PIN);
  int flameRight = analogRead(FLAME_RIGHT_PIN);

  Serial.print("Flame - Left: ");
  Serial.print(flameLeft);
  Serial.print(" | Front: ");
  Serial.print(flameFront);
  Serial.print(" | Right: ");
  Serial.println(flameRight);

  if (flameLeft < API_TERPANTAU || flameFront < API_TERPANTAU || flameRight < API_TERPANTAU) {
    currentMode = FIRE_DETECTED;
    fireDetected = true;
  } else {
    currentMode = WALL_FOLLOWING;
    fireDetected = false;
  }

  switch (currentMode) {
    case WALL_FOLLOWING:
      wallFollowerMode();
      break;
      
    case FIRE_DETECTED:
      fireDetectionMode(flameLeft, flameFront, flameRight);
      break;
  }

  delay(200);
}

void wallFollowerMode() {
  if (fireDetected) return; // Jika ada api, keluar dari mode ini
  
  Serial.println("Mode: WALL FOLLOWING");
  
  if (!scanning) {
    delay(300);

    int frontDistance = sonar.ping_cm();
    Serial.print("Jarak Depan: ");
    Serial.println(frontDistance);

    if (frontDistance > 0 && frontDistance < STOP_DISTANCE && !alreadyBacked) {
      Serial.println("Terlalu dekat dinding - Mundur");
      mundur();
      delay(400);
      stopMotors();
      alreadyBacked = true;
      scanning = true;
    } else if (frontDistance > WALL_THRESHOLD || frontDistance == 0) {
      Serial.println("Jalan bebas - Maju");
      maju();
      alreadyBacked = false;
    } else {
      Serial.println("Dinding terdeteksi - Mulai scan");
      stopMotors();
      scanning = true;
    }
  }

  if (scanning) {
    performUltrasonicScan();
  }
}

void performUltrasonicScan() {
  Serial.println("Melakukan scan ultrasonik...");
  
  int distances[3]; // Kiri, Tengah, Kanan
  int angles[3] = {45, 110, 180};
  
  for (int i = 0; i < 3; i++) {
    myServo.write(angles[i]);
    delay(400);
    distances[i] = sonar.ping_cm();
    if (distances[i] == 0) distances[i] = MAX_DISTANCE;
    
    Serial.print("Scan ");
    Serial.print(angles[i]);
    Serial.print("°: ");
    Serial.println(distances[i]);
  }

  myServo.write(originalServoPos);
  delay(200);

  int maxIndex = 0;
  for (int i = 1; i < 3; i++) {
    if (distances[i] > distances[maxIndex]) {
      maxIndex = i;
    }
  }

  if (maxIndex == 0) {
    Serial.println("Belok kiri - Jalan terbuka");
    belokKiri();
  } else if (maxIndex == 2) {
    Serial.println("Belok kanan - Jalan terbuka");
    belokKanan();
  } else {
    Serial.println("Lanjut maju - Jalan tengah terbuka");
    maju();
  }

  delay(900);
  stopMotors();
  scanning = false;
  alreadyBacked = false;
}

void fireDetectionMode(int flameLeft, int flameFront, int flameRight) {
  Serial.println("Mode: FIRE DETECTED");
  
  stopMotors();

  if (flameFront < API_DEPAN_DEKAT) {
    Serial.println("API SANGAT DEKAT DI DEPAN!");
    Serial.println("Aktifkan pompa dan servo pemadam");
    
    digitalWrite(PUMP_RELAY_PIN, HIGH);
    
    if (keKanan) {
      myServo.write(SERVO_KANAN);
      Serial.println("Servo ke kanan");
    } else {
      myServo.write(SERVO_KIRI);
      Serial.println("Servo ke kiri");
    }
    keKanan = !keKanan;
    delay(500);
    
    delay(1000); // Waktu penyemprotan
    
  }
  
  else if (flameRight < API_MENDEKATI || flameLeft < API_MENDEKATI) {
    Serial.println("Api terdeteksi di kejauhan - Mendekati sumber api");
    
    digitalWrite(PUMP_RELAY_PIN, LOW);
    
    myServo.write(SERVO_DEPAN);
    
    int diff = abs(flameLeft - flameRight);
    
    if (flameLeft < API_MENDEKATI && flameLeft < flameRight && diff > SENSOR_TOLERANCE) {
      Serial.println("Belok kiri - Api di sebelah kiri");
      belokKiri();
      delay(300);
    } else if (flameRight < API_MENDEKATI && flameRight < flameLeft && diff > SENSOR_TOLERANCE) {
      Serial.println("Belok kanan - Api di sebelah kanan");
      belokKanan();
      delay(300);
    } else {
      Serial.println("Maju - Api di depan atau seimbang");
      maju();
      delay(300);
    }
    
    stopMotors();
  }

  else {
    Serial.println("Api sudah padam - Kembali ke mode wall following");
    
    digitalWrite(PUMP_RELAY_PIN, LOW);
    myServo.write(SERVO_DEPAN);
    
    fireDetected = false;
    fireFighting = false;
    currentMode = WALL_FOLLOWING;
    
    delay(1000); // Tunggu sebentar sebelum lanjut navigasi
  }
}

void maju() {
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, HIGH);
  digitalWrite(MOTOR_IN3, LOW);
  digitalWrite(MOTOR_IN4, HIGH);
  analogWrite(MOTOR_ENA, SPEED + 3); // Koreksi motor kanan
  analogWrite(MOTOR_ENB, SPEED);
}

void mundur() {
  digitalWrite(MOTOR_IN1, HIGH);
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_IN3, HIGH);
  digitalWrite(MOTOR_IN4, LOW);
  analogWrite(MOTOR_ENA, SPEED);
  analogWrite(MOTOR_ENB, SPEED);
}

void belokKiri() {
  digitalWrite(MOTOR_IN1, HIGH);
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_IN3, LOW);
  digitalWrite(MOTOR_IN4, HIGH);
  analogWrite(MOTOR_ENA, SPEED);
  analogWrite(MOTOR_ENB, SPEED);
}

void belokKanan() {
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, HIGH);
  digitalWrite(MOTOR_IN3, HIGH);
  digitalWrite(MOTOR_IN4, LOW);
  analogWrite(MOTOR_ENA, SPEED);
  analogWrite(MOTOR_ENB, SPEED);
}

void stopMotors() {
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_IN3, LOW);
  digitalWrite(MOTOR_IN4, LOW);
  analogWrite(MOTOR_ENA, 0);
  analogWrite(MOTOR_ENB, 0);
}

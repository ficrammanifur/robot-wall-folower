#include <Servo.h>
#include <NewPing.h>

// === Konstanta ===
#define TRIG_PIN 4
#define ECHO_PIN 8
#define MAX_DISTANCE 200
#define WALL_THRESHOLD 25
#define STOP_DISTANCE 23
#define SPEED 100

// === Motor Driver ===
#define MOTOR_ENA 11
#define MOTOR_IN1 12
#define MOTOR_IN2 13
#define MOTOR_ENB 5
#define MOTOR_IN3 6
#define MOTOR_IN4 7

// === Flame Sensor ===
#define FLAME_LEFT_PIN   A3
#define FLAME_FRONT_PIN  A5
#define FLAME_RIGHT_PIN  A4
#define PUMP_RELAY_PIN   A2
#define API_TERPANTAU    1014
#define API_MENDEKATI     950
#define API_DEPAN_DEKAT   890

// === Servo ===
const int SERVO_KANAN = 40;
const int SERVO_KIRI  = 180;
const int SERVO_DEPAN = 110;

Servo myServo;
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

int servoPos = SERVO_DEPAN;
bool scanning = false;
bool alreadyBacked = false;
bool keKanan = true;

// === Function Declarations ===
void maju();
void mundur();
void belokKiri();
void belokKanan();
void stopMotors();

void setup() {
  Serial.begin(9600);

  // Motor Setup
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(MOTOR_IN3, OUTPUT);
  pinMode(MOTOR_IN4, OUTPUT);
  pinMode(MOTOR_ENA, OUTPUT);
  pinMode(MOTOR_ENB, OUTPUT);
  stopMotors();

  // Servo Setup
  myServo.attach(9);
  myServo.write(SERVO_DEPAN);

  // Flame Sensor Setup
  pinMode(FLAME_LEFT_PIN, INPUT);
  pinMode(FLAME_FRONT_PIN, INPUT);
  pinMode(FLAME_RIGHT_PIN, INPUT);
  pinMode(PUMP_RELAY_PIN, OUTPUT);
  digitalWrite(PUMP_RELAY_PIN, LOW);
}

void loop() {
  int flameLeft  = analogRead(FLAME_LEFT_PIN);
  int flameFront = analogRead(FLAME_FRONT_PIN);
  int flameRight = analogRead(FLAME_RIGHT_PIN);

  Serial.print("Left: "); Serial.print(flameLeft);
  Serial.print(" | Front: "); Serial.print(flameFront);
  Serial.print(" | Right: "); Serial.println(flameRight);

  // === API TEPAT DI DEPAN DAN DEKAT ===
  if (flameFront < API_DEPAN_DEKAT) {
    Serial.println("Api tepat di depan! Pemadaman dimulai.");
    stopMotors();
    digitalWrite(PUMP_RELAY_PIN, HIGH);
    if (keKanan) {
      myServo.write(SERVO_KANAN);
    } else {
      myServo.write(SERVO_KIRI);
    }
    keKanan = !keKanan;
    delay(1000);
    return;
  }

  // === API TERDETEKSI DI SEKITAR ===
  if (flameLeft < API_MENDEKATI || flameRight < API_MENDEKATI || flameFront < API_MENDEKATI) {
    Serial.println("🚩 Api terdeteksi di sekitar, mendekati lokasi api...");
    digitalWrite(PUMP_RELAY_PIN, LOW);
    myServo.write(SERVO_DEPAN);

    // Belok ke arah api jika terdeteksi lebih dekat di kanan/kiri
    if (flameRight < flameLeft && flameRight < API_MENDEKATI) {
      Serial.println("➡️ Api lebih dekat di kanan, belok kanan");
      belokKanan(); delay(500); stopMotors();
    } else if (flameLeft < flameRight && flameLeft < API_MENDEKATI) {
      Serial.println("⬅️ Api lebih dekat di kiri, belok kiri");
      belokKiri(); delay(500); stopMotors();
    } else {
      // Api di depan atau hampir seimbang → maju
      Serial.println("⬆️ Api di depan atau seimbang, maju");
      maju();
    }
  } else {
    // === LOGIKA NAVIGASI ===
    if (!scanning) {
      delay(500);
      int frontDistance = sonar.ping_cm();
      Serial.print("Depan: "); Serial.println(frontDistance);

      if (frontDistance > 0 && frontDistance < STOP_DISTANCE && !alreadyBacked) {
        mundur(); delay(400); stopMotors();
        alreadyBacked = true;
        scanning = true;
      } else if (frontDistance > WALL_THRESHOLD || frontDistance == 0) {
        maju();
        alreadyBacked = false;
      } else {
        stopMotors();
        scanning = true;
      }
    }

    if (scanning) {
      int distances[3];
      int angles[3] = {45, 110, 180};

      for (int i = 0; i < 3; i++) {
        myServo.write(angles[i]);
        delay(400);
        distances[i] = sonar.ping_cm();
        if (distances[i] == 0) distances[i] = MAX_DISTANCE;
        Serial.print("Scan "); Serial.print(angles[i]); Serial.print(": "); Serial.println(distances[i]);
      }

      myServo.write(SERVO_DEPAN);
      delay(200);

      int maxIndex = 0;
      for (int i = 1; i < 3; i++) {
        if (distances[i] > distances[maxIndex]) {
          maxIndex = i;
        }
      }

      if (maxIndex == 0) belokKiri();
      else if (maxIndex == 2) belokKanan();
      else maju();

      delay(900);
      stopMotors();
      scanning = false;
      alreadyBacked = false;
    }
  }
}

// === Fungsi Motor ===
void maju() {
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, HIGH);
  digitalWrite(MOTOR_IN3, LOW);
  digitalWrite(MOTOR_IN4, HIGH);
  analogWrite(MOTOR_ENA, SPEED + 3);
  analogWrite(MOTOR_ENB, SPEED);
  Serial.println("MAJU");
}

void mundur() {
  digitalWrite(MOTOR_IN1, HIGH);
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_IN3, HIGH);
  digitalWrite(MOTOR_IN4, LOW);
  analogWrite(MOTOR_ENA, SPEED);
  analogWrite(MOTOR_ENB, SPEED);
  Serial.println("MUNDUR");
}

void belokKiri() {
  digitalWrite(MOTOR_IN1, HIGH);
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_IN3, LOW);
  digitalWrite(MOTOR_IN4, HIGH);
  analogWrite(MOTOR_ENA, SPEED);
  analogWrite(MOTOR_ENB, SPEED);
  Serial.println("BELOK KIRI");
}

void belokKanan() {
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, HIGH);
  digitalWrite(MOTOR_IN3, HIGH);
  digitalWrite(MOTOR_IN4, LOW);
  analogWrite(MOTOR_ENA, SPEED);
  analogWrite(MOTOR_ENB, SPEED);
  Serial.println("BELOK KANAN");
}

void stopMotors() {
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_IN3, LOW);
  digitalWrite(MOTOR_IN4, LOW);
  analogWrite(MOTOR_ENA, 0);
  analogWrite(MOTOR_ENB, 0);
  Serial.println("STOP");
}

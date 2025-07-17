#include <Servo.h>
#include <NewPing.h>

// === Konstanta ===
#define TRIG_PIN 4
#define ECHO_PIN 8
#define MAX_DISTANCE 200
#define WALL_THRESHOLD 30
#define STOP_DISTANCE 27
#define SPEED 100

// === Motor Driver ===
#define MOTOR_ENA 11
#define MOTOR_IN1 12
#define MOTOR_IN2 13
#define MOTOR_ENB 5
#define MOTOR_IN3 6
#define MOTOR_IN4 7

// === Servo & Sensor ===
Servo myServo;
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
int servoPos = 110;
int originalServoPos = 110;

bool scanning = false;
bool alreadyBacked = false;

void setup() {
  Serial.begin(9600);

  // Motor
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(MOTOR_IN3, OUTPUT);
  pinMode(MOTOR_IN4, OUTPUT);
  pinMode(MOTOR_ENA, OUTPUT);
  pinMode(MOTOR_ENB, OUTPUT);
  stopMotors();

  // Servo
  myServo.attach(9);
  myServo.write(originalServoPos);
  delay(500);
}

void loop() {
  if (!scanning) {
    delay(500);
    int frontDistance = sonar.ping_cm();
    Serial.print("Depan: ");
    Serial.println(frontDistance);

    if (frontDistance > 0 && frontDistance < STOP_DISTANCE && !alreadyBacked) {
      // Mundur sekali jika terlalu dekat
      mundur();
      delay(400); // Durasi mundur lebih pendek
      stopMotors();
      alreadyBacked = true;
      scanning = true;
    } else if (frontDistance > WALL_THRESHOLD || frontDistance == 0) {
      maju();
      alreadyBacked = false;
    } else {
      // Jika ada tembok tapi tidak terlalu dekat
      stopMotors();
      scanning = true;
    }
  }

  if (scanning) {
    int distances[3]; // Kiri, Tengah, Kanan
    int angles[3] = {45, 110, 180};

    for (int i = 0; i < 3; i++) {
      myServo.write(angles[i]);
      delay(400); // Waktu tunggu stabil
      distances[i] = sonar.ping_cm();
      if (distances[i] == 0) distances[i] = MAX_DISTANCE; // Jika tidak terbaca
      Serial.print("Scan ");
      Serial.print(angles[i]);
      Serial.print(": ");
      Serial.println(distances[i]);
    }

    // Kembali ke posisi awal
    myServo.write(originalServoPos);
    delay(200);

    // Tentukan arah terbaik
    int maxIndex = 0;
    for (int i = 1; i < 3; i++) {
      if (distances[i] > distances[maxIndex]) {
        maxIndex = i;
      }
    }

    if (maxIndex == 0) {
      belokKiri();
    } else if (maxIndex == 2) {
      belokKanan();
    } else {
      // Tengah, maju lagi
      maju();
    }

    delay(900); // Waktu belok diperpanjang
    stopMotors();
    scanning = false;
    alreadyBacked = false;
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

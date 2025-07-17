#include <Servo.h>

// === Motor Driver Pins ===
#define SPEED 100
#define MOTOR_ENA 11  // Motor kanan (PWM)
#define MOTOR_IN1 12  // Motor kanan
#define MOTOR_IN2 13
#define MOTOR_ENB 5   // Motor kiri (PWM)
#define MOTOR_IN3 6   // Motor kiri
#define MOTOR_IN4 7

// === Flame Sensor Pins ===
#define FLAME_LEFT_PIN   A4
#define FLAME_FRONT_PIN  A5
#define FLAME_RIGHT_PIN  A3

// === Relay Pompa ===
#define PUMP_RELAY_PIN   A2 // Gunakan NO relay, aktif saat LOW (active-LOW relay)

// === Thresholds ===
#define API_TERPANTAU      1014  // Tidak ada api jika semua > ini
#define API_MENDEKATI       950  // Ada api di kejauhan jika < 950
#define API_DEPAN_DEKAT     744  // Api sangat dekat jika < 890 (pas di depan)
#define SENSOR_TOLERANCE    30   // Toleransi perbedaan sensor kiri-kanan untuk lurus

// === Servo Position ===
const int SERVO_KANAN = 40;
const int SERVO_KIRI  = 180;
const int SERVO_DEPAN = 110;

Servo myServo;
bool keKanan = true;

void setup() {
  Serial.begin(9600);

  // Setup motor pins
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(MOTOR_IN3, OUTPUT);
  pinMode(MOTOR_IN4, OUTPUT);
  pinMode(MOTOR_ENA, OUTPUT);
  pinMode(MOTOR_ENB, OUTPUT);

  // Setup flame sensor and pump pins
  pinMode(FLAME_LEFT_PIN, INPUT);
  pinMode(FLAME_FRONT_PIN, INPUT);
  pinMode(FLAME_RIGHT_PIN, INPUT);
  pinMode(PUMP_RELAY_PIN, OUTPUT);

  // Initialize servo
  myServo.attach(9);
  myServo.write(SERVO_DEPAN);

  // Initial state
  stopMotors();
  digitalWrite(PUMP_RELAY_PIN, LOW); // Relay off (active-LOW)

  Serial.println("Kontrol Robot Pemadam Api Otomatis");
  Serial.println("-----------------------------");
}

void loop() {
  int flameLeft  = analogRead(FLAME_LEFT_PIN);
  int flameFront = analogRead(FLAME_FRONT_PIN);
  int flameRight = analogRead(FLAME_RIGHT_PIN);

  // Print sensor values for debugging
  Serial.print("Left (A3): ");
  Serial.print(flameLeft);
  Serial.print(" | Front (A5): ");
  Serial.print(flameFront);
  Serial.print(" | Right (A4): ");
  Serial.print(flameRight);
  Serial.print(" | Diff (L-R): ");
  Serial.println(abs(flameLeft - flameRight));

  // === Tidak ada api sama sekali ===
  if (flameLeft > API_TERPANTAU && flameFront > API_TERPANTAU && flameRight > API_TERPANTAU) {
    Serial.println("✅ Tidak ada api terdeteksi → Servo diam, Pompa mati, Robot berhenti");
    myServo.write(SERVO_DEPAN);
    digitalWrite(PUMP_RELAY_PIN, LOW); // Relay off (active-LOW)
    stopMotors();
  }

  // === Api sangat dekat dan tepat di depan ===
  else if (flameFront < API_DEPAN_DEKAT) {
    Serial.println("🔥 Api tepat di depan dan dekat! → Pompa aktif, servo gerak kiri-kanan, Robot berhenti");
    digitalWrite(PUMP_RELAY_PIN, HIGH); // Relay on (active-LOW)
    stopMotors();

    if (keKanan) {
      myServo.write(SERVO_KANAN);
    } else {
      myServo.write(SERVO_KIRI);
    }
    keKanan = !keKanan;
    delay(300); // Ayun kiri-kanan
  }

  // === Api terdeteksi di kejauhan ===
  else if (flameRight < API_MENDEKATI || flameLeft < API_MENDEKATI) {
    Serial.println("🚩 Api mulai terdeteksi di kejauhan → Bergerak mendekat");
    digitalWrite(PUMP_RELAY_PIN, LOW); // Relay off (active-LOW)
    myServo.write(SERVO_DEPAN); // Servo kembali ke depan

    // Logika arah belok
    int diff = abs(flameLeft - flameRight);
    if (flameLeft < API_MENDEKATI && flameLeft < flameRight && diff > SENSOR_TOLERANCE) {
      Serial.println("⬅️ Belok ke kiri karena api di kiri lebih dekat");
      belokKiri();
    } else if (flameRight < API_MENDEKATI && flameRight < flameLeft && diff > SENSOR_TOLERANCE) {
      Serial.println("➡️ Belok ke kanan karena api di kanan lebih dekat");
      belokKanan();
    } else {
      Serial.println("⬆️ Lanjut lurus, api terdeteksi di depan atau seimbang");
      maju();
    }
  }

  Serial.println("-------------------------------");
  delay(300);
}

// === Fungsi Motor ===
void maju() {
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, HIGH);
  digitalWrite(MOTOR_IN3, LOW);
  digitalWrite(MOTOR_IN4, HIGH);
  analogWrite(MOTOR_ENA, SPEED + 3); // Koreksi motor kanan
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

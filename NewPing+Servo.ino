#include <Servo.h>
#include <NewPing.h>

// === Servo & Sensor ===
#define TRIG_PIN 4
#define ECHO_PIN 8
#define SERVO_PIN 9

#define MAX_DISTANCE 200
#define WALL_THRESHOLD 15
#define STOP_DISTANCE 13

Servo myServo;
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

const int SERVO_KANAN = 40;
const int SERVO_KIRI = 180;
const int SERVO_DEPAN = 110;

// === Fungsi baca jarak ===
int getDistance() {
  delay(50);
  int d = sonar.ping_cm();
  return (d == 0) ? MAX_DISTANCE : d;
}

// === Fungsi gerak servo smooth ===
void scanTo(int target) {
  int current = myServo.read();
  int step = (target > current) ? 1 : -1;
  for (int pos = current; pos != target; pos += step) {
    myServo.write(pos);
    delay(5); // semakin kecil semakin smooth
  }
  myServo.write(target); // pastikan posisi akhir
  delay(300);
}

void setup() {
  Serial.begin(9600);
  myServo.attach(SERVO_PIN);
  myServo.write(SERVO_DEPAN);
  delay(500);
}

void loop() {
  // === Scan Depan ===
  scanTo(SERVO_DEPAN);
  int jarakDepan = getDistance();
  Serial.print("Depan: ");
  Serial.print(jarakDepan);
  Serial.println(" cm");

  if (jarakDepan > WALL_THRESHOLD) {
    Serial.println("➡️ Aman, maju ke depan");
    // logika motor maju
    delay(1000);
  }
  else if (jarakDepan <= STOP_DISTANCE) {
    Serial.println("⛔ Terlalu dekat, mundur!");
    // logika motor mundur
    delay(1000);
  }
  else {
    // === Scan kanan ===
    scanTo(SERVO_KANAN);
    int jarakKanan = getDistance();

    // === Scan kiri ===
    scanTo(SERVO_KIRI);
    int jarakKiri = getDistance();

    // === Kembali ke depan ===
    scanTo(SERVO_DEPAN);

    // === Log hasil ===
    Serial.print("Kanan: ");
    Serial.print(jarakKanan);
    Serial.print(" cm | Kiri: ");
    Serial.print(jarakKiri);
    Serial.println(" cm");

    // === Tentukan arah terbaik ===
    if (jarakKiri > jarakKanan) {
      Serial.println("↩️ Belok kiri (lebih aman)");
      // logika belok kiri
    } else {
      Serial.println("↪️ Belok kanan (lebih aman)");
      // logika belok kanan
    }

    delay(1000);
  }
}

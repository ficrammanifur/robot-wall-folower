#include <Servo.h>
#include <NewPing.h>

// === Konstanta & Pin ===
#define TRIG_PIN 4
#define ECHO_PIN 8
#define SERVO_PIN 9

#define MAX_DISTANCE 200     // cm
#define WALL_THRESHOLD 20    // Batas aman
#define STOP_DISTANCE 17     // Terlalu dekat, mundur

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
Servo myServo;

// Fungsi baca jarak
int getDistance() {
  delay(50); // delay sensor ultrasonic
  int d = sonar.ping_cm();
  return (d == 0) ? MAX_DISTANCE : d;  // anggap 0 sebagai MAX (tidak terdeteksi)
}

void setup() {
  Serial.begin(9600);
  myServo.attach(SERVO_PIN);

  // Servo ke tengah (depan)
  myServo.write(90);
  delay(500);
}

void loop() {
  // === Scan Depan ===
  myServo.write(90);
  delay(300);
  int jarakDepan = getDistance();
  Serial.print("Depan: ");
  Serial.print(jarakDepan);
  Serial.println(" cm");

  if (jarakDepan > WALL_THRESHOLD) {
    Serial.println("➡️ Aman, maju ke depan");
    // logika motor jalan ke depan di sini
    delay(1000);
  } 
  else if (jarakDepan <= STOP_DISTANCE) {
    Serial.println("⛔ Terlalu dekat, mundur!");
    // logika motor mundur di sini
    delay(1000);
  } 
  else {
    // === Scan kiri dan kanan ===
    myServo.write(0); // kanan
    delay(300);
    int jarakKanan = getDistance();
    
    myServo.write(180); // kiri
    delay(300);
    int jarakKiri = getDistance();

    // Tampilkan hasil
    Serial.print("Kanan: ");
    Serial.print(jarakKanan);
    Serial.print(" cm | Kiri: ");
    Serial.print(jarakKiri);
    Serial.println(" cm");

    // Tentukan arah paling aman
    if (jarakKiri > jarakKanan) {
      Serial.println("↩️ Belok kiri (lebih aman)");
      // logika belok kiri di sini
    } else {
      Serial.println("↪️ Belok kanan (lebih aman)");
      // logika belok kanan di sini
    }
    delay(1000);
  }
}

#include <Servo.h>
#include <NewPing.h>

// === Konfigurasi ===
#define TRIG_PIN 4
#define ECHO_PIN 8
#define MAX_DISTANCE 200  // cm, batas jarak maksimum

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
Servo myServo;

void setup() {
  Serial.begin(9600);
  myServo.attach(9);  // Servo pada pin 9
  myServo.write(90);  // Posisi tengah
  delay(500);
}

void loop() {
  // Scan dari kiri ke kanan
  for (int angle = 0; angle <= 180; angle += 10) {
    myServo.write(angle);
    delay(200);  // Biarkan servo stabil

    int distance = sonar.ping_cm();
    Serial.print("Angle: ");
    Serial.print(angle);
    Serial.print("° | Jarak: ");
    if (distance > 0) {
      Serial.print(distance);
      Serial.println(" cm");
    } else {
      Serial.println("Tidak terdeteksi");
    }
  }

  // Scan balik dari kanan ke kiri
  for (int angle = 180; angle >= 0; angle -= 10) {
    myServo.write(angle);
    delay(200);

    int distance = sonar.ping_cm();
    Serial.print("Angle: ");
    Serial.print(angle);
    Serial.print("° | Jarak: ");
    if (distance > 0) {
      Serial.print(distance);
      Serial.println(" cm");
    } else {
      Serial.println("Tidak terdeteksi");
    }
  }

  delay(500);
}

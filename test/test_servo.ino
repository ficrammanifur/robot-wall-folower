#include <Servo.h>

Servo myServo;

void setup() {
  myServo.attach(9);
  Serial.begin(9600);
}

void loop() {
  // Putar ke kanan
  myServo.write(0); 
  Serial.println("kanan");
  delay(2000);

  // Stop
  myServo.write(110);
  Serial.println("Depan");
  delay(2000);

  // Putar ke kiri
  myServo.write(180); 
  Serial.println("kiri");
  delay(2000);

  // Stop
  myServo.write(110);
  Serial.println("Depan");
  delay(2000);
}

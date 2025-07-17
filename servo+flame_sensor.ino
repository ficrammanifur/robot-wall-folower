#include <Servo.h>

// === PIN Flame Sensor ===
#define FLAME_LEFT_PIN   A3
#define FLAME_FRONT_PIN  A5
#define FLAME_RIGHT_PIN  A4

// === Thresholds ===
#define API_TERPANTAU     1000  // Api terdeteksi jika nilai < 1000
#define API_DEPAN_DEKAT    890  // Api sangat dekat jika < 800

Servo myServo;

void setup() {
  Serial.begin(9600);

  pinMode(FLAME_LEFT_PIN, INPUT);
  pinMode(FLAME_FRONT_PIN, INPUT);
  pinMode(FLAME_RIGHT_PIN, INPUT);

  myServo.attach(9); // Servo pada pin 9
  myServo.write(110); // Posisi diam di awal
}

void loop() {
  int flameLeft  = analogRead(FLAME_LEFT_PIN);
  int flameFront = analogRead(FLAME_FRONT_PIN);
  int flameRight = analogRead(FLAME_RIGHT_PIN);

  Serial.print("Left: ");
  Serial.print(flameLeft);
  Serial.print(" | Front: ");
  Serial.print(flameFront);
  Serial.print(" | Right: ");
  Serial.println(flameRight);

  // === Tidak ada api ===
  if (flameLeft > API_TERPANTAU && flameFront > API_TERPANTAU && flameRight > API_TERPANTAU) {
    Serial.println("✅ Tidak ada api terdeteksi → Servo diam");
    myServo.write(110); // Servo diam
  }

  // === Api tepat di depan (sangat dekat) ===
  else if (flameFront < API_DEPAN_DEKAT) {
    Serial.println("🔥🔥 API TEPAT DI DEPAN! → Servo berhenti");
    myServo.write(110); // Servo stop
  }

  // === Api di kiri atau kanan → Servo mengikuti arah
  else {
    if (flameLeft < flameRight && flameLeft < API_TERPANTAU) {
      Serial.println("➡️ Api terdeteksi di KIRI → Servo ke kiri");
      myServo.write(180); // Servo putar kiri
    }
    else if (flameRight < flameLeft && flameRight < API_TERPANTAU) {
      Serial.println("➡️ Api terdeteksi di KANAN → Servo ke kanan");
      myServo.write(45); // Servo putar kanan
    }
    else {
      // Jika nilainya mirip atau api samar
      Serial.println("⚠️ Api samar atau setara kiri/kanan → Servo berhenti");
      myServo.write(110);
    }
  }

  Serial.println("-------------------------------");
  delay(200); // Delay kecil agar lebih responsif
}

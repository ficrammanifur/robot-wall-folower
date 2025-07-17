#include <Servo.h>

// === PIN Flame Sensor ===
#define FLAME_LEFT_PIN   A3
#define FLAME_FRONT_PIN  A5
#define FLAME_RIGHT_PIN  A4

// === Relay Pompa ===
#define PUMP_RELAY_PIN   A2 // Gunakan NO relay, aktif saat HIGH

// === Thresholds ===
#define API_TERPANTAU     1000  // Api terdeteksi jika nilai < 1000
#define API_DEPAN_DEKAT    890  // Api sangat dekat jika < 890 (pas di depan)

Servo myServo;

void setup() {
  Serial.begin(9600);

  pinMode(FLAME_LEFT_PIN, INPUT);
  pinMode(FLAME_FRONT_PIN, INPUT);
  pinMode(FLAME_RIGHT_PIN, INPUT);
  pinMode(PUMP_RELAY_PIN, OUTPUT);

  digitalWrite(PUMP_RELAY_PIN, LOW); // Pompa mati di awal (karena NO relay)
  myServo.attach(9); // Servo pada pin 9
  myServo.write(110); // Posisi netral di awal
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

  // === Tidak ada api sama sekali ===
  if (flameLeft > API_TERPANTAU && flameFront > API_TERPANTAU && flameRight > API_TERPANTAU) {
    Serial.println("✅ Tidak ada api terdeteksi → Servo diam, Pompa mati");
    myServo.write(110);
    digitalWrite(PUMP_RELAY_PIN, LOW); // Pompa OFF
  }

  // === Api tepat di depan dan sangat dekat ===
  else if (flameFront < API_DEPAN_DEKAT) {
    Serial.println("🔥 API DI DEPAN! → Pompa AKTIF");
    myServo.write(110); // Servo diam
    digitalWrite(PUMP_RELAY_PIN, HIGH); // Pompa ON
  }

  // === Api belum tepat di depan → arahkan servo ke kiri atau kanan ===
  else {
    digitalWrite(PUMP_RELAY_PIN, LOW); // Pastikan pompa OFF sampai api tepat di depan

    if (flameLeft < flameRight && flameLeft < API_TERPANTAU) {
      Serial.println("↪️ Api terdeteksi di KIRI → Arahkan servo ke kiri untuk mendekati api");
      myServo.write(180);
    }
    else if (flameRight < flameLeft && flameRight < API_TERPANTAU) {
      Serial.println("↩️ Api terdeteksi di KANAN → Arahkan servo ke kanan untuk mendekati api");
      myServo.write(45);
    }
    else {
      // Api samar, atau kiri kanan setara
      Serial.println("⚠️ Api samar atau setara kiri-kanan → Servo netral");
      myServo.write(110);
    }
  }

  Serial.println("-------------------------------");
  delay(200);
}

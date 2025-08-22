// === PIN Flame Sensor ===
#define FLAME_LEFT_PIN   A3
#define FLAME_FRONT_PIN  A5
#define FLAME_RIGHT_PIN  A4

// Ambang batas api
#define API_JAUH_THRESHOLD     1000
#define API_TEPAT_DEPAN        890

void setup() {
  Serial.begin(9600);

  pinMode(FLAME_LEFT_PIN, INPUT);
  pinMode(FLAME_FRONT_PIN, INPUT);
  pinMode(FLAME_RIGHT_PIN, INPUT);
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

  // === 1. Tidak ada api ===
  if (flameLeft > API_JAUH_THRESHOLD && flameFront > API_JAUH_THRESHOLD && flameRight > API_JAUH_THRESHOLD) {
    Serial.println("✅ Tidak ada api terdeteksi");
  }

  // === 2. Api tepat di depan (sangat dekat) ===
  else if (flameFront < API_TEPAT_DEPAN) {
    Serial.println("🔥🔥 API TEPAT DI DEPAN! Sangat dekat!");
  }

  // === 3. Api di depan tapi masih agak jauh ===
  else if (flameFront < API_JAUH_THRESHOLD) {
    Serial.println("🔥 Api terdeteksi di depan (masih agak jauh)");
  }

  // === 4. Api hanya terdeteksi di kiri/kanan ===
  else if (flameLeft < API_JAUH_THRESHOLD || flameRight < API_JAUH_THRESHOLD) {
    if (flameLeft < flameRight) {
      Serial.println("➡️ Api terdeteksi di KIRI");
    } else if (flameRight < flameLeft) {
      Serial.println("➡️ Api terdeteksi di KANAN");
    } else {
      Serial.println("⚠️ Api terdeteksi di KIRI dan KANAN (sama kuat)");
    }
  }

  Serial.println("-------------------------------");
  delay(500);
}

#include <Servo.h>

// === PIN Flame Sensor ===
#define FLAME_LEFT_PIN   A3
#define FLAME_FRONT_PIN  A5
#define FLAME_RIGHT_PIN  A4

// === Relay Pompa ===
#define PUMP_RELAY_PIN   A2 // Gunakan NO relay, aktif saat HIGH

// === Thresholds ===
#define API_TERPANTAU      1014  // Tidak ada api jika semua > ini
#define API_MENDEKATI       950  // Ada api di kejauhan jika < 950
#define API_DEPAN_DEKAT     890  // Api sangat dekat jika < 890 (pas di depan)

// === Servo Position ===
const int SERVO_KANAN = 40;
const int SERVO_KIRI  = 180;
const int SERVO_DEPAN = 110;

Servo myServo;
bool keKanan = true;

void setup() {
  Serial.begin(9600);

  pinMode(FLAME_LEFT_PIN, INPUT);
  pinMode(FLAME_FRONT_PIN, INPUT);
  pinMode(FLAME_RIGHT_PIN, INPUT);
  pinMode(PUMP_RELAY_PIN, OUTPUT);

  digitalWrite(PUMP_RELAY_PIN, LOW); // Pompa mati di awal
  myServo.attach(9);
  myServo.write(SERVO_DEPAN);
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
    myServo.write(SERVO_DEPAN);
    digitalWrite(PUMP_RELAY_PIN, LOW);
  }

  // === Api tepat di depan dan sangat dekat ===
  else if (flameFront < API_DEPAN_DEKAT) {
    Serial.println("🔥 Api tepat di depan dan dekat! → Pompa aktif, servo gerak kiri-kanan");

    digitalWrite(PUMP_RELAY_PIN, HIGH);

    // Gerak kiri-kanan bergantian
    if (keKanan) {
      myServo.write(SERVO_KANAN);
    } else {
      myServo.write(SERVO_KIRI);
    }
    keKanan = !keKanan;
    delay(300); // delay pendek untuk ayunan
  }

  // === Api mulai terdeteksi di kanan atau kiri, tapi belum tepat di depan ===
  else {
    Serial.println("🚩 Api mulai terdeteksi → Mendekati api");
    digitalWrite(PUMP_RELAY_PIN, LOW);
    myServo.write(SERVO_DEPAN); // Tetap hadap depan
  }

  Serial.println("-------------------------------");
  delay(300);
}

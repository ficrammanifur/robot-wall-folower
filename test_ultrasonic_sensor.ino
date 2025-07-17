#define TRIG_PIN 4
#define ECHO_PIN 8

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  // Kirim sinyal trigger
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Baca durasi pantulan
  long duration = pulseIn(ECHO_PIN, HIGH);

  // Konversi ke jarak dalam cm
  float distance = duration * 0.034 / 2;

  // Tampilkan hasil di Serial Monitor
  Serial.print("Jarak: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(500);
}

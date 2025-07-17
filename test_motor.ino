#define SPEED 100

//right motor pins
const int in1R = 12;
const int in2R = 13;
const int enR = 11;

//left motor pins
const int in1L = 6;
const int in2L = 7;
const int enL = 5;

void setup() {
  Serial.begin(9600);
  pinMode(in1L, OUTPUT);
  pinMode(in2L, OUTPUT);
  pinMode(in1R, OUTPUT);
  pinMode(in2R, OUTPUT);
  pinMode(enL, OUTPUT);
  pinMode(enR, OUTPUT);
  stopMotors();
  
  Serial.println("Kontrol Robot via Serial Monitor");
  Serial.println("Perintah:");
  Serial.println("w = Maju\ta = Kiri");
  Serial.println("s = Mundur\td = Kanan");
  Serial.println("x = Stop");
  Serial.println("-----------------------------");
}

void loop() {
  if (Serial.available()) {
    char cmd = Serial.read();
    switch (cmd) {
      case 'w': maju(); break;
      case 's': mundur(); break;
      case 'a': belokKiri(); break;
      case 'd': belokKanan(); break;
      case 'x': stopMotors(); break;
      default: Serial.println("Perintah tidak valid! Gunakan w,a,s,d,x");
    }
  }
}

void maju() {
  digitalWrite(in1L, LOW);
  digitalWrite(in2L, HIGH);
  digitalWrite(in1R, LOW);
  digitalWrite(in2R, HIGH);
  analogWrite(enL, SPEED+3); // Motor kiri sedikit lebih cepat untuk koreksi
  analogWrite(enR, SPEED);
  Serial.println("MAJU");
}

void mundur() {
  digitalWrite(in1L, HIGH);
  digitalWrite(in2L, LOW);
  digitalWrite(in1R, HIGH);
  digitalWrite(in2R, LOW);
  analogWrite(enL, SPEED+3);
  analogWrite(enR, SPEED);
  Serial.println("MUNDUR");
}

void belokKiri() {
  // Motor kiri mundur, motor kanan maju
  digitalWrite(in1L, HIGH);
  digitalWrite(in2L, LOW);
  digitalWrite(in1R, LOW);
  digitalWrite(in2R, HIGH);
  analogWrite(enL, SPEED);
  analogWrite(enR, SPEED);
  Serial.println("BELOK KIRI");
}

void belokKanan() {
  // Motor kiri maju, motor kanan mundur
  digitalWrite(in1L, LOW);
  digitalWrite(in2L, HIGH);
  digitalWrite(in1R, HIGH);
  digitalWrite(in2R, LOW);
  analogWrite(enL, SPEED);
  analogWrite(enR, SPEED);
  Serial.println("BELOK KANAN");
}

void stopMotors() {
  digitalWrite(in1L, LOW);
  digitalWrite(in2L, LOW);
  digitalWrite(in1R, LOW);
  digitalWrite(in2R, LOW);
  analogWrite(enL, 0);
  analogWrite(enR, 0);
  Serial.println("STOP");
}
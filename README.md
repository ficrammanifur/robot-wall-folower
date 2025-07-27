<h1 align="center">🤖 Robot Wall Follower and Firefighter</h1>
<p align="center"><em>Autonomous Navigation Meets Smart Fire Detection</em></p>

<p align="center">
  <img src="https://img.shields.io/badge/last%20commit-today-brightgreen" />
  <img src="https://img.shields.io/badge/arduino-100%25-blue" />
  <img src="https://img.shields.io/badge/components-robotics-lightgrey" />
</p>

---

This repository contains code and resources for an autonomous robot that:

- 🔁 Menavigasi sepanjang dinding (wall-following)  
- 🔥 Mendeteksi dan memadamkan api (firefighting)  

Dikendalikan oleh mikrokontroler Arduino, robot ini menggunakan sensor ultrasonik untuk wall-following dan sensor api untuk tugas firefighting. Cocok untuk keperluan edukasi, hobi, maupun kosumen robotika.

---

## 📋 Table of Contents

1. [Project Overview](#project-overview)  
2. [Features](#features)  
3. [Hardware Requirements](#hardware-requirements)  
4. [Software Requirements](#software-requirements)  
5. [Installation](#installation)  
6. [Usage](#usage)  
7. [File Structure](#file-structure)  
8. [Contributing](#contributing)  
9. [License](#license)  
10. [Contact](#contact)  

---

## 🧠 Project Overview

Proyek ini mengimplementasikan robot wall-follower dengan fitur firefighting. Robot:

- Menavigasi dengan mengikuti dinding menggunakan sensor ultrasonik dan servo untuk scanning.
- Mendeteksi api menggunakan 3 sensor flame (kiri, depan, kanan).
- Memadamkan api memakai pompa yang dikendalikan relay (akan diintegrasikan di kode final).  
Repo ini berbasis dari `ficrammanifur/robot-wall-folower`, mencakup kode uji dan fitur dasar—namun **kode integrasi firefighter final masih dalam pengembangan**.

---

## ✅ Features

- **Wall Following**: Sensor ultrasonik + servo untuk navigasi otomatis di lingkungan mirip maze.  
- **Fire Detection**: Mendeteksi api dengan 3 flame sensor (kiri, depan, kanan).  
- **Firefighting**: Menyalakan relay-controlled pump saat api terdeteksi (implementasi sebagian tersedia).  
- **Motor Control**: Menggerakkan 2 motor DC untuk arah maju, mundur, belok kiri/kanan.  
- **Modular Testing**: Kode tes untuk masing‑masing komponen (sensor, servo, motor).  
- **Arduino-Based**: Platform Arduino yang mudah diakses untuk semua level pengguna.

---

## 🧰 Hardware Requirements

- Arduino Uno (atau kompatibel)  
- Ultrasonic sensor (HC-SR04)  
- Servo motor (SG90) untuk scanning sensor  
- 3 flame sensors  
- 2 DC motors + motor driver (L298N)  
- Modul relay untuk kontrol pompa  
- Chassis robot with wheels + caster wheel  
- Power supply (battery pack, misal 12 V)  
- Kabel jumper, breadboard / PCB  
- ❗ Opsional: Water pump atau fan (untuk firefighting)

---

## 💻 Software Requirements

- Arduino IDE (disarankan versi terbaru)  
- Library Arduino:  
  - `NewPing` (ultrasonic)  
  - `Servo` (servo control)  
  - Plus library tambahan sesuai hardware  

Instalasi library:  
`Sketch > Include Library > Manage Libraries` → cari dan instal `NewPing`, `Servo`, dll.

---

## ⚙️ Installation

1. **Clone repo**  
    ```bash
    git clone https://github.com/ficrammanifur/robot-wall-folower.git
    ```

2. **Install Arduino IDE**  
   Unduh dari [arduino.cc](https://www.arduino.cc/en/software).

3. **Instal library** seperti dijelaskan di atas.

4. **Upload code**  
   - Buka file `.ino` yang diinginkan dengan Arduino IDE  
   - Sambungkan Arduino lewat USB  
   - Pilih **Tools > Board** dan **Port** yang sesuai  
   - Klik **Upload**

---

## 🚀 Usage

### 1. Assemble Hardware  
Ikuti wiring sesuai `Wiring Diagram.png`. Pastikan semua koneksi rapi dan power cukup.

### 2. Wall Following Mode  
Upload `servo_ultrasonic_wallscanner.ino` untuk navigasi otomatis mengikuti dinding.  
Sesuaikan nilai `WALL_THRESHOLD` dan `STOP_DISTANCE`.

### 3. Fire Detection & Firefighting  
- `test_flame_sensor.ino`: uji pembacaan sensor api  
- `servo+flame_sensor.ino`: arahkan servo dan uji flame detection  
- `servo+relay+flame.ino`: preliminary firefighting dengan kontrol relay

### 4. Testing Components  
Gunakan file test berikut untuk verifikasi masing‑masing modul:  
- `test_servo.ino`, `test_motor.ino`, `test_ultrasonic_sensor.ino`, dll.

### 5. Debugging  
Gunakan **Serial Monitor** (9600 baud) untuk membaca data sensor dan status motor.  
**Catatan:** Kode integrasi final belum tersedia. Pantau pembaruan di repo ini.

---

## 🗂️ File Structure
```
robot-wall-folower/
├── NewPing+Servo.ino
├── Scan_Jarak_Servo+Ultrasonic.ino
├── servo+flame_sensor.ino
├── servo+relay+flame.ino
├── servo_ultrasonic_wallscanner.ino
├── test_flame_sensor.ino
├── test_motor.ino
├── test_servo.ino
├── test_ultrasonic_sensor.ino
├── Wiring Diagram.png
└── README.md ← This file
```

---

## 🤝 Contributing

Kontribusi sangat welcome, terutama untuk pengembangan kode firefighter final:

1. Fork repo ini  
2. Buat branch:  
   ```bash
   git checkout -b feature/your-feature

3. Commit perubahan:
```
git commit -m "Add feature..."
```
Push branch kamu dan buat Pull Request

Sertakan dokumentasi & komentar kode jelas

## 📝 License
<p align="center">
  <a href="https://github.com/ficrammanifur/ficrammanifur/blob/main/LICENSE">
    <img src="https://img.shields.io/badge/license-MIT-blue" alt="License: MIT" />
  </a>
</p>

## 📞 Contact

Have questions, feedback, or ideas?  
Open an [issue](https://github.com/ficrammanifur/robot-wall-folower/issues) or reach out via the main GitHub profile: [ficrammanifur](https://github.com/ficrammanifur)

> ℹ️ This README will be updated as the final firefighting integration is completed.


Buka Issue di GitHub

Atau hubungi direktori repo ficrammanifur

Catatan: README ini akan diupdate lagi setelah kode firefighting final terintegrasi.

<p align="left"><a href="#robot-wall-folower">↑ RETURN</a></p>

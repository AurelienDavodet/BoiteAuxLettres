# 📬 Connected Letter Box Project

This project is a **smart connected letterbox** built on an ESP microcontroller platform. The letterbox is capable of **detecting new mail**, allowing **remote access** for opening and closing, setting **custom opening/closing schedules**, and displaying custom messages like "No ads" on a built-in screen. The project is designed for convenience, enhancing traditional mailboxes with smart features!

## ✨ Features

- **Mail Detection**: Receive instant alerts when a new letter arrives in your mailbox.
- **Remote Access**: Open or close your mailbox from anywhere using a web app or mobile app.
- **Scheduling**: Program specific times for the letterbox to unlock or lock automatically.
- **Custom Messages**: Display messages on an integrated screen to let the postman or passersby know if you'd like no ads, or any other message you set.
- **Low Power Mode**: Optimized to preserve battery life when idle.

## 🛠️ Hardware Requirements

- **ESP32 or ESP8266** microcontroller
- **PIR or IR Sensor** (for mail detection)
- **Solenoid or Servo Motor** (for locking/unlocking the letterbox)
- **RTC Module** (Real-Time Clock for scheduling)
- **OLED or LCD Display** (for displaying custom messages)
- **Battery Pack** (to power the system in a remote environment)

## 🚀 Getting Started

### 1. Wiring the Components

Connect the components according to the schematic:

- **Sensor**: Connect the sensor to the ESP GPIO pins (e.g., D1 for PIR output)
- **Motor**: Connect the solenoid or servo motor to the ESP (e.g., D2 for control)
- **Display**: Connect the OLED or LCD display to the ESP (e.g., SDA to D3, SCL to D4)
- **RTC**: Connect the Real-Time Clock (e.g., SDA to D5, SCL to D6)
- **Power**: Connect the battery pack to the ESP for standalone operation.

### 2. Flashing the Firmware

1. Clone this repository:

   ```bash
   git clone https://github.com/yourusername/connected_letterbox.git
   cd connected_letterbox

### 3. Tutorial

A tutorial is available here: [Click here for the tutorial](https://www.linkedin.com/pulse/bo%C3%AEte-aux-lettres-connect%C3%A9e-aur%C3%A9lien-davodet/?trackingId=Y6K89BuVysOGowvL%2FyqE7g%3D%3D)

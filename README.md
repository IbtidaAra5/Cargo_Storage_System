# Cargo-Storage-System
Smart Cargo Storage System: An Automated Approach to 
Airport Safety and Security

Smart Cargo Storage System (SCSS) is an automated solution for airport cargo management that combines robotics, IoT sensors, AI-based screening, and real-time tracking. It enhances safety by detecting hazardous items, improves security through access control and surveillance, and increases efficiency by automating storage, retrieval, and monitoring of cargo. SCSS reduces human error, ensures regulatory compliance, and enables data-driven logistics decisions.
Smart Cargo Storage System



Features:

RFID Access Control — authenticates vehicles/personnel via MFRC522 reader
Entry Tracking — ultrasonic sensor counts entries, shown on LCD
Fire Detection & Suppression — MQ-2 gas sensor triggers a water pump + buzzer automatically on smoke detection
Smart Lighting — LDR-based automatic LED control
Dual-loop failsafe — fire safety checks run independently of access control


Hardware:

Arduino UNO R3 · MFRC522 RFID Reader · HC-SR04 Ultrasonic Sensor · MQ-2 Gas Sensor · LDR · SG90 Servo · 5V Relay + Water Pump · 16x2 I2C LCD · Buzzer · LED

Future Work:

IoT integration (ESP32) for remote monitoring, computer vision-based face recognition, backup battery for power outages.


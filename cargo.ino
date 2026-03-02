The system#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
 
// ---------- RFID ----------
#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);
 
// ---------- LCD ----------
LiquidCrystal_I2C lcd(0x27, 16, 2);
 
// ---------- PINS ----------
#define SERVO_PIN 6
#define TRIG_PIN 4
#define ECHO_PIN 5
#define LED_PIN 3
#define RELAY_PIN 7  	// Water pump
#define BUZZER_PIN 8
#define LDR_PIN A0
#define GAS_PIN A1
 
Servo doorServo;
 
// ---------- VARIABLES ----------
int peopleCount = 0;
int lightThreshold = 1000;
int gasThreshold = 400;
unsigned long accessDuration = 10000;
unsigned long lastCountTime = 0;
unsigned long countDelay = 1500;
 
// ---------- Authorized RFID Cards ----------
byte allowedUIDs[][4] = {
  {0x47, 0xD8, 0x2D, 0x25},
  {0x25, 0x81, 0x7E, 0xE0}
};
const int totalCards = 2;
 
void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  doorServo.attach(SERVO_PIN);
  doorServo.write(0);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  lcd.init();
  lcd.backlight();
  lcd.print(" Smart Door Sys ");
  delay(2000);
  lcd.clear();
}
 
void loop() {
  checkRFID();
  lightControl();
  gasDetection();
  displayData();
}
 
// ================= RFID =================
void checkRFID() {
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;
  bool accessGranted = false;
  for (int c = 0; c < totalCards; c++) {
	bool match = true;
	for (byte i = 0; i < 4; i++) {
  	if (rfid.uid.uidByte[i] != allowedUIDs[c][i]) {
    	match = false;
    	break;
  	}
	}
	if (match) {
  	accessGranted = true;
  	break;
	}
  }
  lcd.clear();
  if (accessGranted) {
	lcd.print("Access Granted");
	doorServo.write(90);
	unsigned long startTime = millis();
	bool prevDetection = false;
	while (millis() - startTime < accessDuration) {
  	int distance = getDistance();
  	if (distance > 0 && distance < 20) {
    	if (!prevDetection && millis() - lastCountTime > countDelay) {
      	peopleCount++;
      	lastCountTime = millis();
      	lcd.clear();
      	lcd.print("Person Entered");
      	lcd.setCursor(0, 1);
      	lcd.print("Count: ");
      	lcd.print(peopleCount);
    	}
    	prevDetection = true;
  	} else {
    	prevDetection = false;
  	}
	}
	doorServo.write(0);
	lcd.clear();
	lcd.print("Door Closed");
	delay(2000);
  } else {
	lcd.print("Access Denied");
	delay(2000);
  }
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
 
// ================= Ultrasonic =================
int getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return -1;
  return duration * 0.034 / 2;
}
 
// ================= LDR =================
void lightControl() {
  int ldrValue = analogRead(LDR_PIN);
  if (ldrValue < lightThreshold)
	digitalWrite(LED_PIN, HIGH);
  else
	digitalWrite(LED_PIN, LOW);
}
 
// ================= GAS + BUZZER + RELAY =================
void gasDetection() {
  int gasValue = analogRead(GAS_PIN);
  if (gasValue > gasThreshold) {
	digitalWrite(RELAY_PIN, HIGH);   // Water pump ON
	digitalWrite(BUZZER_PIN, HIGH);  // Buzzer ON
	lcd.setCursor(0,0);
	lcd.print("!! GAS ALERT !!");
  } else {
	digitalWrite(RELAY_PIN, LOW);
	digitalWrite(BUZZER_PIN, LOW);
  }
}
 
// ================= LCD =================
void displayData() {
  lcd.setCursor(0, 1);
  lcd.print("People:");
  lcd.print(peopleCount);
  lcd.print(" Gas:");
  lcd.print(analogRead(GAS_PIN));
  lcd.print(" ");
}
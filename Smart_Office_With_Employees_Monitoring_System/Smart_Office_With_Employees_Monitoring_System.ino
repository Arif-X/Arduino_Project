
const int AOUTpin = 0;
int buzzer = 10;
int limit = 400;
int value;
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
int fan = 3;


int isObstaclePin = 22; // This is our input pin
int isObstacle = HIGH; // HIGH MEANS NO OBSTACLE
#include <Servo.h>
Servo servo1;


#include "MFRC522.h"
#include "SPI.h"
#define SS_PIN 5
#define RST_PIN 4
#define LED_PIN1 A1
#define LED_PIN2 A2
MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
Servo servo2; // Inisiasi Servo
int isObstaclePin1 = 24; // This is our input pin
int isObstacle1 = HIGH; // HIGH MEANS NO OBSTACLE
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#define I2C_ADDR 0x27     // Jika 0x3F tidak mau coba gunakan 0x27 atau 0x20
#define BACKLIGHT_PIN 3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7
LiquidCrystal_I2C  lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);


Servo servo3; // Inisiasi Servo
int trigPin = 12; // Inisiasi Trig Pin Pada Sensor Ultrasinic
int echoPin = 13; // Inisiasi Echo Pin Pada Sensor Ultrasinic
long distance; // Inisiasi Jarak
long duration; //Inisiasi Durasi


Servo servo4; // Inisiasi Servo
int trigPin1 = A3; // Inisiasi Trig Pin Pada Sensor Ultrasinic
int echoPin1 = A4; // Inisiasi Echo Pin Pada Sensor Ultrasinic
long distance1; // Inisiasi Jarak
long duration1; //Inisiasi Durasi


int ledLDR = 8;
int LDR = A6;


void setup() {
  Serial.begin(9600);
  pinMode(AOUTpin, INPUT);

  dht.begin();
  pinMode(fan, OUTPUT);

  pinMode(ledLDR, OUTPUT);

  pinMode(isObstaclePin, INPUT_PULLUP);
  servo1.attach(6);

  servo2.attach(7); // Memetakan Pin  yang Dipakai
  SPI.begin();
  rfid.PCD_Init();
  pinMode(isObstaclePin1, INPUT_PULLUP);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  Serial.println("I am waiting for card...");


  lcd.begin (16, 17);
  // Nyalakan lampu backlight
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  delay ( 1000 );
  lcd.setBacklight (LOW);
  delay ( 1000 );
  lcd.setBacklight(HIGH);
  lcd.setCursor ( 0, 0 );        // tulis pada baris kedua
  lcd.print("    SELAMAT     ");
  lcd.setCursor ( 0, 1 );        // tulis pada baris kedua
  lcd.print("    DATANG!     ");


  servo3.attach(11); // Memetakan Pin  yang Dipakai
  Serial.begin(9600); // Inisiasi Serial
  pinMode(trigPin, OUTPUT); // Set Pin Trig ke Output
  pinMode(echoPin, INPUT); // Set Pin Echo ke Input


  servo4.attach(9); // Memetakan Pin  yang Dipakai
  pinMode(trigPin1, OUTPUT); // Set Pin Trig ke Output
  pinMode(echoPin1, INPUT); // Set Pin Echo ke Input
}
void loop() {
  int cahaya = analogRead(A6); //membaca nilai ADC LDR
  Serial.println(cahaya);
  if (cahaya > 850) {
    digitalWrite(ledLDR, HIGH); // LED menyala
  }
  else {
    digitalWrite(ledLDR, LOW); // LED mati
  }

  // Awal Setting Sensor Ultrasonic Untuk Memberi Efek Pada Servo
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); //
  distance = duration * 0.034 / 2;
  // Akhir Setting
  Serial.println(distance); // Print Jarak Objek dari Sensor Ultrasonic
  if (distance <= 10) {
    servo3.write(110);
    delay(5000);
  } else if (distance >= 100) {
    servo3.write(30);
  }
  delay(200);


  // Awal Setting Sensor Ultrasonic Untuk Memberi Efek Pada Servo
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH); //
  distance1 = duration1 * 0.034 / 2;
  // Akhir Setting
  Serial.println(distance1); // Print Jarak Objek dari Sensor Ultrasonic
  if (distance1 <= 10) {
    servo4.write(60);
    delay(5000);
  } else if (distance >= 100) {
    servo4.write(170);
  }
  delay(200);



  // ====================================================================== Gas

  value = analogRead(AOUTpin);
  Serial.print("CO Value : ");
  Serial.println(value);
  Serial.print("Limit Value : ");
  Serial.println(limit);
  delay(100);
  if (value >= limit) {
    tone(buzzer, 1000, 200);
  } else {
  }

  // ====================================================================== Ultra
  isObstacle = digitalRead(isObstaclePin);
  if (isObstacle == LOW) {
    Serial.println("Buka Servo");
    servo1.write(10);
    delay(5000);
    Serial.println("Tutup Servo");
    servo1.write(110);
  } else {
    Serial.println("Sensor Infrared Tidak Mendeteksi Objek");
  }
  delay(200);


  // ====================================================================== Suhu
  float h = dht.readHumidity();
  // Baca temperatur dalam celcius
  float t = dht.readTemperature();
  //tampilkan pada serial monitor
  if (t >= 27.0) {
    digitalWrite(fan, HIGH);
    Serial.println("Fan Bergerak");
    Serial.print("Kelembaban: ");
    Serial.print(h);
    Serial.print(" Suhu: ");
    Serial.println(t);
  } else {
    digitalWrite(fan, LOW);
    Serial.println("Fan Tidak Bergerak");
    Serial.print("Kelembaban: ");
    Serial.print(h);
    Serial.print(" Suhu: ");
    Serial.println(t);
  }
  //delay sesaat sebelum pembacaan selanjutnya
  delay(200);


  // ====================================================================== RFID

  digitalWrite(A1, HIGH);

  // put your main code here, to run repeatedly:
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  // Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  // Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }
  String strID = "";
  for (byte i = 0; i < 4; i++) {
    strID +=
      (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
      String(rfid.uid.uidByte[i], HEX) +
      (i != 3 ? ":" : "");
  }

  strID.toUpperCase();
  Serial.print("Tap card key: ");
  Serial.println(strID);
  delay(1000);

  if (strID.indexOf("89:EC:F0:BB") >= 0) {  //put your own tap card key;
    Serial.println("********************");
    Serial.println("**Authorized access**");
    Serial.println("********************");
    servo2.write(60);
    isObstacle1 = digitalRead(isObstaclePin1);
    if (isObstacle1 == LOW) {
      Serial.println("Buka Servo");
      digitalWrite(A1, LOW);
      digitalWrite(A2, HIGH);
      servo2.write(160);
      delay ( 1000 );
      lcd.setBacklight (LOW);
      delay ( 1000 );
      lcd.setBacklight(HIGH);
      lcd.setCursor ( 0, 0 );        // tulis pada baris kedua
      lcd.print("    SILAHKAN    ");
      lcd.setCursor ( 0, 1 );        // tulis pada baris kedua
      lcd.print("     MASUK!     ");
      delay(5000);
      Serial.println("Tutup Servo");
      digitalWrite(A2, LOW);
      digitalWrite(A1, HIGH);
      servo2.write(60);
      delay ( 1000 );
      lcd.setBacklight (LOW);
      delay ( 1000 );
      lcd.setBacklight(HIGH);
      lcd.setCursor ( 0, 0 );        // tulis pada baris kedua
      lcd.print("    SELAMAT     ");
      lcd.setCursor ( 0, 1 );        // tulis pada baris kedua
      lcd.print("    DATANG!     ");
    } else {
      delay ( 1000 );
      lcd.setBacklight (LOW);
      delay ( 1000 );
      lcd.setBacklight(HIGH);
      lcd.setCursor ( 0, 0 );        // tulis pada baris kedua
      lcd.print("  SENSOR TIDAK  ");
      lcd.setCursor ( 0, 1 );        // tulis pada baris kedua
      lcd.print(" MENDETEKSI OBJ ");
      Serial.println("Sensor Infrared Tidak Mendeteksi Objek");
      digitalWrite(A2, LOW);
      digitalWrite(A1, HIGH);
    }
    return;
  }
  else {
    Serial.println("****************");
    Serial.println("**Acces denied**");
    Serial.println("****************");
    digitalWrite(A1, HIGH);
    return;
  }
}

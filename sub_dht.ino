// //=================================== TES SUBSISTEM SUHU ======================================

// //peltier + pompa / kipas radiator(23) + kipas peltier

// #include "DHT.h"
// #include <SoftwareSerial.h>
// #define DHTPIN A0
// #define DHTPIN1 30
// #define DHTTYPE DHT22

// DHT dht(DHTPIN, DHTTYPE);
// DHT dht1(DHTPIN1, DHTTYPE);
// SoftwareSerial lcd(3, 2);

// const int KipasLV1 = 8;
// const int KipasLV2 = 9;
// const int IN1_4 = 13;
// const int IN2_4 = 12;
// const int IN3_4 = 14;  // MISTX4
// const int IN4_4 = 15;
// const int IN1_2 = 23;  // PELTIERKIPAS
// const int IN2_2 = 22;  // POMPA+RADIATOR
// const int IN1_1 = A6;  // RELAYPELTIER

// float h;
// float t;
// float h1;
// float t1;

// unsigned long interval = 0;
// unsigned long varial = 0;

// struct Modetanamanhias {
//   int kelembaband;
//   float suhud;
// };

// Modetanamanhias miodeterpilikh;

// void display() {

//   if (millis() - varial > 1000) {

//     lcd.print("t4.txt=\"");
//     lcd.print(miodeterpilikh.suhud, 1);
//     lcd.print("`C");
//     lcd.print("\"");
//     lcd.write(0xff);
//     lcd.write(0xff);
//     lcd.write(0xff);

//     lcd.print("t6.txt=\"");
//     lcd.print(miodeterpilikh.kelembaband);
//     lcd.print("%");
//     lcd.print("\"");
//     lcd.write(0xff);
//     lcd.write(0xff);
//     lcd.write(0xff);
//     varial = millis();
//   }
// }

// void readDHT() {
//   if (millis() - interval > 2000) {
//     h = dht.readHumidity();
//     t = dht.readTemperature();
//     h1 = dht1.readHumidity();
//     t1 = dht1.readTemperature();
//     interval = millis();  // Reset interval setelah membaca sensor
//   }
// }

// void runningSystem() {

//   if (miodeterpilikh.kelembaband < 40) {

//     digitalWrite(IN1_4, LOW);
//     digitalWrite(IN2_4, LOW);
//     digitalWrite(IN3_4, LOW);
//     digitalWrite(IN4_4, HIGH);
//     digitalWrite(IN1_2, HIGH);
//   } else {
//     digitalWrite(IN1_4, LOW);
//     digitalWrite(IN2_4, HIGH);
//     digitalWrite(IN3_4, HIGH);
//     digitalWrite(IN4_4, LOW);
//     digitalWrite(IN1_2, LOW);
//   }
// }

// void setup() {
//   Serial.begin(9600);
//   Serial.println(F("DHTxx test!"));
//   lcd.begin(9600);
//   dht.begin();
//   dht1.begin();

//   pinMode(IN3_4, OUTPUT);  // MISTX4
//   pinMode(IN1_2, OUTPUT);  // PELTIERKIPAS
// }

// void loop() {

//   readDHT();
//   display();

//   miodeterpilikh.kelembaband = (h + h1) / 2;
//   miodeterpilikh.suhud = (t + t1) / 2;
//   runningSystem();

//   delay(200);
// }

//=================================== TES SUBSISTEM SUHU ======================================

//peltier + pompa / kipas radiator(23) + kipas peltier

#include "DHT.h"
#include <SoftwareSerial.h>
#define DHTPIN A0  
#define DHTPIN1 A4    
#define DHTTYPE DHT22   

DHT dht(DHTPIN, DHTTYPE);
DHT dht1(DHTPIN1, DHTTYPE);
SoftwareSerial lcd(3,2);

const int KipasLV1 = 8;
const int KipasLV2 = 9;
const int IN1_4 = 13;
const int IN2_4 = 12;
const int IN3_4 = 14; // MISTX4
const int IN4_4 = 15;
const int IN1_2 = 23; // PELTIERKIPAS
const int IN2_2 = 22; // POMPA+RADIATOR
const int IN1_1 = A6; // RELAYPELTIER

float h;
float t;
float h1;
float t1;
float htot;
float ttot;

unsigned long interval = 0;
unsigned long varial = 0;
unsigned long status = 0;

struct Modetanamanhias {
  int kelembaband;
  float suhud;
};

Modetanamanhias miodeterpilikh;

void display(){

  htot = (h + h1) / 2.0;
  ttot = (t + t1) / 2.0;
  if(millis() - varial > 1000){

      lcd.print("t4.txt=\"");
    lcd.print(ttot,1);
    lcd.print("`C");
    lcd.print("\"");
    lcd.write(0xff);
    lcd.write(0xff);
    lcd.write(0xff);

      lcd.print("t6.txt=\"");
    lcd.print(htot,1);
    lcd.print("%");
    lcd.print("\"");
    lcd.write(0xff);
    lcd.write(0xff);
    lcd.write(0xff);
    varial = millis();

  }

}

void readDHT() {
  if (millis() - interval > 2000) {
    h = dht.readHumidity();
    t = dht.readTemperature();
    h1 = dht1.readHumidity();
    t1 = dht1.readTemperature();
    interval = millis(); // Reset interval setelah membaca sensor
  }
}

void runningSystem(Modetanamanhias &varialnya) {


  if(millis() - status > 1000){
    Serial.println(htot + String("%"));
    Serial.println(ttot);
      if (htot > varialnya.kelembaband) {
    Serial.println("iya");
    digitalWrite(IN1_4, HIGH);
    digitalWrite(IN2_4, HIGH);
    digitalWrite(IN3_4, HIGH);
    digitalWrite(IN4_4, LOW);
    digitalWrite(IN1_2, LOW);
    }
     if (htot < varialnya.kelembaband) {
    Serial.println("tidak");
    digitalWrite(IN1_4, LOW);
    digitalWrite(IN2_4, HIGH);
    digitalWrite(IN3_4, HIGH);
    digitalWrite(IN4_4, HIGH);
    digitalWrite(IN1_2, HIGH);
    }
  }

}

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  lcd.begin(9600);
  dht.begin();
  dht1.begin();

  pinMode(IN1_4, OUTPUT);
  pinMode(IN2_4, OUTPUT); 
  pinMode(IN3_4, OUTPUT); 
  pinMode(IN4_4, OUTPUT); 
  pinMode(IN1_2, OUTPUT); 

  miodeterpilikh = {70, 30};

}

void loop() {

  readDHT();
  display();

  runningSystem(miodeterpilikh);

  delay(200);

}
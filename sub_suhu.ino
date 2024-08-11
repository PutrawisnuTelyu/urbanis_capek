#include "DHT.h"
#include <SoftwareSerial.h>
#define DHTPIN A0  
#define DHTPIN1 A4    
#define DHTTYPE DHT22   

DHT dht(DHTPIN, DHTTYPE);
DHT dht1(DHTPIN1, DHTTYPE);
SoftwareSerial lcd(3,2);

const int IN1_2 = 23; // PELTIERKIPAS
const int IN2_2 = 22; // POMPA+RADIATOR
const int PSU   = A6;

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
      if (htot > varialnya.suhud) {
        digitalWrite(IN1_2, LOW);
        digitalWrite(IN2_2, LOW);
        digitalWrite(PSU, LOW);

      }
      else if (htot < varialnya.suhud) {
        digitalWrite(IN1_2, HIGH);
        digitalWrite(IN2_2, HIGH);
        digitalWrite(PSU, HIGH);
      }
  }

}

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  lcd.begin(9600);
  dht.begin();
  dht1.begin();

  pinMode(IN1_2, OUTPUT);
  pinMode(IN2_2, OUTPUT); 
  pinMode(PSU, OUTPUT); 

  miodeterpilikh = {100, 30};

}

void loop() {

  readDHT();
  display();

  runningSystem(miodeterpilikh);

  delay(200);

}
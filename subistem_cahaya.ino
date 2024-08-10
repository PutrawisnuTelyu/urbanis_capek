//=================================== TES SUBSISTEM CAHAYA ==================

#include <SoftwareSerial.h>
#include <PID_v1.h>
#include <Wire.h>
#include <BH1750.h> // Library untuk BH1750

const int ENA_BB = 5;
const int ENB_BB = 4;
const int IN1_BB = 17;
const int IN2_BB = 16;
const int IN3_BB = 19;
const int IN4_BB = 18;
const int ENA_BK = 6;
const int ENB_BK = 7;
const int IN1_BK = 28;
const int IN2_BK = 29;
const int IN3_BK = 26;
const int IN4_BK = 27;

BH1750 lux1; // I2C
BH1750 lux2; // I2C
BH1750 lux3; // I2C
BH1750 lux4; // I2C

double Setpoint ; //will be the desired value
double Input; //photo sensor
double Output ; //driver
double lux11;
double lux22;
double lux33;
double lux44;
float total = 0;
float n1;
float n2;
float n3;
float n4;

double Kp=5, Ki=10, Kd=0; 
unsigned long val = 0;
unsigned long interval4 = 0;


SoftwareSerial lcd(3,2);
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

void TCA9548A(uint8_t bus){
  Wire.beginTransmission(0x70);  // Alamat TCA9548A
  Wire.write(1 << bus);          // Kirim byte untuk memilih bus
  Wire.endTransmission();
}

void running(){

  if(millis() - interval4 > 100){

    hitungLux();
    interval4 = millis();

  } 

}

float hitunglux1(BH1750 lux, int bus){

  TCA9548A(bus);
  float n1 = lux.readLightLevel();
  return n1;
}

void hitungLux(){

  n1 = hitunglux1(lux2, 0);
  n2 = hitunglux1(lux1, 2);
  n3 = hitunglux1(lux3, 1);
  n4 = hitunglux1(lux4, 5);
  total = (n1 + n2 + n3 + n4) / 4.0;

}

void setup(){

  Serial.begin(9600);
  Wire.begin();
  lcd.begin(9600);

  TCA9548A(2);
  lux1.begin();
  Serial.println();
  
  TCA9548A(0);
  lux2.begin();
  Serial.println();
  
  TCA9548A(1);
  lux3.begin();
  Serial.println();
  
  TCA9548A(5);
  lux4.begin();
  Serial.println();

  pinMode(ENA_BB, OUTPUT);
  pinMode(ENB_BB, OUTPUT);
  pinMode(IN1_BB, OUTPUT);
  pinMode(IN2_BB, OUTPUT);
  pinMode(IN3_BB, OUTPUT);
  pinMode(IN4_BB, OUTPUT);
  pinMode(ENA_BK, OUTPUT);
  pinMode(ENB_BK, OUTPUT);
  pinMode(IN1_BK, OUTPUT);
  pinMode(IN2_BK, OUTPUT);
  pinMode(IN3_BK, OUTPUT);
  pinMode(IN4_BK, OUTPUT);

  digitalWrite(IN1_BB, HIGH); //lampu pink atas
  digitalWrite(IN2_BB, LOW);
  digitalWrite(IN3_BB, LOW);  //lampu putih atas
  digitalWrite(IN4_BB, HIGH);

  digitalWrite(IN2_BK, HIGH);
  digitalWrite(IN1_BK, LOW);
  digitalWrite(IN3_BK, LOW);
  digitalWrite(IN4_BK, HIGH);

  Setpoint = 30;
  //Turn the PID on
  myPID.SetMode(AUTOMATIC);
  //Adjust PID values
  myPID.SetTunings(Kp, Ki, Kd);

}

void loop(){

  // analogWrite(ENA_BB, 200); //Untuk IN3 DAN IN4
  // analogWrite(ENB_BB, 200); //Untuk IN1 DAN IN2
  // analogWrite(ENB_BK, 200); //Untuk IN1 DAN IN2
  // analogWrite(ENA_BK, 200); //Untuk IN1 DAN IN2
  running();
  // TCA9548A(0);
  // Serial.println("bus 0 : " + String(lux2.readLightLevel()) + "\n"); //bawah kiri
  
  // TCA9548A(2);
  // Serial.println("bus 2 : " + String(lux1.readLightLevel()) + "\n");
  
  //TCA9548A(1);
  // Serial.println("bus 1 : " + String(lux3.readLightLevel()) + "\n");
  
  // TCA9548A(5);
  // Serial.println("bus 5 : " + String(lux4.readLightLevel()) + "\n");
  // Serial.print("total : " + String(total) + "\n\n\n");
   if(millis() - val > 100){
      Serial.print("\nTotal: ");
      Serial.print(total);
      val = millis();
    }

  Input = map(total, 0, 1023, 0, 200);  
  myPID.Compute();
  analogWrite(ENA_BB,Output); 
  analogWrite(ENB_BB,Output); 
  analogWrite(ENA_BK,Output); 
  analogWrite(ENB_BK,Output); 
  Serial.print(Input);
  Serial.print(" ");
  Serial.println(Output);
  Serial.print(" ");  
  Serial.println(Setpoint);
  delay(100); 
}
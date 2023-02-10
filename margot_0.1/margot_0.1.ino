#include <Servo.h>
#include <RF24.h>
#include <LiquidCrystal_I2C.h>

#define MotorX 3
#define MotorY 5
#define MotorZ 6
#define LedIndicator 2
#define Firmware "Rev. 0.1"


struct MyData {
 int DT_joyL_A = 90;
 int DT_joyL_B = 90;
 int DT_joyR_A = 90;
 int DT_joyR_B = 90;
 bool DT_joyL_btn = false;
 bool DT_joyR_btn = false;
};

MyData data;

Servo Sv1,Sv2, Sv3;
RF24 radio(7,8); //CE - CSN
LiquidCrystal_I2C led(0x27,16,2);

const byte address[6] = "1Node";
int PosX,PosY,PosZ;

void setup() {
  Serial.begin(115200);
  led.init();
  led.backlight();
  
  if(!radio.begin()){
   Display("Margot Started!","Radio Not Resp.");
   while(1);
  }else{
   radio.openReadingPipe(0,address);
   radio.setPALevel(RF24_PA_MIN);
   radio.startListening(); 
   Display("Margot Started!","Radio Started.");
  }
  Sv1.attach(MotorX);
  Sv2.attach(MotorY);
  Sv3.attach(MotorZ);
  pinMode(LedIndicator,OUTPUT);
  
  StartPos();
  Indicator();
  delay(4000);
  led.clear();
}

void loop() {

if(radio.available()){
  radio.read(&data, sizeof(MyData));
  Sync();
  M_Position();
  UpdateInfo(); 
}
}
void StartPos (){
  Sv1.write(90);
  Sv2.write(90);
  Sv3.write(90);
}

void Indicator(){
  digitalWrite(LedIndicator,HIGH);
}
void Sync(){
  PosX = map(data.DT_joyR_A,0,1023,0,180);
  PosY = map(data.DT_joyR_B,0 ,1023,0,180);
  PosZ = map(data.DT_joyL_B,0,1023,0,180);
}
void M_Position(){
  Sv1.write(PosX);

  if(PosY < 90){
    Sv2.write(90);
  }else{
    Sv2.write(PosY);
  }

  Sv3.write(PosZ);
}

void Debug(){
  Serial.println("X:"  + String(PosX));
  Serial.println("Y:"  + String(PosY));
  Serial.println("Z:"  + String(PosZ));
  
}
void Display(String line1,String Line2){
  led.setCursor(0,0);
  led.println(line1);
  led.setCursor(0,1);
  led.println(Line2);
}
void UpdateInfo(){
 Display("Margot: " + String(Firmware),
 "X:" + String(PosX) + " Y:" + String(PosY)+
 " Z:" + String(PosZ)); 
}

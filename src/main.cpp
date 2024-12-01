#include <Arduino.h>
#include<Wire.h>

float rate_roll,rate_pitch,rate_yaw;
float cal_rate_roll,cal_rate_pitch,cal_rate_yaw;

float lsb=65.5;

void gyro();
void setup() {
  Serial.begin(115200);
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  Wire.setClock(400000);
  Wire.begin();
  delay(250);

  Wire.beginTransmission(0x68);
  Wire.write(0x6B);//low pass filter
  Wire.write(0x00);
  Wire.endTransmission();

  for (int i = 0; i < 2000; i++){
    gyro();
    cal_rate_pitch+=rate_pitch;
    cal_rate_roll+=rate_roll;
    cal_rate_yaw+=rate_yaw;
    delay(1);
  }

  cal_rate_pitch/=2000;
  cal_rate_roll/=2000;
  cal_rate_yaw/=2000;
  


}

void loop() {
  gyro();
  rate_roll-=cal_rate_roll;
  Serial.print("roll ");
  Serial.print(rate_roll);
  Serial.print("\t\t");
  
  rate_pitch-=cal_rate_pitch;
  Serial.print("pitch ");
  Serial.print(rate_pitch);
  Serial.print("\t\t");

  rate_yaw-=cal_rate_yaw;
  Serial.print("yaw ");
  Serial.print(rate_yaw);
  Serial.println("\t\t");

  

  delay(50);
  
}

void gyro(){
  Wire.beginTransmission(0x68);
  Wire.write(0x1A);//low pass filter
  Wire.write(0x05);
  Wire.endTransmission();

  Wire.beginTransmission(0x68);
  Wire.write(0x1B);//set the sensivity scale factor
  Wire.write(0x8);
  Wire.endTransmission();

  Wire.beginTransmission(0x68);
  Wire.write(0x43);//access the stroing mesurements
  Wire.endTransmission();

  Wire.requestFrom(0x68,6);// 6==registoers
  int16_t gyro_X = Wire.read()<<8 | Wire.read();
  int16_t gyro_Y = Wire.read()<<8 | Wire.read();
  int16_t gyro_Z = Wire.read()<<8 | Wire.read();

  // because of lsb is 65.5

  rate_roll=(float)gyro_X/lsb;
  rate_pitch=(float)gyro_Y/lsb;
  rate_yaw=(float)gyro_Z/lsb;
  

}

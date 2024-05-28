//Author: benppppp
//Direction LED should be 2s green and 4s red (no need to connect motor to see that)

#include <Wire.h>

void setup() {
  Wire.begin();
}

void loop() {
   //direction (no motor start without this)
  Wire.beginTransmission(0x0f);  // begin transmission
  Wire.write(0xaa);              // Direction control header
  Wire.write(0x0a);              // send direction control information BothClockWise=0x0a, BothAntiClockWise=0x05, M1CWM2ACW=0x06, M1ACWM2CW=0x09
  Wire.write(0x01);              // need to send this byte as the third byte(no meaning)
  Wire.endTransmission();

  //motor PWM frequence (1-65535)
  Wire.beginTransmission(0x0f);  // begin transmission
  Wire.write(0x84);              // set pwm frequence header
  Wire.write(00);                // pwm frequence LSB
  Wire.write(0x4F);              // pwm frequence MSB
  Wire.endTransmission();        //frequence=20224 Hz

  //motor speed (0-255)
  Wire.beginTransmission(0x0f);  // begin transmission
  Wire.write(0x82);              // set pwm header
  Wire.write(200);               // send speed of motor1
  Wire.write(200);               // send speed of motor2
  Wire.endTransmission();

  delay(2000);

    //direction
  Wire.beginTransmission(0x0f);  // begin transmission
  Wire.write(0xaa);              // Direction control header
  Wire.write(0x05);              // send direction control information BothClockWise=0x0a, BothAntiClockWise=0x05, M1CWM2ACW=0x06, M1ACWM2CW=0x09
  Wire.write(0x01);              // need to send this byte as the third byte(no meaning)
  Wire.endTransmission();

  //motor PWM frequence (1-65535)
  Wire.beginTransmission(0x0f);  // begin transmission
  Wire.write(0x84);              // set pwm frequence header
  Wire.write(00);                // pwm frequence LSB
  Wire.write(0x02);              // pwm frequence MSB
  Wire.endTransmission();        //frequence=512 Hz

  //motor speed (0-255)
  Wire.beginTransmission(0x0f);  // begin transmission
  Wire.write(0x82);              // set pwm header
  Wire.write(200);               // send speed of motor1
  Wire.write(200);               // send speed of motor2
  Wire.endTransmission();

  delay(4000);
}

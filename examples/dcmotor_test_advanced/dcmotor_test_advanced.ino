/* 
 * motor_test.ino
 * Example sketch for Grove - I2C Motor Driver v1.3
 *
 * Copyright (c) 2012 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : Jerry Yip, benppppp
 * Create Time: 2017-02
 * Change Log : 2024/05/28 add frequence example
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "Grove_I2C_Motor_Driver.h"

// default I2C address is 0x0f
#define I2C_ADDRESS 0x0f

void setup() {
  Motor.begin(I2C_ADDRESS);
  Serial.begin(9600);
  Serial.println();
  Motor.timeout(1000); //motor stop after this time in millis - available for firmware v105+
  Motor.frequence(1000);  //for motor driver STM32 (L298) - Possible frequences 1 - > 65535 - available for firmware v103+
  //Motor.frequence(F_490Hz); //for motor driver 1.3 - Possible frequences F_31372Hz, F_3921Hz, F_490Hz, F_122Hz, F_30Hz
  
  //Get firmware version
  unsigned int version = Motor.getversion();  //Get the motor driver firmware version - version avaiable for firmware v105+
  if (version == 0) {
    Serial.println("Firmware <105 -> upgrade your L298 motor driver firmware !");
  } else if (version > 0 && version < 6) {
    Serial.println("Wire I2C error: " + (String)version);
  } else {
    Serial.println("Firmware version: " + (String)version);
  }
}

void loop() {
  // Set speed of MOTOR1, Clockwise, speed: -255~255
  Motor.speed(MOTOR1, 100);
  // Set speed of MOTOR2, Anticlockwise
  Motor.speed(MOTOR2, -120);
  // Verify timed out before and after delay
  Serial.print("Timed out1: ");
  Serial.print(Motor.gettimedout());
  delay(4000);
  Serial.print(" Timed out2: ");
  Serial.println(Motor.gettimedout());
  // Change speed and direction of MOTOR1
  Motor.speed(MOTOR1, -50);
  // Change speed and direction of MOTOR2
  Motor.speed(MOTOR2, 50);
  delay(2000);
  // Stop MOTOR1 and MOTOR2
  // Can be stopped with speed 0 too!
  Motor.stop(MOTOR1);
  Motor.stop(MOTOR2);
  delay(2000);
}

// End of file

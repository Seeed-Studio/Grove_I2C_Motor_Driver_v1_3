/*
    steppermotor_test.ino
    Example sketch for Grove - I2C Motor Driver v1.3

    Copyright (c) 2012 seeed technology inc.
    Website    : www.seeed.cc
    Author     : Jerry Yip, benppppp
    Create Time: 2017-02
    Change Log : 2024-05-28 Example & comments with motor type and mode

    The MIT License (MIT)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#include <Grove_I2C_Motor_Driver.h>

// default I2C address is 0x0f
#define I2C_ADDRESS 0x0f


void setup() {
  Serial.begin(9600);
  Motor.begin(I2C_ADDRESS);
  // Drive a stepper motor
  //StepperRun(int _step, int _type, int _mode)
  // _step: -1024~1024, when _step>0, stepper motor runs clockwise; when _step<0,
  // stepper motor runs anticlockwise; when _step is 512, the stepper motor will
  // run a complete turn; if step is 1024, the stepper motor will run 2 turns.
  //  _type: 0 -> 4 phase stepper motor, default
  //         1 -> 2 phase stepper motor
  //  _mode: 0 -> compatible mode (_step=1 corresponds 4 steps)
  //         1 -> fine mode (_step1 corresponds 1 steps)
  Motor.StepperRun(-1024, 1, 1);
  Motor.StepperRun(512, 1, 1);
}

void loop() {
}

// End of file

// this example is CC0 = Public Domain
// written by Stefan Krüger s-light.eu

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

  delay(500);
  Serial.println("steppermotor_test_modes.ino");
  delay(500);

  Serial.println("0, 0 --> 4 phase stepper, compatible mode 1=4");
  Motor.StepperRun(-500, 0, 0);
  delay(5000);

  Serial.println("0, 1 --> 4 phase stepper, fine mode 1=1");
  Motor.StepperRun(-500, 0, 1);
  delay(5000);

  
  Serial.println("1, 0 --> 2 phase stepper, compatible mode 1=4");
  Motor.StepperRun(-500, 1, 0);
  delay(5000);

  Serial.println("1, 1 --> 2 phase stepper, fine mode 1=1");
  Motor.StepperRun(-500, 1, 1);
  delay(5000);
  
}

void loop() {
}

// End of file

# Grove - I2C Motor Driver v1.3

![](https://raw.githubusercontent.com/SeeedDocument/Grove-I2C_Motor_Driver_V1.3/master/img/I2CMotorDriver_New.jpg)


The Grove - I2C Motor Driver V1.3 (latest version) can directly control Stepper Motor or DC Motor. Its heart is a dual channel H-bridge driver chip（L298P）that can handle current up to 2A per channel, controlled by an Atmel ATmega8L which handles the I2C communication with for example an Arduino. Both motors can be driven simultaneously while set to a different speed and direction. It can power two brushed DC motors or one 4-wire two-phase stepper motor. It requires a 6V to 15V power supply to power the motor and has an onboard 5V voltage regulator which can power the I2C bus and the Arduino(selectable by jumper). All driver lines are protected by diodes from back-EMF.

- Grove Compatible
- I2C Interface
- Adjustable motor speed and rotation direction
- Changeable slave address by hardware


## Installation

```
git clone https://github.com/Seeed-Studio/Grove_I2C_Motor_Driver_v1_3.git
```
or download the zip.

## Usage

Simply copy the Grove_I2C_Motor_Driver_v1_3 folder to your Arduino library collection. For example, arduino-1.6.12/libraries. Next time you run the Arduino IDE, you'll have a new option in Sketch -> Include Library -> Grove_I2C_Motor_Driver_v1_3. Review the included examples in Grove_I2C_Motor_Driver_v1_3/examples.

### 1. Set the address of the I2C Motor Driver

- Set the address by dial switch is a new function added to the new I2C Motor Driver.

    ![](https://raw.githubusercontent.com/SeeedDocument/Grove-I2C_Motor_Driver_V1.3/master/img/I2CMotorDriver-9.jpg)

- Then keep the address setup in the program the same as the address setup on the I2C motor driver. The default address setup in the program is 0x0f.
    ```
    // default I2C address is 0x0f
    #define I2C_ADDRESS 0x0f

    void setup() {
      Motor.begin(I2C_ADDRESS);
    }
```

### 2. Drive 2 DC motors

- There are 2 functions to control DC motors:
    ```
    // Set the speed of a motor, speed is equal to duty cycle here
    void speed(unsigned char motor_id, int _speed);

    // Stop one motor
    void stop(unsigned char motor_id);
```
With speed() function, you are able to drive one motor at the speed you want.

    **motor_id** represents which motor to use. You can fill MOTOR1 or MOTOR2.

    **_speed** represents the speed you set to the motor. You can fill -100~100 here. When _speed>0, DC motor runs clockwise, while _speed<0, DC motor runs anticlockwise. And the bigger the absolute value of _speed, the faster the speed of DC motor.

    With stop() function, you are able to stop a running DC motor.

    **motor_id** represents which motor to use. You can fill MOTOR1 or MOTOR2.


### 3. Drive a Stepper Motor

- We provide one function to drive a stepper motor.
    ```
    // Drive a stepper motor
    
    void StepperRun(int _step);
```
**_step** represents the steps you set to the stepper motor to run. You can fill -1024~1024. When _step>0, stepper motor runs clockwise, while _step<0, stepper motor runs anticlockwise. When _step is 512/-512, the stepper motor will run a complete turn and if _step is 1024/-1024, the stepper motor will run 2 turns. The stepper motor will stop automatically after it finishes its steps.

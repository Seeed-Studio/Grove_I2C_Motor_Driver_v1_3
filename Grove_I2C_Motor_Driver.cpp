/*
    Grove_I2C_Motor_Driver.cpp
    A library for Grove - I2C Motor Driver v1.3

    Copyright (c) 2012 seeed technology inc.
    Website    : www.seeed.cc
    Author     : Jerry Yip , benppppp
    Create Time: 2017-02
    Change Log : 2018-05-31 1.support two phase stepper motor
                 2024-06-28 V1.0.4 support frequence. For L298 , needs firmwre 103 update (by benppppp)
		 2025-03-20 V1.0.5 get firmware version and set timeout. For L298 , needs firmwre 105 update (by benppppp)

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


#include <Grove_I2C_Motor_Driver.h>
#include <Wire.h>

// *********************************Initialize*********************************
// Initialize I2C with an I2C address you set on Grove - I2C Motor Driver v1.3
// default i2c address: 0x0f
void I2CMotorDriver::begin(unsigned char i2c_add)
{
	Serial.begin(9600);
	if (i2c_add > 0x0f) {
		Serial.println("Error! I2C address must be between 0x00 to 0x0F");
		while(1);
	}
	Wire.begin();
	delayMicroseconds(10000);
	this->_i2c_add = i2c_add;
	// Motor shield V1.3 Set default frequence to F_3921Hz=0x02
	// Motor shield STM32 (L298) Set default frequence to 514 Hz=0x0202
	frequence(0x0202);
}

// *****************************Private Function*******************************
// Set the direction of 2 motors
// _direction: M1CWM2ACW(M1 ClockWise M2 AntiClockWise), M1ACWM2CW, BothClockWise, BothAntiClockWise, 
void I2CMotorDriver::direction(unsigned char _direction)
{
	Wire.beginTransmission(this->_i2c_add); // begin transmission
  	Wire.write(DirectionSet);               // Direction control header
  	Wire.write(_direction);                 // send direction control information
  	Wire.write(Nothing);                    // need to send this byte as the third byte(no meaning)  
  	Wire.endTransmission();                 // stop transmitting 
 }

// *****************************DC Motor Function******************************
// Set the speed of a motor, speed is equal to duty cycle here
// motor_id: MOTOR1, MOTOR2
// _speed: -255~255, when _speed>0, dc motor runs clockwise; when _speed<0,
// dc motor runs anticlockwise
void I2CMotorDriver::speed(unsigned char motor_id, int _speed) {
    if (motor_id < MOTOR1 || motor_id > MOTOR2) {
        Serial.println("Motor id error! Must be MOTOR1 or MOTOR2");
        return;
    }

    if (motor_id == MOTOR1) {
        if (_speed >= 0) {
            this->_M1_direction = 1;
            _speed = _speed > 255 ? 255 : _speed;
            this->_speed1 = _speed;
        } else if (_speed < 0) {
            this->_M1_direction = -1;
            _speed = _speed < -255 ? 255 : -(_speed);
            this->_speed1 = _speed;
        }
    } else if (motor_id == MOTOR2) {
        if (_speed >= 0) {
            this->_M2_direction = 1;
            _speed = _speed > 255 ? 255 : _speed;
            this->_speed2 = _speed;
        } else if (_speed < 0) {
            this->_M2_direction = -1;
            _speed = _speed < -255 ? 255 : -(_speed);
            this->_speed2 = _speed;
        }
    }
    // Set the direction
    if (_M1_direction == 1 && _M2_direction == 1) {
        direction(BothClockWise);
    }
    if (_M1_direction == 1 && _M2_direction == -1) {
        direction(M1CWM2ACW);
    }
    if (_M1_direction == -1 && _M2_direction == 1) {
        direction(M1ACWM2CW);
    }
    if (_M1_direction == -1 && _M2_direction == -1) {
        direction(BothAntiClockWise);
    }
    // send command
    Wire.beginTransmission(this->_i2c_add); // begin transmission
    Wire.write(MotorSpeedSet);              // set pwm header
    Wire.write(this->_speed1);              // send speed of motor1
    Wire.write(this->_speed2);              // send speed of motor2
    Wire.endTransmission();    		        // stop transmitting
    //delay(4); 				            // Wait
}

// Set the frequence of PWM
// Motor shield V1.3: F_3921Hz is default _frequence: F_31372Hz, F_3921Hz, F_490Hz, F_122Hz, F_30Hz (first byte used only)
// Motor shield STM32 (L298): _frequence send with 2 bytes
void I2CMotorDriver::frequence(unsigned int _frequence)
{
	Wire.beginTransmission(this->_i2c_add); // begin transmission
  	Wire.write(PWMFrequenceSet);            // set frequence header
 	Wire.write(_frequence & 0xFF);          // send frequence LSB
 	Wire.write(_frequence >> 8);            // send _frequence MSB 
 	Wire.endTransmission();                 // stop transmitting
}

// Set the timeout in ms
// Motor stop after that timeout
void I2CMotorDriver::timeout(unsigned int _timeout)
{
	Wire.beginTransmission(this->_i2c_add); // begin transmission
  	Wire.write(TimeoutSet);                 // set GetVersion header
 	Wire.write(_timeout & 0xFF);            // send frequence LSB
 	Wire.write(_timeout >> 8);              // send _frequence MSB 
 	Wire.endTransmission();                 // stop transmitting
}

// Get the firmware version
// Version=0:firmware<105
// Version=65535:I2C no connected
// Version from 1 to 5:wire error
uint16_t I2CMotorDriver::getversion()
{
	//Will return motor speed=0 if version<105
	speed(MOTOR1, 0);
	delayMicroseconds(50);
	speed(MOTOR2, 0);
	delayMicroseconds(50);
	Wire.beginTransmission(this->_i2c_add); // begin transmission
  	Wire.write(GetVersion);                 // set Firmware Version header
 	Wire.write(0);                          // no meaning	
 	Wire.write(0);                          // no meaning 
 	uint8_t _error=Wire.endTransmission();  // stop transmitting
	delayMicroseconds(50);
	if (_error>0) return _error;
	Wire.requestFrom((uint8_t)(this->_i2c_add), (uint8_t)2);
    uint16_t _b1 = Wire.read();
    uint16_t _b2 = Wire.read();
    return (uint16_t)(_b1 + (_b2 << 8));
}

// Get timed out
// Available only with V105+
// True if timeout occured
// Reset with speed, frequence, stop
uint16_t I2CMotorDriver::gettimedout()
{
	Wire.beginTransmission(this->_i2c_add); // begin transmission
  	Wire.write(GetTimedout);                // set GetTimed out Version header
 	Wire.write(0);                          // no meaning	
 	Wire.write(0);                          // no meaning 
 	Wire.endTransmission();                 // stop transmitting
	delayMicroseconds(2);
	Wire.requestFrom((uint8_t)(this->_i2c_add), (uint8_t)2);
    uint16_t _b1 = Wire.read();
    uint16_t _b2 = Wire.read();
    return (uint16_t)(_b1 + (_b2 << 8));
}

// Stop one motor
// motor_id: MOTOR1, MOTOR2
void I2CMotorDriver::stop(unsigned char motor_id)
{
	if (motor_id<MOTOR1 || motor_id>MOTOR2) {
		Serial.println("Motor id error! Must be MOTOR1 or MOTOR2");
		return;
	}
	speed(motor_id, 0);
}

// ***************************Stepper Motor Function***************************
// Drive a stepper motor
// _step: -1024~1024, when _step>0, stepper motor runs clockwise; when _step<0,
// stepper motor runs anticlockwise; when _step is 512, the stepper motor will
// run a complete turn; if step is 1024, the stepper motor will run 2 turns.
//  _type: 0 -> 4 phase stepper motor, default
//         1 -> 2 phase stepper motor
//  _mode: 0 -> compatible mode (_step=1 corresponds 4 steps)
//         1 -> fine mode (_step1 corresponds 1 steps)
void I2CMotorDriver::StepperRun(int _step, int _type, int _mode) {
    int _direction = 1;
    if (_step > 0) {
        _direction = 1; //clockwise
        _step = _step > 1024 ? 1024 : _step;
    } else if (_step < 0) {
        _direction = -1; //anti-clockwise
        _step = _step < -1024 ? 1024 : -(_step);
    }
    this->_speed1 = 255;
    this->_speed2 = 255;
    Wire.beginTransmission(this->_i2c_add); // begin transmission
    Wire.write(MotorSpeedSet);              // set pwm header
    Wire.write(this->_speed1);              // send speed of motor1
    Wire.write(this->_speed2);              // send speed of motor2
    Wire.endTransmission();    		        // stop transmitting

    if (_type == 1) {
        if (_direction == 1) {				// 2 phase motor
            for (int i = 0; i < _step; i++) {
                if (_mode == 0) {
                    direction(0b0001); delay(4);
                    direction(0b0101); delay(4);
                    direction(0b0100); delay(4);
                    direction(0b0110); delay(4);
                    direction(0b0010); delay(4);
                    direction(0b1010); delay(4);
                    direction(0b1000); delay(4);
                    direction(0b1001); delay(4);
                } else {
                    switch (_step_cnt) {
                        case 0 : direction(0b0001); delay(4); direction(0b0101); delay(4); break;
                        case 1 : direction(0b0100); delay(4); direction(0b0110); delay(4); break;
                        case 2 : direction(0b0010); delay(4); direction(0b1010); delay(4); break;
                        case 3 : direction(0b1000); delay(4); direction(0b1001); delay(4); break;
                    }
                    _step_cnt = (_step_cnt + 1) % 4;
                }
            }
        } else if (_direction == -1) {
            for (int i = 0; i < _step; i++) {
                if (_mode == 0) {
                    direction(0b1000); delay(4);
                    direction(0b1010); delay(4);
                    direction(0b0010); delay(4);
                    direction(0b0110); delay(4);
                    direction(0b0100); delay(4);
                    direction(0b0101); delay(4);
                    direction(0b0001); delay(4);
                    direction(0b1001); delay(4);
                } else {
                    switch (_step_cnt) {
                        case 0 : direction(0b1000); delay(4); direction(0b1010);  delay(4); break;
                        case 1 : direction(0b0010); delay(4); direction(0b0110);  delay(4); break;
                        case 2 : direction(0b0100); delay(4); direction(0b0101);  delay(4); break;
                        case 3 : direction(0b0001); delay(4); direction(0b1001);  delay(4); break;
                    }
                    _step_cnt = (_step_cnt + 1) % 4;
                }
            }
        }
    } else if (_type == 0) {
        if (_direction == 1) {				// 4 phase motor
            for (int i = 0; i < _step; i++) {
                if (_mode == 0) {
                    direction(0b0001); delay(4);
                    direction(0b0011); delay(4);
                    direction(0b0010); delay(4);
                    direction(0b0110); delay(4);
                    direction(0b0100); delay(4);
                    direction(0b1100); delay(4);
                    direction(0b1000); delay(4);
                    direction(0b1001); delay(4);
                } else {
                    switch (_step_cnt) {
                        case 0 : direction(0b0001); delay(4); direction(0b0011); delay(4); break;
                        case 2 : direction(0b0010); delay(4); direction(0b0110); delay(4); break;
                        case 3 : direction(0b0100); delay(4); direction(0b1100); delay(4); break;
                        case 4 : direction(0b1000); delay(4); direction(0b1001); delay(4); break;
                    }
                    _step_cnt = (_step_cnt + 1) % 4;
                }
            }
        } else if (_direction == -1) {
            for (int i = 0; i < _step; i++) {
                if (_mode == 0) {
                    direction(0b1000); delay(4);
                    direction(0b1100); delay(4);
                    direction(0b0100); delay(4);
                    direction(0b0110); delay(4);
                    direction(0b0010); delay(4);
                    direction(0b0011); delay(4);
                    direction(0b0001); delay(4);
                    direction(0b1001); delay(4);
                } else {
                    switch (_step_cnt) {
                        case 0 : direction(0b1000); delay(4); direction(0b1100); delay(4); break;
                        case 1 : direction(0b0100); delay(4); direction(0b0110); delay(4); break;
                        case 2 : direction(0b0010); delay(4); direction(0b0011); delay(4); break;
                        case 3 : direction(0b0001); delay(4); direction(0b1001); delay(4); break;
                    }
                    _step_cnt = (_step_cnt + 1) % 4;
                }
            }
        }
    }
}


I2CMotorDriver Motor;

// End of File

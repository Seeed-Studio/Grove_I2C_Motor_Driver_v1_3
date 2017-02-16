/* 
 * Grove_I2C_Motor_Driver.cpp
 * A library for Grove - I2C Motor Driver v1.3
 *
 * Copyright (c) 2012 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : Jerry Yip
 * Create Time: 2017-02
 * Change Log :
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
	// Set default frequence to F_3921Hz
	frequence(F_3921Hz);
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
  	delay(4); 				                // wait
}

// *****************************DC Motor Function******************************
// Set the speed of a motor, speed is equal to duty cycle here
// motor_id: MOTOR1, MOTOR2
// _speed: -100~100, when _speed>0, dc motor runs clockwise; when _speed<0, 
// dc motor runs anticlockwise
void I2CMotorDriver::speed(unsigned char motor_id, int _speed)
{
	if(motor_id<MOTOR1 || motor_id>MOTOR2) {
		Serial.println("Motor id error! Must be MOTOR1 or MOTOR2");
		return;
	}

	if(motor_id == MOTOR1) {
		if (_speed >= 0) {
			this->_M1_direction = 1; 
			_speed = _speed > 100 ? 100 : _speed;
			this->_speed1 = map(_speed, 0, 100, 0, 255);
		}
		else if (_speed < 0) {
			this->_M1_direction = -1;
			_speed = _speed < -100 ? 100 : -(_speed);
			this->_speed1 = map(_speed, 0, 100, 0, 255);
		}
	}
	else if(motor_id == MOTOR2) {
		if (_speed >= 0) {
			this->_M2_direction = 1;
			_speed = _speed > 100 ? 100 : _speed;
			this->_speed2 = map(_speed, 0, 100, 0, 255);
		}
		else if (_speed < 0) {
			this->_M2_direction = -1;
			_speed = _speed < -100 ? 100 : -(_speed);
			this->_speed2 = map(_speed, 0, 100, 0, 255);
		}
	}
	// Set the direction
	if (_M1_direction == 1 && _M2_direction == 1) direction(BothClockWise);
	if (_M1_direction == 1 && _M2_direction == -1) direction(M1CWM2ACW);
	if (_M1_direction == -1 && _M2_direction == 1) direction(M1ACWM2CW);
	if (_M1_direction == -1 && _M2_direction == -1) direction(BothAntiClockWise);
	// send command
	Wire.beginTransmission(this->_i2c_add); // begin transmission
 	Wire.write(MotorSpeedSet);              // set pwm header 
 	Wire.write(this->_speed1);              // send speed of motor1
  	Wire.write(this->_speed2);              // send speed of motor2
  	Wire.endTransmission();    		        // stop transmitting
  	delay(4); 				                // Wait 
}

// Set the frequence of PWM(cycle length = 510, system clock = 16MHz)
// F_3921Hz is default
// _frequence: F_31372Hz, F_3921Hz, F_490Hz, F_122Hz, F_30Hz
void I2CMotorDriver::frequence(unsigned char _frequence)
{
	if (_frequence < F_31372Hz || _frequence > F_30Hz) {
		Serial.println("frequence error! Must be F_31372Hz, F_3921Hz, F_490Hz, F_122Hz, F_30Hz");
		return;
	}
	Wire.beginTransmission(this->_i2c_add); // begin transmission
  	Wire.write(PWMFrequenceSet);            // set frequence header
 	Wire.write(_frequence);                 // send frequence 
 	Wire.write(Nothing);                    // need to send this byte as the third byte(no meaning)  
 	Wire.endTransmission();                 // stop transmitting
 	delay(4); 				                // wait
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
void I2CMotorDriver::StepperRun(int _step) 
{
	int _direction = 1;
	if (_step > 0) {
		_direction = 1; //clockwise
		_step = _step > 1024 ? 1024 : _step;
	}
	else if (_step < 0) {
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
  	delay(4); 				                // wait
  	
  	if (_direction == 1) {	
	  	for (int i=0; i<_step; i++) {
	  		direction(0b0001);
	  		direction(0b0011);
	  		direction(0b0010);
	  		direction(0b0110);
	  		direction(0b0100);
	  		direction(0b1100);
	  		direction(0b1000);
	  		direction(0b1001);
	  	}
	}
	else if (_direction == -1) {
	  	for (int i=0; i<_step; i++) {
	  		direction(0b1000);
	  		direction(0b1100);
	  		direction(0b0100);
	  		direction(0b0110);
	  		direction(0b0010);
	  		direction(0b0011);
	  		direction(0b0001);
	  		direction(0b1001);
	  	}
	}	
}


I2CMotorDriver Motor;

// End of File
#include "MotorControl.h"

#define LSense 0
#define FSense 1
#define startPin 9
#define EncoderMotorLeft	7
#define EncoderMotorRight	8

//Pin Definitions
#define pwmLeft			3
#define dirLeft			12
#define pwmRight		11
#define dirRight		13

//Calibration Variables
#define motorLeftSpeed	250
#define motorRightSpeed 215
#define motorLeftTurn	250
#define motorRightTurn	215
#define milliSecondsPerIn  130
#define milliPerDegree	5.65
#define BackDrive		50

int fixedPWM = 140;

int leftPWM =  0.8*fixedPWM;
int rightPWM = fixedPWM;
int timeout = 250;


void MotorControlClass::motor(String m, int pwm) {

	if (m == "LEFT") {

		if (pwm > 0) {
			digitalWrite(dirLeft, HIGH);
			analogWrite(pwmLeft, pwm);
		}
		else if (pwm < 0) {
			digitalWrite(dirLeft, LOW);
			analogWrite(pwmLeft, -pwm);
		}
		else {
			analogWrite(pwmLeft, 0);
		}
	}// end Left Motor
	if (m == "RIGHT") {
		if (pwm > 0) {
			digitalWrite(dirRight, HIGH);
			analogWrite(pwmRight, pwm);
		}
		else if (pwm < 0) {
			digitalWrite(dirRight, LOW);
			analogWrite(pwmRight, -pwm);
		}
		else {
			analogWrite(pwmRight, 0);
		}
	}// end Right motor
}

void MotorControlClass::correction() {
	float relError = errorLeft - errorRight;
	LSenseVal = analogRead(LSense);
	FSenseVal = analogRead(FSense);

	Serial.print("Left Sensor Val: ");
	Serial.println(LSenseVal);

	Serial.print("Front Sensor Val: ");
	Serial.println(FSenseVal);

	rightPWM = fixedPWM;

	float timer = millis();
	if (FSenseVal > FTol) {
		leftPWM = 0;
		rightPWM = 0;
		leftEncoderCount = countsDesired;
		rightEncoderCount = countsDesired;
		Serial.println("Front wall detected early. Terminating forward motion");
	}
	else if (LSenseVal > wallDist && timer - inittime > timeout) {
		ForwardStop();
		Right(20);
		inittime = millis();
		Forward(0.1);
		errorLeft = errorLeft + 2;
		errorRight = errorRight + 2;
		Serial.println("Too Close to Left wall. Correcting.");
	}
	else if (abs(relError) > 1 && timer - inittime > timeout) {
		rightPWM = fixedPWM;
		float correction = 10000 * relError / (timer-inittime);
		leftPWM = leftPWM + correction;
		inittime = millis();
		Serial.println("Left/Right wheel mismatch. Adjusting left wheel speed");
		Serial.print("Correction: ");
		Serial.println(correction);
	} 
}

void MotorControlClass::Forward(float node) {
	leftEncoderCount = 0;
	rightEncoderCount = 0;

	countsDesired = nodesToMM * node * 36 / (3.14 * 8.25)+2; //12 steps per rev, D = 8.25 cm

	errorLeft = countsDesired - leftEncoderCount;
	errorRight = countsDesired - rightEncoderCount;

	inittime = millis();

	while (errorLeft > 0 || errorRight > 0)
	{
		Serial.println("===============");
		correction();

		if (leftPWM > 255) {
			leftPWM = 255;
		}
		if (leftPWM < -255) {
			leftPWM = -255;
		}
		
		Serial.print("Left PWM: ");
		Serial.println(leftPWM);
		Serial.print("Right PWM: ");
		Serial.println(rightPWM);
		
		Serial.print("Left Error: ");
		Serial.println(errorLeft);

		Serial.print("Right Error: ");
		Serial.println(errorRight);

		motor("LEFT", leftPWM);
		motor("RIGHT", rightPWM);

		errorLeft = countsDesired - leftEncoderCount;
		errorRight = countsDesired - rightEncoderCount;
	}
	ForwardStop();
}

void MotorControlClass::Right(int angle) {
	unsigned long t;
	t = angle*milliPerDegree;

	motor("RIGHT", -motorRightTurn);
	motor("LEFT", motorLeftTurn);

	delay(t);

	RightStop();
}
void MotorControlClass::Left(int angle) {
	unsigned long t;
	t = angle*milliPerDegree;

	motor("LEFT", -motorLeftTurn);
	motor("RIGHT", motorRightTurn);
	delay(t);

	LeftStop();
}
void MotorControlClass::Rev(int t) {
	motor("LEFT", -motorLeftSpeed);
	motor("RIGHT", -motorRightSpeed);

	int delayT = milliSecondsPerIn*t;
	delay(delayT);

	RevStop();
}


void MotorControlClass::ForwardStop() {
	motor("LEFT", 0);
	motor("RIGHT", 0);

	motor("LEFT", -150);
	motor("RIGHT", -150);

	delay(BackDrive);

	motor("LEFT", 0);
	motor("RIGHT", 0);

}
void MotorControlClass::LeftStop() {
	motor("LEFT", 0);
	motor("RIGHT", 0);

	motor("LEFT", 150);
	motor("RIGHT", -150);

	delay(BackDrive);

	motor("LEFT", 0);
	motor("RIGHT", 0);

}
void MotorControlClass::RightStop() {
	motor("LEFT", 0);
	motor("RIGHT", 0);

	motor("LEFT", -150);
	motor("RIGHT", 150);

	delay(BackDrive);

	motor("LEFT", 0);
	motor("RIGHT", 0);

}
void MotorControlClass::RevStop() {
	motor("LEFT", 0);
	motor("RIGHT", 0);

	motor("LEFT", motorLeftSpeed*0.9);
	motor("RIGHT", motorRightSpeed);

	delay(BackDrive);

	motor("LEFT", 0);
	motor("RIGHT", 0);
}

String MotorControlClass::RandDir() {
	if (millis() % 2 == 1) {
		Left(100);
		return "left";
	}
	else {
		Right(100);
		return"right";
	}
}

MotorControlClass MC;
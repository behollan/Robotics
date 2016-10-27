#include "MotorControl.h"
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


void MotorControlClass::motor(String m, int pwm) {

	Serial.print(pwm);
	Serial.print(",");


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

void MotorControlClass::Forward(float speed) {
	float newPWMLeft = speed*motorLeftSpeed;
	float newPWMRight = speed*motorRightSpeed;
	motor("LEFT", newPWMLeft);
	motor("RIGHT", newPWMRight);
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
// 
// 
// 

#include "motorControl.h"
#define A        1
#define B        2
#define pwmA    3
#define dirA    12
#define pwmB    11
#define dirB    13
#define motorASpeed	150
#define motorBSpeed 150
#define motorATurn	255
#define motorBTurn	255
#define FORWARD  0
#define LEFT     1 
#define RIGHT   -1
#define milliSecondsPerCM  100
#define milliPerDegree	10
void MotorControlClass::init()
{


}
void MotorControlClass::motor(int m, int pwm) {
	Serial.print("entering motor");
	Serial.print(m);
	Serial.print("\t");
	Serial.println(pwm);

	if (m == A) {

		if (pwm > 0) {
			digitalWrite(dirA, HIGH);
			analogWrite(pwmA, pwm);
		}
		else if (pwm < 0) {
			digitalWrite(dirA, LOW);
			analogWrite(pwmA, -pwm);
		}
		else {
			analogWrite(pwmA, 0);
		}
	}// end motor A
	if (m == B) {
		if (pwm > 0) {
			digitalWrite(dirB, HIGH);
			analogWrite(pwmB, pwm);
		}
		else if (pwm < 0) {
			digitalWrite(dirB, LOW);
			analogWrite(pwmB, -pwm);
		}
		else {
			analogWrite(pwmB, 0);
		}
	}// end motor B
}
void MotorControlClass::Forward(int distance) {
	unsigned long t;

	t = distance * milliSecondsPerCM;
	motor(A, motorASpeed);
	motor(B, motorBSpeed);
	delay(t);
	motor(A, 0);
	motor(B, 0);
}
void MotorControlClass::Left(int angle) {
	unsigned long t;
	t = angle*milliPerDegree;

	motor(A, motorATurn);
	motor(B, -motorBTurn);
	delay(t);

	motor(A, 0);
	motor(B, 0);
}
void MotorControlClass::Right(int angle) {
	unsigned long t;
	t = angle*milliPerDegree;

	motor(A, -motorATurn);
	motor(B, motorBTurn);
	delay(t);

	motor(A, 0);
	motor(B, 0);
}

MotorControlClass MotorControl;


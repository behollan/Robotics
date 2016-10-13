#include "PinChangeInt.h"
#include "MotorControl.h"


// Encoder pin definitions and variables
#define  EncoderMotorLeft		8
#define  EncoderMotorRight		7
#define  GAINleftMotor			20
#define  GAINrightMotor			20
#define  LEFTmotor				1
#define  RIGHTmotor				2

//Motor Shield Pin Definitions
#define  motorRightPWM			3
#define  motorRightDirection	12
#define  motorLeftPWM			11
#define  motorLeftDirection		13

#define GainAdjust		1


///////////////////////////////////
volatile unsigned int leftEncoderCount = 0;
volatile unsigned int rightEncoderCount = 0;

void setup() {
	// set stuff up
	Serial.begin(9600);
	pinMode(EncoderMotorLeft, INPUT_PULLUP);     //set the pin to input
	attachPinChangeInterrupt(EncoderMotorLeft, indexLeftEncoderCount, FALLING);

	pinMode(EncoderMotorRight, INPUT_PULLUP);     //set the pin to input
	attachPinChangeInterrupt(EncoderMotorRight, indexRightEncoderCount, FALLING);
}

void loop()
{
	forward(30);
	MC.ForwardStop();
	delay(1000);
	left(90);
	MC.LeftStop();
	delay(1000);
	right(90);
	MC.RightStop();
	exit(0);

}

void forward(float distance)
{
	int countsDesired, countsCompleted, CMDleft, CMDright, errorLeft, errorRight;
	int countLeft = 0;
	int countRight = 0;
	
	countsDesired = distance*6/(3.14*8);
	leftEncoderCount = 0;
	rightEncoderCount = 0;
	// we make the errors non-zero so our first test gets us into the loop
	errorLeft = countsDesired - leftEncoderCount;
	errorRight = countsDesired - rightEncoderCount;
	while (errorLeft || errorRight)
	{
		CMDleft = computeCommand(GAINleftMotor, errorLeft);
		CMDright = computeCommand(GAINrightMotor, errorRight);
		if (errorLeft - errorRight > 2) {
			CMDleft = GainAdjust*CMDleft;
		}
		else if (errorLeft-errorRight<-2){
			CMDright = GainAdjust*CMDright;
		}
		Serial.print("Left Error: ");
		Serial.println(errorLeft);
		Serial.print("Right Error: ");
		Serial.println(errorRight);

		MC.motor("LEFT", CMDleft);
		MC.motor("RIGHT", CMDright);
		countLeft = leftEncoderCount;
		countRight = rightEncoderCount;
		errorLeft = countsDesired - countLeft;
		errorRight = countsDesired - countRight;
	}

}
void left(float angle) {
	int countsDesired, countsCompleted, CMDleft, CMDright, errorLeft, errorRight;
	int countLeft = 0;
	int countRight = 0;

	countsDesired = angle/360*2*7.5*6/8;
	leftEncoderCount = 0;
	rightEncoderCount = 0;
	// we make the errors non-zero so our first test gets us into the loop
	errorLeft = countsDesired - leftEncoderCount;
	errorRight = countsDesired - rightEncoderCount;
	while (errorLeft || errorRight)
	{
		CMDleft = computeCommand(GAINleftMotor, errorLeft);
		CMDright = computeCommand(GAINrightMotor, errorRight);
		if (errorLeft - errorRight < 2) {
			CMDleft = GainAdjust*CMDleft;
		}
		else if (errorLeft - errorRight<-2) {
			CMDright = GainAdjust*CMDright;
		}
		MC.motor("LEFT", -CMDleft);
		MC.motor("RIGHT", CMDright);
		countLeft = leftEncoderCount;
		countRight = rightEncoderCount;
		errorLeft = countsDesired - countLeft;
		errorRight = countsDesired - countRight;
	}

}
void right(float angle) {
	int countsDesired, countsCompleted, CMDleft, CMDright, errorLeft, errorRight;
	int countLeft = 0;
	int countRight = 0;

	countsDesired = angle / 360 * 2 * 7.5 * 6 / 8;
	leftEncoderCount = 0;
	rightEncoderCount = 0;
	errorLeft = countsDesired - leftEncoderCount;
	errorRight = countsDesired - rightEncoderCount;
	while (errorLeft || errorRight)
	{
		CMDleft = computeCommand(GAINleftMotor, errorLeft);
		CMDright = computeCommand(GAINrightMotor, errorRight);
		if (errorLeft - errorRight < 2) {
			CMDleft = GainAdjust*CMDleft;
		}
		else if (errorLeft - errorRight<-2) {
			CMDright = GainAdjust*CMDright;
		}
		Serial.print("Left motor error: ");
		Serial.println(errorLeft);
		Serial.print("RIGHT motor error: ");
		Serial.println(errorRight);
		MC.motor("LEFT", CMDleft);
		MC.motor("RIGHT", -CMDright);
		countLeft = leftEncoderCount;
		countRight = rightEncoderCount;
		errorLeft = countsDesired - countLeft;
		errorRight = countsDesired - countRight;
	}

}

int computeCommand(int gain, int error)
{
		int cmdDir = 0;
		cmdDir = (gain * error);
		if (cmdDir  > 255)
			cmdDir = 255;
		else if (-128 < cmdDir < 128) {
			cmdDir = 0;
		}
		else if (cmdDir < -255) {
			cmdDir = -255;
		}
		return(cmdDir);
	
}


void indexLeftEncoderCount()
{
	leftEncoderCount++;
}
//////////////////////////////////////////////////////////
void indexRightEncoderCount()
{
	rightEncoderCount++;
}


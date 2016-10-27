#include "PinChangeInt.h"
#include "MotorControl.h"

/*		Pin Definitions			*/	
// Encoder
#define EncoderMotorLeft		7
#define EncoderMotorRight		8
// Bumpers
#define LeftBumpPin				4
#define RightBumpPin			5
#define startPin				9

//Motor Shield
#define	motorRightPWM			3
#define motorRightDirection		12
#define motorLeftPWM			11
#define motorLeftDirection		13

/*		Calibration values		*/
#define	GAINleftMotor			1
#define	GAINrightMotor			1
#define	GainAdjust				1

/*		Variable inits			*/
volatile unsigned int leftEncoderCount = 0;
volatile unsigned int rightEncoderCount = 0;
int countsDesired, countsCompleted, CMDleft, CMDright, errorLeft, errorRight, countLeft, countRight;
int inittime = millis();
int rate = 50;
int correctedPWMLeft = 190;
int nodesToMM = 30;

void setup() {
	pinMode(startPin, INPUT_PULLUP);

	Serial.begin(115200);
	pinMode(EncoderMotorLeft, INPUT_PULLUP);     //set the pin to input pullup
	attachPinChangeInterrupt(EncoderMotorLeft, indexLeftEncoderCount, CHANGE);

	pinMode(EncoderMotorRight, INPUT_PULLUP);     //set the pin to input pullup
	attachPinChangeInterrupt(EncoderMotorRight, indexRightEncoderCount, CHANGE);
}

String directions[]=
{
	"F","R","F","R","F","L","F","R","F","R","F","L","F","R","F","R","F","L","F","L","F","L","F","L","F","R","F"
};
int vals[]=
{
	1,90,2,90,1,90,2,90,1,90,2,90,1,90,1,90,1,90,1,90,2,90,3,90,1,90,1
};
void loop()
{
	while (digitalRead(startPin) == 0) {
		int i = 0;
		while (i < 26) {
			move(directions[i], vals[i]);
			if (directions[i] == "F") {
				MC.ForwardStop();
			}
			else if (directions[i] == "L") {
				MC.LeftStop();
			}
			else if (directions[i] == "R") {
				MC.RightStop();
			}
			i++;
			delay(1000);
		}
	}
}

void move(String dir, int val){ //input val is cm for forward travel and degrees for L/R
	countLeft = 0;
	countRight = 0;

	if (dir == "F") {
		countsDesired = nodesToMM*val * 36 / (3.14 * 8.25); //12 steps per rev, D = 8.25 cm
	}
	else if (dir == "L" || dir == "R") {
		countsDesired = (7.5/41.25*val)-1; //12 steps per rev r_chasis=7.5cm, r_wheel=4.125cm
	}
	leftEncoderCount = 0;
	rightEncoderCount = 0;

	errorLeft = countsDesired - leftEncoderCount;
	errorRight = countsDesired - rightEncoderCount;

	while (errorLeft > 0 || errorRight > 0)
	{
		correctedPWMLeft = computeCorrection();
		CMDleft = computeCommand("L", errorLeft, correctedPWMLeft);
		CMDright = computeCommand("R", errorRight, 128);


		if (dir == "F") {
			MC.motor("LEFT", CMDleft);
			MC.motor("RIGHT", CMDright);
		}
		else if (dir == "L") {
			MC.motor("LEFT", -CMDleft);
			MC.motor("RIGHT", CMDright);
		}
		else if (dir == "R") {
			MC.motor("LEFT", CMDleft);
			MC.motor("RIGHT", -CMDright);
		}

		Serial.print(errorLeft);
		Serial.print(",");
		Serial.println(errorRight);

		countLeft = leftEncoderCount;
		countRight = rightEncoderCount;
		errorLeft = countsDesired - countLeft;
		errorRight = countsDesired - countRight;
	}

		CMDleft = 175;
		correctedPWMLeft = 175;
}

int computeCorrection() {
	int relError = errorLeft - errorRight;
	int timer = millis()-inittime;
	if (abs(relError) > 1 && timer > rate) {
		correctedPWMLeft = correctedPWMLeft + 1000 * relError / timer;
		/*Serial.print("Timer: ");
		Serial.println(timer);*/
		inittime = millis();
	}
	if (correctedPWMLeft > 255) {
		correctedPWMLeft = 255;
	}
	else if (correctedPWMLeft < 0) {
		correctedPWMLeft = 0;
	}
	return correctedPWMLeft;
}

int computeCommand(String dir, int error, int PWM)
{
	int cmdPWM;
	if (error > 5) {
		cmdPWM = PWM;
	}
	else if (error <= 5) {
		if (dir == "L") {
			cmdPWM = PWM*GAINleftMotor;
		}
		else if (dir == "R") {
			cmdPWM = PWM*GAINrightMotor;
		}
	}
	else {

	}
	if (cmdPWM > 255) {
		cmdPWM = 255;
	}
	else if (cmdPWM < -255) {
		cmdPWM = -255;
	}
	else if (cmdPWM > 0) {
		cmdPWM = map(cmdPWM, 0, 255, 100, 255);
	}
	else if (cmdPWM < 0) {
		cmdPWM = map(cmdPWM, -255, 0, -255, -100);
	}
	else {
		cmdPWM = PWM;
	}
	return cmdPWM;
}

/*Interupt Service Routines*/
void indexLeftEncoderCount()
{
	leftEncoderCount++;
}
void indexRightEncoderCount()
{
	rightEncoderCount++;
}


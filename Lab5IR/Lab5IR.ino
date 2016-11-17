#include "MotorControl.h"
#include "PinChangeInt.h"

#define LSense 0
#define FSense 1
#define startPin 9
#define EncoderMotorLeft	7
#define EncoderMotorRight	8

//     Variables
volatile unsigned int leftEncoderCount = 0;
volatile unsigned int rightEncoderCount = 0;
int countsDesired, countsCompleted, CMDleft, CMDright, errorLeft, errorRight, countLeft, countRight;
int inittime = millis();
int rate = 50;
int correctedPWMLeft = 190;
int nodesToMM = 30;

int LSenseVal, FSenseVal;
int LTol = 162; //15 cm
int FTol = 278; //8 cm
int wallDist = 450; //5cm

#define startNode 21 //Start Location
#define endNode 24 //End location

int buttonCount;
String cardDir[50] = { "N","E","S","W", "N","E","S","W", "N","E","S","W", "N","E","S","W", "N","E","S","W", "N","E","S","W"};
int curDir = 13; //Current direction tracker, start facing east
int currentNode = startNode;

String directions[50] = {};


void setup()
{
	pinMode(startPin, INPUT_PULLUP);
	Serial.begin(115200);

	pinMode(EncoderMotorLeft, INPUT_PULLUP);     //set the pin to input pullup
	attachPinChangeInterrupt(EncoderMotorLeft, indexLeftEncoderCount, CHANGE);

	pinMode(EncoderMotorRight, INPUT_PULLUP);     //set the pin to input pullup
	attachPinChangeInterrupt(EncoderMotorRight, indexRightEncoderCount, CHANGE);
}


void loop()
{
	int buttonState = digitalRead(startPin);
	if (buttonState == 0) {
		buttonCount++;
		Serial.print("Button counter: ");
		Serial.println(buttonCount);
		if (buttonCount == 1) {
			exploreMaze();
	}
		if (buttonCount >= 2) {
			runMaze();
		}
	}
}

void exploreMaze() {
	while (currentNode != endNode) {
		FSenseVal = analogRead(FSense);
		LSenseVal = analogRead(LSense);

		Serial.print("Current direction: ");
		Serial.println(cardDir[curDir]);
		Serial.print("Current node: ");
		Serial.println(currentNode);
		if (FSenseVal < FTol) {
			driveDir("F");
			nodeNumber(cardDir[curDir]);
			computeNewCardDir("F");
		}
		else if (LSenseVal < LTol) {
			driveDir("L");
			nodeNumber(cardDir[curDir]);
			computeNewCardDir("L");
		}
		else {
			driveDir("R");
			nodeNumber(cardDir[curDir]);
			computeNewCardDir("R");
		}
	}
	Serial.println("Maze Solve Complete!");
}
void runMaze() {

}

void computeNewCardDir(String dir) {
	if (dir == "F") {
	}
	if (dir == "L") {
		curDir -= 1;
	}
	if (dir == "R") {
		curDir += 1;
	}
}

void nodeNumber(String cardDir) {
	if (cardDir == "N") {
		currentNode -= 5;
	}
	else if (cardDir == "E") {
		currentNode += 1;
	}
	else if (cardDir == "S") {
		currentNode += 5;
	}
	else if (cardDir == "W") {
		currentNode -= 1;
	}
}

void driveDir(String dir) {
	if (dir == "F") {
		MC.Forward(1);
	}
	else if (dir == "L") {
		MC.Left(90);
		MC.Forward(1);
	}
	else if (dir == "R") {
		MC.Right(90);
		MC.Forward(1);
	}
}

void indexLeftEncoderCount()
{
	leftEncoderCount++;
}
void indexRightEncoderCount()
{
	rightEncoderCount++;
}

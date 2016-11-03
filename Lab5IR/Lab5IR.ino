#define LSense 0
#define FSense 1
#define startPin 9

int LSenseVal, FSenseVal;
int LTol = 162; //15 cm
int FTol = 278; //8 cm
int wallDist = 372; //5cm
int correctionAngle = 20;

#include "MotorControl.h"


void setup()
{
	pinMode(startPin, INPUT_PULLUP);
	Serial.begin(9600);
}

void loop()
{
		LSenseVal = analogRead(LSense);
		FSenseVal = analogRead(FSense);
		Serial.print("Left Sensor: ");
		Serial.println(LSenseVal);
		Serial.print("RightSensor: ");
		Serial.println(FSenseVal);
		/*	LSenseVal = map(LSenseVal, 0, 100, 0, 1023);
			FSenseVal = map(FSenseVal, 0, 100, 0, 1023);
			*/
		if (LSenseVal < LTol) {
			delay(300);
			MC.ForwardStop();
			MC.Left(90);
			MC.LeftStop();
			MC.Forward(150);
			delay(1000);

		}
		else if (FSenseVal > FTol) {
			MC.ForwardStop();
			MC.Right(90);
			MC.RightStop();
		}
		else if (LSenseVal > wallDist) {
			MC.ForwardStop();
			MC.Right(correctionAngle);
			MC.RightStop();
			MC.Forward(150);
			delay(100);
		}

		else {
			MC.Forward(150);
		}


}


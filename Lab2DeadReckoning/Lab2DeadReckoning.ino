/* Dead Reckoning
* 20130206
* Program to get the ardbot to drive a predefined path
* jsteele
*/
// Preprocessor Definitions
#include "motorControl.h" //Custom library built for motor control. See the library definition in the source code for usage

#define pushButton 7
#define FORWARD  0
#define LEFT     1 
#define RIGHT   -1
#define pwmA    3
#define dirA    12
#define pwmB    11
#define dirB    13
// the following converts centimeters into milliseconds as long datatype


// the itemized list of moves for the robot as a 2D array
// the first element is the commanded direction, the second element is the distance (cm) or angle (deg)
int moves2[][2] = {
	{ FORWARD,198 }, // leg 1
	{ RIGHT, 90 },
	{ FORWARD,62.5 }, // leg 2
	{ RIGHT,90 },
	{ FORWARD,58.5 }, // leg 3
	{ LEFT, 90 },
	{ FORWARD,108}, // leg 4
	{ RIGHT,90 },
	{ FORWARD,86 }, // leg 5
	{ RIGHT,90 },
	{ FORWARD,53 }, // leg 6
	{ LEFT,90 },
	{ FORWARD,49.5 }, // leg 7
	{ RIGHT,90 },
	{ FORWARD,115 }, // leg 8
};
void setup() {
	// set up the motor drive ports
	pinMode(pwmA, OUTPUT);
	pinMode(dirA, OUTPUT);
	pinMode(pwmB, OUTPUT);
	pinMode(dirB, OUTPUT);
	// make the pushbutton's pin an input:
	pinMode(pushButton, INPUT);
	// initialize serial communication at 9600 bits per second:
	Serial.begin(9600);

	Serial.println("===========================================");
	Serial.println("Beginning Dead Reckoning Robot");
	Serial.println("===========================================");
}

void loop() {
	int i;
	long time;
	bool buttonState = digitalRead(pushButton);
	while (buttonState) { //Assumes pull down circuit for button state
		for (i = 0; i < 15; i++) {
			Serial.print("Direction\t");
			if (moves2[i][0] == FORWARD) {
				Serial.print("Forward \t");
				int Dist = moves2[i][1];
				Serial.print(Dist);
				Serial.println(" cm");
				MotorControl.Forward(moves2[i][1]);
			}
			else if (moves2[i][0] == LEFT) {
				Serial.print("Left \t");
				int angle = moves2[i][1];
				Serial.print(angle);
				Serial.println(" deg.");
				MotorControl.Left(moves2[i][1]);
			}
			else if (moves2[i][0] == RIGHT) {
				Serial.print("Right \t");
				int angle = moves2[i][1];
				Serial.print(angle);
				Serial.println(" deg.");
				MotorControl.Right(moves2[i][1]);
			}
			else {
				Serial.print("Error - Bad Direction ");
			}
			Serial.print(15 - i);
			Serial.println(" segments of the course remaining.");
			Serial.println();
			Serial.println("===========================================");
			delay(5000);
		}
		Serial.println("Course completed.");
		delay(1000);
		exit(i);
	}
	delay(10);
}
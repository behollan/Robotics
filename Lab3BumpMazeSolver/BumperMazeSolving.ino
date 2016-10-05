#include "motorControl.h"
#define FORWARD  0
#define LEFT     1
#define RIGHT   -1
#define A        1
#define B        2

#define LEFTBumper  4
#define RIGHTBumper 5

#define revDist		2.5 //Reverse distance in inches
#define timeout		300

#define cautionSpeed	0.50
#define driveSpeed		0.9

int timer, initTime;
String bumpState;

void setup() {
 	pinMode(LEFTBumper, INPUT_PULLUP);
	pinMode(RIGHTBumper, INPUT_PULLUP);
	delay(1000);
}

void loop() {
	bumpState = bumpCheck();
	Serial.print("Bump State: ");
	Serial.println(bumpState);
	if (bumpState == "none") {
		MC.Forward(driveSpeed);
	}
	else if (bumpState == "left") {
		MC.Rev(revDist);
		delay(500);
		MC.Right(30);
		delay(500);
	}
	else if (bumpState == "right") {
		MC.Rev(revDist);
		delay(500);
		MC.Left(30);
		delay(500);
	}
	else if (bumpState == "forward") { //Hit an initial front bump
		MC.Rev(revDist); //Reverse
		String newDir = MC.RandDir(); //Turn a random direction and store it
		MC.Forward(cautionSpeed); //Start off cautiously on the new path

		initTime = millis();
		timer = 0;
		while (timer < timeout*2) {
			bumpState = bumpCheck(); //Recheck bump state (Corner Condition)
			if (bumpState == "forward") {
				MC.Rev(revDist);
				MC.Left(170); //Turn the Robot 180 degrees
				Serial.println("U-Turn!");
				MC.Forward(cautionSpeed); //Drive forward cautiously
				
				int initTime2 = millis();
				int timer2 = 0;
				while (timer2 < timeout*2) {
					bumpState = bumpCheck(); //Recheck bump state
					if (bumpState == "forward") { //Trap condition
						if (newDir == "left") { //Is the way back out to the right?
							MC.Right(100);
						}
						else { //Is the way back out to the left?
							MC.Left(100);
						}
					}
					timer2 = millis() - initTime2;
				}
			}
			timer = millis() - initTime;
		}
		timer = 0;
	}
	else {
		Serial.println("INVALID BUMP STATE");
	}
}//End Loop

String bumpCheck() {
	bumpState = "none";

	int l = digitalRead(LEFTBumper);
	int r = digitalRead(RIGHTBumper);
	if (l == 0) {
		MC.ForwardStop();
		initTime = millis();
		timer = 0;
		while (timer < timeout) {
			r = digitalRead(RIGHTBumper);
			if (r == 0) {
				break;
			}
			else {
				timer =millis()-initTime;
			}
		}
		timer = 0;
		if (r == 0) {
			bumpState = "forward";
		}
		else {
			MC.Forward(0.50); //Proceed forward cautiously

			initTime = millis(); //Init a timer
			timer = 0;
			while (timer < timeout) {
				r = digitalRead(RIGHTBumper); //read right bumper
				if (r == 0) {
					MC.ForwardStop();
					bumpState = "forward"; //both bumpers hit. Forward bump
					break;
				}
				else {
					bumpState = "left"; //Only a left hit
				}
				timer = millis() - initTime; //clock timer
			}
			MC.ForwardStop();
			timer = 0;

		}
	}
	else if (r == 0) {
		MC.ForwardStop();
		if (l == 0) {
			initTime = millis();
			timer = 0;
			while (timer < timeout) {
				r = digitalRead(RIGHTBumper);
				if (r == 0) {
					break;
				}
				else {
					timer = millis() - initTime;
				}
			}
			timer = 0;
		}
		else {
			MC.Forward(0.50); //Proceed forward cautiously

			initTime = millis(); //Init a timer
			timer = 0;
			
			while (timer < timeout) {
				l = digitalRead(LEFTBumper); //read left bumper
				if (l == 0) {
					MC.ForwardStop();
					bumpState = "forward"; //both bumpers hit. Forward bump
					delay(timeout);
					break;
				}
				else {
					bumpState = "right"; //Only a right hit
				}
				timer = millis() - initTime; //clock timer
			}
			MC.ForwardStop();
			timer = 0;
		}
	}
	return bumpState;
}
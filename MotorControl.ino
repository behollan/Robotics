
/* Simple Motor Drive With Button Control & Timing
*  jsteele 20160116
*/

// Variable setup and definition
#define PUSHBUTTON   2
#define MOTOR_A_PWM  3
#define MOTOR_A_DIR  12
#define LEDPIN       13
boolean buttonPushed = false;
int buttonState = 0;
int onoffcycles = 0;
long starttime, stoptime, deltatime;


// the setup routine runs once when you press reset:
void setup() {
	// make the pushbutton's pin an input:
	pinMode(PUSHBUTTON, INPUT);
	pinMode(LEDPIN, OUTPUT);
	//make the motor ports outputs
	pinMode(MOTOR_A_PWM, OUTPUT);
	pinMode(MOTOR_A_DIR, OUTPUT);
	digitalWrite(MOTOR_A_DIR, HIGH);
	// initialize serial communication at 9600 bits per second:
	Serial.begin(9600);  // set the serial comm to 9600 BAUD
}


// the loop routine runs over and over again forever:
void loop() {
	// read the input pin:
	buttonState = digitalRead(PUSHBUTTON); // read the state of the button
										   // is the button is pushed?

	if (buttonState == 1) {  // assumes a pull-down switch circuit
		analogWrite(MOTOR_A_PWM, 255); // turns the motor On 100% duty cycle
		if (buttonPushed == false) {
			// buttonPushed is false if this is our first time in the loop
			buttonPushed = true;  // set state to true
			starttime = millis(); // grab the start time
			Serial.println(starttime);  // print the start time to the serial port
			digitalWrite(LEDPIN, HIGH);  // Turn LED on - just for fun
		}
	}

	else {
		analogWrite(MOTOR_A_PWM, 0); // Turn off motor; leave it off
		digitalWrite(LEDPIN, LOW);  // Turn off LED
		if (buttonPushed == true) { // we are coming out of the "ON" state
			stoptime = millis();
			Serial.println(stoptime);
			buttonPushed = false;
			deltatime = stoptime - starttime;
			onoffcycles++;
			Serial.print(onoffcycles);
			Serial.print("\t");
			Serial.print(stoptime);
			Serial.print(" ");
			Serial.print(starttime);
			Serial.print("\t");
			Serial.println(deltatime);
		}
	}
	// print out the state of the button:
	//Serial.println(buttonState);
	delay(10);        // delay in between reads for stability
}

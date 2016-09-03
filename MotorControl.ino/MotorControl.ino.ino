// Variable setup and definition
#define PUSHBUTTON   4
#define MOTOR_A_PWM  3
#define MOTOR_A_DIR  12
#define LEDPIN       13
#define DUTY_CYCLE   255
boolean buttonPushed = false;
int buttonState = 0;
int onoffcycles = 0;
long starttime, stoptime, deltatime;

void setup() {
  pinMode(PUSHBUTTON, INPUT);
  pinMode(LEDPIN, OUTPUT);
  pinMode(MOTOR_A_PWM, OUTPUT);
  pinMode(MOTOR_A_DIR, OUTPUT);
  digitalWrite(MOTOR_A_DIR, HIGH);
  
  Serial.begin(9600); // set the serial comm to 9600 BAUD

//Print a header line
  Serial.print("Cycle #");
  Serial.print("\t");
  Serial.print("Start");
  Serial.print("\t");
  Serial.print("Stop");
  Serial.print("\t");
  Serial.println("Time Delta");
}

void loop() {
  buttonState = digitalRead(PUSHBUTTON); // read the state of the button
  
  if (buttonState == 1) { // assumes a pull-down switch circuit
    analogWrite(MOTOR_A_PWM, DUTY_CYCLE);
    if (buttonPushed == false) {
      // buttonPushed is false if this is our first time in the loop
      buttonPushed = true;  // set state to true
      starttime = millis(); // grab the start time
      digitalWrite(LEDPIN, HIGH);   // Turn LED on - just for fun
    }
  }
  else {
    analogWrite(MOTOR_A_PWM, 0);  // Turn off motor; leave it off
    digitalWrite(LEDPIN, LOW);    // Turn off LED
    if (buttonPushed == true) {   // we are coming out of the "ON" state
      stoptime = millis();  
      buttonPushed = false;       //Reset button state
      onoffcycles++;
      printData();
    }
  }
  delay(10);        // delay in between reads for stability
}

void printData(){
      deltatime = stoptime - starttime;
      Serial.print(onoffcycles);
      Serial.print("\t");
      Serial.print(starttime);
      Serial.print("\t");
      Serial.print(stoptime);
      Serial.print("\t");
      Serial.println(deltatime); 
}


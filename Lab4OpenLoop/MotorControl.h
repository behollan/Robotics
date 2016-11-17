// motorControl.h

#ifndef _MOTORCONTROL_h
#define _MOTORCONTROL_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class MotorControlClass
{
protected:

	void correction();

public:
	void Forward(float speed);
	void Left(int angle);
	void Right(int angle);
	void ForwardStop();
	void RevStop();
	void LeftStop();
	void RightStop();
	void Rev(int t);
	String RandDir();
	void motor(String m, int pwm);
};

extern MotorControlClass MC;

extern volatile unsigned int leftEncoderCount, rightEncoderCount;
extern int countsDesired, countsCompleted, CMDleft, CMDright, errorLeft;
extern int errorRight, countLeft, countRight, inittime, rate, correctedPWMLeft, nodesToMM;

extern int LSenseVal, FSenseVal, LTol, FTol, wallDist, correctionAngle;
#endif
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


public:
	void init();
	void motor(int m, int pwm);
	void Forward(int distance);
	void Left(int angle);
	void Right(int angle);
};

extern MotorControlClass MotorControl;

#endif


#ifndef pirSensor_h
#define pirSensor_h

#include <Arduino.h>

class pirSensor{
public:
	pirSensor(int pin);
	void initializePir();
	int currentStatus();
	bool isActive();


private:
	int pirPin;

};

#endif
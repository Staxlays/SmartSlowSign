#ifndef hcsr04_h
#define hcsr04_h

#include <Arduino.h>

class hcsr04{
public:
	hcsr04(int e, int t);
	void initializeSonar();
	float triggerSonar();
	float calculateSpeed(hcsr04&A, hcsr04&B);

	float distance;
	long echoTime;
	long pulseTime;

	float speed;


private:
	int echoPin;
	int triggerPin;
};

#endif
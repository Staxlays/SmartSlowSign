#ifndef hcsr04_h
#define hcsr04_h

#include <Arduino.h>

class hcsr04{
public:
	hcsr04(int e, int t);
	void initializeSonar();
	void triggerSonar();
	int currentStatus();
	float readDistance();
	//float calculateSpeed(float distance, float prevDistance, long echoTime, long prevEchoTime);
	float calculateSpeed(hcsr04&A, hcsr04&B);

	float distance;
	float prevDistance;
	long echoTime;
	long prevEchoTime;
	long triggerTime;

	float deltaDistance;
	float speed;
	long deltaTime;


private:
	int echoPin;
	int triggerPin;

};

#endif
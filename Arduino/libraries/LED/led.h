#ifndef LED_h
#define LED_h

#include <Arduino.h>

class LED{
public:
	LED(int pin);
	void initializeLED();
	void turnOnLED();
	void turnOffLED();
	void blinkLED();
	void flashingLED();

private:
	int LEDPin;
};

#endif
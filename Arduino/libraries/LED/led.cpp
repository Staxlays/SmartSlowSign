#include "LED.h"

LED::LED(int pin){
	LEDPin = pin;
}

//Set the pin for the LED
void LED::initializeLED(){
	pinMode(LEDPin, OUTPUT);
}

//turn on the LED
void LED::turnOnLED(){
	digitalWrite(LEDPin, HIGH);
}

//turn off the LED
void LED::turnOffLED(){
	digitalWrite(LEDPin, LOW);
}

//Makes the LED do a quick on and off
void LED::blinkLED(){
	//ensure the LED is in an off state
	digitalWrite(LEDPin, LOW);
	//Blink the LED
	digitalWrite(LEDPin, HIGH);
	delay(50);
	digitalWrite(LEDPin, LOW);
}

//Flashes the led for three seconds
void LED::flashingLED(){
	//ensure the LED is in an off state
	digitalWrite(LEDPin, LOW);
	//set the current time
	int x = millis();
	//for three seconds, flash the LED on and off
	for(x; x > millis() - 3000;){
		digitalWrite(LEDPin, HIGH);
		delay(150);
		digitalWrite(LEDPin, LOW);
		delay(100);
	}
	//ensure the LED is off after flashing
	digitalWrite(LEDPin, LOW);
}
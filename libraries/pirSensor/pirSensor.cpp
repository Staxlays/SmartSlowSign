#include "pirSensor.h"

pirSensor::pirSensor(int pin){
	pirPin = pin;
}

void pirSensor::initializePir(){
	pinMode(pirPin, INPUT_PULLUP);
}

int pirSensor::currentStatus(){
	return digitalRead(pirPin);
}

bool pirSensor::isActive(){
	if(currentStatus() == 1){
		return true;
	}else{
		return false;
	}
}
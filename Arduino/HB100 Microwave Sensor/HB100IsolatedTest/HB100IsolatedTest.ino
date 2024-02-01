#define microwavePin 35

int objectDetected;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial);

  pinMode(35, INPUT);
  adcAttachPin(microwavePin);
}

void loop() {
  // put your main code here, to run repeatedly:

  //Check if something has been detected moving at an acceptable speed (ideally 15mph - 35mph)
  if(analogRead(microwavePin) > 470 && analogRead(microwavePin) < 1255){
    //Debug statement for viewing sensor reading
    //Serial.println(float(analogRead(microwavePin)));
    
    //Take note of the time when it is first discovered
    objectDetected = millis();



  }
  /*if(analogRead(microwavePin) != 4095){
    Serial.println(analogRead(microwavePin));
  }*/
}

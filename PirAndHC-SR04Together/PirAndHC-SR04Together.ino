/*
This program is an experiment to see if I can get one sensor to activate another one.
The goal is to have a PIR sensor be tripped which leads to a sonar sensor to take a distance measurement.
If the new distance recorded varies from the previous one, calculate the speed.
*/

//defining the trigger and echo pins for the proximity sensor
#define trig 12
#define echo 14

//define pin for PIR sensor
#define pir 27

//define trigger time for proximity sensor
#define trigdur .01

//set up placeholders for proximity sensor calculations
long timeTrigPrev = millis();
//long timeTrigB = millis();
long timeTrig = millis();
long timeEchoPrev = millis();
//long timeEchoB = millis();
long timeEcho = millis();
float distance = 0;
float distancePrev = 0;
//long distanceB = 0;
float speed = 0;
//Change in distance between two measurements
float deltaDistance = 0;
//Change in time between measurements
long deltaTime = 0;

/*Want to add a timer to check how long it has been since the last measurement recorded.
If it has been long enough, forget the previous recording.
Instead of using a new timer variable, thought I may be able to implement this using the existing 
timeEcho and timeEchoPrev variables.

TRY TO IMPLEMENT IN YOUR IF ELSE STATEMENT BY COMPARING TIMEECHOPREV AND TIMEECHO AS YOUR CONDITIONALS
*/
//Interval Timer
//long intervalTime = 0;

void setup() {
  Serial.begin(115200);
  //establish pinMode for Proximity and PIR sensors
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT_PULLUP);
  pinMode(pir, INPUT_PULLUP);

  //set interrupt for PIR sensor, executed whenever something is waved in front of it
  //attachInterrupt(digitalPinToInterrupt(pir), activateSonar, RISING);
  //set interrupt for echo pin, executed whenever the trigger signal is sent and an echo is received
  //attachInterrupt(digitalPinToInterrupt(echo), readDistance, FALLING);

}

void loop() {
  //Serial.println("PIR: " + String(digitalRead(pir)) + " Sonar: " + String(digitalRead(echo)));
  //wait for PIR pin to rise
  if(digitalRead(pir) == 1){
    activateSonar();
    if(digitalRead(echo) == 1){
      while(digitalRead(echo) == 1){
        //Serial.println("Waiting for the FALL");
      }
      readDistance();
      if((timeEchoPrev >= timeEcho - 7000) && (distancePrev != distance) && (distancePrev != 0)){
        if(distancePrev >= distance){
          deltaDistance = distancePrev - distance;
        }else{
          deltaDistance = distance - distancePrev;
        }
        deltaTime = (timeEcho - timeEchoPrev) / 1000;
        Serial.println("deltaDistance: " + String(deltaDistance) + " deltaTime: " + String(deltaTime));
        speed = deltaDistance / float(deltaTime);
        //speed = (abs(distancePrev - distance) / (abs(timeEchoPrev - timeEcho) / 1000));
        Serial.println("distancePrev : " + String(distancePrev) + " distance: " + String(distance) + " timeEchoPrev: " + String(timeEchoPrev) + " timeEcho: " + String(timeEcho));
        Serial.println("Speed is: " + String(speed) + "in/S");
        timeEchoPrev = timeEcho;
        distancePrev = distance;
        delay(2000);
      }else{
        Serial.println("Distance is: " + String(distance));
        timeEchoPrev = timeEcho;
        distancePrev = distance;
        delay(2000);
      }
    }
  }
 
 
 
 
 
  /*
  Control structure for determining what to print. 

  Still running into issues with getting an accurate speed reading due to
    uncertainty regarding when to update previous measurements. Should
    it be done in the interrupt functions? Should it only be updated when speed
    is calculated? 
  Also hard to tell without a proper testing environment
    (need a controlled moving object to double check recorded values. Maybe I could
    put together a track and motor to make something move at a set speed?)
  */

  //Using this loop to prevent repeated outputs before having the PIR sensor activate again
 /* while(digitalRead(pir) == LOW){
  }

  if((distancePrev != 0) && (distancePrev != distance) && (timeEchoPrev > timeEcho - 5000)) {
    //distancePrev = distance;
    //timeTrigPrev = timeTrig;
    //timeEchoPrev = timeEcho;
    Serial.println("Distance is: " + String(distance) + "in");
    //sanity delay
    //delay(3000);
    Serial.println("111111111");
    
  }else if((distancePrev != 0) && (distancePrev != distance) && (timeEchoPrev < timeEcho - 5000)) {
    //print timing from previous measurement and current measurement
    Serial.println("Time Trig: " + String(timeTrig) + " Time Echo: " + String(timeEcho));
    Serial.println("Previous Trig: " + String(timeTrigPrev) + " Previous Echo: " + String(timeEchoPrev));
  
    //print previous distance and times for debugging
    Serial.println("Previous distance was: " + String(distancePrev) + "in Current distance is: " + String(distance) + "in");
  
    //calculate speed
    speed = (abs(distancePrev - distance))/((abs(timeEchoPrev - timeEcho)) / 1000);

    //print calulcated speed
    Serial.println("Speed is calculated as: " + String(speed) + "in/mS");
  
    //update the previous distance and times recorded to the current one
    //distancePrev = distance;
    //timeTrigPrev = timeTrig;
    //timeEchoPrev = timeEcho;
    //delay for sanity's sake
    //delay(3000);
    Serial.println("2222222");
    
  }else if((distance == distancePrev) && (distancePrev != 0) && (timeEchoPrev > timeEcho - 5000)){
    //If no new measurement has been taken in five seconds, reset the values for the previous recorded measurement
    distancePrev = 0;
    timeEchoPrev = 0;
    timeTrigPrev = 0;
    Serial.println("3333333");
  }else if(distance != 0){
    //Since no change has been detected, update the previous distance and times recorded to the current one
    //distancePrev = distance;
    //timeTrigPrev = timeTrig;
    //timeEchoPrev = timeEcho;

    //Distance does not equal 0, print out the distance
    Serial.println("Distance is: " + String(distance) + "in");
    Serial.println("4444444");
  }
  */

  //Experiementing with eliminating interrupt for PIR sensor
  /*int pirValue = digitalRead(pir);

  if (pirValue == HIGH) {
    delay(1000);
    Serial.println("Motion detected!");
    activateSonar();
    delay(1000);
  }*/
  //Serial.println(distance);

  /*while(digitalRead(pir) == LOW) {
    if(digitalRead(pir) == RISING) {
      Serial.println("Loop Works");
      activateSonar();
      break;
    }
  }*/
  //Serial.println("Waiting for PIR to be tripped");

/*
  Serial.println("Triggering Now");
  //Triggering the sensor
  digitalWrite(trig, HIGH);
  //allowing 10uS for trigger
  delay(.01);
  //disabling the trigger
  digitalWrite(trig, LOW);
  //recording trigger time for distance calculation
  timeTrig = millis();
  //print out distance recorded from readDistance function
  Serial.println(distance);
  //allowing some time to check results
  delay(1000);
*/
}

void activateSonar() {
  /*
  This function executes whenever the PIR sensor is tripped.

  Its purpose is to trigger the sonar sensor to take a distance measurement and record
  the time when the sonar was triggered.
  */

  /* 
  I read that having time-consuming functions like delays and Serial.println 
  statements in an interrupt can lead to issues.

  I commented them all out and moved some print statements to the main loop for 
  debugging.
  */

  //Serial.println("PIR Tripped");
  //trigger the proximity sensor
  //Serial.println("Trigger Start");

  //Save the previous trigger time in case distance has been changed and speed must be calculated
  //timeTrigPrev = timeTrig;
  
  //Update the trigger time
  timeTrig = millis();

  //Trigger the sonar sensor, activating the interrupt to go into the readDistance() function
  digitalWrite(trig, HIGH);
  delay(trigdur);
  digitalWrite(trig, LOW);
  
  //Serial.println("Trigger End");
  //Serial.println(esp_timer_get_time() / 1000);
  //uint64_t timeTrig = esp_timer_get_time() / 1000;
  
  
  
  //delay(500); 
  //readDistance();
  //timeEchoPrev = timeEcho;
  //distancePrev = distance;
  
  /*
  I originally thought I could take two distance measurements within this function.
  Ended up running into interrupt issues where the sonar interrupt would execute
  while the pir interrupt was still going.
  Resolved by just having a variables to record the previous measurements and compare to the current ones
  to see if speed needs to be calculated.
  */

  //Serial.println("First distance recorded " + String(distancePrev) + "in");
  /*Serial.println(distancePrev);
  Serial.println(timeTrigPrev);
  Serial.println(timeEchoPrev);*/  
  //delay(3000);

  //second distance measurement
  /*digitalWrite(trig, HIGH);
  delay(trigdur);
  digitalWrite(trig, LOW);
  timeTrig = millis();
  timeTrigB = timeTrig;
  readDistance();
  timeEchoB = timeEcho;
  distanceB = distance;

  Serial.println("Second Distance Recorded");*/
  /*Serial.println(distanceB);
  Serial.println(timeTrigB);
  Serial.println(timeEchoB);*/

  //calculate speed
  //speed = (abs(distancePrev - distanceB))/(abs((timeEchoPrev - timeTrigPrev) - (timeEchoB - timeTrigB)));
  
  //Serial.println("Speed has been recorded as: " + speed);
}

void readDistance() {
  /*
  This function should execute after the sonar sensor has been 
  triggered, sends out a sound wave, and receives its echo.
  */

  //Save the previous measurement echo time in case a speed needs to be calculated
  //timeEchoPrev = timeEcho;

  //Update the echo time to the current measurement recording
  timeEcho = millis();
  
  //Check if there has been a long enough gap since the last time measured.
  /*if((timeEchoPrev > (timeEcho - 5000)) && (timeEchoPrev != 0)){
    timeEchoPrev = 0;
    timeTrigPrev = 0;
    distancePrev = 0;
  }else{
    distancePrev = distance;
    //distance = ((timeEcho - timeTrig) * 1000) / 148;
  }*/
  
  //Save the current distance value as the previously recorded distance before recording a new one
  distancePrev = distance;

  /*
  Distance formula from the HC-SR04 spec sheet:
  Formula: uS / 58 = centimeters or uS / 148 =inch; or: 
  the range = high level time * velocity (340M/S) / 2; 
  we suggest to use over 60ms measurement cycle, in order to prevent trigger signal to the echo signal. 
  */

  //Original distance formula
  //distance = ((timeEcho - timeTrig) * 340) / 2;
  
  //Distance formula for Inches
  distance = ((timeEcho - timeTrig) * 1000) / 148;

  //Distance formula for centimeters
  //distance = ((timeEcho - timeTrig) * 1000) / 58;

  //Experimenting with removing millis()
  //uint64_t timeEcho = esp_timer_get_time() / 1000;
  //Serial.println("Time Triggered: " + String(timeTrig));
  //Serial.println("Time Echo'd: " + String(timeEcho));
}
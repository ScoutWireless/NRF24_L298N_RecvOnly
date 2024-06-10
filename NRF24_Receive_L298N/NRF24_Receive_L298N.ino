
#include <SPI.h>
#include "printf.h"
#include "RF24.h"
#include <L298NX2.h>

#define CE_PIN 9
#define CSN_PIN 10

const unsigned int EN_A = 3;
const unsigned int IN1_A = 4;
const unsigned int IN2_A = 2;

const unsigned int IN1_B = 6;
const unsigned int IN2_B = 7;
const unsigned int EN_B = 5;

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars]; 

RF24 radio(CE_PIN, CSN_PIN);

L298NX2 motors(EN_A, IN1_A, IN2_A, EN_B, IN1_B, IN2_B);

const byte address[6] = "00001";

int X = 0;
int UP = 0;
int DOWN = 0;

void setup() {
  Serial.begin(9600);
  motors.setSpeed(0);
  motors.stop();
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

}

void loop() {

    if (radio.available()) {
        radio.read(&receivedChars, sizeof(receivedChars));
    }

    strcpy(tempChars, receivedChars);

    parseData();
    
  /*Serial.println(X);
  Serial.println(UP);
  Serial.println(DOWN);*/
    if (UP == 0 && DOWN == 0){
      motors.stop();
    }else if (UP==0 && DOWN == 1){
      int direction = steering();
      if(direction == 0){
        motors.setSpeedA(255);
        motors.setSpeedB(255);
        motors.forward();
      } else if (direction == 1){
        motors.setSpeedA(122);
        motors.setSpeedB(255);
        motors.forward();
      } else {
        motors.setSpeedA(255);
        motors.setSpeedB(122);
        motors.forward();
      }

    } else if (DOWN == 0 && UP == 1){
      int direction = steering();
      if(direction == 0){
        motors.setSpeedA(255);
        motors.setSpeedB(255);
        motors.backward();
      } else if (direction == 1){
        motors.setSpeedA(122);
        motors.setSpeedB(255);
        motors.backward();
      } else {
        motors.setSpeedA(255);
        motors.setSpeedB(122);
        motors.backward();
      }
    } else if (DOWN == 1 && UP == 1){
      motors.stop();
    }
  

}
int steering() {
  if(X>520){
    return 1;
  } else if(X<504){
    return 2;
  } else {
    return 0;
  }

}
void parseData() {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    X = atoi(strtokIndx);
 
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    UP = atoi(strtokIndx);     // convert this part to an integer

    strtokIndx = strtok(NULL, ",");
    DOWN = atoi(strtokIndx);     // convert this part to a float


}
//============

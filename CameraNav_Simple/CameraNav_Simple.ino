//#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#include <SPI.h>
#include <Pixy.h>
#include <math.h>

//The pin mapping
static const int steeringServoIn = 6;
static const int throttleServoIn = 7;
static const int alarmSystemIn = 9;
static const int doorLatch = 2;
static const int overRideButton = 5;
static const int overrideButton = 13;
static const int rightLED = 11;
static const int leftLED = 12;
static const int straightLED = 13;

int buttonState = 0;
const int sentenceSize = 80;
char sentence[sentenceSize];

//Create a Servo Object
Servo steeringServo;
Servo throttleServo;

// This is the main Pixy object 
Pixy pixy;



void setup()
{
  Serial.begin(9600);
  Serial.print("Starting...\n");
    //Initilization of Servo
  steeringServo.attach(steeringServoIn);
  throttleServo.attach(throttleServoIn);

    pinMode(overrideButton, OUTPUT);
  pinMode(alarmSystemIn,OUTPUT);
  pinMode(doorLatch,OUTPUT);
  pinMode(overRideButton, INPUT);

  pixy.init();
}

void loop()
{ 
  static int i = 0;
  int j;
  int target_x;
  int target_y;
  int boat_x;
  int boat_y;
  uint16_t blocks;
  char buf[32]; 
  int humanSignature = 1;
  int redSignature = 2;
  
  // grab blocks!
  blocks = pixy.getBlocks();
  
  // If there are detect blocks, print them!
  if (blocks)
  {
    i++;
    
    // do this (print) every 50 frames because printing every
    // frame would bog down the Arduino
    if (i%50==0)
    {
      sprintf(buf, "Detected %d:\n", blocks);
      Serial.print(buf);
      for (j=0; j<blocks; j++)
      {
        if(pixy.blocks[j].signature == humanSignature) {
                  target_x = pixy.blocks[j].x;
                  target_y = pixy.blocks[j].y;
                  Serial.println("Found Target");
                  Serial.println("Location Tar");
                  Serial.println(target_x);
                }
                if(pixy.blocks[j].signature == redSignature) {
                  boat_x = pixy.blocks[j].x;
                  boat_y = pixy.blocks[j].y;
                  Serial.println("Found EMILY");
                  Serial.println("Location Boat");
                  Serial.println(boat_x);
      }
    }

     if(boat_x == 0 || boat_y == 0 || target_x == 0 || target_y == 0 ) {
          stopServo();
          Serial.println("Stop");
        }
        else if(boat_x == target_x) {
          
          goStraightSlow();
          
        }

        //If the boat is left of where it should be turn right
        else if(boat_x < target_x) {
          
          Serial.println("Right");
          turnPartialRight();
          
        }

        //If the boat is right of where it should be turn left.
        else if (boat_x > target_x){
          
          Serial.println("left");
          turnPartialLeft();
          
        }
  }  
}
}


void goStraightFull() {
  steeringServo.write(90);
  throttleServo.write(180); 
  delay(1000);
}

void goStraightSlow() {
  steeringServo.write(90);
  throttleServo.write(30); 
  delay(500);
}
void goStraightMedium() {
  steeringServo.write(90);
  throttleServo.write(90); 
  delay(1000);
}


void turnPartialRight() {
  steeringServo.write(70);
  throttleServo.write(30); 
  delay(500);
}

void turnFullRight() {
 steeringServo.write(65);
  throttleServo.write(45); 
  delay(1000);
}

void turnPartialLeft() {
  steeringServo.write(110);
  throttleServo.write(30); 
  delay(500);
}

void turnFullLeft() {
  steeringServo.write(115);
  throttleServo.write(45); 
  delay(1000);
}



void stopServo() {
  steeringServo.write(90);
  throttleServo.write(0); 
  delay(5000);
}

void launch() {
  digitalWrite(overrideButton, HIGH);
  digitalWrite(alarmSystemIn,HIGH);
  digitalWrite(doorLatch,HIGH);
  
}

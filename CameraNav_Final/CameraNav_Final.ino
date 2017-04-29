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
static const int beaconIn = 8;
static const int doorLatchServoIn = 37;
static const int STOP_RANGE = 25;


int buttonState = 0;
const int sentenceSize = 80;
char sentence[sentenceSize];

//Create a Servo Object
Servo steeringServo;
Servo throttleServo;
Servo doorLatchServo;

// This is the main Pixy object 
Pixy pixy;



void setup()
{
  Serial.begin(9600);
  Serial.print("Starting...\n");
    //Initilization of Servo
  steeringServo.attach(steeringServoIn);
  throttleServo.attach(throttleServoIn);
  doorLatchServo.attach(doorLatchServoIn);

    
  pinMode(alarmSystemIn,OUTPUT);
  pinMode(beaconIn,OUTPUT);
  
  

  pixy.init();
}

void loop()
{ 
  int iterator = 0;
while(iterator == 0)  
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
                  launch();
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
        else if ((abs((target_x - boat_x) < 25 ) && (abs(target_y - boat_y) < 25))){
            stopServo();
          Serial.println("Stop");
          delay(20000);
          delay(20000);
          delay(20000);
          delay(20000);
          delay(20000);
        }
        else if(boat_x == target_x) {
          Serial.println("Staright");
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
  steeringServo.write(80);
  throttleServo.write(30); 
  delay(500);
}

void turnFullRight() {
 steeringServo.write(65);
  throttleServo.write(45); 
  delay(1000);
}

void turnPartialLeft() {
  steeringServo.write(100);
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
  doorLatchServo.write(55);
  digitalWrite(alarmSystemIn,HIGH);
  digitalWrite(beaconIn,HIGH);
  delay(4000);

  doorLatchServo.write(10);
  digitalWrite(alarmSystemIn,LOW);
  digitalWrite(beaconIn,LOW);
  
  
}


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
static const int gpsRX = 10;
static const int gpsTX = 11;
static const int overrideButton = 13;

int buttonState = 0;
const int sentenceSize = 80;
char sentence[sentenceSize];

//Create a Servo Object
Servo steeringServo;
Servo throttleServo;

//Serial connection to GPS Device
SoftwareSerial gpsSerial(gpsRX, gpsTX);
SoftwareSerial XBee(0,1);



//Create a Pixy Object
Pixy pixy;

void setup() {

  //Initilization of GPS
  Serial.begin(9600);
  XBee.begin(9600);
  gpsSerial.begin(9600);
  
  pixy.init();
  
  //Initilization of Servo
  steeringServo.attach(steeringServoIn);
  throttleServo.attach(throttleServoIn);
  
 // steeringServo.write(0);
 // steeringServo.write(180);

 
  pinMode(overrideButton, OUTPUT);
  pinMode(alarmSystemIn,OUTPUT);
  pinMode(doorLatch,OUTPUT);
  pinMode(overRideButton, INPUT);
  /*
  //Initilization of RX
  vw_set_ptt_inverted(true);
  vw_set_rx_pin(78);
  vw_setup(4000); //Bits per second
  vw_rx_start();
  */
}




void loop() {
  

  double latitude;
  double longitude; 
  double target_angle;
  double current_angle;
  int target_x;
  int target_y;
  int prev_x;
  int prev_y;
  int humanSignature = 2;
  int redSignature = 1;

  char field[20];
  
  int launchSignal = 0;
  static int i = 0;
  int j=0;
  uint16_t blockSize;
  char buf[32];
  uint16_t blocks;
  int boat_x=0;
  int boat_y=0;
  int tempSig = -1;
  int searching = 1;
  int dirCounter = 0;
  
    //Waiting for person to be detected
    blocks = pixy.getBlocks();
    while(searching) {
      blocks = pixy.getBlocks();
      for(j = 0; j < blocks; j++) {
        if(pixy.blocks[j].signature == humanSignature) {
          searching = 0;
        }
      }
    }

     //Launch the Boat
    launch();
    
    //give time for the boat to launch and get ready
    delay(10000); 
  
      
      //Main navigation loop.
      do { 
    
          //Manual Override
          buttonState = digitalRead(overRideButton);
          if(buttonState == HIGH) {
              digitalWrite(overrideButton,HIGH);
              exit(0);
          }  

             //Getting location of boat and person
             blocks = pixy.getBlocks();
             for(j = 0; j < blocks; j++) {
                //Human location
                if(pixy.blocks[j].signature == humanSignature) {
                  target_x = pixy.blocks[j].x;
                  target_y = pixy.blocks[j].y;
                }
                if(pixy.blocks[j].signature == redSignature) {
                  boat_x = pixy.blocks[j].x;
                  boat_y = pixy.blocks[j].y;
                }
             }

        //Going Straight
        if(dirCounter == 0 || dirCounter == 1) {
          goStraightSlow();
        }

        //If the boat is left of where it should be turn right
        else if(boat_x < target_x) {
          turnPartialRight();
        }

        //If the boat is right of where it should be turn left.
        else if(boat_x > target_x) {
          turnPartialLeft();
        }

        else {
          goStraightSlow();
        }
        dirCounter++;
        if(dirCounter == 3) {
          dirCounter = 0;
        }

        

 



          
    
      }while(abs((boat_x - target_x) > 20) && (abs(boat_y - target_y) > 20));
         
  }
  
  
  


//Functions to control the two servos responsible for controlling EMILY.
//The servos operate in a range of 0-1, 0 being full left, 1 full right

void goStraightFull() {
  steeringServo.write(90);
  throttleServo.write(180); 
  delay(1000);
}

void goStraightSlow() {
  steeringServo.write(90);
  throttleServo.write(20); 
  delay(1000);
}
void goStraightMedium() {
  steeringServo.write(90);
  throttleServo.write(90); 
  delay(1000);
}


void turnPartialRight() {
  steeringServo.write(80);
  throttleServo.write(20); 
  delay(500);
}

void turnFullRight() {
 steeringServo.write(65);
  throttleServo.write(45); 
  delay(1000);
}

void turnPartialLeft() {
  steeringServo.write(100);
  throttleServo.write(20); 
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





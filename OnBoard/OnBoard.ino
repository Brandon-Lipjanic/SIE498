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

 
  pinMode(13, OUTPUT);
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
  int current_x = 160;
  int current_y = 199;
  int prev_x;
  int prev_y;
  double lon_curr;
  double lon_prev;
  double lon_target;
  double lat_curr;
  double lat_prev;
  double lat_target;
  char field[20];
  
  int launchSignal = 0;
  static int i = 0;
  int j=0;
  uint16_t blockSize;
  char buf[32];
  char latCheck;
  char longCheck;

  //go straight for 2 seconds to get a reading of where we are at.
  goStraightSlow();
  goStraightSlow();

      //get initial values
               getField(field,3);
               lat_curr = atof(field);
               getField(field,4);
               lon_curr = atof(field);
      
      //Main navigation loop.
      do { 
    
          //Manual Override
          buttonState = digitalRead(overRideButton);
          if(buttonState == HIGH) {
              digitalWrite(13,HIGH);
              exit(0);
          }  

          if (gpsSerial.available()){
              char ch = gpsSerial.read();
              if (ch != '\n' && i < sentenceSize)
              {
                sentence[i] = ch;
                i++;
              }
              else
              {
               sentence[i] = '\0';
               i = 0;
               //update old values
               lat_prev = lat_curr;
               lon_prev = lon_curr;

               //grab new valyes
               getField(field,3);
               lat_curr = atof(field);
               getField(field,4);
               lon_curr = atof(field);
              }
          }

          //get target values
         lat_target = XBee.read();
         lon_target = XBee.read();
        while(lat_target < 0) {
          lat_target = atof(XBee.read());
        }

        while(lon_target > 0)
        {
          lon_target = atof(XBee.read());
        }



          
    
      }while(abs((lat_curr - lat_target) > 0.0001) && (abs(lon_curr - lon_target) > 0.0001));
         
  }
  
  
  
  /*
  //Testing the Servos
  delay(4000);
  turnFullRight();
  delay(4000);
  stopServo();
  delay(10000);
  turnFullLeft();
  delay(4000);
  stopServo();
  delay(6000);
  */
  /*
  //Testing the Alarm
digitalWrite(alarmSystemIn,HIGH);
  delay(10000);
  digitalWrite(alarmSystemIn,LOW);
  delay(2000);
  digitalWrite(alarmSystemIn,HIGH);
*/

/*
  //Testing the Door Latch
  delay(5000);
  digitalWrite(doorLatch,HIGH);
  delay(5000);
  digitalWrite(doorLatch,LOW);

*/

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
  throttleServo.write(180); 
  delay(1000);
}

void turnFullRight() {
 steeringServo.write(65);
  throttleServo.write(45); 
  delay(1000);
}

void turnPartialLeft() {
  steeringServo.write(100);
  throttleServo.write(180); 
  delay(1000);
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

void navigate(double lat_curr, double lat_prev, double lat_target,double lon_curr,double lon_prev,double lon_target) {

    //figure out what relative direction emily is currently moving (NW,NE,SW,SE)
    double NS_curr = lon_curr - lon_prev;
    double EW_curr = lat_curr - lat_prev;

    //figure out what relative direction emily is in relation to the target (NW,NE,SW,SE)
    double NS_target = lon_target - lon_curr;
    double EW_target = lat_target - lat_curr;

    boolean flagNS = false;
    boolean flagEW = false;

        //mark flagNS as true if EMILY is going north or south correctly
    if ((NS_curr > 1 && NS_target > 1 )|| (NS_curr < 1 && NS_target < 1 )) {
    
      flagNS = true;
    
    }

        //mark flagEW as true if EMILY is going east or west correctly
    if ((EW_curr > 1 && EW_target > 1 )|| (EW_curr < 1 && EW_target < 1 )) {
    
      flagEW = true;
    
    }

    //Calculate the target angle and currentl angle, adjust accordingly.
    double tarAngle = atan((lat_target - lat_curr)/(lon_target-lon_curr));
    double currAngle = atan((lat_curr - lat_prev)/(lon_curr-lon_prev));
    
    //If EMILY is going in the correct general direction, get more accurate by comparing current, previous, and target locations
    if(flagNS == true && flagEW == true){
    
      //Stay on path
      if (((tarAngle - currAngle) < 0.075) || ((tarAngle - currAngle) < -0.075)){
        goStraightSlow();
      }
    
    
      //Northwest
      if(NS_curr > 0 && EW_curr < 0) {
        
        if(tarAngle > currAngle) {
          turnPartialLeft();
        }
        
        if(tarAngle < currAngle) {
          turnPartialRight();
        }
      }
      
        //Northeast
      if(NS_curr > 0 && EW_curr > 0) {
        
        if(tarAngle > currAngle) {
          turnPartialRight();
        }
        
        if(tarAngle < currAngle) {
          turnPartialLeft();
        }
      }
      
        //Southwest
      if(NS_curr < 0 && EW_curr < 0) {
        
        if(tarAngle > currAngle) {
          turnPartialRight();
        }
        
        if(tarAngle < currAngle) {
          turnPartialRight();
        }
      }
      
        //Southeast
      if(NS_curr < 0 && EW_curr > 0) {
        
        if(tarAngle > currAngle) {
          turnPartialLeft();
        }
        
        if(tarAngle < currAngle) {
          turnPartialRight();
        }
      }
      
      
    }
    
    //If EMILY is not going in the right direction at all, spin and try again.
    else {
      turnFullRight();
    }


}

void displayGPS(){
  char field[20];
  getField(field, 0);
  if (strcmp(field, "$GPRMC") == 0)
  {
    Serial.print("Lat: ");
    getField(field, 3);  // number
    Serial.print(field);
    getField(field, 4); // N/S
    Serial.print(field);
    
    Serial.print(" Long: ");
    getField(field, 5);  // number
    Serial.print(field);
    getField(field, 6);  // E/W
    Serial.println(field);
    getField(field,7);
    Serial.print(" Speed: ");
    Serial.println(field);
  }
}

void getField(char* buffer, int index){
  int sentencePos = 0;
  int fieldPos = 0;
  int commaCount = 0;
  while (sentencePos < sentenceSize)
  {
    if (sentence[sentencePos] == ',')
    {
      commaCount ++;
      sentencePos ++;
    }
    if (commaCount == index)
    {
      buffer[fieldPos] = sentence[sentencePos];
      fieldPos ++;
    }
    sentencePos ++;
  }
  buffer[fieldPos] = '\0';
}


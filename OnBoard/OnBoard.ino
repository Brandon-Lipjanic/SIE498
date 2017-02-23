//#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Servo.h>
//#include <VirtualWire.h>
/*
//The pin mapping for the GPS
static const int GPSDataIn = 51;
static const int GPSDataOut = 53;
static const uint32_t GPSBaud = 9600;
*/
//The pin mapping for the servos
static const int steeringServoIn = 6;
static const int throttleServoIn = 7;
static const int alarmSystemIn = 9;
static const int doorLatch = 2;
/*
//The pin mapping for the RX 
static const int rxDataIn = 78;
//Create a TinyGPS Object
TinyGPSPlus gps;
*/
//Create a Servo Object
Servo steeringServo;
Servo throttleServo;

//Serial connection to GPS Device
//SoftwareSerial ss(GPSDataIn,GPSDataOut);

void setup() {

  //Initilization of GPS
  //Serial.begin(115200);
 // ss.begin(GPSBaud);
  
  //Initilization of Servo
  steeringServo.attach(steeringServoIn);
  throttleServo.attach(throttleServoIn);
  pinMode(13, OUTPUT);
  pinMode(alarmSystemIn,OUTPUT);
  pinMode(doorLatch,OUTPUT);
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
  int launchSignal = 0;
/*
//Testing the Servos

  while(1){
  delay(4000);
  turnFullRight();
  delay(4000);
  stopServo();
  delay(10000);
  turnFullLeft();
  delay(4000);
  stopServo();
  delay(6000);
  }
  */

  //Testing the Alarm

  delay(5000);
  digitalWrite(alarmSystemIn,HIGH);
  digitalWrite(doorLatch,HIGH);
  delay(5000);
  digitalWrite(alarmSystemIn,LOW);
  digitalWrite(doorLatch,LOW);
  
}

/*
//Function used to recieve lat data from the EMILY
double getLatitudeFromEMILY() {
  double latitude = 0.0;
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  
  if(vw_get_message(buf,&buflen)) {
    latitude = buf;
  }
  
  return latitude;
}
*/

//Function used to recieve long data from the EMILY
/*
 getLongitudeFromEMILY() {
  double longitude = 0.0;
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  
  if(vw_get_message(buf,&buflen)) {
    longitude = buf;
  }
  
  return longitude;
}
*/
//Functions to control the two servos responsible for controlling EMILY.
//The servos operate in a range of 0-1, 0 being full left, 1 full right

void turnFullRight() {
 steeringServo.write(75);
  throttleServo.write(45);
}

void turnFullLeft() {
  steeringServo.write(115);
  throttleServo.write(55);
}

void goStraightFull() {
  steeringServo.write(90);
  throttleServo.write(180);
}

void goStraightPartial() {
  steeringServo.write(90);
  throttleServo.write(180);
}

void turnPartialRight() {
  steeringServo.write(105);
  throttleServo.write(180);
}

void turnPartialLeft() {
  steeringServo.write(75);
  throttleServo.write(180);
}

void stopServo() {
  steeringServo.write(90);
  throttleServo.write(0);
}


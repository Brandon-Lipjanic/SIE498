#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <VirtualWire.h>

//The pins mapping for all the modules
static const int GPSDataIn = 51;
static const int GPSDataOut = 53;
static const int TXDataOut = 78;
static const uint32_t GPSBaud = 4800;

//Create a TinyGPS Object
TinyGPSPlus gps;

//Serial connection to GPS Device
SoftwareSerial ss(GPSDataIn,GPSDataOut);

void setup() {
  
  //Initilization of GPS
  Serial.begin(9600);
  ss.begin(GPSBaud);
  /*
  //Initilization of TX
  const char *lat_signal = "0";
  const char *long_signal = "0";
  uint8_t buf [VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  pinMode(78,OUTPUT);
  vw_set_ptt_inverted(true);
  vw_set_tx_pin(78);
  vw_setup(4000);
  */
}

void loop () {

double latitude;
double longitude;
int launchSignal = 0; 
Serial.print("1");
delay(2000);
  //While data is still being read
  while(ss.available() > 0){
    Serial.print("2");
    if(gps.encode(ss.read())) {
      Serial.print("3");
      if(gps.location.isValid()) {
        Serial.print("4");
        latitude = gps.location.lat();
        longitude = gps.location.lng();
        Serial.print("New Coordinates ");
        Serial.print(" Latitude ");
        Serial.print(latitude);
        Serial.print(" Longitude ");
        Serial.print(longitude);
        
       // sendData(latitude,longitude);
      }
    } 
  }
}


//Send data to the main ardunio via the TX-Module
/*void sendData(double in_latitude, double in_longitude) {

  //The data to be sent to the EARL arduino.
  lat_signal = in_latitude;
  long_signal = in_longitude;
  
  vw_send((uint8_t*)in_latitude,strlen(in_latitude));
  vw_wait_tx(); //Wait for the message to be clear.
  digitalWrite(78,1); //Set the write pin high for debugging
  
  delay(100); //Wait 100ms for the signal.
  digitalWrite(78,0); //Set the write pin low for deubgging
  
  vw_send((uint8_t*)in_longitude,strlen(in_longitude));
  vw_wait_tx(); //Wait fot the message to be clear.
  digitalWrite(78,1); //Set the write pin high for debugging 
  
  delay(100); //Wait 100ms for the signal;
}
*/



/* Output-side (LED) Arduino code */
#include "SoftwareSerial.h"
#include "String.h"
// RX: Arduino pin 0, XBee pin DOUT.  TX:  Arduino pin 1, XBee pin DIN are for the serial monitor
SoftwareSerial XBee(0, 1);  //blue goes to 0, yellow goes 1 
// int LED = 13;
//char c[40] ={0};
void setup()
{
  Serial.begin(9600);
 // Serial.println(" :GPS Data Recieved");
//Baud rate MUST match XBee settings (as set in XCTU)
  XBee.begin(9600);
  //pinMode(LED, OUTPUT);
}
void loop()
{
  //digitalWrite(LED,LOW);
 while (Serial.available()>0)  
 { 
   char c = Serial.read();
  
      Serial.print(c);
      Serial.print(" ");
      

  }
}



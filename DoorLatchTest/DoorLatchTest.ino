#include <SoftwareSerial.h>
#include <Servo.h>
#include <SPI.h>
#include <Pixy.h>
#include <math.h>

static const int doorLatch = 37;

void setup() {
  // put your setup code here, to run once:
pinMode(doorLatch,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
delay(5000);
digitalWrite(doorLatch,HIGH);
delay(5000);
digitalWrite(doorLatch,LOW);
}

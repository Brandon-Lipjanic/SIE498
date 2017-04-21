#include <Servo.h>

Servo myservo; 
bool started= false;
bool ended = false;
char incomingByte ; 
bool thr_start = false;
bool thr_end = false;
char thr_byte;
byte thr_index;
char throttle[4];
char msg[4];		
byte index;			

void setup() {
  myservo.attach(7);
  Serial.begin(57600); 
}

void loop() {
	
  while (Serial.available()>0){
  	//Read the incoming byte
    incomingByte = Serial.read();
    //Serial.println(incomingByte);
    if(incomingByte == '<')
    {
     started = true;
     index = 0;
     msg[index] = '\0'; // Throw away any incomplete packet
   }
  
   else if(incomingByte == '>')
   {
     ended = true;
     break; 
   }

   else
   {
     if(index < 4) 
     {
       msg[index] = incomingByte; 
       index++;
       msg[index] = '\0';
     }
   }
 }
 
 if(started && ended)
 {
   int value = atoi(msg);
   Serial.println(value);
   myservo.writeMicroseconds(value);
   index = 0;
   msg[index] = '\0';
   started = false;
   ended = false;
 }
}

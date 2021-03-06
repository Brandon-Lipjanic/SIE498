#include <SoftwareSerial.h>
#include <String.h>

SoftwareSerial gpsSerial(10, 11); // RX, TX (TX not used) green wire, purple wire
SoftwareSerial XBee(2, 3 );  // RX, TX
const int sentenceSize = 80;

char sentence[sentenceSize];

void setup()
{
  Serial.begin(9600);
  gpsSerial.begin(9600);
   XBee.begin(9600);
}

void loop()
{
  static int i = 0;
  if (gpsSerial.available())
  {
   
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
    
     displayGPS();
    }
  }
}

void displayGPS()
{
  char field[20];
  getField(field, 0);

  if (strcmp(field, "$GPRMC") == 0)
  {
    getField(field, 3);  // number
    Serial.println(field);
    XBee.write(field);

    
    
    getField(field, 5);  // number
    Serial.println(field);
    XBee.write(field);                 
 

    //delay(50);
  }
}

void getField(char* buffer, int index)
{
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

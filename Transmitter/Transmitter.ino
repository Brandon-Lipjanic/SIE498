int x = 0;
int rudder = 0;
int y = 0;
int throttle = 0;

void setup() {
  Serial.begin(57600);
}

void loop() {
  x = analogRead(A1);
  rudder = x + 1000;
  y = analogRead(A0);
  throttle = y + 1000;
  //Serial.print('$');
  //Serial.print(throttle);
 // Serial.print('>');
  Serial.print('<');
  Serial.println(rudder);
  Serial.println('>');
}


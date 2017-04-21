int x = 0;
float y = 0;
int rudder = 0;
int a = 0;
float b = 0;
int throttle = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  x = analogRead(A0);
  y = -0.138*x+156.138;
  rudder = y;
  analogWrite(9,rudder);
  x = analogRead(A2);
  y = x*(5.0/1023.0);
  Serial.println(y);
  
  
  a = analogRead(A1);
  if(a>=505){
  b = 0.156*a-20.78;
  }
  else if((a<=504)&&(a>=501)){
  b = 58;
  }
  else if(a<=500){
  b = 0.0641*a+25.615;
  }
  throttle = b;
  analogWrite(5,throttle);
}



#include <Servo.h>

unsigned long previousMillis=0;
unsigned long interval=3200L;
const int PIN_D2=2;
const int PIN_D6=6;
const int PIN_D10=10;
bool flag=false;

Servo servo;
void boutonArretUrgence(){
  Serial.println("Arret funny action");
  
  
}

void servoMoteur(){
  servo.write(60);
  Serial.println("Fusee lancee");
}

void setup() {
  // put your setup code here, to run once:
  attachInterrupt(digitalPinToInterrupt(PIN_D2),boutonArretUrgence,HIGH);
  pinMode(PIN_D6,INPUT);
  pinMode(PIN_D10,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(3,OUTPUT);
  Serial.begin(9600);
  servo.attach(PIN_D2);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("0");
  digitalWrite(7,HIGH);
  if (digitalRead(PIN_D6)==HIGH&&flag==false) {
    if (previousMillis<=interval){
      previousMillis=millis();
      Serial.print("previousMillis = "); 
      Serial.print(previousMillis);
      delay(100);
      Serial.println("\nmillis = ");
      Serial.print(millis());
    
          
  }
  servoMoteur();
  flag=true;
  }
  
  
}

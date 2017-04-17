#include <Servo.h>
#include <Arduino.h>

unsigned long interval=92000L; //Au bout de 90s, lancement funny action
const int PIN_D2=2; //Pin pour le bouton arret d'urgence, arret funny action si = HIGH
const int PIN_D3=3; //Pin pour la lancé de la funny action, ordre venant de la carte mère : HIGH
const int PIN_D5=5; // Pin test sortant un état HIGH
const int PIN_D6=6; // Pin test sortant un état HIGH

bool flag=false;    //arret de la funny une fois lancée

Servo servo;

// permet le reset de la millis
struct myMillis {
  unsigned long offset = 0;
  void set(unsigned long current) {
    offset = millis() - current;
  }
  void reset() {
    offset = millis();
  }
  unsigned long get() {
    return millis() - offset;
  }
};

myMillis MM;


void  servoMoteur()
{
      servo.write(60);  // angle à corriger
      Serial.println("Fusee lancee");
}

void setup() {
  
      pinMode(PIN_D2,INPUT);
      pinMode(PIN_D5,OUTPUT);
      pinMode(PIN_D6,OUTPUT);
      attachInterrupt(digitalPinToInterrupt(PIN_D3),interruption,HIGH);   
      Serial.begin(9600);
      servo.attach(10);
      digitalWrite(PIN_D5,HIGH);
      digitalWrite(PIN_D6,HIGH);
      
}

/**
 * fonction d'interruption à la pin D2, reset la milis au lancement de l'ordre
 */
void interruption() {
      MM.reset();
      Serial.println("reset millis");
}


void loop() {
  
      if (digitalRead(PIN_D3)==HIGH&&flag==false&&digitalRead(PIN_D2)==LOW){ 
        
            Serial.println(MM.get());
            if(MM.get()>=interval){
                    servoMoteur();  
                    flag=true;     
            }
  }
   
  

}

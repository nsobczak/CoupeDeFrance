/***************** Capteur IR DETECTION POSITION ***********************
 * 
 * Ce programme permet d'informer la carte maitre
 * si il y a présence ou non d'un obstacle, ici d'un 
 * cylindre et de déterminer sa position.
 * 
 ************************************************************/
//____________________________________________________________________________________________________
// I2C
#include <Wire.h>
#include "i2cCommunication.h"


//____________________________________________________________________________________________________
// I2C

#define _RECEIVEADRESS_ 10
#define _SENDADRESS_ 11

#include <math.h>


int pin_capteur = A0;                   // 
float c;
float d;


void setup() {
  Serial.begin(9600);
}

void loop() {
  //Serial.println(analogRead(pin_capteur));
  
    c=analogRead(pin_capteur);
    Serial.println(c);
    d=exp((c-740)/(-198));
    Serial.println("Distance : ");
    Serial.print(d);
    Serial.print("cm");
    Serial.println("");
    
    
 
  
  
  delay(500); 
}

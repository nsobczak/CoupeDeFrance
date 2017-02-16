/**
 *    \file Capteur_IR_detection_position.cpp
 *    \brief Ce programme permet d'informer la carte maitre si il y a présence ou non d'un obstacle, ici d'un cylindre et de déterminer sa position.
 *    \author Olivier Jombart
 *    \date décembre 2016
 */
//____________________________________________________________________________________________________
//____________________________________________________________________________________________________
// I2C
#include <Wire.h>
#include "i2cCommunication.h"
// exponentielle
#include <math.h>


//____________________________________________________________________________________________________
// define
#define _RECEIVEADRESS_ 10
#define _SENDADRESS_ 11


//____________________________________________________________________________________________________
// variables globales
int pin_capteur = A0;                
float c;
float d;


//____________________________________________________________________________________________________
//____________________________________________________________________________________________________
/**
 * \fn void setup() 
 * \brief Fonction setup d'arduino
 */
void setup() {
  Serial.begin(9600);
}


/**
 * \fn void loop() 
 * \brief Fonction loop d'arduino
 */
void loop() {
  //Serial.println(analogRead(pin_capteur));
  
  c=analogRead(pin_capteur);
  //Serial.println(c);
  d=exp((c-740)/(-198));
  Serial.println("Distance : ");
  Serial.print(d);
  Serial.print("cm");
  Serial.println("");
    
  delay(500); 
}

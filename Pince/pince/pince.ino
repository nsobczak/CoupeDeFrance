/* |=====================|
 * | Gestion de la pince |
 * |=====================|
 */
/**
 *    \file pince.cpp
 *    \brief Gestion de la pince
 *    \author Olivier JOMBART
 *    \date Novemmbre 2016
*/

#include <Servo.h>

Servo myservo;        // create servo object to control a servo

int interrupt_pin=7;  //exemple de pin d'entrée à toi de choisir celle qui convient le mieux

void setup() {
  pinMode(interrupt_pin,INPUT); 
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void mouvement_pince(const int pin_in){   //la pin d'entrée qui commande l'ouverture ou fermeture
  if(digitalRead(pin_in)==LOW) {
    myservo.write(55);     //fermée
  }
  if (digitalRead(pin_in)==HIGH) { 
    myservo.write(10);     // ouverte
  }
}

void loop() {
  
  mouvement_pince(interrupt_pin);
}

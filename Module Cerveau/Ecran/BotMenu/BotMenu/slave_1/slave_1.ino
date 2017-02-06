#include <Arduino.h>

/**
 *    \file slave_1.cpp
 *    \brief Esclave du BotMenu I2C
 *    \author Nicolas Sobczak
 *    \date decembre 2016
*/

#include <Wire.h>
#include "i2cCommunication.h"

#define _RECEIVEADRESS_ 8
#define _SENDADRESS_ 9


//_____________________________________________________________________________________________
//_____________________________________________________________________________________________
//Définition des variables
int x = 24;
int y = 309;
int z = 97;
int vitesse_1 = 123;
int vitesse_2 = 580;
int vitesse_3 = 298;

//Tableau de correspondance des variables
int correspondance[256];

void initialisationCorrespondance(int correspondance[])
{
  correspondance[0] = x ;
  correspondance[1] = y ;
  correspondance[2] = z ;
  correspondance[3] = vitesse_1 ;
  correspondance[4] = vitesse_2 ;
  correspondance[5] = vitesse_3 ;
}


//_____________________________________________________________________________________________
//_____________________________________________________________________________________________
// variable to store the reading from analog
byte data[3]; 

void setup() 
{
  initialisationCorrespondance(correspondance);
  Serial.begin(9600);           // Begin Serial coomunication 
  Wire.begin(_SENDADRESS_);     // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
}


void loop() 
{

}


/**
 * \fn void requestEvent()
 * \brief function that executes whenever data is requested by master this function is registered as an event, see setup()
 */
void requestEvent() 
{
  changeData(data, correspondance, 6);
  Wire.write(data, 3);
}



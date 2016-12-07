/**
 *    \file Moteur_deplacement.cpp
 *    \brief Code de déplacement de la pince
 *    \author Olivier Jombart
 *    \date décembre 2016
*/
//____________________________________________________________________________________________________
// I2C
#include <Wire.h>
#include "i2cCommunication.h"


//____________________________________________________________________________________________________
// I2C
#define _RECEIVEADRESS_ 10
#define _SENDADRESS_ 11


// Shield - For RAMPS 1.4
#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#define X_MIN_PIN           3
#define X_MAX_PIN           2

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_MIN_PIN          14
#define Y_MAX_PIN          15

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19

#define E_STEP_PIN         26
#define E_DIR_PIN          28
#define E_ENABLE_PIN       24

#define SDPOWER            -1
#define SDSS               53
#define LED_PIN            13

#define FAN_PIN            9

#define PS_ON_PIN          12
#define KILL_PIN           -1

#define HEATER_0_PIN       10
#define HEATER_1_PIN       8
#define TEMP_0_PIN          13   // ANALOG NUMBERING
#define TEMP_1_PIN          14   // ANALOG NUMBERING


//____________________________________________________________________________________________________
// Variables globales
int index;
int demarrerMoteur;
int finInitialisation;


//____________________________________________________________________________________________________
//____________________________________________________________________________________________________
/**
 * \fn void vitesse_moteur(int vitesse)
 * \brief fonction qui... ??
 * \param int vitesse
 */
void vitesse_moteur(int vitesse) 
{                                
    digitalWrite(X_STEP_PIN,HIGH);            //PWM pour MOTEUR X
    digitalWrite(Y_STEP_PIN,HIGH);            //PWM pour MOTEUR Y
    delayMicroseconds(vitesse);
    digitalWrite(X_STEP_PIN,LOW);             //Fin PWM pour MOTEUR X
    digitalWrite(Y_STEP_PIN,LOW);             //Fin PWM pour MOTEUR Y
    delayMicroseconds(vitesse); 
}


/**
 * \fn void vitesse_moteur_rail(int vitesse)
 * \brief fonction qui... ??
 * \param int vitesse
 */
void vitesse_moteur_rail(int vitesse) 
{        
    digitalWrite(Z_STEP_PIN,HIGH);            //PWM pour MOTEUR Z
    delayMicroseconds(vitesse);
    digitalWrite(Z_STEP_PIN,LOW);             //Fin PWM pour MOTEUR Z
    delayMicroseconds(vitesse); 
}


/**
 * \fn void remonter_legere() 
 * \brief fonction qui... ??
 */
void remonter_legere() 
{
    for(index=0;index<2000;index++) //un tour entier
      { 
        //sens trigo
        digitalWrite(X_DIR_PIN,LOW);             //Sens trigo MOTEUR X
        digitalWrite(Y_DIR_PIN,LOW);             //Sens trigo MOTEUR Y
        digitalWrite(X_ENABLE_PIN,LOW);          //Activé MOTEUR X
        digitalWrite(Y_ENABLE_PIN,LOW);          //Activé MOTEUR Y
        vitesse_moteur(200);
      }
}


/**
 * \fn void rail_initialisation(int vitesse)
 * \brief fonction qui... ??
 * \param int vitesse
 */
void rail_initialisation(int vitesse)
{
      digitalWrite(Z_DIR_PIN,HIGH);              //Sens horaire MOTEUR Z  
      digitalWrite(Z_ENABLE_PIN,LOW);            //Activé MOTEUR Z
      while(digitalRead(Z_MIN_PIN )!=HIGH)  {    //Tant que le capteur de fin de course à droite n'est pas activé
      vitesse_moteur_rail(vitesse);      }
      digitalWrite(Z_ENABLE_PIN,HIGH);           //Désactivé MOTEUR 
      delay(1000);         
}


/**
 * \fn void monter_descente_initialisation(int vitesse)
 * \brief fonction qui... ??
 * \param int vitesse
 */
void monter_descente_initialisation(int vitesse)
{
      digitalWrite(X_DIR_PIN,HIGH);              //Sens horaire MOTEUR X (on descend)
      digitalWrite(Y_DIR_PIN,HIGH);              //Sens horaire MOTEUR Y (on descend)
      digitalWrite(X_ENABLE_PIN,LOW);            //Activé MOTEUR X
      digitalWrite(Y_ENABLE_PIN,LOW);            //Activé MOTEUR Y
      while(digitalRead(X_MIN_PIN )!=HIGH)  {
      vitesse_moteur(vitesse);      }
      digitalWrite(X_ENABLE_PIN,HIGH);           //Désactivé MOTEUR X
      digitalWrite(Y_ENABLE_PIN,HIGH);           //Désactivé MOTEUR Y
      delay(1000);
      remonter_legere();            
}


/**
 * \fn void initialisation()
 * \brief fonction qui... ??
 */
void initialisation() 
{
      monter_descente_initialisation(100);
      rail_initialisation(200);
      finInitialisation = 1;
      delay(1000);
}

//____________________________________________________________________________________________________
//____________________________________________________________________________________________________
// Reception I2C
/**
 * \fn void receiveEvent(int howMany - fonction qui est exécutée lorsque des données sont envoyées par le Maître. Cette fonction est enregistrée comme un événement ("event" en anglais), voir la fonction setup()
 * \param int howMany
 */
void receiveEvent2(int howMany)
{
  byte x = Wire.read();      
  demarrerMoteur = x;
}


/**
 * \fn void i2creceive(int adresse) - fonction de lecture de données reçues via l'i2c
 * \param int adresse sur laquelle recevoir les donnees
 */
void i2creceive2(int adresse)
{
  Wire.begin(adresse);           // Joindre le Bus I2C avec adresse
  Wire.onReceive(receiveEvent2); // enregistrer l'événement (lorsqu'une demande arrive)
  Wire.endTransmission();    		// fin transmission
}


//____________________________________________________________________________________________________
//____________________________________________________________________________________________________
/**
 * \fn void setup()
 * \brief fonction setup d'arduino
 */
void setup() 
{
      pinMode(LED_PIN, OUTPUT);                    //Vérifie que tout a bien été téléversé 
      pinMode(X_ENABLE_PIN, OUTPUT);               //Enable | Activé si la pin est à l'état "LOW" desactivé si elle est à l'état "HIGH" MOTEUR X
      pinMode(X_STEP_PIN, OUTPUT);                 //Step PWM MOTEUR X
      pinMode(X_DIR_PIN, OUTPUT);                  //Direction LOW=SENS TRIGO / HIGH=SENS HORAIRE  MOTEUR X 
      pinMode(X_MIN_PIN , INPUT);
      pinMode(Y_ENABLE_PIN, OUTPUT);               //Enable | Activé si la pin est à l'état "LOW" desactivé si elle est à l'état "HIGH" MOTEUR Y
      pinMode(Y_STEP_PIN, OUTPUT);                 //Step PWM MOTEUR Y
      pinMode(Y_DIR_PIN, OUTPUT);                  //Direction LOW=SENS TRIGO / HIGH=SENS HORAIRE  MOTEUR Y
      pinMode(Z_ENABLE_PIN, OUTPUT);               //Enable | Activé si la pin est à l'état "LOW" desactivé si elle est à l'état "HIGH" MOTEUR X
      pinMode(Z_STEP_PIN, OUTPUT);                 //Step PWM MOTEUR X
      pinMode(Z_DIR_PIN, OUTPUT);                  //Direction LOW=SENS TRIGO / HIGH=SENS HORAIRE  MOTEUR X 
      pinMode(Z_MIN_PIN , INPUT);
      
      demarrerMoteur = 0;
      finInitialisation = 0;
      
      Serial.begin(9600);
}


/**
 * \fn void loop()
 * \brief fonction loop d'arduino
 */
void loop() 
{
    i2creceive2(_RECEIVEADRESS_);
    
    if (demarrerMoteur == 1)
    {
       initialisation();
       finInitialisation = 0;
       i2csend(demarrerMoteur, _SENDADRESS_);
       demarrerMoteur = 0;
    }
}



  


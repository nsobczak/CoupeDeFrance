/* |===================================|
 * | Bibliothèque de communication i2c |
 * |===================================|
 */
/**
 *    \file i2cCommunication.cpp
 *    \brief Bibliothèque de communication i2c
 *	  \author Nicolas SOBCZAK
 *    \date Octobre 2016
*/


/* ======================================================================================================
 *      Include
 * ======================================================================================================
 */
#include <Wire.h>
#include "Arduino.h"
#include "i2cCommunication.h"


/* ======================================================================================================
 *      Fonctions
 * ======================================================================================================
 */
//_____________________________________________________________________________________________
//_____________________________________________________________________________________________
// LED : ordre 0 ou 1

/**
 * \fn void ledOff(int pin) - fonction qui eteint une LED
 * \param int pin de la LED
 */
void ledOff(int pin)
{
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}


/**
 * \fn void ledOn(int pin) - fonction qui allume une LED
 * \param int pin de la LED
 */
void ledOn(int pin)
{
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
}


/**
 * \fn void ledControl(int pin, int order) - fonction qui eteint une LED
 * \param int pin de la LED, int order =1 pour allumer ou =0 pour eteindre
 */
void ledControl(int pin, int order)
{
  if (order==0)
  {
    ledOff(pin);
  }
  if (order==1)
  {
    ledOn(pin);
  }
}


//_____________________________________________________________________________________________
/**
 * \fn void orderNumber(uint8_t order) - fonction qui execute l'ordre dont le numéro est entrée en parametre
 * \param uint8_t order = numero de l'ordre a executer
 */
void orderNumber(uint8_t order)
{
  Serial.println("ReceivedOrder: ");
  Serial.print(order);   // Afficher la valeur numérique
  switch (order)
  {
    case 0:
    {
      int pin = 13;
      ledControl(pin, order);              // Allume ou éteint la led
      break;
    }
    case 1:
    {
      int pin = 13;
      ledControl(pin, order);              // Allume ou éteint la led
      break;
    }
    default: 
    {
      Serial.println("Ordre non renseigne");
      // if nothing else matches, do the default
      // default is optional
    }
    break;
  }
}


//_____________________________________________________________________________________________
// Reception

/**
 * \fn void receiveEvent(int howMany - fonction qui est exécutée lorsque des données sont envoyées par le Maître. Cette fonction est enregistrée comme un événement ("event" en anglais), voir la fonction setup()
 * \param int howMany
 */
void receiveEvent(int howMany)
{
  while(1 < Wire.available()) // Lire tous les octets sauf le dernier => utile si on a écrit qqch devant le numéro de l'ordre mais il vaut mieux ne pas le faire pour éviter les pb
  {
    char c = Wire.read();     // lecture de l'octet/byte comme caractère
    Serial.print(c);          // afficher le caractère
  }
  byte x = Wire.read();        // lecture de l'octet/byte ignoré comme un entier
  orderNumber(x);			  // lecture de l'ordre à executer
}


/**
 * \fn void i2creceive(int adresse) - fonction de lecture de données reçue via l'i2c
 * \param int adresse sur laquelle recevoir les donnees
 */
void i2creceive(int adresse)
{
  Wire.begin(adresse);          // Joindre le Bus I2C avec adresse
  Wire.onReceive(receiveEvent); // enregistrer l'événement (lorsqu'une demande arrive)
  Serial.begin(9600);           // Démarrer une communication série
}


//_____________________________________________________________________________________________
// Envoi
/**
 * \fn void i2csend(uint8_t order, int adresse) - fonction d'envoi de données via l'i2c
 * \param uint8_t order numero de l'ordre a envoyer, int adresse sur laquelle envoyer les donnees
 */
void i2csend(uint8_t order, int adresse)
{
  Wire.begin(); 					// joindre le bus i2c (adresse est optionnelle pour un maître)
  Wire.beginTransmission(adresse);  // Commencer transmission vers l'esclave  #4
  //Wire.write("order: ");       		// Envoi de 5 octets (5 bytes)
  Wire.write(order);             	// envoi d'un byte/octet (valeur numérique)  
  Wire.endTransmission();    		// fin transmission
}


//_____________________________________________________________________________________________
//_____________________________________________________________________________________________


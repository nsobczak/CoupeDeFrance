/* |===================================|
 * | Bibliothèque de communication i2c |
 * |===================================|
 */
/**
 *    \file i2cCommunication.h
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



#ifndef I2CCOMMUNICATION_H
#define I2CCOMMUNICATION_H

#ifdef  __cplusplus
extern "C" {
#endif


/* =============================================================================
 *      *** Definition des ordres ***
 * =============================================================================
 */

// _____________________________________________________________________________
// # LEDs:
// Eteint une led
void ledOff(int);

// Allume une led
void ledOn(int);

// Allume une led si order = 1, eteint la led si order = 0
void ledControl(int, int);


// _____________________________________________________________________________
// # Ordres:

// Fonction qui affiche l'octet reçu
void byteReceived(byte);

// Execute l'ordre dont le numéro est entrée en parametre
void orderNumber(uint8_t);

// Fonction qui change la variable du tableau correspondace envoyée
void changeData(byte data[], int correspondance[], int numberOfVariables);


/* =============================================================================
 *      *** i2c ***
 * =============================================================================
 */

// _____________________________________________________________________________
// # Envoi:

// Fonction d'envoi d'1 octet via l'i2c
void i2csend(uint8_t, int);

// Fonction d'envoi de 3 octets via l'i2c avec initialisation
void i2csend3bytes(uint8_t, uint8_t, uint8_t, int);
// Fonction d'envoi de 5 octets via l'i2c avec initialisation
void i2csend5bytes(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, uint8_t byte5, int adresse);


// _____________________________________________________________________________
// # Réception:

// Fonction qui est exécutée lorsque des données sont envoyées par le Maître.
// Cette fonction est enregistrée comme un événement ("event" en anglais), voir la fonction setup()
void receiveEvent(int);
// Fonction qui demande l'envoi d'une certaine variable à un esclave
void i2creceive(int);


// _____________________________________________________________________________
// # Request

// Fonction d'envoi d'1 octet via l'i2c
byte* i2crequest(int adresse, int nbBytes, int variable, int numberOfVariables);


// _____________________________________________________________________________
// # Conversion:

// Fonction qui convertit un entier en 2 bytes - low ici
byte getLowByte(int);

// Fonction qui convertit un entier en 2 bytes - high ici
byte getHighByte(int n);

// Fonction qui affiche les 2 bytes d'un nombre entier converti en binaire
void intTo2Bytes(byte[], int);

// Fonction qui affiche un entier ayant ete converti en 2 bytes binaires
int recoverIntFrom2Bytes(byte[]);

// Fonction qui affiche un float ayant ete converti en 2 bytes binaires
float recoverFloatFrom2Bytes(byte byte1_intPart, byte byte2_decPart, int precision);


// =============================================================================
// =============================================================================
#ifdef  __cplusplus
}
#endif

#endif  /* I2CCOMMUNICATION_H */

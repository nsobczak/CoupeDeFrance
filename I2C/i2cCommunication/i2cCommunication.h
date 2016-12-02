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
#define	I2CCOMMUNICATION_H

#ifdef	__cplusplus
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

//Execute l'ordre dont le numéro est entrée en parametre
void orderNumber(uint8_t);

//fonction qui affiche l'octet reçu
void byteReceived(byte);



/* =============================================================================
 *      *** i2c ***
 * =============================================================================
 */

// _____________________________________________________________________________
// # Envoi:

//Fonction d'envoi d'1 octet via l'i2c
void i2csend(uint8_t, int);

//Fonction d'envoi de 3 octets via l'i2c
void i2csend3bytes(uint8_t, uint8_t, uint8_t, int);

// _____________________________________________________________________________
// # Réception:

// Fonction qui est exécutée lorsque des données sont envoyées par le Maître.
// Cette fonction est enregistrée comme un événement ("event" en anglais), voir la fonction setup()
void receiveEvent(int);

//Fonction de lecture de données reçue via l'i2c
void i2creceive(int);


// _____________________________________________________________________________
// # Conversion:

// Fonction qui convertit un entier en 2 bytes - low ici
byte getLowByte(int);

// Fonction qui convertit un entier en 2 bytes - high ici
byte getHighByte(int n);

// Fonction qui affiche les 2 bytes d'un nombre entier convertit en binaire
void intTo2Bytes(byte[], int);

// Fonction qui affiche un entier ayant ete convertit en 2 bytes binaire
int recoverIntFrom2Bytes(byte[]);


// =============================================================================
// =============================================================================
#ifdef	__cplusplus
}
#endif

#endif	/* I2CCOMMUNICATION_H */

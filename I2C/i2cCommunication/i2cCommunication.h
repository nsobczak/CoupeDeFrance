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



/* =============================================================================
 *      *** i2c ***
 * =============================================================================
 */

// _____________________________________________________________________________
// # Envoi:

//Fonction d'envoi de données via l'i2c
void i2csend(uint8_t, int);


// _____________________________________________________________________________
// # Réception:

// Fonction qui est exécutée lorsque des données sont envoyées par le Maître.
// Cette fonction est enregistrée comme un événement ("event" en anglais), voir la fonction setup()
void receiveEvent(int);

//Fonction de lecture de données reçue via l'i2c
void i2creceive(int);



// =============================================================================
// =============================================================================
#ifdef	__cplusplus
}
#endif

#endif	/* I2CCOMMUNICATION_H */

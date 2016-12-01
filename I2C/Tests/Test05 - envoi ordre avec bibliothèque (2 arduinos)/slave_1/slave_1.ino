#include <Wire.h>
#include "i2cCommunication.h"

#define _RECEIVEADRESS_ 8


//_____________________________________________________________________________________________
//_____________________________________________________________________________________________

/**
 * \fn void byteReceived(byte octet)
 * \brief fonction qui affiche l'octet reçu
 * \param byte octet
 */
void byteReceived(byte octet)
{
  Serial.print("ReceivedByte: ");
  Serial.println(octet);   // Afficher la valeur numérique
}
  
//_____________________________________________________________________________________________
// Reception

/**
 * \fn void receiveEvent(int howMany - fonction qui est exécutée lorsque des données sont envoyées par le Maître. Cette fonction est enregistrée comme un événement ("event" en anglais), voir la fonction setup()
 * \param int howMany
 */
void receiveEvent2(int howMany)
{
  byte x = Wire.read();        // lecture de l'octet/byte ignoré comme un entier
  //orderNumber(x);			  // lecture de l'ordre à executer
  byteReceived(x);
}


/**
 * \fn void i2creceive(int adresse) - fonction de lecture de données reçues via l'i2c
 * \param int adresse sur laquelle recevoir les donnees
 */
void i2creceive2(int adresse)
{
  Wire.begin(adresse);          // Joindre le Bus I2C avec adresse
  Wire.onReceive(receiveEvent2); // enregistrer l'événement (lorsqu'une demande arrive)
}


/**
 * \fn void i2creceive3Bytes(int adresse, int nbOctets)
 * \brief fonction qui reçoit des octets
 * \param int adresse - adresse sur laquelle recevoir les donnees
 * \param int nbOctets - nombre d'octets à recevoir
 */
void i2creceive3Bytes(int adresse, int nbOctets){
  
  for (int i = 0; i < nbOctets; ++i){
    i2creceive2(adresse);
  }
  
}


//_____________________________________________________________________________________________
//_____________________________________________________________________________________________
void setup()
{
  Serial.begin(9600);           // Démarrer une communication série
}

void loop()
{
  Serial.println("\nAttente des octets "); 

  //Reçoit octets
  i2creceive3Bytes(_RECEIVEADRESS_, 3);

  //recoverIntFrom2Bytes(bytesTab);

  delay(500);
}



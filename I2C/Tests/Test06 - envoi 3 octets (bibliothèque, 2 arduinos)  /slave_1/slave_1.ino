#include <Wire.h>
#include "i2cCommunication.h"

#define _RECEIVEADRESS_ 8


//_____________________________________________________________________________________________
//_____________________________________________________________________________________________

  
//_____________________________________________________________________________________________
// Reception

/**
 * \fn void receiveEvent(int howMany - fonction qui est exécutée lorsque des données sont envoyées par le Maître. Cette fonction est enregistrée comme un événement ("event" en anglais), voir la fonction setup()
 * \param int howMany
 */
void receiveEvent3Bytes(int howMany)
{
  //Serial.print("howMany = ");
  //Serial.println(howMany);
  byte var = Wire.read();             // lecture de l'octet/byte ignoré comme un entier
  byte highByte = Wire.read();        // lecture de l'octet/byte ignoré comme un entier
  byte lowByte = Wire.read();         // lecture de l'octet/byte ignoré comme un entier
  
  byte bytesTab[2] = {highByte, lowByte};
  int value = recoverIntFrom2Bytes(bytesTab);
  //byteReceived(value);
  orderNumber(value);			  // lecture de l'ordre à executer

}


/**
 * \fn void i2creceive(int adresse) - fonction de lecture de données reçues via l'i2c
 * \param int adresse sur laquelle recevoir les donnees
 */
void i2creceive2(int adresse)
{
  Wire.begin(adresse);           // Joindre le Bus I2C avec adresse
  Wire.onReceive(receiveEvent3Bytes); // enregistrer l'événement (lorsqu'une demande arrive)
}


/**
 * \fn void i2creceive3Bytes(int adresse, int nbOctets)
 * \brief fonction qui reçoit des octets
 * \param int adresse - adresse sur laquelle recevoir les donnees
 * \param int nbOctets - nombre d'octets à recevoir
 */
void i2creceive3Bytes(int adresse, int nbOctets)
{
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
  i2creceive2(_RECEIVEADRESS_);

  //recoverIntFrom2Bytes(bytesTab);

  delay(800);
}



#include <Wire.h>
#include "i2cCommunication.h"

#define _RECEIVEADRESS_ 8


//_____________________________________________________________________________________________
//_____________________________________________________________________________________________

/**
 * \fn void affiche()
 */
void affiche()
{
  Serial.println("affiche!");
}

/**
 * \fn void affiche()
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
  while(2 < Wire.available()) // Lire tous les octets sauf le dernier => utile si on a écrit qqch devant le numéro de l'ordre mais il vaut mieux ne pas le faire pour éviter les pb
  {
    char c = Wire.read();     // lecture de l'octet/byte comme caractère
    Serial.print(c);          // afficher le caractère
  }
  byte x = Wire.read();        // lecture de l'octet/byte ignoré comme un entier
  orderNumber(x);			  // lecture de l'ordre à executer
  //byte y = Wire.read(); 
  //orderNumber(y);
  //byteReceived(x);
}


/**
 * \fn void i2creceive(int adresse) - fonction de lecture de données reçues via l'i2c
 * \param int adresse sur laquelle recevoir les donnees
 */
void i2creceive2(int adresse)
{
  Wire.begin(adresse);          // Joindre le Bus I2C avec adresse
  Serial.println("===receive2_avant_onReceive===");
  Wire.onReceive(receiveEvent2); // enregistrer l'événement (lorsqu'une demande arrive)
  Serial.println("===receive2_apres_onReceive===");
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
  i2creceive2(_RECEIVEADRESS_);
  i2creceive2(_RECEIVEADRESS_);

  delay(500);
}



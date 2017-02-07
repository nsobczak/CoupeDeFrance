/**
 *    \file Test_I2C.cpp
 *    \brief Code de test de l'i2c
 *    \author Nicolas Sobczak
 *    \date février 2017
 */
//____________________________________________________________________________________________________
// I2C
#include <Wire.h>
#include "i2cCommunication.h"

#define _RECEIVEADRESS_ 12
#define _SENDADRESS_ 13

#define LED_1 26
#define LED_2 28  // ne fera jamais rien car n'est pas encore soudée



//_____________________________________________________________________________________________
//_____________________________________________________________________________________________
//Définition des variables globales
int allumerLED;
int value;

/*Tableau de correspondance des variables
   0 => allumerLED
   1 =>
   2 =>
   3 =>
   4 =>
   5 =>
 */


//____________________________________________________________________________________________________
//____________________________________________________________________________________________________
// Reception I2C
/**
 * \fn void receiveEvent(int howMany - fonction qui est exécutée lorsque des données sont envoyées par le Maître. Cette fonction est enregistrée comme un événement ("event" en anglais), voir la fonction setup()
 * \param int howMany
 */
void receiveEvent2(int howMany)
{
        Serial.println("receiveEvent");

        if (Wire.available() == 3)
        {
                //lecture de la variable
                byte var = Wire.read();
                //lecture des 2 octets suivants
                byte x = Wire.read();
                byte y = Wire.read();
                //reconstitution de la valeur
                byte bytesTab[2] = {x, y};
                int value = recoverIntFrom2Bytes(bytesTab);

                switch ( var ) // cf. les références des variables en haut du fichier
                {
                case 0:
                        Serial.println("variable recue : allumerLED");
                        allumerLED = value-1;
                        break;
                default:
                        Serial.println("variable recue inconnue");
                }

        }
        // else de debug
        else
        {
                Serial.println("Erreur : Pas 3 octets envoyes");
        }
}


/**
 * \fn void i2creceive(int adresse) - fonction de lecture de données reçues via l'i2c
 * \param int adresse sur laquelle recevoir les donnees
 */
void i2creceive2(int adresse)
{
        Serial.println("i2creceive2");
        Wire.begin(adresse);     // Joindre le Bus I2C avec adresse
        Wire.onReceive(receiveEvent2); // enregistrer l'événement (lorsqu'une demande arrive)
        Wire.endTransmission(); // fin transmission
}


//____________________________________________________________________________________________________
//____________________________________________________________________________________________________
/**
 * \fn void setup()
 * \brief fonction setup d'arduino
 */
void setup()
{
        pinMode(LED_1, OUTPUT);                  //Vérifie que tout a bien été téléversé
        pinMode(LED_2, OUTPUT);             //Enable | Activé si la pin est à l'état "LOW" desactivé si elle est à l'état "HIGH" MOTEUR X

        allumerLED = -1;
        value = 0;

        Serial.begin(9600);
}


/**
 * \fn void loop()
 * \brief fonction loop d'arduino
 */
void loop()
{
        // i2creceive2(_RECEIVEADRESS_);
        //
        // if ((allumerLED >= 0) && (allumerLED <= 1))
        // {
        //         Serial.println("allumerLED recue");
        //         ledControl(LED_1, allumerLED);
        // }


        // conversion sur 2 octets de la valeur à envoyer
        byte bytesTab[2];
        intTo2Bytes(bytesTab, value);
        // envoi une fois que l'initialisation est terminée
        i2csend3bytes(1, bytesTab[0], bytesTab[1], _SENDADRESS_);
        
        value += 1;
        if (value == 2) {value = 0; }

        // i2csend3bytes(1, 1, 1, _SENDADRESS_);

        delay(1000);
}

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
#include "i2cCommunication.h"


/* ======================================================================================================
 *      Fonctions
 * ======================================================================================================
 */
//_____________________________________________________________________________________________
//_____________________________________________________________________________________________
// LED : ordre 0 ou 1

/**
 * \fn void ledOff(int pin)
 * \brief fonction qui eteint une LED
 * \param int pin de la LED
 */
void ledOff(int pin)
{
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
}


/**
 * \fn void ledOn(int pin)
 * \brief fonction qui allume une LED
 * \param int pin de la LED
 */
void ledOn(int pin)
{
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);
}


/**
 * \fn void ledControl(int pin, int order)
 * \brief fonction qui eteint une LED
 * \param int pin de la LED,
 * \param int order =1 pour allumer ou =0 pour eteindre
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
 * \fn void byteReceived(byte octet)
 * \brief fonction qui affiche l'octet reçu
 * \param byte octet
 */
void byteReceived(byte octet)
{
        Serial.print("ReceivedByte: ");
        Serial.println(octet); // Afficher la valeur numérique
}


/**
 * \fn void orderNumber(uint8_t order)
 * \brief fonction qui execute l'ordre dont le numéro est entrée en parametre
 * \param uint8_t order = numero de l'ordre a executer
 */
void orderNumber(uint8_t order)
{
        Serial.print("ReceivedOrder: ");
        Serial.println(order); // Afficher la valeur numérique
        switch (order)
        {
        case 0:
        {
                int pin = 12;
                ledControl(pin, order);    // Allume ou éteint la led
                break;
        }
        case 1:
        {
                int pin = 12;
                ledControl(pin, order);    // Allume ou éteint la led
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


/**
 * \fn void changeData(byte data[])
 * \brief Fonction qui change la variable du tableau correspondace envoyée
 * \param byte data[]
 */
void changeData(byte data[], int correspondance[], int numberOfVariables)
{
        byte value[2]; // tableau pour stocker la variable

        //Changement de l'indice du tableau correspondance
        int variable = data[0] + 1;
        if (variable == numberOfVariables)
        {
                variable = 0;
        }
        intTo2Bytes(value, correspondance[variable]);

        // Write the value of variables to the array
        data[0] = variable;
        data[1] = value[0];
        data[2] = value[1];
}


//_____________________________________________________________________________________________
// Reception

/**
 * \fn void receiveEvent(int howMany)
 * \brief fonction qui est exécutée lorsque des données sont envoyées par le Maître. Cette fonction est enregistrée comme un événement ("event" en anglais), voir la fonction setup()
 * \param int howMany
 */
void receiveEvent(int howMany)
{
        while(1 < Wire.available()) // Lire tous les octets sauf le dernier => utile si on a écrit qqch devant le numéro de l'ordre mais il vaut mieux ne pas le faire pour éviter les pb
        {
                char c = Wire.read(); // lecture de l'octet/byte comme caractère
                Serial.print(c); // afficher le caractère
        }
        byte x = Wire.read(); // lecture de l'octet/byte ignoré comme un entier
        orderNumber(x); // lecture de l'ordre à executer
}


/**
 * \fn void i2creceive(int adresse)
 * \brief fonction de lecture de données reçues via l'i2c
 * \param int adresse sur laquelle recevoir les donnees
 */
void i2creceive(int adresse)
{
        Wire.begin(adresse);    // Joindre le Bus I2C avec adresse
        Wire.onReceive(receiveEvent); // enregistrer l'événement (lorsqu'une demande arrive)
        Wire.endTransmission(); // fin transmission
}



//_____________________________________________________________________________________________
// Envoi
/**
 * \fn void i2csend(uint8_t order, int adresse)
 * \brief fonction d'envoi d'1 octet via l'i2c
 * \param uint8_t order numero de l'ordre a envoyer,
 * \param int adresse sur laquelle envoyer les donnees
 */
void i2csend(uint8_t order, int adresse)
{
        Wire.begin();     // joindre le bus i2c (adresse est optionnelle pour un maître)
        Wire.beginTransmission(adresse); // Commencer transmission vers l'esclave  #4
        //Wire.write("order: ");          // Envoi de 5 octets (5 bytes)
        Wire.write(order);        // envoi d'un byte/octet (valeur numérique)
        Serial.print("Envoi via i2c de : ");
        Serial.print(order);
        Serial.print(" a l'adresse : ");
        Serial.println(adresse);
        Wire.endTransmission(); // fin transmission
}


/**
 * \fn void i2csend3bytes(uint8_t byte1, uint8_t byte2, uint8_t byte3, int adresse)
 * \brief fonction d'envoi de 3 octets via l'i2c avec initialisation de l'I2C
 * \param 3 uint8_t byte1, 2 et 3: octets a envoyer,
 * \param int adresse sur laquelle envoyer les donnees
 */
void i2csend3bytes(uint8_t byte1, uint8_t byte2, uint8_t byte3, int adresse)
{
        Wire.begin();     // joindre le bus i2c (adresse est optionnelle pour un maître)
        Wire.beginTransmission(adresse); // Commencer transmission vers l'esclave
        Wire.write(byte1);        // envoi d'un byte/octet (valeur numérique)
        Wire.write(byte2);
        Wire.write(byte3);
        // Serial.print("Envoi via i2c de : ");
        // Serial.print(byte1);
        // Serial.print(", ");
        // Serial.print(byte2);
        // Serial.print(" et ");
        // Serial.print(byte3);
        // Serial.print(" a l'adresse ");
        // Serial.println(adresse);
        Wire.endTransmission(); // fin transmission
}


/**
 * \fn void i2csend3bytes(uint8_t byte1, uint8_t byte2, uint8_t byte3, int adresse)
 * \brief fonction d'envoi de 3 octets via l'i2c avec initialisation de l'I2C
 * \param 3 uint8_t byte1, 2 et 3: octets a envoyer,
 * \param int adresse sur laquelle envoyer les donnees
 */
void i2csend5bytes(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, uint8_t byte5, int adresse)
{
        Wire.begin();     // joindre le bus i2c (adresse est optionnelle pour un maître)
        Wire.beginTransmission(adresse); // Commencer transmission vers l'esclave
        Wire.write(byte1);        // envoi d'un byte/octet (valeur numérique)
        Wire.write(byte2);
        Wire.write(byte3);
        Wire.write(byte4);
        Wire.write(byte5);
        Wire.endTransmission(); // fin transmission
}



//_____________________________________________________________________________________________
// Request
/**
 * \fn void i2crequest(int adresse, int nbBytes, int variable)
 * \brief fonction qui demande l'envoi d'une certaine variable à un esclave
 * \param uint8_t order numero de l'ordre a envoyer,
 * \param int adresse sur laquelle envoyer les donnees
 * \param int variable à envoyer
 */
byte* i2crequest(int adresse, int nbBytes, int variable, int numberOfVariables)
{
        byte* dataI2C = new byte[nbBytes];

        do
        {
                // request 3 bytes from slave device on adress adresse
                Wire.requestFrom(adresse, nbBytes);

                while (Wire.available()) //check if data is available
                {
                        for(byte i = 0; i < nbBytes; i++)
                        {
                                dataI2C[i] = Wire.read(); // it assigne the data to the array
                        }
                }

                delay(100);

        } while ((dataI2C[0] != variable) && (dataI2C[0] < numberOfVariables - 1));

        return dataI2C;
}


//_____________________________________________________________________________________________
// Conversion
/**
 * \fn byte getLowByte(int n)
 * \brief fonction qui convertit un entier en 2 bytes - low ici
 * \param 1 tableau de bytes
 */
byte getLowByte(int n)
{
        byte result;

        result = n%256;
        result = byte(result);

        return result;
}


/**
 * \fn byte getHighByte(int n)
 * \brief fonction qui convertit un entier en 2 bytes - high ici
 * \param 1 tableau de byte
 */
byte getHighByte(int n)
{
        byte result;

        result = n/256;
        result = byte(result);

        return result;
}


/**
 * \fn void intTo2Bytes(byte bytesTab[], int n)
 * \brief fonction qui affiche les 2 bytes d'un nombre entier converti en binaire
 * \param 1 tableau de byte
 * \param 1 entier : nombre à convertir
 */
void intTo2Bytes(byte bytesTab[], int n)
{
        Serial.println("high byte.low byte : ");

        bytesTab[0] = getHighByte(n);
        bytesTab[1] = getLowByte(n);
        Serial.print(bytesTab[0], HEX);
        Serial.print(".");
        Serial.println(bytesTab[1], HEX);
}


/**
 * \fn int recoverIntFrom2Bytes(byte bytesTab[])
 * \brief fonction qui affiche un entier ayant ete converti en 2 bytes binaire
 * \param 1 tableau de byte
 * \param 1 entier : nombre à convertir
 */
int recoverIntFrom2Bytes(byte bytesTab[])
{
        int result;

        result = bytesTab[0]*256;
        result += bytesTab[1];

        return result;
}


//_____________________________________________________________________________________________
//_____________________________________________________________________________________________

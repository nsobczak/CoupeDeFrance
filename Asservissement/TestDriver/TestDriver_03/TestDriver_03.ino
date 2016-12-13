/**
 *    \file TestDriver_03.ino
 *    \brief test pour faire fonctionner les moteurs du robot et donc faire rouler le robot
 *
 *    \author Arthur Duytschaever et Nicolas Sobczak
 *    \date Decembre 2016
 */
//_______________________________________________________________________________________________________
//_______________________________________________________________________________________________________
/* ======================================================================================================
 *      Include
 * ======================================================================================================
 */
// I2C
#include <Wire.h>
#include "i2cCommunication.h"


/* ======================================================================================================
 *      Define
 * ======================================================================================================
 */
// I2C
#define _RECEIVEADRESS_ 12
#define _SENDADRESS_ 13


/* ======================================================================================================
 *      Variables globales
 * ======================================================================================================
 */
int MotorR =3; // Attention sur Due PWM ou Pwm sont des keyword -> donc ne pas les utiliser pour des nom de variable
int MotorL = 6; 
int IN1MotorL = 22;
int IN2MotorL = 21;
int IN1MotorR = 52;
int IN2MotorR = 53;

int demarrerMoteurRobot = -1;
int startAfter5Sec = -1;
/*Tableau de correspondance des variables
0 => demarrerMoteurRobot
1 => StartAfter5Sec
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
    
    switch ( var )  // cf. les références des variables en haut du fichier
    {
       case 0:  
          Serial.println("variable recue : demarrerMoteurRobot");
          demarrerMoteurRobot = value;
          break;
       case 1:
          Serial.println("variable recue : StartAfter5Sec");
          startAfter5Sec = value; 
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
  Wire.begin(adresse);           // Joindre le Bus I2C avec adresse
  Wire.onReceive(receiveEvent2); // enregistrer l'événement (lorsqu'une demande arrive)
  Wire.endTransmission();       // fin transmission
}


//_______________________________________________________________________________________________________
/**
 * \fn void startMoteur()
 * \brief fonction qui démarre le moteur
 */
void startMoteur() {
  /*digitalWrite(IN1MotorR,LOW);
  digitalWrite(IN2MotorR,LOW);
  digitalWrite(IN1MotorL,LOW);
  digitalWrite(IN2MotorL,LOW);
  analogWrite(MotorR,10);
  analogWrite(MotorL,10);*/
  //delay(500);
  digitalWrite(IN1MotorR,HIGH);
  digitalWrite(IN2MotorR,LOW);
  digitalWrite(IN1MotorL,HIGH);
  digitalWrite(IN2MotorL,LOW);
  analogWrite(MotorR,10);
  analogWrite(MotorL,10);
  /*delay(1000);
  digitalWrite(IN1MotorR,LOW);
  digitalWrite(IN2MotorR,LOW);
  digitalWrite(IN1MotorL,LOW);
  digitalWrite(IN2MotorL,LOW);
  analogWrite(MotorR,10);
  analogWrite(MotorL,10);
  delay(500);
  digitalWrite(IN1MotorR,HIGH);
  digitalWrite(IN2MotorR,LOW);
  digitalWrite(IN1MotorL,HIGH);
  digitalWrite(IN2MotorL,LOW);
  analogWrite(MotorR,10);
  analogWrite(MotorL,10);
  delay(1000);*/
}


//_______________________________________________________________________________________________________
//_______________________________________________________________________________________________________
/**
 * \fn void setup()
 * \brief fonction setup d'arduino
 */
void setup() {
  pinMode(MotorR,OUTPUT);
  pinMode(MotorL,OUTPUT);
  pinMode(IN1MotorR,OUTPUT);
  pinMode(IN2MotorR,OUTPUT);
  pinMode(IN1MotorL,OUTPUT);
  pinMode(IN2MotorL,OUTPUT);
  //analogWriteResolution(8);// seulement sur Arduino Due pour gere la Resolution du convertisseur DAC 
}


/**
 * \fn void loop()
 * \brief fonction loop d'arduino
 */
void loop() {
    i2creceive2(_RECEIVEADRESS_);
    
    if ((demarrerMoteurRobot > 0) && (demarrerMoteurRobot < 5))
    {
      startMoteur();
      /*
      // conversion sur 2 octets de la valeur à envoyer
      byte bytesTab[2];
      intTo2Bytes(bytesTab, finInitialisation);     
      // envoi une fois que l'initialisation est terminée
      i2csend3bytes(1, bytesTab[0], bytesTab[1], _SENDADRESS_); 
      
      //finInitialisation = 0;
      demarrerMoteurRobot = -1;
      */
    }
}

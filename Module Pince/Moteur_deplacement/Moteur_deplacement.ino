/**
 *    \file Moteur_deplacement.cpp
 *    \brief Code de déplacement de la pince
 *    \author Olivier Jombart
 *    \date décembre 2016
*/
//____________________________________________________________________________________________________
// I2C
#include <Wire.h>
#include "i2cCommunication.h"


//____________________________________________________________________________________________________
// I2C
#define _RECEIVEADRESS_ 10
#define _SENDADRESS_ 11


// Shield - For RAMPS 1.4
#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#define X_MIN_PIN           3
#define X_MAX_PIN           2

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_MIN_PIN          14
#define Y_MAX_PIN          15

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19

#define E_STEP_PIN         26
#define E_DIR_PIN          28
#define E_ENABLE_PIN       24

#define SDPOWER            -1
#define SDSS               53
#define LED_PIN            13

#define FAN_PIN            9

#define PS_ON_PIN          12
#define KILL_PIN           -1

#define HEATER_0_PIN       10
#define HEATER_1_PIN       8
#define TEMP_0_PIN          13   // ANALOG NUMBERING
#define TEMP_1_PIN          14   // ANALOG NUMBERING

#include <Servo.h>
#include <Stepper.h>
#include <math.h>



//_____________________________________________________________________________________________
//_____________________________________________________________________________________________
//Définition des variables globales
int index;
int demarrerMoteur;
int finInitialisation;
volatile byte state = LOW;
int pin_capteur = A14;                
float c;
float d;
int isInt = 0;

Stepper motor_X(200, 54, 55);
Stepper motor_Y(200, 60, 61);
Stepper motor_Z(200, 46, 48);
Servo servo_capture;        
Servo servo_rotation;



//____________________________________________________________________________________________________
//____________________________________________________________________________________________________
// Déplacement du rail



void rail_initialisation(int tour) // 800 sans cavalier = 1 Tour complet
{ 
    digitalWrite(Y_ENABLE_PIN,LOW); 
    motor_Y.step(tour);
    Serial.println("Mouvement moteur Y à gauche");           
}



void rail_interruption_initialisation_gauche()
{
    digitalWrite(Z_ENABLE_PIN,HIGH);           
    Serial.println("STOP Y GAUCHE");
    delay(1000);
    digitalWrite(Z_ENABLE_PIN,LOW);
    deplacement_leger_droit();     
  }


void deplacement_leger_droit() {
   motor_Y.step(-800);
   Serial.println("Fin initialisation moteur Y");
   delay(1000);
}

//____________________________________________________________________________________________________
//____________________________________________________________________________________________________
// Déplacement du vis sans fin


void remonter_legere() 
{
    motor_Z.step(-800);
    Serial.println("Moteurs Z en mouvement");  
}


void monter_descente_initialisation(int tour)// 800 sans cavalier = 1 Tour complet
{
    digitalWrite(Z_ENABLE_PIN,LOW);           
    digitalWrite(Z_DIR_PIN,LOW);
    motor_Z.step(tour);
    Serial.println("Moteur Z en mouvement");             
}

 
void interruption_descente_Z(){
    //digitalWrite(Z_ENABLE_PIN,HIGH);          
    Serial.println("STOP_Z");
    //remonter_legere(); 
}


//____________________________________________________________________________________________________
//____________________________________________________________________________________________________
// Gestion de la pince

/**
 * \fn void attraper_cylindre(int angle_fermeture, int angle_rotation_droite,int temps)
 * \brief, Fn qui permet d'attraper et de retourner verticalement le cylindre
 */
 
void attraper_cylindre(int angle_fermeture, int angle_rotation_droite,int temps){  
    servo_capture.write(angle_fermeture);                              // la pince se ferme (100)
    delay(temps);                      
    servo_rotation.write(angle_rotation_droite);                       // rotation de la pince vers la droite (20)
    delay(temps);      
  
}

/**
 * \fn void relacher_cylindre(int angle_ouverture, int angle_rotation_initial, int temps)
 * \brief, Fn qui permet de relacher le cylindre après sa capture, la pince revient à son état inital (ouverte)
 */
 
void relacher_cylindre(int angle_ouverture, int angle_rotation_initial, int temps) {
    servo_capture.write(angle_ouverture);                             // la pince s'ouvre (140)
    delay(temps);
    servo_rotation.write(angle_rotation_initial);                     // rotation à l'état initial de la pince (80)
    delay(temps);
}

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
          Serial.println("variable recue : index");
          index = value;
          break;  
       case 1:  
          Serial.println("variable recue : demarrerMoteur");
          demarrerMoteur = value;
          break;  
       case 2:  
          Serial.println("variable recue : finInitialisation");
          finInitialisation = value;
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

 
/**
 * \fn void loop()
 * \brief fonction loop d'arduino
 */
void i2creceive2(int adresse)
{
  Wire.begin(adresse);           // Joindre le Bus I2C avec adresse
  Wire.onReceive(receiveEvent2); // enregistrer l'événement (lorsqu'une demande arrive)
  Wire.endTransmission();    	 // fin transmission
}


//____________________________________________________________________________________________________
//____________________________________________________________________________________________________
/**
 * \fn void setup()
 * \brief fonction setup d'arduino
 */
void setup() 
{
      
     //pinMode(X_ENABLE_PIN, OUTPUT);               //Enable | Activé si la pin est à l'état "LOW" desactivé si elle est à l'état "HIGH" MOTEUR X
     // attachInterrupt(digitalPinToInterrupt(X_MIN_PIN),interruption_descente_x,LOW);
    
     //motor_X.setSpeed(1000);
      pinMode(Y_ENABLE_PIN, OUTPUT);               //Enable | Activé si la pin est à l'état "LOW" desactivé si elle est à l'état "HIGH" MOTEUR Y
     // attachInterrupt(digitalPinToInterrupt(Y_MIN_PIN),interruption_droite,LOW);
     // attachInterrupt(digitalPinToInterrupt(Y_MAX_PIN),interruption_gauche,LOW);
      motor_Y.setSpeed(1000);
      pinMode(Z_DIR_PIN,OUTPUT);
      pinMode(Z_ENABLE_PIN, OUTPUT);               //Enable | Activé si la pin est à l'état "LOW" desactivé si elle est à l'état "HIGH" MOTEUR X
      pinMode(Z_MIN_PIN,INPUT);
      attachInterrupt(digitalPinToInterrupt(Z_MIN_PIN),interruption_descente_Z,LOW);
      motor_Z.setSpeed(1000);
      servo_rotation.attach(4);                    // attaches the servo on pin 3 to the servo object
      servo_capture.attach(5);
      demarrerMoteur = 0;
      finInitialisation = 0;
      
      Serial.begin(9600);
}

/**
 * \fn void loop()
 * \brief fonction loop d'arduino
 */
void loop() 
{   
    
    
 //  attraper_cylindre(160,75,1000);           //angle à respecter
  // relacher_cylindre(80,120,1000);           //angle à respecter
//  digitalWrite(LED_PIN,HIGH);
//  delay(1000);
//  digitalWrite(LED_PIN,LOW);
//  delay(1000);
  
   monter_descente_initialisation(3600);
 //  delay(1000);
 //  monter_descente_initialisation(-1600);
//   delay(1000);
//   rail_initialisation(-800);

//    delay(500); 
//    i2creceive2(_RECEIVEADRESS_);
    
//      if (demarrerMoteur == 1)
//      {
//        initialisation();
//  
//        // conversion sur 2 octets de la valeur à envoyer
//        byte bytesTab[2];
//        intTo2Bytes(bytesTab, finInitialisation);     
//        // envoi une fois que l'initialisation est terminée
//        i2csend3bytes(1, bytesTab[0], bytesTab[1], _SENDADRESS_); 
//        
//        //finInitialisation = 0;
//        demarrerMoteur = 0;
//      }
    
}



  


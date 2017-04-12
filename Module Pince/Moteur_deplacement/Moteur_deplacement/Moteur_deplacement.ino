/**
 *    \file Moteur_deplacement.cpp
 *    \brief Code de déplacement de la pince
 *    \author Olivier Jombart
 *    \date décembre 2016
 */
//____________________________________________________________________________________________________
// Initialisation
#include <Arduino.h>
#include <Wire.h>
#include "i2cCommunication.h"

#include <Servo.h>
#include <Stepper.h>
#include <math.h>

#define _DEBUG_ true

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


//_____________________________________________________________________________________________
//_____________________________________________________________________________________________
//Définition des variables globales
int index;
int demarrerMoteur;
int finInitialisation;
volatile byte state = LOW;

int isInt = 0;
int initt=0;
int descente_init=0;
int rail_init=0;
bool etat=true;
bool test=false;

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
        delay(1000);
        digitalWrite(X_ENABLE_PIN,LOW);
        motor_X.step(tour);
        if (_DEBUG_) Serial.println("Mouvement moteur X ");
}


void rail_interruption_gauche()
{
        digitalWrite(X_ENABLE_PIN,HIGH);
        if (_DEBUG_) Serial.println("STOP X GAUCHE");
        for(int i=0; i<100; i++) {
                delayMicroseconds(500);
                if (_DEBUG_) Serial.println("Capteur gauche on");
        }
        rail_initialisation(-400);
}


void rail_interruption_droit()
{
        digitalWrite(X_ENABLE_PIN,HIGH);
        if (_DEBUG_) Serial.println("STOP X DROIT");
        for(int i=0; i<50; i++) {
                delayMicroseconds(500);
                if (_DEBUG_) Serial.println("Capteur droit on");
        }
        rail_initialisation(400);
}


//____________________________________________________________________________________________________
//____________________________________________________________________________________________________
// Déplacement du vis sans fin

void monter_descente_initialisation(long tour) // 800 sans cavalier = 1 Tour complet
{

        digitalWrite(Z_ENABLE_PIN,LOW);
        digitalWrite(Z_DIR_PIN,LOW);
        motor_Z.step(tour);
        if (_DEBUG_) Serial.println("Moteur Z en mouvement");
}

void interruption_descente_Z()
{
        digitalWrite(Z_ENABLE_PIN,HIGH);
        if (_DEBUG_) Serial.println("STOP_Z");
        for(int i=0; i<50; i++) {
                delayMicroseconds(500);
                if (_DEBUG_) Serial.println("Butee descente Z actionnee");
        }
        if (_DEBUG_) Serial.println("Sortie Boucle");
        isInt = 1;
}


//____________________________________________________________________________________________________
//____________________________________________________________________________________________________
// Gestion de la pince

/**
 * \fn void attraper_cylindre
 * \param int angle_fermeture, int angle_rotation_droite, int temps
 * \brief, Fn qui permet d'attraper et de retourner verticalement le cylindre
 */
void attraper_cylindre(int angle_fermeture, int angle_rotation_droite, int temps)
{
        servo_capture.write(angle_fermeture);                          // la pince se ferme (100)
        delay(temps);
        servo_rotation.write(angle_rotation_droite);                   // rotation de la pince vers la droite (20)
        delay(temps);

}

/**
 * \fn void relacher_cylindre
 * \param int angle_ouverture, int angle_rotation_initial, int temps
 * \brief, fonction qui permet de relacher le cylindre après sa capture, la pince revient à son état inital (ouverte)
 */

void relacher_cylindre(int angle_ouverture, int angle_rotation_initial, int temps)
{
        servo_capture.write(angle_ouverture);                         // la pince s'ouvre (140)
        delay(temps);
        servo_rotation.write(angle_rotation_initial);                 // rotation à l'état initial de la pince (80)
        delay(temps);
}


//____________________________________________________________________________________________________
//___________________________________________________________________________________________________
//3 fonctions permettant de piloter la pince
/**
 * fonction nous permettant d'initialiser le mécanisme pince au démarrage du systeme
 */
void initialisation_pince(bool etat)
{

        while(isInt == 0) {
                monter_descente_initialisation(400);
        }
        isInt = 0;
        monter_descente_initialisation(-1600);

        if (_DEBUG_) Serial.println("Initialisation de l'axe z : done");
        //rail_initialisation(800);                            Comme on ne s'en sert pas je l'ai mis en commentaire
        //Serial.println("Initialisation de l'axe x : done");
        //digitalWrite(Z_ENABLE_PIN,HIGH);
        relacher_cylindre(170,120,1000);
        if (_DEBUG_)
        {
                Serial.println("Initialisation de la pince : done");
                Serial.println("Position pince en Z = 0");
                Serial.println("Fin de l'initialisation de la pince");
        }
}

/**
 * fonction permettant la capture du cylindre
 */
void capture_cylindre_pince (bool etat)
{
        while(etat!=false) {
                attraper_cylindre(170,60,1000);
                if (_DEBUG_) Serial.println("Cylindre attrape");
                monter_descente_initialisation(-10800);
                if (_DEBUG_) Serial.println("Pince monte le cylindre");
                attraper_cylindre(80,60,1000);
                if (_DEBUG_) Serial.println("Cylindre a l'horizontal");
                etat=false;
        }
        if (_DEBUG_) Serial.println("Fin de la capture du cylindre");
}

/**
 * Fonction permettant de relacher le cylindre
 */
void relacher_cylindre_pince (bool etat)
{
        while(etat!=false) {
                monter_descente_initialisation(10800);
                if (_DEBUG_) Serial.println("Pince descend le cylindre");
                relacher_cylindre(80,120,1000);
                if (_DEBUG_) Serial.println("Cylindre relacher");
                etat=false;
                relacher_cylindre(170,120,1000);
                if (_DEBUG_) Serial.println("Pince a son état initial");
        }
        if (_DEBUG_) Serial.println("Fin du relachement du cylindre");
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

                switch ( var ) // cf. les références des variables en haut du fichier
                {
                case 0:
                        if (_DEBUG_) Serial.println("variable recue : index");
                        index = value;
                        break;
                case 1:
                        if (_DEBUG_) Serial.println("variable recue : demarrerMoteur");
                        demarrerMoteur = value;
                        break;
                case 2:
                        if (_DEBUG_) Serial.println("variable recue : finInitialisation");
                        finInitialisation = value;
                        break;
                default:
                        if (_DEBUG_) Serial.println("variable recue inconnue");
                }

        }
        // else de debug
        else
        {
                if (_DEBUG_) Serial.println("Erreur : Pas 3 octets envoyes");
        }
}


/**
 * \fn void loop()
 * \brief fonction loop d'arduino
 */
void i2creceive2(int adresse)
{
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
        motor_X.setSpeed(500);
        pinMode(X_ENABLE_PIN, OUTPUT);
        pinMode(X_MIN_PIN,OUTPUT);
        //attachInterrupt(digitalPinToInterrupt(X_MIN_PIN),rail_interruption_gauche,LOW); // utile pour le rail
        pinMode(X_MAX_PIN,OUTPUT);
        //attachInterrupt(digitalPinToInterrupt(X_MAX_PIN),rail_interruption_droit,LOW); // utile pour le rail

        pinMode(Z_DIR_PIN,OUTPUT);
        pinMode(Z_ENABLE_PIN, OUTPUT);             //Enable | Activé si la pin est à l'état "LOW" desactivé si elle est à l'état "HIGH" MOTEUR X
        pinMode(Z_MIN_PIN,INPUT);
        attachInterrupt(digitalPinToInterrupt(Z_MIN_PIN), interruption_descente_Z,FALLING);
        motor_Z.setSpeed(1000);
        servo_rotation.attach(4);
        servo_capture.attach(5);
        demarrerMoteur = 0;
        finInitialisation = 0;

        Serial.begin(9600);
}

//____________________________________________________________________________________________________
//____________________________________________________________________________________________________
/**
 * \fn void loop()
 * \brief fonction loop d'arduino
 */
void loop()
{
        boolean Var = true;

        initialisation_pince(etat);
        capture_cylindre_pince(etat);
        relacher_cylindre_pince(etat);
        while(Var == true) motor_Z.step(0);
        //digitalWrite(Z_ENABLE_PIN,HIGH);
}

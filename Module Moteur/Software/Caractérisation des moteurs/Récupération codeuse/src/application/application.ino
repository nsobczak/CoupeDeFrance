/**
 *    \file application.ino
 *    \brief asservissement vitesse
 *
 *    \author Arthur Duytschaever & Nicolas SOBCZAK
 *    \date Février 2017
 */
//_______________________________________________________________________________________________________
/* ======================================================================================================
 *      Include
 * ======================================================================================================
 */
#include <Arduino.h>


/* ======================================================================================================
 *      Variables globales
 * ======================================================================================================
 */

#define encoder0PinA_L 13   //encodeur gauche A
#define encoder0PinB_L 12   //encodeur gauche B
#define encoder0PinA_R 11   //encodeur droit A
#define encoder0PinB_R 10   //encodeur droit B

int MotorR =3; // Attention sur Due PWM ou Pwm sont des keyword -> donc ne pas les utiliser pour des nom de variable
int MotorL = 6;
int IN1MotorL = 22;
int IN2MotorL = 23;
int IN1MotorR = 52;
int IN2MotorR = 53;

const int _MOTEUR =  9;            // Digital pin pour commande moteur
unsigned int tick_codeuse = 0;     // Compteur de tick de la codeuse
uint32_t timer;


/* ======================================================================================================
 *      Fonctions
 * ======================================================================================================
 */

/* Routine d'initialisation */
void setup() {
    Serial.begin(115200);
    pinMode(MotorR,OUTPUT);
    pinMode(MotorL,OUTPUT);
    pinMode(IN1MotorR,OUTPUT);
    pinMode(IN2MotorR,OUTPUT);
    pinMode(IN1MotorL,OUTPUT);
    pinMode(IN2MotorL,OUTPUT);

    pinMode(encoder0PinA_L, INPUT);
    pinMode(encoder0PinB_L, INPUT);
    pinMode(encoder0PinA_R, INPUT);
    pinMode(encoder0PinB_R, INPUT);

    pinMode(_MOTEUR, OUTPUT);   // Sortie moteur
    analogWrite(_MOTEUR, 255);  // Sortie moteur à 0
    delay(5000);                // Pause de 5 sec pour laisser le temps au moteur de s'arréter si celui-ci est en marche

    attachInterrupt(encoder0PinA_L, compteur, CHANGE);    // Interruption sur tick de la codeuse (interruption 0 = pin2 arduino mega)
    timer = millis();
}

/* Fonction principale */
void loop(){
    if (millis() - timer > 5000)
    {
      Serial.print("\t tick_codeuse : \t");
      Serial.println(tick_codeuse);
      timer = millis();
      // delay(10);
    }
}

/* Interruption sur tick de la codeuse */
void compteur(){
    tick_codeuse++;  // On incrémente le nombre de tick de la codeuse
}

/**
 *    \file caraterisationVitesseMoteur.ino
 *    \brief asservissement vitesse
 *
 *    \author Arthur Duytschaever et Nicolas Sobczak
 *    \date Février 2017
 */
//_______________________________________________________________________________________________________
/* ======================================================================================================
 *      Include
 * ======================================================================================================
 */
#include <Arduino.h>
#include <SimpleTimer.h>           // http://arduino.cc/playground/Code/SimpleTimer


/* ======================================================================================================
 *      Variables globales
 * ======================================================================================================
 */

#define _DEBUG true
#define encoder0PinA_L 13   //encodeur gauche A
#define encoder0PinB_L 12   //encodeur gauche B
#define encoder0PinA_R 11   //encodeur droit A
#define encoder0PinB_R 10   //encodeur droit B

#define MotorR 6 // Attention sur Due PWM ou Pwm sont des keyword -> donc ne pas les utiliser pour des nom de variable
#define MotorL 3
#define IN1MotorR 22
#define IN2MotorR 23
#define IN1MotorL 52
#define IN2MotorL 53

SimpleTimer timer;                 // Timer pour échantillonnage
unsigned int tick_codeuse_R = 0;   // Compteur de tick de la codeuse
unsigned int tick_codeuse_L = 0;   // Compteur de tick de la codeuse

int cmd = 0;                       // Commande du moteur


const int tick_par_tour_codeuse = 2500;      // Nombre de tick codeuse par tour de roue codeuse
const int tick_par_tour_non_codeuse = 3836;      // Nombre de tick codeuse par tour de roue non codeuse
const float rapport_roueCodeuse_roueNonCodeuse = (52.28)/(80.22);

int i; //Arthur pwm
unsigned long duration;


/* ======================================================================================================
 *      Fonctions
 * ======================================================================================================
 */

/**
 * \fn void setup()
 * \brief fonction setup d'arduino
 */
void setup()
{
        Serial.begin(115200);     // Initialisation port COM
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

        

        attachInterrupt(encoder0PinA_R, compteur_tick_R, CHANGE);    // Interruption sur tick de la codeuse (interruption 0 = pin2 arduino mega)
        //attachInterrupt(encoder0PinA_L, compteur_tick_L, CHANGE); // Interruption sur tick de la codeuse (interruption 0 = pin2 arduino mega)
        
        i = 255;
        
        digitalWrite(IN1MotorL, LOW);
        digitalWrite(IN2MotorL, LOW);
        analogWrite(MotorL,255);
        
        //delay(1000);              // Pause de 5 sec pour laisser le temps au moteur de s'arréter si celui-ci est en marche
}


/**
 * \fn void loop()
 * \bLief fonction loop d'arduino
 */
void loop()
{
  if ((tick_codeuse_R%100 >= 0) && (tick_codeuse_R%100 <= 50)){
    Serial.print("\t tick_codeuse_L : \t");
    Serial.println(tick_codeuse_R);
  }
  delay(100);
}


/**
 * \fn void compteur_tick_R()
 * \brief Interruption sur tick de la codeuse right
 */
void compteur_tick_R(){
        //Serial.println("Interruption");
        tick_codeuse_R++; // On incrémente le nombre de tick de la codeuse
        //duration += pulseIn(encoder0PinA_R, HIGH); // in microsecond
        //Serial.println("Je compte les ticks");
}


/**
 * \fn void compteur_tick_L()
 * \brief Interruption sur tick de la codeuse right
 */
void compteur_tick_L(){
        //Serial.println("Interruption");
        tick_codeuse_L++; // On incrémente le nombre de tick de la codeuse
        //Serial.println("Je compte les ticks");

        /*Calcul du nombre de ticks dans un certain temps (pulseIn)*/
        //Peut poser pb parce qu'on est dans une interruption et on veut mesurer la durée de l'interruption
        //duration += pulseIn(encoder0PinA_L, HIGH); // in microsecond
        // duration = duration/2500 // durée / 2500tick = nb de tours/microsecond
}


/**
 *    \file testsEncodeurs.ino
 *    \brief test des encodeurs
 *
 *    \author Arthur Duytschaever et Nicolas Sobczak
 *    \date Février 2017
 */
//_______________________________________________________________________________________________________
#include <Arduino.h>

#define encoder0PinA_L 13   //encodeur gauche A
#define encoder0PinB_L 12   //encodeur gauche B
#define encoder0PinA_R 11   //encodeur droit A
#define encoder0PinB_R 10   //encodeur droit B

volatile int compteur_tick_R = 0;   // Compteur de tick de la codeuse
volatile int compteur_tick_L = 0;   // Compteur de tick de la codeuse


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

        pinMode(encoder0PinA_L, INPUT);
        pinMode(encoder0PinB_L, INPUT);
        pinMode(encoder0PinA_R, INPUT);
        pinMode(encoder0PinB_R, INPUT);

        attachInterrupt(encoder0PinA_R, incremente_compteur_tick_R, CHANGE);    // Interruption sur tick de la codeuse
        // attachInterrupt(encoder0PinA_L, incremente_compteur_tick_L, CHANGE);    // Interruption sur tick de la codeuse
        // attachInterrupt(encoder0PinB_R, incremente_compteur_tick_R, CHANGE);    // Interruption sur tick de la codeuse
        attachInterrupt(encoder0PinB_L, incremente_compteur_tick_L, CHANGE);    // Interruption sur tick de la codeuse
}


/**
 * \fn void loop()
 * \brief fonction loop d'arduino
 */
void loop()
{
        if ((compteur_tick_L%100 >= 0) && (compteur_tick_L%100 <= 40)) {
                Serial.print("\t tick_codeuse_L : \t");
                Serial.println(compteur_tick_L);
        }

        if ((compteur_tick_R%100 >= 0) && (compteur_tick_R%100 <= 40)) {
                Serial.print("\t tick_codeuse_R : \t");
                Serial.println(compteur_tick_R);
        }

        Serial.println("\n");
        delay(100);
}


/**
 * \fn void incremente_compteur_tick_R()
 * \brief Interruption sur tick de la codeuse right
 */
void incremente_compteur_tick_R(){
        compteur_tick_R++; // On incrémente le nombre de tick de la codeuse
}


/**
 * \fn void incremente_compteur_tick_L()
 * \brief Interruption sur tick de la codeuse left
 */
void incremente_compteur_tick_L(){
        compteur_tick_L++; // On incrémente le nombre de tick de la codeuse
}

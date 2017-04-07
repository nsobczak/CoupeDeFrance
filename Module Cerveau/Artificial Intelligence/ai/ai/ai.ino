/**
 *    \file ai.ino
 *    \brief bot artificial intelligence
 *    \author Nicolas Sobczak
 *    \date Mars 2017
 */
//_______________________________________________________________________________________________________
/* === Notes ===
   On sélectionne le programme avec l'écran puis on tire la tirette.
 */

/* ======================================================================================================
 *      Include
 * ======================================================================================================
 */
#include <Arduino.h>
#include "Bot.h"

/* ======================================================================================================
 *      Initialisation
 * ======================================================================================================
 */
#define _DEBUG_ true
Bot elPadre;


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
        Serial.begin(9600);
}


/**
 * \fn void loop()
 * \brief fonction loop d'arduino
 * si on détecte que la tirette est tirée => on lance le programme
 * le programme lance un timer + déplacement vers un cylindre
 * à la fin des 90", on lance la funny action
 */
void loop()
{
        /*TODO:
           - si c'est la fin des 90" - le temps de lâcher un cylindre,
         * si on a un cylindre, on le lache
         * dans tous les cas, on s'arrête et on attends de dépasser les 90" pour lancer la funny action avec une fonction
           - sinon
         * on corrige avec l'asservissement
         * si on détecte l'adversaire => manoeuvre dévitement
         * si on détecte un cylindre => on le ramasse
         */
         elPadre.getClamp().initialisation();

        if (_DEBUG_) {
                Serial.println("=== _DEBUG_ ===");
                // Serial.println();
        }
}

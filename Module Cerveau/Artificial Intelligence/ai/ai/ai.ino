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
#define _TEST_CLAMP_ false

Bot elPadre;
bool epreuveFaite;


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
        epreuveFaite = false;
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
        if (_TEST_CLAMP_)
        {
                //Test pince
                if (_DEBUG_) Serial.println("initialisation");
                elPadre.getClamp().initialisation();
                delay(9000); //pour laisser le temps à l'action de se réaliser
                if (_DEBUG_) Serial.println("catch");
                elPadre.getClamp().catchCylinder();
                delay(6500); //pour laisser le temps à l'action de se réaliser
                if (_DEBUG_) Serial.println("release");
                elPadre.getClamp().releaseCylinder();
                delay(8000); //pour laisser le temps à l'action de se réaliser
        }

        if (_DEBUG_)
        {
                Serial.println("=== _DEBUG_ ===");
                // Serial.println();
        }


        //Code final
        if (!epreuveFaite && elPadre.isTiretteTiree())
        {
                elPadre.buildBase();
                epreuveFaite = true;
        }

        delay(3000);
}

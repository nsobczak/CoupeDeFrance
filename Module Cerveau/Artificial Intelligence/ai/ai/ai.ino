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
#define _TEST_SENSORS_ false
#define _TEST_ASSERVISSEMENT_ false
#define _TEST_FUNNY_ACTION_ false

//TODO: replace by the right values
const float _BLUE_X_START_POSITION_ = 0.4;
const float _YELLOW_X_START_POSITION_ = 2.4;
const float _Y_START_POSITION_ = 0.2;
const float _BLUE_START_ANGLE_ = 0;
const float _YELLOW_START_ANGLE_ = PI;

Bot elPadre;
bool epreuveFaite;


/* ======================================================================================================
 *      Fonctions
 * ======================================================================================================
 */

void testClamp()
{
        //Test pince
        if (_DEBUG_) Serial.println("\n===_TEST_CLAMP_===");
        if (_DEBUG_) Serial.println("initialisation");
        elPadre.getClamp().initialisation();
        delay(9000); //pour laisser le temps à l'action de se réaliser
        if (_DEBUG_) Serial.println("catch");
        elPadre.getClamp().catchCylinder();
        delay(6500); //pour laisser le temps à l'action de se réaliser
        if (_DEBUG_) Serial.println("release");
        elPadre.getClamp().releaseCylinder();
        delay(8500); //pour laisser le temps à l'action de se réaliser
}


void testSensors()
{
        if (_DEBUG_) Serial.println("\n===_TEST_SENSORS_===");
        elPadre.getSensorsBoard().updateAllSensorsValue();
        if (_DEBUG_)
        {
                Serial.print("\t getInfraredSensorBackValue : \t");
                Serial.println(elPadre.getSensorsBoard().getInfraredSensorBackValue());
                Serial.print("\t getInfraredSensorFrontBottomCenterValue : \t");
                Serial.println(elPadre.getSensorsBoard().getInfraredSensorFrontBottomCenterValue());
                Serial.print("\t getInfraredSensorFrontBottomLeftValue : \t");
                Serial.println(elPadre.getSensorsBoard().getInfraredSensorFrontBottomLeftValue());
                Serial.print("\t getInfraredSensorFrontBottomRightValue : \t");
                Serial.println(elPadre.getSensorsBoard().getInfraredSensorFrontBottomRightValue());
                Serial.print("\t getInfraredSensorFrontTopValue : \t");
                Serial.println(elPadre.getSensorsBoard().getInfraredSensorFrontTopValue());
                Serial.print("\t getUltrasonicBackValue : \t");
                Serial.println(elPadre.getSensorsBoard().getUltrasonicBackValue());
                Serial.print("\t getUltrasonicFrontValue : \t");
                Serial.println(elPadre.getSensorsBoard().getUltrasonicFrontValue());
                Serial.print("\t getUltrasonicLeftValue : \t");
                Serial.println(elPadre.getSensorsBoard().getUltrasonicLeftValue());
                Serial.print("\t getUltrasonicRightValue : \t");
                Serial.println(elPadre.getSensorsBoard().getUltrasonicRightValue());
                delay(500);
        }
}


void testAsservissement_goStraightAhead()
{
        do
        {
                if (_DEBUG_) Serial.println("botGoForward(0.4, 0.5);");
                elPadre.getAsservissement().botGoForward(0.4, 0.5);
        } while(elPadre.getAsservissement().isOrderFinished() != 1);
        elPadre.getAsservissement().handleStraightOrderEnd();
}

void testAsservissement_goBackward()
{
        do
        {
                if (_DEBUG_) Serial.println("botGoBackward(0.2, 0.25);");
                elPadre.getAsservissement().botGoBackward(0.2, 0.25);
        } while(elPadre.getAsservissement().isOrderFinished() != 1);
        elPadre.getAsservissement().handleStraightOrderEnd();
}

void testAsservissement_turnAroundRight()
{
        do
        {
                if (_DEBUG_) Serial.println("botTurnAroundRight(PI/2, 0.3);");
                elPadre.getAsservissement().botTurnAroundRight(PI/2, 0.3);
        } while(elPadre.getAsservissement().isOrderFinished() != 1);
        elPadre.getAsservissement().handleRotationOrderEnd();
}

void testAsservissement_turnAroundLeft()
{
        do
        {
                if (_DEBUG_) Serial.println("botTurnAroundRight(PI/2, 0.3);");
                elPadre.getAsservissement().botTurnAroundLeft(PI/2, 0.3);
        } while(elPadre.getAsservissement().isOrderFinished() != 1);
        elPadre.getAsservissement().handleRotationOrderEnd();
}

void testAsservissement()
{
        if (_DEBUG_) Serial.println("=== _ASSERVISSEMENT_ ===");
        testAsservissement_goStraightAhead();
        testAsservissement_turnAroundRight();
        testAsservissement_goBackward();
}


void testFunnyAction()
{
        if (_DEBUG_) Serial.println("=== _TEST_FUNNY_ACTION_ ===");
        elPadre.startFunnyActionTimer();
}


//_______________________________________________________________________________________________________
/**
 * \fn void initializePosition()
 * \brief initialiser la position avec les bonnes distances: 1 = bleu, 2 = jaune
 */
void initializePosition()
{
        if (elPadre.getColorNumber() == 1) //blue
        {
                elPadre.getAsservissement().setX_position(_BLUE_X_START_POSITION_);
                elPadre.getAsservissement().setAngle_position(_BLUE_START_ANGLE_);
        }
        else // yellow
        {
                elPadre.getAsservissement().setX_position(_YELLOW_X_START_POSITION_);
                elPadre.getAsservissement().setAngle_position(_YELLOW_START_ANGLE_);
        }
        elPadre.getAsservissement().setY_position(_Y_START_POSITION_);
}


//_______________________________________________________________________________________________________
/**
 * \fn void setup()
 * \brief fonction setup d'arduino
 */
void setup()
{
        epreuveFaite = false;
        initializePosition();
        Wire.begin();

        Serial.begin(115200);
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
        if (_TEST_CLAMP_) testClamp();

        if (_TEST_SENSORS_) testSensors();

        if (_TEST_ASSERVISSEMENT_) testAsservissement();

        if (_TEST_FUNNY_ACTION_) testFunnyAction();

        if (_DEBUG_) { Serial.println("=== _DEBUG_ ==="); }


        //Code final
        if (!epreuveFaite && elPadre.isTiretteTiree())
        {
                // elPadre.buildBase();
                epreuveFaite = true;
        }

        delay(1000);
}

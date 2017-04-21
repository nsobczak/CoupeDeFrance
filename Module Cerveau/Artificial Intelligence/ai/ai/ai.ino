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
 *      Initialisation
 * ======================================================================================================
 */
#include <Arduino.h>
#include "Bot.h"

#define _DEBUG_ true
#define _TEST_SANS_I2C_ true

// I2C
#include <Wire.h>
#include "i2cCommunication.h"

#define _RECEIVEADRESS_ 13

// Sans I2C
#define _TEST_CLAMP_ false
#define _TEST_SENSORS_ false
#define _TEST_ASSERVISSEMENT_ false
#define _TEST_FUNNY_ACTION_ true


//TODO: replace by the right values
const float _BLUE_X_START_POSITION_ = 0.4;
const float _YELLOW_X_START_POSITION_ = 2.4;
const float _Y_START_POSITION_ = 0.2;
const float _BLUE_START_ANGLE_ = 0;
const float _YELLOW_START_ANGLE_ = PI;

Bot elPadre;
bool epreuveFaite;

// Ecran
int varStartBot = 0;
int varTestMotorStraightAhead = 0;
int varTestMotorBackward = 0;
int varTestMotorLeftRotation = 0;
int varTestMotorRightRotation = 0;
int varTestMotorGlobal = 0;
int varTestClampInitialisation = 0;
int varTestClampCatch = 0;
int varTestClampRelease = 0;
int varTestClampGlobal = 0;
int varTestSensors = 0;
int varTestFunnyAction = 0;


/* ======================================================================================================
 *      Fonctions
 * ======================================================================================================
 */
/**
 * \fn void testClampInitialisation()
 * \brief test d'initialisation de la pince
 */
void testClampInitialisation()
{
        if (_DEBUG_) Serial.println("initialisation");
        elPadre.getClamp().initialisation();
        delay(9000); //pour laisser le temps à l'action de se réaliser
}

/**
 * \fn void testClampCatch()
 * \brief test de la prise de cylindre de la pince
 */
void testClampCatch()
{
        if (_DEBUG_) Serial.println("catch");
        elPadre.getClamp().catchCylinder();
        delay(6500); //pour laisser le temps à l'action de se réaliser
}

/**
 * \fn void testClampRelease()
 * \brief test du lachage de cylindre de la pince
 */
void testClampRelease()
{
        if (_DEBUG_) Serial.println("release");
        elPadre.getClamp().releaseCylinder();
        delay(8500); //pour laisser le temps à l'action de se réaliser
}

/**
 * \fn void testClampRelease()
 * \brief test global de la pince: initialisation + attraper + relacher
 */
void testClamp()
{
        //Test pince
        if (_DEBUG_) Serial.println("\n===_TEST_CLAMP_===");
        testClampInitialisation();
        testClampCatch();
        testClampRelease();
}


/**
 * \fn void testSensors()
 * \brief test de la réception de la valeur des capteurs
 */
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


/**
 * \fn void testAsservissement_goStraightAhead()
 * \brief test de l'asservissement: ligne droite vers l'avant
 */
void testAsservissement_goStraightAhead()
{
        do
        {
                if (_DEBUG_) Serial.println("botGoForward(0.4, 0.5);");
                elPadre.getAsservissement().botGoForward(0.4, 0.5);
        } while(elPadre.getAsservissement().isOrderFinished() != 1);
        elPadre.getAsservissement().handleStraightOrderEnd();
}

/**
 * \fn void testAsservissement_goStraightAhead()
 * \brief test de l'asservissement: ligne droite vers l'arrière
 */
void testAsservissement_goBackward()
{
        do
        {
                if (_DEBUG_) Serial.println("botGoBackward(0.2, 0.25);");
                elPadre.getAsservissement().botGoBackward(0.2, 0.25);
        } while(elPadre.getAsservissement().isOrderFinished() != 1);
        elPadre.getAsservissement().handleStraightOrderEnd();
}

/**
 * \fn void testAsservissement_turnAroundRight()
 * \brief test de l'asservissement: rotation vers la droite
 */
void testAsservissement_turnAroundRight()
{
        do
        {
                if (_DEBUG_) Serial.println("botTurnAroundRight(PI/2, 0.3);");
                elPadre.getAsservissement().botTurnAroundRight(PI/2, 0.3);
        } while(elPadre.getAsservissement().isOrderFinished() != 1);
        elPadre.getAsservissement().handleRotationOrderEnd();
}

/**
 * \fn void testAsservissement_turnAroundLeft()
 * \brief test de l'asservissement: rotation vers la gauche
 */
void testAsservissement_turnAroundLeft()
{
        do
        {
                if (_DEBUG_) Serial.println("botTurnAroundRight(PI/2, 0.3);");
                elPadre.getAsservissement().botTurnAroundLeft(PI/2, 0.3);
        } while(elPadre.getAsservissement().isOrderFinished() != 1);
        elPadre.getAsservissement().handleRotationOrderEnd();
}

/**
 * \fn void testAsservissement()
 * \brief test global de l'asservissement
 */
void testAsservissement()
{
        if (_DEBUG_) Serial.println("=== _ASSERVISSEMENT_ ===");
        testAsservissement_goStraightAhead();
        testAsservissement_turnAroundRight();
        testAsservissement_goBackward();
}


/**
 * \fn void testFunnyAction()
 * \brief test de lancement de la funny action
 */
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


/**
 * \fn void startBotIfTiretteTiree()
 * \brief fonction qui lance la routine d'épreuve du robot si la tirette est tiree
 */
void startBotIfTiretteTiree() {
        //Code final
        if (!epreuveFaite && elPadre.isTiretteTiree())
        {
                Wire.begin();
                // elPadre.buildBase();
                epreuveFaite = true;
        }
}


//____________________________________________________________________________________________________
// Reception I2C
/**
 * \fn void receiveEvent(int howMany - fonction qui est exécutée lorsque des données sont envoyées par le Maître. Cette fonction est enregistrée comme un événement ("event" en anglais), voir la fonction setup()
 * \param int howMany
 */
void receiveEventAI(int howMany)
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
                        if (_DEBUG_) Serial.println("variable recue : varStartBot");
                        varStartBot = value;
                        break;
                case 1:
                        if (_DEBUG_) Serial.println("variable recue : varTestMotorStraightAhead");
                        varTestMotorStraightAhead = value;
                        break;
                case 2:
                        if (_DEBUG_) Serial.println("variable recue : varTestMotorBackward");
                        varTestMotorBackward = value;
                        break;
                case 3:
                        if (_DEBUG_) Serial.println("variable recue : varTestMotorLeftRotation");
                        varTestMotorLeftRotation = value;
                        break;
                case 4:
                        if (_DEBUG_) Serial.println("variable recue : varTestMotorRightRotation");
                        varTestMotorRightRotation = value;
                        break;
                case 5:
                        if (_DEBUG_) Serial.println("variable recue : varTestMotorGlobal");
                        varTestMotorGlobal = value;
                        break;
                case 6:
                        if (_DEBUG_) Serial.println("variable recue : varTestClampInitialisation");
                        varTestClampInitialisation = value;
                        break;
                case 7:
                        if (_DEBUG_) Serial.println("variable recue : varTestClampCatch");
                        varTestClampCatch = value;
                        break;
                case 8:
                        if (_DEBUG_) Serial.println("variable recue : varTestClampRelease");
                        varTestClampRelease = value;
                        break;
                case 9:
                        if (_DEBUG_) Serial.println("variable recue : varTestClampGlobal");
                        varTestClampGlobal = value;
                        break;
                case 10:
                        if (_DEBUG_) Serial.println("variable recue : varTestSensors");
                        varTestSensors = value;
                        break;
                case 11:
                        if (_DEBUG_) Serial.println("variable recue : varTestFunnyAction");
                        varTestFunnyAction = value;
                        break;
                default:
                        if (_DEBUG_) Serial.println("variable recue inconnue");
                }

        }
        // else de debug
        else if (_DEBUG_) Serial.println("Erreur : Pas 3 octets envoyes");
}


/**
 * \fn void i2creceiveAI()
 * \param int adresse de réception de l'I2C
 * \brief fonction de réception I2C
 */
void i2creceiveAI(int adresse)
{
        Wire.begin(adresse);     // Joindre le Bus I2C avec adresse
        Wire.onReceive(receiveEventAI); // enregistrer l'événement (lorsqu'une demande arrive)
        Wire.endTransmission(); // fin transmission
}


/**
 * \fn void receiveVarFromScreen()
 * \brief fonction qui déclenche le bon test suivant la variable reçue depuis l'écran
 */
void receiveVarFromScreen()
{
        if (varStartBot == 0 && varTestMotorStraightAhead == 0 &&
            varTestMotorBackward == 0 && varTestMotorLeftRotation == 0 &&
            varTestMotorRightRotation == 0 && varTestMotorGlobal == 0 &&
            varTestClampInitialisation == 0 && varTestClampCatch == 0 &&
            varTestClampRelease == 0 && varTestClampGlobal == 0 &&
            varTestSensors == 0 && varTestFunnyAction == 0)
                i2creceiveAI(_RECEIVEADRESS_);
        else
        {
                if (varStartBot == 1) {startBotIfTiretteTiree(); varStartBot = 0; }
                else if (varTestMotorStraightAhead == 1) {testAsservissement_goStraightAhead(); varTestMotorStraightAhead = 0; }
                else if (varTestMotorBackward == 1) {testAsservissement_goBackward(); varTestMotorBackward = 0; }
                else if (varTestMotorLeftRotation == 1) {testAsservissement_turnAroundLeft(); varTestMotorLeftRotation = 0; }
                else if (varTestMotorRightRotation == 1) {testAsservissement_turnAroundRight(); varTestMotorRightRotation = 0; }
                else if (varTestMotorGlobal == 1) {testAsservissement(); varTestMotorGlobal = 0; }
                else if (varTestClampInitialisation == 1) {testClampInitialisation(); varTestClampInitialisation = 0; }
                else if (varTestClampCatch == 1) {testClampCatch(); varTestClampCatch = 0; }
                else if (varTestClampRelease == 1) {testClampRelease(); varTestClampRelease = 0; }
                else if (varTestClampGlobal == 1) {testClamp(); varTestClampGlobal = 0; }
                else if (varTestSensors == 1) {testSensors(); varTestSensors = 0; }
                else if (varTestFunnyAction == 1) {testFunnyAction(); varTestFunnyAction = 0; }
        }
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
        if (_TEST_SANS_I2C_)
        {
                if (_TEST_CLAMP_) testClamp();

                if (_TEST_SENSORS_) testSensors();

                if (_TEST_ASSERVISSEMENT_) testAsservissement();

                if (_TEST_FUNNY_ACTION_) testFunnyAction();

                if (_DEBUG_) { Serial.println("=== _DEBUG_ ==="); }

                startBotIfTiretteTiree();
        }
        else receiveVarFromScreen();

        delay(1000);
}

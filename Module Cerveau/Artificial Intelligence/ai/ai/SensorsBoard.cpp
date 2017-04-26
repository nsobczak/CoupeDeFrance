/**
 *    \file SensorsBoard.cpp
 *    \brief classe carte capteurs
 *
 *    \author Nicolas SOBCZAK
 *    \date Mars 2017
 */
//_______________________________________________________________________________________________________
/* ======================================================================================================
 *      Initialisation
 * ======================================================================================================
 */
#include "SensorsBoard.h"

int SensorsBoard::infraredSensorFrontBottomRightValue = 0;
int SensorsBoard::infraredSensorFrontBottomLeftValue = 0;
int SensorsBoard::infraredSensorFrontBottomCenterValue = 0;
int SensorsBoard::infraredSensorFrontTopValue = 0;
int SensorsBoard::infraredSensorBackValue = 0;
int SensorsBoard::ultrasonicFrontValue = 0;
int SensorsBoard::ultrasonicRightValue = 0;
int SensorsBoard::ultrasonicLeftValue = 0;
int SensorsBoard::ultrasonicBackValue = 0;


/* ======================================================================================================
 *      Constructor, getter, setter
 * ======================================================================================================
 */
SensorsBoard::SensorsBoard()
{
}

int SensorsBoard::getInfraredSensorFrontBottomRightValue()
{
        return infraredSensorFrontBottomRightValue;
}

int SensorsBoard::getInfraredSensorFrontBottomLeftValue()
{
        return infraredSensorFrontBottomLeftValue;
}

int SensorsBoard::getInfraredSensorFrontBottomCenterValue()
{
        return infraredSensorFrontBottomCenterValue;
}

int SensorsBoard::getInfraredSensorFrontTopValue()
{
        return infraredSensorFrontTopValue;
}

int SensorsBoard::getInfraredSensorBackValue()
{
        return infraredSensorBackValue;
}

int SensorsBoard::getUltrasonicFrontValue()
{
        return ultrasonicFrontValue;
}

int SensorsBoard::getUltrasonicRightValue()
{
        return ultrasonicRightValue;
}

int SensorsBoard::getUltrasonicLeftValue()
{
        return ultrasonicLeftValue;
}

int SensorsBoard::getUltrasonicBackValue()
{
        return ultrasonicBackValue;
}


void SensorsBoard::setInfraredSensorFrontBottomRightValue(int infraredSensorFrontBottomRightValue)
{
        SensorsBoard::infraredSensorFrontBottomRightValue = infraredSensorFrontBottomRightValue;
}

void SensorsBoard::setInfraredSensorFrontBottomLeftValue(int infraredSensorFrontBottomLeftValue)
{
        SensorsBoard::infraredSensorFrontBottomLeftValue = infraredSensorFrontBottomLeftValue;
}

void SensorsBoard::setInfraredSensorFrontBottomCenterValue(int infraredSensorFrontBottomCenterValue)
{
        SensorsBoard::infraredSensorFrontBottomCenterValue = infraredSensorFrontBottomCenterValue;
}

void SensorsBoard::setInfraredSensorFrontTopValue(int infraredSensorFrontTopValue)
{
        SensorsBoard::infraredSensorFrontTopValue = infraredSensorFrontTopValue;
}

void SensorsBoard::setInfraredSensorBackValue(int infraredSensorBackValue)
{
        SensorsBoard::infraredSensorBackValue = infraredSensorBackValue;
}

void SensorsBoard::setUltrasonicFrontValue(int ultrasonicFrontValue)
{
        SensorsBoard::ultrasonicFrontValue = ultrasonicFrontValue;
}

void SensorsBoard::setUltrasonicRightValue(int ultrasonicRightValue)
{
        SensorsBoard::ultrasonicRightValue = ultrasonicRightValue;
}

void SensorsBoard::setUltrasonicLeftValue(int ultrasonicLeftValue)
{
        SensorsBoard::ultrasonicLeftValue = ultrasonicLeftValue;
}

void SensorsBoard::setUltrasonicBackValue(int ultrasonicBackValue)
{
        SensorsBoard::ultrasonicBackValue = ultrasonicBackValue;
}


/* ======================================================================================================
 *      Methods
 * ======================================================================================================
 */

/**
 * \fn void receiveEvent(int howMany - fonction qui est exécutée lorsque des données sont envoyées par le Maître. Cette fonction est enregistrée comme un événement ("event" en anglais), voir la fonction setup()
 * \param int howMany
 */
void SensorsBoard::receive3bytes()
{
        if (_DEBUG_SENSORSBOARD_) {Serial.print("Wire.available(): "); Serial.println(Wire.available()); }
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
                        if (_DEBUG_SENSORSBOARD_) Serial.println("variable recue : InfraredSensorBack");
                        this->setInfraredSensorBackValue(value);
                        break;
                case 1:
                        if (_DEBUG_SENSORSBOARD_) Serial.println("variable recue : InfraredSensorFrontTop");
                        this->setInfraredSensorFrontTopValue(value);
                        break;
                case 2:
                        if (_DEBUG_SENSORSBOARD_) Serial.println("variable recue : InfraredSensorFrontBottomRight");
                        this->setInfraredSensorFrontBottomRightValue(value);
                        break;
                case 3:
                        if (_DEBUG_SENSORSBOARD_) Serial.println("variable recue : InfraredSensorFrontBottomCenter");
                        this->setInfraredSensorFrontBottomCenterValue(value);
                        break;
                case 4:
                        if (_DEBUG_SENSORSBOARD_) Serial.println("variable recue : InfraredSensorFrontBottomLeft");
                        this->setInfraredSensorFrontBottomLeftValue(value);
                        break;
                case 5:
                        if (_DEBUG_SENSORSBOARD_) Serial.println("variable recue : UltrasonicRight");
                        this->setUltrasonicRightValue(value);
                        break;
                case 6:
                        if (_DEBUG_SENSORSBOARD_) Serial.println("variable recue : UltrasonicLeft");
                        this->setUltrasonicLeftValue(value);
                        break;
                case 7:
                        if (_DEBUG_SENSORSBOARD_) Serial.println("variable recue : UltrasonicFront (back1)");
                        this->setUltrasonicFrontValue(value);
                        break;
                case 8:
                        if (_DEBUG_SENSORSBOARD_) Serial.println("variable recue : UltrasonicBack (back2)");
                        this->setUltrasonicBackValue(value);
                        break;
                default:
                        if (_DEBUG_SENSORSBOARD_) Serial.println("variable recue inconnue");
                }

        }
        // else de debug
        else if (_DEBUG_SENSORSBOARD_) Serial.println("Erreur : Pas 3 octets envoyes");
}


/**
 * \fn void SensorsBoard::update()
 * \brief reception + maj infos du module capteurs
 */
void SensorsBoard::updateAllSensorsValue()
{
        for (int i = 0; i < _NUMBER_OF_SENSORS_; i++)
        {
                Wire.requestFrom(_SENSORSBOARD_RECEIVEADRESS_, 3);
                this->receive3bytes();
        }
}


// === CHECK CYLINDER FUNCTIONS ===

bool SensorsBoard::checkForCylinderOnSensorFrontBottomLeft()
{
        if ( this->getInfraredSensorFrontBottomLeftValue() < _CYLINDER_DISTANCE_) return true;
        else return false;
}

bool SensorsBoard::checkForCylinderOnSensorFrontBottomCenter()
{
        if ( this->getInfraredSensorFrontBottomCenterValue() < _CYLINDER_DISTANCE_) return true;
        else return false;
}

bool SensorsBoard::checkForCylinderOnSensorFrontBottomRight()
{
        if ( this->getInfraredSensorFrontBottomRightValue() < _CYLINDER_DISTANCE_) return true;
        else return false;
}

//return true if there is a cylinder
bool SensorsBoard::checkForCylinder()
{
        if ( this->checkForCylinderOnSensorFrontBottomLeft() ||
             this->checkForCylinderOnSensorFrontBottomCenter()||
             this->checkForCylinderOnSensorFrontBottomRight())
                return true;
        return false;
}


// === CHECK BASE FUNCTIONS ===

bool SensorsBoard::checkForBaseOnSensorFrontBottomLeft()
{
        if ( this->getInfraredSensorFrontBottomLeftValue() < _BASE_DISTANCE_) return true;
        else return false;
}

bool SensorsBoard::checkForBaseOnSensorFrontBottomCenter()
{
        if ( this->getInfraredSensorFrontBottomCenterValue() < _BASE_DISTANCE_) return true;
        else return false;
}

bool SensorsBoard::checkForBaseOnSensorFrontBottomRight()
{
        if ( this->getInfraredSensorFrontBottomRightValue() < _BASE_DISTANCE_) return true;
        else return false;
}

bool SensorsBoard::checkForBase()
{
        if ( this->checkForBaseOnSensorFrontBottomLeft() ||
             this->checkForBaseOnSensorFrontBottomCenter()||
             this->checkForBaseOnSensorFrontBottomRight())
                return true;
        return false;
}


// === CHECK HINDRANCE FUNCTIONS ===

bool SensorsBoard::checkForHindranceOnFront()
{
        if ( this->getInfraredSensorFrontTopValue() < _FOE_DISTANCE_ ||  this->getUltrasonicFrontValue() < _FOE_DISTANCE_) return true;
        else return false;
}

bool SensorsBoard::checkForHindranceOnRight()
{
        if ( this->getUltrasonicRightValue() < _FOE_DISTANCE_) return true;
        else return false;
}

bool SensorsBoard::checkForHindranceOnLeft()
{
        if ( this->getUltrasonicLeftValue() < _FOE_DISTANCE_) return true;
        else return false;
}

bool SensorsBoard::checkForHindranceOnBack()
{
        if ( this->getInfraredSensorBackValue() < _FOE_DISTANCE_ ||  this->getUltrasonicBackValue() < _FOE_DISTANCE_) return true;
        else return false;
}

//return true if there is an hindrance
bool SensorsBoard::checkForHindrance()
{
        if ( this->checkForHindranceOnFront() ||
             this->checkForHindranceOnRight() ||
             this->checkForHindranceOnLeft() ||
             this->checkForHindranceOnBack())
                return true;
        else return false;
}

/**
 *    \file SensorsBoard.cpp
 *    \brief classe carte capteurs
 *
 *    \author Nicolas SOBCZAK
 *    \date Mars 2017
 */
//_______________________________________________________________________________________________________
/* ======================================================================================================
 *      Include
 * ======================================================================================================
 */
#include "SensorsBoard.h"


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
void SensorsBoard::receiveEvent3bytes(int howMany)
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
                        if (_DEBUG_) Serial.println("variable recue : InfraredSensorBack");
                        SensorsBoard::setInfraredSensorBackValue(value);
                        break;
                case 1:
                        if (_DEBUG_) Serial.println("variable recue : InfraredSensorFrontTop");
                        SensorsBoard::setInfraredSensorFrontTopValue(value);
                        break;
                case 2:
                        if (_DEBUG_) Serial.println("variable recue : InfraredSensorFrontBottomRight");
                        SensorsBoard::setInfraredSensorFrontBottomRightValue(value);
                        break;
                case 3:
                        if (_DEBUG_) Serial.println("variable recue : InfraredSensorFrontBottomCenter");
                        SensorsBoard::setInfraredSensorFrontBottomCenterValue(value);
                        break;
                case 4:
                        if (_DEBUG_) Serial.println("variable recue : InfraredSensorFrontBottomLeft");
                        SensorsBoard::setInfraredSensorFrontBottomLeftValue(value);
                        break;
                case 5:
                        if (_DEBUG_) Serial.println("variable recue : UltrasonicRight");
                        SensorsBoard::setUltrasonicRightValue(value);
                        break;
                case 6:
                        if (_DEBUG_) Serial.println("variable recue : UltrasonicLeft");
                        SensorsBoard::setUltrasonicLeftValue(value);
                        break;
                case 7:
                        if (_DEBUG_) Serial.println("variable recue : UltrasonicFront (back1)");
                        SensorsBoard::setUltrasonicFrontValue(value);
                        break;
                case 8:
                        if (_DEBUG_) Serial.println("variable recue : UltrasonicBack (back2)");
                        SensorsBoard::setUltrasonicBackValue(value);
                        break;
                default:
                        if (_DEBUG_) Serial.println("variable recue inconnue");
                }

        }
        // else de debug
        else if (_DEBUG_)
        {
                Serial.println("Erreur : Pas 3 octets envoyes");
        }
}


/**
 * \fn void i2creceive3bytes
 * \param int adresse
 * \brief
 */
void SensorsBoard::i2creceive3bytes(int adresse)
{
        Wire.begin(adresse);
        Wire.onReceive(SensorsBoard::receiveEvent3bytes); // enregistrer l'événement (lorsqu'une demande arrive)
        Wire.endTransmission();
}


/**
 * \fn void SensorsBoard::update()
 * \brief reception + maj infos du module capteurs
 */
void SensorsBoard::updateAllSensorsValue()
{
        for (int i = 0; i < _NUMBER_OF_SENSORS_; i++)
        {
                SensorsBoard::i2creceive3bytes(_SENSORSBOARD_RECEIVEADRESS_);
        }
}


// === CHECK CYLINDER FUNCTIONS ===

bool SensorsBoard::checkForCylinderOnSensorFrontBottomLeft()
{
        if (SensorsBoard::infraredSensorFrontBottomLeftValue < _CYLINDER_DISTANCE_) return true;
        else return false;
}

bool SensorsBoard::checkForCylinderOnSensorFrontBottomCenter()
{
        if (SensorsBoard::infraredSensorFrontBottomCenterValue < _CYLINDER_DISTANCE_) return true;
        else return false;
}

bool SensorsBoard::checkForCylinderOnSensorFrontBottomRight()
{
        if (SensorsBoard::infraredSensorFrontBottomRightValue < _CYLINDER_DISTANCE_) return true;
        else return false;
}

//return true if there is a cylinder
bool SensorsBoard::checkForCylinder()
{
        if (SensorsBoard::checkForCylinderOnSensorFrontBottomLeft() ||
            SensorsBoard::checkForCylinderOnSensorFrontBottomCenter()||
            SensorsBoard::checkForCylinderOnSensorFrontBottomRight())
                return true;
        return false;
}


// === CHECK BASE FUNCTIONS ===

bool SensorsBoard::checkForBaseOnSensorFrontBottomLeft()
{
        if (SensorsBoard::infraredSensorFrontBottomLeftValue < _BASE_DISTANCE_) return true;
        else return false;
}

bool SensorsBoard::checkForBaseOnSensorFrontBottomCenter()
{
        if (SensorsBoard::infraredSensorFrontBottomCenterValue < _BASE_DISTANCE_) return true;
        else return false;
}

bool SensorsBoard::checkForBaseOnSensorFrontBottomRight()
{
        if (SensorsBoard::infraredSensorFrontBottomRightValue < _BASE_DISTANCE_) return true;
        else return false;
}

bool SensorsBoard::checkForBase()
{
        if (SensorsBoard::checkForBaseOnSensorFrontBottomLeft() ||
            SensorsBoard::checkForBaseOnSensorFrontBottomCenter()||
            SensorsBoard::checkForBaseOnSensorFrontBottomRight())
                return true;
        return false;
}


// === CHECK HINDRANCE FUNCTIONS ===

bool SensorsBoard::checkForHindranceOnFront()
{
        if (SensorsBoard::infraredSensorFrontTopValue < _FOE_DISTANCE_ || SensorsBoard::ultrasonicFrontValue < _FOE_DISTANCE_) return true;
        else return false;
}

bool SensorsBoard::checkForHindranceOnRight()
{
        if (SensorsBoard::ultrasonicRightValue < _FOE_DISTANCE_) return true;
        else return false;
}

bool SensorsBoard::checkForHindranceOnLeft()
{
        if (SensorsBoard::ultrasonicLeftValue < _FOE_DISTANCE_) return true;
        else return false;
}

bool SensorsBoard::checkForHindranceOnBack()
{
        if (SensorsBoard::infraredSensorBackValue < _FOE_DISTANCE_ || SensorsBoard::ultrasonicBackValue < _FOE_DISTANCE_) return true;
        else return false;
}

//return true if there is an hindrance
bool SensorsBoard::checkForHindrance()
{
        if (SensorsBoard::checkForHindranceOnFront() ||
            SensorsBoard::checkForHindranceOnRight() ||
            SensorsBoard::checkForHindranceOnLeft() ||
            SensorsBoard::checkForHindranceOnBack())
                return true;
        else return false;
}

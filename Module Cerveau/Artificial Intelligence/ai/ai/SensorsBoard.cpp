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

void SensorsBoard::update()
{
        //TODO: recevoir infos du module capteurs
//    if i2c receive, switch, set le capteur qu'il faut
        SensorsBoard::i2creceive3bytes(_SENSORSBOARD_RECEIVEADRESS_);

}


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
                case 1:
                        if (_DEBUG_) Serial.println("variable recue : InfraredSensorFrontBottomRight");
                        SensorsBoard::setInfraredSensorFrontBottomRight(value);
                        break;
                case 2:
                        if (_DEBUG_) Serial.println("variable recue : InfraredSensorFrontBottomLeft");
                        SensorsBoard::setInfraredSensorFrontBottomLeft(value);
                        break;
                case 3:
                        if (_DEBUG_) Serial.println("variable recue : InfraredSensorFrontBottomCenter");
                        SensorsBoard::setInfraredSensorFrontBottomCenter(value);
                        break;
                case 4:
                        if (_DEBUG_) Serial.println("variable recue : InfraredSensorFrontTop");
                        SensorsBoard::setInfraredSensorFrontTop(value);
                        break;
                case 5:
                        if (_DEBUG_) Serial.println("variable recue : InfraredSensorBack");
                        SensorsBoard::setInfraredSensorBack(value);
                        break;
                case 6:
                        if (_DEBUG_) Serial.println("variable recue : UltrasonicFront");
                        SensorsBoard::setUltrasonicFront(value);
                        break;
                case 7:
                        if (_DEBUG_) Serial.println("variable recue : UltrasonicRight");
                        SensorsBoard::setUltrasonicRight(value);
                        break;
                case 8:
                        if (_DEBUG_) Serial.println("variable recue : UltrasonicLeft");
                        SensorsBoard::setUltrasonicLeft(value);
                        break;
                case 9:
                        if (_DEBUG_) Serial.println("variable recue : UltrasonicBack");
                        SensorsBoard::setUltrasonicBack(value);
                        break;
                //TODO: uncomment if the last ultrasonic sensors works properly
                // case 10:
                //         if (_DEBUG_) Serial.println("variable recue : UltrasonicBack");
                //         SensorsBoard::setUltrasonicBack(value);
                //         break;
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



// === CHECK FUNCTIONS ===

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

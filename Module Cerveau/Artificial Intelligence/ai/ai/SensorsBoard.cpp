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

int SensorsBoard::getInfraredSensorFrontBottomRight() const
{
    return infraredSensorFrontBottomRight;
}

int SensorsBoard::getInfraredSensorFrontBottomLeft() const
{
    return infraredSensorFrontBottomLeft;
}

int SensorsBoard::getInfraredSensorFrontBottomCenter() const
{
    return infraredSensorFrontBottomCenter;
}

int SensorsBoard::getInfraredSensorFrontTop() const
{
    return infraredSensorFrontTop;
}

int SensorsBoard::getInfraredSensorBack() const
{
    return infraredSensorBack;
}

int SensorsBoard::getUltrasonicFront() const
{
    return ultrasonicFront;
}

int SensorsBoard::getUltrasonicRight() const
{
    return ultrasonicRight;
}

int SensorsBoard::getUltrasonicLeft() const
{
    return ultrasonicLeft;
}

int SensorsBoard::getUltrasonicBack() const
{
    return ultrasonicBack;
}

void SensorsBoard::setInfraredSensorFrontBottomRight(int infraredSensorFrontBottomRight)
{
    SensorsBoard::infraredSensorFrontBottomRight = infraredSensorFrontBottomRight;
}

void SensorsBoard::setInfraredSensorFrontBottomLeft(int infraredSensorFrontBottomLeft)
{
    SensorsBoard::infraredSensorFrontBottomLeft = infraredSensorFrontBottomLeft;
}

void SensorsBoard::setInfraredSensorFrontBottomCenter(int infraredSensorFrontBottomCenter)
{
    SensorsBoard::infraredSensorFrontBottomCenter = infraredSensorFrontBottomCenter;
}

void SensorsBoard::setInfraredSensorFrontTop(int infraredSensorFrontTop)
{
    SensorsBoard::infraredSensorFrontTop = infraredSensorFrontTop;
}

void SensorsBoard::setInfraredSensorBack(int infraredSensorBack)
{
    SensorsBoard::infraredSensorBack = infraredSensorBack;
}

void SensorsBoard::setUltrasonicFront(int ultrasonicFront)
{
    SensorsBoard::ultrasonicFront = ultrasonicFront;
}

void SensorsBoard::setUltrasonicRight(int ultrasonicRight)
{
    SensorsBoard::ultrasonicRight = ultrasonicRight;
}

void SensorsBoard::setUltrasonicLeft(int ultrasonicLeft)
{
    SensorsBoard::ultrasonicLeft = ultrasonicLeft;
}

void SensorsBoard::setUltrasonicBack(int ultrasonicBack)
{
    SensorsBoard::ultrasonicBack = ultrasonicBack;
}

/* ======================================================================================================
 *      Methods
 * ======================================================================================================
 */

void SensorsBoard::update()
{
    //TODO: recevoir infos du module capteurs
//    if i2c receive, switch, set le capteur qu'il faut
}

//return true if there is a cylinder
bool SensorsBoard::checkForCylinder()
{
    bool result = false;
    this->update();
//    if (this->infraredSensorFrontBottomLeft){
//
//    }

    return result;
}

//return true if there is an hindrance
bool SensorsBoard::checkForHindrance()
{
    bool result = false;
    this->update();
    return result;
}

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


bool SensorsBoard::checkForCylinderOnSensorFrontBottomLeft()
{
        if (this->infraredSensorFrontBottomLeft < _CYLINDER_DISTANCE_) return true;
        else return false;
}

bool SensorsBoard::checkForCylinderOnSensorFrontBottomCenter()
{
        if (this->infraredSensorFrontBottomCenter < _CYLINDER_DISTANCE_) return true;
        else return false;
}

bool SensorsBoard::checkForCylinderOnSensorFrontBottomRight()
{
        if (this->infraredSensorFrontBottomRight < _CYLINDER_DISTANCE_) return true;
        else return false;
}

//return true if there is a cylinder
bool SensorsBoard::checkForCylinder()
{
        if (this->checkForCylinderOnSensorFrontBottomLeft() ||
            this->checkForCylinderOnSensorFrontBottomCenter()||
            this->checkForCylinderOnSensorFrontBottomRight())
                return true;
        return false;
}


bool SensorsBoard::checkForHindranceOnFront()
{
        if (this->infraredSensorFrontTop < _FOE_DISTANCE_ || this->ultrasonicFront < _FOE_DISTANCE_) return true;
        else return false;
}

bool SensorsBoard::checkForHindranceOnRight()
{
        if (this->ultrasonicRight < _FOE_DISTANCE_) return true;
        else return false;
}

bool SensorsBoard::checkForHindranceOnLeft()
{
        if (this->ultrasonicLeft < _FOE_DISTANCE_) return true;
        else return false;
}

bool SensorsBoard::checkForHindranceOnBack()
{
        if (this->infraredSensorBack < _FOE_DISTANCE_ || this->ultrasonicBack < _FOE_DISTANCE_) return true;
        else return false;
}

//return true if there is an hindrance
bool SensorsBoard::checkForHindrance()
{
        if (this->checkForHindranceOnFront() ||
            this->checkForHindranceOnRight() ||
            this->checkForHindranceOnLeft() ||
            this->checkForHindranceOnBack())
                return true;
        else return false;
}

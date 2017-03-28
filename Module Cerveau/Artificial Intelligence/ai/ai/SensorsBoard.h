/**
 *    \file SensorsBoard.h
 *    \brief classe carte capteurs
 *
 *    \author Nicolas SOBCZAK
 *    \date Mars 2017
 */
//_______________________________________________________________________________________________________
//TODO: inclure les fonctions de detection de la carte capteurs
/* ======================================================================================================
 *      Include
 * ======================================================================================================
 */
#include "Arduino.h"


/* ======================================================================================================
 *      Class
 * ======================================================================================================
 */
class SensorsBoard
{
private:

int infraredSensorFrontBottomRight;
int infraredSensorFrontBottomLeft;
int infraredSensorFrontBottomCenter;
int infraredSensorFrontTop;
int infraredSensorBack;

int ultrasonicFront;
int ultrasonicRight;
int ultrasonicLeft;
int ultrasonicBack;


public:

SensorsBoard();

    int getInfraredSensorFrontBottomRight() const;
    int getInfraredSensorFrontBottomLeft() const;
    int getInfraredSensorFrontBottomCenter() const;
    int getInfraredSensorFrontTop() const;
    int getInfraredSensorBack() const;

    int getUltrasonicFront() const;
    int getUltrasonicRight() const;
    int getUltrasonicLeft() const;
    int getUltrasonicBack() const;

    void setInfraredSensorFrontBottomRight(int infraredSensorFrontBottomRight);
    void setInfraredSensorFrontBottomLeft(int infraredSensorFrontBottomLeft);
    void setInfraredSensorFrontBottomCenter(int infraredSensorFrontBottomCenter);
    void setInfraredSensorFrontTop(int infraredSensorFrontTop);
    void setInfraredSensorBack(int infraredSensorBack);

    void setUltrasonicFront(int ultrasonicFront);
    void setUltrasonicRight(int ultrasonicRight);
    void setUltrasonicLeft(int ultrasonicLeft);
    void setUltrasonicBack(int ultrasonicBack);

    void update();

    bool checkForCylinder();
    bool checkForHindrance();


};

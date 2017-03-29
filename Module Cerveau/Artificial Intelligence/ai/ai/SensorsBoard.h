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

//TODO: changer les define pour les remplacer par la bonne disance
#define cylinderDistance 1.2 //voir la pince
#define foeDistance 10.0 //10cm par rapport au capteur

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

//____________________
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

bool checkForCylinderOnSensorFrontBottomLeft();
bool checkForCylinderOnSensorFrontBottomCenter();
bool checkForCylinderOnSensorFrontBottomRight();
bool checkForCylinder();

bool checkForHindranceOnFront();
bool checkForHindranceOnRight();
bool checkForHindranceOnLeft();
bool checkForHindranceOnBack();
bool checkForHindrance();


};

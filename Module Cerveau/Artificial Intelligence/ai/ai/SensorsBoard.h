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
#include <Wire.h>
#include "i2cCommunication.h"

#define _SENSORSBOARD_RECEIVEADRESS_ 11

#define _DEBUG_ true

//TODO: changer les define pour les remplacer par la bonne disance
#define _CYLINDER_DISTANCE_ 1.2 //voir la pince
#define _FOE_DISTANCE_ 10.0 //10cm par rapport au capteur

/*Notes
   1 => infraredSensorFrontBottomRightValue;
   2 => infraredSensorFrontBottomLeftValue;
   3 => infraredSensorFrontBottomCenterValue;
   4 => infraredSensorFrontTopValue;
   5 => infraredSensorBackValue;
   6 => ultrasonicFrontValue;
   7 => ultrasonicRightValue;
   8 => ultrasonicLeftValue;
   9 => ultrasonicBackValue;
 */

/* ======================================================================================================
 *      Class
 * ======================================================================================================
 */
class SensorsBoard
{
private:

static int infraredSensorFrontBottomRightValue;
static int infraredSensorFrontBottomLeftValue;
static int infraredSensorFrontBottomCenterValue;
static int infraredSensorFrontTopValue;
static int infraredSensorBackValue;

static int ultrasonicFrontValue;
static int ultrasonicRightValue;
static int ultrasonicLeftValue;
static int ultrasonicBackValue;

//____________________
public:

SensorsBoard();

static int getInfraredSensorFrontBottomRight();
static int getInfraredSensorFrontBottomLeft();
static int getInfraredSensorFrontBottomCenter();
static int getInfraredSensorFrontTop();
static int getInfraredSensorBack();

static int getUltrasonicFront();
static int getUltrasonicRight();
static int getUltrasonicLeft();
static int getUltrasonicBack();

static void setInfraredSensorFrontBottomRight(int infraredSensorFrontBottomRight);
static void setInfraredSensorFrontBottomLeft(int infraredSensorFrontBottomLeft);
static void setInfraredSensorFrontBottomCenter(int infraredSensorFrontBottomCenter);
static void setInfraredSensorFrontTop(int infraredSensorFrontTop);
static void setInfraredSensorBack(int infraredSensorBack);

static void setUltrasonicFront(int ultrasonicFront);
static void setUltrasonicRight(int ultrasonicRight);
static void setUltrasonicLeft(int ultrasonicLeft);
static void setUltrasonicBack(int ultrasonicBack);

static void update();

static void receiveEvent3bytes(int howMany);
static void i2creceive3bytes(int adresse);

static bool checkForCylinderOnSensorFrontBottomLeft();
static bool checkForCylinderOnSensorFrontBottomCenter();
static bool checkForCylinderOnSensorFrontBottomRight();
static bool checkForCylinder();

static bool checkForHindranceOnFront();
static bool checkForHindranceOnRight();
static bool checkForHindranceOnLeft();
static bool checkForHindranceOnBack();
static bool checkForHindrance();


};

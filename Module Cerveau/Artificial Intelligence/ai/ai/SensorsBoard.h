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
#define _CYLINDER_DISTANCE_ 10.0 //voir la pince
#define _DISTANCE_WHERE_CYLINDER_IS_READY_TO_BE_CAUGHT_ 8.0 //voir la distance pince-cylindre
#define _BASE_DISTANCE_ 30.0 //3cm
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

static int getInfraredSensorFrontBottomRightValue();
static int getInfraredSensorFrontBottomLeftValue();
static int getInfraredSensorFrontBottomCenterValue();
static int getInfraredSensorFrontTopValue();
static int getInfraredSensorBackValue();
static int getUltrasonicFrontValue();
static int getUltrasonicRightValue();
static int getUltrasonicLeftValue();
static int getUltrasonicBackValue();

static void setInfraredSensorFrontBottomRightValue(int infraredSensorFrontBottomRightValue);
static void setInfraredSensorFrontBottomLeftValue(int infraredSensorFrontBottomLeftValue);
static void setInfraredSensorFrontBottomCenterValue(int infraredSensorFrontBottomCenterValue);
static void setInfraredSensorFrontTopValue(int infraredSensorFrontTopValue);
static void setInfraredSensorBackValue(int infraredSensorBackValue);
static void setUltrasonicFrontValue(int ultrasonicFrontValue);
static void setUltrasonicRightValue(int ultrasonicRightValue);
static void setUltrasonicLeftValue(int ultrasonicLeftValue);
static void setUltrasonicBackValue(int ultrasonicBackValue);

static void update();

static void receiveEvent3bytes(int howMany);
static void i2creceive3bytes(int adresse);

static bool checkForCylinderOnSensorFrontBottomLeft();
static bool checkForCylinderOnSensorFrontBottomCenter();
static bool checkForCylinderOnSensorFrontBottomRight();
static bool checkForCylinder();

static bool checkForBaseOnSensorFrontBottomLeft();
static bool checkForBaseOnSensorFrontBottomCenter();
static bool checkForBaseOnSensorFrontBottomRight();
static bool checkForBase();

static bool checkForHindranceOnFront();
static bool checkForHindranceOnRight();
static bool checkForHindranceOnLeft();
static bool checkForHindranceOnBack();
static bool checkForHindrance();

};

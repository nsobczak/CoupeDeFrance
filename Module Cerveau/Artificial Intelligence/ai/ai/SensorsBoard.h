/**
 *    \file SensorsBoard.h
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
#include "Arduino.h"
#include <Wire.h>
#include "i2cCommunication.h"

#define _DEBUG_ false

#define _SENSORSBOARD_RECEIVEADRESS_ 11

#define _NUMBER_OF_SENSORS_ 9 // 5 ir + 4 us

//TODO: changer les define pour les remplacer par la bonne disance
#define _CYLINDER_DISTANCE_ 14.0 //voir la pince
#define _DISTANCE_WHERE_CYLINDER_IS_READY_TO_BE_CAUGHT_ 8.0 //voir la distance pince-cylindre
#define _BASE_DISTANCE_ 8.5 //3cm
#define _FOE_DISTANCE_ 10.0 //10cm par rapport au capteur

/*Notes: tableau id i2c
   {sharpBT, sharpFT, sharpFBR, sharpFBC, sharpFBL};
   {echoPinR, echoPinL, echoPinB1, echoPinB2}

   0 => infraredSensorBackValue;
   1 => infraredSensorFrontTopValue;
   2 => infraredSensorFrontBottomRightValue;
   3 => infraredSensorFrontBottomCenterValue;
   4 => infraredSensorFrontBottomLeftValue;

   5 => ultrasonicRightValue;
   6 => ultrasonicLeftValue;
   7 => ultrasonicBack1Value;
   8 => ultrasonicBack2Value;
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


void receive3bytes();
void updateAllSensorsValue();

bool checkForCylinderOnSensorFrontBottomLeft();
bool checkForCylinderOnSensorFrontBottomCenter();
bool checkForCylinderOnSensorFrontBottomRight();
bool checkForCylinder();

bool checkForBaseOnSensorFrontBottomLeft();
bool checkForBaseOnSensorFrontBottomCenter();
bool checkForBaseOnSensorFrontBottomRight();
bool checkForBase();

bool checkForHindranceOnFront();
bool checkForHindranceOnRight();
bool checkForHindranceOnLeft();
bool checkForHindranceOnBack();
bool checkForHindrance();




};

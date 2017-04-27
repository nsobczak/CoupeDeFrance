/**
 *    \file Bot.h
 *    \brief classe robot
 *
 *    \author Nicolas SOBCZAK
 *    \date Mars 2017
 */
//_______________________________________________________________________________________________________
/* ======================================================================================================
 *      Initialisation
 * ======================================================================================================
 */
#include <cstdint>
#include "Arduino.h"
#include "i2cCommunication.h"
#include "moonBoard.h"

#include "Asservissement.h"

#include "Clamp.h"

#include "SensorsBoard.h"
// #include "I2Cdev.h"
// #include "MPU6050.h"

#define _DEBUG_BOT_ true

#define _PIN_BOUTON_ARRET_URGENCE_ 16 //état haut quand il est appuyé
#define _PIN_TIRETTE_ 15
#define _PIN_ARDUINO_NANO_FUNNY_ACTION_ 24

#define _NUMBER_OF_CYLINDERS_TO_CATCH_ 6 //TODO: replace by the right number of cylinders
#define _NUMBER_OF_MOON_BASES_ 2         //TODO: replace by the right number of cylinders
#define _TEMPS_RECHERCHE_BASE_MAXIMUM_ 3000 //ms
#define _TEMPS_RECHERCHE_CYLINDRE_MAXIMUM_ 8000 //ms

#define _TIMER_LIMIT_ 80000

#define _SLOW_SPEED_ 0.35
#define _MEDIUM_SPEED_ 0.7
#define _FAST_SPEED_ 1.0

//TODO: replace by the right values
const float _BLUE_X_START_POSITION_ = 0.4;
const float _YELLOW_X_START_POSITION_ = 2.4;
const float _Y_START_POSITION_ = 0.2;
const float _BLUE_START_ANGLE_ = 0;
const float _YELLOW_START_ANGLE_ = PI;


// === Cylinders ===
const CylinderPosition cylinderToCatchList_1B[_NUMBER_OF_CYLINDERS_TO_CATCH_]={
        {1, 1.150, _Y_LIMIT_ - 0.0315}, {2, 1.150, _Y_LIMIT_ - 0.0315}, {3, 1.150, _Y_LIMIT_ - 0.0315},
        {4, 1.150, _Y_LIMIT_ - 0.0315}, {5, 0.200, _Y_LIMIT_ - 0.600}, {6, 0.800, _Y_LIMIT_ - 1.850}
};
const CylinderPosition cylinderToCatchList_2Y[_NUMBER_OF_CYLINDERS_TO_CATCH_]={
        {1, 1.850, _Y_LIMIT_ - 0.0315}, {2, 1.850, _Y_LIMIT_ - 0.0315}, {3, 1.850, _Y_LIMIT_ - 0.0315},
        {4, 2.800, _Y_LIMIT_ - 0.600}, {5, 1.850, _Y_LIMIT_ - 0.0315}, {6, 2.200, _Y_LIMIT_ - 1.850}
};

// === Moon Bases ===
const MoonBase moonBases_1B[_NUMBER_OF_MOON_BASES_]={
        {1, 0, _Y_LIMIT_ - 0.700, 0.080, _Y_LIMIT_ - 0.700, 0.080, _Y_LIMIT_ - 1.150, 0, _Y_LIMIT_ - 1.150},
        {2, 0, _Y_LIMIT_, 1.070, _Y_LIMIT_, 1.070, _Y_LIMIT_ - 0.360, 0, _Y_LIMIT_ - 0.360}
};

const MoonBase moonBases_2Y[_NUMBER_OF_MOON_BASES_]={
        {1, 0, _Y_LIMIT_, 1.070, _Y_LIMIT_, 1.070, _Y_LIMIT_ - 0.360, 0, _Y_LIMIT_ - 0.360},
        {2, 2.920, _Y_LIMIT_ - 0.700, _X_LIMIT_, _Y_LIMIT_ - 0.700, _X_LIMIT_, _Y_LIMIT_ - 1.150, 2.920, _Y_LIMIT_ - 1.150},
};


/* ======================================================================================================
 *      Class
 * ======================================================================================================
 */
class Bot
{
private:

Asservissement asservissement;
Clamp clamp;
SensorsBoard sensorsBoard;
// MPU6050 accelgyro;
CylinderPosition cylinderToCatchList[_NUMBER_OF_CYLINDERS_TO_CATCH_];

int colorNumber;
int strategyNumber;
int16_t angleZ;

//____________________
public:

Bot();
Bot(int newColorNumber, int newStrategyNumber);

Asservissement getAsservissement();
void setAsservissement(Asservissement newAsservissement);
Clamp getClamp();
void setClamp(Clamp newClamp);
SensorsBoard getSensorsBoard();
void setSensorsBoard(SensorsBoard newSensorsBoard);
int getColorNumber();
void setColorNumber(int newColorNumber);
int getStrategyNumber();
void setStrategyNumber(int newStrategyNumber);
// MPU6050 getAccelgyro();
// void setAccelgyro(MPU6050 newAccelgyro);
int16_t getAngleZ();
void setAngleZ(int16_t newAngleZ);
const CylinderPosition *getCylinderToCatchList() const;

bool isEmergencyStopButtonOn();
void handleEmergencyStopButton();

bool isTiretteTiree();

void startFunnyActionTimer();

bool turnBotInFrontOFCylinder();
void catchCylinder();
void findCylinder();
void findMoonBase();
void findAndCatchCylinder();
void releaseCylinderInBase();

void initializePosition();
void goInFrontOfRocketCylinders(unsigned long timer);
void handleRocketCylinders(unsigned long timer);
void buildBase();

// void build1BaseCylinder(float x_coord, float y_coord);
//
// void goToPosition(int xAxis, int yAxis);
//
// void updateAngleZ();

};

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
#include "I2Cdev.h"
#include "MPU6050.h"


#define _PIN_BOUTON_ARRET_URGENCE_ 16 //état haut quand il est appuyé
#define _PIN_TIRETTE_ 15
#define _PIN_ARDUINO_NANO_FUNNY_ACTION_ 33

#define _NUMBER_OF_CYLINDERS_TO_CATCH_ 10 //TODO: replace by the right number of cylinders
#define _TEMPS_RECHERCHE_CYLINDRE_MAXIMUM_ 8000 //ms

#define _SLOW_SPEED_ 0.4
#define _MEDIUM_SPEED_ 0.8
#define _FAST_SPEED_ 1.2


//TODO: replace by the right positions of cylinders
const CylinderPosition cylinderToCatchList_1B[_NUMBER_OF_CYLINDERS_TO_CATCH_]={
        {1, 1.3, 124.34}, {2, 1, 1}, {3, 1, 1}, {4, 1, 1}, {5, 1, 1},
        {6, 1, 1}, {7, 1, 1},{8, 1, 1}, {9, 1, 1}, {10, 1, 1}
};
const CylinderPosition cylinderToCatchList_2Y[_NUMBER_OF_CYLINDERS_TO_CATCH_]={
        {1, 1.3, 124.34}, {2, 1, 1}, {3, 1, 1}, {4, 1, 1}, {5, 1, 1},
        {6, 1, 1}, {7, 1, 1},{8, 1, 1}, {9, 1, 1}, {10, 1, 1}
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
MPU6050 accelgyro;
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

MPU6050 getAccelgyro();
void setAccelgyro(MPU6050 newAccelgyro);
int16_t getAngleZ();
void setAngleZ(int16_t newAngleZ);
const CylinderPosition *getCylinderToCatchList() const;

bool isTiretteTiree();

void goToPosition(int xAxis, int yAxis);

void startFunnyActionTimer();

void updateAngleZ();

bool turnBotInFrontOFCylinder();
void catchCylinder();
void findAndCatchCylinder();
void releaseCylinderInBase();
void build1BaseCylinder(float x_coord, float y_coord);
void buildBase();


};

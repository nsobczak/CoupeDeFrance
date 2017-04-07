/**
 *    \file Bot.h
 *    \brief classe robot
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
#include "Clamp.h"
#include "SensorsBoard.h"
#include "I2Cdev.h"
#include "MPU6050.h"

//TODO: replace by the right pin
#define _PIN_ARDUINO_NANO_FUNNY_ACTION_ 13


/* ======================================================================================================
 *      Class
 * ======================================================================================================
 */
class Bot
{
private:

Clamp clamp;
SensorsBoard sensorsBoard;
MPU6050 accelgyro;

int colorNumber;
int strategyNumber;
int16_t angleZ;

//____________________
public:

Bot();
Bot(int newColorNumber, int newStrategyNumber);

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

void startFunnyActionTimer();

void updateAngleZ();

//TODO: add les fonctions de la pince

};

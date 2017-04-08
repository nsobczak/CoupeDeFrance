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
#include "i2cCommunication.h"
#include "Clamp.h"
#include "SensorsBoard.h"
#include "I2Cdev.h"
#include "MPU6050.h"

//TODO: replace by the right pin#define _ASSERVISSMENT_BOTTURN_ 2
#define _PIN_ARDUINO_NANO_FUNNY_ACTION_ 13

#define _ASSERVISSMENT_SENDADRESS_ 6

#define _ASSERVISSMENT_BOTGOFORWARD_ 1
#define _ASSERVISSMENT_BOTGOBACKWARD_ 2
#define _ASSERVISSMENT_BOTTURNRIGHT_ 3
#define _ASSERVISSMENT_BOTTURNLEFT_ 4
#define _ASSERVISSMENT_BOTSTOP_ 5



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


void botGoForward(int sendAddress, int speed);
void botGoBackward(int sendAddress, int speed);
void botTurnAroundRight(int sendAddress, int speed);
void botTurnAroundLeft(int sendAddress, int speed);
void botStop(int sendAddress);

void startFunnyActionTimer();

void updateAngleZ();

//TODO: add les fonctions de la pince

};

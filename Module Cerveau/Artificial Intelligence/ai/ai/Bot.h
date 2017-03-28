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


/* ======================================================================================================
 *      Class
 * ======================================================================================================
 */
class Bot
{
private:

Clamp clamp;
SensorsBoard sensorsBoard;

int colorNumber;
int strategyNumber;

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


//TODO: add les fonctions de la pince

};

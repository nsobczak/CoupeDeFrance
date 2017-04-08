/**
 *    \file Bot.cpp
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
#include "Bot.h"


/* ======================================================================================================
 *      Constructor, getter, setter
 * ======================================================================================================
 */
Bot::Bot(){
}

Bot::Bot(int newColorNumber, int newStrategyNumber)
{
        //couleur: 1 => bleu; 2 => jaune
        //strategie: 1 => ? ; 2 => ?
        if (newColorNumber != 1 && newColorNumber != 2) newColorNumber = 1;
        if (newStrategyNumber != 1 && newStrategyNumber != 2) newStrategyNumber = 1;
        this->colorNumber = newColorNumber;
        this->strategyNumber = newStrategyNumber;
}

Clamp Bot::getClamp(){
        return this->clamp;
}
void Bot::setClamp(Clamp newClamp){
        this->clamp = newClamp;
}

SensorsBoard Bot::getSensorsBoard(){
        return this->sensorsBoard;
}
void Bot::setSensorsBoard(SensorsBoard newSensorsBoard){
        this->sensorsBoard = newSensorsBoard;
}

int Bot::getColorNumber(){
        return this->colorNumber;
}
void Bot::setColorNumber(int newColorNumber){
        if (newColorNumber != 1 && newColorNumber != 2) newColorNumber = 1;
        this->colorNumber = newColorNumber;
}

int Bot::getStrategyNumber(){
        return this->strategyNumber;
}
void Bot::setStrategyNumber(int newStrategyNumber){
        if (newStrategyNumber != 1 && newStrategyNumber != 2) newStrategyNumber = 1;
        this->strategyNumber = newStrategyNumber;
}


MPU6050 Bot::getAccelgyro()
{
        return this->accelgyro;
}
void Bot::setAccelgyro(MPU6050 newAccelgyro)
{
        this->accelgyro = newAccelgyro;
}

int16_t Bot::getAngleZ()
{
        return this->angleZ;
}
void Bot::setAngleZ(int16_t newAngleZ)
{
        this->angleZ = newAngleZ;
}


/* ======================================================================================================
 *      Methods
 * ======================================================================================================
 */

// === FUNNY ACTION ===

/* \fn void Bot::startFunnyActionTimer
 * \brief fonction qui lance la funny action
 */
void Bot::startFunnyActionTimer()
{
        //TODO: write etat haut dans le pin de la nano, puis le remettre à bas
        analogWrite(_PIN_ARDUINO_NANO_FUNNY_ACTION_, HIGH);
}


// === ANGLE ===

/* \fn void Bot::updateAngleZ()
 * \brief fonction qui récupère l'angle à partir du gyroscope et qui actualise la variable du robot correspondante
 */
void Bot::updateAngleZ()
{
        //TODO: see if we need to initialize accelgyro in a different function that updateAngleZ
        //TODO: see if endTransmission fou la merde ou pas
        Wire.begin();
        this->getAccelgyro().initialize();
        this->setAngleZ(this->getAccelgyro().getRotationZ());
        Wire.endTransmission();
}


// === CYLINDER ===

//TODO: fonction qui va regarder où se trouve le cylindre de manière précise avec les fonctions check bottom sensors pui qui va attraper le cylindre
//TODO: fonction qui va attraper le cylindre, déplacer le robot, lacher le cylindre
//TODO: fonction qui va rammasser les cylindres dans un certain ordre suivant la stratégie

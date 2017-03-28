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


/* ======================================================================================================
 *      Methods
 * ======================================================================================================
 */
//TODO: fonction qui va rammasser les cylindres dans un certain ordre suivant la stratégie

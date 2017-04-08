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

        // copy structure using memcpy
        for (int i = 0; i < _NUMBER_OF_CYLINDERS_TO_CATCH_; i++)
        {
                if (this->colorNumber == 1) memcpy ( &this->cylinderToCatchList[i], &cylinderToCatchList_1B[i], sizeof(CylinderPosition) );
                else memcpy ( &this->cylinderToCatchList[i], &cylinderToCatchList_2Y[i], sizeof(CylinderPosition) );
        }
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

MPU6050 Bot::getAccelgyro()
{
        return this->accelgyro;
}
void Bot::setAccelgyro(MPU6050 newAccelgyro)
{
        this->accelgyro = newAccelgyro;
}

const CylinderPosition *Bot::getCylinderToCatchList() const
{
        return cylinderToCatchList;
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

// === BOT TRAVEL ===

/* \fn void Bot::botGoForward
 * \param int sendAddress, int speed
 * \brief fonction qui déplace le robot vers l'avant
 */
void Bot::botGoForward(int sendAddress, int speed)
{
        //TODO: I2C - envoyer info de déplacer le robot au module asservissement
        // conversion sur 2 octets de la valeur à envoyer
        byte bytesTab[2];
        intTo2Bytes(bytesTab, speed);
        i2csend3bytes(_ASSERVISSMENT_BOTGOFORWARD_, bytesTab[0], bytesTab[1], _ASSERVISSMENT_SENDADRESS_);
}

/* \fn void Bot::botGoBackward
 * \param int sendAddress, int speed
 * \brief fonction qui déplace le robot vers l'arrière
 */
void Bot::botGoBackward(int sendAddress, int speed)
{
        //TODO: I2C - envoyer info de déplacer le robot au module asservissement
        // conversion sur 2 octets de la valeur à envoyer
        byte bytesTab[2];
        intTo2Bytes(bytesTab, speed);
        i2csend3bytes(_ASSERVISSMENT_BOTGOBACKWARD_, bytesTab[0], bytesTab[1], _ASSERVISSMENT_SENDADRESS_);
}

void Bot::botTurnAroundRight(int sendAddress, int speed)
{
        //TODO: I2C - envoyer info de tourner le robot vers la droite au module asservissement
        // conversion sur 2 octets de la valeur à envoyer
        byte bytesTab[2];
        intTo2Bytes(bytesTab, speed);
        i2csend3bytes(_ASSERVISSMENT_BOTTURNRIGHT_, bytesTab[0], bytesTab[1], _ASSERVISSMENT_SENDADRESS_);
}

void Bot::botTurnAroundLeft(int sendAddress, int speed)
{
        //TODO: I2C - envoyer info de tourner le robot vers la gauche au module asservissement
        // conversion sur 2 octets de la valeur à envoyer
        byte bytesTab[2];
        intTo2Bytes(bytesTab, speed);
        i2csend3bytes(_ASSERVISSMENT_BOTTURNLEFT_, bytesTab[0], bytesTab[1], _ASSERVISSMENT_SENDADRESS_);
}

void Bot::botStop(int sendAddress)
{
        //TODO: I2C - envoyer info d'arrêter le robot au module asservissement
        // conversion sur 2 octets de la valeur à envoyer
        byte bytesTab[2];
        intTo2Bytes(bytesTab, 1);
        i2csend3bytes(_ASSERVISSMENT_BOTSTOP_, bytesTab[0], bytesTab[1], _ASSERVISSMENT_SENDADRESS_);
}


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

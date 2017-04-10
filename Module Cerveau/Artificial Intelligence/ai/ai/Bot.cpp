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

// === BOT TIRETTE ===

bool Bot::isTiretteTiree()
{
        pinMode(_PIN_TIRETTE_, INPUT);
        if (digitalRead(_PIN_TIRETTE_) == LOW) return true; //tirette à l'état bas => lance le robot
        else return false;
}

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

bool Bot::turnBotInFrontOFCylinder()
{
        bool inFrontOfCylinder = false;
        unsigned long timer = millis();

        while (!inFrontOfCylinder && (millis() - timer < _TEMPS_RECHERCHE_CYLINDRE_MAXIMUM_))
        {
                //TODO: on le fait tourner arbitrairement vers la droite
                unsigned long timer2 = millis();
                do {
                        this->botTurnAroundRight(_ASSERVISSMENT_SENDADRESS_, 200);
                        // this->updateAngleZ();
                } while(millis() - timer2 < 3000); //TODO: see the right time + include  && this->getAngleZ()
                if (this->getSensorsBoard().checkForCylinderOnSensorFrontBottomRight()) {
                        //tourner le robot vers la droite
                        this->botTurnAroundRight(_ASSERVISSMENT_SENDADRESS_, 200);  //TODO: replace by the right speed
                }
                else if (this->getSensorsBoard().checkForCylinderOnSensorFrontBottomLeft()) {
                        //tourner le robot vers la gauche
                        this->botTurnAroundLeft(_ASSERVISSMENT_SENDADRESS_, 200);  //TODO: replace by the right speed
                }

                inFrontOfCylinder = this->getSensorsBoard().checkForCylinderOnSensorFrontBottomCenter();
        }
        return inFrontOfCylinder;
}


void Bot::catchCylinder()
{
        unsigned long timer = millis();
        do {
                this->botGoForward(_ASSERVISSMENT_SENDADRESS_, 200);
        } while(this->getSensorsBoard().getInfraredSensorFrontBottomCenterValue() > _DISTANCE_WHERE_CYLINDER_IS_READY_TO_BE_CAUGHT_
                && (millis() - timer < 3000));
        this->getClamp().catchCylinder();
}


//TODO: fonction qui va regarder où se trouve le cylindre de manière précise avec les fonctions check bottom sensors puis qui va attraper le cylindre
void Bot::findAndCatchCylinder()
{
        if (this->getSensorsBoard().checkForCylinder())
        {
                //cylindre présent à la bonne distance sur un des 3 capteurs
                if (this->getSensorsBoard().checkForCylinderOnSensorFrontBottomCenter()) this->catchCylinder();
                else{
                        if (this->getSensorsBoard().checkForCylinderOnSensorFrontBottomRight()) {
                                //tourner le robot vers la droite
                                this->botTurnAroundRight(_ASSERVISSMENT_SENDADRESS_, 200);  //TODO: replace by the right speed
                        }
                        else if (this->getSensorsBoard().checkForCylinderOnSensorFrontBottomLeft()) {
                                //tourner le robot vers la gauche
                                this->botTurnAroundLeft(_ASSERVISSMENT_SENDADRESS_, 200);  //TODO: replace by the right speed
                        }
                        else this->turnBotInFrontOFCylinder();   //tourner le robot de droite a gauche pour chercher le cylindre
                }
        }
}


//TODO: fonction qui va regarder où se trouve la base de manière précise avec les fonctions check bottom sensors puis qui va lacher le cylindre
void Bot::releaseCylinderInBase()
{
        //TODO: check for base

        this->getClamp().releaseCylinder();
}


//TODO: fonction qui va attraper le cylindre, déplacer le robot, lacher le cylindre
void Bot::build1BaseCylinder(float x_coord, float y_coord)
{
        this->findAndCatchCylinder();
        //TODO: go near theoretical base position
        this->releaseCylinderInBase();
}


//TODO: fonction qui va rammasser les cylindres dans un certain ordre suivant la stratégie
void Bot::buildBase()
{
        //TODO: regarder comment on peut passer le timer partout pour arrêter le robot si on est à la fin
        unsigned long timer = millis();

        float x_coord;
        float y_coord;
        for (int i = 0; i < _NUMBER_OF_CYLINDERS_TO_CATCH_; i++)
        {
                if (millis() - timer < 70000) //TODO: on se donne 20" pour aller attraper et déplacer un robot
                {
                        x_coord = cylinderToCatchList[i].cylinder_x;
                        y_coord = cylinderToCatchList[i].cylinder_y;
                        this->build1BaseCylinder(x_coord, y_coord);
                }
        }
        this->botStop(_ASSERVISSMENT_SENDADRESS_);
}

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
        //strategie: 1 => safe ; 2 => risky
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

Asservissement Bot::getAsservissement()
{
        return this->asservissement;
}
void Bot::setAsservissement(Asservissement newAsservissement)
{
        this->asservissement = newAsservissement;
}

Clamp Bot::getClamp()
{
        return this->clamp;
}
void Bot::setClamp(Clamp newClamp)
{
        this->clamp = newClamp;
}

SensorsBoard Bot::getSensorsBoard()
{
        return this->sensorsBoard;
}
void Bot::setSensorsBoard(SensorsBoard newSensorsBoard)
{
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


int Bot::getColorNumber()
{
        return this->colorNumber;
}
void Bot::setColorNumber(int newColorNumber){
        if (newColorNumber != 1 && newColorNumber != 2) newColorNumber = 1;
        this->colorNumber = newColorNumber;
}

int Bot::getStrategyNumber()
{
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

// === BOT EMERGENCY STOP BUTTON ===

/**
 * \fn bool Bot::isEmergencyStopButtonOn
 * \brief fonction qui regarde si la tirette est tirée ou pas
 * \return true si le bouton est enclenché
 */
bool Bot::isEmergencyStopButtonOn()
{
        pinMode(_PIN_BOUTON_ARRET_URGENCE_, INPUT);
        if (_DEBUG_BOT_) Serial.println(digitalRead(_PIN_BOUTON_ARRET_URGENCE_));
        if (digitalRead(_PIN_BOUTON_ARRET_URGENCE_) == LOW) return true;
        else return false;
}

/**
 * \fn void Bot::handleEmergencyStopButton
 * \brief fonction qui fige le robot dans une boucle infini si le bouton d'arrêt d'urgence est enclenché
 */
void Bot::handleEmergencyStopButton()
{
        if (_DEBUG_BOT_) Serial.println("handleEmergencyStopButton");
        while (this->isEmergencyStopButtonOn()) delay(2000);
}


// === BOT TIRETTE ===

/**
 * \fn bool Bot::isTiretteTiree
 * \brief fonction qui regarde si la tirette est tirée ou pas
 * \return true si la tirette est tirée
 */
bool Bot::isTiretteTiree()
{
        pinMode(_PIN_TIRETTE_, INPUT);
        if (digitalRead(_PIN_TIRETTE_) == LOW) return true;  //tirette à l'état bas => lance le robot
        else return false;
}


// === FUNNY ACTION ===

/**
 * \fn void Bot::startFunnyActionTimer
 * \brief fonction qui lance la funny action
 */
void Bot::startFunnyActionTimer()
{
        pinMode(_PIN_ARDUINO_NANO_FUNNY_ACTION_, OUTPUT);
        digitalWrite(_PIN_ARDUINO_NANO_FUNNY_ACTION_, HIGH);
}


// === CYLINDER ===

bool Bot::turnBotInFrontOFCylinder()
{
        bool inFrontOfCylinder = false;
        unsigned long timer = millis();

        while (!inFrontOfCylinder && (millis() - timer < _TEMPS_RECHERCHE_CYLINDRE_MAXIMUM_))
        {                //TODO: on le fait tourner arbitrairement vers la droite
                unsigned long timer2 = millis();
                do {
                        this->getAsservissement().botTurnAroundRight(PI/12, _SLOW_SPEED_);         //TODO: replace by the right speed
                } while((this->getAsservissement().isOrderFinished() != 1) && (millis() - timer2 < 3000));         //TODO: see the right time
                this->getAsservissement().handleRotationOrderEnd();
                if (this->getSensorsBoard().checkForCylinderOnSensorFrontBottomRight())
                {
                        do {         //tourner le robot vers la droite
                                this->getAsservissement().botTurnAroundRight(PI/12, _SLOW_SPEED_);
                        } while((this->getAsservissement().isOrderFinished() != 1));
                        this->getAsservissement().handleRotationOrderEnd();
                }
                else if (this->getSensorsBoard().checkForCylinderOnSensorFrontBottomLeft())
                {
                        do {         //tourner le robot vers la gauche
                                this->getAsservissement().botTurnAroundLeft(PI/12, _SLOW_SPEED_);
                        } while((this->getAsservissement().isOrderFinished() != 1));
                        this->getAsservissement().handleRotationOrderEnd();
                }
                inFrontOfCylinder = this->getSensorsBoard().checkForCylinderOnSensorFrontBottomCenter();
        }
        return inFrontOfCylinder;
}


void Bot::catchCylinder()
{
        unsigned long timer = millis();
        do {
                this->getAsservissement().botGoForward(0.01, _SLOW_SPEED_); //TODO: check 0.01
        } while(this->getSensorsBoard().getInfraredSensorFrontBottomCenterValue() > _DISTANCE_WHERE_CYLINDER_IS_READY_TO_BE_CAUGHT_
                && (millis() - timer < 3000));
        this->handleEmergencyStopButton();
        this->getClamp().catchCylinder();
}


//fonction qui va regarder où se trouve le cylindre de manière précise avec les fonctions check bottom sensors
void Bot::findCylinder()
{
        if (this->getSensorsBoard().checkForCylinder() && !this->getSensorsBoard().checkForCylinderOnSensorFrontBottomCenter())
        {
                if (this->getSensorsBoard().checkForCylinderOnSensorFrontBottomRight())
                {
                        do { //tourner le robot vers la droite
                                this->getAsservissement().botTurnAroundRight(PI/12, _SLOW_SPEED_);
                        } while((this->getAsservissement().isOrderFinished() != 1));
                        this->getAsservissement().handleRotationOrderEnd();
                }
                else if (this->getSensorsBoard().checkForCylinderOnSensorFrontBottomLeft())
                {
                        do { //tourner le robot vers la gauche
                                this->getAsservissement().botTurnAroundLeft(PI/12, _SLOW_SPEED_);
                        } while((this->getAsservissement().isOrderFinished() != 1));
                        this->getAsservissement().handleRotationOrderEnd();
                }
                else this->turnBotInFrontOFCylinder();           //tourner le robot de droite a gauche pour chercher le cylindre
        }
}


void Bot::findMoonBase()
{
        unsigned long timer = millis();
        if (this->getSensorsBoard().checkForBase() &&
            !this->getSensorsBoard().checkForCylinderOnSensorFrontBottomCenter()) //cylindre présent à la bonne distance sur un des 3 capteurs
        {
                do {          //faire avancer le robot vers l'avant
                        this->getAsservissement().botGoForward(0.05, _SLOW_SPEED_);         //TODO: replace by the right speed
                } while((!this->getSensorsBoard().checkForBaseOnSensorFrontBottomCenter() ||
                         !this->getSensorsBoard().checkForBaseOnSensorFrontBottomLeft() ||
                         !this->getSensorsBoard().checkForBaseOnSensorFrontBottomRight())
                        && (millis() - timer < _TEMPS_RECHERCHE_BASE_MAXIMUM_));
        }
}


void Bot::findAndCatchCylinder()
{
        this->findCylinder();
        this->catchCylinder();
}


//fonction qui va regarder où se trouve la base de manière précise avec les fonctions check bottom sensors puis qui va lacher le cylindre
void Bot::releaseCylinderInBase()
{
        this->findMoonBase();
        this->handleEmergencyStopButton();
        this->getClamp().releaseCylinder();
}


//__________________________________________________________________________________
/**
 * \fn void initializePosition()
 * \brief initialiser la position avec les bonnes distances: 1 = bleu, 2 = jaune
 */
void Bot::initializePosition()
{
        if (this->getColorNumber() == 1) //blue
        {
                this->getAsservissement().setX_position(_BLUE_X_START_POSITION_);
                this->getAsservissement().setAngle_position(_BLUE_START_ANGLE_);
        }
        else // yellow
        {
                this->getAsservissement().setX_position(_YELLOW_X_START_POSITION_);
                this->getAsservissement().setAngle_position(_YELLOW_START_ANGLE_);
        }
        this->getAsservissement().setY_position(_Y_START_POSITION_);
}


/**
 * \fn void goInFrontOfRocketCylinders()
 * \param unsigned long timer
 * \brief fonction qui amène le robot devant la fusée contenant les cylindres
 */
void Bot::goInFrontOfRocketCylinders(unsigned long timer)
{
        if (millis() - timer < _TIMER_LIMIT_) this->getAsservissement().botGoForward(0.3, _SLOW_SPEED_);
        if (this->getColorNumber() == 1) //blue
        {
                if (millis() - timer < _TIMER_LIMIT_) this->getAsservissement().botTurnAroundLeft(PI/2, _SLOW_SPEED_);
                if (millis() - timer < _TIMER_LIMIT_) this->getAsservissement().botGoForward((1.15 - _BLUE_X_START_POSITION_), _SLOW_SPEED_);
                if (millis() - timer < _TIMER_LIMIT_) this->getAsservissement().botTurnAroundLeft(PI/2, _SLOW_SPEED_);
        }
        else // yellow
        {
                if (millis() - timer < _TIMER_LIMIT_) this->getAsservissement().botTurnAroundRight(PI/2, _SLOW_SPEED_);
                if (millis() - timer < _TIMER_LIMIT_) this->getAsservissement().botGoForward((_YELLOW_X_START_POSITION_ - 1.85), _SLOW_SPEED_);
                if (millis() - timer < _TIMER_LIMIT_) this->getAsservissement().botTurnAroundRight(PI/2, _SLOW_SPEED_);
        }
}


/**
 * \fn void handleRocketCylinders()
 * \param unsigned long timer
 * \brief fonction qui s'occupe des cylindres de la fusée
 */
void Bot::handleRocketCylinders(unsigned long timer)
{
        if (millis() - timer < _TIMER_LIMIT_) this->getAsservissement().botGoForward((0.25 - _Y_START_POSITION_), _SLOW_SPEED_);
        // if (millis() - timer < _TIMER_LIMIT_) this->findAndCatchCylinder();
        if (millis() - timer < _TIMER_LIMIT_) this->getClamp().catchCylinder();
        if (millis() - timer < _TIMER_LIMIT_) this->getAsservissement().botGoBackward((0.25 - _Y_START_POSITION_), _SLOW_SPEED_);
        if (millis() - timer < _TIMER_LIMIT_) this->getClamp().bringUpCylinder();

        if ((this->getColorNumber() == 1) && // blue
            (millis() - timer < _TIMER_LIMIT_)) this->getAsservissement().botTurnAroundLeft(PI/2, _SLOW_SPEED_);  //yellow
        else if (millis() - timer < _TIMER_LIMIT_) this->getAsservissement().botTurnAroundRight(PI/2, _SLOW_SPEED_);

        if (millis() - timer < _TIMER_LIMIT_) this->getAsservissement().botGoForward(0.25, _SLOW_SPEED_);
        if (millis() - timer < _TIMER_LIMIT_) this->getClamp().releaseCylinder();
        if (millis() - timer < _TIMER_LIMIT_) this->getAsservissement().botGoBackward(0.25, _SLOW_SPEED_);

        if ((this->getColorNumber() == 1) && // blue
            (millis() - timer < _TIMER_LIMIT_)) this->getAsservissement().botTurnAroundRight(PI/2, _SLOW_SPEED_);
        else if (millis() - timer < _TIMER_LIMIT_) this->getAsservissement().botTurnAroundLeft(PI/2, _SLOW_SPEED_);  //yellow
}


/**
 * \fn void buildBase()
 * \brief fonction qui s'occupe des cylindres (construit la base)
 */
void Bot::buildBase()
{
        //TODO: regarder comment on peut passer le timer partout pour arrêter le robot si on est à la fin
        unsigned long timer = millis();

        this->goInFrontOfRocketCylinders(timer);
        //cylindres de la fusée
        for (int i = 0; i < 4; i++) handleRocketCylinders(timer);

        //cylindre
        // if (millis() - timer < _TIMER_LIMIT_)

        this->getAsservissement().botStop();
}







/*
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
                           // this->build1BaseCylinder(x_coord, y_coord);
                   }
           }
           this->getAsservissement().botStop();
   }
 */


// === BOT TRAVEL ===

// void Bot::goToPosition(int xAxis, int yAxis)
// {
//         //TODO: déplacer le robot à une position donnée par rapport à la position actuelle
// }


// === ANGLE ===

/* \fn void Bot::updateAngleZ()
 * \brief fonction qui récupère l'angle à partir du gyroscope et qui actualise la variable du robot correspondante
 */
// void Bot::updateAngleZ()
// {
//         //TODO: see if we need to initialize accelgyro in a different function that updateAngleZ
//         //TODO: see if endTransmission fout la merde ou pas
//         // Wire.begin();
//         this->getAccelgyro().initialize();
//         this->setAngleZ(this->getAccelgyro().getRotationZ());
//         // Wire.endTransmission();
// }

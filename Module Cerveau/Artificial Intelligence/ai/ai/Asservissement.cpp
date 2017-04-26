/**
 *    \file Asservissement.cpp
 *    \brief classe asservissement
 *
 *    \author Nicolas SOBCZAK
 *    \date Avril 2017
 */
//_______________________________________________________________________________________________________
/* ======================================================================================================
 *      Include
 * ======================================================================================================
 */
#include "Asservissement.h"


/* ======================================================================================================
 *      Constructor, getter, setter
 * ======================================================================================================
 */

Asservissement::Asservissement()
{
}

bool Asservissement::getOrderFinished()
{
        return this->orderFinished;
}
void Asservissement::setOrderFinished(bool state)
{
        this->orderFinished = state;
}

unsigned int Asservissement::getTick_codeuse_l()
{
        return this->tick_codeuse_l;
}
void Asservissement::setTick_codeuse_l(unsigned int ticks)
{
        this->tick_codeuse_l = ticks;
}

unsigned int Asservissement::getTick_codeuse_r()
{
        return this->tick_codeuse_r;
}
void Asservissement::setTick_codeuse_r(unsigned int ticks)
{
        this->tick_codeuse_r = ticks;
}

float Asservissement::getDistanceParcourue()
{
        return this->distanceParcourue;
}
void Asservissement::setDistanceParcourue(float distance)
{
        this->distanceParcourue = distance;
}

float Asservissement::getAngleEffectue()
{
        return this->angleEffectue;
}
void Asservissement::setAngleEffectue(float angle)
{
        this->angleEffectue = angle;
}

float Asservissement::getX_position()
{
        return x_position;
}
void Asservissement::setX_position(float x_position)
{
        this->x_position = x_position;
}

float Asservissement::getY_position()
{
        return y_position;
}
void Asservissement::setY_position(float y_position)
{
        this->y_position = y_position;
}

float Asservissement::getAngle_position()
{
        return angle_position;
}
void Asservissement::setAngle_position(float angle_position)
{
        this->angle_position = angle_position;
}


/* ======================================================================================================
 *      Methods
 * ======================================================================================================
 */
// === Orders ===
/** \fn void Asservissement::botGoForward
 *  \param int sendAddress, int speed
 *  \brief fonction qui déplace le robot vers l'avant
 */
void Asservissement::botGoForward(double distance, double speed)
{
        this->setOrderFinished(0);
        //TODO: I2C - envoyer info de déplacer le robot au module asservissement
        int distanceIntPart = (int)distance;
        double distanceDoubleDecPart = (distance - distanceIntPart)*_DISTANCE_PRECISION_;
        int distanceIntDecPart = (int)distanceDoubleDecPart;

        int speedIntPart = (int)speed;
        double speedDoubleDecPart = (speed - speedIntPart)*_SPEED_PRECISION_;
        int speedIntDecPart = (int)speedDoubleDecPart;

        i2csend5bytes(_ASSERVISSMENT_BOTGOFORWARD_, distanceIntPart, distanceIntDecPart, speedIntPart, speedIntDecPart, _ASSERVISSMENT_SENDADRESS_);
}

/** \fn void Asservissement::botGoBackward
 *  \param int sendAddress, int speed
 *  \brief fonction qui déplace le robot vers l'arrière
 */
void Asservissement::botGoBackward(double distance, double speed)
{
        this->setOrderFinished(0);
        //TODO: I2C - envoyer info de déplacer le robot au module asservissement
        int distanceIntPart = (int)distance;
        double distanceDoubleDecPart = (distance - distanceIntPart)*_DISTANCE_PRECISION_;
        int distanceIntDecPart = (int)distanceDoubleDecPart;

        int speedIntPart = (int)speed;
        double speedDoubleDecPart = (speed - speedIntPart)*_SPEED_PRECISION_;
        int speedIntDecPart = (int)speedDoubleDecPart;

        i2csend5bytes(_ASSERVISSMENT_BOTGOBACKWARD_, distanceIntPart, distanceIntDecPart, speedIntPart, speedIntDecPart, _ASSERVISSMENT_SENDADRESS_);
}

void Asservissement::botTurnAroundRight(double angle, double speed)
{
        this->setOrderFinished(0);
        //TODO: I2C - envoyer info de tourner le robot vers la droite au module asservissement
        int angleIntPart = (int)angle;
        double angleDoubleDecPart = (angle - angleIntPart)*_DISTANCE_PRECISION_;
        int angleIntDecPart = (int)angleDoubleDecPart;

        int speedIntPart = (int)speed;
        double speedDoubleDecPart = (speed - speedIntPart)*_SPEED_PRECISION_;
        int speedIntDecPart = (int)speedDoubleDecPart;

        i2csend5bytes(_ASSERVISSMENT_BOTTURNRIGHT_, angleIntPart, angleIntDecPart, speedIntPart, speedIntDecPart, _ASSERVISSMENT_SENDADRESS_);
}

void Asservissement::botTurnAroundLeft(double angle, double speed)
{
        this->setOrderFinished(0);
        //TODO: I2C - envoyer info de tourner le robot vers la gauche au module asservissement
        int angleIntPart = (int)angle;
        double angleDoubleDecPart = (angle - angleIntPart)*_DISTANCE_PRECISION_;
        int angleIntDecPart = (int)angleDoubleDecPart;

        int speedIntPart = (int)speed;
        double speedDoubleDecPart = (speed - speedIntPart)*_SPEED_PRECISION_;
        int speedIntDecPart = (int)speedDoubleDecPart;

        i2csend5bytes(_ASSERVISSMENT_BOTTURNLEFT_, angleIntPart, angleIntDecPart, speedIntPart, speedIntDecPart, _ASSERVISSMENT_SENDADRESS_);
}

void Asservissement::botStop()
{
        //TODO: I2C - envoyer info d'arrêter le robot au module asservissement
        // conversion sur 2 octets de la valeur à envoyer
        byte bytesTab[2];
        intTo2Bytes(bytesTab, 1);
        i2csend5bytes(_ASSERVISSMENT_BOTSTOP_, bytesTab[0], bytesTab[1], 0, 0, _ASSERVISSMENT_SENDADRESS_);
}


// === I2C ===
void Asservissement::receive3bytesAndUpdate()
{
        /*Notes: tableau id i2c
                 10 => ordre_termine;
                 11 => somme_ordre_tick_codeuse_L;
                 12 => somme_ordre_tick_codeuse_R;
         */
        if (_DEBUG_ASSERVISSEMENT_) {Serial.print("Wire.available(): "); Serial.println(Wire.available()); }
        if (Wire.available() == 3)
        {
                //lecture de la variable
                byte var = Wire.read();
                //lecture des 2 octets suivants
                byte x = Wire.read();
                byte y = Wire.read();
                //reconstitution de la valeur
                byte bytesTab[2] = {x, y};
                int value = recoverIntFrom2Bytes(bytesTab);

                switch ( var ) // cf. les références des variables en haut du fichier
                {
                case 10:
                        if (_DEBUG_ASSERVISSEMENT_) {Serial.println("variable recue : ordre_termine");
                                      Serial.print("value\t"); Serial.println(value); }
                        if (value == 1) this->setOrderFinished(true);
                        else this->setOrderFinished(false);
                        break;
                case 11:
                        if (_DEBUG_ASSERVISSEMENT_) Serial.println("variable recue : somme_ordre_tick_codeuse_L");
                        this->setTick_codeuse_l(value);
                        break;
                case 12:
                        if (_DEBUG_ASSERVISSEMENT_) Serial.println("variable recue : somme_ordre_tick_codeuse_R");
                        this->setTick_codeuse_r(value);
                        break;
                default:
                        if (_DEBUG_ASSERVISSEMENT_) Serial.println("variable recue inconnue");
                }
        }
        // else de debug
        else if (_DEBUG_ASSERVISSEMENT_) Serial.println("Erreur : Pas 3 octets envoyes");
}


bool Asservissement::isOrderFinished()
{
        for (int i = 0; i < 3; i++)
        {
                Wire.requestFrom(_ASSERVISSMENT_SENDADRESS_, 3);
                this->receive3bytesAndUpdate();
                // delay(5);
        }
        if (_DEBUG_ASSERVISSEMENT_)  {Serial.print("this->getOrderFinished()\t =\t "); Serial.println(this->getOrderFinished()); }
        return this->getOrderFinished();
}

/**
 * \fn calculDistance
 * \param unsigned int tick_codeuse
 * \brief calcule la distance parcourue à partir d'un nombre de ticks
 * \return float distance parcourue en metre
 */
float Asservissement::calculDistance1Roue(unsigned int tick_codeuse)
{
        double nombre_tours = (double) tick_codeuse / (double) _NOMBRE_TICK_1_TOUR_ROUE_;
        return _PERIMETRE_ROUE_CODEUSE_ * (float)nombre_tours;
}


void Asservissement::computeAverageDistance()
{
        this->setDistanceParcourue( (this->calculDistance1Roue(this->getTick_codeuse_l()) + this->calculDistance1Roue(this->getTick_codeuse_r())) /2);
}


void Asservissement::computeRotationAngle()
{
        unsigned int ticks = max(this->getTick_codeuse_l(), this->getTick_codeuse_r());
        this->setAngleEffectue(ticks / _VOIE_ROUES_); //en rad
        if (_DEBUG_ASSERVISSEMENT_) {Serial.print("angle (rad)= "); Serial.println(this->getAngleEffectue()); }
}


void Asservissement::computePosition()
{
        this->setAngle_position(this->getAngle_position() + this->getAngleEffectue());
        this->setX_position(this->getX_position() + cos(this->getAngle_position())*this->getDistanceParcourue());
        this->setY_position(this->getY_position() + sin(this->getAngle_position())*this->getDistanceParcourue());
        this->setAngleEffectue(0);
        this->setDistanceParcourue(0);
}


void Asservissement::handleRotationOrderEnd()
{
        if (_DEBUG_ASSERVISSEMENT_) Serial.println("handleRotationOrderEnd");
        if (_DEBUG_ASSERVISSEMENT_) { Serial.print("this->getOrderFinished()\t=\t"); Serial.println(this->getOrderFinished()); }
        if (this->getOrderFinished() == 1) //this->isOrderFinished())
        {
                if (_DEBUG_ASSERVISSEMENT_) Serial.println("orderFinished + compute");
                this->computeRotationAngle();
                this->computePosition();
        }
}


void Asservissement::handleStraightOrderEnd()
{
        if (_DEBUG_ASSERVISSEMENT_) Serial.println("handleStraightOrderEnd");
        if (_DEBUG_ASSERVISSEMENT_) { Serial.print("this->getOrderFinished()\t=\t"); Serial.println(this->getOrderFinished()); }
        if (this->getOrderFinished() == 1) //this->isOrderFinished())
        {
                if (_DEBUG_ASSERVISSEMENT_) Serial.println("orderFinished + compute");
                this->computeAverageDistance();
                this->computePosition();
        }
}

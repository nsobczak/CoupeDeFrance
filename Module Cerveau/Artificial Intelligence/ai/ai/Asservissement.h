/**
 *    \file Asservissement.h
 *    \brief classe asservissement
 *
 *    \author Nicolas SOBCZAK
 *    \date Avril 2017
 */
//_______________________________________________________________________________________________________
/* ======================================================================================================
 *      Initialisation
 * ======================================================================================================
 */
#include "Arduino.h"
#include <Wire.h>
#include "i2cCommunication.h"

#define _DEBUG_ true

#define _ASSERVISSMENT_SENDADRESS_ 6

#define _ASSERVISSMENT_BOTGOFORWARD_ 1
#define _ASSERVISSMENT_BOTGOBACKWARD_ 2
#define _ASSERVISSMENT_BOTTURNRIGHT_ 3
#define _ASSERVISSMENT_BOTTURNLEFT_ 4
#define _ASSERVISSMENT_BOTSTOP_ 5


#define _SPEED_PRECISION_ 100
#define _DISTANCE_PRECISION_ 100

//=== CONSTANTES ===
const float _DIAMETRE_ROUE_CODEUSE_ = 0.05228; // 52,28mm
const int _NOMBRE_TICK_1_TOUR_ROUE_ = 1250;
const float _PERIMETRE_ROUE_CODEUSE_ = _DIAMETRE_ROUE_CODEUSE_*PI;
const float _VOIE_ROUES_ = 0.3; //en metre, ecart entre les roues d'un même essieu //TODO: replace by the right measure

/* ======================================================================================================
 *      Class
 * ======================================================================================================
 */
class Asservissement
{
private:
bool orderFinished;
unsigned int tick_codeuse_l;
unsigned int tick_codeuse_r;
float distanceParcourue;
float angleEffectue;
float x_position;
float y_position;


public:
Asservissement();
bool getOrderFinished();
void setOrderFinished(bool state);
unsigned int getTick_codeuse_l();
void setTick_codeuse_l(unsigned int ticks);
unsigned int getTick_codeuse_r();
void setTick_codeuse_r(unsigned int ticks);
float getDistanceParcourue();
void setDistanceParcourue(float distance);
float getAngleEffectue();
void setAngleEffectue(float angle);
float getX_position() const;
float getY_position() const;
void setX_position(float x_position);
void setY_position(float y_position);

void botGoForward(double distance, double speed);
void botGoBackward(double distance, double speed);
void botTurnAroundRight(double angle, double speed);
void botTurnAroundLeft(double angle, double speed);
void botStop();

void receive3bytesAndUpdate();
bool isOrderFinished();
float calculDistance1Roue(unsigned int tick_codeuse);
void computeAverageDistance();
void computeRotationAngle();
void computePosition();
void handleOrderEnd();


};

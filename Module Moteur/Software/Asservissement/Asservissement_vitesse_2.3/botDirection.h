/**
 *    \file botDirection.cpp
 *    \brief asservissement vitesse
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


#ifndef BOTDIRECTION_H
#define	BOTDIRECTION_H

#ifdef	__cplusplus
extern "C" {
#endif


/* ======================================================================================================
 *      Define
 * ======================================================================================================
 */
#define _MOTOR_R_ 6 // Attention sur Due PWM ou Pwm sont des keyword -> donc ne pas les utiliser pour des nom de variable
#define _MOTOR_L_ 3
#define _IN1_MOTOR_L_ 22
#define _IN2_MOTOR_L_ 23
#define _IN1_MOTOR_R_ 52
#define _IN2_MOTOR_R_ 53



//______________________________________________________________________________
/* ======================================================================================================
 *      Function
 * ======================================================================================================
 */
/**
 * \fn robotGo
 * \brief fonction pour diriger le robot
 * \param int pwmLeft, int pwmRight, int direction
 */
void robotGo(int pwmLeft, int pwmRight, int direction);

void robotStop();

void robotGoStraightAhead(int pwmLeft, int pwmRight);

void robotGoBack(int pwmLeft, int pwmRight);

/**
 * \fn robotTurnAround
 * \brief fonction faisant tourner le robot sur lui-même avec un seul moteur
 * \param int pwm, int direction
 */
void robotTurnAround(int pwm, int direction);

void robotTurnAroundFrontRight(int pwm);

void robotTurnAroundBackRight(int pwm);

void robotTurnAroundFrontLeft(int pwm);

void robotTurnAroundBackLeft(int pwm);


// =============================================================================
// =============================================================================
#ifdef	__cplusplus
}
#endif

#endif	/* BOTDIRECTION_H */

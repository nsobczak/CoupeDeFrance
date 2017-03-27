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
#include "botDirection.h"


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
void robotGo(int pwmLeft, int pwmRight, int direction)
{
    switch (direction) {
        case 1:
            digitalWrite(_IN1_MOTOR_R_, HIGH);
            digitalWrite(_IN2_MOTOR_R_, LOW);
            digitalWrite(_IN1_MOTOR_L_, HIGH);
            digitalWrite(_IN2_MOTOR_L_, LOW);
            break;
        case -1:
            digitalWrite(_IN1_MOTOR_R_, LOW);
            digitalWrite(_IN2_MOTOR_R_, HIGH);
            digitalWrite(_IN1_MOTOR_L_, LOW);
            digitalWrite(_IN2_MOTOR_L_, HIGH);
            break;
        default:
            digitalWrite(_IN1_MOTOR_R_, LOW);
            digitalWrite(_IN2_MOTOR_R_, LOW);
            digitalWrite(_IN1_MOTOR_L_, LOW);
            digitalWrite(_IN2_MOTOR_L_, LOW);
            break;
    }
    analogWrite(_MOTOR_L_, pwmLeft);
    analogWrite(_MOTOR_R_, pwmRight);
}

void robotStop()
{
    robotGo(0, 0, 0);
}

void robotGoStraightAhead(int pwmLeft, int pwmRight)
{
    robotGo(pwmLeft, pwmRight, 1);
}

void robotGoBack(int pwmLeft, int pwmRight)
{
    robotGo(pwmLeft, pwmRight, -1);
}

/**
 * \fn robotTurnAround
 * \brief fonction faisant tourner le robot sur lui-même avec un seul moteur
 * \param int pwm, int direction
 */
void robotTurnAround(int pwm, int direction)
{
    switch (direction) {
        case 1:
            digitalWrite(_IN1_MOTOR_R_, HIGH);
            digitalWrite(_IN2_MOTOR_R_, LOW);
            digitalWrite(_IN1_MOTOR_L_, LOW);
            digitalWrite(_IN2_MOTOR_L_, LOW);
            analogWrite(_MOTOR_L_, 255);
            analogWrite(_MOTOR_R_, pwm);
            break;
        case -1:
            digitalWrite(_IN1_MOTOR_R_, LOW);
            digitalWrite(_IN2_MOTOR_R_, HIGH);
            digitalWrite(_IN1_MOTOR_L_, LOW);
            digitalWrite(_IN2_MOTOR_L_, LOW);
            analogWrite(_MOTOR_L_, 255);
            analogWrite(_MOTOR_R_, pwm);
            break;
        case 2:
            digitalWrite(_IN1_MOTOR_R_, LOW);
            digitalWrite(_IN2_MOTOR_R_, LOW);
            digitalWrite(_IN1_MOTOR_L_, HIGH);
            digitalWrite(_IN2_MOTOR_L_, LOW);
            analogWrite(_MOTOR_L_, pwm);
            analogWrite(_MOTOR_R_, 255);
            break;
        case -2:
            digitalWrite(_IN1_MOTOR_R_, LOW);
            digitalWrite(_IN2_MOTOR_R_, LOW);
            digitalWrite(_IN1_MOTOR_L_, LOW);
            digitalWrite(_IN2_MOTOR_L_, HIGH);
            analogWrite(_MOTOR_L_, pwm);
            analogWrite(_MOTOR_R_, 255);
            break;
        default:
            robotStop();
            break;
    }
}

void robotTurnAroundBackLeft(int pwm)
{
    robotTurnAround(pwm, 1);
}

void robotTurnAroundFrontRight(int pwm)
{
    robotTurnAround(pwm, -1);
}

void robotTurnAroundBackRight(int pwm)
{
    robotTurnAround(pwm, 2);
}

void robotTurnAroundFrontLeft(int pwm)
{
    robotTurnAround(pwm, -2);
}

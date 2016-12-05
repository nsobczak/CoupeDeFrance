/**
 *    \file asservissement.cpp
 *    \brief Classe Asservissement. Code asservissement du grand Robot, permet d'asservir en angle et en position le robot pour envoyer une consigne vitesse au robot à l'aide d'un PID
 *    Il y a des define a mettre a jour.
 *
 *Codé pour "Spark core"  www.spark.io
 *
 *Par : Thibaut LOCQUET
 *      Janvier 2013
 *Modification: Clément LETELLIER
 *              Mars 2014
 *Modification: Nicolas SOBCZAK
 *              Octobre 2016
*/


/* ======================================================================================================
 *      Include
 * ======================================================================================================
 */
#include "asservissement.h"
#include "math.h"
#include "structures.h"
#include "odometrie.h"
#include "moteur.h"
#include "PID.h"
#include "Arduino.h"


/* ======================================================================================================
 *      Define
 * ======================================================================================================
 */
#define PI 3.14159


/* ======================================================================================================
 *      Class asservissement
 * ======================================================================================================
 */

  /**
 * \fn Asservissement   
 * \brief constructeur qui initialise et met des valeurs par defauts
 * \param ???
 * \param ??? 
 */
Asservissement::Asservissement(double periode, Moteur grobot):m_periode(periode), m_moteur(grobot)
{

    //init PID -> controle distance
    //PID distance(30.46,76.14,3.05,m_periode);
    //PID distance(0.7*0.6,0.002,140,m_periode);
    PID distance(1.8,0,0,m_periode);
    m_distance=distance;

    //init PID -> controle angle
    //PID angle(1056.0,2641.0,105.6,m_periode);
    //PID angle(130*0.6,0.002,140,m_periode); //kp=0.6*kplim  ki=1/(0.5*Tosc)  kd=0.125*Tosc

    //PID angle(150*0.7,0.00181,207.15,m_periode); //ok angle en Radian

    PID angle(7*0.6,0.0017,10.5,m_periode);
    //PID angle(7*0.6,0.0018,10.0,m_periode);
    m_angle=angle;

    m_commandeType=0;
    m_directionType=2;
    //Odometrie m_robot;
    m_stop=0;
    m_maxSpeed=60;

    //robot can't move
    L_done = 1;
    theta_done = 1;
}
/*
 * \fn Asservissement   
 * \brief constructeur qui initialise et met des valeurs par defauts
 * \param ???
 * \param ??? 
 */

double Asservissement::boundError(double e)
{
    double r = e;

    //borne l'angle entre Pi et -Pi
    while (r > 180)
    {
        r -= 2 * 180;
    }

    while (r < -180)
    {
        r += 2 * 180;
    }

    return r;
}


//contrôle la distance
/*
 * \fn contDistance    
 * \brief calcule la commande de distance ?? 
 * \param ???
 * \param ??? 
 */
double Asservissement::contDistance(Position destination, Position robot)
{

    double _PID_, error;

    //error =destination.x - robot.x ;
    error= destination.distance;

    L_done = 0;

    //appel PID
    _PID_ = m_distance.computePID(error);

    return _PID_;

}


//contrôle l'angle
/*
 * \fn contAngle     
 * \brief calcule la commande d'angle ?? 
 * \param ???
 * \param ??? 
 * \return _PID_ ??
 */
double Asservissement::contAngle(Position destination, Position robot)
{

    double _PID_, error;

    //m_robot.calculer_angle(&destination,robot);
    error=destination.thetha - robot.thetha;

    //appel PID
    _PID_ = m_angle.computePID(error);

    return _PID_;

}


//Procedure qui réinitialise l'angle ordonné
/*
 * \fn newOrderAngle    
 * \brief calcule la commande de distance 
 * \param ???
 * \param ??? 
 */
void Asservissement::newOrderAngle(Position destination, double theta_order)
{
    /*set parameters for new order*/
    destination.thetha = theta_order; //new order

    theta_done = 0; //=false the robot not reaches angle
}

/*
 * \fn checkEnslavementType     
 * \brief ??? 
 * \param ???
 * \param ??? 
 * \return _PID_ ??
 */

void Asservissement::checkEnslavementType(double error_L, double error_T, int *asserv_L, int *asserv_T)
{

    if (error_L >= 50)  //both asserv
    {
        (*asserv_L) = 1;

        if (abs(error_T) >= ERROR_THETA_ACCEPTED)  //if robot is far from target or if angle error is too bigger
        {
            (*asserv_T) = 1;
        }
        else
        {
            (*asserv_T) = 0;
        }
    }
    else if (error_L >= ERROR_L_ACCEPTED)    //only angle asserv and THEN distance asserv
    {
        if (abs(error_T) >= ERROR_THETA_ACCEPTED)  //if robot is far from target or if angle error is too bigger
        {
            (*asserv_T) = 1;
        }
        else
        {
            (*asserv_T) = 0;
        }

        if ((*asserv_T) == 0)  //if angle asserv is finished and distance error is too bigger
        {
            (*asserv_L) = 1;
        }
        else
        {
            (*asserv_L) = 0;
        }
    }
    else    //robot is not so far from target, so he can be considered on the target
    {
        (*asserv_T) = 0;
        (*asserv_L) = 0;
    }
}


//creer une valleur PWM pour atteindre l'angle et la distance
/*
 * \fn addPWM     
 * \brief cree une commande PWM ?? 
 * \param ???
 * \param ??? 
 * \return _PID_ ??
 */
void Asservissement::addPWM( double pwmControl_L, double pwmControl_T, int sens)
{

    double speedL, speedR;
    double balancing_Theta = 0.5;
    double percent;

    /*if (pwmControl_L != 0)
        percent = abs1(pwmControl_T) / abs1(pwmControl_L);
    else
        percent = 1;

    if (percent != 0)
        pwmControl_T *= (balancing_Theta / percent);
    */
    //speedL = pwmControl_L * sens + pwmControl_T;
    //speedR = pwmControl_L * sens - pwmControl_T;

    speedL = pwmControl_L  + pwmControl_T;
    speedR = pwmControl_L - pwmControl_T;

    double max;
    double min;

    double ratio;

    if (speedL > speedR)
    {
        max = speedL;
        min = speedR;
    }
    else
    {
        max = speedR;
        min = speedL;
    }

    if ((max > m_maxSpeed || min<-m_maxSpeed))//if an overflow occured
    {
        if (abs1(max) > abs1(min))//if max overflow more than min
        {
            ratio = (m_maxSpeed * 1.0) / abs1(max);
        }
        else  //else min overflow more than max
        {
            ratio = (m_maxSpeed * 1.0) / abs1(min);
        }

        speedL = speedL*ratio;
        speedR = speedR*ratio;
    }

    m_leftMotorSpeed = speedL;
    m_rightMotorSpeed = speedR;




}


void Asservissement::appliquerOrdre(Position destination, Position robot,int sens)
{

    m_commandeType=sens;

    switch(m_commandeType)
    {

    case 1:
        atteindreAngle(destination,robot);

        break;
    case 2:

        atteindreDistance(destination,robot);

        break;
    case 3:

        //atteindrePosition ( destination, robot); //courbe curviligne
        curve ( destination, robot);
        //atteindrePosition ( destination, robot);

        break;

    default:
        m_leftMotorSpeed=0;
        m_rightMotorSpeed=0;

        break;
    }

    if(m_stop == 1 ||  (theta_done ==1 && L_done==1 ))
    {

        m_leftMotorSpeed=0;
        m_rightMotorSpeed=0;

    }

    //envoie de la consigne vitesse au moteur
    m_moteur.fonctionnement_moteur( m_leftMotorSpeed, m_rightMotorSpeed);

}


//angle ordre à atteindre
void Asservissement::atteindreAngle(Position destination, Position robot)
{

    double pwmControl_T = 0;
    // static int delayT = 0;

    double error = destination.thetha - robot.thetha;

    error = boundError(error);

    /* if (abs1(error) > ERROR_THETA_ACCEPTED) {
         theta_done = 0;
         delayT = 0;
     } else {
         if (delayT < 10)
             delayT++;
     }

     if (delayT < 10) {
         //calculate order for angle
         pwmControl_T = contAngle(destination,robot);

     } else if (abs1(error) <= ERROR_THETA_ACCEPTED) {
         theta_done = 1; //robot reaches point
     }
      */


    pwmControl_T = contAngle(destination,robot);
    if (abs1(error) <= ERROR_THETA_ACCEPTED)
    {
        theta_done = 1;
    }
    else
    {
        theta_done = 0;
    }

    if (pwmControl_T > m_maxSpeed)
    {
        pwmControl_T = m_maxSpeed;
    }
    else if (pwmControl_T < - m_maxSpeed)
    {
        pwmControl_T = -m_maxSpeed ;
    }


    m_leftMotorSpeed =    pwmControl_T;
    m_rightMotorSpeed = - pwmControl_T;
}


void Asservissement::atteindreDistance(Position destination, Position robot)
{

    double pwmControl_L = 0;
    int forward =0;
    double error;
    double beta=0, depassement=0;


    error= robot.distance;

    if (error != 0)
    {
        beta = asin((destination.y - robot.y) / error);

        if (destination.x < robot.x)  //if target is negative
        {
            if (beta >= 0)  //target on the left (in the work plan)
            {
                beta = PI - beta;
            }
            else    //right
            {
                beta = -PI - beta;
            }
        }
    }
    else    //else target is on robot position
    {
        beta=0;
    }
    /*if (abs1(error) > ERROR_L_ACCEPTED) {
        L_done = 0;
        delayL = 0;
    } else {
        if (delayL < 10)
            delayL++;
    }

    if (delayL < 10) {
        //calculate order for angle
        pwmControl_L = contDistance(destination,robot);

    } else if (abs1(error) <= ERROR_L_ACCEPTED) {
        L_done = 1; //robot reaches point
    }*/
    beta = beta*180.0/3.14;
    depassement = beta - robot.thetha;
    depassement=boundError(depassement);
    if (abs1(depassement) <= 90)  //if target ahead robot
    {
        forward = 1; //forward
    }
    else    //else target behind robot
    {
        forward = -1; //backward
    }


    pwmControl_L = m_distance.computePID(error);


    if (pwmControl_L > m_maxSpeed)
    {
        pwmControl_L = m_maxSpeed;
    }

    Serial.print(destination.distance);
    Serial.print(" , ");
    Serial.print(error);
    Serial.print(" , ");
    Serial.print(forward);



    if(forward == 1)
    {
        m_leftMotorSpeed =  pwmControl_L;
        m_rightMotorSpeed = pwmControl_L;
    }
    else
    {
        m_leftMotorSpeed =  -  pwmControl_L;
        m_rightMotorSpeed = - pwmControl_L;

    }



}


void Asservissement::curve(Position destination, Position robot)
{

//   int asserv_L = 1;
//   int asserv_T = 1;
    double pwmControl_T = 0;
    double pwmControl_L = 0;
    int forward = 0; // 1=forward -1=backward
    double beta=0, depassement=0;

    //ERROR
    double errorL = robot.distance;
    double errorT = destination.thetha;// - robot.thetha;

    errorT = boundError(errorT);

    /*
    if (errorL != 0) {
        beta = asin((destination.y - robot.y) / errorL);

        if (destination.x < robot.x) {//if target is negative
            if (beta >= 0) {//target on the left (in the work plan)
                beta = PI - beta;
            } else {//right
                beta = -PI - beta;
            }
        }
    } else {//else target is on robot position
        beta=0;
    }

    beta = beta*180.0/3.14;


     depassement = beta - robot.thetha;
     depassement=boundError(depassement);
     //errorT=depassement;
    if (abs1(depassement) <= 90) {//if target ahead robot
            forward = -1; //forward
        } else {//else target behind robot
            forward = 1; //backward
        }

     */
    pwmControl_T = m_angle.computePID(errorT);
    pwmControl_L = m_distance.computePID(errorL);




    if (abs1(errorT) <= ERROR_THETA_ACCEPTED)
    {
        theta_done = 1;
        pwmControl_T=0;

    }
    else
    {
        theta_done = 0;
    }

    if (abs1(errorL) <= ERROR_L_ACCEPTED)
    {
        L_done = 1;
        pwmControl_L=0;
    }
    else
    {
        L_done = 0;
    }

    if (abs1(errorL) >= 40)
    {
        addPWM(pwmControl_L, pwmControl_T, forward);
    }
    else
    {
        if (pwmControl_T > m_maxSpeed)
        {
            pwmControl_T = m_maxSpeed;
        }
        else if (pwmControl_T < - m_maxSpeed)
        {
            pwmControl_T = -m_maxSpeed ;
        }

        m_leftMotorSpeed =    pwmControl_T;
        m_rightMotorSpeed = - pwmControl_T;
    }
}


//Procedure pour que le robot attient sa position en courbe curviligne
void Asservissement::atteindrePosition(Position destination, Position robot)
{

    int asserv_L = 1;
    int asserv_T = 1;
    static int delayL = 10;
    static int delayT = 10;

    double d; //distance between actual position an target
    double beta; //angle order
    double errorT; //angle error between actual orientation an target
    int forward = 1; // 1=forward -1=backward

    //PID output
    double pwmControl_L = 0;
    double pwmControl_T = 0;

    //computes distance between actual position an target
    d = robot.distance;

    //computes angle between robot position and target (in the work plan)
    if (d != 0)
    {
        beta = asin((destination.y - robot.y) / d);

        if (destination.x < robot.x)  //if target is negative
        {
            if (beta >= 0)  //target on the left (in the work plan)
            {
                beta = PI - beta;
            }
            else    //right
            {
                beta = -PI - beta;
            }
        }
    }
    else    //else target is on robot position
    {
        beta=0;
    }

    //real angle between robot and target
    errorT = beta - robot.thetha;
    //bound error between [-Pi;+Pi]
    errorT = abs1(boundError(errorT));

    if (m_directionType == 1)  //if robot is in "forward" mode (only forward)
    {
        forward = 1; //forward
    }
    else if (m_directionType == 2)    //if robot is in "freemode" (forward and backward)
    {
        if (abs1(errorT) <= PI / 2)  //if target ahead robot
        {
            forward = 1; //forward
        }
        else    //else target behind robot
        {
            forward = -1; //backward

            //if |error|>PI/2 then recalculate beta, so robot can reaches the point backward
            if (beta >= 0)  //target on the left
            {
                beta = beta - PI; //move right
            }
            else    //target on the right
            {
                beta = beta + PI; //move left
            }

            //change error between robot and target (changes if in backward mode)
            errorT = beta - robot.thetha;
            //bound error between [-Pi;+Pi]
            errorT = abs1(boundError(errorT));
        }
    }
    Serial.print(errorT);
    Serial.print("\t");
    //check if must do curvilinera enslavement or angle then distance
    checkEnslavementType(d, errorT, &asserv_L, &asserv_T);

    //if robot must move
    if (asserv_L == 1)
    {
        delayL = 0;
    }
    else
    {
        if (delayL < 10)
            delayL++;
    }

    if (delayL < 10)
    {
        pwmControl_L = contDistance(destination, robot);
    }
    else if (d <= ERROR_L_ACCEPTED)
    {
        L_done = 1; //robot reaches point
    }

    Serial.print(asserv_T);
    Serial.print("\t");
    //if robot must move angle

    if (asserv_T == 1)
    {
        if (destination.thetha != beta)
        {
            // newOrderAngle(destination,beta); //angle order = beta
            //resetPID(&(robot->angle));
        }
        delayT = 0;

    }
    else
    {
        if (delayT < 10)
            delayT++;
    }

    errorT = destination.thetha - robot.thetha;

    if (abs1(errorT) > ERROR_THETA_ACCEPTED)
    {
        theta_done = 0;
        delayT = 0;
    }
    else
    {
        if (delayT < 10)
            delayT++;
    }

    if (delayT < 10)
    {
        //calculate order for angle (must be called after control_L cause SET current value PWM orders)
        pwmControl_T = contAngle(destination,robot);
    }
    else if (errorT <= ERROR_THETA_ACCEPTED)
    {
        theta_done = 1; //robot reaches point
    }


    //Do the sum of 2 PID values, and store them into motors orders
    addPWM(pwmControl_L, pwmControl_T, forward);
}


double Asservissement::abs1(double nombre)
{
    if(nombre>0)
    {
        return nombre;
    }
    else
    {
        return -nombre;
    }
}


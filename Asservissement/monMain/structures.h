/**
 *    \file structure.cpp
 *    \brief definition des structures
 *
 *Par: Nicolas SOBCZAK
 *     Octobre 2016
*/

/* ======================================================================================================
 *      Define
 * ======================================================================================================
 */

#ifndef STRUCTURES_H
#define STRUCTURES_H


/* ======================================================================================================
 *      Structures
 * ======================================================================================================
 */
//Position
typedef struct{
	double x;              //position en x
	double y;              //position en y
        double thetha;         //angle
        double distance;
}Position;


//Tick
typedef struct{
	long int ND;            //nombre de tick droit
        long int NG;            //nombre de tick gauche

}Tick;


//Vitesse
typedef struct{
	double motorG;
        double motorD;
}Vitesse;


//PidCoeff
typedef struct
{
	double kP;		//proportionnal coefficient
	double kI;		//integral coefficient
	double kD;		//derivation coefficient

}PidCoeff;

#endif


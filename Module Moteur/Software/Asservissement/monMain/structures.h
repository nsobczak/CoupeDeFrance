/**
 *    \file structures.h
 *    \brief definition des structures
 *
 *Par: Nicolas SOBCZAK
 *     Octobre 2016
 */
//_______________________________________________________________________________________________________


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
/**
 * \struct Position
 * \brief 
 */
typedef struct
{
  double x;              //position en x
  double y;              //position en y
  double thetha;         //angle
  double distance;
}Position;


/**
 * \struct Tick
 * \brief 
 */
typedef struct{
  long int ND;            //nombre de tick droit
  long int NG;            //nombre de tick gauche

}Tick;


/**
 * \struct Vitesse
 * \brief 
 */
typedef struct{
  double motorG;
  double motorD;
}Vitesse;


/**
 * \struct PidCoeff
 * \brief 
 */
typedef struct
{
  double kP;		//proportionnal coefficient
  double kI;		//integral coefficient
  double kD;		//derivation coefficient
}PidCoeff;

#endif


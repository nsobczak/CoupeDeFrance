/**
 *    \file odometrie.cpp
 *    \brief Code odometrie du grand robot. Permet de connaitre la prosition du robot sur l'axe x,y ainsi que son angle.
 *
 *Codé pour "Spark core"  www.spark.io
 *
 *Par: Clément LETELLIER
 *     Mars 2014
 *Modification: Nicolas SOBCZAK
 *              Octobre 2016
 */
//_______________________________________________________________________________________________________



/* ======================================================================================================
 *      Include
 * ======================================================================================================
 */
#include "structures.h"
#include "odometrie.h"
#include "math.h"


/* ======================================================================================================
 *      Define
 * ======================================================================================================
 */
#define PI 3.14159


/* ======================================================================================================
 *      Class Odometrie
 * ======================================================================================================
 */
/**
 * \fn Odometrie::Odometrie()  
 * \brief constructeur
 */
Odometrie::Odometrie()
{
	m_perimetre_roue=154.0;//153.934;
	m_nombre_ticktour=5000; //2000; nombres de tick par tour
	m_distance_roue_codeuse=256;//256
	calculer_ImpulionParmm();
        calculer_ImpulsionParrad();
        CORFUGE=0;

        //initialise variable
        m_variation_angle=180;
        m_memory_variation_distance=0;
}


//_______________________________________________________________________________________________________
/**
 * \fn void Odometrie::calculer_ImpulionParmm()
 * \brief constructeur qui initialise et met des valeurs par defauts
 */
void Odometrie::calculer_ImpulionParmm()
{
	m_impulsionParmm=m_nombre_ticktour/m_perimetre_roue;
}


//_______________________________________________________________________________________________________
/**
 * \fn void Odometrie::calculer_ImpulsionParrad()
 * \brief permet de connaitre sa position en x, sa position en y et son angle
 */
void Odometrie::calculer_ImpulsionParrad()
{

       m_impulsionParrad=(m_nombre_ticktour/PI);

}


//_______________________________________________________________________________________________________
/**
 * \fn void Odometrie::calculer_variation_distance()
 * \brief 
 */
void Odometrie::calculer_variation_distance()
{
	//m_variation_distance=((double)m_DistanceRoueGauche/2.0) + ((double)m_DistanceRoueDroit/2.0);
	m_variation_distance=(m_DistanceRoueGauche/2.0) + (m_DistanceRoueDroit/2.0);
        m_delta_distance=m_variation_distance-m_memory_variation_distance;

        m_memory_variation_distance=m_variation_distance;
}


//_______________________________________________________________________________________________________
/**
 * \fn void Odometrie::calculer_variation_angle()
 * \brief 
 */
void Odometrie::calculer_variation_angle()
{
	m_variation_angle=(m_DistanceRoueGauche - m_DistanceRoueDroit);///m_distance_roue_codeuse;
        m_delta_angle=m_variation_angle - m_memory_variation_angle;
        m_variation_angle_moy=(m_variation_angle + m_memory_variation_angle)/2.0;

        m_memory_variation_angle=m_variation_angle;

        m_delta_angleRad=m_delta_angle/m_distance_roue_codeuse;
        m_variation_angle_moyRad= m_variation_angle_moy/m_distance_roue_codeuse;
}


//_______________________________________________________________________________________________________
/**
 * \fn void Odometrie::calculer_distanceParcourueRoue(Tick codeuse)
 * \brief 
 */
void Odometrie::calculer_distanceParcourueRoue(Tick codeuse)
{
	m_DistanceRoueGauche=codeuse.NG/m_impulsionParmm;
	m_DistanceRoueDroit=codeuse.ND/m_impulsionParmm;
}


//_______________________________________________________________________________________________________
/**
 * \fn void Odometrie::retournerValeur(Position *roueCodeuse, Tick codeuse)
 * \brief permet de connaitre sa position en x, sa position en y et son angle
 * \param Position *roueCodeuse
 * \param Tick codeuse
 */
void Odometrie::retournerValeur(Position *roueCodeuse, Tick codeuse)
{
        long int derive_X,derive_Y;
        double dX,dY;

        calculer_distanceParcourueRoue(codeuse);
        calculer_variation_angle();
        calculer_variation_distance();

        while (  m_variation_angle_moyRad > PI) {
             m_variation_angle_moyRad -= 2 * PI;
        }

        while (  m_variation_angle_moyRad < -PI) {
              m_variation_angle_moyRad += 2 * PI;
        }

        if (  m_delta_angle == 0) {
          K = 1;
        } else {
            K = sin(  m_delta_angleRad / 2.0) / ( m_delta_angleRad/ 2.0);
        }

        dX= K * m_delta_distance * cos(m_variation_angle_moyRad);
        dY= K * m_delta_distance * sin(m_variation_angle_moyRad);

        derive_X =  CORFUGE * m_delta_angle * dY;
        derive_Y = -CORFUGE * m_delta_angle * dX;

        //met à jour avec position actuelle réelle du robot
      	(*roueCodeuse).thetha =   m_variation_angle_moyRad;
      	(*roueCodeuse).x = (*roueCodeuse).x + dX;
      	(*roueCodeuse).y = (*roueCodeuse).y + dY;
}


//_______________________________________________________________________________________________________
/**
 * \fn void Odometrie::calculer_angle(Position *destination, Position robot)
 * \brief 
 * \param Position *destination
 * \param Position robot
 */
void Odometrie::calculer_angle(Position *destination, Position robot)
{
         if(((*destination).x != robot.x) /*&& ((*destination).y != robot.y)*/){
            (*destination).thetha =atan2(((*destination).y - robot.y),((*destination).x - robot.x));
         }

         if((*destination).x < robot.x){
            (*destination).thetha +=PI;
         }

         if(((*destination).x == robot.x) && ((*destination).y > robot.y) ){
             (*destination).thetha =PI/2;
         }

         if(((*destination).x == robot.x) && ((*destination).y < robot.y) ){
             (*destination).thetha = -PI/2;
         }
}


//_______________________________________________________________________________________________________
/**
 * \fn double Odometrie::calculer_distance(Position destination, Position robot)
 * \brief calculer la distance que le robot doit ateindre
 * \param Position destination
 * \param Position robot
 */
double Odometrie::calculer_distance(Position destination, Position robot)
{
      m_distance =sqrt(pow(destination.y - robot.y,2) + pow(destination.x - robot.x,2));
      //m_distance=(double)m_variation_distance;
      return m_distance;
}


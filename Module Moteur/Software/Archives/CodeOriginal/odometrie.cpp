/*
 *Code odometrie du grand robot.
 *Premet de connaitre la prosition du robot sur l'axe x,y ainsi que son angle
 *
 *Codé pour "Spark core"  www.spark.io
 *
 *Par: Clément LETELLIER
 *     Mars 2014
*/


#include "odometrie.h"
#include "application.h"
#include "math.h"
#define PI 3.14159

Odometrie::Odometrie(){

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

void Odometrie::calculer_ImpulionParmm(){

	m_impulsionParmm=m_nombre_ticktour/m_perimetre_roue;
	
}

void Odometrie::calculer_ImpulsionParrad(){
  
       m_impulsionParrad=(m_nombre_ticktour/PI);
  
}

void Odometrie::calculer_variation_distance(){

	//m_variation_distance=((double)m_DistanceRoueGauche/2.0) + ((double)m_DistanceRoueDroit/2.0);
	m_variation_distance=(m_DistanceRoueGauche/2.0) + (m_DistanceRoueDroit/2.0);
        m_delta_distance=m_variation_distance-m_memory_variation_distance;

        m_memory_variation_distance=m_variation_distance;
}

void Odometrie::calculer_variation_angle(){

        
	m_variation_angle=(m_DistanceRoueGauche - m_DistanceRoueDroit);///m_distance_roue_codeuse;
        m_delta_angle=m_variation_angle - m_memory_variation_angle;
        m_variation_angle_moy=(m_variation_angle + m_memory_variation_angle)/2.0;
        
        m_memory_variation_angle=m_variation_angle;
        
        m_delta_angleRad=m_delta_angle/m_distance_roue_codeuse;
        m_variation_angle_moyRad= m_variation_angle_moy/m_distance_roue_codeuse;
        
        

}


void Odometrie::calculer_distanceParcourueRoue(Tick codeuse){

	m_DistanceRoueGauche=codeuse.NG/m_impulsionParmm;
	m_DistanceRoueDroit=codeuse.ND/m_impulsionParmm;

}

//permet de connaitre sa position en x, sa position en y et son angle
void Odometrie::retournerValeur(Position *roueCodeuse,Tick codeuse){
         
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
         
	(*roueCodeuse).thetha =   m_variation_angle_moyRad;
	(*roueCodeuse).x = (*roueCodeuse).x + dX;
	(*roueCodeuse).y = (*roueCodeuse).y + dY;
        
        
         
        
       
}


//calculer l'angle que le robot doit atteindre
void Odometrie::calculer_angle(Position *destination, Position robot){
  
          
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
 
//calculer la distance que le robot doit ateindre       
double Odometrie::calculer_distance(Position destination, Position robot){
   
        m_distance =sqrt(pow(destination.y - robot.y,2) + pow(destination.x - robot.x,2)); 
        //m_distance=(double)m_variation_distance;
          return m_distance;
      
}
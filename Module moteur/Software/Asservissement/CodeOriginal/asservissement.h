/*
 *Class asservissement du grand robot
 *
 *Codé pour "Spark core"  www.spark.io
 *
 *Par: Clément LETELLIER 
 *     Mars 2014
*/

#ifndef DEF_ASSERVISSEMENT
#define DEF_ASSERVISSEMENT

#include "structures.h"
#include "odometrie.h"
#include "moteur.h"
#include "PID.h"
#include "spark_wiring.h"
#include "application.h"

#define ERROR_L_ACCEPTED 10.0 //if robot reaches this value or minus, orders are satisfied (in mm) 
#define ERROR_THETA_ACCEPTED 1.0 //if robot reaches this value or minus, orders are satisfied (to reach angle)

class Asservissement{

	public:
        
        Asservissement(double periode,Moteur grobot);
        
        double boundError(double e);
        double contDistance(Position destination, Position robot);
        double contAngle(Position destination, Position robot);
        void newOrderAngle(Position destination, double theta_order);
        
        void checkEnslavementType(double error_L, double error_T, int *asserv_L, int *asserv_T);
        void addPWM( double pwmControl_L, double pwmControl_T, int sens);
        
        void appliquerOrdre(Position destination, Position robot,int sens);
        void atteindreAngle(Position destination, Position robot);
        void atteindreDistance(Position destination, Position robot);
        void curve(Position destination, Position robot);
        void atteindrePosition(Position destination, Position robot);
        double abs1(double nombre);
        
	private:
        
        PID m_distance;
        PID m_angle;
        Odometrie m_robot;
        Moteur m_moteur;
        
        
        int m_maxSpeed;
        int m_minSpeed;
	double m_consigne;
        double m_periode;
        
        int m_commandeType;   //1 consigne en angle _ 2 consigne en position _ 3 cconsigne en Curve
        int m_directionType;  //1 FORWARD _ 2 FREEMODE
        
        double m_leftMotorSpeed;
        double m_rightMotorSpeed;
        
        int m_stop; //0=false _ non arrêter /-/ 1=true _ arrêter//
        
        int L_done; //if te robot reaches distance order (0=false/1=true)
        int theta_done; //if te robot reaches angle order (0=false/1=true)
};

#endif
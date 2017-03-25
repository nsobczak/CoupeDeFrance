#ifndef STRUCTURES_H
#define STRUCTURES_H


typedef struct{
	double x;              //position en x
	double y;              //position en y
        double thetha;         //angle
        double distance;
}Position;

typedef struct{
	long int ND;            //nombre de tick droit
        long int NG;            //nombre de tick gauche
        
}Tick;

typedef struct{
	double motorG;
        double motorD;
}Vitesse;

typedef struct
{
	double kP;		//proportionnal coefficient
	double kI;		//integral coefficient
	double kD;		//derivation coefficient

} PidCoeff;

#endif


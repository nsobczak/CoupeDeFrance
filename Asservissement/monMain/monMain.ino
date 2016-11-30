/**
 *    \file monMain.ino
 *    \brief appel des fonctions
 *
 *Par: Nicolas SOBCZAK
 *     Octobre 2016
 */


/* ======================================================================================================
 *      Include
 * ======================================================================================================
 */
#include "Arduino.h"
#include "odometrie.h"
#include "PID.h"
#include "moteur.h"
#include "math.h"
#include "asservissement.h"
#include "structures.h"          // pour les types


/* ======================================================================================================
 *      Define
 * ======================================================================================================
 */
#define TEMPS 115
// les 4 ci-dessous sont à redéfinir dans erreur.cpp
#define encoder0PinA_L 21
#define encoder0PinB_L 22
#define encoder0PinA_R 30
#define encoder0PinB_R 31


/* ======================================================================================================
 *      Function prototypes
 * ======================================================================================================
 */
//Interruption roue encoder
void doEncoderA_L();
void doEncoderA_R();
void doEncoderB_L();
void doEncoderB_R();
//Distance angle
void errorLT();


/* ======================================================================================================
 *      Notes
 * ======================================================================================================

// choix équipe (couleur)
endodeur = roues codeuses pour connaitre la position de la roue (pour asservissement)


*/



/* ======================================================================================================
 *      Initialisation variables globales
 * ======================================================================================================
 */

//_______________________________________________________________________________________________________
volatile long int encoder0Pos_L = 0;
volatile long int encoder0Pos_R = 0;

//_______________________________________________________________________________________________________
Position destination;   // variable pour choisir le point d'arrivée
Odometrie roue;         // class Odometrie utilise pour recuperer les donnees de position actuelle
Position roueCodeuse;   // class position regroupe les donnees de position variable pour regrouper les donnees de position des roues codeuses
Position memory_destination;

//_______________________________________________________________________________________________________
Position memory_position;
int targetAngle;
Moteur crysteo;
Asservissement robot(TEMPS, crysteo);
Tick codeuse;
int Doangle;

//_______________________________________________________________________________________________________
int forward; // 1=forward -1=backward
double beta, depassement;
double TargetAngle;
int DoAngle;



//_______________________________________________________________________________________________________
//_______________________________________________________________________________________________________
void setup()
{
    Serial.begin(250000);
    crysteo.initPWM();
    //interruption pour encodeurs
    attachInterrupt(encoder0PinA_L, doEncoderA_L, CHANGE);
    attachInterrupt(encoder0PinB_L, doEncoderB_L, CHANGE);

    attachInterrupt(encoder0PinA_R, doEncoderA_R, CHANGE);
    attachInterrupt(encoder0PinB_R, doEncoderB_R, CHANGE);

}


void loop()
{   

     
    square(destination, roueCodeuse, robot, roue, codeuse);
}


/* ======================================================================================================
 *      Fonctions
 * ======================================================================================================
 */
 void square(Position destination, Position roueCodeuse, Asservissement robot, Odometrie roue, Tick codeuse)
{
    destination.x=0;
    destination.y=0;
    destination.thetha=0.0;                              //"destination.distance" utile car utile dans odométrie (pour les calculs)
    roueCodeuse.x=0;
    roueCodeuse.y=0;
    roueCodeuse.thetha=0;
    robot.appliquerOrdre(destination, roueCodeuse, 3);   //attention pas tout pigé
    roue.retournerValeur(&roueCodeuse, codeuse);         //attention pas tout pigé non plus
    errorLT();


    destination.x = 0;
    destination.y = 500;
    destination.thetha = 90;
    robot.appliquerOrdre(destination,roueCodeuse,3);
    roue.retournerValeur(&roueCodeuse,codeuse);
    errorLT();

    destination.x = 500;
    destination.y = 500;
    destination.thetha = 90;
    robot.appliquerOrdre(destination,roueCodeuse,3);
    roue.retournerValeur(&roueCodeuse,codeuse);
    errorLT();

    destination.x = 500;
    destination.y = 0;
    destination.thetha = 90;
    robot.appliquerOrdre(destination,roueCodeuse,3);
    roue.retournerValeur(&roueCodeuse,codeuse);
    errorLT();

    destination.x = 0;
    destination.y = 0;
    destination.thetha = 0;
    robot.appliquerOrdre(destination,roueCodeuse,3);
    roue.retournerValeur(&roueCodeuse,codeuse);
    errorLT();

}


//_______________________________________________________________________________________________________
/**
 * \fn void errorLT() - fonction qui calcule l'erreur de position
 * \param nothing
 */
void errorLT() {

	roueCodeuse.thetha = (roueCodeuse.thetha * 180) / 3.14;

	roueCodeuse.distance = roue.calculer_distance(destination, roueCodeuse);
	if (roueCodeuse.distance != 0) {
		beta = asin((destination.y - roueCodeuse.y) / roueCodeuse.distance);

		if (destination.x < roueCodeuse.x) {//if target is negative
			if (beta >= 0) {//target on the left (in the work plan)
				beta = PI - beta;
			}
			else {//right
				beta = -PI - beta;
			}
		}
	}
	else {//else target is on robot position
		beta = 0;
	}
	beta = beta*180.0 / 3.14;
	depassement = beta - roueCodeuse.thetha;
	depassement = robot.boundError(depassement);
	//errorT=depassement;
	if (robot.abs1(depassement) <= 90) {//if target ahead robot
		forward = -1; //forward
	}
	else {//else target behind robot
		forward = 1; //backward
		depassement = beta - 180 - roueCodeuse.thetha;
		depassement = robot.boundError(depassement);
		beta = beta + 180;
		beta = robot.boundError(beta);
	}

	if (DoAngle == 0) {
		destination.thetha = depassement;
		memory_destination.thetha = beta;
	}
	else {
		destination.thetha = TargetAngle - roueCodeuse.thetha;
	}
	roueCodeuse.distance = roueCodeuse.distance*forward;
}


//_______________________________________________________________________________________________________
/**
 * \fn void doEncoderA_L() - fonction qui relève le nombre de ticks
 * \param nothing
 */
void doEncoderA_L() {

	// look for a low-to-high on channel A
	if (digitalRead(encoder0PinA_L) == HIGH) {
		// check channel B to see which way encoder is turning
		if (digitalRead(encoder0PinB_L) == LOW) {
			encoder0Pos_L = encoder0Pos_L + 1;         // CW
		}
		else {
			encoder0Pos_L = encoder0Pos_L - 1;         // CCW
		}
	}
	else   // must be a high-to-low edge on channel A
	{
		// check channel B to see which way encoder is turning
		if (digitalRead(encoder0PinB_L) == HIGH) {
			encoder0Pos_L = encoder0Pos_L + 1;          // CW
		}
		else {
			encoder0Pos_L = encoder0Pos_L - 1;          // CCW
		}
	}

	// use for debugging - remember to comment out
}


/**
 * \fn void doEncoderB_L() - fonction qui relève le nombre de ticks
 * \param nothing
 */
void doEncoderB_L() {

	// look for a low-to-high on channel B
	if (digitalRead(encoder0PinB_L) == HIGH) {
		// check channel A to see which way encoder is turning
		if (digitalRead(encoder0PinA_L) == HIGH) {
			encoder0Pos_L = encoder0Pos_L + 1;         // CW
		}
		else {
			encoder0Pos_L = encoder0Pos_L - 1;         // CCW
		}
	}
	// Look for a high-to-low on channel B
	else {
		// check channel B to see which way encoder is turning
		if (digitalRead(encoder0PinA_L) == LOW) {
			encoder0Pos_L = encoder0Pos_L + 1;          // CW
		}
		else {
			encoder0Pos_L = encoder0Pos_L - 1;          // CCW
		}
	}
}


/**
 * \fn void doEncoderA_R() - fonction qui relève le nombre de ticks
 * \param nothing
 */
void doEncoderA_R() {

	// look for a low-to-high on channel A
	if (digitalRead(encoder0PinA_R) == HIGH) {
		// check channel B to see which way encoder is turning
		if (digitalRead(encoder0PinB_R) == LOW) {
			encoder0Pos_R = encoder0Pos_R + 1;         // CW
		}
		else {
			encoder0Pos_R = encoder0Pos_R - 1;         // CCW
		}
	}
	else   // must be a high-to-low edge on channel A
	{
		// check channel B to see which way encoder is turning
		if (digitalRead(encoder0PinB_R) == HIGH) {
			encoder0Pos_R = encoder0Pos_R + 1;          // CW
		}
		else {
			encoder0Pos_R = encoder0Pos_R - 1;          // CCW
		}
	}

	// use for debugging - remember to comment out
}


/**
 * \fn void doEncoderB_R() - fonction qui relève le nombre de ticks
 * \param nothing
 */
void doEncoderB_R() {

	// look for a low-to-high on channel B
	if (digitalRead(encoder0PinB_R) == HIGH) {
		// check channel A to see which way encoder is turning
		if (digitalRead(encoder0PinA_R) == HIGH) {
			encoder0Pos_R = encoder0Pos_R + 1;         // CW
		}
		else {
			encoder0Pos_R = encoder0Pos_R - 1;         // CCW
		}
	}
	// Look for a high-to-low on channel B
	else {
		// check channel B to see which way encoder is turning
		if (digitalRead(encoder0PinA_R) == LOW) {
			encoder0Pos_R = encoder0Pos_R + 1;          // CW
		}
		else {
			encoder0Pos_R = encoder0Pos_R - 1;          // CCW
		}
	}
}


//_______________________________________________________________________________________________________
//interruption pour encodeurs
void interruptionEncodeurs(){
    attachInterrupt(encoder0PinA_L, doEncoderA_L, CHANGE);
    attachInterrupt(encoder0PinB_L, doEncoderB_L, CHANGE);

    attachInterrupt(encoder0PinA_R, doEncoderA_R, CHANGE);
    attachInterrupt(encoder0PinB_R, doEncoderB_R, CHANGE);
}





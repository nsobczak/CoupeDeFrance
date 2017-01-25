/**
 *    \file monMain.ino
 *    \brief appel des fonctions
 *
 *    \author Nicolas SOBCZAK
 *    \date Octobre 2016  
 */
//_______________________________________________________________________________________________________
//_______________________________________________________________________________________________________
/* ======================================================================================================
 *      Include
 * ======================================================================================================
 */
#include "asservissement.h"
#include "structures.h"          // pour les types

#include "odometrie.h"
#include "PID.h"
#include "moteur.h"
#include "math.h"


/* ======================================================================================================
 *      Define
 * ======================================================================================================
 */
#define TEMPS 115
// les 4 ci-dessous sont à redéfinir dans erreur.cpp
#define encoder0PinA_L 13   //encodeur gauche A 
#define encoder0PinB_L 12   //encodeur gauche B 
#define encoder0PinA_R 11   //encodeur droit A 
#define encoder0PinB_R 10   //encodeur droit B 


/* ======================================================================================================
 *      Notes
 * ======================================================================================================

// choix équipe (couleur)
endodeur = roues codeuses pour connaitre la position de la roue (pour asservissement)

// Pour débuguer on a besoin de:
coefficients du PID


*/


//_______________________________________________________________________________________________________
/* ======================================================================================================
 *      Initialisation variables globales
 * ======================================================================================================
 */
volatile long int encoder0Pos_L = 0;
volatile long int encoder0Pos_R = 0;

Position destination;   // variable pour choisir le point d'arrivée
Odometrie roue;         // class Odometrie utilise pour recuperer les donnees de position actuelle
Position roueCodeuse;   // class position regroupe les donnees de position variable pour regrouper les donnees de position des roues codeuses
Position memory_destination;

Position memory_position;
int targetAngle;
Moteur crysteo;
Asservissement robot(TEMPS, crysteo);
Tick codeuse;
int Doangle;

int forward; // 1=forward -1=backward
double beta, depassement;
double TargetAngle;
int DoAngle;

int value,test;
int number[7];
long int val;
int etape;
unsigned long timer;

//Détection adversaire
int StopIR;
// //Donné objet capter par la raspberry
// uint32_t dataRaspberry;
// int detecteObjet_Raspberry;
// double distance_Raspberry;
// double Angle_Raspberry;
//Variable pour la corde
boolean startRound;

String robotCouleur;

//_______________________________________________________________________________________________________
//_______________________________________________________________________________________________________
/* ======================================================================================================
 *      Fonctions arduino
 * ======================================================================================================
 */
/**
 * \fn void setup()
 * \brief fonction setup d'arduino
 */
void setup()
{
    Serial.begin(9600);
    
    etape=0; 

    codeuse.ND=0;
    codeuse.NG=0;
    
    forward=0;
    depassement=0;
    beta=0;
    DoAngle=0;
    TargetAngle=0;
    StopIR=0;
    startRound = true;
    
    crysteo.initPWM();

  	pinMode(encoder0PinA_L, INPUT);
  	pinMode(encoder0PinB_L, INPUT);
  
  	pinMode(encoder0PinA_R, INPUT);
  	pinMode(encoder0PinB_R, INPUT);

    //interruption pour encodeurs
    attachInterrupt(encoder0PinA_L, doEncoderA_L, CHANGE);
    attachInterrupt(encoder0PinB_L, doEncoderB_L, CHANGE);

    attachInterrupt(encoder0PinA_R, doEncoderA_R, CHANGE);
    attachInterrupt(encoder0PinB_R, doEncoderB_R, CHANGE);
    
    roueCodeuse.x=0;                                      //position actuelle
    roueCodeuse.y=0;                                      // Itinialisation de la roue codeuse (a verifier par test)
    roueCodeuse.thetha=0;                                 //
    // rajouter codeuse ND NG  à initialiser à 0 

	  destination.x=0;     //consigne de positions et d'angle (ordre)
    destination.y=0;
    destination.thetha=0.0;
    destination.distance=0;

    memory_destination.x = 0.0;
    memory_destination.y = 0.0;
    memory_destination.thetha = 0.0;
    memory_destination.distance = 0.0;

    timer=millis();
}


/**
 * \fn void loop()
 * \brief fonction loop d'arduino
 */
 void loop()
{
    Serial.println("Entering_loop...");
    
    codeuse.NG = encoder0Pos_L;
    codeuse.ND = encoder0Pos_R;
    
    //receiveIRSONAR();
    
    
    // delay(1000);
    //interrupts();   // enable interrupts
    //    do{
    //    }while(millis()-timer < 10);
    
    if(startRound == false){
        while(startRound != true)
        {
            crysteo.fonctionnement_moteur(0,0);
            //CORDE();
            //Serial.print("Start :\t");
            //Serial.println(startRound);
        }
        /*
        display.clearDisplay();   // clears the screen and buffer
        display.setTextSize(1);    //taille de la police
        display.setTextColor(WHITE); //couleur du texte blanc sur noir
        display.setCursor(0, 0);            //Position du texte [colonne=128| ligne=32]
        display.print("START");
        display.display();
        */
        //TimerRound=millis();
        delay(1000);
        //permet d'afficher
        Serial.print("Start :\t");
        Serial.println(startRound);
    }
    
    //noInterrupts(); // disable interrupts
    //Serial.print("StopIR : \t");
    //Serial.println(StopIR);
    // if(millis()-TimerRound <=90000){ //timer coupe
    Serial.println("coucou_1");
    if(StopIR==0){
        Serial.println("coucou_2");
        Serial.print("millis()-timer : ");
        Serial.println(millis()-timer);
        if(millis()-timer >= 100){
            Serial.println("coucou_3");
            //LED_SetRGBColor(RGB_COLOR_RED);
            //LED_On(LED_RGB);
            //Serial.print(millis());
            //Serial.print(" \t ");
            //Serial.print (destination.thetha);
            //Serial.print(" \t ");
            //Serial.println (roueCodeuse.thetha);
            //Serial.println (200 - roueCodeuse.distance);
            //Serial.println (roue.calculer_distance(destination, roueCodeuse));
            //destination.distance=roue.calculer_distance(destination, roueCodeuse);
            //robot.appliquerOrdre(destination,roueCodeuse,1); //Angle
            //robot.appliquerOrdre(destination,roueCodeuse,2);  //distance
                robot.appliquerOrdre(destination,roueCodeuse,3);  //curve
    	    //This will run in a loop
                timer=millis();
        }
        else
        {
            Serial.println("coucou_4");
            //LED_SetRGBColor(RGB_COLOR_BLUE);
            //LED_On(LED_RGB);
        
            codeuse.ND=encoder0Pos_R;
            codeuse.NG=encoder0Pos_L;
               /* Serial.print (encoder0Pos_L);
                Serial.print ("\t");
                Serial.println (encoder0Pos_R);
               */
            roue.retournerValeur(&roueCodeuse,codeuse);
           
            errorLT();
            /*
            if(robotCouleur=="VERT"){
                //printOLED(roueCodeuse,"VERT");
            }
            else{
                //printOLED(roueCodeuse,"JAUNE");
            }
            */
            
            int X =roueCodeuse.x;
            int Y = roueCodeuse.y;
            int ANGLE = roueCodeuse.thetha;
            //Serial.println("ANGLE :");
            //Serial.println(ANGLE);
            
            int DX = destination.x;
            int DY = destination.y;
            int DA = memory_destination.thetha;
             
             
           //if( (millis()-timer2) >= 5000){
            //if( roueCodeuse.distance <= 60){
            if(((X <= DX + 50) && (X >= DX - 50)) || (memory_destination.x == destination.x) ){
                Serial.println("coucou_5_if_DX");
                if(((Y <= DY + 50) && (Y >= DY - 50)) || (memory_destination.y == destination.y) ){
                    Serial.println("coucou_6_if_DY");
                    Serial.println("ANGLE :");
                    Serial.println(ANGLE);
                    Serial.println("DA :");
                    Serial.println(DA);
                    Serial.println("coucou_6_if_DY");
                    if((ANGLE <= DA + 10) && (ANGLE >= DA - 10)){
                            Serial.println("coucou_7_if_DA");
                            //LED_SetRGBColor(RGB_COLOR_GREEN);
                            //LED_On(LED_RGB);
                            //keepPosition(destination, roueCodeuse, robot, roue, codeuse);
                            Serial.println("___entering square___");
                            square(destination, roueCodeuse, robot, roue, codeuse);
                            Serial.println("exiting square___");
                            //exemple1();
                            //deplacementColor();
                            delay(100);
                            //timer2=millis();
                    }
                }
            }
            
              
            //Serial.print(" \t THETHA : \t");
            //Serial.println (roueCodeuse.thetha);

            //Serial.print(" \t X : \t");
            //Serial.println (roueCodeuse.x);

            //Serial.print(" \t Y : \t");
            //Serial.println (roueCodeuse.y);
            //crysteo.fonctionnement_moteur( 0, 0);
            }
    }
    else{
          Serial.println("coucou_8");
          crysteo.fonctionnement_moteur(0,0);  
     }
/*  }
  else{
          crysteo.fonctionnement_moteur(0,0);  
     }*/
  
     /*   if(test==1){
            val= (number[0] <<24) |(number[1] << 16)|(number[2] << 8) | number[3];
            val=(-(val & 4294967296))+(val & 4294967295);
            test=0;
        }
    delay(100);
    Serial.print("la valeur est de :\t");
    Serial.println(val);
    
    Serial.print("la number[0] est de :\t");
    Serial.println(number[0],BIN);
    
    Serial.print("la number[1] est de :\t");
    Serial.println(number[1],BIN);
    Serial.print("la number[2] est de :\t");
    Serial.println(number[2],BIN);
    Serial.print("la number[3] est de :\t");
    Serial.println(number[3],BIN);
   */
   /*  if(!songDone) { // Start song
      digitalWrite(D7,HIGH); // Light the onboard Blue LED while the song plays
      songDone = true;
      begin_rtttl(song);
    }
    if(!next_rtttl()) { // Play next note
      digitalWrite(D7,LOW); // Turn off the onboard Blue LED.
      songDone = false;
      remoteTriggered = false;
      if(DEBUG) Serial1.println("Done!");
      delay(2000);
    }
    */  
    
    Serial.println("...Exiting_loop");
}



/* ======================================================================================================
 *      Fonctions
 * ======================================================================================================
 */
 /**
 * \fn  void keepPosition(Position destination, Position roueCodeuse, Asservissement robot, Odometrie roue, Tick codeuse)
 * \brief fonction qui fait attendre le robotà la même position
 */
 void keepPosition(Position destination, Position roueCodeuse, Asservissement robot, Odometrie roue, Tick codeuse)
{
    destination.x=0;
    destination.y=0;
    destination.thetha=0.0;                               //"destination.distance" utile car utile dans odométrie (pour les calculs)
    robot.appliquerOrdre(destination, roueCodeuse, 3);    // fonctionnement moteur
    roue.retournerValeur(&roueCodeuse, codeuse);          // met à jour roue codeuse
    errorLT();  
}

 
 /**
 * \fn  void square(Position destination, Position roueCodeuse, Asservissement robot, Odometrie roue, Tick codeuse)
 * \brief fonction qui fait faire un carré au robot
 */
 void square(Position destination, Position roueCodeuse, Asservissement robot, Odometrie roue, Tick codeuse)
{
    
    memory_destination.x=destination.x;
    memory_destination.y=destination.y;
    //memory_destination.thetha=destination.thetha;
    
    switch (etape){
        
    case 0 : 
            Serial.println("switch case 0");
            destination.x=0;
            destination.y=0;
            DoAngle=0;
            TargetAngle=0;
            etape++;
    break;
    case 1 : 
            Serial.println("switch case 1");
            destination.x=300;
            destination.y=0;
            TargetAngle=0;
            DoAngle=0;
            etape++;
    break;
    case 2 : 
            Serial.println("switch case 2");
            destination.x=300;
            destination.y=0;
            TargetAngle+=90;
            DoAngle=1;
            etape++;
    break;
    case 3 : 
            Serial.println("switch case 3");
            destination.x=300;
            destination.y=300;
            //destination.thetha=90;
            DoAngle=0;
            etape++;
    break;
    case 4 : 
            Serial.println("switch case 4");
            destination.x=300;
            destination.y=300;
            TargetAngle+=90;
            DoAngle=1;
            etape++;
    break;
    case 5 : 
            Serial.println("switch case 5");
            destination.x=0;
            destination.y=300;
            //destination.thetha=180;
            DoAngle=0;
            etape++;
    break;
    case 6 : 
            Serial.println("switch case 6");
            destination.x=0;
            destination.y=300;
            TargetAngle=-90;
            DoAngle=1;
            etape++;
   break;
   case 7: 
            Serial.println("switch case 7");
            destination.x=0;
            destination.y=0;
            //destination.thetha=90;
            DoAngle=0;
            etape++;
    break;
    case 8 : 
            Serial.println("switch case 8");
            destination.x=0;
            destination.y=0;
            TargetAngle=0;
            DoAngle=1;
            etape=0;
    break;
    }
     memory_destination.thetha=TargetAngle;       
}

/*
 void square(Position destination, Position roueCodeuse, Asservissement robot, Odometrie roue, Tick codeuse)// faire machine d'état
{
    destination.x=0;
    destination.y=0;
    destination.thetha=0.0;                               //"destination.distance" utile car utile dans odométrie (pour les calculs)
    roueCodeuse.x=0;                                      //
    roueCodeuse.y=0;                                      // Itinialisation de la roue codeuse (a verifier par test)
    roueCodeuse.thetha=0;                                 // 
    robot.appliquerOrdre(destination, roueCodeuse, 3);    // attention pas tout pigé
    roue.retournerValeur(&roueCodeuse, codeuse);          // met à jour roue codeuse
    errorLT();                                            // met à jour roue codeuse


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
*/


//_______________________________________________________________________________________________________
/**
 * \fn void errorLT() 
 * \brief fonction qui calcule l'erreur de position et met à jour roue codeuse
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
 * \fn void doEncoderA_L()
 * \brief fonction qui relève le nombre de ticks
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
 * \fn void doEncoderB_L()  
 * \brief fonction qui relève le nombre de ticks
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
 * \fn void doEncoderA_R()  
 * \brief fonction qui relève le nombre de ticks
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
 * \fn void doEncoderB_R() 
 * \brief fonction qui relève le nombre de ticks
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
/**
 * \fn void doEncoderB_R() 
 * \brief fonction interruption pour encodeurs
 */
void interruptionEncodeurs(){
    attachInterrupt(encoder0PinA_L, doEncoderA_L, CHANGE);
    attachInterrupt(encoder0PinB_L, doEncoderB_L, CHANGE);

    attachInterrupt(encoder0PinA_R, doEncoderA_R, CHANGE);
    attachInterrupt(encoder0PinB_R, doEncoderB_R, CHANGE);
}


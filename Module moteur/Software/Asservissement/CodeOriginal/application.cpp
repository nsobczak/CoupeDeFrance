/**
 ******************************************************************************
 * @file    application.cpp
 * @authors  Satish Nair, Zachary Crockett and Mohit Bhoite
 * @version V1.0.0
 * @date    05-November-2013
 * @brief   Tinker application
 ******************************************************************************
  Copyright (c) 2013 Spark Labs, Inc.  All rights reserved.

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this program; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/  
#include "application.h"
#include "spark_utilities.h"
#include "structures.h"
#include "odometrie.h"
#include "PID.h"
#include "moteur.h"
#include "math.h"
#include "asservissement.h"
//#include "Wire.h"

#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"


#define PI 3.14159
#define OLED_RESET D2
Adafruit_SSD1306 display(OLED_RESET);

/* Function prototypes -------------------------------------------------------*/
int tinkerDigitalRead(String pin);
int tinkerDigitalWrite(String command);
int tinkerAnalogRead(String pin);
int tinkerAnalogWrite(String command);
//Interruption roue encoder
void doEncoderA_L();
void doEncoderA_R();
void doEncoderB_L();
void doEncoderB_R();
//Affichage écran OLED
void printOLED(Position alpha,char couleur[]);
//PLay Song
/*void begin_rtttl(char *p);
bool next_rtttl();
void tone(int pin, int16_t note, int16_t duration);
*/
//UART
void recevoir_tick();
//Distance angle
void errorLT();
void ComputerVision();
//Déplacement 
void scare();
void exemple1();
void homologationJ();
void homologationV();
void exemple3();
void strategyOneJaune();
void strategyOneVert();
void deplacementColor();
//communication actionnneur I2C
#define _CLAP_      0x20
#define _POPCORN_   0x21
#define _IRSONAR_   0x23
void clap(char action[]);
void sendIRSONAR(char action[]);
void receiveIRSONAR();
void RASPBERRY_IR(); //communication avec la raspberry en passant par un arduino en I2C Raspberry => //UART// => Arduino => //I2C// => Spark Core
//Action Bouton
void CORDE(); // Démarrage du robot pour le match    

#define encoder0PinA_L A0 //D0
#define encoder0PinB_L A1 //D1

#define encoder0PinA_R D3 //D3
#define encoder0PinB_R D4 //D4

//corde
#define cordeDemarrage A3
//couleur 
#define cVert D6
#define cJaune D5
//Play Song
/*
#define DEBUG false
#define OCTAVE_OFFSET 0
*/
//int16_t tonePin = D6;
//bool remoteTriggered = false; // global state variable for remoteTrigger() function

// Notes defined in microseconds (Period/2) 
// from note C to B, Octaves 3 through 7
/*int notes[] = 
{0,
3817,3597,3401,3205,3030,2857,2703,2551,2404,2273,2146,2024,
1908,1805,1701,1608,1515,1433,1351,1276,1205,1136,1073,1012,
956,903,852,804,759,716,676,638,602,568,536,506,
478,451,426,402,379,358,338,319,301,284,268,253,
239,226,213,201,190,179,169,159,151,142,134,127};

//char *song = (char *)"The Simpsons:d=4,o=5,b=168:c.6,e6,f#6,8a6,g.6,e6,c6,8a,8f#,8f#,8f#,2g,8p,8p,8f#,8f#,8f#,8g,a#.,8c6,8c6,8c6,c6";
//char *song = (char *)"Indiana:d=4,o=5,b=250:e,8p,8f,8g,8p,1c6,8p.,d,8p,8e,1f,p.,g,8p,8a,8b,8p,1f6,p,a,8p,8b,2c6,2d6,2e6,e,8p,8f,8g,8p,1c6,p,d6,8p,8e6,1f.6,g,8p,8g,e.6,8p,d6,8p,8g,e.6,8p,d6,8p,8g,f.6,8p,e6,8p,8d6,2c6";
//char *song = (char *)"TakeOnMe:d=4,o=4,b=160:8f#5,8f#5,8f#5,8d5,8p,8b,8p,8e5,8p,8e5,8p,8e5,8g#5,8g#5,8a5,8b5,8a5,8a5,8a5,8e5,8p,8d5,8p,8f#5,8p,8f#5,8p,8f#5,8e5,8e5,8f#5,8e5,8f#5,8f#5,8f#5,8d5,8p,8b,8p,8e5,8p,8e5,8p,8e5,8g#5,8g#5,8a5,8b5,8a5,8a5,8a5,8e5,8p,8d5,8p,8f#5,8p,8f#5,8p,8f#5,8e5,8e5";
//char *song = (char *)"Entertainer:d=4,o=5,b=140:8d,8d#,8e,c6,8e,c6,8e,2c.6,8c6,8d6,8d#6,8e6,8c6,8d6,e6,8b,d6,2c6,p,8d,8d#,8e,c6,8e,c6,8e,2c.6,8p,8a,8g,8f#,8a,8c6,e6,8d6,8c6,8a,2d6";
//char *song = (char *)"Muppets:d=4,o=5,b=250:c6,c6,a,b,8a,b,g,p,c6,c6,a,8b,8a,8p,g.,p,e,e,g,f,8e,f,8c6,8c,8d,e,8e,8e,8p,8e,g,2p,c6,c6,a,b,8a,b,g,p,c6,c6,a,8b,a,g.,p,e,e,g,f,8e,f,8c6,8c,8d,e,8e,d,8d,c";
//char *song = (char *)"Xfiles:d=4,o=5,b=125:e,b,a,b,d6,2b.,1p,e,b,a,b,e6,2b.,1p,g6,f#6,e6,d6,e6,2b.,1p,g6,f#6,e6,d6,f#6,2b.,1p,e,b,a,b,d6,2b.,1p,e,b,a,b,e6,2b.,1p,e6,2b.";
//char *song = (char *)"Looney:d=4,o=5,b=140:32p,c6,8f6,8e6,8d6,8c6,a.,8c6,8f6,8e6,8d6,8d#6,e.6,8e6,8e6,8c6,8d6,8c6,8e6,8c6,8d6,8a,8c6,8g,8a#,8a,8f";
//char *song = (char *)"20thCenFox:d=16,o=5,b=140:b,8p,b,b,2b,p,c6,32p,b,32p,c6,32p,b,32p,c6,32p,b,8p,b,b,b,32p,b,32p,b,32p,b,32p,b,32p,b,32p,b,32p,g#,32p,a,32p,b,8p,b,b,2b,4p,8e,8g#,8b,1c#6,8f#,8a,8c#6,1e6,8a,8c#6,8e6,1e6,8b,8g#,8a,2b";
//char *song = (char *)"Bond:d=4,o=5,b=80:32p,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d#6,16d#6,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d6,16c#6,16c#7,c.7,16g#6,16f#6,g#.6";
//char *song = (char *)"MASH:d=8,o=5,b=140:4a,4g,f#,g,p,f#,p,g,p,f#,p,2e.,p,f#,e,4f#,e,f#,p,e,p,4d.,p,f#,4e,d,e,p,d,p,e,p,d,p,2c#.,p,d,c#,4d,c#,d,p,e,p,4f#,p,a,p,4b,a,b,p,a,p,b,p,2a.,4p,a,b,a,4b,a,b,p,2a.,a,4f#,a,b,p,d6,p,4e.6,d6,b,p,a,p,2b";
char *song = (char *)"StarWars:d=4,o=5,b=45:32p,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#.6,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#6";
//char *song = (char *)"GoodBad:d=4,o=5,b=56:32p,32a#,32d#6,32a#,32d#6,8a#.,16f#.,16g#.,d#,32a#,32d#6,32a#,32d#6,8a#.,16f#.,16g#.,c#6,32a#,32d#6,32a#,32d#6,8a#.,16f#.,32f.,32d#.,c#,32a#,32d#6,32a#,32d#6,8a#.,16g#.,d#";
//char *song = (char *)"TopGun:d=4,o=4,b=31:32p,16c#,16g#,16g#,32f#,32f,32f#,32f,16d#,16d#,32c#,32d#,16f,32d#,32f,16f#,32f,32c#,16f,d#,16c#,16g#,16g#,32f#,32f,32f#,32f,16d#,16d#,32c#,32d#,16f,32d#,32f,16f#,32f,32c#,g#";
//char *song = (char *)"A-Team:d=8,o=5,b=125:4d#6,a#,2d#6,16p,g#,4a#,4d#.,p,16g,16a#,d#6,a#,f6,2d#6,16p,c#.6,16c6,16a#,g#.,2a#";
//char *song = (char *)"Flinstones:d=4,o=5,b=40:32p,16f6,16a#,16a#6,32g6,16f6,16a#.,16f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c6,d6,16f6,16a#.,16a#6,32g6,16f6,16a#.,32f6,32f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c6,a#,16a6,16d.6,16a#6,32a6,32a6,32g6,32f#6,32a6,8g6,16g6,16c.6,32a6,32a6,32g6,32g6,32f6,32e6,32g6,8f6,16f6,16a#.,16a#6,32g6,16f6,16a#.,16f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c.6,32d6,32d#6,32f6,16a#,16c.6,32d6,32d#6,32f6,16a#6,16c7,8a#.6";
//char *song = (char *)"Jeopardy:d=4,o=6,b=125:c,f,c,f5,c,f,2c,c,f,c,f,a.,8g,8f,8e,8d,8c#,c,f,c,f5,c,f,2c,f.,8d,c,a#5,a5,g5,f5,p,d#,g#,d#,g#5,d#,g#,2d#,d#,g#,d#,g#,c.7,8a#,8g#,8g,8f,8e,d#,g#,d#,g#5,d#,g#,2d#,g#.,8f,d#,c#,c,p,a#5,p,g#.5,d#,g#";
//char *song = (char *)"Gadget:d=16,o=5,b=50:32d#,32f,32f#,32g#,a#,f#,a,f,g#,f#,32d#,32f,32f#,32g#,a#,d#6,4d6,32d#,32f,32f#,32g#,a#,f#,a,f,g#,f#,8d#";
//char *song = (char *)"Smurfs:d=32,o=5,b=200:4c#6,16p,4f#6,p,16c#6,p,8d#6,p,8b,p,4g#,16p,4c#6,p,16a#,p,8f#,p,8a#,p,4g#,4p,g#,p,a#,p,b,p,c6,p,4c#6,16p,4f#6,p,16c#6,p,8d#6,p,8b,p,4g#,16p,4c#6,p,16a#,p,8b,p,8f,p,4f#";
//char *song = (char *)"MahnaMahna:d=16,o=6,b=125:c#,c.,b5,8a#.5,8f.,4g#,a#,g.,4d#,8p,c#,c.,b5,8a#.5,8f.,g#.,8a#.,4g,8p,c#,c.,b5,8a#.5,8f.,4g#,f,g.,8d#.,f,g.,8d#.,f,8g,8d#.,f,8g,d#,8c,a#5,8d#.,8d#.,4d#,8d#.";
//char *song = (char *)"LeisureSuit:d=16,o=6,b=56:f.5,f#.5,g.5,g#5,32a#5,f5,g#.5,a#.5,32f5,g#5,32a#5,g#5,8c#.,a#5,32c#,a5,a#.5,c#.,32a5,a#5,32c#,d#,8e,c#.,f.,f.,f.,f.,f,32e,d#,8d,a#.5,e,32f,e,32f,c#,d#.,c#";
//char *song = (char *)"MissionImp:d=16,o=6,b=95:32d,32d#,32d,32d#,32d,32d#,32d,32d#,32d,32d,32d#,32e,32f,32f#,32g,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,a#,g,2d,32p,a#,g,2c#,32p,a#,g,2c,a#5,8c,2p,32p,a#5,g5,2f#,32p,a#5,g5,2f,32p,a#5,g5,2e,d#,8d";
//char *song = (char *)"SMBtheme:d=4,o=5,b=100:16e6,16e6,32p,8e6,16c6,8e6,8g6,8p,8g,8p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,16p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16c7,16p,16c7,16c7,p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,16d#6,8p,16d6,8p,16c6";
//char *song = (char *)"SMBunderground:d=16,o=6,b=100:c,c5,a5,a,a#5,a#,2p,8p,c,c5,a5,a,a#5,a#,2p,8p,f5,f,d5,d,d#5,d#,2p,8p,f5,f,d5,d,d#5,d#,2p,32d#,d,32c#,c,p,d#,p,d,p,g#5,p,g5,p,c#,p,32c,f#,32f,32e,a#,32a,g#,32p,d#,b5,32p,a#5,32p,a5,g#5";
//char *song = (char *)"SMBwater:d=8,o=6,b=225:4d5,4e5,4f#5,4g5,4a5,4a#5,b5,b5,b5,p,b5,p,2b5,p,g5,2e.,2d#.,2e.,p,g5,a5,b5,c,d,2e.,2d#,4f,2e.,2p,p,g5,2d.,2c#.,2d.,p,g5,a5,b5,c,c#,2d.,2g5,4f,2e.,2p,p,g5,2g.,2g.,2g.,4g,4a,p,g,2f.,2f.,2f.,4f,4g,p,f,2e.,4a5,4b5,4f,e,e,4e.,b5,2c.";
//char *song = (char *)"SMBdeath:d=4,o=5,b=90:32c6,32c6,32c6,8p,16b,16f6,16p,16f6,16f.6,16e.6,16d6,16c6,16p,16e,16p,16c"; 
//char *song = (char *)"RickRoll:d=4,o=5,b=200:8g,8a,8c6,8a,e6,8p,e6,8p,d6.,p,8p,8g,8a,8c6,8a,d6,8p,d6,8p,c6,8b,a.,8g,8a,8c6,8a,2c6,d6,b,a,g.,8p,g,2d6,2c6.,p,8g,8a,8c6,8a,e6,8p,e6,8p,d6.,p,8p,8g,8a,8c6,8a,2g6,b,c6.,8b,a,8g,8a,8c6,8a,2c6,d6,b,a,g.,8p,g,2d6,2c6.";
//char *song = (char *)"2.34kHzBeeps:d=4,o=7,b=240:d,p,d,p,d,p,d,p";
//char *song = (char *)"Superman:d=4,o=6,b=200:8d5,8d5,8d5,8g.5,16p,8g5,2d,8p,8d,8e,8d,8c,1d,8p,8d5,8d5,8d5,8g.5,16p,8g5,2d,8d,8d,8e,8c,8g5,8e,2d.,p,8g5,8g5,8g5,2f#.,d.,8g5,8g5,8g5,2f#.,d.,8g5,8g5,8g5,8f#,8e,8f#,2g.,8g5,8g5,8g5,2g.5";
//char *song = (char *)"Wannabe:d=4,o=5,b=125:16g,16g,16g,16g,8g,8a,8g,8e,8p,16c,16d,16c,8d,8d,8c,e,p,8g,8g,8g,8a,8g,8e,8p,c6,8c6,8b,8g,8a,16b,16a,g";
//char *song = (char *)"YMCA:d=4,o=5,b=160:8c#6,8a#,2p,8a#,8g#,8f#,8g#,8a#,c#6,8a#,c#6,8d#6,8a#,2p,8a#,8g#,8f#,8g#,8a#,c#6,8a#,c#6,8d#6,8b,2p,8b,8a#,8g#,8a#,8b,d#6,8f#6,d#6,f.6,d#.6,c#.6,b.,a#,g#";
//char *song = (char *)"ForeverY:d=8,o=6,b=180:a#,4c7,a#,4a#,4p,f,g,4g,a#,4g#,4g,2p.,f,f,f,f,g,4g,f,4c,4p,4c,4p,f,4g,4f";

//Variable Pour le SON
byte default_dur = 4;
byte default_oct = 6;
byte lowest_oct = 3;
int bpm = 63;
int num;
long wholenote;
long duration;
byte note;
byte scale;
bool songDone = false;
char *songPtr;
*/

// variable Compteur Roue codeuse
volatile long int encoder0Pos_L = 0;
volatile long int encoder0Pos_R = 0;

//Encoder gR;
Moteur crysteo;
Odometrie roue;

Tick codeuse;

Position destination;
Position memory_destination;
Position roueCodeuse;

double tempOsc =115;
Asservissement robot(tempOsc ,crysteo);

unsigned long timer;
unsigned long timer2;
unsigned long TimerRound;

SYSTEM_MODE(AUTOMATIC);

int value,test;
int number[7];
long int val;
int etape;

int forward; // 1=forward -1=backward
double beta, depassement;
double TargetAngle;
int DoAngle;
//Détection adversaire
int StopIR;
//Donné objet capter par la raspberry
uint32_t dataRaspberry;
int detecteObjet_Raspberry;
double distance_Raspberry;
double Angle_Raspberry;
//Variable pour la corde
boolean startRound;

String robotCouleur;

/* This function is called once at start up ----------------------------------*/
void setup()
{
	//Setup the Tinker application here
         Serial.begin (115200);
         //Serial1.begin(28800);
    
    //gyro.init();
    //gyro.zeroCalibrate(200,10);//sample 200 times to calibrate and it will take 200*10m
         
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
    delay(2000);
    clap("CENTRE"); //Init position clap  
    

     
   /* roueCodeuse.x=0;        //initialise le robot au départ
    roueCodeuse.y=0;
    roueCodeuse.thetha=0;

    destination.x=0;     //consigne de positions et d'angle (ordre)
    destination.y=0;
    destination.thetha=0.0;
    destination.distance=0;
    */
   
    etape=0;
    
    codeuse.ND=0;
    codeuse.NG=0;
    
    forward=0;
    depassement=0;
    beta=0;
    DoAngle=0;
    TargetAngle=0;
    StopIR=0;
    dataRaspberry=0;
    //gR.init();
    crysteo.initPWM(); //initialise la PWM des moteurs pour qu'il puisse rouler

  pinMode(encoder0PinA_L, INPUT);
  pinMode(encoder0PinB_L, INPUT);

  pinMode(encoder0PinA_R, INPUT);
  pinMode(encoder0PinB_R, INPUT);

  attachInterrupt(encoder0PinA_L, doEncoderA_L, CHANGE);
  attachInterrupt(encoder0PinB_L, doEncoderB_L, CHANGE);

  attachInterrupt(encoder0PinA_R, doEncoderA_R, CHANGE);
  attachInterrupt(encoder0PinB_R, doEncoderB_R, CHANGE);
  
  startRound=false;
  
  //Homologation
  pinMode(cordeDemarrage, INPUT);
  pinMode(cVert, INPUT);
  pinMode(cJaune, INPUT);
  
  //Choix couleur
  int color = 0;
   display.clearDisplay();   // clears the screen and buffer
   display.setTextSize(1);    //taille de la police
   display.setTextColor(WHITE); //couleur du texte blanc sur noir
   display.setCursor(0, 0);            //Position du texte [colonne=128| ligne=32]
   display.print("Choix de la couleur !");
   display.display(); 
    
  while(color==0){
  //if Jaune team
  if(digitalRead(cJaune)==HIGH)
  {
      robotCouleur="JAUNE";
      color=1;
  }
  if(digitalRead(cVert)==HIGH)
  {
      robotCouleur="VERT";
      color=1;

  }
  
  }
  
  //if Jaune team
  if(robotCouleur=="JAUNE")
  {
     memory_destination.x=0;
    memory_destination.y=0;
    memory_destination.thetha=0;
    
    //roueCodeuse.x=200;        //initialise le robot au départ
    //roueCodeuse.y=1000;
    //roueCodeuse.thetha=0;
    
    //destination.x=200;     //consigne de positions et d'angle (ordre)
    //destination.y=1000;
    //destination.thetha=0.0;
    //destination.distance=0;
    
    roueCodeuse.x=0;        //initialise le robot au départ
    roueCodeuse.y=0;
    roueCodeuse.thetha=0;

    destination.x=0;     //consigne de positions et d'angle (ordre)
    destination.y=0;
    destination.thetha=0.0;
    destination.distance=0;
    
    //désactive l'ultrason de derriere!
    //sendIRSONAR("AVANT");
    
    //Serial.println("Couleur Jaune");
    display.clearDisplay();   // clears the screen and buffer
    display.setTextSize(1);    //taille de la police
    display.setTextColor(WHITE); //couleur du texte blanc sur noir
    display.setCursor(0, 0);            //Position du texte [colonne=128| ligne=32]
    display.print("Couleur : ");
    display.setCursor(80, 0); 
    display.println("JAUNE");
    display.display();              //permet d'afficher
  }
  else//if Verte team
  {
    memory_destination.x=0;
    memory_destination.y=0;
    memory_destination.thetha=180;
    
    //roueCodeuse.x=2800;        //initialise le robot au départ
    //roueCodeuse.y=1000;
    //roueCodeuse.thetha=3.14;

    //destination.x=2800;     //consigne de positions et d'angle (ordre)
    //destination.y=1000;
    //destination.thetha=180;
    //destination.distance=0;
    
     roueCodeuse.x=0;        //initialise le robot au départ
    roueCodeuse.y=0;
    roueCodeuse.thetha=0;

    destination.x=0;     //consigne de positions et d'angle (ordre)
    destination.y=0;
    destination.thetha=0.0;
    destination.distance=0;
    
    //sendIRSONAR("ARRIERE");
       // Serial.println("Couleur Vert");
    display.clearDisplay();   // clears the screen and buffer
    display.setTextSize(1);    //taille de la police
    display.setTextColor(WHITE); //couleur du texte blanc sur noir
    display.setCursor(0, 0);            //Position du texte [colonne=128| ligne=32]
    display.print("Couleur : ");
    display.setCursor(80, 0); 
    display.println("VERT");
    display.display();              //permet d'afficher

  }
  //char colorRobot= robotCouleur;
   //printOLED(roueCodeuse,colorRobot);
  //attachInterrupt(cordeDemarrage, CORDE , CHANGE);
 

	//Register all the Tinker functions
	Spark.function("digitalread", tinkerDigitalRead);
	Spark.function("digitalwrite", tinkerDigitalWrite);

	Spark.function("analogread", tinkerAnalogRead);
	Spark.function("analogwrite", tinkerAnalogWrite);
        
  //pinMode(tonePin,OUTPUT);
  //pinMode(D7,OUTPUT); 

    timer=millis(); 
    timer2=millis();
    delay(2000);
    
}

/* This function loops forever --------------------------------------------*/
void loop()
{
    receiveIRSONAR();
    //RASPBERRY_IR();
    
   //timer=millis();
   // delay(1000);
   // Serial.println(dataRaspberry);
   //interrupts();   // enable interrupts
    //    do{
    //    }while(millis()-timer < 10);
    if(startRound == false){
        while(startRound != true)
        {
            crysteo.fonctionnement_moteur(0,0);
            CORDE();
    //Serial.print("Start :\t");
    //Serial.println(startRound);
        }
    display.clearDisplay();   // clears the screen and buffer
    display.setTextSize(1);    //taille de la police
    display.setTextColor(WHITE); //couleur du texte blanc sur noir
    display.setCursor(0, 0);            //Position du texte [colonne=128| ligne=32]
    display.print("START");
    display.display();
    TimerRound=millis();
    delay(1000);
    //permet d'afficher
  //Serial.print("Start :\t");
  //Serial.println(startRound);
    }
     //noInterrupts(); // disable interrupts
  Serial.print("StopIR : \t");
  Serial.println(StopIR);
 // if(millis()-TimerRound <=90000){ //timer coupe
    if(StopIR==0){
        if(millis()-timer >= 100){
            LED_SetRGBColor(RGB_COLOR_RED);
            LED_On(LED_RGB);
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
        else{
            LED_SetRGBColor(RGB_COLOR_BLUE);
            LED_On(LED_RGB);
        
            codeuse.ND=encoder0Pos_R;
            codeuse.NG=encoder0Pos_L;
               /* Serial.print (encoder0Pos_L);
                Serial.print ("\t");
                Serial.println (encoder0Pos_R);
               */
            roue.retournerValeur(&roueCodeuse,codeuse);
           
            errorLT();
            if(robotCouleur=="VERT"){
                printOLED(roueCodeuse,"VEiRT");
            }
            else{
                printOLED(roueCodeuse,"JAUNE");
            }
            
            int X =roueCodeuse.x;
            int Y = roueCodeuse.y;
            int ANGLE = roueCodeuse.thetha;
            
            int DX =destination.x;
            int DY =destination.y;
            int DA=memory_destination.thetha;
             
             
           //if( (millis()-timer2) >= 5000){
            //if( roueCodeuse.distance <= 60){
            if(((X <= DX + 50) && (X >= DX - 50)) || (memory_destination.x == destination.x) ){
                if(((Y <= DY + 50) && (Y >= DY - 50)) || (memory_destination.y == destination.y) ){
                    if((ANGLE <= DA + 10) && (ANGLE >= DA - 10)){
                            LED_SetRGBColor(RGB_COLOR_GREEN);
                            LED_On(LED_RGB);
                            //scare();
                            //exemple1();
                            deplacementColor();
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
    }else{
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
    
}

void recevoir_tick(){
    value =1;
}
/*******************************************************************************
 * Function Name  : tinkerDigitalRead
 * Description    : Reads the digital value of a given pin
 * Input          : Pin 
 * Output         : None.
 * Return         : Value of the pin (0 or 1) in INT type
                    Returns a negative number on failure
 *******************************************************************************/
int tinkerDigitalRead(String pin)
{
	//convert ascii to integer
	int pinNumber = pin.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

	if(pin.startsWith("D"))
	{
		pinMode(pinNumber, INPUT_PULLDOWN);
		return digitalRead(pinNumber);
	}
	else if (pin.startsWith("A"))
	{
		pinMode(pinNumber+10, INPUT_PULLDOWN);
		return digitalRead(pinNumber+10);
	}
	return -2;
}

/*******************************************************************************
 * Function Name  : tinkerDigitalWrite
 * Description    : Sets the specified pin HIGH or LOW
 * Input          : Pin and value
 * Output         : None.
 * Return         : 1 on success and a negative number on failure
 *******************************************************************************/
int tinkerDigitalWrite(String command)
{
	bool value = 0;
	//convert ascii to integer
	int pinNumber = command.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

	if(command.substring(3,7) == "HIGH") value = 1;
	else if(command.substring(3,6) == "LOW") value = 0;
	else return -2;

	if(command.startsWith("D"))
	{
		pinMode(pinNumber, OUTPUT);
		digitalWrite(pinNumber, value);
		return 1;
	}
	else if(command.startsWith("A"))
	{
		pinMode(pinNumber+10, OUTPUT);
		digitalWrite(pinNumber+10, value);
		return 1;
	}
	else return -3;
}

/*******************************************************************************
 * Function Name  : tinkerAnalogRead
 * Description    : Reads the analog value of a pin
 * Input          : Pin 
 * Output         : None.
 * Return         : Returns the analog value in INT type (0 to 4095)
                    Returns a negative number on failure
 *******************************************************************************/
int tinkerAnalogRead(String pin)
{
	//convert ascii to integer
	int pinNumber = pin.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

	if(pin.startsWith("D"))
	{
		pinMode(pinNumber, INPUT);
		return analogRead(pinNumber);
	}
	else if (pin.startsWith("A"))
	{
		pinMode(pinNumber+10, INPUT);
		return analogRead(pinNumber+10);
	}
	return -2;
}

/*******************************************************************************
 * Function Name  : tinkerAnalogWrite
 * Description    : Writes an analog value (PWM) to the specified pin
 * Input          : Pin and Value (0 to 255)
 * Output         : None.
 * Return         : 1 on success and a negative number on failure
 *******************************************************************************/
int tinkerAnalogWrite(String command)
{
	//convert ascii to integer
	int pinNumber = command.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

	String value = command.substring(3);

	if(command.startsWith("D"))
	{
		pinMode(pinNumber, OUTPUT);
		analogWrite(pinNumber, value.toInt());
		return 1;
	}
	else if(command.startsWith("A"))
	{
		pinMode(pinNumber+10, OUTPUT);
		analogWrite(pinNumber+10, value.toInt());
		return 1;
	}
	else return -2;
}

 /******************************************************************************
 * Fonction pour recevoir les ticks des encoders
 * Utilise 4 interruptions  
 ******************************************************************************/
void doEncoderA_L(){

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
      encoder0Pos_L= encoder0Pos_L + 1;          // CW
    }
    else {
      encoder0Pos_L = encoder0Pos_L - 1;          // CCW
    }
  }

  // use for debugging - remember to comment out
}

void doEncoderB_L(){

  // look for a low-to-high on channel B
  if (digitalRead(encoder0PinB_L) == HIGH) {
   // check channel A to see which way encoder is turning
    if (digitalRead(encoder0PinA_L) == HIGH) {
      encoder0Pos_L = encoder0Pos_L + 1;         // CW
    }
    else {
      encoder0Pos_L= encoder0Pos_L - 1;         // CCW
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

void doEncoderA_R(){

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

void doEncoderB_R(){

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

/*****************************************************************************
 * Fonction pour Afficher les Informations
 * sur l'Ecran OLED 128*32
 *****************************************************************************/
void printOLED(Position alpha,char couleur[]){
    
    display.clearDisplay();   // clears the screen and buffer
    
    display.setTextSize(1);    //taille de la police
    display.setTextColor(WHITE); //couleur du texte blanc sur noir
    
    display.setCursor(0, 0);            //Position du texte [colonne=128| ligne=32]
    display.print("Couleur : ");
    display.setCursor(80, 0); 
    display.println(couleur);
 
    display.print("X : ");
    display.setCursor(20, 10); 
    display.print(alpha.x);
    
    display.setCursor(60, 10); 
    display.print("Y : ");
    display.setCursor(95, 10); 
    display.println(alpha.y);
    
    display.print("@ : ");
    display.setCursor(20, 20); 
    display.println((alpha.thetha));//*180/3.14);
    
    display.display();              //permet d'afficher
}

/*****************************************************************************
 * Fonction pour Joué de la music
 * EN UART
 *****************************************************************************/
/*
void begin_rtttl(char *p)
{
  // Absolutely no error checking in here

  // format: d=N,o=N,b=NNN:
  // find the start (skip name, etc)

  while(*p != ':') p++;    // ignore name
  p++;                     // skip ':'

  // get default duration
  if(*p == 'd')
  {
    p++; p++;              // skip "d="
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    if(num > 0) default_dur = num;
    p++;                   // skip comma
  }

  if(DEBUG) { Serial1.print("ddur: "); Serial1.println(default_dur, 10); }

  // get default octave
  if(*p == 'o')
  {
    p++; p++;              // skip "o="
    num = *p++ - '0';
    if(num >= 3 && num <=7) default_oct = num;
    p++;                   // skip comma
  }

  if(DEBUG) { Serial1.print("doct: "); Serial1.println(default_oct, 10); }

  // get BPM
  if(*p == 'b')
  {
    p++; p++;              // skip "b="
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    bpm = num;
    p++;                   // skip colon
  }

  if(DEBUG) { Serial1.print("bpm: "); Serial1.println(bpm, 10); }

  // BPM usually expresses the number of quarter notes per minute
  wholenote = (60 * 1000L / bpm) * 2;  // this is the time for whole note (in milliseconds)

  if(DEBUG) { Serial1.print("wn: "); Serial1.println(wholenote, 10); }
  
  // Save current song pointer...
  songPtr = p;
}

bool next_rtttl() {

  char *p = songPtr;
  // if notes remain, play next note
  if(*p)
  {
    // first, get note duration, if available
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    
    if(num) duration = wholenote / num;
    else duration = wholenote / default_dur;  // we will need to check if we are a dotted note after

    // now get the note
    note = 0;

    switch(*p)
    {
      case 'c':
        note = 1;
        break;
      case 'd':
        note = 3;
        break;
      case 'e':
        note = 5;
        break;
      case 'f':
        note = 6;
        break;
      case 'g':
        note = 8;
        break;
      case 'a':
        note = 10;
        break;
      case 'b':
        note = 12;
        break;
      case 'p':
      default:
        note = 0;
    }
    p++;

    // now, get optional '#' sharp
    if(*p == '#')
    {
      note++;
      p++;
    }

    // now, get optional '.' dotted note
    if(*p == '.')
    {
      duration += duration/2;
      p++;
    }
  
    // now, get scale
    if(isdigit(*p))
    {
      scale = *p - '0';
      p++;
    }
    else
    {
      scale = default_oct;
    }

    scale += OCTAVE_OFFSET;

    if(*p == ',')
      p++;       // skip comma for next note (or we may be at the end)

    // Save current song pointer...
    songPtr = p;

    // now play the note
    if(note)
    {
      if(DEBUG) {
        Serial1.print("Playing: ");
        Serial1.print(scale, 10); Serial1.print(' ');
        Serial1.print(note, 10); Serial1.print(" (");
        Serial1.print(notes[(scale - lowest_oct) * 12 + note], 10);
        Serial1.print(") ");
        Serial1.println(duration, 10);
      }
      tone(tonePin, notes[(scale - lowest_oct) * 12 + note], duration);
      //noTone(tonePin);
    }
    else
    {
      if(DEBUG) {
        Serial1.print("Pausing: ");
        Serial1.println(duration, 10);
      }
      delay(duration);
    }
    return 1; // note played successfully.
  }
  else {
    return 0; // all done
  }
}

void tone(int pin, int16_t note, int16_t duration) {
  for(int16_t x=0;x<(duration*1000/note);x++) {
    PIN_MAP[pin].gpio_peripheral->BSRR = PIN_MAP[pin].gpio_pin; // HIGH
    delayMicroseconds(note);
    PIN_MAP[pin].gpio_peripheral->BRR = PIN_MAP[pin].gpio_pin;  // LOW
    delayMicroseconds(note);
  }
}
 * */
/*****************************************************************************
 * Fonction Distance angle  & object détecter
 * 
 *****************************************************************************/
void errorLT(){
    
    roueCodeuse.thetha=(roueCodeuse.thetha*180)/3.14;
             
    roueCodeuse.distance=roue.calculer_distance( destination,roueCodeuse);
    if (roueCodeuse.distance != 0) {
        beta = asin((destination.y - roueCodeuse.y) / roueCodeuse.distance);

        if (destination.x < roueCodeuse.x) {//if target is negative
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
    depassement = beta - roueCodeuse.thetha;
    depassement=robot.boundError(depassement);
     //errorT=depassement;
    if (robot.abs1(depassement) <= 90) {//if target ahead robot
            forward = -1; //forward
        } else {//else target behind robot
            forward = 1; //backward
            depassement=beta-180-roueCodeuse.thetha;
            depassement=robot.boundError(depassement);
            beta=beta+180;
            beta=robot.boundError(beta);
        }
    
    if(DoAngle==0){
        destination.thetha=depassement;
        memory_destination.thetha=beta;
    }else{
        destination.thetha=TargetAngle- roueCodeuse.thetha;     
    }
    roueCodeuse.distance=roueCodeuse.distance*forward;
}

void ComputerVision(){
    double AngleRadian=0;
    
    
    AngleRadian=(Angle_Raspberry*3.14)/180;
    
    if(detecteObjet_Raspberry == 1){ //raspberry a détecté un objet
    //changement de coordonné Polaire au cartésien
        destination.x=roueCodeuse.x + distance_Raspberry*cos(AngleRadian);
        destination.y=roueCodeuse.y + distance_Raspberry*sin(AngleRadian);
        TargetAngle= roueCodeuse.thetha+Angle_Raspberry;
    }
}
/*****************************************************************************
 * Fonction déplacement
 * 
 *****************************************************************************/

void scare(){
    
    memory_destination.x=destination.x;
    memory_destination.y=destination.y;
    //memory_destination.thetha=destination.thetha;
    switch (etape){
        
    case 0 : 
            destination.x=0;
            destination.y=0;
            DoAngle=0;  // si à 0 => faut lui donner que la position, si à 1 => faut donner l'angle et la distance
            TargetAngle=0;// angle.destination
            etape++;
    break;
    case 1 : 
            destination.x=300;
            destination.y=0;
            TargetAngle=0;
             clap("OPEN_GAUCHE");
             DoAngle=0;
            etape++;
    break;
    case 2 : 
            destination.x=300;
            destination.y=0;
            TargetAngle+=90;
             DoAngle=1;
            etape++;
    break;
    case 3 : 
            destination.x=300;
            destination.y=300;
            //destination.thetha=90;
             DoAngle=0;
            clap("OPEN");
            etape++;
    break;
    case 4 : 
            destination.x=300;
            destination.y=300;
            TargetAngle+=90;
             DoAngle=1;
            etape++;
    break;
    case 5 : 
            destination.x=0;
            destination.y=300;
            //destination.thetha=180;
             DoAngle=0;
            clap("OPEN_DROITE");
            etape++;
    break;
    case 6 : 
            destination.x=0;
            destination.y=300;
            TargetAngle=-90;
             DoAngle=1;
            etape++;
   break;
   case 7: 
            destination.x=0;
            destination.y=0;
            //destination.thetha=90;
            clap("CENTRE");
             DoAngle=0;
            etape++;
    break;
    case 8 : 
            destination.x=0;
            destination.y=0;
           TargetAngle=0;
             DoAngle=1;
            etape=0;
    break;
    }
     memory_destination.thetha=TargetAngle;       
}

void exemple1(){
    
    memory_destination.x=destination.x;
    memory_destination.y=destination.y;
    //memory_destination.thetha=destination.thetha;
    
    switch (etape){
        
case 0 : 
            destination.x=910;
            destination.y=830;
            //destination.thetha=-13.5;
            etape++;
        break;
       
 case 1 : 
            destination.x=870;
            destination.y=1355;
            //destination.thetha=90;
            etape++;
        break;
        
case 2 : 
            destination.x=200;
            destination.y=1000;
            //destination.thetha=-30;
            etape++;
        break;
       
 case 3 : 
            destination.x=200;
            destination.y=1000;
            DoAngle=1;
            TargetAngle=0;
            //destination.thetha=0;
            //etape;
        break;
        

    }
    
    memory_destination.thetha=TargetAngle;
            
}
//Moitier pour les claps Jaune
void homologationJ(){
   memory_destination.x=destination.x;
    memory_destination.y=destination.y; 
    switch (etape){
        
case 0 : 
            destination.x=200;
            destination.y=1000;
            DoAngle=0;
            TargetAngle=0;
            etape++;
            sendIRSONAR("AVANT");
        break;
       
 case 1 : 
            destination.x=700;
            destination.y=1000;
            //destination.thetha=0;
            etape++;
            sendIRSONAR("AVANT");
            
        break;
        
case 2 : 
            destination.x=700;
            destination.y=1000;
            DoAngle=1;
            TargetAngle+=90;
            //destination.thetha=90;
            etape++;
            sendIRSONAR("BOTH");
        break;
       
 
 case 3 : 
            destination.x=700;
            destination.y=1770;
            DoAngle=0;
            //destination.thetha=90;
            etape++;
        break;
case 4 : 
            destination.x=700;
            destination.y=1770;
            DoAngle=1;
	    // Rotation de 90°DoAngle = autorisation a faire son angle
            TargetAngle=180;
            etape++;
        break;
// Avant les claps etaient a 110mm devant maintenant ils sont a 110 mm derriere
case 5 : 
            //destination.x=200;
            destination.x=200;
            destination.y=1770;
            DoAngle=0;
            
            //destination.thetha=180;
            etape++;
        break;
case 6 : 
            //destination.x=300;
            destination.x=500 ; 
            destination.y=1770;
            DoAngle=0;
            clap("OPEN_GAUCHE");
            
            //destination.thetha=180;
            etape++;
        break;       
case 7: 
            //destination.x=600;
            destination.x=750;
            destination.y=1770;
            clap("CLOSE_GAUCHE");
           
            //destination.thetha=180;
            etape++;
        break;
case 8 : 
            //destination.x=900;
            destination.x=1055;
            destination.y=1770;
            DoAngle=0;
            clap("OPEN_GAUCHE");
            etape++;
            //destination.thetha=180;
            
        break;
case 9 : 
            //destination.x=1500;
            destination.x=1000;
	    destination.y=1400;
            DoAngle=0;
            
            etape++;;
            //destination.thetha=180;
            
        break;
case 10 : 
            //destination.x=1500;
            destination.x=1500;
	    destination.y=1000;
            clap("CLOSE_GAUCHE");
            DoAngle=0;
          
            //destination.thetha=180;
            
        break;
    }
    memory_destination.thetha=TargetAngle;        
}

//Moitier pour les claps Vert
void homologationV(){
   memory_destination.x=destination.x;
    memory_destination.y=destination.y; 
    switch (etape){
        
case 0 : 
            destination.x=2800;
            destination.y=1000;
            DoAngle=0;
            TargetAngle=180;
            sendIRSONAR("ARRIERE");
            etape++;
            
        break;
       
 case 1 : 
            destination.x=2300;
            destination.y=1000;
            sendIRSONAR("ARRIERE");
            sendIRSONAR("BOTH");
            //destination.thetha=0;
            etape++;
        break;
        
case 2 : 
            destination.x=2300;
            destination.y=1000;
            DoAngle=1;
            TargetAngle=90;
            sendIRSONAR("BOTH");
            //destination.thetha=90;
            etape++;
        break;
       
 
 case 3 : 
            destination.x=2300;
            destination.y=1770;
            DoAngle=0;
            //destination.thetha=90;
            etape++;
        break;
case 4 : 
            destination.x=2300;
            destination.y=1770;
            DoAngle=1;
	    // Rotation de 90°DoAngle = autorisation a faire son angle
            TargetAngle=0;
            etape++;
        break;
// Avant les claps etaient a 110mm devant maintenant ils sont a 110 mm derriere
case 5 : 
            //destination.x=200;
            destination.x=2800;
            destination.y=1770;
            DoAngle=0;
            
            //destination.thetha=180;
            etape++;
        break;
case 6 : 
            //destination.x=300;
            destination.x=2500 ; 
            destination.y=1770;
            DoAngle=0;
            clap("OPEN_DROITE");
            
            //destination.thetha=180;
            etape++;
        break;       
case 7: 
            //destination.x=600;
            destination.x=2250;
            destination.y=1770;
            clap("CLOSE_DROITE");
           
            //destination.thetha=180;
            etape++;
        break;
case 8 : 
            //destination.x=900;
            destination.x=2000;
            destination.y=1770;
            DoAngle=0;
            clap("OPEN_DROITE");
            etape++;
            //destination.thetha=180;
            
        break;
case 9 : 
            //destination.x=1500;
            destination.x=2000;
	    destination.y=1400;
            DoAngle=0;
           
            etape++;
            //destination.thetha=180;
            
        break;
case 10 : 
            //destination.x=1500;
            destination.x=1500;
	    destination.y=1000;
             clap("CLOSE_DROITE");
            DoAngle=0;
            etape++;
            //destination.thetha=180;
            
        break;
case 11 : 
            //destination.x=1500;
            destination.x=2800;
            destination.y=1000;
            DoAngle=0;
            
            //destination.thetha=180;
            
        break;   
        
    }
    memory_destination.thetha=TargetAngle;        
}
void exemple3(){
     memory_destination.x=destination.x;
    memory_destination.y=destination.y; 
    switch (etape){
        
case 0 : 
            destination.x=200;
            destination.y=1000;
            DoAngle=0;
            TargetAngle=0;
            etape++;
            sendIRSONAR("AVANT");
        break;
       
 case 1 : 
            destination.x=700;
            destination.y=1000;
            //destination.thetha=0;
            etape++;
            sendIRSONAR("AVANT");
        break;
        
case 2 : 
            destination.x=700;
            destination.y=1000;
            DoAngle=1;
            TargetAngle+=90;
            //destination.thetha=90;
            etape++;
            sendIRSONAR("BOTH");
        break;
       
 
 case 3 : 
            destination.x=700;
            destination.y=1770;
            DoAngle=0;
            //destination.thetha=90;
            etape++;
        break;
case 4 : 
            destination.x=700;
            destination.y=1770;
            DoAngle=1;
	
   // Rotation de 90°DoAngle = autorisation a faire son angle
            TargetAngle=180;
            etape++;
        break;
// Avant les claps etaient devant de rebot maintenant ils sont derriere 
case 5 : 
            destination.x=200;
            destination.y=1770;
            DoAngle=0;
            
            //destination.thetha=180;
            etape++;
        break;
case 6 : 
            destination.x=500;
            destination.y=1770;
            DoAngle=0;
            clap("OPEN_GAUCHE");
            
            //destination.thetha=180;
            etape++;
        break;       
case 7: 
            destination.x=750;
            destination.y=1770;
            clap("CLOSE_GAUCHE");
           
            //destination.thetha=180;
            etape++;
        break;
case 8 : 
            destination.x= 1000 ;
            destination.y=1770;
            DoAngle=0;
            clap("OPEN_GAUCHE");
            etape++;
            //destination.thetha=180;
            
        break;
case 9 : 
            destination.x=1500;
            destination.y=1000;
            DoAngle=0;
            
            //destination.thetha=180;
            etape++;
            
        break;
case 10 : 
            destination.x=2130;
            destination.y=1000;
            clap("CLOSE_GAUCHE");
            etape++ ;
            
        break;
case 11 : 
            destination.x=2130;
            destination.y=1000;
            DoAngle=1;
            TargetAngle=90;
            etape++ ;
            
        break;
case 12 : 
            destination.x=2130;
            destination.y=1770;
            DoAngle=0;
           
            etape++ ;
            
        break;
case 13 : 
            destination.x=2130;
            destination.y=1770;
            DoAngle=1;
            TargetAngle=0;
           
            etape++ ;
            
        break;
case 14 : 
            destination.x=2650;
            destination.y=1770;
            DoAngle=0;
           
            etape++ ;
            
        break;
case 15 : 
            destination.x=2200;
            destination.y=1770;
            DoAngle=0;
           
            clap("OPEN_DROITE");
            etape++;
       
        break;
case 16 : 
            destination.x=2000;
            destination.y=1770;
            DoAngle=0;
           
            
            etape++;
       
        break;
case 17 : 
            //destination.x=1500;
            destination.x=1500;
	    destination.y=1000;
            clap("CLOSE_DROITE");
            DoAngle=0;
            etape++;
            //clap("CLOSE_GAUCHE");
            //destination.thetha=180;
            
        break;
case 18 : 
            //destination.x=1500;
            destination.x=200;
	    destination.y=1000;
            DoAngle=0;
            //clap("CLOSE_GAUCHE");
            //destination.thetha=180;
            
        break;       
      
    }
    memory_destination.thetha=TargetAngle;              
}

void strategyOneJaune(){
    memory_destination.x=destination.x;
    memory_destination.y=destination.y; 

switch (etape){
        
    case 0 : 
            destination.x=200;
            destination.y=1000;
            DoAngle=0;
            TargetAngle=0;
            etape++;
            sendIRSONAR("AVANT");
    break;
    
    case 1 : 
            destination.x=700;
            destination.y=1000;
            //destination.thetha=0;
            etape++;
            sendIRSONAR("AVANT");
    break;
        
    case 2 : 
            destination.x=700;
            destination.y=1000;
            DoAngle=1;
            TargetAngle+=90;
            //destination.thetha=90;
            etape++;
            sendIRSONAR("BOTH");
    break;

    case 3 : 
            destination.x=700;
            destination.y=1750;
            DoAngle=0;
            //destination.thetha=90;
            etape++;
    break;
    
    case 4 : 
            destination.x=700;
            destination.y=1750;
            DoAngle=1;
	
   // Rotation de 90°DoAngle = autorisation a faire son angle
            TargetAngle=180;
            etape++;
        break;
// Avant les claps etaient devant de rebot maintenant ils sont derriere 
    case 5 : 
            destination.x=200;
            destination.y=1750;
            DoAngle=0;
            
            //destination.thetha=180;
            etape++;
    break;
        
    case 6 : 
            destination.x=500;
            destination.y=1750;
            DoAngle=0;
            clap("OPEN_GAUCHE");
            
            //destination.thetha=180;
            etape++;
    break;       
    
    case 7: 
            destination.x=750;
            destination.y=1750;
            clap("CLOSE_GAUCHE");
           
            //destination.thetha=180;
            etape++;
    break;
    
    case 8 : 
            destination.x= 1080 ;
            destination.y=1750;
            DoAngle=0;
            clap("OPEN_GAUCHE");
            etape++;
            //destination.thetha=180;
            
        break;
    
    case 9 : 
            destination.x=1300;
            destination.y=1400;
            DoAngle=0;
            
            //destination.thetha=180;
            etape++;
            
        break;
        
    case 10 : 
            destination.x=870;
            destination.y=1355;
            clap("CLOSE_GAUCHE");
            etape++ ;
            
    break;
    
    case 11 : 
            destination.x=400;
            destination.y=1000;
            etape++ ;
            
    break;
    
    case 12 : 
            destination.x=910;
            destination.y=830;
            DoAngle=0;          
            etape++ ;
            
    break;
    
    case 13 : 
            destination.x=1500;
            destination.y=830;
            DoAngle=0;
            etape++ ;
            
        break;
    
    case 14 : 
            destination.x=2090;
            destination.y=830;
            DoAngle=0;
            etape++ ;
            
        break;
    
    case 15 : 
            destination.x=2700;
            destination.y=500;
            DoAngle=0;
            etape++ ;
            
        break;
    
    case 16 : 
            destination.x=2200;
            destination.y=600;
            DoAngle=0;           
            etape++;
       
        break;
    
    case 17 : 
            destination.x=2200;
            destination.y=600;
            DoAngle=1;
            TargetAngle=90;
            etape++;
       
        break;
    
    case 18 : 
            
            destination.x=2150;
	    destination.y=1000;
            DoAngle=0;
            etape++;

            
        break;
    
    case 19 : 
            
            destination.x=2400;
	    destination.y=1750;
            DoAngle=0;
            etape++;
            
    break;
    
    case 20 : 
            
            destination.x=2400;
	    destination.y=1750;
            DoAngle=1;
            TargetAngle = 180;
            clap("OPEN_GAUCHE");
            etape++;
            
    break;
    
    case 21 : 
            
            destination.x=2750;
	    destination.y=1400;
            DoAngle=0;
            clap("CENTRE");
            
            
    break; 
      
    }
    memory_destination.thetha=TargetAngle; 
}

void strategyOneVert(){
    memory_destination.x=destination.x;
    memory_destination.y=destination.y; 

switch (etape){
        
    case 0 : 
            destination.x=2800;
            destination.y=1000;
            DoAngle=0;
            TargetAngle=0;
            etape++;
            sendIRSONAR("ARRIERE");
    break;
    
    case 1 : 
            destination.x=2300;
            destination.y=1000;
            //destination.thetha=0;
            etape++;
            sendIRSONAR("ARRIERE");
    break;
        
    case 2 : 
            destination.x=2300;
            destination.y=1000;
            DoAngle=1;
            TargetAngle+=90;
            //destination.thetha=90;
            etape++;
            sendIRSONAR("BOTH");
    break;

    case 3 : 
            destination.x = 2300;
            destination.y = 1750;
            DoAngle=0;
            //destination.thetha=90;
            etape++;
    break;
    
    case 4 : 
            destination.x= 2300;
            destination.y= 1750;
            DoAngle=1;
	
   // Rotation de 90°DoAngle = autorisation a faire son angle
            TargetAngle=0;
            etape++;
        break;
// Avant les claps etaient devant de rebot maintenant ils sont derriere 
    case 5 : 
            destination.x=2800;
            destination.y=1750;
            DoAngle=0;
            
            //destination.thetha=180;
            etape++;
    break;
        
    case 6 : 
            destination.x=2500;
            destination.y=1750;
            DoAngle=0;
            clap("OPEN_DROITE");
            //destination.thetha=180;
            etape++;
    break;       
    
    case 7: 
            destination.x=2250;
            destination.y=1750;
            clap("CLOSE_DROITE"); 
            //destination.thetha=180;
            etape++;
    break;
    
    case 8 : 
            destination.x= 1920 ;
            destination.y=1750;
            DoAngle=0;
            clap("OPEN_DROITE");
            etape++;
            //destination.thetha=180;
            
        break;
    
    case 9 : 
            destination.x=1700;
            destination.y=1400;
            DoAngle=0;
            
            //destination.thetha=180;
            etape++;
            
        break;
        
    case 10 : 
            destination.x=2130;
            destination.y=1355;
            clap("CLOSE_DROITE");
            etape++ ;
            
    break;
    
    case 11 : 
            destination.x=2600;
            destination.y=1000;
            etape++ ;
            
    break;
    
    case 12 : 
            destination.x=2090;
            destination.y=830;
            DoAngle=0;          
            etape++ ;
            
    break;
    
    case 13 : 
            destination.x=1500;
            destination.y=830;
            DoAngle=0;
            etape++ ;
            
        break;
    
    case 14 : 
            destination.x=810;
            destination.y=830;
            DoAngle=0;
            etape++ ;
            
        break;
    
    case 15 : 
            destination.x=300;
            destination.y=500;
            DoAngle=0;
            etape++ ;
            
        break;
    
    case 16 : 
            destination.x=700;
            destination.y=600;
            DoAngle=0;           
            etape++;
       
        break;
    
    case 17 : 
            destination.x=700;
            destination.y=600;
            DoAngle=1;
            TargetAngle=90;
            etape++;
       
        break;
    
    case 18 : 
            
            destination.x=700;
	    destination.y=1000;
            DoAngle=0;
            etape++;

            
        break;
    
    case 19 : 
            
            destination.x=600;
	    destination.y=1750;
            DoAngle=0;
            etape++;
            
    break;
    
    case 20 : 
            
            destination.x=600;
	    destination.y=1750;
            DoAngle=1;
            TargetAngle = 0;
            clap("OPEN_DROITE");
            etape++;
            
    break;
    
    case 21 : 
            
            destination.x=250;
	    destination.y=1400;
            DoAngle=0;
            clap("CENTRE");
            
            
    break; 
      
    }
    memory_destination.thetha=TargetAngle; 
}

void deplacementColor(){
    if(robotCouleur=="VERT"){
        //homologationV();
        //strategyOneVert();
        scare();
    }
    else{
        //homologationJ();
        //exemple3();
        //strategyOneJaune();
        scare();
    }
}

/*****************************************************************************
 * Fonction communication Arduino pour les actionneurs
 * 
 *****************************************************************************/
void clap(char action[]){
    int number=0;
    Wire.beginTransmission(_CLAP_);
    
    if(action == "CENTRE"){
        number=0;
    }else if (action == "OPEN_DROITE"){
        number=1;// number=3;
    }else if (action == "OPEN_GAUCHE"){
       number=2; // number=4;
    }else if (action == "CLOSE_DROITE"){
       number=0; // number=2;
    }else if (action == "CLOSE_GAUCHE"){
       number=0; // number=2;
    }else if (action == "OPEN"){
       number=5; 
    }

    switch(number){
        case 0:
            Wire.write(0);
            break;
        case 1:
            Wire.write(1);
            break;
        case 2:
            Wire.write(2);
            break;
        case 3:
            Wire.write(3);
            break;
        case 4:
            Wire.write(4);
            break;
        case 5:
            Wire.write(5);
            break;
        default:
           Wire.write(2); 
            break;
        
    }
    Serial.println(number);
    
    Wire.endTransmission();
            
}

void  sendIRSONAR(char action[]){
    
     int number=0;
    Wire.beginTransmission(_IRSONAR_);
    
    if(action == "BOTH"){
        number=0;
    }else if (action == "AVANT"){
        number=1;// number=3;
    }else if (action == "ARRIERE"){
       number=2; // number=4;
    }

    switch(number){
        case 0:
            Wire.write(0);
            break;
        case 1:
            Wire.write(1);
            break;
        case 2:
            Wire.write(2);
            break;
        case 3:
            Wire.write(3);
            break;
        case 4:
            Wire.write(4);
            break;
        case 5:
            Wire.write(5);
            break;
        default:
           Wire.write(0); 
            break;
        
    }
    Serial.println(number);
    
    Wire.endTransmission();
    
}
void receiveIRSONAR(){
    
    Wire.requestFrom(_IRSONAR_, 1);//request 1 bytes
      if( Wire.available() ){
          StopIR=Wire.read();
      }
        
            
}

void RASPBERRY_IR(){

    String inString= "";
    Wire.requestFrom(_CLAP_, 8 );//request 4 bytes
      while( Wire.available() ){
         // dataRaspberry=Wire.read();
        char c=Wire.read();
        //Serial.print(c);
        inString += c;
       
      }
    
    dataRaspberry=inString.toInt();
            inString = "";
     
}

//démarrage du robot par la ficelle 
void CORDE(){
    
    if(digitalRead(cordeDemarrage)==LOW ){
         startRound= true;
    }
   
}
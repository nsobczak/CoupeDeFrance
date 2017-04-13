/**
 *    \file CoupeDeFranceDetection.ino
 *    \brief code de la carte cateurs
 *    \author Arthur D. & Nicolas Sobczak
 *    \date Mars 2017
 *  This version of code sends messages with only data from the IR sensors.
 *  During final testing, the ultrasonic sensors were found to be too inconsistent.
 */
//_______________________________________________________________________________________________________


/* ======================================================================================================
 *      Initialisation
 * ======================================================================================================
 */
#include <Arduino.h>
//Librairie OF Infra RED :
#include "SharpIR.h"
#include "i2cCommunication.h"
#include <Wire.h>

#define _DEBUG_ true

#define _SENSORSBOARD_SENDADRESS_ 11

#define OWN_STD_ID 01 //avant = 23
#define OWN_EXT_ID 1000
#define ULTRASONIC_EXT_ID 1002
#define IR_EXT_ID 1001


/* === IR pins === */
const int IRPinB = A6, IRPinL = A3, IRPinR = A2, IRPinFL = A1, IRPinFR = A0;
const int NUM_IR_SAMPLES = 25;
SharpIR sharpBT(IRPinFR, NUM_IR_SAMPLES, 93, 1080);
SharpIR sharpFT(IRPinFL, NUM_IR_SAMPLES, 93, 1080);
SharpIR sharpFBR(IRPinR, NUM_IR_SAMPLES, 93, 1080);
SharpIR sharpFBC(IRPinL, NUM_IR_SAMPLES, 93, 1080);
SharpIR sharpFBL(IRPinB, NUM_IR_SAMPLES, 93, 1080);
SharpIR infrared[] = {sharpBT, sharpFT, sharpFBR, sharpFBC, sharpFBL};
const int NUM_IR = 5; // Number of IR sensor

/* === pin for UltraSound === */
//Trig
int trigPin = 11;
//echo
const int echoPinR = 10, echoPinB2 = 9, echoPinB1 = 8, echoPinL = 7, echoPinFL = 6;
const int ultrasonic[] = {echoPinR, echoPinL, echoPinB1, echoPinB2}; //echoPinFL,
const int NUM_ULTRASONIC = 4; // Number of ultrasonic sensor
//Use Variable
//int durationFR, durationFL, durationR, durationL, durationB,
//      distanceCMFR, distanceCMFL, distanceCMR, distanceCML, distanceCMB;

/* === pin For MUX === */
int A=2;
int B=3;
int C=12;

int irValue[NUM_IR];
int ultrasonicValue[NUM_ULTRASONIC];
int variableSent = 0;


/* ======================================================================================================
 *      Fonctions
 * ======================================================================================================
 */

/**
 * \fn void requestEvent()
 * \brief function that executes whenever data is requested by master this function is registered as an event, see setup()
 */
void requestEvent()
{
        /*Notes: tableau id i2c
           {sharpBT, sharpFT, sharpFBR, sharpFBC, sharpFBL};
           {echoPinR, echoPinL, echoPinB1, echoPinB2}

           0 => infraredSensorBackValue;
           1 => infraredSensorFrontTopValue;
           2 => infraredSensorFrontBottomRightValue;
           3 => infraredSensorFrontBottomCenterValue;
           4 => infraredSensorFrontBottomLeftValue;

           5 => ultrasonicRightValue;
           6 => ultrasonicLeftValue;
           7 => ultrasonicBack1Value;
           8 => ultrasonicBack2Value;
         */
        if ( variableSent > 8) variableSent = 0;
        Serial.println("request received");
        byte bytesTab[2];
        byte data[3];

        switch (variableSent) {
        case 0 ... 4:
                intTo2Bytes(bytesTab, irValue[variableSent]); // conversion sur 2 octets de la valeur à envoyer
                data[0] = variableSent;
                data[1] = bytesTab[0];
                data[2] = bytesTab[1];
                Wire.write(data, 3);
                break;
        case 5 ... 8:
                intTo2Bytes(bytesTab, ultrasonicValue[variableSent - NUM_IR]); // conversion sur 2 octets de la valeur à envoyer
                data[0] = variableSent;
                data[1] = bytesTab[0];
                data[2] = bytesTab[1];
                Wire.write(data, 3);
                break;
        default:
                if (_DEBUG_) Serial.println("resquestEvent: variable recue incinnue");
                break;
        }
        variableSent++;
}


int getIRValue(SharpIR sharpIR){
        return sharpIR.distance();
}


boolean setChannel(int channelNumber){
        //Set MUX
        switch(channelNumber) {
        case 0: //Back
                digitalWrite(A,LOW);
                digitalWrite(B,LOW);
                digitalWrite(C,LOW);
                break;
        case 1: //Left
                digitalWrite(A,HIGH);
                digitalWrite(B,LOW);
                digitalWrite(C,LOW);
                break;
        case 2: //Right
                digitalWrite(A,LOW);
                digitalWrite(B,HIGH);
                digitalWrite(C,LOW);
                break;
        case 3: //Front Left
                digitalWrite(A,HIGH);
                digitalWrite(B,HIGH);
                digitalWrite(C,LOW);
                break;
        case 4:  //Front Right
                digitalWrite(A,LOW);
                digitalWrite(B,LOW);
                digitalWrite(C,HIGH);
                break;
        default:
                break;
        }
}


int getUltrasonicValue(int echoPin){
        int channel = 0;
        switch(echoPin) {
        case echoPinB2:
                channel = 0;
                break;
        case echoPinB1:
                channel = 1;
                break;
        case echoPinL:
                channel = 2;
                break;
        case echoPinFL:
                channel = 3;
                break;
        case echoPinR:
                channel = 4;
                break;
        default:
                break;
        }

        setChannel(channel);

        delay(10);
        //Take Data
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        //calculations made, but not necessary to use other variables
        //  duration = pulseIn(echoPin , HIGH);
        //
        //  distanceCM = durationFR/58.2;

        //Wait for maximum 15 ms, should be able to see about 2.5m from sensor
        return pulseIn(echoPin, HIGH, 15000)/58.2;
}


//_____________________________________________________________________________________________________________
/**
 * \fn void setup()
 * \brief fonction setup d'arduino
 */
void setup()
{
        // === Setup Ultrasonic ===
        pinMode(trigPin, OUTPUT); //trig
        pinMode(echoPinR, INPUT); //echo
        // === Setup MUX ===
        pinMode(A,OUTPUT);
        pinMode(B,OUTPUT);
        pinMode(C,OUTPUT);

        Wire.begin(_SENSORSBOARD_SENDADRESS_); // join i2c bus with address #8
        Wire.onRequest(requestEvent); // register event

        Serial.begin(9600);
}

/**
 * \fn void loop()
 * \brief fonction loop d'arduino : Takes readings (and sends via I2C)
 */
void loop() {
        // long start = millis();

        for(int i = 0; i < NUM_IR; i++) irValue[i] = getIRValue(infrared[i]);
        for(int i = 0; i < NUM_ULTRASONIC; i++) ultrasonicValue[i] = getUltrasonicValue(ultrasonic[i]);

        if (_DEBUG_) {
                Serial.println("=== IR values ===");
                for(int i = 0; i < NUM_IR; i++)
                {
                        Serial.print("\tirValue = \t");
                        Serial.println(irValue[i]);
                }
                Serial.println("=== Ultrasonic values ===");
                for(int i = 0; i < NUM_ULTRASONIC; i++)
                {
                        Serial.print("\tUltrasonic value = \t");
                        Serial.println(ultrasonicValue[i]);
                }
        }

        delay(300);
}

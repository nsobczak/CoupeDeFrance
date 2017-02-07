/*
 * This version of code sends messages with only data from the IR sensors.
 *  During final testing, the ultrasonic sensors were found to be too inconsistent.
*/

#include <MCP23X08.h>
//#include <Spi.h>
#include <MCP2510.h>
#include <Canutil.h>
#include <Wire.h>
#include <SPI.h>
#include <LiquidCrystal.h>

//Librairie OF Infra RED : 
#include <SharpIR.h>

//IR pins
const int IRPinB = A4, IRPinL = A3, IRPinR = A2, IRPinFL = A1, IRPinFR = A0;
const int NUM_IR_SAMPLES = 25;
SharpIR sharpFR(IRPinFR, NUM_IR_SAMPLES, 93, 1080);
SharpIR sharpFL(IRPinFL, NUM_IR_SAMPLES, 93, 1080);
SharpIR sharpR(IRPinR, NUM_IR_SAMPLES, 93, 1080);
SharpIR sharpL(IRPinL, NUM_IR_SAMPLES, 93, 1080);
SharpIR sharpB(IRPinB, NUM_IR_SAMPLES, 93, 1080);
SharpIR infrared[] = {sharpFR, sharpFL, sharpR, sharpL, sharpB};
const int NUM_IR = 5;
//pin for UltraSound
//Trig
int trigPin = 4;

//echo
const int echoPinB = 9, echoPinL = 8, echoPinR = 7, echoPinFL = 6, echoPinFR = 5;
const int ultrasonic[] = {echoPinFR, echoPinFL, echoPinR, echoPinL, echoPinB};
const int NUM_ULTRASONIC = 5;
//Use Variable
//int durationFR, durationFL, durationR, durationL, durationB,
//      distanceCMFR, distanceCMFL, distanceCMR, distanceCML, distanceCMB;

//pin For MUX
int A=3;
int B=0;
int C=1;

//******************************************************************************************************************CAN ID NE PAS TOUCHER*******************************************************

#define OWN_STD_ID 01 //avant = 23
#define OWN_EXT_ID 1000
#define ULTRASONIC_EXT_ID 1002
#define IR_EXT_ID 1001

MCP2510  can_dev (10); // defines pb1 (arduino pin9) as the _CS pin for MCP2510
Canutil  canutil(can_dev);

void setup() {
//******************************************************************************************************************CAN NE PAS TOUCHER**********************************************************
  uint8_t opmode;
  // put your setup code here, to run once:
  canutil.setClkoutMode(0, 0); // disables CLKOUT
  canutil.setTxnrtsPinMode(0, 0, 0); // all TXnRTS pins as all-purpose digital input

  canutil.setOpMode(4); // sets configuration mode
  // IMPORTANT NOTE: configuration mode is the ONLY mode where bit timing registers (CNF1, CNF2, CNF3), acceptance
  // filters and acceptance masks can be modified

  canutil.waitOpMode(4);  // waits configuration mode
  // Bit timing section
  //  setting the bit timing registers with Fosc = 16MHz -> Tosc = 62,5ns
  // data transfer = 125kHz -> bit time = 8us, we choose arbitrarily 8us = 16 TQ  (8 TQ <= bit time <= 25 TQ)
  // time quanta TQ = 2(BRP + 1) Tosc, so BRP =3
  // sync_seg = 1 TQ, we choose prop_seg = 2 TQ
  // Phase_seg1 = 7TQ yields a sampling point at 10 TQ (60% of bit length, recommended value)
  // phase_seg2 = 6 TQ SJSW <=4 TQ, SJSW = 1 TQ chosen
  can_dev.write(CNF1, 0x03); // SJW = 1, BRP = 3
  can_dev.write(CNF2, 0b10110001); //BLTMODE = 1, SAM = 0, PHSEG = 6, PRSEG = 1
  can_dev.write(CNF3, 0x05);  // WAKFIL = 0, PHSEG2 = 5

  // SETUP MASKS / FILTERS FOR CAN
  canutil.setRxOperatingMode(2, 1, 0);  // standard ID messages only  and rollover
  canutil.setAcceptanceFilter(0x102, 0, 0, 1); // 0 <= stdID <= 2047, 0 <= extID <= 262143, 1 = extended, filter# 0
  canutil.setAcceptanceFilter(0x101, 0, 0, 5); // 0 <= stdID <= 2047, 0 <= extID <= 262143, 1 = extended, filter# 0
  canutil.setAcceptanceFilter(0x100, 0, 0, 0); // 0 <= stdID <= 2047, 0 <= extID <= 262143, 1 = extended, filter# 1
  canutil.setAcceptanceMask(0x000, 0x00000000, 0); // 0 <= stdID <= 2047, 0 <= extID <= 262143, buffer# 0

  canutil.setOpMode(0); // sets normal mode
  opmode = canutil.whichOpMode();

  canutil.setTxBufferDataLength(0, 5, 0); // TX normal data, 1 byte long, with buffer 0
  
//******************************************************************************************************************FIN CAN NE PAS TOUCHER**********************************************************

//********************************************************************Setup Ultrasonic

//trig
pinMode(trigPin, OUTPUT);

//echo
pinMode(echoPinFR, INPUT);

//********************************************************************Setup MUX
pinMode(A,OUTPUT);
pinMode(B,OUTPUT);
pinMode(C,OUTPUT);
 
}

//Takes readings and sends them over CAN bus using approriate message identifiers
void loop() {
  int IRValue[NUM_IR];
  int ultrasonicValue[NUM_ULTRASONIC];
  long start = millis();

  for(int i = 0; i < NUM_IR; i++){
    IRValue[i] = getIRValue(infrared[i]);
  }
  sendIRValues(IRValue);
  
//  for(int i = 0; i < NUM_ULTRASONIC; i++){
//    ultrasonicValue[i] = getUltrasonicValue(ultrasonic[i]);
//  }
//  delay(10);
//  sendUltrasonicValues(ultrasonicValue);
  
  //Make sure messages are spaced out enough (100ms should work)
  if(millis() - start < 50){
    delay(200 - (millis() - start));
  }
}

int getIRValue(SharpIR sharpIR){
  return sharpIR.distance();
}

int getUltrasonicValue(int echoPin){
  int channel = 0;
  switch(echoPin){
    case echoPinB:
    channel = 0;
    break;
    case echoPinL:
    channel = 1;
    break;
    case echoPinR:
    channel = 2;
    break;
    case echoPinFL:
    channel = 3;
    break;
    case echoPinFR:
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
  return pulseIn(echoPin , HIGH, 15000)/58.2;
}

boolean setChannel(int channelNumber){
      //Set MUX
  switch(channelNumber){
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
    case 4: //Front Right
      digitalWrite(A,LOW);
      digitalWrite(B,LOW);
      digitalWrite(C,HIGH);
      break;
    default:
      break;
  }
}

void sendIRValues(int IRValue[NUM_IR]){
  
  //**************************************************************************************Début de la transmission CAN : ***************************************************************************
  delay(10);
    uint8_t message[8];
    uint8_t txstatus;
    message[0] = IRValue[0];
    message[1] = IRValue[1];
    message[2] = IRValue[2];
    message[3] = IRValue[3];
    message[4] = IRValue[4];
    canutil.setTxBufferID(OWN_STD_ID, IR_EXT_ID, 1, 0); // sets the message ID, specifies standard message (i.e. short ID) with buffer 0
    canutil.setTxBufferDataField(message, 0);   // fills TX buffer
    //Serial.print("writing with id n100 with message: ");
    //Serial.println(message[1]);
    canutil.messageTransmitRequest(0, 1, 3); // requests transmission of buffer 0 with highest priority*/
    do {
      txstatus = 0;
      txstatus = canutil.isTxError(0);  // checks tx error
      txstatus = txstatus + canutil.isArbitrationLoss(0);  // checks for arbitration loss
      txstatus = txstatus + canutil.isMessageAborted(0);  // ckecks for message abort
      txstatus = txstatus + canutil.isMessagePending(0);   // checks transmission
    }
    while (txstatus != 0);
    
  //**************************************************************************************Fin de la transmission CAN : ***************************************************************************  
}
void sendUltrasonicValues(int ultrasonicValue[NUM_ULTRASONIC]){
  
  //**************************************************************************************Début de la transmission CAN : ***************************************************************************
    delay(10);
    uint8_t message[8];
    uint8_t txstatus;
    message[0] = ultrasonicValue[0];
    message[1] = ultrasonicValue[1];
    message[2] = ultrasonicValue[2];
    message[3] = ultrasonicValue[3];
    message[4] = ultrasonicValue[4];
    canutil.setTxBufferID(OWN_STD_ID, ULTRASONIC_EXT_ID, 1, 0); // sets the message ID, specifies standard message (i.e. short ID) with buffer 0
    canutil.setTxBufferDataField(message, 0);   // fills TX buffer
    //Serial.print("writing with id n100 with message: ");
    //Serial.println(message[0]);
    canutil.messageTransmitRequest(0, 1, 3); // requests transmission of buffer 0 with highest priority*/
    do {
      txstatus = 0;
      txstatus = canutil.isTxError(0);  // checks tx error
      txstatus = txstatus + canutil.isArbitrationLoss(0);  // checks for arbitration loss
      txstatus = txstatus + canutil.isMessageAborted(0);  // ckecks for message abort
      txstatus = txstatus + canutil.isMessagePending(0);   // checks transmission
    }
    while (txstatus != 0);
    
  //**************************************************************************************Fin de la transmission CAN : ***************************************************************************  
}

/**
 *    \file master.cpp
 *    \brief Esclave du Test07 I2C
 *    \author Nicolas Sobczak
 *    \date decembre 2016
*/

#include <Wire.h>
#include "i2cCommunication.h"

#define _SENDADRESS_ 8
#define _RECEIVEADRESS_ 9


//_____________________________________________________________________________________________
//_____________________________________________________________________________________________

byte data[3]; 
byte value[2];
int recoveredValue;
String var;


void setup() 
{
  Wire.begin();                    // join i2c bus (address optional for master)
  Serial.begin(9600);              // starts the serial communication 
}


void loop() 
{
  
  
  // request 3 bytes from slave device on adress 9
  Wire.requestFrom(_RECEIVEADRESS_, 3);         
  
  while (Wire.available())         //check if data is available 
  {       
    Serial.print("tab : ");  
    Serial.print('\t');
    for(byte i = 0; i < 3; i++)
    {
      data[i] = Wire.read();       // it assigne the data to the array
      Serial.print(data[i]);       // print the array  
      Serial.print('\t');
    }
  }
  
  //******************** DO STUFF ********************\\ 
  
  var = String(data[0]);
  value[0] = data [1];
  value[1] = data [2];
  recoveredValue = recoverIntFrom2Bytes(value);
  Serial.print("\nrecovery : ");  
  Serial.print(var);
  Serial.print(" = ");  
  Serial.println(recoveredValue);
  
  
  Serial.println();  
 
}


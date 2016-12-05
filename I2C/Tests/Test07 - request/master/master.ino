#include <Wire.h>
#include "i2cCommunication.h"

#define _SENDADRESS_ 8
#define _RECEIVEADRESS_ 9


//_____________________________________________________________________________________________
//_____________________________________________________________________________________________

byte data[3];


void setup() 
{
  Wire.begin();                    // join i2c bus (address optional for master)
  Serial.begin(9600);              // starts the erial communication 
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
  
  Serial.println();                // Print a new line 

//******************** DO STUFF ********************\\ 
 
}


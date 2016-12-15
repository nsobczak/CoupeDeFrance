#include <Wire.h>

void i2csend(int x)
{
  Wire.begin(); // joindre le bus i2c (adresse est optionnelle pour un maître)
  Wire.beginTransmission(4); // Commencer transmission vers l'esclave  #4
  Wire.write("x is ");       // Envoi de 5 octets (5 bytes)
  Wire.write(x);             // envoi d'un byte/octet (valeur numérique)  
  Wire.endTransmission();    // fin transmission
}

void setup()
{
}

byte x = 0;

void loop()
{
  i2csend(x);
  x++;
  delay(500); // delay de 500 ms
}


/*****************************************************************************/
//	Function:    Cpp file for ITG3200 
//  Hardware:    Grove - 3-Axis Digital Gyro
//	Arduino IDE: Arduino-1.0
//	Author:	 Frankie.Chu		
//	Date: 	 Jan 11,2013
//	Version: v1.0
//	by www.seeedstudio.com
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
/*******************************************************************************/

#include "spark_wiring.h"
#include "application.h"
#include "ITG3200.h"

/**********************************************************************/
/*Function: Read a byte with the register address of ITG3200.         */
/*Parameter:-uint8_t _register,the register address  of ITG3200 to read; */
/*Return:	-int8_t,the byte that is read from the register.		  */
int8_t ITG3200::read(uint8_t _register)
{
    int8_t data;
    Wire.beginTransmission(GYRO_ADDRESS);
    Wire.write(_register);
    Wire.endTransmission();
    Wire.requestFrom(GYRO_ADDRESS, 1);
    if(Wire.available() > 0)
    {
        data = Wire.read();
    }
	Wire.endTransmission();
    return data;
}
/*Function: Write a byte to the register of the MMA7660*/
void ITG3200::write(uint8_t _register, uint8_t _data)
{
	Wire.begin();
	Wire.beginTransmission(GYRO_ADDRESS);
	Wire.write(_register);   
	Wire.write(_data);
	Wire.endTransmission();
}

/**********************************************************************/
/*Function: Initialization for ITG3200.         					  */
void ITG3200::init()
{
	Wire.begin();
	write(ITG3200_PWR_M,0x80);//send a reset to the device
 	write(ITG3200_SMPL,0x00);//sample rate divider
 	write(ITG3200_DLPF,0x18);//+/-2000 degrees/s (default value)
}

int16_t ITG3200::read(uint8_t addressh, uint8_t addressl)
{
    int data, t_data;
 
    Wire.beginTransmission(GYRO_ADDRESS);
    Wire.write(addressh);
    Wire.endTransmission();
    Wire.requestFrom(GYRO_ADDRESS, 1);
    if(Wire.available() > 0)
    {
        t_data = Wire.read();
        data = t_data << 8;
    }
    Wire.beginTransmission(GYRO_ADDRESS);
    Wire.write(addressl);
    Wire.endTransmission();
	Wire.requestFrom(GYRO_ADDRESS, 1);
    if(Wire.available() > 0)
    {
        data |= Wire.read();
    }
    return data;
}
/*Function: Get the temperature from ITG3200 that with a on-chip*/
/*           temperature sensor.                                */
double ITG3200::getTemperature()
{
	int temp;
	double temperature;
	temp = read(ITG3200_TMP_H, ITG3200_TMP_L);
	temperature = 35+ ((double) (temp + 13200)) / 280;
	return(temperature);
}
/*Function: Get the contents of the registers in the ITG3200*/
/*          so as to calculate the angular velocity.        */
void ITG3200::getXYZ(int16_t *x,int16_t *y,int16_t *z)
{
	*x = read(ITG3200_GX_H,ITG3200_GX_L)+x_offset;
	*y = read(ITG3200_GY_H,ITG3200_GY_L)+y_offset;
	*z = read(ITG3200_GZ_H,ITG3200_GZ_L)+z_offset;
}
/*Function: Get the angular velocity and its unit is degree per second.*/
void ITG3200::getAngularVelocity(float *ax,float *ay,float *az)
{
	int16_t x,y,z;
	getXYZ(&x,&y,&z);
	*ax = x/14.375;
	*ay = y/14.375;
	*az = z;
}
void ITG3200::zeroCalibrate(unsigned int samples, unsigned int sampleDelayMS) 
{
  int16_t x_offset_temp;
  int16_t y_offset_temp;
  int16_t z_offset_temp;
  int16_t x,y,z;
  x_offset = 0;
  y_offset = 0;
  z_offset = 0;
  getXYZ(&x,&y,&z);//
  for (int i = 0;i < samples;i++){
    delay(sampleDelayMS);
    getXYZ(&x,&y,&z);
    x_offset_temp += x;
    y_offset_temp += y;
    z_offset_temp += z;
  }

  x_offset = abs(x_offset_temp)/samples;
  y_offset = abs(y_offset_temp)/samples;
  z_offset = abs(z_offset_temp)/samples;
  if(x_offset_temp > 0)x_offset = -x_offset;
  if(y_offset_temp > 0)y_offset = -y_offset;
  if(z_offset_temp > 0)z_offset = -z_offset;
}

/*****************************************************************************/
//	Function:    Header file for ITG3200 
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
#ifndef __ITG3200_H__
#define __ITG3200_H__


#define GYRO_ADDRESS 0x68
// ITG3200 Register Defines
#define ITG3200_WHO	0x00
#define	ITG3200_SMPL	0x15
#define ITG3200_DLPF	0x16
#define ITG3200_INT_C	0x17
#define ITG3200_INT_S	0x1A
#define	ITG3200_TMP_H	0x1B
#define	ITG3200_TMP_L	0x1C
#define	ITG3200_GX_H	0x1D
#define	ITG3200_GX_L	0x1E
#define	ITG3200_GY_H	0x1F
#define	ITG3200_GY_L	0x20
#define ITG3200_GZ_H	0x21
#define ITG3200_GZ_L	0x22
#define ITG3200_PWR_M	0x3E

class ITG3200
{
private:
	int8_t read(uint8_t _register);
	int16_t x_offset;
	int16_t y_offset;
	int16_t z_offset;
public:
	void init();
	int16_t read(uint8_t addressh, uint8_t addressl);
	void write(uint8_t _register, uint8_t _data);
	double getTemperature();
	void getXYZ(int16_t *x,int16_t *y,int16_t *z);
	void getAngularVelocity(float *ax,float *ay,float *az);
	void zeroCalibrate(unsigned int samples, unsigned int sampleDelayMS); 
};

#endif
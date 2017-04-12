/**
 *    \file CoupeDeFranceDetection.ino
 *    \brief code de la carte cateurs
 *    \author Nicolas Sobczak
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
#include "I2Cdev.h"
#include "MPU6050.h"

#define _CALCUL_OFFSET_ false
#define _NUMBER_OF_TESTS_FOR_OFFSET_   100

#define MPU6050_ADDRESS_AD0_LOW     0x68 // address pin low (GND), default for InvenSense evaluation board
#define MPU6050_ADDRESS_AD0_HIGH    0x69 // address pin high (VCC)

MPU6050 accelgyro_1(MPU6050_ADDRESS_AD0_LOW);
MPU6050 accelgyro_2(MPU6050_ADDRESS_AD0_HIGH);


int16_t ax, ay, az;
int16_t gx, gy, gz;
float AXoff, AYoff, AZoff, GXoff, GYoff, GZoff;
const int minVal = 265;
const int maxVal = 402;

#define LED_PIN 13
bool blinkState = false;


//_____________________________________________________________________________________________
//_____________________________________________________________________________________________
void setup() {
        // join I2C bus (I2Cdev library doesn't do this automatically)
        Wire.begin();

        Serial.begin(9600);

        // initialize device
        Serial.println("Initializing I2C devices...");
        accelgyro_1.initialize();
        // accelgyro_2.initialize();

        // accelgyro_1.setXAccelOffset(17401.43);
        // accelgyro_1.setYAccelOffset(-113.11);
        // accelgyro_1.setZAccelOffset(-2602.38 - 16384);
        // accelgyro_1.setXGyroOffset(51.44);
        // accelgyro_1.setYGyroOffset(350.57);
        // accelgyro_1.setZGyroOffset(177.36);

        // verify connection
        Serial.println("Testing device connections...");
        Serial.println(accelgyro_1.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
        // Serial.println(accelgyro_2.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

        // configure Arduino LED for
        pinMode(LED_PIN, OUTPUT);

        if (_CALCUL_OFFSET_)
        {
                AXoff = 0; AYoff = 0; AZoff = 0;
                GXoff = 0; GYoff = 0; GZoff = 0;
        }
}


void loop() {
        // read raw accel/gyro measurements from device
        // accelgyro_1.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

        // these methods (and a few others) are also available
        accelgyro_1.getAcceleration(&ax, &ay, &az);
        // accelgyro_1.getRotation(&gx, &gy, &gz);

        if (_CALCUL_OFFSET_)
        {
                calibrateOffsets();
                Serial.print("AXoff =\t"); Serial.println(AXoff);
                Serial.print("AYoff =\t"); Serial.println(AYoff);
                Serial.print("AZoff =\t"); Serial.println(AZoff);
                Serial.print("GXoff =\t"); Serial.println(GXoff);
                Serial.print("GYoff =\t"); Serial.println(GYoff);
                Serial.print("GZoff =\t"); Serial.println(GZoff);
        }


        // // === méthode 1 ===
        // int xAng = map(ax, minVal,maxVal, -90, 90); //Re-maps AcX from one range to another.
        // int yAng = map(ay, minVal,maxVal, -90, 90); //Re-maps AcY from one range to another.
        // int zAng = map(az, minVal,maxVal, -90, 90); //Re-maps AcZ from one range to another.
        //
        // int x = RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
        // int y = RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
        // int z = RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);
        //
        // Serial.print("AngleX= "); Serial.println(x);
        // Serial.print("AngleY= "); Serial.println(y);
        // Serial.print("AngleZ= "); Serial.println(z);
        // Serial.println("   ===   ");


        // === méthode 2 ===
        double arx = RAD_TO_DEG * atan(ax / sqrt(ay*ay + az*az));
        double ary = RAD_TO_DEG * atan(ay / sqrt(ax*ax + az*az));
        double arz = RAD_TO_DEG * atan(sqrt(ay*ay + ax*ax) / az);

        Serial.print("AngleX2= "); Serial.println(arx);
        Serial.print("AngleY2= "); Serial.println(ary);
        Serial.print("AngleZ2= "); Serial.println(arz);
        Serial.println("-----------------------------------------");


        // blink LED to indicate activity
        blinkState = !blinkState;
        digitalWrite(LED_PIN, blinkState);

        delay(1000);
}


//_____________________________________________________________________________________________
// offset récupéré par test, moyenne de 100 valeurs :
// 1 -  41,52
// 2 -  344,63
// 3 -  174,90
float convertGyroValue(int gValue, int offset, int gain)
{
        return (float) (gValue - offset)*gain;
}

void calibrateOffsets(void)
{
        //Count and average the first n values, defined by numberOfTests above..
        for(int i=0; i<_NUMBER_OF_TESTS_FOR_OFFSET_; i++)
        {
                accelgyro_1.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
                AXoff += ax;
                AYoff += ay;
                AZoff += az;
                GXoff += gx;
                GYoff += gy;
                GZoff += gz;

                //Uncomment these lines to show serial output...
                //sprintf(buf, "Test Number: %d \n\r", i);
                //SERIAL_UartPutString(buf);
                //sprintf(buf, "AX:%d, AY:%d, AZ:%d || GX:%d, GY:%d, GZ:%d,\t", CAX,CAY,CAZ,CGX,CGY,CGZ);
                //SERIAL_UartPutString(buf);
                //SERIAL_UartPutString("\n\r");
                delay(50);
        }

        AXoff = AXoff/_NUMBER_OF_TESTS_FOR_OFFSET_;
        AYoff = AYoff/_NUMBER_OF_TESTS_FOR_OFFSET_;
        AZoff = AZoff/_NUMBER_OF_TESTS_FOR_OFFSET_;
        GXoff = GXoff/_NUMBER_OF_TESTS_FOR_OFFSET_;
        GYoff = GYoff/_NUMBER_OF_TESTS_FOR_OFFSET_;
        GZoff = GZoff/_NUMBER_OF_TESTS_FOR_OFFSET_;

        Serial.println("\nTest finished, offset values are shown below\n\r");
        Serial.print("\n\rAXoff:\t"); Serial.println((int)AXoff);
        Serial.print("\n\rAYoff:\t"); Serial.println((int)AYoff);
        Serial.print("\n\rAZoff:\t"); Serial.println((int)AZoff);
        Serial.print("\n\rGXoff:\t"); Serial.println((int)GXoff);
        Serial.print("\n\rGYoff:\t"); Serial.println((int)GYoff);
        Serial.print("\n\rGZoff:\t"); Serial.println((int)GZoff);
}

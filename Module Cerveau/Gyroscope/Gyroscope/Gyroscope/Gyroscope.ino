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

#define MPU6050_ADDRESS_AD0_LOW     0x68 // address pin low (GND), default for InvenSense evaluation board
#define MPU6050_ADDRESS_AD0_HIGH    0x69 // address pin high (VCC)

MPU6050 accelgyro_1(MPU6050_ADDRESS_AD0_LOW);
MPU6050 accelgyro_2(MPU6050_ADDRESS_AD0_HIGH);


int16_t ax, ay, az;
int16_t gx, gy, gz_1, gz_2;
float GXoff, GYoff, GZoff;
int offsetCompteur;

#define LED_PIN 13
bool blinkState = false;


//_____________________________________________________________________________________________
//_____________________________________________________________________________________________
void setup() {
        // join I2C bus (I2Cdev library doesn't do this automatically)
        Wire.begin();

        // initialize serial communication
        // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
        // it's really up to you depending on your project)
        Serial.begin(9600);

        // initialize device
        Serial.println("Initializing I2C devices...");
        accelgyro_1.initialize();
        accelgyro_1.setFullScaleGyroRange(0);
        // accelgyro_2.initialize();

        // verify connection
        Serial.println("Testing device connections...");
        Serial.println(accelgyro_1.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
        // Serial.println(accelgyro_2.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

        // configure Arduino LED for
        pinMode(LED_PIN, OUTPUT);

        offsetCompteur = 0;
        GXoff = 0;
        GYoff = 0;
        GZoff = 0;
}


void loop() {
        // read raw accel/gyro measurements from device
        // accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

        // these methods (and a few others) are also available
        // accelgyro_1.getAcceleration(&ax, &ay, &az);
        // accelgyro_1.getRotation(&gx, &gy, &gz_1);

        gx = accelgyro_1.getRotationX();
        gy = accelgyro_1.getRotationY();
        gz_1 = accelgyro_1.getRotationZ();
        // gz_2 = accelgyro_2.getRotationZ();

        if (_CALCUL_OFFSET_) {
                Serial.print("offsetCompteur =\t");
                Serial.println(offsetCompteur);
                if (offsetCompteur < 100)
                {
                        GXoff += (float)gx;
                        GYoff += (float)gy;
                        GZoff += (float)gz_1;
                        offsetCompteur += 1;
                }
                else
                {
                        GXoff /= offsetCompteur;
                        GYoff /= offsetCompteur;
                        GZoff /= offsetCompteur;
                        Serial.print("GXoff =\t");
                        Serial.println(GXoff);
                        Serial.print("GYoff =\t");
                        Serial.println(GYoff);
                        Serial.print("GZoff =\t");
                        Serial.println(GZoff);
                }
        }



        // display tab-separated accel/gyro x/y/z values
        Serial.println("\nrotation =\t");
        // Serial.print(ax); Serial.print("\t");
        // Serial.print(ay); Serial.print("\t");
        // Serial.print(az); Serial.print("\t");
        Serial.print(gx); Serial.print("\t");
        Serial.println(convertGyroValue(gx, 41.52, 32768/250));//131.07 is just 32768/250 to get us our 1deg/sec value
        Serial.print(gy); Serial.print("\t");
        Serial.println(convertGyroValue(gy, 344.63, 32768/250));
        Serial.print(gz_1); Serial.print("\t");
        Serial.println(convertGyroValue(gz_1, 174.90, 32768/250));

        // Serial.print("\ngz_2 =\t");
        // Serial.println(gz_2);

        // blink LED to indicate activity
        blinkState = !blinkState;
        digitalWrite(LED_PIN, blinkState);

        delay(100);
}

// offset :
// 1 -  41,52
// 2 -  344,63
// 3 -  174,90
float convertGyroValue(int gValue, int offset, int gain)
{
        return (float) (gValue - offset)*gain;
}

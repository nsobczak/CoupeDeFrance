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
int16_t gx, gy, gz, gz_2;
float AXoff, AYoff, AZoff, GXoff, GYoff, GZoff, roll, pitch, yaw;

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
        accelgyro_1.setFullScaleAccelRange(0);
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

        AXoff = 0;
        AYoff = 0;
        AZoff = 0;
        GXoff = 0;
        GYoff = 0;
        GZoff = 0;
}


void loop() {
        // read raw accel/gyro measurements from device
        accelgyro_1.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);


        // accelgyro_1.readNormalizeGyro();

        // these methods (and a few others) are also available
        // accelgyro_1.getAcceleration(&ax, &ay, &az);
        // accelgyro_1.getRotation(&gx, &gy, &gz);

        // gz_2 = accelgyro_2.getRotationZ();

        if (_CALCUL_OFFSET_) {
                calibrateOffsets();
                Serial.print("AXoff =\t");
                Serial.println(AXoff);
                Serial.print("AYoff =\t");
                Serial.println(AYoff);
                Serial.print("AZoff =\t");
                Serial.println(AZoff);
                Serial.print("GXoff =\t");
                Serial.println(GXoff);
                Serial.print("GYoff =\t");
                Serial.println(GYoff);
                Serial.print("GZoff =\t");
                Serial.println(GZoff);
        }



        // display tab-separated accel/gyro x/y/z values
        Serial.println("\nrotation =\t");
        // Serial.print(ax); Serial.print("\t");
        // Serial.print(ay); Serial.print("\t");
        // Serial.print(az); Serial.print("\t");
        // Serial.print(ax); Serial.print("\t");
        // Serial.println(convertGyroValue(ax, 17401.43, 16384.0)); //16384 is just 32768/2 to get our 1G value
        // Serial.print(ay); Serial.print("\t");
        // Serial.println(convertGyroValue(ay, -113.11, 16384.0));
        // Serial.print(az); Serial.print("\t");
        // Serial.println(convertGyroValue(az, -2602.38 - 16384, 16384.0)); //z => remove 1G before dividing
        // Serial.print(gx); Serial.print("\t");
        // Serial.println(convertGyroValue(gx, 51.44, 131.07)); //131.07 is just 32768/250 to get us our 1deg/sec value
        // Serial.print(gy); Serial.print("\t");
        // Serial.println(convertGyroValue(gy, 350.57, 131.07));
        // Serial.print(gz); Serial.print("\t");
        // Serial.println(convertGyroValue(gz, 177.36, 131.07));

        roll = atan2(gy, gz) * 180/PI;        //rotation en x
        pitch = atan2(gx, sqrt(gy*gy + gz*gz)) * 180/PI; //rotation en y

        //Ignore the gyro if our angular velocity does not meet our threshold
        if (gz > 1 || gz < -1) {
                gz /= 100;
                yaw += gz;
        }

        //Keep our angle between 0-359 degrees
        if (yaw < 0)
                yaw += 360;
        else if (yaw > 359)
                yaw -= 360;

        Serial.print("roll = \t");
        Serial.println(roll);
        Serial.print("pitch = \t");
        Serial.println(pitch);
        Serial.print("yaw = \t");
        Serial.println(yaw);

        // blink LED to indicate activity
        blinkState = !blinkState;
        digitalWrite(LED_PIN, blinkState);

        delay(1000);
}

// offset :
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

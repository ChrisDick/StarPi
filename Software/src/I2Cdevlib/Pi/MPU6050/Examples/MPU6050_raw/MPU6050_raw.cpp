// I2C device class (I2Cdev) demonstration Arduino sketch for MPU6050 class
// 10/7/2011 by Jeff Rowberg <jeff@rowberg.net>
// Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib
//
// Changelog:
//      2013-05-08 - added multiple output formats
//                 - added seamless Fastwire support
//      2011-10-07 - initial release

/* ============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2011 Jeff Rowberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "MPU6050.h"
#include <math.h>
#include <iostream>
using namespace std;

// uncomment "OUTPUT_READABLE_ACCELGYRO" if you want to see a tab-separated
// list of the accel X/Y/Z and then gyro X/Y/Z values in decimal. Easy to read,
// not so easy to parse, and slow(er) over UART.
#define OUTPUT_READABLE_ACCELGYRO

// uncomment "OUTPUT_BINARY_ACCELGYRO" to send all 6 axes of data as 16-bit
// binary, one right after the other. This is very fast (as fast as possible
// without compression or data loss), and easy to parse, but impossible to read
// for a human.
//#define OUTPUT_BINARY_ACCELGYRO


int main( int argsc, char** argsv) {
    // class default I2C address is 0x68
    // specific I2C addresses may be passed as a parameter here
    // AD0 low = 0x68 (default for InvenSense evaluation board)
    // AD0 high = 0x69
    MPU6050 accelgyro;
    //MPU6050 accelgyro(0x69); // <-- use for AD0 high

    int16_t ax, ay, az;
    //int16_t gx, gy, gz;

    // initialize device
    accelgyro.initialize();
    accelgyro.setAccelXSelfTest(false);
    accelgyro.setAccelYSelfTest(false);
    accelgyro.setAccelZSelfTest(false);
    
    // verify connection
    bool test = accelgyro.testConnection();
#ifdef DEBUG_OUTPUT
    cout << test;
    cout << "\n\r";
#endif
    // use the code below to change accel/gyro offset values
    int16_t x_offset = (accelgyro.getXAccelOffset());
    int16_t y_offset = (accelgyro.getYAccelOffset());
    int16_t z_offset = (accelgyro.getZAccelOffset());
    // -76	-2359	1688	0	0	0
#ifdef DEBUG_OUTPUT
    cout << x_offset; // -76
    cout << "\n\r";
    cout << y_offset; // -2359
    cout << "\n\r";
    cout << z_offset; // 1688
    cout << "\n\r";
#endif
  //  cout << (accelgyro.getXGyroOffset()); // 0
  //  cout << "\n\r";
  //  cout << (accelgyro.getYGyroOffset()); // 0
  //  cout << "\n\r";
  //  cout << (accelgyro.getZGyroOffset()); // 0
  //  cout << "\n\r";
    //accelgyro.setXGyroOffset(220);
    //accelgyro.setYGyroOffset(76);
    //accelgyro.setZGyroOffset(-85);
    //accelgyro.getXAccelOffset()); // -76
    //accelgyro.getYAccelOffset()); // -2359
    //accelgyro.getZAccelOffset()); // 1688
    //accelgyro.getXGyroOffset()); // 0
    //accelgyro.getYGyroOffset()); // 0
    //accelgyro.getZGyroOffset()); // 0
    
    //int16_t count = 0;

   

    // read raw accel/gyro measurements from device
    //accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    //count = accelgyro.getAcceleration(&ax, &ay, &az);
    //accelgyro.getRotation(&gx, &gy, &gz);
    // these methods (and a few others) are also available
    accelgyro.getAcceleration(&ax, &ay, &az);
    //accelgyro.getRotation(&gx, &gy, &gz);

    float x_axis = (((float)(ax + x_offset) / 32767.0) * (2*9.81));
    float y_axis = (((float)(ay + y_offset) / 32767.0) * (2*9.81));
    float z_axis = (((float)(az + z_offset) / 32767.0) * (2*9.81));
#ifdef DEBUG_OUTPUT    
    #ifdef OUTPUT_READABLE_ACCELGYRO
        //cout << count;
        cout << " Bytes read. \n\r";
        // display tab-separated accel/gyro x/y/z values
        cout << "ax:";
        cout << x_axis; 
        cout << "\tay:";
        cout << y_axis; 
        cout << ("\taz:");
        cout << z_axis; 
        /*
        cout << ("\tgx:");
        cout << (gx); 
        cout << ("\tgy:");
        cout << (gy); 
        cout << ("\tgz:");
        cout << (gz);
        cout << "\n\r";
        */
    #endif
#endif
    #ifdef OUTPUT_BINARY_ACCELGYRO
        Serial.write((uint8_t)(ax >> 8)); Serial.write((uint8_t)(ax & 0xFF));
        Serial.write((uint8_t)(ay >> 8)); Serial.write((uint8_t)(ay & 0xFF));
        Serial.write((uint8_t)(az >> 8)); Serial.write((uint8_t)(az & 0xFF));
        Serial.write((uint8_t)(gx >> 8)); Serial.write((uint8_t)(gx & 0xFF));
        Serial.write((uint8_t)(gy >> 8)); Serial.write((uint8_t)(gy & 0xFF));
        Serial.write((uint8_t)(gz >> 8)); Serial.write((uint8_t)(gz & 0xFF));
    #endif
    
    float elevation = asin((0-y_axis)/9.81);
    cout << ((elevation/M_PI) * 180 );
    cout << " degrees\n\r";
}

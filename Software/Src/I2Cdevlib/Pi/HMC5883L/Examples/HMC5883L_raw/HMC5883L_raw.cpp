// I2C device class (I2Cdev) demonstration Arduino sketch for HMC5883L class
// 10/7/2011 by Jeff Rowberg <jeff@rowberg.net>
// Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib
//
// Changelog:
//     2013-05-04 - Added Heading Calculation in degrees
//     2011-10-07 - initial release

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

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
//#include "Wire.h"
#include <wiringPiI2C.h>
// I2Cdev and HMC5883L must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
//#include "I2Cdev.h"
#include "HMC5883L.h"
#include <math.h>
// class default I2C address is 0x1E
// specific I2C addresses may be passed as a parameter here
// this device only supports one I2C address (0x1E)
#include <iostream>
using namespace std;

  
int main (int argscc, char** argsv)
{
    int result = 0;
    HMC5883L mag; 
    int16_t mx = 0; 
    int16_t my = 0; 
    int16_t mz = 0;
    bool test = false;

    mag.Initialize();
    test = mag.testConnection() ;
    cout << test;
    cout << "\n\r";
    mag.setMode(HMC5883L_MODE_CONTINUOUS);
    while (1)
    {
        mag.GetHeading(&mx, &my, &mz);
        
        cout << "mx:";
        cout << mx;
        cout << " my:";
        cout << my;
        cout << " mz:";
        cout << mz;
        float heading = atan2(my, mx);
        if(heading < 0)
        {
            heading += 2 * M_PI;
        }
        result = (int)heading;
        cout << " heading:";
        cout << (heading * ( 180 / M_PI ));
        cout << "\n\r";
    }
    return result;
}






    //setup();
// = new HMC5883L()
    //result = (int)loop();
//void setup ( void);
//float loop ( void);

//void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    //Wire.begin();

    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
//    Serial.begin(38400);

    // initialize device
//    Serial.println("Initializing I2C devices...");
 //   mag.initialize();

    // verify connection
//    Serial.println("Testing device connections...");
//    Serial.println(mag.testConnection() ? "HMC5883L connection successful" : "HMC5883L connection failed");

    // configure Arduino LED for
//    pinMode(LED_PIN, OUTPUT);
//}

//float loop() {
    // read raw heading measurements from device
    //mag.getHeading(&mx, &my, &mz);

    // display tab-separated gyro x/y/z values
//    Serial.print("mag:\t");
 //   Serial.print(mx); Serial.print("\t");
 //   Serial.print(my); Serial.print("\t");
 //   Serial.print(mz); Serial.print("\t");
    
// To calculate heading in degrees. 0 degree indicates North
    // float heading = atan2(my, mx);
    // if(heading < 0)
      // heading += 2 * M_PI;
  //  Serial.print("heading:\t");
  //  Serial.println(heading * 180/M_PI);

    // blink LED to indicate activity
  //  blinkState = !blinkState;
  //  digitalWrite(LED_PIN, blinkState);
//return heading;
 // }

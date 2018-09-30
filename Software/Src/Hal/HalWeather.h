/**
HalWeather provides all weather measurements. 

Author and copyright of this file:
Chris Dick, 2018

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef HALWEATHER_H
#define HALWEATHER_H

#include <stdint.h>
#include "Runnable.h"
#include "BME280.h"

/** HalWeather
 * - Class for handling all weather measurments
 */
class HalWeather: public Runnable 
{
    public:
        BME280 bme280;
    /** Constructor
     */
        HalWeather( void );
    /** Initialise the GPSD connection
     * @return bool Initialisation status  
     */
        bool Init( void );
    /** Runs the filter
     */
        void Run( void );
    /** Get the Temperature
     * @return float Temperature 
     */
        float getTemperature( void );
    /** Get the Pressure
     * @return float Pressure 
     */
        float getPressure( void );
    /** Get the Humidity
     * @return float Humidity 
     */
        float getHumidity( void );
        
        static HalWeather Weather; /**< Only one is required */
        
    private:
        float Temperature;
        float Humidity;
        float Pressure;
};

#endif /* HALWEATHER_H */

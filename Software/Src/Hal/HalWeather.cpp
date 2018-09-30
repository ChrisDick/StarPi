/*
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
#include "HalWeather.h"
#include "Config.h"

#ifdef TIMING
#include "GPIO.h"
#endif

HalWeather  HalWeather::Weather;

/* HalGps
 *  Constructor
 */
HalWeather::HalWeather( void )
{

}

/* HalWeather Init
 *  Initialise the GPSD connection
 * @return bool Initialisation status  
 */
bool HalWeather::Init( void )
{
    bool result = false;
//    if ( bme280.testConnection())
    {
        bme280.initialize();
        bme280.setHumidityOversampling( OVERSAMPLING_X1 );
        bme280.setTemperatureOversampling( OVERSAMPLING_X1 );
        bme280.setPressureOversampling( OVERSAMPLING_X1 );
        bme280.setMode( NORMAL_MODE );
        bme280.setStandbyTime( TIME_1000 );
        bme280.setFilter( FILTER_OFF );
        result = true;
    }
#ifdef TIMING
    GPIO::gpio.SetupOutput( HAL_WEATHER_PIN );
    GPIO::gpio.SetPullMode( HAL_WEATHER_PIN , PULL_UP );
#endif
    return result;
}

/* HalWeather Run
 *  Check for any new data and update time and location
 */
void HalWeather::Run( void )
{
    Temperature = bme280.getTemperature();   
    Pressure = bme280.getPressure();
    Humidity = bme280.getHumidity();
}

/* Get the Temperature
 * @return float Temperature 
 */
float HalWeather::getTemperature( void )
{
    return Temperature;
}
/* Get the Pressure
 * @return float Pressure 
 */
float HalWeather::getPressure( void )
{
    return Pressure;
}
/* Get the Humidity
 * @return float Humidity 
 */
float HalWeather::getHumidity( void )
{
    return Humidity;
}

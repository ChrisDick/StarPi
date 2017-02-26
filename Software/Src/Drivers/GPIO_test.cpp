
#include <stdio.h>
#include "GPIO.h"
/* 
 magnetic model test program

 */

int main()
{
    uint8_t index = 0;
    uint16_t count = 0;
    GPIO::gpio.Init();
    for ( index = 0; index < PIN_MAX ; index++ )
    {
        GPIO::gpio.SetupOutput((pin_name_t)index);
    }
    printf( "toggling pins" );
    while (1 )
    {
        count++;
        for ( index = 0; index < PIN_MAX ; index++ )
        {
            GPIO::gpio.SetPinState((pin_name_t)index, !GPIO::gpio.GetPinState((pin_name_t)index));
        }
    }
    printf("done");
  
    return 0;  
}
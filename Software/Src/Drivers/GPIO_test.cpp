
#include <stdio.h>
#include "GPIO.h"
/* 
 magnetic model test program

 */

int main()
{
    uint8_t index = 0
    uint16_t count = 0;
    for ( index = 0; index < PIN_MAX ; index++ )
    {
        SetupOutput(index);
    }
    printf( "toggling pins" );
    while (count < 10000 )
    {
        count++;
        for ( index = 0; index < PIN_MAX ; index++ )
        {
            GPIOSetPinState(index, !GPIOGetPinState(index));
        }
    }
    printf("done");
  
    return 0;  
}
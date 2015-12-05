
#include <stdio.h>
#include "MagModel.h"
/* 
 magnetic model test program

 */

int main()
{
    float MagneticDeclination = 0;
    MagModel MagCorrect;
    float Latitude = 0;
    float Logitude = 0;
    float HieghtAboveGround = 0;
    int Day = 29;
    int Month = 11;
    int Year = 2015;
    
    MagCorrect.MagModelSetParams( Latitude, Logitude, HieghtAboveGround, Day, Month, Year );
    
    MagneticDeclination = MagCorrect.MagModelGetDeclination();
    
    printf ("Magnatic Declination at Latitude %f, Longitude %f, on %d/%d/%d = %f", Latitude, Logitude, Day, Month, Year, MagneticDeclination );
    
    return 0;
}

/**  
 * World Magnetic Model Point Calculation Module.
 * C++ Wrapper for C library from http://www.ngdc.noaa.gov/
 * Note: The C library and header have been edited to remove passing 
 * arrays of undefined size.
 * Usage:
 * Set location, hieght above sea level and date to calculate model.
 * Get required parameter.
 * Only get function for magnetic declination currently supported.
 * Get functions could be added for the following
 * F, Total Intensity
 * H, Horizontal Intensity
 * X, Grid variation x axis ?? 
 * Y, Grid Variation y axis ??
 * Z, Vertical Intensity
 * Magnetic Inclination
 * See http://www.ngdc.noaa.gov/geomag/WMM/DoDWMM.shtml for more details on the model used.
 * The program expects the file WMM.COF to be in the same directory as the executable was called from. 
 */
#ifndef MAGMODEL_H
#define MAGMODEL_H

#include "GeomagnetismHeader.h"


class MagModel
{ 
 
    public:
    /** Constructor
     *
     */
        MagModel() {};
    /** MagModelSetParams
     * @param Latitude - in decimal degrees.
     * @param Logitude - in decimal degrees, East longitude positive, West negative. 
     * @param HieghtAboveGround - hieght above mean sea level. 
     * @param Day - current date.
     * @param Month - current month.
     * @param Year - current year.
     */
        void MagModelSetParams( float Latitude, float Logitude, float HieghtAboveGround, int Day, int Month, int Year );

     /** Get Magnetic Declination at Location
      * @return float Magnetic Declination in degrees
      */
        float MagModelGetDeclination( void );

        MAGtype_GeoMagneticElements GeoMagneticElements; /**< storage for calculated params */ 
        MAGtype_GeoMagneticElements Errors; /**< storage for errors in calculations */
   
};

#endif /* MAGMODEL_H */
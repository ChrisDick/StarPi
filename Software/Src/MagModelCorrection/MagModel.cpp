/* 
 * World Magnetic Model Point Calculation Module.
 *
 * Set location, hieght above sea level and date to calculate model.
 * Get required parameter, only magnetic declination currently supported.
 * The program expects the file WMM.COF to be in the same directory. 
 */



#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "GeomagnetismHeader.h"
#include "EGM9615.h"

#include "MagModel.h"

/* Set Magnetic Model User data
 * Once set all params will bw recalculated.
 * @param Latitude - in decimal degrees.
 * @param Logitude - in decimal degrees, East longitude positive, West negative. 
 * @param HieghtAboveGround - hieght above mean sea level. 
 * @param Day - current date.
 * @param Month - current month.
 * @param Year - current year.
 */
void MagModel::MagModelSetParams( float Latitude, float Logitude, float HieghtAboveGround, int Day, int Month, int Year )
{
    MAGtype_MagneticModel * MagneticModels[1];
    MAGtype_MagneticModel *TimedMagneticModel = NULL;
    MAGtype_Ellipsoid Ellip;
    MAGtype_CoordSpherical CoordSpherical;
    MAGtype_CoordGeodetic CoordGeodetic;
    MAGtype_Date UserDate;
    MAGtype_Geoid Geoid;
    char ans[20];
    char filename[] = "WMM.COF";
    int NumTerms, nMax = 0;
    int epochs = 1;
    /* Memory allocation */

    if(!MAG_robustReadMagModels(filename, &MagneticModels, epochs)) {
        printf("\n WMM.COF not found.\n ");
    }
    else
    {
        if(nMax < MagneticModels[0]->nMax) nMax = MagneticModels[0]->nMax;
        NumTerms = ((nMax + 1) * (nMax + 2) / 2);
        TimedMagneticModel = MAG_AllocateModelMemory(NumTerms); /* For storing the time modified WMM Model parameters */
        if(MagneticModels[0] == NULL || TimedMagneticModel == NULL)
        {
            MAG_Error(2);
        }
        MAG_SetDefaults(&Ellip, &Geoid); /* Set default values and constants */
        /* Check for Geographic Poles */
        //??
        /* Set EGM96 Geoid parameters */
        Geoid.GeoidHeightBuffer = GeoidHeightBuffer;
        Geoid.Geoid_Initialized = 1;
        /* Set EGM96 Geoid parameters END */
        
        CoordGeodetic.phi = Latitude;
        CoordGeodetic.lambda = Logitude;
        // CoordGeodetic->HeightAboveEllipsoid = 
        Geoid.UseGeoid = 1;
        CoordGeodetic.HeightAboveGeoid = HieghtAboveGround;
        MAG_ConvertGeoidToEllipsoidHeight(&CoordGeodetic, &Geoid);
        //MagneticDate->DecimalYear = 
        UserDate.Month = Month;
        UserDate.Day = Day;
        UserDate.Year = Year;
        MAG_DateToYear(&UserDate, ans);
        
        
        MAG_GeodeticToSpherical(Ellip, CoordGeodetic, &CoordSpherical); /*Convert from geodetic to Spherical Equations: 17-18, WMM Technical report*/
        MAG_TimelyModifyMagneticModel(UserDate, MagneticModels[0], TimedMagneticModel); /* Time adjust the coefficients, Equation 19, WMM Technical report */
        MAG_Geomag(Ellip, CoordSpherical, CoordGeodetic, TimedMagneticModel, &GeoMagneticElements); /* Computes the geoMagnetic field elements and their time change*/
        MAG_CalculateGridVariation(CoordGeodetic, &GeoMagneticElements);
        MAG_WMMErrorCalc(GeoMagneticElements.H, &Errors);
        //MAG_PrintUserDataWithUncertainty(GeoMagneticElements, Errors, CoordGeodetic, UserDate, TimedMagneticModel, &Geoid); /* Debug Print the results */
    } 
     
    MAG_FreeMagneticModelMemory(TimedMagneticModel);
    MAG_FreeMagneticModelMemory(MagneticModels[0]);
    
}

/* Get Magnetic Declination at Location
* @return float Magnetic Declination in degrees
*/
float MagModel::MagModelGetDeclination( void )
{
    return GeoMagneticElements.Decl;
}
#ifndef TELESCOPE_MANAGER_H
#define TELESCOPE_MANAGER_H

#include <stdint.h>
#include "Runnable.h"
#include "CelestrialConverter.h"
#include "HalAccelerometer.h"
#include "HalCompass.h"

/** TelescopeManager
 * Class to manage the functionality of the telescope.
 */
class TelescopeManager : public Runnable 
{
    
        CelestrialConverter Calculator;
    public:
    /** constructor
     */
        TelescopeManager();
        
    /** main run function of the telescope manager
    */
        void Run( void );
    /** interface to set the target
     * @param Ra
     * @param Dec     
     */
        static void SetGotoTarget(float Ra, float Dec);

    /** Export the RightAscension and Declination
     */
        static void GetRaDec ( float* Ra, float* Dec );
        static float RightAscension;         /**< Right ascension */
        static float Declination;            /**< Declination */

    /** only one instance of this class is required.
     */
        static TelescopeManager Telescope;
        
    private:
        
        static float TargetRightAscension;   /**< Target right ascension */
        static float TargetDeclination;      /**< Target declination */
};

#endif /* TELESCOPE_MANAGER_H */
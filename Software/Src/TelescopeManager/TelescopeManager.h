#ifndef TELESCOPE_MANAGER_H
#define TELESCOPE_MANAGER_H

#include <stdint.h>
#include "Runnable.h"
#include "CelestrialConverter.h"

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
        static void SetGotoTarget(double Ra, double Dec);

    /** Export the RightAscension and Declination
     */
        static void GetRaDec ( double* Ra, double* Dec );
        static double RightAscension;         /**< Right ascension */
        static double Declination;            /**< Declination */

    /** only one instance of this class is required.
     */
        static TelescopeManager Telescope;
        
       // static void testCalculator ( void );
    private:
        
        static double TargetRightAscension;   /**< Target right ascension */
        static double TargetDeclination;      /**< Target declination */
};

#endif /* TELESCOPE_MANAGER_H */
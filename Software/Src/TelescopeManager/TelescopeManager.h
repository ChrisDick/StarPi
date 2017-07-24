/**
Telescope manager is a higher level system manager

Author and copyright of this file:
Chris Dick, 2015

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
        
    /** Telescope initialisation
     */
        void TelescopeManagerInit( void );

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
        static bool HeadingCorrected;  
        static double TargetRightAscension;   /**< Target right ascension */
        static double TargetDeclination;      /**< Target declination */
        
        static void iau_calc( CC_ANGLES_T* Angles, float UnixTime);
};

#endif /* TELESCOPE_MANAGER_H */
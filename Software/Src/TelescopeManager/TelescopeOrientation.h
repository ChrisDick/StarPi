#ifndef TELESCOPEORIENTATION_H
#define TELESCOPEORIENTATION_H

#include <stdint.h>
#include "Runnable.h"

/** TelescopeOrientation
 * - Class to provide use to the magnetometer
 */
class TelescopeOrientation: public Runnable {
    public:
    /** Constructor
     */
        TelescopeOrientation( void );
    
    /** Initialise the Orientation
     * @return bool Initialisation status  
     */
        bool TelescopeOrientationInit( void );
    /** Runs the filter
     */
        void Run( void );
    /** Get the heading of the Telescope
     * @return double heading 
     */
        float TelescopeOrientationGetHeading( void );
    /** Get the Pitch of the Telescope
     */
        float TelescopeOrientationGetPitch( void );
        
        static TelescopeOrientation Orient;      /**< Only one is required */  
};

#endif /* TELESCOPEORIENTATION_H */

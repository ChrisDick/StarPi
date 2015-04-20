#ifndef TELESCOPE_MANAGER_H
#define TELESCOPE_MANAGER_H

#include <stdint.h>

/** TelescopeManager
 * Class to manage the functionality of the telescope.
 */
class TelescopeManager {

    public:
    /** constructor
     */
        TelescopeManager();
        
    /** main run function of the telescope manager
    */
        void TelescopeManagerRun();
    /** interface to set the target
     * @param ra
     * @param dec     
     */
        static void set_goto_target(float ra, float dec);

    private:
    /** only one instance of this class is required.
     */
        static TelescopeManager telescope;
};

#endif /* TELESCOPE_MANAGER_H */
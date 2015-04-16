#ifndef TELESCOPE_MANAGER_H
#define TELESCOPE_MANAGER_H

#include <stdint.h>

/** TelescopeManager
 * Class to manage the functionality of the telescope.
 */
class TelescopeManager {

    public:
    TelescopeManager();
    static void set_goto_target(float ra, float dec);

    private:
    static TelescopeManager telescope;
};

#endif /* TELESCOPE_MANAGER_H */
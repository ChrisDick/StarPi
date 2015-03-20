#include <stdint.h>

#include "Runnable.h"

/* ------Public constants-------------------------------------------*/

/* 
    The maximum number of tasks required at any one time
    during the execution of the program
    MUST BE ADJUSTED FOR EACH NEW PROJECT
*/
#define SCH_MAX_TASKS 3

class TTC_Sched
{
private:
    Runnable * tasks[SCH_MAX_TASKS];

public:
    uint8_t add_task(Runnable * new_task);
    uint8_t delete_task(const uint8_t index);
    void    dispatch_tasks(void);
    void    update_tasks(void);
    
    virtual void    init(void)  = 0;
    virtual void    start(void) = 0;

    static const uint8_t RETURN_ERROR  = 0;
    static const uint8_t RETURN_NORMAL = 255;
};


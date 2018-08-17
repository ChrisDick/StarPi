#include <stdint.h>

#include "Runnable.h"

/* ------Public constants-------------------------------------------*/

/* 
    MUST BE ADJUSTED FOR EACH NEW PROJECT
    When a new task is added to a project, a rebuild of the scheduler is required.
    ToDo - change this to a const?
*/
#define SCH_MAX_TASKS 5 /**< The maximum number of tasks required at any one time during the execution of the program*/

/** Class for the scheduler 
 */
class TTC_Sched
{

protected:
    Runnable * Tasks[SCH_MAX_TASKS]; /**< task list */

public:
/** Causes a task (function) to be executed at regular intervals
 * or after a user-defined delay
 *
 * pFunction - The name of the function which is to be scheduled.
 * delay      - The interval (TICKS) before the task is first executed
 * period      - If 'PERIOD' is 0, the function is only called once,
 *
 * RETURN VALUE:
 * 
 * Returns the position in the task array at which the task has been
 * added.  If the return value is SCH_MAX_TASKS then the task could
 * not be added to the array (there was insufficient space).  If the
 * return value is < SCH_MAX_TASKS, then the task was added
 * successfully.
 */
    uint8_t AddTask( Runnable * NewTask );
/** Removes a task from the scheduler.  Note that this does
 * *not* delete the associated function from memory:
 * it simply means that it is no longer called by the scheduler.
 * @param Index The task Index.  Provided by TTC_Sched::add_task().
 * @return RETURN VALUE:  RETURN_ERROR or RETURN_NORMAL
 */
    uint8_t DeleteTask(const uint8_t Index);
/** This is the 'dispatcher' function.  When a task (function)
 * is due to run, TTC_Sched::dispatch_tasks() will run it.
 * This function must be called (repeatedly) from the main loop.
 */
    void DispatchTasks(void);
/** This is the scheduler ISR.  It is called at a rate
 * determined by the timer settings in TTC_Sched::init().
 * This version is triggered by Timer 0 interrupts.
 */
    void UpdateTasks(void);    
/** virtual function from platform specific implementation 
 */
    virtual void Init(void)  = 0;
/** virtual function from platform specific implementation 
 */
    virtual void Start(void) = 0;

    static const uint8_t RETURN_ERROR  = 255;   /**< return error code */
    static const uint8_t RETURN_NORMAL = 0; /**< return normal code */
};


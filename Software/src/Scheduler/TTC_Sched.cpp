#include "TTC_Sched.h"

/*
TTC_Sched::add_task()

Causes a task (function) to be executed at regular intervals
or after a user-defined delay

pFunction - The name of the function which is to be scheduled.
delay      - The interval (TICKS) before the task is first executed
period      - If 'PERIOD' is 0, the function is only called once,

RETURN VALUE:

Returns the position in the task array at which the task has been
added.  If the return value is SCH_MAX_TASKS then the task could
not be added to the array (there was insufficient space).  If the
return value is < SCH_MAX_TASKS, then the task was added
successfully.
*/
uint8_t    TTC_Sched::add_task(Runnable * new_task)
{
    uint8_t index = 0;

    // First find a gap in the array (if there is one)
    while ( (0 != this->tasks[index]) && (index < SCH_MAX_TASKS))
    {
        index++;
    }

    // Check if we reached the end of the list.
    if (index == SCH_MAX_TASKS)
    {
        // Task list is full
        // return an error code
        return SCH_MAX_TASKS;
    }

    // If we're here, there is a space in the task array
    this->tasks[index] = new_task;

    // make the task not runnable (reset runnable flag).
    new_task->set_runnable(0);

    // return position of task (to allow later deletion)
    return index; 
}

/*
TTC_Sched::delete_task()

Removes a task from the scheduler.  Note that this does
*not* delete the associated function from memory:
it simply means that it is no longer called by the scheduler.

index - The task index.  Provided by TTC_Sched::add_task().

RETURN VALUE:  RETURN_ERROR or RETURN_NORMAL
*/
uint8_t TTC_Sched::delete_task(const uint8_t index)
{
    uint8_t result = 0;

    if (0 == this->tasks[index])
    {
        result = TTC_Sched::RETURN_ERROR;
    }
    else
    {
        result = TTC_Sched::RETURN_NORMAL;
    }

    this->tasks[index] = 0;

    return result;       // return status
}

/*
TTC_Sched::dispatch_tasks()

This is the 'dispatcher' function.  When a task (function)
is due to run, TTC_Sched::dispatch_tasks() will run it.
This function must be called (repeatedly) from the main loop.
*/
void TTC_Sched::dispatch_tasks(void)
{
    uint8_t index;

    // Dispatches (runs) the next task (if one is ready)
    for (index = 0; index < SCH_MAX_TASKS; index++)
    {
        if ( this->tasks[index]->is_runnable() )
        {
            // Run the task
            this->tasks[index]->run();

            // Reset (or reduce) runnable flag
            this->tasks[index]->decrease_run();

            // Periodic tasks will automatically run again
            // - if this is a 'one shot' task, remove it from the array
            if (0 == this->tasks[index]->get_period())
            {
                TTC_Sched::delete_task(index);
            }
        }
    }
}

/*
TTC_Sched::update_tasks()

This is the scheduler ISR.  It is called at a rate
determined by the timer settings in TTC_Sched::init().
This version is triggered by Timer 0 interrupts.
*/
void TTC_Sched::update_tasks(void)
{
    uint8_t index;

    // NOTE: calculations are in *TICKS* (not milliseconds)
    for (index = 0; index < SCH_MAX_TASKS; index++)
    {
        // Check if there is a task at this location
        if (0 != this->tasks[index])
        {
            if ( this->tasks[index]->is_timed_out() )
            {
                // The task is due to run:
                this->tasks[index]->increase_run();

                if (this->tasks[index]->get_period() > 0)
                {
                    // Schedule periodic tasks to run again
                    TTC_Sched::tasks[index]->reload_delay();
                }
            }
            
            if (this->tasks[index]->get_delay() > 0)
            {
                // Not yet ready to run
                this->tasks[index]->decrease_delay();
            }
        }
    }
}




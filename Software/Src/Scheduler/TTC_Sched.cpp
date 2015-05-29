#include "TTC_Sched.h"

/* Causes a task (function) to be executed at regular intervals
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
uint8_t    TTC_Sched::AddTask(Runnable * NewTask)
{
    uint8_t Index = 0;

    // First find a gap in the array (if there is one)
    while ( (0 != this->Tasks[Index]) && (Index < SCH_MAX_TASKS))
    {
        Index++;
    }

    // Check if we reached the end of the list.
    if (Index == SCH_MAX_TASKS)
    {
        // Task list is full
        // return an error code
        return SCH_MAX_TASKS;
    }

    // If we're here, there is a space in the task array
    this->Tasks[Index] = NewTask;

    // make the task not runnable (reset runnable flag).
    NewTask->SetRunnable(0);

    // return position of task (to allow later deletion)
    return Index; 
}

/* Removes a task from the scheduler.  Note that this does
 * *not* delete the associated function from memory:
 * it simply means that it is no longer called by the scheduler.
 * 
 * @param Index The task Index.  Provided by TTC_Sched::add_task().
 * 
 * @return RETURN VALUE:  RETURN_ERROR or RETURN_NORMAL
 */
uint8_t TTC_Sched::DeleteTask( const uint8_t Index )
{
    uint8_t Result = 0;

    if (0 == this->Tasks[Index])
    {
        Result = TTC_Sched::RETURN_ERROR;
    }
    else
    {
        Result = TTC_Sched::RETURN_NORMAL;
    }

    this->Tasks[Index] = 0;

    return Result;       // return status
}

/* This is the 'dispatcher' function.  When a task (function)
 * is due to run, TTC_Sched::dispatch_tasks() will run it.
 * This function must be called (repeatedly) from the main loop.
 */
void TTC_Sched::DispatchTasks( void )
{
    uint8_t Index;

    // Dispatches (runs) the next task (if one is ready)
    for (Index = 0; Index < SCH_MAX_TASKS; Index++)
    {
        if ( this->Tasks[Index]->IsRunnable() )
        {
            // Run the task
            this->Tasks[Index]->Run();

            // Reset (or reduce) runnable flag
            this->Tasks[Index]->DecreaseRun();

            // Periodic Tasks will automatically run again
            // - if this is a 'one shot' task, remove it from the array
            if (0 == this->Tasks[Index]->GetPeriod())
            {
                TTC_Sched::DeleteTask(Index);
            }
        }
    }
}

/* This is the scheduler ISR.  It is called at a rate
 * determined by the timer settings in TTC_Sched::init().
 * This version is triggered by Timer 0 interrupts.
 */
void TTC_Sched::UpdateTasks( void )
{
    uint8_t Index;

    // NOTE: calculations are in *TICKS* (not milliseconds)
    for (Index = 0; Index < SCH_MAX_TASKS; Index++)
    {
        // Check if there is a task at this location
        if (0 != this->Tasks[Index])
        {
            if ( this->Tasks[Index]->IsTimedOut() )
            {
                // The task is due to run:
                this->Tasks[Index]->IncreaseRun();

                if (this->Tasks[Index]->GetPeriod() > 0)
                {
                    // Schedule periodic Tasks to run again
                    TTC_Sched::Tasks[Index]->ReloadDelay();
                }
            }
            
            if (this->Tasks[Index]->GetDelay() > 0)
            {
                // Not yet ready to run
                this->Tasks[Index]->DecreaseDelay();
            }
        }
    }
}

#include "TTC_Sched_Pi_Impl.h"

/*
TTC_Sched_Pi_Impl::init()

Scheduler initialisation function. Prepares scheduler
data structures and sets up timer interrupts at required rate.
Must call this function before using the scheduler.
*/
void TTC_Sched_Pi_Impl::init(void)
{

    //tasks[SCH_MAX_TASKS] = {0};

    /* setup your timer...TO DO */
}

/*
TTC_Sched_Pi_Impl::start()

Starts the scheduler, by enabling interrupts.

NOTE: Usually called after all regular tasks are added,
to keep the tasks synchronised.

NOTE: ONLY THE SCHEDULER INTERRUPT SHOULD BE ENABLED!!!
*/
void TTC_Sched_Pi_Impl::start(void)
{
    // Enable interrupt
    // hardware-specific code required...TO DO
}


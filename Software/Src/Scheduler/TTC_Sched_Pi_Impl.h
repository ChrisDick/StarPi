#ifndef TTC_SCHED_PI_IMPL
#define TTC_SCHED_PI_IMPL

#include "TTC_Sched.h"

#define SCHED_TIMEOUT    500 /**< Time of each tick */

/** Class for platform specific implementation ofthe scheduler
 */ 
class TTC_Sched_Pi_Impl: public TTC_Sched
{
    private:
        static TTC_Sched * THIS; /**< only one instance of the scheduler is required */

    public:
    /** Scheduler initialisation function. Prepares scheduler
     * data structures and sets up timer interrupts at required rate.
     * Must call this function before using the scheduler.
     */
        void    Init( void );
    /** Starts the scheduler, by enabling interrupts.
     * NOTE: Usually called after all regular tasks are added,
     * to keep the tasks synchronised.
     * NOTE: ONLY THE SCHEDULER INTERRUPT SHOULD BE ENABLED!!!
     */
        void    Start( void );
};


#endif /* TTC_SCHED_PI_IMPL */
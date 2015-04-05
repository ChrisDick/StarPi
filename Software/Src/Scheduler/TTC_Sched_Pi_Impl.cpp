#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <csignal>

#include "TTC_Sched_Pi_Impl.h"

/*
    private variable.
*/
struct sigaction sa;
struct itimerval timer;

/* 
    Obtained a poniter to the object of class TTC_Sched_Pi_Impl to be used
    inside the C function (timer_handler) because the function is not member 
    of the class and so has no right to access the member (function/data) of 
    the class. 

    Basically The_Sched will hold 'this' which is set inside the
    function member init(). 
*/
TTC_Sched * The_Sched;

/*
    private functions.
*/
void timer_handler (int signum)
{
    /*
         Used C style call of the function member (update_tasks) because
         the handler is a C function NOT a member of the class TTC_Sched_Pi_Impl
         See this link for more details:
         http://stackoverflow.com/questions/3583353/calling-c-class-member-function-from-c-code
    */
    static_cast<TTC_Sched*>(The_Sched)->update_tasks();
}


/*
TTC_Sched_Pi_Impl::init()

Scheduler initialisation function. Prepares scheduler
data structures and sets up timer interrupts at required rate.
Must call this function before using the scheduler.
*/
void TTC_Sched_Pi_Impl::init(void)
{
    uint8_t i = 0;

    for(i=0;i< SCH_MAX_TASKS;i++)
    {
        this->tasks[i] = 0;
    }    

    /* Get pointer to the current object */
    The_Sched = this;

    /* Install timer_handler as the signal handler for SIGVTALRM. */
    memset (&sa, 0, sizeof (sa));
    sa.sa_handler = &timer_handler;
    sigaction (SIGVTALRM, &sa, NULL);

    /* Configure the timer to expire after 250msec */
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = SCHED_TIMEOUT;
     
    /* and every 250msec after that. */
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = SCHED_TIMEOUT;
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
    /* 
        Start a virtual timer. It counts down whenever this process 
        is executing. 
    */
    setitimer (ITIMER_VIRTUAL, &timer, NULL);
}


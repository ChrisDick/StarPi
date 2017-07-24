#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <csignal>
#include "TTC_Sched_Pi_Impl.h"

/*
    private variable.
*/
struct sigaction SA;
struct itimerval Timer;

/* 
    Obtained a poniter to the object of class TTC_Sched_Pi_Impl to be used
    inside the C function (timer_handler) because the function is not member 
    of the class and so has no right to access the member (function/data) of 
    the class. 

    Basically TheSched will hold 'this' which is set inside the
    function member init(). 
*/
TTC_Sched * TheSched;

/*
    private functions.
*/
void timer_handler (int signum)
{
    (void) signum;
    /*
         Used C style call of the function member (update_tasks) because
         the handler is a C function NOT a member of the class TTC_Sched_Pi_Impl
         See this link for more details:
         http://stackoverflow.com/questions/3583353/calling-c-class-member-function-from-c-code
    */
    static_cast<TTC_Sched*>(TheSched)->UpdateTasks();
}


/* Scheduler initialisation function. Prepares scheduler
 * data structures and sets up Timer interrupts at required rate.
 * Must call this function before using the scheduler.
 */
void TTC_Sched_Pi_Impl::Init(void)
{
    uint8_t i = 0;

    for(i=0;i< SCH_MAX_TASKS;i++)
    {
        this->Tasks[i] = 0;
    }    

    /* Get pointer to the current object */
    TheSched = this;

    /* Install timer_handler as the signal handler for SIGVTALRM. */
    memset (&SA, 0, sizeof (SA));
    SA.sa_handler = &timer_handler;
    sigaction (SIGVTALRM, &SA, NULL);

    /* Configure the Timer to expire after 250msec */
    Timer.it_value.tv_sec = 0;
    Timer.it_value.tv_usec = SCHED_TIMEOUT;
     
    /* and every 250msec after that. */
    Timer.it_interval.tv_sec = 0;
    Timer.it_interval.tv_usec = SCHED_TIMEOUT;
}

/* Starts the scheduler, by enabling interrupts.
 * NOTE: Usually called after all regular Tasks are added,
 * to keep the Tasks synchronised.
 * NOTE: ONLY THE SCHEDULER INTERRUPT SHOULD BE ENABLED!!!
 */
void TTC_Sched_Pi_Impl::Start( void )
{
    /* 
        Start a virtual Timer. It counts down whenever this process 
        is executing. 
    */
    setitimer (ITIMER_VIRTUAL, &Timer, NULL);
}


#ifndef RUNNABLE
#define RUNNABLE

#include <stdint.h>


/** Class to handle properties of runnable functions
 */
class Runnable
{

private:
    uint32_t Delay;    /**< How many ticks to delay the run function to allow management of load */
    uint32_t Period;   /**< Period for function in number of ticks */
    uint32_t Runnable; /**< Is the function runnable */

public:
/** Set the delay
 */
    void     SetDelay( uint32_t Delay );
/** Get the current delay
 */
    uint32_t GetDelay( void );
/** Reload the delay
 */
    void     ReloadDelay( void );
/** Decrease the delay
 */
    void     DecreaseDelay( void );
/** Is the delay timed out
 */
    uint8_t  IsTimedOut( void );
/** Set the period
 */
    void     SetPeriod( uint32_t Period );
/** Get the period
 */
    uint32_t GetPeriod( void );
/** Set the function to be run
 */
    void     SetRunnable( uint32_t Runnable );
/** Allow the function to be run again
 */
    void     IncreaseRun( void );
/** prevent the function from being run
 */
    void     DecreaseRun( void );
/** Check to see if the function is to be run
 */
    uint8_t  IsRunnable( void );

    /** function to be run by the scheduler 
     */
    virtual void Run( void ) = 0;
};

#endif /* RUNNABLE */
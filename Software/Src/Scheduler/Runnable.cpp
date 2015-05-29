#include "Runnable.h"

/* Set the delay
 */
void Runnable::SetDelay( uint32_t Delay )
{
    this->Delay = Delay;
}

/* Get the current delay
 */
uint32_t Runnable::GetDelay( void )
{
    return this->Delay;
}

/* Reload the delay
 */
void Runnable::ReloadDelay( void )
{
    this->Delay = this->Period;
}

/* Decrease the delay
 */
void Runnable::DecreaseDelay( void )
{
    this->Delay -= 1;
}

/* Is the delay timed out
 */
uint8_t Runnable::IsTimedOut( void )
{
    return (0 == this->Delay);
}

/* Set the period
 */
void Runnable::SetPeriod( uint32_t Period )
{
    this->Period = Period;
}
 
/* Get the period
 */
uint32_t Runnable::GetPeriod( void )
{
    return this->Period;
}

/* Set the function to be run
 */
void Runnable::SetRunnable( uint32_t Runnable )
{
    this->Runnable = Runnable;
}

/* Allow the function to be run again
 */
void Runnable::IncreaseRun( void )
{
    this->Runnable += 1;
}

/* prevent the function from being run
 */
void Runnable::DecreaseRun( void )
{
    this->Runnable -= 1;
}

/* Check to see if the function is to be run
 */
uint8_t Runnable::IsRunnable( void )
{
    if(this->Runnable > 0)
    {
         return 1;
    }
    return 0;
}

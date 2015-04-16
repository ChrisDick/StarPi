#include "Runnable.h"

void Runnable::set_delay(uint32_t delay)
{
    this->delay = delay;
}

uint32_t Runnable::get_delay(void)
{
    return this->delay;
}

void Runnable::reload_delay(void)
{
    this->delay = this->period;
}

void Runnable::decrease_delay(void)
{
    this->delay -=1;
}

uint8_t Runnable::is_timed_out(void)
{
    return (0 == this->delay);
}

void Runnable::set_period(uint32_t period)
{
    this->period = period;
}
 
uint32_t Runnable::get_period(void)
{
    return this->period;
}

void Runnable::set_runnable(uint32_t runnable)
{
    this->runnable = runnable;
}
void Runnable::increase_run(void)
{
    this->runnable += 1;
}

void Runnable::decrease_run(void)
{
    this->runnable -= 1;
}

uint8_t Runnable::is_runnable(void)
{
    if(this->runnable > 0)
    {
         return 1;
    }

    return 0;
}

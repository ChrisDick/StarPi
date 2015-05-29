#include "TTC_Sched_Pi_Impl.h"

#include <stdio.h>

class Reader: public Runnable
{
    public:
        void Run(void);
};

class Processor: public Runnable
{
    public:
        void Run(void);
};

class Writer: public Runnable
{
    public:
        void Run(void);
};

void Reader::Run(void)
{
    static int count_i = 0;
    printf ("Reader Run %d \n", ++count_i);
}

void Processor::Run(void)
{
    static int count_p = 0;
    printf ("Processor Run %d \n", ++count_p);
}

void Writer::Run(void)
{
    static int count_o = 0;
    printf ("Writer Run %d \n", ++count_o);
}

int main ()
{
    TTC_Sched_Pi_Impl   scheduler;
    
    Reader      reader;
    Processor   processor;
    Writer      writer;
    
    scheduler.Init();   // call first to reset task table and configure timer.
    printf ("scheduler initialised.\n");

    reader.SetDelay(0);    // no offset is set here.
    reader.SetPeriod(4);   // Run every 4 ticks (1 tick == 250ms).
    
    processor.SetDelay(1); // Run one tick after Reader Run.
    processor.SetPeriod(4); // also Run every 4 ticks.
    
    writer.SetDelay(2); // Run one tick after Processor Run.
    writer.SetPeriod(4); // also Run every 4 ticks.
        
    printf ("tasks configured.\n");

    scheduler.AddTask(&reader);
    scheduler.AddTask(&processor);
    scheduler.AddTask(&writer);

    printf ("tasks added.\n");
    
    scheduler.Start();

    printf ("scheduler started.\n");
    
    /* Do busy work. */
    while (1)
    {
          scheduler.DispatchTasks();
    }
    
    return 0; // unreachable statement.
}

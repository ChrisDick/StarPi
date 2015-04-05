#include "TTC_Sched_Pi_Impl.h"

#include <stdio.h>

class Reader: public Runnable
{
    public:
        void run(void);
};

class Processor: public Runnable
{
    public:
        void run(void);
};

class Writer: public Runnable
{
    public:
        void run(void);
};

void Reader::run(void)
{
    static int count_i = 0;
    printf ("Reader run %d \n", ++count_i);
}

void Processor::run(void)
{
    static int count_p = 0;
    printf ("Processor run %d \n", ++count_p);
}

void Writer::run(void)
{
    static int count_o = 0;
    printf ("Writer run %d \n", ++count_o);
}

int main ()
{
    TTC_Sched_Pi_Impl   scheduler;
    
    Reader      reader;
    Processor   processor;
    Writer      writer;
    
    scheduler.init();   // call first to reset task table and configure timer.
    printf ("scheduler initialised.\n");

    reader.set_delay(0);    // no offset is set here.
    reader.set_period(4);   // run every 4 ticks (1 tick == 250ms).
    
    processor.set_delay(1); // run one tick after Reader run.
    processor.set_period(4); // also run every 4 ticks.
    
    writer.set_delay(2); // run one tick after Processor run.
    writer.set_period(4); // also run every 4 ticks.
        
    printf ("tasks configured.\n");

    scheduler.add_task(&reader);
    scheduler.add_task(&processor);
    scheduler.add_task(&writer);

    printf ("tasks added.\n");
    
    scheduler.start();

    printf ("scheduler started.\n");
    
    /* Do busy work. */
    while (1)
    {
          scheduler.dispatch_tasks();
    }
    
    return 0; // unreachable statement.
}

#include "TTC_Sched.h"

class TTC_Sched_Pi_Impl: public TTC_Sched
{  
public:
    void    init(void);
    void    start(void);
    
    static const unsigned long SCHED_TIMEOUT = 1000;
};

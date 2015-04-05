#include "TTC_Sched.h"

#define SCHED_TIMEOUT    250000 // 250msec.

class TTC_Sched_Pi_Impl: public TTC_Sched
{

private:
    static TTC_Sched * THIS;

public:
    void    init(void);
    void    start(void);
};

#include <stdint.h>
#include "Runnable.h"

/* ------Public constants-------------------------------------------*/

/* 
	The maximum number of tasks required at any one time
	during the execution of the program
	MUST BE ADJUSTED FOR EACH NEW PROJECT
*/
#define SCH_MAX_TASKS 3

class TTC_Sched
{
private:
	//static TTC_Task_t	tasks[SCH_MAX_TASKS];
	static Runnable * tasks[SCH_MAX_TASKS];

public:
	static void		init(void);
	static void		start(void);
	static uint8_t	add_task(Runnable * new_task);
	static uint8_t  delete_task(const uint8_t index);
	static void		dispatch_tasks(void);
	static void		update_tasks(void);

	static const uint8_t RETURN_ERROR  = 0;
	static const uint8_t RETURN_NORMAL = 255;
};


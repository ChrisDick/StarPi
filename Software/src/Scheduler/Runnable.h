#include <stdint.h>

class Runnable
{

private:
	uint32_t delay;
	uint32_t period;
	uint32_t runnable;

public:
	void	 set_delay(uint32_t delay);
	uint32_t get_delay(void);
	void	 reload_delay(void);
	void	 decrease_delay(void);
	uint8_t  is_timed_out(void);

	void	 set_period(uint32_t period);
	uint32_t get_period(void);

	void	 set_runnable(uint32_t runnable);
	void	 increase_run();
	void	 decrease_run();
	uint8_t  is_runnable(void);

	virtual void run(void) = 0;
};


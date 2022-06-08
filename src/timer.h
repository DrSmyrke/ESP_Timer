#ifndef __MAIN_TIMER_H_
#define __MAIN_TIMER_H_

#include <stdint.h>

class Timer
{
	public:
		explicit Timer(const uint8_t timerNum, const uint16_t samplingRate);
		bool isInterrupt();
		void confirmInerrupt();
		void counterReset();
};


#endif //__MAIN_TIMER_H_
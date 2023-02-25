#ifndef __MAIN_TIMER_H_
#define __MAIN_TIMER_H_

#include <stdint.h>

#if defined(ARDUINO_ARCH_ESP8266)
	#include <os_type.h>
	#include <osapi.h>
#elif defined(ARDUINO_ARCH_ESP32)
	#include <esp32-hal.h>
#endif

class Timer
{
	public:
#if defined(ARDUINO_ARCH_ESP8266)
		explicit Timer(const uint8_t timerNum, const uint16_t samplingRate, void (*callback)(void*) = nullptr);
#elif defined(ARDUINO_ARCH_ESP32)
		explicit Timer(const uint8_t timerNum, const uint16_t samplingRate, void (*callback)(void) = nullptr);
#endif
		void reset();
		void setMaximum(void);
		uint64_t getCounterValue(void);
		void setCounterValue(const uint64_t &value);
	private:
#if defined(ARDUINO_ARCH_ESP8266)
		os_timer_t m_timer;
#elif defined(ARDUINO_ARCH_ESP32)
		hw_timer_t* m_pTimer;
#endif
};


#endif //__MAIN_TIMER_H_
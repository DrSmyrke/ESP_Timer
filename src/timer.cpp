#include "timer.h"
#if defined(ARDUINO_ARCH_ESP8266)
	#include <os_type.h>
	#include <osapi.h>
#elif defined(ARDUINO_ARCH_ESP32)
	#include <esp32-hal.h>
#endif

//------------------- VARIABLES ------------------------
uint8_t interruptFlag			= 0;

#if defined(ARDUINO_ARCH_ESP8266)
	os_timer_t myTimer;
#elif defined(ARDUINO_ARCH_ESP32)
	portMUX_TYPE timerMux		= portMUX_INITIALIZER_UNLOCKED;
	uint16_t counterMax			= 0;
	uint16_t counter			= 0;
#endif

//------------------------------------------------------
#if defined(ARDUINO_ARCH_ESP8266)
	void timerCallback(void *pArg)
	{
		interruptFlag = 1;
	}
#elif defined(ARDUINO_ARCH_ESP32)
	void IRAM_ATTR onTimer()
	{
		portENTER_CRITICAL_ISR( &timerMux );
		if( counter++ >= counterMax ){
			interruptFlag = 1;
			counter = 0;
		}
		portEXIT_CRITICAL_ISR( &timerMux );
	}
#endif


//------------------------------------------------------
Timer::Timer(const uint8_t timerNum, const uint16_t samplingRate)
{
#if defined(ARDUINO_ARCH_ESP8266)
	
	os_timer_setfn( &myTimer, timerCallback, NULL );
	os_timer_arm( &myTimer, samplingRate, true );
#elif defined(ARDUINO_ARCH_ESP32)
	/*
	// инициализация таймера 0, группы 0
	timer_config_t config;
	config.divider = 80; // тактирование счетчика 1 мкс
	config.counter_dir = TIMER_COUNT_UP; // прямой счет
	config.counter_en = TIMER_START; // счетчик работает
	config.alarm_en = TIMER_ALARM_DIS; // событие перезагрузка запрещено
	config.auto_reload = TIMER_AUTORELOAD_DIS ; // аппаратная перезагрузка запрещена
	timer_init(TIMER_GROUP_0, TIMER_0 , &config); // инициализация
	*/

	counterMax = samplingRate;
	hw_timer_t* timer = timerBegin( timerNum, getCpuFrequencyMhz(), true );	//Begin timer with 1 MHz frequency (80MHz/80)
	timerAttachInterrupt(timer, &onTimer, true);			//Attach the interrupt to Timer1
	timerAlarmWrite(timer, samplingRate, true);				//Initialize the timer
	timerAlarmEnable(timer);
#endif
}


//------------------------------------------------------
bool Timer::isInterrupt()
{
	return ( interruptFlag ) ? true : false;
}

//------------------------------------------------------
void Timer::confirmInerrupt()
{
	interruptFlag = 0;
}

//------------------------------------------------------
void Timer::counterReset()
{
#if defined(ARDUINO_ARCH_ESP32)
	counter = 0;
#endif
}

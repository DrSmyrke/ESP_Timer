#include "timer.h"

//------------------- VARIABLES ------------------------


#if defined(ARDUINO_ARCH_ESP8266)

#elif defined(ARDUINO_ARCH_ESP32)
	// portMUX_TYsPE timerMux		= portMUX_INITIALIZER_UNLOCKED;

#endif

//------------------------------------------------------
#if defined(ARDUINO_ARCH_ESP8266)
	void timerCallback(void *pArg)
	{
		interruptFlag = 1;
	}
#elif defined(ARDUINO_ARCH_ESP32)
	// void IRAM_ATTR onTimer()
	// {
	// 	portENTER_CRITICAL_ISR( &timerMux );
	// 	// if( counter++ >= counterMax ){
	// 		// interruptFlag = 1;
	// 		// counter = 0;
	// 	// }
	// 	portEXIT_CRITICAL_ISR( &timerMux );
	// }
#endif


//------------------------------------------------------
Timer::Timer(const uint8_t timerNum, const uint16_t samplingRate, void (*callback)(void))
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

	// counterMax = samplingRate;
	uint32_t freq = getApbFrequency();
	if( freq > 1000000 ) freq /= 1000000;
	m_pTimer = timerBegin( timerNum, freq, true );	//Begin timer with 1 MHz frequency
	//Attach the interrupt to Timer
	timerAttachInterrupt( m_pTimer, callback, true );
	//Initialize the timer
	timerAlarmWrite( m_pTimer, 1000000, true );
	timerAlarmEnable( m_pTimer );
#endif
}


//------------------------------------------------------
//------------------------------------------------------
//------------------------------------------------------
void Timer::reset()
{
	timerRestart( m_pTimer );
}

//------------------------------------------------------
//------------------------------------------------------

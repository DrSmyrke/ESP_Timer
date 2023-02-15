#include "timer.h"

//------------------- VARIABLES ------------------------
//------------------------------------------------------
//------------------------------------------------------
#if defined(ARDUINO_ARCH_ESP8266)
Timer::Timer(const uint8_t timerNum, const uint16_t samplingRate, void (*callback)(void*))
{
	os_timer_setfn( &m_timer, callback, NULL );
	os_timer_arm( &m_timer, samplingRate, true );
}

#elif defined(ARDUINO_ARCH_ESP32)
Timer::Timer(const uint8_t timerNum, const uint16_t samplingRate, void (*callback)(void))
{
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

	uint32_t freq = getApbFrequency();
	if( freq > 1000000 ) freq /= 1000000;
	m_pTimer = timerBegin( timerNum, freq, true );	//Begin timer with 1 MHz frequency
	//Attach the interrupt to Timer
	timerAttachInterrupt( m_pTimer, callback, true );
	//Initialize the timer
	timerAlarmWrite( m_pTimer, samplingRate * 1000, true );
	timerAlarmEnable( m_pTimer );
}
#endif

//------------------------------------------------------
//------------------------------------------------------
//------------------------------------------------------
void Timer::reset()
{
#if defined(ARDUINO_ARCH_ESP8266)
	// os_time
#elif defined(ARDUINO_ARCH_ESP32)
	timerRestart( m_pTimer );
#endif
}

//------------------------------------------------------
//------------------------------------------------------

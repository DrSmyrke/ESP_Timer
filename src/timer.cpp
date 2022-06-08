#include "timer.h"
// #include "driver/timer.h"

//------------------- VARIABLES ------------------------
portMUX_TYPE timerMux		= portMUX_INITIALIZER_UNLOCKED;
uint8_t interruptFlag		= 0;
uint16_t counter			= 0;
uint16_t counterMax			= 0;
//------------------------------------------------------
void IRAM_ATTR onTimer()
{
	portENTER_CRITICAL_ISR( &timerMux );
	if( counter++ >= counterMax ){
		interruptFlag = 1;
		counter = 0;
	}
	portEXIT_CRITICAL_ISR( &timerMux );
}

//------------------------------------------------------
Timer::Timer(const uint8_t timerNum, const uint16_t samplingRate)
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

	counterMax = samplingRate;
	hw_timer_t* timer = timerBegin( timerNum, getCpuFrequencyMhz(), true );	//Begin timer with 1 MHz frequency (80MHz/80)
	timerAttachInterrupt(timer, &onTimer, true);			//Attach the interrupt to Timer1
	timerAlarmWrite(timer, samplingRate, true);				//Initialize the timer
	timerAlarmEnable(timer);
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
	counter = 0;
}

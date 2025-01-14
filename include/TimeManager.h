#ifndef __TIME_MANAGER_H_
#define __TIME_MANAGER_H_

#include <Arduino.h>
#include "time.h"
#include "Configuration.h"
#if RUN_WITHOUT_WIFI == false
	#include "WiFi.h"
#endif

class TimeManager
{
	friend void IRAM_ATTR onTimer();
public:
	typedef struct 
	{
		uint8_t hours;
		uint8_t minutes;
		uint8_t seconds;
	}TimeInfo;

	typedef void (*TimerCallBack)(void);
private:
	TimeInfo currentTime;
	hw_timer_t* timer;
	uint32_t offlineTimeCounter;
	static TimeManager* TimeManagerSingelton;
	TimeInfo TimerInitialDuration;
	TimeInfo TimerDuration;
	TimerCallBack TimerTickCallback;
	TimerCallBack TimerDoneCallback;
	bool TimerModeActive;

	TimeManager();
	void advanceByOneSecondOffline();
	void TimerCountDOwnByOneSecond();
public:
	~TimeManager();

	/**
	 * @brief Get the singelton instance ot the Time Manager
	 */
	static TimeManager* getInstance();

	/**
	 * @brief Disable the timer and deactivate the interrupt
	 */
	void disableTimer();

	/**
	 * @brief Synchronize the time with the NTP server
	 */
	bool synchronize();

	/**
	 * @brief get the current time in a struct
	 */
	TimeInfo getCurrentTime();

	/**
	 * @brief get the current time as a string
	 */
	String getCurrentTimeString();

	/**
	 * @brief Set the duration for the Timer
	 */
	void setTimerDuration(TimeInfo newTimerDuration);

	/**
	 * @brief Start the timer
	 */
	void startTimer();

	/**
	 * @brief Stop the timer
	 */
	void stopTimer();

	void setTimerTickCallback(TimerCallBack callback);

	void setTimerDoneCallback(TimerCallBack callback);


};

#endif
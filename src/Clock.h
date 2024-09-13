/*
 * Library: Clock
 *
 * Description: Library for controlling the clock.
 * Works by the unix time system.
 * 
 * Author: Nazar Vereshchynskyi
 * Email: verechnazar12@gmail.com
 * Version: 1.0.0
 * Date: 08.04.2024
 */

#ifndef _CLOCK_H
#define _CLOCK_H

#include <Arduino.h>
#include <stdint.h>

/* --- latest unix time & macros with seconds count --- */
#define LAST_UNIX 1725999065
#define SECONDS_IN_MINUTE 60
#define SECONDS_IN_HOUR 3600
#define SECONDS_IN_DAY 86400

/* --- leap year check --- */
#define LEAP_YEAR(Y) ( !((Y) % 4) && ( ((Y) % 100) || !((Y) % 400) ) )

/* --- day count in months --- */
const uint8_t monthDays[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

/* --- main time template --- */
struct TimeT {
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint8_t weekday;
	
	uint8_t day;
	uint8_t month;
	uint16_t year;
};

/* --- main class --- */
class Clock {
public:
	Clock();

	uint8_t status();

	void setTime(int8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint16_t);
	void setTime(int8_t, TimeT);
	TimeT getTime(int8_t);

	void setUnix(uint32_t);
	uint32_t getUnix();

	uint8_t hour(int8_t);
	uint8_t minute(int8_t);
	uint8_t second(int8_t);

	uint8_t weekday(int8_t);
	uint8_t day(int8_t);
	uint8_t month(int8_t);
	uint16_t year(int8_t);

	void printDate(int8_t);
	void printDate(TimeT);

private:
	uint16_t dataFromUnix(int8_t, uint8_t);
	TimeT dataFromUnix(int8_t);

	int32_t unix;
	uint32_t last_mls;

};

#endif
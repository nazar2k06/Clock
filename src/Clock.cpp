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

#include "Clock.h"

/* --- constuctor --- */
Clock::Clock() {
	unix = 0;
	last_mls = 0;
}


/* --- status --- */
uint8_t Clock::status() {
	if (unix < LAST_UNIX) {
		return 1;
	}

	return 0;
}


/* --- set the time from the parameters (considering gmt) --- */
void Clock::setTime(int8_t gmt, uint8_t hour, uint8_t minute, uint8_t second,
				   uint8_t day, uint8_t month, uint16_t year) {
	uint32_t days = 0;

	if (year < 1970 ||                              /*Year must be >= 1970*/
		month < 1 || month > 12 ||                  /*Month must be >= 1 && <= 12*/
		day < 1 || day > 31 ||                      /*Day must be >= 1 & <= 31*/
		hour > 23 || minute > 59 || second > 59 ||
		(year == 1970 && (hour - gmt) < 0)) {

		return;
	}

	last_mls = millis();

	days = (year - 1970) * 365 + (day - 1); //�� ��� ���������� ������ ����, �� ��������� ��� �����
	unix = ((hour - gmt) * 60 + minute) * 60 + second;

	for (uint16_t i = 1970;i < year;i++) {
		days += LEAP_YEAR(i);
	}

	for (uint8_t i = 0;i < month - 1;i++) {
		days += monthDays[i];
	}

	if (month >= 2) {
		days += LEAP_YEAR(year);
	}

	unix += days * SECONDS_IN_DAY;
}

/* --- set the time from the main time template (considering gmt) --- */
void Clock::setTime(int8_t gmt, TimeT time) {
	setTime(gmt, time.hour, time.minute, time.second, time.day, time.month, time.year);
}

/* --- get time from the current unix time (considering gmt) --- */
TimeT Clock::getTime(int8_t gmt) {
	return dataFromUnix(gmt);
}


/* --- get the current unix time --- */
uint32_t Clock::getUnix() {
	uint32_t delta = (millis() - last_mls) / 1000;

	if (delta) {
		unix += delta;
		last_mls += delta * 1000;
	}

	return unix;
}

/* --- set the current unix time --- */
void Clock::setUnix(uint32_t Unix) {
	unix = Unix;

	last_mls = millis();
}


/* --- get the current hour (considering gmt) --- */
uint8_t Clock::hour(int8_t gmt) {
	return dataFromUnix(gmt, 2);
}

/* --- get the current minute (considering gmt) --- */
uint8_t Clock::minute(int8_t gmt) {
	return dataFromUnix(gmt, 1);
}

/* --- get the current second (considering gmt) --- */
uint8_t Clock::second(int8_t gmt) {
	return dataFromUnix(gmt, 0);
}

/* --- get the current weekday (considering gmt) --- */
uint8_t Clock::weekday(int8_t gmt) {
	return dataFromUnix(gmt, 3);
}

/* --- get the current day (considering gmt) --- */
uint8_t Clock::day(int8_t gmt) {
	return dataFromUnix(gmt, 6);
}

/* --- get the current month (considering gmt) --- */
uint8_t Clock::month(int8_t gmt) {
	return dataFromUnix(gmt, 5);
}

/* --- get the current year (considering gmt) --- */
uint16_t Clock::year(int8_t gmt) {
	return dataFromUnix(gmt, 4);
}


/* --- print the current time (considering gmt) --- */
void Clock::printDate(int8_t gmt) {
	Serial.println(String(hour(gmt)) + ":" + minute(gmt) + ":" + second(gmt));
	Serial.println(String(day(gmt)) + "." + month(gmt) + "." + year(gmt) + " | " + weekday(gmt));

}

/* --- print the time from the main time template (considering gmt) --- */
void Clock::printDate(TimeT time) {
	Serial.println(String(time.hour) + ":" + time.minute + ":" + time.second);
	Serial.println(String(time.day) + "." + time.month + "." + time.year + " | " + time.weekday);

}


/* --- get a certain value from the current unix time (considering gmt) --- */
uint16_t Clock::dataFromUnix(int8_t gmt, uint8_t what) {
	getUnix();

	uint16_t Year = 1970;
	uint16_t Month = 1;
	uint32_t Day = unix;

	int32_t delta_time = gmt * SECONDS_IN_HOUR;

	if (unix < 0 || unix < -delta_time) {
		return 0;
	}

	Day += delta_time;

	if (!what) {
		return Day % 60;
	}
	Day /= 60;

	if (what == 1) {
		return Day % 60;
	}
	Day /= 60;

	if (what == 2) {
		return Day % 24;
	}
	Day /= 24;

	if (what == 3) {
		return ((Day + 3) % 7) + 1;
	}

	while (true) {
		uint16_t DaysInYear = 365 + LEAP_YEAR(Year);

		if (Day < DaysInYear) {
			break;
		}

		Day -= DaysInYear;
		Year++;
	}

	if (what == 4) {
		return Year;
	}

	while (true) {
		if (Day < monthDays[Month - 1]) {
			break;
		}

		if (Month == 2) {
			if (Day < (monthDays[1] + LEAP_YEAR(Year))) {
				break;
			}

			Month++;
			Day -= monthDays[1] + LEAP_YEAR(Year);
			continue;
		}

		Day -= monthDays[Month - 1];
		Month++;
	}

	if (what == 5) {
		return Month;
	}

	if (what == 6) {
		return Day + 1;
	}

	return 0;
}

/* --- get the main time template from the current unix time (considering gmt) --- */
TimeT Clock::dataFromUnix(int8_t gmt) {
	TimeT read_time;
	getUnix();

	read_time.year = 1970;
	read_time.month = 1;
	uint32_t Unix = unix;


	int32_t delta_time = gmt * SECONDS_IN_HOUR;

	if (unix < 0 || unix < -delta_time) {
		return read_time;
	}
	Unix += delta_time;

	read_time.second = Unix % 60;
	Unix /= 60;

	read_time.minute = Unix % 60;
	Unix /= 60;

	read_time.hour = Unix % 24;
	Unix /= 24;

	read_time.weekday = ((Unix + 3) % 7) + 1;

	uint16_t DaysInYear;
	while (true) {
		DaysInYear = 365 + LEAP_YEAR(read_time.year);

		if (Unix < DaysInYear) {
			break;
		}

		Unix -= DaysInYear;
		read_time.year++;
	}

	while (true) {
		if (Unix < monthDays[read_time.month - 1]) {
			break;
		}

		if (read_time.month == 2) {
			if (Unix < (monthDays[1] + LEAP_YEAR(read_time.year))) {
				break;
			}

			read_time.month++;
			Unix -= monthDays[1] + LEAP_YEAR(read_time.year);
			continue;
		}

		Unix -= monthDays[read_time.month - 1];
		read_time.month++;
	}

	read_time.day = Unix + 1;

	return read_time;
}
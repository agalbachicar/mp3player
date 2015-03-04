/*
 * RTC.h
 *
 *  Created on: 11/05/2013
 *      Author: Pablo M.
 */

#ifndef RTC_H_
#define RTC_H_


typedef struct {
	unsigned int seconds; 		/*!< Seconds Register */
	unsigned int minutes; 		/*!< Minutes Register */
	unsigned int hours; 		/*!< Hours Register */
	unsigned int dayofmonth;		/*!< Day of Month Register */
	unsigned int dayofweek; 		/*!< Day of Week Register */
	unsigned int dayofyear; 		/*!< Day of Year Register */
	unsigned int month; 	/*!< Months Register */
	unsigned int year; 		/*!< Years Register */
} RTCDate;

int RTCset_date (RTCDate * date);
int RTCset_alarm (RTCDate * alarm_date, void(*callback_alarm_handler)(void));


#endif /* RTC_H_ */

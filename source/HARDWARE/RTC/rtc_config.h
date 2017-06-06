
#ifndef __RTC_CONFIG_H__
#define __RTC_CONFIG_H__

#ifndef NULL
#define NULL 	0
#endif

extern void rtc_init(void);
extern uint8_t* get_time_str(uint8_t *buf);
extern uint16_t get_rtc_second(void);
extern uint16_t get_rtc_minute(void);
extern uint16_t get_rtc_hour(void);
extern uint16_t get_rtc_day(void);
extern uint16_t get_rtc_month(void);
extern uint16_t get_rtc_year(void);
extern void rtc_set_time(uint32_t year, uint32_t month, uint32_t day,
		uint32_t hours, uint32_t minutes, uint32_t seconds);


#endif //__RTC_CONFIG_H__

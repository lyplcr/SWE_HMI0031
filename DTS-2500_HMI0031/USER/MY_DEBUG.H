#ifndef __MY_DEBUG_H
#define __MY_DEBUG_H

/* 调试总开关 */
#define __DEBUG__

#define DEBUG_ON           				0x80U
#define DEBUG_OFF           			0x00U

/* 使能某一功能调试语句 */
#define	DEBUG_TIMER						DEBUG_OFF
#define DEBUG_CMD						DEBUG_OFF
#define DEBUG_LINK						DEBUG_OFF
#define DEBUG_USB						DEBUG_OFF
#define DEBUG_LINK_MODE					DEBUG_OFF
#define DEBUG_SPEED						DEBUG_OFF
#define DEBUG_SAMPLE_FH					DEBUG_OFF
#define DEBUG_FORCE_KY					DEBUG_OFF
#define DEBUG_FORCE_KZ					DEBUG_OFF
#define DEBUG_COORDINATE_DRAW_LINE		DEBUG_OFF
#define DEBUG_HARD_TEST					DEBUG_OFF
#define DEBUG_TEST_LOAD					DEBUG_OFF
#define	DEBUG_REAL_TIME_DATA_FRAM		DEBUG_OFF
#define	DEBUG_KEY						DEBUG_OFF
#define DEBUG_DEFORM_SHOW				DEBUG_OFF
#define	DEBUG_EXCEPTION_DIVISION_ZERO	DEBUG_OFF	//除零异常
#define DEBUG_PRINTF_TIMING				DEBUG_OFF	//打印耗时

#ifdef __DEBUG__
	#define ECHO(debug,...) do{\
								if ((debug) & DEBUG_ON)\
								{\
									printf(__VA_ARGS__);\
								}\
							  }while(0)  
#else  
	#define ECHO(debug, message) ((void)0)
#endif

#endif

#ifndef __MY_DEBUG_H
#define __MY_DEBUG_H

/* 调试总开关 */
#if (VERSION_RELEASE == 0x01U)
	#define __DEBUG__						0x00U	
	#define __ASSERT__						0x00U	
#else
	#define __DEBUG__						0x01U	
	#define __ASSERT__						0x00U	
#endif

#define DEBUG_ON           					0x80U
#define DEBUG_OFF           				0x00U

/* 使能某一功能调试语句 */
#define	DEBUG_TIMER							DEBUG_OFF
#define DEBUG_CMD							DEBUG_OFF
#define DEBUG_LINK							DEBUG_OFF
#define DEBUG_USB							DEBUG_OFF
#define DEBUG_LINK_MODE						DEBUG_OFF
#define DEBUG_SPEED							DEBUG_OFF
#define DEBUG_SAMPLE_FH						DEBUG_OFF
#define DEBUG_COORDINATE_DRAW_LINE			DEBUG_OFF
#define DEBUG_HARD_TEST						DEBUG_OFF
#define DEBUG_TEST_LOAD						DEBUG_OFF
#define	DEBUG_REAL_TIME_DATA_FRAME			DEBUG_OFF
#define	DEBUG_KEY							DEBUG_OFF
#define DEBUG_DEFORM_SHOW					DEBUG_OFF	
#define DEBUG_COORDINATE_LINE_STATUS		DEBUG_OFF
#define DEBUG_COMM_BUFF						DEBUG_OFF
#define DEBUG_ACTIVE						DEBUG_OFF
#define DEBUG_SCREW_UP_DOWN					DEBUG_OFF
#define DEBUG_CURVE_SHOW					DEBUG_OFF
#define DEBUG_PARAMETER_LEN					DEBUG_OFF
#define DEBUG_YIELD_DEFORM					DEBUG_ON


#if (__DEBUG__ == 0x01U)
	#define ECHO(debug,...) do{\
								if ((debug) & DEBUG_ON)\
								{\
									printf(__VA_ARGS__);\
								}\
							  }while(0)  
#else  
	#define ECHO(debug,...) ((void)0)
#endif
	
#if (__ASSERT__ == 0x01U)
	#define ECHO_ASSERT(assertion,...) do { if(!(assertion)) \
		printf(__VA_ARGS__); } while(0)
#else
	#define ECHO_ASSERT(assertion,...) 
#endif

#endif

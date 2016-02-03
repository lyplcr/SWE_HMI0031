#ifndef __MY_DEBUG_H
#define __MY_DEBUG_H

/* 调试总开关 */
#ifndef VERSION_RELEASE
	#define __DEBUG__
	#define __ASSERT__
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
#define DEBUG_PRINTF_TIMING					DEBUG_OFF	
#define DEBUG_COORDINATE_LINE_STATUS		DEBUG_OFF
#define DEBUG_COMM_BUFF						DEBUG_ON


#ifdef __DEBUG__
	#define ECHO(debug,...) do{\
								if ((debug) & DEBUG_ON)\
								{\
									printf(__VA_ARGS__);\
								}\
							  }while(0)  
#else  
	#define ECHO(debug,...) ((void)0)
#endif
	
#ifdef __ASSERT__
	#define ECHO_ASSERT(assertion,...) do { if(!(assertion)) \
		printf(__VA_ARGS__); } while(0)
#else
	#define ECHO_ASSERT(assertion,...) 
#endif

#endif

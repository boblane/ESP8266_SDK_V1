/*
 * Debug.h
 *
 *  Created on: 2017Äê5ÔÂ30ÈÕ
 *      Author: boblane
 */

#ifndef APP_USER_DEBUG_H_
#define APP_USER_DEBUG_H_

//#undef OPT_DEBUG_ON
#define OPT_DEBUG_ON
//#undef DEBUG_ON
//#define DEBUG_ON

#if defined(OPT_DEBUG_ON)
#define INFO( format, ... ) os_printf( format, ## __VA_ARGS__ )
#elif defined(DEBUG_ON)
#define INFO( format, ... ) printf( format, ## __VA_ARGS__ )
#else
#define INFO( format, ... )
#endif

void Sleep_ms(unsigned long _ms);

#endif /* APP_USER_DEBUG_H_ */

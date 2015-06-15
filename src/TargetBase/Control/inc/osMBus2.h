/* $Header: /4CReleased/V2.20.00/Target/4CWin/CONTROL/inc/osMBus2.h 1     28.02.07 19:10 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: osMBus2.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/4CWin/CONTROL/inc/osMBus2.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				RunTime
 *
 * =COMPONENT			vmLib
 *
 * =CURRENT 	 $Date: 28.02.07 19:10 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _OSMBUS2_H_
#define _OSMBUS2_H_

#if defined(INC_4C)
#error "Must not be included in 4C!"
#endif

#define PORT_OK 				0
#define PORT_CANNOT_OPEN		1
#define PORT_NOT_SUPPORTED		2
#define PORT_NO_FREE_MEM		3
#define PORT_CANNOT_SET_PARAMS	4
#define PORT_NO_FREE_PORTS		5

#include <windows.h>

/* Mutex start
 */
#define MBUS2_MUTEX_NAME		"mbus2 mutex"
#define MBUS2_MUTEX_T			HANDLE
#define MUTEX_LOCK_OK			WAIT_OBJECT_0

#define MBUS2_MUTEX_INIT(m) 									\
	{															\
		m = CreateMutex(NULL, FALSE, TEXT(MBUS2_MUTEX_NAME));	\
	}
#define MBUS2_MUTEX_DESTROY(m)									\
	{															\
		CloseHandle(m); 										\
	}

#define MBUS2_MUTEX_UNLOCK(m)	ReleaseMutex(m)
#define MBUS2_MUTEX_TRYLOCK(m)	WaitForSingleObject(m, 0L)
/* Mutex end
 */

IEC_DINT mbOpenPort(const IEC_STRING_IMPL *strPort, const long baud);
IEC_DINT mbClosePort(const IEC_STRING_IMPL *strPort );
IEC_DINT mbSendTelegram(const IEC_STRING_IMPL *strPort, IEC_BYTE* bypInBuff, size_t len);
IEC_UINT mbReceiveTelegram(const IEC_STRING_IMPL *strPort, IEC_STRMAX* sBuff, size_t out_count);

#endif /* _OSMBUS2_H_ */

/* ---------------------------------------------------------------------------- */

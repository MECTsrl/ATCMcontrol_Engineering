/* $Header: /4CReleased/V2.20.00/Target/4CWin/CONTROL/inc.bac/bacLib.h 1     28.02.07 19:10 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: bacLib.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/4CWin/CONTROL/inc.bac/bacLib.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				RunTime
 *
 * =COMPONENT			ioBac
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

#ifndef _BACLIB_H_
#define _BACLIB_H_

#if defined(INC_4C)
#error "Must not be included in 4C!"
#endif

#if defined(RTS_CFG_BACNET)

#define RTS_PRAGMA_PACK_1	/* >>>> Align 1 Begin >>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
#include "osAlign.h"
#undef	RTS_PRAGMA_PACK_1


#if defined(IP_CFG_INST8) || defined(IP_CFG_INST16)
#error Instance alignment 8 and 16 bit is not supported for the BACnet library
#endif


/* BACnetAny
 * ----------------------------------------------------------------------------
 */
typedef struct	
{
	DEC_VAR(IEC_WORD,	wType);

	DEC_VAR(IEC_WORD,	word_value);
	DEC_VAR(IEC_REAL,	real_value);
	DEC_VAR(IEC_UDINT,	udint_value);
	DEC_VAR(IEC_BOOL,	bool_value);
	DEC_VAR(IEC_BYTE,	dummy08);
	DEC_VAR(IEC_WORD,	dummy16);

} IBACnetAny;


/* BACnetDate
 * ----------------------------------------------------------------------------
 */
typedef struct
{
	DEC_VAR(IEC_UINT,	year);
	DEC_VAR(IEC_UINT,	month);
	DEC_VAR(IEC_UINT,	day);
	DEC_VAR(IEC_UINT,	dayOfWeek);
	
} IBACnetDate;


/* BACnetTime
 * ----------------------------------------------------------------------------
 */
typedef struct
{
	DEC_VAR(IEC_UINT,	hour);
	DEC_VAR(IEC_UINT,	minute);
	DEC_VAR(IEC_UINT,	second);
	DEC_VAR(IEC_UINT,	hundrethsOfASec);

} IBACnetTime;


/* BACnetWeekNDay
 * ----------------------------------------------------------------------------
 */
typedef struct 
{
	DEC_VAR(IEC_UINT,	month);
	DEC_VAR(IEC_UINT,	weekOfMonth);
	DEC_VAR(IEC_UINT,	dayOfWeek);

} IBACnetWeekNDay;


/* BACnetDateTime
 * ----------------------------------------------------------------------------
 */
typedef struct
{
	DEC_VAR(IEC_UINT,	idDate);
	DEC_VAR(IEC_UINT,	idTime);

} IBACnetDateTime;

typedef struct
{
	IBACnetTime *pTime;
	IBACnetDate *pDate;

} IBACnetDateTimeEx;


/* BACnetTimeStamp
 * ----------------------------------------------------------------------------
 */
typedef struct	
{
	DEC_VAR(IEC_WORD,	wType);
	DEC_VAR(IEC_UINT,	idDate);
	DEC_VAR(IEC_UINT,	idTime);
	DEC_VAR(IEC_UINT,	dummy);

	DEC_VAR(IEC_UDINT,	ulSequence);

} IBACnetTimeStamp;

typedef struct	
{
	IEC_WORD	wType;

	IBACnetTime *pTime;
	IBACnetDate *pDate;

	IEC_UDINT	ulSequence;

} IBACnetTimeStampEx;


/* BACnetDateRange
 * ----------------------------------------------------------------------------
 */
typedef struct
{
	DEC_VAR(IEC_UINT,	idStart);
	DEC_VAR(IEC_UINT,	idEnd);

} IBACnetDateRange;

typedef struct	
{
	IBACnetDate *pStart;
	IBACnetDate *pEnd;

} IBACnetDateRangeEx;


/* BACnetTimeValue
 * ----------------------------------------------------------------------------
 */
typedef struct	
{
	DEC_VAR(IEC_UINT,	idTime);
	DEC_VAR(IEC_UINT,	idValue);

} IBACnetTimeValue;

typedef struct
{
	IBACnetTime *pTime;
	IBACnetAny	*pValue;

} IBACnetTimeValueEx;


/* BACnetCalendarEntry
 * ----------------------------------------------------------------------------
 */
typedef struct
{
	DEC_VAR(IEC_WORD,	wType);

	DEC_VAR(IEC_UINT,	idDateRange);
	DEC_VAR(IEC_UINT,	idWeekNDay);

} IBACnetCalendarEntry;

typedef struct	
{
	IEC_WORD	wType;

	IBACnetDateRangeEx *pDateRange;
	IBACnetWeekNDay    *pWeekNDay;

} IBACnetCalendarEntryEx;


/* BACnetDailySchedule
 * ----------------------------------------------------------------------------
 */
typedef struct 
{
	DEC_VAR(IEC_UDINT,	ulElements);
	
	DEC_VAR(IEC_UINT,	idDaySched);	/* ID of IEC-array of IBACnetTimeValue	*/
	DEC_VAR(IEC_WORD,	dummy01);

} IBACnetDailySchedule;

typedef struct 
{
	IEC_UDINT ulElements;

	IBACnetTimeValueEx *ppDaySched[0];	/* Array of IBACnetTimeValueEx			*/

} IBACnetDailyScheduleEx;


/* BACnetSpecialEvent
 * ----------------------------------------------------------------------------
 */
typedef struct
{
	DEC_VAR(IEC_WORD,	wType); 		/* Type of period						*/
	DEC_VAR(IEC_WORD,	dummy01);
	DEC_VAR(IEC_DINT,	ObjectID);		/* Corresponds to CalendarReference of	*/
	DEC_VAR(IEC_DINT,	TypeID);		/* type BACnetObjectIdentifier. 		*/
	
	DEC_VAR(IEC_UINT,	idCalEntry);
	DEC_VAR(IEC_WORD,	dummy02);

	DEC_VAR(IEC_UDINT,	EventPriority);
	
	DEC_VAR(IEC_UDINT,	ulElements);
	DEC_VAR(IEC_UINT,	idTimeVal); 	/* ID of IEC-array of IBACnetTimeValue	*/ 
	DEC_VAR(IEC_WORD,	dummy03);

} IBACnetSpecialEvent;

typedef struct 
{
	IEC_WORD	wType;
	IEC_DINT	ObjectID;
	IEC_DINT	TypeID;
	
	IBACnetCalendarEntryEx *pCalEntry;

	IEC_UDINT	EventPriority;

	IEC_UDINT	ulElements;
	IBACnetTimeValueEx	*ppTimeVal[0];	/* Array of IBACnetTimeValueEx			*/

} IBACnetSpecialEventEx;


/* BACnetLogRecord
 * ----------------------------------------------------------------------------
 */
typedef struct	
{
	DEC_VAR(IEC_UINT,	idTimeStamp);

	DEC_VAR(IEC_WORD,	wType);

	DEC_VAR(IEC_WORD,	word_value);
	DEC_VAR(IEC_WORD,	dummy00);
	DEC_VAR(IEC_REAL,	real_value);
	DEC_VAR(IEC_UDINT,	udint_value);
	DEC_VAR(IEC_BOOL,	bool_value);
	DEC_VAR(IEC_BYTE,	dummy08);
	DEC_VAR(IEC_WORD,	dummy16);

} IBACnetLogRecord;

typedef struct	
{
	IBACnetDateTimeEx	*pTimeStamp;

	IEC_WORD	wType;

	IEC_WORD	word_value;
	IEC_REAL	real_value;
	IEC_UDINT	udint_value;
	IEC_BOOL	bool_value;
	
} IBACnetLogRecordEx;


/* BACnetStatusFlags
 * ----------------------------------------------------------------------------
 */
typedef struct	
{
	/* 7 | 6 | 5 | 4 | InAlarm | Fault | Overridden | OutOfService */
	
	DEC_VAR(IEC_BYTE,	bits);

} IBACnetStatusFlags;


/* BACnetEventTransition
 * ----------------------------------------------------------------------------
 */
typedef struct	
{
	/* 7 | 6 | 5 | 4 | 3 | To OffNormal | To Fault | To Normal */

	DEC_VAR(IEC_BYTE,	bits);

} IBACnetEventTransition;


/* BACnetLimitEnable
 * ----------------------------------------------------------------------------
 */
typedef struct	
{
	/* 7 | 6 | 5 | 4 | 3 | 2 | LowLimit | HighLimit */

	DEC_VAR(IEC_BYTE,	bits);

} IBACnetLimitEnable;


/* BACnetDeviceObjectPropertyReference
 * ----------------------------------------------------------------------------
 */
typedef struct	
{
	DEC_VAR(IEC_DINT, iObjType);
	DEC_VAR(IEC_DINT, iObjectID);
	DEC_VAR(IEC_DINT, iDeviceID);
	DEC_VAR(IEC_DINT, iPropType);
	DEC_VAR(IEC_DINT, iArrayIndex);

} IBACnetDevObjPropRef;

typedef struct	
{
	IBACnetDevObjPropRef *pRef;
	
} IBACnetDevObjPropRefEx;


/* BACnetEvent
 * ----------------------------------------------------------------------------
 */
typedef struct	
{
	DEC_VAR(IEC_DINT,  DeviceID);
	DEC_VAR(IEC_DINT,  ObjectID);
	DEC_VAR(IEC_DINT,  TypeID);

	DEC_VAR(IEC_UDINT, NotifClass);
	DEC_VAR(IEC_UDINT, Priority);
	DEC_VAR(IEC_WORD,  EventType);
	DEC_VAR(IEC_WORD,  NotifyType);
	DEC_VAR(IEC_WORD,  FromState);
	DEC_VAR(IEC_WORD,  ToState);
	
	DEC_VAR(IEC_UDINT, Buff_PrevNotif);
	DEC_VAR(IEC_UDINT, Buff_CurrNotif);

	DEC_VAR(IEC_UINT,  id_timestamp);

	/* 7 | 6 | 5 | 4 | 3 | 2 | 1 | AckRequired */

	DEC_VAR(IEC_BYTE,	bits);

	DEC_VAR(IEC_BYTE,	dummy08);
	DEC_VAR(IEC_WORD,	dummy16);

} IBACnetEvent;

typedef struct	
{
	IEC_DINT  DeviceID;
	IEC_DINT  ObjectID;
	IEC_DINT  TypeID;

	IEC_UDINT NotifClass;
	IEC_UDINT Priority;
	IEC_WORD  EventType;
	IEC_WORD  NotifyType;
	IEC_WORD  FromState;
	IEC_WORD  ToState;

	IEC_UDINT Buff_PrevNotif;
	IEC_UDINT Buff_CurrNotif;

	IBACnetTimeStampEx *pTimeStamp;

	/* 7 | 6 | 5 | 4 | 3 | 2 | 1 | AckRequired */

	IEC_BYTE	bits;

} IBACnetEventEx;


/* BACnet IO Layer Library Functions
 * ----------------------------------------------------------------------------
 */
typedef struct
{
	DEC_FUN_PTR(IEC_STRING, Object_Name);

	DEC_FUN_PTR(IEC_DINT,	ObjectID);
	DEC_FUN_PTR(IEC_DINT,	DeviceID);
	DEC_FUN_PTR(IEC_DINT,	TypeID);

	DEC_FUN_WORD(wRet);

} S_BAC_FindObject;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(DeviceID); 

	DEC_FUN_PTR(IEC_WORD, State);

	DEC_FUN_WORD(wRet);

} S_BAC_GetDevState;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);

	DEC_FUN_PTR(IEC_WORD, Quality);

	DEC_FUN_WORD(wRet);

} S_BAC_GetObjState;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);

	DEC_FUN_PTR(IEC_UDINT, Priority);

	DEC_FUN_WORD(wRet);

} S_BAC_GetObjPrio;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);

	DEC_FUN_UDINT(Priority);

	DEC_FUN_WORD(wRet);

} S_BAC_SetObjPrio;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_PTR(IEC_DINT, DeviceID);

	DEC_FUN_WORD(wRet);

} S_BAC_GetLocDeviceID;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_PTR(IEC_BOOL, Value);

	DEC_FUN_WORD(wRet);

} S_BAC_GP_Boolean;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_PTR(IEC_WORD, Value);

	DEC_FUN_WORD(wRet);

} S_BAC_GP_Enumerated;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_PTR(IEC_STRING, Value);

	DEC_FUN_WORD(wRet);

} S_BAC_GP_CharacterString;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_PTR(IEC_UDINT, Value);

	DEC_FUN_WORD(wRet);

} S_BAC_GP_Unsigned;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_PTR(IEC_REAL, Value);

	DEC_FUN_WORD(wRet);

} S_BAC_GP_Real;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_PTR(IEC_BYTE, Value);

	DEC_FUN_WORD(wRet);

} S_BAC_GP_Bitstring;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_PTR(IBACnetDateTime, Value);

	DEC_FUN_WORD(wRet);

} S_BAC_GP_DateTime;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_PTR(IBACnetStatusFlags, Value);

	DEC_FUN_WORD(wRet);

} S_BAC_GP_StatusFlags;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_PTR(IBACnetLimitEnable, Value);

	DEC_FUN_WORD(wRet);

} S_BAC_GP_LimitEnable;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_PTR(IBACnetEventTransition, Value);

	DEC_FUN_WORD(wRet);

} S_BAC_GP_EventTransition;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_PTR(IBACnetAny, Value);

	DEC_FUN_WORD(wRet);

} S_BAC_GP_Any;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_PTR(IBACnetDateRange, Value);

	DEC_FUN_WORD(wRet);

} S_BAC_GP_DateRange;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_PTR(IBACnetDevObjPropRef, Value);

	DEC_FUN_WORD(wRet);

} S_BAC_GP_DevObjPropReference;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_PTR(IEC_UDINT, Elements);
	DEC_FUN_PTR(IEC_DATA,  Value);	/* IEC array of UDINT */

	DEC_FUN_WORD(wRet);

} S_BAC_GPA_Unsigned;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_PTR(IEC_UDINT, Elements);
	DEC_FUN_PTR(IEC_DATA,  Value);	/* IEC array of WORD */

	DEC_FUN_WORD(wRet);

} S_BAC_GPA_Enumerated;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_PTR(IEC_UDINT, Elements);
	DEC_FUN_PTR(IEC_DATA,  Value);	/* IEC array of REAL */

	DEC_FUN_WORD(wRet);

} S_BAC_GPA_Real;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_PTR(IEC_UDINT, Elements);
	DEC_FUN_PTR(IEC_DATA,  Value);	/* IEC array of STRING */

	DEC_FUN_WORD(wRet);

} S_BAC_GPA_CharacterString;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_PTR(IEC_UDINT, Elements);
	DEC_FUN_PTR(IEC_DATA,  Value);	/* IEC array of IBACnetTimeStamp */

	DEC_FUN_WORD(wRet);

} S_BAC_GPA_TimeStamp;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_PTR(IEC_UDINT, Elements);
	DEC_FUN_PTR(IEC_DATA,  Value);	/* IEC array of IBACnetDevObjPropRef */

	DEC_FUN_WORD(wRet);

} S_BAC_GPA_DevObjPropReference;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_PTR(IEC_UDINT, Elements);
	DEC_FUN_PTR(IEC_DATA,  Value);	/* IEC array of IBACnetDailySchedule */

	DEC_FUN_WORD(wRet);

} S_BAC_GPA_DailySchedule;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_PTR(IEC_UDINT, Elements);
	DEC_FUN_PTR(IEC_DATA,  Value);	/* IEC array of IBACnetSpecialEvent */

	DEC_FUN_WORD(wRet);

} S_BAC_GPA_SpecialEvent;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_BOOL(Value);

	DEC_FUN_WORD(wRet);

} S_BAC_SP_Boolean;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_WORD(Value);

	DEC_FUN_WORD(wRet);

} S_BAC_SP_Enumerated;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_PTR(IEC_STRING, Value);

	DEC_FUN_WORD(wRet);

} S_BAC_SP_CharacterString;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_UDINT(Value);

	DEC_FUN_WORD(wRet);

} S_BAC_SP_Unsigned;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_REAL(Value);

	DEC_FUN_WORD(wRet);

} S_BAC_SP_Real;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_BYTE(Value);

	DEC_FUN_WORD(wRet);

} S_BAC_SP_Bitstring;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_PTR(IBACnetDateTime, Value);

	DEC_FUN_WORD(wRet);

} S_BAC_SP_DateTime;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_PTR(IBACnetLimitEnable, Value);

	DEC_FUN_WORD(wRet);

} S_BAC_SP_LimitEnable;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_PTR(IBACnetEventTransition, Value);

	DEC_FUN_WORD(wRet);

} S_BAC_SP_EventTransition;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_PTR(IBACnetAny, Value);

	DEC_FUN_WORD(wRet);

} S_BAC_SP_Any;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_PTR(IBACnetDateRange, Value);

	DEC_FUN_WORD(wRet);

} S_BAC_SP_DateRange;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_PTR(IBACnetDevObjPropRef, Value);

	DEC_FUN_WORD(wRet);

} S_BAC_SP_DevObjPropReference;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_UDINT(Elements);
	DEC_FUN_PTR(IEC_DATA, Value);

	DEC_FUN_WORD(wRet);

} S_BAC_SPA_Unsigned;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_UDINT(Elements);
	DEC_FUN_PTR(IEC_DATA, Value);

	DEC_FUN_WORD(wRet);

} S_BAC_SPA_CharacterString;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_UDINT(Elements);
	DEC_FUN_PTR(IEC_DATA, Value);

	DEC_FUN_WORD(wRet);

} S_BAC_SPA_DevObjPropReference;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_UDINT(Elements);
	DEC_FUN_PTR(IEC_DATA, Value);

	DEC_FUN_WORD(wRet);

} S_BAC_SPA_DailySchedule;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_UDINT(Elements);
	DEC_FUN_PTR(IEC_DATA, Value);

	DEC_FUN_WORD(wRet);

} S_BAC_SPA_SpecialEvent;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_TIME(PollRate);
	
	DEC_FUN_WORD(wRet);

} S_BAC_SubscribeProperty;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(PropertyID);

	DEC_FUN_WORD(wRet);

} S_BAC_UnSubscribeProperty;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	
	DEC_FUN_WORD(wRet);

} S_BAC_EventSubscribe;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 

	DEC_FUN_WORD(wRet);

} S_BAC_EventUnSubscribe;

/* ---------------------------------------------------------------------------- */

typedef struct 
{
	DEC_FUN_PTR(IBACnetEvent, pEvent);

	DEC_FUN_BOOL(bDeleteEvent);

	DEC_FUN_WORD(wRet);

} S_BAC_EventGet;

/* ---------------------------------------------------------------------------- */

typedef struct 
{
	DEC_FUN_PTR(IEC_STRING, Line1);
	DEC_FUN_PTR(IEC_STRING, Line2);

	DEC_FUN_BOOL(bDeleteEvent);

	DEC_FUN_WORD(wRet);

} S_BAC_EventGetString;

/* ---------------------------------------------------------------------------- */

typedef struct 
{
	DEC_FUN_PTR(IBACnetEvent, pEvent);

	DEC_FUN_WORD(wRet);

} S_BAC_EventAcknowledge;

/* ---------------------------------------------------------------------------- */

typedef struct 
{
	DEC_FUN_PTR(IEC_WORD, pRes);

	DEC_FUN_WORD(wRet);

} S_BAC_GetConfigRes;

/* ---------------------------------------------------------------------------- */

typedef struct 
{
	DEC_FUN_PTR(IEC_DINT, pDeviceID);
	DEC_FUN_PTR(IEC_BOOL, pTransitionToGood);

	DEC_FUN_WORD(wRet);

} S_BAC_GetDevTrans;

/* ---------------------------------------------------------------------------- */

typedef struct 
{
	DEC_FUN_PTR(IBACnetEvent, pEvent);

	DEC_FUN_PTR(IEC_UDINT, pNoOfRecords);
	DEC_FUN_PTR(IEC_DATA,  pRecords);		/* ARRAY of BACnetLogRecord */

	DEC_FUN_WORD(wRet);

} S_BAC_GetLogBuffer;

/* ---------------------------------------------------------------------------- */

typedef struct 
{
	DEC_FUN_DINT(iDeviceID);
	DEC_FUN_WORD(wToDo);

	DEC_FUN_PTR(IEC_STRING, PassWord);

	DEC_FUN_WORD(wRet);
	
} S_BAC_ReinitializeDev;

/* ---------------------------------------------------------------------------- */

typedef struct 
{
	DEC_FUN_DINT(iDeviceID);
	DEC_FUN_WORD(wTodo);
	DEC_FUN_TIME(tDuration);

	DEC_FUN_PTR(IEC_STRING, PassWord);

	DEC_FUN_WORD(wRet);

} S_BAC_DevCommControl;

/* ---------------------------------------------------------------------------- */

typedef struct 
{
	DEC_FUN_DINT(iDeviceID);
	DEC_FUN_DINT(iObjectID);
	
	DEC_FUN_PTR(IEC_STRING, FileName);

	DEC_FUN_WORD(wRet);

} S_BAC_ReadFile;

/* ---------------------------------------------------------------------------- */

typedef struct 
{
	DEC_FUN_DINT(iDeviceID);
	DEC_FUN_DINT(iObjectID);
	
	DEC_FUN_PTR(IEC_STRING, FileName);

	DEC_FUN_WORD(wRet);

} S_BAC_WriteFile;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(Priority);

	DEC_FUN_WORD(PresValue);

	DEC_FUN_WORD(wRet);

} S_BAC_SPV_Enumerated;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(Priority);

	DEC_FUN_UDINT(PresValue);

	DEC_FUN_WORD(wRet);

} S_BAC_SPV_Unsigned;

/* ---------------------------------------------------------------------------- */

typedef struct
{
	DEC_FUN_DINT(ObjectID); 
	DEC_FUN_DINT(DeviceID); 
	DEC_FUN_DINT(TypeID);
	DEC_FUN_DINT(Priority);

	DEC_FUN_REAL(PresValue);

	DEC_FUN_WORD(wRet);

} S_BAC_SPV_Real;

/* ---------------------------------------------------------------------------- */

#define RTS_PRAGMA_PACK_DEF 	
#include "osAlign.h"
#undef	RTS_PRAGMA_PACK_DEF 	/* <<<< Align 1 end <<<<<<<<<<<<<<<<<<<<<<<<<<< */

#endif	/* RTS_CFG_BACNET */ 

#endif	/* _BACLIB_H_ */

/* ---------------------------------------------------------------------------- */

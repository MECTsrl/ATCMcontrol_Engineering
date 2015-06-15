/* $Header: /4CReleased/V2.20.00/Target/4CWin/INC/bacShared.h 1     28.02.07 19:10 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: bacShared.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/4CWin/INC/bacShared.h $
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

#ifndef _BACSHARED_H_
#define _BACSHARED_H_

/* bacShared.h
 * ----------------------------------------------------------------------------
 * This file is supposed to be included both on the PC (CG, OPC) and on the 
 * Run Time System side and includes the configuration structures for the 
 * BACnet IO Layer.
 *
 * Configuration structures must begin with an 'X' and are supposed to be two byte
 * alligned. Don't add anything not commonly shared between the the PC components 
 * and the Run Time System.
 *
 * IMPORTANT:	Always ensure, that the structures of this file are compile with 
 * ==========	an one byte aligment both on the Engineering as on the Run Time
 *				System side!
 */

#if ! defined(INC_RTS)

  /* Include in a windows component. Must only be included from within vmSharedDef.h
   * in order to assure the correct alignement settings.
   */

  #if !defined(_VMSHAREDDEF_H_)
	#error "Include via vmSharedDef.h only!"
  #endif

  /* Structures in this file MUST be compiled with one byte alignment!
   */
  #pragma pack(1)

  #else

  /* Included in the Run Time System. Must only be included via stdInc.h in order
   * to have the correct include order.
   */
  #if !defined(_STDINC_H_)
	#error "Include via stdInc.h only!"
  #endif

  /* Structures in this file MUST be compiled with one byte alignment!
   */
  #define RTS_PRAGMA_PACK_1
  #include "osAlign.h"
  #undef  RTS_PRAGMA_PACK_1

#endif

#include "common_enum.h"


/* Configuration File Names
 * ----------------------------------------------------------------------------
 */
#define BAC_CFG_STACK				"bacStk.cfg"
#define BAC_CFG_DEVICE				"bacDev.cfg"
#define BAC_CFG_OBJ_LOC 			"bacLoc.cfg"
#define BAC_CFG_OBJ_REM 			"bacRem.cfg"
#define BAC_CFG_LOC_INIVAL			"bacIni.cfg"


/* BACnet Time Synchronization
 * ----------------------------------------------------------------------------
 */
#define BAC_TIMESYNC_OFF			0
#define BAC_TIMESYNC_SLAVE			1
#define BAC_TIMESYNC_MASTER 		2
#define BAC_TIMESYNC_MASTER_UTC 	4


/* BACnet Stack Configuration (includes Definition for Local Device Object)
 * ----------------------------------------------------------------------------
 */
typedef struct		/* --> aligned (2) */
{
	DEC_VAR(IEC_DINT,  iDeviceID);			/* Device ID - Local BACnet Device. 		0 .. 0x3fffff					*/

	DEC_VAR(IEC_DINT,  iDefPrioLoc);		/* Default priority for local objects.		0 .. 16 (0) 					*/
	DEC_VAR(IEC_DINT,  iDefPrioRem);		/* Default priority for remote objects. 	1 .. 16 (16)					*/

	DEC_VAR(IEC_UDINT, ulNoLocObj); 		/* Number of local BACnet objects.			0 .. 2000 (local + remote)		*/ 
	DEC_VAR(IEC_UDINT, ulNoRemObj); 		/* Number of remote BACnet objects. 		0 .. 2000						*/

	DEC_VAR(IEC_DINT,  iCOVLifeTime);		/* COV subscription lifetime in seconds.	0 .. MAX_INT32(3600)			*/
	DEC_VAR(IEC_UDINT, bConfirmedCOV);		/* Use confirmed COV notifications. 		TRUE | FALSE (TRUE) 			*/

	DEC_VAR(IEC_UDINT, ulNetworkNo);		/* BACnet network number.					1 .. 65534 (1)					*/
	DEC_VAR(IEC_UDINT, ulUDPPortNo);		/* UDP port number							1 .. MAX_INT16	(47808) 		*/

	DEC_VAR(IEC_UDINT, ulTimeSyncMode); 	/* BACnet time synchronization mode.		0 .. 4 (0)						*/
	DEC_VAR(IEC_UDINT, ulTimeSyncInterval); /* Sync. interval in seconds if master. 	1 .. MAX_UDINT (21600)			*/
	DEC_VAR(IEC_CHAR,  szIPTimeServer[16]); /* IP address of external SNTP time server. <valid IP address> ("") 		*/
											/* (Only used if acting as BACnet time server.) 							*/

	DEC_VAR(IEC_UDINT, ulMinOfflineTime);	/* Min. configuration off line time.		0 .. MAX_UDINT (0)				*/
	DEC_VAR(IEC_UDINT, bDisablePersDB); 	/* Disable object persistence database. 	TRUE | FALSE (FALSE)			*/
	DEC_VAR(IEC_UDINT, bDontUnsubCOV);		/* Don't un-subscribe COV notifications.	TRUE | FALSE (FALSE)			*/
	DEC_VAR(IEC_UDINT, ulEnableLogging);	/* Enable BACnet logging.					0..MAX_UDINT (0)				*/

	DEC_VAR(IEC_UDINT, ulAPDUMaxLength);	/* Max. APDU length accepted.				50|128|206|480|1024|1476 (480)	*/
	DEC_VAR(IEC_UDINT, ulAPDUTimeOut);		/* APDU time out.							0 .. MAX_UDINT (6000)			*/
	DEC_VAR(IEC_UDINT, ulAPDUSegTimeOut);	/* APDU segment time out.					0 .. MAX_UDINT (6000)			*/
	DEC_VAR(IEC_UDINT, ulAPDURetries);		/* No. of APDU retries. 					0 .. MAX_UDINT (3)				*/
	
	DEC_VAR(IEC_CHAR,  szBBMDAddress[16]);	/* IP address of BBMD server.				<valid IP address> ("") 		*/
											/* (If != "" act as Foreign Device.)										*/
	DEC_VAR(IEC_UDINT, ulBBMDUDPPortNo);	/* UDP port number of BBMD server.			1 .. MAX_SHORT (47808)			*/
	DEC_VAR(IEC_UDINT, ulBBMDMaxFD);		/* Number of accepted FD's. 				0 .. MAX_UDINT (0)				*/
											/* (If > 0 act as BBMD Server.) 											*/

	DEC_VAR(BACnetSegmentation, eSegSupp);	/* Supported Segmentation.					0 .. 4 (0)						*/

	DEC_VAR(IEC_UDINT, ulReserved1);		/* Reserved for future use. 												*/
	DEC_VAR(IEC_UDINT, ulReserved2);
	DEC_VAR(IEC_UDINT, ulReserved3);
	DEC_VAR(IEC_UDINT, ulReserved4);

	DEC_VAR(IEC_UDINT, ulSMOffset); 		/* Offset of device struct in Shared Memory.								*/
	DEC_VAR(IEC_UDINT, ulNameLen);			/* Length of local device name - without ZB!								*/
	DEC_VAR(IEC_CHAR,  szName[0]);			/* Local device name - including ZB!										*/

} XBACnetStack; 						/* ------------------------------------ */


/* BACnet Remote Device Definition
 * ----------------------------------------------------------------------------
 */
typedef struct		/* --> aligned (2) */
{
	DEC_VAR(IEC_DINT,  iDeviceID);		/* Device ID - BACnet Device.			*/
	DEC_VAR(IEC_UDINT, ulSMOffset); 	/* Offset in Shared Memory. 			*/

	DEC_VAR(IEC_UDINT, bDisableConnMon);/* Disable connection monitoring.		*/

	DEC_VAR(IEC_UDINT, ulReserved1);	/* Reserved for future use. 			*/
	DEC_VAR(IEC_UDINT, ulReserved2);
	DEC_VAR(IEC_UDINT, ulReserved3);
	DEC_VAR(IEC_UDINT, ulReserved4);

	DEC_VAR(IEC_UDINT, ulNameLen);		/* Length of device name - without ZB!	*/
	DEC_VAR(IEC_CHAR,  szName[0]);		/* Object name - including ZB!			*/

} XBACnetDevice;


/* BACnet Object Definition
 * ----------------------------------------------------------------------------
 */
typedef struct		/* --> aligned (2) */
{
	DEC_VAR(BACnetObjectType, eType);	/* Type of BACnet object.				*/

	DEC_VAR(IEC_DINT,  iDeviceID);		/* Device ID of the BACnet server.		*/
	DEC_VAR(IEC_DINT,  iObjectID);		/* Object ID of BACnet object.			*/
	DEC_VAR(IEC_UDINT, ulSMOffset); 	/* Offset in Shared Memory. 			*/

	DEC_VAR(IEC_UDINT, ulOffIniVal);	/* Offset in initial value file.		*/
	DEC_VAR(IEC_UDINT, ulNoOfIniVal);	/* Number of initial values.			*/
	
	DEC_VAR(IEC_UDINT, ulPersistent);	/* Persistence mode. (Default: 1)		*/
	DEC_VAR(IEC_UDINT, ulReserved); 	/* Reserved for future use				*/

	DEC_VAR(IEC_UINT,  bSupportsCOV);	/* Supports COV (only remote objects)	*/
	DEC_VAR(IEC_UINT,  bCommandable);	/* Object is command able (remote obj.) */

	DEC_VAR(IEC_UDINT, ulNameLen);		/* Length of object name - without ZB!	*/	
	DEC_VAR(IEC_CHAR,  szName[0]);		/* Object name - including ZB!			*/

} XBACnetObj;							/* ------------------------------------ */


/* BACnet Property Initial Value Definition
 * ----------------------------------------------------------------------------
 */
typedef struct		/* --> aligned (2) */
{
	DEC_VAR(BACnetPropertyIdentifier, eType);	/* Type of Initial value.		*/

	DEC_VAR(IEC_DINT,  iObjectID);		/* Object ID of BACnet object.			*/

	DEC_VAR(IEC_UINT,  uDataSize);		/* Data size in bytes.					*/
	DEC_VAR(IEC_UINT,  uArrayElem); 	/* Number of array elements 			*/

	DEC_VAR(IEC_BYTE,  pData[0]);		/* Initial value						*/

} XBACnetIniVal;						/* ------------------------------------ */


/* BACnetAny
 * ----------------------------------------------------------------------------
 */
#define ANY_NULL	0
#define ANY_BOOL	1
#define ANY_REAL	2
#define ANY_WORD	3
#define ANY_UDINT	4

typedef struct		/* --> aligned (2) */  
{
	DEC_VAR(IEC_WORD,	wType); 		/* Type of value						*/

	DEC_VAR(IEC_WORD,	word_value);	/* Word value (of type 'wType') 		*/
	DEC_VAR(IEC_REAL,	real_value);	/* Real value (of type 'wType') 		*/
	DEC_VAR(IEC_UDINT,	udint_value);	/* UDINT value (of type 'wType')		*/
	DEC_VAR(IEC_BOOL,	bool_value);	/* BOOL value (of type 'wType') 		*/
	DEC_VAR(IEC_BYTE,	dummy08);		/* Alignment dummy						*/
	DEC_VAR(IEC_WORD,	dummy16);		/* Alignment dummy						*/

} XBACnetAny;							/* ------------------------------------ */


/* BACnetDate
 * ----------------------------------------------------------------------------
 */
typedef struct		/* --> aligned (2) */
{
	DEC_VAR(IEC_UINT,	year);
	DEC_VAR(IEC_UINT,	month);
	DEC_VAR(IEC_UINT,	day);
	DEC_VAR(IEC_UINT,	dayOfWeek);
	
} XBACnetDate;


/* BACnetTime
 * ----------------------------------------------------------------------------
 */
typedef struct		/* --> aligned (2) */
{
	DEC_VAR(IEC_UINT,	hour);
	DEC_VAR(IEC_UINT,	minute);
	DEC_VAR(IEC_UINT,	second);
	DEC_VAR(IEC_UINT,	hundrethsOfASec);

} XBACnetTime;


/* BACnetWeekNDay
 * ----------------------------------------------------------------------------
 */
typedef struct		/* --> aligned (2) */
{
	DEC_VAR(IEC_UINT,	month);
	DEC_VAR(IEC_UINT,	weekOfMonth);
	DEC_VAR(IEC_UINT,	dayOfWeek);

} XBACnetWeekNDay;


/* BACnetDateTime
 * ----------------------------------------------------------------------------
 */
typedef struct		/* --> aligned (2) */
{
	DEC_VAR(XBACnetDate, Date);
	DEC_VAR(XBACnetTime, Time);

} XBACnetDateTime;


/* BACnetDateRange
 * ----------------------------------------------------------------------------
 */
typedef struct		/* --> aligned (2) */
{
	DEC_VAR(XBACnetDate, Start);
	DEC_VAR(XBACnetDate, End);

} XBACnetDateRange;


/* BACnetTimeValue
 * ----------------------------------------------------------------------------
 */
typedef struct		/* --> aligned (2) */  
{
	DEC_VAR(XBACnetTime, Time); 
	DEC_VAR(XBACnetAny,  Value);

} XBACnetTimeValue;


/* BACnetCalendarEntry
 * ----------------------------------------------------------------------------
 */
typedef struct		/* --> aligned (2) */  
{
	DEC_VAR(IEC_WORD,	wType); 		/* Type of calendar entry				*/

	DEC_VAR(XBACnetDateRange,	DateRange);
	DEC_VAR(XBACnetWeekNDay,	WeekNDay);

} XBACnetCalendarEntry;


/* BACnetDeviceObjectPropertyReference
 * ----------------------------------------------------------------------------
 */
typedef struct		/* --> aligned (2) */
{
	DEC_VAR(BACnetObjectType, eObjType);
	DEC_VAR(IEC_DINT,  iObjectID);
	DEC_VAR(IEC_DINT,  iDeviceID);
	DEC_VAR(BACnetPropertyIdentifier, ePropType);
	DEC_VAR(IEC_DINT,  iArrayIndex);

} XBACnetDevObjPropRef;


/* BACnetDailySchedule
 * ----------------------------------------------------------------------------
 */
typedef struct		/* --> aligned (2) */  
{
	DEC_VAR(IEC_UDINT,	ulElements);	/* Number of day TimeValue objects. 	*/

	/* uElements objects of type XBACnetTimeVal to follow.
	 */
	DEC_VAR(XBACnetTimeValue, pDaySched[0]);	/* TimeValue objects.			*/

} XBACnetDailySchedule; 				/* ------------------------------------ */


/* BACnetSpecialEvent
 * -----------------------------------------------------------------------------
 */
typedef struct		/* --> aligned (2)	*/ 
{
	DEC_VAR(IEC_WORD,	wType); 		/* Type of period						*/
	DEC_VAR(IEC_DINT,	ObjectID);		/* Corresponds to CalendarReference of	*/
	DEC_VAR(IEC_DINT,	TypeID);		/* type BACnetObjectIdentifier. 		*/
	
	DEC_VAR(XBACnetCalendarEntry, CalEntry);

	DEC_VAR(IEC_UDINT,	EventPriority);
	
	DEC_VAR(IEC_UDINT,	ulElements);	/* Number of day TimeValue objects. 	*/

	/* uElements objects of type XBACnetTimeVal to follow.
	 */
	DEC_VAR(XBACnetTimeValue, pTimeVal[0]);

} XBACnetSpecialEvent;


/* Structures in this file MUST be compiled with one byte alignment!
 */
#if ! defined(INC_RTS)

  #pragma pack()

#else

  #define RTS_PRAGMA_PACK_DEF
  #include "osAlign.h"
  #undef  RTS_PRAGMA_PACK_DEF

#endif

#endif	/* _BACSHARED_H_ */

/* ---------------------------------------------------------------------------- */

/* $Header: /4CReleased/V2.20.00/Target/4CWin/CONTROL/inc.bac/bacDef.h 1     28.02.07 19:10 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: bacDef.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/4CWin/CONTROL/inc.bac/bacDef.h $
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

#ifndef _BACDEF_H_
#define _BACDEF_H_

#if defined(INC_4C)
#error "Must not be included in 4C!"
#endif

#if !defined(_STDINC_H_)
#error "Include via stdInc.h only!"
#endif

#include "bacnapi.h"


/* Vendor Informations
 * ----------------------------------------------------------------------------
 */
#define BAC_VENDOR_ID				187
#define BAC_VENDOR_NAME 			"ATCM"
#define BAC_PROFILE_NAME			"187- ATCMControl V2.20.00"


/* BACnet Definitions
 * ----------------------------------------------------------------------------
 */
#define BAC_COV_PROC_ID 			2709ul


/* BACnet Time Out and Interval Values
 * ----------------------------------------------------------------------------
 */
#define BAC_SCN_INTERVAL		   1020ul		/* Scanner thread interval time.	*/
#define BAC_SCN_CYCLE				120ul		/* Scanner thread cycle time.		*/
#define BAC_SCN_BREAK				120ul		/* Delay time between work cycles.	*/
#define BAC_SCN_WORK				120ul		/* Maximum Scanner working time.	*/
#define BAC_SCN_MAX_OBJ 			 40ul		/* Maximum no. of objects per scan. */

#define BAC_COV_INTERVAL		   5040ul		/* COV thread interval time.		*/
#define BAC_COV_CYCLE				220ul		/* COV thread cycle time.			*/
#define BAC_COV_BREAK				 60ul		/* Delay time between work cycles.	*/
#define BAC_COV_WORK				500ul		/* Maximum COV thread working time. */
#define BAC_COV_TO				  10000ul		/* Time out for COV acknowledges.	*/
#define BAC_COV_RETRY			  10000ul		/* Waiting time after COV time out. */

#define BAC_IAM_INTERVAL		   5000ul		/* Cyclic I-Am interval time.		*/
#define BAC_WHOIS_INTERVAL		   7000ul		/* Cyclic Who-Is interval time. 	*/
#define BAC_TIMESYNC_INTERVAL	  31000ul		/* Cyclic Time-Sync interval.		*/
#define BAC_DEVICE_CYCLE			380ul		/* Device thread cycle time.		*/
#define BAC_IAM_TO				   3000ul		/* Time out for I-Am response.		*/			

#define BAC_FLH_INTERVAL		   1040ul		/* Flash thread interval time.		*/
#define BAC_FLH_CYCLE				140ul		/* Scanner thread cycle time.		*/
#define BAC_FLH_BREAK				140ul		/* Delay time between work cycles.	*/
#define BAC_FLH_WORK				250ul		/* Maximum flashing working time.	*/
#if defined(DEBUG)
#define BAC_FLH_FLASH			   2000ul		/* Flash interval time. 			*/
#else
#define BAC_FLH_FLASH			  20000ul		/* Flash interval time. 			*/
#endif


/* BACnet IEC Objects
 * ----------------------------------------------------------------------------
 */
#define MAX_OBJECT_NAME_LEN 		50


/* BACnet Object Qualities
 * ----------------------------------------------------------------------------
 */
#define BAC_QUAL_NOT_INIT			0x0000u
#define BAC_QUAL_INIT				0x0001u
#define BAC_QUAL_COV				0x0002u

#define BAC_QUAL_GOOD				0x0100u
#define BAC_QUAL_BAD				0x0200u

#define BAC_QUAL_ERROR				0x8000u


/* IEC Property Offsets Index
 * ----------------------------------------------------------------------------
 */
#define BAC_PROP_SM_OBJ_ID			0u
#define BAC_PROP_SM_OBJ_NAME		1u
#define BAC_PROP_SM_PRES_VALUE		2u
#define BAC_PROP_SM_STATUS_FLAGS	3u
#define BAC_PROP_SM_QUALITY 		4u
#define BAC_PROP_SM_CMD_VALUE		5u
#define BAC_PROP_SM_DEV_ID			6u

#define BAC_PROP_SM_COUNT			7u		


/* BACnet Priority Values
 * ----------------------------------------------------------------------------
 */
#define BAC_PRIO_GATEWAY			0

#define BAC_PRIO_MIN				1
#define BAC_PRIO_MAX			   16


/* Present-Value NULL values
 * ----------------------------------------------------------------------------
 */
#define BAC_PV_NULL_A				((IEC_REAL) (3.4E-38))			/* Analog objects		*/
#define BAC_PV_NULL_B				((IEC_WORD) (0xffffu))			/* Binary objects		*/
#define BAC_PV_NULL_M				((IEC_UDINT)(0x00000000ul)) 	/* Multi-state objects	*/


/* PV Initial Values
 * ----------------------------------------------------------------------------
 */
#define BAC_PV_INIT_A				((IEC_REAL) (0.0))				/* Analog objects		*/
#define BAC_PV_INIT_B				((IEC_WORD) (0x0000u))			/* Binary objects		*/
#define BAC_PV_INIT_M				((IEC_UDINT)(0x0001u))			/* Multi-state objects	*/


/* Status-Flag Bit Masks
 * ----------------------------------------------------------------------------
 */
#define BAC_SF_IN_ALARM 			0x0008u
#define BAC_SF_FAULT				0x0004u
#define BAC_SF_OVERRIDDEN			0x0002u
#define BAC_SF_OUT_OF_SERVICE		0x0001u


/* BACnetAny Types
 * ----------------------------------------------------------------------------
 */
#define BAC_ANY_NULL				0
#define BAC_ANY_BOOL				1
#define BAC_ANY_REAL				2
#define BAC_ANY_WORD				3
#define BAC_ANY_UDINT				4
#define BAC_ANY_DINT				5
#define BAC_ANY_FAILURE 			6
#define BAC_ANY_TIMECHANGE			7
#define BAC_ANY_BITSTRING			8
#define BAC_ANY_LOGSTATUS			9

#define BAC_ANY_NOT_SUPPORTED		0xffffu


/* BACnAPI Compatibility
 * ----------------------------------------------------------------------------
 */
#define BACNAPI_OK					0			/* Sucessfull return value BACnAPI				*/
#define BACTEMP_OK					0xFF		/* Sucessfull return value Template Library.	*/

#define TO_DT						0			/* bacnapi_access_xxx convert to BACnetDataType 	*/
#define FROM_DT 					1			/* bacnapi_access_xxx convert from BACnetDataType	*/

#define BAC_BUILDKEY(t,id)			((id) | ((t) << 22))


/* BACnet Logging Features
 * ----------------------------------------------------------------------------
 */
#define BACLOG_APPLICATION_LAYER	0x00000001ul
#define BACLOG_NETWORK				0x00000002ul
#define BACLOG_CLIENT_TSM			0x00000004ul
#define BACLOG_SERVER_TSM			0x00000008ul
#define BACLOG_SERVICE_INDICATION	0x00000010ul
#define BACLOG_ADDRESS_BINDING		0x00000020ul
#define BACLOG_ETHERNET 			0x00000040ul
#define BACLOG_ANEX_H				0x00000080ul
#define BACLOG_IP					0x00000100ul
#define BACLOG_MSTP 				0x00000200ul
#define BACLOG_PTP					0x00000400ul
#define BACLOG_API					0x00000800ul
#define BACLOG_DATALINK 			0x00001000ul
#define BACLOG_DATATYPE_UTILS		0x00002000ul
#define BACLOG_DRIVER				0x00004000ul
#define BACLOG_MEM					0x00008000ul
#define BACLOG_OBJECT				0x00010000ul
#define BACLOG_CACHE				0x00020000ul
#define BACLOG_MAC					0x00040000ul
#define BACLOG_OBJECT_API			0x00080000ul
#define BACLOG_SERIALIZE			0x00100000ul
#define BACLOG_PDU					0x00200000ul
#define BACLOG_DATABASE 			0x00400000ul


/* Other Defines
 * ----------------------------------------------------------------------------
 */
#define BAC_FLASH_SECTOR			0x8000u


#ifndef _BACIEC_H_
#include "bacIec.h"
#endif


/* Event Buffer
 * ----------------------------------------------------------------------------
 */
#define BAC_MAX_EVENTS				25


/* Forward Declarations
 * ----------------------------------------------------------------------------
 */
class CBACnetObj;


/* class CListObj
 * ----------------------------------------------------------------------------
 */
class CListObj
{
	public:
		CListObj();
		virtual ~CListObj();

	public:
		virtual IEC_DINT GetListID(void) = 0;
		virtual IEC_DINT GetListID2(void);
};


/* class CBACnetEvent
 * ----------------------------------------------------------------------------
 */
class CBACnetEvent : public CListObj
{
	public:
		CBACnetEvent();
		virtual ~CBACnetEvent();

	public:
		virtual IEC_DINT GetListID(void);

	public:
		IEC_UINT Init(BACnetServiceEventNotificationRequest *pEvent);
		
	public:
		IEC_UINT GetInfoString(IEC_CHAR *szInfo, IEC_UDINT ulDataLen);
		IEC_UINT GetValueString(IEC_CHAR *szInfo, IEC_UDINT ulDataLen);

	public:
		IEC_UINT GetEvent(IEC_DATA *pData);

	public:
		BACnetServiceEventNotificationRequest *m_pEvent;
};


/* class CBACnetEventSubs
 * ----------------------------------------------------------------------------
 */
class CBACnetEventSubs : public CListObj
{
	public:
		CBACnetEventSubs();
		virtual ~CBACnetEventSubs();

	public:
		virtual IEC_DINT GetListID(void);
		virtual IEC_DINT GetListID2(void);

	public:
		IEC_UINT Init(IEC_DINT iDeviceID, IEC_DINT iObjectID);

		IEC_DINT GetDeviceID(void);
		IEC_DINT GetObjectID(void);

	protected:
		IEC_DINT	m_iDeviceID;
		IEC_DINT	m_iObjectID;
};


/* class CDevTrans
 * ----------------------------------------------------------------------------
 */
class CDevTrans : public CListObj
{
	public:
		CDevTrans();
		virtual ~CDevTrans();

	public:
		virtual IEC_DINT GetListID(void);

	public:
		IEC_UINT Init(IEC_DINT iDeviceID, IEC_BOOL bTransitionToGood);

	public:
		IEC_BOOL IsTransitionToGood(void);

	protected:
		IEC_DINT	m_iDeviceID;
		IEC_BOOL	m_bTransitionToGood;
};


/* class CBACnetProperty
 * ----------------------------------------------------------------------------
 */
class CBACnetProperty : public CListObj
{
	public:
		CBACnetProperty();
		virtual ~CBACnetProperty();

	public:
		virtual IEC_DINT GetListID(void);

	public:
		IEC_UINT Subscribe(IEC_DINT iDeviceID, IEC_DINT iObjectID, BACnetPropertyIdentifier eProperty, BACnetObjectType eObjType, IEC_UDINT ulPollRate);
		IEC_UINT SetPollRate(IEC_UDINT ulPollRate);
		IEC_UINT UnSubscribe(void);

	protected:
		BACnetPropertyIdentifier	m_eProperty;
		BACnetObjectType			m_eObjectType;

		IEC_UDINT	m_ulPollRate;
};


/* class CBACnetList
 * ----------------------------------------------------------------------------
 */
class CBACnetList
{
	private:
		CBACnetList();

	public:
		CBACnetList(IEC_UDINT ulMaxObjects, IEC_UDINT ulIncrement, IEC_BOOL bFreeObjects, IEC_BOOL bSortList);
		~CBACnetList();

	public:
		IEC_UINT	AddObject(CListObj *pObj);
		IEC_UINT	DeleteAllObjects(void);
		IEC_UDINT	GetSize(void);

	public:
		CListObj	*FindElement(IEC_DINT iID);
		CListObj	*FindElement(IEC_DINT iID, IEC_DINT iID2);
		IEC_UDINT	FindIndex(IEC_DINT iID);
		IEC_UDINT	FindIndex(IEC_DINT iID, IEC_DINT iID2);

	public:
		CListObj	*GetElement(IEC_UDINT ulIndex);
		CListObj	*RemoveElement(IEC_UDINT ulIndex);

	protected:
		IEC_UDINT	m_ulIncrement;
		IEC_UDINT	m_ulMaxObjects;
		IEC_UDINT	m_ulObjects;
		
		IEC_BOOL	m_bFreeObjects;
		IEC_BOOL	m_bSortList;

	protected:
		CListObj **m_ppObj;
};


/* class CBACnetDevice
 * ----------------------------------------------------------------------------
 */
class CBACnetDevice : public CListObj
{	
	public:
		CBACnetDevice();
		virtual ~CBACnetDevice();

	public:
		virtual 	IEC_DINT GetListID(void);

	public:
		IEC_UINT	InitializeDevice(IEC_DINT iDeviceID, IEC_CHAR *szName, IEC_BOOL bDisConnMon);
		IEC_UINT	SMAttach(IEC_UDINT ulSMOffset, IEC_UINT uIOLayer, SIOConfig *pIO);

		IEC_UINT	WhoIs(void);
		IEC_UINT	HandleIAm(IEC_DINT iDeviceID);
		IEC_UINT	HandleStateTransit(void);
		IEC_DINT	GetDeviceID(void);
		IEC_UINT	AddObject(CListObj *pObj);

	public:
		IEC_UINT	Get_Quality(void);
		IEC_CHAR	*Get_Object_Name(void);

	protected:
		IEC_UINT	Set_Quality(IEC_WORD wQuality);
		IEC_UINT	Set_Object_Name(IEC_CHAR *szName);

	protected:
		IEC_DINT	m_iDeviceID;
		IEC_WORD	m_wQuality;

		IEC_UDINT	m_ulSMOffset;
		IEC_BOOL	m_bDisConnMon;
		IEC_CHAR	*m_szName;

		IEC_UDINT	m_ulLastWhoIs;
		IEC_UDINT	m_ulLastIAm;

		IEC_BOOL	m_bTransitGood;
		IEC_BOOL	m_bTransitBad;

		IEC_BOOL	m_bWhoIsPending;

	protected:
		CBACnetList *m_pObjects;

	protected:
		IEC_BOOL	m_bIEC;
		IEC_UDINT	m_ulSMSize; 		/* Size of Shared Memory area.		*/
		IEC_DATA	*m_pSM; 			/* Pointer to Shared Memory.		*/
		IEC_DATA	*m_pWF; 			/* Pointer to Write Flags.			*/

		SDV 		*m_pDV;
		IEC_WORD	m_pPropOff[BAC_PROP_SM_COUNT];
};


/* class CBACnetFile
 * ----------------------------------------------------------------------------
 */
class CBACnetFile
{
	public:
		CBACnetFile();
		virtual ~CBACnetFile();

	public:
		IEC_UINT	Open(void);
		IEC_UINT	OpenEx(void);
		IEC_UINT	Set(IEC_UDINT ulOffset);
		IEC_DATA   *Get(void);
		IEC_UINT	Close(void);

	protected:
		virtual IEC_UDINT GetSize(void) 				= 0;
		virtual IEC_UDINT GetDataSize(IEC_DATA *pData)	= 0;

	protected:
		IEC_UDINT	m_hFile;
		
		IEC_UDINT	m_ulLen;
		IEC_UDINT	m_ulDone;
		IEC_UDINT	m_ulEOF;
		IEC_BOOL	m_bRead;

		IEC_DATA	m_pData[2048];

	protected:
		IEC_CHAR	*m_szFile;
		IEC_CHAR	*m_szDir;
		IEC_UINT	(* m_fpGetDir) (IEC_CHAR *, IEC_UINT);
};


/* class CFileIniVal
 * ----------------------------------------------------------------------------
 */
class CFileIniVal : public CBACnetFile
{
	public:
		CFileIniVal();
		virtual ~CFileIniVal();

	protected:
		virtual IEC_UDINT GetSize(void);
		virtual IEC_UDINT GetDataSize(IEC_DATA *pData);
};


/* class CFileLocObj
 * ----------------------------------------------------------------------------
 */
class CFileLocObj : public CBACnetFile
{
	public:
		CFileLocObj();
		virtual ~CFileLocObj();

	protected:
		virtual IEC_UDINT GetSize(void);
		virtual IEC_UDINT GetDataSize(IEC_DATA *pData);
};


/* class CFileRemObj
 * ----------------------------------------------------------------------------
 */
class CFileRemObj : public CBACnetFile
{
	public:
		CFileRemObj();
		virtual ~CFileRemObj();

	protected:
		virtual IEC_UDINT GetSize(void);
		virtual IEC_UDINT GetDataSize(IEC_DATA *pData);
};


/* class CFileRemDev
 * ----------------------------------------------------------------------------
 */
class CFileRemDev : public CBACnetFile
{
	public:
		CFileRemDev();
		virtual ~CFileRemDev();

	protected:
		virtual IEC_UDINT GetSize(void);
		virtual IEC_UDINT GetDataSize(IEC_DATA *pData);
};


/* class CFileStack
 * ----------------------------------------------------------------------------
 */
class CFileStack : public CBACnetFile
{
	public:
		CFileStack();
		virtual ~CFileStack();

	protected:
		virtual IEC_UDINT GetSize(void);
		virtual IEC_UDINT GetDataSize(IEC_DATA *pData);
};


/* class CBACnet
 * ----------------------------------------------------------------------------
 */
class CBACnet
{		
	/* Construction & Destruction
	 * ------------------------------------------------------------------------
	 */
	public:
		CBACnet();
		~CBACnet();
	

	/* Initialization - bacStack.cpp
	 * ------------------------------------------------------------------------
	 */
	public:
		IEC_UINT  ReadConfiguration(void);
		IEC_UINT  StartUp(IEC_UINT uIOLayer, SIOConfig *pIO, IEC_BOOL bWarmstart);
		IEC_UINT  DestroyBACnetStack(void);

	protected:
		IEC_UINT  InitializeIOLayer(IEC_UINT uIOLayer, SIOConfig *pIO);
		IEC_UINT  InitializeBACnetDevices(void);
		IEC_UINT  InitializeBACnetObjects(IEC_BOOL bLocal, IEC_BOOL bWarmStart);
		IEC_UINT  CreateBACnetStack(IEC_BOOL *bpWarmStart);
		IEC_UINT  VerifyDataBase(IEC_CHAR *szBACnetDB, IEC_BOOL *bpWarmStart);
		IEC_UINT  VerifyFlashFile(IEC_BOOL bWarmStart);
		IEC_UINT  VerifyPAFile(IEC_CHAR *szFile, IEC_BOOL *bpExist, IEC_UDINT ulSize);
		IEC_UINT  CreateBACnetObjects(IEC_BOOL bWarmStart);
		IEC_UINT  EnableBACnetObjects(void);
		IEC_UINT  EnableBACnetStack(void);
		IEC_UINT  AddObjectToDevice(CBACnetObj *pObj);
		IEC_UINT  EnableBACnetLogging(IEC_UDINT ulLogging);

	public:
		IEC_UINT  HandleInsertBACnetObject(BACnetObjectContainer *pObjCont);
		IEC_UINT  HandleDeleteBACnetObject(BACnetObjectContainer *pObjCont);
		

	/* COV Handling - bacCov.cpp
	 * ------------------------------------------------------------------------
	 */
	public:
		void	  COVSubscribe(IEC_UDINT *pbDone);
		void	  COVVerify(void);

		IEC_UINT  COVHandleCOV(BACnetServiceCOVNotificationRequest *pCOV);
		IEC_UINT  COVHandleUPD(BACnetDeviceObjectPropertyReference *pObjRef, BACnetDataType *pVAL, IEC_DINT iSF, IEC_DINT iErr);

	protected:
		IEC_UDINT	m_ulLastCOVSub;


	/* BACnet Device - bacDev.cpp
	 * ------------------------------------------------------------------------
	 */
	public:
		IEC_UINT	WhoIs(void);
		IEC_UINT	IAm(void);
		
		CBACnetList *GetDevices(void);

		IEC_UINT	AddDevTransition(IEC_DINT iDeviceID, IEC_BOOL bTransitionToGood);
		IEC_UINT	GetDevTransition(IEC_DINT *ipDeviceID, IEC_BOOL *bpTransitionToGood);

	protected:
		IEC_UDINT	m_tLastIAm;

		CBACnetList *m_pDevices;
		CBACnetList *m_pDevTrans;


	/* Property Change Handling
	 * ------------------------------------------------------------------------
	 */
	public:
		void		ScanIEC(IEC_UDINT *pbDone);

	protected:
		IEC_UDINT	m_ulLastIECScan;


	/* Configuration File Import
	 * ------------------------------------------------------------------------
	 */ 	
	protected:
		CBACnetObj *CreateObject(BACnetObjectType eType);

	protected:
		CFileIniVal m_IniVal;
		CFileRemObj m_RemObj;
		CFileLocObj m_LocObj;


	/* IO Layer Configuration
	 * ------------------------------------------------------------------------
	 */
	protected:
		IEC_UINT	m_uIOLayer;
		SIOConfig  *m_pIO;
		IEC_UDINT	m_ulSMOffset;

		
	/* BACnet Configuration
	 * ------------------------------------------------------------------------
	 */
	public:
		static IEC_DINT  m_iDeviceID;		/* Device ID - Local BACnet Device. 		0 .. 0x3fffff					*/

		static IEC_DINT  m_iCOVLifeTime;	/* COV subscription lifetime in seconds.	0 .. MAX_INT					*/
		static IEC_BOOL  m_bConfirmedCOV;	/* Use confirmed COV notifications. 		TRUE | FALSE (FALSE)			*/

		IEC_UDINT m_ulNoLocObj; 			/* Number of local BACnet objects.			0 .. 2000 (local + remote)		*/ 
		IEC_UDINT m_ulNoRemObj; 			/* Number of remote BACnet objects. 		0 .. 2000						*/
		IEC_DINT  m_iDefPrioLoc;			/* Default priority for local objects.		0 .. 16 (0) 					*/
		IEC_DINT  m_iDefPrioRem;			/* Default priority for remote objects. 	1 .. 16 (16)					*/
		IEC_UDINT m_ulNetworkNo;			/* BACnet network number.					1 .. 65534 (1)					*/
		IEC_UDINT m_ulUDPPortNo;			/* UDP port number							1 .. MAX_SHORT	(47808) 		*/
		IEC_UDINT m_ulTimeSyncMode; 		/* BACnet time synchronization mode.		0 .. 4 (0)						*/
		IEC_UDINT m_ulTimeSyncInterval; 	/* Sync. interval in seconds if master. 	1 .. MAX_INT (21600)			*/
		IEC_CHAR  m_szIPTimeServer[16]; 	/* IP address of external SNTP time server. <valid IP address> ("") 		*/
		IEC_UDINT m_ulMinOfflineTime;		/* Min. configuration off line time.		0 .. MAX_INT (0)				*/
		IEC_BOOL  m_bDisablePersDB; 		/* Disable object persistence database. 	TRUE | FALSE (FALSE)			*/
		IEC_BOOL  m_bDontUnsubCOV;			/* Don't un-subscribe COV notifications.	TRUE | FALSE (FALSE)			*/
		IEC_UDINT m_ulEnableLogging;		/* Enable BACnet logging.					0 .. 23 bit (0) 				*/
		IEC_UDINT m_ulAPDUMaxLength;		/* Max. APDU length accepted.				50|128|206|480|1024|1476 (480)	*/
		IEC_UDINT m_ulAPDUTimeOut;			/* APDU time out.							0 .. MAX_INT (3)				*/
		IEC_UDINT m_ulAPDUSegTimeOut;		/* APDU segment time out.					0 .. MAX_INT (3)				*/
		IEC_UDINT m_ulAPDURetries;			/* No. of APDU retries. 					0 .. MAX_INT (0)				*/
		IEC_CHAR  m_szBBMDAddress[16];		/* IP address of BBMD server.				<valid IP address> ("") 		*/
		IEC_UDINT m_ulBBMDUDPPortNo;		/* UDP port number of BBMD server.			1 .. MAX_SHORT (47808)			*/
		IEC_UDINT m_ulBBMDMaxFD;			/* Number of accepted FD's. 				0 .. MAX_UDINT (0)				*/
		BACnetSegmentation m_eSegSupported;

		/* if m_szBBMDAddress != "" act as BACnet Foreign Device.
		 * if m_ulBBMDMaxFD   > 0	act as BACnet Broadcast Management Device.
		 * m_szIPTimeServer is only used if BACnet Device acts as BACnet Time Server.
		 */

		
	/* BACnet Objects
	 * ------------------------------------------------------------------------
	 */
	public: 	
		IEC_UINT	FindObject(IEC_DINT ilDeviceID, IEC_DINT ilObjectID, IEC_DINT ilTypeID, CBACnetObj **ppObj);
		IEC_UINT	FindObject(IEC_CHAR *szName, CBACnetObj **ppObj);

		IEC_BOOL	ObjectsEnabled(void);

	protected:
		CBACnetList *m_pObjects;
		CBACnetList *m_pRemObj;
		CBACnetList *m_pLocObj;
	
		IEC_BOOL	m_bEnableObj;


	/* Event handling - bacEvent.cpp
	 * ------------------------------------------------------------------------
	 */
	public:
		IEC_UINT	EventSubscribe(IEC_DINT iDeviceID, IEC_DINT iObjectID);
		IEC_UINT	EventUnSubscribe(IEC_DINT iDeviceID, IEC_DINT iObjectID);

		IEC_UINT	EventUnSubscribeAll(IEC_BOOL bIsWarmStart);

		IEC_UINT	EventGet(IEC_DATA *pEV, IEC_BOOL bDeleteEvent);
		IEC_UINT	EventGetString(IEC_STRING *pL1, IEC_STRING *pL2, IEC_BOOL bDeleteEvent);
		IEC_UINT	EventAcknowledge(IEC_DATA *pEV);

		IEC_UINT	GetLogRecord(STaskInfoVM *pVM, IEC_DATA *pEV, IEC_UINT uMaxElem, IEC_UDINT *ulpElem, IEC_UINT *pDst);

		IEC_UINT	HandleEvent(BACnetServiceEventNotificationRequest *pEvent); 

	protected:
		CBACnetList *m_pEventRecv;
		CBACnetList *m_pEventSubs;


	/* Common Properties - bacStack.cpp
	 * ------------------------------------------------------------------------
	 */
	public:
		IEC_BOOL	IsApplStartingUp(void);
		IEC_BOOL	IsStackStartingUp(void);

	protected:
		IEC_BOOL	m_bStartUpAppl;
		IEC_BOOL	m_bStartUpStack;
		IEC_UDINT	m_ulStartUpTime;


	/* Time Synchronization - bacStack.cpp
	 * ------------------------------------------------------------------------
	 */
	public:
		IEC_UINT	TimeSynchronization(void);
		IEC_DINT	OverrideTimeSync(bacnet_address *pAdr, IEC_BOOL bUTC);
		IEC_UINT	HandleTimeSync(BACnetDateTime *pDT, IEC_BOOL bUTC);

	protected:
		IEC_UDINT	m_ulLastTimeSync;


	/* Reinitialize Device - bacStack.cpp
	 * ------------------------------------------------------------------------
	 */
	public:
		IEC_UINT	ReinitDevice(IEC_DINT iDeviceID, BACnetStateOfDevice SoD, IEC_STRING *pStr);
		IEC_DINT	HandleReinitDevice(bacnet_address *pAdr, BYTE *pConfirm, BACnetServiceReinitializeDeviceRequest *pInit, BACnetError *pErr);


	/* Device Communication Control - bacStack.cpp
	 * ------------------------------------------------------------------------
	 */
	public:
		IEC_UINT	DevCommCtrl(IEC_DINT iDeviceID, BACnetEnableDisable ED, IEC_UDINT tDuration, IEC_STRING *pStr);
		IEC_BOOL	IsCommDisabled(void);
		IEC_UINT	VerifyDevCommCtrl(void);
		IEC_DINT	HandleDevCommCtrl(bacnet_address *pAdr, BYTE *pConfirm, BACnetServiceDeviceCommunicationControlRequest *pCtrl, BACnetError *pErr);

	protected:
		IEC_BOOL	m_bCommDisabled;
		IEC_UDINT	m_tContinueComm;


	/* Property Persistence - bacFlash.cpp
	 * ------------------------------------------------------------------------
	 */
	public:
		IEC_DINT	HandleUpdatePriorityArray(IEC_DINT iDeviceID, BACnetObjectType eType, IEC_DINT iObjectID, IEC_DINT iPriority, const BACnetDataType *pVal);
		void		Flash(IEC_UDINT *pbDone);

	protected:
		IEC_UDINT	m_ulLastFlash;

		
	/* Helper Functions - bacStack.cpp
	 * ------------------------------------------------------------------------
	 */
	public:
		static IEC_DINT GetDeviceID(void);
		static void 	TraceAPIError(IEC_CHAR *szAPI, BACnetErrorClass Class, BACnetErrorCode Code);
};


/* class CBACnetGlobal
 * ----------------------------------------------------------------------------
 */
class CBACnetGlobal
{
	public:
		CBACnetGlobal();
		~CBACnetGlobal();

	public:
		CBACnet *GetBACnet(void);

		IEC_UINT Create(void);
		IEC_UINT ReqDestroy(IEC_UDINT tTimeOut);
		IEC_UINT Destroy(IEC_UDINT tTimeOut);
		IEC_UINT Wait(IEC_UDINT tTimeOut);

	public:
		IEC_UINT Lock(void);
		IEC_UINT Lock(CBACnet **ppBACnet);
		IEC_UINT Test(void);
		IEC_UINT UnLock(void);

	public:
		IEC_BOOL IsCreated(void);

	protected:
		CBACnet *m_pBACnet;

		IEC_UDINT m_ulLocked;
		IEC_UDINT m_bDestroy;

	public:
		/* Wrapper functions for CBACnet member functions
		 */
		IEC_UINT ReadConfiguration(void);
		IEC_UINT StartUp(IEC_UINT uIOLayer, SIOConfig *pIO, IEC_BOOL bWarmstart);
		IEC_UINT AddDevTransition(IEC_DINT iDeviceID, IEC_BOOL bTransitionToGood);

		void	 COVSubscribe(IEC_UDINT *pbDone);
		void	 COVVerify(void);
		void	 ScanIEC(IEC_UDINT *pbDone);
		void	 Flash(IEC_UDINT *pbDone);

		IEC_DINT HandleUpdatePriorityArray(IEC_DINT iDeviceID, BACnetObjectType eType, IEC_DINT iObjectID, IEC_DINT iPriority, const BACnetDataType *pVal);
};


/* BACnet IO Layer Debug Outputs
 * ----------------------------------------------------------------------------
 */
#if defined(RTS_CFG_IO_BAC_TRACE)

  #define TRB_EVT(s,d,t,o)			osTrace("--- EVT: " s "\r\n", (d),(t),(o))
  #define TRB_BAC(s,d)				osTrace("--- BAC: " s "\r\n", (d))
  #define TRB_API(f,t1,t2,d1,d2)	osTrace("*** BAC: API Call (%s) failed: %s - %s (%d/%d)\r\n", f, t1, t2, d1, d2)

  #define TRE_COV(s,d,t,o)			osTrace("*** COV: " s, (d),(t),(o))
 
#else

  #define TRB_EVT(s,d,t,o)
  #define TRB_BAC(s,d)
  #define TRB_API(f,t1,t2,d1,d2)

  #define TRE_COV(s,d,t,o)

#endif

#if defined(RTS_CFG_IO_BAC_TRACE_EX)

  #define TRB_COV(s,d,t,o)			osTrace("--- COV: " s, (d),(t),(o))
  #define TRB_UPD(s,t,o)			osTrace("--- UPD: " s, (t),(o))
  #define TRB_COV2(s,d,t,o) 		osTrace(s, (d),(t),(o))
  #define TRB_OBJ(s,t,o)			osTrace("--- OBJ: " s, (t),(o))

#else

  #define TRB_COV(s,d,t,o)
  #define TRB_UPD(s,t,o)
  #define TRB_COV2(s,d,t,o)
  #define TRB_OBJ(s,t,o)

#endif


#endif /* _BACDEF_H_ */

/* ---------------------------------------------------------------------------- */

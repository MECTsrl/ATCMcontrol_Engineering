/* $Header: /4CReleased/V2.20.00/Target/4CWin/CONTROL/inc.bac/bacObj.h 1     28.02.07 19:10 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: bacObj.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/4CWin/CONTROL/inc.bac/bacObj.h $
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

#ifndef _BACOBJ_H_
#define _BACOBJ_H_

#if defined(INC_4C)
#error "Must not be included in 4C!"
#endif

#if !defined(_STDINC_H_)
#error "Include via stdInc.h only!"
#endif

#ifndef _BACIEC_H_
#include "bacIec.h"
#endif

/* class CBACnetObj
 * ----------------------------------------------------------------------------
 */
class CBACnetObj : public CListObj
{
	public:
		CBACnetObj();
		virtual ~CBACnetObj();


	/* List element handling
	 * --------------------------------------------------------------------------------------------
	 */
	public:
		virtual IEC_DINT GetListID(void);
		virtual IEC_DINT GetListID2(void);


	/* Shared Memory access
	 * --------------------------------------------------------------------------------------------
	 */
	public:
		virtual IEC_UINT SMCreate(void);
		virtual IEC_UINT SMAttach(IEC_UDINT ulSMOffset, IEC_UINT uIOLayer, SIOConfig *pIO);

	protected:
		virtual IEC_UINT SMAttachTyped(void)	= 0;
		

	/* BACnet stack access
	 * --------------------------------------------------------------------------------------------
	 */
	public:
		virtual IEC_UINT BACnetInitialize(IEC_DINT iObjectID, IEC_DINT iDeviceID, IEC_CHAR *szName, IEC_DINT iPriority, IEC_BOOL bLocal, IEC_BOOL bWarmStart);
		virtual IEC_UINT BACnetInitialize2(IEC_BOOL bSupportsCOV, IEC_BOOL bCommandable);
		virtual IEC_UINT BACnetCreate(IEC_CHAR *szInitial, IEC_BOOL bWarmStart);
	

	/* Object Handling
	 * --------------------------------------------------------------------------------------------
	 */
	public:
		IEC_UINT EnableObject(void);

		void HandleToGood(void);
		void HandleToBad(void);

	public:
		virtual void PrepareObject(void);
		virtual void CheckObject(void);

	protected:
		IEC_UDINT m_bTransitGood;
		IEC_UDINT m_bTransitBad;
		IEC_UDINT m_bDeviceIsBad;


	/* COV handling - bacCOV.cpp
	 * --------------------------------------------------------------------------------------------
	 */
	public:
		virtual IEC_UINT COVSubscribe(void);

		virtual IEC_UINT COVHandleCOV(BACnetServiceCOVNotificationRequest *pCOV);
		virtual IEC_UINT COVHandleUPD(BACnetDataType *pVAL, IEC_DINT iSF);

	protected:
		virtual IEC_UINT COVHandleVAL(BACnetPropertyIdentifier eProperty, BACnetDataType *pVAL);


	/* Access to properties in the Shared Memory.
	 * --------------------------------------------------------------------------------------------
	 */
	public:
		virtual void ScanPresentValue(void);
		virtual void PreparePVScanning(void);

	protected:
		IEC_UINT  Set_Object_Identifier(IEC_DINT iID);
		IEC_UINT  Set_Object_Name(IEC_CHAR *szName);
		IEC_UINT  Set_Device_ID(IEC_UDINT ulDeviceID);
		
	public:
		IEC_DINT  Get_Object_Identifier(void);
		IEC_CHAR *Get_Object_Name(void);

	protected:
		virtual IEC_UINT Ini_Command_Value(void);
		virtual IEC_UINT Ini_Command_Value_Retain(void);

		virtual IEC_UINT Ini_Present_Value(void);
		virtual IEC_UINT Ini_Present_Value_Retain(void);

		virtual IEC_UINT Set_Present_Value(IEC_DATA *pData, IEC_UDINT ulSize);
		virtual IEC_UINT Set_Present_Value_Retain(BACnetDataType *pVal);
		
	protected:
		IEC_UINT Set_Status_Flags(IEC_WORD wStatus);

	public:
		IEC_WORD Get_Status_Flags(void);
		IEC_BOOL SF_Fault(void);
		IEC_BOOL SF_InAlarm(void);
		IEC_BOOL SF_Overriden(void);
		IEC_BOOL SF_OutOfService(void);

	public:
		IEC_UINT Set_Quality(IEC_WORD wQuality);
		IEC_WORD Get_Quality(void);

	protected:
		IEC_WORD  m_wQuality;


	/* Access to BACnet properties
	 * --------------------------------------------------------------------------------------------
	 */
	public:
		virtual IEC_UINT BACnetSetIniVal(IEC_DATA *pVal);

	public:
		virtual IEC_UINT SetProperty(BACnetPropertyIdentifier eProperty, IEC_BOOL bSync, IEC_DATA *pVal, IEC_UDINT ulDataSize, IEC_UDINT ulArrayElem, IEC_DINT iPriority);
		virtual IEC_UINT GetProperty(BACnetPropertyIdentifier eProperty, IEC_BOOL bSync, IEC_DATA *pVal, IEC_UDINT ulDataSize, IEC_UDINT *ulpArrayElem);

	protected:
		IEC_UINT BACnetSetPropertyEx(BACnetPropertyIdentifier eProperty, IEC_BOOL bSync, BACnetDataType *pVal, IEC_DINT iPriority);
		IEC_UINT BACnetGetPropertyEx(BACnetPropertyIdentifier eProperty, IEC_BOOL bSync, BACnetDataType *pVal);
		
	protected:
		IEC_UINT BACnetSetProperty(BACnetPropertyIdentifier eProperty, IEC_BOOL bSync, IEC_REAL fVal 		, IEC_DINT iPriority);
		IEC_UINT BACnetSetProperty(BACnetPropertyIdentifier eProperty, IEC_BOOL bSync, IEC_UDINT ulVal		, IEC_DINT iPriority);
		IEC_UINT BACnetSetProperty(BACnetPropertyIdentifier eProperty, IEC_BOOL bSync, BACnetBinaryPV Val	, IEC_DINT iPriority);
		IEC_UINT BACnetSetProperty(BACnetPropertyIdentifier eProperty, IEC_BOOL bSync, IEC_CHAR *szVal		, IEC_DINT iPriority);
		IEC_UINT BACnetSetProperty(BACnetPropertyIdentifier eProperty, IEC_BOOL bSync, IEC_WORD wVal 		, IEC_DINT iPriority);
		IEC_UINT BACnetSetProperty(BACnetPropertyIdentifier eProperty, IEC_BOOL bSync, IEC_BOOL bVal 		, IEC_DINT iPriority);
		IEC_UINT BACnetSetProperty(BACnetPropertyIdentifier eProperty, IEC_BOOL bSync, BACnetDataType *pVal	, IEC_DINT iPriority);

	protected:
		IEC_UINT BACnetGetProperty(BACnetPropertyIdentifier eProperty, IEC_BOOL bSync, IEC_REAL *pVal		);
		IEC_UINT BACnetGetProperty(BACnetPropertyIdentifier eProperty, IEC_BOOL bSync, IEC_UDINT *pVal		);
		IEC_UINT BACnetGetProperty(BACnetPropertyIdentifier eProperty, IEC_BOOL bSync, BACnetBinaryPV *pVal	);
		IEC_UINT BACnetGetProperty(BACnetPropertyIdentifier eProperty, IEC_BOOL bSync, IEC_CHAR *pVal		, IEC_UDINT ulSize);
		IEC_UINT BACnetGetProperty(BACnetPropertyIdentifier eProperty, IEC_BOOL bSync, IEC_WORD *pVal		);
		IEC_UINT BACnetGetProperty(BACnetPropertyIdentifier eProperty, IEC_BOOL bSync, IEC_BOOL *pVal		);
		IEC_UINT BACnetGetProperty(BACnetPropertyIdentifier eProperty, IEC_BOOL bSync, BACnetObjectIdentifier *pVal);
		IEC_UINT BACnetGetProperty(BACnetPropertyIdentifier eProperty, IEC_BOOL bSync, BACnetDataType *pVal	);


	/* Property Polling
	 * ------------------------------------------------------------------------
	 */
	public:
		IEC_UINT SubscribeProperty(BACnetPropertyIdentifier eProperty, IEC_UDINT ulPollRate, IEC_UDINT ulArrayElem);
		IEC_UINT UnSubscribeProperty(BACnetPropertyIdentifier eProperty);

	protected:
		CBACnetList *m_pSubProp;
				

	/* Property Initialization (bacObjIni.cpp)
	 * ------------------------------------------------------------------------
	 */
	public:
		virtual IEC_UINT InitProperty(BACnetObjectContainer *pObjCont, IEC_DATA *pIniVal);
		virtual IEC_UINT InitPresentValue(BACnetObjectContainer *pObjCont, IEC_BOOL bWarmStart);
		virtual IEC_UINT InitPrioArray(BACnetObjectContainer *pObjCont);
		virtual IEC_UINT VerifyObject(BACnetObjectContainer *pObjCont);

	protected:
		virtual IEC_UINT InitPresentValueEx(BACnetPriorityArrayAna	*pPV, IEC_BOOL bWarmStart);
		virtual IEC_UINT InitPresentValueEx(BACnetPriorityArrayBin	*pPV, IEC_BOOL bWarmStart);
		virtual IEC_UINT InitPresentValueEx(BACnetPriorityArrayUint *pPV, IEC_BOOL bWarmStart);

		virtual IEC_UINT InitPrioArrayEx(BACnetPriorityArrayAna  *pPV);
		virtual IEC_UINT InitPrioArrayEx(BACnetPriorityArrayBin  *pPV);
		virtual IEC_UINT InitPrioArrayEx(BACnetPriorityArrayUint *pPV);


	/* Property Persistence (Flash)
	 * ------------------------------------------------------------------------
	 */
	public:
		IEC_UDINT	GetPASize(void);

	public:
		virtual void	 Flash(void);
		
	public:
		virtual IEC_UINT CreatePrioArray(void);
		virtual IEC_UINT UpdatePrioArray(IEC_UDINT ulPriority, const BACnetDataType *pVal);
		virtual IEC_UINT LoadPrioArray(BACnetObjectContainer *pObjCont);

	protected:
		virtual IEC_UINT FlashPrioArray(void);

	protected:
		IEC_BOOL	m_bPADirty;
		IEC_UDINT	m_ulPADirtyFrom;
		IEC_UDINT	m_ulPADirtyTo;
		IEC_UDINT	m_tPANextStore;
		IEC_DATA	*m_pPACurrent;
		IEC_DATA	*m_pPAStored;

		IEC_UDINT	m_ulPASize;
		IEC_UDINT	m_ulPAOffset;

	public:
		static IEC_UDINT m_ulPASize_Ana;
		static IEC_UDINT m_ulPASize_Bin;
		static IEC_UDINT m_ulPASize_Mul;


	/* Common Functions
	 * ------------------------------------------------------------------------
	 */
	public:
		BACnetObjectType GetType(void);

		IEC_DINT  GetDeviceID(void);
		IEC_DINT  GetObjectID(void);

		IEC_BOOL  IsLocalObject(void);
		IEC_BOOL  IsInitialized(void);
		IEC_BOOL  IsCreated(void);
		IEC_BOOL  IsIECEmbedded(void);
		IEC_BOOL  IsSupportingCOV(void);
		IEC_BOOL  IsEnabled(void);

		IEC_BOOL  IsExternal(void);
		IEC_UINT  SetExternal(IEC_BOOL bExternal);

		IEC_DINT  GetPriority(void);
		IEC_UINT  SetPriority(IEC_DINT iPriority);

		IEC_UDINT GetPVRetainOffset(void);
		IEC_UINT  SetPVRetainOffset(IEC_UDINT ulPVRetOffs);

		IEC_UINT  SetIniValOffset(IEC_UDINT ulOffIniVal, IEC_UDINT ulNoOfIniVal);
		IEC_UDINT GetNoOfIniVal(void);
		IEC_UDINT GetIniValOffset(void);


	/* Common Properties
	 * ------------------------------------------------------------------------
	 */
	protected:
		IEC_BOOL	m_bIEC; 			/* Object embedded into IEC world.	*/
		IEC_BOOL	m_bLocal;			/* Object is a local BACnet object. */
		IEC_BOOL	m_bInit;			/* Object is initialized.			*/
		IEC_BOOL	m_bCreated; 		/* Object is created in stack.		*/
		IEC_BOOL	m_bSupportCOV;		/* Object supports COV. 			*/
		IEC_BOOL	m_bEnableObject;	/* Object is enabled - ready to use.*/
		IEC_BOOL	m_bExternal;		/* Object is external created.		*/

		IEC_DINT	m_iDeviceID;		/* ID of associated BACnet device.	*/
		IEC_DINT	m_iObjectID;		/* ID of BACnet object. 			*/

		IEC_UDINT	m_ulPVSize; 		/* Size of Present_Value.			*/
		IEC_UDINT	m_ulPVRetOffs;		/* Retain offset of Present_Value.	*/
		IEC_UDINT	m_bPVScanning;		/* Present-Value scanning activated.*/

		IEC_UDINT	m_ulOffIniVal;		/* Offset in initial value file.	*/
		IEC_UDINT	m_ulNoOfIniVal; 	/* Number of initial values.		*/

		IEC_BOOL	m_bTBDSupportsCOV;	/* Object supports COV. 			*/ 
		IEC_BOOL	m_bTBDCommandable;	/* Object is command able.			*/

		
	protected:
		IEC_DINT	m_iPriority;		/* Priority for BACnet writing. 	*/

	protected:
		BACnetObjectType  m_eType;		/* Object Type. 					*/
		
		IEC_DATA	*m_pSM; 			/* Pointer to Shared Memory.		*/
		IEC_DATA	*m_pWF; 			/* Pointer to Write Flags.			*/
		IEC_UDINT	m_ulSMSize; 		/* Size of Shared Memory area.		*/
		
		IEC_WORD	m_pPropOff[BAC_PROP_SM_COUNT];

		IEC_DATA	*m_pPVRetain;		/* Pointer to PV retentive memory.	*/
};


/* class CBACnetCOV
 * ----------------------------------------------------------------------------
 */
class CBACnetCOV : public CBACnetObj
{
	public:
		CBACnetCOV();
		virtual ~CBACnetCOV();


	/* BACnet stack access
	 * --------------------------------------------------------------------------------------------
	 */
	public:
		virtual IEC_UINT BACnetInitialize(IEC_DINT iObjectID, IEC_DINT iDeviceID, IEC_CHAR *szName, IEC_DINT iPriority, IEC_BOOL bLocal, IEC_BOOL bWarmStart);


	/* Object Handling
	 * --------------------------------------------------------------------------------------------
	 */
	public:
		virtual void PrepareObject(void);
		virtual void CheckObject(void);


	/* COV handling
	 * --------------------------------------------------------------------------------------------
	 */
	public:
		virtual IEC_UINT COVSubscribe(void);

		virtual IEC_UINT COVHandleCOV(BACnetServiceCOVNotificationRequest *pCOV);
		virtual IEC_UINT COVHandleUPD(BACnetDataType *pVAL, IEC_DINT iSF);

	protected:
		virtual IEC_UINT COVHandleVAL(BACnetPropertyIdentifier eProperty, BACnetDataType *pVAL);

	protected:
		IEC_UDINT	m_ulLastCOV;		/* Last COV subscription timestamp. */
		IEC_UDINT	m_ulLastFailed; 	/* Last failed COV subscription.	*/
		IEC_UDINT	m_bCOVPending;		/* COV subscription pending.		*/
		IEC_UDINT	m_uLifeTime;		/* COV subscription lifetime.		*/
		IEC_UDINT	m_uFailCount;		/* COV subscription fail counter.	*/
};


/* class CBACnetAnalog
 * ----------------------------------------------------------------------------
 */
class CBACnetAnalog : public CBACnetCOV
{
	public:
		CBACnetAnalog();
		virtual ~CBACnetAnalog();

	public:
		virtual void ScanPresentValue(void);

	public:
		virtual IEC_UINT CreatePrioArray(void);
		virtual IEC_UINT UpdatePrioArray(IEC_UDINT ulPriority, const BACnetDataType *pVal);
		virtual IEC_UINT LoadPrioArray(BACnetObjectContainer *pObjCont);

	protected:
		virtual IEC_UINT FlashPrioArray(void);

	public:
		static IEC_UINT  CreatePAFile(IEC_CHAR *szFile);

	protected:
		virtual IEC_UINT COVHandleVAL(BACnetPropertyIdentifier eProperty, BACnetDataType *pVAL);

	public:
		virtual IEC_UINT SetProperty(BACnetPropertyIdentifier eProperty, IEC_BOOL bSync, IEC_DATA *pVal, IEC_UDINT ulDataSize, IEC_UDINT ulArrayElem, IEC_DINT iPriority);
		virtual IEC_UINT GetProperty(BACnetPropertyIdentifier eProperty, IEC_BOOL bSync, IEC_DATA *pVal, IEC_UDINT ulDataSize, IEC_UDINT *ulpArrayElem);

	protected:
		virtual IEC_UINT Ini_Command_Value(void);
		virtual IEC_UINT Ini_Command_Value_Retain(void);

		virtual IEC_UINT Ini_Present_Value(void);
		virtual IEC_UINT Ini_Present_Value_Retain(void);

		virtual IEC_UINT Set_Present_Value(IEC_DATA *pData, IEC_UDINT ulSize);
		virtual IEC_UINT Set_Present_Value_Retain(BACnetDataType *pVal);
};


/* class CBACnetBinary
 * ----------------------------------------------------------------------------
 */
class CBACnetBinary : public CBACnetCOV
{
	public:
		CBACnetBinary();
		virtual ~CBACnetBinary();

	public:
		virtual void ScanPresentValue(void);

	public:
		virtual IEC_UINT CreatePrioArray(void);
		virtual IEC_UINT UpdatePrioArray(IEC_UDINT ulPriority, const BACnetDataType *pVal);
		virtual IEC_UINT LoadPrioArray(BACnetObjectContainer *pObjCont);

	protected:
		virtual IEC_UINT FlashPrioArray(void);

	public:
		static IEC_UINT  CreatePAFile(IEC_CHAR *szFile);

	protected:
		virtual IEC_UINT COVHandleVAL(BACnetPropertyIdentifier eProperty, BACnetDataType *pVAL);

	public:
		virtual IEC_UINT SetProperty(BACnetPropertyIdentifier eProperty, IEC_BOOL bSync, IEC_DATA *pVal, IEC_UDINT ulDataSize, IEC_UDINT ulArrayElem, IEC_DINT iPriority);
		virtual IEC_UINT GetProperty(BACnetPropertyIdentifier eProperty, IEC_BOOL bSync, IEC_DATA *pVal, IEC_UDINT ulDataSize, IEC_UDINT *ulpArrayElem);

	protected:
		virtual IEC_UINT Ini_Command_Value(void);
		virtual IEC_UINT Ini_Command_Value_Retain(void);

		virtual IEC_UINT Ini_Present_Value(void);
		virtual IEC_UINT Ini_Present_Value_Retain(void);

		virtual IEC_UINT Set_Present_Value(IEC_DATA *pData, IEC_UDINT ulSize);
		virtual IEC_UINT Set_Present_Value_Retain(BACnetDataType *pVal);
};


/* class CBACnetMulti
 * ----------------------------------------------------------------------------
 */
class CBACnetMulti : public CBACnetCOV
{
	public:
		CBACnetMulti();
		virtual ~CBACnetMulti();

	public:
		virtual void ScanPresentValue(void);

	public:
		virtual IEC_UINT CreatePrioArray(void);
		virtual IEC_UINT UpdatePrioArray(IEC_UDINT ulPriority, const BACnetDataType *pVal);
		virtual IEC_UINT LoadPrioArray(BACnetObjectContainer *pObjCont);

	protected:
		virtual IEC_UINT FlashPrioArray(void);

	public:
		static IEC_UINT  CreatePAFile(IEC_CHAR *szFile);

	protected:
		virtual IEC_UINT COVHandleVAL(BACnetPropertyIdentifier eProperty, BACnetDataType *pVAL);

	public:
		virtual IEC_UINT SetProperty(BACnetPropertyIdentifier eProperty, IEC_BOOL bSync, IEC_DATA *pVal, IEC_UDINT ulDataSize, IEC_UDINT ulArrayElem, IEC_DINT iPriority);
		virtual IEC_UINT GetProperty(BACnetPropertyIdentifier eProperty, IEC_BOOL bSync, IEC_DATA *pVal, IEC_UDINT ulDataSize, IEC_UDINT *ulpArrayElem);

	protected:
		virtual IEC_UINT Ini_Command_Value(void);
		virtual IEC_UINT Ini_Command_Value_Retain(void);

		virtual IEC_UINT Ini_Present_Value(void);
		virtual IEC_UINT Ini_Present_Value_Retain(void);

		virtual IEC_UINT Set_Present_Value(IEC_DATA *pData, IEC_UDINT ulSize);
		virtual IEC_UINT Set_Present_Value_Retain(BACnetDataType *pVal);
};


/* class CAI (Analog Input)
 * ----------------------------------------------------------------------------
 */
class CAI : public CBACnetAnalog
{
	public:
		CAI();
		virtual ~CAI();

	protected:
		virtual IEC_UINT SMAttachTyped(void);

	public:
		virtual IEC_UINT CreatePrioArray(void);

		virtual IEC_UINT InitProperty(BACnetObjectContainer *pObjCont, IEC_DATA *pIniVal);
		virtual IEC_UINT InitPresentValue(BACnetObjectContainer *pObjCont, IEC_BOOL bWarmStart);

	protected:
		SAI 	*m_pAI;
};


/* class CAO (Analog Output)
 * ----------------------------------------------------------------------------
 */
class CAO : public CBACnetAnalog
{
	public:
		CAO();
		virtual ~CAO();

	protected:
		virtual IEC_UINT SMAttachTyped(void);

	public:
		virtual IEC_UINT InitProperty(BACnetObjectContainer *pObjCont, IEC_DATA *pIniVal);
		virtual IEC_UINT InitPresentValue(BACnetObjectContainer *pObjCont, IEC_BOOL bWarmStart);
		virtual IEC_UINT InitPrioArray(BACnetObjectContainer *pObjCont);

	protected:
		SAO 	*m_pAO;
};


/* class CAV (Analog Value)
 * ----------------------------------------------------------------------------
 */
class CAV : public CBACnetAnalog
{
	public:
		CAV();
		virtual ~CAV();

	protected:
		virtual IEC_UINT SMAttachTyped(void);

	public:
		virtual IEC_UINT InitProperty(BACnetObjectContainer *pObjCont, IEC_DATA *pIniVal);
		virtual IEC_UINT InitPresentValue(BACnetObjectContainer *pObjCont, IEC_BOOL bWarmStart);
		virtual IEC_UINT InitPrioArray(BACnetObjectContainer *pObjCont);

	protected:
		SAV 	*m_pAV;
};


/* class CBI (Binary Input)
 * ----------------------------------------------------------------------------
 */
class CBI : public CBACnetBinary
{
	public:
		CBI();
		virtual ~CBI();

	protected:
		virtual IEC_UINT SMAttachTyped(void);

	public:
		virtual IEC_UINT CreatePrioArray(void);

		virtual IEC_UINT InitProperty(BACnetObjectContainer *pObjCont, IEC_DATA *pIniVal);
		virtual IEC_UINT InitPresentValue(BACnetObjectContainer *pObjCont, IEC_BOOL bWarmStart);

	protected:
		SBI 	*m_pBI;
};


/* class CBO (Binary Output)
 * ----------------------------------------------------------------------------
 */
class CBO : public CBACnetBinary
{
	public:
		CBO();
		virtual ~CBO();

	protected:
		virtual IEC_UINT SMAttachTyped(void);

	public:
		virtual IEC_UINT InitProperty(BACnetObjectContainer *pObjCont, IEC_DATA *pIniVal);
		virtual IEC_UINT InitPresentValue(BACnetObjectContainer *pObjCont, IEC_BOOL bWarmStart);
		virtual IEC_UINT InitPrioArray(BACnetObjectContainer *pObjCont);

	protected:
		SBO 	*m_pBO;
};


/* class CBV (Binary Value)
 * ----------------------------------------------------------------------------
 */
class CBV : public CBACnetBinary
{
	public:
		CBV();
		virtual ~CBV();

	protected:
		virtual IEC_UINT SMAttachTyped(void);

	public:
		virtual IEC_UINT InitProperty(BACnetObjectContainer *pObjCont, IEC_DATA *pIniVal);
		virtual IEC_UINT InitPresentValue(BACnetObjectContainer *pObjCont, IEC_BOOL bWarmStart);
		virtual IEC_UINT InitPrioArray(BACnetObjectContainer *pObjCont);

	protected:
		SBV 	*m_pBV;
};


/* class CMI (Multi-state Input)
 * ----------------------------------------------------------------------------
 */
class CMI : public CBACnetMulti
{
	public:
		CMI();
		virtual ~CMI();

	protected:
		virtual IEC_UINT SMAttachTyped(void);

	public:
		virtual IEC_UINT CreatePrioArray(void);

		virtual IEC_UINT InitProperty(BACnetObjectContainer *pObjCont, IEC_DATA *pIniVal);
		virtual IEC_UINT InitPresentValue(BACnetObjectContainer *pObjCont, IEC_BOOL bWarmStart);
		virtual IEC_UINT VerifyObject(BACnetObjectContainer *pObjCont);

	protected:
		SMI 	*m_pMI;
};


/* class CMO (Multi-state Output)
 * ----------------------------------------------------------------------------
 */
class CMO : public CBACnetMulti
{
	public:
		CMO();
		virtual ~CMO();

	protected:
		virtual IEC_UINT SMAttachTyped(void);
		
	public:
		virtual IEC_UINT InitProperty(BACnetObjectContainer *pObjCont, IEC_DATA *pIniVal);
		virtual IEC_UINT InitPresentValue(BACnetObjectContainer *pObjCont, IEC_BOOL bWarmStart);
		virtual IEC_UINT InitPrioArray(BACnetObjectContainer *pObjCont);
		virtual IEC_UINT VerifyObject(BACnetObjectContainer *pObjCont);

	protected:
		SMO 	*m_pMO;
};


/* class CMV (Multi-state Value)
 * ----------------------------------------------------------------------------
 */
class CMV : public CBACnetMulti
{
	public:
		CMV();
		virtual ~CMV();

	protected:
		virtual IEC_UINT SMAttachTyped(void);

	public:
		virtual IEC_UINT InitProperty(BACnetObjectContainer *pObjCont, IEC_DATA *pIniVal);
		virtual IEC_UINT InitPresentValue(BACnetObjectContainer *pObjCont, IEC_BOOL bWarmStart);
		virtual IEC_UINT InitPrioArray(BACnetObjectContainer *pObjCont);
		virtual IEC_UINT VerifyObject(BACnetObjectContainer *pObjCont);

	protected:
		SMV 	*m_pMV;
};


/* class CNC (Notification Class)
 * ----------------------------------------------------------------------------
 */
class CNC : public CBACnetObj
{
	public:
		CNC();
		virtual ~CNC();

	protected:
		virtual IEC_UINT SMAttachTyped(void);

	public:
		virtual IEC_UINT InitProperty(BACnetObjectContainer *pObjCont, IEC_DATA *pIniVal);

	protected:
		SNC 	*m_pNC;
};


/* class CTL (Trend Log)
 * ----------------------------------------------------------------------------
 */
class CTL : public CBACnetObj
{
	public:
		CTL();
		virtual ~CTL();

	protected:
		virtual IEC_UINT SMAttachTyped(void);

	public:
		virtual IEC_UINT InitProperty(BACnetObjectContainer *pObjCont, IEC_DATA *pIniVal);

	protected:
		STL 	*m_pTL;
};


/* class CSC (Schedule)
 * ----------------------------------------------------------------------------
 */
class CSC : public CBACnetObj
{
	public:
		CSC();
		virtual ~CSC();

	protected:
		virtual IEC_UINT SMAttachTyped(void);

	public:
		virtual IEC_UINT SetProperty(BACnetPropertyIdentifier eProperty, IEC_BOOL bSync, IEC_DATA *pVal, IEC_UDINT ulDataSize, IEC_UDINT ulArrayElem, IEC_DINT iPriority);
		virtual IEC_UINT GetProperty(BACnetPropertyIdentifier eProperty, IEC_BOOL bSync, IEC_DATA *pVal, IEC_UDINT ulDataSize, IEC_UDINT *ulpArrayElem);

	public:
		virtual IEC_UINT InitProperty(BACnetObjectContainer *pObjCont, IEC_DATA *pIniVal);

	protected:
		SSC 	*m_pSC;
};


#endif /* _BACOBJ_H_ */

/* ---------------------------------------------------------------------------- */

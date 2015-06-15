/* $Header: /4CReleased/V2.20.00/Target/4CWin/CONTROL/inc.bac/bacMain.h 1     28.02.07 19:10 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: bacMain.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/4CWin/CONTROL/inc.bac/bacMain.h $
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

#ifndef _BACMAIN_H_
#define _BACMAIN_H_

#if defined(INC_4C)
#error "Must not be included in 4C!"
#endif

#if !defined(_STDINC_H_)
#error "Include via stdInc.h only!"
#endif


/* bacCallB.cpp
 * ----------------------------------------------------------------------------
 */

/* Service Indications - Who-Is, Who-Has, I-Am, I-Have
 */
IEC_DINT bacIamNotificatinCB (bacnet_address *pAdr, BACnetServiceIam *pIam);
IEC_DINT bacWhoIsOverrideCB  (bacnet_address *pAdr, BACnetServiceWhoIs	*pWhoIs);
IEC_DINT bacWhoHasOverrideCB (bacnet_address *pAdr, BACnetServiceWhoHas *pWhoHas);

/* Service Indications - Object Creation/Deletion
 */
void bacInsertObjectNotificationCB(BACnetObjectContainer *pObjCont);
void bacRemoveObjectNotificationCB(BACnetObjectContainer *pObjCont);

/* Service Overrides - Object Creation/Deletion
 */
IEC_DINT bacCreateObjectOverrideCB(bacnet_address *pAdr, IEC_BYTE *pConfirm, BACnetServiceCreateObjectRequest *pReq, BACnetServiceCreateObjectAck *pAck, BACnetError *pErr);
IEC_DINT bacDeleteObjectOverrideCB(bacnet_address *pAdr, IEC_BYTE *pConfirm, BACnetServiceDeleteObjectRequest *pReq, BACnetError *pErr);

/* Service Indications - Change Of Value
 */
IEC_DINT bacExtCOVNotificationCB(BACnetServiceCOVNotificationRequest *pCOV);
void	 bacIntCOVNotificationCB(BACnetDeviceObjectPropertyReference *pObjRef, IEC_USINT bInternal, BACnetDataType *pVAL, IEC_DINT iErr, IEC_DINT iSF, IEC_USINT usPrio);

/* Service Indications - Alarm & Event
 */
IEC_DINT bacEventNotificationCB(BACnetServiceEventNotificationRequest *pEvent);

/* Service Indications - Time Synchronization
 */
IEC_DINT bacTimeSyncNotificationCB(BACnetServiceTimeSynchronization *pSync);
IEC_DINT bacUTCSyncNotificationCB(BACnetServiceUtcTimeSynchronization *pSync);

IEC_DINT bacTimeSyncOverrideCB(bacnet_address *pAdr, BACnetServiceTimeSynchronization *pSync);
IEC_DINT bacUTCSyncOverrideCB(bacnet_address *pAdr, BACnetServiceUtcTimeSynchronization *pSync);

/* Service Indications - Reinitialize Device
 */
IEC_DINT bacReinitDevOverrideCB (bacnet_address *pAdr, BYTE *pConfirm, BACnetServiceReinitializeDeviceRequest *pInit, BACnetError *pErr);

/* Service Indications - Device Communication Control
 */
IEC_DINT bacDevCommCtrlOverrideCB(bacnet_address *pAdr, BYTE *pConfirm, BACnetServiceDeviceCommunicationControlRequest *pCtrl, BACnetError *pErr);

/* Service Indications - Priority Array Persistence
 */
IEC_DINT bacUpdataAnaPriorityArrayCB(IEC_DINT iDeviceID, IEC_DINT iObjectType, IEC_DINT iObjectID, IEC_DINT iPriority, const BACnetDataType *pVal);
IEC_DINT bacUpdataBinaryPVPriorityArrayCB(IEC_DINT iDeviceID, IEC_DINT iObjectType, IEC_DINT iObjectID, IEC_DINT iPriority, const BACnetDataType *pVal);
IEC_DINT bacUpdataUintPriorityArrayCB(IEC_DINT iDeviceID, IEC_DINT iObjectType, IEC_DINT iObjectID, IEC_DINT iPriority, const BACnetDataType *pVal);

/* Service Overrides - Not Supported Services
 */
IEC_DINT bacConfirmedPrivateTransferOverrideCB(bacnet_address *pAdr, IEC_BYTE *pConfirm, BACnetServicePrivateTransferRequest *pReq, BACnetServiceConfirmedPrivateTransferAck *pAck, BACnetError *pErr);
IEC_DINT bacUnConfirmedPrivateTransferOverrideCB(bacnet_address *pAdr, BACnetServicePrivateTransferRequest *pReq);
IEC_DINT bacConfirmedTextMessageOverrideCB(bacnet_address *pAdr, IEC_BYTE *pConfirm, BACnetServiceTextMessageRequest *pReq, BACnetError *pErr);
IEC_DINT bacUnConfirmedTextMessageOverrideCB(bacnet_address *pAdr, BACnetServiceTextMessageRequest *pReq);
IEC_DINT bacLifeSafetyOperationOverrideCB(bacnet_address *pAdr, IEC_BYTE *pConfirm, BACnetServiceLifeSafetyOperationRequest *pReq, BACnetError *pErr);

/* Call Back - Enable/Disable BACnet Network Layer
 */
IEC_DINT bacEnableDataLinkLayerCB(void);


/* bacMain.cpp
 * ----------------------------------------------------------------------------
 */
IEC_UINT bacInitialize(IEC_UINT uIOLayer, SIOLayerIniVal *pIni);
IEC_UINT bacNotifyConfigMain(IEC_UINT uIOLayer, SIOConfig *pIO);
IEC_UINT bacNotifyConfigConf(IEC_UINT uIOLayer, SIOConfig *pIO, IEC_BOOL bWarmStart);
IEC_UINT bacNotifyStart(IEC_UINT uIOLayer, SIOConfig *pIO);
IEC_UINT bacNotifyStopPrepare(IEC_UINT uIOLayer, SIOConfig *pIO);
IEC_UINT bacNotifyStopVerify(IEC_UINT uIOLayer, SIOConfig *pIO);
IEC_UINT bacNotifyTerm(IEC_UINT uIOLayer, IEC_BOOL bWarmStart);
IEC_UINT bacFinalizeMain(IEC_UINT uIOLayer, IEC_BOOL bWarmStart);
IEC_UINT bacFinalizeConf(IEC_UINT uIOLayer, IEC_BOOL bWarmStart);


/* bacUtil.cpp
 * ----------------------------------------------------------------------------
 */
IEC_UINT IECtoBAC_ArrayOfUnsigned(BACnetDataType *pDT, IEC_UDINT *pVal, IEC_UDINT ulDataSize, IEC_UDINT ulArrayElem);
IEC_UINT BACtoIEC_ArrayOfUnsigned(BACnetDataType *pDT, IEC_UDINT *pVal, IEC_UDINT ulDataSize, IEC_UDINT *ulpArrayElem);
IEC_UINT BACtoIEC_PrioArrayOfUnsigned(BACnetDataType *pDT, IEC_UDINT *pVal, IEC_UDINT ulDataSize, IEC_UDINT *ulpArrayElem);
IEC_UINT BACtoIEC_PrioArrayOfReal(BACnetDataType *pDT, IEC_REAL *pVal, IEC_UDINT ulDataSize, IEC_UDINT *ulpArrayElem);
IEC_UINT BACtoIEC_PrioArrayOfBinaryPV(BACnetDataType *pDT, IEC_WORD *pVal, IEC_UDINT ulDataSize, IEC_UDINT *ulpArrayElem);
IEC_UINT IECtoBAC_ArrayOfString(BACnetDataType *pDT, IEC_CHAR *pVal, IEC_UDINT ulDataSize, IEC_UDINT ulArrayElem);
IEC_UINT BACtoIEC_ArrayOfString(BACnetDataType *pDT, IEC_CHAR *pVal, IEC_UDINT ulDataSize, IEC_UDINT *ulpArrayElem);
IEC_UINT IECtoBAC_BitString(BACnetDataType *pDT, IEC_DATA *pVal, IEC_UDINT ulDataSize, IEC_UDINT ulBits);
IEC_UINT BACtoIEC_BitString(BACnetDataType *pDT, IEC_BYTE *pVal, IEC_UDINT ulDataSize, IEC_UDINT ulBits);
IEC_UINT IECtoBAC_DevObjPropRev(BACnetDataType *pDT, IEC_DATA *pVal, IEC_UDINT ulDataSize);
IEC_UINT IECtoBAC_DevObjPropRevEx(BACnetDeviceObjectPropertyReference *pR, IEC_DATA *pVal, IEC_UDINT ulDataSize);
IEC_UINT BACtoIEC_DevObjPropRev(BACnetDataType *pDT, IEC_DATA *pVal, IEC_UDINT ulDataSize);
IEC_UINT IECtoBAC_ArrayOfDevObjPropRev(BACnetDataType *pDT, IEC_DATA *pVal, IEC_UDINT ulDataSize, IEC_UDINT ulArrayElem);
IEC_UINT BACtoIEC_ArrayOfDevObjPropRev(BACnetDataType *pDT, IEC_DATA *pVal, IEC_UDINT ulDataSize, IEC_UDINT *ulpArrayElem);
IEC_UINT INItoIEC_ArrayOfDevObjPropRev(IEC_DATA *pData, IEC_DATA **ppBuff, IEC_UDINT *ulpBuffSize);
IEC_UINT IECtoBAC_DateTime(BACnetDataType *pDT, IEC_DATA *pVal, IEC_UDINT ulDataSize);
IEC_UINT IECtoBAC_DateTimeEx(BACnetDateTime *pDT, IEC_DATA *pVal, IEC_UDINT ulDataSize);
IEC_UINT BACtoIEC_DateTime(BACnetDataType *pDT, IEC_DATA *pVal, IEC_UDINT ulDataSize);
IEC_UINT IECtoBAC_TimeStamp(BACnetDataType *pDT, IEC_DATA *pVal, IEC_UDINT ulDataSize);
IEC_UINT BACtoIEC_TimeStamp(BACnetDataType *pDT, IEC_DATA *pVal, IEC_UDINT ulDataSize);
IEC_UINT BACtoIEC_ArrayOfTimeStamp(BACnetDataType *pDT, IEC_DATA *pVal, IEC_UDINT ulDataSize, IEC_UDINT *ulpArrayElem);
IEC_UINT IECtoBAC_Any(BACnetDataType *pDT, IEC_DATA *pVal, IEC_UDINT ulDataSize);
IEC_UINT BACtoIEC_Any(BACnetDataType *pDT, IEC_DATA *pVal, IEC_UDINT ulDataSize);
IEC_UINT IECtoBAC_DateRange(BACnetDataType *pDT, IEC_DATA *pVal, IEC_UDINT ulDataSize);
IEC_UINT IECtoBAC_DateRangeEx(BACnetDateRange *pDR, IEC_DATA *pVal, IEC_UDINT ulDataSize);
IEC_UINT BACtoIEC_DateRange(BACnetDataType *pDT, IEC_DATA *pVal, IEC_UDINT ulDataSize);
IEC_UINT INItoIEC_DateRange(IEC_DATA *pData, IEC_DATA **ppBuff, IEC_UDINT *ulpBuffSize);
IEC_UINT IECtoBAC_ArrayOfDailySchedule(BACnetDataType *pDT, IEC_DATA *pVal, IEC_UDINT ulDataSize, IEC_UDINT ulArrayElem);
IEC_UINT BACtoIEC_ArrayOfDailySchedule(BACnetDataType *pDT, IEC_DATA *pVal, IEC_UDINT ulDataSize, IEC_UDINT *ulpArrayElem);
IEC_UINT INItoIEC_ArrayOfDailySchedule(IEC_DATA *pData, IEC_DATA **ppBuff, IEC_UDINT *ulpBuffSize);
IEC_UINT LIBtoIEC_ArrayOfDailySchedule(IEC_DATA *pData, IEC_DATA **ppBuff, IEC_UDINT *ulpBuffSize, IEC_UDINT **ppElementsBackup, STaskInfoVM *pVM, IEC_UDINT ulArrayElem, IEC_UDINT *ulpTimeVals);
IEC_UINT IECtoLIB_ArrayOfDailySchedule(IEC_DATA *pData, IEC_DATA **ppBuff, IEC_UDINT *ulpBuffSize, IEC_UDINT **ppElementsBackup, STaskInfoVM *pVM, IEC_UDINT ulArrayElem, IEC_UDINT ulTimeVals);
IEC_UINT IECtoBAC_ArrayOfSpecialEvent(BACnetDataType *pDT, IEC_DATA *pVal, IEC_UDINT ulDataSize, IEC_UDINT ulArrayElem);
IEC_UINT BACtoIEC_ArrayOfSpecialEvent(BACnetDataType *pDT, IEC_DATA *pVal, IEC_UDINT ulDataSize, IEC_UDINT *ulpArrayElem);
IEC_UINT INItoIEC_ArrayOfSpecialEvent(IEC_DATA *pData, IEC_DATA **ppBuff, IEC_UDINT *ulpBuffSize);
IEC_UINT LIBtoIEC_ArrayOfSpecialEvent(IEC_DATA *pData, IEC_DATA **ppBuff, IEC_UDINT *ulpBuffSize, IEC_UDINT **ppElementsBackup, STaskInfoVM *pVM, IEC_UDINT ulArrayElem, IEC_UDINT *ulpTimeVals);
IEC_UINT IECtoLIB_ArrayOfSpecialEvent(IEC_DATA *pData, IEC_DATA **ppBuff, IEC_UDINT *ulpBuffSize, IEC_UDINT **ppElementsBackup, STaskInfoVM *pVM, IEC_UDINT ulArrayElem, IEC_UDINT ulTimeVals);
IEC_UINT BACtoIEC_ArrayOfLogRecord(BACnetDataType *pDT, IEC_DATA *pVal, IEC_UDINT ulDataSize, IEC_UDINT *ulpArrayElem);

IEC_UINT bacBACnetDateTimeToTimeb(const BACnetDateTime *pSrc, struct timeb *pDst, int DSTFlag);
IEC_UINT bacTimebToBACnetDateTime(BACnetDateTime *pDst, const struct timeb *pSrc);
IEC_UINT bacUTCtoLocalTime(struct timeb *pSrc);

IEC_BYTE bacToggleBits(IEC_BYTE bySrc, IEC_UDINT ulBits);
IEC_UINT bacSetAPIString(IEC_CHAR **ppDst, IEC_CHAR *pSrc);


/* bacString.cpp
 * ----------------------------------------------------------------------------
 */
IEC_CHAR *bacStrObjType(BACnetObjectType eType);
IEC_CHAR *bacStrStatusFlag(BACnetStatusFlags *pState);
IEC_CHAR *bacStrNotifyType(BACnetNotifyType eType);
IEC_CHAR *bacStrEventState(BACnetEventState eState);
IEC_CHAR *bacStrEventType(BACnetEventType eType);
IEC_CHAR *bacStrPriority(IEC_UDINT ulPrio);
IEC_CHAR *bacStrAckRequired(IEC_BYTE byAckRequired);
IEC_CHAR *bacStrPropertyState(BACnetPropertyStatesChoice eState);
IEC_CHAR *bacStrData(BACnetDataType *pDT);
IEC_UINT bacStrData(BACnetDataType *pDT, IEC_CHAR *szData, IEC_UDINT ulDataLen); 
IEC_CHAR *bacStrStateOfDevice(BACnetStateOfDevice eState);
IEC_CHAR *bacStrEnableDisable(BACnetEnableDisable eED);
IEC_CHAR *bacStrErrorCode(BACnetErrorCode eEC);
IEC_CHAR *bacStrErrorClass(BACnetErrorClass eEC);

#endif /* _BACMAIN_H_ */

/* ---------------------------------------------------------------------------- */

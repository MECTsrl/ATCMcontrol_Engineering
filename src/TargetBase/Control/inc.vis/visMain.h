/* $Header: /4CReleased/V2.20.00/Target/4CWin/CONTROL/inc.vis/visMain.h 1     28.02.07 19:10 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: visMain.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/4CWin/CONTROL/inc.vis/visMain.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				RunTime
 *
 * =COMPONENT			visLib
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

#ifndef _VISMAIN_H_
#define _VISMAIN_H_

#if defined(INC_4C)
#error "Must not be included in 4C!"
#endif

#if !defined(_STDINC_H_)
#error "Include via stdInc.h only!"
#endif

#ifdef __cplusplus
extern "C" { 
#endif


/* Internal Interfaces
 * ----------------------------------------------------------------------------
 */

/* visAdapt.c
 */
IEC_UINT domVisuCommInit(void);

/* visCmd.c
 */
IEC_UINT domGetConfig(SVisInfo *pVI, XConfig *xpConf);
IEC_UINT domGetProjVersion(SVisInfo *pVI, IEC_DATA **ppGUID);
IEC_UINT domLogin (SVisInfo *pVI, IEC_DATA *pProjectID);
IEC_UINT domLogout(SVisInfo *pVI);

IEC_UINT domGetChildren(SVisInfo *pVI, IEC_DATA **ppData, IEC_UINT *upLen, IEC_BOOL bRelease);
IEC_UINT domGetAddress (SVisInfo *pVI, IEC_DATA **ppData, IEC_UINT *upLen, IEC_BOOL bRelease);

IEC_UINT domGetValue   (SVisInfo *pVI, IEC_DATA **ppData, IEC_UINT *upLen, IEC_BOOL bRelease);
IEC_UINT domGetValue20 (SVisInfo *pVI, IEC_DATA **ppData, IEC_UINT *upLen, IEC_BOOL bRelease);
IEC_UINT domGetValue21 (SVisInfo *pVI, IEC_DATA **ppData, IEC_UINT *upLen, IEC_BOOL bRelease);

IEC_UINT domSetValue   (SVisInfo *pVI, IEC_DATA **ppData, IEC_UINT *upLen, IEC_BOOL bRelease);
IEC_UINT domSetValue20 (SVisInfo *pVI, IEC_DATA **ppData, IEC_UINT *upLen, IEC_BOOL bRelease);
IEC_UINT domSetValue21 (SVisInfo *pVI, IEC_DATA **ppData, IEC_UINT *upLen, IEC_BOOL bRelease);

IEC_UINT domGetResState (SVisInfo *pVI, IEC_UDINT *ulpState);
IEC_UINT domGetTaskState(SVisInfo *pVI, IEC_CHAR *szTask, IEC_UDINT *ulpState);
IEC_UINT domGetXState	(SVisInfo *pVI, IEC_CHAR *szTask, IEC_UDINT *ulpState);

IEC_UINT domRetainWrite   (SVisInfo *pVI);
IEC_UINT domRetainSetCycle(SVisInfo *pVI, IEC_UDINT ulCycle);

/* visLic.c
 */
IEC_UINT domGetInstKey		(SVisInfo *pVI, IEC_DATA  **ppKey,		IEC_UINT *upLen);
IEC_UINT domSetLicKey		(SVisInfo *pVI, IEC_DATA  *pKey,		IEC_UINT uLen);
IEC_UINT domGetSerialNo 	(SVisInfo *pVI, IEC_UDINT *ulpSN);
IEC_UINT domGetFeature		(SVisInfo *pVI, IEC_UINT  *upAvailable, IEC_UINT *upLicensed);
IEC_UINT domGetTargetType	(SVisInfo *pVI, IEC_DATA  **ppType, 	IEC_UINT *upLen);
IEC_UINT domGetTargetVersion(SVisInfo *pVI, IEC_DATA  **ppVersion,	IEC_UINT *upLen);
IEC_UINT domSetLicEx		(SVisInfo *pVI, IEC_DATA  *pKey,		IEC_UINT uLen);

/* visCom.c
 */
IEC_UINT domInitComm	(SVisInfo *pVI);
IEC_UINT domGetPort(IEC_UINT i);
IEC_UINT domOpenComm	(SVisInfo *pVI, IEC_CHAR const *szAddress, IEC_UINT uPort);
IEC_UINT domCloseComm	(SVisInfo *pVI);
IEC_UINT domSockSend	(SVisInfo *pVI, IEC_DATA *pData, IEC_UINT uLen);
IEC_UINT domSockRecv	(SVisInfo *pVI, IEC_DATA *pData, IEC_UINT *upLen);
IEC_UINT domTransferData(SVisInfo *pVI, IEC_BYTE byCmd, IEC_DATA **ppData, IEC_UINT *upLen, IEC_BOOL bRelease);

/* visUtil.c
 */
IEC_UINT  domSwap16(SVisInfo *pVI, IEC_UINT  uVal);
IEC_UDINT domSwap32(SVisInfo *pVI, IEC_UDINT ulVal);
IEC_ULINT domSwap64(SVisInfo *pVI, IEC_ULINT ullVal);

IEC_UINT  domSwapVar20(SVisInfo *pVI, IEC_DATA *pVar, IEC_UINT uVar, IEC_DATA *pVal, IEC_UINT uVal);
IEC_UINT  domSwapVar21(SVisInfo *pVI, IEC_DATA *pVar, IEC_UINT uVar, IEC_DATA *pVal, IEC_UINT uVal);

IEC_UINT  domSwapVal(SVisInfo *pVI, IEC_DATA *pVal, IEC_UINT uVal);

IEC_UINT  domConvertVarToVal(SVisInfo *pVI, IEC_DATA *pVar, IEC_UINT uVar, XVariable *pxDest);
IEC_UINT  domConvertDBIVarToVar(SVisInfo *pVI, IEC_DATA *pDBIVar, IEC_DATA **ppDest, IEC_UINT *upDest);

#ifdef __cplusplus
} 
#endif

#endif /* _VISMAIN_H_ */

/* ---------------------------------------------------------------------------- */

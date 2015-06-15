/* $Header: /4CReleased/V2.20.00/Target/4CWin/CONTROL/inc.tst/tstMain.h 1     28.02.07 19:10 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: tstMain.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/4CWin/CONTROL/inc.tst/tstMain.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				RunTime
 *
 * =COMPONENT			ioTest
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

#ifndef _TSTMAIN_H_
#define _TSTMAIN_H_

#if defined(INC_4C)
#error "Must not be included in 4C!"
#endif

#if !defined(_STDINC_H_)
#error "Include via stdInc.h only!"
#endif


/* tstImpl.c
 * ----------------------------------------------------------------------------
 */
IEC_UINT tstInitialize(IEC_UINT uIOLayer);
IEC_UINT tstFinalize(IEC_UINT uIOLayer, SIOConfig *pIO);
IEC_UINT tstNotifyConfig(IEC_UINT uIOLayer, SIOConfig *pIO);
IEC_UINT tstNotifyStart(IEC_UINT uIOLayer, SIOConfig *pIO);
IEC_UINT tstNotifyStop(IEC_UINT uIOLayer, SIOConfig *pIO);
IEC_UINT tstNotifyGet(IEC_UINT uIOLayer, SIOConfig *pIO, SIONotify *pNotify);
IEC_UINT tstNotifySet(IEC_UINT uIOLayer, SIOConfig *pIO, SIONotify *pNotify);


/* tstMain.c
 * ----------------------------------------------------------------------------
 */
IEC_UINT tstInitBus(IEC_UINT uIOLayer);
IEC_UINT tstFinalBus(IEC_UINT uIOLayer);

IEC_UINT tstConfigBus(IEC_UINT uIOLayer, SIOConfig *pIO);

IEC_UINT tstLockRead(IEC_UINT uIOLayer, IEC_DATA OS_DPTR **ppIn, IEC_DATA OS_DPTR **ppOut);
IEC_UINT tstReleaseRead(IEC_UINT uIOLayer);

IEC_UINT tstLockWriteFirst(IEC_UINT uIOLayer, IEC_DATA OS_DPTR **ppOut);
IEC_UINT tstLockWriteSecond(IEC_UINT uIOLayer, IEC_DATA OS_DPTR **ppOut);
IEC_UINT tstReleaseWrite(IEC_UINT uIOLayer);

IEC_UINT tstGetBusState(IEC_UINT uIOLayer);

#endif /* _TSTMAIN_H_ */

/* ---------------------------------------------------------------------------- */

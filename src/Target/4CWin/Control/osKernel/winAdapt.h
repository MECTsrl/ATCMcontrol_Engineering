/* $Header: /4CReleased/V2.20.00/Target/4CWin/CONTROL/osKernel/winAdapt.h 1     28.02.07 19:10 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: winAdapt.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/4CWin/CONTROL/osKernel/winAdapt.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				RunTime
 *
 * =COMPONENT			osKernel
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

#ifndef _WINADAPT_H_
#define _WINADAPT_H_

IEC_UINT winGetDir(IEC_CHAR *szDir, IEC_UINT uSize);

DWORD WINAPI SOCKET_ListenThread(LPVOID lpParam);
DWORD WINAPI SOCKET_CommThread(LPVOID lpParam);
DWORD WINAPI VM_Proc(LPVOID lpParam);
DWORD WINAPI VMM_OnlineChangeThread(LPVOID lpParam);
DWORD WINAPI VM_TimerThread(LPVOID lpParam);
DWORD WINAPI IO_Layer_Test(LPVOID lpParam);
DWORD WINAPI IO_Layer_BACnet(LPVOID lpParam);
DWORD WINAPI VMM_RetainThread(LPVOID lpParam);
DWORD WINAPI VMM_FirmwareThread(LPVOID lpParam);
DWORD WINAPI BAC_DeviceThread(LPVOID lpParam);
DWORD WINAPI BAC_COVThread(LPVOID lpParam);
DWORD WINAPI BAC_ScanThread(LPVOID lpParam);
DWORD WINAPI BAC_FlashThread(LPVOID lpParam);
DWORD WINAPI BAC_ConfigThread(LPVOID lpParam);

IEC_UINT winOpenCom(void);
IEC_UINT winReadData(IEC_DATA *pByte);
IEC_UINT winWriteData(IEC_DATA *pData, IEC_UINT uLen);

#endif /* _WINADAPT_H_ */

/* ---------------------------------------------------------------------------- */

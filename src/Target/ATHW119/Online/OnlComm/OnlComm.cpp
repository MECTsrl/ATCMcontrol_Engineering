/* $Header: /4CReleased/V2.20.00/Target/Templates/Online/OnlComm/OnlComm.cpp 2     14.05.07 17:46 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: OnlComm.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/Target/Templates/Online/OnlComm/OnlComm.cpp $
 *
 * =PROJECT 			4CONTROL V2.x
 *
 * =SWKE				Target / Online
 *
 * =COMPONENT			OnlComm
 *
 * =CURRENT 	 $Date: 14.05.07 17:46 $
 *			 $Revision: 2 $
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"

#include "vmSharedDef.h"

#include "utilif.h"
#include "traceif.h"

#include "4CLayer.h"
#include "PhysSocket.h"
#include "Phys232.h"
#include "LogBlock.h"
#include "LogSimple.h"

/* ----  Local Defines:   ----------------------------------------------------- */

// Communication types
#define	OC_PHYS_TYPE_TCP_IP			"TCP"
#define OC_PHYS_TYPE_RS232			"RS232"

#define OC_LOG_TYPE_BLOCK			"BLOCK"
#define OC_LOG_TYPE_SIMPLE			"SIMPLE"

/* ----  Global Variables:   -------------------------------------------------- */

/* ----  Local Functions:   --------------------------------------------------- */

/* ----  Implementations:   --------------------------------------------------- */


/* ---------------------------------------------------------------------------- */
/**
 * DllMain
 *
 */
extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
		/* TODO: Replace with target name.
		 * --------------------------------------------------->
		 */
        InitTrace(hInstance,_T("SOFTWARE\\SOFTING\\4ControlV2\\ATHW119\\"),_T("VM_OnlComm"));
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        TrcTerminate();
    }
    return TRUE;    // ok
}

/* ---------------------------------------------------------------------------- */
/**
 * Construct4CLayer
 *
 */
_ONLCOMM C4CLayer* Construct4CLayer(LPCTSTR lpParam)
{
	if (lpParam == NULL)
	{
		return NULL;
	}

    // parse control string
    CString strTargetControlString = lpParam;

    CString strCommChannel;				/* Physical comm. layer:	TCP / RS232 / ...					*/
    CString strConnectPar;				/* Phys. configuration:		1.1.1.1 / wln / COM1:19200,n,8,1	*/
    CString strProtocol;				/* Logical comm. layer:		SIMPLE / BLOCK / ...				*/
    CString strProtocolPar;				/* Log. configuration:		<data block size> / ...				*/
    CString strFirmwareVersion;			/* Target Firmware version:	V2.00 -> 20000 / V2.05 -> 20500 ...	*/

    HRESULT hr = UTIL_ParseTargetControlString(strTargetControlString, 
        NULL,   // don't need opc instance name
        strCommChannel.GetBuffer(UTIL_MAX_COMM_CHANNEL_LEN), 
        strConnectPar.GetBuffer(UTIL_MAX_CONNECT_PAR_LEN), 
        strProtocol.GetBuffer(UTIL_MAX_PROTOCOL_LEN), 
        strProtocolPar.GetBuffer(UTIL_MAX_PROTOCOL_PAR_LEN), 
        strFirmwareVersion.GetBuffer(UTIL_MAX_FIRMWARE_VERSION_LEN));

    strCommChannel.ReleaseBuffer();
    strConnectPar.ReleaseBuffer();
    strProtocol.ReleaseBuffer();
    strProtocolPar.ReleaseBuffer();
    strFirmwareVersion.ReleaseBuffer();

	if (FAILED(hr))
	{
		return NULL;
	}

	// common communication configuration
	///////////////////////////////////////////////////////////////////////////
	SCommPara Para;

	memset (&Para, 0x00, sizeof(Para));

	Para.bBigEndian		= 0;
	Para.dwFirmware		= atoi(strFirmwareVersion);

	Para.dwRecvTimeOut	  = 8500;

	Para.dwDownEndRecvTO  = 8500;
	Para.dwDownBegRecvTO  = 8500;
	Para.dwOCCommitRecvTO = 8500;
	Para.dwClearResRecvTO = 8500;
	Para.dwClearResRecvTO = 8500;
	Para.dwColdWarmTO	  = 8500;

	Para.dwSendTimeOut	  = 500;
	Para.uBlockLen		  = atoi(strProtocolPar);

	TrcPrint(TRC_WARNING, "Connect:   %s\n", strTargetControlString);
    TrcPrint(TRC_WARNING, "Parameter: Big Endian: %d  Timeout: %d/%d\n", Para.bBigEndian, Para.dwSendTimeOut, Para.dwRecvTimeOut);

	CPhysLayer	*pPhys		= NULL;
	CLogLayer	*pLog		= NULL;
    C4CLayer	*p4CLayer	= NULL;

    TRY
	{
		// create physical layer
		///////////////////////////////////////////////////////////////////////////
        if(strCommChannel.CompareNoCase(OC_PHYS_TYPE_TCP_IP) == 0)
		{
			pPhys = (CPhysSocket *)CPhysSocket::CreateObject();

			((CPhysSocket *)pPhys)->SetParam(17290);
			
			/* ... */
		}
		else if(strCommChannel.CompareNoCase(OC_PHYS_TYPE_RS232) == 0)
		{
    		pPhys = (CPhys232 *)CPhys232::CreateObject();
			
			((CPhys232 *)pPhys)->SetRecvCharTimeOut(100);

			/* ... */
		}
		else
		{
			return NULL;
		}

   		pPhys->Initialize(&Para, strConnectPar);
		

		// create logical layer
		///////////////////////////////////////////////////////////////////////////
		if(strProtocol.CompareNoCase(OC_LOG_TYPE_BLOCK) == 0)
		{
			pLog = (CLogBlock *)CLogBlock::CreateObject();

			((CLogBlock *)pLog)->SetParam(250, 3);
			
			/* ... */
		}
		else if(strProtocol.CompareNoCase(OC_LOG_TYPE_SIMPLE) == 0)
		{
			pLog = (CLogSimple *)CLogSimple::CreateObject();

			/* ... */
		}
		else
		{
			return NULL;
		}

		pLog->Initialize(pPhys, &Para, strProtocolPar);

		// create 4C interface layer
		///////////////////////////////////////////////////////////////////////////
		p4CLayer = (C4CLayer *)C4CLayer::CreateObject();
		p4CLayer->Initialize(pLog, &Para, strTargetControlString);
	}

	CATCH_ALL( e )
	{
		return NULL;
	
	} END_CATCH_ALL


    return p4CLayer;
}

/* ---------------------------------------------------------------------------- */
/**
 * Dispose4CLayer
 *
 */
_ONLCOMM VOID Dispose4CLayer(C4CLayer* lpLayer)
{

	delete lpLayer;
}

/* ---------------------------------------------------------------------------- */
/**
 * SupportProgressCallBack
 *
 * If this function exists, the OnlComm DLL supports progress bar callback
 * (Just to test the existence.)
 */
_ONLCOMM BOOL SupportProgressCallBack(VOID)
{
	return TRUE;
}

/* ---------------------------------------------------------------------------- */

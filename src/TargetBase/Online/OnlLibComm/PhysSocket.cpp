/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibComm/PhysSocket.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: PhysSocket.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibComm/PhysSocket.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlLibComm
 *
 * =CURRENT 	 $Date: 28.02.07 19:06 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include "StdAfx.h"
#include <atlconv.h>
#include "PhysSocket.h"

//----  Static Initializations:   ----------------------------------*

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CPhysSocket, CPhysLayer)

static void TraceWSAError(LPCTSTR szText);

/* CPhysSocket
 * ----------------------------------------------------------------------------
 */
 CPhysSocket::CPhysSocket() : m_Socket(INVALID_SOCKET)
{
    WSADATA WSAData;

    WSAData.wVersion		= MAKEWORD(2,0);
    WSAData.wHighVersion	= 2;
    WSAData.iMaxSockets		= 10;
    WSAData.iMaxUdpDg		= 0;
    WSAData.lpVendorInfo	= NULL;

    if (WSAStartup(MAKEWORD(2,0), &WSAData) != 0)
    {
        TRACE(_T("WSAStartup failed"));
	    TrcPrint(TRC_ERROR, "[CPhysSocket][CPhysSocket] WSAStartup failed.\n");
    }

	m_uPort = 0;
}

/* ~CPhysSocket
 * ----------------------------------------------------------------------------
 */
CPhysSocket::~CPhysSocket()
{
	if (IsCommOpen())
	{
		CloseComm();
	}
}

/* OpenComm
 * ----------------------------------------------------------------------------
 */
ECommError CPhysSocket::OpenComm()
{
    sockaddr_in Addr;
    int         nAddrSize = sizeof(Addr);
    int         nTimeOut;

    if (m_Socket != INVALID_SOCKET)
    {
	    TrcPrint(TRC_ERROR, _T("[CPhysSocket][OpenComm] Already Open.\n"));
        return ceAlreadyOpen;
    }

    if ((m_Para.dwRecvTimeOut > INT_MAX) ||
        (m_Para.dwSendTimeOut > INT_MAX))
    {
	    TrcPrint(TRC_ERROR, _T("[CPhysSocket][OpenComm] Invalid Parameters.\n"));
        return ceInvalidParameters;
    }

    m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_Socket == INVALID_SOCKET)
    {
	    TraceWSAError(_T("[CPhysSocket][OpenComm][socket] %d.\n"));
        return ceError;
    }

#ifdef DEBUG
    {
        BOOL bTrue = TRUE;
        setsockopt(m_Socket, SOL_SOCKET, SO_DEBUG, (char *)&bTrue, sizeof(bTrue));
    }
#endif

    memset(&Addr, 0, nAddrSize);
    Addr.sin_family = AF_INET;
    Addr.sin_port = htons(0);
    Addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

    if (bind(m_Socket, (sockaddr *)(&Addr), nAddrSize) != 0)
    {
	    TraceWSAError(_T("[CPhysSocket][OpenComm][bind] %d.\n"));
        closesocket(m_Socket);
        m_Socket = INVALID_SOCKET;
        return ceError;
    }

    memset(&Addr, 0, nAddrSize);
    Addr.sin_family = AF_INET;
    Addr.sin_port = htons(m_uPort);
    Addr.sin_addr.S_un.S_addr = GetAddress();
    if (connect(m_Socket, (sockaddr *)(&Addr), nAddrSize) != 0)
    {
	    TraceWSAError(_T("[CPhysSocket][OpenComm][connect] %d.\n"));
        closesocket(m_Socket);
        m_Socket = INVALID_SOCKET;
        return ceError;
    }

    nTimeOut = m_Para.dwRecvTimeOut;
    setsockopt(m_Socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&nTimeOut, sizeof(nTimeOut));

    nTimeOut = m_Para.dwSendTimeOut;
    setsockopt(m_Socket, SOL_SOCKET, SO_SNDTIMEO, (char*)&nTimeOut, sizeof(nTimeOut));
    
    m_bCommOpen	 = TRUE;

    TrcPrint(TRC_WARNING, _T("[CPhysSocket][OpenComm]\n"));

    return ceOK;
}

/* CloseComm
 * ----------------------------------------------------------------------------
 */
ECommError CPhysSocket::CloseComm()
{
    ECommError Ret = ceOK;

    if (m_Socket == INVALID_SOCKET)
    {
	    TrcPrint(TRC_ERROR, _T("[CPhysSocket][CloseComm] Not Open.\n"));
        return ceNotOpen;
    }

    if (shutdown(m_Socket, SD_BOTH) != 0)
    {
	    TraceWSAError(_T("[CPhysSocket][CloseComm][shutdown] %d.\n"));
        Ret = ceError;
    }

    if (closesocket(m_Socket) != 0)
    {
	    TraceWSAError(_T("[CPhysSocket][CloseComm][closesocket] %d.\n"));
        Ret = ceError;
    }

    m_Socket = INVALID_SOCKET;

	m_bCommOpen	 = FALSE;
	m_bConnected = FALSE;

    TrcPrint(TRC_WARNING, _T("[CPhysSocket][CloseComm]\n"));

	return Ret;
}

/* Flush
 * ----------------------------------------------------------------------------
 */
ECommError CPhysSocket::Flush()
{
	return ceOK;
}

/* Connect
 * ----------------------------------------------------------------------------
 */
ECommError CPhysSocket::Connect()
{
	m_bConnected = TRUE;

	return ceOK;
}

/* Disconnect
 * ----------------------------------------------------------------------------
 */
ECommError CPhysSocket::Disconnect()
{
    if (! m_bConnected)
    {
        return ceNotConnected;
    }

	return ceOK;
}

/* Send
 * ----------------------------------------------------------------------------
 */
ECommError CPhysSocket::Send (BYTE *pData, UINT uLen)
{
	UINT uDataLen	= 0;
	UINT uSent		= 0;

	if (! m_bCommOpen)
	{
	    TrcPrint(TRC_ERROR, _T("[CPhysSocket][Send] Not Open.\n"));
		return ceNotOpen;
	}

    while(1)
	{
		uSent = send(m_Socket, (char *)pData + uDataLen, uLen - uDataLen, 0);
		if (uSent == SOCKET_ERROR || uSent == 0)
		{
			TraceWSAError(_T("[CPhysSocket][Send][send] %d.\n"));
			return ceErrorSendingData;
		}

		uDataLen += uSent;

		if (uDataLen >= uLen)
		{
			return ceOK;
		}
    }

    return ceError;
}

/* Receive
 * ----------------------------------------------------------------------------
 */
ECommError CPhysSocket::Receive (BYTE *pData, UINT *upLen)
{
    UINT uDataLen  = 0;
	UINT uReceived = 0;

	if (! m_bCommOpen)
	{
	    TrcPrint(TRC_ERROR, _T("[CPhysSocket][Receive] Not Open.\n"));
		return ceNotOpen;
	}

    while(1)
	{
		uReceived = recv(m_Socket, (char *)pData + uDataLen, *upLen - uDataLen, 0);
		if (uReceived == SOCKET_ERROR || uReceived == 0)
		{
			TraceWSAError(_T("[CPhysSocket][Receive][recv] %d.\n"));
			return ceErrorReadingData;
		}

		uDataLen += uReceived;

		if (uDataLen >= *upLen)
		{
			return ceOK;
		}

		if (uDataLen >= m_uHeader && uDataLen == TtoH(*(USHORT *)(pData + m_uOffset)) + m_uHeader)
		{
			*upLen = uDataLen;
			return ceOK;
		}
	}

    return ceError;
}

/* SetSendTimeOut
 * ----------------------------------------------------------------------------
 */
ECommError CPhysSocket::SetSendTimeOut(DWORD dwTimeOut)
{
	int nTimeOut = dwTimeOut;
    
	int iRes = setsockopt(m_Socket, SOL_SOCKET, SO_SNDTIMEO, (char*)&nTimeOut, sizeof(nTimeOut));

	if (iRes != 0)
	{
		TraceWSAError(_T("[CPhysSocket][SetSendTimeOut] %d.\n"));
		return ceError;
	}

	return CPhysLayer::SetSendTimeOut(dwTimeOut); 
}

/* SetRecvTimeOut
 * ----------------------------------------------------------------------------
 */
ECommError CPhysSocket::SetRecvTimeOut(DWORD dwTimeOut)
{
	int nTimeOut = dwTimeOut;

    int iRes = setsockopt(m_Socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&nTimeOut, sizeof(nTimeOut));

	if (iRes != 0)
	{
		TraceWSAError(_T("[CPhysSocket][SetRecvTimeOut] %d.\n"));
		return ceError;
	}

	return CPhysLayer::SetRecvTimeOut(dwTimeOut); 
}

/* GetAddress
 * ----------------------------------------------------------------------------
 */
ULONG CPhysSocket::GetAddress()
{
    TCHAR szAddr[255];
	strcpy(szAddr, m_sConnect);

	USES_CONVERSION;

    char	*szAddress = T2A(szAddr);
    IN_ADDR	InAddr;

    InAddr.s_addr = inet_addr(szAddress);

	if (InAddr.s_addr == INADDR_NONE)
	{
        // that is no dotted IP address
        // resolve name to address
		LPHOSTENT lphost;
		lphost = gethostbyname(szAddress);
		if (lphost != NULL)
			InAddr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
		else
		{
			WSASetLastError(WSAEINVAL);
			return INADDR_NONE;
		}
	}

    return InAddr.s_addr;
}

/* SetParam
 * ----------------------------------------------------------------------------
 */
ECommError	CPhysSocket::SetParam(USHORT uPort)
{
	m_uPort = uPort;
	return ceOK;
}

/* TraceWSAError
 * ----------------------------------------------------------------------------
 */
static void TraceWSAError(LPCTSTR szText)
{
	int iWSAError = WSAGetLastError();

	TRACE1(szText, iWSAError);
	TrcPrint(TRC_ERROR, szText, iWSAError);
}

/* ---------------------------------------------------------------------------- */

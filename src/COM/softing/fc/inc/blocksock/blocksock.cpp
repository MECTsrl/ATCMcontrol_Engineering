/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/blocksock/blocksock.cpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: blocksock.cpp $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/blocksock/blocksock.cpp $
 *
 * =PROJECT				CAK1020  ATCMControl V2.0
 *
 * =SWKE                INC
 *
 * =COMPONENT           INC
 *
 * =CURRENT      $Date: 28.02.07 19:01 $
 *           $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *

 



 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *== blocksock.cpp (CBlockingSocketException, CBlockingSocket, CHttpBlockingSocket)
 *== Block socket classes gratefully taken from David J. Kruglinski
 *== They've been only slightly modified to remove any MFC dependencies.
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  07.11.2000  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/
/**
 * @file blocksock.cpp classes to facilitate programming of the 
 * windows sockets API (WinSock). The windows sockets API is based
 * on the Berkely sockets API for UNIX. The classes provided are
 * devised for synchronous operation in a worker thread. Hence the calls
 * are blocking. Time limits are provided to avoid endless blocking of
 * a worker thread during an invocation.
 * <b>Acknowledgement: Blocking socket classes gratefully taken from 
 * David J. Kruglinski.</b><br>
 * They've been only slightly modified to remove MFC dependencies.
*/
//
//----  Local Defines:   -------------------------------------------*

//----  Includes:   ------------------------------------------------*
#include "blocksock.h"
//----  Static Initializations:   ----------------------------------*

//------------------------------------------------------------------*
/**
 * Constructor - initializes a CBlockingSocketException - typically
 * thrown by a method of CBlockingSocket. CBlockingSocket throws 
 * exceptions of type CBlockingSocketException in the event of errors
 * or timeouts that have occurred with a call to a method of class 
 * CBlockingSocket. Copies the message string for the user and sets
 * m_bTimeOut flag. Calls WinSock API function WSAGetLastError() to
 * obtain the WinSock error code that caused this exception.
 *
 * @param       pzsMessage - message string.
 * @param       bTimeOut - time out flag.
 * @see         class CBlockingSocket, class CBlockingSocketException 
*/
CBlockingSocketException::CBlockingSocketException(LPCTSTR pszMessage, bool bTimeOut)
{
	m_pszMessage = NULL;
	m_pszMessage = new TCHAR[_tcslen(pszMessage) +1];
	if(m_pszMessage != NULL) {
		_tcscpy(m_pszMessage,pszMessage);
	}
	m_nError = WSAGetLastError();
	m_bTimeOut = bTimeOut;
}

//------------------------------------------------------------------*
/**
 * Destructor - deletes copy of message string allocated during 
 * construction of the CBlockingSocketException.
 * @see     CBlockingSocketException()
*/
CBlockingSocketException::~CBlockingSocketException() 
{ 
	if(m_pszMessage) {
		delete [] m_pszMessage;
	}
}

//------------------------------------------------------------------*
/**
 * Retrieves the error message stored in the CBlockingSocketException
 * instance. The error message will be copied to argument lpstrError
 * Only sizeBuffer characters will be copied.
 *
 * @param       lpstrError - destination buffer receiving error string.
 *              Error string will be copied there.
 * @param       sizeBuffer - size of the buffer in characters.
 * @return      TRUE.
 *
 * @see         class CBlockingSocketException 
*/
BOOL CBlockingSocketException::GetErrorMessage(LPTSTR lpstrError, size_t sizeBuffer,
		PUINT pnHelpContext /*= NULL*/)
{

	TCHAR text[BLOCKSOCKET_LOCAL_BUFLEN];
	if(m_nError == 0) {
		_stprintf(text, _T("%s error"), m_pszMessage);
	}
	else {
		_stprintf(text, _T("%s error #%d"), m_pszMessage, m_nError);
	}
	_tcsncpy(lpstrError, text, sizeBuffer - 1);
	return TRUE;
}

//------------------------------------------------------------------*
/**
 * Traces the contents of the CBlockingSocketException exception to
 * debug output. The format of the string written to debug output is
 * as follows:<br>
 * "WINSOCK ERROR--message string #:WinSock API error code time stamp"
 * 
 * @see     class CBlockingSocketException
*/
void CBlockingSocketException::Trace()
{
#ifdef _DEBUG
	SYSTEMTIME localtime;
	TCHAR text[200];
	::GetLocalTime(&localtime);
	_stprintf(text,_T("WINSOCK ERROR--%s #:%d  %d/%d/%d %d:%d:%d\r\n"),m_pszMessage,m_nError,
localtime.wMonth,localtime.wDay,localtime.wYear,localtime.wHour,localtime.wMinute,localtime.wSecond);
	::OutputDebugString(text);
#endif
}

//------------------------------------------------------------------*
/**
 * Closes the socket if it has been opened by a call to Create().
 * Does not throw a class CBlockingSocketException in contrast to Close().
 * Hence the method can be called in a <b>catch block</b> to close
 * the socket gracefully. Asserts if closesocket() returns an error.
 *
 * @see     Close(),Create()
*/
void CBlockingSocket::Cleanup()
{
	// doesn't throw an exception because it's called in a catch block
	if(m_hSocket == NULL) return;
	BLOCKSOCKET_ASSERT(closesocket(m_hSocket) != SOCKET_ERROR);
	m_hSocket = NULL;
}

//------------------------------------------------------------------*
/**
 * Calls the WinSock function socket() and assigns the returned 
 * SOCKET handle to the member m_hSocket. Call Close() to close the
 * opened socket.
 *
 * @param       nType - socket type, shall be SOCK_STREAM (TCP)
 *              or SOCK_DGRAM (UDP). SOCK_STREAM is the default value.
 * @exception   class CBlockingSocketException with WinSock API error
 *              code if socket() returned INVALID_SOCKET.
 * @see         class CBlockingSocketException, Close()
*/
void CBlockingSocket::Create(int nType /* = SOCK_STREAM */)
{
	BLOCKSOCKET_ASSERT(m_hSocket == NULL);
	if((m_hSocket = socket(AF_INET, nType, 0)) == INVALID_SOCKET) {
		throw new CBlockingSocketException(_T("Create"));
	}
}

//------------------------------------------------------------------*
/**
 * Calls the WinSock function bind() to bind a created socket (see
 * Create()) to a given socket address. A server programm calls 
 * usually Bind() with the socket address containing the port to 
 * observe and the IP address of server prior to invoking Listen().
 * If INADDR_ANY has been passed as IP address, WinSock determines
 * the IP address of this machine. Asserts if socket has not been 
 * created.
 *
 * @param       a CSockAddr object or a refence to a variable of type
 *              sockaddr or SOCKADDR.
 * @exception   class CBlockingSocketException with WinSock API error
 *              code if bind() returned SOCKET_ERROR.
 * @see         Create(),Listen(),class CBlockingSocketException
*/
void CBlockingSocket::Bind(LPCSOCKADDR psa)
{
	BLOCKSOCKET_ASSERT(m_hSocket != NULL);
	if(bind(m_hSocket, psa, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		throw new CBlockingSocketException(_T("Bind"));
	}
}

//------------------------------------------------------------------*
/**
 * Calls the WinSock function listen(). The server usually calls 
 * Listen() to start monitoring the port that has been specified with
 * a prior call to Bind(). Call Bind() before invoking Listen().
 * Method returns immediately and does not block. Asserts if socket  
 * has not been created.
 *
 * @exception   class CBlockingSocketException with WinSock API error
 *              code if listen() returned SOCKET_ERROR.
 * @see         Bind(),class CBlockingSocketException
*/
void CBlockingSocket::Listen()
{
	BLOCKSOCKET_ASSERT(m_hSocket != NULL);
	if(listen(m_hSocket, 5) == SOCKET_ERROR) {
		throw new CBlockingSocketException(_T("Listen"));
	}
}

//------------------------------------------------------------------*
/**
 * Calls the socket function accept(). The server program should call
 * Accept() immediately after calling Listen(). Accept() returns the
 * control if a client establishes a connection to this socket. The 
 * connection is represented by another socket object resp. socket
 * handle. It is returned with the CBlockingSocket argument sConnect.<br>
 * <b>NOTE:</b> Call this function in an own thread context because 
 * the method is blocking until a client establishes a connection which
 * might never happen. Typically call Bind(),Listen() in one thread context
 * then spawn a new thread that contains the Accept() call.
 * 
 * @param   sConnect - reference to a class CBlockingSocket object. Create()
 *          shall not be called for this reference.
 * @param   psa - a CSockAddr object or a pointer to a variable of type
 *          sockaddr. Contains socket address of connection established.
 * @return  TRUE - success, FALSE - This "monitoring" socket has been 
 *          closed.
 * @exception   class CBlockingSocketException with WinSock API error
 *              code if accept() returned INVALID_SOCKET.
 * @see     Listen(), Connect()
*/
BOOL CBlockingSocket::Accept(CBlockingSocket& sConnect, LPSOCKADDR psa)
{
	BLOCKSOCKET_ASSERT(m_hSocket != NULL);
	BLOCKSOCKET_ASSERT(sConnect.m_hSocket == NULL);
	int nLengthAddr = sizeof(SOCKADDR);
	sConnect.m_hSocket = accept(m_hSocket, psa, &nLengthAddr);
	if(sConnect == INVALID_SOCKET) {
		// no exception if the listen was canceled
		if(WSAGetLastError() != WSAEINTR) {
			throw new CBlockingSocketException(_T("Accept"));
		}
		return FALSE;
	}
	return TRUE;
}

//------------------------------------------------------------------*
/**
 * Closes an opened socket by calling the WinSock API function
 * closesocket(). Create() must have been called prior to invoking this
 * method. The destructor of CBlockingSocket does not call this function
 * cause it is not possible to catch exceptions with global objects of
 * class CBlockingSocket. Hence always call Close() prior to destruction
 * of the object. Asserts if Create() hasn't been called previously.
 * Close() can also be called to wake up a worker thread blocked by a
 * a CBlockingSocket method call before ending the thread.
 *
 * @exception   class CBlockingSocketException with WinSock API error
 *              code if closesocket() returned SOCKET_ERROR.
 * @see         Create(),class CBlockingSocketException
*/
void CBlockingSocket::Close()
{
	BLOCKSOCKET_ASSERT(m_hSocket != NULL);
	if(closesocket(m_hSocket) == SOCKET_ERROR) {
		// should be OK to close if closed already
		throw new CBlockingSocketException(_T("Close"));
	}
	m_hSocket = NULL;
}

//------------------------------------------------------------------*
/**
 * Calls the socket function connect(). The client shall call Connect()
 * after it has called Create() for the socket resp. CBlockingSocket
 * object. Connect() returns the control when a connection has been
 * established or a connection timeout has been occurred. The connection
 * timeout value depends on the OS used. 
 * 
 * @param       psa - a CSockAddr object or a pointer to a variable of type
 *              sockaddr. Contains the destinationsocket address to connect 
 *              to.
 * @exception   class CBlockingSocketException with WinSock API error
 *              code if connect() returned SOCKET_ERROR. In case of an
 *              connection time out the exception will be thrown with
 *              time out flag set.
 * @see         Create(),class CBlockingSocketException
*/
void CBlockingSocket::Connect(LPCSOCKADDR psa)
{
	BLOCKSOCKET_ASSERT(m_hSocket != NULL);
	// should timeout by itself
	if(connect(m_hSocket, psa, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		throw new CBlockingSocketException(_T("Connect"));
	}
}

//------------------------------------------------------------------*
/**
 * Calls Write() - but the buffer pointer is of type BYTE*. Other
 * arguments see Write(). 
 * @see     Write()
*/
int CBlockingSocket::Write(BYTE* pb, const int nSize, const int nSecs)
{
	return Write(reinterpret_cast<const char*>(pb),nSize,nSecs);
}

//------------------------------------------------------------------*
/**
 * This function calls Send() until all bytes (passed with argument 
 * nSize) have been sent or the receiver closes the socket.
 * @param       pch - pointer to a buffer, containing the data to send.
 * @param       nSize - Size of the buffer block to send in bytes.
 * @param       nSec - time limit in seconds for select. If the send timeout
 *              expires a CBlockingSocketException will be thrown with the time
 *              out flag set by the called method Send().
 * @return      int - number of sent bytes.
 * @see         Send()
*/
int CBlockingSocket::Write(const char* pch, const int nSize, const int nSecs)
{
	int nBytesSent = 0;
	int nBytesThisTime;
	const char* pch1 = pch;
	do {
		nBytesThisTime = Send(pch1, nSize - nBytesSent, nSecs);
		nBytesSent += nBytesThisTime;
		pch1 += nBytesThisTime;
	} while(nBytesSent < nSize);
	return nBytesSent;
}


//------------------------------------------------------------------*
/**
 * Calls Send() - but the buffer pointer is of type BYTE*. Other
 * arguments see Send(). 
 * @see     Send()
*/
int CBlockingSocket::Send(BYTE* pb, const int nSize, const int nSecs)
{
	return Send(reinterpret_cast<const char*>(pb),nSize,nSecs);
}

//------------------------------------------------------------------*
/**
 * This method calls the sockets function send() after a call to select()
 * to activate the time limit. The number of actually with a call to Send()
 * transferred bytes depends how fast the program on the other side of the
 * connection can receive. Send() throws an exception if the program on the
 * other side of the connection closes the socket before receiving all data.
 * 
 * @param       pch - pointer to a buffer, containing the data to send.
 * @param       nSize - Size of the buffer block to send in bytes.
 * @param       nSec - time limit in seconds for select. If the send timeout
 *              expires a CBlockingSocketException will be thrown with the time
 *              out flag set.
 * @return      int - number of actually sent bytes.
 * @exception   class CBlockingSocketException with WinSock API error
 *              code if send() returned SOCKET_ERROR. In case of an
 *              send time out the exception will be thrown with
 *              time out flag set.
 * @see         class CBlockingSocketException.
*/
int CBlockingSocket::Send(const char* pch, const int nSize, const int nSecs)
{
	BLOCKSOCKET_ASSERT(m_hSocket != NULL);
	// returned value will be less than nSize if client cancels the reading
	FD_SET fd = {1, m_hSocket};
	TIMEVAL tv = {nSecs, 0};
	if(select(0, NULL, &fd, NULL, &tv) == 0) {
		throw new CBlockingSocketException(_T("Send timeout"),true);
	}
	int nBytesSent;
	if((nBytesSent = send(m_hSocket, pch, nSize, 0)) == SOCKET_ERROR) {
		throw new CBlockingSocketException(_T("Send"));
	}
	return nBytesSent;
}

//------------------------------------------------------------------*
/**
 * Calls Receive() - but the buffer pointer is of type BYTE*. Other
 * arguments see Receive(). 
 * @see     Receive()
*/
int CBlockingSocket::Receive(BYTE* pb, const int nSize, const int nSecs)
{
	return Receive(reinterpret_cast<char *>(pb),nSize,nSecs);
}

//------------------------------------------------------------------*
/**
 * Calls the sockets function recv() after a call to select()
 * to activate the time limit. The maximum number of bytes to receive
 * will be determined with the argument nSize passed to this method.
 * The method returns the actual number of bytes received with one call
 * to recv(). This can be less than nSize. If the sender on the other side
 * of the connection sends more slowly the function has to be called repeatedly.
 * @param       pch - pointer to a existing buffer, to receive the incoming data.
 * @param       nSize - Size of the buffer block or maximum number of bytes to
 *              receive with this call.
 * @param       nSec - time limit in seconds for select. If the receive timeout
 *              expires a CBlockingSocketException will be thrown with the time
 *              out flag set.
 * @return      int - number of actually received bytes.
 * @exception   class CBlockingSocketException with WinSock API error
 *              code if recv() returned SOCKET_ERROR. In case of an
 *              receive time out the exception will be thrown with
 *              time out flag set.
 * @see         class CBlockingSocketException, Socket function recv(), Send()
*/
int CBlockingSocket::Receive(char* pch, const int nSize, const int nSecs)
{
	BLOCKSOCKET_ASSERT(m_hSocket != NULL);
	FD_SET fd = {1, m_hSocket};
	TIMEVAL tv = {nSecs, 0};
	if(select(0, &fd, NULL, NULL, &tv) == 0) {
		throw new CBlockingSocketException(_T("Receive timeout"),true);
	}

	int nBytesReceived;
	if((nBytesReceived = recv(m_hSocket, pch, nSize, 0)) == SOCKET_ERROR) {
		throw new CBlockingSocketException(_T("Receive"));
	}
	return nBytesReceived;
}

//------------------------------------------------------------------*
/**
 * This UDP function calls the sockets function recvfrom(). The function
 * returns control if the program/entity on the other side of the 
 * "connection" calls SendDatagram() or sendto(). For UDP datagrams the 
 * functions Listen(),Accept() and  Connect() do not have to be called. 
 * The socket resp. CBlockingSocket object has to be created (Create()) 
 * with parameter <b>SOCK_DGRAM</b> set.
 * @param       pch - pointer to a existing buffer, to receive the incoming data.
 * @param       nSize - Size of the buffer block or maximum number of bytes to
 *              receive with this call.
 * @param       psa - a CSockAddr object or a pointer to a variable of type
 *              sockaddr. Contains the socket address of the sending entity.
 * @param       nSec - time limit in seconds for select. If the receive timeout
 *              expires a CBlockingSocketException will be thrown with the time
 *              out flag set.
 * @return      int - number of actually received bytes.
 * @exception   class CBlockingSocketException with WinSock API error
 *              code if recvfrom() returned SOCKET_ERROR. In case of an
 *              receive time out the exception will be thrown with
 *              time out flag set.
 * @see         class CBlockingSocketException, Socket function recvfrom(),
 *              SendDatagram().
*/
int CBlockingSocket::ReceiveDatagram(char* pch, const int nSize, LPSOCKADDR psa, const int nSecs)
{
	BLOCKSOCKET_ASSERT(m_hSocket != NULL);
	FD_SET fd = {1, m_hSocket};
	TIMEVAL tv = {nSecs, 0};
	if(select(0, &fd, NULL, NULL, &tv) == 0) {
		throw new CBlockingSocketException(_T("Receive timeout"),true);
	}

	// input buffer should be big enough for the entire datagram
	int nFromSize = sizeof(SOCKADDR);
	int nBytesReceived = recvfrom(m_hSocket, pch, nSize, 0, psa, &nFromSize);
	if(nBytesReceived == SOCKET_ERROR) {
		throw new CBlockingSocketException(_T("ReceiveDatagram"));
	}
	return nBytesReceived;
}

//------------------------------------------------------------------*
/**
 * This UDP function calls the sockets function sendto(). The program
 * on the other side of the "connection" has to call ReceiveDatagram() or
 * recvfrom(). For UDP datagrams the functions Listen(),Accept() and 
 * Connect() do not have to be called. The socket resp. CBlockingSocket
 * object has to be created (Create()) with parameter <b>SOCK_DGRAM</b> set.
 *
 * @param       pch - pointer to a buffer, containing the data to send.
 * @param       nSize - Size of the buffer block to send in bytes.
 * @param       psa - a CSockAddr object or a pointer to a variable of type
 *              sockaddr. Contains the socket address of the receiving entity.
 * @param       nSec - time limit in seconds for select. If the send timeout
 *              expires a CBlockingSocketException will be thrown with the time
 *              out flag set.
 * @return      int - number of actually sent bytes.
 * @exception   class CBlockingSocketException with WinSock API error
 *              code if sendto() returned SOCKET_ERROR. In case of an
 *              send time out the exception will be thrown with
 *              time out flag set.
 * @see         class CBlockingSocketException, ReceiveDatagram(),Socket function 
 *              sendto().
*/
int CBlockingSocket::SendDatagram(const char* pch, const int nSize, LPCSOCKADDR psa, const int nSecs)
{
	BLOCKSOCKET_ASSERT(m_hSocket != NULL);
	FD_SET fd = {1, m_hSocket};
	TIMEVAL tv = {nSecs, 0};
	if(select(0, NULL, &fd, NULL, &tv) == 0) {
		throw new CBlockingSocketException(_T("Send timeout"),true);
	}

	int nBytesSent = sendto(m_hSocket, pch, nSize, 0, psa, sizeof(SOCKADDR));
	if(nBytesSent == SOCKET_ERROR) {
		throw new CBlockingSocketException(_T("SendDatagram"));
	}
	return nBytesSent;
}

//------------------------------------------------------------------*
/**
 * Calls the sockets function getpeername(). The function returns the 
 * port number and IP address of the socket on the other side of the
 * connection. A valid TCP/IP connection has to be establishe prior 
 * to calling this function.
 * 
 * @param       psa - a CSockAddr object or a pointer to a variable of type
 *              sockaddr. The socket address of the connection counter part
 *              will be returned with this parameter.
 * @exception   class CBlockingSocketException with WinSock API error
 *              code if getpeername() returned SOCKET_ERROR. 
 * @see         class CBlockingSocketException, GetSockAddr()
*/
void CBlockingSocket::GetPeerAddr(LPSOCKADDR psa)
{
	BLOCKSOCKET_ASSERT(m_hSocket != NULL);
	// gets the address of the socket at the other end
	int nLengthAddr = sizeof(SOCKADDR);
	if(getpeername(m_hSocket, psa, &nLengthAddr) == SOCKET_ERROR) {
		throw new CBlockingSocketException(_T("GetPeerName"));
	}
}

//------------------------------------------------------------------*
/**
 * Calls the sockets function getsockname(). Returns the socket address
 * assigned to this side of the connection. Hence the socket address 
 * assigned to the "calling" host machine will be returned.
 * 
 * @param       psa - a CSockAddr object or a pointer to a variable of type
 *              sockaddr. The socket address of this connection side
 *              will be returned with this parameter.
 * @exception   class CBlockingSocketException with WinSock API error
 *              code if getsockname() returned SOCKET_ERROR. 
 * @see         class CBlockingSocketException, GetPeerAddr()
*/
void CBlockingSocket::GetSockAddr(LPSOCKADDR psa)
{
	BLOCKSOCKET_ASSERT(m_hSocket != NULL);
	// gets the address of the socket at this end
	int nLengthAddr = sizeof(SOCKADDR);
	if(getsockname(m_hSocket, psa, &nLengthAddr) == SOCKET_ERROR) {
		throw new CBlockingSocketException(_T("GetSockName"));
	}
}

//------------------------------------------------------------------*
/**
 * This static method calls sockets function gethostbyname(). The 
 * functions queries a name server (DNS) with the passed hostname and
 * returns the socket address along with the host name in a CSockAddr
 * object. The function gethostbyname() itself sets a time limit.
 * The returned socket address has network format.
 * @param       pchName - pointer to a string containing the host name
 *              to query the name server.
 * @param       ushPort - Port number (default value is 0). The port 
 *              number will be set into the returned CSockAddr object.
 * @return      CSockAddr object resp. socket address containing the 
 *              IP address returned from the DNS and the port number
 *              ushPort.
 * @exception   class CBlockingSocketException with WinSock API error
 *              code if gethostbyname() returned a null hostent pointer. 
 * @see         class CBlockingSocketException, socket function 
 *              gethostbyname(), GetHostByAddr()
*/
CSockAddr CBlockingSocket::GetHostByName(const char* pchName, const USHORT ushPort)
{
	hostent* pHostEnt = gethostbyname(pchName);
	if(pHostEnt == NULL) {
		throw new CBlockingSocketException(_T("GetHostByName"));
	}
	ULONG* pulAddr = (ULONG*) pHostEnt->h_addr_list[0];
	SOCKADDR_IN sockTemp;
	sockTemp.sin_family = AF_INET;
	sockTemp.sin_port = htons(ushPort);
	sockTemp.sin_addr.s_addr = *pulAddr; // address is already in network byte order
	return sockTemp;
}

//------------------------------------------------------------------*
/**
 * This static method calls the sockets function gethostbyaddr().
 * The function queries the name server and returns the host name
 * that correspondst to the passed socket address.The function 
 * gethostbyaddr() itself sets a time limit.
 * The passed socket address shall have network format.
 * @param       psa - a CSockAddr object or a pointer to a variable of type
 *              sockaddr. The socket address to query the host name for 
 *              is passed with this argument. Socket addess shall have
 *              network format.
 * @return      Pointer to a string containing the host name. Caller shall
 *              not delete the string referenced by the returned pointer.
 * @exception   class CBlockingSocketException with WinSock API error
 *              code if gethostbyaddr() returned a null hostent pointer. 
 * @see         class CBlockingSocketException, socket function 
 *              gethostbyaddr(), GetHostByName()
*/
const char* CBlockingSocket::GetHostByAddr(LPCSOCKADDR psa)
{
	hostent* pHostEnt = gethostbyaddr((char*) &((LPSOCKADDR_IN) psa)
				->sin_addr.s_addr, 4, PF_INET);
	if(pHostEnt == NULL) {
		throw new CBlockingSocketException(_T("GetHostByAddr"));
	}
	return pHostEnt->h_name; // caller shouldn't delete this memory
}

//------------------------------------------------------------------*
/**
 * The static method retrieves the socket address of the (local) host
 * machine. Calls the sockets function gethostname(). The function returns
 * the host name of this (local) host machine. Then the method calls
 * GetHostByName() to build the socket address from the host name and the
 * passed port number.
 * 
 * @param       ushPort - Port number (default value is 0). The port 
 *              number will be set into the returned CSockAddr object.
 * @return      CSockAddr object resp. socket address containing the 
 *              IP address returned the local host machine and the 
 *              port number ushPort.
 * @exception   class CBlockingSocketException with WinSock API error
 *              code if gethostname() returned a SOCKET_ERROR. 
 * @see         class CBlockingSocketException, socket function gethostname()
 *              GetHostByName().
*/
CSockAddr CBlockingSocket::GetLocalAddr(const USHORT ushPort)
{
	char szBuffer[BLOCKSOCKET_LOCAL_BUFLEN];
	if (::gethostname(szBuffer,sizeof(szBuffer)/sizeof(szBuffer[0])) == SOCKET_ERROR) {
		throw new CBlockingSocketException(_T("GetLocalAddr"));
	}
	return GetHostByName(szBuffer,ushPort);				
}

//------------------------------------------------------------------*
/**
 * The static method retrieves the host name of the (local) host
 * machine. Calls the sockets function gethostname(). The function returns
 * the host name of this (local) host machine. This host name will be
 * written to the passed parameter pszBuffer. This the <b>ANSI</b> version the
 * GetLocalHost* function set.
 * 
 * @param       pszBuffer - character buffer to receive the host name of
 *              the local host machine.
 * @param       sizeChars - size of the character buffer in characters. If
 *              the retrieved host name does not fit into the buffer only the
 *              first sizeChars characters will be copied into the buffer.
 * @exception   class CBlockingSocketException with WinSock API error
 *              code if gethostname() returned a SOCKET_ERROR. 
 * @see         class CBlockingSocketException, socket function gethostname()
 *              GetLocalHostW().
*/
void CBlockingSocket::GetLocalHostA(LPSTR pszBuffer, size_t sizeChars)
{
	char szBuffer[BLOCKSOCKET_LOCAL_BUFLEN];
	if (::gethostname(szBuffer,sizeof(szBuffer)/sizeof(szBuffer[0])) == SOCKET_ERROR) {
		throw new CBlockingSocketException(_T("GetLocalHost"));
	}
	strncpy(pszBuffer,szBuffer,sizeChars);
}

//------------------------------------------------------------------*
/**
 * The static method retrieves the host name of the (local) host
 * machine. Calls the sockets function gethostname(). This the <b>UNICODE</b> 
 * version the GetLocalHost* function set. For the interface of this
 * function see method GetLocalHostA().
 * @see     GetLocalHostA().
*/
void CBlockingSocket::GetLocalHostW(LPWSTR pszBuffer, size_t sizeChars)
{
	char szBuffer[BLOCKSOCKET_LOCAL_BUFLEN];
	if (::gethostname(szBuffer,sizeof(szBuffer)/sizeof(szBuffer[0])) == SOCKET_ERROR) {
		throw new CBlockingSocketException(_T("GetLocalHost"));
	}
	//multibyte to wide character conversion
	size_t len = strlen(szBuffer) + 1;
	WCHAR *pUniBuf = new WCHAR[len];
	BLOCKSOCKET_ASSERT(pUniBuf);
	::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szBuffer, -1, pUniBuf, len);
	wcsncpy(pszBuffer,pUniBuf,sizeChars);
	delete [] pUniBuf;
}


// Class CHttpBlockingSocket
CHttpBlockingSocket::CHttpBlockingSocket()
{
	m_pReadBuf = new char[nSizeRecv];
	m_nReadBuf = 0;
}

CHttpBlockingSocket::~CHttpBlockingSocket()
{
	delete [] m_pReadBuf;
}

int CHttpBlockingSocket::ReadHttpHeaderLine(char* pch, const int nSize, const int nSecs)
// reads an entire header line through CRLF (or socket close)
// inserts zero string terminator, object maintains a buffer
{
	int nBytesThisTime = m_nReadBuf;
	int nLineLength = 0;
	char* pch1 = m_pReadBuf;
	char* pch2;
	do {
		// look for lf (assume preceded by cr)
		if((pch2 = (char*) memchr(pch1 , '\n', nBytesThisTime)) != NULL) {
			BLOCKSOCKET_ASSERT((pch2) > m_pReadBuf);
			BLOCKSOCKET_ASSERT(*(pch2 - 1) == '\r');
			nLineLength = (pch2 - m_pReadBuf) + 1;
			if(nLineLength >= nSize) nLineLength = nSize - 1;
			memcpy(pch, m_pReadBuf, nLineLength); // copy the line to caller
			m_nReadBuf -= nLineLength;
			memmove(m_pReadBuf, pch2 + 1, m_nReadBuf); // shift remaining characters left
			break;
		}
		pch1 += nBytesThisTime;
		nBytesThisTime = Receive(m_pReadBuf + m_nReadBuf, nSizeRecv - m_nReadBuf, nSecs);
		if(nBytesThisTime <= 0) { // sender closed socket or line longer than buffer
			throw new CBlockingSocketException(_T("ReadHeaderLine"));
		}
		m_nReadBuf += nBytesThisTime;
	}
	while(TRUE);
	*(pch + nLineLength) = '\0';
	return nLineLength;
}

int CHttpBlockingSocket::ReadHttpResponse(char* pch, const int nSize, const int nSecs)
// reads remainder of a transmission through buffer full or socket close
// (assume headers have been read already)
{
	int nBytesToRead, nBytesThisTime, nBytesRead = 0;
	if(m_nReadBuf > 0) { // copy anything already in the recv buffer
		memcpy(pch, m_pReadBuf, m_nReadBuf);
		pch += m_nReadBuf;
		nBytesRead = m_nReadBuf;
		m_nReadBuf = 0;
	}
	do { // now pass the rest of the data directly to the caller
		nBytesToRead = min(nSizeRecv, nSize - nBytesRead);
		nBytesThisTime = Receive(pch, nBytesToRead, nSecs);
		if(nBytesThisTime <= 0) break; // sender closed the socket
		pch += nBytesThisTime;
		nBytesRead += nBytesThisTime;
	}
	while(nBytesRead <= nSize);
	return nBytesRead;
}


/*
 *----------------------------------------------------------------------------*
 *  $History: blocksock.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc/blocksock
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc/blocksock
 * 
 * *****************  Version 3  *****************
 * User: Su           Date: 3.12.01    Time: 11:02
 * Updated in $/4Control/COM/softing/fc/inc/blocksock
 * Precompiled headers do not work correctly with system generation. Need
 * to make a library out ouf blocksock.cpp. Postpone this action right
 * now.
 * 
 * *****************  Version 2  *****************
 * User: Su           Date: 28.03.01   Time: 9:43
 * Updated in $/4Control/COM/softing/fc/inc/blocksock
 * Finalized documentation
 * 
 * *****************  Version 1  *****************
 * User: Su           Date: 19.03.01   Time: 19:30
 * Created in $/4Control/COM/softing/fc/inc/blocksock
 * Moved from "Messeprojekt" - common socket classes for socket
 * programming. Adapted from David J. Kruglinsky
 * 
 * *****************  Version 5  *****************
 * User: Su           Date: 13.12.00   Time: 19:12
 * Updated in $/4ControlMesse/COM/softing/fc/inc
 * Added missing methods: GetLocalAddr, GetLocalHostA/W.
 * 
 *==
 *----------------------------------------------------------------------------*
*/

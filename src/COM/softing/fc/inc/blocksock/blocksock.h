/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/blocksock/blocksock.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: blocksock.h $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/blocksock/blocksock.h $
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
 *== see file section
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  07.11.2000  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/
/**
 * @file blocksock.h classes to facilitate programming of the 
 * windows sockets API (WinSock). The windows sockets API is based
 * on the Berkely sockets API for UNIX. The classes provided are
 * devised for synchronous operation in a worker thread. Hence the calls
 * are blocking. Time limits are provided to avoid endless blocking of
 * a worker thread during an invocation.
 * <b>Acknowledgement: Blocking socket classes gratefully taken from 
 * David J. Kruglinski.</b><br>
 * They've been only slightly modified to remove MFC dependencies.
*/
#ifndef __BLOCKSOCK_H_
#define __BLOCKSOCK_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//----  Aggregate Includes:   --------------------------------------*
#include "winsock.h"
#include "assert.h"
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*
#ifndef BLOCKSOCKET_ASSERT
/**
* @def BLOCKSOCKET_ASSERT - for other assert mechanisms replace only the define here.
*/
#define BLOCKSOCKET_ASSERT(x) assert((x))
#endif
/**
* @def BLOCKSOCKET_LOCAL_BUFLEN - buffer length of temporary buffer.
*/
#define BLOCKSOCKET_LOCAL_BUFLEN	(200)
//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*
typedef const struct sockaddr* LPCSOCKADDR;

//------------------------------------------------------------------*
/**
 * class CBlockingSocketException - exception class for exceptions
 * thrown by class CBlockingSocket. CBlockingSocket has been devised
 * for sychronous resp. blocking operation in a worker thread. 
 * CBlockingSocket throws exceptions of type CBlockingSocketException
 * in the event of errors or timeouts that have occurred with a
 * call to a method of class CBlockingSocket. These exceptions enable
 * the user to avoid tedious error checks after each call to a 
 * CBlockingSocket method.<br>
 * <b>Usage:</b><br>
 * Encapsulate calls to CBlockingSocket methods into a try<br>
 * <b>try</b><br>
 * {<br>
 *  CBlockingSocket method calls ...<br>
 * }<br>
 * <b>catch(CBlockingSocketException* e)</b><br>
 * {<br>
 *      error handling ...<br>
 *      delete e;<br>
 * }<br>
 * catch block and perform error handling in the catch block.<br>
 * @see     class CBlockingSocket
*/
class CBlockingSocketException
{
public:
	CBlockingSocketException(LPCTSTR pszMessage,bool bTimeOut = false);

public:
	~CBlockingSocketException();
	virtual BOOL GetErrorMessage(LPTSTR lpstrError, size_t sizeBuffer,
		PUINT pnHelpContext = NULL);
	virtual void Trace();
	
    //------------------------------------------------------------------*
    /**
     * Returns the WinSock API error code constructed with this exception.
     * During construction of the exception the error code will be read
     * from the WinSock API by a call to the WinSock API function 
     * WSAGetLastError().
     * @return  WinSock API error code or 0 in case of timeout exception.
     * @see     WSAGetLastError(),IsTimeOut()
    */	
	int GetErrorCode()
	{ 
		return m_nError;
	}

    //------------------------------------------------------------------*
    /**
     * Returns true if a timeout has been occurred. Otherwise false.
     * Another indication of a timeout is if an exception occurs but
     * the error code (see GetErrorCode()) is 0.
     * @return  true - timeout occurred, false no timeout.
     * @see     GetErrorCode()
    */	
	bool IsTimeOut() {
		return m_bTimeOut;
	}
private:
	int m_nError;           /**< WinSock API error code - see GetErrorCode()*/
	bool m_bTimeOut;        /**< time out ?*/
	TCHAR * m_pszMessage;   /**< error message */
};

//------------------------------------------------------------------*
/**
 * class CSockAddr - Encapsulation of sockaddr_in.
 * Many functions of the WinSock API receive socket addresses as arguments.
 * Those socket addresses are comprised of a 32 bit IP address and 
 * a 16 bit wide port number. The WinSock API  works with a structure 
 * called sockaddr_in and stores the IP address in a structure of type
 * in_addr. Both structures are very unhandy. To facilitate usage a user
 * friendly class has been defined with CSockAddr.<br>
 * <b>NOTE:<br>
 * IP address and port number are always stored in network byte order.</b><br>
 * @see     sockaddr_in, in_addr, class CBlockingSocket
*/
class CSockAddr : public sockaddr_in {
public:
	// constructors
    //------------------------------------------------------------------*
    /**
     * Default constructor - sets family to AF_INET, port and IP address
     * to all zeros.
     * @see     class CSockAddr description
    */	
	CSockAddr()
    {   sin_family = AF_INET;
        sin_port = 0;
        sin_addr.s_addr = 0; 
    }
    //------------------------------------------------------------------*
    /**
     * Conversion constructor - from SOCKADDR reference to CSockAddr.
     * 
     * @param   reference to a SOCKADDR structure often used with WinSock
     *          API.
     * @see     class CSockAddr description
    */	
	CSockAddr(const SOCKADDR& sa) { memcpy(this, &sa, sizeof(SOCKADDR)); }

    //------------------------------------------------------------------*
    /**
     * Conversion constructor - from SOCKADDR_IN reference to CSockAddr.
     * 
     * @param   reference to a SOCKADDR_IN structure often used with WinSock
     *          API.
     * @see     class CSockAddr description
    */	
	CSockAddr(const SOCKADDR_IN& sin) { memcpy(this, &sin, sizeof(SOCKADDR_IN)); }
	
    //------------------------------------------------------------------*
    /**
     * Conversion constructor - takes IP address and port number represented
     * in host byte order and initializes CSockAddr object. IP address and
     * port number are always stored in network byte order.
     *
     * @param   ulAddr - unsigned long representing IP address in host byte
     *          order.
     * @param   ushPort - unsigned short representing port number in host byte
     *          order. Parameter can be omitted. In this case the port number
     *          will be initialized to 0.
     * @see     class CSockAddr description
    */	
	CSockAddr(const ULONG ulAddr, const USHORT ushPort = 0) // parms are host byte ordered
    {   sin_family = AF_INET;
        sin_port = htons(ushPort);
        sin_addr.s_addr = htonl(ulAddr);
    }

    //------------------------------------------------------------------*
    /**
     * Conversion constructor - takes dotted IP address string and port 
     * number represented in host byte order and initializes CSockAddr 
     * object. IP address and port number are always stored in network 
     * byte order. The dotted IP address string ("127.0.0.1") will be 
     * converted into an IP address represented in network byte order.
     *
     * @param   pchIP - dotted IP address string ("127.0.0.1").
     * @param   ushPort - unsigned short representing port number in host byte
     *          order. Parameter can be omitted. In this case the port number
     *          will be initialized to 0.
     * @see     class CSockAddr description
    */	    
	CSockAddr(const char* pchIP, const USHORT ushPort = 0) 
	{   sin_family = AF_INET;
	    sin_port = htons(ushPort);
	    sin_addr.s_addr = inet_addr(pchIP); // already network byte ordered
	}
	
    //------------------------------------------------------------------*
    /**
     * Returns the IP address stored in the CSockAddr object in dotted
     * decimal format (example "127.0.0.1"). Performs unicode conversion if
     * appropriate.
     * @param   pszBuffer - pointer to string buffer receiving the IP
     *          address in dotted decimal format.
     * @param   sizeChars - size of the provided buffer in characters.
     *          If the size is too small only the number of characters
     *          that fit the buffer will be copied.
     * @see     CSockAddr()
    */
	void DottedDecimal(LPTSTR pszBuffer,size_t sizeChars)
	{ 
        char * pc = inet_ntoa(sin_addr);
        if(!pc)	return;
#ifdef UNICODE
        //multibyte to wide character conversion
        size_t len = strlen(pc) + 1;
        WCHAR *pUniBuf = new WCHAR[len];
        BLOCKSOCKET_ASSERT(pUniBuf);
        ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pc, -1, pUniBuf, len);
        _tcsncpy(pszBuffer,pUniBuf,sizeChars);
        delete [] pUniBuf;
#else
        strncpy(pszBuffer,pc,sizeChars);
#endif
	}
	
    //------------------------------------------------------------------*
    /**
     * Returns the port number stored in the CSockAddr object in host
     * byte order. To get the IP address in host byte order call IPAddr()
     *
     * @return      unsigned short port number in host byte order.
     * @see         IPAddr()
    */
	USHORT Port() const
    {   
        return ntohs(sin_port);
    }

    //------------------------------------------------------------------*
    /**
     * Returns the IP address stored in the CSockAddr object in host
     * byte order. To get the port number in host byte order call Port()
     *
     * @return      unsigned long IP address in host byte order.
     * @see         Port()
    */
	ULONG IPAddr() const
	{
	    return ntohl(sin_addr.s_addr);
    }
	
	// operators added for efficiency
    //------------------------------------------------------------------*
    /**
     * Assignment operator - from SOCKADDR reference to CSockAddr.
     * 
     * @param   reference to a SOCKADDR structure often used with WinSock
     *          API.
     * @return  reference to this CSockAddr object.
     * @see     class CSockAddr description, operator =().
    */
	const CSockAddr& operator=(const SOCKADDR& sa)
    {   memcpy(this, &sa, sizeof(SOCKADDR));
        return *this;
    }
		  
    //------------------------------------------------------------------*
    /**
     * Assignment operator - from SOCKADDR_IN reference to CSockAddr.
     * 
     * @param   reference to a SOCKADDR_IN structure often used with WinSock
     *          API.
     * @return  reference to this CSockAddr object.
     * @see     class CSockAddr description, operator =().
    */
	const CSockAddr& operator=(const SOCKADDR_IN& sin)
    {   memcpy(this, &sin, sizeof(SOCKADDR_IN));
        return *this;
    }
    
    //------------------------------------------------------------------*
    /**
     * conversion operator to convert CSockAddr object into SOCKADDR 
     * structure often used with WinSock API.
     * 
     * @return      this CSockAddr object as SOCKADDR structure.
     *
     * @see         operator LPSOCKADDR(), operator LPSOCKADDR_IN()
    */		  
	operator SOCKADDR()
    {   
        return *((LPSOCKADDR) this);
    }

    //------------------------------------------------------------------*
    /**
     * conversion operator to get address of SOCKADDR structure from
     * CSockAddr object. The structure is often used with WinSock API.
     * 
     * @return      address of SOCKADDR structure encapsulated in 
     *              this CSockAddr object.
     * @see         operator SOCKADDR(), operator LPSOCKADDR_IN()
    */		  
	operator LPSOCKADDR()
    {
        return (LPSOCKADDR) this;
    }
    
    //------------------------------------------------------------------*
    /**
     * conversion operator to get address of SOCKADDR_IN structure from
     * CSockAddr object. The structure is often used with WinSock API.
     * 
     * @return      address of SOCKADDR_IN structure encapsulated in 
     *              this CSockAddr object.
     * @see         operator SOCKADDR(), operator LPSOCKADDR()
    */		
	operator LPSOCKADDR_IN()
    {   
        return (LPSOCKADDR_IN) this;
    }
};

//------------------------------------------------------------------*
/**
 * class CBlockingSocket - blocking socket class.
 * The windows sockets API is based on the Berkely sockets API for UNIX. 
 * The blocking socket class has been  devised for synchronous operation
 * in a worker thread. Hence the calls are blocking. 
 * Time limits are provided to avoid endless blocking of a worker thread
 * during an invocation. CBlockingSocket throws exceptions of type 
 * CBlockingSocketException in the event of errors or timeouts that have
 * occurred with a call to a method of class CBlockingSocket.<br>
 * <b>NOTE: Member functions truly block and must not be used 
 * in UI threads. Use them in separate worker threads solely.<br>
 * Acknoledgement: Blocking socket classes gratefully taken from 
 * David J. Kruglinski.</b>
 * @see     class CBlockingSocketException
*/
class CBlockingSocket
{
public:
	SOCKET m_hSocket;           /**< the SOCKET handle*/
	
    //------------------------------------------------------------------*
    /**
     * constructor - creates a non initialized CBlockingSocket instance.
     * Call Create() to establish a windows socket and associate the socket
     * with the CBlockingSocket object.
     * @see     Create()
    */	
	CBlockingSocket() 
	{ 
	    m_hSocket = NULL;
	}
	
	void Cleanup();
	void Create(int nType = SOCK_STREAM);
	void Close();
	void Bind(LPCSOCKADDR psa);
	void Listen();
	void Connect(LPCSOCKADDR psa);
	BOOL Accept(CBlockingSocket& s, LPSOCKADDR psa);
	int Send(BYTE* pb, const int nSize, const int nSecs);
	int Send(const char* pch, const int nSize, const int nSecs);
    int Write(BYTE* pb, const int nSize, const int nSecs);
	int Write(const char* pch, const int nSize, const int nSecs);
	int Receive(BYTE* pb, const int nSize, const int nSecs);
	int Receive(char* pch, const int nSize, const int nSecs);
	int SendDatagram(const char* pch, const int nSize, LPCSOCKADDR psa, 
		const int nSecs);
	int ReceiveDatagram(char* pch, const int nSize, LPSOCKADDR psa, 
		const int nSecs);
	void GetPeerAddr(LPSOCKADDR psa);
	void GetSockAddr(LPSOCKADDR psa);
	static CSockAddr GetHostByName(const char* pchName, 
		const USHORT ushPort = 0);
	static CSockAddr GetLocalAddr(const USHORT ushPort= 0);
	static void GetLocalHostA(LPSTR pszBuffer, size_t sizeChars);
	static void GetLocalHostW(LPWSTR pszBuffer, size_t sizeChars);
	static const char* GetHostByAddr(LPCSOCKADDR psa);
	
    //------------------------------------------------------------------*
    /**
     * conversion operator to get the SOCKET handle used with the
     * WinSock API. The overloaded operator allows for the usage of the
     * CBlockingSocket object instead of the SOCKET parameter.
     *
     * @return      SOCKET handle - maybe NULL if Create() hasn't been 
     *              called or Cleanup(),Close() has been called prior to
     *              invocation of the operator. 
     * @see         Create(),Close(),Cleanup()     
    */		  
	operator SOCKET()
    {   
        return m_hSocket; 
    }

    //------------------------------------------------------------------*
    /**
     * Checks for a valid socket handle. Returns true for a valid non NULL
     * socket handle of the WinSock API. The socket handle is valid between
     * calls to Create() and invocation of Cleanup() or Close().
     * 
     * @return      true - valid socket handle, false - socket handle = NULL.
     * @see         operator SOCKET(),Create(),Close(),Cleanup()
    */		  
	bool IsValidSocket()
	{
		return m_hSocket != NULL;
	}
};

class CHttpBlockingSocket : public CBlockingSocket
{
public:
	enum {nSizeRecv = 1000}; // max receive buffer size (> hdr line length)
	CHttpBlockingSocket();
	~CHttpBlockingSocket();
	int ReadHttpHeaderLine(char* pch, const int nSize, const int nSecs);
	int ReadHttpResponse(char* pch, const int nSize, const int nSecs);
private:
	char* m_pReadBuf; // read buffer
	int m_nReadBuf; // number of bytes in the read buffer
};

#endif // __BLOCKSOCK_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: blocksock.h $
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
 * User: Su           Date: 28.03.01   Time: 9:44
 * Updated in $/4Control/COM/softing/fc/inc/blocksock
 * Finalized documentation
 * 
 * *****************  Version 2  *****************
 * User: Su           Date: 19.03.01   Time: 19:42
 * Updated in $/4Control/COM/softing/fc/inc/blocksock
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

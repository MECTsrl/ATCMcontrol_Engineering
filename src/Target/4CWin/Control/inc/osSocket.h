/* $Header: /4CReleased/V2.20.00/Target/4CWin/CONTROL/inc/osSocket.h 1     28.02.07 19:10 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: osSocket.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/4CWin/CONTROL/inc/osSocket.h $
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

#ifndef _OSSOCKET_H_
#define _OSSOCKET_H_

/* Operating system dependent include files
 * ----------------------------------------------------------------------------
 */
#include <windows.h>
#include <winsock2.h>

#define VMS_TO_SELECT_SEC	1
#define VMS_TO_SELECT_USEC	0
		
#define VMS_TCP_TIMEOUT 	100

#define VMS_TCP_ERROR_DELAY 500


/* Socket Definitions
 * ----------------------------------------------------------------------------
 */
#define VMS_SOCKET			SOCKET
#define VMS_sockaddr_in 	struct sockaddr_in
#define VMS_timeval 		struct timeval
#define VMS_fd_set			fd_set

#define VMS_ERRNO			WSAGetLastError()	/* Socket error number		*/

#define VMS_INVALID_SOCKET	INVALID_SOCKET		/* Invalid socket notifier	*/

#define VMS_FD_ZERO(st) 	FD_ZERO(st)
#define VMS_FD_SET(fd,st)	FD_SET(fd,st)
#define VMS_FD_ISSET(fd,st) FD_ISSET(fd,st)

/* shutdown
 */
#define VMS_SD_RECEIVE		SD_RECEIVE
#define VMS_SD_SEND 		SD_SEND
#define VMS_SD_BOTH 		SD_BOTH

/* socket
 */
#define VMS_AF_INET 		AF_INET
#define VMS_SOCK_STREAM 	SOCK_STREAM
#define VMS_IPPROTO_TCP 	IPPROTO_TCP

/* setsockopt
 */
#define VMS_SOL_SOCKET		SOL_SOCKET
#define VMS_SO_DEBUG		SO_DEBUG
#define VMS_SO_REUSEADDR	SO_REUSEADDR
#define VMS_SO_RCVTIMEO 	SO_RCVTIMEO
#define VMS_SO_SNDTIMEO 	SO_SNDTIMEO

/* bind
 */
#define VMS_INADDR_ANY		INADDR_ANY

/* listen
 */
#define VMS_BACKLOG 		1

/* ioctl
 */
#define VMS_FIONBIO 		FIONBIO

/* select
 */
#define VMS_SELPAR_LISTEN	0


/* Return Values
 * ----------------------------------------------------------------------------
 */
#define VMS_SOCKET_ERROR	SOCKET_ERROR
#define VMS_RET_OK			0


/* Socket Error numbers
 * ----------------------------------------------------------------------------
 */
#define VMS_ETIMEDOUT		WSAETIMEDOUT
#define VMS_EWOULDBLOCK 	WSAEWOULDBLOCK


/* Socket OS API functions
 * ----------------------------------------------------------------------------
 */
#define osSocket(af,type,prot)				socket(af,type,prot)
#define osRecv(s,buf,len,flags) 			recv(s,buf,len,flags)
#define osSend(s,buf,len,flags) 			send(s,buf,len,flags)
#define osShutDown(s,how)					shutdown(s,how)
#define osCloseSocket(s)					closesocket(s)
#define osSetSockOpt(s,level,name,val,len)	setsockopt(s,level,name,val,len)
#define osBind(s,name,len)					bind(s,name,len)
#define osListen(s,backlog) 				listen(s,backlog)
#define osSelect(nfds,rf,wf,ef,to)			select(nfds,rf,wf,ef,to)
#define osAccept(s,addr,len)				accept(s,addr,len)
#define osIOCtl(s,cmd,argp) 				ioctlsocket(s,cmd,argp)
#define osConnect(s,name,len)				connect(s,name,len)
#define osInet_Addr(cp) 					inet_addr(cp)

#endif /* _OSSOCKET_H_ */

/* ---------------------------------------------------------------------------- */

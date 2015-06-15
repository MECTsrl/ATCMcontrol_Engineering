/* $Header: /4CReleased/V2.20.00/Target/4CWin/CONTROL/osShared/osMsg.c 1     28.02.07 19:10 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: osMsg.c $
 *						 $Logfile: /4CReleased/V2.20.00/Target/4CWin/CONTROL/osShared/osMsg.c $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				RunTime
 *
 * =COMPONENT			osShared
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

/* ----  Local Defines:   ----------------------------------------------------- */

#define __4CFILE__	"osMsg.c"

/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdInc.h"

#if defined(RTS_CFG_TCP_NATIVE)
  #include "osSocket.h"
#endif

/* ----  Target Specific Includes:	 ------------------------------------------ */

#include <windows.h>

/* ----  Local Defines:   ----------------------------------------------------- */

#define OS_INVALID_QUEUE	NULL
#define OS_HANDLE			HANDLE

/* ----  Global Variables:	 -------------------------------------------------- */

static OS_HANDLE g_hQueue[MAX_IPC_QUEUE];

#define WIN_MAX_MSG 	10

typedef struct
{
	IEC_UINT	uNext;					/* Next free queue entry			*/
	IEC_UINT	uLast;					/* Last free queue entry			*/

	SMessage	pMessage[WIN_MAX_MSG];	/* Queued messages					*/

} WMsgQueue;							/* -------------------------------- */

static WMsgQueue		g_pMsgQueue[MAX_IPC_QUEUE];
static CRITICAL_SECTION g_CS_Comm;


/* ----  Local Functions:	--------------------------------------------------- */

/* ----  Implementations:	--------------------------------------------------- */


/* ---------------------------------------------------------------------------- */
/**
 * osInitializeShared
 *
 * Do operating system dependent on time initializations here.
 *
 * @return			OK if successful else error number.
 */
IEC_UINT osInitializeShared(void)
{
	IEC_UINT uRes = OK; 
	IEC_UINT i;

	for (i = 0; i < MAX_IPC_QUEUE; i++)
	{
		g_hQueue[i] = OS_INVALID_QUEUE;
	}

	OS_MEMSET(g_pMsgQueue,	0x00, sizeof(WMsgQueue) 	* MAX_IPC_QUEUE);

	InitializeCriticalSection(&g_CS_Comm);

	RETURN(uRes);
}

/* ---------------------------------------------------------------------------- */
/**
 * osCreateIPCQueue
 *
 * Creates a message queue for the given queue ID.
 *
 * @return			OK if successful else ERR_ERROR
 */
IEC_UINT osCreateIPCQueue(IEC_UINT uQueue)
{
	IEC_UINT uRes = OK;

	if (uQueue >= MAX_IPC_QUEUE)
	{
		RETURN(ERR_INVALID_PARAM);
	}

	if (g_hQueue[uQueue] != OS_INVALID_QUEUE)
	{
		osDestroyIPCQueue(uQueue);
	}

	g_hQueue[uQueue] = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (g_hQueue[uQueue] == OS_INVALID_QUEUE)
	{
		RETURN_e(ERR_CREATE_QUEUE);
	}

	RETURN(uRes);
}

/* ---------------------------------------------------------------------------- */
/**
 * osDestroyIPCQueue
 *
 * Destroys a message queue for the given queue ID.
 *
 * @return			OK if successful else ERR_ERROR
 */
IEC_UINT osDestroyIPCQueue(IEC_UINT uQueue)
{
	IEC_UINT uRes = OK;

	if (uQueue >= MAX_IPC_QUEUE)
	{
		RETURN(ERR_INVALID_PARAM);
	}

	if (g_hQueue[uQueue] != OS_INVALID_QUEUE)
	{
		CloseHandle(g_hQueue[uQueue]);
		g_hQueue[uQueue] = OS_INVALID_QUEUE;
	}

	RETURN(uRes);
}

/* ---------------------------------------------------------------------------- */
/**
 * osRecvMessage
 *
 * Receive (wait) for a message on the given message queue.
 *
 * @return			OK if successful, WRN_TIME_OUT if time out 
 *					else ERR_ERROR
 */
#if defined(RTS_CFG_VM_IPC)

IEC_UINT osRecvMessage(SMessage *pMessage, IEC_UINT uQueue, IEC_UDINT ulTimeOut)
{
	IEC_UINT	uRes	= OK;

	DWORD dwTimeOut;
	DWORD dwResult;

	if (uQueue > MAX_IPC_QUEUE)
	{
		RETURN(ERR_INVALID_QUEUE);
	}

	dwTimeOut = ulTimeOut == VMM_WAIT_FOREVER ? INFINITE : ulTimeOut;
	dwResult  = WaitForMultipleObjects(1, g_hQueue + uQueue, FALSE, dwTimeOut);

	switch (dwResult)
	{
		case WAIT_TIMEOUT:
			return WRN_TIME_OUT;

		case WAIT_FAILED:
		{
			if (ulTimeOut == VMM_NO_WAIT)
			{
				return	WRN_NO_MESSAGE;
			}

			RETURN(ERR_IPC_RECV_FAILED);
		}

		case WAIT_OBJECT_0:
		{
			WMsgQueue *pQueue = g_pMsgQueue + uQueue;

			EnterCriticalSection(&g_CS_Comm);
			{
				SMessage *pMsgQueue = pQueue->pMessage + pQueue->uLast;

				if (pQueue->uLast == pQueue->uNext)
				{
					/* Ops! No message in queue.
					 */
					ResetEvent(g_hQueue[uQueue]);
					LeaveCriticalSection(&g_CS_Comm);

					RETURN(ERR_IPC_RECV_FAILED);
				}

				/* Get Message
				 */
				memcpy(pMessage, pMsgQueue, sizeof(SMessage) - MAX_MSG_DATA + pMsgQueue->uLen);

				pQueue->uLast = (IEC_UINT)((pQueue->uLast + 1) % WIN_MAX_MSG);

				if (pQueue->uLast == pQueue->uNext)
				{
					/* All messages consumed
					 */
					ResetEvent(g_hQueue[uQueue]);
				}
			
			} LeaveCriticalSection(&g_CS_Comm);

			RETURN(OK);
		}

	} /* switch (dwResult) */
	
	uRes = ERR_ERROR;
	RETURN(uRes);
}
#endif	/* RTS_CFG_VM_IPC */

/* ---------------------------------------------------------------------------- */
/**
 * osSendMessage
 *
 * Sends a message on the given message queue.
 *
 * @return			OK if successful, WRN_TIME_OUT if time out 
 *					else ERR_ERROR
 */
#if defined(RTS_CFG_VM_IPC)

IEC_UINT osSendMessage(SMessage *pMessage, IEC_UINT uQueue)
{
	IEC_UINT uRes = OK;

	WMsgQueue *pQueue = g_pMsgQueue + uQueue;

	if (uQueue > MAX_IPC_QUEUE)
	{
		RETURN(ERR_INVALID_QUEUE);
	}
	
	EnterCriticalSection(&g_CS_Comm);
	{
		SMessage *pMsgQueue = pQueue->pMessage + pQueue->uNext;

		ResetEvent(g_hQueue[uQueue]);

		if ((pQueue->uNext + 1) % WIN_MAX_MSG == pQueue->uLast)
		{
			/* Queue full, skip (overwrite) oldest entry
			 */
			pQueue->uLast = (IEC_UINT)((pQueue->uLast + 1) % WIN_MAX_MSG);
		}

		/* Queue message
		 */
		memcpy(pMsgQueue, pMessage, sizeof(SMessage) - MAX_MSG_DATA + pMessage->uLen);
		
		pQueue->uNext = (IEC_UINT)((pQueue->uNext + 1) % WIN_MAX_MSG);

		if (pMessage->uRespQueue != IPC_Q_NONE)
		{
			/* Clear the response queue
			 */
			g_pMsgQueue[pMessage->uRespQueue].uLast = g_pMsgQueue[pMessage->uRespQueue].uNext;

			ResetEvent(g_hQueue[pMessage->uRespQueue]);
		}

		SetEvent(g_hQueue[uQueue]);

	} LeaveCriticalSection(&g_CS_Comm);

	RETURN(uRes);
}
#endif	/* RTS_CFG_VM_IPC */

/* ---------------------------------------------------------------------------- */
/**
 * osGetMessageCount
 *
 * This function is called to determine the number of waiting messages in a
 * message queue associated to a VM task. (This number is used to realize
 * the watchdog functionality.)
 *
 * @return			OK if successful, else error number.
 */
#if defined(RTS_CFG_VM_IPC)

IEC_UINT osGetMessageCount(IEC_UINT uQueue, IEC_UINT *upMsgCount)
{
	IEC_UINT uRes = OK;

	WMsgQueue *pQueue = g_pMsgQueue + uQueue;

	if (uQueue > MAX_IPC_QUEUE)
	{
		RETURN(ERR_INVALID_QUEUE);
	}

	EnterCriticalSection(&g_CS_Comm);
	{
		if (pQueue->uNext >= pQueue->uLast)
		{
			*upMsgCount = (IEC_UINT)(pQueue->uNext - pQueue->uLast);
		}
		else
		{
			*upMsgCount = (IEC_UINT)(WIN_MAX_MSG - pQueue->uLast + pQueue->uNext);
		}

	} LeaveCriticalSection(&g_CS_Comm);

	RETURN(uRes);
}
#endif	/* RTS_CFG_VM_IPC */

/* ---------------------------------------------------------------------------- */

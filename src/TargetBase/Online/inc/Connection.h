/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/Connection.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: Connection.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/Connection.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlLibServ
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

#ifndef __CONNECTION_H__INC
#define __CONNECTION_H__INC

#define _ONLCOMM __declspec(dllimport)
#include "4CLayer.h"

#include "vmSharedDef.h"

void	SwapDouble(double *val, double *result);
void	SwapFloat(float *val, float *result);
__int64 SwapI64(__int64);
long	SwapLong(long);
short	SwapShort(short);

#define TRANSLATE_DOUBLE(x,y) SwapDouble(x,y)
#define TRANSLATE_FLOAT(x,y)  SwapFloat(x,y)
#define TRANSLATE_I64(x)	  SwapI64(x)
#define TRANSLATE_LONG(x)	  SwapLong(x)
#define TRANSLATE_SHORT(x)	  SwapShort(x)


/* class CCommObj
 * ----------------------------------------------------------------------------
 */
class CCommObj
{

	public:
		CCommObj();
		~CCommObj();

	protected:
		CCriticalSection	m_CritSec;
		C4CLayer			*m_pLayer;

		LPCONSTRUCT4CLAYER	m_fpConstruct4CLayer;
		LPDISPOSE4CLAYER	m_fpDispose4CLayer;

		HMODULE 	m_hModule;

		CRegKey 	m_RegKey;
		TCHAR		m_szCommParam[256];

	// Initialize / Finalize
	public:
		BOOL Construct(void);
		BOOL Dispose(void);
		BOOL CheckCommParams(void);

		BOOL OpenComm(void);
		BOOL CloseComm(void);

		BOOL Connect(void);
		BOOL Disconnect(void);
		
	protected:
		BOOL ReadAdaptParameter(void);

	// Communication
	public:
		BOOL TransferNT(BYTE *bypCmd, BYTE **ppData, UINT *upLen);
		BOOL TransferNT(BYTE *bypCmd, LPCTSTR cszFile, BYTE **ppData, UINT *upLen);

	// Member Access
	public:
		DWORD GetFirmware(void);

	// Helper
	public:
		void SendMsgToEng(BSTR bMsg);
		void SendMsgToEng(LPCTSTR szMsg);
		void SendNLToEng();
		void SendErrToEng(UINT uErrNo);

		BOOL Adapt_SendErrToEng(UINT uErrNo);

        BOOL GetMsgString(BYTE byCommand, BYTE *pData, UINT uLen, BSTR *pMessage);

	// Danger, experts only!
	public:
		C4CLayer *Get4CLayer(void);
};


/* class CConnection
 * ----------------------------------------------------------------------------
 */
class CConnection
{
	public:
		CConnection();
		~CConnection();

	protected:
		static DWORD		m_dwFirmware;
		
	protected:
		CCommObj			m_CommObj;
		CCriticalSection	m_CritSec;

	private:
		// Must always be accessed within m_CritSec!
		BOOL	m_bConnected;
		BOOL	m_bLogon;
		BOOL	m_bShutdown;

	protected:
		BOOL	m_bConfigChanged;
		HANDLE	m_hThread;
		DWORD	m_ThreadID;
		GUID	m_GUID;
		DWORD	m_resourceState;
		BOOL	m_bDebugSession;
		BOOL	m_bRequestBPSync;


	// Communication
	public:
		HRESULT SendCommand (BOOL bLogon, BYTE byCmd,   BYTE *pData, UINT uLen);
		HRESULT SendCommand (BOOL bLogon, BYTE byCmd,   LPCTSTR cszFile);
		HRESULT TransferData(BOOL bLogon, BYTE byCmd,   BYTE **ppData, UINT *upLen);
		HRESULT TransferData(             BYTE byCmd,   BYTE **ppData, UINT *upLen);
		HRESULT SendDirect  (			  BYTE *bypCmd, BYTE **ppData, UINT *upLen);

	private:
		BOOL	TransferNT(BYTE *bypCmd, BYTE **ppData, UINT *upLen);
		BOOL	TransferNT(BYTE *bypCmd, LPCTSTR cszFile, BYTE **ppData, UINT *upLen);
		HRESULT IsSafeNT(BOOL bLogon);

	// Common
	public:
		HRESULT CheckMessages(void);
		HRESULT CheckVersion(void);

		BOOL	IsConnected(void);
		BOOL	IsLogon(void);

		void	Disconnect(BOOL bConfigChanged);

		void	Lock(void);
		void	Unlock(void);

		void	SetGUID(const GUID &guid);
		BOOL	SetDebugSession(BOOL bON);

		DWORD	GetResourceState(void);
		void	RequestBPSync(BOOL bRequestBPSync);

		void	TraceCommError(UINT uCommand, LPBYTE lpData, UINT cbData);
		void	TraceComm(LPCTSTR lpMsg);

		static	DWORD GetFirmware(void);
		
		BOOL	StartConnectThread(void);
		BOOL	StopConnectThread(void);

	// Internal
	protected:
		BOOL	CheckCommParams(void);

	protected:
		void HandleMessages(UINT nMessages, LPSTR lpstrMsg, UINT cbMsg);
		void HandleBreakpoints(UINT nBreakpoints, XBPNotification *pNotifications);
		void InitGUID(void);

	private:
		static DWORD WINAPI ThreadProc(LPVOID lpParam);
		VOID ThreadMain();

	// Danger, experts only!
	public:
		C4CLayer *Get4CLayer(void);
};


extern CConnection myConnection;


#endif

/* ---------------------------------------------------------------------------- */

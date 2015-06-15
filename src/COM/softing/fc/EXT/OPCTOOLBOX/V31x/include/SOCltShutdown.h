//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SOClt                              |
//                                                                            |
//  Filename    : SOCltShutdown.h                                             |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : COM Callback object classes                                 |
//                - SOCltShutdown: callback for shutdown requests             |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SOCLTSHUTDOWN_H_
#define _SOCLTSHUTDOWN_H_

#pragma pack(push,4)

//-----------------------------------------------------------------------------
// SOCltShutdown                                                              |
//-----------------------------------------------------------------------------

class SOCltServer;

class SOCLT_EXPORT SOCltShutdown : public IOPCShutdown
{
public:
	SOCltShutdown(SOCltServer *server);
	virtual ~SOCltShutdown(void);

	// IUnknown interface
	STDMETHODIMP QueryInterface(IN REFIID iid, OUT LPVOID* ppInterface);
	STDMETHODIMP_(ULONG) AddRef (void);
	STDMETHODIMP_(ULONG) Release(void);

	// IOPCShutdown interface
	STDMETHODIMP ShutdownRequest(IN LPCWSTR reason);

	HRESULT advise(void);		// advise shutdown event sink
	HRESULT unadvise(void);		// unadvise shutdown event sink

protected:
	LONG m_refCount;		// object reference count
	SOCltServer *m_server;	// server object
	DWORD m_cookie;			// advise token
	BOOL m_established;		// connection established ?

	// object trace id
	LPCTSTR getObjTraceId(void);
	virtual void onShutdown(IN LPCTSTR reason);
}; // SOCltShutdown

#define OPCShutdown SOCltShutdown

#pragma pack(pop)
#endif 

//-----------------------------------------------------------------------------
//                                 OPC SESSION                                |
//                                                                            |
//  Filename    : OPCADVISESINK.H                                             |
//  Version     : 2.00.0.00.release                                           |
//  Date        : 29-March-1999                                               |
//                                                                            |
//  Description : Event handler classes                                       |
//                - OPCAdviseSink: OPC V1.0 sink for asynchronous advises     |
//                - OPCDataCallback: OPC V2.0 sink for asynchronous advises   |
//                                                                            |
//  CHANGE_NOTES                                                              |
//                                                                            |
//  date      name      change                                                |
//  ------------------------------------------------------------------------- |
//  12/29/98  RT        added OPCDataCallback class                           |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _OPCADVISESINK_H_
#define _OPCADVISESINK_H_

#include <opcda.h>

#pragma warning( disable : 4275 ) 

//-----------------------------------------------------------------------------
// OPCAdviseSink                                                              |
//-----------------------------------------------------------------------------

class ClientGroup;

class OPCENGINE_EXPORT OPCAdviseSink : public IAdviseSink
{
public:
	OPCAdviseSink(IN ClientGroup *group);
	~OPCAdviseSink(void);

	// IUnknown interface
	STDMETHODIMP QueryInterface(IN REFIID iid, OUT LPVOID* ppInterface);
	STDMETHODIMP_(ULONG) AddRef (void);
	STDMETHODIMP_(ULONG) Release(void);

	// IAdviseSink interface
	STDMETHODIMP_(void)	OnDataChange(IN LPFORMATETC pFmt, IN LPSTGMEDIUM pStg);
	STDMETHODIMP_(void)	OnViewChange(IN DWORD dwA, IN LONG lindex);
	STDMETHODIMP_(void)	OnRename(IN IMoniker* moniker);
	STDMETHODIMP_(void)	OnSave (void);
	STDMETHODIMP_(void)	OnClose(void);

	// get connection identifiers
	DWORD getReadConnection(void) const;
	DWORD getWriteConnection(void) const;

	// advise
	HRESULT advise(void);

	// unadvise
	HRESULT unadvise(void);

protected:
	DWORD m_readConnection;		// id of read connection
	DWORD m_writeConnection;	// id of write connection

	// object trace id
	LPCTSTR getObjTraceId(void);

	// handle OnDataChange advise
	void handleAdvise(IN CLIPFORMAT format, IN HGLOBAL memory);

	ULONG m_refCount;		// object reference count
	ClientGroup *m_group;	// group object
}; // OPCAdviseSink

inline DWORD OPCAdviseSink::getReadConnection(void) const
{ return m_readConnection; }

inline DWORD OPCAdviseSink::getWriteConnection(void) const
{ return m_writeConnection; }



//-----------------------------------------------------------------------------
// OPCDataCallback                                                            |
//-----------------------------------------------------------------------------

class OPCENGINE_EXPORT OPCDataCallback : public IOPCDataCallback
{
public:
	OPCDataCallback(IN ClientGroup *group);
	~OPCDataCallback(void);

	// IUnknown interface
	STDMETHODIMP QueryInterface(IN REFIID iid, OUT LPVOID* ppInterface);
	STDMETHODIMP_(ULONG) AddRef (void);
	STDMETHODIMP_(ULONG) Release(void);

	// IOPCDataCallback interface
	STDMETHODIMP OnDataChange(IN DWORD dwTransid, IN OPCHANDLE hGroup,
            IN HRESULT hrMasterquality, IN HRESULT hrMastererror,
            IN DWORD dwCount, IN OPCHANDLE *phClientItems,
            IN VARIANT *pvValues, IN WORD *pwQualities, IN FILETIME *pftTimeStamps,
            IN HRESULT *pErrors);

	STDMETHODIMP OnReadComplete(IN DWORD dwTransid, IN OPCHANDLE hGroup,
            IN HRESULT hrMasterquality, IN HRESULT hrMastererror,
            IN DWORD dwCount, IN OPCHANDLE *phClientItems,
            IN VARIANT *pvValues, IN WORD *pwQualities, IN FILETIME *pftTimeStamps,
            IN HRESULT *pErrors);

	STDMETHODIMP OnWriteComplete(IN DWORD dwTransid,IN OPCHANDLE hGroup,
            IN HRESULT hrMastererr, IN DWORD dwCount, 
			IN OPCHANDLE *pClienthandles, IN HRESULT *pErrors);
        
	STDMETHODIMP OnCancelComplete(IN DWORD dwTransid, IN OPCHANDLE hGroup);

	// get connection identifier
	DWORD getConnection(void) const;

	// advise connection to server
	HRESULT advise(void);
	
	// unadvise connection to server
	HRESULT unadvise(void);

protected:
	DWORD m_connection;		// id of read, write and cancel connection (OPC V2.0)

	HRESULT handleReadAdvise(IN DWORD dwTransid, IN OPCHANDLE hGroup,
            IN HRESULT hrMasterquality, IN HRESULT hrMastererror,
            IN DWORD dwCount, IN OPCHANDLE *phClientItems,
            IN VARIANT *pvValues, IN WORD *pwQualities, IN FILETIME *pftTimeStamps,
            IN HRESULT *pErrors);

	// object trace id
	LPCTSTR getObjTraceId(void);

	ULONG m_refCount;		// object reference count
	ClientGroup *m_group;	// group object
}; // OPCDataCallback

inline DWORD OPCDataCallback::getConnection(void) const
{ return m_connection; }

#endif

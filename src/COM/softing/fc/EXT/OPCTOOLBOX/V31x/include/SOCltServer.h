//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SOClt                              |
//                                                                            |
//  Filename    : SOCltServer.h                                               |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : Server object                                               |
//                - SOCltServer: server object class                          |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SOCLTSERVER_H_
#define _SOCLTSERVER_H_

#include "SOCltElement.h"
#include "SOCltThread.h"

#pragma pack(push,4)

// server update parameters

#define SOCLT_SRVPAR_CLIENT_NAME	        0x00000001
#define SOCLT_SRVPAR_LCID			            0x00000002
#define SOCLT_SRVPAR_ALL			            0xFFFFFFFF

//-----------------------------------------------------------------------------
// SOCltServer                                                                |
//-----------------------------------------------------------------------------

class SOCltShutdown;

class SOCLT_EXPORT SOCltServer : public SOCltElement
{
public:
	SOCltServer(IN OPTIONAL DWORD objType = 0);  

	// the server requests to shutdown the connection
	virtual void shutdown(IN LPCTSTR reason);	

	// name of a remote maschine
	virtual BOOL setNodeName(IN LPCTSTR nodeName);
	virtual SOCmnString getNodeName(void) const;

	// CLSID of this Server
	virtual BOOL setClassID(IN REFCLSID clsid);
	virtual CLSID getClassID(void) const;

	// Flags for valid parameter
	virtual DWORD getValidParameters(void) const;
	virtual void resetValidParameters(void);

	// Execution context of Server
	virtual BOOL setClsctx(IN DWORD clsctx);
	virtual DWORD getClsctx(void) const;

	// server name
	virtual BOOL setServerName(IN LPCTSTR serverName);
	virtual SOCmnString getServerName(void) const;

	// local Id
	virtual BOOL setLCID(IN DWORD lcid);
	virtual DWORD getLCID(void) const;

	// server name
	virtual BOOL setClientName(IN LPCTSTR serverName);
	virtual SOCmnString getClientName(void) const;
	
	// IOPCCommon interface
	virtual HRESULT getErrorString(IN HRESULT error, OUT SOCmnString *string);
	virtual HRESULT queryAvailableLCIDs(SOCmnDWORDList& list);

	// overrides
	virtual BOOL setParentBranch(IN SOCmnElement *parent);
	virtual SOCltWorkerThread* getWorkerThread(void);

protected:
	CLSID m_clsid;					    // class ID of the OPC Server
	SOCmnString m_nodeName;			// computer name the OPC Server object should run on
	DWORD m_clsctx;					    // execution context of the OPC server 
	SOCmnString m_clientName;		// client name in the OPC Server
	SOCmnString m_serverName;		// logical server name of the proxy
	LCID m_lcid;					      // default locale id 
	DWORD m_validParameter;			// valid parameters

	SOCltWorkerThread m_workThread;
  DWORD m_cookieConnectMonitor;

	CComPtr<IMalloc> m_malloc;													// IMalloc interface

	SOCltInterfaceAccess<IUnknown,&IID_IUnknown> m_unknown;						// IUnknown interface of server object
	SOCltInterfaceAccess<IOPCCommon, &_ATL_IIDOF(IOPCCommon)> m_common;				  // IOPCCommon interface of server object
	
	SOCltShutdown *m_shutdown;	// shutdown event sink

protected:
	virtual ~SOCltServer(void);

	// callbacks
	virtual void onSetLCID(void);
	virtual void onSetClientName(void);
	virtual void onSetServerName(void);

	// shutdown callback
	virtual void onShutdown(void);

	virtual SOCmnObject *getObjectCmnElementIList(void);
}; // SOCltServer


inline BOOL SOCltServer::setNodeName(IN LPCTSTR nodeName)
{ m_nodeName = nodeName; return TRUE; }

inline SOCmnString SOCltServer::getNodeName(void) const
{ return m_nodeName; }

inline BOOL SOCltServer::setClassID(IN REFCLSID clsid)
{ m_clsid = clsid; return TRUE; }

inline CLSID SOCltServer::getClassID(void) const
{ return m_clsid; }

inline BOOL SOCltServer::setClsctx(IN DWORD clsctx)
{ m_clsctx = clsctx; return TRUE; }

inline DWORD SOCltServer::getClsctx(void) const
{ return m_clsctx; }

inline DWORD SOCltServer::getLCID(void) const
{ return m_lcid; }

inline SOCmnString SOCltServer::getClientName(void) const
{ return m_clientName; }

inline SOCmnString SOCltServer::getServerName(void) const
{ return m_serverName; }

inline DWORD SOCltServer::getValidParameters(void) const
{ return m_validParameter; }

inline void SOCltServer::resetValidParameters(void) 
{ m_validParameter = 0; }

#pragma pack(pop)
#endif 

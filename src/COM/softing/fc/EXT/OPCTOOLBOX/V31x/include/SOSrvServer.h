//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SOSrv                              |
//                                                                            |
//  Filename    : SOSrvServer.h                                               |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : OPC Server object related classes                           |
//                - SOSrvServer: OPC server object                            |
//                - SOSrvObjectRoot: root object of OPC runtime object tree   |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SOSRVSERVER_H_
#define _SOSRVSERVER_H_

#pragma pack(push,4)

#define SOSRVSERVER_SPECID_DA 0x01FF0000
#define SOSRVSERVER_SPECID_AE 0x02FF0000

//-----------------------------------------------------------------------------
// SOSrvServer                                                                |
//-----------------------------------------------------------------------------

class SOSRV_EXPORT SOSrvServer : 
	public SOCmnElement
{
public:
	SOSrvServer(IN DWORD objType, IN OPTIONAL BYTE serverInstance = 0);  

	// special release method
	virtual LONG release(void);	

	// locale Id
	DWORD getLCID(void) const;

	// watch overload 
	virtual BOOL setParentBranch(IN SOCmnElement *parent);

	// client name
	SOCmnString getClientName(void) const;

	// IOPCCommon methods
	virtual void queryAvailableLCIDs(OUT SOCmnDWORDList &lcidList);

	HRESULT getErrorString(IN HRESULT error, OUT SOCmnString &string);
	virtual HRESULT getErrorString(IN HRESULT error, IN DWORD lcid, 
			OUT SOCmnString &string);

	// start the sending of the shutdown request to the OPC client
	virtual HRESULT startShutdownRequest(IN LPCTSTR reason, 
			IN OPTIONAL HANDLE event = INVALID_HANDLE_VALUE) = 0;
	// send a shutdown request to the OPC client
	virtual HRESULT sendShutdownRequest(IN LPCTSTR reason) = 0;

	// check if connection to OPC client is OK
	virtual BOOL checkClientConnection(void) = 0;
	// disconnect the OPC client belonging to the server object -> deletes the server object
	virtual void disconnectClient(void) = 0;

    // standard string filter implementations
	static BOOL stdMatchStringFilter(IN LPCTSTR string, IN LPCTSTR filter);
	static BOOL opcMatchStringFilter(IN LPCTSTR string, IN LPCTSTR filter, IN BOOL caseSensitive);
	virtual BOOL includesFilterChars(IN LPCTSTR string);

	// signal work event of working thread
	BOOL signalWorkEvent(void) const;
	HANDLE getWorkEvent(void) const;

	// locale Id
	virtual BOOL supportLCID(IN DWORD lcid);

	// working thread main routine
	DWORD workThread(void);

	// server instance
	BYTE getServerInstance(void) { return m_serverInstance; }

	virtual void getCLSID(OUT CLSID &classID) = 0; 

	// locale Id
	virtual BOOL setLCID(IN DWORD lcid);

	// client name
	virtual BOOL setClientName(IN LPCTSTR clientName);

	// server watch object
	virtual SOSrvWatch *getWatchObject(void);

protected:
	virtual ~SOSrvServer();

	void *m_me;                             // me pointer (to COM object)
	SOCmnSync m_meSync;                     // synchronisation of me pointer
	DWORD m_lcid;                           // local Id
	SOCmnString m_clientName;               // client name
	HANDLE m_events[2];                     // events of working thread
	HANDLE m_workThread;                    // thread handle of working thread
	DWORD m_workThreadId;                   // id of working thread
	DWORD m_minRequestLoopTime;             // minimal loop difference of the working thread
	SOCmnPointer<SOSrvWatch> m_watchObject; // watch object of the server object
	BYTE m_serverInstance;                  // server instance number

	// create working thread
	HANDLE createWorkThread(void);

	// what type of server object
	virtual DWORD getSpecID(void) = 0;

	// working thread helper
	virtual DWORD doWork(IN DWORD startTime) = 0;

	// callbacks
	virtual void onSetLCID(IN DWORD lcid);
	virtual void onSetClientName(IN LPCTSTR clientName);
}; // SOSrvServer


inline DWORD SOSrvServer::getLCID(void) const
{ return m_lcid; }

inline SOCmnString SOSrvServer::getClientName(void) const
{ return m_clientName; }

inline BOOL SOSrvServer::signalWorkEvent(void) const
{ return ::SetEvent(m_events[SOCMN_EVENT_WORK]); }

inline HANDLE SOSrvServer::getWorkEvent(void) const
{ return m_events[SOCMN_EVENT_WORK]; }

inline HRESULT SOSrvServer::getErrorString(IN HRESULT error, OUT SOCmnString &string)
{ return getErrorString(error, m_lcid, string); }




//-----------------------------------------------------------------------------
// SOSrvObjectRoot                                                            |
//-----------------------------------------------------------------------------

class SOSrvEntry;

class SOSRV_EXPORT SOSrvObjectRoot : 
	public SOCmnElement,
	virtual public SOCmnElementListBranch
{
public:
	SOSrvObjectRoot(); 

	virtual BOOL addLeaf(IN SOCmnElement *newLeaf);
	virtual BOOL addBranch(IN SOCmnElement *newBranch);

	virtual SOCmnObject *getObjectCmnElementIList(void);

protected:
	void addDCOMCnfgWatchData(IN SOSrvEntry *entry, IN MSXML::IXMLDOMDocumentPtr pDoc, IN MSXML::IXMLDOMNodePtr pAttrsNode);
}; // SOSrvObjectRoot

#pragma pack(pop)
#endif

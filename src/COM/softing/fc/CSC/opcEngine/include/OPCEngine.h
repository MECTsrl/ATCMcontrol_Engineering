//-----------------------------------------------------------------------------
//                                  OPC ENGINE                                |
//                                                                            |
//  Filename    : OPCENGINE.H                                                 |
//  Version     : 2.01.0.00.release                                           |
//  Date        : 7-May-1999                                                  |
//                                                                            |
//  Description : Entry point for Server Toolkit                              |
//				  - OPCEngine: OPC engine object                              |
//                - GenericCreator: Object factory for toolkit objects        |
//				  - GenericEventHandler: Handler for toolkit events           |
//                - GenericConfigFileHandler: Configuration file access       |

//                                                                            |
//  CHANGE_NOTES                                                              |
//                                                                            |
//  date      name      change                                                |
//  ------------------------------------------------------------------------- |
//  12/21/98  RT        changed tag parameters of creator methods to item tag |
//  12/22/98  RT        added createTag and createTagProperty to creator      |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _OPCENGINE_H_
#define _OPCENGINE_H_

#include <opcda.h>
#include "GenObj.h"
#include "GenReq.h"

//-----------------------------------------------------------------------------
// Trace Groups                                                               |
//-----------------------------------------------------------------------------

#define OPCENGINE_TRACE_ALL				0xFFFFFFFF
#define OPCENGINE_TRACE_NOTHING			0x00000000

#define OPCENGINE_TRACE_REF_COUNTS		0x01000001
#define OPCENGINE_TRACE_IO				0x00003F00
#define OPCENGINE_TRACE_LOG_OBJECTS		0x30000000

#define OPCENGINE_TRACE_GENERIC			0x00000001
#define OPCENGINE_TRACE_TREE			0x00000002
#define OPCENGINE_TRACE_ENGINE			0x00010000
#define OPCENGINE_TRACE_REQUEST			0x00002000
#define OPCENGINE_TRACE_CACHE			0x00001000
#define OPCENGINE_TRACE_NAMESPACE		0x00000010
#define OPCENGINE_TRACE_SERVER			0x00000100
#define OPCENGINE_TRACE_GROUP			0x00000200
#define OPCENGINE_TRACE_ITEM			0x00000400
#define OPCENGINE_TRACE_TRANSACTION		0x00000800
#define OPCENGINE_TRACE_COM				0x01000000
#define OPCENGINE_TRACE_OTHER			0x08000000
#define OPCENGINE_TRACE_LOG_GENOBJ		0x10000000
#define OPCENGINE_TRACE_LOG_COMOBJ		0x20000000


//-----------------------------------------------------------------------------
// Error and Warning Codes                                                    |
//-----------------------------------------------------------------------------

// out of memory
#define OPCENGINE_ERR_MEMORY			0xE0800001			
// COM error
#define OPCENGINE_ERR_COM				0xE0800002
// exception
#define OPCENGINE_ERR_EXCEPTION			0xE0800003
// assert
#define OPCENGINE_ERR_ASSERT			0xE0800004

// invalid COM interface parameter
#define OPCENGINE_WRN_IF_PRM			0xA0800001		
// private function called on public group		
#define OPCENGINE_WRN_GRP_PUBLIC		0xA0800002
// connection parameter of COM interface function is not valid
#define OPCENGINE_WRN_BAD_CONNECTION	0xA0800003
// problems starting or stoping threads
#define OPCENGINE_WRN_THREAD			0xA0800004
// disconnected a not responding client 
#define OPCENGINE_WRN_DISCONNECT_CLIENT	0xA0800005


//-----------------------------------------------------------------------------
// GenericCreator                                                             |
//-----------------------------------------------------------------------------

class GenericServer;
class GenericGroup;
class GenericItem;
class GenericCache;
class GenericItemTag;
class GenericTag;
class GenericTagProperty;

class OPCENGINE_EXPORT GenericCreator : public GenericObject
{
public:
	GenericCreator();
	virtual GenericServer *createServer(IN BOOL publicGroups);
	virtual GenericGroup *createGroup(IN GenericServer *parent);
	virtual GenericItem *createItem(IN GenericGroup *parent, IN GenericItemTag *tag);
	virtual GenericRequest *createRequest(IN GenericServer *server, IN GenericItem *item, 
			IN GenericItemTag *tag, IN BYTE operation, IN BYTE type, IN BYTE deviceIOMode);
	virtual GenericCache *createCache(IN GenericItemTag *tag);
	virtual GenericTag *createTag(IN GenericServer *server, IN LPCTSTR itemID);
	virtual GenericTagProperty *createTagProperty(IN GenericTag *tag, IN DWORD propertyId);

	void lock(void);
	void unlock(void);
protected:
	CCriticalSection m_lock;
}; // GenericCreator

inline void GenericCreator::lock(void)
{ m_lock.Lock(); }
inline void GenericCreator::unlock(void)
{ m_lock.Unlock(); }




//-----------------------------------------------------------------------------
// GenericEventHandler                                                        |
//-----------------------------------------------------------------------------

class OPCENGINE_EXPORT GenericEventHandler : public GenericObject
{
public:
	GenericEventHandler();
	virtual void onError(IN DWORD id, IN LPCTSTR description);
	virtual void onWarning(IN DWORD id, IN LPCTSTR description);
	virtual void onReleasedLastOPCObject(void);
};


//-----------------------------------------------------------------------------
// GenericConfigFileHandler                                                   |
//-----------------------------------------------------------------------------

struct OPCENGINE_EXPORT GenericConfigFileHandler : public GenericObject
{
public:
	GenericConfigFileHandler();
	virtual BOOL isDirty(IN GenericServer *server) = 0;
	virtual HRESULT load(IN GenericServer *server, IN LPCTSTR name, IN DWORD mode) = 0;
	virtual HRESULT save(IN GenericServer *server, IN LPCTSTR name, IN BOOL remember) = 0;
	virtual HRESULT getCurFile(IN GenericServer *server, OUT CString &name) = 0;
}; // GenericConfigFileHandler




//-----------------------------------------------------------------------------
// OPCStatistics                                                              |
//-----------------------------------------------------------------------------

// Statistics data of server toolkit          
struct OPCStatistics
{
	//-- REQUESTS --
	struct _requests
	{
		// completed I/O requests
		ULONG asyncCacheRead;
		ULONG asyncDeviceRead;
		ULONG asyncWrite;
		ULONG syncCacheRead;
		ULONG syncDeviceRead;
		ULONG syncWrite;
		ULONG cyclicRead;
		ULONG reportRead;

		// failed I/O requests
		ULONG asyncCacheReadBad;
		ULONG asyncDeviceReadBad;
		ULONG asyncWriteBad;
		ULONG syncCacheReadBad;
		ULONG syncDeviceReadBad;
		ULONG syncWriteBad;
		ULONG cyclicReadBad;
		ULONG reportReadBad;
		
		// times in milliseconds:
		// complete time in device operation 
		ULONG timeRead;
		ULONG timeWrite;
		// average time in device operation
		ULONG averageTimeRead;
		ULONG averageTimeWrite;
		// maximum time in device operation
		ULONG maxTimeRead;
		ULONG maxTimeWrite;
	} req;

	//-- OPC OBJECTS --
	struct _objects
	{
		ULONG serverObjects;
		ULONG groupObjects;
		ULONG publicGroupObjects;
		ULONG publicGroupTemplates;
		ULONG itemObjects;
	} obj;

	//-- STATISTICS --
	ULONG allCount;
	DWORD startTime;
}; // OPCStatistics




//-----------------------------------------------------------------------------
// OPCShutdownRequest                                                         |
//-----------------------------------------------------------------------------

struct OPCShutdownRequest
{
	GenericServer *server;
	LPCTSTR reason;
	HANDLE event;
}; // OPCShutdownRequest

typedef CTypedPtrList<CPtrList, OPCShutdownRequest *> ShutdownRequestList;




//-----------------------------------------------------------------------------
// OPCEngine                                                                  |
//-----------------------------------------------------------------------------

class OPCRootNS;
class OPCRootOPC;
class OPCTrace;

class OPCENGINE_EXPORT OPCEngine : public GenericObject
{
friend OPCTrace;
friend GenericRequest;
friend GenericServer;

public:
	OPCEngine(void);

	enum serverTypeCOM
	{ 
		InProc,		// COM in process server (DLL)
		OutProc		// COM out of process server (EXE)
	};

	// initialize engine
	BOOL initialize(IN enum serverTypeCOM type, IN REFCLSID clsid, 
			IN UINT rgsResource, IN HINSTANCE hInst, 
			OPTIONAL IN DWORD coInit = COINIT_APARTMENTTHREADED);
	BOOL terminate(void);
	BOOL start(void);
	BOOL stop(void);

	// namespace root
	OPCRootNS *getNamespaceRoot(void);

	// root of OPC object tree
	OPCRootOPC *getOPCRoot(void);

	// get the public groups server object
	GenericServer *getPublicGroups(void);

	// object factory for OPC objects
	void setCreator(IN GenericCreator *creator);
	GenericCreator *getCreator(void);

	// handler object for configuration file calls (IPersistFile)
	void setConfigurationFileHandler(IN GenericConfigFileHandler *cfgHandler);
	GenericConfigFileHandler *getConfigurationFileHandler(void);

	// engine event handler
	void setEventHandler(IN GenericEventHandler *cbHandler);
	GenericEventHandler *getEventHandler(void);

	// COM helper methods
	HRESULT getClassObject(IN REFCLSID rclsid, IN REFIID riid, 
			OUT LPVOID *ppvObj);
	HRESULT canUnloadNow(void);
	HRESULT registerServer(void);
	HRESULT unregisterServer(void);
	HRESULT revokeClassObjects(void);

	// security 
	HRESULT inititalizeDCOMSecurity(IN DWORD authenticationLevel, 
			IN DWORD impersonationLevel, IN CStringList &accessPermit, 
			IN CStringList &accessDeny);
	BOOL enableRequestUserNames(IN BOOL enable);

	// tracing (for compatiblity do this over trace object !)
	void setTraceLevel(IN DWORD error, IN DWORD warning, IN DWORD info, 
			IN DWORD debug);
	BOOL setTraceFile(IN LPCTSTR file, 
			IN OPTIONAL LPCTSTR secondFile = NULL, 
			IN OPTIONAL DWORD maxFileSize = 1000000,
			IN OPTIONAL DWORD creationDisposition = CREATE_ALWAYS);

	// statistics
	BOOL getStatistics(OUT OPCStatistics *statAll, 
			OUT OPCStatistics *statSinceLastCall, OUT DWORD *msSinceLastCall);

	// tree hierarchy level delimiter and not allowed chars
	void setTreeDelimiter(IN TCHAR create, IN LPCTSTR accept, 
			IN LPCTSTR bad);
	TCHAR getTreeDelimiter(OUT LPCTSTR *accept, OUT LPCTSTR *bad);

	// delimiter between tag path and property id (is a not allowed char of tree)
	void setPropertyDelimiter(IN TCHAR propDel);
	TCHAR getPropertyDelimiter(void) const;

	// vendor string
	BOOL setVendorInfo(IN LPCTSTR vendorInfo);
	CString getVendorInfo(void) const;

	// version info
	BOOL setVersionInfo(IN WORD majorVersion, IN WORD minorVersion, 
			IN WORD buildNumber);
	void getVersionInfo(OUT WORD *majorVersion, OUT WORD *minorVersion, 
			OUT WORD *buildNumber) const;

	// OPC server state
	BOOL setServerState(IN OPCSERVERSTATE serverState);
	OPCSERVERSTATE getServerState(void) const;

	// minimal update rate 
	void setMinUpdateRate(DWORD minUpdateRate);
	DWORD getMinUpdateRate(void);

	// start time of OPC engine
	BOOL getStartTime(OUT LPFILETIME startTime) const;

	// special release method
	virtual LONG release(void);	

	// parameter for the check routines of the engine
	void setCheckParameter(IN DWORD clientInterval);

	// send shutdown request to all connected clients
	void shutdownAllClients(IN LPCTSTR reason);

	// event pool
	HANDLE popPoolEvent(void);
	void pushPoolEvent(HANDLE event);
	void freeAllPoolEvents(void);

	DWORD mainOPCThread(void);

protected:
	virtual ~OPCEngine(void);

	// signal error or warning
	void error(IN DWORD id, IN LPCTSTR description, IN BOOL error);

	// add shutdown request to shutdown queue
	BOOL addShutdownRequest(IN GenericServer *server, IN LPCTSTR reason, 
			IN HANDLE event);

	// current statistics values
	OPCStatistics *getStatisticsBuffer(void);
	void releaseStatisticsBuffer(void);

private:
	CString m_vendorInfo;			// vendor string
	WORD m_majorVersion;			// version info
	WORD m_minorVersion; 
	WORD m_buildNumber; 
	OPCSERVERSTATE m_serverState;	// OPC server state
	FILETIME m_startTime;			// start time of engine

	TCHAR m_delimNScreate;			// creation namespace delimiter
	CString m_delimNSaccept;		// accepted namespace delimiter
	CString m_badNSchars;			// bad namespace characters
	TCHAR m_delimProp;				// tag property delimiter

	OPCRootNS *m_nsRoot;			// namespace root
	OPCRootOPC *m_opcRoot;			// OPC object root
	GenericEventHandler *m_evtHandler;	
									// event handler
	GenericConfigFileHandler *m_cfgHandler;
									// config handler
	GenericCreator *m_creator;		// creator

	BOOL m_terminateLock;			// termination lock
	BOOL m_initialized;				// initialized engine?
	DWORD m_coInit;

	HANDLE m_hOPCThread;			// handle of OPC main thread
	HANDLE m_events[2];				// communications events

	OPCStatistics m_curStat;		// current statistic values
	OPCStatistics m_allStat;		// all statistic values since engine start
	DWORD m_lastStatTime;			// time of last statistics fetch
	CCriticalSection m_curStatLock;	// lock for current statistic values

	DWORD m_clientCheckInterval;	// time intervall in ms for check functions
	serverTypeCOM m_type;			// COM server type
	DWORD m_minUpdateRate;			// minimal update rate (maximal update rate is 40 days)

	ShutdownRequestList m_shutdownList;
									// shutdown request list
	CPtrList m_eventPool;			// event pool
	CCriticalSection m_eventPoolLock;	// event pool lock
}; // OPCEngine


inline void OPCEngine::setTreeDelimiter(IN TCHAR create, 
	IN LPCTSTR accept, IN LPCTSTR bad)
{ m_delimNScreate = create; m_delimNSaccept = accept;m_badNSchars = bad; }

inline TCHAR OPCEngine::getTreeDelimiter(OUT LPCTSTR *accept, 
	OUT LPCTSTR *bad)
{ if (accept) *accept = m_delimNSaccept;
  if (bad) *bad = m_badNSchars;
  return m_delimNScreate;}

inline void OPCEngine::setPropertyDelimiter(IN TCHAR propDel)
{ m_delimProp = propDel; }

inline TCHAR OPCEngine::getPropertyDelimiter(void) const
{ return m_delimProp; }

inline BOOL OPCEngine::setVendorInfo(IN LPCTSTR vendorInfo)
{ m_vendorInfo = vendorInfo; return TRUE; }

inline CString OPCEngine::getVendorInfo(void) const
{ return m_vendorInfo; }

inline BOOL OPCEngine::setVersionInfo(IN WORD majorVersion, 
	IN WORD minorVersion, IN WORD buildNumber)
{ m_majorVersion = majorVersion; m_minorVersion = minorVersion; 
  m_buildNumber = buildNumber; return TRUE; }

inline void OPCEngine::getVersionInfo(OUT WORD *majorVersion, 
	OUT WORD *minorVersion, OUT WORD *buildNumber) const
{ if (majorVersion) *majorVersion = m_majorVersion; 
  if (minorVersion) *minorVersion = m_minorVersion; 
  if (buildNumber) *buildNumber = m_buildNumber; }

inline BOOL OPCEngine::setServerState(IN OPCSERVERSTATE serverState)
{ m_serverState = serverState; return TRUE; }

inline OPCSERVERSTATE OPCEngine::getServerState(void) const
{ return m_serverState; }

inline BOOL OPCEngine::getStartTime(LPFILETIME startTime) const
{ if (!startTime) return FALSE;
  startTime->dwLowDateTime = m_startTime.dwLowDateTime; 
  startTime->dwHighDateTime = m_startTime.dwHighDateTime; return TRUE; }

inline GenericCreator *OPCEngine::getCreator(void)
{ if (m_creator) m_creator->addRef(); return m_creator; }

inline GenericEventHandler *OPCEngine::getEventHandler(void)
{ if (m_evtHandler) m_evtHandler->addRef(); return m_evtHandler; }

inline GenericConfigFileHandler *OPCEngine::getConfigurationFileHandler(void)
{ if (m_cfgHandler) m_cfgHandler->addRef(); return m_cfgHandler; }

inline void OPCEngine::setCheckParameter(IN DWORD clientInterval)
{ m_clientCheckInterval = clientInterval; }

inline void OPCEngine::setMinUpdateRate(DWORD minUpdateRate)
{ m_minUpdateRate = minUpdateRate; }

inline DWORD OPCEngine::getMinUpdateRate(void)
{ return m_minUpdateRate; }



//-----------------------------------------------------------------------------
// Global Functions                                                           |
//-----------------------------------------------------------------------------

// get pointer of the OPC engine object
OPCENGINE_EXPORT OPCEngine *getOPCEngine(void);

#endif

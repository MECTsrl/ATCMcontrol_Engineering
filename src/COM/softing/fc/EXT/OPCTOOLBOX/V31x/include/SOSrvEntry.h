//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SOSrv                              |
//                                                                            |
//  Filename    : SOSrvEntry.h                                                |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : Entry point for Server Toolkit                              |
//                - SOSrvEntry: Entry point class                             |
//                - SOSrvEventHandler: Handler for toolkit events             |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SOSRVENTRY_H_
#define _SOSRVENTRY_H_

#include <opcda.h>
#include "SOCmnObject.h"
#include "SOSrvWatch.h"

#pragma pack(push,4)


//-----------------------------------------------------------------------------
// SOSrvEventHandler                                                          |
//-----------------------------------------------------------------------------

class SOSRV_EXPORT SOSrvEventHandler : public SOCmnObject
{
public:
	SOSrvEventHandler();
	
	// released last COM object of the server
	virtual void onReleasedLastOPCObject(void);
}; // SOSrvEventHandler

#define GenericEventHandler SOSrvEventHandler




//-----------------------------------------------------------------------------
// SOSrvShutdownData                                                          |
//-----------------------------------------------------------------------------

class SOSrvServer;

class SOSrvShutdownData : public SOCmnData
{
public:
	SOCmnPointer<SOSrvServer> server; // server object
	SOCmnString reason;               // shutdown reason
	HANDLE event;                     // notification event for shutdown completion
}; // SOSrvShutdownData




//-----------------------------------------------------------------------------
// SOSrvConverter                                                             |
//-----------------------------------------------------------------------------

#define SOSRVCONVERTER_LCID_DEFAULT 0xFFFF

class SOSRV_EXPORT SOSrvConverter : public SOCmnObject
{
public:
	SOSrvConverter();

	virtual LPCTSTR bool2string(IN SOCmnString &buffer, IN BOOL b, OPTIONAL IN LCID localeID = SOSRVCONVERTER_LCID_DEFAULT);
	virtual LPCTSTR variant2string(IN SOCmnString &buffer, IN LPCVARIANT var, OPTIONAL IN LCID localeID = SOSRVCONVERTER_LCID_DEFAULT);
	virtual LPCTSTR vartype2string(IN SOCmnString &buffer, IN VARTYPE vt, OPTIONAL IN LCID localeID = SOSRVCONVERTER_LCID_DEFAULT);
	virtual LPCTSTR accessRights2string(IN SOCmnString &buffer, IN DWORD accessRight, OPTIONAL IN LCID localeID = SOSRVCONVERTER_LCID_DEFAULT);
	virtual LPCTSTR quality2string(IN SOCmnString &buffer, IN WORD quality, OPTIONAL IN LCID localeID = SOSRVCONVERTER_LCID_DEFAULT);
	virtual LPCTSTR filetime2string(IN SOCmnString &buffer, IN LPFILETIME filetime, OPTIONAL IN LCID localeID = SOSRVCONVERTER_LCID_DEFAULT);
	virtual LPCTSTR IID2string(IN SOCmnString &buffer, IN REFIID riid);
	virtual LPCTSTR CLSID2string(IN SOCmnString &buffer, IN REFCLSID rclsid);
	virtual LPCTSTR LCID2string(IN SOCmnString &buffer, IN LCID lcid, OPTIONAL IN LCID localeID = SOSRVCONVERTER_LCID_DEFAULT);
	virtual LPCTSTR serverState2string(IN SOCmnString &buffer, IN DWORD serverState, OPTIONAL IN LCID localeID = SOSRVCONVERTER_LCID_DEFAULT);
	virtual LPCTSTR publicState2string(IN SOCmnString &buffer, IN BOOL pub, IN BOOL noTempl, OPTIONAL IN LCID localeID = SOSRVCONVERTER_LCID_DEFAULT);
	virtual LPCTSTR euType2string(IN SOCmnString &buffer, IN OPCEUTYPE euType, OPTIONAL IN LCID localeID = SOSRVCONVERTER_LCID_DEFAULT);
	virtual LPCTSTR filters2string(IN SOCmnString &buffer, IN DWORD avFilter, OPTIONAL IN LCID localeID = SOSRVCONVERTER_LCID_DEFAULT);
	virtual LPCTSTR eventTypeFilter2string(IN SOCmnString &buffer, IN DWORD eventType, OPTIONAL IN LCID localeID = SOSRVCONVERTER_LCID_DEFAULT);
	virtual LPCTSTR conditionState2string(IN SOCmnString &buffer, IN BYTE conditionState, OPTIONAL IN LCID localeID = SOSRVCONVERTER_LCID_DEFAULT);
	virtual LPCTSTR authentificationLevel2string(IN SOCmnString &buffer, IN DWORD authLevel, OPTIONAL IN LCID localeID = SOSRVCONVERTER_LCID_DEFAULT);
	virtual LPCTSTR impersonationLevel2string(IN SOCmnString &buffer, IN DWORD impLevel, OPTIONAL IN LCID localeID = SOSRVCONVERTER_LCID_DEFAULT);
	virtual LPCTSTR account2string(IN SOCmnString &buffer, IN LPCTSTR identity, OPTIONAL IN LCID localeID = SOSRVCONVERTER_LCID_DEFAULT);

	LCID getLCID(void) const { return m_lcid; }
	virtual BOOL setLCID(IN LCID lcid);

protected:
	LCID m_lcid;
}; // SOSrvConverter




//-----------------------------------------------------------------------------
// SOSrvCreator                                                               |
//-----------------------------------------------------------------------------

class SOSrvWatch;

class SOSRV_EXPORT SOSrvCreator : public SOCmnObject
{
public:
	SOSrvCreator();
	virtual SOSrvWatch *createWatch(void);
	virtual SOSrvConverter *createConverter(void);
}; // SOSrvCreator




//-----------------------------------------------------------------------------
// SOSrvEntry                                                                 |
//-----------------------------------------------------------------------------

#define SOSRVENTRY_COM_OBJMAP_ENTRY(clsid, class) &clsid, class::UpdateRegistry, class::_ClassFactoryCreatorClass::CreateInstance, class::_CreatorClass::CreateInstance, class::GetObjectDescription, class::GetCategoryMap, class::ObjectMain

#define SOSRVENTRY_CATEGORY_DA10	0x01
#define SOSRVENTRY_CATEGORY_DA20	0x02
#define SOSRVENTRY_CATEGORY_AE10	0x10

#define SOSRVENTRY_FLAG_ATL_MODULE		0x01
#define SOSRVENTRY_FLAG_MAIN_THREAD		0x02

#define SOSRVENTRY_INIT_COM								0x01
#define SOSRVENTRY_INIT_SECURITY					0x02
#define SOSRVENTRY_INIT_HAVETO_UNINIT_COM	0x80

class SOCmnTrace;
class SOSrvObjectRoot;
class SOSrvWatch;

class SOSRV_EXPORT SOSrvEntry : public SOCmnObject
{
friend SOCmnTrace;

public:
	SOSrvEntry(void);

	enum serverTypeCOM
	{ 
		InProc,		// COM in process server (DLL)
		OutProc		// COM out of process server (EXE)
	};

	// initialize/terminate toolkit
	virtual BOOL initialize(IN enum serverTypeCOM type, IN REFCLSID clsid, 
			IN HINSTANCE hInst, OPTIONAL IN DWORD coInit = COINIT_APARTMENTTHREADED);
	virtual BOOL terminate(void);

	// start/stop main thread of toolkit 
	//  only if the main thread of the toolkit is running a OPC client will be able
	//  to create COM objects in the server
	virtual BOOL start(void);
	virtual BOOL stop(void);

	// server registration
	virtual HRESULT registerServer(void);
	HRESULT registerCategories(void);
	virtual HRESULT unregisterServer(void);
	HRESULT unregisterCategories(void);

	// helpers for InProc server export functions
	virtual HRESULT getClassObject(IN REFCLSID rclsid, IN REFIID riid, 
			OUT LPVOID *ppvObj) = 0;
	virtual HRESULT canUnloadNow(void) = 0;

	// set the register information
	BOOL setRegisterResource(IN UINT rgsResource);
	BOOL addRegisterResourceReplacement(IN LPCTSTR key, IN LPCTSTR data);
	BOOL setRegisterInfo(IN LPCTSTR progID, IN LPCTSTR verIndProgID, 
			IN LPCTSTR description, OPTIONAL IN LPCTSTR ntServiceName = NULL);
	BOOL setRegisterCategories(IN BYTE catFlag);

	// do the server registartion work
	virtual HRESULT updateRegistry(BOOL doRegister);

	// root of OPC object tree
	SOSrvObjectRoot *getObjectRoot(void);

	// engine event handler
	void setEventHandler(IN SOSrvEventHandler *cbHandler);
	SOSrvEventHandler *getEventHandler(void);

	// vendor string
	BOOL setVendorInfo(IN LPCTSTR vendorInfo);
	SOCmnString getVendorInfo(void) const;

	// version info
	BOOL setVersionInfo(IN WORD majorVersion, IN WORD minorVersion, 
			IN WORD buildNumber);
	void getVersionInfo(OUT WORD *majorVersion, OUT WORD *minorVersion, 
			OUT WORD *buildNumber) const;

	// start time of OPC engine
	BOOL getStartTime(OUT LPFILETIME startTime) const;

	// special release method
	virtual LONG release(void);	

	// parameter for the check routines of the engine
	void setCheckParameter(IN DWORD clientInterval);

	// send shutdown request to all connected clients
	virtual void shutdownAllClients(IN LPCTSTR reason);

	// main thread of the toolkit
	virtual DWORD mainThread(void);

	// get COM initialization mode of applications main thread
	DWORD getCoInit(void) const;

	// get class id of the OPC server
	virtual BOOL getCLSID(OUT CLSID &clsid, IN OPTIONAL BYTE serverInstance = 0);

	// entry flags
	BYTE getEntryFlags(void) const;
	void setEntryFlags(IN BYTE flags);	

	// set shutdown event
	void setShutdownEvent(IN HANDLE event);

	// main thread helpers
	void sendShutdownRequests(void);
	void checkClientConnections(void);
	void disconnectAllClients(void);

	BOOL isInitialized(IN BYTE mask) { return ((m_initialized & mask) == mask); }

	// object factory for toolbox objects
	virtual SOSrvCreator *getSrvCreator(void) = 0;

	// converter object
	SOSrvConverter *getConverter(void) { m_converter.addRef(); return m_converter; }
	void setConverter(IN SOSrvConverter *conv) { conv = m_converter; m_converter.addRef(); }

	// security
	virtual BOOL initializeSecurity(IN DWORD authnLevel, IN DWORD impLevel, 
			IN DWORD cbAuthnLevel, IN DWORD cbImpLevel);
	DWORD getAuthentificationLevel(void) { return m_serverAuthnLevel; }
	DWORD getAuthentificationLevelForAdvise(void) { return m_callbackAuthnLevel; }
	DWORD getImpersonationLevel(void) { return m_serverImpLevel; }
	DWORD getImpersonationLevelForAdvise(void) { return m_callbackImpLevel; }

	// ATL module access
	virtual HRESULT	registerClassObjects(void) = 0;
	virtual HRESULT	revokeClassObjects(void) = 0;
	virtual BOOL addComObjMapEntry(IN const CLSID *pClsid, IN void *pfnUpdateRegistry,
			IN void *pfnGetClassObject, IN void *pfnCreateInstance, IN void *pfnGetObjectDescription,
			IN void *pfnGetCategoryMap, IN void *pfnObjectMain) = 0;

	// server watch object
	SOCmnList<SOSrvWatch> *getWatchList(void) { return &m_watchList; } 
	MSXML::IXMLDOMDocumentPtr getWatchXMLDoc(void) { return m_watchDoc; }
	virtual SOSrvWatch *addWatchObject(void);
	virtual BOOL watchAttributeChange(IN SOCmnObject *pObj, IN MSXML::IXMLDOMNodePtr pAttrChange);
	virtual BOOL watchAddObject(IN SOCmnObject *pParent, IN SOCmnObject *pObj);
	virtual BOOL watchRemoveObject(IN SOCmnObject *pParent, IN SOCmnObject *pObj);

protected:
	virtual ~SOSrvEntry(void);

	// add shutdown request to shutdown queue
	BOOL addShutdownRequest(IN SOSrvServer *server, IN LPCTSTR reason, 
			IN HANDLE event);

	CLSID m_clsid;						// class id of the server
	HINSTANCE m_appInstanceHandle;		// instance handle of the server application
	SOSrvObjectRoot *m_objectRoot;		// OPC object root
	SOSrvEventHandler *m_evtHandler;	// event handler
	FILETIME m_startTime;				// start time of server
	BOOL m_terminateLock;				// termination lock
	BYTE m_initialized;					// initialized flags
	HANDLE m_hOPCThread;				// handle of OPC main thread
	HANDLE m_events[2];					// communications events
	serverTypeCOM m_type;				// COM server type
	DWORD m_coInit;						// COM initialiation mode
	DWORD m_clientCheckInterval;		// time intervall in ms for check functions
	SOCmnList<SOSrvShutdownData> m_shutdownList;
										// shutdown request list
	// register / unregister data
	UINT m_regRGSResID;					// resource id of ATL registar script
	SOCmnStringList m_regRGSRepList;	// replacement list for ATL registar script
	SOCmnString m_regProgID;			// progId of the server
	SOCmnString m_regVerIndProgID;		// version independent progId
	SOCmnString m_regDescription;		// server description
	SOCmnString m_regNTServiceName;		// register server as NT Service with this name
	BYTE m_regCategories;				// component categories (SOSRVENTRY_CATEGORY...)
	
	// server inforation
	SOCmnString m_vendorInfo;			// vendor string
	WORD m_majorVersion;				// version info
	WORD m_minorVersion; 
	WORD m_buildNumber; 

	// security settings
	DWORD m_serverAuthnLevel;
	DWORD m_callbackAuthnLevel;
	DWORD m_serverImpLevel;
	DWORD m_callbackImpLevel;

	// ATL and thread customizing
	BYTE m_entryFlags;

	// OPC server monitoring
	SOCmnPointer<SOSrvConverter> m_converter;
	SOCmnList<SOSrvWatch> m_watchList;
	MSXML::IXMLDOMDocumentPtr m_watchDoc;

	// ATL module access
	virtual void term(void) = 0;
	virtual void init(IN HINSTANCE instanceHandle) = 0;
	virtual HRESULT regServer(void) = 0;
	virtual HRESULT unregServer(void) = 0;
	virtual HRESULT updateRegistryWithRGS(IN BOOL doRegister, IN void *replacementArray) = 0;
}; // SOSrvEntry


inline BOOL SOSrvEntry::setVendorInfo(IN LPCTSTR vendorInfo)
{ m_vendorInfo = vendorInfo; return TRUE; }

inline SOCmnString SOSrvEntry::getVendorInfo(void) const
{ return m_vendorInfo; }

inline BOOL SOSrvEntry::setVersionInfo(IN WORD majorVersion, 
	IN WORD minorVersion, IN WORD buildNumber)
{ m_majorVersion = majorVersion; m_minorVersion = minorVersion; 
  m_buildNumber = buildNumber; return TRUE; }

inline void SOSrvEntry::getVersionInfo(OUT WORD *majorVersion, 
	OUT WORD *minorVersion, OUT WORD *buildNumber) const
{ if (majorVersion) *majorVersion = m_majorVersion; 
  if (minorVersion) *minorVersion = m_minorVersion; 
  if (buildNumber) *buildNumber = m_buildNumber; }

inline BOOL SOSrvEntry::getStartTime(LPFILETIME startTime) const
{ if (!startTime) return FALSE;
  startTime->dwLowDateTime = m_startTime.dwLowDateTime; 
  startTime->dwHighDateTime = m_startTime.dwHighDateTime; return TRUE; }

inline SOSrvEventHandler *SOSrvEntry::getEventHandler(void)
{ if (m_evtHandler) m_evtHandler->addRef(); return m_evtHandler; }

inline void SOSrvEntry::setCheckParameter(IN DWORD clientInterval)
{ m_clientCheckInterval = clientInterval; }

inline BOOL SOSrvEntry::setRegisterResource(IN UINT rgsResource)
{ m_regRGSResID = rgsResource; return TRUE; }

inline BOOL SOSrvEntry::setRegisterInfo(IN LPCTSTR progID, IN LPCTSTR verIndProgID, 
	IN LPCTSTR description, OPTIONAL IN LPCTSTR ntServiceName)
{ if (m_regRGSResID) return FALSE; m_regProgID = progID; m_regVerIndProgID = verIndProgID;
  m_regDescription = description; if (ntServiceName) m_regNTServiceName = ntServiceName; return TRUE; }

inline BOOL SOSrvEntry::setRegisterCategories(IN BYTE catFlag)
{ m_regCategories = catFlag; return TRUE; }

inline DWORD SOSrvEntry::getCoInit(void) const
{ return m_coInit; }

inline BYTE SOSrvEntry::getEntryFlags(void) const
{ return m_entryFlags; }

inline void SOSrvEntry::setEntryFlags(IN BYTE flags)
{ m_entryFlags = flags; }



//-----------------------------------------------------------------------------
// Global functions and defines                                               |
//-----------------------------------------------------------------------------

SOSRV_EXPORT void setSOSrvEntry(IN SOSrvEntry *entry);
SOSRV_EXPORT SOSrvEntry *getSOSrvEntry(void);

SOSRV_EXPORT SOSrvConverter *getSOSrvConverter(void);

#pragma pack(pop)
#endif

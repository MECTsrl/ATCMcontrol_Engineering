//-----------------------------------------------------------------------------
//                                  OPC ENGINE                                |
//                                                                            |
//  Filename    : OPCENGINE.H                                                 |
//  Version     : 2.00.0.00.release                                           |
//  Date        : 29-March-1999                                               |
//  Author      : ATCM                                                |
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

#include "stdafx.h"
#include <comcat.h>
#include "OPCEngine.h"
#include "GenNS.h"
#include "GenSrv.h"
#include "GenGrp.h"
#include "GenItem.h"
#include "GenCache.h"
#include "GenReq.h"
#include "buildnr.h"
#include "OPCTrace.h"
#include "OPCServer.h"

#ifdef SOFTING
#include "Softing/SoftingOPCTB.h"
#endif

//-----------------------------------------------------------------------------
// Globals                                                                    |
//-----------------------------------------------------------------------------

CComModule _Module;

static OPCEngine *g_engine = NULL;

// ATL global variables
CLSID g_clsid;
UINT g_registerId;

BEGIN_OBJECT_MAP(g_objectMap)
    OBJECT_ENTRY(g_clsid, OPCServer)
END_OBJECT_MAP()


// OPC Server categories
EXTERN_C const GUID CATID_OPCDAServer10;
EXTERN_C const GUID CATID_OPCDAServer20;


//-----------------------------------------------------------------------------
// GenericCreator                                                             |
//-----------------------------------------------------------------------------

GenericServer *GenericCreator::createServer(IN BOOL publicGroups)
{ return new GenericServer(publicGroups); }
GenericGroup *GenericCreator::createGroup(IN GenericServer *parent)
{ return new GenericGroup(); }
GenericItem *GenericCreator::createItem(IN GenericGroup *parent, IN GenericItemTag *tag)
{ return new GenericItem(); }
GenericRequest *GenericCreator::createRequest(IN GenericServer *server, 
	IN GenericItem *item, IN GenericItemTag *tag, IN BYTE operation, IN BYTE type, IN BYTE deviceIOMode)
{ return new GenericRequest(operation, type, deviceIOMode); }
GenericCache *GenericCreator::createCache(GenericItemTag *tag)
{ return new GenericCache(); }
GenericTag *GenericCreator::createTag(IN GenericServer *server, IN LPCTSTR itemID)
{ return NULL; }
GenericTagProperty *GenericCreator::createTagProperty(IN GenericTag *tag, IN DWORD propertyId)
{ return new GenericTagProperty(tag, propertyId); }
GenericCreator::GenericCreator(void)
	: GenericObject(GENOBJ_TYPE_CALLBACK)
{
	m_objectTraceId.Format(_T("[%s|%8.8X| %s]"), getObjTypeString(), this, _T("CREATOR"));
}




//-----------------------------------------------------------------------------
// GenericEventHandler                                                        |
//-----------------------------------------------------------------------------

void GenericEventHandler::onError(DWORD id, LPCTSTR description)
{}
void GenericEventHandler::onWarning(DWORD id, LPCTSTR description)
{}
void GenericEventHandler::onReleasedLastOPCObject(void)
{}
GenericEventHandler::GenericEventHandler(void)
	: GenericObject(GENOBJ_TYPE_CALLBACK)
{
	m_objectTraceId.Format(_T("[%s|%8.8X| %s]"), getObjTypeString(), this, _T("EVENT"));
}


GenericConfigFileHandler::GenericConfigFileHandler(void)
	: GenericObject(GENOBJ_TYPE_CALLBACK)
{
	m_objectTraceId.Format(_T("[%s|%8.8X| %s]"), getObjTypeString(), this, _T("CONFIG"));
}

//-----------------------------------------------------------------------------
// OPCEngine                                                                  |
//-----------------------------------------------------------------------------

#define END_EVENT		0
#define SHUTDOWN_EVENT	1

OPCEngine *getOPCEngine(void)
{
	if (g_engine)
	{
		g_engine->addRef();
	}
	else
	{
#ifdef SOFTING
		if (SOFTING_MAGIC_CHECK)
#endif
		{
			g_engine = new OPCEngine();
		}
	}

	return g_engine;
}


OPCEngine::OPCEngine(void)	
 : GenericObject(GENOBJ_TYPE_ENGINE)
{
	_TRACE(TL_DEB, TG_ENG, (_T("> constructor")));
	m_delimNScreate = _T('.');
	m_delimNSaccept = _T(".,/\\!");	
	m_badNSchars = _T("\"`´'|#");	
	m_delimProp = _T('#');

	m_vendorInfo = "OPC Engine";
	m_majorVersion = MAJOR_VERSION_NUMBER;
	m_minorVersion = MINOR_VERSION_NUMBER; 
	m_buildNumber = BUILD_NUMBER; 
	m_serverState = OPC_STATUS_RUNNING;	
    m_startTime.dwLowDateTime  = 0L;
    m_startTime.dwHighDateTime = 0L;
	m_hOPCThread = INVALID_HANDLE_VALUE;
	m_initialized = FALSE;
	m_clientCheckInterval = INFINITE;
	m_minUpdateRate = 20;	// minimal NT timer is 10 msecs !!

	m_nsRoot = new OPCRootNS();
	m_opcRoot = new OPCRootOPC();

	m_terminateLock = TRUE;
	m_cfgHandler = NULL;
	m_evtHandler = new GenericEventHandler();
	m_creator = new GenericCreator();

	memset(&m_curStat, 0, sizeof(OPCStatistics));
	memset(&m_allStat, 0, sizeof(OPCStatistics));
	m_lastStatTime = ::GetTickCount();
	m_allStat.startTime = ::GetTickCount();
	m_type = InProc;

	m_events[END_EVENT] = popPoolEvent();
	m_events[SHUTDOWN_EVENT] = popPoolEvent();
	_TRACE(TL_DEB, TG_ENG, (_T("< constructor")));
}


OPCEngine::~OPCEngine(void)
{
	OPCShutdownRequest *sreq; 

	_TRACE(TL_DEB, TG_ENG, (_T("> destructor")));
	setEventHandler(NULL);
	setConfigurationFileHandler(NULL);
	m_creator->release();
	m_nsRoot->release();
	m_opcRoot->release();
	g_engine = NULL;

	while(!m_shutdownList.IsEmpty())
	{
		sreq = m_shutdownList.RemoveHead();
		if (sreq->server)
			sreq->server->release();
		delete sreq;
	}

	if (m_events[END_EVENT] != INVALID_HANDLE_VALUE)
		CloseHandle(m_events[END_EVENT]);
	if (m_events[SHUTDOWN_EVENT] != INVALID_HANDLE_VALUE)
		CloseHandle(m_events[SHUTDOWN_EVENT]);

	freeAllPoolEvents();

	_TRACE(TL_DEB, TG_ENG, (_T("< destructor")));
}


//-----------------------------------------------------------------------------
// terminate
//
// - reset terminate lock
//
BOOL OPCEngine::terminate(void)
{
	_TRACE(TL_INF, TG_ENG, (_T("terminate engine")));
	m_terminateLock = FALSE;

	GenericPointer<GenericServer> pg;
	if ((GenericServer *)(pg = getPublicGroups()))
	{
		m_opcRoot->removeBranch(pg);
		pg->removeAllLeafs();
		pg->removeAllBranches();
	}	

	_Module.Term();

	if (m_initialized)
	{
		::CoUninitialize();
	}

	_TRACE(TL_DEB, TG_ENG, (_T("< terminate")));
	return TRUE;
} // terminate


//-----------------------------------------------------------------------------
// release
//
// - decrement reference counter
// - free object if no reference left and terminate lock not set
//
LONG OPCEngine::release(void)
{
	BOOL terminate = m_terminateLock;
	LONG refCount = InterlockedDecrement(&m_refCount);
	_TRACE(TL_DEB, TG_GEN, (_T("release %li locked:%s"), refCount, BOOL2STR(terminate)))
	if ((refCount <= 0) && (!terminate))
	{
		stop();
		delete this;	// last reference and terminate flag set -> free object 
		return 0;
	}
	return refCount;
} // release


BOOL OPCEngine::initialize(
	IN enum serverTypeCOM type, 
	IN REFCLSID clsid, 
	IN UINT rgsResource, 
	IN HINSTANCE hInst,
	IN DWORD coInit)	// COM init mode
{ 
	_TRACE(TL_INF, TG_ENG, (_T("initialize engine; clsid:%8.8X-%4.4X-%4.4X-%2.2X%2.2X%2.2X%2.2X%2.2X%2.2X%2.2X%2.2X"), clsid.Data1, clsid.Data2, clsid.Data3, clsid.Data4[0], clsid.Data4[1], clsid.Data4[2], clsid.Data4[3], clsid.Data4[4], clsid.Data4[5], clsid.Data4[6], clsid.Data4[7]));
	HRESULT res;

	g_clsid = clsid;
	g_registerId = rgsResource;

	m_type = type;

	if (type != InProc)
	{
		if (SUCCEEDED(res = ::CoInitializeEx(NULL, coInit)))
			m_initialized = TRUE;
		else
			_ERROR(TG_ENG, OPCENGINE_ERR_COM,(_T("Can't initialize COM: coInit:0x%X [0x%X]"), coInit, res));
	}

	_Module.Init(g_objectMap, hInst);
    ::CoFileTimeNow(&m_startTime);

	GenericPointer<GenericServer> pg = m_creator->createServer(TRUE);
	m_opcRoot->addBranch(pg);
	pg->setName(_T("PublicGroups"));
	
	_TRACE(TL_DEB, TG_ENG, (_T("< initialize 0x%X"), res));
	return SUCCEEDED(res); 
}


DWORD WINAPI OPCMainThread(
	IN LPVOID context)	// application object
{
	OPCEngine *eng = (OPCEngine *)context;
	return eng->mainOPCThread();
} // requestThreadMain

DWORD OPCEngine::mainOPCThread(void)
{
	DWORD ret;
	BOOL end = FALSE;

	try
	{
		_TRACE(TL_INF, TG_ENG, (_T("started engine thread")));

		//-- initialize engine thread --
		_TRACE(TL_DEB, TG_ENG, (_T("COM initialize (multithreaded)")));
		HRESULT res = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
		if( FAILED(res)) 
		{
			_ERROR(TG_ENG, OPCENGINE_ERR_COM,(_T("Can't initialize COM: 0x%X"), res));
			return res;
		}

		if (m_type == OutProc)
		{
			_TRACE(TL_DEB, TG_ENG, (_T("register class objects")));
			_Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER | CLSCTX_REMOTE_SERVER, REGCLS_MULTIPLEUSE);
		}

		//-- wait till engine termiantion --
		while (!end)
		{
			ret = ::WaitForMultipleObjects(2, m_events, FALSE, m_clientCheckInterval);

			switch (ret - WAIT_OBJECT_0)
			{
				case END_EVENT:
				{ // end event
					end = TRUE;
					_TRACE(TL_DEB, TG_ENG, (_T("received end event")));
				} 
				break;

				case SHUTDOWN_EVENT:
				{ // shutdown event
					OPCShutdownRequest *sreq; 

					_TRACE(TL_DEB, TG_ENG, (_T("received shutdown event")));
					while(!m_shutdownList.IsEmpty())
					{
						sreq = m_shutdownList.RemoveHead();
						if (sreq->server)
						{
							sreq->server->sendShutdownRequest(sreq->reason);
							if (sreq->event != INVALID_HANDLE_VALUE)
								::SetEvent(sreq->event);	
							sreq->server->release();
						}
						delete sreq;
					}
				} 
				break;

				default:
				{ // timeout
					//-- check client connections --
					GenericList<GenericBranch> serverListCopy;
					POSITION posSrv;
					GenericServer *server;	
					BOOL conOK;

					serverListCopy.create();

					{
				 	GenericList<GenericBranch> serverList(m_opcRoot->getBranchList());
					posSrv = serverList.getStartPosition();
					while(posSrv)
					{
						server = (GenericServer *)serverList.getNext(posSrv);
						server->addRef();
						serverListCopy.add(server);
					}
					}

					posSrv = serverListCopy.getStartPosition();
					while(posSrv)
					{
						server = (GenericServer *)serverListCopy.getNext(posSrv);

						conOK = server->checkClientConnection();
							
						if (!conOK)
						{
							server->disconnectClient();
						}
						server->release();
					}

					serverListCopy.free();
				}
				break;
			}
		}

		//-- terminated engine --
		if (m_type == OutProc)
		{
			_TRACE(TL_DEB, TG_ENG, (_T("revoke class objects")));
			_Module.RevokeClassObjects(); 
		}

		//-- disconnect all clients --
	 	GenericList<GenericBranch> serverList(m_opcRoot->getBranchList());
		POSITION posSrv;
		GenericServer *server;	
		posSrv = serverList.getStartPosition();
		while(posSrv)
		{
			server = (GenericServer *)serverList.getNext(posSrv);
			server->disconnectClient();
		}
		serverList.unlock();

		_TRACE(TL_DEB, TG_ENG, (_T("COM uninitialize")));
		::CoUninitialize();
		_TRACE(TL_INF, TG_ENG, (_T("terminated engine thread")));

	}
	catch(CException* e)
	{
		TCHAR excText[100];
		CRuntimeClass* rt = e->GetRuntimeClass();
		e->GetErrorMessage(excText, 100);
		_ERROR(TG_ENG, OPCENGINE_ERR_EXCEPTION,(_T("MFC EXCEPTION in OPC Engine Main Thread: %s (%s)"), rt->m_lpszClassName, excText));
		return 1;
	}	
	catch(...)
	{
		_ERROR(TG_ENG, OPCENGINE_ERR_EXCEPTION,(_T("EXCEPTION in OPC Engine Main Thread")));
		return 1;
	}	

    return S_OK;
}


BOOL OPCEngine::start(void)
{ 
	if (m_hOPCThread == INVALID_HANDLE_VALUE) 
	{
		DWORD idOPCThread;

		m_hOPCThread = ::CreateThread(NULL, 0,
				(LPTHREAD_START_ROUTINE) &OPCMainThread,	
				 this, 0, &idOPCThread);
	}

	_TRACE(TL_INF, TG_ENG, (_T("start engine")));
	return (m_hOPCThread != INVALID_HANDLE_VALUE); 
}


BOOL OPCEngine::stop(void)
{
	if (m_hOPCThread != INVALID_HANDLE_VALUE) 
	{
		::SetEvent(m_events[END_EVENT]);
		if (WAIT_FAILED == ::WaitForSingleObject(m_hOPCThread, 10000)) 
		{
			_WARNING(TG_GRP, OPCENGINE_WRN_THREAD,(_T("Can't terminate engine thread")));
		}
		::CloseHandle(m_hOPCThread);
		m_hOPCThread = INVALID_HANDLE_VALUE;
	}

	_TRACE(TL_INF, TG_ENG, (_T("stop engine")));
	return (m_hOPCThread == INVALID_HANDLE_VALUE); 
}

HRESULT OPCEngine::getClassObject(
	IN REFCLSID rclsid, 
	IN REFIID riid, 
	OUT LPVOID *ppvObj)
{ 
	return _Module.GetClassObject(rclsid, riid, ppvObj);
}

HRESULT OPCEngine::canUnloadNow(void)
{ 
	LONG lockCount = _Module.GetLockCount();
	_TRACE(TL_INF, TG_COM, (_T("canUnloadNow: lock count %ld"), lockCount));
	return (lockCount == 0) ? S_OK : S_FALSE; 
}

HRESULT OPCEngine::registerServer(void)
{ 
	_TRACE(TL_INF, TG_ENG, (_T("register server")));
	HRESULT res;

	// register COM Server object
	res = _Module.RegisterServer(FALSE);

	if (SUCCEEDED(res))
	{
		ICatRegister *cr;
		CATEGORYINFO catInfo[4];
		CATID catIDs[2];

		if (SUCCEEDED(res = ::CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
			NULL, CLSCTX_ALL, IID_ICatRegister, (void **)&cr)))
		{
			// register category
			catInfo[0].catid = CATID_OPCDAServer10;
			catInfo[1].catid = CATID_OPCDAServer10;
			catInfo[2].catid = CATID_OPCDAServer20;
			catInfo[3].catid = CATID_OPCDAServer20;
			catInfo[0].lcid = MAKELCID( MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US) , SORT_DEFAULT);
			catInfo[1].lcid = MAKELCID( MAKELANGID(LANG_GERMAN, SUBLANG_GERMAN),  SORT_DEFAULT);
			catInfo[2].lcid = MAKELCID( MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US) , SORT_DEFAULT);
			catInfo[3].lcid = MAKELCID( MAKELANGID(LANG_GERMAN, SUBLANG_GERMAN),  SORT_DEFAULT);
			wcscpy(catInfo[0].szDescription, L"OPC Data Access Server V1.0A");
			wcscpy(catInfo[1].szDescription, L"OPC Daten Server V1.0A");
			wcscpy(catInfo[2].szDescription, L"OPC Data Access Server V2.0");
			wcscpy(catInfo[3].szDescription, L"OPC Daten Server V2.0");
			res = cr->RegisterCategories(4, catInfo);
			_TRACE(TL_INF, TG_ENG, (_T("register component categories [0x%X]"), res));

			// register server in categories
			catIDs[0] = CATID_OPCDAServer10;
			catIDs[1] = CATID_OPCDAServer20;
			res = cr->RegisterClassImplCategories(g_clsid, 2, catIDs);
			_TRACE(TL_INF, TG_ENG, (_T("register component implementations [0x%X]"), res));

			cr->Release();
		}
	}
	return res; 
} // registerServer

HRESULT OPCEngine::unregisterServer(void)
{ 
	_TRACE(TL_INF, TG_ENG, (_T("unregister server")));
	HRESULT res;

	{
		ICatRegister *cr;
		ICatInformation *ci = NULL ;
		IEnumCLSID* enumCLSID = NULL ;
		CATID catIDs[2];
		CLSID clsid ;

		if (SUCCEEDED(res = ::CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
			NULL, CLSCTX_ALL, IID_ICatRegister, (void **)&cr)))
		{
			// unregister server in categories
			catIDs[0] = CATID_OPCDAServer10;
			catIDs[1] = CATID_OPCDAServer20;
			res = cr->UnRegisterClassImplCategories(g_clsid, 2, catIDs);
			_TRACE(TL_INF, TG_ENG, (_T("unregister component implementations [0x%X]"), res));

			// check if this server was the last in the category
			if (SUCCEEDED(res = cr->QueryInterface(IID_ICatInformation, (void**)&ci)))
			{
				// OPC DA Server V1.0A
				if (SUCCEEDED (res = ci->EnumClassesOfCategories(1, &catIDs[0], 0, NULL, &enumCLSID)))
				{
					res = enumCLSID->Next(1, &clsid, NULL) ;

					// if result is S_FALSE, there no components are in this category, so remove it.
					if (res == S_FALSE)
					{
						// unregister the component category
						res = cr->UnRegisterCategories(1, &catIDs[0]) ;
						_TRACE(TL_INF, TG_ENG, (_T("unregister DA V1.0A component category [0x%X]"), res));
					}
					enumCLSID->Release();
				}

				// OPC DA Server V2.0
				if (SUCCEEDED (res = ci->EnumClassesOfCategories(1, &catIDs[1], 0, NULL, &enumCLSID)))
				{
					res = enumCLSID->Next(1, &clsid, NULL) ;

					// if result is S_FALSE, there no components are in this category, so remove it.
					if (res == S_FALSE)
					{
						// unregister the component category
						res = cr->UnRegisterCategories(1, &catIDs[1]) ;
						_TRACE(TL_INF, TG_ENG, (_T("unregister DA V2.0 component category [0x%X]"), res));
					}
					enumCLSID->Release();
				}

				ci->Release();
			}
			cr->Release();
		}

		CRegKey key, keyCLSID;
		DWORD ret;

		if (ret = keyCLSID.Open(HKEY_CLASSES_ROOT, _T("CLSID")) == ERROR_SUCCESS)
		{
			USES_CONVERSION;
			LPOLESTR wszClsid;
			LPTSTR clsid;
			StringFromCLSID(g_clsid, &wszClsid);
			clsid = W2T(wszClsid);
			CoTaskMemFree(wszClsid);

			if (ret = key.Open(keyCLSID, clsid) == ERROR_SUCCESS)
			{
				key.RecurseDeleteKey(_T("Implemented Categories"));
				key.Close();
			}
			else
				res = HRESULT_FROM_WIN32(ret);

			keyCLSID.Close();
		}
		else
			res = HRESULT_FROM_WIN32(ret);
	}

	// unregister COM Server object
	res = _Module.UnregisterServer();

	return res; 
} // unregisterServer


HRESULT OPCEngine::inititalizeDCOMSecurity(
	IN DWORD authenticationLevel, 
	IN DWORD impersonationLevel, 
	IN CStringList &accessPermit, 
	IN CStringList &accessDeny)
{ return E_NOTIMPL; }

BOOL OPCEngine::enableRequestUserNames(
	IN BOOL enable)
{ return FALSE; }

BOOL OPCEngine::getStatistics(
	OUT OPCStatistics *statAll, 
	OUT OPCStatistics *statSinceLastCall, 
	OUT DWORD *msSinceLastCall)
{ 
	OPCStatistics curStat;
	ULONG reads;
	ULONG writes;

	_TRACE(TL_DEB, TG_ENG, (_T("get statistics >")));
	m_curStatLock.Lock();
	memcpy(&curStat, &m_curStat, sizeof(OPCStatistics));
	memset(&m_curStat, 0, sizeof(OPCStatistics));
	m_curStatLock.Unlock();

	DWORD now = ::GetTickCount();

	if (getTimeSpan(m_allStat.startTime, now) > 86400000)
	{
		// reset all statistics after 1 day
		memset(&m_allStat, 0, sizeof(OPCStatistics));
		m_allStat.startTime = ::GetTickCount();
	}

	//-- calculate current statistics --

	// times
	if (reads = (curStat.req.asyncDeviceRead + curStat.req.syncDeviceRead	+ curStat.req.cyclicRead 
			- curStat.req.asyncDeviceReadBad - curStat.req.syncDeviceReadBad - curStat.req.cyclicReadBad))
		curStat.req.averageTimeRead = (curStat.req.timeRead / reads);

	if (writes = (curStat.req.asyncWrite + curStat.req.syncWrite - curStat.req.asyncWriteBad - curStat.req.syncWriteBad))
		curStat.req.averageTimeWrite = (curStat.req.timeWrite / writes);

	// objects
	{
 	GenericList<GenericBranch> serverList(m_opcRoot->getBranchList());
	GenericList<GenericBranch> groupList;
	GenericList<GenericLeaf> itemList;
	POSITION posSrv;
	POSITION posGrp;
	GenericServer *server;	
	GenericGroup *group;	

	curStat.obj.serverObjects = 0;
	curStat.obj.groupObjects = 0;
	curStat.obj.publicGroupObjects = 0;
	curStat.obj.publicGroupTemplates = 0;
	curStat.obj.itemObjects = 0;

	posSrv = serverList.getStartPosition();
	while(posSrv)
	{
		server = (GenericServer *)serverList.getNext(posSrv);
		if (server)
		{
			groupList = server->getBranchList();
			groupList.lock();

			if (!server->isPublicGroups())
			{
				curStat.obj.serverObjects++;	
				posGrp = groupList.getStartPosition();
				while(posGrp)
				{
					group = (GenericGroup *)groupList.getNext(posGrp);

					if (!group->isPublic())
					{
						curStat.obj.groupObjects++;
					}			
					else
					{
						curStat.obj.publicGroupObjects++;
					}

					itemList = group->getLeafList();
					curStat.obj.itemObjects += itemList.getCount();
				}
			}
			else
			{
				curStat.obj.publicGroupTemplates = groupList.getCount();;
			}

			groupList.unlock();
		}
	}
	}

	//-- add current to all statistics --
	m_allStat.req.asyncCacheRead += curStat.req.asyncCacheRead;
	m_allStat.req.asyncDeviceRead += curStat.req.asyncDeviceRead;
	m_allStat.req.asyncWrite += curStat.req.asyncWrite;
	m_allStat.req.syncCacheRead += curStat.req.syncCacheRead;
	m_allStat.req.syncDeviceRead += curStat.req.syncDeviceRead;
	m_allStat.req.syncWrite += curStat.req.syncWrite;
	m_allStat.req.cyclicRead += curStat.req.cyclicRead;
	m_allStat.req.reportRead += curStat.req.reportRead;
	m_allStat.req.asyncCacheReadBad += curStat.req.asyncCacheReadBad;
	m_allStat.req.asyncDeviceReadBad += curStat.req.asyncDeviceReadBad;
	m_allStat.req.asyncWriteBad += curStat.req.asyncWriteBad;
	m_allStat.req.syncCacheReadBad += curStat.req.syncCacheReadBad;
	m_allStat.req.syncDeviceReadBad += curStat.req.syncDeviceReadBad;
	m_allStat.req.syncWriteBad += curStat.req.syncWriteBad;
	m_allStat.req.cyclicReadBad += curStat.req.cyclicReadBad;
	m_allStat.req.reportReadBad += curStat.req.reportReadBad;
	m_allStat.req.timeRead += curStat.req.timeRead;
	m_allStat.req.timeWrite += curStat.req.timeWrite;

	//-- calculate all statistics --
	if (curStat.req.maxTimeRead > m_allStat.req.maxTimeRead)
		m_allStat.req.maxTimeRead = curStat.req.maxTimeRead;
	if (curStat.req.maxTimeWrite > m_allStat.req.maxTimeWrite)
		m_allStat.req.maxTimeWrite = curStat.req.maxTimeWrite;

	if (reads = (m_allStat.req.asyncDeviceRead + m_allStat.req.syncDeviceRead	+ m_allStat.req.cyclicRead 
			- m_allStat.req.asyncDeviceReadBad - m_allStat.req.syncDeviceReadBad - m_allStat.req.cyclicReadBad))
		m_allStat.req.averageTimeRead = (m_allStat.req.timeRead / reads);

	if (writes = (m_allStat.req.asyncWrite + m_allStat.req.syncWrite - m_allStat.req.asyncWriteBad - m_allStat.req.syncWriteBad))
		m_allStat.req.averageTimeWrite = (m_allStat.req.timeWrite / writes);

	if (curStat.obj.serverObjects > m_allStat.obj.serverObjects)
		m_allStat.obj.serverObjects = curStat.obj.serverObjects;
	if (curStat.obj.groupObjects > m_allStat.obj.groupObjects)
		m_allStat.obj.groupObjects = curStat.obj.groupObjects;
	if (curStat.obj.publicGroupObjects > m_allStat.obj.publicGroupObjects)
		m_allStat.obj.publicGroupObjects = curStat.obj.publicGroupObjects;
	if (curStat.obj.publicGroupTemplates > m_allStat.obj.publicGroupTemplates)
		m_allStat.obj.publicGroupTemplates = curStat.obj.publicGroupTemplates;
	if (curStat.obj.itemObjects > m_allStat.obj.itemObjects)
		m_allStat.obj.itemObjects = curStat.obj.itemObjects;

	m_allStat.allCount++;
	curStat.allCount = 1;

	//-- copy to output buffer --
	if (statAll)
	{
		memcpy(statAll, &m_allStat, sizeof(OPCStatistics));
	}
	if (statSinceLastCall)
	{
		memcpy(statSinceLastCall, &curStat, sizeof(OPCStatistics));
	}
	if (msSinceLastCall)
	{
		*msSinceLastCall = getTimeSpan(m_lastStatTime, now);
	}
	m_lastStatTime = now;

	_TRACE(TL_DEB, TG_ENG, (_T("< get statistics")));
	return TRUE; 
}



void OPCEngine::setTraceLevel(
	IN DWORD error,		// level error 
	IN DWORD warning,	// level warning
	IN DWORD info,		// level info
	IN DWORD debug)		// level debug
{
	g_trace.setLevels(error, warning, info, debug);
	_TRACE(TL_DEB, TG_ENG, (_T("set trace levels E:%X W:%X I%X D:%X"), error, warning, info, debug));
} // setTraceLevel


//-----------------------------------------------------------------------------
// setTraceFile
//
// - set new trace file
// - opens the trace file
//
// returns:
//		TRUE -  new trace file opened
//		FALSE - error open trace file 
//
BOOL OPCEngine::setTraceFile(
	IN LPCTSTR file,						// file name
	IN OPTIONAL LPCTSTR secondFile, 
	IN OPTIONAL DWORD maxFileSize,
	IN OPTIONAL DWORD creationDisposition)
{ 
	BOOL res = g_trace.setFile(file, secondFile, maxFileSize, creationDisposition);
	_TRACE(TL_DEB, TG_ENG, (_T("set trace file %s %s %lu"), file, secondFile, maxFileSize));
	return res;
} // setTraceFile


//-----------------------------------------------------------------------------
// setEventHandler
//
// - set event handler object
//
void OPCEngine::setEventHandler(
	IN GenericEventHandler *events)	// event handler object
{ 
	if (m_evtHandler)
		m_evtHandler->release();
 
	_TRACE(TL_DEB, TG_ENG, (_T("set event handler 0x%X"), events));
	m_evtHandler = events; 

	if (m_evtHandler)
		m_evtHandler->addRef();
} // setEventHandler


//-----------------------------------------------------------------------------
// setConfigurationFileHandler
//
// - set configuration file handler object
//
void OPCEngine::setConfigurationFileHandler(
	IN GenericConfigFileHandler *cfgHandler)	// configuration handler object
{ 
	if (m_cfgHandler)
		m_cfgHandler->release();

	_TRACE(TL_DEB, TG_ENG, (_T("set configuration file handler 0x%X"), cfgHandler));
	m_cfgHandler = cfgHandler; 

	if (m_cfgHandler)
		m_cfgHandler->addRef();
} // setConfigurationFileHandler


//-----------------------------------------------------------------------------
// setCreator
//
// - set OPC object factory
//
void OPCEngine::setCreator(
	IN GenericCreator *creator)	// OPC object factory
{ 
	if (creator) 
	{
		if (m_creator)
			m_creator->release();

		m_creator = creator; 
		_TRACE(TL_DEB, TG_ENG, (_T("set OPC creator 0x%X"), creator));

		if (m_creator)
			m_creator->addRef();
	}
} // setCreator


//-----------------------------------------------------------------------------
// getNamespaceRoot
//
// - get namespace root object
//
// returns:
//		namespace root
//
OPCRootNS *OPCEngine::getNamespaceRoot(void)
{ 
	m_nsRoot->addRef();
	return m_nsRoot; 
} // getNamespaceRoot


//-----------------------------------------------------------------------------
// getOPCRoot
//
// - get OPC tree root object
//
// returns:
//		OPC tree root
//
OPCRootOPC *OPCEngine::getOPCRoot(void)
{ 
	m_opcRoot->addRef();
	return m_opcRoot; 
} // getOPCRoot


// get the public groups server object
GenericServer *OPCEngine::getPublicGroups(void)
{
	GenericServer *server;
 	GenericList<GenericBranch> list(m_opcRoot->getBranchList());
	POSITION pos;

	pos = list.getStartPosition();
	while(pos)
	{
		server = (GenericServer *)list.getNext(pos);
		if (server->isPublicGroups())
		{ // public groups server found
			_TRACE(TL_DEB, TG_SRV, (_T("public groups server %s"), server->getObjTraceId()));
			server->addRef();
			return (GenericServer *)server;
		}
	}
	_TRACE(TL_DEB, TG_SRV, (_T("no public groups server")));
	return NULL;
}

void OPCEngine::error(
	IN DWORD id, 
	IN LPCTSTR description, 
	IN BOOL error)
{
	if (m_evtHandler)
	{
		if (error)
			m_evtHandler->onError(id, description);
		else
			m_evtHandler->onWarning(id, description);
	}
}

OPCStatistics *OPCEngine::getStatisticsBuffer(void)
{ 
	m_curStatLock.Lock();
	return &m_curStat; 
}

void OPCEngine::releaseStatisticsBuffer(void)
{ 
	m_curStatLock.Unlock();
}


BOOL OPCEngine::addShutdownRequest(
	IN GenericServer *server,
	IN LPCTSTR reason,
	IN HANDLE event)
{
	if (server)
		server->addRef();
	else
		return FALSE;

	OPCShutdownRequest *sreq = new OPCShutdownRequest;
	sreq->server = server;
	sreq->reason = reason;
	sreq->event = event;
	m_shutdownList.AddTail(sreq);
	::SetEvent(m_events[SHUTDOWN_EVENT]);
	return TRUE;
}


void OPCEngine::shutdownAllClients(
	IN LPCTSTR reason)
{
 	GenericList<GenericBranch> list(m_opcRoot->getBranchList());
	POSITION posSrv;
	GenericServer *server;	

	posSrv = list.getStartPosition();
	while(posSrv)
	{
		server = (GenericServer *)list.getNext(posSrv);
		if (!server->isPublicGroups())
		{
			OPCShutdownRequest *sreq = new OPCShutdownRequest;
			server->addRef();
			sreq->server = server;
			sreq->reason = reason;
			sreq->event = INVALID_HANDLE_VALUE;
			m_shutdownList.AddTail(sreq);
		}
	}
	list.unlock();
	::SetEvent(m_events[SHUTDOWN_EVENT]);
}

HANDLE OPCEngine::popPoolEvent(void)
{
	CSingleLock lock(&m_eventPoolLock, TRUE);
	HANDLE event;

	if(!m_eventPool.IsEmpty())
	{
		event = m_eventPool.RemoveHead();
	}
	else
	{
		event = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	}

	return event;
}

void OPCEngine::pushPoolEvent(HANDLE event)
{
	CSingleLock lock(&m_eventPoolLock, TRUE);
	::ResetEvent(event);
	m_eventPool.AddTail(event);
}

void OPCEngine::freeAllPoolEvents(void)
{
	CSingleLock lock(&m_eventPoolLock, TRUE);
	HANDLE event;

	while(!m_eventPool.IsEmpty())
	{
		event = m_eventPool.RemoveHead();
		CloseHandle(event);
	}
}

HRESULT OPCEngine::revokeClassObjects(void)
{
	return _Module.RevokeClassObjects(); 
}

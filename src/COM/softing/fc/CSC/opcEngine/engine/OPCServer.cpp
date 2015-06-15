//-----------------------------------------------------------------------------
//                                  OPC ENGINE                                |
//                                                                            |
//  Filename    : OPCSERVER.CPP                                               |   
//  Version     : 2.00.0.00.release                                           |
//  Date        : 29-March-1999                                               |
//                                                                            |
//  Description : OPCServer class                                             |
//                                                                            |
//  CHANGE_NOTES                                                              | 
//                                                                            |
//  date      name      change                                                |
//  ------------------------------------------------------------------------- |
//                                                                            |
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "OPCEngine.h"
#include "OPCServer.h"
#include "OPCGroup.h"
#include "GenSrv.h"
#include "GenGrp.h"
#include "OPCTrace.h"
#include "enum.h"
#include "globals.h"
#include <opcerror.h>


//-----------------------------------------------------------------------------
// OPCServer                                                                  |
//-----------------------------------------------------------------------------

OPCServer::OPCServer()
{
	m_objectTracePos = NULL;
	_TRACE(TL_DEB, TG_SRV, (_T("> constructor")));
	_TRACE_CREATE_COMSRV();
	m_me = NULL;
	_TRACE(TL_DEB, TG_SRV, (_T("< constructor")));
}

OPCServer::~OPCServer()
{
	_TRACE(TL_DEB, TG_SRV, (_T("> destructor")));
	_TRACE_DELETE_COMSRV();
	GenericPointer<OPCEngine> engine = ::getOPCEngine();
	GenericPointer<OPCRootOPC> root = engine->getOPCRoot();

	// remove tree object from tree
	root->removeBranch(m_me);
	// release tree object
	m_me->release();
	m_me = NULL;

	::lastCOMObjectActions();
	_TRACE(TL_DEB, TG_SRV, (_T("< destructor")));
}

void OPCServer::initObject(void)
{
	GenericPointer<OPCEngine> engine = ::getOPCEngine();
	GenericPointer<OPCRootOPC> root;
	GenericPointer<GenericCreator> creator;	

	// create OPC tree object
	creator = engine->getCreator();
	m_me = creator->createServer(FALSE);

	m_me->m_me = this;
	// add to tree
	root = engine->getOPCRoot();
	root->addBranch(m_me);
}

BOOL OPCServer::releaseMe(void)
{
	BOOL ret = TRUE;
	if (m_me)
	{
		m_me->m_meSync.Lock();
		ret = (m_me->m_me != NULL);
		m_me->m_me = NULL;
		m_me->m_meSync.Unlock();

		if (ret)
		{
			while(m_dwRef > 1)
				Sleep(0);
		}
	}
	return ret;
}

//-----------------------------------------------------------------------------
// getObjTraceId
//
// - get objects trace string id
//
// returns:
//		trace id string
//
LPCTSTR OPCServer::getObjTraceId(void)
{ 
	if (m_objectTraceId.IsEmpty())
	{
		m_objectTraceId.Format(_T("(servr|%8.8X)"), this);
		m_objectTraceId.FreeExtra();
	}
	return m_objectTraceId; 
} // getObjTraceId


//-----------------------------------------------------------------------------
// createOPCGroup
//
// - creates a COM OPCGroup object
//
// returns:
//		result of QueryInterface
//
HRESULT OPCServer::createOPCGroup(
	IN GenericGroup *grp,		// generic group object
	IN REFIID riid,				// id of requested interface
	OUT LPUNKNOWN * ppUnk,		// pointer to requested interface
	IN OPTIONAL BOOL doAddRef)	// AddRef the created object?
{
	_TRACE(TL_DEB, TG_SRV, (_T("> createOPCGroup")));
	HRESULT res;

	// create COM group object
	OPCGroupObj *group = new OPCGroupObj;
	group->init(this, grp);	// when group object is deleted it releases grp

	// query the requested interface
	res = group->QueryInterface(riid, (void **)ppUnk);

	if (SUCCEEDED(res))
	{
		if (doAddRef)
		{
			group->internalAddRef();	// add reference for creation, 
								// will released via RemoveGroup 
		}
		grp->m_serverHandle = (OPCHANDLE)group;
		_TRACE(TL_INF, TG_SRV, (_T("created group %s %s"), group->getObjTraceId(), grp->getObjTraceId()));
	}
	else
	{ // can't query the interface
	  // delete both group objects
		_TRACE(TL_WRN, TG_SRV, (_T("can't QueryInterface")));
		m_me->removeGroup(grp->getServerHandle(), TRUE);
		delete group;
	}		

	_TRACE(TL_DEB, TG_SRV, (_T("< createOPCGroup (0x%X)"), res));
	return res;
} // createOPCGroup


//-----------------------------------------------------------------------------
// IOPCCommon                                                                 |
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SetLocaleID (interface IOPCCommon)
//
// - set default locale id of server
//
// returns:
//		S_OK          - set locale id
//		E_INVALIDARG  - invalid locale id for this server
//
STDMETHODIMP OPCServer::SetLocaleID(
	IN LCID dwLcid)	// LCID
{
	HRESULT res;
	_TRACE(TL_DEB, TG_SRV, (_T("> SetLocaleID 0x%X"), dwLcid));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);

	if (m_me->setLCID(dwLcid))
		res = S_OK;
	else
		res = E_INVALIDARG;

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCCommon::SetLocaleID: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_SRV, (_T("< SetLocaleID (0x%X)"), res));
	return res;
} // SetLocaleID
        

//-----------------------------------------------------------------------------
// GetLocaleID (interface IOPCCommon)
//
// - get default locale id of server
//
// returns:
//		S_OK          - got locale id
//		E_INVALIDARG  - invalid output parameter
//
STDMETHODIMP OPCServer::GetLocaleID(
	OUT LCID *pdwLcid)	// LCID
{
	_TRACE(TL_DEB, TG_SRV, (_T("> GetLocaleID")));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);

	if (pdwLcid == NULL)
	{
		_WARNING(TG_SRV, OPCENGINE_WRN_IF_PRM,(_T("GetLocaleID: bad output parameter")));
		return E_INVALIDARG;
	}

	*pdwLcid = m_me->getLCID();
	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCCommon::GetLocaleID: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_SRV, (_T("< GetLocaleID 0x%X"), *pdwLcid));
	return S_OK;
} // GetLocaleID
        

//-----------------------------------------------------------------------------
// QueryAvailableLocaleIDs (interface IOPCCommon)
//
// - get all available locale ids
//
// returns:
//		S_OK          - got all ids
//		E_INVALIDARG  - invalid output parameter
//		E_OUTOFMEMORY - out of memory
//
STDMETHODIMP OPCServer::QueryAvailableLocaleIDs(
	OUT DWORD *pdwCount,	// number of available locale ids
	OUT LCID **pdwLcid)		// list of the available locale ids
{
	LCID *lcidArray;
	_TRACE(TL_DEB, TG_SRV, (_T("> QueryAvailableLocaleIDs")));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);

	if ((pdwCount == NULL) || (pdwLcid == NULL))
	{
		_WARNING(TG_SRV, OPCENGINE_WRN_IF_PRM,(_T("GetLocaleID: bad output parameter")));
		return E_INVALIDARG;
	}

	*pdwCount = m_me->queryAvailableLCIDs(&lcidArray);
	if (*pdwCount)
	{
		*pdwLcid = (LCID *)CoTaskMemAlloc((*pdwCount) * sizeof(LCID));
		if (*pdwLcid)	
			memcpy(*pdwLcid, lcidArray, (*pdwCount) * sizeof(LCID));
		else
			return E_OUTOFMEMORY;

		free(lcidArray);
	}

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCCommon::QueryAvailableLocaleIDs: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_SRV, (_T("< QueryAvailableLocaleIDs cnt:%lu"), *pdwCount));
	return S_OK;
} // QueryAvailableLocaleIDs
        

//-----------------------------------------------------------------------------
// GetErrorString (interface IOPCCommon)
//
// - get error string for an error code
//
// returns:
//		S_OK          - got error string
//		E_INVALIDARG  - no string for the error code
//		E_OUTOFMEMORY - can't allocate memory
//
STDMETHODIMP OPCServer::GetErrorString(
	IN HRESULT dwError,		// error code
	OUT LPWSTR *ppString)	// error text
{
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
    HRESULT res = getErrorString(dwError, FALSE, 0, ppString);
	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCCommon::GetErrorString: %s"), _TRACE_ELAPSED_TIME));
	return res;
} // GetErrorString


//-----------------------------------------------------------------------------
// getErrorString
//
// - get error string for an error code
//
// returns:
//		S_OK          - got error string
//		E_INVALIDARG  - no string for the error code
//		E_OUTOFMEMORY - can't allocate memory
//
HRESULT OPCServer::getErrorString(
	IN HRESULT dwError,		// error code
	IN BOOL useLocale,		// use locale id
	IN LCID dwLocale,		// locale for string
	OUT LPWSTR *ppString)	// error text
{
	_TRACE(TL_DEB, TG_SRV, (_T("> GetErrorString 0x%X"), dwError));
	USES_CONVERSION;
	HRESULT res;
	CString message;

	// parameter check
	if (ppString == NULL)
	{
		_WARNING(TG_SRV, OPCENGINE_WRN_IF_PRM,(_T("GetErrorString: bad output parameter")));
		return E_INVALIDARG;
	}
	*ppString = NULL;

	// get error string
	if (useLocale)
		res = m_me->getErrorString(dwError, dwLocale, message);
	else
		res = m_me->getErrorString(dwError, message);

	// copy error string
	if (SUCCEEDED(res))
	{
		*ppString = (LPWSTR)CoTaskMemAlloc(2+message.GetLength()*2);
		if (*ppString)	
			wcscpy(*ppString, T2COLE((LPCTSTR)message));
		else
			res = E_OUTOFMEMORY;
	}

	_TRACE(TL_DEB, TG_SRV, (_T("< GetErrorString (0x%X)"), res));
    return res;
} // getErrorString
      
  
//-----------------------------------------------------------------------------
// SetClientName
//
// - set the name of the client
//
// returns:
//		S_OK 
//
STDMETHODIMP OPCServer::SetClientName(
	IN LPCWSTR szName)	// client name
{
	USES_CONVERSION;
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
	CString name(W2CT(szName));

	_TRACE(TL_DEB, TG_SRV, (_T("> SetClientName %s"), name));
	m_me->setClientName(name);
	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCCommon::SetClientName: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_SRV, (_T("< SetClientName")));
	return S_OK;
} // SetClientName


//-----------------------------------------------------------------------------
// IOPCServer                                                                 |
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// AddGroup (interface IOPCServer)
//
// - adds a group to the server
// - creates tree and COM group object
// - set initialization parameter
//
// returns:
//		S_OK                  - added group
//		OPC_S_UNSUPPORTEDRATE - changed update rate (but added group)
//		E_NOINTERFACE         - object don't support requested interface
//		E_INVALIDARG          - bad arguments (NULL for manadatory out value 
//                              or bad name)
//		OPC_E_DUPLICATENAME   - group name not unique
//
STDMETHODIMP OPCServer::AddGroup(
	IN LPCWSTR szName,				// name of group
	IN BOOL bActive,				// activation state 
	IN DWORD dwRequestedUpdateRate,	// requested update rate
	IN OPCHANDLE hClientGroup,		// client handle
	IN LONG *pTimeBias,				// time zone
	IN FLOAT *pPercentDeadband,		// deadband
	IN DWORD dwLCID,				// LCID
	OUT OPCHANDLE *phServerGroup,	// server handle
	OUT DWORD *pRevisedUpdateRate,	// revised update rate
	IN REFIID riid,					// id of requested interface
	OUT LPUNKNOWN * ppUnk)			// pointer to requested interface
{
	USES_CONVERSION;
	CString name(OLE2CT(szName));
	_TRACE(TL_DEB, TG_SRV, (_T("> AddGroup %s"), name));
	HRESULT res;
	GenericPointer<GenericGroup> grp;
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);

	// check parameters
	if ( (phServerGroup == NULL) ||
		 (ppUnk == NULL) )
	{
		_WARNING(TG_SRV, OPCENGINE_WRN_IF_PRM,(_T("AddGroup: bad output parameter")));
		return E_INVALIDARG;
	}
	*ppUnk = NULL;

	// create tree group object
	res = m_me->addGroup(name, bActive, dwRequestedUpdateRate, hClientGroup,
				pTimeBias, pPercentDeadband, dwLCID, (GenericGroup **)grp, pRevisedUpdateRate);

	if (SUCCEEDED(res))
	{
		// create COM group object
		if (SUCCEEDED(res = createOPCGroup(grp, riid, ppUnk)))
			*phServerGroup = grp->getServerHandle();
	}

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCServer::AddGroup: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_SRV, (_T("< AddGroup (0x%X)"), res));
	return res;
} // AddGroup


//-----------------------------------------------------------------------------
// GetErrorString (interface IOPCServer)
//
// - get error string for an error code
//
// returns:
//		S_OK          - got error string
//		E_INVALIDARG  - no string for the error code
//		E_OUTOFMEMORY - can't allocate memory
//
STDMETHODIMP OPCServer::GetErrorString(
	IN HRESULT dwError,		// error code 
	IN LCID dwLocale,		// locale for string
	OUT LPWSTR *ppString)	// error string
{
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
    HRESULT res = getErrorString(dwError, TRUE, dwLocale, ppString);
	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCServer::GetErrorString: %s"), _TRACE_ELAPSED_TIME));
	return res;
} // GetErrorString


//-----------------------------------------------------------------------------
// GetGroupByName (interface IOPCServer)
//
// - get group interface by the name of the group
//
// returns:
//		S_OK          - interface return
//		E_NOINTERFACE - object don't support requested interface
//		E_INVALIDARG  - no group with the name
//
STDMETHODIMP OPCServer::GetGroupByName(
	IN LPCWSTR szName,		// group name
	IN REFIID riid,			// interface id
	OUT LPUNKNOWN *ppUnk)	// interface pointer
{
	USES_CONVERSION;
	CString name(OLE2CT(szName));
	_TRACE(TL_DEB, TG_SRV, (_T("> GetGroupByName %s"), name));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
	HRESULT res;

	// parameter check
	if (ppUnk == NULL)
	{
		_WARNING(TG_SRV, OPCENGINE_WRN_IF_PRM,(_T("GetGroupByName: bad output parameter")));
		return E_INVALIDARG;
	}
	*ppUnk = NULL;

	GenericPointer<GenericGroup> grp;
	grp = m_me->getGroupByName(name);
	if (!grp)
		return E_INVALIDARG;

	OPCGroupObj *group = (OPCGroupObj *)grp->m_serverHandle;
	res = group->QueryInterface(riid, (void **)ppUnk);
	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCServer::GetGroupByName: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_SRV, (_T("< GetGroupByName %s (0x%X)"), group->getObjTraceId(), res));
	return res; 
} // GetGroupByName


//-----------------------------------------------------------------------------
// GetStatus (interface IOPCServer)
//
// - gets the server status
//
// returns:
//		S_OK          - return status sturcture
//		E_OUTOFMEMORY - can't allocate memory
//
STDMETHODIMP OPCServer::GetStatus(
	OUT OPCSERVERSTATUS **ppServerStatus)	// server status
{
	_TRACE(TL_DEB, TG_SRV, (_T("> GetStatus")));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
	HRESULT res;

	if (ppServerStatus == NULL)	
	{
		_WARNING(TG_SRV, OPCENGINE_WRN_IF_PRM,(_T("GetStatus: bad output parameter")));
		return E_INVALIDARG;
	}

	// allocate server sturcture
    *ppServerStatus = (OPCSERVERSTATUS*)CoTaskMemAlloc(sizeof(OPCSERVERSTATUS));
    if( *ppServerStatus == NULL ) 
	{
		_ERROR(TG_SRV, OPCENGINE_ERR_MEMORY,(_T("GetStatus: Out of memory")));
        return E_OUTOFMEMORY;
    }

	// fill structure
	res = m_me->getStatus(*ppServerStatus);
	if (FAILED(res))
	{
		LocalFree(*ppServerStatus);
		*ppServerStatus = NULL;
	}

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCServer::GetStatus: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_SRV, (_T("< GetStatus (0x%X)"), res));
	return res;
} // GetStatus


//-----------------------------------------------------------------------------
// RemoveGroup (interface IOPCServer)
//
// - removes a group from the server
//
// returns:
//		S_OK         - removed
//		OPC_S_INUSE  - group is in use by others
//		E_INVALIDARG - bad server handle
//
STDMETHODIMP OPCServer::RemoveGroup(
	IN OPCHANDLE hServerGroup,	// server handle
	IN BOOL bForce)				// force remove
{
	_TRACE(TL_DEB, TG_SRV, (_T("> RemoveGroup 0x%X"), hServerGroup));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
	HRESULT res;	

	res = m_me->removeGroup(hServerGroup);

	if (SUCCEEDED(res))
	{   // group found and removed from tree
		// -> hServerGroup is the pointer to the OPCGroup object
		OPCGroupObj *group = (OPCGroupObj *)hServerGroup;
		ULONG refCnt;		

		group->internalRelease(&refCnt);

		if (refCnt)		
		{
			_TRACE(TL_INF, TG_SRV, (_T("removed group %s in use"), group->getObjTraceId()));
			res = OPC_S_INUSE;

			if (bForce)
			{ // force removal
				_TRACE(TL_INF, TG_SRV, (_T("force removal of group %s; reference count %lu"), group->getObjTraceId(), refCnt));
				CoDisconnectObject((IOPCItemMgt *)group, 0);
			}
			else
			{ // remove server connection of COM group 
				group->releaseServer();
			}
		}	
		else
			res = S_OK;
	}

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCServer::RemoveGroup: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_SRV, (_T("< RemoveGroup (0x%X)"), res));
    return res;
} // RemoveGroup


//-----------------------------------------------------------------------------
// CreateGroupEnumerator (interface IOPCServer)
//
// - creates enumerator over group objects
//
// returns:
//		S_OK         - enumerator returned
//		S_FALSE      - empty enumerator returned 
//		E_INVALIDARG - bad argument
//
STDMETHODIMP OPCServer::CreateGroupEnumerator(
	IN OPCENUMSCOPE dwScope,	// which groups
	IN REFIID riid,				// which enumerator
	OUT LPUNKNOWN *ppUnk)		// enumerator
{
	_TRACE(TL_DEB, TG_SRV, (_T("> CreateGroupEnumerator")));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);

	// parameter check
	if (ppUnk == NULL)
	{
		_WARNING(TG_SRV, OPCENGINE_WRN_IF_PRM,(_T("CreateGroupEnumerator: bad output parameter")));
		return E_INVALIDARG;
	}
	*ppUnk = NULL;

	if ((riid != IID_IEnumString) && (riid != IID_IEnumUnknown))
	{
		_WARNING(TG_SRV, OPCENGINE_WRN_IF_PRM,(_T("CreateGroupEnumerator: requested wrong interface")));
		return E_NOINTERFACE;
	}

	// create the list
	COMUnknownEnumObject *unkList = NULL;
	COMStringEnumObject *strList = NULL;
	BOOL makeStrList = (riid == IID_IEnumString);
	HRESULT res = S_OK;
	
	if (makeStrList) 
	{
		_TRACE(TL_DEB, TG_SRV, (_T("string enumerator")));
		strList = new COMStringEnumObject;
	}
	else
	{
		_TRACE(TL_DEB, TG_SRV, (_T("object enumerator")));
		unkList = new COMUnknownEnumObject;
	}

	// public groups
	if ( (dwScope == OPC_ENUM_PUBLIC_CONNECTIONS) ||
		 (dwScope == OPC_ENUM_PUBLIC) ||
		 (dwScope == OPC_ENUM_ALL_CONNECTIONS) ||
		 (dwScope == OPC_ENUM_ALL) )
	{
		GenericPointer<OPCEngine> engine = ::getOPCEngine();
		GenericPointer<GenericServer> pgserver = engine->getPublicGroups();

		if ((GenericServer *)pgserver)
		{
		 	GenericList<GenericBranch> groupList(pgserver->getBranchList());
			GenericGroup *grp;
			POSITION pos;
			IUnknown *unknown;

			// for all groups of the server
			pos = groupList.getStartPosition();
			while(pos)
			{
				grp = (GenericGroup *)groupList.getNext(pos);
			
				_TRACE(TL_DEB, TG_SRV, (_T("public group object: %s"), grp->getObjTraceId()));
				if (makeStrList)
				{
					strList->m_list.AddTail(grp->getName());
				}
				else
				{
					USES_CONVERSION;
					if (SUCCEEDED(GetPublicGroupByName(T2COLE(grp->getName()), IID_IUnknown, &unknown)))
						unkList->m_list.AddTail(unknown);
				}
			}
		}
	}

	// private groups
	if ( (dwScope == OPC_ENUM_PRIVATE_CONNECTIONS) ||
		 (dwScope == OPC_ENUM_PRIVATE) ||
		 (dwScope == OPC_ENUM_ALL_CONNECTIONS) ||
		 (dwScope == OPC_ENUM_ALL) )
	{
	 	GenericList<GenericBranch> groupList(m_me->getBranchList());
		GenericGroup *grp;
		OPCGroupObj *group;
		POSITION pos;
		IUnknown *unknown;

		// for all groups of the server
		pos = groupList.getStartPosition();
		while(pos)
		{
			grp = (GenericGroup *)groupList.getNext(pos);

			if (!grp->isPublicGroup())		
			{
				_TRACE(TL_DEB, TG_SRV, (_T("private group object: %s"), grp->getObjTraceId()));
				if (makeStrList)
				{
					strList->m_list.AddTail(grp->getName());
				}
				else
				{
					group = (OPCGroupObj *)grp->m_serverHandle;
					group->QueryInterface(IID_IUnknown, (void **)&unknown);
					unkList->m_list.AddTail(unknown);
				}				
			}
		}
	}

	// reset list 
	if (makeStrList)
	{
		if (strList->m_list.IsEmpty())
		{
			res = S_FALSE;	// no list entry -> return OK but nothing found
			delete strList;	// free list enumerator
			*ppUnk = NULL;			
		}
		else
		{
			_TRACE(TL_INF, TG_SRV, (_T("created string enumerator with %d groups"), strList->m_list.GetCount()));
			strList->Reset();
			strList->QueryInterface(IID_IEnumString, (void **)ppUnk);
		}
	}
	else
	{
		if (unkList->m_list.IsEmpty())
		{
			res = S_FALSE;	// no list entry -> return OK but nothing found
			delete unkList;	// free list enumerator
			*ppUnk = NULL;			
		}
		else
		{
			_TRACE(TL_INF, TG_SRV, (_T("created object enumerator with %d groups"), unkList->m_list.GetCount()));
			unkList->Reset();
			unkList->QueryInterface(IID_IEnumUnknown, (void **)ppUnk);
		}
	}

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCServer::CreateGroupEnumerator: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_SRV, (_T("< CreateGroupEnumerator (0x%X)"), res));
    return res;
} // CreateGroupEnumerator



//-----------------------------------------------------------------------------
// IOPCServerPublicGroups                                                     |
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// GetPublicGroupByName (interface IOPCServerPublicGroups)
//
// - get public group interface by the name of the group
//
// returns:
//		S_OK           - interface return
//		OPC_E_NOTFOUND - no public groups server or group not found
//		E_NOINTERFACE  - object don't support requested interface
//		E_INVALIDARG   - invalid output buffer
//
STDMETHODIMP OPCServer::GetPublicGroupByName(
	IN LPCWSTR szName,		// group name
	IN REFIID riid,			// interface id
	OUT LPUNKNOWN *ppUnk)	// interface pointer
{
	USES_CONVERSION;
	CString name(OLE2CT(szName));
	_TRACE(TL_DEB, TG_SRV, (_T("> GetPublicGroupByName %s"), name));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
	HRESULT res;

	// parameter check
	if (ppUnk == NULL)
	{
		_WARNING(TG_SRV, OPCENGINE_WRN_IF_PRM,(_T("GetPublicGroupByName: bad output parameter")));
		return E_INVALIDARG;
	}
	*ppUnk = NULL;

	// find existing instance of public group
	GenericPointer<GenericGroup> grp;

	grp = m_me->getGroupByName(name);
	if ((GenericGroup *)grp)
	{
		if (grp->isPublic())
		{
			OPCGroupObj *group = (OPCGroupObj *)grp->m_serverHandle;
			res = group->QueryInterface(riid, (void **)ppUnk);
			_TRACE(TL_DEB, TG_SRV, (_T("< GetPublicGroupByName %s (0x%X)"), grp->getObjTraceId(), res));
			return res;
		}
	}

	// no exisiting public group
	// -> lookup public group template
	GenericPointer<OPCEngine> engine = ::getOPCEngine();
	GenericPointer<GenericServer> pgserver = engine->getPublicGroups();

	if (!pgserver)
	{
		_TRACE(TL_INF, TG_SRV, (_T("no public group server")));
		return OPC_E_NOTFOUND;
	}

	grp = pgserver->getGroupByName(name);
	if (!grp)
	{
		_TRACE(TL_DEB, TG_SRV, (_T("< GetPublicGroupByName no public group")));
		return res = E_INVALIDARG;
	}

	// found public group template
	// -> clone template of group
	GenericPointer<GenericGroup> clone;

	res = grp->clone(m_me, name, (GenericGroup **)clone);
	
	if (SUCCEEDED(res))
	{
		res = createOPCGroup(clone, riid, ppUnk, FALSE);
	}

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCServerPublicGroups::GetPublicGroupByName: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_SRV, (_T("< GetPublicGroupByName %s (0x%X)"), grp->getObjTraceId(), res));
	return res;
} // GetPublicGroupByName

        
//-----------------------------------------------------------------------------
// RemovePublicGroup (interface IOPCServerPublicGroups)
//
// - removes a public group from the server
//
// returns:
//		S_OK         - removed
//		E_FAIL		 - group removal rejected or no public groups server
//		OPC_S_INUSE  - group not removed because it's in use by others
//		E_INVALIDARG - bad server handle
//
STDMETHODIMP OPCServer::RemovePublicGroup(
	IN OPCHANDLE hServerGroup,	// server handle
	IN BOOL bForce)				// force remove
{
	_TRACE(TL_DEB, TG_SRV, (_T("> RemovePublicGroup %X"), hServerGroup));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
	GenericPointer<OPCEngine> engine = ::getOPCEngine();
	GenericPointer<GenericServer> pgserver = engine->getPublicGroups();
	HRESULT res;	

	if (!pgserver)
	{
		_TRACE(TL_INF, TG_SRV, (_T("no public group server")));
		return E_FAIL;
	}

	res = pgserver->removeGroup(hServerGroup);

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCServerPublicGroups::RemovePublicGroup: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_SRV, (_T("< RemovePublicGroup (0x%X)"), res));
    return res;
} // RemovePublicGroup


//-----------------------------------------------------------------------------
// IOPCItemProperties                                                         |
//-----------------------------------------------------------------------------

HRESULT OPCServer::getStandardProperties(
	IN VARTYPE tagNativeDatatype,
	OUT TagPropertyDataList &propertyList)
{
	OPCTagPropertyData *propData;

	propData = new OPCTagPropertyData;
	propData->m_pid = 1; propData->m_datatype = VT_I2; propData->m_descr = _tcsdup(_T("Item Canonical DataType"));
	propertyList.AddTail(propData);

	propData = new OPCTagPropertyData;
	propData->m_pid = 2; propData->m_datatype = tagNativeDatatype; propData->m_descr = _tcsdup(_T("Item Value"));
	propertyList.AddTail(propData);

	propData = new OPCTagPropertyData;
	propData->m_pid = 3; propData->m_datatype = VT_I2; propData->m_descr = _tcsdup(_T("Item Quality"));
	propertyList.AddTail(propData);

	propData = new OPCTagPropertyData;
	propData->m_pid = 4; propData->m_datatype = VT_DATE; propData->m_descr = _tcsdup(_T("Item Timestamp"));
	propertyList.AddTail(propData);

	propData = new OPCTagPropertyData;
	propData->m_pid = 5; propData->m_datatype = VT_I4; propData->m_descr = _tcsdup(_T("Item Access Rights"));
	propertyList.AddTail(propData);

	propData = new OPCTagPropertyData;
	propData->m_pid = 6; propData->m_datatype = VT_R4; propData->m_descr = _tcsdup(_T("Server Scan Rate"));
	propertyList.AddTail(propData);

	propData = new OPCTagPropertyData;
	propData->m_pid = 108; propData->m_datatype = VT_I4; propData->m_descr = _tcsdup(_T("Item Timezone"));
	propertyList.AddTail(propData);

	return S_OK;
}

HRESULT OPCServer::isTagProperty(
	IN LPCTSTR itemId,
	IN GenericTag *tag,
	IN DWORD propId)
{
	if (m_me->isStandardTagProperty(propId))
		return S_OK;	// standard property

	if (tag)
	{
		GenericPointer<GenericTagProperty> prop;
		GenericList<GenericTagProperty> propList;

		propList = tag->getPropertyList();
		prop = propList.find(propId);
		if ((GenericTagProperty *)prop)
			return S_OK;
	}

	if (m_me->isDynamicTagProperty(itemId, propId))
		return S_OK;	// dynamic property

	return OPC_E_INVALID_PID;
}

STDMETHODIMP OPCServer::QueryAvailableProperties(
	IN LPWSTR szItemID,				// tag path 
	OUT DWORD *pdwCount,			// number of available properties
	OUT DWORD **ppPropertyIDs,		// property ids
	OUT LPWSTR **ppDescriptions,	// property descriptions
	OUT VARTYPE **ppvtDataTypes)	// property datatypes
{
	HRESULT res = S_OK;
	USES_CONVERSION;
	CString itemId(W2CT(szItemID));
	_TRACE(TL_DEB, TG_SRV, (_T("> QueryAvailableProperties %s"), itemId));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);

	// check parameters
	if ((pdwCount == NULL) || (ppPropertyIDs == NULL) || 
		(ppDescriptions == NULL) || (ppvtDataTypes == NULL))
	{
		_WARNING(TG_SRV, OPCENGINE_WRN_IF_PRM,(_T("QueryAvailableProperties: bad output parameter")));
		return E_INVALIDARG;
	}	
	*pdwCount = 0;
	*ppPropertyIDs = NULL;
	*ppDescriptions = NULL;
	*ppvtDataTypes = NULL;

	// check for vaild item id
	GenericPointer<GenericTag> tag = m_me->getTag(itemId);
	if ((!tag) && (!m_me->isDynamicTag(itemId)))
	{
		_TRACE(TL_DEB, TG_SRV, (_T("< QueryAvailableProperties invalid item id")));
		return OPC_E_UNKNOWNITEMID;
	}

	TagPropertyDataList statList;
	TagPropertyDataList dynList;
	TagPropertyDataList defList;
	TagPropertyDataList *propList = NULL;
	TagPropertyDataList *actList = NULL;
	POSITION pos;
	OPCTagPropertyData *propData;
	BOOL mustCompare;
	POSITION posS;
	OPCTagPropertyData *propDataS;
	BOOL addToList;
	int j;

	for (j = 0; ((j < 3) && (SUCCEEDED(res))); j++)
	{
		mustCompare = (propList != NULL);
		actList = NULL;

		if (j == 0)
		{ // get static properties
			if ((GenericTag *)tag)
			{
				if (SUCCEEDED(res = m_me->queryAvailableProperties(tag, statList)))
				{
					if (statList.GetCount())
						actList = &statList;
				}
			}
		}

		if (j == 1)
		{ // get dynamic properties
			if (SUCCEEDED(res = m_me->queryAvailableDynamicProperties(itemId, dynList)))
			{
				if (dynList.GetCount())
					actList = &dynList;
			}
		}

		if (j == 2)
		{ // standard properties
			VARTYPE datatype;
			if ((GenericTag *)tag)
				datatype = tag->getNativeDatatype();
			else
				datatype = m_me->getDynamicTagDatatype(itemId);

			if (SUCCEEDED(res = getStandardProperties(datatype, defList)))
			{
				if (defList.GetCount())
					actList = &defList;
			}
		}

		if (actList)
		{
			if (mustCompare)
			{
				// make one list out of the two and remove duplicates
				pos = actList->GetHeadPosition();
				while (pos)
				{
					propData = actList->GetNext(pos);
					addToList = TRUE;
					posS = propList->GetHeadPosition();
					while (posS)
					{
						propDataS = propList->GetNext(posS);
						if (propData->m_pid == propDataS->m_pid)
						{
							addToList = FALSE;
							break;
						}
					}

					if (addToList)
						propList->AddTail(propData);
					else
					{
						if (propData->m_descr) delete propData->m_descr;
						delete propData;
					}
				}

				actList->RemoveAll();	
			} 
			else
				propList = actList;
		}
	}

	// copy property data to out buffers	
	if (SUCCEEDED(res) && (propList))
	{
		DWORD i;

		res = S_OK;
		*pdwCount = propList->GetCount();
        *ppPropertyIDs = (DWORD *)::CoTaskMemAlloc(*pdwCount * sizeof(DWORD));
        *ppDescriptions = (LPWSTR *)::CoTaskMemAlloc(*pdwCount * sizeof(LPWSTR));
        *ppvtDataTypes = (VARTYPE *)::CoTaskMemAlloc(*pdwCount * sizeof(VARTYPE));

		if ((*ppPropertyIDs == NULL) || (*ppDescriptions == NULL) || (*ppvtDataTypes == NULL))
		{
			res = E_OUTOFMEMORY;
		}

		if (SUCCEEDED(res))
		{
			pos = propList->GetHeadPosition();

			for (i = 0; i < *pdwCount; i++)
			{
				propData = propList->GetNext(pos);

				(*ppDescriptions)[i] = (LPWSTR) ::CoTaskMemAlloc((_tcslen(propData->m_descr) + 1) * sizeof(WCHAR));
				if ((*ppDescriptions)[i] == NULL)
				{
					res = E_OUTOFMEMORY;
					break;
				}
	
				wcscpy((*ppDescriptions)[i], T2CW(propData->m_descr));
				(*ppPropertyIDs)[i] = propData->m_pid;
				(*ppvtDataTypes)[i] = propData->m_datatype;
			}
		}

		if (res == E_OUTOFMEMORY)
		{
			if (*ppPropertyIDs != NULL) 
			{
				CoTaskMemFree(*ppPropertyIDs); 
				*ppPropertyIDs = NULL;
			}
			if (*ppDescriptions != NULL)
			{
				for (i = 0; i < *pdwCount; i++)
				{
					if ((*ppDescriptions)[i])
						CoTaskMemFree((*ppDescriptions)[i]); 
				}
				CoTaskMemFree(*ppDescriptions); 
				*ppDescriptions = NULL;
			}
			if (ppvtDataTypes != NULL)
			{
				CoTaskMemFree(*ppvtDataTypes); 
				*ppvtDataTypes = NULL;
			}
			*pdwCount = 0;
			_ERROR(TG_SRV, OPCENGINE_ERR_MEMORY,(_T("QueryAvailableProperties: Out of memory")));
			return E_OUTOFMEMORY;
		}
	}

	// free memory
	if (propList)
	{
		pos = propList->GetHeadPosition();
		while (pos)
		{
			propData = propList->GetNext(pos);
			if (propData->m_descr) delete propData->m_descr;
			delete propData;
		}
	}

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCItemProperties::QueryAvailableProperties: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_SRV, (_T("< QueryAvailableProperties cnt:%lu (0x%X)"), *pdwCount, res));
	return res;
} // QueryAvailableProperties

        
STDMETHODIMP OPCServer::GetItemProperties(
	IN LPWSTR szItemID,			// tag path
	IN DWORD dwCount,			// number of properties
	IN DWORD *pdwPropertyIDs,	// property ids
	OUT VARIANT **ppvData,		// property data
	OUT HRESULT **ppErrors)		// read errors
{
	USES_CONVERSION;
	HRESULT res = S_OK;
	CString itemId(W2CT(szItemID));
	_TRACE(TL_DEB, TG_SRV, (_T("> GetItemProperties %s"), itemId));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);

	// check parameters
	if ((dwCount == 0) || (pdwPropertyIDs == NULL))
	{
		_WARNING(TG_SRV, OPCENGINE_WRN_IF_PRM,(_T("GetItemProperties: bad input parameter")));
		return E_INVALIDARG;
	}	
	if ((ppvData == NULL) || (ppErrors == NULL))
	{
		_WARNING(TG_SRV, OPCENGINE_WRN_IF_PRM,(_T("GetItemProperties: bad output parameter")));
		return E_INVALIDARG;
	}	
	*ppvData = NULL;
	*ppErrors = NULL;

	// create tag object
	GenericPointer<GenericTag> tag = m_me->getTag(itemId);
	if (!tag)
	{
		_TRACE(TL_DEB, TG_SRV, (_T("%s is no static tag"), itemId));
		tag = m_me->createTag(itemId);

		if (!tag)
		{
			_TRACE(TL_DEB, TG_SRV, (_T("< GetItemProperties invalid item id")));
			return OPC_E_UNKNOWNITEMID;
		}
		tag->setDynamic(TRUE);
	}

	// create return data
	*ppvData = (LPVARIANT) CoTaskMemAlloc(dwCount * sizeof(VARIANT));
	*ppErrors = (HRESULT*) CoTaskMemAlloc(dwCount * sizeof(HRESULT));

	if ((*ppvData == NULL) || (*ppErrors == NULL))	
	{
		res = E_OUTOFMEMORY;
	}

	RequestList reqList;
	GenericPointer<OPCEngine> engine = ::getOPCEngine();
	GenericPointer<GenericCreator> creator = engine->getCreator();
	GenericPointer<GenericTagProperty> prop;
	GenericRequest *req;
	DWORD i;

	// for every property id
	for (i = 0; ((SUCCEEDED(res)) && (i < dwCount)); i++)
	{
		prop = m_me->createTagProperty(tag, itemId, pdwPropertyIDs[i]);
		::VariantInit((*ppvData)+i);
		if ((GenericTagProperty *)prop)
		{
			// sync property requests
			(*ppErrors)[i] = S_OK;
			req = creator->createRequest(m_me, NULL, prop, GenericRequest::read, GenericRequest::syncProperty, (GenericRequest::requestDeviceIOMode)prop->getDeviceIOMode());
			req->setItemTag(prop);
			req->setResultAddress((*ppErrors)+i);
			req->setResult(E_FAIL);
			req->setReadValueAddress((*ppvData)+i);
			reqList.AddTail((GenericRequest *)req);
		}
		else
		{
			(*ppErrors)[i] = OPC_E_INVALID_PID;
		}
	}

	if (SUCCEEDED(res))
	{
		POSITION pos;

		if (!reqList.IsEmpty())
		{
			// perform read
			res = m_me->syncRead(reqList, OPC_DS_DEVICE, _T(""));

			// free request list
			pos = reqList.GetHeadPosition();
			while (pos)
			{
				req = (GenericRequest *)reqList.GetNext(pos);
				req->release();
			}	
			reqList.RemoveAll();
		}
	}

	if (FAILED(res))
	{
		if (*ppvData)
		{
			CoTaskMemFree(*ppvData);		
			*ppvData = NULL;
		}
		if (*ppErrors)
		{
			CoTaskMemFree(*ppErrors);		
			*ppErrors = NULL;
		}
		if (res == E_OUTOFMEMORY)
			_ERROR(TG_SRV, OPCENGINE_ERR_MEMORY,(_T("GetItemProperties: Out of memory")));
	}	

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCItemProperties::GetItemProperties: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_SRV, (_T("< GetItemProperties (0x%X)"), res));
	return res;
} // GetItemProperties
        

STDMETHODIMP OPCServer::LookupItemIDs(
	IN LPWSTR szItemID,				// tag path
	IN DWORD dwCount,				// number of properties
	IN DWORD *pdwPropertyIDs,		// property ids
	OUT LPWSTR **ppszNewItemIDs,	// item ids of properties
	OUT HRESULT **ppErrors)			// errors
{
	USES_CONVERSION;
	HRESULT res = S_OK;
	GenericPointer<OPCEngine> engine;
	CString itemId(W2CT(szItemID));
	CString propPath;
	TCHAR propDelim;
	DWORD i;
	_TRACE(TL_DEB, TG_SRV, (_T("> LookupItemIDs %s"), itemId));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);

	// check parameters
	if ((dwCount == 0) || (pdwPropertyIDs == NULL))
	{
		_WARNING(TG_SRV, OPCENGINE_WRN_IF_PRM,(_T("LookupItemIDs: bad input parameter")));
		return E_INVALIDARG;
	}	
	if ((ppszNewItemIDs == NULL) || (ppErrors == NULL))
	{
		_WARNING(TG_SRV, OPCENGINE_WRN_IF_PRM,(_T("LookupItemIDs: bad output parameter")));
		return E_INVALIDARG;
	}	
	*ppszNewItemIDs = NULL;
	*ppErrors = NULL;

	// check for vaild item id
	GenericPointer<GenericTag> tag = m_me->getTag(itemId);
	if ((!tag) && (!m_me->isDynamicTag(itemId)))
	{
		_TRACE(TL_DEB, TG_SRV, (_T("< LookupItemIDs invalid item id")));
		return OPC_E_UNKNOWNITEMID;
	}

	// create return data
	*ppszNewItemIDs = (LPWSTR*) CoTaskMemAlloc(dwCount * sizeof(LPWSTR));
	*ppErrors = (HRESULT*) CoTaskMemAlloc(dwCount * sizeof(HRESULT));

	if ((*ppszNewItemIDs == NULL) || (*ppErrors == NULL))	
	{
		res = E_OUTOFMEMORY;
	}
	
	engine = ::getOPCEngine();
	propDelim = engine->getPropertyDelimiter();
	// for every property id
	for (i = 0; ((SUCCEEDED(res)) && (i < dwCount)); i++)
	{
		if (SUCCEEDED((*ppErrors)[i] = isTagProperty(itemId, (GenericTag *)tag, pdwPropertyIDs[i])))
		{
			propPath.Format(_T("%s%c%lu"), itemId, propDelim, pdwPropertyIDs[i]); 
			(*ppszNewItemIDs)[i] = (LPWSTR) CoTaskMemAlloc((propPath.GetLength() + 1) * sizeof(WCHAR));
			wcscpy((*ppszNewItemIDs)[i], T2CW(propPath));
		}
	}

	if (res == E_OUTOFMEMORY)
	{
		if (*ppszNewItemIDs)
		{
			CoTaskMemFree(*ppszNewItemIDs);		
			*ppszNewItemIDs = NULL;
		}
		if (*ppErrors)
		{
			CoTaskMemFree(*ppErrors);		
			*ppErrors = NULL;
		}
		_ERROR(TG_SRV, OPCENGINE_ERR_MEMORY,(_T("LookupItemIDs: Out of memory")));
	}	

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCItemProperties::LookupItemIDs: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_SRV, (_T("< LookupItemIDs (0x%X)"), res));
	return res;
} // LookupItemIDs


//-----------------------------------------------------------------------------
// IOPCBrowseServerAddressSpace                                               |
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// QueryOrganization (interface IOPCBrowseServerAddressSpace)
//
// - checks the organisation type of the namespace
//
// returns:
//		S_OK         - returned type
//		E_INVALIDARG - bad arguments
//
STDMETHODIMP OPCServer::QueryOrganization(
	OUT OPCNAMESPACETYPE *pNameSpaceType)
{
	_TRACE(TL_DEB, TG_SRV, (_T("> QueryOrganization")));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
	HRESULT res;

	if (!pNameSpaceType)
	{
		_WARNING(TG_SRV, OPCENGINE_WRN_IF_PRM,(_T("QueryOrganization: bad output parameter")));
		return E_INVALIDARG;
	}

	res = m_me->queryOrganization(pNameSpaceType);
	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCBrowseServerAddressSpace::QueryOrganization: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_SRV, (_T("< QueryOrganization %d (0x%X)"), *pNameSpaceType, res));
	return res;
} // QueryOrganization

        
//-----------------------------------------------------------------------------
// ChangeBrowsePosition (interface IOPCBrowseServerAddressSpace)
//
// - changes the current position in the namespace 
//
// returns:
//		S_OK         - position changed
//		E_INVALIDARG - bad name or direction
//
STDMETHODIMP OPCServer::ChangeBrowsePosition(
	IN OPCBROWSEDIRECTION dwBrowseDirection,	// direction
	IN LPCWSTR szString)						// down node name
{
	HRESULT res;
	USES_CONVERSION;
	_TRACE(TL_DEB, TG_SRV, (_T("> ChangeBrowsePosition")));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);

	// check parameters
	if ( (dwBrowseDirection != OPC_BROWSE_UP) &&
		 (dwBrowseDirection != OPC_BROWSE_TO) &&	
		 (dwBrowseDirection != OPC_BROWSE_DOWN) )
	{
		_WARNING(TG_SRV, OPCENGINE_WRN_IF_PRM,(_T("ChangeBrowsePosition: invalid direction")));
		return E_INVALIDARG;
	}
	if ( (dwBrowseDirection == OPC_BROWSE_DOWN) &&
		 (wcslen(szString) == 0) )
	{
		_WARNING(TG_SRV, OPCENGINE_WRN_IF_PRM,(_T("ChangeBrowsePosition: no down branch")));
		return E_INVALIDARG;
	}

	if (((GenericBranch *)m_me->m_curBrowse) || (dwBrowseDirection == OPC_BROWSE_TO))
	{ // current browse position is in static namespace
		res = m_me->changeBrowsePosition(dwBrowseDirection, OLE2CT(szString));

		if (FAILED(res) && (dwBrowseDirection != OPC_BROWSE_UP))
		{ // can't change to new position and direction is not up
		  // -> may change to a dynamic node
			res = m_me->changeDynamicBrowsePosition(dwBrowseDirection, OLE2CT(szString));
		}
	}
	else
	{ // current browse position is in dynamic namespace
		res = m_me->changeDynamicBrowsePosition(dwBrowseDirection, OLE2CT(szString));
	}

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCBrowseServerAddressSpace::QueryOrganization: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_SRV, (_T("< ChangeBrowsePosition (0x%X)"), res));
	return res; 
} // ChangeBrowsePosition

        
//-----------------------------------------------------------------------------
// BrowseOPCItemIDs (interface IOPCBrowseServerAddressSpace)
//
// - browses the current branch for elements
//
// returns:
//		S_OK         - string enumerator returned
//		S_FALSE      - empty string enumerator returned 
//		E_INVALIDARG - bad argument
//
STDMETHODIMP OPCServer::BrowseOPCItemIDs(
	IN OPCBROWSETYPE dwBrowseFilterType,
	IN LPCWSTR szFilterCriteria, 
	IN VARTYPE vtDataTypeFilter,
	IN DWORD dwAccessRightsFilter, 
	OUT LPENUMSTRING *ppIEnumString)
{
	HRESULT res;
	CStringList dynList;
	USES_CONVERSION;
	_TRACE(TL_DEB, TG_SRV, (_T("> BrowseOPCItemIDs")));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);

	if (ppIEnumString == NULL)
	{
		_WARNING(TG_SRV, OPCENGINE_WRN_IF_PRM,(_T("BrowseOPCItemIDs: bad output parameter")));
		return E_INVALIDARG;
	}	
	*ppIEnumString = NULL;

	if ( (dwBrowseFilterType != OPC_BRANCH) &&
		 (dwBrowseFilterType != OPC_LEAF) &&
		 (dwBrowseFilterType != OPC_FLAT) )
	{
		_WARNING(TG_SRV, OPCENGINE_WRN_IF_PRM,(_T("BrowseOPCItemIDs: invalid browse filter")));
		return E_INVALIDARG;
	}

	COMStringEnumObject *itemNameList = new COMStringEnumObject;

	if ((GenericBranch *)m_me->m_curBrowse)
	{ // current browse position is in static namespace
		// get the static childs
		res = m_me->browseCurrentBranch(dwBrowseFilterType, OLE2CT(szFilterCriteria), 
				vtDataTypeFilter, dwAccessRightsFilter, itemNameList->m_list);

		if (SUCCEEDED(res))
		{
			// get the dynamic childs
			res = m_me->getDynamicChildNames(dwBrowseFilterType, 
				OLE2CT(szFilterCriteria), vtDataTypeFilter, 
				dwAccessRightsFilter, dynList);
		}
	}
	else
	{ // current browse position is in dynamic namespace
		// get only the dynamic childs
		res = m_me->getDynamicChildNames(dwBrowseFilterType, OLE2CT(szFilterCriteria), 
				vtDataTypeFilter, dwAccessRightsFilter, dynList);
	}

	if (SUCCEEDED(res))
	{
		if (!(dynList.IsEmpty()))
		{
			copyAppStringList(dynList, itemNameList->m_list);
		}

		// no list entry -> return OK but nothing found
		if (itemNameList->m_list.IsEmpty())
			res = S_FALSE;

		itemNameList->Reset();
		itemNameList->QueryInterface(IID_IEnumString, (void **)ppIEnumString);
	}
	else
	{
		delete itemNameList;	// free list enumerator
		*ppIEnumString = NULL;
	}

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCBrowseServerAddressSpace::BrowseOPCItemIDs: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_SRV, (_T("< BrowseOPCItemIDs (0x%X)"), res));
    return res;
} // BrowseOPCItemIDs

        
//-----------------------------------------------------------------------------
// GetItemID (interface IOPCBrowseServerAddressSpace)
//
// - get the full qualified path for a tag or node name
//
// returns:
//		S_OK          - returned fully qualified name
//		E_INVALIDARG  - return pointer was NULL
//		E_OUTOFMEMORY - error allocating buffer for return
//
STDMETHODIMP OPCServer::GetItemID(
	IN LPWSTR szItemDataID,	// element name	 
	OUT LPWSTR *szItemID)	// fully qualified name
{
	USES_CONVERSION;
	CString name(OLE2CT(szItemDataID));
	CString path;
	_TRACE(TL_DEB, TG_SRV, (_T("> GetItemID %s"), name));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);

    if (szItemID == NULL) 
	{
		_WARNING(TG_SRV, OPCENGINE_WRN_IF_PRM,(_T("GetItemID: bad output parameter")));
        return E_INVALIDARG;
	}
    *szItemID = NULL;

	// get the path and check if valid element
    if (m_me->getBrowseElementPath(name, path))
    { // item id is valid
		// copy path 
        *szItemID = (LPWSTR) ::CoTaskMemAlloc((path.GetLength() + 1) * sizeof(WCHAR));
        if (*szItemID == NULL) 
		{
			_ERROR(TG_SRV, OPCENGINE_ERR_MEMORY,(_T("GetItemID: Out of memory")));
            return E_OUTOFMEMORY;
        }
        ::wcscpy(*szItemID, T2COLE((LPCTSTR)path));
		_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
		_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCBrowseServerAddressSpace::GetItemID: %s"), _TRACE_ELAPSED_TIME));
		_TRACE(TL_DEB, TG_SRV, (_T("< GetItemID %s"), path));
        return S_OK;
    }
	
	_TRACE(TL_DEB, TG_SRV, (_T("< GetItemID invalid item")));
    return E_INVALIDARG;
} // GetItemID

        
//-----------------------------------------------------------------------------
// BrowseAccessPaths (interface IOPCBrowseServerAddressSpace)
//
// - get the access path for a fully qualified tag
//
// returns:
//		S_FALSE - no access path
//
STDMETHODIMP OPCServer::BrowseAccessPaths(
	IN LPCWSTR szItemID,				// fully qualified tag name
	OUT LPENUMSTRING *ppIEnumString)	// access path list enumerator

{
	GenericPointer<OPCEngine> engine = ::getOPCEngine();
	GenericPointer<OPCRootNS> root;
	GenericPointer<GenericTag> tag;
	HRESULT res;
	USES_CONVERSION;
	CString name(OLE2CT(szItemID));
	_TRACE(TL_DEB, TG_SRV, (_T("> BrowseAccessPaths %s"), name));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);

	if (ppIEnumString == NULL)
	{
		_WARNING(TG_SRV, OPCENGINE_WRN_IF_PRM,(_T("BrowseAccessPaths: bad output parameter")));
		return E_INVALIDARG;
	}
	*ppIEnumString = NULL;

	COMStringEnumObject *accessPathList = new COMStringEnumObject;

	// find namespace tag
	root = engine->getNamespaceRoot();
	tag = (GenericTag *)root->findLeaf(name, TRUE);
	if ((GenericTag *)tag)
	{
		tag->getAccessPaths(&accessPathList->m_list);
		res = S_OK;
	}
	else
	{
		if (m_me->isDynamicTag(name))
		{
			CStringList dynList;
			res = m_me->getDynamicBrowseAccessPaths(name, dynList);
			if ((SUCCEEDED(res)) && (!(dynList.IsEmpty())))
			{
				copyAppStringList(dynList, accessPathList->m_list);
			}
		}
		else
			res = E_INVALIDARG;
	}

	if (SUCCEEDED(res))
	{
		// no list entry -> return OK but nothing found
		if (accessPathList->m_list.IsEmpty())
		{
			res = S_FALSE;
			delete accessPathList;	// free list enumerator
			*ppIEnumString = NULL;
		}
		else
		{
			accessPathList->Reset();
			accessPathList->QueryInterface(IID_IEnumString, (void **)ppIEnumString);
		}
	}
	else
	{
		delete accessPathList;	// free list enumerator
		*ppIEnumString = NULL;
	}

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IOPCBrowseServerAddressSpace::BrowseAccessPaths: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_SRV, (_T("< BrowseAccessPaths (0x%X)"), res));
    return res;
} // BrowseAccessPaths


//-----------------------------------------------------------------------------
// copyAppStringList 
//
// - copies a string list provided by the application and let the application
//   free the strings
//
void OPCServer::copyAppStringList(
	IN CStringList &appList,		// application list
	OUT CStringList &engineList)	// engine list
{
	POSITION pos;
	CString listEntry;

	// copy the list
	pos = appList.GetHeadPosition();
	while(pos)
	{
		listEntry = appList.GetNext(pos);
		if (engineList.Find(listEntry) == NULL)
		{
			engineList.AddTail(listEntry);
		}
	}
} // copyAppStringList



//-----------------------------------------------------------------------------
// IPersistFile                                                               |
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// GetClassID (interface IPersist)
//
// - get the class id of the object
//
// returns:
//		S_OK         - returned class id
//		E_INVALIDARG - bad arguments
//
STDMETHODIMP OPCServer::GetClassID(
	OUT CLSID *pClassID)	// class id
{
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
	if (pClassID == NULL) 
	{
		_WARNING(TG_SRV, OPCENGINE_WRN_IF_PRM,(_T("GetClassID: bad output parameter")));
		return E_INVALIDARG;
	}

	::memcpy(pClassID, &g_clsid, sizeof(CLSID));
	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IPersist::GetClassID: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_INF, TG_SRV, (_T("GetClassID")));
    return S_OK;
} // GetClassID


//-----------------------------------------------------------------------------
// IsDirty (interface IPersistFile)
//
// - check if the configuration file has changed since last save
//
// returns:
//		S_OK    - configuration file has changed
//		S_FALSE - configuration file hasn't changed
//
STDMETHODIMP OPCServer::IsDirty(void)
{
	_TRACE(TL_DEB, TG_SRV, (_T("> IsDirty")));
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);
	GenericPointer<OPCEngine> engine = ::getOPCEngine();
	GenericPointer<GenericConfigFileHandler> cfg = engine->getConfigurationFileHandler();
	HRESULT res = S_FALSE;

	if ((GenericConfigFileHandler *)cfg)
	{
		// call configuration file handler
		if (cfg->isDirty(m_me))
			res = S_OK;	
		else
			res = S_FALSE;
	}

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IPersistFile::IsDirty: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_SRV, (_T("< IsDirty (0x%X)"), res));
	return res;
} // IsDirty

        
//-----------------------------------------------------------------------------
// Load (interface IPersistFile)
//
// - called to load a configuration file
//
// returns:
//		all allowed returns of IPersistFile::Load
//
STDMETHODIMP OPCServer::Load(
	IN LPCOLESTR pszFileName,	// file name
	IN DWORD dwMode)			// open mode
{
	USES_CONVERSION;
	HRESULT res = E_FAIL;	
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);

	// check output parameter
	if (pszFileName == NULL) 
	{
		_WARNING(TG_SRV, OPCENGINE_WRN_IF_PRM,(_T("Load: no file name")));
		return E_INVALIDARG;
	}

	GenericPointer<OPCEngine> engine = ::getOPCEngine();
	GenericPointer<GenericConfigFileHandler> cfg = engine->getConfigurationFileHandler();
	CString fileName(OLE2CT(pszFileName));
	_TRACE(TL_DEB, TG_SRV, (_T("> Load %s"), fileName));

	if ((GenericConfigFileHandler *)cfg)
	{
		// call configuration file handler
		res = cfg->load(m_me, fileName, dwMode);
	}

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IPersistFile::Load: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_SRV, (_T("< Load (0x%X)"), res));
	return res;
} // Load
        

//-----------------------------------------------------------------------------
// Save (interface IPersistFile)
//
// - called to store a configuration file
//
// returns:
//		all allowed returns of IPersistFile::Save
//
STDMETHODIMP OPCServer::Save(
	IN LPCOLESTR pszFileName,	// file name
	IN BOOL fRemember)			// set current file to the saved file?
{
	GenericPointer<OPCEngine> engine = ::getOPCEngine();
	GenericPointer<GenericConfigFileHandler> cfg = engine->getConfigurationFileHandler();
	CString fileName;
	LPCTSTR name = NULL;  
	HRESULT res = E_FAIL;	
	USES_CONVERSION;
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);

	if (pszFileName != NULL) 
	{
		fileName = OLE2CT(pszFileName);
		name = (LPCTSTR) fileName;
		_TRACE(TL_DEB, TG_SRV, (_T("> Save %s"), fileName));
	}
	else
		_TRACE(TL_DEB, TG_SRV, (_T("> Save")));

	if ((GenericConfigFileHandler *)cfg)
	{
		// call configuration file handler
		res = cfg->save(m_me, name, fRemember);
	}

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IPersistFile::Save: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_SRV, (_T("< Save (0x%X)"), res));
	return res;
} // Save
        
STDMETHODIMP OPCServer::SaveCompleted(IN LPCOLESTR pszFileName)
{
	_TRACE(TL_DEB, TG_SRV, (_T("SaveCompleted S_OK")));
	return S_OK;
}
        
STDMETHODIMP OPCServer::GetCurFile(OUT LPOLESTR *ppszFileName)
{
	_TRACE(TL_DEB, TG_SRV, (_T("> GetCurFile")));
	HRESULT res = E_FAIL;	
	CString fileName;
	_TRACE_START_TIME(TL_DEB, TG_TIMECOM);

	if (ppszFileName == NULL) 
	{
		_WARNING(TG_SRV, OPCENGINE_WRN_IF_PRM,(_T("GetCurFile: bad output parameter")));
		return E_INVALIDARG;
	}
	*ppszFileName = NULL;

	GenericPointer<OPCEngine> engine = ::getOPCEngine();
	GenericPointer<GenericConfigFileHandler> cfg = engine->getConfigurationFileHandler();

	if ((GenericConfigFileHandler *)cfg)
	{
		USES_CONVERSION;
		// call configuration file handler
		res = cfg->getCurFile(m_me, fileName);
		if (SUCCEEDED(res))
		{
			*ppszFileName = (LPWSTR) ::CoTaskMemAlloc((fileName.GetLength() + 1) * sizeof(WCHAR));
			::wcscpy(*ppszFileName, T2COLE(fileName));
		}
	}

	_TRACE_STOP_TIME(TL_DEB, TG_TIMECOM);
	_TRACE(TL_DEB, TG_TIMECOM, (_T("IPersistFile::GetCurFile: %s"), _TRACE_ELAPSED_TIME));
	_TRACE(TL_DEB, TG_SRV, (_T("< GetCurFile %s (0x%X)"), fileName, res));
	return res;
}


//-----------------------------------------------------------------------------
// IConnectionPoint                                                           |
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Advise (interface IConnectionPoint)
//
// - establishes a connection between the connection point object and 
//   the client's sink
//
// returns:
//		S_OK                    - the connection has been established 
//		E_POINTER               - a parameter value is not valid
//		CONNECT_E_ADVISELIMIT   - the connection point has already reached its
//                                limit of connections 
//		CONNECT_E_CANNOTCONNECT - the sink does not support the interface required 
//                                by this connection point
//
STDMETHODIMP OPCServer::Advise(
	IN IUnknown* unknown,	// client sink
	OUT DWORD* cookie)		// connection cookie
{
	m_advisesSync.Lock();
	HRESULT res = IConnectionPointImpl<OPCServer, &IID_IOPCShutdown, CComDynamicUnkArray>::Advise(unknown, cookie);
	m_advisesSync.Unlock();
	return res;
} // Advise


//-----------------------------------------------------------------------------
// Unadvise (interface IConnectionPoint)
//
// - terminates an advisory connection
//
// returns:
//		S_OK                   - The connection was successfully terminated
//		CONNECT_E_NOCONNECTION - the cookie does not represent a valid connection
//
STDMETHODIMP OPCServer::Unadvise(
	IN DWORD cookie)	// connection cookie
{
	m_advisesSync.Lock();
	HRESULT res = IConnectionPointImpl<OPCServer, &IID_IOPCShutdown, CComDynamicUnkArray>::Unadvise(cookie);
	m_advisesSync.Unlock();
	return res;
} // Unadvise



HRESULT OPCServer::sendShutdownRequest(
	IN LPCTSTR reason)
{
	USES_CONVERSION;
	IConnectionPointImpl<OPCServer, &IID_IOPCShutdown, CComDynamicUnkArray>* p = this;
	IUnknown** pp;
	IOPCShutdown* pIShutdown;
	HRESULT res = S_FALSE;

	pp = p->m_vec.begin();
	while (pp < p->m_vec.end())
	{
		if (*pp != NULL)
		{
			pIShutdown = (IOPCShutdown *)*pp;
			res = pIShutdown->ShutdownRequest(T2CW(reason));
		}
		pp++;
	}

	return res;
}

BOOL OPCServer::connectedShutdownInterface(void)
{
	m_advisesSync.Lock();
	IConnectionPointImpl<OPCServer, &IID_IOPCShutdown, CComDynamicUnkArray>* p = this;
	BOOL ret = (p->m_vec.begin() < p->m_vec.end());
	m_advisesSync.Unlock();
	return ret;
}

BOOL OPCServer::checkClientConnection(void)
{
	IConnectionPointImpl<OPCServer, &IID_IOPCShutdown, CComDynamicUnkArray>* p = this;
	IUnknown** pp;
	IOPCShutdown* pIShutdown;
	IUnknown *unk;
	HRESULT res;
	m_advisesSync.Lock();
	BOOL conOK = (p->m_vec.begin() < p->m_vec.end());

	pp = p->m_vec.begin();
	while (pp < p->m_vec.end())
	{
		if (*pp != NULL)
		{
			pIShutdown = (IOPCShutdown *)*pp;

			try 
			{
				res = pIShutdown->QueryInterface(IID_IOPCServer, (void **)&unk);
			}
			catch (...)
			{
				res = 0x800706BA;
			}
			if (FAILED(res))
			{
				if ((res == 0x800706BA)	|| (res == 0x800706BF) || (res == 0x80010108)) // RPC server unavailable
				{
					conOK = FALSE;
					break;
				}
			}
			else
				unk->Release();
		}
		pp++;
	}

	m_advisesSync.Unlock();
	return conOK;
}


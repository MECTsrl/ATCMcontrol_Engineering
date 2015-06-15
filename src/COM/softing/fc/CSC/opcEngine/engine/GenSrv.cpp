//-----------------------------------------------------------------------------
//                                  OPC ENGINE                                |
//                                                                            |
//  Filename    : GENSRV.CPP                                                  |
//  Version     : 2.01.0.00.release                                           |
//  Date        : 7-May-1999                                                  |
//  Author      : ATCM                                                 |
//                                                                            |
//  Description : OPC Server object related classes                           |
//                - GenericServer: OPC server object                          |
//                - OPCRootOPC: root object of OPC runtime tree               |
//                                                                            |
//  CHANGE_NOTES                                                              |
//                                                                            |
//  date      name      change                                                |
//  ------------------------------------------------------------------------- |
//  12/02/98  RT        added IOPCShutdown access                             |
//  12/04/98  RT        added IOPCCommon support                              |
//  12/16/98  RT        added IOPCItemProperties support                      |
//                                                                            |
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "GenSrv.h"
#include "GenCache.h"
#include "GenNS.h"
#include "OPCEngine.h"
#include "GenGrp.h"
#include "GenItem.h"
#include "OPCTrace.h"
#include "dllmain.h"
#include "OPCGroup.h"
#include "OPCServer.h"
#include <opcerror.h>


#define END_EVENT	0
#define START_EVENT	1


//-----------------------------------------------------------------------------
// requestThreadMain
//
// - thread main routine of request handling thread
//
// returns:
//		thread end state
//
DWORD WINAPI requestThreadMain(
	IN LPVOID context)	// application object
{
	GenericServer *srv = (GenericServer *)context;
	return srv->requestThread();
} // requestThreadMain


//-----------------------------------------------------------------------------
// GenericServer                                                              |
//-----------------------------------------------------------------------------

GenericServer::GenericServer(
	IN OPTIONAL BOOL publicGroups,			// creating public groups server
	IN OPTIONAL DWORD minRequestLoopTime,	// minimal request loop time
	IN OPTIONAL BYTE whatGroupList,			// generic list type of group list
	IN OPTIONAL int blockSizeGroupList,		// block size of group list
	IN OPTIONAL int hashSizeGroupList)		// hash table size of group map
 : GenericBranch(GENOBJ_TYPE_SERVER, GENOBJ_TYPE_BRANCH, 0, whatGroupList, 0, blockSizeGroupList, 0, hashSizeGroupList)
{
	_TRACE(TL_DEB, TG_SRV, (_T("> constructor")));
	GenericPointer<OPCEngine> engine = ::getOPCEngine();
	CString genName;

	m_publicGroupTemplates = publicGroups; 

    m_lastUpdateTime.dwLowDateTime  = 0L;
    m_lastUpdateTime.dwHighDateTime = 0L;
	m_bandwidth = 0;
	m_dueTimeRead = INFINITE;
	m_dueTimeWrite = INFINITE; 
	m_minRequestLoopTime = minRequestLoopTime;

	m_curBrowse = engine->getNamespaceRoot();
	m_curBrowsePath = _T("");
	m_me = NULL;
	m_transactionListChanged = FALSE;

	m_clientName = _T("?");
	m_lcid = MAKELCID( MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US) , SORT_DEFAULT);

	// set unique name
	genName.Format(_T("SRV%08.8X"), this);
	setName(genName);	

	if (!isPublicGroups())
	{
		// create events
		m_events[END_EVENT] = engine->popPoolEvent();
		m_events[START_EVENT] = engine->popPoolEvent();
		
		// create asynchronous request thread
		m_requestThread = ::CreateThread(NULL, 0, requestThreadMain, (void *)this, 0, &m_requestId);
	}
	else
	{
		m_events[END_EVENT] = INVALID_HANDLE_VALUE;
		m_events[START_EVENT] = INVALID_HANDLE_VALUE;
		m_requestThread = INVALID_HANDLE_VALUE;
		m_requestId	= 0;
	}
	_TRACE(TL_DEB, TG_SRV, (_T("< constructor")));
}


GenericServer::~GenericServer(void)
{
	_TRACE(TL_DEB, TG_SRV, (_T("> destructor")));
	GenericPointer<OPCEngine> engine = ::getOPCEngine();
	POSITION pos;
	OPCTransaction *tac;

	// release all active transactions of the server
	pos = m_transaction.GetHeadPosition();
	while (pos)
	{
		tac = m_transaction.GetNext(pos);
		tac->release();
	}
	m_transaction.RemoveAll();

	if (m_events[START_EVENT] != INVALID_HANDLE_VALUE)
		engine->pushPoolEvent(m_events[START_EVENT]);

	if (m_events[END_EVENT] != INVALID_HANDLE_VALUE)
		engine->pushPoolEvent(m_events[END_EVENT]);

	_TRACE(TL_DEB, TG_SRV, (_T("< destructor")));
}


//-----------------------------------------------------------------------------
// release
//
// - decrement reference counter
// - free object if no reference left
//
// returns:
//		reference count
//
LONG GenericServer::release(void)
{
	LONG refCount = InterlockedDecrement(&m_refCount);
	_TRACE(TL_DEB, TG_GEN, (_T("release %li"), refCount))
	if (refCount <= 0)
	{
		// terminate request thread
		if (m_requestThread != INVALID_HANDLE_VALUE)
		{
			::SetEvent(m_events[END_EVENT]);
			::WaitForSingleObject(m_requestThread, INFINITE);
			::CloseHandle(m_requestThread);
			m_requestId	= 0;
		}
		delete this;	// last reference -> free object
		return 0;
	}
	return refCount;
} // release


//-----------------------------------------------------------------------------
// addLeaf
//
// - don't add leaf into server object
//
// returns:
//		FALSE - not added
//
BOOL GenericServer::addLeaf(
	IN GenericLeaf *newLeaf, 
	IN OPTIONAL int index)
{
	return FALSE;
} // addLeaf


//-----------------------------------------------------------------------------
// addBranch
//
// - adds branch to branch list, if branch is a group object
//
// returns:
//		TRUE  - added
//		FALSE - not added
//
BOOL GenericServer::addBranch(
	IN GenericBranch *newBranch, 
	IN OPTIONAL int index)
{
	if (!newBranch)
		return FALSE;

	if (newBranch->is(GENOBJ_TYPE_GROUP))
		return GenericBranch::addBranch(newBranch, index);
	else
		return FALSE;
} // addBranch


//-----------------------------------------------------------------------------
// setLCID
//
// - set local Id
//
// returns:
//		TRUE
//
BOOL GenericServer::setLCID(
	IN DWORD lcid)	// local Id
{
	LCID *lcidArray;
	DWORD nrLCIDs;
	DWORD i;

	nrLCIDs = queryAvailableLCIDs(&lcidArray);
	for (i = 0; i < nrLCIDs; i++)
	{
		if (lcidArray[i] == lcid)
		{
			_TRACE(TL_INF, TG_SRV, (_T("set locale id to 0x%X"), lcid));
			onSetLCID(lcid);
			m_lcid = lcid;
			free(lcidArray);
			return TRUE;
		}
	}
	if (nrLCIDs)
		free(lcidArray);

	_TRACE(TL_INF, TG_SRV, (_T("can't set locale id to 0x%X (LCID not supported)"), lcid));
	return FALSE;
} // setLCID


//-----------------------------------------------------------------------------
// onSetLCID
//
// - called before local id is set
// - default implementation: do nothing
//
void GenericServer::onSetLCID(
	IN DWORD lcid)
{} // onSetLCID


//-----------------------------------------------------------------------------
// setClientName
//
// - set client name
//
// returns:
//		TRUE
//
BOOL GenericServer::setClientName(
	IN LPCTSTR clientName)	// client name
{
	_TRACE(TL_INF, TG_SRV, (_T("set client name to %s"), clientName));
	onSetClientName(clientName);
	m_clientName = clientName;
	return TRUE;
} // setClientName


//-----------------------------------------------------------------------------
// onSetClientName
//
// - called before client name is set
// - default implementation: do nothing
//
void GenericServer::onSetClientName(
	IN LPCTSTR name)	// client name
{} // onSetClientName


//-----------------------------------------------------------------------------
// queryAvailableLCIDs
//
// - return all available locale Ids
//
// returns:
//		number of elements of the array
//
DWORD GenericServer::queryAvailableLCIDs(
	OUT LCID **lcidArray)
{
	if (!lcidArray)
		return 0;

	*lcidArray = (LCID *)malloc(2 * sizeof(LCID));
	*(*lcidArray) = MAKELCID( MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT);
	*((*lcidArray) + 1) = MAKELCID( MAKELANGID(LANG_GERMAN, SUBLANG_GERMAN), SORT_DEFAULT);

	return 2;
} // queryAvailableLCIDs


//-----------------------------------------------------------------------------
// getGroupByHandle
//
// - retrieve group pointer by the server handle value
//
// returns:
//		pointer to the group
//		NULL - no group with this server handle
//
GenericGroup *GenericServer::getGroupByHandle(
	IN OPCHANDLE serverHandle)	// server handle
{
	GenericGroup *group;
	GenericList<GenericBranch> list(getBranchList());
	POSITION pos;

	list.lock();
	pos = list.getStartPosition();
	while(pos)
	{
		group = (GenericGroup *)list.getNext(pos);
		if (group->getServerHandle() == serverHandle)
		{ // group found
			_TRACE(TL_DEB, TG_SRV, (_T("groupByHandle h:0x%X %s"), serverHandle, group->getObjTraceId()));
			group->addRef();
			return group;
		}
	}
	_TRACE(TL_INF, TG_SRV, (_T("no groupByHandle h:0x%X"), serverHandle));
	return NULL;
} // getGroupByHandle


//-----------------------------------------------------------------------------
// getGroupByName
//
// - retrieve group pointer by the name of the group
//
// returns:
//		pointer to the group
//		NULL - no group with this name
//
GenericGroup *GenericServer::getGroupByName(
	IN LPCTSTR name)	// group name
{
	return (GenericGroup *)findBranch(name, FALSE);
} // getGroupByName


//-----------------------------------------------------------------------------
// addGroup (interface IOPCServer)
//
// - adds a group to the server
// - creates tree group object and put it to the tree
// - set initialization parameter
//
// returns:
//		S_OK                  - added group
//		OPC_S_UNSUPPORTEDRATE - changed update rate (but added group)
//		E_INVALIDARG          - bad name)
//		OPC_E_DUPLICATENAME   - group name not unique
//
HRESULT GenericServer::addGroup(
	IN LPCTSTR name,			// name of group
	IN BOOL active,				// activation state
	IN DWORD reqUpdateRate,		// requested update rate
	IN OPCHANDLE clientHandle,	// client handle
	IN LONG *pTimeBias,			// time zone
	IN FLOAT *pPercentDeadband,	// deadband 
	IN DWORD lcid,				// LCID
	OUT GenericGroup **group,	// created group
	OUT DWORD *revUpdateRate)	// revised update rate
{ 
	_TRACE(TL_DEB, TG_SRV, (_T("> addGroup %s"), name));
	GenericPointer<OPCEngine> engine = ::getOPCEngine();
	GenericPointer<OPCTransaction> trans;
	CString grpName;
	GenericPointer<GenericCreator> creator;	
	HRESULT res = S_OK;
	BOOL changedUpdateRate;
	GenericGroup *grp;

	// check the name
	if ((name) && (_tcslen(name) != 0))
	{
		// valid name ?
		if (FAILED(validateName(name, FALSE)))
		{
			_TRACE(TL_INF, TG_SRV, (_T("can't add group; no valid name %s"), name));
			return E_INVALIDARG;
		}

		// unique name ?
		if (isUniqueChildName(name))
			grpName = name;
		else
		{
			_TRACE(TL_INF, TG_SRV, (_T("can't add group; no unique name %s"), name));
			return OPC_E_DUPLICATENAME;
		}
	}

#ifdef DEMO_VERSION
	// demo version limited groups
	{
	GenericList<GenericBranch> list(getBranchList());

	if (list.getCount() >= 2)
	{
		_TRACE(TL_INF, TG_SRV, (_T("DEMO Version!!! only two groups per client")));
		return E_FAIL;
	}
	}
#endif

	// create OPC tree object
	creator = engine->getCreator();
	grp = creator->createGroup(this);

	if ((!name) || (_tcslen(name) == 0))
	{ // no name -> generate unique name
		grpName.Format(_T("GRP%08.8X"), grp);
	}

	// set name
	grp->setName(grpName);	
	// add to tree
	addBranch(grp);
	_TRACE(TL_INF, TG_SRV, (_T("added group %s"), grp->getObjTraceId()));

	// add cyclic transaction
	trans = grp->getCyclicTransaction();
	addTransaction(trans);

	// set group parameters
	grp->setActive(active);
	changedUpdateRate = !(grp->setUpdateRate(reqUpdateRate));
	grp->setClientHandle(clientHandle);
	if (pTimeBias)
	{ 
		grp->setTimeBias(*pTimeBias);
	}
	if (pPercentDeadband)
	{
		grp->setDeadBand(*pPercentDeadband);
	}
	grp->setLCID(lcid);

	// check update rate
    if (revUpdateRate != NULL) 
	{
		*revUpdateRate = grp->getUpdateRate();
		if (changedUpdateRate)	
			res = OPC_S_UNSUPPORTEDRATE;
    }

	if (group != NULL)
		*group = grp;
	else
		grp->release();
	
	_TRACE(TL_DEB, TG_SRV, (_T("< addGroup (0x%X)"), res));
	return res; 
} // addGroup


//-----------------------------------------------------------------------------
// removeGroup
//
// - removes a group to the server
//
// returns:
//		S_OK         - removed
//		E_INVALIDARG - bad server handle
//
HRESULT GenericServer::removeGroup(
	IN OPCHANDLE serverHandle,		// server handle
	IN OPTIONAL BOOL remPublicGrp)	// remove public group objects
{ 
	_TRACE(TL_DEB, TG_SRV, (_T("> removeGroup 0x%X"), serverHandle));
	GenericPointer<GenericGroup> group;	
	GenericPointer<OPCTransaction> trans;
	HRESULT res = S_OK;

	// get group
	group = getGroupByHandle(serverHandle);
	if (!group)
	{
		_TRACE(TL_INF, TG_SRV, (_T("can't remove group; no group with handle 0x%X"), serverHandle));
		return E_INVALIDARG;	// no group with this handle
	}

	if ((group->isPublicGroup()) && (!remPublicGrp))
	{
		// don't remove public group objects !!
		_TRACE(TL_INF, TG_SRV, (_T("don't remove public group objects")));
		return E_INVALIDARG;	
	}

	if (isPublicGroups())
	{
		// public group template
		if (!canRemovePublicGroup(group))
		{
			_TRACE(TL_INF, TG_SRV, (_T("can't remove this public group")));
			return E_FAIL;
		}
	}

	group->setRemoved();	// removed flag

	// remove all item objects
	group->removeAllLeafs(FALSE);

	// remove cyclic transaction
	trans = group->getCyclicTransaction();
	removeTransaction(trans);

	removeBranch(group);
	_TRACE(TL_INF, TG_SRV, (_T("removed group %s"), group->getObjTraceId()));

	if (group->getRefCount() > 2)	// one ref from COM object and on of the
	{								// local group pointer are OK
		res = OPC_S_INUSE;
	}

	_TRACE(TL_DEB, TG_SRV, (_T("< removeGroup (0x%X)"), res));
	return res; 
} // removeGroup



BOOL GenericServer::canRemovePublicGroup(
	IN GenericGroup *group)
{
	return TRUE;
}



//-----------------------------------------------------------------------------
// getErrorString
//
// - get error string for an error code
//
// returns:
//		S_OK          - got error string
//		E_INVALIDARG  - no string for the error code
//
HRESULT GenericServer::getErrorString(
	IN HRESULT error,		// error code 
	IN DWORD lcid,			// locale for string
	OUT CString &string)	// error string
{ 
	_TRACE(TL_DEB, TG_SRV, (_T("> getErrorString 0x%X"), error));
	LPVOID msgBuf = NULL;
	DWORD formatResult;
	WORD langID = LANGIDFROMLCID(lcid);	// locale identifier 
 
	// search for system errorcodes
	formatResult = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 
						NULL, error, langID, (LPTSTR) &msgBuf, 0, NULL);

	if (formatResult == 0)	
	{
		// search for OPC errorcodes
		HMODULE module = ::getInstanceHandle();

		formatResult = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_HMODULE, 
						module, error, langID, (LPTSTR) &msgBuf, 0, NULL);
	}

	if (formatResult != 0)	
	{
		_TRACE(TL_INF, TG_SRV, (_T("getErrorString 0x%X >%s<"), error, string));
		string = (LPTSTR)msgBuf;
		LocalFree(msgBuf);
		return S_OK;
	}
	else
	{
		_TRACE(TL_INF, TG_SRV, (_T("getErrorString failed 0x%X"), GetLastError()));
		return E_INVALIDARG;
	}
} // getErrorString


//-----------------------------------------------------------------------------
// getStatus
//
// - gets the server status
//
// returns:
//		S_OK          - filled status sturcture
//		E_OUTOFMEMORY - can't allocate vendor string memory
//
HRESULT GenericServer::getStatus(
	OUT OPCSERVERSTATUS *status)	// server status
{ 
	_TRACE(TL_DEB, TG_SRV, (_T("> getStatus")));
	GenericPointer<OPCEngine> engine = ::getOPCEngine();
	CString vendor = engine->getVendorInfo();
 	GenericList<GenericBranch> list(getBranchList());
	USES_CONVERSION;

    status->szVendorInfo = (LPWSTR)CoTaskMemAlloc(2*vendor.GetLength()+2);
	if (status->szVendorInfo == NULL)	
	{
		_ERROR(TG_SRV, OPCENGINE_ERR_MEMORY,(_T("GetStatus: Out of memory")));
		return E_OUTOFMEMORY;
	}
    wcscpy(status->szVendorInfo, T2COLE((LPCTSTR)vendor));

    engine->getStartTime(&status->ftStartTime);
    CoFileTimeNow(&status->ftCurrentTime);
    getUpdateTime(&status->ftLastUpdateTime);
    status->dwServerState = engine->getServerState();
    status->dwGroupCount = list.getCount();
    status->dwBandWidth = getBandwidth();
   	engine->getVersionInfo(&status->wMajorVersion, &status->wMinorVersion,
						   &status->wBuildNumber);

	_TRACE(TL_INF, TG_SRV, (_T("get server status")));
    return S_OK;
} // getStatus


DWORD GenericServer::getBandwidth(void) 
{
	GenericPointer<OPCEngine> engine = ::getOPCEngine();
	DWORD bandwidth;
	DWORD timeSpan;
	OPCStatistics stat;

	engine->getStatistics(NULL, &stat, &timeSpan);

	bandwidth = (stat.req.timeRead + stat.req.timeWrite) * 100 /
				(timeSpan + 1);

	_TRACE(TL_DEB, TG_SRV, (_T("bandwidth: %lu"), bandwidth));
	return bandwidth;
}

GenericTag *GenericServer::getTag(IN LPCTSTR fullName)
{
	GenericPointer<OPCEngine> engine = ::getOPCEngine();
	GenericPointer<OPCRootNS> root;
	GenericTag *tag;

	root = engine->getNamespaceRoot();
	tag = (GenericTag *)root->findLeaf(fullName, TRUE);
	if ((GenericTag *)tag)
		return tag;
	else
		return NULL;
}

HRESULT GenericServer::queryAvailableProperties(
	IN GenericTag *tag,
	OUT TagPropertyDataList &propertyList)	
{
	HRESULT res;
	GenericList<GenericTagProperty> tagList;
	GenericTagProperty *prop;
	OPCTagPropertyData *propData;
	POSITION pos;

	tagList = tag->getPropertyList();
	tagList.lock();
	pos = (POSITION)tagList;
	while (pos)
	{
		prop = tagList.getNext(pos);
		propData = new OPCTagPropertyData;
		prop->getPropertyData(propData);
		propertyList.AddTail(propData);
	}
	res = S_OK;

	return res;
} // queryAvailableProperties

HRESULT GenericServer::queryAvailableDynamicProperties(
	IN LPCTSTR itemId,
	OUT TagPropertyDataList &propertyList)
{
	return S_OK;
}

//-----------------------------------------------------------------------------
// queryOrganization
//
// - checks if the namespace tree is flat or hierarchial
//
// returns:
//		S_OK
//
HRESULT GenericServer::queryOrganization(
	OUT OPCNAMESPACETYPE *nameSpaceType)	// type of namespace
{ 
	_TRACE(TL_DEB, TG_SRV, (_T("> queryOrganisation")));
	GenericPointer<OPCEngine> engine = ::getOPCEngine();
	// get namespace root
	GenericPointer<OPCRootNS> root = engine->getNamespaceRoot();
 	GenericList<GenericBranch> list(root->getBranchList());
		
	if (list.isEmpty())
	{ // branch list of namespace root empty
	  // -> flat namespace
		*nameSpaceType = OPC_NS_FLAT;
		_TRACE(TL_INF, TG_NS, (_T("flat namespace")));
	}
	else
	{ // branch list of namespace root not empty
	  // -> hierarchial namespace
		*nameSpaceType = OPC_NS_HIERARCHIAL;
		_TRACE(TL_INF, TG_NS, (_T("hierarchial namespace")));
	}

	return S_OK; 
} // queryOrganization


//-----------------------------------------------------------------------------
// changeBrowsePosition
//
// - changes the current position in the namespace tree to a static node
// - only called if the current position is a static node
//
// returns:
//		S_OK         - position changed
//		E_INVALIDARG - bad name or direction
//
HRESULT GenericServer::changeBrowsePosition(
	IN OPCBROWSEDIRECTION direction,	// direction to go in the tree
	IN LPCTSTR name)					// name of down branch
{ 
	GenericBranch *newPos = NULL;
	HRESULT res;

	_TRACE(TL_DEB, TG_NS, (_T("> changeBrowsePosition 0x%X %s"), 
			direction, name));

	switch (direction)
	{
		case OPC_BROWSE_UP:
			newPos = m_curBrowse->getParentBranch();
		break;
		case OPC_BROWSE_DOWN:
			newPos = m_curBrowse->findBranch(name);
		break;
		case OPC_BROWSE_TO:
		{
			GenericPointer<OPCEngine> engine = ::getOPCEngine();
			GenericPointer<OPCRootNS> root = engine->getNamespaceRoot();

			if (_tcslen(name) == 0)
			{
				newPos = root;	
				newPos->addRef();
			}
			else
				newPos = root->findBranch(name, TRUE);
		}
		break;
	}

	if (newPos)
	{
		m_curBrowse = newPos;
		m_curBrowsePath = m_curBrowse->getFullName();
		_TRACE(TL_INF, TG_NS, (_T("changed browse position to %s %s"), m_curBrowsePath, m_curBrowse->getObjTraceId()));
		res = S_OK;
	}
	else
		res = E_INVALIDARG;	// called up to root node
							// or down to a none existing child branch

	_TRACE(TL_DEB, TG_NS, (_T("< changeBrowsePosition (0x%X)"), res));
	return res; 
} // changeBrowsePosition


//-----------------------------------------------------------------------------
// changeDynamicBrowsePosition
//
// - changes the current position in the namespace tree to a dynamic node
// - checks if change leads to a static node
//
// returns:
//		S_OK         - position changed
//		E_INVALIDARG - no dynamic child node
//
HRESULT GenericServer::changeDynamicBrowsePosition(
	IN OPCBROWSEDIRECTION direction,	// direction to go in the tree
	IN LPCTSTR name)					// name of down branch
{
	GenericPointer<OPCEngine> engine = ::getOPCEngine();
	CString path((LPCTSTR)m_curBrowsePath);
	HRESULT res = S_OK;

	_TRACE(TL_DEB, TG_NS, (_T("> changeDynamicBrowsePosition 0x%X %s"), 
			direction, name));

	switch (direction)
	{
		case OPC_BROWSE_UP:
		{
			GenericPointer<OPCRootNS> root = engine->getNamespaceRoot();
			LPCTSTR upperLevel = NULL;
			LPCTSTR name = NULL;

			reducePath(path, upperLevel, name, FALSE);
			if (upperLevel)
			{
				m_curBrowsePath = upperLevel;
				// check if upper level is static
				m_curBrowse = root->findBranch(upperLevel);
			}
			else
			{ 
				// reached the static root object
				m_curBrowse = root;
				m_curBrowse.addRef();
				m_curBrowsePath = "";
			}
		}
		break;
		case OPC_BROWSE_DOWN:
		{
			TCHAR pathSeparator = engine->getTreeDelimiter(NULL, NULL);
			if (!path.IsEmpty())
				path += pathSeparator;
			path += name;
			if (isDynamicNode(path))
			{ // found dynamic child node
				m_curBrowsePath = path;
				m_curBrowse = NULL;
				res = S_OK;
			}			
			else
				res = E_INVALIDARG;
		}
		break;
		case OPC_BROWSE_TO:
		{
			if (isDynamicNode(name))
			{ // found dynamic child node
				m_curBrowsePath = name;
				m_curBrowse = NULL;
				res = S_OK;
			}			
			else
				res = E_INVALIDARG;
		}
		break;
	}

	if (res == S_OK)
	{
		_TRACE(TL_INF, TG_NS, (_T("changed dynamic browse position to %s 0x%X"), m_curBrowsePath, m_curBrowse));
	}

	_TRACE(TL_DEB, TG_NS, (_T("< changeDynamicBrowsePosition (0x%X)"), res));
	return res;
} // changeDynamicBrowsePosition


//-----------------------------------------------------------------------------
// browseCurrentBranch
//
// - browses the current branch for branches, leafs or flat
//
// returns:
//		S_OK - filled list
//
HRESULT GenericServer::browseCurrentBranch(
	IN OPCBROWSETYPE browseType,	// what to browse
	IN LPCTSTR filterCriteria,		// string filter
	IN VARTYPE dataTypeFilter,		// data type filter
	IN DWORD accessRightsFilter,	// access rights filter
	OUT CStringList &nameList,		// found elements
	IN OPTIONAL LPCTSTR path)		// path for OPC_FLAT browseType 
{ 
	BOOL addToList;
	POSITION pos;

	_TRACE(TL_DEB, TG_NS, (_T("> browseCurrentBranch type 0x%X"), browseType));

	// ignore browse type for flat namespace
	if (browseType == OPC_BRANCH) 
	{
		OPCNAMESPACETYPE nsOrg;
		queryOrganization(&nsOrg);
		if (nsOrg == OPC_NS_FLAT)
			browseType = OPC_LEAF;
	}

	switch (browseType)
	{
		// return only the branches
		case OPC_BRANCH:
		{
			_TRACE(TL_INF, TG_NS, (_T("browse current branch %s; OPC_BRANCH"), m_curBrowsePath));
		 	GenericList<GenericBranch> list(m_curBrowse->getBranchList());
			GenericBranch *branch;
		
			if (!(!list))
			{
				list.lock();
				pos = list.getStartPosition();
				while (pos)
				{
					branch = list.getNext(pos);
					_TRACE(TL_DEB, TG_NS, (_T("child node: %s"), branch->getObjTraceId()));

					nameList.AddTail(branch->getName());
				}
			}
		}
		break;

		// return only the leafs
		case OPC_LEAF:
		{
			_TRACE(TL_INF, TG_NS, (_T("browse current branch %s; OPC_LEAF datatype:%d access:0x%X"), 
					m_curBrowsePath, dataTypeFilter, accessRightsFilter));
			GenericList<GenericLeaf> list(m_curBrowse->getLeafList());
			GenericTag *tag;
			CString name;

			if (!(!(list)))
			{
				list.lock();
				pos = list.getStartPosition();
				while (pos)
				{
					tag = (GenericTag *)list.getNext(pos);
					_TRACE(TL_DEB, TG_NS, (_T("child tag: %s"), tag->getObjTraceId()));

					if (!path)
						name = tag->getName();
					else
					{
						name = path; 
						name += tag->getName();
					}
					
					addToList = TRUE;

					// string filter
					if ((filterCriteria) && (_tcslen(filterCriteria) != 0))
						addToList = matchStringFilter(name, filterCriteria);					

					// data type filter
					if (addToList)	
					{
						if (dataTypeFilter != VT_EMPTY)
						{
							if (dataTypeFilter != tag->getNativeDatatype())
							{
								_TRACE(TL_DEB, TG_NS, (_T("other datatype: %d"), tag->getNativeDatatype()));
								addToList = FALSE;
							}
						}
					}

					// access rights filter
					if (addToList)	
					{
						if (accessRightsFilter != 0)
						{
							if ((accessRightsFilter & tag->getAccessRights()) 
								!= accessRightsFilter)
							{
								_TRACE(TL_DEB, TG_NS, (_T("bad access rights: 0x%X"), tag->getAccessRights()));
								addToList = FALSE;
							}
						}
					}

					// passed all filters?
					if (addToList)
						nameList.AddTail(name);	// add to list
				}
			}
		}
		break;

		// return all leafs including the leaf of the child branches
		case OPC_FLAT:
		{
			GenericPointer<OPCEngine> engine = ::getOPCEngine();
			TCHAR pathSeparator = engine->getTreeDelimiter(NULL, NULL);
			engine.release();
			GenericBranch *orgBranch;
			CString orgPath; 
			CString thisPath;

			if (path == NULL)
			{ // recursion start
				_TRACE(TL_INF, TG_NS, (_T("browse current branch %s; OPC_FLAT"), m_curBrowsePath));
				orgBranch = m_curBrowse;
				orgBranch->addRef();
				orgPath = m_curBrowsePath;
			}
			else
				thisPath = path;

			browseCurrentBranch(OPC_LEAF, filterCriteria, dataTypeFilter,
					accessRightsFilter,	nameList, thisPath);

		 	GenericList<GenericBranch> list(m_curBrowse->getBranchList());
			if (!(!list))
			{
				list.lock();
				pos = list.getStartPosition();
				while (pos)
				{
					m_curBrowse = list.getNext(pos);
					m_curBrowse.addRef();
					_TRACE(TL_DEB, TG_NS, (_T("child node: %s"), m_curBrowse->getObjTraceId()));

					m_curBrowsePath = m_curBrowse->getFullName();

					browseCurrentBranch(OPC_FLAT, filterCriteria, dataTypeFilter,
						accessRightsFilter,	nameList, thisPath + m_curBrowse->getName() + pathSeparator);
				}
			}

			if (path == NULL)
			{ // recursion start
				m_curBrowse = orgBranch;
				m_curBrowsePath = orgPath;
			}
		}
		break;
	}

	_TRACE(TL_DEB, TG_NS, (_T("< browseCurrentBranch")));
	return S_OK; 
} // browseCurrentBranch


//-----------------------------------------------------------------------------
// getBrowseElementPath
//
// - get the full qualified element name from current browse position
//
// returns:
//		TRUE  - element found
//		FALSE - element not found
//
BOOL GenericServer::getBrowseElementPath(
	IN LPCTSTR name,	// element name
	OUT CString &path)	// full qualified element name
{
	BOOL found = FALSE;
	_TRACE(TL_DEB, TG_NS, (_T("> getBrowseElementPath %s"), name));

	// calculate the full path
	path = m_curBrowsePath;
	if (!CString(name).IsEmpty())
	{
		GenericPointer<OPCEngine> engine = ::getOPCEngine();
		TCHAR pathSeparator = engine->getTreeDelimiter(NULL, NULL);
		if (!path.IsEmpty())
			path += pathSeparator;	// don't add separator on root
		path += name;
	}

	// check if a element with the name exists
	if ((GenericBranch *)m_curBrowse)
	{
		// serach static name space
		GenericPointer<GenericElement> el;
		el = m_curBrowse->findLeaf(name, TRUE);
		if ((GenericElement *)el == NULL)
		{
			el = m_curBrowse->findBranch(name, TRUE);
		}
		found = ((GenericElement *)el != NULL);
	}

	if (!found)
	{
		// serach dynamic name space
		if (!(found = isDynamicTag(path)))
			found = isDynamicNode(path);
	}

	_TRACE(TL_INF, TG_NS, (_T("getBrowseElementPath %s valid: %s"), 
			path, BOOL2STR(found)));

	return found;
} // getBrowseElementPath


//-----------------------------------------------------------------------------
// getDynamicBrowseAccessPaths
//
// - get the access path for a fully qualified tag
//
// returns:
//		S_FALSE - no access path
//
HRESULT GenericServer::getDynamicBrowseAccessPaths(
	IN LPCTSTR name,			// fully qualified tag name
	OUT CStringList &pathList)	// access path list
{ 
	return S_FALSE; 
} // getDynamicBrowseAccessPaths


//-----------------------------------------------------------------------------
// isDynamicNode
//
// - check if the fullName is a dynamic node 
//
// returns:
//		FALSE - no dynamic node
//
BOOL GenericServer::isDynamicNode(
	IN LPCTSTR fullName)	// name of node
{ 
	return FALSE; 
} // isDynamicNode


//-----------------------------------------------------------------------------
// isDynamicTag
//
// - check if the fullName is a dynamic tag 
//
// returns:
//		FALSE - no dynamic tag
//
BOOL GenericServer::isDynamicTag(
	IN LPCTSTR fullName)	// name of tag
{ 
	return FALSE; 
} // isDynamicTag

BOOL GenericServer::isDynamicTagProperty(IN LPCTSTR itemId, IN DWORD propId)
{ 
	return FALSE; 
} // isDynamicTagProperty

VARTYPE GenericServer::getDynamicTagDatatype(IN LPCTSTR fullName)
{
	return VT_EMPTY;
}

//-----------------------------------------------------------------------------
// getDynamicChildNames
//
// - called to receive the dymanic child names 
// - default implementation: do nothing
//
// returns:
//		see IOPCBrowseServerAddressSpace::BrowseOPCItemIDs
//
HRESULT GenericServer::getDynamicChildNames(
	IN OPCBROWSETYPE browseType,	// browse type
	IN LPCTSTR filterCriteria,		// string filter
	IN VARTYPE dataTypeFilter,		// data type filter
	IN DWORD accessRightsFilter,	// access filter
	OUT CStringList &nameList)		// list of dynamic child names
{
	return S_OK;
} // getDynamicChildNames


//-----------------------------------------------------------------------------
// createTag
//
// - create tag dynamic from fullName
// 
// returns:
//		NULL - can't create tag
//
GenericTag *GenericServer::createTag(
	IN LPCTSTR fullName)
{
	GenericTag *tag = NULL;

	if (isDynamicTag(fullName))
	{
		GenericPointer<OPCEngine> engine = ::getOPCEngine();
		GenericPointer<GenericCreator> creator = engine->getCreator();
		tag = creator->createTag(this, fullName);
	}

	return tag;
} // createTag



//-----------------------------------------------------------------------------
BOOL GenericServer::matchStringFilter(
	IN LPCTSTR string, 
	IN LPCTSTR filter)
{
	return GenericServer::opcMatchStringFilter(string, filter, TRUE);
}

//-----------------------------------------------------------------------------
BOOL GenericServer::stdMatchStringFilter(
	IN LPCTSTR string, 
	IN LPCTSTR filter)
{
	BOOL match;
	int actS, actF;
	int prev;

	if (!filter)
		return TRUE;
		
	if (_tcslen(filter) == 0)
		return TRUE;

	if (!string)
		return FALSE;

	actS = 0;
	actF = 0;
	prev = -1;	

	while ( ( (string[actS] == filter[actF]) || 
			  (filter[actF] == _T('*')) ||
			  (prev != -1) )
		 && (string[actS] != _T('\0')) )
	{
		if (filter[actF] != _T('*'))
		{
			if (string[actS] == filter[actF])
			{
				actF++;		
			}
			else
			{
				actF = prev;
				if (string[actS] == filter[actF+1])
				{
					// match Puchheim with *heim
					actF+=2;		
				}
			}
		}

		if (filter[actF] == _T('*'))
		{
			prev = actF;
			if (filter[actF+1] != _T('\0'))
			{
				if (string[actS+1] == filter[actF+1])
				{
					actF++;
				}
			}
		}
		actS++;
	}

	
	if (filter[actF] == _T('*'))
		actF++;		

	match = ((string[actS] == _T('\0')) && (filter[actF] == _T('\0')));
	return match;
}


BOOL GenericServer::opcMatchStringFilter(
	IN LPCTSTR String, 
	IN LPCTSTR Pattern, 
	IN BOOL bCaseSensitive)
{ 
	if( !String )
		return FALSE;
	if( !Pattern )
		return TRUE;
	TCHAR c, p, l;

	for (; ;)
	{
		switch (p = ((bCaseSensitive) ? *Pattern++ : toupper(*Pattern++)) )
		{
			case 0:                             // end of pattern
				return *String ? FALSE : TRUE;  // if end of string TRUE

			case _T('*'):
				while (*String) 
				{               // match zero or more char
					if (opcMatchStringFilter (String++, Pattern, bCaseSensitive))
						return TRUE; 
				}
				return opcMatchStringFilter (String, Pattern, bCaseSensitive );

			case _T('?'):
				if (*String++ == 0)             // match any one char 
					return FALSE;                   // not end of string 
			break; 

			case _T('['): 
				if ((c = ((bCaseSensitive) ? *String++ : toupper(*String++)) ) == 0) // match char set 
					return FALSE;                   // syntax 
				l = 0; 
				if( *Pattern == _T('!') )  // match a char if NOT in set []
				{
					++Pattern;

					while( (p = ((bCaseSensitive) ? *Pattern++ : toupper(*Pattern++)) ) != _T('\0') ) 
					{
						if (p == _T(']'))               // if end of char set, then 
							break;           // no match found 

						if (p == _T('-')) 
						{            // check a range of chars? 
							p = ((bCaseSensitive) ? *Pattern++ : toupper(*Pattern++));   // get high limit of range 
							if (p == 0  ||  p == _T(']')) 
								return FALSE;           // syntax 

							if (c >= l  &&  c <= p) 
								return FALSE;              // if in range, return FALSE 
						} 
						l = p;
						if (c == p)                 // if char matches this element 
							return FALSE;                  // return false 
					} 
				}
				else	// match if char is in set []
				{
					while( (p = ((bCaseSensitive) ? *Pattern++ : toupper(*Pattern++)) ) != _T('\0') ) 
					{
						if (p == _T(']'))               // if end of char set, then 
							return FALSE;           // no match found 

						if (p == _T('-')) 
						{            // check a range of chars? 
							p = ((bCaseSensitive) ? *Pattern++ : toupper(*Pattern++));   // get high limit of range 
							if (p == 0  ||  p == _T(']')) 
								return FALSE;           // syntax 

							if (c >= l  &&  c <= p) 
								break;              // if in range, move on 
						} 
						l = p;
						if (c == p)                 // if char matches this element 
							break;                  // move on 
					} 

					while (p  &&  p != _T(']'))         // got a match in char set 
						p = *Pattern++;             // skip to end of set 
				}
			break; 

			case _T('#'):
				c = *String++; 
				if( !_istdigit( c ) )
					return FALSE;		// not a digit
			break;

			default: 
				c = ((bCaseSensitive) ? *String++ : toupper(*String++)); 
				if( c != p )            // check for exact char 
					return FALSE;                   // not a match 
			break; 
		} 
	} 
} 

HANDLE GenericServer::getStartEvent(void) const
{ 
	return m_events[START_EVENT]; 
}


//-----------------------------------------------------------------------------
// requestThread
//
// - thread routine of request handling thread
//
// returns:
//		thread end state
//
DWORD GenericServer::requestThread(void)
{
	BOOL end = FALSE;
	DWORD ret;
	DWORD waitTime = m_minRequestLoopTime;
	DWORD i = 10;
	DWORD loopStart;	
	DWORD loopTime;	

	try
	{
		_TRACE(TL_INF, TG_SRV, (_T("started request thread")));
		while(!end)
		{
			ret = ::WaitForMultipleObjects(2, m_events, FALSE, waitTime);

			switch (ret - WAIT_OBJECT_0)
			{
				case END_EVENT:
				{
					_TRACE(TL_INF, TG_SRV, (_T("terminate request thread")));
					end = TRUE;
				}
				break;

				case START_EVENT:
					_TRACE(TL_DEB, TG_SRV, (_T("received start event")));
					// fall through default
				default:
				{
					_TRACE(TL_DEB, TG_SRV, (_T("> request loop")));
					loopStart = ::GetTickCount();
					waitTime = startCyclicTransactions(loopStart, INFINITE);

					waitTime = updateTransactions(loopStart, waitTime);				

					if (m_minRequestLoopTime < waitTime)
						waitTime = m_minRequestLoopTime;

					loopTime = getTimeSpan(loopStart, ::GetTickCount());
					if ((loopTime != 0) && (waitTime >= loopTime) && (waitTime != INFINITE))
						waitTime -= loopTime;
					_TRACE(TL_DEB, TG_SRV, (_T("< request loop %lu %lu"), waitTime, loopTime));
				}
				break;
			}
		}
	}
	catch(CException* e)
	{
		TCHAR excText[100];
		CRuntimeClass* rt = e->GetRuntimeClass();
		e->GetErrorMessage(excText, 100);
		_ERROR(TG_SRV, OPCENGINE_ERR_EXCEPTION,(_T("MFC EXCEPTION in Request Thread: %s (%s)"), rt->m_lpszClassName, excText));
		return 1;
	}	
	catch(...)
	{
		_ERROR(TG_SRV, OPCENGINE_ERR_EXCEPTION,(_T("unknown EXCEPTION in Request Thread")));
		return 1;
	}	

	return 0;
} // requestThread

DWORD GenericServer::startCyclicTransactions(
	IN DWORD loopStart,
	IN DWORD minSpan)
{
	_TRACE(TL_DEB, TG_SRV, (_T("> startCyclicTransactions")));

 	GenericList<GenericBranch> list(getBranchList());
	GenericGroup *grp;
	GenericPointer<OPCTransaction> tac;
	POSITION posGrp;
	DWORD actSpan;

	// for every group
	posGrp = list.getStartPosition();
	while (posGrp)
	{
		grp = (GenericGroup *)list.getNext(posGrp);
		actSpan = INFINITE;
		_TRACE(TL_DEB, TG_SRV, (_T("update span group %s"), grp->getObjTraceId()));
		if (grp->updateSpan(loopStart, &actSpan))
		{ 
			tac = grp->getCyclicTransaction();
			if (grp->getUpdateMode() == GenericGroup::poll)
			{
				// start the cyclic requests of the group
				_TRACE(TL_DEB, TG_SRV, (_T("start requests of cyclic transaction %s"), tac->getObjTraceId()));
				tac->startRequests();
			}
			else
			{
				// complete all reported changes
				_TRACE(TL_DEB, TG_SRV, (_T("complete report requests of cyclic transaction %s"), tac->getObjTraceId()));
				tac->completeReportRequests();
			}
		}

		if (actSpan < minSpan)
			minSpan = actSpan;
	}

	_TRACE(TL_DEB, TG_SRV, (_T("< startCyclicTransactions minSpan: %lu"), minSpan));
	return minSpan;
}

DWORD GenericServer::updateTransactions(
	IN DWORD loopStart,
	IN DWORD minSpan)
{
	_TRACE(TL_DEB, TG_SRV, (_T("> updateTransactions")));
	GenericPointer<OPCTransaction> tac;
	POSITION pos;
	POSITION thisPos;
	DWORD dueTime;
	DWORD actSpan;

	m_transactionLock.Lock();
	m_transactionListChanged = FALSE;
	pos = m_transaction.GetHeadPosition();
	m_transactionLock.Unlock();

	while (pos)
	{
		m_transactionLock.Lock();
		if (m_transactionListChanged)
		{
			// check current position!
			POSITION checkPos;
			BOOL found = FALSE;

			checkPos = m_transaction.GetHeadPosition();
			while (checkPos)
			{
				if (checkPos == pos)
				{
					found = TRUE;
					break;
				}
				m_transaction.GetNext(checkPos);
			}

			if (!found)
				pos = m_transaction.GetHeadPosition();

			_TRACE(TL_DEB, TG_SRV, (_T("transaction changed; pos of current tac 0x%X; found %s"), pos, BOOL2STR(found)));
			m_transactionListChanged = FALSE;
			m_transactionLock.Unlock();
			continue;
		}

		thisPos = pos;
		tac = m_transaction.GetNext(pos);
		tac.addRef();
		m_transactionLock.Unlock();

		if (tac->getType() != OPCTransaction::write)
			dueTime = m_dueTimeRead;
		else
			dueTime = m_dueTimeWrite;

		_TRACE(TL_DEB, TG_SRV, (_T("update transaction %s dueTime: %lu"), tac->getObjTraceId(), dueTime));
		actSpan = INFINITE;
		if (tac->update(loopStart, dueTime, &actSpan))
		{
			_TRACE(TL_DEB, TG_SRV, (_T("completed transaction %s"), tac->getObjTraceId()));
			if (tac->getType() != OPCTransaction::cyclic)
			{
				if (tac->getType() == OPCTransaction::report)
				{
					// remove all report requests after completed the transaction
					GenericList<GenericRequest> reqList(tac->getRequestList());
					GenericRequest *req;
					POSITION reqpos, reqcurpos;

					reqpos = reqList.getStartPosition();
					while(reqpos)
					{
						reqcurpos = reqpos;
						req = reqList.getNext(reqpos);
						if (req->getObjectState() & GENREQ_FLAG_PROCESSED)
						{
							reqList.getList()->RemoveAt(reqcurpos);
							req->release();
						}
					}
				}
				else
				{
					m_transactionLock.Lock();
					if (m_transactionListChanged)
					{
						thisPos = m_transaction.Find((OPCTransaction *)tac);
						_TRACE(TL_DEB, TG_SRV, (_T("transaction changed; pos of remove tac 0x%X"), thisPos));
					}
					if (thisPos)
					{
						m_transaction.RemoveAt(thisPos);// remove from list
						tac->release();					// free transaction (list)
					}
					m_transactionLock.Unlock();
				}
			}
		}

		if (actSpan < minSpan)
			minSpan = actSpan;
	}

	_TRACE(TL_DEB, TG_SRV, (_T("< updateTransactions minSpan: %lu"), minSpan));
	return minSpan;
}


BOOL GenericServer::insertCyclicRequest(
	IN GenericItem *item)
{
	if (isPublicGroups())
		return FALSE;

	if (!item)
		return FALSE;

	GenericPointer<GenericGroup> grp = (GenericGroup *)item->getParentBranch();

	item->setCyclicData(new GenericItemCyclicData);

	if (grp->getUpdateMode() == GenericGroup::report)
		return TRUE;

	_TRACE(TL_DEB, TG_SRV, (_T("> insertCyclicRequest for item %s"), item->getObjTraceId()));
	GenericPointer<OPCEngine> engine = ::getOPCEngine();
	GenericPointer<GenericCreator> creator = engine->getCreator();
	GenericPointer<GenericRequest> req;
	GenericPointer<GenericRequest> reqNow;
	GenericPointer<OPCTransaction> tac = grp->getCyclicTransaction();
	GenericPointer<GenericItemTag> tag;
	BOOL ret;

	req = tac->getRequestByItem(item);
	if ((GenericRequest *)req)
		return FALSE;

	// get the value into cache
	tag = item->getItemTag();
	reqNow = creator->createRequest(this, item, tag, GenericRequest::read, 
				GenericRequest::asyncDevice, (GenericRequest::requestDeviceIOMode)tag->getDeviceIOMode());
	reqNow->setItem(item);
	reqNow->start();

	// add cyclic request to the group
	req = creator->createRequest(this, item, tag, GenericRequest::read, 
				GenericRequest::cyclic, (GenericRequest::requestDeviceIOMode)tag->getDeviceIOMode());
	if (m_minRequestLoopTime == INFINITE)
		req->setCompletedEvent(m_events[START_EVENT]);
	req->setItem(item);
	_TRACE(TL_INF, TG_SRV, (_T("inserted cyclic request %s"), req->getObjTraceId()));

	ret = tac->insertRequest(req);
	_TRACE(TL_DEB, TG_SRV, (_T("< insertCyclicRequest %s"), BOOL2STR(ret)));
	return ret;
}

BOOL GenericServer::removeCyclicRequest(
	IN GenericItem *item)
{
	if (isPublicGroups())
		return FALSE;

	if (!item)
		return FALSE;

	GenericPointer<GenericGroup> grp = (GenericGroup *)item->getParentBranch();
	BOOL ret = FALSE;

	_TRACE(TL_DEB, TG_SRV, (_T("> removeCyclicRequest for item %s"), item->getObjTraceId()));
	GenericPointer<OPCTransaction> tac = grp->getCyclicTransaction();
	GenericPointer<GenericRequest> req;

	req = tac->getRequestByItem(item);
	if ((GenericRequest *)req)
	{
		_TRACE(TL_INF, TG_SRV, (_T("removed cyclic request %s"), req->getObjTraceId()));
		ret = tac->removeRequest(req);
	}
	item->setCyclicData(NULL);

	return ret;
}


void GenericServer::addTransaction(
	IN OPCTransaction *trans)
{
	if (isPublicGroups())
		return;

	m_transactionLock.Lock();
	m_transactionListChanged = TRUE;
	trans->addRef();
	m_transaction.AddTail(trans);
	m_transactionLock.Unlock();
	_TRACE(TL_INF, TG_SRV, (_T("added transaction %s"), trans->getObjTraceId()));
	::SetEvent(m_events[START_EVENT]);
}

BOOL GenericServer::removeTransaction(
	IN OPCTransaction *trans,
	IN OPTIONAL DWORD transId)
{
	if (isPublicGroups())
		return FALSE;

	_TRACE(TL_DEB, TG_SRV, (_T("> removeTransaction ptr:0x%X id:0x%X"), trans, transId));
	OPCTransaction *tac;
	POSITION pos;

	m_transactionLock.Lock();

	if (trans)
	{
		pos = m_transaction.Find(trans);
		tac = trans;
	}
	else
	{
		POSITION thisPos;

		pos = m_transaction.GetHeadPosition();
		while (pos)
		{
			thisPos = pos;
			tac = m_transaction.GetNext(pos);
			if (tac->getTransactionId() == transId)
			{
				pos = thisPos;
				break;
			}
		}
	}

	if (pos)
	{
		m_transaction.RemoveAt(pos);
		m_transactionListChanged = TRUE;
		m_transactionLock.Unlock();
		_TRACE(TL_INF, TG_SRV, (_T("removed transaction %s"), tac->getObjTraceId()));
		tac->cancel();
		tac->release();
		return TRUE;
	}
	else
	{
		m_transactionLock.Unlock();
		_TRACE(TL_DEB, TG_SRV, (_T("< removeTransaction; transaction not found")));
		return FALSE;
	}
}


//-----------------------------------------------------------------------------
// sendShutdownRequest
//
// - send shutdown request to the OPC client
//
// returns:
//		S_OK    - send shutdown request
//		S_FALSE - tried to shutdown public groups server
//		E_FAIL  - no COM server object
//
HRESULT GenericServer::startShutdownRequest(
	IN LPCTSTR reason,
	IN HANDLE event)
{
	GenericPointer<OPCEngine> engine = ::getOPCEngine();
	HRESULT res = S_OK;

	if (isPublicGroups())
		return S_FALSE;

	if (m_me) 
		engine->addShutdownRequest(this, reason, event);
	else	
		res = E_FAIL;

	return res;
}


HRESULT GenericServer::sendShutdownRequest(
	IN LPCTSTR reason)
{
	if (isPublicGroups())
		return S_FALSE;

	HRESULT res;

	m_meSync.Lock();
	OPCObject<OPCServer> *srv = (OPCObject<OPCServer> *)m_me;
	if (srv)
		srv->internalAddRef();
	m_meSync.Unlock();

	if (srv)
	{ 
		res = srv->sendShutdownRequest(reason);
		srv->internalRelease();
	}
	else	
		res = E_FAIL;

	return res;
} // sendShutdownRequest


//-----------------------------------------------------------------------------
// disconnectClient
//
// - disconnect the OPC client (forced shutdown)
//
void GenericServer::disconnectClient(void)
{
	OPCGroupObj *grp;
 	GenericList<GenericBranch> groupList;
	GenericGroup *group;
	POSITION posGrp;
	DWORD extRefs;

	if (isPublicGroups())
		return;

	m_meSync.Lock();
	OPCObject<OPCServer> *srv = (OPCObject<OPCServer> *)m_me;
	if (srv)
		srv->internalAddRef();
	m_meSync.Unlock();

	if (srv == NULL)
		return;

	_WARNING(TG_ENG, OPCENGINE_WRN_DISCONNECT_CLIENT, (_T("disconnect client %s"), getName()));

	_TRACE(TL_DEB, TG_SRV, (_T("disconnecting refCnt:%u intRefCnt:%u"), srv->m_dwRef, (ULONG)srv->m_internalCnt));
	// remove the groups
	groupList = getBranchList();
	posGrp = groupList.getStartPosition();
	while(posGrp)
	{
		group = (GenericGroup *)groupList.getNext(posGrp);
		grp = (OPCGroupObj *)group->m_serverHandle;

		_TRACE(TL_DEB, TG_SRV, (_T("disconnecting group %s"), group->getObjTraceId()));
		if (grp) 
		{
			extRefs = grp->m_dwRef - grp->m_internalCnt;
			_TRACE(TL_DEB, TG_GRP, (_T("disconnecting refCnt:%u intRefCnt:%u"), grp->m_dwRef, (ULONG)grp->m_internalCnt));
			removeGroup(group->getServerHandle(), TRUE);
			// release the internal reference added when creating the group via AddGroup
			// or CloneGroup (in SODaSComServer::createOPCGroup)
			grp->releaseServer();
			while (grp->internalRelease())
				;
			if (extRefs)
				CoDisconnectObject((IOPCItemMgt *)grp, 0);
		}
	}
	groupList.unlock();
	extRefs = srv->m_dwRef - srv->m_internalCnt;
	while (srv->internalRelease())
		;
	if (extRefs)
		CoDisconnectObject((IOPCServer *)srv, 0);
} // disconnectClient


BOOL GenericServer::checkClientConnection(void)
{
	BOOL conOK = TRUE;
    BOOL checked = FALSE;
    BOOL extRef = FALSE;

	if (!isPublicGroups())
	{
		m_meSync.Lock();
		OPCObject<OPCServer> *srv = (OPCObject<OPCServer> *)m_me;
		if (srv)
			srv->internalAddRef();
		m_meSync.Unlock();

		if (!srv)
			return FALSE;

		if (srv->connectedShutdownInterface())
		{
			// OPC Spec V2.0 client
            checked = TRUE;
			conOK = srv->checkClientConnection();
		}

		if (conOK) 
		{
			// OPC Spec V1.0A client
			// connection check only possible if advised async group I/O
			POSITION posGrp;
			GenericGroup *group;	
			AdviseMap* adviseMap;
			OPCAdvise advise;
			POSITION pos;
			DWORD connection;
			IUnknown *unk;

			HRESULT res;
		 	GenericList<GenericBranch> groupList(getBranchList());

			posGrp = groupList.getStartPosition();
			while (posGrp && conOK)
			{
				group = (GenericGroup *)groupList.getNext(posGrp);

				CSingleLock lock(group->getAdvisesLockObj(), TRUE);
				adviseMap = group->getAdvises();
		
				pos = adviseMap->GetStartPosition();
				while (pos && conOK)
				{
					checked = TRUE;
					adviseMap->GetNextAssoc(pos, connection, advise);

					res = advise.m_adviseSink->QueryInterface(IID_IOPCServer, (void **)&unk);
					if (FAILED(res))
					{
						if ((res == 0x800706BA)	|| (res == 0x800706BF) || (res == 0x80010108)) // RPC server unavailable
						{
							conOK = FALSE;
							posGrp = NULL;
							pos = NULL;
							break;
						}
					}
					else
						unk->Release();
				}
                lock.Unlock();

                if (conOK) {
                    group->m_meSync.Lock();
	                OPCObject<OPCGroup> *opcgrp = (OPCObject<OPCGroup> *)group->m_me;
	                if (opcgrp)
		                opcgrp->internalAddRef();
                    group->m_meSync.Unlock();

		            if (opcgrp)
                    {
		                CSingleLock opclock(opcgrp->getAdvisesLockObj(), TRUE);
		                CComDynamicUnkArray *advises = opcgrp->getAdvises();
		                IUnknown** pp, *unk;
		                IOPCDataCallback *dataCallback;

		                pp = advises->begin();
		                while (pp < advises->end() && conOK)
		                {
			                if (*pp != NULL)
			                {
    					        checked = TRUE;
				                dataCallback = (IOPCDataCallback *)*pp;
					            res = dataCallback->QueryInterface(IID_IOPCServer, (void **)&unk);
					            if (FAILED(res))
					            {
						            if ((res == 0x800706BA)	|| (res == 0x800706BF) || (res == 0x80010108)) // RPC server unavailable
						            {
							            conOK = FALSE;
							            posGrp = NULL;
							            pos = NULL;
							            break;
						            }
					            }
					            else
						            unk->Release();
			                }
			                pp++;
		                }

		                opclock.Unlock();

                        ULONG refCnt;
	                    ULONG intRef = opcgrp->internalRelease(&refCnt);
                        if (refCnt > intRef)
                            extRef = TRUE;
                    }
                }
			}
		}

        ULONG refCnt;
		ULONG intRef = srv->internalRelease(&refCnt);
        if (refCnt > intRef)
            extRef = TRUE;
	}

    if (!checked && !extRef)
        conOK = FALSE;

	return conOK;
}

GenericTagProperty *GenericServer::createTagProperty(
	IN GenericTag *tag,
	IN LPCTSTR itemId,
	IN DWORD propId)
{
	GenericList<GenericTagProperty> propList;
	GenericTagProperty* tagProp = NULL;

	propList = tag->getPropertyList();
	tagProp = propList.find(propId);
	if (!tagProp)
	{ // no property with the id in tag try to create
		if ((!isStandardTagProperty(propId)) && 
			(!isDynamicTagProperty(itemId, propId)))
		{
			_TRACE(TL_INF, TG_SRV, (_T("%s has no property %lu"), itemId, propId));
			return NULL;
		}

		tagProp = tag->addProperty(propId);
		if (tagProp)
			tagProp->setDynamic(TRUE);
	}

	return tagProp;
}

HRESULT GenericServer::syncRead(
	IN RequestList &reqList,
	IN OPCDATASOURCE source,	// data source: cache or device 
	IN LPCTSTR user)
{ 
	_TRACE(TL_DEB, TG_SRV, (_T("> syncRead")));
	POSITION pos;
	GenericRequest *req;	
	GenericPointer<GenericCache> cache;	
	GenericPointer<GenericItem> item;	
	GenericPointer<GenericItemTag> tag;
	HANDLE *events;
	DWORD i;
	DWORD waitCnt;
	DWORD waitTime;
	DWORD startWaitTime;
	DWORD timeSpan;
	DWORD reqCnt = reqList.GetCount();

	// device read must wait for events
	if (source == OPC_DS_DEVICE)
	{
		_TRACE(TL_INF, TG_SRV, (_T("sync device read; %d items"), reqCnt));
		events = (HANDLE *)malloc(sizeof(HANDLE) * reqCnt);
		if (!events)
			return E_OUTOFMEMORY;
	}
	else
	{
		_TRACE(TL_INF, TG_SRV, (_T("sync cache read; %d items"), reqCnt));
	}

	// start all requests
	i = 0;
	pos = reqList.GetHeadPosition();
	while (pos)
	{
		req = (GenericRequest *)reqList.GetNext(pos);
		req->start();
		if (source == OPC_DS_DEVICE)
		{
			// get the completed event
			events[i] = req->getCompletedEvent();
			i++;
		}
	}

	if (source == OPC_DS_DEVICE)
	{
		_TRACE(TL_DEB, TG_SRV, (_T("wait for completion")));

		// wait for completion
		i = 0;
		startWaitTime = ::GetTickCount();
		do
		{
			timeSpan = getTimeSpan(startWaitTime, ::GetTickCount());		
			if (timeSpan < getDuetimeRead())
				waitTime =  getDuetimeRead() - timeSpan;
			else
				waitTime = 0;
			waitCnt = ((reqCnt - i) <= MAXIMUM_WAIT_OBJECTS) ? (reqCnt - i) : MAXIMUM_WAIT_OBJECTS;
			::WaitForMultipleObjects(waitCnt, &events[i], TRUE, waitTime);
			i += waitCnt;
		} while (i < reqCnt);

		free(events);

		_TRACE(TL_DEB, TG_SRV, (_T("all requests completed or time ellapsed")));
	}

	// copy values from cache to OPCITEMSTATE structure
	pos = reqList.GetHeadPosition();
	while (pos)
	{
		req = (GenericRequest *)reqList.GetNext(pos);
		if (req->isCompleted())
		{
			if (req->getResult() == S_OK)
			{
				cache = req->getCache();

				if (req->getType() != GenericRequest::syncProperty)
				{
					OPCITEMSTATE *value;

					item = req->getItem();
					value = req->getItemStateAddress();

					cache->getValue(req, item->getReqDatatype(), &value->vDataValue);
					value->hClient = item->getClientHandle();
					cache->getTimeStamp(req, &value->ftTimeStamp);
					value->wQuality = cache->getQuality(req);
					if (source == OPC_DS_CACHE)
					{
						if (!item->isActive())
							value->wQuality = OPC_QUALITY_OUT_OF_SERVICE;
					}
				}
				else
				{
					tag = req->getItemTag();		
					LPVARIANT value;

					value = req->getReadValueAddress();
					cache->getValue(req, tag->getNativeDatatype(), value);
				}
			}
			_TRACE(TL_DEB, TG_SRV, (_T("request %s completed 0x%X"), req->getObjTraceId(), req->getResult()));
		}
		else
		{
			// not completed after due time
			// -> abort request
			_TRACE(TL_DEB, TG_SRV, (_T("cancel request %s; not completed in time"), req->getObjTraceId()));
			req->cancel(E_FAIL, TRUE);			
		}
	}

	_TRACE(TL_DEB, TG_SRV, (_T("< syncRead")));
	return S_OK; 
}

//-----------------------------------------------------------------------------
// accessToMethod
//
// - allow the user to call the OPC function of the interface
//
// returns:
//		TRUE  - allow access
//		FALSE - deny access
//
BOOL GenericServer::accessToMethod(
	IN REFIID iid,				// interface id of called interface
	IN enum methodOPCServer method,	// method called
	IN LPCTSTR user)			// user name
{ return TRUE; } // accessToMethod




//-----------------------------------------------------------------------------
// OPCRootOPC                                                                 |
//-----------------------------------------------------------------------------

OPCRootOPC::OPCRootOPC(void)
 : GenericBranch(GENOBJ_TYPE_OPC_ROOT, GENOBJ_TYPE_BRANCH, 0, GENLIST_CREATE_LIST, 0, 1)
{
	m_objectTraceId.Format(_T("[%s|%8.8X| %s]"), getObjTypeString(), this, _T("OPC root"));
}

//-----------------------------------------------------------------------------
// addLeaf
//
// - don't add leaf into OPC root object
//
// returns:
//		FALSE - not added
//
BOOL OPCRootOPC::addLeaf(
	IN GenericLeaf *newLeaf, 
	IN OPTIONAL int index)
{
	return FALSE;
} // addLeaf


//-----------------------------------------------------------------------------
// addBranch
//
// - adds branch to branch list, if branch is a server object
// - adds branch to branch list, if branch is the first public groups object
//
// returns:
//		TRUE  - added
//		FALSE - not added
//
BOOL OPCRootOPC::addBranch(
	IN GenericBranch *newBranch, 
	IN OPTIONAL int index)
{
	if (!newBranch)
		return FALSE;

	if (newBranch->is(GENOBJ_TYPE_SERVER))
	{ // branch is a server object -> add
		if (!((GenericServer *)newBranch)->isPublicGroups())
			return GenericBranch::addBranch(newBranch, index);
		else 
		{ // branch is a public groups object -> check if public groups already exists
			POSITION pos;
			GenericServer *branch;
		 	GenericList<GenericBranch> list(getBranchList());

			pos = list.getStartPosition();
			while(pos)
			{
				branch = (GenericServer *)list.getNext(pos);
				if (branch->isPublicGroups())
				{ // there one public groups object
					return FALSE;
				}
			}
			list.unlock();
			return GenericBranch::addBranch(newBranch, index);	// add to branch list
		}
	}
	else
		return FALSE;
} // addBranch





/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/opcDynServer.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: opcDynServer.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/opcDynServer.cpp $
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

/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "Security.h"
#include "opcDynServer.h"
#include "opcItem.h"
#include "SysVar.h"
#include "SysVarPool.h"
#include "NSpaceHolder.h"
#include "iecResource.h"

//----  Local Defines:   -------------------------------------------*
#define _MAX_IEC_VARS       200

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
static char szversioninfo[] = "$Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/opcDynServer.cpp 1     28.02.07 19:06 Ln $";


/////////////////////////////////////////////////////////////////////
//------------- MEMBER FUNCTIONS OF CLASS CDynamicOpcServer
/////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------*
/**
 * constructor
 *
 * @param           void
 * @return          void
 * @exception       -
*/
CDynamicOpcServer::CDynamicOpcServer(BOOL bPublicGroups)
  : GenericServer(bPublicGroups)
{
    if (!isPublicGroups())
        _Module.Lock();
    m_sessionRights = g_security.GetUserRights(m_strPassword);
    setDuetimeRead(60 * 1000);
    setDuetimeWrite(60 * 1000);
}

//------------------------------------------------------------------*
/**
 * destructor
 *
 * @param           void
 * @return          void
 * @exception       -
*/
CDynamicOpcServer::~CDynamicOpcServer()
{
    if (!isPublicGroups())
        _Module.Unlock();
}

//------------------------------------------------------------------*
/**
 * changeDynamicBrowsePosition
 *
 * @param           void
 * @return          void
 * @exception       -
*/
//HRESULT
//    CDynamicOpcServer::changeDynamicBrowsePosition(IN OPCBROWSEDIRECTION direction, IN LPCTSTR name)
//{
//}

// callbacks for dynamic namespace
//------------------------------------------------------------------*
/**
 * isDynamicNode
 *
 * @param           void
 * @return          void
 * @exception       -
*/
BOOL
    CDynamicOpcServer::isDynamicNode(IN LPCTSTR fullName)
{
    BOOL bDynamic = FALSE;
    HRESULT hr = S_FALSE;
    CStringList listParsedName;
    GenericPointer<IEC_Resource>    pRes;
    CString strFullName = fullName;
    strFullName.MakeLower();
    hr = GetDynamicResource(&pRes, listParsedName, strFullName);
    if (SUCCEEDED(hr))
    {
        if (!pRes) {
            return FALSE;
        }
        // have resource, remove resource name and hand over to resource
        listParsedName.RemoveHead();

        // walk down in control to see if node exists
        bDynamic = pRes->isDynamicNode(listParsedName);
    }
	return bDynamic;
}

//------------------------------------------------------------------*
/**
 * isDynamicTag
 *
 * @param           void
 * @return          void
 * @exception       -
*/
BOOL
    CDynamicOpcServer::isDynamicTag(IN LPCTSTR fullName)
{
    BOOL bDynamic = FALSE;
    HRESULT hr = S_FALSE;
    CStringList listParsedName;
    GenericPointer<IEC_Resource>    pRes;
    CString strFullName = fullName;
    strFullName.MakeLower();
    hr = GetDynamicResource(&pRes, listParsedName, strFullName);
    if (SUCCEEDED(hr))
    {
        if (!pRes) {
            return FALSE;
        }
        // have resource, remove resource name and hand over to resource
        listParsedName.RemoveHead();

        // walk down in control to see if node exists
        bDynamic = pRes->isDynamicTag(listParsedName);
    }
	return bDynamic;
}

//-----------------------------------------------------------------------------
// createTag
//
// - create tag dynamic from fullName
// 
// returns:
//		NULL - can't create tag
//
GenericTag *CDynamicOpcServer::createTag(
	IN LPCTSTR fullName)
{
	GenericTag *tag = NULL;

//  +++ this function is overloaded to save 50% CPU +++
//	if (isDynamicTag(fullName))
//	{
		GenericPointer<OPCEngine> engine = ::getOPCEngine();
		GenericPointer<GenericCreator> creator = engine->getCreator();
		tag = creator->createTag(this, fullName);
//	}

	return tag;
} // createTag



//------------------------------------------------------------------*
/**
 * getDynamicChildNames
 *
 * @param           void
 * @return          void
 * @exception       -
*/
HRESULT
    CDynamicOpcServer::getDynamicChildNames(IN OPCBROWSETYPE browseType, 
			                IN LPCTSTR filterCriteria, IN VARTYPE dataTypeFilter, 
			                IN DWORD accessRightsFilter, OUT CStringList &nameList)
{
    HRESULT hr = S_FALSE;
    CStringList listParsedNames;
    GenericPointer<IEC_Resource>    pRes;
    hr = GetDynamicResource(&pRes, listParsedNames, m_curBrowsePath);
    if (SUCCEEDED(hr))
    {
        // special handling for browsing from root
        if (m_curBrowsePath.IsEmpty())
        {
            BranchList dynList;
            hr = NameSpaceHolder::GetDynResourceList(&dynList);
            if (FAILED(hr)) {
                return S_FALSE;
            }
            while (!dynList.IsEmpty())
            {
                IEC_Resource *pDynRes = (IEC_Resource *) dynList.RemoveHead();
                if (pDynRes == NULL) {
                    continue;
                }
                switch (browseType) {
                case OPC_BRANCH:
                    nameList.AddTail(pDynRes->getName());
                    break;
                case OPC_FLAT:
                    hr = pDynRes->FillChildrenList(nameList, pDynRes->getName(), listParsedNames, browseType, filterCriteria, dataTypeFilter, accessRightsFilter);
                    break;
                case OPC_LEAF:
                default:
                    break;
                }
                pDynRes->release();
            }
            return S_OK;
        }
        if (!pRes) {
            return S_FALSE;
        }
        // have resource, remove resource name and hand over to resource
        listParsedNames.RemoveHead();

        // walk down in control
        // deliver list of requested children
        hr = pRes->FillChildrenList(nameList, m_curBrowsePath, listParsedNames, browseType, filterCriteria, dataTypeFilter, accessRightsFilter);
    }
	return hr;
}


//------------------------------------------------------------------*
/**
 * getDynamicBrowseAccessPaths
 *
 * @param           void
 * @return          void
 * @exception       -
*/
HRESULT
    CDynamicOpcServer::getDynamicBrowseAccessPaths(IN LPCTSTR name, OUT CStringList &pathList)
{
    UNUSED_ALWAYS(name);
    UNUSED_ALWAYS(pathList);
    return S_FALSE;
}

//------------------------------------------------------------------*
/**
 * getBandwidth
 *
 * @param           void
 * @return          current bandwith in percent
 * @exception       -
*/
DWORD
    CDynamicOpcServer::getBandwidth(void) 
{
    DWORD   dwItemCount = NameSpaceHolder::GetItemsPerSecond();

    return ((dwItemCount * 100) / _MAX_IEC_VARS);
}

//------------------------------------------------------------------*
/**
 * createDynamicTag
 *
 * @param           void
 * @return          void
 * @exception       -
*/
GenericTag *
    CDynamicOpcServer::createDynamicTag(IN LPCTSTR fullName)
{
    HRESULT hr = S_FALSE;
    CStringList listParsedName;
    GenericPointer<IEC_Resource>    pRes;
    CString strFullName = fullName;
    strFullName.MakeLower();
    hr = GetDynamicResource(&pRes, listParsedName, strFullName);
    if (SUCCEEDED(hr))
    {
        if (!pRes) {
            return FALSE;
        }
        // have resource, remove resource name and hand over to resource
        listParsedName.RemoveHead();

        // create tag
        return pRes->createDynamicTag(listParsedName, strFullName);
    }
    return NULL;
}

//------------------------------------------------------------------*
/**
 * GetDynamicResource
 *
 * @param           void
 * @return          void
 * @exception       -
*/
HRESULT
    CDynamicOpcServer::GetDynamicResource(OUT IEC_Resource **ppRes, OUT CStringList &listParsedNames, LPCTSTR pszPath)
{
    // split current path
    if (!NameSpaceHolder::SplitVariableName(pszPath, listParsedNames)) {
        return S_FALSE;
    }
    if (listParsedNames.IsEmpty()) {
        // have no dynamic tags/nodes below root (besides resource, but handled separately)
        return S_FALSE;
    }
    // decide if path points to control or system variables
    if (!NameSpaceHolder::IsDynamicResource(listParsedNames, ppRes)) {
        return S_FALSE;
    }
    return S_OK;
}

/* ---------------------------------------------------------------------------- */

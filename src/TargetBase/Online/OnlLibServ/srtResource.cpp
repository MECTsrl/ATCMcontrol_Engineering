/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/srtResource.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: srtResource.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/srtResource.cpp $
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
#include <genSrv.h>
#include "Security.h"
#include "iecResource.h"
#include "srtResource.h"
#include "opcError.h"
#include "opcItem.h"
#include "SysVar.h"
#include "SysVarPool.h"
#include "iecVar.h"
#include "NSpaceHolder.h"
#include "CSC_OnlineErr.h"
#include "config.h"


/////////////////////////////////////////////////////////////////////
//------------- MEMBER FUNCTIONS OF CLASS SRT_Resource
/////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------*
/**
 * IsDirectVarAddress
 * syntax for direct address:
 *    srt0.__system.direct.X.<seg>.<off>.<bit>
 *    srt0.__system.direct.S.<seg>.<off>.<len>
 *    srt0.__system.direct.B|W|D|L|F|G.<seg>.<off>
 * 
 * F = float, G = double
 *
 * @param           path          name of pc where resouce is to be started
 * @return          retval
 * @exception       -
 */

BOOL IsDirectVarAddress(LPCTSTR path, CSC_VARADDR &retval)
{
    LPCTSTR sep = _T(".");
    CString strPath = path;
    LPTSTR p = _tcstok((LPTSTR)(LPCTSTR)strPath, sep);
    if (p == NULL || _tcsicmp(p, g_csSystem) != 0)
        return FALSE;
    p = _tcstok(NULL, sep);
    if (p == NULL || _tcsicmp(p, _T("direct")) != 0)
        return FALSE;
    p = _tcstok(NULL, sep);
    if (p == NULL || p[0] == _T('\0') || p[1] != _T('\0'))
        return FALSE;
    _tcsupr(p);
    switch(p[0]) {
          case _T('X'):
              retval.iecType = BL_BOOL;
              break;
          case _T('B'):
              retval.iecType = BL_BYTE;
              break;
          case _T('W'):
              retval.iecType = BL_WORD;
              break;
          case _T('D'):
              retval.iecType = BL_DWORD;
              break;
          case _T('L'):
              retval.iecType = BL_LWORD;
              break;
          case _T('S'):
              retval.iecType = BL_STRING;
              break;
          case _T('F'):
              retval.iecType = BL_REAL;
              break;
          case _T('G'):
              retval.iecType = BL_LREAL;
              break;
          default:
              return FALSE;
    }
    retval.task = 0;
    p = _tcstok(NULL, sep);
    if (p == NULL)
        return FALSE;
    retval.segment = _ttoi(p);
    p = _tcstok(NULL, sep);
    if (p == NULL)
        return FALSE;
    retval.offset = _ttoi(p);
    retval.bitnr = 0;
    if (retval.iecType == BL_BOOL) {
        p = _tcstok(NULL, sep);
        if (p == NULL)
            return FALSE;
        retval.bitnr = _ttoi(p);
    }
    retval.strlen = 0;
    if (retval.iecType == BL_STRING) {
        p = _tcstok(NULL, sep);
        if (p == NULL)
            return FALSE;
        retval.strlen = _ttoi(p);
    }

    p = _tcstok(NULL, sep);
    if (p != NULL)
        return FALSE;

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * constructor
 *
 * @param           pszPC_Name          name of pc where resouce is to be started
 * @return          void
 * @exception       -
 */
SRT_Resource::SRT_Resource(LPCTSTR pszPC_Name, int iEnumRes /*= 0*/)
  : m_downloadCounter(-1),
    m_iEnumRes(iEnumRes)
{
    // save configuration name
    assert(pszPC_Name != NULL);
    m_sConfigName = pszPC_Name;
    m_strSystemName = g_csResSRT0;
    setName(m_strSystemName);
}

//------------------------------------------------------------------*
/**
 * destructor
 *
 * @param           void
 * @return          void
 * @exception       -
*/
SRT_Resource::~SRT_Resource()
{
}


//------------------------------------------------------------------*
/**
 * create resource's system variables and add to my namsepace
 *
 * @param           
 * @return          HRESULT
 * @exception       -
*/
HRESULT
    SRT_Resource::CreateSysVars()
{
    HRESULT hr;
    hr = NameSpaceHolder::DeleteResourceSystemVariables(m_iEnumRes);
    if (FAILED(hr)) return hr;
    hr = NameSpaceHolder::CreateResourceSystemVariables(m_iEnumRes);
    if (FAILED(hr)) return hr;

    // add "__system" branch for sys vars
    // add general resource sys variables:
    GenericPointer<GenericBranch>    pSystem = (CSysVarPool*) NameSpaceHolder::GetResourceSysVars(m_iEnumRes);
    if (!!pSystem)
    {
        addBranch(pSystem);
        
        CSysVarPool *pPool = (CSysVarPool*) NameSpaceHolder::GetResourceDLoadSysVars(m_iEnumRes);
        if (pPool != NULL) {
            pSystem->addBranch(pPool);
            pPool->release();
        }
        pPool = (CSysVarPool*) NameSpaceHolder::GetResourceDebugSysVars(m_iEnumRes);
        if (pPool != NULL) {
            pSystem->addBranch(pPool);
            pPool->release();
        }
    }
    return S_OK;
}

//------------------------------------------------------------------*
/**
 * return IResource ptr of this resource
 * recreate COM instance of it if necessary
 * caller must Release(), when ptr no longer used !!
 *
 * @param           void
 * @return          HRESULT
 * @exception       -
 */
HRESULT
    SRT_Resource::OpenResourceInstance()
{
    CSingleLock lock(&m_cs,TRUE);

    // save current download counter to check for namespace changes
    m_downloadCounter++;

    // global system variables:
    CSysVarPool *pPool = (CSysVarPool*) NameSpaceHolder::GetGlobalSysVars();
    if (pPool != NULL)
    {
        // lookup in project and resource names
        GenericPointer<CSysVar> pVar = pPool->LookupSysVar(g_csSVarRes0Name);
        pPool->release();
        if (!!pVar)
        {
            GenericPointer<GenericCache> pCache = pVar->getCache();
            CComVariant Val;
            if (pCache->getValue(&Val))
            {
                if (SUCCEEDED(Val.ChangeType(VT_BSTR)))
                {
                    CString strResName = Val.bstrVal;
                    strResName.MakeLower();
                    if (strResName.IsEmpty())
                        setName(GetSystemName());
                    else
                        setName(strResName);
                }
            }
        }
    }

    return S_OK;
}

//------------------------------------------------------------------*
/**
 * CD1: used to update namespace after new download
 *
 * @param           pCounter    returns current counter
 * @return          HRESULT
 * @exception       -
*/
HRESULT SRT_Resource::GetCurrentDownloadCounter(long *pCounter)
{
    CSingleLock lock(&m_cs,TRUE);
    if (pCounter == NULL) {
        return E_POINTER;
    }
    *pCounter = m_downloadCounter;
    return S_OK;
}

//------------------------------------------------------------------*
/**
 * CD1: used to update namespace after new download
 *
 * @param           pCounter    returns current counter
 * @return          HRESULT
 * @exception       -
*/
long SRT_Resource::GetCachedDownloadCounter()
{
    return m_downloadCounter;
}

//------------------------------------------------------------------*
/**
 * CD1: check if namespace has changed after new download
 *
 * @param           void
 * @return          TRUE if changed
 * @exception       -
*/
BOOL SRT_Resource::CheckForNamespaceUpdate(void)
{
    long counter = 0;
    HRESULT hr = GetCurrentDownloadCounter(&counter);
    if (FAILED(hr)) {
        return TRUE;    // reconnect !!
    }
    BOOL bChanged = (m_downloadCounter != counter);
    m_downloadCounter = counter;
    return bChanged;
}

//------------------------------------------------------------------*
HRESULT
    SRT_Resource::FillChildrenList(OUT CStringList &listChildren,
                                             IN LPCTSTR pszCurrentPath, IN CStringList &parsedPath,
                                             OPCBROWSETYPE BrowseFilterType, LPCTSTR pszFilterCriteria,
                                             VARTYPE DataTypeFilter, DWORD AccessRightsFilter)
{
    HRESULT hr;

    BOOL    bSysVarPath = FALSE;
    if (!parsedPath.IsEmpty()) {
        bSysVarPath = NameSpaceHolder::IsSystemBranchName(parsedPath.GetHead());
    }
    // Step1: fill from sys var list:
    if (   parsedPath.IsEmpty()
        || bSysVarPath)
    {
        GenericPointer<GenericBranch>   pCurrBranch;
        hr = WalkToSystemBranch(parsedPath, &pCurrBranch);
        if (SUCCEEDED(hr))
        {
            POSITION pos;
            GenericList<GenericBranch>  pBranches;
            GenericList<GenericLeaf>    pLeaves;
            switch (BrowseFilterType) {
            case OPC_BRANCH:
                pBranches = pCurrBranch->getBranchList();
                for (pos = pBranches.getStartPosition(); pos != NULL; )
                {
                    GenericElement *pElem = pBranches.getNext(pos);
                    // check if filter criteriae match:
                    LPCTSTR pszName = pElem->getName();
                    BOOL bMatch = CheckFilterCriteria(pElem, pszName, BrowseFilterType, pszFilterCriteria, DataTypeFilter, AccessRightsFilter);
                    if (bMatch) {
                        listChildren.AddTail(pszName);
                    }
                }
                break;
            case OPC_LEAF:
                pLeaves = pCurrBranch->getLeafList();
                for (pos = pLeaves.getStartPosition(); pos != NULL; )
                {
                    GenericElement *pElem = pLeaves.getNext(pos);
                    // check if filter criteriae match:
                    LPCTSTR pszName = pElem->getName();
                    BOOL bMatch = CheckFilterCriteria(pElem, pszName, BrowseFilterType, pszFilterCriteria, DataTypeFilter, AccessRightsFilter);
                    if (bMatch) {
                        listChildren.AddTail(pszName);
                    }
                }
                break;
            case OPC_FLAT:
                // for each obj in name list, do a deep look down its trail
                hr = BuildFlatList(listChildren, pCurrBranch, pszCurrentPath,
                    BrowseFilterType, pszFilterCriteria,
                    DataTypeFilter, AccessRightsFilter);
                break;
            }
        }
        // we're done if current path points to system variable path!
        if (bSysVarPath) {
            return S_OK;
        }
    }

    // Step2: fill from resource's OD
    // save
    CSingleLock lock(CSC_GetConfigLock(), TRUE);
    CString strPath;
    POSITION position = parsedPath.GetHeadPosition();
    while (position!=NULL)
      {
          if (!strPath.IsEmpty())
              strPath += NameSpaceHolder::sm_cStdSeparatorChar;
          strPath += parsedPath.GetNext(position);
      }

    CMapVarDef *pStructDef = CSC_LookupStructDefByInstance(strPath);
    if (pStructDef == NULL) {
        return E_FAIL;
    }

    if (BrowseFilterType == OPC_FLAT)
    {
        // for each obj in name list, do a deep look down its trail
        hr = BuildFlatList(listChildren, pszCurrentPath, strPath,
                                 BrowseFilterType, pszFilterCriteria,
                                 DataTypeFilter, AccessRightsFilter);
    }
    else
    {
        // control variables:
        // loop thru all objects in this level:
        POSITION pos = pStructDef->GetStartPosition();
        while (pos != NULL)
        {
            CString name;
            CSC_VARDEF vardef;
            pStructDef->GetNextAssoc(pos, name, vardef);
            
            // check if filter criteriae match:
            BOOL bMatch = CheckFilterCriteria(&vardef, name, BrowseFilterType, pszFilterCriteria, DataTypeFilter, AccessRightsFilter);

            if (bMatch) {
                listChildren.AddTail(name);
            }
        }
    }
    
    return S_OK;
}

//------------------------------------------------------------------*
/**
 * isDynamicNode
 *
 * @param           
 * @return          TRUE if dynamic node
 * @exception       -
 * @see             
*/
BOOL
    SRT_Resource::isDynamicNode(CStringList &parsedPath)
{
    HRESULT hr;

    // Step1: try to find in sys var list:
    GenericPointer<GenericBranch> pSysBranch;
    hr = WalkToSystemBranch(parsedPath, &pSysBranch);
    if (SUCCEEDED(hr)) {
        return (!!pSysBranch);
    }

    // Step2: try to find in resource's OD
    // walk down to given node:
    CString strPath;
    POSITION position = parsedPath.GetHeadPosition();
    while (position!=NULL)
      {
          if (!strPath.IsEmpty())
              strPath += NameSpaceHolder::sm_cStdSeparatorChar;
          strPath += parsedPath.GetNext(position);
      }

    CSingleLock lock(CSC_GetConfigLock(), TRUE);

    CMapVarDef *pOD = CSC_LookupStructDefByInstance(strPath);

    return (pOD != NULL);
}

//------------------------------------------------------------------*
/**
 * isDynamicTag
 *
 * @param           
 * @return          TRUE if dynamic tag
 * @exception       -
 * @see             
*/
BOOL
    SRT_Resource::isDynamicTag(CStringList &parsedPath)
{
    HRESULT hr;
    // looking for leaf, save tag name, walk to node above...
    CString strPath;
    POSITION position = parsedPath.GetHeadPosition();
    while (position!=NULL)
      {
          if (!strPath.IsEmpty())
              strPath += NameSpaceHolder::sm_cStdSeparatorChar;
          strPath += parsedPath.GetNext(position);
      }

    CString strTag;
    if (!parsedPath.IsEmpty()) {
        strTag = parsedPath.RemoveTail();
    }
    // Step1: try to find in sys var list:
    GenericPointer<GenericBranch> pSysBranch;
    hr = WalkToSystemBranch(parsedPath, &pSysBranch);
    if (SUCCEEDED(hr))
    {
        GenericPointer<GenericLeaf> pSysTag;
        pSysTag = pSysBranch->findLeaf(strTag);
        if (!!pSysTag) {
            return TRUE;
        }
    }

    // Step2: try to find in resource's OD
    CSC_VARADDR dummy;
    return CSC_LookupVariableDef(strPath, dummy) ||
           IsDirectVarAddress(strPath, dummy);
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
    SRT_Resource::createDynamicTag(IN CStringList & parsedPath, LPCTSTR pszFullName)
{
    HRESULT hr;
    CString strPath;
    POSITION position = parsedPath.GetHeadPosition();
    while (position!=NULL)
      {
          if (!strPath.IsEmpty())
              strPath += NameSpaceHolder::sm_cStdSeparatorChar;
          strPath += parsedPath.GetNext(position);
      }

    // looking for leaf, save tag name, walk to node above...
    CString strTag;
    if (!parsedPath.IsEmpty()) {
        strTag = parsedPath.RemoveTail();
    }

    // Step1: try to find in sys var list:
    if (   !parsedPath.IsEmpty()
        && NameSpaceHolder::IsSystemBranchName(parsedPath.GetHead())) 
    {
        GenericPointer<GenericBranch> pSysBranch;
        hr = WalkToSystemBranch(parsedPath, &pSysBranch);
        if (SUCCEEDED(hr))
        {
            GenericTag *pTag = (GenericTag *) pSysBranch->findLeaf(strTag);
            return pTag;
        }
    }

    CSingleLock lock(&m_cs, TRUE);
    // Step2: try to find in resource's OD
    CSC_VARADDR varaddr;
    if (!(CSC_LookupVariableDef(strPath, varaddr)||
          IsDirectVarAddress(strPath, varaddr)) )
        return NULL;

    // try if it is a variable:
    IEC_Variable  *pTag = new IEC_Variable();

    // if ptr is != NULL, then we have a hit
    if (pTag != NULL) {
        TRACE(_T("Found variable '%s' in control\n"), pszFullName);
        pTag->SetParent(this);
        pTag->SetFullName(strPath);
        pTag->Attach(&varaddr, m_downloadCounter);
    }
    
    return pTag;
}

//------------------------------------------------------------------*
HRESULT
    SRT_Resource::RefreshVariable(IEC_Variable *pTag)
{
    CSingleLock lock(&m_cs, TRUE);
    CSC_VARADDR varaddr;
    assert(pTag != NULL);
    if ((CSC_LookupVariableDef(pTag->GetFullName(), varaddr)||
         IsDirectVarAddress(pTag->GetFullName(), varaddr)) )
        pTag->Attach(&varaddr, m_downloadCounter);
    else
        pTag->Attach(NULL, m_downloadCounter);
    return S_OK;
}

//------------------------------------------------------------------*
/**
 * WalkToSystemBranch
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT
    SRT_Resource::WalkToSystemBranch(CStringList &parsedPath, GenericBranch **ppBranch)
{
    assert(ppBranch != NULL);
    *ppBranch = NULL;

    GenericPointer<GenericBranch> pCurr = this;
    pCurr.addRef();    // assignment above does NOT increment ref count!!
    CString strNodeName;
    for (POSITION pos = parsedPath.GetHeadPosition(); pos != NULL; )
    {
        strNodeName = parsedPath.GetNext(pos);
//        pCurr->release();   // before copying new current branch, release old one !!
        pCurr = pCurr->findBranch(strNodeName);
        if (!pCurr) {
            return OPC_E_UNKNOWNITEMID;
        }
    }
    *ppBranch = pCurr;
    (*ppBranch)->addRef();

    return S_OK;
}

//------------------------------------------------------------------*
/**
 * BuildFlatList
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT
    SRT_Resource::BuildFlatList(CStringList &listObjNames, LPCTSTR pszCurrentPath, LPCTSTR pszRelPath,
                                 OPCBROWSETYPE BrowseFilterType, LPCTSTR pszFilterCriteria,
                                 VARTYPE DataTypeFilter, DWORD AccessRightsFilter)
{
    HRESULT hr;
    CSingleLock lock(CSC_GetConfigLock(), TRUE);

    CMapVarDef *pCurrentOD = CSC_LookupStructDefByInstance(pszRelPath);
    if (pCurrentOD == NULL) {
        return S_OK;
    }

    // loop thru all objects in this level:
    // Phase 1: first only add all leaves, all branches are added to a temporary list
    POSITION pos = pCurrentOD->GetStartPosition();
    while (pos != NULL)
    {
        CString name;
        CSC_VARDEF vardef;
        pCurrentOD->GetNextAssoc(pos, name, vardef);

        // must have OD as branch!
        if (vardef.vardefType != CSC_VARDEF_STRUCT)
        {
            // add this leaf with prepended path to list
            CString strName = pszCurrentPath;
            if (_tcslen(pszCurrentPath) > 0) {
                strName += NameSpaceHolder::sm_cStdSeparatorChar;
            }
            strName += name;
            // add this leave to flat list:
            BOOL bMatch = CheckFilterCriteria(&vardef, name, BrowseFilterType, pszFilterCriteria, DataTypeFilter, AccessRightsFilter);

            if (bMatch) {
                listObjNames.AddTail(strName);
            }
        }
    }

    // Phase 2: walk thru branches when all leaves are already in item list
    pos = pCurrentOD->GetStartPosition();
    while (pos != NULL)
    {
        CString name;
        CSC_VARDEF vardef;
        pCurrentOD->GetNextAssoc(pos, name, vardef);

        // must have OD as branch!
        if (vardef.vardefType == CSC_VARDEF_STRUCT)
        {
            CString strPath = pszCurrentPath;
            if (_tcslen(pszCurrentPath) > 0) {
                strPath += NameSpaceHolder::sm_cStdSeparatorChar;
            }
            strPath += name;

            CString strRelPath = pszRelPath;
            if (_tcslen(pszRelPath) > 0) {
                strRelPath += NameSpaceHolder::sm_cStdSeparatorChar;
            }
            strRelPath += name;
            // walk down recursively !!
            hr = BuildFlatList(listObjNames, strPath, strRelPath,
                                BrowseFilterType, pszFilterCriteria, DataTypeFilter, AccessRightsFilter);
        }
    }

    return S_OK;
}

//------------------------------------------------------------------*
/**
 * BuildFlatList
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT
    SRT_Resource::BuildFlatList(CStringList &listObjNames, GenericBranch *pCurrBranch, LPCTSTR pszCurrentPath,
                                 OPCBROWSETYPE BrowseFilterType, LPCTSTR pszFilterCriteria,
                                 VARTYPE DataTypeFilter, DWORD AccessRightsFilter)
{
    HRESULT hr;
    POSITION    pos;
    LPCTSTR     pszName = NULL;

    // Phase 1: first only add all leaves
    GenericList<GenericLeaf>    pLeaves = pCurrBranch->getLeafList();
    for (pos = pLeaves.getStartPosition(); pos != NULL; )
    {
        GenericLeaf *pLeaf = pLeaves.getNext(pos);
        assert(pLeaf);
        pszName = pLeaf->getName();
        // add this leaf with prepended path to list
        CString strName = pszCurrentPath;
        if (_tcslen(pszCurrentPath) > 0) {
            strName += NameSpaceHolder::sm_cStdSeparatorChar;
        }
        strName += pszName;
        // add this leave to flat list:
        BOOL bMatch = CheckFilterCriteria(pLeaf, pszName, BrowseFilterType, pszFilterCriteria, DataTypeFilter, AccessRightsFilter);
        
        if (bMatch) {
            listObjNames.AddTail(strName);
        }
    }

    // Phase 2: walk thru branches when all leaves are already in item list
    GenericList<GenericBranch>    pBranches = pCurrBranch->getBranchList();
    for (pos = pBranches.getStartPosition(); pos != NULL; )
    {
        GenericBranch *pBranch = pBranches.getNext(pos);
        assert(pBranch);
        pszName = pBranch->getName();
        CString strPath = pszCurrentPath;
        if (_tcslen(pszCurrentPath) > 0) {
            strPath += NameSpaceHolder::sm_cStdSeparatorChar;
        }
        strPath += pszName;
        // walk down recursively !!
        hr = BuildFlatList(listObjNames, pBranch, strPath,
                            BrowseFilterType, pszFilterCriteria, DataTypeFilter, AccessRightsFilter);
    }

    return S_OK;
}



//------------------------------------------------------------------*
BOOL 
    SRT_Resource::CheckFilterCriteria(CSC_VARDEF *pVarDef, LPCTSTR pszName,
                                     OPCBROWSETYPE browseTypeFilter, LPCTSTR filterCriteria,
                                     VARTYPE dataTypeFilter, DWORD accessRightsFilter)
{
    BOOL    bMatch = TRUE;

    if (bMatch && filterCriteria && (_tcslen(filterCriteria) != 0))
    {
        bMatch &= GenericServer::stdMatchStringFilter(pszName, filterCriteria);
    }
    
    if (browseTypeFilter == OPC_BRANCH)
    {
        // check for OPC_BRANCH
        bMatch &= (pVarDef->vardefType == CSC_VARDEF_STRUCT);    // must be OD to be a branch!
    
    }
    else if (   (browseTypeFilter == OPC_LEAF)
             || (browseTypeFilter == OPC_FLAT))
    {
        bMatch &= (BOOL)(pVarDef->vardefType != CSC_VARDEF_STRUCT);    // must be no OD to be a leaf!

        if (bMatch && (dataTypeFilter != VT_EMPTY) && (dataTypeFilter != VT_NULL)) {
            bMatch &= TRUE;
        }

        if (bMatch && (accessRightsFilter != 0)) {
            bMatch &= TRUE;
        }
    }
    
    return bMatch;
}

//------------------------------------------------------------------*
BOOL 
    SRT_Resource::CheckFilterCriteria(GenericElement *pElem, LPCTSTR pszName,
                                     OPCBROWSETYPE browseTypeFilter, LPCTSTR filterCriteria,
                                     VARTYPE dataTypeFilter, DWORD accessRightsFilter)
{
    assert(pElem);
    BOOL    bMatch = TRUE;

    if (bMatch && filterCriteria && (_tcslen(filterCriteria) != 0))
    {
        bMatch &= GenericServer::stdMatchStringFilter(pszName, filterCriteria);
    }

    if (browseTypeFilter == OPC_BRANCH)
    {
        // check for OPC_BRANCH
        bMatch &= pElem->is(GENOBJ_TYPE_BRANCH);
    
    }
    else if (   (browseTypeFilter == OPC_LEAF)
             || (browseTypeFilter == OPC_FLAT))
    {
        bMatch &= pElem->is(GENOBJ_TYPE_LEAF);    // must be no OD to be a leaf!
        
        if (bMatch)
        {
            if (pElem->is(GENOBJ_TYPE_TAG))
            {
                GenericTag  *pTag = (GenericTag *) pElem;

                if (bMatch && (dataTypeFilter != VT_EMPTY) && (dataTypeFilter != VT_NULL)) {
                    bMatch &= (dataTypeFilter == pTag->getNativeDatatype());
                }
                
                DWORD dwAccessRights = pTag->getAccessRights();
                if (bMatch && (accessRightsFilter != 0)) {
                    bMatch &= ((accessRightsFilter & dwAccessRights) == accessRightsFilter);
                }
            }
        }
    }
    
    return bMatch;
}

/* ---------------------------------------------------------------------------- */

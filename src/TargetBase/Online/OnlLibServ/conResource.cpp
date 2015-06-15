/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/conResource.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: conResource.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/conResource.cpp $
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
#include "opcItem.h"
#include "SysVar.h"
#include "SysVarPool.h"
#include "iecResource.h"
#include "conResource.h"
#include "opcError.h"
#include "NSpaceHolder.h"

//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////
//------------- MEMBER FUNCTIONS OF CLASS CON_Resource
/////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------*
/**
 * constructor
 *
 * @param           pszPC_Name          name of pc where resouce is to be started
 * @return          void
 * @exception       -
 */
CON_Resource::CON_Resource(LPCTSTR pszPC_Name)
{
    // save configuration name
    assert(pszPC_Name != NULL);
    m_sConfigName = pszPC_Name;

    m_strSystemName = g_csResCon;
    setName(m_strSystemName);
    // add "__system" branch for sys vars
    GenericPointer<GenericNode> pNode = new GenericNode;
    if (!pNode) {
        TRACE(_T("ctor CON_Resource: Cannot create new __system node!\n"));
        return;
    }
    pNode->setName(g_csSystem);
    addBranch(pNode);
}

//------------------------------------------------------------------*
/**
 * destructor
 *
 * @param           void
 * @return          void
 * @exception       -
*/
CON_Resource::~CON_Resource()
{
}



//------------------------------------------------------------------*
HRESULT
    CON_Resource::OpenResourceInstance()
{
    return S_FALSE; // always open!!
}


//------------------------------------------------------------------*
BOOL
    CON_Resource::CheckForNamespaceUpdate(void)
{
    return FALSE;   // never a change
}


//------------------------------------------------------------------*
HRESULT
    CON_Resource::FillChildrenList(OUT CStringList &listChildren,
                                             IN LPCTSTR pszCurrentPath, IN CStringList &parsedPath,
                                             OPCBROWSETYPE BrowseFilterType, LPCTSTR pszFilterCriteria,
                                             VARTYPE DataTypeFilter, DWORD AccessRightsFilter)
{
    UNUSED_ALWAYS(listChildren);
    UNUSED_ALWAYS(pszCurrentPath);
    UNUSED_ALWAYS(parsedPath);
    UNUSED_ALWAYS(BrowseFilterType);
    UNUSED_ALWAYS(pszFilterCriteria);
    UNUSED_ALWAYS(DataTypeFilter);
    UNUSED_ALWAYS(AccessRightsFilter);
    return S_FALSE;
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
    CON_Resource::isDynamicNode(CStringList &listParsedName)
{
    UNUSED_ALWAYS(listParsedName);
    return FALSE;
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
    CON_Resource::isDynamicTag(CStringList &listParsedName)
{
    UNUSED_ALWAYS(listParsedName);
    return FALSE;
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
    CON_Resource::createDynamicTag(IN CStringList &parsedPath, LPCTSTR pszFullName)
{
    UNUSED_ALWAYS(parsedPath);
    UNUSED_ALWAYS(pszFullName);
    return NULL;
}

/* ---------------------------------------------------------------------------- */

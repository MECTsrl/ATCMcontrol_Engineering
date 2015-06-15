/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/opcItem.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: opcItem.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/opcItem.cpp $
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
#include <genitem.h>
#include "Security.h"
#include "opcItem.h"
#include "sysVar.h"
#include "opcDynServer.h"
#include "opcError.h"


/////////////////////////////////////////////////////////////////////
//------------- MEMBER FUNCTIONS OF CLASS COpcItem
/////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------*
/**
 * constructor
 *
 * @param           void
 * @return          void
 * @exception       -
*/
COpcTag::COpcTag()
{
}

//------------------------------------------------------------------*
/**
 * destructor
 *
 * @param           void
 * @return          void
 * @exception       -
*/
COpcTag::~COpcTag()
{
}


//------------------------------------------------------------------*
/**
 * CheckAccessRights
 *  if security is enabled, check if requested access rights are granted
 *  for current session
 *
 * @param           
 * @return          S_OK, OPC_E_BADRIGHTS
 * @exception       -
*/
HRESULT
    COpcTag::CheckAccessRights(DWORD dwReqAccessRights, IN GenericItem *item)
{
    if (!g_security.IsSecurityEnabled()) {
        // no security, do not check further!
        return S_OK;
    }
    CSecurity::SECURITY_RIGHTS  rights = CSecurity::NoSecurity;
    GenericPointer<CDynamicOpcServer>   pSession;
    pSession = GetCurrentSession(item);
    if (!!pSession) {
        rights = pSession->GetSessionRights();
    }
    else {
        // this can happen if the user is reading certain properties.
        // I have no Item, no Group, no Session.
        // Workaround: try empty password instead
        rights = g_security.GetUserRights(_T(""));
    }
    BOOL bApproved = FALSE;
    if ((dwReqAccessRights & OPC_READABLE) != 0)
    {
        bApproved = (   (rights == CSecurity::ReadOnly)
                     || (rights == CSecurity::ReadAndWrite)
                     || (rights == CSecurity::Engineering));
    }
    if ((dwReqAccessRights & OPC_WRITEABLE) != 0)
    {
        bApproved = (   (rights == CSecurity::ReadAndWrite)
                     || (rights == CSecurity::Engineering));
    }

    return bApproved ? S_OK : OPC_E_BADRIGHTS;
}

//------------------------------------------------------------------*
/**
 * GetCurrentAccessRights
 *  if security is enabled, return current access rights
 *  for current session
 *
 * @param           
 * @return          security rights
 * @exception       -
*/
CSecurity::SECURITY_RIGHTS
    COpcTag::GetCurrentAccessRights(IN GenericItem *item)
{
    if (!g_security.IsSecurityEnabled()) {
        // no security, do not check further!
        return CSecurity::NoSecurity;
    }
    GenericPointer<CDynamicOpcServer>   pSession;
    pSession = GetCurrentSession(item);
    if (!pSession) {
        return CSecurity::NoRights;
    }
    return pSession->GetSessionRights();
}

//------------------------------------------------------------------*
/**
 * SetAccessRights
 *  set new access rights for current session
 *
 * @param           
 * @return          S_OK
 * @exception       -
*/
HRESULT
    COpcTag::SetAccessRights(CSecurity::SECURITY_RIGHTS newRights, IN GenericItem *item)
{
    GenericPointer<CDynamicOpcServer>   pSession;
    pSession = GetCurrentSession(item);
    if (!pSession) {
        return E_FAIL;
    }
    pSession->SetSessionRights(newRights);
    TrcPrint(TRC_INTERFACE, _T("SetAccessRights: session 0x%08X changed to '%s'\n"),
        pSession, CSecurity::GetSecurityText(newRights));
    TRACE(_T("SetAccessRights: session 0x%08X changed to '%s'\n"),
        pSession, CSecurity::GetSecurityText(newRights));
    return S_OK;
}


//------------------------------------------------------------------*
void
    COpcTag::SavePassword(LPCTSTR pszPassword, IN GenericItem *item)
{
    GenericPointer<CDynamicOpcServer>   pSession;
    pSession = GetCurrentSession(item);
    if (!pSession) {
        return;
    }
    pSession->SavePassword(pszPassword);
}


//------------------------------------------------------------------*
/**
 * GetCurrentSession
 *  return OPC server session to which this item belongs
 *  caller must release() the returned pointer!
 *
 * @param           
 * @return          ptr to current session; NULL on error
 * @exception       -
*/
CDynamicOpcServer *
    COpcTag::GetCurrentSession(IN GenericItem *item)
{
    if (item == NULL) {
        return NULL;
    }
    GenericPointer<GenericBranch> pGroup = item->getParentBranch();
    if (!pGroup) {
        return NULL;
    }
    GenericBranch *pServer = pGroup->getParentBranch();
    if (!pServer) {
        return NULL;
    }
    CDynamicOpcServer *pSession = dynamic_cast<CDynamicOpcServer*> (pServer);
    assert(pSession);
    return pSession;
}

/* ---------------------------------------------------------------------------- */

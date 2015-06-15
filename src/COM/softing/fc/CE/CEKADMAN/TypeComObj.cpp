

// TypeComObj.cpp : Implementation of CTypeComObj
#include "stdafx.h"
#include "KadManager.h"
#include "TypeComObj.h"
#include "ComObj.h"
#include "MenuComObj.h"
#include "EnumStatesComObj.h"
#include "VarDeclComObj.h"


/////////////////////////////////////////////////////////////////////////////
// CTypeComObj

XML_NODE_REF_IMPL(CTypeComObj)

STDMETHODIMP CTypeComObj::GetIcon(BSTR *psIcon)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"ICON",psIcon);
}

STDMETHODIMP CTypeComObj::GetId(BSTR *psId)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"ID",psId);
}

STDMETHODIMP CTypeComObj::GetVisibilityFlag(BOOL *pbVisibilityFlag)
{
    return TFGetAttribute<BOOL *>(&m_XmlNodeRef,L"VISIBILITY",pbVisibilityFlag);
}

STDMETHODIMP CTypeComObj::GetMenu(IMenu **ppIMenu)
{
    return TFGetSubNode<CMenuComObj,IMenu>(
        &m_XmlNodeRef,IID_IMenu,L"MENU",ppIMenu);
}

//------------------------------------------------------------------*
/**
 * Returns the TEXT attribute associated with a TYPE entry
 * of the KAD (Kernel Addon Description). The value of the attribute 
 * TEXT identifies display and filter string used for the type.
 *
 * @param           BSTR * - pointer to BSTR receiving the string value of
 *                  the attribute TEXT. Pointer shall not be NULL.
 * @return          HRESULT - of error, S_OK if successful.
 *
 * @see     KAD description.
*/
STDMETHODIMP CTypeComObj::GetText(BSTR *psText)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"TEXT",psText);
}

STDMETHODIMP CTypeComObj::GetFilterText(BSTR *psFilterText)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"FILTERTEXT",psFilterText);
}

STDMETHODIMP CTypeComObj::GetView(BSTR *psView)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"VIEW",psView);
}

STDMETHODIMP CTypeComObj::GetStateEnumerator(IEnumStates **ppEnumStates)
{
    return TFCreateAndInitEnumObject<CEnumStatesComObj,IEnumStates>(
        IID_IEnumStates,IID_IState,L"STATE",
        m_XmlNodeRef.GetXmlNode(),ppEnumStates);
}

STDMETHODIMP CTypeComObj::GetProjectListViewFlag(
    BOOL *pbProjectListViewFlag)
{
    return TFGetAttribute<BOOL *>(&m_XmlNodeRef,L"PROJ_LIST_VIEW",
        pbProjectListViewFlag);
}


STDMETHODIMP CTypeComObj::GetVarDecl(IVarDecl **ppIVarDecl)
{
    return TFGetSubNode<CVarDeclComObj,IVarDecl>(
        &m_XmlNodeRef,IID_IVarDecl,L"VAR_DECL",ppIVarDecl);
}



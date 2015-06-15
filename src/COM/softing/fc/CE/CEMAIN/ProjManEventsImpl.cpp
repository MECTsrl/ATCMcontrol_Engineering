// ProjManEventsImpl.cpp: implementation of the CProjManEventsImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Navigator.h"
#include "ProjManEventsImpl.h"
#include "XMLHelper/XMLDocument.h"
#include "GenEdCont.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProjManEventsImpl::CProjManEventsImpl()
{

}

CProjManEventsImpl::~CProjManEventsImpl()
{

}

void CProjManEventsImpl::SetNavigatorPtr(CNavigator* pNavigator)
{
    m_pNavigator = pNavigator;
}


STDMETHODIMP CProjManEventsImpl::InitClassTree(IUnknown* pXMLDocument)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());
    if (m_pNavigator!=NULL)
    {
        CXMLDocument*    pXMLDoc = new CXMLDocument;
        pXMLDoc->SetIXMLDocument(pXMLDocument);
        m_pNavigator->InitTree(NAV_CLASSVIEW, pXMLDoc);
        m_pNavigator->BuildTree(NAV_CLASSVIEW);
    }
    return (S_OK);
}

STDMETHODIMP CProjManEventsImpl::InitTargetTree(IUnknown* pXMLDocument)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());
    if (m_pNavigator!=NULL)
    {
        CXMLDocument*    pXMLDoc = new CXMLDocument;
        pXMLDoc->SetIXMLDocument(pXMLDocument);
        m_pNavigator->InitTree(NAV_HWVIEW, pXMLDoc);
        m_pNavigator->BuildTree(NAV_HWVIEW);
    }
    return (S_OK);
}

STDMETHODIMP CProjManEventsImpl::InitInstanceTree(IUnknown* pXMLDocument)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());
    if (m_pNavigator!=NULL)
    {
        CXMLDocument*    pXMLDoc = new CXMLDocument;
        pXMLDoc->SetIXMLDocument(pXMLDocument);
        m_pNavigator->InitTree(NAV_INSTVIEW, pXMLDoc);
        m_pNavigator->BuildTree(NAV_INSTVIEW);
    }
    return (S_OK);
}

STDMETHODIMP CProjManEventsImpl::UpdateClassTree(BSTR sChangedIdPath, E_UPDATE_MODE eMode)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    if (m_pNavigator!=NULL)
    {
        CString strIdPath(sChangedIdPath);
        switch(eMode)
        {
        case eUpdateAll:
            m_pNavigator->UpdateTreeNode(NAV_CLASSVIEW, strIdPath);
            break;
        case eUpdateAttributes:
            m_pNavigator->UpdateAttributes(NAV_CLASSVIEW, strIdPath);
            break;
        case eUpdateDelete:
            m_pNavigator->UpdateDelete(NAV_CLASSVIEW, strIdPath);
            break;
        case eUpdateInsert:
            m_pNavigator->UpdateInsert(NAV_CLASSVIEW, strIdPath);
            break;
        }
        m_pNavigator->InvalidateTree(NAV_CLASSVIEW);
    }
    return (S_OK);
}

STDMETHODIMP CProjManEventsImpl::UpdateTargetTree(BSTR sChangedIdPath, E_UPDATE_MODE eMode)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    if (m_pNavigator!=NULL)
    {
        CString strIdPath(sChangedIdPath);
        switch(eMode)
        {
        case eUpdateAll:
            m_pNavigator->UpdateTreeNode(NAV_HWVIEW, strIdPath);
            break;
        case eUpdateAttributes:
            m_pNavigator->UpdateAttributes(NAV_HWVIEW, strIdPath);
            break;
        case eUpdateDelete:
            m_pNavigator->UpdateDelete(NAV_HWVIEW, strIdPath);
            break;
        case eUpdateInsert:
            m_pNavigator->UpdateInsert(NAV_HWVIEW, strIdPath);
            break;
        }

        m_pNavigator->InvalidateTree(NAV_HWVIEW);
    }
    return (S_OK); 
}


STDMETHODIMP CProjManEventsImpl::UpdateInstanceTree(BSTR sChangedIdPath, E_UPDATE_MODE eMode)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    if (m_pNavigator!=NULL)
    {
        CString strIdPath(sChangedIdPath);
        switch(eMode)
        {
        case eUpdateAll:
            m_pNavigator->UpdateTreeNode(NAV_INSTVIEW, strIdPath);
            break;
        case eUpdateAttributes:
            m_pNavigator->UpdateAttributes(NAV_INSTVIEW, strIdPath);
            break;
        case eUpdateDelete:
            m_pNavigator->UpdateDelete(NAV_INSTVIEW, strIdPath);
            break;
        case eUpdateInsert:
            m_pNavigator->UpdateInsert(NAV_INSTVIEW, strIdPath);
            break;
        }
        m_pNavigator->InvalidateTree(NAV_INSTVIEW);
    }
    return (S_OK); 
}

STDMETHODIMP CProjManEventsImpl::OnFileAdded(BSTR sFilePath, enum E_FV_FileCategory eFileCat)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());
    
    CString strFilePath(sFilePath);

    m_pNavigator->OnFileAdded(strFilePath, eFileCat);
    return (S_OK);
}

STDMETHODIMP CProjManEventsImpl::OnFileRemoved(BSTR sFilePath, enum E_FV_FileCategory eFileCat)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());
    
    CString strFilePath(sFilePath);

    m_pNavigator->OnFileRemoved(strFilePath, eFileCat);
    return (S_OK);
}

STDMETHODIMP CProjManEventsImpl::TargetInstanceAdded(BSTR sId, BSTR sIdPath)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    m_pNavigator->AddTargetInstance(sId, sIdPath);
	return (S_OK);
}

STDMETHODIMP CProjManEventsImpl::TargetInstanceRemoved(BSTR sId)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    m_pNavigator->RemoveTargetInstance(sId);
	return (S_OK);
}


STDMETHODIMP CProjManEventsImpl::ShowContextHelp(DWORD dwContextId)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    CWinApp *pApp;

    pApp = AfxGetApp ();
    if (pApp != NULL)
    {
        pApp->WinHelp(dwContextId);
    }

	return (S_OK);
}


STDMETHODIMP CProjManEventsImpl::ProjectFileChanged()
{
    AFX_MANAGE_STATE (AfxGetAppModuleState());

    // get container
    CGenEdCont* pGenEdCont = CEGetContainer();

    if(pGenEdCont)
    {
        pGenEdCont->ReloadProjectFile();
    }
    return S_OK;
}

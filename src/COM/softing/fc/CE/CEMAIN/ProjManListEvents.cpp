// ProjManListEvents.cpp: implementation of the CProjManListEvents class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CEMAINView.h"
#include "ProjManListEvents.h"
#include "XMLHelper/XMLDocument.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProjManListEvents::CProjManListEvents()
{

}

CProjManListEvents::~CProjManListEvents()
{

}

void CProjManListEvents::SetListViewPtr(CCEMAINView* pListView)
{
    m_pListView = pListView;
}

STDMETHODIMP CProjManListEvents::InitClassTree(IUnknown* pXMLDocument)
{
    return (S_OK);
}

STDMETHODIMP CProjManListEvents::InitTargetTree(IUnknown* pXMLDocument)
{
    return (S_OK);
}

STDMETHODIMP CProjManListEvents::InitInstanceTree(IUnknown* pXMLDocument)
{
    return (S_OK);
}



STDMETHODIMP CProjManListEvents::UpdateClassTree(BSTR sChangedIdPath, E_UPDATE_MODE eMode)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    if (m_pListView!=NULL)
    {
        CString strIdPath(sChangedIdPath);

        switch(eMode)
        {
        case eUpdateAll:
            m_pListView->UpdateTreeNode(strIdPath);
            break;
        case eUpdateAttributes:
            m_pListView->UpdateTreeNodeAttributes(strIdPath);
            break;
        case eUpdateDelete:
            m_pListView->DeleteTreeNode(strIdPath);
            break;
        case eUpdateInsert:
            m_pListView->InsertTreeNode(strIdPath);
            break;
        }
    }

    return (S_OK);
}


STDMETHODIMP CProjManListEvents::UpdateTargetTree(BSTR sChangedIdPath, E_UPDATE_MODE eMode)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    if (m_pListView!=NULL)
    {
        CString strIdPath(sChangedIdPath);

        switch(eMode)
        {
        case eUpdateAll:
            m_pListView->UpdateTreeNode(strIdPath);
            break;
        case eUpdateAttributes:
            m_pListView->UpdateTreeNodeAttributes(strIdPath);
            break;
        case eUpdateDelete:
            m_pListView->DeleteTreeNode(strIdPath);
            break;
        case eUpdateInsert:
            m_pListView->InsertTreeNode(strIdPath);
            break;
        }
    }

    return (S_OK); 
}

STDMETHODIMP CProjManListEvents::UpdateInstanceTree(BSTR sChangedIdPath, E_UPDATE_MODE eMode)
{
    return (S_OK); 
}

STDMETHODIMP CProjManListEvents::OnFileAdded(BSTR sFilePath, enum E_FV_FileCategory eFileCat)
{
    return (S_OK);
}

STDMETHODIMP CProjManListEvents::OnFileRemoved(BSTR sFilePath, enum E_FV_FileCategory eFileCat)
{
    return (S_OK);
}


STDMETHODIMP CProjManListEvents::ShowContextHelp(DWORD dwContextId)
{
	return (S_OK);
}

STDMETHODIMP CProjManListEvents::ProjectFileChanged()
{
    return (S_OK);
}


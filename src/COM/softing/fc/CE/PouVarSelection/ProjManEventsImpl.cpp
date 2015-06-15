


#include "stdafx.h"
#include "ProjManEventsImpl.h"
#include "PouVarSelData.h"

#include "CEProjMan.h"
#include "CEProjMan_i.c"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//------------------------------------------------------------------*
/**
 * constructor
 *
 * @exception       -
 * @see             
*/
CProjManEventsImpl::CProjManEventsImpl()
{
    m_pPouVarSelData = NULL;
}


//------------------------------------------------------------------*
/**
 * destructor
 *
 * @exception       -
 * @see             
*/
CProjManEventsImpl::~CProjManEventsImpl()
{

}


//------------------------------------------------------------------*
/**
 * SetPouVarSelDataPtr pass the pointer to the data holding class.
 *
 * @param           pPouVarSelData: the object pointer
 * @return          
 * @exception       -
 * @see             
*/
void CProjManEventsImpl::SetPouVarSelDataPtr(CPouVarSelData* pPouVarSelData)
{
    m_pPouVarSelData = pPouVarSelData;
}



//------------------------------------------------------------------*
/**
 * UpdateClassTree.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
STDMETHODIMP CProjManEventsImpl::UpdateClassTree(BSTR sChangedIdPath, enum E_UPDATE_MODE eMode)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    if (m_pPouVarSelData!=NULL)
    {
        CString strIdPath = sChangedIdPath;
        m_pPouVarSelData->HandleUpdateClassTree(strIdPath);
    }

    return (S_OK);
}



// all other events can be ignored:


STDMETHODIMP CProjManEventsImpl::InitClassTree(IUnknown* pXMLDocument)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());
    // ignore: not used here
    return (S_OK);
}

STDMETHODIMP CProjManEventsImpl::InitTargetTree(IUnknown* pXMLDocument)
{
    // ignore: not used here
    return (S_OK);
}

STDMETHODIMP CProjManEventsImpl::InitInstanceTree(IUnknown* pXMLDocument)
{
    // ignore: not used here
   return (S_OK);
}


STDMETHODIMP CProjManEventsImpl::UpdateTargetTree(BSTR sChangedIdPath, enum E_UPDATE_MODE eMode)
{
    // ignore: not used here
    return (S_OK); 
}


STDMETHODIMP CProjManEventsImpl::UpdateInstanceTree(BSTR sChangedIdPath, enum E_UPDATE_MODE eMode)
{
    // ignore: not used here
    return (S_OK); 
}


STDMETHODIMP CProjManEventsImpl::OnFileAdded(BSTR sFilePath, enum E_FV_FileCategory eFileCat)
{
    // ignore: not used here
    return (S_OK);
}


STDMETHODIMP CProjManEventsImpl::OnFileRemoved(BSTR sFilePath, enum E_FV_FileCategory eFileCat)
{
    // ignore: not used here
    return (S_OK);
}



STDMETHODIMP CProjManEventsImpl::ShowContextHelp(DWORD dwContextId)
{
    // ignore: not used here
	return (S_OK);
}

STDMETHODIMP CProjManEventsImpl::ProjectFileChanged()
{
    // ignore: not used here
    return S_OK;
}

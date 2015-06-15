// CEFIFComObj.cpp : Implementation of CCEFIFComObj
#include "stdafx.h"
#include "CEFIFComObj.h"
#include "CEMsg_i.c"
#include "FindInFiles.h"
#include "ThrdTmpl.h"


/////////////////////////////////////////////////////////////////////////////
// CCEFIFComObj

//------------------------------------------------------------------*
/**
 * constructor.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
CCEFIFComObj::CCEFIFComObj()
{
    m_bInitialized = FALSE;     // not yet initialized
}

//------------------------------------------------------------------*
/**
 * final release.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CCEFIFComObj::FinalRelease()
{
}

//------------------------------------------------------------------*
/**
 * fire message.
 *
 *  call fire method of ICEMessage connection point.
 *
 * @param            crstrMessage: message to be displayed in message view.
 * @param            hr: HRESULT for message
 * @return           -
 * @exception        -
 * @see              -
*/
void CCEFIFComObj::FireMessage(const CString& crstrMessage, HRESULT hr /*= S_OK*/)
{
    CComBSTR    sMessage(crstrMessage);

    Fire_OnNewMessage(NULL, E_FACILITY_CEFINDINFILES, hr, sMessage);
}


//------------------------------------------------------------------*
/**
 * get interface pointer.
 *
 * @param           ppIFIFMsg: pointer to internal find in files message 
 *                              interface pointer
 * @return          result from QueryInterface()
 * @exception       -
 * @see             
*/
HRESULT CCEFIFComObj::GetInterfacePointer(IFIFMsgInternal** ppIFIFMsg)
{
    HRESULT hr = QueryInterface(IID_IFIFMsgInternal, (void**)ppIFIFMsg);
    return hr;
}

// ICEFindInFiles methods

//------------------------------------------------------------------*
/**
 *  AddFileType()
 *  add a file type and an edit interface GUID for the search.
 *
 *  @param  sExtension: file extension
 *  @param  sEditGUID: GUID string for editor interface to use
 *         
 * @return  S_OK : ok
 *
*/
STDMETHODIMP CCEFIFComObj::AddFileType
(
    /*[in,string]*/ BSTR    sExtension,
    /*[in,string]*/ BSTR    sEditGUID
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    
    m_tFIFThread.Break();

    CEFIFExtData    tData;
    tData.m_strExtension = sExtension;
    tData.m_strGUID = sEditGUID;

    m_tFIFThreadData.m_tExtDataList.AddTail(tData);

    m_tFIFThread.Continue();
    
    return S_OK;
}

    
//------------------------------------------------------------------*
/**
 *  StartFind()
 *
 * @param   sExpression: expression to be found
 * @param   sDirectory: search directory
 * @param   dwFlags: search option flags
 *                      FIF_OPTION_WHOLEWORD | 
 *                      FIF_OPTION_CASESENSITIVE |
 *                      FIF_OPTION_REGEXP
 *                        (see FindFileDlg.h)
 *         
 * @return  S_OK : ok
 *          E_FAIL : start find failed.
 *
*/
STDMETHODIMP CCEFIFComObj::StartFind
(
    /*[in,string]*/ BSTR    sExpression,
    /*[in,string]*/ BSTR    sDirectory,
    /*[in]*/        DWORD   dwFlags
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    
    HRESULT             hr;

    hr = Initialize();
    if(hr != S_OK)
    {
        return hr;
    }

    m_tFIFThread.Destroy();

    m_tFIFThreadData.m_strExpression = sExpression;
    m_tFIFThreadData.m_strDirectory = sDirectory;
    m_tFIFThreadData.m_dwFlags = dwFlags;
    
    if(hr == S_OK)
    {
        if(m_tFIFThread.Create(INFINITE, &m_tFIFThreadData) == -1)
        {
            return E_FAIL;
        }
        HANDLE hSignalEvent = m_tFIFThread.GetSignalEvent();
        ::SetEvent(hSignalEvent);
    }

    return hr;
}



//------------------------------------------------------------------*
/**
 * initialize.
 *
 *  marshall message interface into thread.
 *
 * @return          hr
*/
HRESULT CCEFIFComObj::Initialize()
{
    HRESULT             hr;
    IFIFMsgInternal*    pIFIFMsg;

    hr = GetInterfacePointer(&pIFIFMsg);
    if(hr != S_OK)
    {
        return hr;
    }

    m_tFIFThread.m_pIStream = NULL;
    
    hr = ::CoMarshalInterThreadInterfaceInStream(IID_IFIFMsgInternal, pIFIFMsg, &m_tFIFThread.m_pIStream);
    if(hr == S_OK)
    {
        m_bInitialized = TRUE;
    }
    pIFIFMsg->Release();
    return hr;
}

//------------------------------------------------------------------*
/**
 *  StopFind()
 *  stops running find thread.
 *         
 * @return  S_OK : ok
 *          E_FAIL : stop failed (should never occur)
 *
*/
STDMETHODIMP CCEFIFComObj::StopFind()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    
    HRESULT hr = S_OK;

    if(!m_tFIFThread.Stop())
    {
        hr = E_FAIL;
    }
    CString     strMessage;
    strMessage.LoadString(IDS_FIF_ABORTED);
    CComBSTR    sMessage(strMessage);
    Fire_OnNewMessage(NULL, E_FACILITY_CEFINDINFILES, hr, sMessage);

    return hr;
}

    
//------------------------------------------------------------------*
/**
 *  IsFindActive()
 *         
 * @param   pbActive: is find active?
 * @return  S_OK : ok
 *
*/
STDMETHODIMP CCEFIFComObj::IsFindActive(/*[out]*/ BOOL* pbActive)
{
    if(m_tFIFThread.IsThreadRunning())
    {
        *pbActive = TRUE;
    }
    else
    {
        *pbActive = FALSE;
    }
    return S_OK;
}


//------------------------------------------------------------------*
/**
 *  Reset()
 *  stops the thread, if it is running
 *  and removes all added file types
 *
 * @return  S_OK : ok
 *          E_FAIL : reset failed (should never occur)
*/
STDMETHODIMP CCEFIFComObj::Reset()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    

    HRESULT hr = S_OK;

    if(!m_tFIFThread.Stop())
    {
        return E_FAIL;
    }

    m_tFIFThreadData.Reset();
    
    return hr;
}


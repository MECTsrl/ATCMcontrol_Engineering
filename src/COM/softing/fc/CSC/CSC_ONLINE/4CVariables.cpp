/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/4CVariables.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: 4CVariables.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/4CVariables.cpp $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CSC
 *
 * =COMPONENT           CSC Online Control
 *
 * =CURRENT      $Date: 28.02.07 19:00 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *





 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  15.03.2001  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/

#include "stdafx.h"
#include <float.h>
#include "CSC_Online.h"
#include "4CVariable.h"
#include "4CVariables.h"
#include "FCSession.h"
#include "baselib.h"
#include "VarSession.h"

#include "CSC_OnlineErr.h"

#pragma warning (disable : 4786) // 'xxxxx' : identifier was truncated to '255' characters in the debug information

//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CFCVariableCollection

//------------------------------------------------------------------*
CFCVariableCollection::CFCVariableCollection()
  : m_varHandleMap(200)
{
}

//------------------------------------------------------------------*
CFCVariableCollection::~CFCVariableCollection()
{
}


//------------------------------------------------------------------*
HRESULT CFCVariableCollection::FinalConstruct()
{
    HRESULT hr;
    TrcPrint(TRC_INTERFACE, _T("CFCVariableCollection::FinalConstruct()\n"));
    
    IFCSessions  *pObj = NULL;
    hr = ::CoCreateInstance(CLSID_FCSessions, NULL, CLSCTX_INPROC_SERVER, IID_IFCSessions, (void**) &pObj);
    _ASSERTE(SUCCEEDED(hr));
    if (FAILED(hr)) {
        return hr;
    }
    _ASSERTE(pObj != NULL);
    m_pSessions = pObj;
    pObj->Release();

    return hr;
}

//------------------------------------------------------------------*
void CFCVariableCollection::FinalRelease()
{
    m_pSessions = NULL;

    TrcPrint(TRC_INTERFACE, _T("CFCVariableCollection::FinalRelease()\n"));
}

#ifdef _VAR_COLL_FULL_ACTIVEX_CTRL
//------------------------------------------------------------------*
STDMETHODIMP CFCVariableCollection::InterfaceSupportsErrorInfo(REFIID riid)
{
    static const IID* arr[] = 
    {
        &IID_IFCVariableCollection,
    };
    for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
    {
        if (InlineIsEqualGUID(*arr[i],riid))
            return S_OK;
    }
    return S_FALSE;
}

//------------------------------------------------------------------*
HRESULT CFCVariableCollection::OnDrawAdvanced(ATL_DRAWINFO& di)
{
    RECT& rc = *(RECT*)di.prcBounds;
    Rectangle(di.hdcDraw, rc.left, rc.top, rc.right, rc.bottom);
    DrawText(di.hdcDraw, _T("CSC Online"), -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    return S_OK;
}
#endif // _VAR_COLL_FULL_ACTIVEX_CTRL

/////////////////////////////////////////////////////////////////////
// interface IFCVariableCollection
/////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------*
STDMETHODIMP CFCVariableCollection::Subscribe(BSTR sItem, long *phServer)
{
    CString strAddress,strItemPath;
    CGuid guid;
    CComBSTR sAddr;
    CComObject<CFCVariable> *pVar = NULL;
    IUnknown *pUnk = NULL;
    CFCVarSession *pSession = NULL;
    HRESULT hr = S_OK;
    
    USES_CONVERSION;
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    try {
        if (!phServer) {
            _com_raise_error(E_POINTER);
        }
        *phServer = 0L;
        
        ParseAddress(sItem,strAddress,strItemPath);
        // get OPC server/group which holds this variable, server is started if necessary
        //TODO CHECK THIS
        sAddr = strAddress;
        hr = m_pSessions->OpenVarSession(sAddr, &pUnk);
        if (FAILED(hr)) {
            _com_raise_error(hr);
        }

        pSession = dynamic_cast<CComObject<CFCVarSession> *>(pUnk);
        if (pSession == NULL) {
            _com_raise_error(E_FAIL);
        }

        hr = CComObject<CFCVariable>::CreateInstance(&pVar);
        if(FAILED(hr)) {
            _com_raise_error(hr);
        }
        pVar->AddRef();

        CSingleLock lock(&m_cs, TRUE);
        pVar->Init(strItemPath, this);
        m_varHandleMap.SetAt((long) pVar, pVar);
        pSession->AddVariable(pVar);
        *phServer = (long) pVar;

        TRACE(_T("VarColl 0x%08X add '%s': h=0x%08X\n"), this, strItemPath, *phServer);
        TrcPrint(TRC_INTERFACE, _T("VarColl 0x%08X add '%s': h=0x%08X\n"), this, strItemPath, *phServer);
    }
    catch(_com_error err) {
        hr = err.Error();
    }
    catch(CMemoryException* e) {
        e->Delete();
        hr = E_OUTOFMEMORY;
    }

    FC_RELEASE_PTR(pUnk);
    return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CFCVariableCollection::UnsubscribeWithHandle(long hServer)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    CSingleLock lock(&m_cs, TRUE);
    CComObject<CFCVariable>* pVar = FindVariable(hServer);
    if (pVar == NULL) {
        return E_INVALIDARG;
    }

    TRACE(_T("VarColl 0x%08X del '%s': h=0x%08X\n"), this, (LPCTSTR)(pVar->m_name), hServer);
    TrcPrint(TRC_INTERFACE, _T("VarColl 0x%08X del '%s': h=0x%08X\n"), this, (LPCTSTR)(pVar->m_name), hServer);

    m_varHandleMap.RemoveKey((long) hServer);
    pVar->m_pSession->RemoveVariable(pVar);

    pVar->Release();
    pVar->Release();

    return S_OK;
}

//------------------------------------------------------------------*

/////////////////////////////////////////////////////////////////////
// interface IFCVarCollSyncAccess
/////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------*
STDMETHODIMP CFCVariableCollection::ReadWithHandle(long hServer, short *pQuality, DATE *pDate, VARIANT * pVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }
    CComObject<CFCVariable>* pVar = FindVariable(hServer);
    if (pVar == NULL) {
        return E_INVALIDARG;
    }
    
    HRESULT hr = pVar->getContent(pVal, pQuality, pDate);

    pVar->Release();

    return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP CFCVariableCollection::WriteWithHandle(long hServer, VARIANT newVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    CComObject<CFCVariable>* pVar = FindVariable(hServer);
    if (pVar == NULL) {
        return E_INVALIDARG;
    }
    
    HRESULT hr = pVar->m_pSession->WriteValueAsync(pVar, newVal);
    pVar->Release();
    return hr;
}


//------------------------------------------------------------------*
/**
 * find item by server handle
 *
 * @param   hServer     server supplied handle of variable
 * @return              NULL if not found
 * @see             -
 * @exception       -
*/
CComObject<CFCVariable> * CFCVariableCollection::FindVariable(long hServer)
{
    CComObject<CFCVariable>*    pVariable = NULL;

    // lookup handle..
    CSingleLock lock(&m_cs, TRUE);
    m_varHandleMap.Lookup(hServer, pVariable);

    // must be the same as handle, looked up to be sure
    _ASSERTE((pVariable == NULL) || (long(pVariable) == hServer));
    if (pVariable != NULL)
        pVariable->AddRef();

    return pVariable;
}



//------------------------------------------------------------------*
/**
 * Parses item path of variable to subscribe. The item path has e.g. 
 * the form: {w-ed,8E2A6B33-60E9-11D1-BCDD-006097E1BE2F,COM1,9600}.resource1.program1.count
 * The connection address string embraced by the parenthesis characters
 * will be separated from the actual item path for subscription of the item.
 * The following forms of the connection address will be supported:
 * [address], [{address}] {address}.
 *
 * @param			BSTR sItem - Full item path inclusive of the connection address.
 * @param			CString & strAddress - returned connection address
 * @param			CString & strItemPath - item path will be returned with this
 *                  parameter.
 *
 * @exception       _com_error exception, CMemoryException
 * @see             4CSC_Architecture(V2.0).doc section 3.2.1.3
*/
void CFCVariableCollection::ParseAddress(BSTR sItem,CString & strAddress, CString & strItemPath)
{
    int iPos;
    CString strItem = sItem;

    strItemPath.Empty();
    strAddress.Empty();

    strItem.TrimLeft(); //trim whitespace characters
    strItem.TrimRight();

    iPos = strItem.Find(gs_szMachineNameLeadOut); //}]
    if(iPos != -1) {//success
        strAddress = strItem.Left(iPos + _tcslen(gs_szMachineNameLeadOut));
        strItemPath = strItem.Mid(iPos + _tcslen(gs_szMachineNameLeadOut));
        strItemPath.TrimLeft(_T('.')); //trim the leading '.'
        return;
    }

    iPos = strItem.Find(gs_chMachineNameLeadOut); //}
    if(iPos != -1) {//success 
        strAddress = strItem.Left(iPos + 1);
        strItemPath = strItem.Mid(iPos + 1);
        strItemPath.TrimLeft(_T('.')); //trim the leading '.'
        return;
    }

    iPos = strItem.Find(_T(']')); //]
    if(iPos != -1) {//success 
        strAddress = strItem.Left(iPos + 1);
        strItemPath = strItem.Mid(iPos + 1);
        strItemPath.TrimLeft(_T('.')); //trim the leading '.'
        return;
    }

    //oops error
    _com_raise_error(E_CSC_SESSION_BAD_ADDRESS);
}

/*
 *----------------------------------------------------------------------------*
 *  $History: 4CVariables.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 4  *****************
 * User: Su           Date: 3.04.01    Time: 10:49
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Added tolerance for ParseAddress, address forms can be [{address}],
 * [address] and {address}.
 * 
 * *****************  Version 3  *****************
 * User: Su           Date: 19.03.01   Time: 19:01
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Revision
 * 
 * *****************  Version 2  *****************
 * User: Su           Date: 15.03.01   Time: 19:35
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Removed not implemented methods, New header, removed private trace
 * files, replaced by tracea.lib.
 * 
 * *****************  Version 1  *****************
 * User: Su           Date: 14.03.01   Time: 17:21
 * Created in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * Initial rev.
 * 
 * *****************  Version 69  *****************
 * User: Ed           Date: 11.02.00   Time: 16:16
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Variablen-Connection mit eigenem Thread
 *==
 *----------------------------------------------------------------------------*
*/

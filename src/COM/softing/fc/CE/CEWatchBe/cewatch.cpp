
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

// CEWatch.cpp : Implementation of CCEWatch
#include "stdafx.h"
#include "CEWatch.h"
#include "watchexprman.h"
#include "st_scan.h"
#include "DbgInfo_i.c"


extern HRESULT ParseItem (const CString &strText, CEExpandMode expandMode, const CString &strScope, CExpressionHolder *pHolder);
extern HRESULT ParseFormat (const CString &strText, CExpressionHolder *pHolder, CEWatchItemState &state);
extern void CELockManagers ();
extern void CEUnlockManagers ();
extern CExpressionFactory *CEGetExpressionFactory ();
extern void CETriggerResubscribe ();


/////////////////////////////////////////////////////////////////////////////
// CCEWatch

CCEWatch::CCEWatch ()
: m_ExpressionChangedCB (this, OnExpressionChanged)
{
    m_pExpressionFactory = CEGetExpressionFactory ();
    ASSERT (m_pExpressionFactory != NULL);
}


CCEWatch::~CCEWatch ()
{
}


STDMETHODIMP CCEWatch::Init(BSTR sSymbolFile)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    HRESULT hr;

    hr = CoCreateInstance (CLSID_DebugInfoServer, NULL, CLSCTX_ALL, IID_IDebugInfo, (LPVOID *)&m_pDebugInfo);
    if (FAILED (hr))
    {
        return (hr);
    };
    ASSERT (m_pDebugInfo != NULL);
    if (m_pDebugInfo == NULL)
    {
        return (E_UNEXPECTED);
    };

    hr = m_pDebugInfo->Init (sSymbolFile);
    if (FAILED (hr))
    {
        m_pDebugInfo = NULL;
        return (hr);
    };

    CELockManagers ();

	return (S_OK);
}


STDMETHODIMP CCEWatch::Cleanup()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    DisposeExpressions ();

    if (m_pDebugInfo != NULL)
    {
        m_pDebugInfo->Cleanup ();
        m_pDebugInfo = NULL;
    };

    CEUnlockManagers ();

	return (S_OK);
}


STDMETHODIMP CCEWatch::GetDebugInfo(IUnknown **ppDebugInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    HRESULT hr;

    ASSERT (ppDebugInfo != NULL);
    if (ppDebugInfo == NULL)
    {
        return (E_POINTER);
    };

    ASSERT (m_pDebugInfo != NULL);
    if (m_pDebugInfo == NULL)
    {
        return (E_UNEXPECTED);
    };

    hr = m_pDebugInfo->QueryInterface (IID_IUnknown, (LPVOID *)ppDebugInfo);
    ASSERT (*ppDebugInfo != NULL);
    if (*ppDebugInfo == NULL)
    {
        return (E_UNEXPECTED);
    };

	return (S_OK);
}


STDMETHODIMP CCEWatch::AddItem (BSTR sText, long *lHandle)
{
	AFX_MANAGE_STATE (AfxGetStaticModuleState ())
    HRESULT hr;
    CExpressionHolder *pExpression;

    *lHandle = 0;

    hr = ParseSimple (sText, pExpression);
    if (FAILED (hr))
    {
        return (hr);
    };

    *lHandle = pExpression->GetHandle ();

    m_ExpressionByHandle.SetAt (*lHandle, pExpression);

    pExpression->SetChangedCB (&m_ExpressionChangedCB);

    Fire_OnExpressionChanged (*lHandle);

    return (S_OK);
}


STDMETHODIMP CCEWatch::RemoveItem (long lHandle)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CExpressionHolder *pExpression;

    pExpression = GetHolder (lHandle);
    if (pExpression == NULL)
    {
        return (E_FAIL);
    };

    m_ExpressionByHandle.RemoveKey (lHandle);

    delete pExpression;

    return (S_OK);
}


STDMETHODIMP CCEWatch::GetItemText(long lHandle, BSTR *psText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CExpressionHolder *pExpression;

    pExpression = GetHolder (lHandle);
    if (pExpression == NULL)
    {
        return (E_FAIL);
    };

    *psText = pExpression->GetItemText ().AllocSysString ();
    return (S_OK);
}


STDMETHODIMP CCEWatch::GetItemState(long lHandle, CEWatchItemState *pState)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CExpressionHolder *pExpression;

    pExpression = GetHolder (lHandle);
    if (pExpression == NULL)
    {
        return (E_FAIL);
    };

    *pState = pExpression->GetItemState ();
    return (S_OK);
}


STDMETHODIMP CCEWatch::GetExpressionText (long lHandle, BSTR *psText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CExpressionHolder *pExpression;

    pExpression = GetHolder (lHandle);
    if (pExpression == NULL)
    {
        return (E_FAIL);
    };

    *psText = pExpression->GetExpressionText ().AllocSysString ();
    return (S_OK);
}


STDMETHODIMP CCEWatch::GetExpressionState(long lHandle, CEExpressionState *pState)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CExpressionHolder *pExpression;

    pExpression = GetHolder (lHandle);
    if (pExpression == NULL)
    {
        return (E_FAIL);
    };

    *pState = pExpression->GetExpressionState ();
	return (S_OK);
}


STDMETHODIMP CCEWatch::GetFormatText(long lHandle, BSTR *psText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CExpressionHolder *pExpression;

    pExpression = GetHolder (lHandle);
    if (pExpression == NULL)
    {
        return (E_FAIL);
    };

    *psText = pExpression->GetFormatText ().AllocSysString ();
	return (S_OK);
}


STDMETHODIMP CCEWatch::GetFormatState(long lHandle, CEFormatState *pState)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CExpressionHolder *pExpression;

    pExpression = GetHolder (lHandle);
    if (pExpression == NULL)
    {
        return (E_FAIL);
    };

    *pState = pExpression->GetFormatState ();
	return (S_OK);
}


STDMETHODIMP CCEWatch::GetFormatBase(long lHandle, CENumberBase *pBase)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CExpressionHolder *pExpression;

    pExpression = GetHolder (lHandle);
    if (pExpression == NULL)
    {
        return (E_FAIL);
    };

    *pBase = pExpression->GetFormatBase ();
	return (S_OK);
}


STDMETHODIMP CCEWatch::GetRangesText(long lHandle, BSTR *psText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CExpressionHolder *pExpression;

    pExpression = GetHolder (lHandle);
    if (pExpression == NULL)
    {
        return (E_FAIL);
    };

    *psText = pExpression->GetRangesText ().AllocSysString ();
	return (S_OK);
}


STDMETHODIMP CCEWatch::GetRangesState(long lHandle, CERangesState *pState)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CExpressionHolder *pExpression;

    pExpression = GetHolder (lHandle);
    if (pExpression == NULL)
    {
        return (E_FAIL);
    };

    *pState = pExpression->GetRangesState ();
	return (S_OK);
}


STDMETHODIMP CCEWatch::GetRangeCount(long lHandle, unsigned long *puiCount)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CExpressionHolder *pExpression;

    pExpression = GetHolder (lHandle);
    if (pExpression == NULL)
    {
        return (E_FAIL);
    };

    return (pExpression->GetRangeCount (*puiCount));
}


STDMETHODIMP CCEWatch::GetRange(long lHandle, unsigned long lIndex, long *plLower, long *plUpper)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CExpressionHolder *pExpression;
    CExpressionHolder::Range range;
    HRESULT hr;

    pExpression = GetHolder (lHandle);
    if (pExpression == NULL)
    {
        return (E_FAIL);
    };

    hr = pExpression->GetRange (lIndex, range);
    if (FAILED (hr))
    {
        return (hr);
    };

    *plLower = range.m_lLower;
    *plUpper = range.m_lUpper;
	return (S_OK);
}


STDMETHODIMP CCEWatch::GetType(long lHandle, IUnknown **ppType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CExpressionHolder *pExpression;

    ASSERT (ppType != NULL);
    if (ppType == NULL)
    {
        return (E_POINTER);
    };

    pExpression = GetHolder (lHandle);
    if (pExpression == NULL)
    {
        return (E_FAIL);
    };

    *ppType = pExpression->GetType ();
    if (*ppType == NULL)
    {
        return (S_FALSE);
    };

    (*ppType)->AddRef ();
    return (S_OK);
}


STDMETHODIMP CCEWatch::GetSubItemCount(long lHandle, unsigned long *pulCount)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CExpressionHolder *pExpression;

    *pulCount = 0;

    pExpression = GetHolder (lHandle);
    if (pExpression == NULL)
    {
        return (E_FAIL);
    };

    *pulCount = pExpression->GetSubItemCount ();
    return (S_OK);
}


STDMETHODIMP CCEWatch::GetSubItem(long lHandle, unsigned long ulIndex, long *plSubItemHandle)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CExpressionHolder *pExpression;
    CExpressionHolder *pSubItem;
    HRESULT hr;

    *plSubItemHandle = 0;

    pExpression = GetHolder (lHandle);
    if (pExpression == NULL)
    {
        return (E_FAIL);
    };

    pSubItem = m_pExpressionFactory->NewHolder (pExpression->GetDebugInfo ());
    ASSERT (pSubItem != NULL);
    if (pSubItem == NULL)
    {
        return (E_OUTOFMEMORY);
    };

    hr = pExpression->GetSubItem (ulIndex, pSubItem);
    if (FAILED (hr))
    {
        return (hr);
    };

    *plSubItemHandle = pSubItem->GetHandle ();

    m_ExpressionByHandle.SetAt (*plSubItemHandle, pSubItem);

    pSubItem->SetChangedCB (&m_ExpressionChangedCB);

    Fire_OnExpressionChanged (*plSubItemHandle);

    return (S_OK);
}


STDMETHODIMP CCEWatch::Connect (long lHandle)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CExpressionHolder *pExpression;

    pExpression = GetHolder (lHandle);
    if (pExpression == NULL)
    {
        return (E_FAIL);
    };

    pExpression->Connect ();

    return (S_OK);
}


STDMETHODIMP CCEWatch::Disconnect (long lHandle)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CExpressionHolder *pExpression;

    pExpression = GetHolder (lHandle);
    if (pExpression == NULL)
    {
        return (E_FAIL);
    };

    pExpression->Disconnect ();

    return (S_OK);
}


STDMETHODIMP CCEWatch::GetData(long lHandle, VARIANT *pvarValue, short *psQuality, DATE *pDate)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CExpressionHolder *pExpression;

    pExpression = GetHolder (lHandle);
    if (pExpression == NULL)
    {
        return (E_FAIL);
    };

    VariantInit(pvarValue);
    VariantCopy(pvarValue, &pExpression->GetValue ());
    *psQuality = pExpression->GetQuality ();
    *pDate = pExpression->GetTimestamp ();

	return (S_OK);
}


STDMETHODIMP CCEWatch::GetDataText(long lHandle, BOOL bFormatted, BSTR *psValue, short *psQuality, DATE *pTimestamp)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CExpressionHolder *pExpression;
    CString strValue;
    HRESULT hr;

    pExpression = GetHolder (lHandle);
    if (pExpression == NULL)
    {
        return (E_FAIL);
    };

    hr = pExpression->GetValueText (bFormatted == TRUE, strValue);
    if (FAILED (hr))
    {
        return (hr);
    };

    *psValue = strValue.AllocSysString ();
    *psQuality = pExpression->GetQuality ();
    *pTimestamp = pExpression->GetTimestamp ();

	return S_OK;
}


STDMETHODIMP CCEWatch::GetWriteState(long lHandle, CEWriteState *pState)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CExpressionHolder *pExpression;

    pExpression = GetHolder (lHandle);
    if (pExpression == NULL)
    {
        return (E_FAIL);
    };

    *pState = pExpression->GetWriteState ();
    return (S_OK);
}


STDMETHODIMP CCEWatch::Write(long lHandle, VARIANT varValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CExpressionHolder *pExpression;

    pExpression = GetHolder (lHandle);
    if (pExpression == NULL)
    {
        return (E_FAIL);
    };

    return (pExpression->Write (varValue));
}


STDMETHODIMP CCEWatch::AddItemWithScope(BSTR sText, BSTR sScope, long *plHandle)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    HRESULT hr;
    CExpressionHolder *pExpression;

    *plHandle = 0;

    hr = ParseWithScope (sText, sScope, pExpression);
    if (FAILED (hr))
    {
        return (hr);
    };

    *plHandle = pExpression->GetHandle ();

    m_ExpressionByHandle.SetAt (*plHandle, pExpression);

    pExpression->SetChangedCB (&m_ExpressionChangedCB);

    Fire_OnExpressionChanged (*plHandle);

    return (S_OK);
}


STDMETHODIMP CCEWatch::GetClientData(long lHandle, long *plClientData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CExpressionHolder *pExpression;

    pExpression = GetHolder (lHandle);
    if (pExpression == NULL)
    {
        return (E_FAIL);
    };

    *plClientData = pExpression->GetClientData ();
    return (S_OK);
}

STDMETHODIMP CCEWatch::SetClientData(long lHandle, long lClientData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CExpressionHolder *pExpression;

    pExpression = GetHolder (lHandle);
    if (pExpression == NULL)
    {
        return (E_FAIL);
    };

    pExpression->SetClientData (lClientData);
    return (S_OK);
}


STDMETHODIMP CCEWatch::SetFormatText(long lHandle, BSTR sText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    HRESULT hr;
    CExpressionHolder *pExpression;
    CString strFormat;
    CEWatchItemState state;
    CString strText;
    CString strRanges;

    pExpression = GetHolder (lHandle);
    if (pExpression == NULL)
    {
        return (E_FAIL);
    };

    strFormat = sText;
    hr = ParseFormat (strFormat, pExpression, state);
    if (FAILED (hr))
    {
        return (hr);
    };

    strText = pExpression->GetExpressionText ();

    strRanges = pExpression->GetRangesText ();

    if (!strFormat.IsEmpty () || !strRanges.IsEmpty ())
    {
        strText += _T (",") + strFormat + strRanges;
    };

    pExpression->SetText (strText, state);

	return (S_OK);
}


STDMETHODIMP CCEWatch::AddItemExpand (BSTR sText, CEExpandMode expandMode, long *plHandle)
{
	AFX_MANAGE_STATE (AfxGetStaticModuleState ())
    HRESULT hr;
    CExpressionHolder *pExpression;

    *plHandle = 0;

    if (expandMode == CEExpandQuery)
    {
        return (E_NOTIMPL);
    };

    hr = ParseExpand (sText, expandMode, pExpression);
    if (FAILED (hr))
    {
        return (hr);
    };

    *plHandle = pExpression->GetHandle ();

    m_ExpressionByHandle.SetAt (*plHandle, pExpression);

    pExpression->SetChangedCB (&m_ExpressionChangedCB);

    Fire_OnExpressionChanged (*plHandle);

    return (S_OK);
}


HRESULT CCEWatch::ParseSimple (const CString &strText, CExpressionHolder *&pHolder)
{
    pHolder = NULL;

    if (m_pExpressionFactory == NULL)
    {
        return (E_FAIL);
    };

    pHolder = m_pExpressionFactory->NewHolder (m_pDebugInfo);
    ASSERT (pHolder != NULL);
    if (pHolder == NULL)
    {
        return (E_OUTOFMEMORY);
    };

    return (ParseItem (strText, CEExpandNever, "", pHolder));
}


HRESULT CCEWatch::ParseExpand (const CString &strText, CEExpandMode expandMode, CExpressionHolder *&pHolder)
{
    pHolder = NULL;

    if (m_pExpressionFactory == NULL)
    {
        return (E_FAIL);
    };

    pHolder = m_pExpressionFactory->NewHolder (m_pDebugInfo);
    ASSERT (pHolder != NULL);
    if (pHolder == NULL)
    {
        return (E_OUTOFMEMORY);
    };

    return (ParseItem (strText, expandMode, "", pHolder));
}


HRESULT CCEWatch::ParseWithScope (const CString &strText, const CString &strScope, CExpressionHolder *&pHolder)
{
    pHolder = NULL;

    if (m_pExpressionFactory == NULL)
    {
        return (E_FAIL);
    };

    pHolder = m_pExpressionFactory->NewHolder (m_pDebugInfo);
    ASSERT (pHolder != NULL);
    if (pHolder == NULL)
    {
        return (E_OUTOFMEMORY);
    };

    return (ParseItem (strText, CEExpandNever, strScope, pHolder));
}


CExpressionHolder *CCEWatch::GetHolder (long lHandle)
{
    BOOL bGoodHandle;
    CExpressionHolder *pExpression;

    bGoodHandle = m_ExpressionByHandle.Lookup (lHandle, pExpression);
    ASSERT (bGoodHandle);
    if (!bGoodHandle)
    {
        return (NULL);
    };

    ASSERT (pExpression != NULL);
    return (pExpression);
}


void CCEWatch::OnExpressionChanged (CExpressionHolder *pHolder)
{
    Fire_OnExpressionChanged (pHolder->GetHandle ());
}


void CCEWatch::DisposeExpressions ()
{
    POSITION pPosition;

    while ((pPosition = m_ExpressionByHandle.GetStartPosition ()) != NULL)
    {
        long lHandle;
        CExpressionHolder *pHolder;

        m_ExpressionByHandle.GetNextAssoc (pPosition, lHandle, pHolder);

        if (pHolder->IsConnected ())
        {
            pHolder->Disconnect ();
        };

        m_ExpressionByHandle.RemoveKey (lHandle);

        delete pHolder;
    };
}


CCEWatchAdmin::CCEWatchAdmin ()
{
}


CCEWatchAdmin::~CCEWatchAdmin ()
{
}


STDMETHODIMP CCEWatchAdmin::Resubscribe ()
{
    AFX_MANAGE_STATE (AfxGetStaticModuleState ())

    CETriggerResubscribe ();
    return (S_OK);
}


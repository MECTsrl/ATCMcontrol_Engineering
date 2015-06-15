


#include "stdafx.h"
#include "fc_todebug\fc_assert.h"
#include "fc_todebug\fc_mem.h"
#include "Subscription.h"
#include "utilif.h"


/////////////////////////////////////////////////////////////////////////////
// CSubscription

void *CSubscription::operator new(size_t size)
{
    void *memory;
    FC_NewMemory(&memory, size);
    return memory;
}


void CSubscription::operator delete(void* memory)
{
    FC_FreeMemory(memory);
}


CSubscription *CSubscription::Create (const CString &strObject, BL_IEC_TYP type, CSubscriber *pSubscriber)
{
    return (new CSubscription (strObject, type, pSubscriber));
}


CSubscription::CSubscription (const CString &strObject, BL_IEC_TYP type, CSubscriber *pSubscriber)
: m_strObject (strObject),
  m_Type (type),
  m_pSubscriber (pSubscriber),
  m_hServer (NULL),
  m_bKnown (true),
  m_bHaveData (false),
  m_sQuality (0),
  m_uiReferences (1),
  m_uiSuspensions (1),
  m_pReleaseCB (NULL)
{
}


void CSubscription::RegisterModifiedCB (FC_CALLBACK_BASE (SubscriptionModifiedCB) *pModifiedCB)
{
    m_ModifiedCBs.Add (pModifiedCB);
}


void CSubscription::UnregisterModifiedCB (FC_CALLBACK_BASE (SubscriptionModifiedCB) *pModifiedCB)
{
    m_ModifiedCBs.Remove (pModifiedCB);
}


void CSubscription::SetReleaseCB (FC_CALLBACK_BASE (SubscriptionReleaseCB) *pReleaseCB)
{
    m_pReleaseCB = pReleaseCB;
}


CString CSubscription::GetObject () const
{
    return (m_strObject);
}


BL_IEC_TYP CSubscription::GetType () const
{
    return (m_Type);
}


bool CSubscription::HaveData () const
{
    return (m_bHaveData);
}


bool CSubscription::IsKnown () const
{
    return (m_bKnown);
}


LONG CSubscription::GetServer () const
{
    return (m_hServer);
}


CString CSubscription::GetValue () const
{
    return (m_strValue);
}


CComVariant CSubscription::GetVariantValue () const
{
    return (m_varValue);
}


short CSubscription::GetQuality () const
{
    return (m_sQuality);
}


DATE CSubscription::GetTimestamp () const
{
    return (m_Timestamp);
}


void CSubscription::SetData (CComVariant varValue, short sQuality, DATE timestamp)
{
    if (varValue.vt == VT_BSTR)
    {
        m_strValue = varValue.bstrVal;
    }
    else if (varValue.vt == VT_EMPTY)
    {
        m_strValue = "";
    }
    else
    {
        HRESULT hr;
        OLECHAR szValue[500];

        hr = BL_VariantToString (&varValue, m_Type, NULL, BL_USE_IEC_FORMAT, szValue, sizeof (szValue) / sizeof (OLECHAR));
        if (FAILED (hr))
        {
            FC_DEBUGPRINT1 (_T ("CESUBSCR> Failed to convert notification value (hr=0x%08lx)"), (long)hr);
            return;
        };

        m_strValue = szValue;
    };

    m_varValue = varValue;
    m_sQuality = sQuality;
    m_Timestamp = timestamp;
    m_bHaveData = true;

    // If a variable is subscribed while the variable session
    // is not connected CSC_Online cannot validate the variable
    // name. As soon as the variable session gets connected and
    // we receive a notification we regard the variable name
    // valid.

    m_bKnown = true;

    m_ModifiedCBs.Execute (this);
}


HRESULT CSubscription::Reference (unsigned int uiSuspensions)
{
    HRESULT hr=S_OK;


    // Subscriptions with reference count zero are strange.

    assert (m_uiReferences != 0);
    if (m_uiReferences == 0)
    {
        FC_DEBUGPRINT4 (_T ("CESUBSCR> Trying to reference subscription for %s with rc=0 (s=0x%08lx, h=0x%08lx, sc=%ld)\n"),
                       (const char *)m_strObject, (long)this, (long)m_hServer, (long)m_uiSuspensions);
        return (E_FAIL);
    };


    // Suspension count decrement must not overflow suspension count.

    assert (m_uiSuspensions + uiSuspensions >= m_uiSuspensions);
    if (m_uiSuspensions + uiSuspensions < m_uiSuspensions)
    {
        FC_DEBUGPRINT6 (_T ("CESUBSCR> Trying to reference subscription for %s but sc+scinc<sc (s=0x%08lx, h=0x%08lx, rc=%ld, sc=%ld, scinc=%ld)\n"),
                        (const char *)m_strObject, (long)this, (long)m_hServer, (long)m_uiReferences, (long)m_uiSuspensions, (long)uiSuspensions);
        return (E_FAIL);
    };


    // Suspension count increment must not increment suspension
    // count beyond reference count.

    assert (m_uiSuspensions + uiSuspensions <= m_uiReferences);
    if (m_uiSuspensions + uiSuspensions > m_uiReferences)
    {
        FC_DEBUGPRINT6 (_T ("CESUBSCR> Trying to reference subscription for %s but sc+scinc>rc (s=0x%08lx, h=0x%08lx, rc=%ld, sc=%ld, scinc=%ld)\n"),
                        (const char *)m_strObject, (long)this, (long)m_hServer, (long)m_uiReferences, (long)m_uiSuspensions, (long)uiSuspensions);
        return (E_FAIL);
    };


    // Increment reference count by one and suspension count
    // by additional suspension count. Do (un)subscribing.
    // If (un)subscribing fails restore previous state.

    ++m_uiReferences;
    m_uiSuspensions += uiSuspensions;

    hr = DoSubscribing ();
    if (FAILED (hr))
    {
        --m_uiReferences;
        m_uiSuspensions -= uiSuspensions;
        return (hr);
    };

    FC_DEBUGPRINT5 (_T ("CESUBSCR> Referenced subscription for %s (s=0x%08lx, h=0x%08lx, rc=%ld, sc=%ld)\n"),
                    (const char *)m_strObject, (long)this, (long)m_hServer, (long)m_uiReferences, (long)m_uiSuspensions);

    return (hr);
}


HRESULT CSubscription::Unreference (unsigned int uiSuspensions)
{
    HRESULT hr=S_OK;


    // Subscriptions with reference count zero are strange.

    assert (m_uiReferences != 0);
    if (m_uiReferences == 0)
    {
        FC_DEBUGPRINT4 (_T ("CESUBSCR> Trying to unreference subscription for %s with rc=0 (s=0x%08lx, h=0x%08lx, sc=%ld)\n"),
                       (const char *)m_strObject, (long)this, (long)m_hServer, (long)m_uiSuspensions);
        return (E_FAIL);
    };


    // Suspension count decrement must not exceed suspension count.

    assert (m_uiSuspensions >= uiSuspensions);
    if (m_uiSuspensions < uiSuspensions)
    {
        FC_DEBUGPRINT6 (_T ("CESUBSCR> Trying to reference subscription for %s but scdec>sc (s=0x%08lx, h=0x%08lx, rc=%ld, sc=%ld, scdec=%ld)\n"),
                        (const char *)m_strObject, (long)this, (long)m_hServer, (long)m_uiReferences, (long)m_uiSuspensions, (long)uiSuspensions);
        return (E_FAIL);
    };


    // Decrement reference count by one and suspension count
    // by suspension count decrement. Do (un)subscribing.
    // If (un)subscribing fails restore previous state.

    --m_uiReferences;
    m_uiSuspensions -= uiSuspensions;

    hr = DoSubscribing ();
    if (FAILED (hr))
    {
        ++m_uiReferences;
        m_uiSuspensions += uiSuspensions;
        return (hr);
    };

    FC_DEBUGPRINT5 (_T ("CESUBSCR> Unreferenced subscription for %s (s=0x%08lx, h=0x%08lx, rc=%ld, sc=%ld)\n"),
                    (const char *)m_strObject, (long)this, (long)m_hServer, (long)m_uiReferences, (long)m_uiSuspensions);

    if (m_uiReferences == 0)
    {
        if (m_pReleaseCB != NULL)
        {
            m_pReleaseCB->Execute (this);
        };

        delete this;
    };

    return (S_OK);
}


unsigned int CSubscription::GetReferences ()
{
    return (m_uiReferences);
}


HRESULT CSubscription::Suspend ()
{
    HRESULT hr=S_OK;


    // Subscriptions with reference count zero are strange.

    assert (m_uiReferences != 0);
    if (m_uiReferences == 0)
    {
        FC_DEBUGPRINT4 (_T ("CESUBSCR> Trying to suspend subscription for %s with rc=0 (s=0x%08lx, h=0x%08lx, sc=%ld)\n"),
                       (const char *)m_strObject, (long)this, (long)m_hServer, (long)m_uiSuspensions);
        return (E_FAIL);
    };


    // Suspension count must be less than reference count
    // for suspending to be valid.

    assert (m_uiSuspensions < m_uiReferences);
    if (m_uiSuspensions >= m_uiReferences)
    {
        FC_DEBUGPRINT5 (_T ("CESUBSCR> Trying to suspend subscription for %s but sc>=rc (s=0x%08lx, h=0x%08lx, rc=%ld, sc=%ld)\n"),
                        (const char *)m_strObject, (long)this, (long)m_hServer, (long)m_uiReferences, (long)m_uiSuspensions);
        return (E_FAIL);
    };


    // Increment suspension count by one. Do (un)subscribing.
    // If (un)subscribing fails restore previous state.

    ++m_uiSuspensions;

    hr = DoSubscribing ();
    if (FAILED (hr))
    {
        --m_uiSuspensions;
        return (hr);
    };

    FC_DEBUGPRINT5 (_T ("CESUBSCR> Suspended subscription for %s (s=0x%08lx, h=0x%08lx, rc=%ld, sc=%ld)\n"),
                    (const char *)m_strObject, (long)this, (long)m_hServer, (long)m_uiReferences, (long)m_uiSuspensions);

    return (hr);
}


HRESULT CSubscription::Resume ()
{
    HRESULT hr=S_OK;


    // Subscriptions with reference count zero are strange.

    assert (m_uiReferences != 0);
    if (m_uiReferences == 0)
    {
        FC_DEBUGPRINT4 (_T ("CESUBSCR> Trying to resume subscription for %s with rc=0 (s=0x%08lx, h=0x%08lx, sc=%ld)\n"),
                       (const char *)m_strObject, (long)this, (long)m_hServer, (long)m_uiSuspensions);
        return (E_FAIL);
    };


    // Suspension count must not be zero for resuming to be valid.

    assert (m_uiSuspensions != 0);
    if (m_uiSuspensions == 0)
    {
        FC_DEBUGPRINT4 (_T ("CESUBSCR> Trying to resume subscription for %s but sc=0 (s=0x%08lx, h=0x%08lx, rc=%ld)\n"),
                        (const char *)m_strObject, (long)this, (long)m_hServer, (long)m_uiReferences);
        return (E_FAIL);
    };


    // Decrement suspension count by one. Do (un)subscribing.
    // If (un)subscribing fails restore previous state.

    --m_uiSuspensions;

    hr = DoSubscribing ();
    if (FAILED (hr))
    {
        ++m_uiSuspensions;
        return (hr);
    };

    FC_DEBUGPRINT5 (_T ("CESUBSCR> Resumed subscription for %s (s=0x%08lx, h=0x%08lx, rc=%ld, sc=%ld)\n"),
                    (const char *)m_strObject, (long)this, (long)m_hServer, (long)m_uiReferences, (long)m_uiSuspensions);

    return (hr);
}


unsigned int CSubscription::GetSuspensions ()
{
    return (m_uiSuspensions);
}


HRESULT CSubscription::DoSubscribing ()
{
    assert (m_uiSuspensions <= m_uiReferences);
    if (m_uiSuspensions > m_uiReferences)
    {
        return (E_FAIL);
    };

    if (m_hServer == NULL && m_uiSuspensions < m_uiReferences)
    {
        return (m_pSubscriber->Subscribe (m_strObject, m_Type, m_hServer, m_bKnown));
    };

    if (m_hServer != NULL && m_uiSuspensions == m_uiReferences)
    {
        HRESULT hr;

        hr = m_pSubscriber->Unsubscribe (m_strObject, m_hServer);
        if (FAILED (hr))
        {
            return (hr);
        };

        m_hServer = NULL;
        return (S_OK);
    };

    return (S_OK);
}


HRESULT CSubscription::Read ()
{
    HRESULT hr;
    CComVariant varValue;
    short sQuality;
    DATE timestamp;

    if (m_hServer == NULL)
    {
        return (E_FAIL);
    };

    hr = m_pSubscriber->Read (m_strObject, m_hServer, varValue, sQuality, timestamp);
    if (FAILED (hr))
    {
        return (hr);
    };

    SetData (varValue, sQuality, timestamp);
    return (S_OK);
}


HRESULT CSubscription::Write (const CComVariant &varValue)
{
    if (m_hServer == NULL)
    {
        return (E_FAIL);
    };

    return (m_pSubscriber->Write (m_strObject, m_hServer, varValue));
}


HRESULT CSubscription::Resubscribe ()
{
    HRESULT hr;

    if (m_hServer == NULL)
        return (S_OK);

    ASSERT (m_pSubscriber != NULL);
    if (m_pSubscriber == NULL)
    {
        return (E_FAIL);
    };

    hr = m_pSubscriber->Unsubscribe (m_strObject, m_hServer);
    if (FAILED (hr))
    {
        return (hr);
    };
    m_hServer = NULL;

    hr = m_pSubscriber->Subscribe (m_strObject, m_Type, m_hServer, m_bKnown);
    if (FAILED (hr))
    {
        return (hr);
    };

    return (S_OK);
}


/////////////////////////////////////////////////////////////////////////////
// CSubscriber


CSubscriber *CSubscriber::Create (CComPtr<IFCVariableCollection> pIFCVarCollection)
{
    return (new CSubscriber (pIFCVarCollection));
}


void CSubscriber::Delete ()
{
    if (m_bDelayedDelete)
    {
        ASSERT (m_bEntered);
        return;
    };

    if (m_bEntered)
    {
        m_bDelayedDelete = true;
        return;
    };

    delete this;
}


CSubscriber::CSubscriber (CComPtr<IFCVariableCollection> pIFCVarCollection)
: m_pIFCVarCollection (pIFCVarCollection),
  m_bEntered (false),
  m_bDelayedDelete (false)
{
}


CSubscriber::~CSubscriber ()
{
}


HRESULT CSubscriber::Subscribe (const CString &strObject, BL_IEC_TYP type, long &hServer, bool &bKnown)
{
    HRESULT hr;

    if (m_bEntered)
    {
        FC_DEBUGPRINT1 (_T ("CESUBSCR> Recursive entry to <Subscribe> for object %s\n"),
                        (const char *)strObject);
        return (E_FAIL);
    };
    Enter ();

    hr = m_pIFCVarCollection->Subscribe (CComBSTR (strObject), &hServer);
    if (FAILED (hr))
    {
        FC_DEBUGPRINT2 (_T ("CESUBSCR> Failed to subscribe for object %s (hr=0x%08lx)\n"),
		                (const char *)strObject, (long)hr);
        Leave ();
        return (hr);
    };

    bKnown = hr == S_OK;

    if (bKnown)
    {
        FC_DEBUGPRINT2 (_T ("CESUBSCR> Subscribed object %s (h=0x%08lx)\n"),
                        (const char *)strObject, (long)hServer);
    }
    else
    {
        FC_DEBUGPRINT2 (_T ("CESUBSCR> Subscribed unknown object %s (h=0x%08lx)\n"),
                        (const char *)strObject, (long)hServer);
    };

    Leave ();
    return (S_OK);
}


HRESULT CSubscriber::Unsubscribe (const CString &strObject, long hServer)
{
    HRESULT hr;

    if (m_bEntered)
    {
        FC_DEBUGPRINT2 (_T ("CESUBSCR> Recursive entry to <Unsubscribe> for object %s (h=0x%08lx)\n"),
                        (const char *)strObject, hServer);
        return (E_FAIL);
    };
    Enter ();

    hr = m_pIFCVarCollection->UnsubscribeWithHandle (hServer);
    FC_DEBUGPRINT3 (_T ("CESUBSCR> Unsubscribed object %s (h=0x%08lx, hr=0x%08lx)\n"),
                    (const char *)strObject, (long)hServer, (long)hr);

    Leave ();

    // CSC_Online strategy is to unsubscribe as good as possible
    // even if there is some error. So for our purpose we assume
    // to have unsubscribed in case of CSC_Online returning an
    // error code.
    return (S_OK);
}


HRESULT CSubscriber::Read (const CString &strObject, long hServer, CComVariant &varValue, short &sQuality, DATE &timestamp)
{
    HRESULT hr;

    if (m_bEntered)
    {
        FC_DEBUGPRINT2 (_T ("CESUBSCR> Recursive entry to <Read> for object %s (h=0x%08lx)\n"),
                        (const char *)strObject, hServer);
        return (E_FAIL);
    };
    Enter ();

    hr = m_pIFCVarCollection->ReadWithHandle (hServer, &sQuality, &timestamp, &varValue);
    if (FAILED (hr))
    {
        Leave ();
        return (hr);
    };

    Leave ();
    return (S_OK);
}


HRESULT CSubscriber::Write (const CString &strObject, long hServer, const CComVariant &varValue)
{
    HRESULT hr;

    if (m_bEntered)
    {
        FC_DEBUGPRINT2 (_T ("CESUBSCR> Recursive entry to <Read> for object %s (h=0x%08lx)\n"),
                        (const char *)strObject, hServer);
        return (E_FAIL);
    };
    Enter ();

    hr = m_pIFCVarCollection->WriteWithHandle (hServer, varValue);

    Leave ();
    return (hr);
}


void CSubscriber::Enter ()
{
    m_bEntered = true;
}


void CSubscriber::Leave ()
{
    ASSERT (m_bEntered);

    m_bEntered = false;

    if (m_bDelayedDelete)
    {
        delete this;
    };
}


/////////////////////////////////////////////////////////////////////////////
// CSubscriptionManager

CSubscriptionManager::CSubscriptionManager ()
: m_bInitialized (false),
  m_pSubscriber (NULL),
  m_ReleaseCB (this, OnSubscriptionReleased),
  m_pNotifyCB (NULL),
  m_pVariableNotify (NULL),
  m_dwVarCookie (0)
{
}


void CSubscriptionManager::SetNotifyCB (FC_CALLBACK_BASE (SubscriptionNotifyCB) *pNotifyCB)
{
	m_pNotifyCB = pNotifyCB;
}


HRESULT CSubscriptionManager::Start ()
{
	HRESULT hr;

    if (m_bInitialized)
    {
        return (E_FAIL);
    };

	hr = CoCreateInstance (CLSID_FCVariableCollection, NULL, CLSCTX_ALL, IID_IFCVariableCollection, (LPVOID *)&m_pIFCVarCollection);
	if (FAILED (hr))
	{
        UTIL_FatalError (_T ("Failed to create VariableCollection component"), hr);
		FC_DEBUGPRINT1 (_T ("CESUBSCR> Failed to CoCreateInstance FCVariableCollection object (hr=0x%08lx)\n"), (long)hr);
		return (hr);
	};

    m_pSubscriber = CSubscriber::Create (m_pIFCVarCollection);
    if (m_pSubscriber == NULL)
    {
        FC_DEBUGPRINT (_T ("CESUBSCR> Failed to create Subscriber object"));
        m_pIFCVarCollection = NULL;
        return (E_OUTOFMEMORY);
    };

	hr = CComObject<CVariableNotify>::CreateInstance (&m_pVariableNotify);
	if (FAILED (hr))
	{
		FC_DEBUGPRINT1 (_T ("CESUBSCR> Failed to create notification object (hr=0x%08lx)\n"), (long)hr);
        m_pSubscriber->Delete ();
        m_pSubscriber = NULL;
		m_pIFCVarCollection = NULL;
		return (hr);
	};
    m_pVariableNotify->AddRef ();

	hr = AtlAdvise (m_pIFCVarCollection, m_pVariableNotify->GetUnknown (), IID__FCVariableNotification, &m_dwVarCookie);
	if (FAILED (hr))
	{
		FC_DEBUGPRINT1 (_T ("CESUBSCR> Failed to advise variable notification (hr=0x%08lx)\n"), (long)hr);
        m_pSubscriber->Delete ();
        m_pSubscriber = NULL;
		m_pIFCVarCollection = NULL;
		return (hr);
	};

	m_pVariableNotify->Init (this);

    m_bInitialized = true;
	return (S_OK);
}


HRESULT CSubscriptionManager::Stop ()
{
	HRESULT hr;

    if (!m_bInitialized)
    {
        return (E_FAIL);
    };

    m_pVariableNotify->Init (NULL);

	hr = AtlUnadvise (m_pIFCVarCollection, IID__FCVariableNotification, m_dwVarCookie);
	if (FAILED (hr))
	{
		FC_DEBUGPRINT1 (_T ("CESUBSCR> Failed to unadvise variable notification (hr=0x%08lx)\n"), (long)hr);
	};

    m_pVariableNotify->Release ();
    m_pVariableNotify = NULL;

    m_pSubscriber->Delete ();
    m_pSubscriber = NULL;

	m_pIFCVarCollection = NULL;

    while (!m_SubscriptionList.IsEmpty ())
    {
        CSubscription *pSubscription;

        pSubscription = m_SubscriptionList.RemoveHead ();
        pSubscription->SetReleaseCB (NULL);
    };

    m_bInitialized = false;
	return (hr);
}


bool CSubscriptionManager::IsInitialized ()
{
    return (m_bInitialized);
}


HRESULT CSubscriptionManager::GetSubscription (const CString &strObject, BL_IEC_TYP type, CSubscription *&pSubscription)
{
	POSITION pPosition;
	HRESULT hr;

    if (!m_bInitialized)
    {
        return (E_FAIL);
    };

	pPosition = m_SubscriptionList.GetHeadPosition ();
	while (pPosition != NULL)
	{
		pSubscription = m_SubscriptionList.GetNext (pPosition);
		assert (pSubscription != NULL);
		if (pSubscription == NULL)
			continue;

		if (strObject.CompareNoCase (pSubscription->GetObject ()) == 0)
		{
			hr = pSubscription->Reference ();
			if (FAILED (hr))
			{
				pSubscription = NULL;
				return (hr);
			};

			return (S_OK);
		};
	};

    pSubscription = CSubscription::Create (strObject, type, m_pSubscriber);
	assert (pSubscription != NULL);
	if (pSubscription == NULL)
	{
		return (E_FAIL);
	};

	pSubscription->SetReleaseCB (&m_ReleaseCB);

	m_SubscriptionList.AddTail (pSubscription);

	FC_DEBUGPRINT2 (_T ("CESUBSCR> Added subscription for %s (s=0x%08lx)\n"),
	                (const char *)strObject, (long)pSubscription);

	hr = pSubscription->Resume ();
	if (FAILED (hr))
	{
		(void)pSubscription->Unreference (1);
		pSubscription = NULL;
		return (hr);
	};

	return (S_OK);
}


void CSubscriptionManager::OnNewData (LONG hServer, const CComVariant &varValue, short sQuality, DATE date)
{
	bool bFound=false;
	POSITION pPosition;
	CSubscription *pSubscription;
	HRESULT hr=S_OK;
	CString strValue;

	pPosition = m_SubscriptionList.GetHeadPosition ();
	while (pPosition != NULL)
	{
		pSubscription = m_SubscriptionList.GetNext (pPosition);
		assert (pSubscription != NULL);
		if (pSubscription == NULL)
			continue;

		if (hServer == pSubscription->GetServer ())
		{
			bFound = true;
			break;
		};
	};

	if (!bFound)
	{
		FC_DEBUGPRINT1 (_T ("CESUBSCR> Did not find subscription for notification of handle 0x%08lx\n"), (long)hServer);
		return;
	};

#if 0
	FC_DEBUGPRINT3 (_T ("CESUBSCR> Found subscription for notification of handle 0x%08lx (s=0x%08lx, o=%s)\n"),
	                (long)hServer, (long)pSubscription, (const char *)pSubscription->GetObject ());
#endif

	pSubscription->SetData (varValue, sQuality, date);

	if (m_pNotifyCB != NULL)
	{
		m_pNotifyCB->Execute (pSubscription);
	};
}


void CSubscriptionManager::OnSubscriptionReleased (CSubscription *pSubscription)
{
	POSITION pPosition;

	pPosition = m_SubscriptionList.Find (pSubscription);
	assert (pPosition != NULL);
	if (pPosition != NULL)
	{
		m_SubscriptionList.RemoveAt (pPosition);
	};

	FC_DEBUGPRINT3 (_T ("CESUBSCR> Removed subscription for %s (s=0x%08lx, h=0x%08lx)\n"),
	                (const char *)pSubscription->GetObject (), (long)pSubscription, (long)pSubscription->GetServer ());
}


POSITION CSubscriptionManager::GetHeadPosition ()
{
	return (m_SubscriptionList.GetHeadPosition ());
}


CSubscription *CSubscriptionManager::GetNext (POSITION &pPosition)
{
	return (m_SubscriptionList.GetNext (pPosition));
}


void CSubscriptionManager::ResubscribeAll ()
{
    POSITION pPosition;

    pPosition = m_SubscriptionList.GetHeadPosition ();
    while (pPosition != NULL)
    {
        CSubscription *pSubscription;

        pSubscription = m_SubscriptionList.GetNext (pPosition);
        ASSERT (pSubscription != NULL);
        if (pSubscription == NULL)
            continue;

        pSubscription->Resubscribe ();
    };
}


/////////////////////////////////////////////////////////////////////////////
// CVariableNotify

CVariableNotify::CVariableNotify ()
: m_pSubscriptionManager (NULL)
{
}


void CVariableNotify::Init (CSubscriptionManager *pSubscriptionManager)
{
	m_pSubscriptionManager = pSubscriptionManager;
}


STDMETHODIMP_(HRESULT) CVariableNotify::OnChanged ()
{
    return S_OK;
}


STDMETHODIMP_(HRESULT) CVariableNotify::OnNewData (/*[in]*/ long hServer, /*[in]*/ VARIANT newVal, /*[in]*/ short Quality, /*[in]*/ DATE Date)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState ())

	if (m_pSubscriptionManager == NULL)
	{
        return S_OK; 
    };

	m_pSubscriptionManager->OnNewData (hServer, newVal, Quality, Date);
    return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// SFCHelper

const LPCTSTR SFCHelper::VisName = _T ("__sfcvis");
const LPCTSTR SFCHelper::ManualModeName = _T ("__manualMode");
const LPCTSTR SFCHelper::DoNextName = _T ("__doNext");
const LPCTSTR SFCHelper::OnceForcedActionsName = _T ("__onceForcedActions");
const LPCTSTR SFCHelper::ForcedActionsName = _T ("__forcedActions");
const LPCTSTR SFCHelper::BlockedActionsName = _T ("__blockedActions");
const LPCTSTR SFCHelper::AllActionsOffName = _T ("__allActionsOff");
const LPCTSTR SFCHelper::EnabledTransitionsName = _T ("__enabledTransitions");
const LPCTSTR SFCHelper::OnceForcedTransitionsName = _T ("__onceForcedTransitions");
const LPCTSTR SFCHelper::ForcedTransitionsName = _T ("__forcedTransitions");
const LPCTSTR SFCHelper::BlockedTransitionsName = _T ("__blockedTransitions");
const LPCTSTR SFCHelper::AllTransitionsOnName = _T ("__allTransitionsOn");
const LPCTSTR SFCHelper::ManCmdName = _T ("__manCmd");

const TCHAR SFCHelper::ListSeparator = _T (';');

const LPCTSTR SFCHelper::StepFieldX = _T ("X");
const LPCTSTR SFCHelper::ActionFieldA = _T ("A");


bool SFCHelper::IsStepInVis (const CString &strSFCVis, long lStepNumber)
{
    CString strSteps;
    CString strActions;
    CString strTransitions;

    if (!SplitVis (strSFCVis, strSteps, strActions, strTransitions))
    {
        return (false);
    };

    return (IsNumberInList (strSteps, lStepNumber));
}


bool SFCHelper::IsActionInVis (const CString &strSFCVis, long lActionNumber)
{
    CString strSteps;
    CString strActions;
    CString strTransitions;

    if (!SplitVis (strSFCVis, strSteps, strActions, strTransitions))
    {
        return (false);
    };

    return (IsNumberInList (strActions, lActionNumber));
}


bool SFCHelper::IsTransitionInVis (const CString &strSFCVis, long lTransitionNumber)
{
    CString strSteps;
    CString strActions;
    CString strTransitions;

    if (!SplitVis (strSFCVis, strSteps, strActions, strTransitions))
    {
        return (false);
    };

    return (IsNumberInList (strTransitions, lTransitionNumber));
}


bool SFCHelper::StepsInVis (const CString &strSFCVis, CArray<long, long> &steps)
{
    CString strSteps;
    CString strActions;
    CString strTransitions;
    long lStep;

    steps.RemoveAll ();

    if (!SplitVis (strSFCVis, strSteps, strActions, strTransitions))
    {
        return (false);
    };

    while (!strSteps.IsEmpty ())
    {
        if (!ExtractNumberFromList (strSteps, lStep))
        {
            steps.RemoveAll ();
            return (false);
        };

        steps.Add (lStep);
    };

    return (true);
}


bool SFCHelper::ActionsInVis (const CString &strSFCVis, CArray<long, long> &actions)
{
    CString strSteps;
    CString strActions;
    CString strTransitions;
    long lAction;

    actions.RemoveAll ();

    if (!SplitVis (strSFCVis, strSteps, strActions, strTransitions))
    {
        return (false);
    };

    while (!strActions.IsEmpty ())
    {
        if (!ExtractNumberFromList (strActions, lAction))
        {
            actions.RemoveAll ();
            return (false);
        };

        actions.Add (lAction);
    };

    return (true);
}


bool SFCHelper::TransitionsInVis (const CString &strSFCVis, CArray<long, long> &transitions)
{
    CString strSteps;
    CString strActions;
    CString strTransitions;
    long lStep;

    transitions.RemoveAll ();

    if (!SplitVis (strSFCVis, strSteps, strActions, strTransitions))
    {
        return (false);
    };

    while (!strTransitions.IsEmpty ())
    {
        if (!ExtractNumberFromList (strTransitions, lStep))
        {
            transitions.RemoveAll ();
            return (false);
        };

        transitions.Add (lStep);
    };

    return (true);
}


bool SFCHelper::StepsInList (const CString &strList, CArray<long, long> &steps)
{
    return (NumbersFromList (strList, steps));
}


bool SFCHelper::ActionsInList (const CString &strList, CArray<long, long> &actions)
{
    return (NumbersFromList (strList, actions));
}


bool SFCHelper::TransitionsInList (const CString &strList, CArray<long, long> &transitions)
{
    return (NumbersFromList (strList, transitions));
}


bool SFCHelper::AddStepToList (CString &strList, long lStepNumber)
{
    return (AddNumberToList (strList, lStepNumber));
}


bool SFCHelper::AddActionToList (CString &strList, long lActionNumber)
{
    return (AddNumberToList (strList, lActionNumber));
}


bool SFCHelper::AddTransitionToList (CString &strList, long lTransitionNumber)
{
    return (AddNumberToList (strList, lTransitionNumber));
}


bool SFCHelper::RemoveStepFromList (CString &strList, long lStepNumber)
{
    return (RemoveNumberFromList (strList, lStepNumber));
}


bool SFCHelper::RemoveActionFromList (CString &strList, long lActionNumber)
{
    return (RemoveNumberFromList (strList, lActionNumber));
}


bool SFCHelper::RemoveTransitionFromList (CString &strList, long lTransitionNumber)
{
    return (RemoveNumberFromList (strList, lTransitionNumber));
}


bool SFCHelper::SplitVis (const CString &strSFCVis, CString &strSteps, CString &strActions, CString &strTransitions)
{
    CString strItems;

    strItems = strSFCVis;

    if (!ExtractCountedList (strItems, strSteps))
    {
        return (false);
    };

    if (!ExtractCountedList (strItems, strActions))
    {
        return (false);
    };

    if (!ExtractCountedList (strItems, strTransitions))
    {
        return (false);
    };

    return (true);
}


bool SFCHelper::ExtractCountedList (CString &strItems, CString &strList)
{
    long lNumber;
    int iPos;

    if (!ExtractNumberFromList (strItems, lNumber))
    {
        return (false);
    };

    if (lNumber < 0)
    {
        return (false);
    };

    iPos = 0;
    while (lNumber != 0)
    {
        iPos = strItems.Find (ListSeparator, iPos) + 1;
        if (iPos == 0)
        {
            return (false);
        };

        --lNumber;
    };

    if (iPos == 0)
    {
        strList.Empty ();
    }
    else
    {
        strList = strItems.Left (iPos - 1);
    };

    strItems = strItems.Mid (iPos);

    return (true);
}


bool SFCHelper::IsNumberInList (const CString &strList, long lNumber)
{
    CString strItems;

    strItems = strList;

    while (!strItems.IsEmpty ())
    {
        long lItem;

        if (ExtractNumberFromList (strItems, lItem))
        {
            if (lItem == lNumber)
            {
                return (true);
            };
        };
    };

    return (false);
}


bool SFCHelper::NumbersFromList (const CString &strList, CArray<long, long> &numbers)
{
    CString strItems;

    numbers.RemoveAll ();

    strItems = strList;

    while (!strItems.IsEmpty ())
    {
        long lItem;

        if (!ExtractNumberFromList (strItems, lItem))
        {
            return (false);
        };

        numbers.Add (lItem);
    };

    return (true);
}


bool SFCHelper::ListFromNumbers (CString &strList, const CArray<long, long> &numbers)
{
    int iIndex;

    strList = "";

    for (iIndex = 0; iIndex < numbers.GetSize (); ++iIndex)
    {
        CString strNumber;

        if (iIndex != 0)
        {
            strList += _T (";");
        };

        strNumber.Format ("%ld", numbers[iIndex]);

        strList += strNumber;
    };

    return (true);
}


bool SFCHelper::ExtractNumberFromList (CString &strItems, long &lItem)
{
    int iPos;
    CString strItem;
    LPCTSTR pszItem;
    LPTSTR pszEnd=NULL;
    bool bGood;

    iPos = strItems.Find (ListSeparator);

    if (iPos == -1)
    {
        strItem = strItems;
        strItems.Empty ();
    }
    else
    {
        strItem = strItems.Left (iPos);
        strItems = strItems.Mid (iPos + 1);
    };

    strItem.TrimLeft ();
    strItem.TrimRight ();

    pszItem = strItem.GetBuffer (0);

    errno = 0;
    lItem = _tcstol (pszItem, &pszEnd, 10);
    bGood = errno == 0 && (pszEnd == NULL || *pszEnd == _T ('\0'));
    strItem.ReleaseBuffer ();

    return (bGood);
}


bool SFCHelper::AddNumberToList (CString &strList, long lNumber)
{
    CArray<long, long> numbers;
    int iIndex;

    if (!NumbersFromList (strList, numbers))
    {
        return (false);
    };

    for (iIndex = 0; iIndex < numbers.GetSize (); ++iIndex)
    {
        if (numbers[iIndex] == lNumber)
        {
            return (true);
        };
    };

    numbers.Add (lNumber);

    return (ListFromNumbers (strList, numbers));
}


bool SFCHelper::RemoveNumberFromList (CString &strList, long lNumber)
{
    CArray<long, long> numbers;
    int iIndex;

    if (!NumbersFromList (strList, numbers))
    {
        return (false);
    };

    for (iIndex = 0; iIndex < numbers.GetSize (); ++iIndex)
    {
        if (numbers[iIndex] == lNumber)
        {
            numbers.RemoveAt (iIndex);
            return (ListFromNumbers (strList, numbers));
        };
    };

    return (true);
}

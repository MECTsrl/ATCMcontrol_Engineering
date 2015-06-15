

#include "stdafx.h"
#include <afxadv.h>
#include "fc_todebug\fc_assert.h"
#include "PrgEditorIF.h"
#include "PrgEditorDoc.h"
#include "CEMainDrivers.h"
#include "DbgInfo.h"
#include "Watchpoint.h"
#include "CEWatchBE_i.c"
#include "DbgInfo_i.c"
#include "DragItem.h"
// For SFCHelper class only
#include "Subscription.h"
// Subscription library does not supply CSC_Online symbols
#include "CSC_Online_i.c"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL (CWatchExpressionDragItem, CObject, CB_FORMAT_4CWATCHEXPR_VERSION)


/////////////////////////////////////////////////////////////////////////////
// CSTStepClientData

CSTStepClientData::CSTStepClientData (const CStdEditorPosition &keywordPosition, 
                                      const CStdEditorPosition &behindIdPosition)
: m_KeywordPosition (keywordPosition),
  m_BehindIdPosition (behindIdPosition)
{
}


CStdEditorPosition CSTStepClientData::GetKeywordPosition ()
{
    return (m_KeywordPosition);
}


CStdEditorPosition CSTStepClientData::GetBehindIdPosition ()
{
    return (m_BehindIdPosition);
}

CSTActionClientData::CSTActionClientData (const CStdEditorPosition &keywordPosition, const CStdEditorPosition &behindIdPosition)
: m_KeywordPosition (keywordPosition),
  m_BehindIdPosition (behindIdPosition)
{
}


CStdEditorPosition CSTActionClientData::GetKeywordPosition ()
{
    return (m_KeywordPosition);
}


CStdEditorPosition CSTActionClientData::GetBehindIdPosition ()
{
    return (m_BehindIdPosition);
}


CSTTransitionClientData::CSTTransitionClientData (const CStdEditorPosition &keywordPosition, const CStdEditorPosition &behindIdPosition)
: m_KeywordPosition (keywordPosition),
  m_BehindIdPosition (behindIdPosition)
{
}


CStdEditorPosition CSTTransitionClientData::GetKeywordPosition ()
{
    return (m_KeywordPosition);
}


CStdEditorPosition CSTTransitionClientData::GetBehindIdPosition ()
{
    return (m_BehindIdPosition);
}

/////////////////////////////////////////////////////////////////////////////
// CPrgEditorDoc

IMPLEMENT_DYNAMIC(CPrgEditorDoc, CStdEditorDoc)

BEGIN_MESSAGE_MAP(CPrgEditorDoc, CStdEditorDoc)
	//{{AFX_MSG_MAP(CPrgEditorDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CPrgEditorDoc, CStdEditorDoc)
	//{{AFX_DISPATCH_MAP(CPrgEditorDoc)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


BEGIN_INTERFACE_MAP(CPrgEditorDoc, CStdEditorDoc)
	INTERFACE_PART(CPrgEditorDoc, IID_ICEPrgEdit, CEPrgEditObj)
	INTERFACE_PART(CPrgEditorDoc, IID_ICEDebug, CEDebugObj)
	INTERFACE_PART(CPrgEditorDoc, IID_ICEMonitor, CEMonitorObj)
    INTERFACE_PART(CPrgEditorDoc, IID_ICEWatchNotify, CEWatchNotifyObj)
END_INTERFACE_MAP()


const int CPrgEditorDoc::PreConnectHint     = 2000;
const int CPrgEditorDoc::PostConnectHint    = 2001;
const int CPrgEditorDoc::PreDisconnectHint  = 2002;
const int CPrgEditorDoc::PostDisconnectHint = 2003;

const int CPrgEditorDoc::ShowErrorHint = 2004;

const int CPrgEditorDoc::WatchpointDataHint   = 2005;

const int CPrgEditorDoc::ShowBreakpointHint = 2006;
const int CPrgEditorDoc::HideBreakpointHint = 2007;

const int CPrgEditorDoc::ShowWatchpointHint = 2008;
const int CPrgEditorDoc::HideWatchpointHint = 2009;

const int CPrgEditorDoc::ShowStoppointHint = 2010;
const int CPrgEditorDoc::HideStoppointHint = 2011;

const int CPrgEditorDoc::InsertFBCallHint = 2012;

const int CPrgEditorDoc::ExpressionChangedHint = 2013;

const int CPrgEditorDoc::BeginSFCUpdateHint = 2014;
const int CPrgEditorDoc::EndSFCUpdateHint   = 2015;

const int CPrgEditorDoc::StepAttributeChangedHint = 2016;
const int CPrgEditorDoc::ActionAttributeChangedHint = 2017;
const int CPrgEditorDoc::TransitionAttributeChangedHint = 2018;

const int CPrgEditorDoc::AllTransitionsOnToggledHint = 2021;

const int CPrgEditorDoc::AllActionsOffToggledHint = 2022;

const int CPrgEditorDoc::ManualModeToggledHint      = 2023;
const int CPrgEditorDoc::PerformingCycleToggledHint = 2024;

const int CPrgEditorDoc::InsertVariableHint  = 2025;
const int CPrgEditorDoc::IdleHint = 2026;
const int CPrgEditorDoc::ToolTipNfyHint = 2027;

const int CPrgEditorDoc::ViewActivatedHint = 2028;
const int CPrgEditorDoc::ViewDeactivatedHint = 2029;

CPrgEdSessions *CPrgEditorDoc::m_pSessions;


CPrgEdStep::CPrgEdStep (const CString &strName, long lNumber, long lX, long lY)
: m_strName (strName),
  m_lNumber (lNumber),
  m_lX (lX),
  m_lY (lY),
  m_pClientData (NULL),
  m_bActive (false)
{
}


CString CPrgEdStep::GetName ()
{
    return (m_strName);
}


long CPrgEdStep::GetNumber ()
{
    return (m_lNumber);
}


long CPrgEdStep::GetX ()
{
    return (m_lX);
}


long CPrgEdStep::GetY ()
{
    return (m_lY);
}


void *CPrgEdStep::GetClientData ()
{
    return (m_pClientData);
}


void CPrgEdStep::SetClientData (void *pClientData)
{
    m_pClientData = pClientData;
}


bool CPrgEdStep::IsActive ()
{
    return (m_bActive);
}


void CPrgEdStep::SetActive (bool bOn)
{
    m_bActive = bOn;
}


CPrgEdAction::CPrgEdAction (const CString &strName, long lNumber, long lX, long lY)
: m_strName (strName),
  m_lNumber (lNumber),
  m_lX (lX),
  m_lY (lY),
  m_pClientData (NULL),
  m_bActive (false),
  m_bForcedOnce (false),
  m_bForcedPermanently (false),
  m_bBlocked (false)
{
}


CString CPrgEdAction::GetName ()
{
    return (m_strName);
}


long CPrgEdAction::GetNumber ()
{
    return (m_lNumber);
}


long CPrgEdAction::GetX ()
{
    return (m_lX);
}


long CPrgEdAction::GetY ()
{
    return (m_lY);
}


void *CPrgEdAction::GetClientData ()
{
    return (m_pClientData);
}


void CPrgEdAction::SetClientData (void *pClientData)
{
    m_pClientData = pClientData;
}


bool CPrgEdAction::IsActive ()
{
    return (m_bActive);
}


void CPrgEdAction::SetActive (bool bOn)
{
    m_bActive = bOn;
}


bool CPrgEdAction::IsForced ()
{
    return (m_bForcedOnce || m_bForcedPermanently);
}


bool CPrgEdAction::IsForcedPermanently ()
{
    return (m_bForcedPermanently);
}


void CPrgEdAction::SetForcedPermanently (bool bOn)
{
    m_bForcedPermanently = bOn;
}


bool CPrgEdAction::IsForcedOnce ()
{
    return (m_bForcedOnce);
}


void CPrgEdAction::SetForcedOnce (bool bOn)
{
    m_bForcedOnce = bOn;
}


bool CPrgEdAction::IsBlocked ()
{
    return (m_bBlocked);
}


void CPrgEdAction::SetBlocked (bool bOn)
{
    m_bBlocked = bOn;
}


CPrgEdTransition::CPrgEdTransition (const CString &strName, long lNumber, long lX, long lY)
: m_strName (strName),
  m_lNumber (lNumber),
  m_lX (lX),
  m_lY (lY),
  m_pClientData (NULL),
  m_bEnabled (false),
  m_bForcedOnce (false),
  m_bForcedPermanently (false),
  m_bBlocked (false)
{
}


CString CPrgEdTransition::GetName ()
{
    return (m_strName);
}


long CPrgEdTransition::GetNumber ()
{
    return (m_lNumber);
}


long CPrgEdTransition::GetX ()
{
    return (m_lX);
}


long CPrgEdTransition::GetY ()
{
    return (m_lY);
}


void *CPrgEdTransition::GetClientData ()
{
    return (m_pClientData);
}


void CPrgEdTransition::SetClientData (void *pClientData)
{
    m_pClientData = pClientData;
}


bool CPrgEdTransition::IsEnabled ()
{
    return (m_bEnabled);
}


void CPrgEdTransition::SetEnabled (bool bOn)
{
    m_bEnabled = bOn;
}


bool CPrgEdTransition::IsForced ()
{
    return (m_bForcedOnce || m_bForcedPermanently);
}


bool CPrgEdTransition::IsForcedPermanently ()
{
    return (m_bForcedPermanently);
}


void CPrgEdTransition::SetForcedPermanently (bool bOn)
{
    m_bForcedPermanently = bOn;
}


bool CPrgEdTransition::IsForcedOnce ()
{
    return (m_bForcedOnce);
}


void CPrgEdTransition::SetForcedOnce (bool bOn)
{
    m_bForcedOnce = bOn;
}


bool CPrgEdTransition::IsBlocked ()
{
    return (m_bBlocked);
}


void CPrgEdTransition::SetBlocked (bool bOn)
{
    m_bBlocked = bOn;
}


CPrgEdSessions::CPrgEdSessions ()
: m_uiReferences (1),
  m_SessionManager (true)
{
}


CPrgEdSessions::~CPrgEdSessions ()
{
}


void CPrgEdSessions::Reference ()
{
    ++m_uiReferences;
}


void CPrgEdSessions::Unreference ()
{
    if (--m_uiReferences == 0)
    {
        delete this;
    };
}


CCESessionManager *CPrgEdSessions::GetSessionManager ()
{
    return (&m_SessionManager);
}



CPrgWPClientData::CPrgWPClientData(const CString &strVariableName, const CStdEditorPosition &startPosition, const CStdEditorPosition &endPosition)
: m_strVariableName (strVariableName),
  m_StartPosition (startPosition),
  m_EndPosition (endPosition),
  m_bActiveVisibility (false),
  m_bInactiveVisibility (false),
  m_lDisplayX (startPosition.GetX ()),
  m_lDisplayY (startPosition.GetY () + 1),
  m_lDisplayOffsetX (0),
  m_lDisplayOffsetY (0),
  m_uiZ (0),
  m_Area (0, 0, 0, 0),
  m_lLargestWidth (0),
  m_lLargestHeight (0),
  m_BackgroundColor (RGB (255, 255, 230)),
  m_bCrossedOut (false)
{
}


CString CPrgWPClientData::GetVariableName () const
{
	return (m_strVariableName);
}


CStdEditorPosition CPrgWPClientData::GetStartPosition () const
{
    return (m_StartPosition);
}


CStdEditorPosition CPrgWPClientData::GetEndPosition () const
{
    return (m_EndPosition);
}


CString CPrgWPClientData::GetText () const
{
	return (m_strText);
}


void CPrgWPClientData::SetText (const CString &strText)
{
	m_strText = strText;
}


CRect CPrgWPClientData::GetArea () const
{
	return (m_Area);
}


void CPrgWPClientData::SetArea (const CRect &area)
{
	m_Area = area;

    if (area.Width () > m_lLargestWidth)
        m_lLargestWidth = area.Width ();

    if (area.Height () > m_lLargestHeight)
        m_lLargestHeight = area.Height ();
}


CRect CPrgWPClientData::GetLargestArea () const
{
    CRect rect (m_Area);

    rect.right = rect.left + m_lLargestWidth;
    rect.bottom = rect.top + m_lLargestHeight;

    return (rect);
}


bool CPrgWPClientData::GetActiveVisibility () const
{
	return (m_bActiveVisibility);
}


void CPrgWPClientData::SetActiveVisibility (bool bVisible)
{
	m_bActiveVisibility = bVisible;
}


bool CPrgWPClientData::GetInactiveVisibility () const
{
	return (m_bInactiveVisibility);
}


void CPrgWPClientData::SetInactiveVisibility (bool bVisible)
{
	m_bInactiveVisibility = bVisible;
}


void CPrgWPClientData::GetDisplay (long &lDisplayX, long &lDisplayY, long &lDisplayOffsetX, long &lDisplayOffsetY) const
{
	lDisplayX = m_lDisplayX;
	lDisplayY = m_lDisplayY;
	lDisplayOffsetX = m_lDisplayOffsetX;
	lDisplayOffsetY = m_lDisplayOffsetY;
}


void CPrgWPClientData::SetDisplay (long lDisplayX, long lDisplayY, long lDisplayOffsetX, long lDisplayOffsetY)
{
	m_lDisplayX = lDisplayX;
	m_lDisplayY = lDisplayY;
	m_lDisplayOffsetX = lDisplayOffsetX;
	m_lDisplayOffsetY = lDisplayOffsetY;
}


unsigned int CPrgWPClientData::GetZ ()
{
    return (m_uiZ);
}


void CPrgWPClientData::SetZ (unsigned int uiZ)
{
    m_uiZ = uiZ;
}


COLORREF CPrgWPClientData::GetBackgroundColor ()
{
    return (m_BackgroundColor);
}


void CPrgWPClientData::SetBackgroundColor (COLORREF backgroundColor)
{
    m_BackgroundColor = backgroundColor;
}


bool CPrgWPClientData::GetCrossedOut ()
{
    return (m_bCrossedOut);
}


void CPrgWPClientData::SetCrossedOut (bool bOn)
{
    m_bCrossedOut = bOn;
}


/////////////////////////////////////////////////////////////////////////////
// Construction/destruction

    static CSubscriptionManager subscriptionManager;


CPrgEditorDoc::CPrgEditorDoc ()
: m_pSession (NULL),
  m_DebugSessionState (not_connected),
  m_bReferencedWatch (false),
  m_pSymbolTable (NULL),
  m_SessionModifiedCB (this, OnSessionModified),
  m_bStopped (false),
  m_bDelayClose (false),
  m_bWatchInitialized (false),
  m_dwWatchCookie (0),
  m_lNextWPHandle (1),
  m_lSFCVisHandle (0),
  m_SFCVisState (CEEmptyValue),
  m_lManualModeHandle (0),
  m_ManualModeState (CEEmptyValue),
  m_bManualModeValue (false),
  m_lDoNextHandle (0),
  m_DoNextState (CEEmptyValue),
  m_bDoNextValue (false),
  m_lOnceForcedActionsHandle (0),
  m_OnceForcedActionsState (CEEmptyValue),
  m_lForcedActionsHandle (0),
  m_ForcedActionsState (CEEmptyValue),
  m_lBlockedActionsHandle (0),
  m_BlockedActionsState (CEEmptyValue),
  m_lOnceForcedTransitionsHandle (0),
  m_OnceForcedTransitionsState (CEEmptyValue),
  m_lForcedTransitionsHandle (0),
  m_ForcedTransitionsState (CEEmptyValue),
  m_lBlockedTransitionsHandle (0),
  m_BlockedTransitionsState (CEEmptyValue),
  m_lAllTransitionsOnHandle (0),
  m_AllTransitionsOnState (CEEmptyValue),
  m_bAllTransitionsOnValue (false),
  m_lAllActionsOffHandle (0),
  m_AllActionsOffState (CEEmptyValue),
  m_bAllActionsOffValue (false),
  m_lManCmdHandle (0),
  m_ManCmdState (CEEmptyValue),
  m_bSFCEnabled (true),
  m_bInternalSFCEnabled (false),
  m_bEnableWPChanges (false),
  m_bSpecialWPDisplay (false),
//  m_bHadReadOnlyWarning (false),
  m_pLastActiveView(NULL)
{
    HRESULT hr;

    // HACK HACK HACK
    //
    // For some unknown reason stopping the last subscription manager
    // (which for program editors is normally done as soon as the watch
    // backend is finally released) takes a notable amount of time. As
    // a hack create and keep alive one subscription manager. I believe
    // that time is wasted inside CSC_Online during its final release.
    //
    // HACK HACK HACK

    if (!subscriptionManager.IsInitialized ())
    {
        subscriptionManager.Start ();
    };

	m_uiWatchExpressionCBFormat = RegisterClipboardFormat (CB_FORMAT_4CWATCHEXPR);

    if (m_pSessions == NULL)
    {
        m_pSessions = new CPrgEdSessions;
    }
    else
    {
        m_pSessions->Reference ();
    };

    hr = CoCreateInstance (CLSID_CEWatchBE, NULL, CLSCTX_ALL, IID_ICEWatch, (LPVOID *)&m_pWatch);
    if (SUCCEEDED (hr))
    {
        ASSERT (m_pWatch != NULL);
    };

    if (m_pWatch != NULL)
    {
        IUnknown *pUnknown;

        pUnknown = GetControllingUnknown ();
        ASSERT (pUnknown != NULL);
        if (pUnknown != NULL)
        {
            hr = AtlAdvise (m_pWatch, pUnknown, IID_ICEWatchNotify, &m_dwWatchCookie);
            ASSERT (SUCCEEDED (hr));
            pUnknown->Release ();   // avoid circular reference
        };
    };
}


CPrgEditorDoc::~CPrgEditorDoc ()
{
    if (m_pWatch != NULL)
    {
        if (m_bWatchInitialized)
        {
            m_pWatch->Cleanup ();
        };

        if (m_dwWatchCookie != 0)
        {
            IUnknown *pUnknown;

            pUnknown = GetControllingUnknown ();
            ASSERT (pUnknown != NULL);
            if (pUnknown != NULL)
            {
                pUnknown->AddRef ();   // see corresponding AtlAdvise
                AtlUnadvise (m_pWatch, IID_ICEWatchNotify, m_dwWatchCookie);
            };
        };
    };

    if (m_pSession != NULL)
    {
        m_pSession->UnregisterModifiedCB (&m_SessionModifiedCB);
        m_pSession->Unreference ();
        m_pSession = NULL;
    };

	delete m_pSymbolTable;

    if (m_pSessions != NULL)
    {
        m_pSessions->Unreference ();
    };

    // stop subscription memory finally, to avoid memory leaks (see DCS d-910)
    subscriptionManager.Stop();
}


void CPrgEditorDoc::DeleteContents () 
{
   ClearAllWatchpoints ();
   HideAllBreakpoints ();
   
   StopSFCTracing ();
   DestroyAllSteps ();
   DestroyAllActions ();
   DestroyAllTransitions ();
   StopLDTracing ();
   
   CStdEditorDoc::DeleteContents ();
}


BOOL CPrgEditorDoc::PrepareEdit()
{
    // if instance string is empty -> already in class tab -> return
    if(m_strInstance.IsEmpty())
    {
        if(CheckReadonly())
        {
            return FALSE;
        }
        return TRUE;
    }

	HRESULT hr;
	CComPtr<IDispatch> pGenEdDoc;
	CGenEdDocDriver driver;

	hr = GetGenEdDoc (pGenEdDoc);
	if (FAILED (hr))
	{
		return FALSE;
	}

	assert (pGenEdDoc != NULL);
	if (pGenEdDoc == NULL)
	{
		return FALSE;
	}

    bool bSavedDelayClose;

	driver.AttachDispatch (pGenEdDoc, FALSE);
	TRY
	{
        bSavedDelayClose = m_bDelayClose;
        m_bDelayClose = true;
		hr = driver.OnEdit();
        m_bDelayClose = bSavedDelayClose;
	}
	CATCH (COleException, e)
	{
		hr = COleException::Process(e);
        m_bDelayClose = bSavedDelayClose;
	}
	END_CATCH

    if(hr != S_OK)
    {
        return FALSE;
    }
    return TRUE;
}


void CPrgEditorDoc::EnableSFC (bool bOn)
{
    if (bOn)
    {
        if (!m_bSFCEnabled)
        {
            m_bSFCEnabled = true;

            if (m_pSession != NULL && m_pSession->GetState () == RW_established)
            {
                StartSFCTracing ();
            };
        };
    }
    else
    {
        if (m_bSFCEnabled)
        {
            StopSFCTracing ();
            DestroyAllSteps ();
            DestroyAllActions ();
            DestroyAllTransitions ();

            m_bSFCEnabled = false;
        };
    };
}


/////////////////////////////////////////////////////////////////////////////
// Convenience methods used by subclasses and views

bool CPrgEditorDoc::CanMonitor ()
{
	return (m_strInstance != _T (""));
}


bool CPrgEditorDoc::CanDebug ()
{
	return (m_strInstance != _T (""));
}


bool CPrgEditorDoc::IsConnected ()
{
    if (m_pSession == NULL)
    {
        return (false);
    };

    if (!m_pSession->IsConnected ())
    {
        return (false);
    };

    if (m_pSession->GetState () != RW_established)
    {
        return (false);
    };

    return (true);
}


bool CPrgEditorDoc::IsDebugConnected ()
{
    return (m_DebugSessionState == exclusive_established);
}





POSITION CPrgEditorDoc::GetFirstWPPosition ()
{
	return (m_WatchpointList.GetHeadPosition ());
}


CWatchpoint *CPrgEditorDoc::GetNextWP (POSITION &pPosition)
{
	return (m_WatchpointList.GetNext (pPosition));
}


CWatchpoint *CPrgEditorDoc::LookupWatchpoint (long lHandle)
{
	POSITION pPosition;

	pPosition = m_WatchpointList.GetHeadPosition ();
	while (pPosition != NULL)
	{
		CWatchpoint *pWP;

		pWP = m_WatchpointList.GetNext (pPosition);
		assert (pWP != NULL);
		if (pWP == NULL)
			continue;

		if (pWP->GetHandle () == lHandle)
		{
			return (pWP);
		};
	};

	return (NULL);
}


HRESULT CPrgEditorDoc::SetWatchpoint (const  CStdEditorPosition &position, 
                                      bool   bFailIfBad)
{
    long lHandle;

	return (AddWatchpoint (position, bFailIfBad, lHandle));
}


HRESULT CPrgEditorDoc::SetExpressionWatchpoint (const CStdEditorPosition &startPosition, 
                                                const CStdEditorPosition &endPosition, 
                                                bool  bFailIfBad)
{
    long lHandle;

	return (AddExpressionWatchpoint (startPosition, endPosition, bFailIfBad, lHandle));
}


HRESULT CPrgEditorDoc::ClearWatchpoint (CWatchpoint *pWP)
{
   return (RemoveWatchpoint (pWP->GetHandle ()));
}


void CPrgEditorDoc::ClearAllWatchpoints ()
{
	POSITION pPosition;

	pPosition = m_WatchpointList.GetHeadPosition ();
	while (pPosition != NULL)
	{
		CWatchpoint *pWP;

		pWP = m_WatchpointList.GetNext (pPosition);
		assert (pWP != NULL);
		if (pWP == NULL)
			continue;

      if (pWP->GetPosition().GetPosQual() == EDP_GRLDBOX)
         continue;

		RemoveWatchpoint (pWP->GetHandle ());
	};
}


HRESULT CPrgEditorDoc::SuspendWatchpoint (CWatchpoint *pWP)
{
	HRESULT hr;
    bool bSavedDelayClose;
    long lWatchItem;

	assert (pWP != NULL);
	if (pWP == NULL)
	{
		return (E_FAIL);
	};

	if (pWP->IsSuspended ())
	{
		return (S_FALSE);
	};

    bSavedDelayClose = m_bDelayClose;
    m_bDelayClose = true;

	hr = pWP->Suspend ();

    lWatchItem = pWP->GetWatchItem ();
    if (lWatchItem != 0)
    {
        m_pWatch->Disconnect (lWatchItem);
    };

    m_bDelayClose = bSavedDelayClose;

	return (hr);
}


HRESULT CPrgEditorDoc::ResumeWatchpoint (CWatchpoint *pWP)
{
	HRESULT hr;
    bool bSavedDelayClose;
    long lWatchItem;

	assert (pWP != NULL);
	if (pWP == NULL)
	{
		return (E_FAIL);
	};

	if (!pWP->IsSuspended ())
	{
		return (S_FALSE);
	};

    bSavedDelayClose = m_bDelayClose;
    m_bDelayClose = true;

    hr = pWP->Resume ();

    lWatchItem = pWP->GetWatchItem ();
    if (lWatchItem != 0)
    {
        m_pWatch->Connect (lWatchItem);
    };

    m_bDelayClose = bSavedDelayClose;

	return (hr);
}


POSITION CPrgEditorDoc::GetFirstBPPosition ()
{
	return (m_BreakpointList.GetHeadPosition ());
}


CBreakpoint *CPrgEditorDoc::GetNextBP (POSITION &pPosition)
{
	return (m_BreakpointList.GetNext (pPosition));
}


CBreakpoint *CPrgEditorDoc::LookupBreakpoint (const CStdEditorPosition& position)
{
	POSITION pPosition;

	pPosition = m_BreakpointList.GetHeadPosition ();
	while (pPosition != NULL)
	{
		CBreakpoint *pBP;

		pBP = m_BreakpointList.GetNext (pPosition);
		assert (pBP != NULL);
		if (pBP == NULL)
			continue;

		if (pBP->GetPosition () == position)
		{
			return (pBP);
		};
	};

	return (NULL);
}


HRESULT CPrgEditorDoc::SetBreakpoint (const CStdEditorPosition& position)
{
	HRESULT hr;
	CComPtr<IDispatch> pGenEdDoc;
	CGenEdDocDriver driver;

	hr = GetGenEdDoc (pGenEdDoc);
	if (FAILED (hr))
	{
		return (hr);
	};

	assert (pGenEdDoc != NULL);
	if (pGenEdDoc == NULL)
	{
		return (E_UNEXPECTED);
	};

    bool bSavedDelayClose;

	driver.AttachDispatch (pGenEdDoc, FALSE);

	TRY
	{
        bSavedDelayClose = m_bDelayClose;
        m_bDelayClose = true;
		hr = driver.SetBreakpoint (position.GetPosQual(), 
                                   position.GetX(),
		                           position.GetY(),
                                   position.GetCX(),
                                   position.GetCY()
		                          );
        m_bDelayClose = bSavedDelayClose;
	}
	CATCH (COleException, e)
	{
		hr = COleException::Process (e);
        m_bDelayClose = bSavedDelayClose;
	}
	END_CATCH

	return (hr);
}


HRESULT CPrgEditorDoc::ClearBreakpoint (const CStdEditorPosition& position)
{
	HRESULT hr;
	CComPtr<IDispatch> pGenEdDoc;
	CGenEdDocDriver driver;

	hr = GetGenEdDoc (pGenEdDoc);
	if (FAILED (hr))
	{
		return (hr);
	};

	assert (pGenEdDoc != NULL);
	if (pGenEdDoc == NULL)
	{
		return (E_UNEXPECTED);
	};

    bool bSavedDelayClose;

	driver.AttachDispatch (pGenEdDoc, FALSE);

	TRY
	{
        bSavedDelayClose = m_bDelayClose;
        m_bDelayClose = true;
		hr = driver.ClearBreakpoint (position.GetPosQual(),
                                     position.GetX(),
		                             position.GetY(),
                                     position.GetCX(),
                                     position.GetCY()
		                            );
        m_bDelayClose = bSavedDelayClose;
	}
	CATCH (COleException, e)
	{
		hr = COleException::Process (e);
        m_bDelayClose = bSavedDelayClose;
	}
	END_CATCH

	return (S_OK);
}


HRESULT CPrgEditorDoc::ClearInstanceBreakpoints ()
{
	HRESULT hr;
	CComPtr<IDispatch> pGenEdDoc;
	CGenEdDocDriver driver;

	hr = GetGenEdDoc (pGenEdDoc);
	if (FAILED (hr))
	{
		return (hr);
	};

	assert (pGenEdDoc != NULL);
	if (pGenEdDoc == NULL)
	{
		return (E_UNEXPECTED);
	};

    bool bSavedDelayClose;

	driver.AttachDispatch (pGenEdDoc, FALSE);

	TRY
	{
        bSavedDelayClose = m_bDelayClose;
        m_bDelayClose = true;
		hr = driver.ClearInstanceBreakpoints ();
        m_bDelayClose = bSavedDelayClose;
	}
	CATCH (COleException, e)
	{
		hr = COleException::Process (e);
        m_bDelayClose = bSavedDelayClose;
	}
	END_CATCH

	return (hr);
}


HRESULT CPrgEditorDoc::ClearAllBreakpoints ()
{
	HRESULT hr;
	CComPtr<IDispatch> pGenEdDoc;
	CGenEdDocDriver driver;

	hr = GetGenEdDoc (pGenEdDoc);
	if (FAILED (hr))
	{
		return (hr);
	};

	assert (pGenEdDoc != NULL);
	if (pGenEdDoc == NULL)
	{
		return (E_UNEXPECTED);
	};

    bool bSavedDelayClose;

	driver.AttachDispatch (pGenEdDoc, FALSE);

	TRY
	{
        bSavedDelayClose = m_bDelayClose;
        m_bDelayClose = true;
		hr = driver.ClearAllBreakpoints ();
        m_bDelayClose = bSavedDelayClose;
	}
	CATCH (COleException, e)
	{
		hr = COleException::Process (e);
        m_bDelayClose = bSavedDelayClose;
	}
	END_CATCH

	return (hr);
}


HRESULT CPrgEditorDoc::ActivateBreakpoint (const CStdEditorPosition& position)
{
	HRESULT hr;
	CComPtr<IDispatch> pGenEdDoc;
	CGenEdDocDriver driver;

	hr = GetGenEdDoc (pGenEdDoc);
	if (FAILED (hr))
	{
		return (hr);
	};

	assert (pGenEdDoc != NULL);
	if (pGenEdDoc == NULL)
	{
		return (E_UNEXPECTED);
	};

    bool bSavedDelayClose;

	driver.AttachDispatch (pGenEdDoc, FALSE);

	TRY
	{
        bSavedDelayClose = m_bDelayClose;
        m_bDelayClose = true;
		hr = driver.ActivateBreakpoint (position.GetPosQual(),
                                        position.GetX(),
		                                position.GetY(),
                                        position.GetCX(),
		                                position.GetCY()
		                               );
        m_bDelayClose = bSavedDelayClose;
	}
	CATCH (COleException, e)
	{
		hr = COleException::Process (e);
        m_bDelayClose = bSavedDelayClose;
	}
	END_CATCH

	return (hr);
}


HRESULT CPrgEditorDoc::DeactivateBreakpoint (const CStdEditorPosition& position)
{
	HRESULT hr;
	CComPtr<IDispatch> pGenEdDoc;
	CGenEdDocDriver driver;

	hr = GetGenEdDoc (pGenEdDoc);
	if (FAILED (hr))
	{
		return (hr);
	};

	assert (pGenEdDoc != NULL);
	if (pGenEdDoc == NULL)
	{
		return (E_UNEXPECTED);
	};

    bool bSavedDelayClose;

	driver.AttachDispatch (pGenEdDoc, FALSE);

	TRY
	{
        bSavedDelayClose = m_bDelayClose;
        m_bDelayClose = true;
		hr = driver.DeactivateBreakpoint (position.GetPosQual(),
                                          position.GetX(),
		                                  position.GetY(),
                                          position.GetCX(),
		                                  position.GetCY()
		                                  );
        m_bDelayClose = bSavedDelayClose;
	}
	CATCH (COleException, e)
	{
		hr = COleException::Process (e);
        m_bDelayClose = bSavedDelayClose;
	}
	END_CATCH

	return (hr);
}


void CPrgEditorDoc::HideAllBreakpoints ()
{
	POSITION pPosition;

	pPosition = m_BreakpointList.GetHeadPosition ();
	while (pPosition != NULL)
	{
		CBreakpoint *pBP;

		pBP = m_BreakpointList.GetNext (pPosition);
		assert (pBP != NULL);
		if (pBP == NULL)
			continue;

		HideBreakpoint (pBP->GetPosition ());
	};
}


bool CPrgEditorDoc::GetStopPosition (CStdEditorPosition& position)
{
	if (!m_bStopped)
	{
		return (false);
	};

	position = m_StopPosition;

	return (true);
}


HRESULT CPrgEditorDoc::ContinueTask ()
{
	HRESULT hr;
	CComPtr<IDispatch> pGenEdDoc;
	CGenEdDocDriver driver;

	hr = GetGenEdDoc (pGenEdDoc);
	if (FAILED (hr))
	{
		return (hr);
	};

	assert (pGenEdDoc != NULL);
	if (pGenEdDoc == NULL)
	{
		return (E_UNEXPECTED);
	};

    bool bSavedDelayClose;

	driver.AttachDispatch (pGenEdDoc, FALSE);

	TRY
	{
        bSavedDelayClose = m_bDelayClose;
        m_bDelayClose = true;
		hr = driver.ContinueTask ();
        m_bDelayClose = bSavedDelayClose;
	}
	CATCH (COleException, e)
	{
		hr = COleException::Process (e);
        m_bDelayClose = bSavedDelayClose;
	}
	END_CATCH

	return (hr);
}


HRESULT CPrgEditorDoc::StepTask (long mode)
{
	HRESULT hr;
	CComPtr<IDispatch> pGenEdDoc;
	CGenEdDocDriver driver;

	hr = GetGenEdDoc (pGenEdDoc);
	if (FAILED (hr))
	{
		return (hr);
	};

	assert (pGenEdDoc != NULL);
	if (pGenEdDoc == NULL)
	{
		return (E_UNEXPECTED);
	};

    bool bSavedDelayClose;

	driver.AttachDispatch (pGenEdDoc, FALSE);

	TRY
	{
        bSavedDelayClose = m_bDelayClose;
        m_bDelayClose = true;
		hr = driver.StepTask (mode);
        m_bDelayClose = bSavedDelayClose;
	}
	CATCH (COleException, e)
	{
		hr = COleException::Process (e);
        m_bDelayClose = bSavedDelayClose;
	}
	END_CATCH

	return (hr);
}



// CGenEdContDriver

HRESULT CPrgEditorDoc::ShowURL(const CString& strURL)
{
    HRESULT hr;
	CComPtr<IDispatch> pGenEdCont;
	CGenEdContDriver driver;

	hr = GetGenEdCont (pGenEdCont);
	if (FAILED (hr))
	{
		return (hr);
	};

	assert (pGenEdCont != NULL);
	if (pGenEdCont == NULL)
	{
		return (E_UNEXPECTED);
	};

	driver.AttachDispatch (pGenEdCont, FALSE);

	TRY
	{
		hr = driver.ShowURL (strURL);
	}
	CATCH (COleException, e)
	{
		hr = COleException::Process (e);
	}
	END_CATCH

	return (hr);}



HRESULT CPrgEditorDoc::ShowFile (const CString &strFile)
{
	HRESULT hr;
	CComPtr<IDispatch> pGenEdCont;
	CGenEdContDriver driver;

	hr = GetGenEdCont (pGenEdCont);
	if (FAILED (hr))
	{
		return (hr);
	};

	assert (pGenEdCont != NULL);
	if (pGenEdCont == NULL)
	{
		return (E_UNEXPECTED);
	};

	driver.AttachDispatch (pGenEdCont, FALSE);

	TRY
	{
		hr = driver.ShowFile (strFile);
	}
	CATCH (COleException, e)
	{
		hr = COleException::Process (e);
	}
	END_CATCH

	return (hr);
}


HRESULT CPrgEditorDoc::ShowUserHelp (const CString &strText)
{
    HRESULT hr;
	CComPtr<IDispatch> pGenEdCont;
	CGenEdContDriver driver;

	hr = GetGenEdCont (pGenEdCont);
	if (FAILED (hr))
	{
		return (hr);
	};

	assert (pGenEdCont != NULL);
	if (pGenEdCont == NULL)
	{
		return (E_UNEXPECTED);
	};

    bool bSavedDelayClose;

	driver.AttachDispatch (pGenEdCont, FALSE);

	TRY
	{
        bSavedDelayClose = m_bDelayClose;
        m_bDelayClose = true;
		hr = driver.ShowHelp((LPCTSTR)strText);
        m_bDelayClose = bSavedDelayClose;
	}
	CATCH (COleException, e)
	{
		hr = COleException::Process (e);
        m_bDelayClose = bSavedDelayClose;
	}
	END_CATCH

	return (hr);
}



HRESULT CPrgEditorDoc::ShowHeader(const CString &strPOU)
{
    HRESULT hr;
	CComPtr<IDispatch> pGenEdCont;
	CGenEdContDriver driver;

	hr = GetGenEdCont (pGenEdCont);
	if (FAILED (hr))
	{
		return (hr);
	};

	assert (pGenEdCont != NULL);
	if (pGenEdCont == NULL)
	{
		return (E_UNEXPECTED);
	};

    bool bSavedDelayClose;

	driver.AttachDispatch (pGenEdCont, FALSE);

	TRY
	{
        bSavedDelayClose = m_bDelayClose;
        m_bDelayClose = true;
		hr = driver.ShowHeader(strPOU);
        m_bDelayClose = bSavedDelayClose;
	}
	CATCH (COleException, e)
	{
		hr = COleException::Process (e);
        m_bDelayClose = bSavedDelayClose;
	}
	END_CATCH

	return (hr);
}


HRESULT CPrgEditorDoc::ShowHeaderFile(const CString &strFileName)
{
    HRESULT hr;
	CComPtr<IDispatch> pGenEdCont;
	CGenEdContDriver driver;

	hr = GetGenEdCont (pGenEdCont);
	if (FAILED (hr))
	{
		return (hr);
	};

	assert (pGenEdCont != NULL);
	if (pGenEdCont == NULL)
	{
		return (E_UNEXPECTED);
	};

    bool bSavedDelayClose;

	driver.AttachDispatch (pGenEdCont, FALSE);

	TRY
	{
        bSavedDelayClose = m_bDelayClose;
        m_bDelayClose = true;
		hr = driver.ShowHeaderFile(strFileName);
        m_bDelayClose = bSavedDelayClose;
	}
	CATCH (COleException, e)
	{
		hr = COleException::Process (e);
        m_bDelayClose = bSavedDelayClose;
	}
	END_CATCH

	return (hr);
}



HRESULT CPrgEditorDoc::SelectFBForCall ()
{
	HRESULT hr;
	CComPtr<IDispatch> pGenEdCont;
	CGenEdContDriver driver;

	hr = GetGenEdCont (pGenEdCont);
	if (FAILED (hr))
	{
		return (hr);
	};

	assert (pGenEdCont != NULL);
	if (pGenEdCont == NULL)
	{
		return (E_UNEXPECTED);
	};

    bool bSavedDelayClose;

	driver.AttachDispatch (pGenEdCont, FALSE);

	TRY
	{
        bSavedDelayClose = m_bDelayClose;
        m_bDelayClose = true;
		hr = driver.SelectFBForCall ();
        m_bDelayClose = bSavedDelayClose;
	}
	CATCH (COleException, e)
	{
		hr = COleException::Process (e);
        m_bDelayClose = bSavedDelayClose;
	}
	END_CATCH

	return (hr);
}

HRESULT CPrgEditorDoc::ShowVarSelection(const CString& strPouName)
{
    HRESULT hr;
	CComPtr<IDispatch> pGenEdCont;
	CGenEdContDriver driver;

	hr = GetGenEdCont (pGenEdCont);
	if (FAILED (hr))
	{
		return (hr);
	};

	assert (pGenEdCont != NULL);
	if (pGenEdCont == NULL)
	{
		return (E_UNEXPECTED);
	};

    bool bSavedDelayClose;

	driver.AttachDispatch (pGenEdCont, FALSE);

	TRY
	{
        bSavedDelayClose = m_bDelayClose;
        m_bDelayClose = true;
		hr = driver.ShowVarSelection ((LPCTSTR)strPouName);
        m_bDelayClose = bSavedDelayClose;
	}
	CATCH (COleException, e)
	{
		hr = COleException::Process (e);
        m_bDelayClose = bSavedDelayClose;
	}
	END_CATCH

	return (hr);
}


HRESULT CPrgEditorDoc::SetVarSelectionHint(const CString& strPouName, 
                                           const CString& strVarNamePrefix,
                                           const CString& strDataType)
{
    HRESULT hr;
	CComPtr<IDispatch> pGenEdCont;
	CGenEdContDriver driver;

	hr = GetGenEdCont (pGenEdCont);
	if (FAILED (hr))
	{
		return (hr);
	};

	assert (pGenEdCont != NULL);
	if (pGenEdCont == NULL)
	{
		return (E_UNEXPECTED);
	};

    bool bSavedDelayClose;

	driver.AttachDispatch (pGenEdCont, FALSE);

	TRY
	{
        bSavedDelayClose = m_bDelayClose;
        m_bDelayClose = true;
		hr = driver.SetVarSelectionHint ((LPCTSTR)strPouName, (LPCTSTR)strVarNamePrefix, (LPCTSTR)strDataType);
        m_bDelayClose = bSavedDelayClose;
	}
	CATCH (COleException, e)
	{
		hr = COleException::Process (e);
        m_bDelayClose = bSavedDelayClose;
	}
	END_CATCH

	return (hr);
}


HRESULT CPrgEditorDoc::GetLRUPous(CList<CString, CString&>& listPous)
{
    HRESULT hr;
	CComPtr<IDispatch> pGenEdCont;
	CGenEdContDriver driver;

	hr = GetGenEdCont (pGenEdCont);
	if (FAILED (hr))
	{
		return (hr);
	};

	assert (pGenEdCont != NULL);
	if (pGenEdCont == NULL)
	{
		return (E_UNEXPECTED);
	};

    bool bSavedDelayClose;

	driver.AttachDispatch (pGenEdCont, FALSE);

	TRY
	{
        listPous.RemoveAll();

        bSavedDelayClose = m_bDelayClose;
        m_bDelayClose = true;
		VARIANT vPous;
        ::VariantInit(&vPous);
        hr = driver.GetLRUPous(&vPous);
        if (hr==S_OK)
        {
            ASSERT(vPous.vt == (VT_ARRAY|VT_BSTR));
            if (vPous.vt == (VT_ARRAY|VT_BSTR))
            {
                BSTR HUGEP *pbstr;
                BSTR bstr;
                unsigned long i;
                hr = ::SafeArrayAccessData(vPous.parray, (void HUGEP**)&pbstr);
                if (hr==S_OK)
                {
                    for (i = 0; i < ((vPous.parray)->rgsabound[0]).cElements; i++)
                    {
                        CString str;
                        bstr = pbstr[i];
                        str = bstr;
                        listPous.AddTail(str);
                    }

                    ::SafeArrayUnaccessData(vPous.parray);
                }
            }
            else
            {
                hr = E_FAIL;
            }
            ::VariantClear(&vPous);
        }

        m_bDelayClose = bSavedDelayClose;
	}
	CATCH (COleException, e)
	{
		hr = COleException::Process (e);
        m_bDelayClose = bSavedDelayClose;
	}
	END_CATCH

	return (hr);
}

HRESULT CPrgEditorDoc::GetPouXML(const CString& strPouName, CString& strPouDef)
{
    HRESULT hr;
	CComPtr<IDispatch> pGenEdCont;
	CGenEdContDriver driver;

	hr = GetGenEdCont (pGenEdCont);
	if (FAILED (hr))
	{
		return (hr);
	};

	assert (pGenEdCont != NULL);
	if (pGenEdCont == NULL)
	{
		return (E_UNEXPECTED);
	};

    bool bSavedDelayClose;

	driver.AttachDispatch (pGenEdCont, FALSE);

	TRY
	{
        bSavedDelayClose = m_bDelayClose;
        m_bDelayClose = true;
        CComBSTR sPouDef;
		hr = driver.GetPouXML(strPouName, &sPouDef);
        if (hr==S_OK)
        {
            strPouDef = sPouDef;
        }
        m_bDelayClose = bSavedDelayClose;
	}
	CATCH (COleException, e)
	{
		hr = COleException::Process (e);
        m_bDelayClose = bSavedDelayClose;
	}
	END_CATCH

	return (hr);
}

HRESULT CPrgEditorDoc::GetVarList(CList<CString, CString&>& listVars)
{
    HRESULT hr;
	CComPtr<IDispatch> pGenEdCont;
	CGenEdContDriver driver;

	hr = GetGenEdCont (pGenEdCont);
	if (FAILED (hr))
	{
		return (hr);
	};

	assert (pGenEdCont != NULL);
	if (pGenEdCont == NULL)
	{
		return (E_UNEXPECTED);
	};

    bool bSavedDelayClose;

	driver.AttachDispatch (pGenEdCont, FALSE);

	TRY
	{
        listVars.RemoveAll();

        bSavedDelayClose = m_bDelayClose;
        m_bDelayClose = true;
		VARIANT vVars;
        ::VariantInit(&vVars);
        hr = driver.GetVarList(&vVars);
        if (hr==S_OK)
        {
            ASSERT(vVars.vt == VT_ARRAY);
            if (vVars.vt == VT_ARRAY)
            {
                BSTR HUGEP *pbstr;
                BSTR bstr;
                unsigned long i;
                hr = ::SafeArrayAccessData(vVars.parray, (void HUGEP**)&pbstr);
                if (hr==S_OK)
                {
                    for (i = 0; i < ((vVars.parray)->rgsabound[0]).cElements; i++)
                    {
                        CString str;
                        bstr = pbstr[i];
                        str = bstr;
                        listVars.AddTail(str);
                    }

                    ::SafeArrayUnaccessData(vVars.parray);
                }
            }
            else
            {
                hr = E_FAIL;
            }
            ::VariantClear(&vVars);
        }
        m_bDelayClose = bSavedDelayClose;
	}
	CATCH (COleException, e)
	{
		hr = COleException::Process (e);
        m_bDelayClose = bSavedDelayClose;
	}
	END_CATCH

	return (hr);
}

HRESULT CPrgEditorDoc::GetPouList(CList<CString, CString&>& listPous)
{
    HRESULT hr;
	CComPtr<IDispatch> pGenEdCont;
	CGenEdContDriver driver;

	hr = GetGenEdCont (pGenEdCont);
	if (FAILED (hr))
	{
		return (hr);
	};

	assert (pGenEdCont != NULL);
	if (pGenEdCont == NULL)
	{
		return (E_UNEXPECTED);
	};

    bool bSavedDelayClose;

	driver.AttachDispatch (pGenEdCont, FALSE);

	TRY
	{
        listPous.RemoveAll();

        bSavedDelayClose = m_bDelayClose;
        m_bDelayClose = true;
		VARIANT vPous;
        ::VariantInit(&vPous);
        hr = driver.GetPouList(&vPous);
        if (hr==S_OK)
        {
            ASSERT(vPous.vt == VT_ARRAY);
            if (vPous.vt == VT_ARRAY)
            {
                BSTR HUGEP *pbstr;
                BSTR bstr;
                unsigned long i;
                hr = ::SafeArrayAccessData(vPous.parray, (void HUGEP**)&pbstr);
                if (hr==S_OK)
                {
                    for (i = 0; i < ((vPous.parray)->rgsabound[0]).cElements; i++)
                    {
                        CString str;
                        bstr = pbstr[i];
                        str = bstr;
                        listPous.AddTail(str);
                    }

                    ::SafeArrayUnaccessData(vPous.parray);
                }
            }
            else
            {
                hr = E_FAIL;
            }
            ::VariantClear(&vPous);
        }
        m_bDelayClose = bSavedDelayClose;	
    }
	CATCH (COleException, e)
	{
		hr = COleException::Process (e);
        m_bDelayClose = bSavedDelayClose;
	}
	END_CATCH

	return (hr);
}

HRESULT CPrgEditorDoc::GetPouDefinition(const CString& strPouName, CString& strPouDef)
{
    HRESULT hr;
	CComPtr<IDispatch> pGenEdCont;
	CGenEdContDriver driver;

	hr = GetGenEdCont (pGenEdCont);
	if (FAILED (hr))
	{
		return (hr);
	};

	assert (pGenEdCont != NULL);
	if (pGenEdCont == NULL)
	{
		return (E_UNEXPECTED);
	};

    bool bSavedDelayClose;

	driver.AttachDispatch (pGenEdCont, FALSE);

	TRY
	{
        bSavedDelayClose = m_bDelayClose;
        m_bDelayClose = true;
        CComBSTR sPouDef;
		hr = driver.GetPouDefinition(strPouName, &sPouDef);
        if (hr==S_OK)
        {
            strPouDef = sPouDef;
        }
        m_bDelayClose = bSavedDelayClose;
	}
	CATCH (COleException, e)
	{
		hr = COleException::Process (e);
        m_bDelayClose = bSavedDelayClose;
	}
	END_CATCH

	return (hr);
}

HRESULT CPrgEditorDoc::GetPouMember(const CString& strPouName, CList<CString, CString&>& listMembers)
{
    HRESULT hr;
	CComPtr<IDispatch> pGenEdCont;
	CGenEdContDriver driver;

	hr = GetGenEdCont (pGenEdCont);
	if (FAILED (hr))
	{
		return (hr);
	};

	assert (pGenEdCont != NULL);
	if (pGenEdCont == NULL)
	{
		return (E_UNEXPECTED);
	};

    bool bSavedDelayClose;

	driver.AttachDispatch (pGenEdCont, FALSE);

	TRY
	{
        listMembers.RemoveAll();

        bSavedDelayClose = m_bDelayClose;
        m_bDelayClose = true;
		VARIANT vMembers;
        ::VariantInit(&vMembers);
        hr = driver.GetPouMember(strPouName, &vMembers);
        if (hr==S_OK)
        {
            ASSERT(vMembers.vt == VT_ARRAY);
            if (vMembers.vt == VT_ARRAY)
            {
                BSTR HUGEP *pbstr;
                BSTR bstr;
                unsigned long i;
                hr = ::SafeArrayAccessData(vMembers.parray, (void HUGEP**)&pbstr);
                if (hr==S_OK)
                {
                    for (i = 0; i < ((vMembers.parray)->rgsabound[0]).cElements; i++)
                    {
                        CString str;
                        bstr = pbstr[i];
                        str = bstr;
                        listMembers.AddTail(str);
                    }

                    ::SafeArrayUnaccessData(vMembers.parray);
                }
            }
            else
            {
                hr = E_FAIL;
            }
            ::VariantClear(&vMembers);
        }
        m_bDelayClose = bSavedDelayClose;	
	}
	CATCH (COleException, e)
	{
		hr = COleException::Process (e);
        m_bDelayClose = bSavedDelayClose;
	}
	END_CATCH

	return (hr);
}


HRESULT CPrgEditorDoc::XRefChangeRefPosition(BOOL bForward)
{
    HRESULT hr;
	CComPtr<IDispatch> pGenEdCont;
	CGenEdContDriver driver;

	hr = GetGenEdCont (pGenEdCont);
	if (FAILED (hr))
	{
		return (hr);
	};

	assert (pGenEdCont != NULL);
	if (pGenEdCont == NULL)
	{
		return (E_UNEXPECTED);
	};

    bool bSavedDelayClose;

	driver.AttachDispatch (pGenEdCont, FALSE);

	TRY
	{
        bSavedDelayClose = m_bDelayClose;
        m_bDelayClose = true;
		hr = driver.XRefChangeRefPosition (bForward);
        m_bDelayClose = bSavedDelayClose;
	}
	CATCH (COleException, e)
	{
		hr = COleException::Process (e);
        m_bDelayClose = bSavedDelayClose;
	}
	END_CATCH

	return (hr);
}


HRESULT CPrgEditorDoc::XRefSetActiveQuery(const CString& strVariableName)
{
    HRESULT hr;
	CComPtr<IDispatch> pGenEdCont;
	CGenEdContDriver driver;

	hr = GetGenEdCont (pGenEdCont);
	if (FAILED (hr))
	{
		return (hr);
	};

	assert (pGenEdCont != NULL);
	if (pGenEdCont == NULL)
	{
		return (E_UNEXPECTED);
	};

    bool bSavedDelayClose;

	driver.AttachDispatch (pGenEdCont, FALSE);

	TRY
	{
        bSavedDelayClose = m_bDelayClose;
        m_bDelayClose = true;
		hr = driver.XRefSetActiveQuery ((LPCTSTR)strVariableName);
        m_bDelayClose = bSavedDelayClose;
	}
	CATCH (COleException, e)
	{
		hr = COleException::Process (e);
        m_bDelayClose = bSavedDelayClose;
	}
	END_CATCH

	return (hr);
}


HRESULT CPrgEditorDoc::ShowVarSelectionBox(LONG lXPos, LONG lYPos, 
                                           const CString& strLine, LONG lColumn,
                                           const CString& strTypeHint,
                                           const CString& strPouName,
                                           CString& strSelection,
                                           LONG& lStartColumn,
                                           LONG& lEndColumn)
{ 
    HRESULT hr;

    CComPtr<IDispatch> pGenEdCont;
	CGenEdContDriver driver;

	hr = GetGenEdCont (pGenEdCont);
	if (FAILED (hr))
	{
		return (hr);
	};

	assert (pGenEdCont != NULL);
	if (pGenEdCont == NULL)
	{
		return (E_UNEXPECTED);
	};

    bool bSavedDelayClose;

	driver.AttachDispatch (pGenEdCont, FALSE);

	TRY
	{
        bSavedDelayClose = m_bDelayClose;
        m_bDelayClose = true;
        CComBSTR sSelection;
		hr = driver.ShowVarSelectionBox(lXPos, lYPos, 
                                        (LPCTSTR)strLine, lColumn,
                                        (LPCTSTR)strTypeHint,
                                        (LPCTSTR)strPouName,
                                        &sSelection,
                                        &lStartColumn,
                                        &lEndColumn);
        if (hr==S_OK)
        {
            strSelection = sSelection;
        }
        m_bDelayClose = bSavedDelayClose;
	}
	CATCH (COleException, e)
	{
		hr = COleException::Process (e);
        m_bDelayClose = bSavedDelayClose;
	}
	END_CATCH

    return hr;

}


HRESULT CPrgEditorDoc::GetProjectManager(IUnknown** pProjManUnk)
{
    HRESULT hr;

    CComPtr<IDispatch> pGenEdCont;
	CGenEdContDriver driver;

	hr = GetGenEdCont (pGenEdCont);
	if (FAILED (hr))
	{
		return (hr);
	};

	assert (pGenEdCont != NULL);
	if (pGenEdCont == NULL)
	{
		return (E_UNEXPECTED);
	};

    bool bSavedDelayClose;

	driver.AttachDispatch (pGenEdCont, FALSE);

	TRY
	{
        bSavedDelayClose = m_bDelayClose;
        m_bDelayClose = true;
        CComBSTR sSelection;
		hr = driver.GetProjectManager(pProjManUnk);
        m_bDelayClose = bSavedDelayClose;
	}
	CATCH (COleException, e)
	{
		hr = COleException::Process (e);
        m_bDelayClose = bSavedDelayClose;
	}
	END_CATCH

    return hr;
}




// end CGenEdContDriver



CString CPrgEditorDoc::GetConfiguration ()
{
	CString strPath;
	CString strConfiguration;

	strPath = m_strInstance;

	strConfiguration = StripInstanceQualifier (strPath);
	assert (strConfiguration != _T (""));
	if (strConfiguration == _T (""))
	{
		return ("");
	};

	return (strConfiguration);
}


CString CPrgEditorDoc::GetWithoutConfiguration ()
{
	CString strPath;
	CString strConfiguration;

	strPath = m_strInstance;

	strConfiguration = StripInstanceQualifier (strPath);
	assert (strConfiguration != _T (""));
	if (strConfiguration == _T (""))
	{
		return ("");
	};

	return (strPath);
}


CString CPrgEditorDoc::GetResource ()
{
	CString strPath;
	CString strConfiguration;
	CString strResource;

	strPath = m_strInstance;

	strConfiguration = StripInstanceQualifier (strPath);
	assert (strConfiguration != _T (""));
	if (strConfiguration == _T (""))
	{
		return ("");
	};

	strResource = StripInstanceQualifier (strPath);
	assert (strResource != _T (""));
	if (strResource == _T (""))
	{
		return ("");
	};

	return (strResource);
}


CString CPrgEditorDoc::StripInstanceQualifier (CString &strPath)
{
	int iPos;
	CString strResult;

	iPos = strPath.Find (_T ('.'));
	if (iPos < 0)
	{
		return (strPath);
	};

	strResult = strPath.Left (iPos);
	strPath = strPath.Mid (iPos + 1);
	return (strResult);
}


void CPrgEditorDoc::CacheWatchExpression (const CString &strExpression, COleDataSource *pDataSource)
{
    CWatchExpressionDragItem dragItem (strExpression, m_strInstance);
    CSharedFile file;
    CArchive ar (&file, CArchive::store);

    if (m_strInstance.IsEmpty ())
    {
        return;
    };

    dragItem.Serialize (ar);

    ar.Close ();

    pDataSource->CacheGlobalData (m_uiWatchExpressionCBFormat, file.Detach ());
}



/////////////////////////////////////////////////////////////////////////////
// Watch backend handling methods

HRESULT CPrgEditorDoc::AddWatchItem (const CString &strItem, LONG &lHandle)
{
    HRESULT hr;

    ASSERT (m_pWatch != NULL);
    if (m_pWatch == NULL)
    {
        return (E_UNEXPECTED);
    };

    hr = m_pWatch->AddItemWithScope (CComBSTR (strItem), CComBSTR (m_strInstance), &lHandle);
    if (FAILED (hr))
    {
        return (hr);
    };

    return (S_OK);
}


HRESULT CPrgEditorDoc::RemoveWatchItem (LONG lHandle)
{
//    ASSERT (m_pWatch != NULL);
    if (m_pWatch == NULL)
    {
        return (E_UNEXPECTED);
    };

    m_pWatch->RemoveItem (lHandle);

    return (S_OK);
}


HRESULT CPrgEditorDoc::ConnectWatchItem (LONG lHandle)
{
    HRESULT hr;

    ASSERT (m_pWatch != NULL);
    if (m_pWatch == NULL)
    {
        return (E_UNEXPECTED);
    };

    hr = m_pWatch->Connect (lHandle);
    if (FAILED (hr))
    {
        return (hr);
    };

    return (S_OK);
}


HRESULT CPrgEditorDoc::DisconnectWatchItem (LONG lHandle)
{
    HRESULT hr;

//    ASSERT (m_pWatch != NULL);
    if (m_pWatch == NULL)
    {
        return (E_UNEXPECTED);
    };

    hr = m_pWatch->Disconnect (lHandle);
    if (FAILED (hr))
    {
        return (hr);
    };

    return (S_OK);
}


HRESULT CPrgEditorDoc::WriteWatchItem (LONG lHandle, const CComVariant &varValue)
{
    HRESULT hr;

    ASSERT (m_pWatch != NULL);
    if (m_pWatch == NULL)
    {
        return (E_UNEXPECTED);
    };

    hr = m_pWatch->Write (lHandle, varValue);
    if (FAILED (hr))
    {
        return (hr);
    };

    return (S_OK);
}


bool CPrgEditorDoc::IsUnknownWatchItemType (LONG lHandle)
{
    HRESULT hr;
    CComPtr<IUnknown> pUnknown;

    ASSERT (m_pWatch != NULL);
    if (m_pWatch == NULL)
    {
        return (true);
    };

    hr = m_pWatch->GetType (lHandle, (IUnknown **)&pUnknown);
    if (hr != S_OK)
    {
        return (true);
    };

    return (false);
}


bool CPrgEditorDoc::IsSimpleWatchItemType (LONG lHandle, BL_IEC_TYP &iecType)
{
    HRESULT hr;
    CComPtr<IUnknown> pUnknown;
    CComPtr<ITypeDBI> pType;
    DBITypeKind kind;
    short sIECType;

    ASSERT (m_pWatch != NULL);
    if (m_pWatch == NULL)
    {
        return (false);
    };

    hr = m_pWatch->GetType (lHandle, (IUnknown **)&pUnknown);
    if (hr != S_OK)
    {
        return (false);
    };

    hr = pUnknown->QueryInterface (IID_ITypeDBI, (LPVOID *)&pType);
    if (FAILED (hr))
    {
        return (false);
    };
    ASSERT (pType != NULL);
    if (pType == NULL)
    {
        return (false);
    };

    hr = pType->get_Kind (&kind);
    if (hr != S_OK)
    {
        return (false);
    };

    if (kind != DBISimpleType)
    {
        return (false);
    };

    hr = pType->get_IECType (&sIECType);
    if (hr != S_OK)
    {
        return (false);
    };

    iecType = (BL_IEC_TYP)sIECType;
    return (true);
}


CEExpressionState CPrgEditorDoc::GetWatchExpressionState (LONG lHandle)
{
    HRESULT hr;
    CEExpressionState state;

    ASSERT (m_pWatch != NULL);
    if (m_pWatch == NULL)
    {
        return (CEEmptyValue);
    };

    hr = m_pWatch->GetExpressionState (lHandle, &state);
    if (FAILED (hr))
    {
        return (CEEmptyValue);
    };

    return (state);
}


CString CPrgEditorDoc::GetWatchExpressionValue (LONG lHandle)
{
    HRESULT hr;
    CComBSTR sValue;
    short sQuality;
    DATE timestamp;

    ASSERT (m_pWatch != NULL);
    if (m_pWatch == NULL)
    {
        return ("");
    };

	hr = m_pWatch->GetDataText (lHandle, TRUE, &sValue, &sQuality, &timestamp);
    if (FAILED (hr))
    {
        return ("");
    };

    return (sValue);
}


CString CPrgEditorDoc::GetWatchExpressionValue (LONG lHandle, short &sQuality, DATE &timestamp)
{
    HRESULT hr;
    CComBSTR sValue;

    ASSERT (m_pWatch != NULL);
    if (m_pWatch == NULL)
    {
        return ("");
    };

	hr = m_pWatch->GetDataText (lHandle, TRUE, &sValue, &sQuality, &timestamp);
    if (FAILED (hr))
    {
        return ("");
    };

    return (sValue);
}


/////////////////////////////////////////////////////////////////////////////
// ICEPrgEdit functionality

void CPrgEditorDoc::SetInstance (const CString &strInstanceName, const CString &strMachineName, const CString &strMachineAddress)
{
    // SIS: workaround for problems with tool tips
    RecreateToolTip();

	ClearAllWatchpoints ();
	HideAllBreakpoints ();
	HideStoppoint ();

    StopSFCTracing ();
    DestroyAllSteps ();
    DestroyAllActions ();
    DestroyAllTransitions ();
    StopLDTracing ();

    m_pInstanceDBI = NULL;

    if (!m_strMachineName.IsEmpty ())
    {
        if (m_pSession != NULL)
        {
            m_pSession->UnregisterModifiedCB (&m_SessionModifiedCB);
            m_pSession->Unreference ();
            m_pSession = NULL;
        };

        m_DebugSessionState = not_connected;
    };

    m_bInternalSFCEnabled = false;
	m_strInstance = strInstanceName;
	m_strMachineName = strMachineName;
    m_strMachineAddress = strMachineAddress;

	if (!m_strInstance.IsEmpty ())
	{
        GetInstanceDBI ();
    };

    OnInstancePathModified ();

    if (!m_strInstance.IsEmpty ())
    {
        ReadSFCDBI ();
    };

    if (!m_strMachineName.IsEmpty ())
    {
        HRESULT hr;
        CCESessionManager *pSessionManager;

        pSessionManager = GetSessionManager ();
        if (pSessionManager != NULL)
        {
            hr = pSessionManager->GetSession (strMachineAddress, m_pSession);
            if (SUCCEEDED (hr))
            {
                ASSERT (m_pSession != NULL);
                if (m_pSession != NULL)
                {
    	            m_pSession->RegisterModifiedCB (&m_SessionModifiedCB);

                    if (!m_pSession->IsOpen ())
                    {
                        m_pSession->Open ();
                    };

                    if (m_pSession->GetState () == RW_established)
                    {
                       if (m_bSFCEnabled)
                       {
                           StartSFCTracing ();
                       }
                       StartLDTracing ();
                    }
                };
            };
        };
	};

	if (m_pSymbolTable != NULL)
	{
		return;
	};

	ReadSymbols ();
}


//------------------------------------------------------------------*
/**
 * recreate tool tip.
 *
 *  base class does nothing.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CPrgEditorDoc::RecreateToolTip()
{
}


void CPrgEditorDoc::SetSymbolFile (const CString &strSymbolFile)
{
	m_strSymbolFile = strSymbolFile;

    if (m_pWatch != NULL)
    {
        if (m_bWatchInitialized)
        {
            m_pDebugInfo = NULL;

            m_pWatch->Cleanup ();
            m_bWatchInitialized = false;
        };

        if (!m_strSymbolFile.IsEmpty ())
        {
            int iPos;

            iPos = strSymbolFile.ReverseFind (_T ('\\'));
            if (iPos != -1)
            {
                HRESULT hr;
                CString strDebugPath;

                strDebugPath = strSymbolFile.Left (iPos);

                hr = m_pWatch->Init (CComBSTR (strDebugPath));
                if (SUCCEEDED (hr))
                {
                    CComPtr<IUnknown> pDBIUnknown;

                    m_bWatchInitialized = true;

                    hr = m_pWatch->GetDebugInfo (&pDBIUnknown);
                    ASSERT (hr == S_OK);
                    if (hr == S_OK)
                    {
                        hr = pDBIUnknown->QueryInterface (IID_IDebugInfo, (LPVOID *)&m_pDebugInfo);
                        ASSERT (SUCCEEDED (hr));
                        if (SUCCEEDED (hr))
                        {
                            ASSERT (m_pDebugInfo != NULL);
                        };
                    };
                };
            };
        };
    };

	ReadSymbols ();
}


HRESULT CPrgEditorDoc::NotifyConnect (const CString &strMachineName)
{
    HRESULT hr;
    CCESessionManager *pSessionManager;
    CCESession *pSession;

    pSessionManager = GetSessionManager ();
    if (pSessionManager == NULL)
    {
        return (E_FAIL);
    };

    pSession = pSessionManager->FindSession (strMachineName);
    if (pSession == NULL)
    {
        return (S_OK);
    };

    if (pSession->IsConnected ())
    {
        pSession->Unreference ();
        return (S_OK);
    };

    if (pSession == m_pSession)
    {
        UpdateAllViews (NULL, PreConnectHint, NULL);
    };

    hr = pSession->Connect ();
    if (FAILED (hr))
    {
        pSession->Unreference ();
        return (hr);
    };

    if (pSession == m_pSession)
    {
        if (pSession->GetState () == RW_established)
        {
            if (m_bSFCEnabled)
            {
                StartSFCTracing ();
            };
            StartLDTracing ();
        };

        UpdateAllViews (NULL, PostConnectHint, NULL);
    };

    pSession->Unreference ();
    return (S_OK);
}


HRESULT CPrgEditorDoc::NotifyDisconnect (const CString &strMachineName)
{
    HRESULT hr;
    CCESessionManager *pSessionManager;
    CCESession *pSession;

    pSessionManager = GetSessionManager ();
    if (pSessionManager == NULL)
    {
        return (E_FAIL);
    };

    pSession = pSessionManager->FindSession (strMachineName);
    if (pSession == NULL)
    {
        return (S_OK);
    };

    if (!pSession->IsConnected ())
    {
        pSession->Unreference ();
        return (S_OK);
    };

    if (pSession == m_pSession)
    {
        UpdateAllViews (NULL, PreDisconnectHint, NULL);
    };

    hr = pSession->Disconnect ();
    if (FAILED (hr))
    {
        pSession->Unreference ();
        return (hr);
    };

    if (pSession == m_pSession)
    {
        StopSFCTracing ();
        StopLDTracing ();

        UpdateAllViews (NULL, PostDisconnectHint, NULL);
    };

    pSession->Unreference ();
    return (S_OK);
}


void CPrgEditorDoc::ShowError (CStdEditorPosition position)
{
    UpdateAllViews (NULL, ShowErrorHint, &position);
}


HRESULT CPrgEditorDoc::InsertFBCall (const CString &strFB)
{
    CString strTemp=strFB;

    UpdateAllViews (NULL, InsertFBCallHint, (CObject *)&strTemp);

    return (S_OK);
}

HRESULT CPrgEditorDoc::InsertVariable (const CString &strVarName)
{
    CString strTemp=strVarName;

    UpdateAllViews (NULL, InsertVariableHint, (CObject *)&strTemp);

    return (S_OK);
}


HRESULT CPrgEditorDoc::NotifyDebugState (CSC_SESSION_STATE state)
{
    m_DebugSessionState = state;
    return (S_OK);
}


HRESULT CPrgEditorDoc::Resubscribe ()
{
    HRESULT hr;
    CComPtr<ICEWatchAdmin> pWatchAdmin;

    hr = CoCreateInstance (CLSID_CEWatchAdminBE, NULL, CLSCTX_ALL, IID_ICEWatchAdmin, (LPVOID *)&pWatchAdmin);
    if (FAILED (hr))
    {
        return (hr);
    };

    return (pWatchAdmin->Resubscribe ());
}



/////////////////////////////////////////////////////////////////////////////
// ICEMonitor functionality

HRESULT CPrgEditorDoc::AddWatchpoint (const CStdEditorPosition &position, bool bFailIfBad, long &lHandle)
{
   CWatchpoint *pWP;
   HRESULT hr;
   
   lHandle = m_lNextWPHandle++;
   
   pWP = new CWatchpoint (position, lHandle);
   
   hr = FinishAddWatchpoint (pWP, bFailIfBad);
   if (hr != S_OK)
   {
      delete pWP;
      return (hr);
   };
   
   return (S_OK);
}


HRESULT CPrgEditorDoc::AddExpressionWatchpoint (const CStdEditorPosition &startPosition, const CStdEditorPosition &endPosition, bool bFailIfBad, long &lHandle)
{
	CWatchpoint *pWP;
	HRESULT hr;

    lHandle = m_lNextWPHandle++;

	pWP = new CWatchpoint (startPosition, endPosition, lHandle);

    hr = FinishAddWatchpoint (pWP, bFailIfBad);
	if (hr != S_OK)
	{
		delete pWP;
		return (hr);
	};

	return (S_OK);
}


HRESULT CPrgEditorDoc::FinishAddWatchpoint (CWatchpoint *pWP, bool bFailIfBad)
{
    HRESULT hr;

    hr = OnCreateWatchpoint (pWP);
    if (hr != S_OK)
    {
        return (hr);
    };

    if (m_bEnableWPChanges)
    {
        hr = OnConnectWatchpoint (pWP, bFailIfBad);
    }
    else
    {
        hr = OnConnectWatchpoint (pWP);
    };
    if (hr != S_OK)
    {
        OnDestroyWatchpoint (pWP);
        return (hr);
    };

    m_WatchpointList.AddTail (pWP);

    OnShowWatchpoint (pWP);

    return (S_OK);
}


HRESULT CPrgEditorDoc::RemoveWatchpoint (long lHandle)
{
	CWatchpoint *pWP;
	POSITION pPosition;

	if ((pWP = LookupWatchpoint (lHandle)) == NULL)
	{
		return (S_FALSE);
	};

	pPosition = m_WatchpointList.Find (pWP);
	m_WatchpointList.RemoveAt (pPosition);

	OnHideWatchpoint (pWP);

	if (pWP->GetWatchItem () != 0)
	{
		OnDisconnectWatchpoint (pWP);
	};

	OnDestroyWatchpoint (pWP);

	delete pWP;

	return (S_OK);
}


HRESULT CPrgEditorDoc::LoadWatchpoints (SAFEARRAY *pWPInfos)
{
	HRESULT hr;
	long lLowerBound;
	long lUpperBound;
	long lIndex;

	if (pWPInfos == NULL)
	{
		return (S_OK);
	};

	hr = ::SafeArrayGetLBound (pWPInfos, 1, &lLowerBound);
	if (FAILED (hr))
	{
		FC_DebugPrint("STEDITOR> Failed to get watchpoint safe arrays lower bound (hr=0x%08x)\n", hr);
		return (hr);
	};

	hr = ::SafeArrayGetUBound (pWPInfos, 1, &lUpperBound);
	if (FAILED (hr))
	{
		FC_DebugPrint("STEDITOR> Failed to get watchpoint safe arrays upper bound (hr=0x%08x)\n", hr);
		return (hr);
	};

	ClearAllWatchpoints ();
   StopLDTracing ();

	for (lIndex = lLowerBound; lIndex <= lUpperBound; ++lIndex)
	{
		BSTR sWPInfo;

		hr = ::SafeArrayGetElement (pWPInfos, &lIndex, &sWPInfo);
		if (FAILED (hr))
		{
			FC_DebugPrint("STEDITOR> Failed to get watchpoint safe array element (hr=0x%08x)\n", hr);
			continue;
		};

		OnRestoreWatchpoint (sWPInfo);
		::SysFreeString (sWPInfo);
	};
   StartLDTracing ();

	return (S_OK);
}


HRESULT CPrgEditorDoc::StoreWatchpoints (SAFEARRAY *&pWPInfos)
{
	SAFEARRAYBOUND bound;
	long lIndex=0;
	POSITION pPosition;
	HRESULT hr;
    CWatchpoint *pWP;

	bound.lLbound = 0;
	bound.cElements = 0;


	pPosition = m_WatchpointList.GetHeadPosition ();
	while (pPosition != NULL)
	{
		pWP = m_WatchpointList.GetNext (pPosition);
		assert (pWP != NULL);
        if(pWP && pWP->GetPosition().GetPosQual() != EDP_GRLDBOX)
            bound.cElements++;//EF: ignore ladder contact/coils, they are always reloaded in StartLDTracing
    }

    if (bound.cElements == NULL)
	{
		pWPInfos = NULL;
		return (S_OK);
	};


	pWPInfos = ::SafeArrayCreate (VT_BSTR, 1, &bound);
	assert (pWPInfos != NULL);
	if (pWPInfos == NULL)
	{
		FC_DebugPrint("STEDITOR> Failed to create watchpoint safe array\n");
		return (E_FAIL);
	};

	pPosition = m_WatchpointList.GetHeadPosition ();
	while (pPosition != NULL)
	{
		CString strWPInfo;

		pWP = m_WatchpointList.GetNext (pPosition);
		assert (pWP != NULL);
		if (pWP == NULL)
			continue;

        if(pWP->GetPosition().GetPosQual() == EDP_GRLDBOX)
            continue;  //EF: ignore ladder contact/coils, they are always reloaded in StartLDTracing

		strWPInfo = OnStoreWatchpoint (pWP);

		hr = ::SafeArrayPutElement (pWPInfos, &lIndex, (void *)(BSTR)CComBSTR (strWPInfo));
		if (FAILED (hr))
		{
			FC_DebugPrint("STEDITOR> Failed to put watchpoint safe array element (hr=0x%08x)\n", hr);
			::SafeArrayDestroy (pWPInfos);
			pWPInfos = NULL;
			return (hr);
		};

		++lIndex;
	};

	return (S_OK);
}


HRESULT CPrgEditorDoc::EnableStepMonitoring (bool bOn)
{
	return (S_OK);
}


/////////////////////////////////////////////////////////////////////////////
// ICEDebug functionality

HRESULT CPrgEditorDoc::ShowBreakpoint (const CStdEditorPosition& position, short sState)
{
	CBreakpoint *pBP;

	pBP = LookupBreakpoint (position);

	if (pBP == NULL)
	{
		pBP = new CBreakpoint (position);
		m_BreakpointList.AddTail (pBP);
	};

	pBP->SetActive (sState != 0);
	pBP->SetSubscribed (sState == 1);

	OnShowBreakpoint (pBP);

	return (S_OK);
}


HRESULT CPrgEditorDoc::HideBreakpoint (const CStdEditorPosition& position)
{
	CBreakpoint *pBP;
	POSITION pPosition;

	pBP = LookupBreakpoint (position);
	if (pBP == NULL)
	{
		return (S_FALSE);
	};

	pPosition = m_BreakpointList.Find (pBP);
	assert (pPosition != NULL);
	if (pPosition == NULL)
	{
		return (E_FAIL);
	};

	m_BreakpointList.RemoveAt (pPosition);

	OnHideBreakpoint (pBP);

	delete pBP;

	return (S_OK);
}


HRESULT CPrgEditorDoc::ShowStoppoint (const CStdEditorPosition& position)
{
	HRESULT hr;

	if (m_bStopped)
	{
		hr = HideStoppoint ();
		if (FAILED (hr))
		{
			return (hr);
		};
	};

	m_bStopped = true;
	m_StopPosition = position;

	OnShowStoppoint ();

	return (S_OK);
}


HRESULT CPrgEditorDoc::HideStoppoint ()
{
	if (!m_bStopped)
	{
		return (S_FALSE);
	};

	OnHideStoppoint ();

	m_bStopped = false;

	return (S_OK);
}


/////////////////////////////////////////////////////////////////////////////
// Event handlers

void CPrgEditorDoc::OnNewWatchpointData (CWatchpoint *pWP)
{
	UpdateAllViews (NULL, WatchpointDataHint, pWP);
}


HRESULT CPrgEditorDoc::OnConnectWatchpoint (CWatchpoint *pWP)
{
    return (S_OK);
}

void CPrgEditorDoc::OnShowWatchpoint (CWatchpoint *pWP)
{
	UpdateAllViews (NULL, ShowWatchpointHint, pWP);
}


void CPrgEditorDoc::OnHideWatchpoint (CWatchpoint *pWP)
{
	UpdateAllViews (NULL, HideWatchpointHint, pWP);
}


void CPrgEditorDoc::OnShowBreakpoint (CBreakpoint *pBP)
{
	UpdateAllViews (NULL, ShowBreakpointHint, pBP);
}


void CPrgEditorDoc::OnHideBreakpoint (CBreakpoint *pBP)
{
	UpdateAllViews (NULL, HideBreakpointHint, pBP);
}


void CPrgEditorDoc::OnShowStoppoint ()
{
	UpdateAllViews (NULL, ShowStoppointHint, NULL);
}


void CPrgEditorDoc::OnHideStoppoint ()
{
	UpdateAllViews (NULL, HideStoppointHint, NULL);
}


void CPrgEditorDoc::OnCreateStep (CPrgEdStep *pStep)
{
}


void CPrgEditorDoc::OnDestroyStep (CPrgEdStep *pStep)
{
}


void CPrgEditorDoc::OnStepAttributeChanged (CPrgEdStep *pStep)
{
    UpdateAllViews (NULL, StepAttributeChangedHint, pStep);
}


void CPrgEditorDoc::OnCreateAction (CPrgEdAction *pAction)
{
}


void CPrgEditorDoc::OnDestroyAction (CPrgEdAction *pAction)
{
}


void CPrgEditorDoc::OnActionAttributeChanged (CPrgEdAction *pAction)
{
    UpdateAllViews (NULL, ActionAttributeChangedHint, pAction);
}


void CPrgEditorDoc::OnCreateTransition (CPrgEdTransition *pTransition)
{
}


void CPrgEditorDoc::OnDestroyTransition (CPrgEdTransition *pTransition)
{
}


void CPrgEditorDoc::OnTransitionAttributeChanged (CPrgEdTransition *pTransition)
{
    UpdateAllViews (NULL, TransitionAttributeChangedHint, pTransition);
}


void CPrgEditorDoc::OnAllActionsOffToggled ()
{
    UpdateAllViews (NULL, AllActionsOffToggledHint);
}


void CPrgEditorDoc::OnAllTransitionsOnToggled ()
{
    UpdateAllViews (NULL, AllTransitionsOnToggledHint);
}


void CPrgEditorDoc::OnManualModeToggled ()
{
    UpdateAllViews (NULL, ManualModeToggledHint);
}


void CPrgEditorDoc::OnPerformingCycleToggled ()
{
    UpdateAllViews (NULL, PerformingCycleToggledHint);
}


void CPrgEditorDoc::OnInstancePathModified ()
{
}


/////////////////////////////////////////////////////////////////////////////
// Superclass queries


bool CPrgEditorDoc::QueryDelayClose ()
{
    if (CStdEditorDoc::QueryDelayClose ())
    {
        return (true);
    };

    return (m_bDelayClose);
}


/////////////////////////////////////////////////////////////////////////////
// Private helpers

CCESessionManager *CPrgEditorDoc::GetSessionManager ()
{
    CCESessionManager *pSessionManager;

    ASSERT (m_pSessions != NULL);
    if (m_pSessions == NULL)
    {
        return (NULL);
    };

    pSessionManager = m_pSessions->GetSessionManager ();
    ASSERT (pSessionManager != NULL);

    return (pSessionManager);
}


HRESULT CPrgEditorDoc::GetInstanceDBI ()
{
    HRESULT hr;
    CComPtr<IUnknown> pUnknown;
    CComPtr<IProjectDBI> pProjectDBI;

    m_pInstanceDBI = NULL;
    m_pTypeDBI = NULL;

    if (m_pDebugInfo == NULL)
    {
        return (E_FAIL);
    };

    hr = m_pDebugInfo->get_Project (&pUnknown);
    ASSERT (SUCCEEDED (hr));
    if (hr != S_OK)
    {
        return (hr);
    };

    hr = pUnknown->QueryInterface (IID_IProjectDBI, (LPVOID *)&pProjectDBI);
    pUnknown = NULL;
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        return (hr);
    };
    ASSERT (pProjectDBI != NULL);
    if (pProjectDBI == NULL)
    {
        return (E_UNEXPECTED);
    };

    hr = pProjectDBI->get_InstanceByPath (CComBSTR (m_strInstance), &pUnknown);
    ASSERT (SUCCEEDED (hr));
    if (hr != S_OK)
    {
        return (hr);
    };

    hr = pUnknown->QueryInterface (IID_IInstanceDBI, (LPVOID *)&m_pInstanceDBI);
    pUnknown = NULL;
    //ASSERT (SUCCEEDED (hr));  // SIS: gvl files do not have a instance dbi
    if (FAILED (hr))
    {
        return (hr);
    };
    ASSERT (m_pInstanceDBI != NULL);
    if (m_pInstanceDBI == NULL)
    {
        return (E_UNEXPECTED);
    };

    hr = m_pInstanceDBI->get_Type (&pUnknown);
    if (hr != S_OK)
    {
        return (hr);
    };
    ASSERT (pUnknown != NULL);
    if (pUnknown == NULL)
    {
        return (E_UNEXPECTED);
    };

    hr = pUnknown->QueryInterface (IID_ITypeDBI, (LPVOID *)&m_pTypeDBI);
    pUnknown = NULL;
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        return (hr);
    };
    ASSERT (m_pTypeDBI != NULL);
    if (m_pTypeDBI == NULL)
    {
        return (E_UNEXPECTED);
    };

    return (S_OK);
}


void CPrgEditorDoc::ReadSFCDBI ()
{
    HRESULT hr;
    unsigned long ulCount;

    if (m_pTypeDBI == NULL)
    {
        return;
    };

    hr = m_pTypeDBI->get_StepCount (&ulCount);
    ASSERT (hr == S_OK);
    if (hr == S_OK)
    {
        unsigned long ulIndex;

        for (ulIndex = 0; ulIndex < ulCount; ++ulIndex)
        {
            CComPtr<IUnknown> pUnknown;
            CComPtr<IStepDBI> pStepDBI;
            hr = m_pTypeDBI->get_StepByIndex (ulIndex, &pUnknown);
            ASSERT (hr == S_OK);
            if (hr != S_OK)
                continue;

            hr = pUnknown->QueryInterface (IID_IStepDBI, (LPVOID *)&pStepDBI);
            ASSERT (SUCCEEDED (hr));
            if (FAILED (hr))
                continue;

            CreateStep (pStepDBI);
        };
    };

    hr = m_pTypeDBI->get_ActionCount (&ulCount);
    ASSERT (hr == S_OK);
    if (hr == S_OK)
    {
        unsigned long ulIndex;

        for (ulIndex = 0; ulIndex < ulCount; ++ulIndex)
        {
            CComPtr<IUnknown> pUnknown;
            CComPtr<IActionDBI> pActionDBI;

            hr = m_pTypeDBI->get_ActionByIndex (ulIndex, &pUnknown);
            ASSERT (hr == S_OK);
            if (hr != S_OK)
                continue;

            hr = pUnknown->QueryInterface (IID_IActionDBI, (LPVOID *)&pActionDBI);
            ASSERT (SUCCEEDED (hr));
            if (FAILED (hr))
                continue;

            CreateAction (pActionDBI);
        };
    };

    hr = m_pTypeDBI->get_TransitionCount (&ulCount);
    ASSERT (hr == S_OK);
    if (hr == S_OK)
    {
        unsigned long ulIndex;

        for (ulIndex = 0; ulIndex < ulCount; ++ulIndex)
        {
            CComPtr<IUnknown> pUnknown;
            CComPtr<ITransitionDBI> pTransitionDBI;

            hr = m_pTypeDBI->get_TransitionByIndex (ulIndex, &pUnknown);
            ASSERT (hr == S_OK);
            if (hr != S_OK)
                continue;

            hr = pUnknown->QueryInterface (IID_ITransitionDBI, (LPVOID *)&pTransitionDBI);
            ASSERT (SUCCEEDED (hr));
            if (FAILED (hr))
                continue;

            CreateTransition (pTransitionDBI);
        };
    };
}


bool CPrgEditorDoc::CreateStep (const CComPtr<IStepDBI> &pStepDBI)
{
    HRESULT hr;
    CComBSTR sName;
    long lNumber;
    long lX;
    long lY;
    CPrgEdStep *pStep;

    m_bInternalSFCEnabled = true;

    hr = pStepDBI->get_Name (&sName);
    ASSERT (hr == S_OK);
    if (hr != S_OK)
    {
        return (false);
    };

    hr = pStepDBI->get_Number (&lNumber);
    ASSERT (hr == S_OK);
    if (hr != S_OK)
    {
        return (false);
    };

    hr = pStepDBI->get_X (&lX);
    ASSERT (hr == S_OK);
    if (hr != S_OK)
    {
        return (false);
    };

    hr = pStepDBI->get_Y (&lY);
    ASSERT (hr == S_OK);
    if (hr != S_OK)
    {
        return (false);
    };

    pStep = new CPrgEdStep (CString (sName), lNumber, lX, lY);

    OnCreateStep (pStep);

    m_StepList.AddTail (pStep);

    return (true);
}


void CPrgEditorDoc::DestroyStep (CPrgEdStep *pStep)
{
    POSITION pPosition;

    pPosition = m_StepList.Find (pStep);
    if (pPosition == NULL)
        return;

    m_StepList.RemoveAt (pPosition);

    OnDestroyStep (pStep);
    delete pStep;
}


void CPrgEditorDoc::DestroyAllSteps ()
{
    while (!m_StepList.IsEmpty ())
    {
        DestroyStep (m_StepList.GetHead ());
    };
}


bool CPrgEditorDoc::CreateAction (const CComPtr<IActionDBI> &pActionDBI)
{
    HRESULT hr;
    CComBSTR sName;
    long lNumber;
    long lX;
    long lY;
    CPrgEdAction *pAction;

    m_bInternalSFCEnabled = true;

    hr = pActionDBI->get_Name (&sName);
    ASSERT (hr == S_OK);
    if (hr != S_OK)
    {
        return (false);
    };

    hr = pActionDBI->get_Number (&lNumber);
    ASSERT (hr == S_OK);
    if (hr != S_OK)
    {
        return (false);
    };

    hr = pActionDBI->get_X (&lX);
    ASSERT (hr == S_OK);
    if (hr != S_OK)
    {
        return (false);
    };

    hr = pActionDBI->get_Y (&lY);
    ASSERT (hr == S_OK);
    if (hr != S_OK)
    {
        return (false);
    };

    pAction = new CPrgEdAction (CString (sName), lNumber, lX, lY);

    OnCreateAction (pAction);

    m_ActionList.AddTail (pAction);

    return (true);
}


void CPrgEditorDoc::DestroyAction (CPrgEdAction *pAction)
{
    POSITION pPosition;

    pPosition = m_ActionList.Find (pAction);
    if (pPosition == NULL)
        return;

    m_ActionList.RemoveAt (pPosition);

    OnDestroyAction (pAction);
    delete pAction;
}


void CPrgEditorDoc::DestroyAllActions ()
{
    while (!m_ActionList.IsEmpty ())
    {
        DestroyAction (m_ActionList.GetHead ());
    };
}


bool CPrgEditorDoc::CreateTransition (const CComPtr<ITransitionDBI> &pTransitionDBI)
{
    HRESULT hr;
    CComBSTR sName;
    long lNumber;
    long lX;
    long lY;
    CPrgEdTransition *pTransition;

    m_bInternalSFCEnabled = true;

    hr = pTransitionDBI->get_Name (&sName);
    ASSERT (hr == S_OK);
    if (hr != S_OK)
    {
        return (false);
    };

    hr = pTransitionDBI->get_Number (&lNumber);
    ASSERT (hr == S_OK);
    if (hr != S_OK)
    {
        return (false);
    };

    hr = pTransitionDBI->get_X (&lX);
    ASSERT (hr == S_OK);
    if (hr != S_OK)
    {
        return (false);
    };

    hr = pTransitionDBI->get_Y (&lY);
    ASSERT (hr == S_OK);
    if (hr != S_OK)
    {
        return (false);
    };

    pTransition = new CPrgEdTransition (CString (sName), lNumber, lX, lY);

    OnCreateTransition (pTransition);

    m_TransitionList.AddTail (pTransition);

    return (true);
}


void CPrgEditorDoc::DestroyTransition (CPrgEdTransition *pTransition)
{
    POSITION pPosition;

    pPosition = m_TransitionList.Find (pTransition);
    if (pPosition == NULL)
        return;

    m_TransitionList.RemoveAt (pPosition);

    OnDestroyTransition (pTransition);
    delete pTransition;
}


void CPrgEditorDoc::DestroyAllTransitions ()
{
    while (!m_TransitionList.IsEmpty ())
    {
        DestroyTransition (m_TransitionList.GetHead ());
    };
}


void CPrgEditorDoc::ReadSymbols ()
{
	CString strPrefix;

	if (m_strSymbolFile.IsEmpty ())
	{
		return;
	};

	if (m_strInstance.IsEmpty ())
	{
		return;
	};

	if (m_strMachineName.IsEmpty ())
	{
		return;
	};

	if (m_pSymbolTable == NULL)
	{
		m_pSymbolTable = new CSymbolTable;
	};


	strPrefix.Format ("%s.%s.", m_strMachineAddress, GetWithoutConfiguration ());
	strPrefix.MakeLower ();

	m_pSymbolTable->SetSymbolFile (m_strSymbolFile, strPrefix);

	m_strLocalPrefix.Format ("%s.%s.", m_strMachineAddress, GetWithoutConfiguration ());
	m_strGlobalPrefix.Format ("%s.%s.", m_strMachineAddress, GetResource ());
}


void CPrgEditorDoc::OnSessionModified (CCESession *pSession, bool bDebug)
{
    if (!bDebug)
    {
        if (pSession->GetState () == RW_established)
        {
            if (m_bSFCEnabled)
            {
                StartSFCTracing ();
            };
            StartLDTracing ();
        }
        else if (pSession->GetState () == interrupted)
        {
            StopSFCTracing ();
            StopLDTracing ();
        };
    };
}


void CPrgEditorDoc::OnExpressionChanged (LONG lHandle)
{
    POSITION pPosition;

    if (lHandle == m_lSFCVisHandle)
    {
        SFCVisChanged ();
        return;
    };

    if (lHandle == m_lManualModeHandle)
    {
        ManualModeChanged ();
        return;
    };

    if (lHandle == m_lDoNextHandle)
    {
        DoNextChanged ();
        return;
    };

    if (lHandle == m_lOnceForcedActionsHandle)
    {
        OnceForcedActionsChanged ();
        return;
    };

    if (lHandle == m_lForcedActionsHandle)
    {
        ForcedActionsChanged ();
        return;
    };

    if (lHandle == m_lBlockedActionsHandle)
    {
        BlockedActionsChanged ();
        return;
    };

    if (lHandle == m_lAllActionsOffHandle)
    {
        AllActionsOffChanged ();
        return;
    };

    if (lHandle == m_lOnceForcedTransitionsHandle)
    {
        OnceForcedTransitionsChanged ();
        return;
    };

    if (lHandle == m_lForcedTransitionsHandle)
    {
        ForcedTransitionsChanged ();
        return;
    };

    if (lHandle == m_lBlockedTransitionsHandle)
    {
        BlockedTransitionsChanged ();
        return;
    };

    if (lHandle == m_lAllTransitionsOnHandle)
    {
        AllTransitionsOnChanged ();
        return;
    };

    UpdateAllViews (NULL, ExpressionChangedHint, (CObject *)lHandle);

	pPosition = m_WatchpointList.GetHeadPosition ();
	while (pPosition != NULL)
	{
		CWatchpoint *pWP;

		pWP = m_WatchpointList.GetNext (pPosition);
		assert (pWP != NULL);
		if (pWP == NULL)
			continue;

		if (lHandle == pWP->GetWatchItem ())
		{
#if 0
			TO_DEBUGPRINT6 (_T ("Found watchpoint for subscription of %s (s=0x%08lx, h=0x%08lx, wp=0x%08lx, x=%ld, y=%ld)\n"),
			                (const char *)pSubscription->GetObject (), (long)pSubscription, (long)pSubscription->GetServer (),
			                (long)pWP, (long)pWP->GetPosition ().GetX (), (long)pWP->GetPosition ().GetY ());
#endif

			OnNewWatchpointData (pWP);
		}
	}
}


bool CPrgEditorDoc::GetWatchItemStateAndValue (long lHandle, int iType, CEExpressionState &state, CComVariant &varValue)
{
    HRESULT hr;
    short sQuality;
    DATE timestamp;

    varValue.Clear();

    ASSERT (m_pWatch != NULL);
    if (m_pWatch == NULL)
    {
        return (false);
    };


    // Get state of watch item. We should not get any error here.
    /////////////////////////////////////////////////////////////

    hr = m_pWatch->GetExpressionState (lHandle, &state);
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        return (false);
    };


    // Watch item state CEGoodExpression
    // only is good enough to get a value.
    //////////////////////////////////////

    if (state != CEGoodExpression)
    {
        return (true);
    };


    // Get value of watch item. Should not get any error
    // here. Do not continue if we get an error or the
    // type is not what we expected.
    ////////////////////////////////////////////////////

    hr = m_pWatch->GetData (lHandle, &varValue, &sQuality, &timestamp);
    if (FAILED (hr))
    {
        return (false);
    };
    if (varValue.vt != iType)
    {
        return (false);
    };

    return (true);
}


void CPrgEditorDoc::IncludeNumberArray (CArray<long, long> &numbers, const CArray<long, long> &moreNumbers)
{
    int iMoreIndex;

    for (iMoreIndex = 0; iMoreIndex < moreNumbers.GetSize (); ++iMoreIndex)
    {
        long lNumber;
        int iIndex;

        lNumber = moreNumbers[iMoreIndex];

        for (iIndex = 0; iIndex < numbers.GetSize (); ++iIndex)
        {
            if (lNumber == numbers[iIndex])
                break;
        };

        if (iIndex == numbers.GetSize ())
        {
            numbers.Add (lNumber);
        };
    };
}


/////////////////////////////////////////////////////////////////////////////
// Ladder tracing methods

bool CPrgEditorDoc::IsLadderActive (long lX, long lY)
{
   CPrgWPClientData *pClientData=NULL;
	POSITION pPosition;
   CString  strText;
   LPTSTR   psz;
   bool     bActive = false;
   CStdEditorPosition edipos;

	pPosition = m_WatchpointList.GetHeadPosition ();
	while (pPosition != NULL)
	{
		CWatchpoint *pWP;

		pWP = m_WatchpointList.GetNext (pPosition);
		assert (pWP != NULL);
		if (pWP == NULL)
			continue;

      edipos = pWP->GetPosition();
      if (edipos.GetX() == lX && 
          edipos.GetY() == lY &&
          edipos.GetPosQual() == EDP_GRLDBOX)
      {

         pClientData = (CPrgWPClientData *)pWP->GetClientData ();
	      ASSERT (pClientData != NULL);
	      if (pClientData == NULL)
		      continue;

         strText = pClientData->GetText();
         psz = strText.GetBuffer(0);
         psz = _tcschr(psz, _T(':'));
         if (psz && _tcsstr(psz, _T("True")) != NULL)
         {
            bActive = true;
         } 
         strText.ReleaseBuffer();
   
         break;
      }
   }

   return (bActive);
}


/////////////////////////////////////////////////////////////////////////////
// SFC tracing methods


CString CPrgEditorDoc::GetSFCVisValue ()
{
    return (m_strSFCVisValue);
}


CString CPrgEditorDoc::GetForcedActionsValue ()
{
    return (m_strForcedActionsValue);
}


CString CPrgEditorDoc::GetBlockedActionsValue ()
{
    return (m_strBlockedActionsValue);
}


CString CPrgEditorDoc::GetOnceForcedTransitionsValue ()
{
    return (m_strOnceForcedTransitionsValue);
}


CString CPrgEditorDoc::GetForcedTransitionsValue ()
{
    return (m_strForcedTransitionsValue);
}


CString CPrgEditorDoc::GetBlockedTransitionsValue ()
{
    return (m_strBlockedTransitionsValue);
}


HRESULT CPrgEditorDoc::SetManualMode (bool bOn)
{
    HRESULT hr;
    CComVariant varValue;

    if (m_lManualModeHandle == 0)
    {
        return (S_FALSE);
    };

    if (bOn == m_bManualModeValue)
    {
        return (S_OK);
    };

    ASSERT (m_pWatch != NULL);
    if (m_pWatch == NULL)
    {
        return (E_UNEXPECTED);
    };

    varValue = bOn;
    hr = m_pWatch->Write (m_lManualModeHandle, varValue);

    return (hr);
}


bool CPrgEditorDoc::InManualMode (bool &bIn)
{
    if (m_ManualModeState != CEGoodExpression)
    {
        return (false);
    };

    bIn = m_bManualModeValue;
    return (true);
}


HRESULT CPrgEditorDoc::PerformSFCCycle ()
{
    HRESULT hr;
    CComVariant varValue;

    if (m_lDoNextHandle == 0)
    {
        return (S_FALSE);
    };

    ASSERT (m_pWatch != NULL);
    if (m_pWatch == NULL)
    {
        return (E_UNEXPECTED);
    };

    varValue = true;
    hr = m_pWatch->Write (m_lDoNextHandle, varValue);

    return (hr);
}


bool CPrgEditorDoc::InSFCCycle (bool &bIn)
{
    if (m_DoNextState != CEGoodExpression)
    {
        return (false);
    };

    bIn = m_bDoNextValue;
    return (true);
}


bool CPrgEditorDoc::IsStepPosition (long lX, long lY)
{
    return (LookupStepByPosition (lX, lY) != NULL);
}


bool CPrgEditorDoc::IsStepActive (long lX, long lY)
{
    CPrgEdStep *pStep;

    pStep = LookupStepByPosition (lX, lY);
    if (pStep == NULL)
    {
        return (false);
    };

    return (pStep->IsActive ());
}


bool CPrgEditorDoc::IsActionPosition (long lX, long lY)
{
    return (LookupActionByPosition (lX, lY) != NULL);
}


bool CPrgEditorDoc::IsActionActive (long lX, long lY)
{
    CPrgEdAction *pAction;

    pAction = LookupActionByPosition (lX, lY);
    if (pAction == NULL)
    {
        return (false);
    };

    return (pAction->IsActive ());
}


bool CPrgEditorDoc::IsActionForced (long lX, long lY)
{
    return (IsActionForcedOnce (lX, lY) || IsActionForcedPermanently (lX, lY));
}


bool CPrgEditorDoc::IsActionForcedOnce (long lX, long lY)
{
    CPrgEdAction *pAction;

    pAction = LookupActionByPosition (lX, lY);
    if (pAction == NULL)
    {
        return (false);
    };

    return (pAction->IsForcedOnce ());
}


bool CPrgEditorDoc::IsActionForcedPermanently (long lX, long lY)
{
    CPrgEdAction *pAction;

    pAction = LookupActionByPosition (lX, lY);
    if (pAction == NULL)
    {
        return (false);
    };

    return (pAction->IsForcedPermanently ());
}


bool CPrgEditorDoc::IsActionBlocked (long lX, long lY)
{
    CPrgEdAction *pAction;

    pAction = LookupActionByPosition (lX, lY);
    if (pAction == NULL)
    {
        return (false);
    };

    return (pAction->IsBlocked ());
}


bool CPrgEditorDoc::AreAllActionsBlocked ()
{
    return (m_bAllActionsOffValue);
}


bool CPrgEditorDoc::AreAllActionsBlockedGlobally ()
{
    return (m_bAllActionsOffValue);
}


HRESULT CPrgEditorDoc::ForceActionOnce (bool bOn, long lX, long lY)
{
    HRESULT hr;
    CComPtr<IActionDBI> pActionDBI;
    long lNumber;
    CString strActions;
    CComVariant varValue;

    if (m_lOnceForcedActionsHandle == 0 || m_OnceForcedActionsState != CEGoodExpression)
    {
        return (E_FAIL);
    };

    pActionDBI = LookupActionDBIByPosition (lX, lY);
    if (pActionDBI == NULL)
    {
        return (S_FALSE);
    };

    hr = pActionDBI->get_Number (&lNumber);
    ASSERT (hr == S_OK);
    if (hr != S_OK)
    {
        return (E_UNEXPECTED);
    };

    strActions = m_strOnceForcedActionsValue;

    if (bOn)
    {
        SFCHelper::AddActionToList (strActions, lNumber);
    }
    else
    {
        SFCHelper::RemoveActionFromList (strActions, lNumber);
    };

    varValue = strActions;

    hr = m_pWatch->Write (m_lOnceForcedActionsHandle, varValue);

    return (hr);
}


HRESULT CPrgEditorDoc::ForceAction (bool bOn, long lX, long lY)
{
    HRESULT hr;
    CComPtr<IActionDBI> pActionDBI;
    long lNumber;
    CString strActions;
    CComVariant varValue;

    if (m_lForcedActionsHandle == 0 || m_ForcedActionsState != CEGoodExpression)
    {
        return (E_FAIL);
    };

    pActionDBI = LookupActionDBIByPosition (lX, lY);
    if (pActionDBI == NULL)
    {
        return (S_FALSE);
    };

    hr = pActionDBI->get_Number (&lNumber);
    ASSERT (hr == S_OK);
    if (hr != S_OK)
    {
        return (E_UNEXPECTED);
    };

    strActions = m_strForcedActionsValue;

    if (bOn)
    {
        SFCHelper::AddActionToList (strActions, lNumber);
    }
    else
    {
        SFCHelper::RemoveActionFromList (strActions, lNumber);
    };

    varValue = strActions;

    hr = m_pWatch->Write (m_lForcedActionsHandle, varValue);

    return (hr);
}


HRESULT CPrgEditorDoc::BlockAction (bool bOn, long lX, long lY)
{
    HRESULT hr;
    CComPtr<IActionDBI> pActionDBI;
    long lNumber;
    CString strActions;
    CComVariant varValue;

    if (m_lBlockedActionsHandle == 0 || m_BlockedActionsState != CEGoodExpression)
    {
        return (E_FAIL);
    };

    pActionDBI = LookupActionDBIByPosition (lX, lY);
    if (pActionDBI == NULL)
    {
        return (S_FALSE);
    };

    hr = pActionDBI->get_Number (&lNumber);
    ASSERT (hr == S_OK);
    if (hr != S_OK)
    {
        return (E_UNEXPECTED);
    };

    strActions = m_strBlockedActionsValue;

    if (bOn)
    {
        SFCHelper::AddActionToList (strActions, lNumber);
    }
    else
    {
        SFCHelper::RemoveActionFromList (strActions, lNumber);
    };

    varValue = strActions;

    hr = m_pWatch->Write (m_lBlockedActionsHandle, varValue);

    return (hr);
}


HRESULT CPrgEditorDoc::ForceAllActionsOnce (bool bOn)
{
    HRESULT hr;
    CString strActions;
    POSITION pPosition;
    CComVariant varValue;

    if (m_lOnceForcedActionsHandle == 0 || m_OnceForcedActionsState != CEGoodExpression)
    {
        return (E_FAIL);
    };

    if (bOn)
    {
        pPosition = m_ActionList.GetHeadPosition ();
        while (pPosition != NULL)
        {
            CPrgEdAction *pAction;

            pAction = m_ActionList.GetNext (pPosition);
            ASSERT (pAction != NULL);
            if (pAction == NULL)
                continue;

            SFCHelper::AddActionToList (strActions, pAction->GetNumber ());
        };

        varValue = strActions;
    }
    else
    {
        varValue = _T ("");
    };

    hr = m_pWatch->Write (m_lOnceForcedActionsHandle, varValue);

    return (hr);
}


HRESULT CPrgEditorDoc::ForceAllActions (bool bOn)
{
    HRESULT hr;
    CString strActions;
    POSITION pPosition;
    CComVariant varValue;

    if (m_lForcedActionsHandle == 0 || m_ForcedActionsState != CEGoodExpression)
    {
        return (E_FAIL);
    };

    if (bOn)
    {
        pPosition = m_ActionList.GetHeadPosition ();
        while (pPosition != NULL)
        {
            CPrgEdAction *pAction;

            pAction = m_ActionList.GetNext (pPosition);
            ASSERT (pAction != NULL);
            if (pAction == NULL)
                continue;

            SFCHelper::AddActionToList (strActions, pAction->GetNumber ());
        };

        varValue = strActions;
    }
    else
    {
        varValue = _T ("");
    };

    hr = m_pWatch->Write (m_lForcedActionsHandle, varValue);

    return (hr);
}


HRESULT CPrgEditorDoc::BlockAllActions (bool bOn)
{
    HRESULT hr;
    CString strActions;
    POSITION pPosition;
    CComVariant varValue;

    if (m_lBlockedActionsHandle == 0 || m_BlockedActionsState != CEGoodExpression)
    {
        return (E_FAIL);
    };

    if (bOn)
    {
        pPosition = m_ActionList.GetHeadPosition ();
        while (pPosition != NULL)
        {
            CPrgEdAction *pAction;

            pAction = m_ActionList.GetNext (pPosition);
            ASSERT (pAction != NULL);
            if (pAction == NULL)
                continue;

            SFCHelper::AddActionToList (strActions, pAction->GetNumber ());
        };

        varValue = strActions;
    }
    else
    {
        varValue = _T ("");
    };

    hr = m_pWatch->Write (m_lBlockedActionsHandle, varValue);

    return (hr);
}


HRESULT CPrgEditorDoc::GloballyBlockAllActions (bool bOn)
{
    HRESULT hr;
    CComVariant varValue;

    if (m_lAllActionsOffHandle == 0 || m_AllActionsOffState != CEGoodExpression)
    {
        return (E_FAIL);
    };

    varValue = bOn;

    hr = m_pWatch->Write (m_lAllActionsOffHandle, varValue);

    return (hr);
}


bool CPrgEditorDoc::IsTransitionPosition (long lX, long lY)
{
    return (LookupTransitionByPosition (lX, lY) != NULL);
}


bool CPrgEditorDoc::IsTransitionEnabled (long lX, long lY)
{
    CPrgEdTransition *pTransition;

    pTransition = LookupTransitionByPosition (lX, lY);
    if (pTransition == NULL)
    {
        return (false);
    };

    return (pTransition->IsEnabled ());
}


bool CPrgEditorDoc::IsTransitionForced (long lX, long lY)
{
    return (IsTransitionForcedOnce (lX, lY) || IsTransitionForcedPermanently (lX, lY));
}


bool CPrgEditorDoc::IsTransitionForcedOnce (long lX, long lY)
{
    CPrgEdTransition *pTransition;

    pTransition = LookupTransitionByPosition (lX, lY);
    if (pTransition == NULL)
    {
        return (false);
    };

    return (pTransition->IsForcedOnce ());
}


bool CPrgEditorDoc::IsTransitionForcedPermanently (long lX, long lY)
{
    CPrgEdTransition *pTransition;

    pTransition = LookupTransitionByPosition (lX, lY);
    if (pTransition == NULL)
    {
        return (false);
    };

    return (pTransition->IsForcedPermanently ());
}


bool CPrgEditorDoc::IsTransitionBlocked (long lX, long lY)
{
    CPrgEdTransition *pTransition;

    pTransition = LookupTransitionByPosition (lX, lY);
    if (pTransition == NULL)
    {
        return (false);
    };

    return (pTransition->IsBlocked ());
}


bool CPrgEditorDoc::AreAllTransitionsForced ()
{
    return (m_bAllTransitionsOnValue);
}


bool CPrgEditorDoc::AreAllTransitionsForcedGlobally ()
{
    return (m_bAllTransitionsOnValue);
}


HRESULT CPrgEditorDoc::ForceTransitionOnce (bool bOn, long lX, long lY)
{
    HRESULT hr;
    CComPtr<ITransitionDBI> pTransitionDBI;
    long lNumber;
    CString strTransitions;
    CComVariant varValue;

    if (m_lOnceForcedTransitionsHandle == 0 || m_OnceForcedTransitionsState != CEGoodExpression)
    {
        return (E_FAIL);
    };

    pTransitionDBI = LookupTransitionDBIByPosition (lX, lY);
    if (pTransitionDBI == NULL)
    {
        return (S_FALSE);
    };

    hr = pTransitionDBI->get_Number (&lNumber);
    ASSERT (hr == S_OK);
    if (hr != S_OK)
    {
        return (E_UNEXPECTED);
    };

    hr = WriteTransitionCommand (bOn, lNumber);

    return (hr);
}


HRESULT CPrgEditorDoc::WriteTransitionCommand (bool bSet, long lNumber)
{
    HRESULT hr;
    POSITION pPosition;
    CString strSetCommand;
    CString strResetCommand;
    CString strCommand;
    CComVariant varValue;

    pPosition = m_TransitionList.GetHeadPosition ();
    while (pPosition != NULL)
    {
        CPrgEdTransition *pTransition;
        CString strParameter;
        bool bForced;

        pTransition = m_TransitionList.GetNext (pPosition);
        ASSERT (pTransition != NULL);
        if (pTransition == NULL)
            continue;

        strParameter.Format ("%ld;", pTransition->GetNumber ());

        if (lNumber == pTransition->GetNumber ())
        {
            bForced = bSet;
        }
        else
        {
            bForced = pTransition->IsForcedOnce ();
        };

        if (bForced)
        {
            strSetCommand += strParameter;
        }
        else
        {
            strResetCommand += strParameter;
        };
    };

    if (!strSetCommand.IsEmpty ())
    {
        strSetCommand = _T ("s;") + strSetCommand;
    };

    if (!strResetCommand.IsEmpty ())
    {
        strResetCommand = _T ("r;") + strResetCommand;
    };

    strCommand = strSetCommand + strResetCommand;

    if (strCommand.IsEmpty ())
    {
        return (S_OK);
    };

    varValue = strCommand;

    hr = m_pWatch->Write (m_lManCmdHandle, varValue);

    return (hr);
}


HRESULT CPrgEditorDoc::ForceTransition (bool bOn, long lX, long lY)
{
    HRESULT hr;
    CComPtr<ITransitionDBI> pTransitionDBI;
    long lNumber;
    CString strTransitions;
    CComVariant varValue;

    if (m_lForcedTransitionsHandle == 0 || m_ForcedTransitionsState != CEGoodExpression)
    {
        return (E_FAIL);
    };

    pTransitionDBI = LookupTransitionDBIByPosition (lX, lY);
    if (pTransitionDBI == NULL)
    {
        return (S_FALSE);
    };

    hr = pTransitionDBI->get_Number (&lNumber);
    ASSERT (hr == S_OK);
    if (hr != S_OK)
    {
        return (E_UNEXPECTED);
    };

    strTransitions = m_strForcedTransitionsValue;

    if (bOn)
    {
        SFCHelper::AddTransitionToList (strTransitions, lNumber);
    }
    else
    {
        SFCHelper::RemoveTransitionFromList (strTransitions, lNumber);
    };

    varValue = strTransitions;

    hr = m_pWatch->Write (m_lForcedTransitionsHandle, varValue);

    return (hr);
}


HRESULT CPrgEditorDoc::BlockTransition (bool bOn, long lX, long lY)
{
    HRESULT hr;
    CComPtr<ITransitionDBI> pTransitionDBI;
    long lNumber;
    CString strTransitions;
    CComVariant varValue;

    if (m_lBlockedTransitionsHandle == 0 || m_BlockedTransitionsState != CEGoodExpression)
    {
        return (E_FAIL);
    };

    pTransitionDBI = LookupTransitionDBIByPosition (lX, lY);
    if (pTransitionDBI == NULL)
    {
        return (S_FALSE);
    };

    hr = pTransitionDBI->get_Number (&lNumber);
    ASSERT (hr == S_OK);
    if (hr != S_OK)
    {
        return (E_UNEXPECTED);
    };

    strTransitions = m_strBlockedTransitionsValue;

    if (bOn)
    {
        SFCHelper::AddTransitionToList (strTransitions, lNumber);
    }
    else
    {
        SFCHelper::RemoveTransitionFromList (strTransitions, lNumber);
    };

    varValue = strTransitions;

    hr = m_pWatch->Write (m_lBlockedTransitionsHandle, varValue);

    return (hr);
}


HRESULT CPrgEditorDoc::ForceAllTransitionsOnce (bool bOn)
{
    HRESULT hr;
    CString strTransitions;
    POSITION pPosition;
    CComVariant varValue;

    if (m_lOnceForcedTransitionsHandle == 0 || m_OnceForcedTransitionsState != CEGoodExpression)
    {
        return (E_FAIL);
    };

    pPosition = m_TransitionList.GetHeadPosition ();
    while (pPosition != NULL)
    {
        CPrgEdTransition *pTransition;

        pTransition = m_TransitionList.GetNext (pPosition);
        ASSERT (pTransition != NULL);
        if (pTransition == NULL)
            continue;

        SFCHelper::AddTransitionToList (strTransitions, pTransition->GetNumber ());
    };

    varValue = (bOn ? _T ("s;") : _T ("r;")) + strTransitions;

    hr = m_pWatch->Write (m_lManCmdHandle, varValue);

    return (hr);
}


HRESULT CPrgEditorDoc::ForceAllTransitions (bool bOn)
{
    HRESULT hr;
    CString strTransitions;
    POSITION pPosition;
    CComVariant varValue;

    if (m_lForcedTransitionsHandle == 0 || m_ForcedTransitionsState != CEGoodExpression)
    {
        return (E_FAIL);
    };

    if (bOn)
    {
        pPosition = m_TransitionList.GetHeadPosition ();
        while (pPosition != NULL)
        {
            CPrgEdTransition *pTransition;

            pTransition = m_TransitionList.GetNext (pPosition);
            ASSERT (pTransition != NULL);
            if (pTransition == NULL)
                continue;

            SFCHelper::AddTransitionToList (strTransitions, pTransition->GetNumber ());
        };

        varValue = strTransitions;
    }
    else
    {
        varValue = _T ("");
    };

    hr = m_pWatch->Write (m_lForcedTransitionsHandle, varValue);

    return (hr);
}


HRESULT CPrgEditorDoc::BlockAllTransitions (bool bOn)
{
    HRESULT hr;
    CString strTransitions;
    POSITION pPosition;
    CComVariant varValue;

    if (m_lBlockedTransitionsHandle == 0 || m_BlockedTransitionsState != CEGoodExpression)
    {
        return (E_FAIL);
    };

    if (bOn)
    {
        pPosition = m_TransitionList.GetHeadPosition ();
        while (pPosition != NULL)
        {
            CPrgEdTransition *pTransition;

            pTransition = m_TransitionList.GetNext (pPosition);
            ASSERT (pTransition != NULL);
            if (pTransition == NULL)
                continue;

            SFCHelper::AddTransitionToList (strTransitions, pTransition->GetNumber ());
        };

        varValue = strTransitions;
    }
    else
    {
        varValue = _T ("");
    };

    hr = m_pWatch->Write (m_lBlockedTransitionsHandle, varValue);

    return (hr);
}


HRESULT CPrgEditorDoc::GloballyForceAllTransitions (bool bOn)
{
    HRESULT hr;
    CComVariant varValue;

    if (m_lAllTransitionsOnHandle == 0 || m_AllTransitionsOnState != CEGoodExpression)
    {
        return (E_FAIL);
    };

    varValue = bOn;

    hr = m_pWatch->Write (m_lAllTransitionsOnHandle, varValue);

    return (hr);
}


bool CPrgEditorDoc::IsSFCDebugging ()
{
    CList<CPrgEdAction *, CPrgEdAction *> actions;
    CList<CPrgEdTransition *, CPrgEdTransition *> transitions;

    if (m_bManualModeValue)
    {
        return (true);
    };

    if (m_bAllTransitionsOnValue)
    {
        return (true);
    };

    if (m_bAllActionsOffValue)
    {
        return (true);
    };

    GetActions (actions, ActionForcedOnce);
    GetActions (actions, ActionForcedPermanently);
    GetActions (actions, ActionBlocked);
    if (!actions.IsEmpty ())
    {
        return (true);
    };

    GetTransitions (transitions, TransitionForcedOnce);
    GetTransitions (transitions, TransitionForcedPermanently);
    GetTransitions (transitions, TransitionBlocked);
    if (!transitions.IsEmpty ())
    {
        return (true);
    };

    return (false);
}


void CPrgEditorDoc::GetSteps (CList<CPrgEdStep *, CPrgEdStep *> &steps, StepAttribute attribute)
{
    POSITION pPosition;

    pPosition = m_StepList.GetHeadPosition ();
    while (pPosition != NULL)
    {
        CPrgEdStep *pStep;

        pStep = m_StepList.GetNext (pPosition);
        ASSERT (pStep != NULL);
        if (pStep == NULL)
            continue;

        switch (attribute)
        {
        case StepActive:
            if (pStep->IsActive ())
            {
                steps.AddTail (pStep);
            };
            break;
        };
    };
}


void CPrgEditorDoc::GetStepsAtLine (CList<CPrgEdStep *, CPrgEdStep *> &steps, int iLine)
{
    POSITION pPosition;

    pPosition = m_StepList.GetHeadPosition ();
    while (pPosition != NULL)
    {
        CPrgEdStep *pStep;
        CSTStepClientData *pClientData;

        pStep = m_StepList.GetNext (pPosition);
        ASSERT (pStep != NULL);
        if (pStep == NULL)
            continue;

        pClientData = (CSTStepClientData *)pStep->GetClientData ();
        ASSERT (pClientData != NULL);
        if (pClientData == NULL)
            continue;

        if (iLine == pClientData->GetKeywordPosition ().GetY ())
        {
            steps.AddTail (pStep);
        };
    };
}


void CPrgEditorDoc::GetAllSteps (CList<CPrgEdStep *, CPrgEdStep *> &steps)
{
    POSITION pPosition;

    pPosition = m_StepList.GetHeadPosition ();
    while (pPosition != NULL)
    {
        CPrgEdStep *pStep;

        pStep = m_StepList.GetNext (pPosition);
        ASSERT (pStep != NULL);
        if (pStep == NULL)
            continue;

        steps.AddTail (pStep);
    };
}


void CPrgEditorDoc::GetActions (CList<CPrgEdAction *, CPrgEdAction *> &actions, ActionAttribute attribute)
{
    POSITION pPosition;

    pPosition = m_ActionList.GetHeadPosition ();
    while (pPosition != NULL)
    {
        CPrgEdAction *pAction;

        pAction = m_ActionList.GetNext (pPosition);
        ASSERT (pAction != NULL);
        if (pAction == NULL)
            continue;

        switch (attribute)
        {
        case ActionActive:
            if (pAction->IsActive ())
            {
                actions.AddTail (pAction);
            };
            break;

        case ActionForcedOnce:
            if (pAction->IsForcedOnce ())
            {
                actions.AddTail (pAction);
            };
            break;

        case ActionForcedPermanently:
            if (pAction->IsForcedPermanently ())
            {
                actions.AddTail (pAction);
            };
            break;

        case ActionBlocked:
            if (pAction->IsBlocked ())
            {
                actions.AddTail (pAction);
            };
            break;
        };
    };
}


void CPrgEditorDoc::GetActionsAtLine (CList<CPrgEdAction *, CPrgEdAction *> &actions, int iLine)
{
    POSITION pPosition;

    pPosition = m_ActionList.GetHeadPosition ();
    while (pPosition != NULL)
    {
        CPrgEdAction *pAction;
        CSTActionClientData *pClientData;

        pAction = m_ActionList.GetNext (pPosition);
        ASSERT (pAction != NULL);
        if (pAction == NULL)
            continue;

        pClientData = (CSTActionClientData *)pAction->GetClientData ();
        ASSERT (pClientData != NULL);
        if (pClientData == NULL)
            continue;

        if (iLine == pClientData->GetKeywordPosition ().GetY ())
        {
            actions.AddTail (pAction);
        };
    };
}


void CPrgEditorDoc::GetAllActions (CList<CPrgEdAction *, CPrgEdAction *> &actions)
{
    POSITION pPosition;

    pPosition = m_ActionList.GetHeadPosition ();
    while (pPosition != NULL)
    {
        CPrgEdAction *pAction;

        pAction = m_ActionList.GetNext (pPosition);
        ASSERT (pAction != NULL);
        if (pAction == NULL)
            continue;

        actions.AddTail (pAction);
    };
}


void CPrgEditorDoc::GetTransitions (CList<CPrgEdTransition *, CPrgEdTransition *> &transitions, TransitionAttribute attribute)
{
    POSITION pPosition;

    pPosition = m_TransitionList.GetHeadPosition ();
    while (pPosition != NULL)
    {
        CPrgEdTransition *pTransition;

        pTransition = m_TransitionList.GetNext (pPosition);
        ASSERT (pTransition != NULL);
        if (pTransition == NULL)
            continue;

        switch (attribute)
        {
        case TransitionEnabled:
            if (pTransition->IsEnabled ())
            {
                transitions.AddTail (pTransition);
            };
            break;

        case TransitionForcedOnce:
            if (pTransition->IsForcedOnce ())
            {
                transitions.AddTail (pTransition);
            };
            break;

        case TransitionForcedPermanently:
            if (pTransition->IsForcedPermanently ())
            {
                transitions.AddTail (pTransition);
            };
            break;

        case TransitionBlocked:
            if (pTransition->IsBlocked ())
            {
                transitions.AddTail (pTransition);
            };
            break;
        };
    };
}


void CPrgEditorDoc::GetTransitionsAtLine (CList<CPrgEdTransition *, CPrgEdTransition *> &transitions, int iLine)
{
    POSITION pPosition;

    pPosition = m_TransitionList.GetHeadPosition ();
    while (pPosition != NULL)
    {
        CPrgEdTransition *pTransition;
        CSTTransitionClientData *pClientData;

        pTransition = m_TransitionList.GetNext (pPosition);
        ASSERT (pTransition != NULL);
        if (pTransition == NULL)
            continue;

        pClientData = (CSTTransitionClientData *)pTransition->GetClientData ();
        ASSERT (pClientData != NULL);
        if (pClientData == NULL)
            continue;

        if (iLine == pClientData->GetKeywordPosition ().GetY ())
        {
            transitions.AddTail (pTransition);
        };
    };
}


void CPrgEditorDoc::GetAllTransitions (CList<CPrgEdTransition *, CPrgEdTransition *> &transitions)
{
    POSITION pPosition;

    pPosition = m_TransitionList.GetHeadPosition ();
    while (pPosition != NULL)
    {
        CPrgEdTransition *pTransition;

        pTransition = m_TransitionList.GetNext (pPosition);
        ASSERT (pTransition != NULL);
        if (pTransition == NULL)
            continue;

        transitions.AddTail (pTransition);
    };
}


HRESULT CPrgEditorDoc::StartSFCTracing ()
{
    HRESULT hr;
    HRESULT hrResult=S_OK;

    if (!(m_bSFCEnabled && m_bInternalSFCEnabled))
    {
        return (S_FALSE);
    };


    // Start monitoring of __sfcvis variable.
    /////////////////////////////////////////

    hr = StartVisMonitoring ();
    if (FAILED (hr) && hrResult == S_OK)
    {
        hrResult = hr;
    };


    // Start monitoring of __manualMode variable.
    /////////////////////////////////////////////

    hr = StartManualModeMonitoring ();
    if (FAILED (hr) && hrResult == S_OK)
    {
        hrResult = hr;
    };


    // Start monitoring of __doNext variable.
    /////////////////////////////////////////

    hr = StartDoNextMonitoring ();
    if (FAILED (hr) && hrResult == S_OK)
    {
        hrResult = hr;
    };


    // Start monitoring of __forcedActions, __blockedActions
    // and __allActionsOff variables.
    ////////////////////////////////////////////////////////

    hr = StartActionForcingMonitoring ();
    if (FAILED (hr) && hrResult == S_OK)
    {
        hrResult = hr;
    };


    // Start monitoring of __onceForcedTransitions, __forcedTransitions,
    // __blockedTransitions, __allTransitionsOn and __manCmd variables.
    ////////////////////////////////////////////////////////////////////

    hr = StartTransitionForcingMonitoring ();
    if (FAILED (hr) && hrResult == S_OK)
    {
        hrResult = hr;
    };

    return (hrResult);
}


HRESULT CPrgEditorDoc::StopSFCTracing ()
{
    HRESULT hr;
    HRESULT hrResult=S_OK;

    if (!(m_bSFCEnabled && m_bInternalSFCEnabled))
    {
        return (S_FALSE);
    };


    // Stop monitoring of __sfcvis variable.
    ////////////////////////////////////////

    hr = StopVisMonitoring ();
    if (FAILED (hr) && hrResult == S_OK)
    {
        hrResult = hr;
    };


    // Stop monitoring of __manualMode variable.
    ////////////////////////////////////////////

    hr = StopManualModeMonitoring ();
    if (FAILED (hr) && hrResult == S_OK)
    {
        hrResult = hr;
    };


    // Stop monitoring of __doNext variable.
    ////////////////////////////////////////

    hr = StopDoNextMonitoring ();
    if (FAILED (hr) && hrResult == S_OK)
    {
        hrResult = hr;
    };


    // Stop monitoring of __forcedActions, __blockedActions
    // and __allActionsOff variables.
    ///////////////////////////////////////////////////////

    hr = StopActionForcingMonitoring ();
    if (FAILED (hr) && hrResult == S_OK)
    {
        hrResult = hr;
    };


    // Stop monitoring of __onceForcedTransitions, __forcedTransitions,
    // __blockedTransitions, __allTransitionsOn and __manCmd variables.
    ///////////////////////////////////////////////////////////////////

    hr = StopTransitionForcingMonitoring ();
    if (FAILED (hr) && hrResult == S_OK)
    {
        hrResult = hr;
    };


    BeginSFCUpdate ();
    ResetAllSteps ();
    ResetAllActions ();
    ResetAllTransitions ();
    EndSFCUpdate ();


    return (hrResult);
}


HRESULT CPrgEditorDoc::StartLDTracing (void)
{
    return (S_OK);
}

HRESULT CPrgEditorDoc::StopLDTracing (void)
{
    return (S_OK);
}

HRESULT CPrgEditorDoc::StartVisMonitoring ()
{
    HRESULT hr;

    if (m_lSFCVisHandle != 0)
    {
        return (S_OK);
    };

    hr = StartVariableMonitoring (SFCHelper::VisName, m_lSFCVisHandle);
    return (hr);
}


HRESULT CPrgEditorDoc::StopVisMonitoring ()
{
    HRESULT hr;

    if (m_lSFCVisHandle == 0)
    {
        return (S_OK);
    };

    hr = StopVariableMonitoring (m_lSFCVisHandle);

    m_lSFCVisHandle = 0;

    return (hr);
}


HRESULT CPrgEditorDoc::StartManualModeMonitoring ()
{
    HRESULT hr;

    if (m_lManualModeHandle != 0)
    {
        return (S_OK);
    };

    hr = StartVariableMonitoring (SFCHelper::ManualModeName, m_lManualModeHandle);
    return (hr);
}


HRESULT CPrgEditorDoc::StopManualModeMonitoring ()
{
    HRESULT hr;

    if (m_lManualModeHandle == 0)
    {
        return (S_OK);
    };

    hr = StopVariableMonitoring (m_lManualModeHandle);

    m_lManualModeHandle = 0;

    m_bManualModeValue = false;

    return (hr);
}


HRESULT CPrgEditorDoc::StartDoNextMonitoring ()
{
    HRESULT hr;

    if (m_lDoNextHandle != 0)
    {
        return (S_OK);
    };

    hr = StartVariableMonitoring (SFCHelper::DoNextName, m_lDoNextHandle);
    return (hr);
}


HRESULT CPrgEditorDoc::StopDoNextMonitoring ()
{
    HRESULT hr;

    if (m_lDoNextHandle == 0)
    {
        return (S_OK);
    };

    hr = StopVariableMonitoring (m_lDoNextHandle);

    m_lDoNextHandle = 0;

    m_bDoNextValue = false;

    return (hr);
}


HRESULT CPrgEditorDoc::StartActionForcingMonitoring ()
{
    HRESULT hr;

    if (m_lOnceForcedActionsHandle == 0)
    {
        hr = StartVariableMonitoring (SFCHelper::OnceForcedActionsName, m_lOnceForcedActionsHandle);
        if (FAILED (hr))
        {
            return (hr);
        };
    };

    if (m_lForcedActionsHandle == 0)
    {
        hr = StartVariableMonitoring (SFCHelper::ForcedActionsName, m_lForcedActionsHandle);
        if (FAILED (hr))
        {
            StopVariableMonitoring (m_lOnceForcedActionsHandle);
            return (hr);
        };
    };

    if (m_lBlockedActionsHandle == 0)
    {
        hr = StartVariableMonitoring (SFCHelper::BlockedActionsName, m_lBlockedActionsHandle);
        if (FAILED (hr))
        {
            StopVariableMonitoring (m_lOnceForcedActionsHandle);
            StopVariableMonitoring (m_lForcedActionsHandle);
            return (hr);
        };
    };

    if (m_lAllActionsOffHandle == 0)
    {
        hr = StartVariableMonitoring (SFCHelper::AllActionsOffName, m_lAllActionsOffHandle);
        if (FAILED (hr))
        {
            StopVariableMonitoring (m_lOnceForcedActionsHandle);
            StopVariableMonitoring (m_lForcedActionsHandle);
            StopVariableMonitoring (m_lBlockedActionsHandle);
            return (hr);
        };
    };

    return (S_OK);
}


HRESULT CPrgEditorDoc::StopActionForcingMonitoring ()
{
    HRESULT hr;
    HRESULT hrResult=S_OK;

    if (m_lOnceForcedActionsHandle != 0)
    {
        hr = StopVariableMonitoring (m_lOnceForcedActionsHandle);

        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        };

        m_lOnceForcedActionsHandle = 0;
    };

    if (m_lForcedActionsHandle != 0)
    {
        hr = StopVariableMonitoring (m_lForcedActionsHandle);

        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        };

        m_lForcedActionsHandle = 0;
    };

    if (m_lBlockedActionsHandle != 0)
    {
        hr = StopVariableMonitoring (m_lBlockedActionsHandle);

        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        };

        m_lBlockedActionsHandle = 0;
    };

    if (m_lAllActionsOffHandle != 0)
    {
        hr = StopVariableMonitoring (m_lAllActionsOffHandle);

        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        };

        m_lAllActionsOffHandle = 0;
    };

    return (hrResult);
}


HRESULT CPrgEditorDoc::StartTransitionForcingMonitoring ()
{
    HRESULT hr;

    if (m_lOnceForcedTransitionsHandle == 0)
    {
        hr = StartVariableMonitoring (SFCHelper::OnceForcedTransitionsName, m_lOnceForcedTransitionsHandle);
        if (FAILED (hr))
        {
            return (hr);
        };
    };

    if (m_lForcedTransitionsHandle == 0)
    {
        hr = StartVariableMonitoring (SFCHelper::ForcedTransitionsName, m_lForcedTransitionsHandle);
        if (FAILED (hr))
        {
            StopVariableMonitoring (m_lOnceForcedTransitionsHandle);
            return (hr);
        };
    };

    if (m_lBlockedTransitionsHandle == 0)
    {
        hr = StartVariableMonitoring (SFCHelper::BlockedTransitionsName, m_lBlockedTransitionsHandle);
        if (FAILED (hr))
        {
            StopVariableMonitoring (m_lOnceForcedTransitionsHandle);
            StopVariableMonitoring (m_lForcedTransitionsHandle);
            return (hr);
        };
    };

    if (m_lAllTransitionsOnHandle == 0)
    {
        hr = StartVariableMonitoring (SFCHelper::AllTransitionsOnName, m_lAllTransitionsOnHandle);
        if (FAILED (hr))
        {
            StopVariableMonitoring (m_lOnceForcedTransitionsHandle);
            StopVariableMonitoring (m_lForcedTransitionsHandle);
            StopVariableMonitoring (m_lBlockedTransitionsHandle);
            return (hr);
        };
    };

    if (m_lManCmdHandle == 0)
    {
        hr = StartVariableMonitoring (SFCHelper::ManCmdName, m_lManCmdHandle);
        if (FAILED (hr))
        {
            StopVariableMonitoring (m_lOnceForcedTransitionsHandle);
            StopVariableMonitoring (m_lForcedTransitionsHandle);
            StopVariableMonitoring (m_lBlockedTransitionsHandle);
            StopVariableMonitoring (m_lAllTransitionsOnHandle);
            return (hr);
        };
    };

    return (S_OK);
}


HRESULT CPrgEditorDoc::StopTransitionForcingMonitoring ()
{
    HRESULT hr;
    HRESULT hrResult=S_OK;

    if (m_lOnceForcedTransitionsHandle != 0)
    {
        hr = StopVariableMonitoring (m_lOnceForcedTransitionsHandle);

        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        };

        m_lOnceForcedTransitionsHandle = 0;
    };

    if (m_lForcedTransitionsHandle != 0)
    {
        hr = StopVariableMonitoring (m_lForcedTransitionsHandle);

        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        };

        m_lForcedTransitionsHandle = 0;
    };

    if (m_lBlockedTransitionsHandle != 0)
    {
        hr = StopVariableMonitoring (m_lBlockedTransitionsHandle);

        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        };

        m_lBlockedTransitionsHandle = 0;
    };

    if (m_lAllTransitionsOnHandle != 0)
    {
        hr = StopVariableMonitoring (m_lAllTransitionsOnHandle);

        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        };

        m_lAllTransitionsOnHandle = 0;
    };

    if (m_lManCmdHandle != 0)
    {
        hr = StopVariableMonitoring (m_lManCmdHandle);

        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        };

        m_lManCmdHandle = 0;
    };

    return (hrResult);
}


HRESULT CPrgEditorDoc::StartVariableMonitoring (const CString &strName, long &lHandle)
{
    HRESULT hr;
    CString strPath;

    strPath.Format ("%s.%s", (LPCTSTR)m_strInstance, (LPCTSTR)strName);

    hr = m_pWatch->AddItem (CComBSTR (strPath), &lHandle);
    if (FAILED (hr))
    {
        return (hr);
    };

    hr = m_pWatch->Connect (lHandle);
    if (FAILED (hr))
    {
        m_pWatch->RemoveItem (lHandle);
        return (hr);
    };

    return (S_OK);
}


HRESULT CPrgEditorDoc::StopVariableMonitoring (long lHandle)
{
    HRESULT hr;
    HRESULT hrResult=S_OK;

    hr = m_pWatch->Disconnect (lHandle);
    if (FAILED (hr) && hrResult == S_OK)
    {
        hrResult = hr;
    };

    hr = m_pWatch->RemoveItem (lHandle);
    if (FAILED (hr) && hrResult == S_OK)
    {
        hrResult = hr;
    };

    return (hrResult);
}


void CPrgEditorDoc::SFCVisChanged ()
{
    CComVariant varValue;
    CArray<long, long> activeSteps;
    CArray<long, long> activeActions;
    CArray<long, long> enabledTransitions;

    if (!GetWatchItemStateAndValue (m_lSFCVisHandle, VT_BSTR, m_SFCVisState, varValue))
    {
        m_SFCVisState = CEEmptyValue;
    };
    if (m_SFCVisState != CEGoodExpression)
    {
        return;
    };


    // Cache value of __sfcvis.
    ///////////////////////////

    m_strSFCVisValue = varValue.bstrVal;


    // Update cycle for SFC state change.
    /////////////////////////////////////

    BeginSFCUpdate ();

    SFCHelper::StepsInVis (m_strSFCVisValue, activeSteps);
    UpdateSteps (StepActive, activeSteps);

    SFCHelper::ActionsInVis (m_strSFCVisValue, activeActions);
    UpdateActions (ActionActive, activeActions);

    SFCHelper::TransitionsInVis (m_strSFCVisValue, enabledTransitions);
    UpdateTransitions (TransitionEnabled, enabledTransitions);

    EndSFCUpdate ();
}


void CPrgEditorDoc::ManualModeChanged ()
{
    CComVariant varValue;
    bool bChanged;

    if (!GetWatchItemStateAndValue (m_lManualModeHandle, VT_BOOL, m_ManualModeState, varValue))
    {
        m_ManualModeState = CEEmptyValue;
    };
    if (m_ManualModeState != CEGoodExpression)
    {
        return;
    };


    bChanged = m_bManualModeValue != (varValue.boolVal != 0);


    // Cache value of __manualMode.
    ///////////////////////////////

    m_bManualModeValue = varValue.boolVal != 0;


    if (bChanged)
    {
        BeginSFCUpdate ();
        OnManualModeToggled ();
        EndSFCUpdate ();
    };
}


void CPrgEditorDoc::DoNextChanged ()
{
    CComVariant varValue;
    bool bChanged;

    if (!GetWatchItemStateAndValue (m_lDoNextHandle, VT_BOOL, m_DoNextState, varValue))
    {
        m_DoNextState = CEEmptyValue;
    };
    if (m_DoNextState != CEGoodExpression)
    {
        return;
    };


    bChanged = m_bDoNextValue != (varValue.boolVal != 0);


    // Cache value of __doNext.
    ///////////////////////////

    m_bDoNextValue = varValue.boolVal != 0;


    if (bChanged)
    {
        BeginSFCUpdate ();
        OnPerformingCycleToggled ();
        EndSFCUpdate ();
    };
}


void CPrgEditorDoc::OnceForcedActionsChanged ()
{
    CComVariant varValue;
    CArray<long, long> forcedActions;

    if (!GetWatchItemStateAndValue (m_lOnceForcedActionsHandle, VT_BSTR, m_OnceForcedActionsState, varValue))
    {
        m_OnceForcedActionsState = CEEmptyValue;
    };
    if (m_OnceForcedActionsState != CEGoodExpression)
    {
        return;
    };


    // Cache value of __onceForcedActions.
    //////////////////////////////////////

    m_strOnceForcedActionsValue = varValue.bstrVal;


    // Update cycle for SFC state change.
    /////////////////////////////////////

    BeginSFCUpdate ();

    SFCHelper::ActionsInList (m_strOnceForcedActionsValue, forcedActions);

    UpdateActions (ActionForcedOnce, forcedActions);

    EndSFCUpdate ();
}


void CPrgEditorDoc::ForcedActionsChanged ()
{
    CComVariant varValue;
    CArray<long, long> forcedActions;

    if (!GetWatchItemStateAndValue (m_lForcedActionsHandle, VT_BSTR, m_ForcedActionsState, varValue))
    {
        m_ForcedActionsState = CEEmptyValue;
    };
    if (m_ForcedActionsState != CEGoodExpression)
    {
        return;
    };


    // Cache value of __forcedActions.
    //////////////////////////////////

    m_strForcedActionsValue = varValue.bstrVal;


    // Update cycle for SFC state change.
    /////////////////////////////////////

    BeginSFCUpdate ();

    SFCHelper::ActionsInList (m_strForcedActionsValue, forcedActions);

    UpdateActions (ActionForcedPermanently, forcedActions);

    EndSFCUpdate ();
}


void CPrgEditorDoc::BlockedActionsChanged ()
{
    CComVariant varValue;
    CArray<long, long> blockedActions;

    if (!GetWatchItemStateAndValue (m_lBlockedActionsHandle, VT_BSTR, m_BlockedActionsState, varValue))
    {
        m_BlockedActionsState = CEEmptyValue;
    };
    if (m_BlockedActionsState != CEGoodExpression)
    {
        return;
    };


    // Cache value of __blockedActions.
    ///////////////////////////////////

    m_strBlockedActionsValue = varValue.bstrVal;


    // Update cycle for SFC state change.
    /////////////////////////////////////

    BeginSFCUpdate ();

    SFCHelper::ActionsInList (m_strBlockedActionsValue, blockedActions);

    UpdateActions (ActionBlocked, blockedActions);

    EndSFCUpdate ();
}


void CPrgEditorDoc::AllActionsOffChanged ()
{
    CComVariant varValue;
    bool bChanged;

    if (!GetWatchItemStateAndValue (m_lAllActionsOffHandle, VT_BOOL, m_AllActionsOffState, varValue))
    {
        m_AllActionsOffState = CEEmptyValue;
    };
    if (m_AllActionsOffState != CEGoodExpression)
    {
        return;
    };


    bChanged = m_bAllActionsOffValue != (varValue.boolVal != 0);


    // Cache value of __allActionsOff.
    //////////////////////////////////

    m_bAllActionsOffValue = varValue.boolVal != 0;


    // Update cycle for SFC state change.
    /////////////////////////////////////

    if (bChanged)
    {
        BeginSFCUpdate ();
        OnAllActionsOffToggled ();
        EndSFCUpdate ();
    };
}


void CPrgEditorDoc::OnceForcedTransitionsChanged ()
{
    CComVariant varValue;
    CArray<long, long> forcedTransitions;
    CArray<long, long> moreForcedTransitions;

    if (!GetWatchItemStateAndValue (m_lOnceForcedTransitionsHandle, VT_BSTR, m_OnceForcedTransitionsState, varValue))
    {
        m_OnceForcedTransitionsState = CEEmptyValue;
    };
    if (m_OnceForcedTransitionsState != CEGoodExpression)
    {
        return;
    };


    // Cache value of __onceForcedTransitions.
    //////////////////////////////////////////

    m_strOnceForcedTransitionsValue = varValue.bstrVal;


    // Update cycle for SFC state change.
    /////////////////////////////////////

    BeginSFCUpdate ();

    SFCHelper::TransitionsInList (m_strOnceForcedTransitionsValue, forcedTransitions);

    UpdateTransitions (TransitionForcedOnce, forcedTransitions);

    EndSFCUpdate ();
}


void CPrgEditorDoc::ForcedTransitionsChanged ()
{
    CComVariant varValue;
    CArray<long, long> forcedTransitions;
    CArray<long, long> moreForcedTransitions;

    if (!GetWatchItemStateAndValue (m_lForcedTransitionsHandle, VT_BSTR, m_ForcedTransitionsState, varValue))
    {
        m_ForcedTransitionsState = CEEmptyValue;
    };
    if (m_ForcedTransitionsState != CEGoodExpression)
    {
        return;
    };


    // Cache value of __forcedTransitions.
    //////////////////////////////////////

    m_strForcedTransitionsValue = varValue.bstrVal;


    // Update cycle for SFC state change.
    /////////////////////////////////////

    BeginSFCUpdate ();

    SFCHelper::TransitionsInList (m_strForcedTransitionsValue, forcedTransitions);

    UpdateTransitions (TransitionForcedPermanently, forcedTransitions);

    EndSFCUpdate ();
}


void CPrgEditorDoc::BlockedTransitionsChanged ()
{
    CComVariant varValue;
    CArray<long, long> blockedTransitions;

    if (!GetWatchItemStateAndValue (m_lBlockedTransitionsHandle, VT_BSTR, m_BlockedTransitionsState, varValue))
    {
        m_BlockedTransitionsState = CEEmptyValue;
    };
    if (m_BlockedTransitionsState != CEGoodExpression)
    {
        return;
    };


    // Cache value of __blockedTransitions.
    ///////////////////////////////////////

    m_strBlockedTransitionsValue = varValue.bstrVal;


    // Update cycle for SFC state change.
    /////////////////////////////////////

    BeginSFCUpdate ();

    SFCHelper::TransitionsInList (m_strBlockedTransitionsValue, blockedTransitions);

    UpdateTransitions (TransitionBlocked, blockedTransitions);

    EndSFCUpdate ();
}


void CPrgEditorDoc::AllTransitionsOnChanged ()
{
    CComVariant varValue;
    bool bChanged;

    if (!GetWatchItemStateAndValue (m_lAllTransitionsOnHandle, VT_BOOL, m_AllTransitionsOnState, varValue))
    {
        m_AllTransitionsOnState = CEEmptyValue;
    };
    if (m_AllTransitionsOnState != CEGoodExpression)
    {
        return;
    };


    bChanged = m_bAllTransitionsOnValue != (varValue.boolVal != 0);


    // Cache value of __allTransitionsOn.
    /////////////////////////////////////

    m_bAllTransitionsOnValue = varValue.boolVal != 0;


    // Update cycle for SFC state change.
    /////////////////////////////////////

    if (bChanged)
    {
        BeginSFCUpdate ();
        OnAllTransitionsOnToggled ();
        EndSFCUpdate ();
    };
}


void CPrgEditorDoc::BeginSFCUpdate ()
{
    UpdateAllViews (NULL, BeginSFCUpdateHint);
}


void CPrgEditorDoc::EndSFCUpdate ()
{
    UpdateAllViews (NULL, EndSFCUpdateHint);
}


void CPrgEditorDoc::UpdateSteps (StepAttribute attribute, const CArray<long, long> &stepNumbers)
{
    POSITION pPosition;

    pPosition = m_StepList.GetHeadPosition ();
    while (pPosition != NULL)
    {
        CPrgEdStep *pStep;
        int iIndex;

        pStep = m_StepList.GetNext (pPosition);
        ASSERT (pStep != NULL);
        if (pStep == NULL)
            continue;

        for (iIndex = 0; iIndex < stepNumbers.GetSize (); ++iIndex)
        {
            if (pStep->GetNumber () == stepNumbers.GetAt (iIndex))
            {
                break;
            };
        };

        if (iIndex == stepNumbers.GetSize ())
        {
            switch (attribute)
            {
            case StepActive:
                if (pStep->IsActive ())
                {
                    pStep->SetActive (false);
                    OnStepAttributeChanged (pStep);
                };
                break;
            };
        }
        else
        {
            switch (attribute)
            {
            case StepActive:
                if (!pStep->IsActive ())
                {
                    pStep->SetActive (true);
                    OnStepAttributeChanged (pStep);
                };
                break;
            };
        };
    };
}


CPrgEdStep *CPrgEditorDoc::LookupStepByPosition (long lX, long lY)
{
    HRESULT hr;
    CComPtr<IStepDBI> pStepDBI;
    long lNumber;
    POSITION pPosition;

    pStepDBI = LookupStepDBIByPosition (lX, lY);
    if (pStepDBI == NULL)
    {
        return (NULL);
    };

    hr = pStepDBI->get_Number (&lNumber);
    if (FAILED (hr))
    {
        return (NULL);
    };

    pPosition = m_StepList.GetHeadPosition ();
    while (pPosition != NULL)
    {
        CPrgEdStep *pStep;

        pStep = m_StepList.GetNext (pPosition);
        if (pStep->GetNumber () == lNumber)
        {
            return (pStep);
        };
    };

    return (NULL);
}


CComPtr<IStepDBI> CPrgEditorDoc::LookupStepDBIByPosition (long lX, long lY)
{
    HRESULT hr;
    CComPtr<IUnknown> pUnknown;
    CComPtr<IStepDBI> pStepDBI;

    if (m_pTypeDBI == NULL)
    {
        return (NULL);
    };

    hr = m_pTypeDBI->get_StepByPosition (lX, lY, &pUnknown);
    ASSERT (SUCCEEDED (hr));
    if (hr != S_OK)
    {
        return (NULL);
    };
    ASSERT (pUnknown != NULL);
    if (pUnknown == NULL)
    {
        return (NULL);
    };

    hr = pUnknown->QueryInterface (IID_IStepDBI, (LPVOID *)&pStepDBI);
    pUnknown = NULL;
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        return (NULL);
    };
    ASSERT (pStepDBI != NULL);

    return (pStepDBI);
}


void CPrgEditorDoc::ResetAllSteps ()
{
    POSITION pPosition;

    pPosition = m_StepList.GetHeadPosition ();
    while (pPosition != NULL)
    {
        CPrgEdStep *pStep;
        bool bChanged;

        pStep = m_StepList.GetNext (pPosition);
        ASSERT (pStep != NULL);
        if (pStep == NULL)
            continue;

        bChanged = pStep->IsActive ();

        pStep->SetActive (false);

        if (bChanged)
        {
            OnStepAttributeChanged (pStep);
        };
    };
}


void CPrgEditorDoc::UpdateActions (ActionAttribute attribute, const CArray<long, long> &actionNumbers)
{
    POSITION pPosition;

    pPosition = m_ActionList.GetHeadPosition ();
    while (pPosition != NULL)
    {
        CPrgEdAction *pAction;
        int iIndex;
        bool bOn;

        pAction = m_ActionList.GetNext (pPosition);
        ASSERT (pAction != NULL);
        if (pAction == NULL)
            continue;

        for (iIndex = 0; iIndex < actionNumbers.GetSize (); ++iIndex)
        {
            if (pAction->GetNumber () == actionNumbers.GetAt (iIndex))
            {
                break;
            };
        };

        bOn = iIndex != actionNumbers.GetSize ();

        switch (attribute)
        {
        case ActionActive:
            if (bOn != pAction->IsActive ())
            {
                pAction->SetActive (bOn);
                OnActionAttributeChanged (pAction);
            };
            break;

        case ActionForcedOnce:
            if (bOn != pAction->IsForcedOnce ())
            {
                pAction->SetForcedOnce (bOn);
                OnActionAttributeChanged (pAction);
            };
            break;

        case ActionForcedPermanently:
            if (bOn != pAction->IsForcedPermanently ())
            {
                pAction->SetForcedPermanently (bOn);
                OnActionAttributeChanged (pAction);
            };
            break;

        case ActionBlocked:
            if (bOn != pAction->IsBlocked ())
            {
                pAction->SetBlocked (bOn);
                OnActionAttributeChanged (pAction);
            };
            break;
        };
    };
}


CPrgEdAction *CPrgEditorDoc::LookupActionByPosition (long lX, long lY)
{
    HRESULT hr;
    CComPtr<IActionDBI> pActionDBI;
    long lNumber;
    POSITION pPosition;

    pActionDBI = LookupActionDBIByPosition (lX, lY);
    if (pActionDBI == NULL)
    {
        return (NULL);
    };

    hr = pActionDBI->get_Number (&lNumber);
    if (FAILED (hr))
    {
        return (NULL);
    };

    pPosition = m_ActionList.GetHeadPosition ();
    while (pPosition != NULL)
    {
        CPrgEdAction *pAction;

        pAction = m_ActionList.GetNext (pPosition);
        if (pAction->GetNumber () == lNumber)
        {
            return (pAction);
        };
    };

    return (NULL);
}


CComPtr<IActionDBI> CPrgEditorDoc::LookupActionDBIByPosition (long lX, long lY)
{
    HRESULT hr;
    CComPtr<IUnknown> pUnknown;
    CComPtr<IActionDBI> pActionDBI;

    if (m_pTypeDBI == NULL)
    {
        return (NULL);
    };

    hr = m_pTypeDBI->get_ActionByPosition (lX, lY, &pUnknown);
    ASSERT (SUCCEEDED (hr));
    if (hr != S_OK)
    {
        return (NULL);
    };
    ASSERT (pUnknown != NULL);
    if (pUnknown == NULL)
    {
        return (NULL);
    };

    hr = pUnknown->QueryInterface (IID_IActionDBI, (LPVOID *)&pActionDBI);
    pUnknown = NULL;
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        return (NULL);
    };
    ASSERT (pActionDBI != NULL);

    return (pActionDBI);
}


void CPrgEditorDoc::ResetAllActions ()
{
    POSITION pPosition;

    pPosition = m_ActionList.GetHeadPosition ();
    while (pPosition != NULL)
    {
        CPrgEdAction *pAction;
        bool bChanged;

        pAction = m_ActionList.GetNext (pPosition);
        ASSERT (pAction != NULL);
        if (pAction == NULL)
            continue;

        bChanged = pAction->IsActive () ||
                   pAction->IsForcedOnce () ||
                   pAction->IsForcedPermanently () ||
                   pAction->IsBlocked ();

        pAction->SetActive (false);
        pAction->SetForcedOnce (false);
        pAction->SetForcedPermanently (false);
        pAction->SetBlocked (false);

        if (bChanged)
        {
            OnActionAttributeChanged (pAction);
        };
    };
}


void CPrgEditorDoc::UpdateTransitions (TransitionAttribute attribute, const CArray<long, long> &transitionNumbers)
{
    POSITION pPosition;

    pPosition = m_TransitionList.GetHeadPosition ();
    while (pPosition != NULL)
    {
        CPrgEdTransition *pTransition;
        int iIndex;
        bool bOn;

        pTransition = m_TransitionList.GetNext (pPosition);
        ASSERT (pTransition != NULL);
        if (pTransition == NULL)
            continue;

        for (iIndex = 0; iIndex < transitionNumbers.GetSize (); ++iIndex)
        {
            if (pTransition->GetNumber () == transitionNumbers.GetAt (iIndex))
            {
                break;
            };
        };

        bOn = iIndex != transitionNumbers.GetSize ();

        switch (attribute)
        {
        case TransitionEnabled:
            if (bOn != pTransition->IsEnabled ())
            {
                pTransition->SetEnabled (bOn);
                OnTransitionAttributeChanged (pTransition);
            };
            break;

        case TransitionForcedOnce:
            if (bOn != pTransition->IsForcedOnce ())
            {
                pTransition->SetForcedOnce (bOn);
                OnTransitionAttributeChanged (pTransition);
            };
            break;

        case TransitionForcedPermanently:
            if (bOn != pTransition->IsForcedPermanently ())
            {
                pTransition->SetForcedPermanently (bOn);
                OnTransitionAttributeChanged (pTransition);
            };
            break;

        case TransitionBlocked:
            if (bOn != pTransition->IsBlocked ())
            {
                pTransition->SetBlocked (bOn);
                OnTransitionAttributeChanged (pTransition);
            };
            break;
        };
    };
}


CPrgEdTransition *CPrgEditorDoc::LookupTransitionByPosition (long lX, long lY)
{
    HRESULT hr;
    CComPtr<ITransitionDBI> pTransitionDBI;
    long lNumber;
    POSITION pPosition;

    pTransitionDBI = LookupTransitionDBIByPosition (lX, lY);
    if (pTransitionDBI == NULL)
    {
        return (NULL);
    };

    hr = pTransitionDBI->get_Number (&lNumber);
    if (FAILED (hr))
    {
        return (NULL);
    };

    pPosition = m_TransitionList.GetHeadPosition ();
    while (pPosition != NULL)
    {
        CPrgEdTransition *pTransition;

        pTransition = m_TransitionList.GetNext (pPosition);
        if (pTransition->GetNumber () == lNumber)
        {
            return (pTransition);
        };
    };

    return (NULL);
}


CComPtr<ITransitionDBI> CPrgEditorDoc::LookupTransitionDBIByPosition (long lX, long lY)
{
    HRESULT hr;
    CComPtr<IUnknown> pUnknown;
    CComPtr<ITransitionDBI> pTransitionDBI;

    if (m_pTypeDBI == NULL)
    {
        return (NULL);
    };

    hr = m_pTypeDBI->get_TransitionByPosition (lX, lY, &pUnknown);
    ASSERT (SUCCEEDED (hr));
    if (hr != S_OK)
    {
        return (NULL);
    };
    ASSERT (pUnknown != NULL);
    if (pUnknown == NULL)
    {
        return (NULL);
    };

    hr = pUnknown->QueryInterface (IID_ITransitionDBI, (LPVOID *)&pTransitionDBI);
    pUnknown = NULL;
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        return (NULL);
    };
    ASSERT (pTransitionDBI != NULL);

    return (pTransitionDBI);
}


void CPrgEditorDoc::ResetAllTransitions ()
{
    POSITION pPosition;

    pPosition = m_TransitionList.GetHeadPosition ();
    while (pPosition != NULL)
    {
        CPrgEdTransition *pTransition;
        bool bChanged;

        pTransition = m_TransitionList.GetNext (pPosition);
        ASSERT (pTransition != NULL);
        if (pTransition == NULL)
            continue;

        bChanged = pTransition->IsEnabled () ||
                   pTransition->IsForcedOnce () ||
                   pTransition->IsForcedPermanently () ||
                   pTransition->IsBlocked ();

        pTransition->SetEnabled (false);
        pTransition->SetForcedOnce (false);
        pTransition->SetForcedPermanently (false);
        pTransition->SetBlocked (false);

        if (bChanged)
        {
            OnTransitionAttributeChanged (pTransition);
        };
    };
}


/////////////////////////////////////////////////////////////////////////////
// embedded interface XCEPrgEditObj commands

STDMETHODIMP_(ULONG) CPrgEditorDoc::XCEPrgEditObj::AddRef ()
{
	METHOD_PROLOGUE_EX (CPrgEditorDoc, CEPrgEditObj)
    
	return (pThis->ExternalAddRef ());
}


STDMETHODIMP_(ULONG) CPrgEditorDoc::XCEPrgEditObj::Release ()
{
	METHOD_PROLOGUE_EX (CPrgEditorDoc, CEPrgEditObj)

	return (pThis->ExternalRelease ());
}


STDMETHODIMP CPrgEditorDoc::XCEPrgEditObj::QueryInterface (REFIID iid, LPVOID *ppvObj)
{
	METHOD_PROLOGUE_EX (CPrgEditorDoc, CEPrgEditObj)

	return ((HRESULT)pThis->ExternalQueryInterface (&iid, ppvObj));
}


STDMETHODIMP CPrgEditorDoc::XCEPrgEditObj::SetInstance (BSTR sInstanceName, BSTR sMachineName, BSTR sMachineAddress)
{
	METHOD_PROLOGUE_EX (CPrgEditorDoc, CEPrgEditObj)

	pThis->SetInstance (sInstanceName, sMachineName, sMachineAddress);
	return (S_OK);
}


STDMETHODIMP CPrgEditorDoc::XCEPrgEditObj::SetSymbolFile (BSTR sSymbolFile)
{
	METHOD_PROLOGUE_EX (CPrgEditorDoc, CEPrgEditObj)

	pThis->SetSymbolFile (sSymbolFile);
	return (S_OK);
}


STDMETHODIMP CPrgEditorDoc::XCEPrgEditObj::NotifyConnect (BSTR sMachineName)
{
	METHOD_PROLOGUE_EX (CPrgEditorDoc, CEPrgEditObj)

	pThis->NotifyConnect (sMachineName);
	return (S_OK);
}


STDMETHODIMP CPrgEditorDoc::XCEPrgEditObj::NotifyDisconnect (BSTR sMachineName)
{
	METHOD_PROLOGUE_EX (CPrgEditorDoc, CEPrgEditObj)

	pThis->NotifyDisconnect (sMachineName);
	return (S_OK);
}


STDMETHODIMP CPrgEditorDoc::XCEPrgEditObj::ShowError (EDP_POS_QUALIFIER posQual, long lX, long lY, long lCX, long lCY)
{
	METHOD_PROLOGUE_EX (CPrgEditorDoc, CEPrgEditObj)

	pThis->ShowError (CStdEditorPosition (posQual, lX, lY, lCX, lCY));
	return (S_OK);
}


STDMETHODIMP CPrgEditorDoc::XCEPrgEditObj::InsertFBCall (BSTR sFB)
{
	METHOD_PROLOGUE_EX (CPrgEditorDoc, CEPrgEditObj)
	HRESULT hr;

	hr = pThis->InsertFBCall (sFB);
	return (hr);
}


STDMETHODIMP CPrgEditorDoc::XCEPrgEditObj::InsertVariable (BSTR sVarName)
{
	METHOD_PROLOGUE_EX (CPrgEditorDoc, CEPrgEditObj)
	HRESULT hr;

	hr = pThis->InsertVariable (sVarName);
	return (hr);
}


STDMETHODIMP CPrgEditorDoc::XCEPrgEditObj::NotifyDebugState (short state)
{
	METHOD_PROLOGUE_EX (CPrgEditorDoc, CEPrgEditObj)

	pThis->NotifyDebugState ((CSC_SESSION_STATE)state);
	return (S_OK);
}


STDMETHODIMP CPrgEditorDoc::XCEPrgEditObj::Resubscribe ()
{
    METHOD_PROLOGUE_EX (CPrgEditorDoc, CEPrgEditObj)
    HRESULT hr;

    hr = pThis->Resubscribe ();
    return (hr);
}


/////////////////////////////////////////////////////////////////////////////
// embedded interface XCEDebugObj commands

STDMETHODIMP_(ULONG) CPrgEditorDoc::XCEDebugObj::AddRef ()
{
	METHOD_PROLOGUE_EX (CPrgEditorDoc, CEDebugObj)
    
	return (pThis->ExternalAddRef ());
}

STDMETHODIMP_(ULONG) CPrgEditorDoc::XCEDebugObj::Release ()
{
	METHOD_PROLOGUE_EX (CPrgEditorDoc, CEDebugObj)

	return (pThis->ExternalRelease ());
}

STDMETHODIMP CPrgEditorDoc::XCEDebugObj::QueryInterface (REFIID iid, LPVOID *ppvObj)
{
	METHOD_PROLOGUE_EX (CPrgEditorDoc, CEDebugObj)

	return (HRESULT)pThis->ExternalQueryInterface (&iid, ppvObj);
}

STDMETHODIMP CPrgEditorDoc::XCEDebugObj::ShowBreakpoint (/*[in]*/ EDP_POS_QUALIFIER posQual, /*[in]*/ long lX, /*[in]*/ long lY, /*[in]*/ long lCX, /*[in]*/ long lCY, short sState)
{
	METHOD_PROLOGUE_EX (CPrgEditorDoc, CEDebugObj)

	return pThis->ShowBreakpoint(CStdEditorPosition(posQual, lX, lY, lCX, lCY), sState);
}

STDMETHODIMP CPrgEditorDoc::XCEDebugObj::HideBreakpoint (/*[in]*/ EDP_POS_QUALIFIER posQual, /*[in]*/ long lX, /*[in]*/ long lY, /*[in]*/ long lCX, /*[in]*/ long lCY)
{
	METHOD_PROLOGUE_EX (CPrgEditorDoc, CEDebugObj)

	return pThis->HideBreakpoint(CStdEditorPosition(posQual, lX, lY, lCX, lCY));
}

STDMETHODIMP CPrgEditorDoc::XCEDebugObj::ShowStoppoint (/*[in]*/ EDP_POS_QUALIFIER posQual, /*[in]*/ long lX, /*[in]*/ long lY, /*[in]*/ long lCX, /*[in]*/ long lCY)
{
	METHOD_PROLOGUE_EX (CPrgEditorDoc, CEDebugObj)

	return pThis->ShowStoppoint(CStdEditorPosition(posQual, lX, lY, lCX, lCY));
}

STDMETHODIMP CPrgEditorDoc::XCEDebugObj::HideStoppoint ()
{
	METHOD_PROLOGUE_EX (CPrgEditorDoc, CEDebugObj)

	return pThis->HideStoppoint();
}

/////////////////////////////////////////////////////////////////////////////
// embedded interface XCEMonitorObj commands

STDMETHODIMP_(ULONG) CPrgEditorDoc::XCEMonitorObj::AddRef ()
{
	METHOD_PROLOGUE_EX (CPrgEditorDoc, CEMonitorObj)
    
	return (pThis->ExternalAddRef ());
}

STDMETHODIMP_(ULONG) CPrgEditorDoc::XCEMonitorObj::Release ()
{
	METHOD_PROLOGUE_EX (CPrgEditorDoc, CEMonitorObj)

	return (pThis->ExternalRelease ());
}

STDMETHODIMP CPrgEditorDoc::XCEMonitorObj::QueryInterface (REFIID iid, LPVOID *ppvObj)
{
	METHOD_PROLOGUE_EX (CPrgEditorDoc, CEMonitorObj)

	return ((HRESULT)pThis->ExternalQueryInterface (&iid, ppvObj));
}

STDMETHODIMP CPrgEditorDoc::XCEMonitorObj::AddWatchpoint (long lX, long lY, long *plHandle)
{
	METHOD_PROLOGUE_EX (CPrgEditorDoc, CEMonitorObj)

	pThis->AddWatchpoint (CStdEditorPosition (EDP_STLINE, lX, lY), true, *plHandle);
	return (S_OK);
}

STDMETHODIMP CPrgEditorDoc::XCEMonitorObj::RemoveWatchpoint (long lHandle)
{
	METHOD_PROLOGUE_EX (CPrgEditorDoc, CEMonitorObj)

	pThis->RemoveWatchpoint (lHandle);
	return (S_OK);
}

STDMETHODIMP CPrgEditorDoc::XCEMonitorObj::LoadWatchpoints (SAFEARRAY *pWPInfos)
{
	METHOD_PROLOGUE_EX (CPrgEditorDoc, CEMonitorObj)

	HRESULT hr;

	hr = pThis->LoadWatchpoints (pWPInfos);

	return (hr);
}

STDMETHODIMP CPrgEditorDoc::XCEMonitorObj::StoreWatchpoints (SAFEARRAY **ppWPInfos)
{
	METHOD_PROLOGUE_EX (CPrgEditorDoc, CEMonitorObj)

	HRESULT hr;

	hr = pThis->StoreWatchpoints (*ppWPInfos);

	return (hr);
}

STDMETHODIMP CPrgEditorDoc::XCEMonitorObj::EnableStepMonitoring (BOOL bOn)
{
	METHOD_PROLOGUE_EX (CPrgEditorDoc, CEMonitorObj)

	pThis->EnableStepMonitoring (bOn != FALSE);
	return (S_OK);
}

STDMETHODIMP CPrgEditorDoc::XCEMonitorObj::AddExpressionWatchpoint (long lStartX, long lStartY, long lEndX, long lEndY, long *plHandle)
{
	METHOD_PROLOGUE_EX (CPrgEditorDoc, CEMonitorObj)

	pThis->AddExpressionWatchpoint (CStdEditorPosition (EDP_STLINE, lStartX, lStartY), CStdEditorPosition (EDP_STLINE, lEndX, lEndY), true, *plHandle);
	return (S_OK);
}

/////////////////////////////////////////////////////////////////////////////
// embedded interface XCEWatchNotifyObj commands

STDMETHODIMP_(ULONG) CPrgEditorDoc::XCEWatchNotifyObj::AddRef ()
{
	METHOD_PROLOGUE_EX (CPrgEditorDoc, CEWatchNotifyObj)
    
	return (pThis->ExternalAddRef ());
}

STDMETHODIMP_(ULONG) CPrgEditorDoc::XCEWatchNotifyObj::Release ()
{
	METHOD_PROLOGUE_EX (CPrgEditorDoc, CEWatchNotifyObj)

	return (pThis->ExternalRelease ());
}

STDMETHODIMP CPrgEditorDoc::XCEWatchNotifyObj::QueryInterface (REFIID iid, LPVOID *ppvObj)
{
	METHOD_PROLOGUE_EX (CPrgEditorDoc, CEWatchNotifyObj)

	return ((HRESULT)pThis->ExternalQueryInterface (&iid, ppvObj));
}

STDMETHODIMP CPrgEditorDoc::XCEWatchNotifyObj::OnExpressionChanged (LONG lHandle)
{
	METHOD_PROLOGUE_EX (CPrgEditorDoc, CEWatchNotifyObj)

    pThis->OnExpressionChanged (lHandle);
    return (S_OK);
}


void CPrgEditorDoc::ForceWPDisplayPos(long X, long Y, long Xofs, long Yofs)
{
   m_bSpecialWPDisplay = true;
   m_lWPDisplayX       = X;
   m_lWPDisplayY       = Y;
   m_lWPDisplayOffsetX = Xofs;
   m_lWPDisplayOffsetY = Yofs;
}


HRESULT CPrgEditorDoc::OnCreateWatchpoint (CWatchpoint *pWP)
{
   CString strText;
   CStdEditorPosition startPosition;
   CStdEditorPosition endPosition;
   CPrgWPClientData *pClientData;
   POSITION pPosition;
   
   if (pWP->IsExpressionWP ())
   {
      // call the child class member
      strText = GetTrimmedText (pWP->GetStartPosition (), pWP->GetEndPosition (), startPosition, endPosition);
   }
   else
   {
      // call the child class member
      strText = GetSmartText (pWP->GetPosition (), startPosition, endPosition);
   };
   
   if (strText == "")
   {
      return (S_FALSE);
   };
   
   if (IsDuplicateWP (startPosition, endPosition))
   {
      return (S_FALSE);
   };
   
   pClientData = new CPrgWPClientData(strText, startPosition, endPosition);
   ASSERT (pClientData != NULL);
   if (pClientData == NULL)
   {
      return (E_OUTOFMEMORY);
   };
   
   if (m_bSpecialWPDisplay)    // use only once
   {
      pClientData->SetDisplay (m_lWPDisplayX, m_lWPDisplayY, m_lWPDisplayOffsetX, m_lWPDisplayOffsetY);
      m_bSpecialWPDisplay = false;
   };
   
   
   // Lower all existing watchpoints by one. New watchpoint
   // has Z coordinate zero, so it will be topmost.
   ////////////////////////////////////////////////////////
   
   pPosition = GetFirstWPPosition ();
   while (pPosition != NULL)
   {
      CWatchpoint *pWP2;
      CPrgWPClientData *pClientData2;
      
      pWP2 = GetNextWP (pPosition);
      ASSERT (pWP2 != NULL);
      if (pWP2 == NULL)
         continue;
      
      pClientData2 = (CPrgWPClientData *)pWP2->GetClientData ();
      ASSERT (pClientData2 != NULL);
      if (pClientData2 == NULL)
         continue;
      
      pClientData2->SetZ (pClientData2->GetZ () + 1);
   };
   
   
   pWP->SetClientData (pClientData);
   
   return (S_OK);
}



void CPrgEditorDoc::OnRestoreWatchpoint (const CString &strWPInfo)
{
   if (strWPInfo.GetLength () >= 2 && strWPInfo[0] == _T ('X') && strWPInfo[1] == _T (':'))
   {
      (void)OnRestoreExpressionWatchpoint (strWPInfo.Mid (2));
      return;
   };
   
   (void)OnRestoreSmartWatchpoint (strWPInfo);
}



HRESULT CPrgEditorDoc::OnRestoreSmartWatchpoint (const CString &strWPInfo)
{
   CString strTemp=strWPInfo;
   LPTSTR pszWPInfo;
   LPCTSTR pszToken;
   EDP_POS_QUALIFIER edp;
   long lX;
   long lY;
   long lCX;
   long lCY;
   long lDisplayX;
   long lDisplayY;
   long lDisplayOffsetX;
   long lDisplayOffsetY;
   HRESULT hr;
   
   pszWPInfo = strTemp.GetBuffer (0);
   
   pszToken = _tcstok (pszWPInfo, _T (";"));
   if (pszToken == NULL)
   {
      strTemp.ReleaseBuffer ();
      return (E_FAIL);
   };
   edp = (EDP_POS_QUALIFIER)_tcstoul (pszToken, NULL, 10);
   
   if(edp == EDP_GRLDBOX)
       return (S_OK); //EF: ignore ladder contact/coils, they are always reloaded in StartLDTracing

   pszToken = _tcstok (NULL, _T (";"));
   if (pszToken == NULL)
   {
      strTemp.ReleaseBuffer ();
      return (E_FAIL);
   };
   lX = _tcstoul (pszToken, NULL, 10);
   
   pszToken = _tcstok (NULL, _T (";"));
   if (pszToken == NULL)
   {
      strTemp.ReleaseBuffer ();
      return (E_FAIL);
   };
   lY = _tcstoul (pszToken, NULL, 10);
   
   pszToken = _tcstok (NULL, _T (";"));
   if (pszToken == NULL)
   {
      strTemp.ReleaseBuffer ();
      return (E_FAIL);
   };
   lCX = _tcstoul (pszToken, NULL, 10);
   
   pszToken = _tcstok (NULL, _T (";"));
   if (pszToken == NULL)
   {
      strTemp.ReleaseBuffer ();
      return (E_FAIL);
   };
   lCY = _tcstoul (pszToken, NULL, 10);
   
   pszToken = _tcstok (NULL, _T (";"));
   if (pszToken == NULL)
   {
      strTemp.ReleaseBuffer ();
      return (E_FAIL);
   };
   lDisplayX = _tcstoul (pszToken, NULL, 10);
   
   pszToken = _tcstok (NULL, _T (";"));
   if (pszToken == NULL)
   {
      strTemp.ReleaseBuffer ();
      return (E_FAIL);
   };
   lDisplayY = _tcstoul (pszToken, NULL, 10);
   
   pszToken = _tcstok (NULL, _T (";"));
   if (pszToken == NULL)
   {
      strTemp.ReleaseBuffer ();
      return (E_FAIL);
   };
   lDisplayOffsetX = _tcstoul (pszToken, NULL, 10);
   
   pszToken = _tcstok (NULL, _T (";"));
   if (pszToken == NULL)
   {
      strTemp.ReleaseBuffer ();
      return (E_FAIL);
   };
   lDisplayOffsetY = _tcstoul (pszToken, NULL, 10);
   
   strTemp.ReleaseBuffer ();
   
   ForceWPDisplayPos(lDisplayX,lDisplayY, lDisplayOffsetX, lDisplayOffsetY);
   hr = SetWatchpoint (CStdEditorPosition (edp, lX, lY, lCX, lCY), false);
   m_bSpecialWPDisplay = false;
   
   if (FAILED (hr))
   {
      return (hr);
   };
   
   return (S_OK);
}


HRESULT CPrgEditorDoc::OnRestoreExpressionWatchpoint (const CString &strWPInfo)
{
   CString strTemp=strWPInfo;
   LPTSTR pszWPInfo;
   LPCTSTR pszToken;
   EDP_POS_QUALIFIER edpStart;
   long lStartX;
   long lStartY;
   long lStartCX;
   long lStartCY;
   EDP_POS_QUALIFIER edpEnd;
   long lEndX;
   long lEndY;
   long lEndCX;
   long lEndCY;
   long lDisplayX;
   long lDisplayY;
   long lDisplayOffsetX;
   long lDisplayOffsetY;
   
   HRESULT hr;
   
   pszWPInfo = strTemp.GetBuffer (0);
   
   pszToken = _tcstok (pszWPInfo, _T (";"));
   if (pszToken == NULL)
   {
      strTemp.ReleaseBuffer ();
      return (E_FAIL);
   };
   edpStart = (EDP_POS_QUALIFIER)_tcstoul (pszToken, NULL, 10);
   
   pszToken = _tcstok (NULL, _T (";"));
   if (pszToken == NULL)
   {
      strTemp.ReleaseBuffer ();
      return (E_FAIL);
   };
   lStartX = _tcstoul (pszToken, NULL, 10);
   
   pszToken = _tcstok (NULL, _T (";"));
   if (pszToken == NULL)
   {
      strTemp.ReleaseBuffer ();
      return (E_FAIL);
   };
   lStartY = _tcstoul (pszToken, NULL, 10);
   
   pszToken = _tcstok (NULL, _T (";"));
   if (pszToken == NULL)
   {
      strTemp.ReleaseBuffer ();
      return (E_FAIL);
   };
   lStartCX = _tcstoul (pszToken, NULL, 10);
   
   pszToken = _tcstok (NULL, _T (";"));
   if (pszToken == NULL)
   {
      strTemp.ReleaseBuffer ();
      return (E_FAIL);
   };
   lStartCY = _tcstoul (pszToken, NULL, 10);
   
   pszToken = _tcstok (NULL, _T (";"));
   if (pszToken == NULL)
   {
      strTemp.ReleaseBuffer ();
      return (E_FAIL);
   };
   edpEnd = (EDP_POS_QUALIFIER)_tcstoul (pszToken, NULL, 10);
   
   pszToken = _tcstok (NULL, _T (";"));
   if (pszToken == NULL)
   {
      strTemp.ReleaseBuffer ();
      return (E_FAIL);
   };
   lEndX = _tcstoul (pszToken, NULL, 10);
   
   pszToken = _tcstok (NULL, _T (";"));
   if (pszToken == NULL)
   {
      strTemp.ReleaseBuffer ();
      return (E_FAIL);
   };
   lEndY = _tcstoul (pszToken, NULL, 10);
   
   pszToken = _tcstok (NULL, _T (";"));
   if (pszToken == NULL)
   {
      strTemp.ReleaseBuffer ();
      return (E_FAIL);
   };
   lEndCX = _tcstoul (pszToken, NULL, 10);
   
   pszToken = _tcstok (NULL, _T (";"));
   if (pszToken == NULL)
   {
      strTemp.ReleaseBuffer ();
      return (E_FAIL);
   };
   lEndCY = _tcstoul (pszToken, NULL, 10);
   
   pszToken = _tcstok (NULL, _T (";"));
   if (pszToken == NULL)
   {
      strTemp.ReleaseBuffer ();
      return (E_FAIL);
   };
   lDisplayX = _tcstoul (pszToken, NULL, 10);
   
   pszToken = _tcstok (NULL, _T (";"));
   if (pszToken == NULL)
   {
      strTemp.ReleaseBuffer ();
      return (E_FAIL);
   };
   lDisplayY = _tcstoul (pszToken, NULL, 10);
   
   pszToken = _tcstok (NULL, _T (";"));
   if (pszToken == NULL)
   {
      strTemp.ReleaseBuffer ();
      return (E_FAIL);
   };
   lDisplayOffsetX = _tcstoul (pszToken, NULL, 10);
   
   pszToken = _tcstok (NULL, _T (";"));
   if (pszToken == NULL)
   {
      strTemp.ReleaseBuffer ();
      return (E_FAIL);
   };
   lDisplayOffsetY = _tcstoul (pszToken, NULL, 10);
   
   strTemp.ReleaseBuffer ();

   ForceWPDisplayPos(lDisplayX,lDisplayY, lDisplayOffsetX, lDisplayOffsetY);
   hr = SetExpressionWatchpoint (CStdEditorPosition (edpStart, lStartX, lStartY, lStartCX, lStartCY), 
                                 CStdEditorPosition (edpEnd, lEndX, lEndY, lEndCX, lEndCY), false);
   m_bSpecialWPDisplay = false;
   
   if (FAILED (hr))
   {
      return (hr);
   };
   
    return (S_OK);
}


CString CPrgEditorDoc::GetSmartText (const CStdEditorPosition &position, CStdEditorPosition &startPosition, CStdEditorPosition &endPosition)
{
   ASSERT(0&&"override in derived classes"); 
   return _T("");
}


CString CPrgEditorDoc::GetTrimmedText (const CStdEditorPosition &startPosition, const CStdEditorPosition &endPosition, CStdEditorPosition &trimmedStart, CStdEditorPosition &trimmedEnd)
{
   ASSERT(0&&"override in derived classes"); 
   return _T("");
}


bool CPrgEditorDoc::IsDuplicateWP (const CStdEditorPosition &startPosition, const CStdEditorPosition &endPosition)
{
   POSITION pPosition;
   
   pPosition = m_WatchpointList.GetHeadPosition ();
   while (pPosition != NULL)
   {
      CWatchpoint *pWP;
      CPrgWPClientData *pClientData;
      
      pWP = m_WatchpointList.GetNext (pPosition);
      assert (pWP != NULL);
      if (pWP == NULL)
         continue;
      
      pClientData = (CPrgWPClientData *) (pWP->GetClientData ());
      assert (pClientData != NULL);
      if (pClientData == NULL)
         continue;
      
      if (startPosition == pClientData->GetStartPosition () &&
          endPosition   == pClientData->GetEndPosition ())
      {
         return (true);
      }
   }
   
   return (false);
}


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
bool CPrgEditorDoc::RaiseWatchpoint (CWatchpoint *pWP)
{
   POSITION pPosition;
   CPrgWPClientData *pClientData;
   
   pClientData = (CPrgWPClientData *)pWP->GetClientData ();
   ASSERT (pClientData != NULL);
   if (pClientData == NULL)
   {
      return (false);
   }
   
   if (pClientData->GetZ () == 0)
   {
      return (false);
   }
   
   pPosition = GetFirstWPPosition ();
   while (pPosition != NULL)
   {
      CWatchpoint *pWP2;
      CPrgWPClientData *pClientData2;
      
      pWP2 = GetNextWP (pPosition);
      ASSERT (pWP2 != NULL);
      if (pWP2 == NULL)
         continue;
      
      pClientData2 = (CPrgWPClientData *)pWP2->GetClientData ();
      ASSERT (pClientData2 != NULL);
      if (pClientData2 == NULL)
         continue;
      
      if (pClientData2->GetZ () < pClientData->GetZ ())
      {
         pClientData2->SetZ (pClientData2->GetZ () + 1);
      };
   };
   
   pClientData->SetZ (0);
   
   return (true);
}



/*func*------------------------------------------------------------------------
description:
in :
out:
-----------------------------------------------------------------------------*/
void CPrgEditorDoc::ZOrderWatchpoints (CArray<CWatchpoint *, CWatchpoint *> &zOrdered)
{
   POSITION pPosition;
   
   zOrdered.RemoveAll ();
   
   pPosition = m_WatchpointList.GetHeadPosition ();
   while (pPosition != NULL)
   {
      CWatchpoint *pWP;
      CPrgWPClientData *pClientData;
      int iIndex;
      
      pWP = m_WatchpointList.GetNext (pPosition);
      ASSERT (pWP != NULL);
      if (pWP == NULL)
         continue;
      
      pClientData = (CPrgWPClientData *)pWP->GetClientData ();
      ASSERT (pClientData != NULL);
      if (pClientData == NULL)
         continue;
      
      for (iIndex = 0; iIndex < zOrdered.GetSize (); ++iIndex)
      {
         CWatchpoint *pWP2;
         CPrgWPClientData *pClientData2;
         
         pWP2 = zOrdered[iIndex];
         pClientData2 = (CPrgWPClientData *)pWP2->GetClientData ();
         
         if (pClientData->GetZ () < pClientData2->GetZ ())
            break;
      };
      
      zOrdered.InsertAt (iIndex, pWP);
   };
}


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CPrgEditorDoc::OnDestroyWatchpoint (CWatchpoint *pWP)
{
    CPrgWPClientData *pClientData;
    POSITION pPosition;
    
    pClientData = (CPrgWPClientData *)pWP->GetClientData ();
    
    pPosition = GetFirstWPPosition ();
    while (pPosition != NULL)
    {
        CWatchpoint *pWP2;
        CPrgWPClientData *pClientData2;
        
        pWP2 = GetNextWP (pPosition);
        ASSERT (pWP2 != NULL);
        
        if (pWP2 == NULL)
        {
            continue;
        }

        pClientData2 = (CPrgWPClientData *)pWP2->GetClientData ();
        ASSERT (pClientData2 != NULL);
        
        if (pClientData2 == NULL)
        {
            continue;
        }

        if (pClientData2->GetZ () > pClientData->GetZ ())
        {
            pClientData2->SetZ (pClientData2->GetZ () - 1);
        };
    };
    
    delete pClientData;
}


HRESULT CPrgEditorDoc::OnConnectWatchpoint (CWatchpoint *pWP, bool bFailIfBad)
{
	HRESULT hr;
	CPrgWPClientData *pClientData;
	CString strVariableName;
    LONG lHandle;
    BL_IEC_TYP iecType;
    CEExpressionState state;

	pClientData = (CPrgWPClientData *)pWP->GetClientData ();
	assert (pClientData != NULL);
	if (pClientData == NULL)
	{
		return (E_FAIL);
	};


    // Add watch item. If adding fails do not continue.

    hr = AddWatchItem (pClientData->GetVariableName (), lHandle);
    if (FAILED (hr))
    {
        return (hr);
    }


    // If watch item type is known and not a simple type do not continue.

    iecType = BL_ANY;
    if (!IsUnknownWatchItemType (lHandle) && !IsSimpleWatchItemType (lHandle, iecType))
    {
        if (bFailIfBad)
        {
            RemoveWatchItem (lHandle);
            return (E_FAIL);
        }
    }


    // Connect watch item if watchpoint is not suspended.
    // If connecting fails do not continue.

    if (!pWP->IsSuspended ())
    {
        // Connect watch item. If connecting fails do not continue.
        // If connecting fails do not continue.
        hr = ConnectWatchItem (lHandle);
        if (FAILED (hr))
        {
            (void)RemoveWatchItem (lHandle);
            return (E_FAIL);
        }
    }


    // Get state of watch item. If the item is unknown in debug info
    // or unknown at the control do not continue. Do not revert re-
    // gistering the watchpoint at watch backend if watch backend tells
    // you the value is empty. This will happen in total correctness if
    // the watchpoint is suspended and thus the watch item not being
    // connected a few lines above.

    state = GetWatchExpressionState (lHandle);
    if (state == CEUnknownSubscription || state == CEUnknownField)
    {
        if (bFailIfBad)
        {
            if (!pWP->IsSuspended ())
            {
                (void)DisconnectWatchItem (lHandle);
            };

            (void)RemoveWatchItem (lHandle);
            return (E_FAIL);
        };
    };

    pWP->AttachWatchItem (lHandle, iecType);

	return (S_OK);
}


void CPrgEditorDoc::OnDisconnectWatchpoint (CWatchpoint *pWP)
{
   LONG lHandle;
   
   lHandle = pWP->DetachWatchItem ();
   if (!pWP->IsSuspended())
   {
      (void)DisconnectWatchItem (lHandle);
   }
   (void)RemoveWatchItem (lHandle);
}



CString CPrgEditorDoc::OnStoreWatchpoint (CWatchpoint *pWP)
{
   CString strWPInfo;
   CPrgWPClientData *pClientData;
   long lDisplayX;
   long lDisplayY;
   long lDisplayOffsetX;
   long lDisplayOffsetY;
   
   pClientData = (CPrgWPClientData *)pWP->GetClientData ();
   assert (pClientData != NULL);
   if (pClientData == NULL)
   {
      FC_DebugPrint ("STEDITOR> No client data for watchpoint\n");
      return ("");
   };
   
   pClientData->GetDisplay (lDisplayX, lDisplayY, lDisplayOffsetX, lDisplayOffsetY);
   
   if (pWP->IsExpressionWP ())
   {
      strWPInfo.Format (_T ("X:%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d"),
         pWP->GetStartPosition ().GetPosQual (),
         pWP->GetStartPosition ().GetX (),
         pWP->GetStartPosition ().GetY (),
         pWP->GetStartPosition ().GetCX (),
         pWP->GetStartPosition ().GetCY (),
         pWP->GetEndPosition ().GetPosQual (),
         pWP->GetEndPosition ().GetX (),
         pWP->GetEndPosition ().GetY (),
         pWP->GetEndPosition ().GetCX (),
         pWP->GetEndPosition ().GetCY (),
         lDisplayX,
         lDisplayY,
         lDisplayOffsetX,
         lDisplayOffsetY
         );
   }
   else
   {
      strWPInfo.Format (_T ("%d;%d;%d;%d;%d;%d;%d;%d;%d"),
         pWP->GetPosition ().GetPosQual(),
         pWP->GetPosition ().GetX (),
         pWP->GetPosition ().GetY (),
         pWP->GetPosition ().GetCX (),
         pWP->GetPosition ().GetCY (),
         lDisplayX,
         lDisplayY,
         lDisplayOffsetX,
         lDisplayOffsetY
         );
   };
   
   return (strWPInfo);
}



void CPrgEditorDoc::OnViewObjHelpPosition (const CStdEditorPosition &position)
{
    CString strText;
    CStdEditorPosition startPos;
    CStdEditorPosition endPos;
    CString parentName;

    strText = GetSmartText (position, startPos, endPos);

    if (strText == "")
    {
        return;
    };

    parentName = m_strBaseFile; //GetDocumentFile();
    int     lbs;
    lbs = parentName.ReverseFind(_T('\\'))+1;
    parentName = parentName.Mid(lbs, parentName.GetLength() - lbs);

    strText = strText + _T("|") + parentName;

    // TEST CODE --> remove
    //CString str = _T("HALLO");
    //CString str2;
    //CList<CString, CString&> list;

    //GetLRUPous(list);
    //GetPouXML(str, str2);

    //HRESULT GetLRUPous(CList<CString, CString&>& listPous);
    //HRESULT GetPouXML(const CString& strPouName, CString& strPouDef);
    //HRESULT GetVarList(CList<CString, CString&>& listVars);
    //HRESULT GetPouList(CList<CString, CString&>& listPous);
    //HRESULT GetPouDefinition(const CString& strPouName, CString& strPouDef);
    //HRESULT GetPouMember(const CString& strPouName, CList<CString, CString&>& listMembers);


    ShowUserHelp(strText);
}


void CPrgEditorDoc::OnViewObjHelpSelection (const CStdEditorPosition &startPosition,
                                            const CStdEditorPosition &endPosition)
{
    CString strText;
    CStdEditorPosition startPos;
    CStdEditorPosition endPos;
    CString parentName;

    strText = GetTrimmedText (startPosition, endPosition, startPos, endPos);

    if (strText == "")
    {
        return;
    };

    parentName = m_strBaseFile; //GetDocumentFile();
    int     lbs;
    lbs = parentName.ReverseFind(_T('\\'))+1;
    parentName = parentName.Mid(lbs, parentName.GetLength() - lbs);

    strText = strText + _T("|") + parentName;

    ShowUserHelp(strText);
}

CString CPrgEditorDoc::GetContextStringPosition(const CStdEditorPosition &position)
{
    CString strText;
    CStdEditorPosition startPos;
    CStdEditorPosition endPos;

    strText = GetSmartText (position, startPos, endPos);

    return strText;
}

CString CPrgEditorDoc::GetContextStringSelection(const CStdEditorPosition &startPosition,
                                  const CStdEditorPosition &endPosition)
{
    CString strText;
    CStdEditorPosition startPos;
    CStdEditorPosition endPos;

    strText = GetTrimmedText (startPosition, endPosition, startPos, endPos);

    return strText;
}

//bool CPrgEditorDoc::HadReadOnlyWarning()
//{ 
//   return m_bHadReadOnlyWarning; 
//}


//void CPrgEditorDoc::HadReadOnlyWarning(bool b) 
//{ 
//   m_bHadReadOnlyWarning= b;    
//}


void CPrgEditorDoc::SetLastActiveView(CView* pView)
{
   if(m_pLastActiveView != pView)
   {
      if(m_pLastActiveView!=NULL)
      {
         UpdateAllViews (NULL, ViewDeactivatedHint, m_pLastActiveView);
      }
      m_pLastActiveView = pView;
      UpdateAllViews (pView, ViewActivatedHint, pView);
   }
}


CView* CPrgEditorDoc::GetLastActiveView()
{
   return m_pLastActiveView;
}



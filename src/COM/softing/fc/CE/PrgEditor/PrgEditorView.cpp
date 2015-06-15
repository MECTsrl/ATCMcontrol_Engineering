#include "stdafx.h"
#include "PrgEditorIF.h"
#include "PrgEditorView.h"
#include "PrgEditorDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CCEDataTip

CCEDataTip::CCEDataTip ()
: m_pPopupCB (NULL),
  m_pTextRequestCB (NULL),
  m_pActionQueryCB (NULL),
  m_bCreated (false),
  m_bActive (false),
  m_pTool (NULL),
  m_bStarted (false),
  m_bDisplayed (false),
  m_strText ("some data tip")
{
//    TRACE("CEDataTip::CEDataTip()\n");
}


CCEDataTip::~CCEDataTip ()
{
    Destroy ();
}


void CCEDataTip::SetPopupCB (FC_CALLBACK_BASE (DataTipPopupCB) *pPopupCB)
{
    m_pPopupCB = pPopupCB;
}


void CCEDataTip::SetTextRequestCB (FC_CALLBACK_BASE (DataTipTextRequestCB) *pTextRequestCB)
{
    m_pTextRequestCB = pTextRequestCB;
}


void CCEDataTip::SetActionQueryCB (FC_CALLBACK_BASE (DataTipActionQueryCB) *pActionQueryCB)
{
    m_pActionQueryCB = pActionQueryCB;
}


HRESULT CCEDataTip::Create (CWnd *pParent, DWORD dwStyle, CWnd *pTool)
{
    if (m_bCreated)
    {
        return (S_FALSE);
    }

    m_pTool = pTool;

    if (!m_ToolTip.Create (pParent, dwStyle))
    {
        return (E_FAIL);
    }

    m_bCreated = true;

//    TRACE("CEDataTip::Create()\n");

    m_ToolTip.SendMessage (TTM_SETMAXTIPWIDTH, 0, SHRT_MAX);
    m_ToolTip.SendMessage (TTM_SETDELAYTIME, TTDT_AUTOPOP, SHRT_MAX);
    m_ToolTip.SendMessage (TTM_SETDELAYTIME, TTDT_INITIAL, MAKELONG(200,0));
    m_ToolTip.SendMessage (TTM_SETDELAYTIME, TTDT_RESHOW, MAKELONG(200,0));

    return (S_OK);
}


HRESULT CCEDataTip::Destroy ()
{
//    TRACE("CEDataTip::Destroy()\n");

    if (!m_bCreated)
    {
//        TRACE("CEDataTip::Destroy() -> Error 1\n");
        return (S_FALSE);
    }

    m_bCreated = false;

    if (!m_ToolTip.DestroyWindow ())
    {
//        TRACE("CEDataTip::Destroy() -> Error 2\n");
        return (E_FAIL);
    }

    return (S_OK);
}


bool CCEDataTip::IsCreated ()
{
    return (m_bCreated);
}


HRESULT CCEDataTip::Activate ()
{
//    TRACE("CEDataTip::Activate()\n");
    if (!m_bCreated)
    {
//        TRACE("CEDataTip::Activate() -> Error 1\n");
        return (E_FAIL);
    }

    if (m_bActive)
    {
//        TRACE("CEDataTip::Activate() -> Already activated\n");
        return (S_FALSE);
    }

    m_ToolTip.Activate (TRUE);
    m_bActive = true;

    return (S_OK);
}


HRESULT CCEDataTip::Deactivate ()
{
//    TRACE("CEDataTip::Deactivate()\n");
    if (!m_bCreated)
    {
//        TRACE("CEDataTip::Deactivate() -> Error 1\n");
        return (E_FAIL);
    }

    if (!m_bActive)
    {
//        TRACE("CEDataTip::Deactivate() -> Already deactiated\n");
        return (S_FALSE);
    }

    if (m_bStarted)
    {
        m_bStarted = false;

        ASSERT (m_pTool != NULL);
        if (m_pTool != NULL)
        {
//            TRACE("CEDataTip::Deactivate() -> DelTool()\n");
            m_ToolTip.DelTool (m_pTool);
        }
    }

    m_ToolTip.Activate (FALSE);
    m_bActive = false;

    return (S_OK);
}


HRESULT CCEDataTip::SetText (const CString &strText)
{
//    TRACE("CEDataTip::SetText(): \"%s\"\n", strText);
    m_strText = strText;

    if (!m_bCreated || !m_bActive || !m_bStarted)
    {
//        TRACE("CEDataTip::SetText() -> Error 1\n");
        return (S_FALSE);
    }

    ASSERT (m_pTool != NULL);
    if (m_pTool == NULL)
    {
//        TRACE("CEDataTip::SetText() -> Error 2\n");
        return (E_UNEXPECTED);
    }

    if (strText.IsEmpty ())
    {
//        TRACE("CEDataTip::SetText() -> Clear Text\n");
        m_ToolTip.UpdateTipText (" ", m_pTool);
    }
    else
    {
//        TRACE("CEDataTip::SetText() -> UpdateText\n");
        m_ToolTip.UpdateTipText (strText, m_pTool);
    }
    m_ToolTip.Update();

    return (S_OK);
}


void CCEDataTip::RelayMessage (LPMSG pMessage)
{
    CCEDataTipAction action;

//    TRACE("CEDataTip::RelayMessage(): pMessage->message = %d\n", pMessage->message);
    if (!m_bCreated && !m_bActive)
        return;

    if (pMessage->message == WM_MOUSEMOVE)
    {
        POINTS points;

        points = MAKEPOINTS (pMessage->lParam);
        m_pActionQueryCB->Execute (CPoint (points.x, points.y), action);

        if (action == CCEHideDataTip)
        {
            Hide ();
        }
        else if (action == CCEFollowDataTip)
        {
            Follow (false);
        }
        else if (action == CCERefreshDataTip)
        {
            Follow (true);
        }
        else if (action == CCEShowDataTip)
        {
            Show ();
        }
    }

    m_ToolTip.RelayEvent (pMessage);
}


HRESULT CCEDataTip::Hide ()
{
//    TRACE("CEDataTip::Hide()\n");
    if (!m_bStarted)
    {
//        TRACE("CEDataTip::Hide() -> Error 1\n");
        return (S_FALSE);
    }

    m_bStarted = false;

    m_bDisplayed = false;

    ASSERT (m_pTool != NULL);
    if (m_pTool == NULL)
    {
//        TRACE("CEDataTip::Hide() -> Error 2\n");
        return (E_UNEXPECTED);
    }

    m_ToolTip.UpdateTipText ("", m_pTool);

    m_ToolTip.DelTool (m_pTool);

    return (S_OK);
}


HRESULT CCEDataTip::Show ()
{
    HRESULT hr;
//    TRACE("CEDataTip::Show()\n");

    hr = Hide ();

    if (FAILED (hr))
    {
//        TRACE("CEDataTip::Show() -> Error 1\n");
        return (hr);
    }

    m_ToolTip.AddTool (m_pTool);

    m_bStarted = true;

    return (S_OK);
}


HRESULT CCEDataTip::Follow (bool bRefresh)
{
//    TRACE("CEDataTip::Follow(%d)\n", bRefresh);
    if (!m_bStarted || !m_bDisplayed)
    {
//        TRACE("CEDataTip::Follow() -> Error 1\n");
        return (S_FALSE);
    }

    if (bRefresh && m_pTextRequestCB != NULL)
    {
        m_pTextRequestCB->Execute (m_strText);
    }

    SetText (m_strText);

    return (S_OK);
}


bool CCEDataTip::OnPopup (UINT /*id*/, NMHDR *pTTTStruct, LRESULT * /*pResult*/)
{
    TOOLTIPTEXT *pTTT=(TOOLTIPTEXT *)pTTTStruct;    

//    TRACE("CEDataTip::OnPopup()\n");
    if (pTTT == NULL)
    {
//        TRACE("CEDataTip::OnPopup() -> Error 1\n");
        return (false);
    }

    pTTT->hinst = NULL;
    pTTT->szText[0] = _T ('\0');
    pTTT->lpszText = pTTT->szText;

    if (!(pTTT->uFlags & TTF_IDISHWND))
    {
//        TRACE("CEDataTip::OnPopup() -> Error 2\n");
        return (false);
    }

    if (!m_bStarted)
    {
//        TRACE("CEDataTip::OnPopup() -> Error 3\n");
        return (false);
    }

    if (m_pPopupCB != NULL)
    {
        bool bAllow=true;

        m_pPopupCB->Execute (bAllow);

        if (!bAllow)
        {
            if (m_pTool != NULL)
            {
                m_ToolTip.UpdateTipText ("", m_pTool);
            }

//            TRACE("CEDataTip::OnPopup() -> UpdateTipText\n");
            return (false);
        }
    }

    if (m_pTextRequestCB != NULL)
    {
        m_pTextRequestCB->Execute (m_strText);
    }

    if(m_strText.IsEmpty())
    {
//       TRACE("CEDataTip::OnPopup() -> Error: Text is empty\n");
       return false;
    }

    SetText (m_strText);

    m_bDisplayed = true;

    m_ToolTip.SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);

    return (true);
}

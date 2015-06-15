

#include "stdafx.h"
#include "cewatchview.h"
#include "CeWatchList.h"
#include "CeWatchEdit.h"
#include "CeWatchElement.h"
#include "resource.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCeWatchEdit

CCeWatchEdit::CCeWatchEdit(CCeWatchEditContainer* pContainer)
{
	m_pContainer = pContainer;

    m_hAccelerators = ::LoadAccelerators (AfxGetResourceHandle (), MAKEINTRESOURCE (IDR_EDIT_ACCELERATORS));
}

CCeWatchEdit::~CCeWatchEdit()
{
}

BEGIN_MESSAGE_MAP(CCeWatchEdit, CEdit)
	//{{AFX_MSG_MAP(CCeWatchEdit)
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnKillFocus)
	ON_WM_KEYDOWN()
    ON_COMMAND(ID_EDIT_COPY, OnCopy)
    ON_COMMAND(ID_EDIT_CUT, OnCut)
    ON_COMMAND(ID_EDIT_PASTE, OnPaste)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCeWatchEdit message handlers

void CCeWatchEdit::OnKillFocus() 
{
	if (!m_pContainer->m_bIgnoreFocus)
		GetParent()->PostMessage(WM_CEWATCH_ENDEDIT, IDCANCEL, NULL);
}

void CCeWatchEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_RETURN)
		GetParent()->PostMessage(WM_CEWATCH_ENDEDIT, IDOK, NULL);
	else if (nChar == VK_ESCAPE)
		GetParent()->PostMessage(WM_CEWATCH_ENDEDIT, IDCANCEL, NULL);
	else
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CCeWatchEdit::OnCopy ()
{
    SendMessage (WM_COPY);
}

void CCeWatchEdit::OnCut ()
{
    SendMessage (WM_CUT);
}

void CCeWatchEdit::OnPaste ()
{
    SendMessage (WM_PASTE);
}

BOOL CCeWatchEdit::PreTranslateMessage (MSG *pMsg)
{
    if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
    {
        if (::TranslateAccelerator (m_hWnd, m_hAccelerators, pMsg))
            return (TRUE);
    }

    return (CEdit::PreTranslateMessage (pMsg));
}

/////////////////////////////////////////////////////////////////////////////
// CCeWatchEditCombo

CCeWatchEditCombo::CCeWatchEditCombo(CCeWatchEditContainer* pContainer)
{
	m_pContainer = pContainer;

    m_hAccelerators = ::LoadAccelerators (AfxGetResourceHandle (), MAKEINTRESOURCE (IDR_EDIT_ACCELERATORS));
}


CCeWatchEditCombo::~CCeWatchEditCombo()
{
}

bool CCeWatchEditCombo::Load(const CString& strDataType)
{
	bool rv = false;
	if (strDataType.GetLength() > 0)
	{
		CString strFormatFile = m_pContainer->GetFormatFile();
		if (strFormatFile.GetLength() > 0)
		{
			CStdioFile file;
			if (file.Open (strFormatFile, CFile::modeRead))
			{
				CString str, strBlockType;
				while (file.ReadString(str))
				{
					str.TrimLeft();
					str.TrimRight();
					if (str.GetLength() > 0 && str.GetAt(0) != ';') // skip comments
					{
						if (str.GetAt(0) == ':')
						{
							strBlockType = str.Mid(1);
						}
						else
						{
							if (strDataType.GetLength() == 0 || strDataType == strBlockType)
							{
								AddString(str);
								rv = true;
							}
						}
					}
				}
			}
		}
	}
	return rv;
}


BEGIN_MESSAGE_MAP(CCeWatchEditCombo, CComboBox)
	//{{AFX_MSG_MAP(CCeWatchEditCombo)
	ON_WM_KEYDOWN()
	ON_CONTROL_REFLECT(CBN_KILLFOCUS, OnKillfocus)
	ON_CONTROL_REFLECT(CBN_SELENDOK, OnSelendok)
    ON_COMMAND(ID_EDIT_COPY, OnCopy)
    ON_COMMAND(ID_EDIT_CUT, OnCut)
    ON_COMMAND(ID_EDIT_PASTE, OnPaste)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCeWatchEditCombo message handlers

void CCeWatchEditCombo::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_RETURN)
		GetParent()->PostMessage(WM_CEWATCH_ENDEDIT, IDOK, NULL);
	else if (nChar == VK_ESCAPE)
		GetParent()->PostMessage(WM_CEWATCH_ENDEDIT, IDCANCEL, NULL);
	else
		CComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CCeWatchEditCombo::OnKillfocus() 
{
	if (!m_pContainer->m_bIgnoreFocus)
		GetParent()->PostMessage(WM_CEWATCH_ENDEDIT, IDCANCEL, NULL);
}

BOOL CCeWatchEditCombo::PreTranslateMessage (MSG *pMsg)
{
    if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
    {
        if (::TranslateAccelerator (m_hWnd, m_hAccelerators, pMsg))
            return (TRUE);
    }

    return (CComboBox::PreTranslateMessage (pMsg));
}

void CCeWatchEditCombo::OnSelendok() 
{
    GetParent()->PostMessage(WM_CEWATCH_ENDEDIT, IDOK, NULL);	
}

void CCeWatchEditCombo::OnCopy ()
{
    SendMessage (WM_COPY);
}

void CCeWatchEditCombo::OnCut ()
{
    SendMessage (WM_CUT);
}

void CCeWatchEditCombo::OnPaste ()
{
    SendMessage (WM_PASTE);
}



/////////////////////////////////////////////////////////////////////////////
// CCeWatchEditContainer

CCeWatchEditContainer::CCeWatchEditContainer(int iEditItem, unsigned nEditType, CCeDisplayElement*  pDisplayElement)
{
	ASSERT(nEditType >= CEWATCH_EDITTYPE_NAME && nEditType <= CEWATCH_EDITTYPE_FORMAT);
	ASSERT(IsValidDisplayElement(pDisplayElement)); 

	m_iEditItem = iEditItem;
	m_nEditType = nEditType;
	m_bIgnoreFocus = false;
	m_pEditWnd = NULL;
	m_pEditCbWnd = NULL;
	m_pDisplayElement = pDisplayElement;
}

CCeWatchEditContainer::~CCeWatchEditContainer()
{
	if (m_pEditWnd != NULL)
		delete m_pEditWnd;

	if (m_pEditCbWnd != NULL)
		delete m_pEditCbWnd;
}

bool CCeWatchEditContainer::IsVisible()
{
	if (m_pEditWnd != NULL && m_pEditWnd->IsWindowVisible())
		return true;
	if (m_pEditCbWnd != NULL && m_pEditCbWnd->IsWindowVisible())
		return true;
	return false;
}

void CCeWatchEditContainer::GetText(CString& str)
{
	if (m_pEditWnd != NULL)
		m_pEditWnd->GetWindowText(str);
	else if (m_pEditCbWnd != NULL)
		m_pEditCbWnd->GetWindowText(str);

	if (GetEditType() == CEWATCH_EDITTYPE_FORMAT)
	{
		int iIndex = str.Find(';');
		if (iIndex > 0)
		{
			str = str.Left(iIndex);
			str.TrimRight();
		}
	}
}

BOOL CCeWatchEditContainer::Create(const RECT& rect, CCeWatchList *pListCtrl, UINT nId)
{
	BOOL rv = FALSE;
	ASSERT(IsValidDisplayElement(m_pDisplayElement));
	CCeWatchElement* pVar = m_pDisplayElement->GetWatchElement();

	CString strName;
	switch (m_nEditType)
	{
	case CEWATCH_EDITTYPE_NAME :
		
		strName = m_pDisplayElement->GetWatchElement()->GetName();
		if (pVar->GetLevel() > 1)
		{
			int ii = strName.ReverseFind('.');
			if (ii > 0 && ii < strName.GetLength()-1)
				strName = strName.Mid(ii+1);
		}

		if (pVar->GetType().IsArray())
			rv = CreateEditWnd(rect, pListCtrl, nId, strName + ", [" + m_pDisplayElement->GetWatchIndices().AsString() + "]");
		else
			rv = CreateEditWnd(rect, pListCtrl, nId, strName);
		break;

	case CEWATCH_EDITTYPE_VALUE :
		if ( pVar->GetType().GetIECType() != BL_BOOL && pVar->GetHistoryCount() > 0)
		{
			rv = CreateComboBox(rect, pListCtrl, nId, pVar->GetValue());
			if (rv)
			{
				for (int ii = 0; ii < pVar->GetHistoryCount(); ii++)
					m_pEditCbWnd->AddString(pVar->GetHistory(ii));
			}
		}
		else
			rv = CreateEditWnd(rect, pListCtrl, nId, pVar->GetValue());
		break;
	
	case CEWATCH_EDITTYPE_FORMAT :
		if (ExistFormatFile())
		{
			rv = CreateComboBox(rect, pListCtrl, nId, pVar->GetFormat());
			if (rv)
				m_pEditCbWnd->Load(pVar->GetType().GetName());
		}
		else
			rv = CreateEditWnd(rect, pListCtrl, nId, pVar->GetFormat());
		break;
	}
	return rv;
}

BOOL CCeWatchEditContainer::CreateComboBox(const RECT& rect, CCeWatchList *pListCtrl, UINT nId, const CString& strText)
{
	ASSERT(m_pEditCbWnd == NULL);
	CRect r = rect;
	r.bottom = r.top + 8 * r.Height();
	m_pEditCbWnd = new CCeWatchEditCombo(this);

	BOOL rv = m_pEditCbWnd->Create(
		WS_VISIBLE | WS_BORDER | CBS_DROPDOWN | CBS_AUTOHSCROLL | WS_VSCROLL, 
		r, pListCtrl, nId);

	m_pEditCbWnd->SetFont(pListCtrl->GetFont());
	m_pEditCbWnd->SetWindowText(strText);
	m_pEditCbWnd->SetFocus();
	return rv;
}

BOOL CCeWatchEditContainer::CreateEditWnd(const RECT& rect, CCeWatchList *pListCtrl, UINT nId, const CString& strText)
{
	ASSERT(m_pEditWnd == NULL);
	m_pEditWnd = new CCeWatchEdit(this);
	BOOL rv = m_pEditWnd->Create(
		WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 
		rect, pListCtrl, nId);
	m_pEditWnd->SetFont(pListCtrl->GetFont());
	m_pEditWnd->SetWindowText(strText);
	m_pEditWnd->SetFocus();
	return rv;
}

CString CCeWatchEditContainer::GetFormatFile()
{
	CString strFormatFile;
	GetModuleFileName(NULL, strFormatFile.GetBuffer(512), 512);
	strFormatFile.ReleaseBuffer();

	int iIndex = strFormatFile.ReverseFind('/');
	if (iIndex < 0)
		iIndex = strFormatFile.ReverseFind('\\');

	if (iIndex > 0)
		strFormatFile = strFormatFile.Left(iIndex) + "/WatchFormats.4cw";

	return strFormatFile;
}

bool CCeWatchEditContainer::ExistFormatFile()
{
	CString strFormatFile = GetFormatFile();
	if (strFormatFile.GetLength() > 0)
	{
		CStdioFile file;
		if (file.Open (strFormatFile, CFile::modeRead))
		{
			file.Close();
			return true;
		}
	}
	return false;
}




//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "CEEdit.h"
#include "CEUTILrc.h"

//----  Static Initializations:   ----------------------------------*

IMPLEMENT_DYNCREATE(CCEEdit, CEdit )

BEGIN_MESSAGE_MAP(CCEEdit, CEdit)
	//{{AFX_MSG_MAP(CCEEdit)
    ON_COMMAND(ID_CEEDIT_COPY, OnEditCopy)
    ON_COMMAND(ID_CEEDIT_CUT, OnEditCut)
    ON_COMMAND(ID_CEEDIT_PASTE, OnEditPaste)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCEEdit::CCEEdit()
{
    // accelerator table
    HINSTANCE hInstance;
    hInstance = AfxFindResourceHandle (MAKEINTRESOURCE (IDR_CECTRL_ACCEL), RT_ACCELERATOR);
    m_hAccelerators = ::LoadAccelerators (hInstance, MAKEINTRESOURCE (IDR_CECTRL_ACCEL));
    ASSERT(m_hAccelerators);
    if(!m_hAccelerators)
    {
        DWORD   dwError = ::GetLastError();
    }
}

CCEEdit::~CCEEdit()
{
}

void CCEEdit::OnEditCut()
{
    Cut();
}

void CCEEdit::OnEditCopy()
{
    Copy();
}

void CCEEdit::OnEditPaste()
{
    Paste();
}


BOOL CCEEdit::PreTranslateMessage(MSG* pMsg) 
{
    if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
    {
        if (m_hAccelerators != NULL && ::TranslateAccelerator (m_hWnd, m_hAccelerators, pMsg))
        {
            return (TRUE);
        }
    }
	return CEdit::PreTranslateMessage(pMsg);
}
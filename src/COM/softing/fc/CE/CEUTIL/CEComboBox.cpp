
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "CEComboBox.h"
#include "CEUTILrc.h"

//----  Static Initializations:   ----------------------------------*

IMPLEMENT_DYNCREATE(CCEComboBox, CComboBox )

BEGIN_MESSAGE_MAP(CCEComboBox, CComboBox)
	//{{AFX_MSG_MAP(CCEComboBox)
    ON_COMMAND(ID_CEEDIT_COPY, OnEditCopy)
    ON_COMMAND(ID_CEEDIT_CUT, OnEditCut)
    ON_COMMAND(ID_CEEDIT_PASTE, OnEditPaste)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCEComboBox::CCEComboBox()
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

CCEComboBox::~CCEComboBox()
{
}

void CCEComboBox::OnEditCut()
{
    Cut();
}

void CCEComboBox::OnEditCopy()
{
    Copy();
}

void CCEComboBox::OnEditPaste()
{
    Paste();
}


BOOL CCEComboBox::PreTranslateMessage(MSG* pMsg) 
{
    if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
    {
        if (m_hAccelerators != NULL && ::TranslateAccelerator (m_hWnd, m_hAccelerators, pMsg))
        {
            return (TRUE);
        }
    }
	return CComboBox::PreTranslateMessage(pMsg);
}
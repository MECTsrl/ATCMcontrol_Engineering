

#include "stdafx.h"
#include "cewatchview.h"
#include "ButtonIcon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CButtonIcon

CButtonIcon::CButtonIcon()
{
    m_hIconEnable = NULL; 
    m_hIconDisable = NULL; 
}

CButtonIcon::~CButtonIcon()
{
}

void CButtonIcon::SetIcons(int IdIconEnable, int IdIconDisable)
{
	m_hIconEnable = LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IdIconEnable)); 
    m_hIconDisable = LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IdIconDisable)); 
	EnableWindow(IsWindowEnabled());
}

BOOL CButtonIcon::EnableWindow(BOOL bEnable/*=TRUE*/)
{
	BOOL rv = CButton::EnableWindow(bEnable);
	if (IsWindowEnabled())
	{
		if (m_hIconEnable != NULL)
			SetIcon(m_hIconEnable);
	}
	else
	{
		if (m_hIconDisable != NULL)
			SetIcon(m_hIconDisable);
	}
	return rv;
}


BEGIN_MESSAGE_MAP(CButtonIcon, CButton)
	//{{AFX_MSG_MAP(CButtonIcon)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CButtonIcon message handlers



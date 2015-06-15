

#include "stdafx.h"
#include "fchtmled.h"
#include "FcHtmlEdOptions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFcHtmlEdOptions

CFcHtmlEdOptions::CFcHtmlEdOptions()
{
	m_bGridOn = FALSE;
	m_GridHorz = 20;
	m_GridVert = 20;
}

CFcHtmlEdOptions::~CFcHtmlEdOptions()
{
}


BOOL CFcHtmlEdOptions::LoadOptions()
{
	CFcHtmlEdApp* pA = (CFcHtmlEdApp*)AfxGetApp();

	int ii;
	ii = pA->GetProfileInt("Settings", "GridActive", 0);
	m_bGridOn  = ii != 0 ? TRUE : FALSE;

	ii = pA->GetProfileInt("Settings", "GridHorz"  , 20);
	m_GridHorz = ii > 0 ? (unsigned)ii : 20;

	ii = pA->GetProfileInt("Settings", "GridVert"  , 20);
	m_GridVert = ii > 0 ? (unsigned)ii : 20;

	return TRUE;
}

BOOL CFcHtmlEdOptions::SaveOptions()
{
	CFcHtmlEdApp* pA = (CFcHtmlEdApp*)AfxGetApp();
	
	pA->WriteProfileInt("Settings", "GridActive", (int)m_bGridOn);
	pA->WriteProfileInt("Settings", "GridHorz"  , (int)m_GridHorz);
	pA->WriteProfileInt("Settings", "GridVert"  , (int)m_GridVert);

	return TRUE;
}

// SECEditHint.cpp

// Implementation of SECEditHint
// Needs cpp and obj for templatized main base classes

#include "stdafx.h"

#include "SECEdit.h"
#include "SECEditHint.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSDIDoc

IMPLEMENT_DYNAMIC(SECEditHint, CObject)

CGlobalOleInit::CGlobalOleInit()
{
	m_hr = OleInitialize(NULL);
}
	
CGlobalOleInit::~CGlobalOleInit()
{
	if(SUCCEEDED(m_hr))
		OleUninitialize();
}

// Initialize OLE
CGlobalOleInit cglobaloleinitOleInit;
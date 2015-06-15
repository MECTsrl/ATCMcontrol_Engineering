// HelpString.cpp: implementation of the CHelpString class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dialogs.h"
#include "HelpString.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHelpString::CHelpString(UINT nID) : CString()
{
    CString::LoadString(nID);
}

CHelpString::~CHelpString()
{

}

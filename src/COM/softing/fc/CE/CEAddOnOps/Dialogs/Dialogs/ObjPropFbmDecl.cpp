
// ObjPropFbmDecl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ObjPropFbmDecl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//------------------------------------------------------------------*
/**
 * Function name    CObjPropFbmDecl::CObjPropFbmDecl
 * Description      constructor for class CObjPropFbmDecl
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropFbmDecl::CObjPropFbmDecl() : CPropertyPage(CObjPropFbmDecl::IDD)
{
	//{{AFX_DATA_INIT(CObjPropFbmDecl)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropFbmDecl::~CObjPropFbmDecl
 * Description      destructor for class CObjPropFbmDecl
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropFbmDecl::~CObjPropFbmDecl()
{
}

void CObjPropFbmDecl::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjPropFbmDecl)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CObjPropFbmDecl, CPropertyPage)
	//{{AFX_MSG_MAP(CObjPropFbmDecl)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjPropFbmDecl message handlers

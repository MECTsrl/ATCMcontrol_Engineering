


#include "stdafx.h"
#include "CeWatchView.h"
#include "CeWatchViewPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CCeWatchViewPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CCeWatchViewPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CCeWatchViewPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CCeWatchViewPropPage, "4C_20_CE.WatchViewPropPage.1",
	0xD735F1A1, 0xA452, 0x4cde, 0x83, 0x5F, 0xA8, 0x08,0xC1, 0x04, 0x7A, 0x6A)


/////////////////////////////////////////////////////////////////////////////
// CCeWatchViewPropPage::CCeWatchViewPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CCeWatchViewPropPage

BOOL CCeWatchViewPropPage::CCeWatchViewPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_CEWATCHVIEW_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CCeWatchViewPropPage::CCeWatchViewPropPage - Constructor

CCeWatchViewPropPage::CCeWatchViewPropPage() :
	COlePropertyPage(IDD, IDS_CEWATCHVIEW_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CCeWatchViewPropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CCeWatchViewPropPage::DoDataExchange - Moves data between page and properties

void CCeWatchViewPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CCeWatchViewPropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CCeWatchViewPropPage message handlers

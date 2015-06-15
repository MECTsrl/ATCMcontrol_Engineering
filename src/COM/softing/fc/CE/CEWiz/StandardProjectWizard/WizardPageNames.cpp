#include "stdafx.h"
#include "WizardPageNames.h"
#include "ProjectWizardResource.h"
#include "ProjectWizardData.h"


//----  Local Defines:   -------------------------------------------*
#define BITMAP_WIDTH 20

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



BEGIN_MESSAGE_MAP(CWizardPageNames, CPropertyPage)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_4CWIZ_LCONFS, OnResourceChanged)
END_MESSAGE_MAP()

CWizardPageNames::CWizardPageNames (CPropertySheet *pParent, CProjectWizardData *pData)
: CProjectWizardPage (CWizardPageNames::IDD, pParent, pData)
{
    m_iActualResource = 0;
	m_sResource = _T("");
	m_sProgram = _T("");
    m_iLanguage = 0;
	m_sTask = _T("");
	m_sTarget = _T("");
	m_sAddress = _T("");
}

void CWizardPageNames::DoDataExchange(CDataExchange* pDX)
// exchange data between the window elements and the corresponding variables
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_4CWIZ_LCONFS, m_resources);
	DDX_Text(pDX, IDC_4CWIZ_RESOURCE, m_sResource);
	DDX_Text(pDX, IDC_4CWIZ_PROGRAM, m_sProgram);
	DDX_CBIndex(pDX, IDC_4CWIZ_LANGUAGE, m_iLanguage);
	DDX_Text(pDX, IDC_4CWIZ_TASK, m_sTask);
	DDX_Text(pDX, IDC_4CWIZ_TARGET, m_sTarget);
	DDX_Text(pDX, IDC_4CWIZ_ADDRESS, m_sAddress);
}

void CWizardPageNames::OnResourceChanged(NMHDR* pNMHDR, LRESULT* pResult) 
// this function saves the data of the actual resource and displays the
// data of the new chosen resource
{
	NM_LISTVIEW *pListView;
    
    pListView = (NM_LISTVIEW*) pNMHDR;
	if (pListView->uNewState == (LVIS_SELECTED|LVIS_FOCUSED)) {
        if (m_iActualResource != pListView->iItem) {
            ReadResourceData();
            m_iActualResource = pListView->iItem;
            WriteResourceData();
        }
	}
	*pResult = 0;

}

//------------------------------------------------------------------*
/**
 * Writes the resource specific data
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CWizardPageNames::WriteResourceData ()
{
    CProjectWizardResource *pResource;
    CProjectWizardTask *pTask;

    pResource = m_pData->GetResource (m_iActualResource);

    m_sResource = pResource->GetResource ();
    m_sTarget = pResource->GetTarget ();
    m_sAddress = pResource->GetAddress ();

    pTask = pResource->GetTask (0);

    m_sProgram = pTask->GetProgram ();
    m_sTask = pTask->GetName ();
    m_iLanguage = pTask->GetLanguage ();

    UpdateData (FALSE);
}


//------------------------------------------------------------------*
/**
 * Reads the resource specific data
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CWizardPageNames::ReadResourceData ()
{
    CProjectWizardResource *pResource;
    CProjectWizardTask *pTask;

    UpdateData (TRUE);

    pResource = m_pData->GetResource (m_iActualResource);

    pResource->SetResource (m_sResource);
    pResource->SetTarget (m_sTarget);
    pResource->SetAddress (m_sAddress);

    pTask = pResource->GetTask (0);

    pTask->SetProgram (m_sProgram);
    pTask->SetName (m_sTask);
    pTask->SetLanguage ((TIecLanguage)m_iLanguage); 
}


void CWizardPageNames::Init()
// initializes the wizard page / its resource list control
{
    CRect rRect;
    CDC *pDC;
    LV_COLUMN column;

    m_resources.GetClientRect(rRect);
    pDC = m_resources.GetDC();
    pDC->LPtoDP(rRect); 
    m_resources.ReleaseDC(pDC);
    column.mask = LVCF_FMT|LVCF_WIDTH;
    column.fmt = LVCFMT_LEFT;
    column.cx = rRect.Width();
    m_resources.InsertColumn(0,&column);
    m_imageList.Create(IDB_WIZARD_IMAGE_LIST,BITMAP_WIDTH,0,RGB(255,0,255));
	m_imageList.SetBkColor(GetSysColor(COLOR_WINDOW));
	m_resources.SetImageList(&m_imageList,LVSIL_SMALL);
}

BOOL CWizardPageNames::OnSetActive() 
// updates the page after it became active
{
    BOOL bResult;
    CString sTmp;
    int iResource;
    int iResourceCount;

    bResult = CProjectWizardPage::OnSetActive();

    m_resources.DeleteAllItems ();
    iResourceCount = m_pData->GetResourceCount ();
    if (m_iActualResource >= iResourceCount)
        m_iActualResource = 0;
    for (iResource = 0; iResource < iResourceCount; ++iResource)
    {
        CString str;

        str.Format (IDS_4CWIZ_NTH_RESOURCE, iResource + 1);
		m_resources.InsertItem (LVIF_TEXT | LVIF_STATE | LVIF_IMAGE, iResource, str, 0, LVIS_FOCUSED | LVIS_SELECTED, 0, 0L);
	};
	if (m_resources.GetItemCount () > 0)
        m_resources.SetItemState (m_iActualResource, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

    WriteResourceData();
    m_pParent->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT);   
    return bResult;
}


LRESULT CWizardPageNames::OnWizardNext ()
// stores the data entered by the user before leaving the actual wizard page
// performs a series of checks on the data
{
    ReadResourceData ();

    if (!m_pData->CheckResources ())
        return (-1);

    return (CPropertyPage::OnWizardNext ());
}


LRESULT CWizardPageNames::OnWizardBack ()
// stores the data entered by the user before leaving the actual wizard page
{
    ReadResourceData ();

	return (CPropertyPage::OnWizardBack ());
}


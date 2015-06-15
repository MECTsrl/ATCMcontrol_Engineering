/* $Header: /4CReleased/V2.20.00/TargetBase/PROJECTWIZARDBASE/ProjectWizardLibsPage.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: ProjectWizardLibsPage.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/PROJECTWIZARDBASE/ProjectWizardLibsPage.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase
 *
 * =COMPONENT			ProjectWizardBase
 *
 * =CURRENT 	 $Date: 28.02.07 19:06 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "ProjectWizardLibsPage.h"
#include "ProjectWizardLibrary.h"
#include "ProjectWizardData.h"

//----  Local Defines:   -------------------------------------------*
#define BITMAP_WIDTH 20

//----  Static Initializations:   ----------------------------------*
static char szVersioninfo[] = "$Header: /4CReleased/V2.20.00/TargetBase/PROJECTWIZARDBASE/ProjectWizardLibsPage.cpp 1     28.02.07 19:06 Ln $";


BEGIN_MESSAGE_MAP(CProjectWizardLibrariesPage, CPropertyPage)
	ON_BN_CLICKED(IDC_4CWIZ_ADD_LIBRARY, OnAddLibraries)
	ON_BN_CLICKED(IDC_4CWIZ_REMOVE_LIBRARY, OnRemoveLibraries)
	ON_BN_CLICKED(IDC_4CWIZ_ADD_ALL_LIBRARIES, OnAddAllLibraries)
	ON_BN_CLICKED(IDC_4CWIZ_REMOVE_ALL_LIBRARIES, OnRemoveAllLibraries)
	ON_NOTIFY(NM_SETFOCUS, IDC_4CWIZ_USED_LIBS, OnSetFocusUsedLibs)
	ON_NOTIFY(NM_SETFOCUS, IDC_4CWIZ_UNUSED_LIBS, OnSetFocusUnusedLibs)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_4CWIZ_USED_LIBS, OnItemChanged)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_4CWIZ_UNUSED_LIBS, OnItemChanged)
	ON_NOTIFY(NM_DBLCLK, IDC_4CWIZ_UNUSED_LIBS, OnDClkUnusedLibs)
	ON_NOTIFY(NM_DBLCLK, IDC_4CWIZ_USED_LIBS, OnDClkUsedLibs)
END_MESSAGE_MAP()


CProjectWizardLibrariesPage::CProjectWizardLibrariesPage (CPropertySheet *pParent, CProjectWizardData *pData, UINT uiCaption)
: CProjectWizardPage (CProjectWizardLibrariesPage::IDD, pParent, pData, uiCaption),
  m_bSetDefaults (true)
{
}


//------------------------------------------------------------------*
/**
 * Initialize page (set image lists, fill list boxes)
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CProjectWizardLibrariesPage::Init ()
{

    // Create image list and attach to list boxes.
    //--------------------------------------------

    m_imageList.Create (IDB_LIBSPAGE_IMAGELIST, BITMAP_WIDTH, 0, RGB (255,0,255));
    m_imageList.SetBkColor (GetSysColor (COLOR_WINDOW));

	m_usedLibs.InsertColumn(0, _T(""), LVCFMT_LEFT | LVSCW_AUTOSIZE, 250);
    m_usedLibs.SetImageList (&m_imageList, LVSIL_SMALL);

	m_unusedLibs.InsertColumn(0, _T(""), LVCFMT_LEFT | LVSCW_AUTOSIZE, 250);
    m_unusedLibs.SetImageList (&m_imageList, LVSIL_SMALL);


    // Fill list controls and adapt button states.
    //--------------------------------------------

    Update ();


    // Select first item in unused libraries list.
    //--------------------------------------------

    m_unusedLibs.SetItemState (0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
}


//------------------------------------------------------------------*
/**
 * Fill list boxes and adapt button states
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CProjectWizardLibrariesPage::Update ()
{
    FillLists ();
    AdaptButtonStates ();
}


//------------------------------------------------------------------*
/**
 * Fill list controls for used and unused libraries.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CProjectWizardLibrariesPage::FillLists ()
{
    int i;

    m_usedLibs.DeleteAllItems ();
    m_unusedLibs.DeleteAllItems ();

    for (i = 0; i < m_pData->GetLibraryCount (); ++i)
    {
        CProjectWizardLibrary *pLibrary;
        int iItem;

        pLibrary = m_pData->GetLibrary (i);

        if (pLibrary->GetSelectFlag ())
        {
            iItem = m_usedLibs.InsertItem (LVIF_TEXT | LVIF_STATE | LVIF_IMAGE, i, pLibrary->GetLibraryName (), 0, 0, 0, 0L);
            m_usedLibs.SetItemData (iItem, i);
        }
        else
        {
            iItem = m_unusedLibs.InsertItem (LVIF_TEXT | LVIF_STATE | LVIF_IMAGE, i, pLibrary->GetLibraryName (), 0, 0, 0, 0L);
            m_unusedLibs.SetItemData (iItem, i);
        };
    };
}


//------------------------------------------------------------------*
/**
 * Adapt button states to list box content and selection.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CProjectWizardLibrariesPage::AdaptButtonStates ()
{
    CButton *pButton;

    pButton = (CButton *)GetDlgItem(IDC_4CWIZ_ADD_ALL_LIBRARIES);
    pButton->EnableWindow (m_unusedLibs.GetItemCount () > 0);

    pButton = (CButton *)GetDlgItem (IDC_4CWIZ_REMOVE_ALL_LIBRARIES);
    pButton->EnableWindow (m_usedLibs.GetItemCount () > 0);

    pButton = (CButton *)GetDlgItem (IDC_4CWIZ_ADD_LIBRARY);
    pButton->EnableWindow (m_unusedLibs.GetSelectedCount () > 0);

    pButton = (CButton *) GetDlgItem (IDC_4CWIZ_REMOVE_LIBRARY);
    pButton->EnableWindow (m_usedLibs.GetSelectedCount () > 0);
}
 

//------------------------------------------------------------------*
/**
 * Updates page on activation
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
BOOL CProjectWizardLibrariesPage::OnSetActive ()
{
    if (m_bSetDefaults)
    {
        m_pData->UseDefaultLibraries ();
        m_bSetDefaults = false;
        Update ();
	};

    m_pParent->SetWizardButtons (PSWIZB_BACK | PSWIZB_NEXT);   

    return (CProjectWizardPage::OnSetActive ());
}


//------------------------------------------------------------------*
/**
 * DDX
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CProjectWizardLibrariesPage::DoDataExchange (CDataExchange *pDX)
{
	CPropertyPage::DoDataExchange (pDX);

	DDX_Control (pDX, IDC_4CWIZ_USED_LIBS, m_usedLibs);
	DDX_Control (pDX, IDC_4CWIZ_UNUSED_LIBS, m_unusedLibs);
}


//------------------------------------------------------------------*
/**
 * Back button handler
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
LRESULT CProjectWizardLibrariesPage::OnWizardBack ()
{

    // Make sure that the default libraries
    // are displayed on return to this page.
    //--------------------------------------

    m_bSetDefaults = true;

	return 0;
}


//------------------------------------------------------------------*
/**
 * On click of add button marks selected unused libraries as used
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CProjectWizardLibrariesPage::OnAddLibraries ()
{
    UseSelectedUnusedLibraries ();
}


//------------------------------------------------------------------*
/**
 * On click of remove button marks selected used libraries as unused
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CProjectWizardLibrariesPage::OnRemoveLibraries ()
{
    UnuseSelectedUsedLibraries ();
}


//------------------------------------------------------------------*
/**
 * On click of add all button marks all libraries as used
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CProjectWizardLibrariesPage::OnAddAllLibraries ()
{
    m_pData->UseAllLibraries ();

    Update();
}


//------------------------------------------------------------------*
/**
 * On click of remove all button marks all libraries as unused
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CProjectWizardLibrariesPage::OnRemoveAllLibraries ()
{
    m_pData->UseNoLibraries ();

    Update();
}


//------------------------------------------------------------------*
/**
 * On used libraries list gaining focus deselects items in unused
 * libraries list and adapts button state.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CProjectWizardLibrariesPage::OnSetFocusUsedLibs (NMHDR *pNMHDR, LRESULT *pResult)
{
    int i;

    for (i = 0; i < m_unusedLibs.GetItemCount (); ++i)
        m_unusedLibs.SetItemState (i, 0, LVIS_FOCUSED | LVIS_SELECTED);

    AdaptButtonStates ();
}


//------------------------------------------------------------------*
/**
 * On unused libraries list gaining focus deselects items in used
 * libraries list and adapts button state.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CProjectWizardLibrariesPage::OnSetFocusUnusedLibs (NMHDR *pNMHDR, LRESULT *pResult)
{
    int i;

    for (i = 0; i < m_usedLibs.GetItemCount (); ++i)
        m_usedLibs.SetItemState (i, 0, LVIS_FOCUSED | LVIS_SELECTED);

    AdaptButtonStates ();
}


//------------------------------------------------------------------*
/**
 * On list item state change (selection) adapts button states.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CProjectWizardLibrariesPage::OnItemChanged (NMHDR *pNMHDR, LRESULT *pResult)
{
    AdaptButtonStates ();
}


//------------------------------------------------------------------*
/**
 * On double click into unused libraries list marks selected unused libraries as used
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CProjectWizardLibrariesPage::OnDClkUnusedLibs (NMHDR* pNMHDR, LRESULT* pResult) 
{
    UseSelectedUnusedLibraries ();
}


//------------------------------------------------------------------*
/**
 * On double click into used libraries list marks selected used libraries as unused
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CProjectWizardLibrariesPage::OnDClkUsedLibs (NMHDR* pNMHDR, LRESULT* pResult) 
{
    UnuseSelectedUsedLibraries ();
}


//------------------------------------------------------------------*
/**
 * Mark selected unused libraries as used and update lists
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CProjectWizardLibrariesPage::UseSelectedUnusedLibraries ()
{
    POSITION pPosition;

    pPosition = m_unusedLibs.GetFirstSelectedItemPosition ();
    while (pPosition != NULL)
    {
        int iItem;

        iItem = m_unusedLibs.GetNextSelectedItem (pPosition);
        m_pData->UseLibrary (m_unusedLibs.GetItemData (iItem), true);
    };

    Update ();
}


//------------------------------------------------------------------*
/**
 * Mark selected used libraries as unused and update lists
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CProjectWizardLibrariesPage::UnuseSelectedUsedLibraries ()
{
    POSITION pPosition;

    pPosition = m_usedLibs.GetFirstSelectedItemPosition ();
    while (pPosition != NULL)
    {
        int iItem;

        iItem = m_usedLibs.GetNextSelectedItem (pPosition);
        m_pData->UseLibrary (m_usedLibs.GetItemData (iItem), false);
    };

    Update ();
}

/* ---------------------------------------------------------------------------- */

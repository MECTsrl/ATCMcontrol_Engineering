
#include "stdafx.h"
#include "resource.h"
#include "ObjPropHelp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "extern.h"

//------------------------------------------------------------------*
/**
 * Function name    CObjPropHelp::CObjPropHelp
 * Description      constructor for class CObjPropHelp
 * @param           DIALOGTYPE DialogType
 * @param           CPropertySheetBase *PropertySheet
 * @param           CObjPropGenBase *pObjPropGenBase
 * @param           CObjPropInfo *pObjPropInfo
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropHelp::CObjPropHelp(DIALOGTYPE DialogType,
                           CPropertySheetBase *PropertySheet,
                           CObjPropGenBase *pObjPropGenBase, 
                           CObjPropInfo *pObjPropInfo) :
	CObjProp(DialogType, PropertySheet, IDD_OBJPROP_HELP)
{
	m_pObjPropGenBase = pObjPropGenBase;
	m_pObjPropInfo = pObjPropInfo;
    m_pPropertySheet = PropertySheet;

    m_PathOfSelectedTemplate = _T("");
    m_EditTemplate = _T("");
    m_GenerateHelp = FALSE;
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropHelp::~CObjPropHelp
 * Description      destructor for class CObjPropHelp
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropHelp::~CObjPropHelp()
{
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropHelp::DoDataExchange
 * Description      exchanges and validates dialog data
 * @param           CDataExchange* pDX
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjPropHelp::DoDataExchange(CDataExchange* pDX)
{
	CObjProp::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_GENERATE_HELP, m_GenerateHelp);
	DDX_Control(pDX, IDC_BROWSE_TEMPLATE, m_BrowseTemplate);
	DDX_Text(pDX, IDC_EDIT_TEMPLATE, m_EditTemplate);

}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropHelp::OnKillActive
 * Description      is called when this property page is no longer the active page
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL CObjPropHelp::OnKillActive()
{
    UpdateData(TRUE);
    BOOL bValid = TRUE;

	if (m_GenerateHelp == TRUE)
	{
        if (m_EditTemplate.IsEmpty())
        {
            ::AfxMessageBox(IDS_ERR_NO_HELP_TEMPLATE, MB_OK|MB_ICONEXCLAMATION);
            bValid = FALSE;
        }
		else //if (m_pObjPropInfo->m_EditHelpUrl == _T(""))
		{
            // SIS: bug fix d-2019
            // if help url is empty -> set standard name
            if(m_pObjPropInfo->m_EditHelpUrl.IsEmpty())
            {
			    // Set help url in info property page.	
			    CString DocuPath = _T("ATCMfile:");//OH: was GetDocuPath();

			    _TCHAR TemplateExtension[_MAX_EXT];
			    _tsplitpath(m_EditTemplate,
					       NULL, 
					       NULL, 
					       NULL, 
					    TemplateExtension);
			    m_pObjPropInfo->m_EditHelpUrl = DocuPath + 
		                                        m_pObjPropGenBase->m_Name + 
					                            TemplateExtension;
            }
		}
    }

    return (bValid & CObjProp::OnKillActive());
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropHelp::OnBrowseTemplateClicked
 * Description      handler for the browse template ('...') button
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjPropHelp::OnBrowseTemplateClicked()
{
	// Transfer contents of edit boxes into members.
	UpdateData(TRUE);

    CFileDialog FileDialog(TRUE, 
                           NULL, 
                           NULL, 
                           OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR, 
                           _T("HTML Files (*.htm; *.html)|*.htm; *.html|Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"), 
                           this);
    CString TemplatePath = GetInstallationPath() + _T("Engineering\\Templates\\Userhelp\\");
    FileDialog.m_ofn.lpstrInitialDir = TemplatePath;
    CString strHelp;
    strHelp.LoadString(IDS_SELECT_FILE_HELP_URL);
    FileDialog.m_ofn.lpstrTitle      = strHelp;

    int ok = FileDialog.DoModal();
	if (ok == IDOK)
	{
		// Set edit box.
		m_EditTemplate = FileDialog.GetPathName();
        m_PathOfSelectedTemplate = m_EditTemplate; //OH: this info is needed too
		UpdateData(FALSE);
	}
}


//------------------------------------------------------------------*
/**
 * Function name			CObjPropHelp::OnGenerateHelpClicked
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CObjPropHelp::OnGenerateHelpClicked()
{
	// Transfer contents of edit boxes into members.
	UpdateData(TRUE);

    //ask about the state of the check box
    if(this->IsDlgButtonChecked(IDC_GENERATE_HELP))
    {
        GetDlgItem(IDC_EDIT_TEMPLATE)->EnableWindow(TRUE);
        GetDlgItem(IDC_BROWSE_TEMPLATE)->EnableWindow(TRUE);
        m_pPropertySheet->m_bGenerateHelp = TRUE;
    }
    else if(!this->IsDlgButtonChecked(IDC_GENERATE_HELP))
    {
        GetDlgItem(IDC_EDIT_TEMPLATE)->EnableWindow(FALSE);
        GetDlgItem(IDC_BROWSE_TEMPLATE)->EnableWindow(FALSE);
        m_pPropertySheet->m_bGenerateHelp = FALSE;
        //empty the edit
        m_EditTemplate.Empty();
    }
    UpdateData(FALSE);
}

//------------------------------------------------------------------*
/**
 * Function name			CObjPropHelp::OnInitDialog
 * Description			
 * @return			BOOL 
 * @exception			
 * @see			
*/
BOOL CObjPropHelp::OnInitDialog() 
{
	CObjProp::OnInitDialog();
    if(!m_pPropertySheet->m_bGenerateHelp)
    {
        GetDlgItem(IDC_EDIT_TEMPLATE)->EnableWindow(FALSE);
        GetDlgItem(IDC_BROWSE_TEMPLATE)->EnableWindow(FALSE);
    }
    UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BEGIN_MESSAGE_MAP(CObjPropHelp, CObjProp)
    ON_BN_CLICKED(IDC_BROWSE_TEMPLATE, OnBrowseTemplateClicked)
    ON_BN_CLICKED(IDC_GENERATE_HELP, OnGenerateHelpClicked)
END_MESSAGE_MAP()



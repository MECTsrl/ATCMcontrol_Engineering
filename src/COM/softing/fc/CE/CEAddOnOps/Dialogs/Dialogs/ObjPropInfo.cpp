
#include "stdafx.h"
#include "resource.h"

#include "ObjPropInfo.h"
#include "extern.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//------------------------------------------------------------------*
/**
 * Function name    CObjPropInfo::CObjPropInfo
 * Description      constructor for class CObjPropInfo
 * @param           DIALOGTYPE DialogType
 * @param           CPropertySheetBase *PropertySheet
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropInfo::CObjPropInfo(DIALOGTYPE DialogType, 
                           CPropertySheetBase *PropertySheet) : 
    CObjProp(DialogType, PropertySheet, IDD_OBJPROP_INFO)
{
    m_DialogType = DialogType;
	m_EditText1 = _T("");
	m_EditText2 = _T("");
	m_EditText3 = _T("");
	m_EditHelpUrl = _T("");
	m_EditUrl = _T("");
    m_pPropertySheet = PropertySheet;
    m_bShowUserHelp = TRUE;
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropInfo::~CObjPropInfo
 * Description      destructor for class CObjPropInfo
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropInfo::~CObjPropInfo()
{
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropInfo::DoDataExchange
 * Description      exchanges and validates dialog data
 * @param           CDataExchange* pDX
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjPropInfo::DoDataExchange(CDataExchange* pDX)
{
	CObjProp::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BROWSE_HELPURL, m_BrowseHelpUrl);
	DDX_Control(pDX, IDC_BROWSE_URL, m_BrowseUrl);
	DDX_Text(pDX, IDC_EDIT_TEXT1, m_EditText1);
	DDX_Text(pDX, IDC_EDIT_TEXT2, m_EditText2);
	DDX_Text(pDX, IDC_EDIT_TEXT3, m_EditText3);
	DDX_Text(pDX, IDC_EDIT_HELPURL, m_EditHelpUrl);
	DDX_Text(pDX, IDC_EDIT_URL, m_EditUrl);
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenResource::OnInitDialog
 * Description      message handler for the WM_INITDIALOG message
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL CObjPropInfo::OnInitDialog() 
{
	CObjProp::OnInitDialog();

    if (m_DialogType == IVIEWPROP)
    {
        GetDlgItem(IDC_EDIT_TEXT1)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_TEXT2)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_TEXT3)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_HELPURL)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_URL)->EnableWindow(FALSE);
        GetDlgItem(IDC_BROWSE_HELPURL)->EnableWindow(FALSE);
        GetDlgItem(IDC_BROWSE_URL)->EnableWindow(FALSE);
    }
    else if(m_pPropertySheet->m_bGenerateHelp)
    {
        GetDlgItem(IDC_EDIT_HELPURL)->EnableWindow(FALSE);
        GetDlgItem(IDC_BROWSE_HELPURL)->EnableWindow(FALSE);
    }
    else if(!m_pPropertySheet->m_bGenerateHelp)
    {
        GetDlgItem(IDC_EDIT_HELPURL)->EnableWindow(TRUE);
        GetDlgItem(IDC_BROWSE_HELPURL)->EnableWindow(TRUE);
    }

    if (!m_bShowUserHelp)
    {
        GetDlgItem(IDC_EDIT_HELPURL)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BROWSE_HELPURL)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_INFO_HELPURL)->ShowWindow(SW_HIDE);
    }

    UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropInfo::OnBrowseUrlClicked
 * Description      handler for the browse url ('...') button
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjPropInfo::OnBrowseUrlClicked()
{
	// Transfer contents of edit boxes into members.
	UpdateData(TRUE);

    CFileDialog FileDialog(TRUE, 
                           NULL, 
                           NULL, 
                           OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR, 
                           _T("HTML Files (*.htm; *.html)|*.htm; *.html|All Files (*.*)|*.*||"), 
                           this);
	CString ProjectPath = GetProjectPath();
    FileDialog.m_ofn.lpstrInitialDir = ProjectPath;
    CString strHelp;
    strHelp.LoadString(IDS_SELECT_FILE_AS_URL);
    FileDialog.m_ofn.lpstrTitle      = strHelp;

    int ok = FileDialog.DoModal();
	if (ok == IDOK)
	{
		// Set edit box.
		m_EditUrl = FileDialog.GetPathName();
        int iPos = m_EditUrl.ReverseFind(_T('\\'));
        ASSERT(iPos > 0);
        CString strHelp = m_EditUrl.Left(iPos + 1);
        if(!strHelp.CollateNoCase(ProjectPath))
        {
            strHelp = m_EditUrl.Right(m_EditUrl.GetLength() - iPos - 1);
            m_EditUrl = _T("ATCMfile:") + strHelp;
        }
		UpdateData(FALSE);
	}
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropInfo::OnBrowseHelpUrlClicked
 * Description      handler for the browse help url ('...') button
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjPropInfo::OnBrowseHelpUrlClicked()
{
	// Transfer contents of edit boxes into members.
	UpdateData(TRUE);

    CFileDialog FileDialog(TRUE, 
                           NULL, 
                           NULL, 
                           OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR, 
                           _T("HTML Files (*.htm; *.html)|*.htm; *.html|Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"), 
                           this);
	CString DocuPath = GetDocuPath();
    FileDialog.m_ofn.lpstrInitialDir = DocuPath;
    CString strHelp;
    strHelp.LoadString(IDS_SELECT_FILE_HELP_URL);
    FileDialog.m_ofn.lpstrTitle      = strHelp;

    int ok = FileDialog.DoModal();
	if (ok == IDOK)
	{
		// Set edit box.
		m_EditHelpUrl = FileDialog.GetPathName();
        int iPos = m_EditHelpUrl.ReverseFind(_T('\\'));
        ASSERT(iPos > 0);
        CString strHelp = m_EditHelpUrl.Left(iPos + 1);
        if(!strHelp.CollateNoCase(DocuPath))
        {
            strHelp = m_EditHelpUrl.Right(m_EditHelpUrl.GetLength() - iPos - 1);
            m_EditHelpUrl = _T("ATCMFile:") + strHelp;
        }
		UpdateData(FALSE);
	}
}

//------------------------------------------------------------------*
/**
 * Function name			CObjPropInfo::OnSetActive
 * Description			
 * @return			BOOL 
 * @exception			
 * @see			
*/
BOOL CObjPropInfo::OnSetActive()
{
    /*if (m_PropertySheet->HasBeenApplied() == FALSE)
    {
        SetModified(TRUE);
    }*/
    if(m_pPropertySheet->m_bGenerateHelp)
    {
        GetDlgItem(IDC_EDIT_HELPURL)->EnableWindow(FALSE);
        GetDlgItem(IDC_BROWSE_HELPURL)->EnableWindow(FALSE);
    }
    else if(!m_pPropertySheet->m_bGenerateHelp)
    {
        GetDlgItem(IDC_EDIT_HELPURL)->EnableWindow(TRUE);
        GetDlgItem(IDC_BROWSE_HELPURL)->EnableWindow(TRUE);
    }
    
    UpdateData(FALSE);

    if (m_PropertySheet->IsReadOnly())
    {
        SetReadOnly();
    }

    return CPropertyPage::OnSetActive();
}

BEGIN_MESSAGE_MAP(CObjPropInfo, CObjProp)
    ON_BN_CLICKED(IDC_BROWSE_URL, OnBrowseUrlClicked)
    ON_BN_CLICKED(IDC_BROWSE_HELPURL, OnBrowseHelpUrlClicked)
END_MESSAGE_MAP()


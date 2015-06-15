
#include "stdafx.h"
#include "resource.h"
#include "ObjPropWiz.h"
#include "SyntaxCheck.h"
#include "GlobConst.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//------------------------------------------------------------------*
/**
 * Function name    CObjPropWiz::CObjPropWiz
 * Description      constructor for class CObjPropWiz
 * @param           DIALOGTYPE DialogType
 * @param           CPropertySheetBase *PropertySheet
 * @param           CObjPropGenBase *pObjPropGenBase
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropWiz::CObjPropWiz(DIALOGTYPE DialogType, 
                         CPropertySheetBase *PropertySheet,
                         CObjPropGenBase *pObjPropGenBase) : 
    CObjProp(DialogType, PropertySheet, IDD_OBJPROP_WIZ)
{
    m_pObjPropGenBase = pObjPropGenBase;
    if (m_pObjPropGenBase != NULL)
    {
        m_pObjPropGenBase->m_pObjPropWiz = this;
    }
	m_Name = _T("");
    m_WizardNames.SetSize(0);
    m_IndexOfSelectedWizardName = -1;

    m_ImageList.Create(IDB_WIZIMAGELIST, 20, 0, RGB(255,0,255));
    m_ImageList.SetBkColor(::GetSysColor(COLOR_WINDOW));  
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropWiz::~CObjPropWiz
 * Description      destructor for class CObjPropWiz
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropWiz::~CObjPropWiz()
{
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropWiz::AddWizardName
 * Description      adds a wizard name and the corresponding iec language
 *                  to the list of wizard names
 * @param           LPCTSTR WizardName
 * @param           LPCTSTR Language
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjPropWiz::AddWizardName(LPCTSTR WizardName, LPCTSTR Language) 
{
    m_WizardNames.Add(WizardName);
    m_Languages.Add(Language);
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropWiz::GetLanguageOfSelectedWizard
 * Description      returns the language of the selected wizard
 * @return          CString
 * @exception       -
 * @see             -
*/
CString CObjPropWiz::GetLanguageOfSelectedWizard() 
{
    if ((m_IndexOfSelectedWizardName >= 0) && (m_IndexOfSelectedWizardName <= m_Languages.GetUpperBound()))
    {
        return m_Languages.GetAt(m_IndexOfSelectedWizardName);
    }
    else
    {
        return _T("UNKNOWN");
    }
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropWiz::DoDataExchange
 * Description      exchanges and validates dialog data
 * @param           CDataExchange* pDX
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjPropWiz::DoDataExchange(CDataExchange* pDX)
{
	CObjProp::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_Wizards);
	DDX_Text(pDX, IDC_OBJNAME, m_Name);
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropWiz::OnInitDialog
 * Description      message handler for the WM_INITDIALOG message
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL CObjPropWiz::OnInitDialog() 
{
	CObjProp::OnInitDialog();

    m_Wizards.SetImageList(&m_ImageList, LVSIL_SMALL);

    // Transfer wizard names from Array into ListCtrl.
    int ArraySize = m_WizardNames.GetSize(); 
    for(int i = 0; i < ArraySize; i++)
    {
        m_Wizards.InsertItem(i+1, m_WizardNames.GetAt(i), 0);
    }

    if (i > 0)
    {
         m_Wizards.SetItemState(0, 
                                LVIS_SELECTED | LVIS_FOCUSED, 
                                LVIS_SELECTED | LVIS_FOCUSED);
    }

    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropWiz::OnKillActive
 * Description      is called when this property page is no longer the active page
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL CObjPropWiz::OnKillActive()
{
    UpdateData(TRUE);
	Trim(m_Name);
	UpdateData(FALSE);

    // Check syntax of name.
	if (m_Name.GetLength() == 0)
	{
        CString strHelp;
        strHelp.LoadString(IDS_OBJ_NAME_MUST_NOT_EMPTY);
		::AfxMessageBox(strHelp, MB_ICONWARNING);
		return FALSE;
	}

    if (IsIdentifier(m_Name) == FALSE)
	{
        CString strMessage;
        strMessage.Format(IDS_OBJ_NAME_INVALID, m_Name);
        ::AfxMessageBox(strMessage, MB_ICONWARNING);
        return FALSE;
	}

    m_IndexOfSelectedWizardName = m_Wizards.GetSelectionMark();

    // Copy name to "General" page
    m_pObjPropGenBase->m_Name = m_Name;

    return CObjProp::OnKillActive();
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenBase::OnChangeName
 * Description      message handler for changes of the object name
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjPropWiz::OnChangeName() 
{
    m_PropertySheet->ResetApplied();
    SetModified(TRUE);
}

BEGIN_MESSAGE_MAP(CObjPropWiz, CObjProp)
    ON_EN_CHANGE(IDC_OBJNAME, OnChangeName)
END_MESSAGE_MAP()


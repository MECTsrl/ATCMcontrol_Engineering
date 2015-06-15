
#include "stdafx.h"
#include "resource.h"
#include "ObjPropGenBase.h"
#include "GlobConst.h"
#include "SyntaxCheck.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenBase::CObjPropGenBase
 * Description      constructor for class CObjPropGenBase
 * @param           DIALOGTYPE DialogType
 * @param           UINT nIDTemplate
 * @param           CPropertySheetBase *PropertySheet
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropGenBase::CObjPropGenBase(DIALOGTYPE DialogType, 
                                 UINT nIDTemplate, 
                                 CPropertySheetBase *PropertySheet) : 
    CObjProp(DialogType, PropertySheet, nIDTemplate)
{
    m_pObjPropWiz = NULL;

	m_Alias = _T("");
	m_Name = _T("");
	m_CreateDate = _T("");
	m_CreateUser = _T("");
	m_LastModDate = _T("");
	m_LastModUser = _T("");
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenBase::~CObjPropGenBase
 * Description      destructor for class CObjPropGenBase
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropGenBase::~CObjPropGenBase()
{
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenBase::OnInitDialog
 * Description      message handler for the WM_INITDIALOG message
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL CObjPropGenBase::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

    if (m_DialogType == CVIEWPROP)
    {
        if(GetDlgItem(IDC_EDIT_LOCATION))
        {
            GetDlgItem(IDC_EDIT_LOCATION)->EnableWindow(FALSE);
        }
    }

    if (!((m_DialogType == CVIEWINSERT) && (GetPageIndex() == 0)))
    {
        GetDlgItem(IDC_OBJNAME)->EnableWindow(FALSE);
    }

    UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenBase::DoDataExchange
 * Description      exchanges and validates dialog data
 * @param           CDataExchange* pDX
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjPropGenBase::DoDataExchange(CDataExchange* pDX)
{
	CObjProp::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ALIAS, m_Alias);
	DDX_Text(pDX, IDC_OBJNAME, m_Name);
	DDX_Text(pDX, IDC_OBJPROP_CREATEDATE, m_CreateDate);
	DDX_Text(pDX, IDC_OBJPROP_CREATEUSER, m_CreateUser);
	DDX_Text(pDX, IDC_OBJPROP_LASTMODDATE, m_LastModDate);
	DDX_Text(pDX, IDC_OBJPROP_LASTMODUSER, m_LastModUser);
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenBase::OnKillActive
 * Description      is called when this property page is no longer the active page
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL CObjPropGenBase::OnKillActive()
{
    if (m_DialogType != IVIEWPROP)
    {
        UpdateData(TRUE);
        Trim(m_Name);
        Trim(m_Alias);
	    UpdateData(FALSE);

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
    }

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
void CObjPropGenBase::OnChangeName() 
{
    m_PropertySheet->ResetApplied();
    SetModified(TRUE);
}

BEGIN_MESSAGE_MAP(CObjPropGenBase, CObjProp)
    ON_EN_CHANGE(IDC_OBJNAME, OnChangeName)
END_MESSAGE_MAP()


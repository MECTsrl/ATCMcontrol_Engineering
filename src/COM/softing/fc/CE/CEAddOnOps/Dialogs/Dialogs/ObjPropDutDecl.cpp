
#include "stdafx.h"
#include "dialogs.h"
#include "ObjPropDutDecl.h"
#include "ModifyDutMember.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name    CObjPropDutDecl::CObjPropDutDecl
 * Description      constructor for class CObjPropDutDecl
 * @param           DIALOGTYPE DialogType
 * @param           CPropertySheetBase *PropertySheet
 * @param           int iColumnStrId
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropDutDecl::CObjPropDutDecl(DIALOGTYPE DialogType,
                                 CPropertySheetBase *PropertySheet,
                                 int iColumnStrId) :
    CObjPropDeclBase(DialogType, IDD_OBJPROP_DUTDECL, PropertySheet, iColumnStrId)
{
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropDutDecl::~CObjPropDutDecl
 * Description      destructor for class CObjPropDutDecl
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropDutDecl::~CObjPropDutDecl()
{

}

BEGIN_MESSAGE_MAP(CObjPropDutDecl, CObjPropDeclBase)
	ON_BN_CLICKED(IDC_ADDMEMBER, OnAddMember)
	ON_BN_CLICKED(IDC_MODIFYMEMBER, OnModifyMember)
    ON_BN_CLICKED(IDC_PROP_UPMEMBER, OnUpMember)
    ON_BN_CLICKED(IDC_PROP_DOWNMEMBER, OnDownMember)
	ON_NOTIFY(NM_CLICK, IDC_MEMBERS, OnClickFbmembers)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjPropDeclBase message handlers

//------------------------------------------------------------------*
/**
 * Function name    CObjPropDutDecl::OnAddMember
 * Description      handler for the add member button
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjPropDutDecl::OnAddMember() 
{
	CString	strName = _T("");
	CString	strAlias = _T("");
	CString	strComment = _T("");
	CString	strType = _T("");
	CString	strInitVal = _T("");
	int     iCount = 0;

    CString sIndex = _T("-1"); 
    CString sRange = _T("-1"); 
    CString sHint  = _T("No");  
    CArray<CString, CString>* pDataArray;
    pDataArray = new CArray<CString, CString>;
    pDataArray->Add(sIndex);
    pDataArray->Add(sRange);
    pDataArray->Add(sHint);

    CModifyDutMember aDutMember;

    aDutMember.SetParent(this);

	switch(aDutMember.DoModal())
	{
	case IDOK:
		aDutMember.GetMemberDecl(strName, strAlias, strComment, strType, strInitVal);

		if(strName.IsEmpty() || strType.IsEmpty())
		{
			break;
		}
        // SIS: add quotation marks
        CheckStringValue(strInitVal, strType, TRUE);

		iCount = m_FbMembers.GetItemCount();
		m_FbMembers.InsertItem(iCount, strName);
		m_FbMembers.SetItemText(iCount, m_aiCol[COL_ALIAS], strAlias);
		m_FbMembers.SetItemText(iCount, m_aiCol[COL_COMMENT], strComment);
		m_FbMembers.SetItemText(iCount, m_aiCol[COL_TYPE], strType);
		m_FbMembers.SetItemText(iCount, m_aiCol[COL_INITVAL], strInitVal);

        sHint = _T("Add");
        pDataArray->SetAt(2, sHint);
        m_FbMembers.SetItemData(iCount, (DWORD)pDataArray);
		break;
	case IDCANCEL:
		break;
	default:
		;
	}

	return;
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropDutDecl::OnModifyMember
 * Description      handler for the modify member button
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjPropDutDecl::OnModifyMember() 
{
	CString	strName = _T("");
	CString	strAlias = _T("");
	CString	strComment = _T("");
	CString	strType = _T("");
	CString	strInitVal = _T("");

    CString sHint = _T("No");
    CArray<CString, CString>* pDataArray;

	// Get selected item
	POSITION pos = m_FbMembers.GetFirstSelectedItemPosition();
    if(!pos)
    {
        return;
    }
	int iItem = m_FbMembers.GetNextSelectedItem(pos);
    if(iItem == -1)
    {
        return;
    }
    else
    {
        strName = m_FbMembers.GetItemText(iItem, m_aiCol[COL_NAME]);
        strAlias = m_FbMembers.GetItemText(iItem, m_aiCol[COL_ALIAS]);
        strComment = m_FbMembers.GetItemText(iItem, m_aiCol[COL_COMMENT]);
        strType = m_FbMembers.GetItemText(iItem, m_aiCol[COL_TYPE]);
        strInitVal = m_FbMembers.GetItemText(iItem, m_aiCol[COL_INITVAL]);
        
        pDataArray = (CArray<CString, CString>*)m_FbMembers.GetItemData(iItem);

        // SIS: strip quotation marks
        CheckStringValue(strInitVal, strType, FALSE);
        
        CModifyDutMember aDutMember(strName, strAlias, strComment, strType, strInitVal);
        m_FbMembers.SetItemText(iItem, m_aiCol[COL_NAME], "");
        aDutMember.SetParent(this);
        switch(aDutMember.DoModal())
        {
        case IDOK:
            aDutMember.GetMemberDecl(strName, strAlias, strComment, strType, strInitVal);
            // SIS: add quotation marks
            CheckStringValue(strInitVal, strType, TRUE);
            m_FbMembers.SetItemText(iItem, m_aiCol[COL_NAME], strName);
            m_FbMembers.SetItemText(iItem, m_aiCol[COL_ALIAS], strAlias);
            m_FbMembers.SetItemText(iItem, m_aiCol[COL_COMMENT], strComment);
            m_FbMembers.SetItemText(iItem, m_aiCol[COL_TYPE], strType);
            m_FbMembers.SetItemText(iItem, m_aiCol[COL_INITVAL], strInitVal);
            sHint = pDataArray->GetAt(2);
            if(sHint != _T("Add"))
            {
                sHint  = _T("Mod");
                pDataArray->SetAt(2, sHint);
                m_FbMembers.SetItemData(iItem, (DWORD)pDataArray);
            }
            break;
        case IDCANCEL:
            m_FbMembers.SetItemText(iItem, m_aiCol[COL_NAME], strName);
            break;
        default:
            ;
        }
        
        return;
    }
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropDutDecl::OnUpMember
 * Description      handler for the up button
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjPropDutDecl::OnUpMember() 
{
	CString	strTmpName = _T("");
	CString	strTmpAlias = _T("");
	CString	strTmpComment = _T("");
	CString	strTmpType = _T("");
	CString	strTmpInitVal = _T("");
    CArray<CString, CString>* pDataArray;
    CArray<CString, CString>* pDataArray1;

	// Get selected item
	POSITION pos = m_FbMembers.GetFirstSelectedItemPosition();
    if(!pos)
    {
        return;
    }
	int iItem = m_FbMembers.GetNextSelectedItem(pos);

	strTmpName    = m_FbMembers.GetItemText(iItem, m_aiCol[COL_NAME]);
	strTmpAlias   = m_FbMembers.GetItemText(iItem, m_aiCol[COL_ALIAS]);
	strTmpComment   = m_FbMembers.GetItemText(iItem, m_aiCol[COL_COMMENT]);
	strTmpType    = m_FbMembers.GetItemText(iItem, m_aiCol[COL_TYPE]);
	strTmpInitVal = m_FbMembers.GetItemText(iItem, m_aiCol[COL_INITVAL]);
    pDataArray = (CArray<CString, CString>*)m_FbMembers.GetItemData(iItem);

	m_FbMembers.SetItemText(iItem, m_aiCol[COL_NAME], m_FbMembers.GetItemText(iItem - 1, m_aiCol[COL_NAME]));
	m_FbMembers.SetItemText(iItem, m_aiCol[COL_ALIAS], m_FbMembers.GetItemText(iItem - 1, m_aiCol[COL_ALIAS]));
	m_FbMembers.SetItemText(iItem, m_aiCol[COL_COMMENT], m_FbMembers.GetItemText(iItem - 1, m_aiCol[COL_COMMENT]));
	m_FbMembers.SetItemText(iItem, m_aiCol[COL_TYPE], m_FbMembers.GetItemText(iItem - 1, m_aiCol[COL_TYPE]));
    pDataArray1 = (CArray<CString, CString>*)m_FbMembers.GetItemData(iItem - 1);
    m_FbMembers.SetItemData(iItem, (DWORD)pDataArray1);

	m_FbMembers.SetItemText(iItem - 1, m_aiCol[COL_NAME], strTmpName);
	m_FbMembers.SetItemText(iItem - 1, m_aiCol[COL_ALIAS], strTmpAlias);
	m_FbMembers.SetItemText(iItem - 1, m_aiCol[COL_COMMENT], strTmpComment);
	m_FbMembers.SetItemText(iItem - 1, m_aiCol[COL_TYPE], strTmpType);
	m_FbMembers.SetItemText(iItem - 1, m_aiCol[COL_INITVAL], strTmpInitVal);
    m_FbMembers.SetItemData(iItem - 1, (DWORD)pDataArray);

    // Highlight item
    UINT uiItemState = 0;
    uiItemState = m_FbMembers.GetItemState(iItem, 15);
    m_FbMembers.SetItemState(iItem, 0, 15);
    m_FbMembers.SetItemState(iItem - 1, uiItemState, 15);

    EnableButtons();
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropDutDecl::OnDownMember
 * Description      handler for the down button
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjPropDutDecl::OnDownMember() 
{
	CString	strTmpName = _T("");
	CString	strTmpAlias = _T("");
	CString	strTmpComment = _T("");
	CString	strTmpType = _T("");
	CString	strTmpInitVal = _T("");
    CArray<CString, CString>* pDataArray;
    CArray<CString, CString>* pDataArray1;

	// Get selected item
	POSITION pos = m_FbMembers.GetFirstSelectedItemPosition();
    if(!pos)
    {
        return;
    }
	int iItem = m_FbMembers.GetNextSelectedItem(pos);

	strTmpName    = m_FbMembers.GetItemText(iItem, m_aiCol[COL_NAME]);
	strTmpAlias   = m_FbMembers.GetItemText(iItem, m_aiCol[COL_ALIAS]);
	strTmpComment = m_FbMembers.GetItemText(iItem, m_aiCol[COL_COMMENT]);
	strTmpType    = m_FbMembers.GetItemText(iItem, m_aiCol[COL_TYPE]);
	strTmpInitVal = m_FbMembers.GetItemText(iItem, m_aiCol[COL_INITVAL]);
    pDataArray = (CArray<CString, CString>*)m_FbMembers.GetItemData(iItem);

	m_FbMembers.SetItemText(iItem, m_aiCol[COL_NAME], m_FbMembers.GetItemText(iItem + 1, m_aiCol[COL_NAME]));
	m_FbMembers.SetItemText(iItem, m_aiCol[COL_ALIAS], m_FbMembers.GetItemText(iItem + 1, m_aiCol[COL_ALIAS]));
	m_FbMembers.SetItemText(iItem, m_aiCol[COL_COMMENT], m_FbMembers.GetItemText(iItem + 1, m_aiCol[COL_COMMENT]));
	m_FbMembers.SetItemText(iItem, m_aiCol[COL_TYPE], m_FbMembers.GetItemText(iItem + 1, m_aiCol[COL_TYPE]));
    pDataArray1 = (CArray<CString, CString>*)m_FbMembers.GetItemData(iItem + 1);
    m_FbMembers.SetItemData(iItem, (DWORD)pDataArray1);

	m_FbMembers.SetItemText(iItem + 1, m_aiCol[COL_NAME], strTmpName);
	m_FbMembers.SetItemText(iItem + 1, m_aiCol[COL_ALIAS], strTmpAlias);
	m_FbMembers.SetItemText(iItem + 1, m_aiCol[COL_COMMENT], strTmpComment);
	m_FbMembers.SetItemText(iItem + 1, m_aiCol[COL_TYPE], strTmpType);
	m_FbMembers.SetItemText(iItem + 1, m_aiCol[COL_INITVAL], strTmpInitVal);
    m_FbMembers.SetItemData(iItem + 1, (DWORD)pDataArray);

    // Highlight item
    UINT uiItemState = 0;
    uiItemState = m_FbMembers.GetItemState(iItem, 15);
    m_FbMembers.SetItemState(iItem, 0, 15);
    m_FbMembers.SetItemState(iItem + 1, uiItemState, 15);

    EnableButtons();
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropDutDecl::OnClickFbmembers
 * Description      -
 * @param           NMHDR* pNMHDR
 * @param           LRESULT* pResult
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjPropDutDecl::OnClickFbmembers(NMHDR* pNMHDR, LRESULT* pResult) 
{
    CObjPropDeclBase::OnClickFbmembers(pNMHDR, pResult);

    EnableButtons();

	*pResult = 0;
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropDutDecl::EnableButtons
 * Description      enables the up and down button
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjPropDutDecl::EnableButtons()
{
    if(m_DialogType != IVIEWPROP)
    {
        int iSelCount = m_FbMembers.GetSelectedCount();
        int iItemCount = m_FbMembers.GetItemCount();

	    // Get selected item
	    POSITION pos = m_FbMembers.GetFirstSelectedItemPosition();
        if(!pos)
        {
            return;
        }
	    int iItem = m_FbMembers.GetNextSelectedItem(pos);

	    // Enable'Modify' and 'Delete' button
	    GetDlgItem(IDC_PROP_UPMEMBER)->EnableWindow(iSelCount == 1 && iItem > 0);
	    GetDlgItem(IDC_PROP_DOWNMEMBER)->EnableWindow(iSelCount == 1 && iItem < iItemCount - 1);
    }
}

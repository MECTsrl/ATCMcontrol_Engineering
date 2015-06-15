/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "AssignmentDialog.h"

#include "ATLControls.h"    // Common control helpers
using namespace ATLControls;



//------------------------------------------------------------------*
/**
 * constructor
 *
 * @param           rastrResource: string array of resource names
 * @param           crstrResource: name of currently selected resource
 * @exception       -
 * @see             -
*/
CAssignmentDialog::CAssignmentDialog(CStringArray& rastrResources, const CString& crstrResource)
{
    m_astrResources.Copy(rastrResources);
    m_strResource = crstrResource;
}


//------------------------------------------------------------------*
/**
 * destructor
 *
 * @exception       -
 * @see             
*/
CAssignmentDialog::~CAssignmentDialog()
{
}


//------------------------------------------------------------------*
/**
 * OnInitDialog does all steps to initialize the assignment dialog
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
LRESULT CAssignmentDialog::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    this->CenterWindow();

    ATLControls::CListBox listBox(GetDlgItem(IDC_LIST_RESOURCES));
    
    CString     strTmp;
    strTmp.LoadString(IDS_NO_ASSIGN);
    listBox.AddString(strTmp);

    int iNumStr = m_astrResources.GetSize();
    for(int iStr = 0; iStr < iNumStr; ++iStr)
    {
        listBox.AddString(m_astrResources[iStr]);
    }

    int sel;
    if(m_strResource.IsEmpty())
    {
        listBox.SetCurSel(0);
    }
    else
    {
        sel = listBox.FindStringExact(0, m_strResource);
        if (sel!=LB_ERR)
        {
            listBox.SetCurSel(sel);
        }
    }

	return 1;  // Let the system set the focus
}


//------------------------------------------------------------------*
/**
 * OnOK the handler used when the user presses the ok button
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CAssignmentDialog::DoOk()
{
    ATLControls::CListBox listBox(GetDlgItem(IDC_LIST_RESOURCES));

    CString strRes;

    int sel = listBox.GetCurSel();
    if (sel == 0) // no assignment
    {
        m_strResource.Empty();
    }
    else
    {
        int textLen = listBox.GetTextLen(sel);
        TCHAR* res;
        res = new TCHAR[textLen+1];
        memset(res,0,textLen+1);
        listBox.GetText(sel, res);
        m_strResource = res;
        delete res;
    }


	EndDialog(ID_OK);
}

LRESULT CAssignmentDialog::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    DoOk();
	return 0;
}


LRESULT CAssignmentDialog::OnDblclkListResources(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    DoOk();
    return 0;
}



//------------------------------------------------------------------*
/**
 * OnCancle the handler used when the user presses the Cancel button.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
LRESULT CAssignmentDialog::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

LRESULT CAssignmentDialog::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    EndDialog(0);
   	return 0;
}


//LRESULT CAssignmentDialog::OnShowHelp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//    ASSERT(m_pWagoTarget!=NULL);
//    if (m_pWagoTarget==NULL)
//    {
//        return 0;
//    }
//
//    m_pWagoTarget->ShowUserHelp(IDD_ASSIGNMENTDIALOG);
//    return 0;
//}
//

/* ---------------------------------------------------------------------------- */


#include "stdafx.h"
#include "Dialogs.h"
#include "ObjProp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name    Trim
 * Description      removes the white space characters from the beginning and end 
 * @param           CString &String
 * @return          -
 * @exception       -
 * @see             -
*/
void Trim(CString &String)
{
    String.TrimLeft();
    String.TrimRight();
}

//------------------------------------------------------------------*
/**
 * Function name    CObjProp::CObjProp
 * Description      constructor for class CObjProp
 * @param           DIALOGTYPE DialogType
 * @param           CPropertySheetBase *PropertySheet
 * @param           UINT nIDTemplate
 * @return          -
 * @exception       -
 * @see             -
*/
CObjProp::CObjProp(DIALOGTYPE DialogType, 
                   CPropertySheetBase *PropertySheet,
                   UINT nIDTemplate) :
    CPropertyPage(nIDTemplate)
{
    m_PropertySheet = PropertySheet;
    m_DialogType = DialogType;
}

//------------------------------------------------------------------*
/**
 * Function name    CObjProp::~CObjProp
 * Description      destructor for class CObjProp
 * @return          -
 * @exception       -
 * @see             -
*/
CObjProp::~CObjProp()
{
}

//------------------------------------------------------------------*
/**
 * Function name    CObjProp::OnSetActive
 * Description      handler which is called when the page becomes active
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL CObjProp::OnSetActive()
{
    if (m_PropertySheet->IsReadOnly())
    {
        SetReadOnly();
    }
    if (m_PropertySheet->HasBeenApplied() == FALSE)
    {
        SetModified(TRUE);
    }
    return CPropertyPage::OnSetActive();
}


//------------------------------------------------------------------*
/**
 * set read only.
 *
 *  disables all child windows.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjProp::SetReadOnly()
{
    CWnd*   pWndTmp;
    pWndTmp = GetWindow(GW_CHILD);
    if(!pWndTmp)
    {
        return;
    }

    if(pWndTmp->GetSafeHwnd())
    {
        pWndTmp->EnableWindow(FALSE);
    }

    do
    {
        pWndTmp = pWndTmp->GetWindow(GW_HWNDNEXT);
        if(pWndTmp->GetSafeHwnd())
        {
            pWndTmp->EnableWindow(FALSE);
        }
    } while(pWndTmp);
}


//------------------------------------------------------------------*
/**
 * Function name    CObjProp::OnApply
 * Description      handler for the apply button
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL CObjProp::OnApply()
{
    BOOL Res;
    if (((m_DialogType == CVIEWINSERT) || (m_DialogType == CVIEWPROP)) /*&& 
        (GetPageIndex() == 0)*/ &&
        (m_PropertySheet->HasBeenApplied() == FALSE))
    {
        // Do this only for the first page.
        Res = m_PropertySheet->Execute();
        m_PropertySheet->SetActivePage(0);
        //Res = Res & CPropertyPage::OnApply();

//        if (Res == TRUE)
//        {
            m_PropertySheet->SetApplied();
//            SetModified(FALSE);
//        }
//        else
//        {
//            m_PropertySheet->ResetApplied();
//            SetModified(TRUE);
//        }

        return Res;
    }
//    else
//    {
//        m_PropertySheet->ResetApplied();
//        return TRUE;
//    }
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * Function name    CObjProp::DisableDialogItem
 * Description      disables the specified dialog item
 * @param           UINT DialogItemId
 * @return          CWnd* 
 * @exception       -
 * @see             -
*/
CWnd* CObjProp::DisableDialogItem(UINT DialogItemId)
{
    CWnd* pWnd = this->GetDlgItem(DialogItemId);
    ASSERT(pWnd != NULL);
    pWnd->ModifyStyle(0, WS_DISABLED);
	return pWnd;
}

//------------------------------------------------------------------*
/**
 * Function name    CObjProp::ErrorBox
 * Description      displays a message bos
 * @param           DWORD Error
 * @param           CString ErrorMessageSuffix
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjProp::ErrorBox(DWORD Error, CString ErrorMessageSuffix)
{
    LPVOID lpMsgBuf;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
                  FORMAT_MESSAGE_FROM_SYSTEM | 
                  FORMAT_MESSAGE_IGNORE_INSERTS,
                  NULL,
                  Error,
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  (LPTSTR) &lpMsgBuf,
                  0,
                  NULL);
    CString Message = (LPTSTR)lpMsgBuf + ErrorMessageSuffix;
    ::AfxMessageBox(Message, MB_OK | MB_ICONINFORMATION);
    LocalFree(lpMsgBuf);
}

//------------------------------------------------------------------*
/**
 * Function name    CObjProp::GetPageIndex
 * Description      returns the index of this property page in the property sheet 
 * @return          int
 * @exception       -
 * @see             -
*/
int CObjProp::GetPageIndex()
{
    return m_PropertySheet->GetPageIndex(this);
}

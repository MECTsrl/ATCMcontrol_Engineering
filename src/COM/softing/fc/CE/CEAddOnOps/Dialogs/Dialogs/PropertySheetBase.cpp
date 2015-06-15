
#include "stdafx.h"
#include "resource.h"
#include "PropertySheetBase.h"
#include "Operation.h"
#include "extern.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name    CPropertySheetBase::CPropertySheetBase
 * Description      constructor for class CPropertySheetBase
 * @param           DIALOGTYPE DialogType
 * @param           LPCTSTR pszCaption
 * @return          -
 * @exception       -
 * @see             -
*/
CPropertySheetBase::CPropertySheetBase(DIALOGTYPE DialogType, LPCTSTR pszCaption) :
    CPropertySheet(pszCaption)
{
	m_psh.dwFlags |= PSH_HASHELP;	  // activate help button

    if (DialogType == CVIEWINSERT)
    {
        ResetApplied();
    }
    else
    {
        m_psh.dwFlags |= PSH_NOAPPLYNOW;  // no apply button
    }

    // ++++ HACK HACK HACK HACK HACK ++++++
    // Disable APPLY button always, APPLY handling not correct!!!!!!!
    m_psh.dwFlags |= PSH_NOAPPLYNOW;

    m_pOperation = NULL;
    m_bGenerateHelp = FALSE;
}

//------------------------------------------------------------------*
/**
 * Function name    CPropertySheetBase::~CPropertySheetBase
 * Description      destructor for class CPropertySheetBase
 * @return          -
 * @exception       -
 * @see             -
*/
CPropertySheetBase::~CPropertySheetBase()
{
}

//------------------------------------------------------------------*
/**
 * Function name    CPropertySheetBase::SetOperation
 * Description      sets the member m_pOperation
 *                  This member implements the operation corresponding to this dialog.
 * @param           COperation *Operation
 * @return          -
 * @exception       -
 * @see             -
*/
void CPropertySheetBase::SetOperation(COperation *Operation)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    if (Operation != NULL)
    {
        m_pOperation = Operation;
        SetProjInfo(m_pOperation->GetProjInfo());
    }
}

//------------------------------------------------------------------*
/**
 * Function name    CPropertySheetBase::DoModal
 * Description      invokes the modal dialog 
 * @return          int
 * @exception       -
 * @see             -
*/
int CPropertySheetBase::DoModal()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    return CPropertySheet::DoModal();
}

//------------------------------------------------------------------*
/**
 * Function name    CPropertySheetBase::Execute
 * Description      calls the execute method of the member m_pOperation
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL CPropertySheetBase::Execute()
{
    BOOL Res;
    if (m_pOperation != NULL)
    {
        Res = m_pOperation->Execute();
        return Res;
    }
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * Function name    CPropertySheetBase::OnInitDialog
 * Description      message handler for the WM_INITDIALOG message
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL CPropertySheetBase::OnInitDialog() 
{
    if (m_pOperation != NULL)
    {
        m_pOperation->FillDialog();
    }
    CPropertySheet::OnInitDialog();

    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//------------------------------------------------------------------*
/**
 * is read only?
 *
 * @param           -
 * @return          is property sheet read only?
 * @exception       -
 * @see             -
*/
BOOL CPropertySheetBase::IsReadOnly()
{
    return m_pOperation->IsSourceReadOnly();
}



//------------------------------------------------------------------*
/**
 * Function name    CPropertySheetBase::SetApplied
 * Description      sets the member m_Applied to true
 * @return          -
 * @exception       -
 * @see             -
*/
void CPropertySheetBase::SetApplied()
{
    m_Applied = TRUE;
}


//------------------------------------------------------------------*
/**
 * Function name    CPropertySheetBase::ResetApplied
 * Description      sets the member m_Applied to false
 * @return          -
 * @exception       -
 * @see             -
*/
void CPropertySheetBase::ResetApplied()
{
    m_Applied = FALSE;
}

//------------------------------------------------------------------*
/**
 * Function name    CPropertySheetBase::ResetApplied
 * Description      returns the value of the member m_Applied
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL CPropertySheetBase::HasBeenApplied()
{
    return m_Applied;
}


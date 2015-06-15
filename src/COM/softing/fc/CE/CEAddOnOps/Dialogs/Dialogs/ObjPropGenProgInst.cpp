#include "stdafx.h"
#include "Dialogs.h"
#include "ObjPropGenProgInst.h"
#include "extern.h"
#include "SyntaxCheck.h"

#include "DialogProgram.h"
#include "InsertProgram.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenProgInst::CObjPropGenProgInst
 * Description      constructor for class CObjPropGenProgInst
 * @param           DIALOGTYPE DialogType
 * @param           CPropertySheetBase *PropertySheet
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropGenProgInst::CObjPropGenProgInst(DIALOGTYPE DialogType, 
                                 CPropertySheetBase *PropertySheet) : 
    CObjPropGenBase(DialogType,
                    IDD_OBJPROP_GEN_PROGINST,
                    PropertySheet)
{
	m_ProgramClass = _T("");
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenProgInst::~CObjPropGenProgInst
 * Description      destructor for class CObjPropGenProgInst
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropGenProgInst::~CObjPropGenProgInst()
{
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenProgInst::DoDataExchange
 * Description      exchanges and validates dialog data
 * @param           CDataExchange* pDX
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjPropGenProgInst::DoDataExchange(CDataExchange* pDX)
{
	CObjPropGenBase::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_PROGCLASS, m_ProgramClass);
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenProgInst::OnInitDialog
 * Description      message handler for the WM_INITDIALOG message
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL CObjPropGenProgInst::OnInitDialog() 
{
	CObjPropGenBase::OnInitDialog();
    CComboBox*  pProgClassCombo = (CComboBox*)GetDlgItem(IDC_PROGCLASS);

    if(pProgClassCombo)
    {
        if (m_DialogType == IVIEWPROP)
        {
            pProgClassCombo->EnableWindow(FALSE);
        }
        if(m_DialogType == CVIEWINSERT)
        {
            FillProgClassesComboBox(); 
        }
        else
        {
            FillProgClassesComboBox(); 
            // Call after the combobox is filled with items, to display the value of m_FbmemberType!
            // UpdateData(FALSE);
        }
    }

    
    UpdateData(FALSE);
    
    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenProgInst::OnKillActive
 * Description      is called when this property page is no longer the active page
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL CObjPropGenProgInst::OnKillActive()
{
    UpdateData(TRUE);
   	Trim(m_ProgramClass);
    UpdateData(FALSE);

    if (IsIdentifier(m_ProgramClass) == FALSE)
    {
        CString strMessage;
        strMessage.Format(IDS_PROG_NAME_INVALID, m_Name);
        ::AfxMessageBox(strMessage, MB_ICONWARNING);
        return FALSE;
    }

    return CObjPropGenBase::OnKillActive();
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenProgInst::FillProgClassesComboBox
 * Description      fills the combobox
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjPropGenProgInst::FillProgClassesComboBox(const CString* pstrNewProg /*=NULL*/)
{
    int iNewProg = -1;

    CComboBox * pCBox = (CComboBox *)(this->GetDlgItem(IDC_PROGCLASS));
    ASSERT(NULL != pCBox);
    pCBox->ResetContent();

    const TCHAR* const *ProgClasses = GetProgramClasses();
    int iIndex = 0;
    int iCount = 0;
    CString strProgName = ProgClasses[iCount];
    while(!strProgName.IsEmpty())
	{
        iIndex = pCBox->AddString(strProgName);
        pCBox->SetItemData(iIndex, (DWORD)iCount);
        if(pstrNewProg)
        {
            if(strProgName.CompareNoCase(*pstrNewProg) == 0)
            {
                m_ProgramClass = strProgName;
                iNewProg = iIndex;
            }
        }
        iCount++;
        strProgName = ProgClasses[iCount];
    }

    // if combo box is not empty select either new program or 1. program
    if(iCount > 0)
    {
        if(iNewProg >= 0)
        {
            pCBox->SetCurSel(iNewProg);
        }
        else
        {
            pCBox->SetCurSel(0);
        }
    }
}

//------------------------------------------------------------------*
/**
 * Function name			CObjPropGenProgInst::OnNewProg
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CObjPropGenProgInst::OnNewProg()
{
    UpdateData(TRUE);

    CDialogProgram *DFB;
    DFB = CDialogProgram::CreateNewObject(CVIEWINSERT);
    
    ICEProjInfo* pICEProjectInfo = m_PropertySheet->m_pOperation->m_pICEProjectInfo;
    
    CComBSTR     sAdditionalInfo = m_PropertySheet->m_pOperation->m_sAdditionalInfo;

    BSTR sProj;
    HRESULT hRes = pICEProjectInfo->getProjectName(&sProj);
    ASSERT(hRes==S_OK);
    CString strHelp = sProj; 
    SysFreeString(sProj);
    CString strId = strHelp;
    strHelp += ".4cp"; 
    CComBSTR     sSourceFile = strHelp;

    strId = _T("/") + strId + _T(".") + _T("CLASSVIEW") + _T("/") + _T("Classes.Programs");
    CComBSTR     sId = strId;

    CInsertProgram IFB(DFB,
                       sSourceFile,
                       sId,
                       pICEProjectInfo,
                       sAdditionalInfo);
    DFB->SetOperation(&IFB);
    IFB.GetDialog()->SetName(m_Name);
    DFB->DoModal();

    CString strNewProgram = IFB.m_sName;
    FillProgClassesComboBox(&strNewProgram);

    CDialogProgram::DeleteObject(DFB);
}


BEGIN_MESSAGE_MAP(CObjPropGenProgInst, CObjPropGenBase)
    ON_BN_CLICKED(IDC_NEWPROG, OnNewProg)
END_MESSAGE_MAP()



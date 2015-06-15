


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "DllInterface.h"
#include "InsertConstantFile.h"
#include "DialogConstantFile.h"
#include "CEContainer.h"
#include "resource.h"
//----  Static Initializations:   ----------------------------------*


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//------------------------------------------------------------------*
/**
 * Function name			CInsertConstantFile::CInsertConstantFile
 * Description			
 * @param			CPropertySheetBase *Dialog
 * @param			BSTR sSourceFile
 * @param			BSTR sId
 * @param			ICEProjInfo *pICEProjectInfo
 * @param			BSTR sAdditionalInfo
 * @return			
 * @exception			
 * @see			
*/
CInsertConstantFile::CInsertConstantFile(CPropertySheetBase *Dialog,
                                           BSTR sSourceFile,
                                           BSTR sId,
                                           ICEProjInfo *pICEProjectInfo,
                                           BSTR sAdditionalInfo) :
    COperation(Dialog, sSourceFile, sId, pICEProjectInfo, sAdditionalInfo)
{
    m_sName = _T("ConstantFile");
}


//------------------------------------------------------------------*
/**
 * Function name			CInsertConstantFile::~CInsertConstantFile
 * Description			
 * @return			
 * @exception			
 * @see			
*/
CInsertConstantFile::~CInsertConstantFile()
{
}


//------------------------------------------------------------------*
/**
 * Function name			CInsertConstantFile::Execute
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
BOOL CInsertConstantFile::Execute()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    Init();
    SetFileName();
    BOOL bRes = !DoesFileExist(m_sFileName, TRUE);
    if(bRes)
    {
        GenConstantFile(m_ConstantFileText);
        WriteFile(m_ConstantFileText);
        InformProjectManager();
    }
    m_ConstantFileText.Empty();
    m_aVarList.RemoveAll();
    return bRes;
}


//------------------------------------------------------------------*
/**
 * Function name			CInsertConstantFile::GenConstantFile
 * Description			
 * @param			CString & StrOut
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertConstantFile::GenConstantFile(CString & StrOut)
{
    CString StrHelp;
    CString Strlf = NEWLINE;
    StrOut = _T("");

    StrHelp.LoadString(IDS_KEY_COMMENT);
    StrOut += StrHelp;
    StrHelp.LoadString(IDS_CONST_FILE);
    StrOut += StrHelp;
    StrHelp.LoadString(IDS_KEY_ENDCOMMENT);
    StrOut += StrHelp;
    StrOut += Strlf;

    GetStdAttribString(StrOut);
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertConstantFile::SetFileName
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertConstantFile::SetFileName()
{
    m_sFileName = m_sName + ".cst"; //Constant file
}


//------------------------------------------------------------------*
/**
 * Function name			CInsertConstantFile::GetDialog
 * Description			
 * @return			CDialogFunctionBlock* 
 * @exception			
 * @see			
*/
CDialogConstantFile* CInsertConstantFile::GetDialog()
{
    return (CDialogConstantFile*) m_pDialog;
}


//------------------------------------------------------------------*
/**
 * Function name			CInsertConstantFile::Init
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertConstantFile::Init()
{
    m_sAlias          = GetDialog()->GetAlias();
    m_sName           = GetDialog()->GetName();
    m_sUsrText[0]     = GetDialog()->GetText1();
    m_sUsrText[1]     = GetDialog()->GetText2();
    m_sUsrText[2]     = GetDialog()->GetText3();
    m_sHelpUrl        = GetDialog()->GetHelpUrl();
    m_sUrl            = GetDialog()->GetUrl();
    
    COperation::Init();
    SetPouDT(TRUE); //TRUE for Insert (FALSE for Prop)
}


//------------------------------------------------------------------*
/**
 * Function name			CInsertConstantFile::FillDialog
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertConstantFile::FillDialog()
{
    //Standart name
    CString sName = GetNameProposal(m_sName);

    GetDialog()->SetCreateUser(m_sCreateUser);
    GetDialog()->SetCreateDate(m_sCreateDate);
    GetDialog()->SetLastModUser(m_sLastModUser);
    GetDialog()->SetLastModDate(m_sLastModDate);
    GetDialog()->SetName(sName);
}


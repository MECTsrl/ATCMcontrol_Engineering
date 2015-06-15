


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "DllInterface.h"
#include "InsertGlobVarFile.h"
#include "DialogGlobVarFile.h"
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
 * Function name			CInsertGlobVarFile::CInsertGlobVarFile
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
CInsertGlobVarFile::CInsertGlobVarFile(CPropertySheetBase *Dialog,
                                           BSTR sSourceFile,
                                           BSTR sId,
                                           ICEProjInfo *pICEProjectInfo,
                                           BSTR sAdditionalInfo) :
    COperation(Dialog, sSourceFile, sId, pICEProjectInfo, sAdditionalInfo)
{
    m_sName = _T("GlobVarFile");
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertGlobVarFile::~CInsertGlobVarFile
 * Description			
 * @return			
 * @exception			
 * @see			
*/
CInsertGlobVarFile::~CInsertGlobVarFile()
{
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertGlobVarFile::Execute
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
BOOL CInsertGlobVarFile::Execute()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    Init();
    SetFileName();
    BOOL bRes = !DoesFileExist(m_sFileName, TRUE);
    if(bRes)
    {
        GenGlobVarFile(m_GlobVarFileText);
        WriteFile(m_GlobVarFileText);
        InformProjectManager();
    }
    m_GlobVarFileText.Empty();
    m_aVarList.RemoveAll();
    return bRes;
}


//------------------------------------------------------------------*
/**
 * Function name			CInsertGlobVarFile::GenConstantFile
 * Description			
 * @param			CString & StrOut
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertGlobVarFile::GenGlobVarFile(CString & StrOut)
{
    CString StrHelp;
    CString Strlf = NEWLINE;
    StrOut = _T("");

    StrHelp.LoadString(IDS_KEY_COMMENT);
    StrOut += StrHelp;
    StrHelp.LoadString(IDS_GLOBVAR_FILE);
    StrOut += StrHelp;
    StrHelp.LoadString(IDS_KEY_ENDCOMMENT);
    StrOut += StrHelp;
    StrOut += Strlf;

    GetStdAttribString(StrOut);
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertGlobVarFile::SetFileName
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertGlobVarFile::SetFileName()
{
    m_sFileName = m_sName + _T(".gvl"); //GlobVar file
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertGlobVarFile::GetDialog
 * Description			
 * @return			CDialogConstantFile* 
 * @exception			
 * @see			
*/
CDialogGlobVarFile* CInsertGlobVarFile::GetDialog()
{
    return (CDialogGlobVarFile*) m_pDialog;
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertGlobVarFile::Init
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertGlobVarFile::Init()
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
 * Function name			CInsertGlobVarFile::FillDialog
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertGlobVarFile::FillDialog()
{
    //Standart name
    CString sName = GetNameProposal(m_sName);

    GetDialog()->SetCreateUser(m_sCreateUser);
    GetDialog()->SetCreateDate(m_sCreateDate);
    GetDialog()->SetLastModUser(m_sLastModUser);
    GetDialog()->SetLastModDate(m_sLastModDate);
    GetDialog()->SetName(sName);
}



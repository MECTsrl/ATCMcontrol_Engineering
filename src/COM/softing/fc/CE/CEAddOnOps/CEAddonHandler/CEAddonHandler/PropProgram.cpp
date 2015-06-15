

//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "DllInterface.h"
#include "PropProgram.h"
#include "DialogProgram.h"
#include "CEContainer.h"

//----  Static Initializations:   ----------------------------------*


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//------------------------------------------------------------------*
/**
 * Function name	CPropProgram::CPropProgram
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

CPropProgram::CPropProgram(CPropertySheetBase *Dialog,
                                       BSTR sSourceFile,
                                       BSTR sId,
                                       ICEProjInfo *pICEProjectInfo,
                                       BSTR sAdditionalInfo) :
    COperation(Dialog, sSourceFile, sId, pICEProjectInfo, sAdditionalInfo)
{
    m_Program = NULL;
}


//------------------------------------------------------------------*
/**
 * Function name	CPropProgram::~CPropProgram
 * Description			
 * @return			
 * @exception			
 * @see			
*/
CPropProgram::~CPropProgram()
{
    delete m_Program;
    if(m_pMembers)
    {
        ::SafeArrayDestroy(m_pMembers);
        m_pMembers = NULL;
    }
}


//------------------------------------------------------------------*
/**
 * Function name	CPropProgram::Execute
 * Description		This function will be executed after clicking on the
 *                  the ok button.
 * @return			void 
 * @exception			
 * @see			
*/
BOOL CPropProgram::Execute()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    Init();
    InsertVarList(m_ProgramText, m_Program);
    InsertSysComment2(m_ProgramText, m_Program);
    // SIS: bug fix d-2019
    // create help file if template is not empty
    if(!m_sHelpTemplate.IsEmpty())
    {
        CreateUsrHelp();
    }
    WriteHeader(m_ProgramText);
    InformProjectManager();

    return TRUE;
}

//------------------------------------------------------------------*
/**
 * Function name	CPropProgram::SetFileName
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CPropProgram::SetFileName()
{
    //File allready exists
    m_sFileName = m_sSourceFile;
}


//------------------------------------------------------------------*
/**
 * Function name	CPropProgram::GetDialog
 * Description		casts the m_pDialog to the associate type.	
 * @return			CDialogProgram* 
 * @exception			
 * @see			
*/
CDialogProgram* CPropProgram::GetDialog()
{
    return (CDialogProgram*) m_pDialog;
}


//------------------------------------------------------------------*
/**
 * Function name	CPropProgram::Init
 * Description		Initialize all members	
 * @return			void 
 * @exception			
 * @see			
*/
void CPropProgram::Init()
{
    m_sAlias          = GetDialog()->GetAlias();
    m_sName           = GetDialog()->GetName();
    m_sUsrText[0]     = GetDialog()->GetText1();
    m_sUsrText[1]     = GetDialog()->GetText2();
    m_sUsrText[2]     = GetDialog()->GetText3();
    m_sHelpUrl        = GetDialog()->GetHelpUrl();
    m_sUrl            = GetDialog()->GetUrl();
    m_sHelpTemplate   = GetDialog()->GetPathOfSelectedTemplate();

    if(m_pMembers)
    {
        ::SafeArrayDestroy(m_pMembers);
        m_pMembers = NULL;
    }
    m_pMembers        = GetDialog()->GetMembers();

    m_aVarList.RemoveAll();
    COperation::Init();
    SetPouDT(TRUE); //TRUE for Insert (FALSE for Prop)
}

//------------------------------------------------------------------*
/**
 * Function name			CPropFunctionBlock::InformProjectManager
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CPropProgram::InformProjectManager()
{
    HRESULT hRes = 0;
    BSTR strFileName = m_sFileName.AllocSysString();
    if(!m_sHelpUrl.IsEmpty())
    {
        CreateUsrHelpLink();
    }
    hRes = m_pICEProjectInfo->fileChangedHint(strFileName);
    SysFreeString(strFileName);
   	ASSERT(hRes==S_OK);
}

//------------------------------------------------------------------*
/**
 * Function name	CPropProgram::FillDialog
 * Description		Called from OnInitDialog() to fill the dialog Items	
 * @return			void 
 * @exception			
 * @see			
*/
void CPropProgram::FillDialog()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    SetFileName();
    string buf;
    ReadFileContent(CString(m_ProjectPath) + _T("\\") + m_sFileName, buf);
    m_ProgramText = buf.c_str();
    m_Program = (Program *)ParsePou(m_ProgramText, m_Program);
    InitFromFile(m_Program);
    COperation::FillDialog();
    GetDialog()->SetCreateUser(m_sCreateUser);
    GetDialog()->SetCreateDate(m_sCreateDate);
    GetDialog()->SetLastModUser(m_sLastModUser);
    GetDialog()->SetLastModDate(m_sLastModDate);
    GetDialog()->SetAlias(m_sAlias);
    GetDialog()->SetName(m_sName);
    GetDialog()->SetText1(m_sUsrText[0]);
	GetDialog()->SetText2(m_sUsrText[1]);
	GetDialog()->SetText3(m_sUsrText[2]);
	GetDialog()->SetHelpUrl(m_sHelpUrl);
    GetDialog()->SetUrl(m_sUrl);

    GetDialog()->SetMembers(m_pMembers);
    m_pMembers = NULL;
}




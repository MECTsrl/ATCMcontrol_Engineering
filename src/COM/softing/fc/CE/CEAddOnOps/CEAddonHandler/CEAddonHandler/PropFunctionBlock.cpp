
#include "stdafx.h"
#include "DllInterface.h"
#include "PropFunctionBlock.h"
#include "DialogFunctionBlock.h"
#include "CEContainer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CPropFunctionBlock::CPropFunctionBlock(CPropertySheetBase *Dialog,
                                       BSTR sSourceFile,
                                       BSTR sId,
                                       ICEProjInfo *pICEProjectInfo,
                                       BSTR sAdditionalInfo) :
    COperation(Dialog, sSourceFile, sId, pICEProjectInfo, sAdditionalInfo)
{
    m_FunctionBlock = NULL;
}

CPropFunctionBlock::~CPropFunctionBlock()
{
    delete m_FunctionBlock;
    if(m_pMembers)
    {
        ::SafeArrayDestroy(m_pMembers);
        m_pMembers = NULL;
    }
}

BOOL CPropFunctionBlock::Execute()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    Init();
    InsertVarList(m_FunctionBlockText, m_FunctionBlock);
    InsertSysComment2(m_FunctionBlockText,m_FunctionBlock);
    // SIS: bug fix d-2019
    // create help file if template is not empty
    if(!m_sHelpTemplate.IsEmpty())
    {
        CreateUsrHelp();
    }
    WriteHeader(m_FunctionBlockText);
    InformProjectManager();

    return TRUE;
}

void CPropFunctionBlock::Init()
{
    CDialogFunctionBlock*   pDialog = GetDialog();
    ASSERT(pDialog);
    if(!pDialog)
    {
        return;
    }

    m_sAlias          = pDialog->GetAlias();
    m_sName           = pDialog->GetName();
    m_sUsrText[0]     = pDialog->GetText1();
    m_sUsrText[1]     = pDialog->GetText2();
    m_sUsrText[2]     = pDialog->GetText3();
    m_sHelpUrl        = pDialog->GetHelpUrl();
    m_sUrl            = pDialog->GetUrl();
    m_sHelpTemplate   = GetDialog()->GetPathOfSelectedTemplate();

    if(m_pMembers)
    {
        ::SafeArrayDestroy(m_pMembers);
        m_pMembers = NULL;
    }
    m_pMembers        = pDialog->GetMembers();

    m_aVarList.RemoveAll();

    COperation::Init();
    SetPouDT(TRUE); //TRUE for Insert (FALSE for Prop)
}

void CPropFunctionBlock::SetFileName()
{
    //File allready exists
    m_sFileName = m_sSourceFile;
}

void CPropFunctionBlock::InformProjectManager()
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


CDialogFunctionBlock* CPropFunctionBlock::GetDialog()
{
    return (CDialogFunctionBlock*) m_pDialog;
}

void CPropFunctionBlock::FillDialog()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
 
    SetFileName();
    string buf;
    ReadFileContent(CString(m_ProjectPath) + _T("\\") + m_sFileName, buf);
    m_FunctionBlockText = buf.c_str();
    m_FunctionBlock = (FunctionBlock *)ParsePou(m_FunctionBlockText, m_FunctionBlock);
    InitFromFile(m_FunctionBlock);
    COperation::FillDialog();
    CDialogFunctionBlock* pDialog = GetDialog();
    ASSERT(pDialog);
    if(!pDialog)
    {
        return;
    }

    pDialog->SetCreateUser(m_sCreateUser);
    pDialog->SetCreateDate(m_sCreateDate);
    pDialog->SetLastModUser(m_sLastModUser);
    pDialog->SetLastModDate(m_sLastModDate);
    pDialog->SetAlias(m_sAlias);
    pDialog->SetName(m_sName);
    pDialog->SetText1(m_sUsrText[0]);
	pDialog->SetText2(m_sUsrText[1]);
	pDialog->SetText3(m_sUsrText[2]);
	pDialog->SetHelpUrl(m_sHelpUrl);
	pDialog->SetUrl(m_sUrl);

    pDialog->SetMembers(m_pMembers);
    m_pMembers = NULL;
}



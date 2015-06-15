


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "DllInterface.h"
#include "PropFunction.h"
#include "DialogFunction.h"
#include "CEContainer.h"
//----  Static Initializations:   ----------------------------------*

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CPropFunction::CPropFunction(CPropertySheetBase *Dialog,
                                       BSTR sSourceFile,
                                       BSTR sId,
                                       ICEProjInfo *pICEProjectInfo,
                                       BSTR sAdditionalInfo) :
    COperation(Dialog, sSourceFile, sId, pICEProjectInfo, sAdditionalInfo)
{
    m_Function = NULL;
}

CPropFunction::~CPropFunction()
{
    if(m_Function)
    {
        delete m_Function;
        m_Function = NULL;
    }
    if(m_pMembers)
    {
        ::SafeArrayDestroy(m_pMembers);
        m_pMembers = NULL;
    }
}

BOOL CPropFunction::Execute()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    Init();
    InsertVarList(m_FunctionText, m_Function);
    InsertFunctionType(m_FunctionText);
    InsertSysComment2(m_FunctionText, m_Function);
    // SIS: bug fix d-2019
    // create help file if template is not empty
    if(!m_sHelpTemplate.IsEmpty())
    {
        CreateUsrHelp();
    }
    WriteHeader(m_FunctionText);
    InformProjectManager();

    return TRUE;
}

void CPropFunction::SetFileName()
{
    //File allready exists
    m_sFileName = m_sSourceFile;
}

CDialogFunction* CPropFunction::GetDialog()
{
    return (CDialogFunction*) m_pDialog;
}

void CPropFunction::Init()
{
    m_sAlias          = GetDialog()->GetAlias();
    m_sName           = GetDialog()->GetName();
    m_sUsrText[0]     = GetDialog()->GetText1();
    m_sUsrText[1]     = GetDialog()->GetText2();
    m_sUsrText[2]     = GetDialog()->GetText3();
    m_sHelpUrl        = GetDialog()->GetHelpUrl();
    m_sUrl            = GetDialog()->GetUrl();
    m_sFunctionType   = GetDialog()->GetReturnType();
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

void CPropFunction::InformProjectManager()
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

void CPropFunction::FillDialog()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    SetFileName();
    string buf;
    ReadFileContent(CString(m_ProjectPath) + _T("\\") + m_sFileName, buf);
    m_FunctionText = buf.c_str();
    m_Function = (Function *)ParsePou(m_FunctionText, m_Function);
    InitFromFile(m_Function);
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

    GetDialog()->SetReturnType(m_sFunctionType);
}

void CPropFunction::InitFromFile(POU* Pou)
{
    ASSERT(Pou);
    COperation::InitFromFile(Pou);
    string FunctionType = m_Function->getFunctionType();
    m_sFunctionType = FunctionType.c_str();
    StringPosRange TypeRange = m_Function->getFunctionTypeRange();
    StringPosRange NameRange = m_Function->getNameRange();
    m_iFunctionTypePos = NameRange.getLast();

    if(!m_sFunctionType.IsEmpty())
    {
        m_iFunctionTypeLength = TypeRange.getLast() - m_iFunctionTypePos;    
    }
    else
    {
        m_iFunctionTypeLength = 0;
    }
}

void CPropFunction::InsertFunctionType(CString & FunctionText)
{
    CString StrHelp,Strlf(NEWLINE),StrOut(_T("")),StrIndent(SPACETAB);
    
    FunctionText.Delete(m_iFunctionTypePos, m_iFunctionTypeLength);
    
    if(!m_sFunctionType.IsEmpty()) 
    {
        StrHelp.LoadString(IDS_KEY_COLON);
        StrOut += _T(" ");      // 25.08.06 SIS
        StrOut += StrHelp;
        StrOut += _T(" ");      // 25.08.06 SIS
        StrOut += m_sFunctionType;
        //StrOut += Strlf;      // 25.08.06 SIS
    }
    FunctionText.Insert(m_iFunctionTypePos, StrOut);
}


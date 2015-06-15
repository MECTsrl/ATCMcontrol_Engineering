


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "DllInterface.h"
#include "PropProgramInst.h"
#include "DialogProgInst.h"
#include "CEContainer.h"
//----  Static Initializations:   ----------------------------------*


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CPropProgramInst::CPropProgramInst(CPropertySheetBase *Dialog,
                                       BSTR sSourceFile,
                                       BSTR sId,
                                       ICEProjInfo *pICEProjectInfo,
                                       BSTR sAdditionalInfo) :
    COperation(Dialog, sSourceFile, sId, pICEProjectInfo, sAdditionalInfo)
{
    m_Resource = NULL;
    m_iPos = 0;
    m_iLength = 0;
}

CPropProgramInst::~CPropProgramInst()
{
    if(m_Resource)
    {
        delete m_Resource;
        m_Resource = NULL;
    }
}

BOOL CPropProgramInst::Execute()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    Init();
    GenProgramInst(m_ProgramInstText);
    InsertProgramInst();
    WriteFile(m_ResourceText);
    InformProjectManager();

    return TRUE;
}

void CPropProgramInst::SetFileName()
{
    //File allready exists
    m_sFileName = m_sSourceFile;
}

void CPropProgramInst::Init()
{
    if(!m_bAutomationMode)
    {
        m_sAlias          = GetDialog()->GetAlias();
        m_sUsrText[0]     = GetDialog()->GetText1();
	    m_sUsrText[1]     = GetDialog()->GetText2();
	    m_sUsrText[2]     = GetDialog()->GetText3();
	    m_sHelpUrl        = GetDialog()->GetHelpUrl();
	    m_sUrl            = GetDialog()->GetUrl();
        m_sClass          = GetDialog()->GetProgClass();
    }
}


void CPropProgramInst::InformProjectManager()
{
    HRESULT hRes = 0;
    BSTR strFileName = m_sFileName.AllocSysString();
    hRes = m_pICEProjectInfo->fileChangedHint(strFileName);
    SysFreeString(strFileName);
   	ASSERT(hRes==S_OK);
}


CDialogProgInst* CPropProgramInst::GetDialog()
{
    return (CDialogProgInst*) m_pDialog;
}

void CPropProgramInst::FillDialog()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    SetFileName();
    string buf;
    ReadFileContent(CString(m_ProjectPath) + _T("\\") + m_sFileName, buf);
    m_ResourceText = buf.c_str();
    m_sName = GetNameFromId();
    m_Resource = (Resource *)ParsePou(m_ResourceText, m_Resource);
    InitFromFile(m_Resource);

    if(!m_bAutomationMode)
    {
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
        GetDialog()->SetProgClass(m_sClass);
    }
}

void CPropProgramInst::InitFromFile(Resource* Pou)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    ASSERT(Pou);

    COperation::Init();
    SetPouDT(FALSE); //TRUE for Insert (FALSE for Prop)

    const program_configurations_type ProgConfigs = Pou->getProgramConfigurations();
    if(ProgConfigs.size()!= 0)
    {
        program_configurations_citer iter;
        for (iter = ProgConfigs.begin(); iter < ProgConfigs.end(); iter++)
        {
            ASSERT(iter);
            string Name = (*iter)->getName();
            CString sName = Name.c_str();
            if(!sName.CompareNoCase(m_sName))
            {
                const SysComment* pSysCmt = (*iter)->GetFileSystemComment();
                if(pSysCmt)
                {
                    CString Dummy;
                    string dummy;
                    Dummy.LoadString(IDS_KEY_ALIAS1);
                    dummy = Dummy;
                    m_sAlias          = (pSysCmt->avm_get(dummy)).c_str();
                    Dummy.LoadString(IDS_KEY_USERTEXT1);
                    dummy = Dummy;
                    m_sUsrText[0]     = (pSysCmt->avm_get(dummy)).c_str();
                    Dummy.LoadString(IDS_KEY_USERTEXT2);
                    dummy = Dummy;
                    m_sUsrText[1]     = (pSysCmt->avm_get(dummy)).c_str();
                    Dummy.LoadString(IDS_KEY_USERTEXT3);
                    dummy = Dummy;
                    m_sUsrText[2]     = (pSysCmt->avm_get(dummy)).c_str();
                    Dummy.LoadString(IDS_KEY_HELPURL);
                    dummy = Dummy;
                    m_sHelpUrl        = (pSysCmt->avm_get(dummy)).c_str();
                    Dummy.LoadString(IDS_KEY_URL);
                    dummy = Dummy;
                    m_sUrl            = (pSysCmt->avm_get(dummy)).c_str();
                }
                m_sTaskName = ((*iter)->getTaskName()).c_str();
                m_sClass   = ((*iter)->getProgramClass()).c_str();
                
                StringPosRange Range = (*iter)->getRange();
                m_iPos = Range.getFirst() - 1;
                m_iLength = Range.getLength() + 1;
            }
        }
    }
}

void CPropProgramInst::GenProgramInst(CString & StrOut)
{
    CString StrHelp;
    CString Strlf(NEWLINE),StrIndent(SPACETAB), strQuote;

    StrHelp.Format(IDS_KEY_PRGINST, m_sName);
    StrOut += StrHelp;
    if(!m_sObjGUID.IsEmpty() || !m_sUsrText[0].IsEmpty() || !m_sUsrText[1].IsEmpty()
        || !m_sUsrText[2].IsEmpty() || !m_sAlias.IsEmpty() || !m_sUrl.IsEmpty())
    {
        StrOut += Strlf;
        GetStdAttribString(StrOut, 4);
        StrOut.Delete((StrOut.GetLength() -1), 1);
        StrOut += StrIndent;
        StrOut += StrIndent;
        StrOut += StrIndent;
    }
    StrHelp.Format(IDS_KEY_PRGINSTTAIL,m_sTaskName, m_sClass);
    StrOut += StrHelp;
    StrOut += Strlf;
}

void CPropProgramInst::InsertProgramInst()
{
    ASSERT(!m_ResourceText.IsEmpty());
    m_ResourceText.Delete(m_iPos, m_iLength);
    m_ResourceText.Insert(m_iPos, m_ProgramInstText);
}

void CPropProgramInst::SetProgramClass(const CString& strClass)
{
    m_sClass = strClass;
}


void CPropProgramInst::SetProgramInstName(const CString& strName)
{
    m_sProgInstName = strName;
}


void CPropProgramInst::SetTaskName(const CString& strTask)
{
    m_sTaskName = strTask;
}


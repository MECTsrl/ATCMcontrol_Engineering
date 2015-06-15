


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "DllInterface.h"
#include "PropProject.h"
#include "DialogProj.h"
#include "CEContainer.h"
#include "CESysDef.h"
//----  Static Initializations:   ----------------------------------*


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CPropProject::CPropProject(CPropertySheetBase *Dialog,
                                       BSTR sSourceFile,
                                       BSTR sId,
                                       ICEProjInfo *pICEProjectInfo,
                                       BSTR sAdditionalInfo) :
    COperation(Dialog, sSourceFile, sId, pICEProjectInfo, sAdditionalInfo)
{
   ;
}

CPropProject::~CPropProject()
{
    ;
}

BOOL CPropProject::Execute()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    Init();
    InformProjectManager();
    return TRUE;
}

void CPropProject::Init()
{
    m_sAlias          = GetDialog()->GetAlias();
    m_sName           = GetDialog()->GetName();
    m_sUsrText[0]     = GetDialog()->GetText1();
    m_sUsrText[1]     = GetDialog()->GetText2();
    m_sUsrText[2]     = GetDialog()->GetText3();
    m_sHelpUrl        = GetDialog()->GetHelpUrl();
    m_sUrl            = GetDialog()->GetUrl();
    m_strLocation     = GetDialog()->GetLocation();

    WriteIntoFile();
}

void ExpendBeginEnd(CString & strText)
{
    CString strHelp = strText;
    strHelp.TrimLeft();
    strHelp.TrimRight();
    strText = strHelp;
}
void CPropProject::WriteIntoFile()
{
    //Set the values of the info in the 4cp file
    HRESULT hRes = 0;
    BSTR sSection, sKey, sValue;
    CString strHelp;
    //Section is Attributes
    strHelp = CE_CP_ATTRIBUTES;
    sSection = strHelp.AllocSysString();
        //Alias
    strHelp.LoadString(IDS_KEY_ALIAS1);
    sKey = strHelp.AllocSysString();
    hRes = m_pICEProjectInfo->CPDeleteFromSection(sSection, sKey);
    ExpendBeginEnd(m_sAlias);
    if (!m_sAlias.IsEmpty())
    {
        sValue = m_sAlias.AllocSysString();
        hRes = m_pICEProjectInfo->CPInsertToSection(sSection, sKey, sValue);
        ::SysFreeString(sValue);
    }
    ::SysFreeString(sKey);
        //UserText1
    strHelp.LoadString(IDS_KEY_USERTEXT1);
    sKey = strHelp.AllocSysString();
    hRes = m_pICEProjectInfo->CPDeleteFromSection(sSection, sKey);
    ExpendBeginEnd(m_sUsrText[0]);
    if (!m_sUsrText[0].IsEmpty())
    {
        sValue = m_sUsrText[0].AllocSysString();
        hRes = m_pICEProjectInfo->CPInsertToSection(sSection, sKey, sValue);
        ::SysFreeString(sValue);
    }
    ::SysFreeString(sKey);
        //UserText2
    strHelp.LoadString(IDS_KEY_USERTEXT2);
    sKey = strHelp.AllocSysString();
    hRes = m_pICEProjectInfo->CPDeleteFromSection(sSection, sKey);
    ExpendBeginEnd(m_sUsrText[1]);
    if (!m_sUsrText[1].IsEmpty())
    {
        sValue = m_sUsrText[1].AllocSysString();
        hRes = m_pICEProjectInfo->CPInsertToSection(sSection, sKey, sValue);
        ::SysFreeString(sValue);
    }
    ::SysFreeString(sKey);
        //UserText3
    strHelp.LoadString(IDS_KEY_USERTEXT3);
    sKey = strHelp.AllocSysString();
    hRes = m_pICEProjectInfo->CPDeleteFromSection(sSection, sKey);
    ExpendBeginEnd(m_sUsrText[2]);
    if (!m_sUsrText[2].IsEmpty())
    {
        sValue = m_sUsrText[2].AllocSysString();
        hRes = m_pICEProjectInfo->CPInsertToSection(sSection, sKey, sValue);
        ::SysFreeString(sValue);
    }
    ::SysFreeString(sKey);
        //Url
    strHelp.LoadString(IDS_KEY_URL);
    sKey = strHelp.AllocSysString();
    hRes = m_pICEProjectInfo->CPDeleteFromSection(sSection, sKey);
    ExpendBeginEnd(m_sUrl);
    if (!m_sUrl.IsEmpty())
    {
        sValue = m_sUrl.AllocSysString();
        hRes = m_pICEProjectInfo->CPInsertToSection(sSection, sKey, sValue);
        ::SysFreeString(sValue);
    }
    ::SysFreeString(sKey);
    ::SysFreeString(sSection);
}

void CPropProject::SetFileName()
{
    //File allready exists
    m_sFileName = m_sName + _T(".4cp");
}

void CPropProject::InformProjectManager()
{
    HRESULT hRes = 0;
    SetFileName();
    BSTR strFileName = m_sFileName.AllocSysString();
    hRes = m_pICEProjectInfo->fileChangedHint(strFileName);
    ::SysFreeString(strFileName);
   	ASSERT(hRes==S_OK);
}


CDialogProj* CPropProject::GetDialog()
{
    return (CDialogProj*) m_pDialog;
}

void CPropProject::FillDialog()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    
    InitFromFile();

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
    GetDialog()->SetLocation(m_strLocation);
}

void TrimBeginEnd(CString & strText)
{
    strText.TrimLeft(_T("\t \""));
    strText.TrimRight(_T("\t \""));
}

void CPropProject::InitFromFile()
{
    //Get the values of the info from the 4cp file
    HRESULT hRes = 0;
    BSTR sSection, sKey, sValue;
    CString strHelp;
    //Section is Attributes
    strHelp = CE_CP_ATTRIBUTES;
    sSection = strHelp.AllocSysString();
        //Alias
    strHelp.LoadString(IDS_KEY_ALIAS1);
    sKey = strHelp.AllocSysString();
    hRes = m_pICEProjectInfo->CPGetValueForKeyFromSection(sSection, sKey, &sValue);
    if(hRes == S_OK)
    {
        m_sAlias = sValue;
        TrimBeginEnd(m_sAlias);
        ::SysFreeString(sKey);
        ::SysFreeString(sValue);
    }
        //UserText1
    strHelp.LoadString(IDS_KEY_USERTEXT1);
    sKey = strHelp.AllocSysString();
    hRes = m_pICEProjectInfo->CPGetValueForKeyFromSection(sSection, sKey, &sValue);
    if(hRes == S_OK)
    {
        m_sUsrText[0] = sValue;
        TrimBeginEnd(m_sUsrText[0]);
        ::SysFreeString(sKey);
        ::SysFreeString(sValue);
    }
        //UserText2
    strHelp.LoadString(IDS_KEY_USERTEXT2);
    sKey = strHelp.AllocSysString();
    hRes = m_pICEProjectInfo->CPGetValueForKeyFromSection(sSection, sKey, &sValue);
    if(hRes == S_OK)
    {
        m_sUsrText[1] = sValue;
        TrimBeginEnd(m_sUsrText[1]);
        ::SysFreeString(sKey);
        ::SysFreeString(sValue);
    }
        //UserText3
    strHelp.LoadString(IDS_KEY_USERTEXT3);
    sKey = strHelp.AllocSysString();
    hRes = m_pICEProjectInfo->CPGetValueForKeyFromSection(sSection, sKey, &sValue);
    if(hRes == S_OK)
    {
        m_sUsrText[2] = sValue;
        TrimBeginEnd(m_sUsrText[2]);
        ::SysFreeString(sKey);
        ::SysFreeString(sValue);
    }
        //Url
    strHelp.LoadString(IDS_KEY_URL);
    sKey = strHelp.AllocSysString();
    hRes = m_pICEProjectInfo->CPGetValueForKeyFromSection(sSection, sKey, &sValue);
    if(hRes == S_OK)
    {
        m_sUrl = sValue;
        TrimBeginEnd(m_sUrl);
        ::SysFreeString(sKey);
        ::SysFreeString(sValue);
    }
        //Project name
    hRes = m_pICEProjectInfo->getProjectName(&sValue);
    if(hRes == S_OK)
    {
        m_sName = sValue;
        ::SysFreeString(sValue);
    }
    ::SysFreeString(sSection);
        //Location
    m_strLocation = m_ProjectPath;
    GetCurrentUserName(m_sCreateUser);
    m_sLastModUser = m_sCreateUser;
    SetFileName();
    SetPouDT(FALSE); //TRUE for Insert (FALSE for Prop)
}


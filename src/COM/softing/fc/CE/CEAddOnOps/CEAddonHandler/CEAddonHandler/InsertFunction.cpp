


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "DllInterface.h"
#include "InsertFunction.h"
#include "DialogFunction.h"
#include "CEContainer.h"
//----  Static Initializations:   ----------------------------------*

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name	CInsertFunction::CInsertFunction
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
CInsertFunction::CInsertFunction(CPropertySheetBase *Dialog,
                                 BSTR sSourceFile,
                                 BSTR sId,
                                 ICEProjInfo *pICEProjectInfo,
                                 BSTR sAdditionalInfo) :
    COperation(Dialog, sSourceFile, sId, pICEProjectInfo, sAdditionalInfo)
{
    m_Function = NULL;
    m_sName = _T("Function");
//    CreateGUID();
}


//------------------------------------------------------------------*
/**
 * Function name			CInsertFunction::~CInsertFunction
 * Description			
 * @return			
 * @exception			
 * @see			
*/
CInsertFunction::~CInsertFunction()
{
    if(m_Function)
    {
        delete m_Function;
        m_Function = NULL;
    }
}

//------------------------------------------------------------------*
/**
 * Function name	CInsertFunction::Execute
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
BOOL CInsertFunction::Execute()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    Init();
    SetFileName();
    BOOL bRes = !DoesFileExist(m_sName, FALSE);
    if(bRes)
    {
        GenFunction(m_FunctionText);
        InsertVarList(m_FunctionText, m_Function);
        if (m_FileType == FILETYPE_ST || m_FileType == FILETYPE_IL)
        {
            WriteFile(m_FunctionText);
        }
        else
        {
            Write4grFile();
        }
        InformProjectManager();
    }
    m_FunctionText.Empty();
    m_aVarList.RemoveAll();
    return bRes;
}


//------------------------------------------------------------------*
/**
 * Function name	CInsertFunction::GenFunction
 * Description			
 * @param			CString & StrOut
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertFunction::GenFunction(CString & StrOut)
{
    CString StrHelp;
    CString Strlf = NEWLINE;
    StrOut = _T("");

    GetStdAttribString(StrOut);
    StrHelp.Format(IDS_KEY_FUNCTION, m_sName);
    StrOut += StrHelp;

    if(!m_sFunctionType.IsEmpty()) 
    {
        StrHelp.LoadString(IDS_KEY_COLON);
        StrOut += _T(" ");      // 25.08.06 SIS
        StrOut += StrHelp;
        StrOut += _T(" ");      // 25.08.06 SIS
        StrOut += m_sFunctionType;
    }
    
    StrOut += Strlf;

    // 23.08.06 SIS >>
    StrHelp.LoadString(IDS_KEY_VARIN);
    StrOut += StrHelp;
    StrOut += Strlf;
    StrHelp.LoadString(IDS_KEY_ENDVAR);
    StrOut += StrHelp;
    StrOut += Strlf;
    // 23.08.06 SIS <<

    switch (m_FileType)
	{
	case FILETYPE_ST:
        StrOut += Strlf;
        StrHelp.LoadString(IDS_KEY_SEMICOL);
        StrOut += StrHelp;
        StrOut += Strlf;
        StrHelp.LoadString(IDS_KEY_ENDFUNCTION);
        StrOut += StrHelp;
        StrOut += Strlf;
		break;
	case FILETYPE_IL:
        StrOut += Strlf;
        StrHelp.LoadString(IDS_KEY_ENDFUNCTION);
        StrOut += StrHelp;
        StrOut += Strlf;
		break;
    }
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertFunction::SetFileName
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertFunction::SetFileName()
{
    m_sFileName = m_sName;
    CString sWizardLanguage = GetDialog()->GetLanguageOfSelectedWizard();

    if(sWizardLanguage == _T("FBD")) 
    {
        m_sFileName += _T(".4gr"); //Function Block Diagram
        m_FileType = FILETYPE_FBD;
        return;
    }
    if(sWizardLanguage == _T("LD")) 
    {
        m_sFileName += _T(".4gr"); //Ladder Diagram
        m_FileType = FILETYPE_LD;
        return;
    }
    if(sWizardLanguage == _T("ST")) 
    {
        m_sFileName += _T(".st"); //Structured Text
        m_FileType = FILETYPE_ST;
        return;
    }
    if(sWizardLanguage == _T("IL")) 
    {
        m_sFileName += _T(".il"); //Instruction List
        m_FileType = FILETYPE_IL;
        return;
    }
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertFunction::GetDialog
 * Description			
 * @return			CDialogFunction* 
 * @exception			
 * @see			
*/
CDialogFunction* CInsertFunction::GetDialog()
{
    return (CDialogFunction*) m_pDialog;
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertFunction::Init
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertFunction::Init()
{
    m_sAlias          = GetDialog()->GetAlias();
    m_sName           = GetDialog()->GetName();
    m_sUsrText[0]     = GetDialog()->GetText1();
	m_sUsrText[1]     = GetDialog()->GetText2();
	m_sUsrText[2]     = GetDialog()->GetText3();
	m_sHelpUrl        = GetDialog()->GetHelpUrl();
	m_sUrl            = GetDialog()->GetUrl();
    m_sFunctionType   = GetDialog()->GetReturnType();
    m_pMembers        = GetDialog()->GetMembers();
    m_sHelpTemplate   = GetDialog()->GetPathOfSelectedTemplate();
    
    COperation::Init();
    SetPouDT(TRUE); //TRUE for Insert (FALSE for Prop)
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertFunction::FillDialog
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertFunction::FillDialog()
{
    //Standart name
    CString sName = GetNameProposal(m_sName);
    
    GetDialog()->SetCreateUser(m_sCreateUser);
    GetDialog()->SetCreateDate(m_sCreateDate);
    GetDialog()->SetLastModUser(m_sLastModUser);
    GetDialog()->SetLastModDate(m_sLastModDate);

    GetDialog()->SetName(sName);//Set the standart name
}

void CInsertFunction::Write4grFile()
{
    CString StrHelp;
    CString Strlf = _T("\n");
    CString Str4gr;

    if (m_FileType == FILETYPE_FBD)
    {
        Str4gr = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>") + Strlf;
        Str4gr += _T("<FCPOU>") + Strlf;
	    Str4gr += _T("<EDITINFO editname=\"\" editversion=\"\" editcatid=\"\"/>") + Strlf;
	    Str4gr += _T("<HEADER>") + Strlf;
        Str4gr += _T("<![CDATA[(*&#91;&#91;&#13;&#10;&#93;&#93;*)&#13;&#10;&#13;&#10;FUNCTION namefbd:BOOL") + Strlf;
        Str4gr += _T("]]>") + Strlf;
	    Str4gr += _T("</HEADER>") + Strlf;
	    Str4gr += _T("<BODY LANG=\"fbd\">") + Strlf;
	    Str4gr += _T("</BODY>") + Strlf;
        Str4gr += _T("</FCPOU>");
    }
    else if (m_FileType == FILETYPE_LD)
    {
        Str4gr += _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>") + Strlf;
        Str4gr += _T("<FCPOU>") + Strlf;
        Str4gr += _T("<EDITINFO EDITNAME=\"\" EDITVERSION=\"\" EDITCATID=\"\" EDITCLASSID=\"\" />") + Strlf;
        Str4gr += _T("<HEADER>") + Strlf;
        Str4gr += _T("<![CDATA[&#13;&#10;&#13;&#10;FUNCTION nameld") + Strlf;
        Str4gr += _T("]]>") + Strlf;
        Str4gr += _T("</HEADER>") + Strlf;
        Str4gr += _T("<BODY LANG=\"ld\" SFC_MENU=\"0\">") + Strlf;
        Str4gr += _T("<POWERRAIL ORIGIN=\"4;4\" SIZE=\"0;80\" />") + Strlf;
        Str4gr += _T("</BODY>") + Strlf;
        Str4gr += _T("</FCPOU>");
    }

    WriteFile(Str4gr);

    MergeHeaderIntoFile(m_FunctionText);
}


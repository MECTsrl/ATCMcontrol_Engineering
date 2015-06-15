


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "DllInterface.h"
#include "InsertProgram.h"
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
 * Function name			CInsertProgram::CInsertProgram
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
CInsertProgram::CInsertProgram(CPropertySheetBase *Dialog,
                               BSTR sSourceFile,
                               BSTR sId,
                               ICEProjInfo *pICEProjectInfo,
                               BSTR sAdditionalInfo) :
    COperation(Dialog, sSourceFile, sId, pICEProjectInfo, sAdditionalInfo)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    
    m_Program = NULL;
    m_sName = _T("Program");
//    CreateGUID();
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertProgram::~CInsertProgram
 * Description			
 * @return			
 * @exception			
 * @see			
*/
CInsertProgram::~CInsertProgram()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    if(m_Program)
    {
        delete m_Program;
        m_Program = NULL;
    }
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertProgram::Execute
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
BOOL CInsertProgram::Execute()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    Init();
    SetFileName();
    BOOL bRes = !DoesFileExist(m_sName, FALSE);
    if(bRes)
    {
        GenProgram(m_ProgramText);
        InsertVarList(m_ProgramText, m_Program);
        if (m_FileType == FILETYPE_ST || m_FileType == FILETYPE_IL)
        {
             WriteFile(m_ProgramText);
        }
        else
        {
             Write4grFile();
        }
        InformProjectManager();
    }
    m_ProgramText.Empty();
    m_aVarList.RemoveAll();
    return bRes;
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertProgram::GenProgram
 * Description			
 * @param			CString & StrOut
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertProgram::GenProgram(CString & StrOut)
{
    CString StrHelp;
    CString Strlf = NEWLINE;
    StrOut = _T("");
    
    GetStdAttribString(StrOut);
    StrHelp.Format(IDS_KEY_PRGINST, m_sName);
    StrOut += StrHelp;
    StrOut += Strlf;

    // 25.08.06 SIS >>
    StrHelp.LoadString(IDS_KEY_VAR);
    StrOut += StrHelp;
    StrOut += Strlf;
    StrHelp.LoadString(IDS_KEY_ENDVAR);
    StrOut += StrHelp;
    StrOut += Strlf;
    // 25.08.06 SIS <<

    switch (m_FileType)
	{
	case FILETYPE_ST:
        StrOut += Strlf;
        StrHelp.LoadString(IDS_KEY_SEMICOL);
        StrOut += StrHelp;
        StrOut += Strlf;
        StrHelp.LoadString(IDS_KEY_ENDPRG);
        StrOut += StrHelp;
        StrOut += Strlf;
		break;
	case FILETYPE_IL:
        StrOut += Strlf;
        StrHelp.LoadString(IDS_KEY_ENDPRG);
        StrOut += StrHelp;
        StrOut += Strlf;
		break;
    }
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertProgram::SetFileName
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertProgram::SetFileName()
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
    if(sWizardLanguage == _T("SFC")) 
    {
        m_sFileName += _T(".4gr"); //Sequential Function Chart
        m_FileType = FILETYPE_SFC;
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
 * Function name			CInsertProgram::GetDialog
 * Description			
 * @return			CDialogProgram* 
 * @exception			
 * @see			
*/
CDialogProgram* CInsertProgram::GetDialog()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return (CDialogProgram*) m_pDialog;
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertProgram::Init
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertProgram::Init()
{
    m_sAlias          = GetDialog()->GetAlias();
    m_sName           = GetDialog()->GetName();
    m_sUsrText[0]     = GetDialog()->GetText1();
	m_sUsrText[1]     = GetDialog()->GetText2();
	m_sUsrText[2]     = GetDialog()->GetText3();
	m_sHelpUrl        = GetDialog()->GetHelpUrl();
	m_sUrl            = GetDialog()->GetUrl();
    m_pMembers        = GetDialog()->GetMembers();
    m_sHelpTemplate   = GetDialog()->GetPathOfSelectedTemplate();
    
    COperation::Init();
    SetPouDT(TRUE); //TRUE for Insert (FALSE for Prop)
    

}

//------------------------------------------------------------------*
/**
 * Function name			CInsertProgram::FillDialog
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertProgram::FillDialog()
{
    // do only use standard name if dialog has not already set a user defined name
    // for example from the insert program instance dialog
    CString sName;
    if (!GetDialog()->GetName().IsEmpty())
    {
        sName = GetDialog()->GetName();
    }
    else
    {
        sName = GetNameProposal(m_sName);
    }

    GetDialog()->SetCreateUser(m_sCreateUser);
    GetDialog()->SetCreateDate(m_sCreateDate);
    GetDialog()->SetLastModUser(m_sLastModUser);
    GetDialog()->SetLastModDate(m_sLastModDate);

    GetDialog()->SetName(sName);
}

void CInsertProgram::Write4grFile()
{
    CString StrHelp;
    CString Strlf = NEWLINE;
    CString Str4gr;

    if (m_FileType == FILETYPE_FBD)
    {
        Str4gr = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>") + Strlf;
        Str4gr += _T("<FCPOU>") + Strlf;
	    Str4gr += _T("<EDITINFO editname=\"\" editversion=\"\" editcatid=\"\"/>") + Strlf;
	    Str4gr += _T("<HEADER>") + Strlf;
        Str4gr += _T("<![CDATA[&#13;&#10;PROGRAM namefbd") + Strlf;
        Str4gr += _T("]]>") + Strlf;
	    Str4gr += _T("</HEADER>") + Strlf;
	    Str4gr += _T("<BODY LANG=\"fbd\">") + Strlf;
	    Str4gr += _T("</BODY>") + Strlf;
        Str4gr += _T("</FCPOU>");
    }
    else if (m_FileType == FILETYPE_SFC)
    {
        Str4gr = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>") + Strlf;
        Str4gr += _T("<FCPOU>") + Strlf;
	    Str4gr += _T("<EDITINFO editname=\"\" editversion=\"\" editcatid=\"\"/>") + Strlf;
	    Str4gr += _T("<HEADER>") + Strlf;
        Str4gr += _T("<![CDATA[&#13;&#10;PROGRAM namesfc ") + Strlf;
        Str4gr += _T("]]>") + Strlf;
	    Str4gr += _T("</HEADER>") + Strlf;
	    Str4gr += _T("<BODY LANG=\"fbd_sfc\" SFC_MENU=\"1\">") + Strlf;
		Str4gr += _T("<LINE ORIGIN=\"6;4\" SIZE=\"0;48\" />") + Strlf;
		Str4gr += _T("<TRANS ORIGIN=\"18;10\" SIZE=\"14;4\" NAME=\"TRUE\">") + Strlf;
		Str4gr += _T("<PINLIST>") + Strlf;
		Str4gr += _T("<PIN CLASS=\"input\" ORIGIN=\"20;10\" />") + Strlf;
		Str4gr += _T("<PIN CLASS=\"input\" ORIGIN=\"18;12\" HIDDEN=\"1\" />") + Strlf;
		Str4gr += _T("<PIN CLASS=\"output\" ORIGIN=\"20;14\" />") + Strlf;
		Str4gr += _T("</PINLIST>") + Strlf;
		Str4gr += _T("</TRANS>") + Strlf;
		Str4gr += _T("<LINE ORIGIN=\"20;6\" SIZE=\"0;-2\" />") + Strlf;
		Str4gr += _T("<STEP ORIGIN=\"16;6\" SIZE=\"8;4\" NAME=\"Step0\" INITSTEP=\"1\">") + Strlf;
		Str4gr += _T("<PINLIST>") + Strlf;
		Str4gr += _T("<PIN CLASS=\"input\" ORIGIN=\"20;6\" />") + Strlf;
		Str4gr += _T("<PIN CLASS=\"output\" ORIGIN=\"24;8\" HIDDEN=\"1\" />") + Strlf;
		Str4gr += _T("<PIN CLASS=\"output\" ORIGIN=\"20;10\" />") + Strlf;
		Str4gr += _T("</PINLIST>") + Strlf;
		Str4gr += _T("</STEP>") + Strlf;
		Str4gr += _T("<LINE ORIGIN=\"6;4\" SIZE=\"14;0\" />") + Strlf;
		Str4gr += _T("<LINE ORIGIN=\"24;32\" SIZE=\"6;0\" />") + Strlf;
		Str4gr += _T("<STEP ORIGIN=\"16;30\" SIZE=\"8;4\" NAME=\"Step1\">") + Strlf;
		Str4gr += _T("<PINLIST>") + Strlf;
		Str4gr += _T("<PIN CLASS=\"input\" ORIGIN=\"20;30\" />") + Strlf;
		Str4gr += _T("<PIN CLASS=\"output\" ORIGIN=\"24;32\" HIDDEN=\"1\" />") + Strlf;
		Str4gr += _T("<PIN CLASS=\"output\" ORIGIN=\"20;34\" />") + Strlf;
		Str4gr += _T("</PINLIST>") + Strlf;
		Str4gr += _T("</STEP>") + Strlf;
		Str4gr += _T("<LINE ORIGIN=\"20;14\" SIZE=\"0;16\" />") + Strlf;
		Str4gr += _T("<TRANS ORIGIN=\"18;34\" SIZE=\"14;4\" NAME=\"TRUE\">") + Strlf;
		Str4gr += _T("<PINLIST>") + Strlf;
		Str4gr += _T("<PIN CLASS=\"input\" ORIGIN=\"20;34\" />") + Strlf;
		Str4gr += _T("<PIN CLASS=\"input\" ORIGIN=\"18;36\" HIDDEN=\"1\" />") + Strlf;
		Str4gr += _T("<PIN CLASS=\"output\" ORIGIN=\"20;38\" />") + Strlf;
		Str4gr += _T("</PINLIST>") + Strlf;
		Str4gr += _T("</TRANS>") + Strlf;
		Str4gr += _T("<LINE ORIGIN=\"20;38\" SIZE=\"0;14\" />") + Strlf;
		Str4gr += _T("<LINE ORIGIN=\"20;52\" SIZE=\"-14;0\" />") + Strlf;
		Str4gr += _T("<LINE ORIGIN=\"24;8\" SIZE=\"6;0\" />") + Strlf;
		Str4gr += _T("<ACTBLCK NAME=\"Action1\" ORIGIN=\"30;31\" SIZE=\"10;2\">") + Strlf;
		Str4gr += _T("<PINLIST>") + Strlf;
		Str4gr += _T("<PIN CLASS=\"input\" ORIGIN=\"30;32\" HIDDEN=\"1\" />") + Strlf;
    	Str4gr += _T("</PINLIST>") + Strlf;
		Str4gr += _T("<ACTQUAL QUALTYPE=\"N\" />") + Strlf;
		Str4gr += _T("</ACTBLCK>") + Strlf;
		Str4gr += _T("<ACTBLCK NAME=\"Action0\" ORIGIN=\"30;7\" SIZE=\"10;2\">") + Strlf;
		Str4gr += _T("<PINLIST>") + Strlf;
		Str4gr += _T("<PIN CLASS=\"input\" ORIGIN=\"30;8\" HIDDEN=\"1\" />") + Strlf;
	    Str4gr += _T("</PINLIST>") + Strlf;
		Str4gr += _T("<ACTQUAL QUALTYPE=\"N\" />") + Strlf;
		Str4gr += _T("</ACTBLCK>") + Strlf;
		Str4gr += _T("<ACTION NAME=\"Action1\" ORIGIN=\"48;31\" SIZE=\"28;16\" />") + Strlf;
		Str4gr += _T("<ACTION NAME=\"Action0\" ORIGIN=\"48;7\" SIZE=\"28;16\" />") + Strlf;
    	Str4gr += _T("</BODY>") + Strlf;
        Str4gr += _T("</FCPOU>") + Strlf;
    }
    else if (m_FileType == FILETYPE_LD)
    {
        Str4gr += _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>") + Strlf;
        Str4gr += _T("<FCPOU>") + Strlf;
        Str4gr += _T("<EDITINFO EDITNAME=\"\" EDITVERSION=\"\" EDITCATID=\"\" EDITCLASSID=\"\" />") + Strlf;
        Str4gr += _T("<HEADER>") + Strlf;
        Str4gr += _T("<![CDATA[&#13;&#10;&#13;&#10;PROGRAM nameld") + Strlf;
        Str4gr += _T("]]>") + Strlf;
        Str4gr += _T("</HEADER>") + Strlf;
        Str4gr += _T("<BODY LANG=\"ld\" SFC_MENU=\"0\">") + Strlf;
        Str4gr += _T("<POWERRAIL ORIGIN=\"4;4\" SIZE=\"0;80\" />") + Strlf;
        Str4gr += _T("</BODY>") + Strlf;
        Str4gr += _T("</FCPOU>");
    }
     
    WriteFile(Str4gr);

    MergeHeaderIntoFile(m_ProgramText);
}


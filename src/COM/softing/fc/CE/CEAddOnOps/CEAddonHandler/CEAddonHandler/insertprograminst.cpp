


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "DllInterface.h"
#include "InsertProgramInst.h"
#include "DialogProgInst.h"
#include "CEContainer.h"
#include "resource.h"
#include "Operation.h"
#include "SBLLexTree.h"

//----  Static Initializations:   ----------------------------------*


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




//------------------------------------------------------------------*
/**
 * Function name			CInsertProgramInst::CInsertProgramInst
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
CInsertProgramInst::CInsertProgramInst(CPropertySheetBase *Dialog,
                                       BSTR sSourceFile,
                                       BSTR sId,
                                       ICEProjInfo *pICEProjectInfo,
                                       BSTR sAdditionalInfo) :
    COperation(Dialog, sSourceFile, sId, pICEProjectInfo, sAdditionalInfo)
{
    m_sName = _T("ProgramInst");
    m_Resource  = NULL;
    m_bPosFirst = FALSE;
}


//------------------------------------------------------------------*
/**
 * Function name			CInsertProgramInst::~CInsertProgramInst
 * Description			
 * @return			
 * @exception			
 * @see			
*/
CInsertProgramInst::~CInsertProgramInst()
{
    if(m_Resource)
    {
        delete m_Resource;
        m_Resource = NULL;
    }
}


//------------------------------------------------------------------*
/**
 * Function name			CInsertProgramInst::Execute
 * Description			
 * @return			BOOL 
 * @exception			
 * @see			
*/
BOOL CInsertProgramInst::Execute()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    Init();
    SetFileName();
    if(!ParseResource())
    {
        return FALSE;
    }

    BOOL Res = DoesProgInstExist(m_sName);
    if(Res)
    {
        GenProgramInst(m_ProgramInstText);
        InsertProgInst();
        CString ResourceText = CString(m_ResourceText.c_str());
        WriteFile(ResourceText);
        InformProjectManager();
    }
    if (m_Resource)
    {
        delete m_Resource;
        m_Resource = NULL;
    }
    m_ProgramInstText.Empty();
    return Res;
}


//------------------------------------------------------------------*
/**
 * Function name			CInsertProgramInst::GenProgramInst
 * Description			
 * @param			CString & StrOut
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertProgramInst::GenProgramInst(CString & StrOut)
{
    CString StrHelp;
    CString Strlf(NEWLINE),StrIndent(SPACETAB), strQuote;

    StrOut = Strlf;
    StrOut += StrIndent;
    StrOut += StrIndent;
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
    //StrOut += Strlf;
}


//------------------------------------------------------------------*
/**
 * Function name			CInsertProgramInst::SetFileName
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertProgramInst::SetFileName()
{
    m_sFileName = m_sSourceFile;
}


//------------------------------------------------------------------*
/**
 * set program class.
 *
 * @param           [in] crstrProgramClass: program class
 * @return          -
*/
void CInsertProgramInst::SetProgramClass(const CString& crstrProgramClass)
{
    m_sClass = crstrProgramClass;
}

void CInsertProgramInst::SetProgramInstName(const CString& crstrProgramInstName)
{
    m_strInstName = crstrProgramInstName;
}


void CInsertProgramInst::SetProgramPos(BOOL bPos)
{
    m_bPosFirst = bPos;
}





//------------------------------------------------------------------*
/**
 * Function name			CInsertProgramInst::GetDialog
 * Description			
 * @return			CDialogProgInst* 
 * @exception			
 * @see			
*/
CDialogProgInst* CInsertProgramInst::GetDialog()
{
    return (CDialogProgInst*) m_pDialog;
}


//------------------------------------------------------------------*
/**
 * Function name			CInsertProgramInst::Init
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertProgramInst::Init()
{
    if(!m_bAutomationMode)
    {
        m_sAlias          = GetDialog()->GetAlias();
        m_sName           = GetDialog()->GetName();
        m_sUsrText[0]     = GetDialog()->GetText1();
	    m_sUsrText[1]     = GetDialog()->GetText2();
	    m_sUsrText[2]     = GetDialog()->GetText3();
	    m_sHelpUrl        = GetDialog()->GetHelpUrl();
	    m_sUrl            = GetDialog()->GetUrl();
        m_sClass          = GetDialog()->GetProgClass();
    }

    m_sTaskName       = GetNameFromId();
    
    COperation::Init();

    SetPouDT(TRUE); //TRUE for Insert (FALSE for Prop)

    if(m_bAutomationMode)
    {
        SetFileName();
        if(!ParseResource())
        {
            return;
        }
        if (m_strInstName.IsEmpty())
        {
            // 25.08.06 SIS >>
            CString strInst;
            strInst.Format(_T("%s_Inst"), m_sClass);
            CString strName = GetNameProposal(strInst);
            // 25.08.06 SIS <<
            m_sName = strName;
        }
        else
        {
            m_sName = m_strInstName;
        }
    }
}


//------------------------------------------------------------------*
/**
 * Function name			CInsertProgramInst::FillDialog
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertProgramInst::FillDialog()
{
    SetFileName();
    if(!ParseResource())
    {
        return;
    }

    //Standart name
    CString sName = GetNameProposal(m_sName);

    GetDialog()->SetCreateUser(m_sCreateUser);
    GetDialog()->SetCreateDate(m_sCreateDate);
    GetDialog()->SetLastModUser(m_sLastModUser);
    GetDialog()->SetLastModDate(m_sLastModDate);

    GetDialog()->SetName(sName);
    GetDialog()->SetProgClass(m_sClass);
}


// SIS: TODO put this and probably other methods
// in a base class for resource objects
//------------------------------------------------------------------*
/**
 * Function name			CInsertProgramInst::ParseResource
 * Description			
 * @return			successful?
 * @exception			
 * @see			
*/
BOOL CInsertProgramInst::ParseResource()
{
    ReadFileContent(CString(m_ProjectPath) + _T("\\") + CString(m_sFileName), 
                    m_ResourceText);
    if(m_Resource)
    {
        delete m_Resource;
        m_Resource = NULL;
    }
    ASSERT(m_ResourceText.length() != 0);
    parse_string(m_ResourceText, &m_Resource);
    if(!m_Resource)
    {
        return FALSE;
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * Function name			CInsertProgramInst::DoesProgInstExist
 * Description			
 * @param			CString strProgInstName
 * @return			BOOL 
 * @exception			
 * @see			
*/
BOOL CInsertProgramInst::DoesProgInstExist(CString strProgInstName)
{
    ASSERT(!strProgInstName.IsEmpty());
    program_configurations_citer iter;
    ASSERT(m_Resource);
    const program_configurations_type ProgInstConfigs = m_Resource->getProgramConfigurations();
    if(ProgInstConfigs.size() != 0)
    {
        for(iter = ProgInstConfigs.begin(); iter < ProgInstConfigs.end(); iter++)
        {
            ASSERT(iter);
            CString  strProgInstNameFromList = ((*iter)->getName()).c_str();
            if(strProgInstName.CompareNoCase((LPCTSTR)strProgInstNameFromList) == 0)
            {
                if(!m_bSilent)
                {
                    CString errorMsg;
                    errorMsg.Format(IDS_INSNAME_EXISTS, strProgInstName);
                    ::AfxMessageBox(errorMsg, MB_OK|MB_ICONWARNING);
                }
                return FALSE; //it does exists already
            }
        }
    }
    return TRUE;        
}


//------------------------------------------------------------------*
/**
 * Function name			CInsertProgramInst::InsertProgInst
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertProgramInst::InsertProgInst()
{
    StringPos insertpos;

    if (m_bPosFirst)
    {
        insertpos = GetNextInsertPosition(m_ResourceText, m_Resource->getNextTaskConfInsertPos());
    }
    else
    {
        insertpos = GetNextInsertPosition(m_ResourceText, m_Resource->getNextProgConfInsertPos());
    }

    m_ResourceText.insert(insertpos, m_ProgramInstText);
}


//------------------------------------------------------------------*
/**
 * Function name			CInsertProgramInst::InformProjectManager
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertProgramInst::InformProjectManager()
{
    HRESULT hRes = 0;
    BSTR strFileName = m_sFileName.AllocSysString();
    hRes = m_pICEProjectInfo->fileChangedHint(strFileName);
    SysFreeString(strFileName);
   	ASSERT(hRes==S_OK);
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertProgramInst::GetNameProposal
 * Description			
 * @param			CString strBasis
 * @return			CString 
 * @exception			
 * @see			
*/
CString CInsertProgramInst::GetNameProposal(CString strBasis)
{
    CSBLLexTree<bool>   tAssoc;             // association memory for names
    bool*               pbTmp = (bool*) 1;  // dummy element for insertion

    int iIndex = 1;
    CString strProgramInstName;

    strProgramInstName.Format("%s%d", strBasis, iIndex);
    
    program_configurations_citer iter;
    const program_configurations_type ProgInstConfigs = m_Resource->getProgramConfigurations();
    
    if(ProgInstConfigs.size() != 0)
    {
        for(iter = ProgInstConfigs.begin(); iter < ProgInstConfigs.end(); iter++)
        {
            strProgramInstName = ((*iter)->getName()).c_str();
            tAssoc.insertData(strProgramInstName, pbTmp); 
        }
    }

    for(iIndex = 1; iIndex < MAX_PROPOSAL_COUNT; ++iIndex)
    {    
        strProgramInstName.Format("%s%d", strBasis, iIndex);
        if(!tAssoc.getData(strProgramInstName))
        {
            break;
        }
    }

    return strProgramInstName;
}


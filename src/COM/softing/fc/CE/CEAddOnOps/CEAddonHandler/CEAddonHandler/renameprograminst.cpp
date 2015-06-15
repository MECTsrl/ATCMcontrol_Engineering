


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "RenameProgramInst.h"
#include "utilif.h"
#include "resource.h"
//----  Static Initializations:   ----------------------------------*


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



//------------------------------------------------------------------*
/**
 * Function name			CRenameProgramInst::CRenameProgramInst
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
CRenameProgramInst::CRenameProgramInst(CPropertySheetBase *Dialog,
                                       BSTR sSourceFile,
                                       BSTR sId,
                                       ICEProjInfo *pICEProjectInfo,
                                       BSTR sAdditionalInfo,
                                       CCEAddonActionComObj* pAddonHandler) : 
  CRenameObject(Dialog, sSourceFile, sId, pICEProjectInfo, sAdditionalInfo, pAddonHandler)
{
    m_sName = GetNameFromId();
    m_Resource = NULL;
}



//------------------------------------------------------------------*
/**
 * Function name			CRenameProgramInst::~CRenameProgramInst
 * Description			
 * @return			
 * @exception			
 * @see			
*/
CRenameProgramInst::~CRenameProgramInst()
{
    if(m_Resource)
    {
        delete m_Resource;
    }
}


//------------------------------------------------------------------*
/**
 * Function name			CRenameProgramInst::Execute
 * Description			
 * @return			BOOL 
 * @exception			
 * @see			
*/
BOOL CRenameProgramInst::Execute()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    CString strHelp = _T("");
    strHelp.LoadString(IDS_SOFTING_ADDON1);
    LPCTSTR g_MessageBoxCaption = (LPCTSTR)strHelp;

    // Check, if identifier is IEC compatible
    HRESULT hr = UTIL_CheckIdentifier(m_sNewName);
    if(hr != S_OK)
    {
        CString strMessage;
        switch(hr)
        {
        case E_UTIL_ID_SYNTAX:
            strMessage.LoadString(IDS_ERROR_IECSYNTAX);
            break;
        case E_UTIL_ID_IS_KEYWORD:
            strMessage.LoadString(IDS_ERROR_KEYWORD);
            break;
        case E_UTIL_ID_TOO_LONG:
            strMessage.LoadString(IDS_ERROR_TOOLONG);
            break;
        }
        if(!m_bSilent)
        {
            CString strHelp1 = _T("");
            strHelp1.Format(IDS_ERROR_INVALID, m_sNewName);
            ::AfxMessageBox(strHelp1 + strMessage, MB_OK|MB_ICONWARNING);
        }
    }
    else if(m_sName != m_sNewName)
    {
        BOOL bRes = FALSE;
        SetFileName();
        if(ParseResource())
        {
            bRes = !DoesProgramInstExist(m_sNewName);
            if(bRes)
            {
                RenameInText();
                WriteFile(m_ResourceText);
                InformProjectManager();//Delete the link
            }
        }
        return bRes;
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * Function name			CRenameProgramInst::InformProjectManager
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CRenameProgramInst::InformProjectManager()
{
    HRESULT hRes = 0;
    BSTR strFileName = m_sFileName.AllocSysString();
    hRes = m_pICEProjectInfo->fileChangedHint(strFileName);
    SysFreeString(strFileName);
   	ASSERT(hRes==S_OK);
}


//------------------------------------------------------------------*
/**
 * Function name			CRenameProgramInst::DoesProgramInstExist
 * Description			
 * @param			CString strProgramInstName
 * @return			does program instance exist? 
 * @exception			
 * @see			
*/
BOOL CRenameProgramInst::DoesProgramInstExist(CString strProgramInstName)
{
    program_configurations_citer iter;
    const program_configurations_type ProgInstConfigs = m_Resource->getProgramConfigurations();
    if(ProgInstConfigs.size() != 0)
    {
        for(iter = ProgInstConfigs.begin(); iter < ProgInstConfigs.end(); iter++)
        {
            CString  strProgramInstNameFromList = ((*iter)->getName()).c_str();
            if(strProgramInstName.CompareNoCase((LPCTSTR)strProgramInstNameFromList) == 0)
            {
                if(!m_bSilent)
                {
                    CString errorMsg;
                    errorMsg.Format(IDS_INSNAME_EXISTS, strProgramInstName);
                    CString errorTitle;
                    errorTitle.LoadString(IDS_ERROR_TITLE);
                    ::AfxMessageBox(errorMsg, MB_OK|MB_ICONWARNING);
                }
                return TRUE; //it exists already
            }
        }
    }
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * Function name			CRenameProgramInst::ParseResource
 * Description			
 * @return			successful? 
 * @exception			
 * @see			
*/
BOOL CRenameProgramInst::ParseResource()
{
    string buf;
    ReadFileContent(CString(m_ProjectPath) + _T("\\") + CString(m_sFileName), 
                    buf);
    ASSERT(m_Resource == NULL);
    parse_string(buf, &m_Resource);
    m_ResourceText = buf.c_str();
    if(m_Resource)
    {
        return TRUE;
    }
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * Function name			CRenameProgramInst::RenameInText
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CRenameProgramInst::RenameInText()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( )); 

    program_configurations_citer iter1;
    const program_configurations_type ProgInstConfigs = m_Resource->getProgramConfigurations();
    if(ProgInstConfigs.size() != 0)
    {
        for(iter1 = ProgInstConfigs.begin(); iter1 < ProgInstConfigs.end(); iter1++)
        {
            CString  strProgInstNameFromList = ((*iter1)->getName()).c_str();
            if(m_sName.CompareNoCase((LPCTSTR)strProgInstNameFromList) == 0)
            {
                StringPosRange Range = (*iter1)->getNameRange();
                m_ResourceText.Delete(Range.getFirst()-1, Range.getLength());
                m_ResourceText.Insert(Range.getFirst()-1, m_sNewName);
            }
        }
    }
}



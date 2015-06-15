


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "DeleteProgramInst.h"
#include "resource.h"
//----  Static Initializations:   ----------------------------------*



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



//------------------------------------------------------------------*
/**
 * Function name			CDeleteProgramInst::CDeleteProgramInst
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
CDeleteProgramInst::CDeleteProgramInst(CPropertySheetBase *Dialog,
                             BSTR sSourceFile,
                             BSTR sId,
                             ICEProjInfo *pICEProjectInfo,
                             BSTR sAdditionalInfo) : 
  COperation(Dialog, sSourceFile, sId, pICEProjectInfo, sAdditionalInfo),
  m_Resource(NULL)
{

}


//------------------------------------------------------------------*
/**
 * Function name			CDeleteProgramInst::~CDeleteProgramInst
 * Description			
 * @return			
 * @exception			
 * @see			
*/
CDeleteProgramInst::~CDeleteProgramInst()
{
    if(m_Resource)
    {
        delete m_Resource;
        m_Resource = NULL;
    }
}


//------------------------------------------------------------------*
/**
 * Function name			CDeleteProgramInst::Execute
 * Description		called when the user clicks on the OK button	
 * @return			BOOL: TRUE if succesful else FALSE
 * @exception			
 * @see			    SetFileName, ParseResource, DeleteProgramInstConfig
                    WriteFile, InformProjectManager 
*/
BOOL CDeleteProgramInst::Execute()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    SetFileName();
    if(!ParseResource())
    {
        return FALSE;
    }
    BOOL bRes = DeleteProgramInstConfig();
    if(bRes)
    {
        WriteFile(m_ResourceText);
        InformProjectManager();//Parse resource file
    }
    if(m_Resource)
    {
        delete m_Resource;
        m_Resource = NULL;
    }
    return bRes;
}



//------------------------------------------------------------------*
/**
 * Function name			CDeleteProgramInst::SetFileName
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CDeleteProgramInst::SetFileName()
{
    m_sFileName = m_sSourceFile;
}




//------------------------------------------------------------------*
/**
 * Function name			CDeleteProgramInst::InformProjectManager
 * Description		invoke the project manager to parse the resource file	
 * @return			void 
 * @exception			
 * @see			
*/
void CDeleteProgramInst::InformProjectManager()
{
    HRESULT hRes = 0;
    BSTR strFileName = m_sFileName.AllocSysString();
    hRes = m_pICEProjectInfo->fileChangedHint(strFileName);
    SysFreeString(strFileName);
   	ASSERT(hRes==S_OK);
}



//------------------------------------------------------------------*
/**
 * Function name			CDeleteProgramInst::ParseResource
 * Description		pareses the resource file	
 * @return			successful? 
 * @exception			
 * @see			    parse_string, ReadFileContent
*/
BOOL CDeleteProgramInst::ParseResource()
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
 * Function name			CDeleteProgramInst::GetProgramInstRange
 * Description		looks up the programm instance list in the resource to get
	                the insert position and the length of the given program instance
 * @param			CString strProgramInstName: instance to look for
 * @return			void 
 * @exception			
 * @see		        program_configurations_citer, program_configurations_type, StringPosRange	
*/
void CDeleteProgramInst::GetProgramInstRange(CString strProgramInstName)
{
    program_configurations_citer iter1;
    const program_configurations_type ProgInstConfigs = m_Resource->getProgramConfigurations();
    if(ProgInstConfigs.size() != 0)
    {
        for(iter1 = ProgInstConfigs.begin(); iter1 < ProgInstConfigs.end(); iter1++)
        {
            CString  strProgInstNameFromList = ((*iter1)->getName()).c_str();
            if(m_sName.CompareNoCase((LPCTSTR)strProgInstNameFromList) == 0)
            {
                StringPosRange Range = (*iter1)->getRange();
                m_iPos = Range.getFirst() - 1; 
                m_iLength = Range.getLength() + 1;
            }
        }
    }
}

//------------------------------------------------------------------*
/**
 * Function name			CDeleteProgramInst::DeleteProgramInstConfig
 * Description		Asks the user if he really wants to delete the program instance
	                if so it does.
 * @return			BOOL: TRUE if deleted else FALSE 
 * @exception			
 * @see			    GetProgramInstRange, GetNameFromId
*/
BOOL CDeleteProgramInst::DeleteProgramInstConfig()
{
    BOOL bDelete = TRUE;

    m_sName = GetNameFromId();

    if(!m_bSilent)
    {

        CString errorMsg;
        errorMsg.Format(IDS_REALLY_DELETE_FILE3, m_sName); 
        int iRes = ::AfxMessageBox(errorMsg, MB_YESNO|MB_ICONWARNING);
        if(iRes != IDYES)
        {
            bDelete = FALSE;
        }
    }
    if(bDelete)
    {
        GetProgramInstRange(m_sName);
        m_ResourceText.Delete(m_iPos, m_iLength);
        return TRUE;
    }
    return FALSE;
}



//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "DeleteGvlImport.h"
//----  Static Initializations:   ----------------------------------*


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



CDeleteGvlImport::CDeleteGvlImport(CPropertySheetBase *Dialog,
                             BSTR sSourceFile,
                             BSTR sId,
                             ICEProjInfo *pICEProjectInfo,
                             BSTR sAdditionalInfo) : 
  COperation(Dialog, sSourceFile, sId, pICEProjectInfo, sAdditionalInfo),
  m_Resource(NULL)
{

}



CDeleteGvlImport::~CDeleteGvlImport()
{
    if(m_Resource)
    {
        delete m_Resource;
        m_Resource = NULL;
    }
}



//------------------------------------------------------------------*
/**
 * Function name			CDeleteGvlImport::Execute
 * Description		Called when the user clicks on the OK button	
 * @return			BOOL: TRUE if succesful else FALSE 
 * @exception			
 * @see			Init, DeleteImport, WriteFile
*/
BOOL CDeleteGvlImport::Execute()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    SetFileName();
    Init();
    int iCount = m_aStrGvlFilesSel.GetSize();
    CString strFileName;
    BOOL bRes = FALSE;
    for(int i=0; i<iCount; i++)
    {
        strFileName = m_aStrGvlFilesSel.GetAt(i);
        bRes = DeleteImport(strFileName);
    }       
    if(bRes)
    {
        WriteFile(m_ResourceText);
        InformProjectManager();//Parse resource file
    }
    return bRes;
}


//------------------------------------------------------------------*
/**
 * Function name			CDeleteGvlImport::SetFileName
 * Description		Sets the file name to the actual file where the changes
	                has to occur
 * @return			void 
 * @exception			
 * @see			
*/
void CDeleteGvlImport::SetFileName()
{
    m_sFileName = m_sSourceFile;
}


//------------------------------------------------------------------*
/**
 * Function name			CDeleteGvlImport::ParseResource
 * Description		Parses the resource	
 * @return			successful?
 * @exception			
 * @see             parse_string, ReadFileContent			
*/
BOOL CDeleteGvlImport::ParseResource()
{
    string buf;
    ReadFileContent(CString(m_ProjectPath) + _T("\\") + CString(m_sFileName), 
                    buf);
    if(m_Resource)
    {
        delete m_Resource;
        m_Resource = NULL;
    }

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
 * Function name			CDeleteGvlImport::GetImportRange
 * Description		Gets insert position and length of the import range	
 * @param			CString strImportName: name of the import statement to look for
 * @return			void 
 * @exception			
 * @see		        StringPosRange, import_directives_type, import_directives_citer	
*/
void CDeleteGvlImport::GetImportRange(CString strImportName)
{
    import_directives_citer iter;
    const import_directives_type Imports = m_Resource->getImportDirectives();
    if(Imports.size() != 0)
    {
        for(iter = Imports.begin(); iter < Imports.end(); iter++)
        {
            CString  strImportNameFromList = ((*iter)->getFileName()).c_str();
            if(strImportName.CompareNoCase((LPCTSTR)strImportNameFromList) == 0)
            {
                StringPosRange Range = (*iter)->getImportDirectiveRange();
                m_iPos = Range.getFirst() - 1;
                m_iLength = Range.getLength();
            }
        }
    }
}


//------------------------------------------------------------------*
/**
 * Function name			CDeleteGvlImport::DeleteImport
 * Description		Asks the user if he really wants to delete the import directive
	                if yes, it does it.
 * @param			CString strFileName: file name where the changes has to occur
 * @return			BOOL: TRUE if deleted esle FALSE 
 * @exception			
 * @see			    parse_string, GetImportRange
*/
BOOL CDeleteGvlImport::DeleteImport(CString strFileName)
{
    BOOL bDelete = TRUE;
    if(!m_bSilent)
    {
        CString errorMsg;
        errorMsg.Format(IDS_REALLY_DELETE_FILE2, strFileName); 
        int iRes = ::AfxMessageBox(errorMsg, MB_YESNO|MB_ICONWARNING);
        if(iRes != IDYES)
        {
            bDelete = FALSE;
        }
    }
    if(bDelete)
    {
        GetImportRange(strFileName);
		// SIS d-113 >>
		// "#import" was not deleted
		// correct position and length
		int	iPos;
		int iLength;
		if(!FindImportStatement(m_ResourceText, iPos, iLength))
		{
			if(m_Resource)
			{
				delete m_Resource;
				m_Resource = NULL;
			}
			return FALSE;
		}
        m_ResourceText.Delete(iPos, iLength);
		// SIS d-113 <<
        //m_ResourceText was ganged !!!
        string buf = (LPCTSTR)m_ResourceText;
        if(m_Resource)
        {
            delete m_Resource;
            m_Resource = NULL;
        }
        parse_string(buf, &m_Resource);
        return TRUE;
    }
    return FALSE;
}


// SIS d-113 >>
//------------------------------------------------------------------*
/**
 * Function name    CDeleteGvlImport::FindImportStatement
 * Description      find complete import statement for deletion
 * @param           CString crstrResourceText: complete text of resource file
 * @return          BOOL: successful?
 * @exception
 * @see
*/
BOOL CDeleteGvlImport::FindImportStatement(const CString& crstrResourceText, int& riPos, int& riLength)
{
    BOOL bReturn = FALSE;
    int iCharCount = 0;
    int iChar;
    // find '#' before gvl text
    for(iChar = m_iPos; iChar >= 0; --iChar)
    {
        ++iCharCount;
        if(crstrResourceText[iChar] == _T('#'))
        {
            break;
        }
    }
    // not found -> return false
    if(iChar < 0)
    {
        return FALSE;
    }

    // verify that statement is correct
    CString strTmp = crstrResourceText.Mid(iChar+1);
    strTmp.TrimLeft();
    if(strTmp.Left(6).CompareNoCase(_T("import")) != 0)
    {
        return bReturn; // statement not correct
    }
    // find beginning of line
    BOOL bStop = FALSE;
    while(!bStop && iChar > 0)
    {
        --iChar;
        ++iCharCount;
        switch(crstrResourceText[iChar])
        {
        case _T('\n'):
            bStop = TRUE;
            break;
        case _T('\t'):
        case _T(' '):
            break;
        default:
            return bReturn;
        }
    }

    // all ok now
    bReturn = TRUE;

    // remove preceding '\r' if present
    if(iChar > 0 && crstrResourceText[iChar-1] == _T('\r'))
    {
        --iChar;
        ++iCharCount;
    }

    // now do correction
    riPos = m_iPos - iCharCount + 1;
    riLength = m_iLength + iCharCount - 1;

    return bReturn;
}
// SIS d-113 <<


//------------------------------------------------------------------*
/**
 * Function name			CDeleteGvlImport::Init
 * Description		Parses the resource for import directives and pops up a dialog
	                contaning a list of those import directives, so the user could
                    choose one to delete
 * @return			void 
 * @exception			
 * @see			    CFileSelDlg
*/
void CDeleteGvlImport::Init()
{
    if(m_bAutomationMode)
    {
        SetFileName();
    }

    if(!ParseResource())
    {
        return;
    }

    CStringArray aStrGvlFiles;
    CString strFileName;
    import_directives_citer iter;
    const import_directives_type Imports = m_Resource->getImportDirectives();
    if(Imports.size() != 0)
    {
        for(iter = Imports.begin(); iter < Imports.end(); iter++)
        {
            strFileName = ((*iter)->getFileName()).c_str();
            aStrGvlFiles.Add(strFileName);
        }
    }
    CFileSelDlg     tFileSelDlg;
    CString         strTitle;
    strTitle.LoadString(IDS_GLOB_VARSELEC);

    if(!m_bAutomationMode)
    {
        tFileSelDlg.Initialize(strTitle, aStrGvlFiles);
        if(tFileSelDlg.DoModal() == IDOK)
        {
            tFileSelDlg.GetSelection(m_aStrGvlFilesSel);
        }
    }
    
}

//------------------------------------------------------------------*
/**
 * Function name			CDeleteProgramInst::InformProjectManager
 * Description		invoke the project manager to parse the resource file	
 * @return			void 
 * @exception			
 * @see			
*/
void CDeleteGvlImport::InformProjectManager()
{
    HRESULT hRes = 0;
    BSTR strFileName = m_sFileName.AllocSysString();
    hRes = m_pICEProjectInfo->fileChangedHint(strFileName);
    SysFreeString(strFileName);
   	ASSERT(hRes==S_OK);
}


void CDeleteGvlImport::AddDeleteFile(const CString& crstrImportFile)
{
    m_aStrGvlFilesSel.Add(crstrImportFile);
}


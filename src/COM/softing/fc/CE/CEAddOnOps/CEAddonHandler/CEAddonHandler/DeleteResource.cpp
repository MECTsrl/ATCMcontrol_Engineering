


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "DeleteResource.h"
#include "resource.h"
#include "CEAddonActionComObj.h"

//----  Static Initializations:   ----------------------------------*


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDeleteResource::CDeleteResource(CPropertySheetBase *Dialog,
                             BSTR sSourceFile,
                             BSTR sId,
                             ICEProjInfo *pICEProjectInfo,
                             BSTR sAdditionalInfo,
                             CCEAddonActionComObj* pAddonHandler) : 
  COperation(Dialog, sSourceFile, sId, pICEProjectInfo, sAdditionalInfo),
  m_pAddonHandler(pAddonHandler)
{
    m_Config = NULL;
}

CDeleteResource::~CDeleteResource()
{
    if(m_Config)
    {
        delete m_Config;
        m_Config = NULL;
    }
}


//------------------------------------------------------------------*
/**
 * Function name			CDeleteResource::Execute
 * Description		called when the user clicks on the OK button	
 * @return			BOOL: TRUE if succesful else FALSE 
 * @exception			
 * @see			    SetFileName, DeleteFile, ParseConfig, DeleteResourceImport
                    WriteFile, InformProjectManager
*/
BOOL CDeleteResource::Execute()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    SetFileName();
    BOOL Res = DeleteFile(m_sFileName);
    if(Res)
    {
        ParseConfig();
        DeleteResourceImport();
        CString ConfigText = m_ConfigText.c_str();
        WriteFile(ConfigText);
        ASSERT(m_pAddonHandler);
        InformProjectManager();
    }
    return Res;
}



//------------------------------------------------------------------*
/**
 * Function name			CDeleteResource::SetFileName
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CDeleteResource::SetFileName()
{
    //File allready exists
    m_sFileName = m_sSourceFile;
}



//------------------------------------------------------------------*
/**
 * Function name			CDeleteResource::DeleteFile
 * Description		Asks the user if he wants to delete resource
	                if so it does
 * @param			CString strFileName: file name to be deleted
 * @return			BOOL: TRUE if succesful else FALSE 
 * @exception			
 * @see			    
*/
BOOL CDeleteResource::DeleteFile(CString strFileName)
{
    try
    {
        BOOL    bDelete = TRUE;
        m_sName = GetNameFromFileName(strFileName);
        if(!m_bSilent)
        {
            CString errorMsg;
            errorMsg.Format(IDS_REALLY_DELETE_FILE1, m_sName, (CString)m_ProjectPath);
            if(::AfxMessageBox(errorMsg, MB_YESNO|MB_ICONWARNING) != IDYES)
            {
                bDelete = FALSE;
            }
        }
        if(bDelete)
        {
            CString strProjectPath(m_ProjectPath);
            CString strFilePathName = strProjectPath + _T("\\") + strFileName;
            CFile::Remove(strFilePathName);
            // file has been removed, now trigger close editor, if open
            ASSERT(m_pAddonHandler);
            if(m_pAddonHandler)
            {
                CComBSTR    sFilePathName(strFilePathName);
                m_pAddonHandler->Fire_OnFileRemoved(sFilePathName);
            }
            return TRUE;
        }
    }
    catch(CFileException* pFileException)
    {
        pFileException->ReportError();
        pFileException->Delete();
    }   
    return FALSE;
}



//------------------------------------------------------------------*
/**
 * Function name			CDeleteResource::GetNameFromFileName
 * Description		Gets the object name from the file name 	
 * @param			CString strFileName
 * @return			CString : object name
 * @exception			
 * @see			
*/
CString CDeleteResource::GetNameFromFileName(CString strFileName)
{
    ASSERT(!strFileName.IsEmpty());
    CString strName;
    int iPos = strFileName.ReverseFind('.');
    strName = strFileName.Left(iPos);
    return strName;
}


//------------------------------------------------------------------*
/**
 * Function name			CDeleteResource::ParseConfig
 * Description		Builds the config name from the project name + the extention
	                ".con". Reads this file and parses it. the purpes is to get
                    the position of the resource import in the config file to 
                    delete it.
 * @return			void 
 * @exception			
 * @see			    ReadFileContent, parse_string
*/
void CDeleteResource::ParseConfig()
{
    //test if it's the config file. If not than the Handler was called from the main menu
    //and we have to construct the name of the config file
    CString SourceFile(m_sSourceFile); 
    if(GetExtentionFromFileName(SourceFile) != _T(".con"))
    {
        SAFEARRAY* parrFileNames;
	
        CComBSTR sClassType = _T("CONFIGURATION");
        HRESULT hRes = m_pICEProjectInfo->getClassesOfType(sClassType, &parrFileNames);
    
        BSTR HUGEP *pbstr;

        // Get a pointer to the elements of the array.
        hRes = ::SafeArrayAccessData(parrFileNames, (void HUGEP**)&pbstr);
        if (hRes==S_OK)
        {
            ASSERT((parrFileNames->rgsabound[0]).cElements>0);
            if ((parrFileNames->rgsabound[0]).cElements>0)
            {
                SourceFile = pbstr[0];
            }
        }

        ::SafeArrayUnaccessData(parrFileNames);
        ::SafeArrayDestroy(parrFileNames);
    }

    ReadFileContent(CString(m_ProjectPath) + _T("\\") + SourceFile, 
                    m_ConfigText);
    ASSERT(m_ConfigText.length() != 0);
    ASSERT(m_Config == NULL);
    parse_string(m_ConfigText, &m_Config);
}


//------------------------------------------------------------------*
/**
 * Function name			CDeleteResource::DeleteResourceImport
 * Description		deletes the resource import, if it doesn't find
                    it a warning pops up.
 * @return			void 
 * @exception			
 * @see			    GetRangeOfResourceImport
*/
void CDeleteResource::DeleteResourceImport()
{
    ASSERT(!m_sFileName.IsEmpty());
    StringPosRange PosRange = GetRangeOfResourceImport(m_sFileName);
    //ASSERT(PosRange != -1);
    if(PosRange.getFirst() != -1)
    {
        //HACK++ EF d-325 : also remove the '#import'
        const TCHAR* pTxtCfg = m_ConfigText.c_str();
        int   nImpChars = 0;
        //#import<space>"Resource1.gvl" 
        if(PosRange.getFirst()-1> 0)
        {
            const TCHAR* pTxt = pTxtCfg + PosRange.getFirst()-1-1;
            while(_istspace(*pTxt) && pTxt > pTxtCfg)
            {
                pTxt--;
                nImpChars++;
            }
            
            if(pTxt-7 >= pTxtCfg && !_tcsnicmp(pTxt-6, _T("#import"), 7))
                nImpChars += 7;
            else
                nImpChars = 0;
        }

        m_ConfigText.erase(PosRange.getFirst()-1 - nImpChars, PosRange.getLength() + nImpChars);
        //HACK--
    }
    else
    {
        if(!m_bSilent)
        {
            CString errorMsg;
            errorMsg.Format(IDS_RESOURCE_NOT_IN_CONF, m_sFileName);
            CString errorTitle;
            errorTitle.LoadString(IDS_ERROR_TITLE);
            int Res = ::AfxMessageBox(errorMsg, MB_OK|MB_ICONWARNING);  
        }
    }
}


//------------------------------------------------------------------*
/**
 * Function name			CDeleteResource::GetRangeOfResourceImport
 * Description		parses the config file and deliveres the position and length
	                of the resource import.
 * @param			CString strResourceFileName
 * @return			StringPosRange 
 * @exception			
 * @see			    import_directives_type, import_directives_citer, StringPosRange
*/
StringPosRange CDeleteResource::GetRangeOfResourceImport(CString strResourceFileName)
{
    ASSERT(!strResourceFileName.IsEmpty());
    ASSERT(m_Config);
    import_directives_type ImportDirectives = m_Config->getImportDirectives();
    import_directives_citer Iter;
    for(Iter = ImportDirectives.begin(); Iter < ImportDirectives.end(); Iter++)
    {
        ASSERT(Iter);
        string sFileName = (*Iter)->getFileName();
        CString strFileName = sFileName.c_str();
        if(!strFileName.CompareNoCase(strResourceFileName))
        {
            StringPosRange PosRange = (*Iter)->getImportDirectiveRange();
            return PosRange;
        }
    }
    return -1;
}


//------------------------------------------------------------------*
/**
 * Function name			CDeleteResource::WriteFile
 * Description		Builds the name of the config file from the project name + 
	                ".con". And writes the file with the new content
 * @param			CString sPouText: text to write in the file
 * @return			void 
 * @exception		internal file exeption 	
 * @see			    
*/
void CDeleteResource::WriteFile(CString sPouText)
{
    try
    {
        //test if it's the config file. If not than the Handler was called from the main menu
        //and we have to construct the name of the config file
        CString SourceFile(m_sSourceFile); 
        if(GetExtentionFromFileName(SourceFile) != _T(".con"))
        {
            SAFEARRAY* parrFileNames;
	
            CComBSTR sClassType = _T("CONFIGURATION");
            HRESULT hRes = m_pICEProjectInfo->getClassesOfType(sClassType, &parrFileNames);

            BSTR HUGEP *pbstr;

            // Get a pointer to the elements of the array.
            hRes = ::SafeArrayAccessData(parrFileNames, (void HUGEP**)&pbstr);
            if (hRes==S_OK)
            {
                ASSERT((parrFileNames->rgsabound[0]).cElements>0);
                if ((parrFileNames->rgsabound[0]).cElements>0)
                {
                    SourceFile = pbstr[0];
                }
            }

            ::SafeArrayUnaccessData(parrFileNames);
            ::SafeArrayDestroy(parrFileNames);
        }

        CStdioFile File((CString)m_ProjectPath + _T("\\") + SourceFile,
                   CFile::modeWrite | CFile::modeCreate);
        File.Write(sPouText, sPouText.GetLength());
        File.Close();
    }
    catch(CFileException* pFileException)
    {
        pFileException->ReportError();
        pFileException->Delete();
    }   
}


//------------------------------------------------------------------*
/**
 * Function name			CDeleteResource::InformProjectManager
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CDeleteResource::InformProjectManager()
{
    //Inform about the deleted resource
    //have to parse the config file
    CString ConFile = m_sSourceFile; 
    HRESULT hRes;
  
    if(GetExtentionFromFileName(ConFile) != _T(".con"))
    {
        SAFEARRAY* parrFileNames;
	
        CComBSTR sClassType = _T("CONFIGURATION");
        hRes = m_pICEProjectInfo->getClassesOfType(sClassType, &parrFileNames);
    
        BSTR HUGEP *pbstr;

        // Get a pointer to the elements of the array.
        hRes = ::SafeArrayAccessData(parrFileNames, (void HUGEP**)&pbstr);
        if (hRes==S_OK)
        {
            ASSERT((parrFileNames->rgsabound[0]).cElements>0);
            if ((parrFileNames->rgsabound[0]).cElements>0)
            {
                ConFile = pbstr[0];
            }
        }

        ::SafeArrayUnaccessData(parrFileNames);
        ::SafeArrayDestroy(parrFileNames);
    }

    if (!ConFile.IsEmpty())
    {
        BSTR strFileName = ConFile.AllocSysString();
        hRes = m_pICEProjectInfo->fileChangedHint(strFileName);
        ::SysFreeString(strFileName);
   	    ASSERT(hRes==S_OK);
    }
}





//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "RenameResource.h"
#include "utilif.h"
#include "resource.h"
#include "CEAddonActionComObj.h"

//----  Static Initializations:   ----------------------------------*

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------*
/**
 * Function name			CRenameResource::CRenameResource
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
CRenameResource::CRenameResource(CPropertySheetBase *Dialog,
                             BSTR sSourceFile,
                             BSTR sId,
                             ICEProjInfo *pICEProjectInfo,
                             BSTR sAdditionalInfo,
                             CCEAddonActionComObj* pAddonHandler) :
  CRenameObject(Dialog, sSourceFile, sId, pICEProjectInfo, sAdditionalInfo, pAddonHandler)
{
    m_Config = NULL;
    m_sName = GetNameFromId();
}


//------------------------------------------------------------------*
/**
 * Function name			CRenameResource::~CRenameResource
 * Description			
 * @return			
 * @exception			
 * @see			
*/
CRenameResource::~CRenameResource()
{
    if(m_Config)
    {
        delete m_Config;
        m_Config = NULL;
    }
}


//------------------------------------------------------------------*
/**
 * Function name			CRenameResource::Execute
 * Description			
 * @return			BOOL 
 * @exception			
 * @see			
*/
BOOL CRenameResource::Execute()
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
            strHelp.Format(IDS_ERROR_INVALID,m_sNewName);
            ::AfxMessageBox(strHelp1 + strMessage, MB_OK|MB_ICONWARNING);
        }
    }
    else if(m_sName != m_sNewName)
    {
        SetFileName();
        CString strNewFileName = m_sNewName + GetExtentionFromFileName(m_sFileName);
        BOOL bRes = !DoesFileExist(strNewFileName, TRUE);
        if(bRes)
        {
            RenameInText();
            WriteFile(m_sObjectText);
            RenameFile(m_sFileName, strNewFileName);
            ParseConfig();
            RenameResourceImport();

            CString ConfigText = m_ConfigText.c_str();
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
            m_sFileName = SourceFile;
            
            WriteFile(ConfigText);
            InformProjectManager();
        }
        return bRes;
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * Function name			CRenameResource::RenameResourceImport
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CRenameResource::RenameResourceImport()
{
    CString sImportText;

    sImportText += '"';  //d-2156
    sImportText += m_sNewName;
    sImportText += GetExtentionFromFileName(m_sFileName);
    sImportText += '"';

    StringPosRange Range = GetImportRange(m_sFileName);
    m_ConfigText.erase(Range.getFirst()-1, Range.getLength());
    m_ConfigText.insert(Range.getFirst()-1, sImportText);
}


//------------------------------------------------------------------*
/**
 * Function name			CRenameResource::GetImportRange
 * Description			
 * @param			CString strFileName
 * @return			StringPosRange 
 * @exception			
 * @see			
*/
StringPosRange CRenameResource::GetImportRange(CString strFileName)
{
    import_directives_type ImportDirectives = m_Config->getImportDirectives();
    if(ImportDirectives.size() != 0)
    {
        import_directives_citer iter;
        for(iter = ImportDirectives.begin(); iter < ImportDirectives.end(); iter ++)
        {
            CString strFileName = ((*iter)->getFileName()).c_str();
            if(!m_sFileName.CompareNoCase(strFileName))
            {
                StringPosRange Range = (*iter)->getImportDirectiveRange();
                return Range;
            }
        }
    }
    StringPosRange NoRange(0,0);
    return NoRange;
}


//------------------------------------------------------------------*
/**
 * Function name			CRenameResource::ParseConfig
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CRenameResource::ParseConfig()
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
 * Function name			CRenameResource::InformProjectManager
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CRenameResource::InformProjectManager()
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

    CString strNewFileName = m_sNewName + GetExtentionFromFileName(m_sFileName);

    // trigger file close and reopen
    ASSERT(m_pAddonHandler);
    if(m_pAddonHandler)
    {
        CComBSTR    sOldFilePath(m_sFileName);
        CComBSTR    sNewFilePath(strNewFileName);
        m_pAddonHandler->Fire_OnFileRenamed(sOldFilePath, sNewFilePath);
    }

    if (!ConFile.IsEmpty())
    {
        BSTR strFileName = ConFile.AllocSysString();
        hRes = m_pICEProjectInfo->fileChangedHint(strFileName);
        ::SysFreeString(strFileName);
   	    ASSERT(hRes==S_OK);
    }
}



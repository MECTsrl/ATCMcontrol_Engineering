


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

#include "stdafx.h"
#include "DllInterface.h"
#include "InsertResource.h"
#include "DialogResource.h"
#include "CEContainer.h"
#include "resource.h"

//----  Static Initializations:   ----------------------------------*


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//------------------------------------------------------------------*
/**
 * Function name			CInsertResource::CInsertResource
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
CInsertResource::CInsertResource(CPropertySheetBase *Dialog,
                                           BSTR sSourceFile,
                                           BSTR sId,
                                           ICEProjInfo *pICEProjectInfo,
                                           BSTR sAdditionalInfo) :
    COperation(Dialog, sSourceFile, sId, pICEProjectInfo, sAdditionalInfo)
{
    m_Resource = NULL;
    m_Config = NULL;
    m_sName = _T("Resource");
//    CreateGUID();
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertResource::~CInsertResource
 * Description			
 * @return			
 * @exception			
 * @see			
*/
CInsertResource::~CInsertResource()
{
    if(m_Config)
    {
        delete m_Config;
        m_Config = NULL;
    }
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertResource::Execute
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
BOOL CInsertResource::Execute()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    Init();
    SetFileName();
    BOOL bRes = !DoesFileExist(m_sFileName, TRUE);
    if(bRes)
    {
        GenResource(m_ResourceText);
        COperation::WriteFile(m_ResourceText);
        ParseConfig();
        GenImportText(m_ImportText);
        InsertResourceImport();
        CString ConfigText = m_ConfigText.c_str();
        WriteFile(ConfigText);
        InformProjectManager();
    }
    m_ConfigText.empty();
    m_ResourceText.Empty();
    m_ImportText.Empty();
    m_aVarList.RemoveAll();
    return bRes;   
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertResource::GenResource
 * Description			
 * @param			CString & StrOut
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertResource::GenResource(CString & StrOut)
{
    CString StrHelp,Strlf(NEWLINE),Strco,StrIndent(SPACETAB), strQuote;
    CString strComment(_T(""));
    CString strImportFile(_T(""));

    GetStdAttribString(StrOut);

    StrHelp.Format(IDS_KEY_RESOURCE, m_sName);
    StrOut += StrHelp;
//    if(!m_sResType.IsEmpty())
//    {
//        StrOut += StrIndent;
//        StrOut += StrIndent;
//        StrHelp.Format(IDS_KEY_RES_ON, m_sResType);
//        StrOut += StrHelp;
//        StrOut += Strlf;
//    }
    StrOut += Strlf;
    StrOut += Strlf;
    StrOut += StrIndent;
    strImportFile = m_sName + _T(".gvl");
    StrHelp.Format(IDS_KEY_IMPORTFILE, strImportFile);
    StrOut += StrHelp;
    StrOut += Strlf;

    StrHelp.LoadString(IDS_KEY_ENDRESOURCE);
    StrOut += StrHelp;
    StrOut += Strlf;
    StrOut += Strlf;
    StrOut += Strlf;
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertResource::SetFileName
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertResource::SetFileName()
{
    m_sFileName = m_sName + _T(".res");
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertResource::GetDialog
 * Description			
 * @return			CDialogResource* 
 * @exception			
 * @see			
*/
CDialogResource* CInsertResource::GetDialog()
{
    return (CDialogResource*) m_pDialog;
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertResource::Init
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertResource::Init()
{
    m_sAlias          = GetDialog()->GetAlias();
    m_sName           = GetDialog()->GetName();
    m_sUsrText[0]     = GetDialog()->GetText1();
	m_sUsrText[1]     = GetDialog()->GetText2();
	m_sUsrText[2]     = GetDialog()->GetText3();
	m_sHelpUrl        = GetDialog()->GetHelpUrl();
	m_sUrl            = GetDialog()->GetUrl();
//    m_sResType        = GetDialog()->GetType();
    
    COperation::Init();
    SetPouDT(TRUE); //TRUE for Insert (FALSE for Prop)
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertResource::FillDialog
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertResource::FillDialog()
{
    //Standart name
    CString sName = GetNameProposal(m_sName);
    
    GetDialog()->SetCreateUser(m_sCreateUser);
    GetDialog()->SetCreateDate(m_sCreateDate);
    GetDialog()->SetLastModUser(m_sLastModUser);
    GetDialog()->SetLastModDate(m_sLastModDate);
    
    GetDialog()->SetName(sName);
}


//------------------------------------------------------------------*
/**
 * Function name			CInsertResource::InformProjectManager
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertResource::InformProjectManager()
{
    //Inform about the new resource
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

    //Inform about the new gvl file
    ICEContainer *pICEContainer = NULL;
    BSTR sSF;
    hRes = m_pICEProjectInfo->getProjectName(&sSF);
    ASSERT(hRes==S_OK);
    CString SourceFile(sSF); 
    SysFreeString(sSF);

    CString StrId = SourceFile;
    SourceFile += _T(".4cp");
    sSF = SourceFile.AllocSysString();
    hRes = m_pICEProjectInfo->getContainerIFforFile(sSF, &pICEContainer);
   	ASSERT(hRes==S_OK);
    CString sImportFile = m_sName + _T(".gvl");
    BSTR strGVLFileName = sImportFile.AllocSysString();
    
    //Contructing the Id needed by the Project Manager
    StrId = _T("/") + StrId + _T(".") + _T("CLASSVIEW") + _T("/") 
         + _T("Classes.GVLs");
    BSTR sId = StrId.AllocSysString();
    //Creating the link
    hRes = pICEContainer->createLink(sSF, sId, strGVLFileName);
    SysFreeString(strGVLFileName);
    SysFreeString(sId);
    SysFreeString(sSF);

    //ASSERT(hRes==S_OK);

    pICEContainer->Release();
}


//------------------------------------------------------------------*
/**
 * Function name			CInsertResource::ParseConfig
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertResource::ParseConfig()
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
 * Function name			CInsertResource::InsertResourceImport
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertResource::InsertResourceImport()
{
    ASSERT(m_Config);
    if(m_Config)
    {
        //Test if the Import is already in the config file
        import_directives_type ImportDirectives = m_Config->getImportDirectives();
        if(ImportDirectives.size() != 0)
        {
            import_directives_citer iter;
            for(iter = ImportDirectives.begin(); iter < ImportDirectives.end(); iter ++)
            {
                CString strFileName = ((*iter)->getFileName()).c_str();
                if(!m_sFileName.CompareNoCase(strFileName))
                {//Import is already in the config, don't do any thing
                    return;
                }
            }
        }
        //If not insert it
        StringPos insertpos = GetNextInsertPosition(m_ConfigText, m_Config->getNextImpDirInsertPos());
        m_ConfigText.insert(insertpos, m_ImportText);
    }
}


//------------------------------------------------------------------*
/**
 * Function name			CInsertResource::GenImportText
 * Description			
 * @param			CString & sImportText
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertResource::GenImportText(CString & sImportText)
{
    CString StrHelp,Strlf(NEWLINE),StrIndent(SPACETAB);

    sImportText += Strlf;
    sImportText += StrIndent;
    StrHelp.Format(IDS_KEY_IMPORTFILE, m_sFileName);
    sImportText += StrHelp;
//    sImportText += Strlf;
}


//------------------------------------------------------------------*
/**
 * Function name			COperation::WriteFile
 * Description			
 * @param			CString sPouText
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertResource::WriteFile(CString sPouText)
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
        //Create the res file
        CStdioFile File((CString)m_ProjectPath + _T("\\") + SourceFile,
                   CFile::modeWrite | CFile::modeCreate);
        File.Write(sPouText, sPouText.GetLength());
        File.Close();
        //Create the gvl file
        CString sImportFile = m_sName + _T(".gvl");
        CStdioFile GVLFile((CString)m_ProjectPath + _T("\\") + sImportFile,
                   CFile::modeWrite | CFile::modeCreate);
        
        CString StrHelp, Strlf = NEWLINE, StrOut = _T("");
        StrHelp.LoadString(IDS_KEY_COMMENT);
        StrOut += StrHelp;
        StrHelp.LoadString(IDS_GLOBVAR_FILE);
        StrOut += StrHelp;
        StrHelp.LoadString(IDS_KEY_ENDCOMMENT);
        StrOut += StrHelp;
        StrOut += Strlf;

        GVLFile.Write(StrOut, StrOut.GetLength());
        GVLFile.Close();
    }
    catch(CFileException* pFileException)
    {
        pFileException->ReportError();
        pFileException->Delete();
    }   
}


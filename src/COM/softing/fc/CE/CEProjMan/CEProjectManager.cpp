

//------------------------------------------------------------------*
// Includes
//
#include "stdafx.h"
#include "CEProjMan.h"
#include "CEProjectManager.h"

#include "CEKadMan.h"
#include "CEKadMan_i.c"
#include "KadManager.h"
#include "KadManager_i.c"
#include "CEProjInfo.h"
#include "CEProjInfo_i.c"

#include "CEMsg.h"
#include "CEMsg_i.c"

#include "resource.h"

#include "utilif.h"
#include "comhelper.h"

#include "cecompmanev_i.c"

#include "FileHelper.h"


#define CEPROJMAN_SECONDS_TO_REPARSE 30




#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//------------------------------------------------------------------*
// Constructor
//

//------------------------------------------------------------------*
/**
 * default constructor
 *
*/
CCEProjectManager::CCEProjectManager()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    m_strProjectName.Empty();
    m_strProjectPath.Empty();
    m_strProjectFileName.Empty();
    m_strProjectFileNameWithPath.Empty();
    m_bProjectLoaded = FALSE;
    m_bProjectReady = FALSE;

    m_pProjectFileHandler = NULL;
    m_pKadManager   = NULL;
    m_pTargetTree   = NULL; 
    m_pClassTree    = NULL; 
    m_pInstanceTree = NULL;

    m_sTargetId = CEPROJMAN_TARGETID;

    m_hMutex = ::CreateMutex(NULL, FALSE, NULL);

    m_hInstance = AfxGetApp()->m_hInstance;

    CFileHelper::pProjMan = this;
}


//------------------------------------------------------------------*
/**
 * default destructor
 *
*/
CCEProjectManager::~CCEProjectManager()
{
    ::CloseHandle(m_hMutex);
    CleanUp();
}


//------------------------------------------------------------------*
/**
 * CleanUp deletes all things and sets pointer to NULL and cleans strings
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CCEProjectManager::CleanUp()
{
    m_strProjectName.Empty();
    m_strProjectPath.Empty();
    m_strProjectFileName.Empty();
    m_strProjectFileNameWithPath.Empty();
    m_bProjectLoaded = FALSE;
    m_bProjectReady = FALSE;

    ClearBackparser();
    ClearContainer();

    if (m_pKadManager!=NULL)
    {
        m_pKadManager->Release();
        m_pKadManager = NULL;
    }

    if (m_pProjectFileHandler)
    {
        delete m_pProjectFileHandler;
        m_pProjectFileHandler = NULL;
    }

    if (m_pClassTree)
    {
        delete m_pClassTree;
        m_pClassTree  = NULL;
    }

    if (m_pTargetTree)
    {
        delete m_pTargetTree;
        m_pTargetTree = NULL;
    }

    if (m_pInstanceTree)
    {
        delete m_pInstanceTree;
        m_pInstanceTree = NULL;
    }
}

void CCEProjectManager::ProjectManagerLock()
{
    ::WaitForSingleObject(m_hMutex, INFINITE);
    
}

void CCEProjectManager::ProjectManagerUnlock()
{
    ::ReleaseMutex(m_hMutex);
}


/////////////////////////////////////////////////////////////////////
// Implementation of ICEProjectManager
//


//------------------------------------------------------------------*
// Open Project
//
STDMETHODIMP CCEProjectManager::OpenProject(BSTR sProject)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    HRESULT hr;
    
    ProjectManagerLock();

    try
    {
        if (m_bProjectLoaded)
        {
            CloseProject();
        }

        m_strProjectFileNameWithPath = sProject;

        // extract project name, with/without path/file extention
        // the parameter should be the project file name with absolute path
        // and file extention
        int lastSlash = m_strProjectFileNameWithPath.ReverseFind(_T('\\'));

        if (lastSlash<0)
        {
            Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                              E_CEPROJMAN_NO_VALID_PROJ_FILE_NAME, 
                              (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_VALID_PROJ_FILE_NAME, 
                                                      m_strProjectFileNameWithPath));
            throw E_CEPROJMAN_NO_VALID_PROJ_FILE_NAME;
        }

        m_strProjectPath = m_strProjectFileNameWithPath.Left(lastSlash);
        m_strProjectFileName = 
            m_strProjectFileNameWithPath.Right(m_strProjectFileNameWithPath.GetLength()-lastSlash-1);

        int lastDot   = m_strProjectFileName.ReverseFind(_T('.'));
        if (lastDot<0)
        {
            Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                      E_CEPROJMAN_NO_VALID_PROJ_FILE_NAME, 
                      (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_VALID_PROJ_FILE_NAME, 
                                              m_strProjectFileNameWithPath));
            throw E_CEPROJMAN_NO_VALID_PROJ_FILE_NAME;
        }
        m_strProjectName = m_strProjectFileName.Left(lastDot);

        // check if project file exists
        CFileStatus fstatus;
        if (!CFile::GetStatus(m_strProjectFileNameWithPath, fstatus))
        {
            ATLTRACE(_T("error project file not found\n"));
            Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                      E_CEPROJMAN_NO_PROJ_FILE, 
                      (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJ_FILE, 
                                              m_strProjectFileNameWithPath));
            throw E_CEPROJMAN_NO_PROJ_FILE;
        }

        // init project file handler
        m_pProjectFileHandler = new CProjectFile(this);
        hr = m_pProjectFileHandler->SetProjectFile(m_strProjectFileNameWithPath);
        if (hr!=S_OK)
	    {
		    throw hr;
	    }

        // check 4cp file version
        CSection* pFormatSection;
        CKeyAndValue* pKV;
        if ((!m_pProjectFileHandler->FindSection(CE_CP_FORMAT, &pFormatSection))
            ||(!pFormatSection->FindKeyAndValue(CE_CP_VERSION, CE_CP_VER_20, &pKV)))
        {
            Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                      E_CEPROJMAN_WRONG_PROJFILE_VERSION, 
                      (CComBSTR)BuildErrorMsg(E_CEPROJMAN_WRONG_PROJFILE_VERSION));
            throw E_CEPROJMAN_WRONG_PROJFILE_VERSION;
        }

        // get/init helper classes
        if (!InitKadManager())
        {
            Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                      E_CEPROJMAN_KADMANAG_CREATE, 
                      (CComBSTR)BuildErrorMsg(E_CEPROJMAN_KADMANAG_CREATE));
            throw E_CEPROJMAN_KADMANAG_CREATE;
        }

        InitBackparser();
        InitContainer();

        // project successfully opened
	    m_bProjectLoaded = TRUE;

        // class tree
        m_pClassTree = new CClassTree(this);
        PROJ_MAN_ASSERT(m_pClassTree);
        if (!m_pClassTree)
        {
            throw E_OUTOFMEMORY;
        }
        m_pClassTree->InitTree();

        // target tree 
        m_pTargetTree = new CTargetTree(this);
        PROJ_MAN_ASSERT(m_pTargetTree);
        if (!m_pTargetTree)
        {
            throw E_OUTOFMEMORY;
        }
        m_pTargetTree->InitTree();

        // target tree 
        m_pInstanceTree = new CInstanceTree(this);
        PROJ_MAN_ASSERT(m_pInstanceTree);
        if (!m_pInstanceTree)
        {
            throw E_OUTOFMEMORY;
        }
        m_pInstanceTree->InitTree();

        // reset reparsing information
        m_bReparseClassesReady = TRUE;
        m_bReparseTargetsReady = TRUE;
        //m_bReparseInstancesReady = TRUE;
        m_timeReparse = CTime::GetCurrentTime();

        m_bProjectReady = TRUE;
        
        ProjectManagerUnlock();
        return S_OK;
    }
    catch(HRESULT hrRes)
    {
        CleanUp();
        ProjectManagerUnlock();
        return hrRes;
    }
    catch (CMemoryException* e)
    {

        e->Delete();
        CleanUp();
        ProjectManagerUnlock();
        return E_OUTOFMEMORY;
    }
}



//------------------------------------------------------------------*
// Close Project
//
STDMETHODIMP CCEProjectManager::CloseProject()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    ProjectManagerLock();

    HRESULT hr = SaveViewData();

    CleanUp();

    ProjectManagerUnlock();
    return hr;
}



//------------------------------------------------------------------*
// Reparse Check
//
STDMETHODIMP CCEProjectManager::ReparseCheck()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
   

    DWORD   dwResult;
    dwResult = ::WaitForSingleObject(m_hMutex, 0);

    if(dwResult == WAIT_TIMEOUT)
    {
        return S_FALSE;
    }

    if (m_bProjectReady)
    {
        try
        {
            PROJ_MAN_ASSERT(m_pClassTree);
            PROJ_MAN_ASSERT(m_pTargetTree);
            PROJ_MAN_ASSERT(m_pInstanceTree);
            PROJ_MAN_ASSERT(m_pProjectFileHandler);

            // time criteria
            CTime currTime = CTime::GetCurrentTime();
            CTimeSpan timeDiff = currTime - m_timeReparse;
            if (timeDiff.GetTotalSeconds()>CEPROJMAN_SECONDS_TO_REPARSE)
            {
                m_bReparseClassesReady = FALSE;
                m_bReparseTargetsReady = FALSE;
                //m_bReparseInstancesReady = FALSE;
                m_timeReparse = CTime::GetCurrentTime();
            }

            //if (timeDiff.GetTotalSeconds()>5)
            //{
            //    m_bReparseInstancesReady = FALSE;
            //}

            // look for new source files;
            if (!m_bReparseClassesReady)
            { 
                m_bReparseClassesReady = m_pClassTree->CheckSourceFilesToReparseIncrementally();
            }
            if (!m_bReparseTargetsReady)
            {
                m_bReparseTargetsReady = m_pTargetTree->CheckSourceFilesToReparseIncrementally();
            }

            m_pClassTree->Reparse();
            m_pTargetTree->Reparse();

            m_pProjectFileHandler->UpdateFileViewInfo();

            //if (!m_bReparseInstancesReady)
            //{
            //    m_bReparseInstancesReady = m_pInstanceTree->CheckSourceFilesToReparseIncrementally();   
            //}

            //m_pInstanceTree->Reparse();
            m_pInstanceTree->UpdateTargets();

            ::ReleaseMutex(m_hMutex);

            if (!m_bReparseTargetsReady || !m_bReparseClassesReady)
            {
                return S_FALSE;
            }

            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            ::ReleaseMutex(m_hMutex);
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                      E_CEPROJMAN_NO_PROJECT_LOADED, 
                      (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        ::ReleaseMutex(m_hMutex);
        return E_CEPROJMAN_NO_PROJECT_LOADED;   
    }
}



//------------------------------------------------------------------*
// Get ClassViewXML
//
STDMETHODIMP CCEProjectManager::GetClassViewXML(BSTR *psClassViewXML)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (psClassViewXML== NULL)
        return E_POINTER;

    if (m_bProjectLoaded)
    {
        try 
        {
            PROJ_MAN_ASSERT(m_pClassTree);
            *psClassViewXML = m_pClassTree->GetTreeXML().AllocSysString();
            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                      E_CEPROJMAN_NO_PROJECT_LOADED, 
                      (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED; 
    }
}



//------------------------------------------------------------------*
// GetClassViewDOM
//
STDMETHODIMP CCEProjectManager::GetClassViewDOM(IUnknown **ppClassViewDOM)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (ppClassViewDOM == NULL)
        return E_POINTER;

    if (m_bProjectLoaded)
    {
        try
        {
            PROJ_MAN_ASSERT(m_pClassTree);
            BOOL res = m_pClassTree->GetIXMLDoc(ppClassViewDOM);
            PROJ_MAN_ASSERT(res);
            if (!res)
            {
                *ppClassViewDOM = NULL;
                return E_FAIL;
            }
            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        *ppClassViewDOM = NULL;
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                      E_CEPROJMAN_NO_PROJECT_LOADED, 
                      (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED; 
    }
}



//------------------------------------------------------------------*
// GetTargetViewXML
//
STDMETHODIMP CCEProjectManager::GetTargetViewXML(BSTR *psTargetViewXML)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (psTargetViewXML== NULL)
        return E_POINTER;

    if (m_bProjectLoaded)
    {
        try
        {
            PROJ_MAN_ASSERT(m_pTargetTree);
            *psTargetViewXML = m_pTargetTree->GetTreeXML().AllocSysString();
            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                      E_CEPROJMAN_NO_PROJECT_LOADED, 
                      (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;     }

    return S_OK;
}



//------------------------------------------------------------------*
// GetTargetViewDOM
//
STDMETHODIMP CCEProjectManager::GetTargetViewDOM(IUnknown **ppTargetViewDOM)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (ppTargetViewDOM == NULL)
        return E_POINTER;

    if (m_bProjectLoaded)
    {
        try
        {
            PROJ_MAN_ASSERT(m_pTargetTree);
            BOOL res = m_pTargetTree->GetIXMLDoc(ppTargetViewDOM);
            PROJ_MAN_ASSERT(res);
            if (!res)
            {
                *ppTargetViewDOM = NULL;
                return E_FAIL;
            }
            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }

    }
    else
    {
        *ppTargetViewDOM = NULL;
        return E_FAIL;
    }
}



//------------------------------------------------------------------*
// SaveViewData
//
STDMETHODIMP CCEProjectManager::SaveViewData()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_bProjectLoaded)
    {
        try
        {
            PROJ_MAN_ASSERT(m_pClassTree);
            PROJ_MAN_ASSERT(m_pTargetTree);
            PROJ_MAN_ASSERT(m_pInstanceTree);
            m_pClassTree->SaveViewData();
            m_pTargetTree->SaveViewData();
            m_pInstanceTree->SaveViewData();
            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                      E_CEPROJMAN_NO_PROJECT_LOADED, 
                      (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;
    }	

}


//------------------------------------------------------------------*
// GetAllFilesOfCategory
//
STDMETHODIMP CCEProjectManager::GetAllFilesOfCategory(enum E_FV_FileCategory eFileCat,
                                                      SAFEARRAY** parrFileNames)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (parrFileNames == NULL)
    {
        return E_POINTER;
    }

    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pClassTree);
        PROJ_MAN_ASSERT(m_pTargetTree);
        PROJ_MAN_ASSERT(m_pProjectFileHandler);
        try
        {
            CList<CString, CString&> listFileNames;
            switch(eFileCat)
            {
            case FV_FILE_SOURCE:
                // SOURCE files
                m_pClassTree->GetAllTreeSourceFiles(listFileNames);
                m_pTargetTree->GetAllTreeSourceFiles(listFileNames);
                break;
            case FV_FILE_HELP:
                // HELP files
                {
                    m_pProjectFileHandler->ReparseProjectFile();
                    CSection* pSection = NULL;
                    if (!m_pProjectFileHandler->FindSection(CE_CP_USERHELP, &pSection))
                    {
                        return S_FALSE;
                    }
                    PROJ_MAN_ASSERT(pSection);
                    CKeyAndValue* pKeyAndValue;
                    BOOL ret = pSection->GetFirstKeyAndValue(&pKeyAndValue);
                    while(ret)
                    {        
                        PROJ_MAN_ASSERT(pKeyAndValue);
                        if (pKeyAndValue->GetKey().CompareNoCase(CE_CP_FILE)==0)
                        {
                            listFileNames.AddTail(pKeyAndValue->GetValue());
                        }
                        ret = pSection->GetNextKeyAndValue(&pKeyAndValue);
                    }
                }
                break;
            case FV_FILE_ADDITIONAL:
                // Additional Files
                {
                    m_pProjectFileHandler->ReparseProjectFile();
                    CSection* pSection = NULL;
                    if (!m_pProjectFileHandler->FindSection(CE_CP_ADDFILES, &pSection))
                    {
                        return S_FALSE;
                    }
                    PROJ_MAN_ASSERT(pSection);
                    CKeyAndValue* pKeyAndValue;
                    BOOL ret = pSection->GetFirstKeyAndValue(&pKeyAndValue);
                    while(ret)
                    {        
                        PROJ_MAN_ASSERT(pKeyAndValue);
                        if (pKeyAndValue->GetKey().CompareNoCase(CE_CP_FILE)==0)
                        {
                            listFileNames.AddTail(pKeyAndValue->GetValue());
                        }
                        ret = pSection->GetNextKeyAndValue(&pKeyAndValue);
                    }
                }
                break;
            default:
                return S_FALSE;
            }

            // fill SAFEARRAY
            HRESULT hr;
            SAFEARRAYBOUND Bound;
            Bound.lLbound = 0;
            Bound.cElements = listFileNames.GetCount();

            *parrFileNames = ::SafeArrayCreate(VT_BSTR, 1, &Bound);
            if (NULL == (*parrFileNames)) 
            {
                return E_FAIL;    
            }

            long lIndex = 0;
            POSITION pos = listFileNames.GetHeadPosition();
            CString str;

            while(pos!=NULL)
            {
                str = listFileNames.GetNext(pos);
                CComBSTR sStr(str);
                hr = ::SafeArrayPutElement(*parrFileNames, &lIndex, 
                                            (void*)sStr);
                if (FAILED(hr))
                {
                    ::SafeArrayDestroy(*parrFileNames);
                    return E_FAIL;
                }
                lIndex++;
            }

            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;
    }
}


//------------------------------------------------------------------*
// GetFileCategory
//

STDMETHODIMP CCEProjectManager::GetFileCategory(BSTR sFileName,
                                                enum E_FV_FileCategory* peFileCat)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (peFileCat == NULL)
    {
        return E_POINTER;
    }

	*peFileCat = FV_FILE_UNKNOWN;

    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pClassTree);
        PROJ_MAN_ASSERT(m_pTargetTree);
        PROJ_MAN_ASSERT(m_pProjectFileHandler);
        try
        {
            CString strFileName(sFileName);
            if (m_pClassTree->IsTreeSourceFile(strFileName)
                || m_pTargetTree->IsTreeSourceFile(strFileName))
            {
                *peFileCat = FV_FILE_SOURCE;
                return S_OK;
            }

            // search in USER_HELP section in 4cp file
            m_pProjectFileHandler->ReparseProjectFile();
            CSection* pSection = NULL;
            if (!m_pProjectFileHandler->FindSection(CE_CP_USERHELP, &pSection))
            {
                *peFileCat = FV_FILE_UNKNOWN;
                return S_OK;
            }
            PROJ_MAN_ASSERT(pSection);
            
            CKeyAndValue* pKeyAndValue;
            if (pSection->FindKeyAndValue(CE_CP_FILE, strFileName, &pKeyAndValue))
            {
                *peFileCat = FV_FILE_HELP;
                return S_OK;
            }

            // search in ADD_FILES section in 4cp file
            if (!m_pProjectFileHandler->FindSection(CE_CP_ADDFILES, &pSection))
            {
                *peFileCat = FV_FILE_UNKNOWN;
                return S_OK;
            }
            PROJ_MAN_ASSERT(pSection);
            
            if (pSection->FindKeyAndValue(CE_CP_FILE, strFileName, &pKeyAndValue))
            {
                *peFileCat = FV_FILE_ADDITIONAL;
                return S_OK;
            }

            *peFileCat = FV_FILE_UNKNOWN;
            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;
    }
}

 
//------------------------------------------------------------------*
// AddFile
//
STDMETHODIMP CCEProjectManager::AddFile(BSTR sFileName,
                                        enum E_FV_FileCategory eFileCat,
                                        BOOL bTry)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_bProjectLoaded)
    {
        try
        {
            PROJ_MAN_ASSERT(m_pClassTree);
            PROJ_MAN_ASSERT(m_pTargetTree);
            PROJ_MAN_ASSERT(m_pProjectFileHandler);
            CString strFileName(sFileName);
            strFileName = CFileHelper::StripProjectPath(strFileName);
            switch(eFileCat)
            {
            case FV_FILE_SOURCE:
                {
                    CComBSTR sBP;
                    ICEBackparse* pBackparser = NULL;

                    CString strFileExt  = CFileHelper::GetFileExt(strFileName); 

                    pBackparser = GetBackparserForFileExt(strFileExt);

                    if (!pBackparser)
                    {
                        return S_FALSE;
                    }
                    CString strFullFileName = CFileHelper::GetFileAbsolute(strFileName); 
                    CComBSTR sFN(strFullFileName);

                    IUnknown* pUnk = GetUnknown();
                    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pProjInfo;
                    pProjInfo = pUnk;
                    PROJ_MAN_ASSERT(!(!pProjInfo));
                    HRESULT hr = pBackparser->doBackparse(sFN, pProjInfo, &sBP);
                    pBackparser->Release();         
                    if (!SUCCEEDED(hr))
                    {
                        return S_FALSE;
                    }
                    CString strBackparser(sBP);

                    if (m_pClassTree->IsTreeSourceFile(strFileName)
                        ||m_pTargetTree->IsTreeSourceFile(strFileName))
                    {
                        return S_FALSE;
                    }

                    if (m_pClassTree->FVAddFile(strFileName, strBackparser, bTry))
                    {
                        return S_OK;
                    }
                    if (m_pTargetTree->FVAddFile(strFileName, strBackparser, bTry))
                    {
                        return S_OK;
                    }
                    return S_FALSE;
                }
                break;
            case FV_FILE_HELP:
                {
                    CSection* pSection;
                    if (!m_pProjectFileHandler->FindSection(CE_CP_USERHELP, &pSection))
                    {
                        if (!m_pProjectFileHandler->AddSection(CE_CP_USERHELP))
                        {
                            return S_FALSE;
                        }
                        if (!m_pProjectFileHandler->FindSection(CE_CP_USERHELP, &pSection))
                        {
                            return S_FALSE;
                        }
                    }
                    // if not only try -> do it
                    if(!bTry)
                    {
                        PROJ_MAN_ASSERT(pSection);
                        CKeyAndValue* pKeyAndValue;
                        if (pSection->FindKeyAndValue(CE_CP_FILE, strFileName, &pKeyAndValue))
                        {
                            return S_FALSE;
                        }
                        if (!pSection->AddKeyAndValue(CE_CP_FILE, strFileName))
                        {
                            return S_FALSE;
                        }
                        m_pProjectFileHandler->WriteProjectFile();
                    }
                    return S_OK;
                }
                break;
            case FV_FILE_ADDITIONAL:
                {
                    CSection* pSection;
                    if (!m_pProjectFileHandler->FindSection(CE_CP_ADDFILES, &pSection))
                    {
                        if (!m_pProjectFileHandler->AddSection(CE_CP_ADDFILES))
                        {
                            return S_FALSE;
                        }
                        if (!m_pProjectFileHandler->FindSection(CE_CP_ADDFILES, &pSection))
                        {
                            return S_FALSE;
                        }
                    }
                    // if not only try -> do it
                    if(!bTry)
                    {
                        PROJ_MAN_ASSERT(pSection);
                        CKeyAndValue* pKeyAndValue;
                        if (pSection->FindKeyAndValue(CE_CP_FILE, strFileName, &pKeyAndValue))
                        {
                            return S_FALSE;
                        }
                        if (!pSection->AddKeyAndValue(CE_CP_FILE, strFileName))
                        {
                            return S_FALSE;
                        }
                        m_pProjectFileHandler->WriteProjectFile();
                    }
                    return S_OK;
                }
                break;
            default:
                return S_FALSE;
            }
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;
    }
}


//------------------------------------------------------------------*
/**
 * Remove file.
 *
 * @param           sFileName: file name
 * @param           eFileCat: file category
 * @param           bTry: just try to remove but don't
 * @return          S_OK: file has been remove from category
 *                  S_FALSE: refused to remove (4cp and con)
 *                  E_FAIL: file was not found in category
 *                  E_OUTOFMEMORY, E_CEPROJMAN_NO_PROJECT_LOADED
 * @exception       -
 * @see             -
*/
STDMETHODIMP CCEProjectManager::RemoveFile(BSTR sFileName,
                                           enum E_FV_FileCategory eFileCat,
                                           BOOL bTry)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pClassTree);
        PROJ_MAN_ASSERT(m_pTargetTree);
        PROJ_MAN_ASSERT(m_pProjectFileHandler);
        try
        {
            CString strFileName(sFileName);
            strFileName = CFileHelper::StripProjectPath(strFileName);
            switch(eFileCat)
            {
            case FV_FILE_SOURCE:
                if (m_pClassTree->IsTreeSourceFile(strFileName))
                {
                    // I don't allow the project or configuration file to be deleted
                    CString strExt = strFileName.Right(4);
                    if(strExt.CompareNoCase(_T(".4cp")) == 0 ||
                        strExt.CompareNoCase(_T(".con")) == 0)
                    {
                        return S_FALSE;
                    }
                    if (!m_pClassTree->FVRemoveFile(strFileName, bTry))
                    {
                        return E_FAIL;
                    }
                    return S_OK;
                }

                if (m_pTargetTree->IsTreeSourceFile(strFileName))
                {
                    if (!m_pTargetTree->FVRemoveFile(strFileName, bTry))
                    {
                        return E_FAIL;
                    }
                    return S_OK;
                }
                return E_FAIL;
            case FV_FILE_HELP:
                {
                    // if not only try -> do it
                    if(!bTry)
                    {
                        CSection* pSection;
                        if (!m_pProjectFileHandler->FindSection(CE_CP_USERHELP, &pSection))
                        {
                            return E_FAIL;
                        }
                        PROJ_MAN_ASSERT(pSection);               
                        if (!pSection->DeleteKeyAndValue(CE_CP_FILE, strFileName))
                        {
                            return E_FAIL;
                        }
                        m_pProjectFileHandler->WriteProjectFile();
                    }
                    return S_OK;
                }
                break;
            case FV_FILE_ADDITIONAL:
                {
                    // if not only try -> do it
                    if(!bTry)
                    {
                        CSection* pSection;
                        if (!m_pProjectFileHandler->FindSection(CE_CP_ADDFILES, &pSection))
                        {
                            return E_FAIL;
                        }
                        PROJ_MAN_ASSERT(pSection);               
                        if (!pSection->DeleteKeyAndValue(CE_CP_FILE, strFileName))
                        {
                            return E_FAIL;
                        }
                        m_pProjectFileHandler->WriteProjectFile();
                    }
                    return S_OK;
                }
                break;
            default:
                return E_FAIL;
            }        
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;
    }	
}


STDMETHODIMP CCEProjectManager::CompileReady()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_bProjectLoaded)
    {
        m_pInstanceTree->UpdateTargets();

        m_pInstanceTree->CheckSourceFilesToReparse();
           
        m_pInstanceTree->Reparse();
    
        return S_OK;
    }
    else
    {
        return E_CEPROJMAN_NO_PROJECT_LOADED;
    }
}








/////////////////////////////////////////////////////////////////////
// Implementation of ICEProjInfo
//


//------------------------------------------------------------------*
// getProjectPath
//
STDMETHODIMP CCEProjectManager::getProjectPath(BSTR * psProjectPath)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (psProjectPath == NULL)
    {
        return E_POINTER;
    }

    if (m_bProjectLoaded)
    {
        try
        {
            *psProjectPath = m_strProjectPath.AllocSysString();
            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;
    }
    
    
}



//------------------------------------------------------------------*
// getProjectName
//
STDMETHODIMP CCEProjectManager::getProjectName(BSTR * psProjectName)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (psProjectName == NULL)
        return E_POINTER;

   
    if (m_bProjectLoaded)
    {
         try
        {
            *psProjectName = m_strProjectName.AllocSysString();
            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;    
    }
    
}



//------------------------------------------------------------------*
// getProjectGenPath
//
STDMETHODIMP CCEProjectManager::getProjectGenPath(BSTR * psGenPath)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (psGenPath == NULL)
    {
        return E_POINTER;
    }
        
   
    if (m_bProjectLoaded)
    {
        try
        {
            CString strGenPath;
            strGenPath = m_strProjectPath +_T("\\") + CE_GEN_PATH;
            *psGenPath = strGenPath.AllocSysString();
            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }
    
}



//------------------------------------------------------------------*
// getLibraryPath
//
STDMETHODIMP CCEProjectManager::getLibraryPath(BSTR* psPath)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (psPath == NULL)
    {
        return E_POINTER;
    }

    try
    {
        TCHAR pszLibPath[1000];   
        HRESULT hr = UTIL_GetStdLibPath(pszLibPath,1000);
        if (hr==S_OK)
        {
            CString strLibPath(pszLibPath);
            *psPath = strLibPath.AllocSysString();
        }
        
        return hr;
    }
    catch (CMemoryException* e)
    {
        e->Delete();
        return E_OUTOFMEMORY;
    }   
}



//------------------------------------------------------------------*
// getInstallationPath
//
STDMETHODIMP CCEProjectManager::getInstallationPath(BSTR* psPath)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (psPath == NULL)
    {
        return E_POINTER;
    }
       
    try
    {
        TCHAR pszInstPath[1000];   
        size_t sizeChars = 1000;
        HRESULT hr = UTIL_GetInstallPath(pszInstPath, &sizeChars);
        if (hr==S_OK)
        {
            CString strInstPath(pszInstPath);
            *psPath = strInstPath.AllocSysString();
        }
        return hr;
    }
    catch (CMemoryException* e)
    {
        e->Delete();
        return E_OUTOFMEMORY;
    }
}



//------------------------------------------------------------------*
// getXMLNodePtr
//
STDMETHODIMP CCEProjectManager::getXMLNodePtr(BSTR sId, IUnknown** ppXMLPtr)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (ppXMLPtr == NULL)
    {
        return E_POINTER;
    }

    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pClassTree);
        PROJ_MAN_ASSERT(m_pTargetTree);
        PROJ_MAN_ASSERT(m_pInstanceTree);
        try
        {
            CString strId(sId);
            if (m_pClassTree->IsViewPath(strId))
            {
                if (!m_pClassTree->GetXMLNodePtr(strId, ppXMLPtr))
                {
                    return S_FALSE;
                }
                return S_OK;
            } 
            else if (m_pTargetTree->IsViewPath(strId))
            {
                if (!m_pTargetTree->GetXMLNodePtr(strId, ppXMLPtr))
                {
                    return S_FALSE;
                }
                return S_OK;
            }
            else if (m_pInstanceTree->IsViewPath(strId))
            {
                if (!m_pInstanceTree->GetXMLNodePtr(strId, ppXMLPtr))
                {
                    return S_FALSE;
                }
                return S_OK;
            }
            return S_FALSE;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }        
}



//------------------------------------------------------------------*
// getContainerIF
//
STDMETHODIMP CCEProjectManager::getContainerIF(BSTR sSourceFile, 
                                               BSTR sId, 
                                               ICEContainer** ppICEContainer, 
                                               BSTR* psParentSource, 
                                               BSTR* psParentId)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (  (ppICEContainer == NULL)        
        ||(psParentSource == NULL)
        ||(psParentId == NULL) )
    {
        return E_POINTER;
    }

    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pClassTree);
        PROJ_MAN_ASSERT(m_pTargetTree);
        try
        {
            BOOL ret = FALSE;
            CString strParentSource;
            CString strParentId;
            CString strSourceFile(sSourceFile);
            // file name only, no path
            strSourceFile = CFileHelper::StripProjectPath(strSourceFile);

            if (m_pClassTree->IsViewPath((CString)sId))
            {
                ret = m_pClassTree->GetParentSourceAndId(strSourceFile, 
                                                         strParentSource,
                                                         strParentId);
            }
            else if (m_pTargetTree->IsViewPath((CString)sId))
            {
                ret = m_pTargetTree->GetParentSourceAndId(strSourceFile,
                                                          strParentSource,
                                                          strParentId);
            }
            if (!ret)
            {
                return S_FALSE;
            }

            CString strFileExt = CFileHelper::GetFileExt(strParentSource);
            PROJ_MAN_ASSERT(strFileExt.GetLength()>0);
            if (strFileExt.IsEmpty())
            {
                return E_FAIL;
            }
            
            ICEContainer* pCont = GetContainerForFileExt(strFileExt);
            if (!pCont)
            {
                return S_FALSE;
            }

            *ppICEContainer = pCont;

            *psParentSource = strParentSource.AllocSysString();           
            *psParentId = strParentId.AllocSysString();

            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }    
}



//------------------------------------------------------------------*
// getContainerIFforFile
//
STDMETHODIMP CCEProjectManager::getContainerIFforFile(BSTR sContainerFile, 
                                                      ICEContainer** ppICEContainer)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (ppICEContainer == NULL)
    {
        return E_POINTER;
    }

    *ppICEContainer = NULL;

    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pClassTree);
        PROJ_MAN_ASSERT(m_pTargetTree);
        try
        {
            CString strFileName = sContainerFile;
            if (strFileName.IsEmpty())
            {
                return E_FAIL;
            }
            CString strFileExt = CFileHelper::GetFileExt(strFileName);
            if (strFileExt.IsEmpty())
            {
                strFileExt = strFileName;
            }
           
            strFileExt.MakeLower();

            ICEContainer* pCont = GetContainerForFileExt(strFileExt);
            if (!pCont)
            {
                return S_FALSE;
            }

            *ppICEContainer = pCont;

            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }
}



//------------------------------------------------------------------*
// getAllTargetTypes
//
STDMETHODIMP CCEProjectManager::getAllTargetTypes(SAFEARRAY** parrNames)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (parrNames == NULL)
    {
        return E_POINTER;
    }

    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pTargetTree);
        try
        {
            if (!m_pTargetTree->GetAllTargetTypes(parrNames))
            {
                return S_FALSE;
            }
            return S_OK;        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }
}



//------------------------------------------------------------------*
// getAllTargets
//
STDMETHODIMP CCEProjectManager::getAllTargets(SAFEARRAY** parrTargets)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (parrTargets == NULL)
    {
        return E_POINTER;
    }
     
    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pTargetTree);
        try
        {
            if (!m_pTargetTree->GetAllTargets(parrTargets,FALSE))
            {
                return S_FALSE;
            }
            return S_OK;        
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }
}


//------------------------------------------------------------------*
// getAllTargets
//
STDMETHODIMP CCEProjectManager::getAllTargetsAsIdPaths(SAFEARRAY** parrTargets)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (parrTargets == NULL)
    {
        return E_POINTER;
    }
     
    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pTargetTree);
        try
        {
            if (!m_pTargetTree->GetAllTargets(parrTargets,TRUE))
            {
                return S_FALSE;
            }
            return S_OK;        
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }
}




//------------------------------------------------------------------*
// getExtForTargetType
//
STDMETHODIMP CCEProjectManager::getExtForTargetType(BSTR sTargetType, BSTR* psExt)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (psExt == NULL)
    {
        return E_POINTER;
    }

    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pTargetTree);
        try
        {
            CString strTargetType = sTargetType;
            CString strExt;
            if (!m_pTargetTree->GetExtForTarget(strTargetType, strExt))
            {
                return S_FALSE;
            }
            *psExt = strExt.AllocSysString();
            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }  
}



//------------------------------------------------------------------*
// getAllClassTypes
//
STDMETHODIMP CCEProjectManager::getAllClassTypes(SAFEARRAY** parrNames)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (parrNames == NULL)
    {
        return E_POINTER;
    }

    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pClassTree);
        try
        {
            if (!m_pClassTree->GetAllClassTypes(parrNames))
            {
                return S_FALSE;
            }
            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }
}



//------------------------------------------------------------------*
// getTextForClassType
//
STDMETHODIMP CCEProjectManager::getTextForClassType(BSTR sType, BSTR* psText)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (psText == NULL)
    {
        return E_POINTER;
    }

    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pClassTree);
        try
        {
            CString strClassType = sType;
            CString strText;
            if (!m_pClassTree->GetTextForClassType(strClassType, strText))
            {
                return S_FALSE;
            }
            *psText = strText.AllocSysString();
            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }    
}



//------------------------------------------------------------------*
// getAllClasses
//
STDMETHODIMP CCEProjectManager::getAllClasses(SAFEARRAY** parrFileNames)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (parrFileNames == NULL)
    {
        return E_POINTER;
    }

    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pClassTree);
        try
        {
            if (!m_pClassTree->GetAllClasses(parrFileNames,FALSE))
            {
                return S_FALSE;
            }
            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }    
}


//------------------------------------------------------------------*
// getAllClassesAsIdPaths
//
STDMETHODIMP CCEProjectManager::getAllClassesAsIdPaths(SAFEARRAY** parrClasses)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (parrClasses == NULL)
    {
        return E_POINTER;
    }

    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pClassTree);
        try
        {
            if (!m_pClassTree->GetAllClasses(parrClasses,TRUE))
            {
                return S_FALSE;
            }
            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }    
}



//------------------------------------------------------------------*
// getClassesOfType
//
STDMETHODIMP CCEProjectManager::getClassesOfType(BSTR sClassType, SAFEARRAY** parrFileNames)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (parrFileNames == NULL)
    {
        return E_POINTER;
    }

    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pClassTree);
        try
        {
            CString strClassType(sClassType);
            if (!m_pClassTree->GetAllClassesOfType(strClassType, parrFileNames,FALSE))
            {
                return S_FALSE;
            }
            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }    
}


//------------------------------------------------------------------*
// getClassesOfTypeAsIdPaths
//
STDMETHODIMP CCEProjectManager::getClassesOfTypeAsIdPaths(BSTR sClassType, 
                                                          SAFEARRAY** parrClasses)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (parrClasses == NULL)
    {
        return E_POINTER;
    }

    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pClassTree);
        try
        {
            CString strClassType(sClassType);
            if (!m_pClassTree->GetAllClassesOfType(strClassType, parrClasses,TRUE))
            {
                return S_FALSE;
            }
            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }    
}



//------------------------------------------------------------------*
// getAllLibraries
//
STDMETHODIMP CCEProjectManager::getAllLibraries(SAFEARRAY** parrLibraries)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (parrLibraries == NULL)
    {
        return E_POINTER;
    }

    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pClassTree);
        try
        {
            CString strClassType(_T("LIBRARY"));
            if (!m_pClassTree->GetAllClassesOfType(strClassType, parrLibraries, FALSE))
            {
                return S_FALSE;
            }
            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }    
}



//------------------------------------------------------------------*
// getClassesOfTypeFromLib
//
STDMETHODIMP CCEProjectManager::getClassesOfTypeFromLib(BSTR sLibraryName, 
                                                        BSTR sClassType, 
                                                        SAFEARRAY** parrFileNames)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (parrFileNames == NULL)
    {
        return E_POINTER;
    }

    if (m_bProjectLoaded)
    {
        try
        {
            // TODO
            return E_NOTIMPL;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }            
}



//------------------------------------------------------------------*
// getTypeOfTarget
//
STDMETHODIMP CCEProjectManager::getTypeOfTarget(BSTR sTargetName, BSTR* psTargetType)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (psTargetType == NULL)
    {
        return E_POINTER;
    }

    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pTargetTree);
        try
        {
            CString strTargetName(sTargetName);
            CString strTargetType;
            if (!m_pTargetTree->GetTypeOfTarget(strTargetName, strTargetType))
            {
                return S_FALSE;
            }
            *psTargetType = strTargetType.AllocSysString();
            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }            
}



//------------------------------------------------------------------*
// getAddressStringForTarget
//
STDMETHODIMP CCEProjectManager::getAddressStringForTarget(BSTR sTargetName, BSTR* psAddrString)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (psAddrString == NULL)
    {
        return E_POINTER;
    }

    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pTargetTree);
        try
        {
            CString strTargetName(sTargetName);
            CString strAddrString;
            if (!m_pTargetTree->GetAddressStringForTarget(strTargetName, strAddrString))
            {
                return S_FALSE;
            }
            *psAddrString = strAddrString.AllocSysString();
            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }            
}



//------------------------------------------------------------------*
// getUserAddressStringForTarget
//
STDMETHODIMP CCEProjectManager::getUserAddressStringForTarget(BSTR sTargetName, BSTR* psAddrString)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (psAddrString == NULL)
    {
        return E_POINTER;
    }

    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pTargetTree);
        try
        {
            CString strTargetName(sTargetName);
            CString strAddrString;
            if (!m_pTargetTree->GetUserAddressStringForTarget(strTargetName, strAddrString))
            {
                return S_FALSE;
            }
            *psAddrString = strAddrString.AllocSysString();
            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }        
}



//------------------------------------------------------------------*
// getTargetNameForAddr
//
STDMETHODIMP CCEProjectManager::getTargetNameForAddr(BSTR sAddr, BSTR* psTargetName)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (psTargetName == NULL)
    {
        return E_POINTER;
    }

    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pTargetTree);
        try
        {
            CString strAddr(sAddr);
            CString strTargetName;
            if (!m_pTargetTree->GetTargetNameForAddr(strAddr, strTargetName))
            {
                return S_FALSE;
            }
            *psTargetName = strTargetName.AllocSysString();
            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }    
}



//------------------------------------------------------------------*
// getResourceNameForTarget
//
STDMETHODIMP CCEProjectManager::getResourceNameForTarget(BSTR sTarget, BSTR* psResourceName)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (psResourceName == NULL)
    {
        return E_POINTER;
    }

    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pTargetTree);
        try
        {
            CString strTarget(sTarget);
            CString strResourceName;
            if (!m_pTargetTree->GetResourceNameForTarget(strTarget, strResourceName))
            {
                return S_FALSE;
            }
            *psResourceName = strResourceName.AllocSysString();
            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }    
}



//------------------------------------------------------------------*
// getFreeChannelNrForPC
//
STDMETHODIMP CCEProjectManager::getFreeChannelNrForPC(BSTR sPCName, long* plChannelNr)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (plChannelNr == NULL)
    {
        return E_POINTER;
    }

    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pTargetTree);
        try
        {
            CString strPCName(sPCName);
            long lNr;
            if (!m_pTargetTree->GetFreeChannelNrForPC(strPCName, lNr))
            {
                return S_FALSE;
            }
            *plChannelNr = lNr;
            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }            
}



//------------------------------------------------------------------*
// getGlobalNameProposal
//
STDMETHODIMP CCEProjectManager::getGlobalNameProposal(BSTR sBasis, BSTR* psName)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (psName == NULL)
    {
        return E_POINTER;
    }

    if (m_bProjectLoaded)
    {
        try
        {
            // first simple try: 
            // the name proposal will be something like 'sBasis'<nr>
            // look at all files in the project directory like 'sBasis'*
            // test all nr until the concatination is a new file name
            // don't look at the file extention

            // get all files 'sBasis'* in project dir, collect all * in list
            CList<CString, CString&> noPossibleCont;
            CString strBasis(sBasis);
            CString strFullFileName;
            strFullFileName = GetProjectPath() + _T("\\") + strBasis + _T("*");

            WIN32_FIND_DATA findFileData;
            HANDLE hFind;

            hFind = FindFirstFile(strFullFileName, &findFileData);
            BOOL bFound = (hFind==INVALID_HANDLE_VALUE)?FALSE:TRUE;

            while (bFound)
            {
                CString strExt;
                CString strFileName = findFileData.cFileName;
                int iPointPos = strFileName.ReverseFind(_T('.'));
                if (iPointPos>1)
                {
                    strFileName = strFileName.Left(iPointPos);
                }
                if (strFileName.GetLength()>=strBasis.GetLength())
                {
                    strExt = strFileName.Right(strFileName.GetLength()-strBasis.GetLength());
                }

                if (!strExt.IsEmpty())
                {
                    noPossibleCont.AddTail(strExt);
                }
                bFound = FindNextFile(hFind, &findFileData);
            }

            int iExt = 1;
            CString strPropExt;
            strPropExt.Format(_T("%i"), iExt);
            while (noPossibleCont.Find(strPropExt))
            {
                iExt++;
                strPropExt.Format(_T("%i"), iExt);
            }
            CString strProposal = strBasis + strPropExt;
            *psName = strProposal.AllocSysString();
            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }            
}







//------------------------------------------------------------------*
// fileChangedHint
//
STDMETHODIMP CCEProjectManager::fileChangedHint(BSTR sSourceFile)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    HRESULT hr = S_FALSE;

    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pClassTree);
        PROJ_MAN_ASSERT(m_pTargetTree);
        try
        {
            CString strFileName(sSourceFile);
            strFileName = CFileHelper::StripProjectPath(strFileName);

            if (strFileName.IsEmpty())
            {
                strFileName = m_strProjectFileName;
            }
            if (m_pClassTree->IsTreeSourceFile(strFileName))
            {
                m_pClassTree->AddSourceFileToReparse(strFileName);
                hr = S_OK;
            }
            if (m_pTargetTree->IsTreeSourceFile(strFileName))
            {
                m_pTargetTree->AddSourceFileToReparse(strFileName);
                hr = S_OK;
            }
            return hr;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }    
}



//------------------------------------------------------------------*
// fileParseNow
//
STDMETHODIMP CCEProjectManager::fileParseNow(BSTR sSourceFile)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    HRESULT hr = S_FALSE;

    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pClassTree);
        PROJ_MAN_ASSERT(m_pTargetTree);
        try
        {
            CString strFileName(sSourceFile);
            strFileName = CFileHelper::StripProjectPath(strFileName);

            if (strFileName.IsEmpty())
            {
                m_pClassTree->Reparse();
                m_pTargetTree->Reparse();
                m_pInstanceTree->UpdateTargets();
            }
            else if (m_pClassTree->IsTreeSourceFile(strFileName))
            {
                m_pClassTree->AddSourceFileToReparse(strFileName);
             ProjectManagerLock();
                m_pClassTree->Reparse();
             ProjectManagerUnlock();
                hr = S_OK;
            }
            else if (m_pTargetTree->IsTreeSourceFile(strFileName))
            {
                m_pTargetTree->AddSourceFileToReparse(strFileName);
             ProjectManagerLock();
                m_pTargetTree->Reparse();
             ProjectManagerUnlock();
                hr = S_OK;
            }

            ASSERT(m_pProjectFileHandler);
            m_pProjectFileHandler->UpdateFileViewInfo();

            return hr;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }    
}


//------------------------------------------------------------------*
// getFileNameForIdPath
//
STDMETHODIMP CCEProjectManager::getFileNameFromIdPath(BSTR  sIdPath, BSTR* psFileName)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (psFileName == NULL)
    {
        return E_POINTER;
    }

    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pClassTree);
        PROJ_MAN_ASSERT(m_pTargetTree);
        PROJ_MAN_ASSERT(m_pInstanceTree);
        try
        {
            CString strIdPath(sIdPath);
            if (m_pClassTree->IsViewPath(strIdPath))
            {
                CString strFileName;
                if (!m_pClassTree->GetFileNameFromIdPath(strIdPath, strFileName))
                {
                    return S_FALSE;
                }
                *psFileName = strFileName.AllocSysString();
                return S_OK;
            }
            else if (m_pTargetTree->IsViewPath(strIdPath))
            {
                CString strFileName;
                if (!m_pTargetTree->GetFileNameFromIdPath(strIdPath, strFileName))
                {
                    return S_FALSE;
                }
                *psFileName = strFileName.AllocSysString();
                return S_OK;
            }
            else if (m_pInstanceTree->IsViewPath(strIdPath))
            {
                CString strFileName;
                if (!m_pInstanceTree->GetFileNameFromIdPath(strIdPath, strFileName))
                {
                    return S_FALSE;
                }
                *psFileName = strFileName.AllocSysString();
                return S_OK;
            }
            return S_FALSE;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }    
}


//------------------------------------------------------------------*
// getIdPathForFileName
//
STDMETHODIMP CCEProjectManager::getIdPathFromFileName(BSTR  sFileName, BSTR* psIdPath)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (psIdPath == NULL)
    {
        return E_POINTER;
    }

    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pClassTree);
        PROJ_MAN_ASSERT(m_pTargetTree);
        try
        {
            CString strFileName(sFileName);
            if (m_pClassTree->IsTreeSourceFile(strFileName))
            {
                CString strIdPath;
                m_pClassTree->GetIdPathFromFileName(strFileName, strIdPath);
                *psIdPath = strIdPath.AllocSysString();
                return S_OK;
            }
            else if (m_pTargetTree->IsTreeSourceFile(strFileName))
            {
                CString strIdPath;
                m_pTargetTree->GetIdPathFromFileName(strFileName, strIdPath);
                *psIdPath = strIdPath.AllocSysString();
                return S_OK;
            }
            return S_FALSE;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }    
}



//------------------------------------------------------------------*
// CPInsertToSection
//
STDMETHODIMP CCEProjectManager::CPInsertToSection(BSTR sSection, BSTR sKey, BSTR sValue)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_bProjectLoaded)
    {
        try
        {
            CString strSection = sSection;
            CString strKey = sKey;
            CString strValue = sValue;
            CSection* sec = NULL;


            PROJ_MAN_ASSERT(m_pProjectFileHandler);
            m_pProjectFileHandler->ReparseProjectFile();
            BOOL ret = m_pProjectFileHandler->FindSection(strSection, &sec);
            if (!ret)
            {
                if (!m_pProjectFileHandler->AddSection(strSection))
                {
                    return S_FALSE;
                }
                ret = m_pProjectFileHandler->FindSection(strSection, &sec);
                if (!ret)
                {
                    return S_FALSE;
                }
            }
            PROJ_MAN_ASSERT(sec!=NULL);
            CKeyAndValue* kav = NULL;
            if (sec->FindKeyAndValue(strKey, strValue, &kav)) // key and value already there
            {
                return S_OK;
            }
            if (!sec->AddKeyAndValue(strKey, strValue))
            {
                return S_FALSE;
            }
            m_pProjectFileHandler->WriteProjectFile(m_strProjectFileNameWithPath);
            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }    
}



//------------------------------------------------------------------*
// CPDeleteFromSection
//
STDMETHODIMP CCEProjectManager::CPDeleteFromSection(BSTR sSection, BSTR sKey)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_bProjectLoaded)
    {
        try
        {
            CString strSection = sSection;
            CString strKey = sKey;
            CSection* sec = NULL;
            CKeyAndValue* key = NULL;

            PROJ_MAN_ASSERT(m_pProjectFileHandler);
            m_pProjectFileHandler->ReparseProjectFile();
            BOOL ret = m_pProjectFileHandler->FindSection(strSection, &sec);
            if (!ret)
            {
                return S_FALSE;
            }
            PROJ_MAN_ASSERT(sec);
            ret = sec->GetFirstKeyAndValue(&key);
            while(ret)
            {
                PROJ_MAN_ASSERT(key);
                if (key->GetKey().CompareNoCase(strKey)==0)
                {
                    if (!sec->DeleteKeyAndValue(key->GetKey(), key->GetValue()))
                    {
                        return S_FALSE;
                    }
                    else 
                    {
                        m_pProjectFileHandler->WriteProjectFile(m_strProjectFileNameWithPath);
                        return S_OK;
                    }
                }
                ret = sec->GetNextKeyAndValue(&key);
            }
            return S_FALSE;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }    
}



//------------------------------------------------------------------*
// CPDeleteKeyWithValueFromSection
//
STDMETHODIMP CCEProjectManager::CPDeleteKeyWithValueFromSection(BSTR sSection, BSTR sKey, BSTR sValue)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    if (m_bProjectLoaded)
    {
        try
        {
            CString strSection = sSection;
            CString strKey = sKey;
            CString strValue = sValue;
            CSection* sec = NULL;

            PROJ_MAN_ASSERT(m_pProjectFileHandler);
            m_pProjectFileHandler->ReparseProjectFile();
            BOOL ret = m_pProjectFileHandler->FindSection(strSection, &sec);
            if (!ret)
            {
                return S_FALSE;
            }
            PROJ_MAN_ASSERT(sec);
            if (!sec->DeleteKeyAndValue(strKey, strValue))
            {
                return S_FALSE;
            }
            m_pProjectFileHandler->WriteProjectFile(m_strProjectFileNameWithPath);
            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }    
}



//------------------------------------------------------------------*
// CPGetValueForKeyFromSection
//
STDMETHODIMP CCEProjectManager::CPGetValueForKeyFromSection(BSTR sSection, 
                                                            BSTR sKey,     
                                                            BSTR * psValue)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (psValue == NULL)
    {
        return E_POINTER;
    }

    if (m_bProjectLoaded)
    {
        try
        {
            CString strSection = sSection;
            CString strKey = sKey;
            CString strValue;
            CSection* sec = NULL;
            CKeyAndValue* key = NULL;

            PROJ_MAN_ASSERT(m_pProjectFileHandler);
            m_pProjectFileHandler->ReparseProjectFile();
            BOOL ret = m_pProjectFileHandler->FindSection(strSection, &sec);
            if (!ret)
            {
                return S_FALSE;
            }
            PROJ_MAN_ASSERT(sec);
            ret = sec->GetFirstKeyAndValue(&key);
            while (ret)
            {
                PROJ_MAN_ASSERT(key);
                if (key->GetKey().CompareNoCase(strKey)==0)
                {
                    strValue = key->GetValue();
                    *psValue = strValue.AllocSysString();
                    return S_OK;
                }
                ret = sec->GetNextKeyAndValue(&key);
            }
            return S_FALSE;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }            
}



//------------------------------------------------------------------*
// CPGetAllValuesForKeyFromSection
//
STDMETHODIMP CCEProjectManager::CPGetAllValuesForKeyFromSection(BSTR sSection, 
                                                                BSTR sKey, 
                                                                SAFEARRAY** parrFileNames)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (parrFileNames == NULL)
    {
        return E_POINTER;
    }

    *parrFileNames = NULL;

    if (m_bProjectLoaded)
    {
        try
        {
            CString strSection = sSection;
            CString strKey = sKey;
            CList<CString, CString&> listValues;
            CSection* sec = NULL;
            CKeyAndValue* key = NULL;

            PROJ_MAN_ASSERT(m_pProjectFileHandler);
            m_pProjectFileHandler->ReparseProjectFile();
            BOOL ret = m_pProjectFileHandler->FindSection(strSection, &sec);
            if (!ret)
            {
                return S_FALSE;
            }
            PROJ_MAN_ASSERT(sec);
            ret = sec->GetFirstKeyAndValue(&key);
            while (ret)
            {
                PROJ_MAN_ASSERT(key);
                if (key->GetKey().CompareNoCase(strKey)==0)
                {
                    listValues.AddTail(key->GetValue());
                }
                ret = sec->GetNextKeyAndValue(&key);
            }
            // fill SAFEARRAY
            if (listValues.GetCount()<=0)
            {
                return S_FALSE;
            }
            HRESULT hr;
            SAFEARRAYBOUND Bound;
            Bound.lLbound = 0;
            Bound.cElements = listValues.GetCount();

            *parrFileNames = ::SafeArrayCreate(VT_BSTR, 1, &Bound);
            if (NULL == (*parrFileNames)) 
            {
                return E_FAIL;    
            }

            long lIndex = 0;
            POSITION pos = listValues.GetHeadPosition();
            CString str;

            while(pos!=NULL)
            {
                str = listValues.GetNext(pos);
                CComBSTR sStr(str);
                hr = ::SafeArrayPutElement(*parrFileNames, &lIndex, 
                                          (void*)sStr);
                if (FAILED(hr))
                {
                    ::SafeArrayDestroy(*parrFileNames);
                    *parrFileNames = NULL;
                    return E_FAIL;
                }
                lIndex++;
            }
            
            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }    
}


//------------------------------------------------------------------*
// GetNodesOFTypeUnderNode
//
STDMETHODIMP CCEProjectManager::GetNodesOfTypeUnderNode(BSTR sStartNode, 
                                                        BSTR sType, 
                                                        SAFEARRAY** parrNodes)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (parrNodes == NULL)
    {
        return E_POINTER;
    }

    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pClassTree);
        PROJ_MAN_ASSERT(m_pTargetTree);
        try
        {
            CString strStartNode(sStartNode);
            CString strType(sType);
            if (m_pClassTree->IsViewPath(strStartNode))
            {
                if (!m_pClassTree->GetAllClassesOfTypeUnderNode(strStartNode, strType, parrNodes))
                {
                    return S_FALSE;
                }
                return S_OK;
            }
            else if (m_pTargetTree->IsViewPath(strStartNode))
            {
                if (!m_pTargetTree->GetAllClassesOfTypeUnderNode(strStartNode, strType, parrNodes))
                {
                    return S_FALSE;
                }
                return S_OK;
            }
            return S_FALSE;  
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }    

    return S_OK;

}


//------------------------------------------------------------------*
// CheckIECIdentifier
//
STDMETHODIMP CCEProjectManager::CheckIECIdentifier(BSTR sIdentifier, 
                                                   enum E_IEC_IDENTIFIER_ERROR* pErrorFlag)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pErrorFlag == NULL)
    {
        return E_POINTER;
    }

    try
    {
        CString strId(sIdentifier);
        HRESULT hr = UTIL_CheckIdentifier(strId);
        if  (hr == E_UTIL_ID_SYNTAX)
        {
            *pErrorFlag = IDENTIFIER_SYNTAX_ERROR;
            return S_FALSE;
        }
        else if (hr == E_UTIL_ID_IS_KEYWORD)
        {
            *pErrorFlag = IDENTIFIER_IS_KEYWORD;
            return S_FALSE;
        }
        else if (hr == E_UTIL_ID_TOO_LONG)
        {
            *pErrorFlag = IDENTIFIER_TOO_LONG;
            return S_FALSE;
        }
        else if (hr == S_OK)
        {
            *pErrorFlag = IDENTIFIER_OK;
            return S_OK;
        }
        else
        {
            return E_FAIL;
        }
    }
    catch (CMemoryException* e)
    {
        e->Delete();
        return E_OUTOFMEMORY;
    }

    return E_FAIL;
}



//------------------------------------------------------------------*
// getSupportedIECDataTypes
//
STDMETHODIMP CCEProjectManager::getSupportedIECDataTypes(SAFEARRAY** parrDataTypes)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (parrDataTypes == NULL)
    {
        return E_POINTER;
    }

    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pTargetTree);
        try
        {
            if (!m_pTargetTree->GetSupportedDataTypes(parrDataTypes))
            {
                return S_FALSE;
            }
            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }    
}



//------------------------------------------------------------------*
// getSupportedIECDataTypesForTarget
//

STDMETHODIMP CCEProjectManager::getSupportedIECDataTypesForTarget(BSTR sTargetType, SAFEARRAY** parrDataTypes)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (parrDataTypes == NULL)
    {
        return E_POINTER;
    }

    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pTargetTree);
        CString strTargetType(sTargetType);
        try
        {
            if (!m_pTargetTree->GetSupportedDataTypesForTarget(strTargetType, parrDataTypes))
            {
                return S_FALSE;
            }
            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }    
}



//------------------------------------------------------------------*
// getSupportedIECLanguages
//

STDMETHODIMP CCEProjectManager::getSupportedIECLanguages(SAFEARRAY** parrLanguages)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (parrLanguages == NULL)
    {
        return E_POINTER;
    }

    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pTargetTree);
        try
        {
            if (!m_pTargetTree->GetSupportedLanguages(parrLanguages))
            {
                return S_FALSE;
            }
            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }    
}



//------------------------------------------------------------------*
// getSupportedIECLanguagesForTarget
//

STDMETHODIMP CCEProjectManager::getSupportedIECLanguagesForTarget(BSTR sTargetType, SAFEARRAY** parrLanguages)
{    
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (parrLanguages == NULL)
    {
        return E_POINTER;
    }

    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pTargetTree);
        CString strTargetType(sTargetType);
        try
        {
            if (!m_pTargetTree->GetSupportedLanguagesForTarget(strTargetType, parrLanguages))
            {
                return S_FALSE;
            }
            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    } 
}



//------------------------------------------------------------------*
// getSupportedIECVarStorageClasses
//

STDMETHODIMP CCEProjectManager::getSupportRetain(BOOL* pbRetain)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pbRetain == NULL)
    {
        return E_POINTER;
    }

    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pTargetTree);
        try
        {
            *pbRetain = m_pTargetTree->GetSupportRetain();
            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    } 
}



//------------------------------------------------------------------*
// getSupportedIECVarStorageClassesForTarget
//

STDMETHODIMP CCEProjectManager::getSupportRetainForTarget(BSTR sTargetType, BOOL* pbRetain)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pbRetain == NULL)
    {
        return E_POINTER;
    }

    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pTargetTree);
        CString strTargetType(sTargetType);
        try
        {
            *pbRetain = m_pTargetTree->GetSupportRetainForTarget(strTargetType);
            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }
}



//------------------------------------------------------------------*
// ReplaceMacros
//

STDMETHODIMP CCEProjectManager::ReplaceMacros(BSTR sIn,BSTR* psOut)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
 
    try
    {
        CString strIn(sIn);
        CString strOut;

        strOut = CFileHelper::ReplaceMacros(strIn);

        *psOut = strOut.AllocSysString();
        return S_OK;
    }
    catch (CMemoryException* e)
    {
        e->Delete();
        return E_OUTOFMEMORY;
    }
}



//------------------------------------------------------------------*
// GetFileAbsolute
//

STDMETHODIMP CCEProjectManager::GetFileAbsolute(BSTR sFileName, BSTR* psAbsFileName)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
 
    if (m_bProjectLoaded)
    {
        try
        {
            CString strIn(sFileName);
            CString strOut;

            strOut = CFileHelper::GetFileAbsoluteReplaced(strIn);

            *psAbsFileName = strOut.AllocSysString();
            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }

    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }    
}


//------------------------------------------------------------------*
// GetFileRelative
//
STDMETHODIMP CCEProjectManager::GetFileRelative(BSTR sFileName, BSTR* psRelFileName)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
 
    if (m_bProjectLoaded)
    {
        try
        {
            CString strIn(sFileName);
            CString strOut;

            strOut = CFileHelper::StripProjectPath(strIn);
            strOut = CFileHelper::ReplaceStdLibPath(strOut);

            *psRelFileName = strOut.AllocSysString();
            return S_OK;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }

    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }    
}


//------------------------------------------------------------------*
// GetVersionOfProjectFile
//
STDMETHODIMP CCEProjectManager::GetVersionOfProjectFile(BSTR sPrjFileName, BSTR* psVersion)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    try
    {
        CProjectFile tempProjFile(this);

        CString strPrjFileName = sPrjFileName;

        if (tempProjFile.SetProjectFile(strPrjFileName)!=S_OK)
        {
            return E_FAIL;
        }

        tempProjFile.ReparseProjectFile();

        CString strSection = CE_CP_FORMAT;
        CString strKey = CE_CP_VERSION;
        CString strValue;

        CSection* pSection;

        if (!tempProjFile.FindSection(strSection, &pSection))
        {
            return E_FAIL;
        }

        CKeyAndValue* pKeyAndValue = NULL;

        pSection->GetFirstKeyAndValue(&pKeyAndValue);

        while (pKeyAndValue)
        {
            if (pKeyAndValue->GetKey().CompareNoCase(strKey)==0)
            {
                strValue = pKeyAndValue->GetValue();
                *psVersion = strValue.AllocSysString();
                return S_OK;
            }
        }
    }   
    catch (CMemoryException* e)
    {
        e->Delete();
        return E_OUTOFMEMORY;
    }

    return E_FAIL;
}


STDMETHODIMP CCEProjectManager::ShowContextHelp(DWORD dwContextId)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    Fire_ShowContextHelp(dwContextId);

    return S_OK;
}


STDMETHODIMP CCEProjectManager::ProjectFileChanged()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    Fire_ProjectFileChanged();

    return S_OK;
}

/////////////////////////////////////////////////////////////////////
// implementation of ICEContainer
//


                                                                
//------------------------------------------------------------------*
// createLink
//
STDMETHODIMP CCEProjectManager::createLink(BSTR sFileName, BSTR sLocation, BSTR sLinkName)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pClassTree);
        PROJ_MAN_ASSERT(m_pTargetTree);
        try
        {
            // get file name from full path, if full path is given
            CString strFileName = sFileName;
            strFileName = CFileHelper::StripProjectPath(strFileName);

            ASSERT((m_strProjectFileName.CompareNoCase(strFileName)==0)
                || (strFileName.IsEmpty()) );

            if (m_strProjectFileName.CompareNoCase(strFileName)!=0)
            {
                if (strFileName.IsEmpty())
                {
                    strFileName = m_strProjectName;
                }
                else
                {
                    return E_FAIL;
                }
            }

            CString strLinkName = CFileHelper::StripProjectPath((CString)sLinkName);
            strLinkName = CFileHelper::ReplaceStdLibPath(strLinkName);

            CString strLocation = sLocation;
            // for addon actions selected from the main menu: create program...
            // sLocation is empty, so we do not know where to add the file
            // so try add file like moving in file view.
            if (strLocation.IsEmpty())
            {
                return AddFile((CComBSTR)strLinkName, FV_FILE_SOURCE, FALSE);
            }

            if (m_pClassTree->IsViewPath(strLocation))
            {
                if (!m_pClassTree->CreateLink(strLocation, strLinkName))
                {
                    return S_FALSE;
                }
                else
                {
                    return S_OK;
                }
            }
            else if (m_pTargetTree->IsViewPath(strLocation))
            {
                if (!m_pTargetTree->CreateLink(strLocation, strLinkName))
                {
                    return S_FALSE;
                }
                else
                {
                    return S_OK;
                }
            }
               
        	return S_FALSE;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }    
}


//------------------------------------------------------------------*
// renameLink
//
STDMETHODIMP CCEProjectManager::renameLink(BSTR sFileName, BSTR sLocation, BSTR sOldLinkName, BSTR sNewLinkName)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
	 
    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pClassTree);
        PROJ_MAN_ASSERT(m_pTargetTree);
        try
        {
            CString strFileName = sFileName;
            strFileName = CFileHelper::StripProjectPath(strFileName);

            ASSERT(m_strProjectFileName.CompareNoCase(strFileName)==0);
            
            if (m_strProjectFileName.CompareNoCase(strFileName)!=0)
            {
                return S_FALSE;
            }

            CString strOldLinkName = CFileHelper::StripProjectPath(sOldLinkName);
            strOldLinkName = CFileHelper::ReplaceStdLibPath(strOldLinkName);
            CString strNewLinkName = CFileHelper::StripProjectPath(sNewLinkName);
            strNewLinkName = CFileHelper::ReplaceStdLibPath(strNewLinkName);

            if (m_pClassTree->IsViewPath((CString)sLocation))
            {
                if (!m_pClassTree->RenameLink((CString)sLocation, 
                                              strOldLinkName,
                                              strNewLinkName))
                {
                    return S_FALSE;
                }
                else
                {
                    return S_OK;
                }
            }
            else if (m_pTargetTree->IsViewPath((CString)sLocation))
            {
                if (!m_pTargetTree->RenameLink((CString)sLocation, 
                                               strOldLinkName,
                                               strNewLinkName))
                {
                    return S_FALSE;
                }
                else
                {
                    return S_OK;
                }
            }
               
        	return S_FALSE;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }    
}


//------------------------------------------------------------------*
// deleteLink
//
STDMETHODIMP CCEProjectManager::deleteLink(BSTR sFileName, BSTR sLocation, BSTR sLinkName)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
	 
    if (m_bProjectLoaded)
    {
        PROJ_MAN_ASSERT(m_pClassTree);
        PROJ_MAN_ASSERT(m_pTargetTree);
        try
        {
            CString strFileName = sFileName;
            strFileName = CFileHelper::StripProjectPath(strFileName);

            ASSERT(m_strProjectFileName.CompareNoCase(strFileName)==0);

            if (m_strProjectFileName.CompareNoCase(strFileName)!=0)
            {
                return S_FALSE;
            }
        
            CString strLinkName = CFileHelper::StripProjectPath((CString)sLinkName);
            strLinkName = CFileHelper::ReplaceStdLibPath(strLinkName);

            if (m_pClassTree->IsViewPath((CString)sLocation))
            {
                if (!m_pClassTree->DeleteLink((CString)sLocation, strLinkName))
                {
                    return S_FALSE;
                }
                else
                {
                    return S_OK;
                }
            }
            else if (m_pTargetTree->IsViewPath((CString)sLocation))
            {
                if (!m_pTargetTree->DeleteLink((CString)sLocation, strLinkName))
                {
                    return S_FALSE;
                }
                else
                {
                    return S_OK;
                }
            }
               
        	return S_FALSE;
        }
        catch (CMemoryException* e)
        {
            e->Delete();
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                  E_CEPROJMAN_NO_PROJECT_LOADED, 
                  (CComBSTR)BuildErrorMsg(E_CEPROJMAN_NO_PROJECT_LOADED));
        return E_CEPROJMAN_NO_PROJECT_LOADED;      
    }    
}



STDMETHODIMP CCEProjectManager::setProjectInfo(IUnknown* pICEProjectInfo)
{
    return S_OK;
}





/////////////////////////////////////////////////////////////////////
// implementation of _ICEMessageEvent
//


                                                                
//------------------------------------------------------------------*
// OnNewMessage
//
STDMETHODIMP CCEProjectManager::OnNewMessage(BSTR sMachine, USHORT wScrFacility, HRESULT hr, BSTR sDescr)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    return Fire_OnNewMessage(sMachine, wScrFacility, hr, sDescr);
}







/////////////////////////////////////////////////////////////////////
// Other functions
//


//------------------------------------------------------------------*
/**
 * InitKadManager
 * TODO: This method creates its own kad manager object. This object
 *       is hold for as long as a project is opened in the project
 *       manager. If the KADManager will be get from the MainFrame
 *       this method is no longer necessary.
 *
 * @return          TRUE - if KadManager could be initialized
 *                  FALSE- KadManager could not be initialized
 * @exception       -
 * @see             
*/
BOOL CCEProjectManager::InitKadManager()
{
    HRESULT hr; 

    CGuid kadGuid(CLSID_KadManagerComObj);
    CComBSTR psGuid;
    hr = kadGuid.ToBSTR(&psGuid);
    PROJ_MAN_ASSERT(hr==S_OK);
    if (!SUCCEEDED(hr))
    {
        return FALSE;
    }
    hr = Fire_OnComponentByCLSID(psGuid, &m_pKadManager);
    if (!SUCCEEDED(hr) || (!m_pKadManager))
    {
        return FALSE;
    }

    return TRUE;
}



//------------------------------------------------------------------*
/**
 * SplitNames splits multiple strings parted with ; in one string and
 * fills a string list with the strings (this function is used to split 
 * fileextention lists in the kad like "gif;bmp;jpg").
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CCEProjectManager::SplitNames(const CString& strNames, CList<CString, CString&> &nameList)
{
    CString left, rest;
    int pos = -1;

    rest = strNames;

    pos = rest.Find(_T(";"));

    while (pos!=-1)
    {
        left = rest.Left(pos);
        rest = rest.Right(rest.GetLength()-pos-1);
        nameList.AddTail(left);
        pos = rest.Find(_T(";"));
    }
    if (rest.GetLength()>0)
    {
        nameList.AddTail(rest);
    }

}


//------------------------------------------------------------------*
/**
 * InitBackparser gets all infos for file extentions and backparser 
 * from the kad.
 *
 * @return          TRUE: on success
 *                  FALSE: some info could not be accessed
 * @exception       -
 * @see             
*/
BOOL CCEProjectManager::InitBackparser()
{
    HRESULT hr;

    if (!m_listBackparser.IsEmpty()
        ||!m_mapBackparser.IsEmpty())
    {
        return FALSE;
    }

    PROJ_MAN_ASSERT(m_pKadManager);

    CComQIPtr<IFiles, &IID_IFiles> kadFiles;
    kadFiles = m_pKadManager;
    if (!kadFiles)
    {
        return FALSE;
    }

    CComPtr<IEnumFiles> kadEnumFiles;
    hr = kadFiles->GetFileEnumerator(&kadEnumFiles);
    if (!SUCCEEDED(hr) || !kadEnumFiles)
    {
        return FALSE;
    }

    PROJ_MAN_ASSERT(kadEnumFiles);
    kadEnumFiles->Reset();
    ULONG n;
    CComPtr<IFile> kadFile;
    hr = kadEnumFiles->Next(1, &kadFile, &n);
    while (SUCCEEDED(hr) && kadFile)
    {
        PROJ_MAN_ASSERT(kadFile);
        GUID parserGUID;
        HRESULT parserHR = kadFile->GetParser(&parserGUID);
        if (parserHR==S_OK)
        {
            CComBSTR fileExt;
            HRESULT fileExtHR = kadFile->GetExtension(&fileExt);
            if (SUCCEEDED(fileExtHR))
            {
                TBackparserInfo* pBPInfo = NULL;
                TBackparserInfo *pFoundBPInfo = NULL;

                // look if GUID is already in the list of backparser
                POSITION pos = m_listBackparser.GetHeadPosition();
                while (pos!=NULL)
                {
                    pBPInfo = m_listBackparser.GetNext(pos);
                    if (parserGUID==pBPInfo->guid)
                    {
                        pFoundBPInfo = pBPInfo;
                        break;
                    }
                }

                // if not found
                if (!pFoundBPInfo) // create new backparser info
                {
                    pFoundBPInfo = new TBackparserInfo();
                    PROJ_MAN_ASSERT(pFoundBPInfo);
                    pFoundBPInfo->error = NO_BPERR;
                    pFoundBPInfo->guid = parserGUID;
                    pFoundBPInfo->pBackparser = NULL;
                    pFoundBPInfo->m_dwMessageAdviseCookie = NULL;
                    m_listBackparser.AddTail(pFoundBPInfo);
                }

                CString strFileExt(fileExt);
                strFileExt.MakeLower();
                // split file extentions info : "jpg;bmp;gif"
                CList<CString, CString&> listExt;
                SplitNames(strFileExt, listExt);

                pos = listExt.GetHeadPosition();
                while (pos)
                {
                    CString enumExt;
                    enumExt = listExt.GetNext(pos);
                    pFoundBPInfo->strlFileExt.AddTail(enumExt);
                    // add file extension -> backparser info to map
                    m_mapBackparser.SetAt(enumExt, pFoundBPInfo);
                }
            }
        }
        kadFile.Release();
        hr = kadEnumFiles->Next(1, &kadFile, &n);
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * ClearBackparser clears all backparser info and release backparser.
 *
 * @return          TRUE: everything could be cleaned up
 *                  FALSE: error during cleaning up
 * @exception       -
 * @see             
*/
BOOL CCEProjectManager::ClearBackparser()
{
    // clear map
    m_mapBackparser.RemoveAll();

    // clear backparser list: for all backparser objects (!=NULL) call release
    while (!m_listBackparser.IsEmpty())
    {
        TBackparserInfo* pBPInfo = m_listBackparser.GetHead();       
        if (pBPInfo->m_dwMessageAdviseCookie != NULL)
        {
            AtlUnadvise(pBPInfo->pBackparser, 
                        IID__ICEMessageEvent, 
                        pBPInfo->m_dwMessageAdviseCookie);
            pBPInfo->m_dwMessageAdviseCookie = NULL;
        }
        if (pBPInfo->pBackparser != NULL)
        {
            (pBPInfo->pBackparser)->Release();
        }
        delete pBPInfo; 
        m_listBackparser.RemoveHead();
    }
    return TRUE;
}



//------------------------------------------------------------------*
/**
 * GetBackparserForFileExt returns an interface pointer to a
 * backparser object if the backparser object for the file
 * extesion could be created. Backparser creation is optimized here
 * if a backparser has been successfully created one time, the project
 * manager holds an pointer to the object and returns the same object 
 * the next time. Before returning the interface pointer an AddRef is
 * done. The consumer of the pointer has to do a release.
 *
 * @param           strFileExt: The file extension for which the 
 *                              backparser should be looked up
 * @return          pointer to the backparser
 *                  NULL - if backparser could not be found
 * @exception       -
 * @see             
*/
ICEBackparse* CCEProjectManager::GetBackparserForFileExt(CString strFileExt)
{
    HRESULT hr;
    ICEBackparse* pBP;
    TBackparserInfo* pBPInfo = NULL;

    strFileExt.MakeLower();
    m_mapBackparser.Lookup(strFileExt, pBPInfo);
    if (!pBPInfo)
    {
        return NULL;
    }

    if (pBPInfo->error!=NO_BPERR)
    {
        return NULL;
    }

    if (pBPInfo->pBackparser == NULL)
    {
        hr = CoCreateInstance(pBPInfo->guid,
		                      NULL,
		                      CLSCTX_SERVER,
		                      __uuidof(ICEBackparse), //IID_ICEBackparse,
						      (LPVOID *)&pBP);
        if (!SUCCEEDED(hr))
        {
            Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                      E_CEPROJMAN_BACKPARSER_CREATE, 
                      (CComBSTR)BuildErrorMsg(E_CEPROJMAN_BACKPARSER_CREATE, 
                                              strFileExt));
            pBPInfo->error = BPERR_CREATE;
            return NULL;
        }

        // try to advise for _ICEMessageEvent interface to foreward messages from backparser
        IUnknown* pUnk = this->GetUnknown();
        hr = AtlAdvise (pBP, pUnk, 
                        IID__ICEMessageEvent, &(pBPInfo->m_dwMessageAdviseCookie));
        if (!SUCCEEDED(hr))
        {
            pBPInfo->m_dwMessageAdviseCookie = NULL;
        }

        pBPInfo->pBackparser = pBP;
    }
    else
    {
        pBP = pBPInfo->pBackparser;
    }

    pBP->AddRef(); // do addref for the returned pointer, so client can do a release
    return pBP;
}




//------------------------------------------------------------------*
/**
 * InitContainer gets all infos for file extentions and container 
 * from the kad.
 *
 * @return          TRUE: on success
 *                  FALSE: some info could not be accessed
 * @exception       -
 * @see             
*/
BOOL CCEProjectManager::InitContainer()
{
    HRESULT hr;

    if (!m_listContainer.IsEmpty()
        ||!m_mapContainer.IsEmpty())
    {
        return FALSE;
    }

    PROJ_MAN_ASSERT(m_pKadManager);

    CComQIPtr<IFiles, &IID_IFiles> kadFiles;
    kadFiles = m_pKadManager;
    if (!kadFiles)
    {
        return FALSE;
    }

    CComPtr<IEnumFiles> kadEnumFiles;
    hr = kadFiles->GetFileEnumerator(&kadEnumFiles);
    if (!SUCCEEDED(hr) || !kadEnumFiles)
    {
        return FALSE;
    }

    kadEnumFiles->Reset();
    ULONG n;
    CComPtr<IFile> kadFile;
    hr = kadEnumFiles->Next(1, &kadFile, &n);
    while (SUCCEEDED(hr) && kadFile)
    {
        GUID contGUID;
        HRESULT contHR = kadFile->GetContainer(&contGUID);
        if (contHR==S_OK)
        {
            CComBSTR fileExt;
            HRESULT fileExtHR = kadFile->GetExtension(&fileExt);
            if (SUCCEEDED(fileExtHR))
            {
                TContainerInfo* pContInfo = NULL;
                TContainerInfo *pFoundContInfo = NULL;

                // look if GUID is already in the list of backparser
                POSITION pos = m_listContainer.GetHeadPosition();
                while (pos!=NULL)
                {
                    pContInfo = m_listContainer.GetNext(pos);
                    if (contGUID==pContInfo->guid)
                    {
                        pFoundContInfo = pContInfo;
                        break;
                    }
                }

                // if not found
                if (!pFoundContInfo) // create new container info
                {
                    pFoundContInfo = new TContainerInfo();
                    PROJ_MAN_ASSERT(pFoundContInfo);
                    pFoundContInfo->error = NO_BPERR;
                    pFoundContInfo->guid = contGUID;
                    pFoundContInfo->pContainer = NULL;
                    pFoundContInfo->m_dwMessageAdviseCookie = NULL;
                    m_listContainer.AddTail(pFoundContInfo);
                }


                CString strFileExt(fileExt);
                strFileExt.MakeLower();
                // split file extentions info : "jpg;bmp;gif"
                CList<CString, CString&> listExt;
                SplitNames(strFileExt, listExt);

                pos = listExt.GetHeadPosition();
                while (pos)
                {
                    CString enumExt;
                    enumExt = listExt.GetNext(pos);
                    pFoundContInfo->strlFileExt.AddTail(enumExt);
                    // add file extension -> backparser info to map
                    m_mapContainer.SetAt(enumExt, pFoundContInfo);
                }
            }
        }
        kadFile.Release();
        hr = kadEnumFiles->Next(1, &kadFile, &n);
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * ClearContainer clears all container info and release backparser.
 *
 * @return          TRUE: everything could be cleaned up
 *                  FALSE: error during cleaning up
 * @exception       -
 * @see             
*/
BOOL CCEProjectManager::ClearContainer()
{
    // clear map
    m_mapContainer.RemoveAll();

    // clear container list: for all container objects (!=NULL) call release
    while (!m_listContainer.IsEmpty())
    {
        TContainerInfo* pContInfo = m_listContainer.GetHead();
        if (pContInfo->m_dwMessageAdviseCookie != NULL)
        {
            AtlUnadvise(pContInfo->pContainer, 
                        IID__ICEMessageEvent, 
                        pContInfo->m_dwMessageAdviseCookie);
            pContInfo->m_dwMessageAdviseCookie = NULL;
        }
        if (pContInfo->pContainer != NULL)
        {
            (pContInfo->pContainer)->Release();
        }

        delete pContInfo; 
        m_listContainer.RemoveHead();
    }
    return TRUE;
}



//------------------------------------------------------------------*
/**
 * GetContainerForFileExt returns an interface pointer to a
 * container object if the container object for the file
 * extension could be created. Container creation is optimized here
 * if a container has been successfully created one time, the project
 * manager holds an pointer to the object and returns the same object 
 * the next time. Before returning the interface pointer an AddRef is
 * done. The consumer of the pointer has to do a release.
 *
 * @param           strFileExt: The file extension for which the 
 *                              container should be looked up
 * @return          pointer to the container
 *                  NULL - if container could not be found
 * @exception       -
 * @see             
*/
ICEContainer* CCEProjectManager::GetContainerForFileExt(CString strFileExt)
{
    HRESULT hr;
    ICEContainer* pCont;
    TContainerInfo* pContInfo = NULL;

    strFileExt.MakeLower();
    if (strFileExt.CompareNoCase(CE_PROJ_FILE_EXT)==0) // container interface for project
    {                                            // project file
        IUnknown* pt = this->GetUnknown();
        ICEContainer* pc = NULL;
        hr = pt->QueryInterface(__uuidof(ICEContainer), (LPVOID*)&pc);
        if (SUCCEEDED(hr))
        {
            return pc;
        }
        else
        {
            return NULL;
        }
    }

    m_mapContainer.Lookup(strFileExt, pContInfo);
    if (!pContInfo)
    {
        return NULL;
    }

    if (pContInfo->error!=NO_BPERR)
    {
        return NULL;
    }

    if (pContInfo->pContainer == NULL)
    {
        hr = CoCreateInstance(pContInfo->guid,
		                      NULL,
		                      CLSCTX_SERVER,
		                      __uuidof(ICEContainer), //IID_ICEContainer,
						      (LPVOID *)&pCont);
        if (!SUCCEEDED(hr))
        {
            
            Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                      E_CEPROJMAN_CONTAINER_CREATE, 
                      (CComBSTR)BuildErrorMsg(E_CEPROJMAN_CONTAINER_CREATE, 
                                              strFileExt));
            pContInfo->error = BPERR_CREATE;
            return NULL;
        }

        // try to advise for _ICEMessageEvent interface to foreward messages from container
        IUnknown* pUnk = this->GetUnknown();
        hr = AtlAdvise (pCont, pUnk, 
                        IID__ICEMessageEvent, &(pContInfo->m_dwMessageAdviseCookie));
        if (!SUCCEEDED(hr))
        {
            pContInfo->m_dwMessageAdviseCookie = NULL;
        }

        pCont->setProjectInfo(pUnk);

        pContInfo->pContainer = pCont;
    }
    else
    {
        pCont = pContInfo->pContainer;
    }

    pCont->AddRef(); // do addref for the returned pointer, so client can do a release
    return pCont;
}





//------------------------------------------------------------------*
/**
 * OutputErrorMessage fires a message into msg view
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CCEProjectManager::OutputErrorMessage(HRESULT hError,
                                           LPCTSTR pszArg1 /*= NULL*/, 
                                           LPCTSTR pszArg2 /*= NULL*/,
                                           LPCTSTR pszArg3 /*= NULL*/, 
                                           LPCTSTR pszArg4 /*= NULL*/)
{
    Fire_OnNewMessage(m_sTargetId, CEPROJMAN_FACILITY, 
                      hError, 
                      (CComBSTR)BuildErrorMsg(hError, 
                                              pszArg1,
                                              pszArg2,
                                              pszArg3,
                                              pszArg4));
}




//------------------------------------------------------------------*
/**
 * BuildErrorMsg
 *
 * @param           hError: the error num
 * @param           pszArg1..4: optional parameter for the message
 * @return          the formatted message
 * @exception       -
 * @see             
*/
CString CCEProjectManager::BuildErrorMsg(HRESULT hError, 
                                         LPCTSTR pszArg1 /*= NULL*/, 
                                         LPCTSTR pszArg2 /*= NULL*/,
                                         LPCTSTR pszArg3 /*= NULL*/, 
                                         LPCTSTR pszArg4 /*= NULL*/)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CString strMsg;
    LPTSTR  pBuf = NULL;    
    DWORD   ret;
    LPCTSTR argArray[5];

    // for all errors and warnings we will insert error code number in %1
    // %2 and ff get pszArg1 etc.
    // for informational messages, we insert first string pszArg1 in %1 
    if (FAILED(hError))
    {
        // is a error or warning message:
        argArray[0] = (LPCTSTR) HRESULT_CODE(hError);     // isolate pure error code
        argArray[1] = pszArg1;
        argArray[2] = pszArg2;
        argArray[3] = pszArg3;
        argArray[4] = pszArg4;
    }
    else
    {
        // is a informational message
        argArray[0] = pszArg1;
        argArray[1] = pszArg2;
        argArray[2] = pszArg3;
        argArray[3] = pszArg4;
        argArray[4] = NULL;
    }

    // TODO: language selection
    WORD    wLangID = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);
    //    WORD    wLangID = MAKELANGID(LANG_GERMAN, SUBLANG_GERMAN);
    //    WORD    wLangID = MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT);

    // try to lookup as local-facility error
    ret = ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|
                          FORMAT_MESSAGE_FROM_HMODULE|FORMAT_MESSAGE_FROM_SYSTEM|
                          FORMAT_MESSAGE_ARGUMENT_ARRAY, 
                          (HMODULE)m_hInstance, //(HMODULE)(AfxGetApp()->m_hInstance), 
                          hError,
                          wLangID,
                          (LPTSTR) &pBuf, 
                          0, 
                          (va_list*) argArray);


    if (ret == 0)
    {
        DWORD error = GetLastError();

        /*
        // no local-facility error, try lookup as system error:
        ret = FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |	
				FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				hError,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT),   // wlangID,
				(LPTSTR) &pBuf,
				0,
				NULL);
        if (ret != 0)
        {
            // prepend system error with this text
            strMsg = _T("fatal system error: ");
        }
        else
        {
            // have no string resource for error, complain as internal error!!
            PROJ_MAN_ASSERT(ret>0 && pBuf);
            if (pBuf == 0 || ret == 0)
            {*/
                pBuf = (LPTSTR) ::LocalLock(::LocalAlloc(NONZEROLPTR, MAX_PATH));
                CString strError;
                strError.LoadString(IDS_INTERNAL_ERROR);
                //wsprintf(pBuf, _T("CEProjMan internal error: no resource for error code 0x%08X\n"), hError);
                wsprintf(pBuf, strError, hError);

            /*}
        }*/

    }
    
    strMsg += pBuf;
    ::LocalFree(pBuf);

    return strMsg;
}

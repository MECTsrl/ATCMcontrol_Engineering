/* $Header: /4CReleased/V2.20.00/TargetBase/PROJECTWIZARDBASE/ProjectWizardData.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: ProjectWizardData.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/PROJECTWIZARDBASE/ProjectWizardData.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase
 *
 * =COMPONENT			ProjectWizardBase
 *
 * =CURRENT 	 $Date: 28.02.07 19:06 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "ProjectWizardData.h"
#include "ProjectWizardRes.h"
#include "fc_tools\fc_fileio.h"
#include "utilif.h"
#include "GHFile.h"


CProjectWizardData::CProjectWizardData()
: m_subSystems(SUBSYSTEM_PROFIBUS)
{
}

CProjectWizardData::~CProjectWizardData()
{
    ResetResources();
}


//------------------------------------------------------------------*
/**
 * Initializes the project data
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT CProjectWizardData::Init(CString strPath, CString strProjectName)
{
    HRESULT hr;
    int iLength;

    m_strPath = strPath;

    iLength = m_strPath.GetLength();
    if(iLength > 0)
    {
        if(m_strPath.GetAt(iLength - 1) != '\\')
            m_strPath += _T('\\');

        m_strPath += strProjectName;
    }

    m_strProjectName = strProjectName;

    hr = SearchLibraries();
    if(FAILED(hr))
        return(hr);

    UseDefaultLibraries();

    return(S_OK);
}


void CProjectWizardData::UseDefaultLibraries()
{
	int i;

    for(i = 0; i < GetLibraryCount(); ++i)
        UseLibrary(i, (GetLibrary(i)->GetSubSystemDefault() & m_subSystems) != 0);
}


void CProjectWizardData::UseAllLibraries()
{
    int i;

    for(i = 0; i < GetLibraryCount(); ++i)
        UseLibrary(i, true);
}


void CProjectWizardData::UseNoLibraries()
{
    int i;

    for(i = 0; i < GetLibraryCount(); ++i)
        UseLibrary(i, false);
}


void CProjectWizardData::UseLibrary(int i, bool bUse)
{
    GetLibrary(i)->SetSelectFlag(bUse);
}


//------------------------------------------------------------------*
/**
 * Returns the path where the project is to be created
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
CString CProjectWizardData::GetPath()
{
    return(m_strPath);
}


//------------------------------------------------------------------*
/**
 * Returns the number of resources
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
int CProjectWizardData::GetResourceCount()
{
    return(m_Resources.GetSize());
}


//------------------------------------------------------------------*
/**
 * Returns resource at given index
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
CProjectWizardResource *CProjectWizardData::GetResource(int i)
{
    CProjectWizardResource* pResource = m_Resources[i];
    return(pResource);
}

//------------------------------------------------------------------*
/**
 * set resource.
 *
 *  old resource will be deleted.
 *
 * @param           i: index of resource
 * @param           pResource: resource to be set.
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CProjectWizardData::SetResource(int i, CProjectWizardResource* pResource)
{
    if(i >= m_Resources.GetSize())
    {
        return FALSE;
    }

    CProjectWizardResource* pResourceTmp = m_Resources[i];
    if(pResourceTmp)
    {
        delete pResourceTmp;
    }

    pResourceTmp = pResource;
    
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * Sets number of resources in project
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CProjectWizardData::SetResourceCount(int iCount)
{
    ResetResources();
    m_Resources.RemoveAll();
    m_Resources.SetSize(iCount);
    InitResources();
    CreateNewResources();
}

//------------------------------------------------------------------*
/**
 * reset resources.
 *
 *  free memory and set all resource pointers to NULL
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CProjectWizardData::ResetResources()
{
    int iNumRes = m_Resources.GetSize();
    for(int iRes = 0; iRes < iNumRes; ++iRes)
    {
        if(m_Resources[iRes])
        {
            delete m_Resources[iRes];
            m_Resources[iRes] = NULL;
        }
    }
}

//------------------------------------------------------------------*
/**
 * init resources.
 *
 *  set all resource pointers to NULL.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CProjectWizardData::InitResources()
{
    int iNumRes = m_Resources.GetSize();
    for(int iRes = 0; iRes < iNumRes; ++iRes)
    {
        m_Resources[iRes] = NULL;
    }
}


//------------------------------------------------------------------*
/**
 * create new resources.
 *
 * @param           -
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CProjectWizardData::CreateNewResources()
{
    int iNumRes = m_Resources.GetSize();
    CProjectWizardResource* pResource;
    for(int iRes = 0; iRes < iNumRes; ++iRes)
    {
        pResource = CreateResource();
        ASSERT(pResource);
        if(!pResource)
        {
            return FALSE;
        }
        m_Resources[iRes] = pResource;
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * create resource.
 *
 *  overwrite this function to create your own resource class derived
 *  from CProjectWizardResource.
 *
 * @param           -
 * @return          newly created project wizard resource
 * @exception       -
 * @see             -
*/
CProjectWizardResource* CProjectWizardData::CreateResource()
{
    return new CProjectWizardResource;
}



//------------------------------------------------------------------*
/**
 * Returns the number of libraries
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
int CProjectWizardData::GetLibraryCount()
{
    return(m_Libraries.GetSize());
}


//------------------------------------------------------------------*
/**
 * Returns library at given index
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
CProjectWizardLibrary *CProjectWizardData::GetLibrary(int i)
{
    return(&m_Libraries[i]);
}


//------------------------------------------------------------------*
/**
 * Sets the chosen IO sub-systems
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CProjectWizardData::SetSubSystems(TSubSystems subSystems)
{
	m_subSystems = subSystems;
}


//------------------------------------------------------------------*
/**
 * Returns the chosen IO sub-systems
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
TSubSystems CProjectWizardData::GetSubSystems()
{
	return(m_subSystems);
}


//------------------------------------------------------------------*
/**
 * Searches for all available libraries in the library path
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT CProjectWizardData::SearchLibraries()
{
    BOOL done;
    int iCount,iNumberLibraries;
    HRESULT hrResult;
    HANDLE hSearch;
    WIN32_FIND_DATA findData;
    CString sSearch;
    CStringArray asLibraries;

    iNumberLibraries = 0;
    asLibraries.SetSize(0);
    m_Libraries.RemoveAll();
	hrResult = UTIL_GetStdLibPath(m_strLibraryPath.GetBuffer(_MAX_PATH), _MAX_PATH);
    m_strLibraryPath.ReleaseBuffer();
    if(SUCCEEDED(hrResult)) {
        if(m_strLibraryPath.IsEmpty()) {
            hrResult = S_FALSE;
        } else {
            sSearch.Format(_T("%s\\*.") FILE_EXT_LIBRARY,m_strLibraryPath);
		    hSearch = FindFirstFile(sSearch,&findData);
            if(hSearch == INVALID_HANDLE_VALUE) {
                hrResult = S_FALSE;
            } else {
                hrResult = S_OK;
                done = FALSE;
                while(! done) {
                    asLibraries.SetAtGrow(iNumberLibraries,CString(findData.cFileName));
                    iNumberLibraries++;
                    if(! done) {
                        done = ! FindNextFile(hSearch,&findData);
                        if(done) {
                            if(GetLastError() != ERROR_NO_MORE_FILES) {
                                hrResult = E_FAIL;
                            }
                        }
                    }
                }
                FindClose(hSearch);
                if(hrResult == S_OK) {
                    m_Libraries.SetSize(iNumberLibraries);
                    for(iCount=0; iCount<iNumberLibraries; iCount++) {
                        m_Libraries[iCount].SetLibraryName(asLibraries[iCount]);
                        m_Libraries[iCount].SetSelectFlag(FALSE);
                    }
                }
            }
        }
    }
    return hrResult;
}


//------------------------------------------------------------------*
/**
 * Sets the project properties
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CProjectWizardData::SetProperties(CString strAlias, CString strUserText1, CString strUserText2, CString strUserText3, CString strURL)
{
    m_strAlias = strAlias;
    m_strUserText1 = strUserText1;
    m_strUserText2 = strUserText2;
    m_strUserText3 = strUserText3;
    m_strURL = strURL;
}

//------------------------------------------------------------------*
/**
 * Adds the system comments
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT CProjectWizardData::AddSystemComment(FILE *pFile)
{

    fprintf(pFile,"(*[[\r\n");
    fprintf(pFile,"]]*)\r\n");
    return(S_OK);
}


//------------------------------------------------------------------*
/**
 * Creates the project directory if it does not yet exist
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT CProjectWizardData::CreateProjectDirectory()
{
    HRESULT hrResult;
    
    hrResult = S_OK;
    if(! FC_ExistSubDir(m_strPath)) {
        FC_CreateSubDir(m_strPath);
        if(! FC_ExistSubDir(m_strPath)) {
            hrResult = ERROR_CANNOT_MAKE;
        }
    }
    return hrResult;
}


//------------------------------------------------------------------*
/**
 * Creates a file in the project directory
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT CProjectWizardData::CreateFile(CString strFilename, FILE **ppFile)
{
    FILE *pFile;
    CString strFullFilename;

    strFullFilename.Format(_T("%s\\%s"), m_strPath, strFilename);

    pFile = fopen(strFullFilename, "rb");
    if(pFile != NULL)
    {
        CString strMessage;

        strMessage.Format(IDS_ERR_FILE_EXISTS, strFilename);
        AfxMessageBox(strMessage);

        fclose(pFile);

        return(S_FALSE);
    }

    *ppFile = fopen(strFullFilename, "wb");
    if(*ppFile == NULL)
        return(ERROR_CANNOT_MAKE);

    return(S_OK);
}

//------------------------------------------------------------------*
/**
 * Creates the latcomm file
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT CProjectWizardData::CreateLatcommFile()
{                           
    HRESULT hrResult;
    FILE *pLatcommFile;
    CString sFilename;


    // Create project file.
    //---------------------

    sFilename.Format(_T("%s." FILE_EXT_LATCOMM), m_strProjectName);

    hrResult = CreateFile(sFilename, &pLatcommFile);
    if(hrResult != S_OK)
        return(hrResult);
	
	fprintf(pLatcommFile,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
    fprintf(pLatcommFile,"<LATCOMM_4C>\r\n");
	fprintf(pLatcommFile,"</LATCOMM_4C>\r\n");

	fclose(pLatcommFile);

    return(S_OK);

}

//------------------------------------------------------------------*
/**
 * Creates the project file
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT CProjectWizardData::CreateProjectFile()
{
    int i;
    HRESULT hrResult;
    FILE *pProjectFile;
    CString sFilename;
    CProjectWizardResource* pResource;


    // Create project file.
    //---------------------

    sFilename.Format(_T("%s." FILE_EXT_PROJECT), m_strProjectName);

    hrResult = CreateFile(sFilename, &pProjectFile);
    if(hrResult != S_OK)
        return(hrResult);

    hrResult = CreateGuid(&m_strProjectGuid);


    // Write [Attributes] section(Alias, UserText1, UserText2, UserText3, UserURL)
    //-----------------------------------------------------------------------------------

    fprintf(pProjectFile, "[Attributes]\r\n");
    fprintf (pProjectFile, "GUID=\"%s\"\r\n", m_strProjectGuid);
    if(!m_strAlias.IsEmpty())
    {
        fprintf(pProjectFile, "Alias=%s\r\n", m_strAlias);
    }
    if(!m_strUserText1.IsEmpty())
    {
        fprintf(pProjectFile, "UserText1=%s\r\n", m_strUserText1);
    }
    if(!m_strUserText2.IsEmpty())
    {
        fprintf(pProjectFile, "UserText2=%s\r\n", m_strUserText2);
    }
    if(!m_strUserText3.IsEmpty())
    {
        fprintf(pProjectFile, "UserText3=%s\r\n", m_strUserText3);
    }
    if(!m_strURL.IsEmpty())
    {
        fprintf(pProjectFile, "UserURL=%s\r\n", m_strURL);
    }
    fprintf(pProjectFile, "\r\n");


    // Write [Format] section (version)
    //---------------------------------

    fprintf(pProjectFile, "[Format]\r\n");
    fprintf(pProjectFile, "version=V2.0\r\n\r\n");


    // Write [Classes.GVLs] section (.gvl file entries)
    //-------------------------------------------------

    fprintf(pProjectFile, "[Classes.GVLs]\r\n");

    for(i = 0; i < GetResourceCount(); ++i)
    {
        pResource = GetResource(i);
        ASSERT(pResource);
        if(pResource)
        {
            fprintf(pProjectFile, "file=%s." FILE_EXT_GLOBAL_VARIABLES "\r\n",
                 pResource->GetResourceName());
        }
    }

    fprintf(pProjectFile,"\r\n");


    // Write [Classes.Configurations] section (configuration .con file entries)
    //-----------------------------------------------------------

    fprintf(pProjectFile, "[Classes.Configurations]\r\n");
    fprintf(pProjectFile, "file=%s.con\r\n", CONFIGURATION_FILE_NAME);
    fprintf(pProjectFile, "\r\n");


    // Write [Classes.Constants] section (constant files)
    //---------------------------------------------------

    fprintf(pProjectFile, "[Classes.Constants]\r\n");

    for(i = 0; i < GetResourceCount(); ++i)
    {
        pResource = GetResource(i);
        ASSERT(pResource);
        if(pResource)
        {
            fprintf(pProjectFile, "file=%s." FILE_EXT_CONSTANTS "\r\n",
                     pResource->GetResourceName());
        }
    }

    fprintf(pProjectFile,"\r\n");


    // Write [Classes.Programs] section (program files)
    //-------------------------------------------------

    fprintf(pProjectFile, "[Classes.Programs]\r\n");

    for(i = 0; i < GetResourceCount(); ++i)
    {
        int k;

        pResource = GetResource(i);
        ASSERT(pResource != NULL);
        if(pResource == NULL)
            continue;

        for(k = 0; k < pResource->GetTaskCount(); ++k)
        {
            CProjectWizardTask *pTask;

            pTask = pResource->GetTask(k);
            ASSERT(pTask != NULL);
            if(pTask == NULL)
                continue;

            fprintf(pProjectFile, "file=%s\r\n", (LPCTSTR)BuildProgramFileName(pTask));
        }
    }

    fprintf(pProjectFile,"\r\n");


    // Write [Libs] section (library files)
    //-------------------------------------

    fprintf(pProjectFile, "[Libs]\r\n");

    for(i = 0; i < GetLibraryCount(); ++i)
        if(m_Libraries[i].GetSelectFlag())
            fprintf(pProjectFile, "file=%s\\%s\r\n", _T("$(LIBDIR)"), m_Libraries[i].GetLibraryName());

    fprintf(pProjectFile,"\r\n");


    // Write [Targets] section (target files)
    //---------------------------------------

    fprintf(pProjectFile,"[Targets]\r\n");

    for(i = 0; i < GetResourceCount(); ++i)
    {
        pResource = GetResource(i);
        ASSERT(pResource);
        if(pResource)
        {
            fprintf(pProjectFile, "file=%s\r\n", BuildTargetFileName(pResource));
        }
    }

    fprintf(pProjectFile, "\r\n");


    fclose(pProjectFile);

    return(S_OK);
}


//------------------------------------------------------------------*
/**
 * Creates the configuration file
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT CProjectWizardData::CreateConfigurationFile()
{
    HRESULT hr;
    FILE *pConfigurationFile;
    CString strFilename;
    int i;

    strFilename.Format(_T("%s." FILE_EXT_CONFIGURATION), CONFIGURATION_FILE_NAME);

    hr = CreateFile(strFilename, &pConfigurationFile);
    if(hr != S_OK)
        return(hr);

    hr = AddSystemComment(pConfigurationFile);
    if(hr != S_OK)
    {
        fclose(pConfigurationFile);
        return(hr);
    }

    fprintf(pConfigurationFile, "CONFIGURATION %s\r\n", CONFIGURATION_FILE_NAME);

    CProjectWizardResource* pResource;
    for(i = 0; i < GetResourceCount(); ++i)
    {
        pResource = GetResource(i);
        ASSERT(pResource);
        if(pResource)
        {
            fprintf(pConfigurationFile, "    #IMPORT \"%s." FILE_EXT_RESOURCE "\"\r\n", pResource->GetResourceName());
        }
    }

    fprintf(pConfigurationFile, "END_CONFIGURATION\r\n");

    fclose(pConfigurationFile);

    return(S_OK);
}


//------------------------------------------------------------------*
/**
 * Creates the resource file
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT CProjectWizardData::CreateResourceFile(CProjectWizardResource *pResource)
{
    HRESULT hr;
    FILE *pResourceFile;
    CString strFilename;
    int i;

    ASSERT(pResource);
    if(!pResource)
    {
        return E_FAIL;
    }

    strFilename.Format("%s." FILE_EXT_RESOURCE, pResource->GetResourceName());

    hr = CreateFile(strFilename, &pResourceFile);
    if(hr != S_OK)
        return(hr);

    hr = AddSystemComment(pResourceFile);
    if(hr != S_OK)
    {
        fclose(pResourceFile);
        return(hr);
    }

    fprintf(pResourceFile, "RESOURCE %s\r\n",
             pResource->GetResourceName());

    fprintf(pResourceFile, "    #IMPORT \"%s.gvl\"\r\n",
             pResource->GetResourceName());

    for(i = 0; i < pResource->GetTaskCount(); ++i)
    {
        CProjectWizardTask *pTask;
        CString strStatement;

        pTask = pResource->GetTask(i);
        ASSERT(pTask != NULL);
        if(pTask == NULL)
            continue;

        strStatement = BuildTaskStatement(pTask);
        fprintf(pResourceFile, "        %s\r\n", strStatement);
    }

    for(i = 0; i < pResource->GetTaskCount(); ++i)
    {
        CProjectWizardTask *pTask;

        pTask = pResource->GetTask(i);
        ASSERT(pTask != NULL);
        if(pTask == NULL)
            continue;

        fprintf(pResourceFile,"        PROGRAM %s WITH %s: %s;\r\n",
                 pTask->GetProgram(),
                 pTask->GetName(),
                 pTask->GetProgram());
    }

    fprintf(pResourceFile,"END_RESOURCE\r\n");

    fclose(pResourceFile);

    return(S_OK);
}


CString CProjectWizardData::BuildTaskStatement(CProjectWizardTask *pTask)
{
    CString strStatement;

    strStatement.Format(_T("TASK %s(INTERVAL:=T#100ms, PRIORITY:=5, AUTOSTART:=TRUE);"), pTask->GetName());
    return(strStatement);
}


//------------------------------------------------------------------*
/**
 * Creates the file containing the constant definitions
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT CProjectWizardData::CreateConstantFile(CString strResourceName)
{
    HRESULT hr;
    FILE *pConstantsFile;
    CString strFilename;

    strFilename.Format(_T("%s." FILE_EXT_CONSTANTS), strResourceName);

    hr = CreateFile(strFilename, &pConstantsFile);
    if(hr != S_OK)
        return(hr);

    fprintf(pConstantsFile,"(* Constant file *)\r\n");
    fprintf(pConstantsFile,"CONST\r\n\r\n");
    fprintf(pConstantsFile,"END_CONST\r\n");
    fclose(pConstantsFile);

    return(S_OK);
}


//------------------------------------------------------------------*
/**
 * Creates the file containing the global variable definitions
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT CProjectWizardData::CreateGlobalVariableFile(CString strResourceName)
{
    HRESULT hr;
    FILE *pGlobalVariableFile;
    CString strFilename;

    strFilename.Format(_T("%s." FILE_EXT_GLOBAL_VARIABLES), strResourceName);

    hr = CreateFile(strFilename, &pGlobalVariableFile);
    if(hr != S_OK)
        return(hr);

    fprintf(pGlobalVariableFile,"(* Global variable file *)\r\n");
    fprintf(pGlobalVariableFile,"VAR_GLOBAL\r\n\r\n");
    fprintf(pGlobalVariableFile,"END_VAR\r\n");
    fclose(pGlobalVariableFile);

    return(S_OK);
}


//------------------------------------------------------------------*
/**
 * Creates a new ST program file
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT CProjectWizardData::CreateStProgramFile(FILE *pProgramFile, CString strProgramName)
{
    HRESULT hr;

    hr = AddSystemComment(pProgramFile);
    if(hr != S_OK)
        return(hr);

    fprintf(pProgramFile, "PROGRAM %s\r\n", strProgramName);
    fprintf(pProgramFile, "VAR\r\n");
    fprintf(pProgramFile, "\td: DINT;\r\n");
    fprintf(pProgramFile, "END_VAR\r\n");
    fprintf(pProgramFile, "\td := d + 1;\r\n");
    fprintf(pProgramFile, "END_PROGRAM\r\n");

    return(S_OK);
}


//------------------------------------------------------------------*
/**
 * Creates a new IL program file
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT CProjectWizardData::CreateIlProgramFile(FILE *pProgramFile, CString strProgramName)
{
    HRESULT hr;

    hr = AddSystemComment(pProgramFile);
    if(hr != S_OK)
        return(hr);

    fprintf(pProgramFile, "PROGRAM %s\r\n", strProgramName);
    fprintf(pProgramFile, "VAR\r\n\r\n");
    fprintf(pProgramFile, "END_VAR\r\n");
//    fprintf(pProgramFile, ";\r\n");
    fprintf(pProgramFile, "END_PROGRAM\r\n");

    return(S_OK);
}


//------------------------------------------------------------------*
/**
 * Creates a new FBD program file
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT CProjectWizardData::CreateFbdProgramFile(FILE *pProgramFile, CString strProgramName)
{
    fprintf(pProgramFile,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
    fprintf(pProgramFile,"<FCPOU>\r\n");
	fprintf(pProgramFile,"	<EDITINFO editname=\"\" editversion=\"\" editcatid=\"\"/>\r\n");
	fprintf(pProgramFile,"	<HEADER>\r\n");
    fprintf(pProgramFile,"<![CDATA[(*&#91;&#91;&#13;&#10;&#93;&#93;*)&#13;&#10;PROGRAM %s \r\n",
        strProgramName);
    fprintf(pProgramFile,"&#13;&#10;VAR&#13;&#10;&#13;&#10;END_VAR&#13;&#10;&#13;&#10;]]>\r\n");
	fprintf(pProgramFile,"	</HEADER>\r\n");
	fprintf(pProgramFile,"	<BODY LANG=\"fbd\">\r\n");
	fprintf(pProgramFile,"	</BODY>\r\n");
    fprintf(pProgramFile,"</FCPOU>\r\n");

    return(S_OK);
}


//------------------------------------------------------------------*
/**
 * Creates a new SFC program file
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT CProjectWizardData::CreateSfcProgramFile(FILE *pProgramFile, CString strProgramName)
{
    fprintf(pProgramFile,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
    fprintf(pProgramFile,"<FCPOU>\r\n");
    fprintf(pProgramFile,"	<EDITINFO editname=\"\" editversion=\"\" editcatid=\"\"/>\r\n");
    fprintf(pProgramFile,"	<HEADER>\r\n");
    fprintf(pProgramFile,"<![CDATA[(*&#91;&#91;&#13;&#10;&#93;&#93;*)&#13;&#10;PROGRAM %s \r\n",
        strProgramName);
    fprintf(pProgramFile,"&#13;&#10;VAR&#13;&#10;&#13;&#10;END_VAR&#13;&#10;&#13;&#10;]]>\r\n");
    fprintf(pProgramFile,"	</HEADER>\r\n");
    fprintf(pProgramFile,"	<BODY LANG=\"fbd_sfc\" SFC_MENU=\"1\">\r\n");
    fprintf(pProgramFile,"		<LINE ORIGIN=\"6;4\" SIZE=\"0;48\" />\r\n");
    fprintf(pProgramFile,"		<TRANS ORIGIN=\"18;10\" SIZE=\"14;4\" NAME=\"TRUE\">\r\n");
    fprintf(pProgramFile,"			<PINLIST>\r\n");
    fprintf(pProgramFile,"				<PIN CLASS=\"input\" ORIGIN=\"20;10\" />\r\n");
    fprintf(pProgramFile,"				<PIN CLASS=\"input\" ORIGIN=\"18;12\" HIDDEN=\"1\" />\r\n");
    fprintf(pProgramFile,"				<PIN CLASS=\"output\" ORIGIN=\"20;14\" />\r\n");
    fprintf(pProgramFile,"\r\n");
    fprintf(pProgramFile,"			</PINLIST>\r\n");
    fprintf(pProgramFile,"\r\n");
    fprintf(pProgramFile,"		</TRANS>\r\n");
    fprintf(pProgramFile,"		<LINE ORIGIN=\"20;6\" SIZE=\"0;-2\" />\r\n");
    fprintf(pProgramFile,"		<STEP ORIGIN=\"16;6\" SIZE=\"8;4\" NAME=\"Step0\" INITSTEP=\"1\">\r\n");
    fprintf(pProgramFile,"			<PINLIST>\r\n");
    fprintf(pProgramFile,"				<PIN CLASS=\"input\" ORIGIN=\"20;6\" />\r\n");
    fprintf(pProgramFile,"				<PIN CLASS=\"output\" ORIGIN=\"24;8\" HIDDEN=\"1\" />\r\n");
    fprintf(pProgramFile,"				<PIN CLASS=\"output\" ORIGIN=\"20;10\" />\r\n");
    fprintf(pProgramFile,"\r\n");
    fprintf(pProgramFile,"			</PINLIST>\r\n");
    fprintf(pProgramFile,"\r\n");
    fprintf(pProgramFile,"		</STEP>\r\n");
    fprintf(pProgramFile,"		<LINE ORIGIN=\"6;4\" SIZE=\"14;0\" />\r\n");
    fprintf(pProgramFile,"		<LINE ORIGIN=\"24;32\" SIZE=\"6;0\" />\r\n");
    fprintf(pProgramFile,"		<STEP ORIGIN=\"16;30\" SIZE=\"8;4\" NAME=\"Step1\">\r\n");
    fprintf(pProgramFile,"			<PINLIST>\r\n");
    fprintf(pProgramFile,"				<PIN CLASS=\"input\" ORIGIN=\"20;30\" />\r\n");
    fprintf(pProgramFile,"				<PIN CLASS=\"output\" ORIGIN=\"24;32\" HIDDEN=\"1\" />\r\n");
    fprintf(pProgramFile,"				<PIN CLASS=\"output\" ORIGIN=\"20;34\" />\r\n");
    fprintf(pProgramFile,"\r\n");
    fprintf(pProgramFile,"			</PINLIST>\r\n");
    fprintf(pProgramFile,"		</STEP>\r\n");
    fprintf(pProgramFile,"		<LINE ORIGIN=\"20;14\" SIZE=\"0;16\" />\r\n");
    fprintf(pProgramFile,"		<TRANS ORIGIN=\"18;34\" SIZE=\"14;4\" NAME=\"TRUE\">\r\n");
    fprintf(pProgramFile,"			<PINLIST>\r\n");
    fprintf(pProgramFile,"				<PIN CLASS=\"input\" ORIGIN=\"20;34\" />\r\n");
    fprintf(pProgramFile,"				<PIN CLASS=\"input\" ORIGIN=\"18;36\" HIDDEN=\"1\" />\r\n");
    fprintf(pProgramFile,"				<PIN CLASS=\"output\" ORIGIN=\"20;38\" />\r\n");
    fprintf(pProgramFile,"\r\n");
    fprintf(pProgramFile,"			</PINLIST>\r\n");
    fprintf(pProgramFile,"\r\n");
    fprintf(pProgramFile,"		</TRANS>\r\n");
    fprintf(pProgramFile,"		<LINE ORIGIN=\"20;38\" SIZE=\"0;14\" />\r\n");
    fprintf(pProgramFile,"		<LINE ORIGIN=\"20;52\" SIZE=\"-14;0\" />\r\n");
    fprintf(pProgramFile,"		<LINE ORIGIN=\"24;8\" SIZE=\"6;0\" />\r\n");
    fprintf(pProgramFile,"		<ACTBLCK NAME=\"Action1\" ORIGIN=\"30;31\" SIZE=\"10;2\">\r\n");
    fprintf(pProgramFile,"			<PINLIST>\r\n");
    fprintf(pProgramFile,"				<PIN CLASS=\"input\" ORIGIN=\"30;32\" HIDDEN=\"1\" />\r\n");
    fprintf(pProgramFile,"\r\n");
    fprintf(pProgramFile,"			</PINLIST>\r\n");
    fprintf(pProgramFile,"			<ACTQUAL QUALTYPE=\"N\" />\r\n");
    fprintf(pProgramFile,"\r\n");
    fprintf(pProgramFile,"		</ACTBLCK>\r\n");
    fprintf(pProgramFile,"		<ACTBLCK NAME=\"Action0\" ORIGIN=\"30;7\" SIZE=\"10;2\">\r\n");
    fprintf(pProgramFile,"			<PINLIST>\r\n");
    fprintf(pProgramFile,"				<PIN CLASS=\"input\" ORIGIN=\"30;8\" HIDDEN=\"1\" />\r\n");
    fprintf(pProgramFile,"\r\n");
    fprintf(pProgramFile,"			</PINLIST>\r\n");
    fprintf(pProgramFile,"			<ACTQUAL QUALTYPE=\"N\" />\r\n");
    fprintf(pProgramFile,"\r\n");
    fprintf(pProgramFile,"		</ACTBLCK>\r\n");
    fprintf(pProgramFile,"		<ACTION NAME=\"Action1\" ORIGIN=\"48;31\" SIZE=\"28;16\" />\r\n");
    fprintf(pProgramFile,"		<ACTION NAME=\"Action0\" ORIGIN=\"48;7\" SIZE=\"28;16\" />\r\n");
    fprintf(pProgramFile,"\r\n");
    fprintf(pProgramFile,"	</BODY>\r\n");
    fprintf(pProgramFile,"\r\n");
    fprintf(pProgramFile,"</FCPOU>\r\n");

    return(S_OK);
}

//------------------------------------------------------------------*
/**
 * Creates a new LD program file
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT CProjectWizardData::CreateLdProgramFile(FILE *pProgramFile, CString strProgramName)
{
    fprintf(pProgramFile,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
    fprintf(pProgramFile,"<FCPOU>\r\n");
    fprintf(pProgramFile,"<EDITINFO EDITNAME=\"\" EDITVERSION=\"\" EDITCATID=\"\" EDITCLASSID=\"\" />\r\n");
    fprintf(pProgramFile,"<HEADER>\r\n");
    fprintf(pProgramFile,"<![CDATA[(*&#91;&#91;&#13;&#10;&#93;&#93;*)&#13;&#10;PROGRAM %s \r\n",
        strProgramName);
    fprintf(pProgramFile,"&#13;&#10;VAR&#13;&#10;&#13;&#10;END_VAR&#13;&#10;&#13;&#10;]]>\r\n");
    fprintf(pProgramFile,"</HEADER>\r\n");
    fprintf(pProgramFile,"<BODY LANG=\"ld\" SFC_MENU=\"0\">\r\n");
    fprintf(pProgramFile,"<POWERRAIL ORIGIN=\"4;4\" SIZE=\"0;80\" />\r\n");
    fprintf(pProgramFile,"</BODY>\r\n");
    fprintf(pProgramFile,"</FCPOU>");

    return(S_OK);
}


//------------------------------------------------------------------*
/**
 * Creates the file containing the global variable definitions
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT CProjectWizardData::CreateProgramFiles(CProjectWizardResource *pResource)
{
    HRESULT hr;
    int i;

    ASSERT(pResource);
    if(!pResource)
    {
        return E_FAIL;
    }

    for(i = 0; i < pResource->GetTaskCount(); ++i)
    {
        CProjectWizardTask *pTask;
        FILE *pProgramFile;

        pTask = pResource->GetTask(i);
        ASSERT(pTask != NULL);
        if(pTask == NULL)
            continue;

        hr = CreateFile(BuildProgramFileName(pTask), &pProgramFile);
        if(hr != S_OK)
            return(hr);

        switch(pTask->GetLanguage())
        {
        case IEC_LANGUAGE_ST:
            hr = CreateStProgramFile(pProgramFile, pTask->GetProgram());
        break;

        case IEC_LANGUAGE_FBD:
            hr = CreateFbdProgramFile(pProgramFile, pTask->GetProgram());
        break;

        case IEC_LANGUAGE_SFC:
            hr = CreateSfcProgramFile(pProgramFile, pTask->GetProgram());
        break;

        case IEC_LANGUAGE_LD:
            hr = CreateLdProgramFile(pProgramFile, pTask->GetProgram());
        break;

        case IEC_LANGUAGE_IL:
            hr = CreateIlProgramFile(pProgramFile, pTask->GetProgram());
        break;
        }

        if(hr != S_OK)
            return(hr);

        fclose(pProgramFile);
    }

    return(S_OK);
}


//------------------------------------------------------------------*
/**
 * Creates the file containing the target description
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT CProjectWizardData::CreateTargetFile(CProjectWizardResource *pResource)
{
    HRESULT hr;
    FILE *pTargetFile;

    ASSERT(pResource);
    if(!pResource)
    {
        return E_FAIL;
    }

    hr = CreateFile(BuildTargetFileName(pResource), &pTargetFile);
    if(hr != S_OK)
    {
        return(hr);
    }

    fprintf(pTargetFile, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");

    fprintf(pTargetFile, "%s",(LPCTSTR)BuildTargetXML(pResource));

    fclose(pTargetFile);

    return(S_OK);
}


//------------------------------------------------------------------*
/**
 * Overridable to build target file name. Default is <targetname>.4cpc.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
CString CProjectWizardData::BuildTargetFileName(CProjectWizardResource *pResource)
{
    CString strFileName;

    ASSERT(pResource);
    if(!pResource)
    {
        return strFileName;
    }

    strFileName.Format(_T("%s.%s"), pResource->GetTarget(), FILE_EXT_TARGETS);
    return(strFileName);
}


//------------------------------------------------------------------*
/**
 * Overridable to build target XML description
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
CString CProjectWizardData::BuildTargetXML(CProjectWizardResource *pResource)
{
    CString strText;

    ASSERT(pResource);
    if(!pResource)
    {
        return strText;
    }

    strText.Format(_T("<PC CONTROL=\"%s\" CONSOLE=\"%s\" RESOURCE=\"%s\" >\r\n</PC>\r\n"),
                    pResource->GetOPCServer(),
                    pResource->GetOPCServer(),
                    pResource->GetResourceName());
    return(strText);
}


//------------------------------------------------------------------*
/**
 * Creates all files that depend on the resources
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT CProjectWizardData::CreateResourceDependentFiles(CProjectWizardResource *pResource)
{
    HRESULT hr;

    ASSERT(pResource);
    if(!pResource)
    {
        return E_FAIL;
    }

    hr = CreateResourceFile(pResource);
    if(hr != S_OK)
        return(hr);

    hr = CreateGlobalVariableFile(pResource->GetResourceName());
    if(hr != S_OK)
        return(hr);

    hr = CreateConstantFile(pResource->GetResourceName());
    if(hr != S_OK)
        return(hr);

    hr = CreateProgramFiles(pResource);
    if(hr != S_OK)
        return(hr);

    hr = CreateTargetFile(pResource);
    if(hr != S_OK)
        return(hr);

    return(S_OK);
}


//------------------------------------------------------------------*
/**
 * Creates all files that depend on the resources
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT CProjectWizardData::CreateResourceDependentFiles()
{
    HRESULT hr;
    HRESULT hRes = S_OK;
    int i;

    for(i = 0; i < GetResourceCount(); ++i)
    {
        hr = CreateResourceDependentFiles(GetResource(i));
        if(hr != S_OK)
        {
            hRes = hr;
        }
    }
    return(hRes);
}


//------------------------------------------------------------------*
/**
 * Creates all files belonging to the project
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT CProjectWizardData::CreateProjectFiles()
{
    HRESULT hr;

    hr = CreateProjectDirectory();
    if(hr != S_OK)
        return(hr);
 
    hr = CreateProjectFile();
    if(hr != S_OK)
        return(hr);

	hr = CreateLatcommFile();
    if(hr != S_OK)
        return(hr);

    hr = CreateConfigurationFile();
    if(hr != S_OK)
        return(hr);

    hr = CreateResourceDependentFiles();
    if(hr != S_OK)
        return(hr);
    

    return(S_OK);
}


bool CProjectWizardData::CheckResources()
{
    if(!CheckUniqueNames())
        return(false);

    if(!CheckIdentifiers())
        return(false);

    return(true);
}


//------------------------------------------------------------------*
/**
 * Checks whether the names of resources, targets and programs are unique
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
bool CProjectWizardData::CheckUniqueNames()
{
    int iResource1;
    int iResource2;
    int iTask1;
    int iTask2;
    CProjectWizardResource *pResource1;
    CProjectWizardResource *pResource2;
    CProjectWizardTask *pTask1;
    CProjectWizardTask *pTask2;
    int iNumResources = GetResourceCount();

    for(iResource1 = 0; iResource1 < iNumResources; ++iResource1)
    {
        pResource1 = GetResource(iResource1);
        ASSERT(pResource1);

        if(!pResource1)
        {
            continue;
        }

        for(iResource2 = iResource1 + 1; iResource2 < GetResourceCount(); ++iResource2)
        {
            pResource2 = GetResource(iResource2);
            ASSERT(pResource2);

            if(!pResource2)
            {
                continue;
            }

            if(!CompareNames(IDS_ERR_NON_UNIQUE_RESOURCE, pResource1->GetResourceName(), pResource2->GetResourceName(), TRUE))
                return(false);

            if(!CompareNames(IDS_ERR_NON_UNIQUE_TARGET, pResource1->GetTarget(), pResource2->GetTarget(), FALSE))
                return(false);
        }

        for(iTask1 = 0; iTask1 < pResource1->GetTaskCount(); ++iTask1)
        {
            pTask1 = pResource1->GetTask(iTask1);

            if(!CompareNames(IDS_ERR_EQUAL_TASK_AND_PROGRAM_NAME, pTask1->GetName(), pTask1->GetProgram(), TRUE))
                return(false);

            for(iTask2 = iTask1 + 1; iTask2 < pResource1->GetTaskCount(); ++iTask2)
            {
                pTask2 = pResource1->GetTask(iTask2);

                if(!CompareNames(IDS_ERR_NON_UNIQUE_TASK, pTask1->GetName(), pTask2->GetName(), TRUE))
                    return(false);

                if(!CompareNames(IDS_ERR_NON_UNIQUE_PROGRAM, pTask1->GetProgram(), pTask2->GetProgram(), TRUE))
                    return(false);

                if(!CompareNames(IDS_ERR_EQUAL_TASK_AND_PROGRAM_NAME, pTask1->GetName(), pTask2->GetProgram(), TRUE))
                    return(false);

                if(!CompareNames(IDS_ERR_EQUAL_TASK_AND_PROGRAM_NAME, pTask1->GetProgram(), pTask2->GetName(), TRUE))
                    return(false);
            }

            for(iResource2 = iResource1 + 1; iResource2 < GetResourceCount(); ++iResource2)
            {
                pResource2 = GetResource(iResource2);
                ASSERT(pResource2);

                if(!pResource2)
                {
                    continue;
                }

                for(iTask2 = 0; iTask2 < pResource2->GetTaskCount(); ++iTask2)
                {
                    pTask2 = pResource2->GetTask(iTask2);

                    if(!CompareNames(IDS_ERR_NON_UNIQUE_PROGRAM, pTask1->GetProgram(), pTask2->GetProgram(), TRUE))
                        return(false);

                    if(!CompareNames(IDS_ERR_EQUAL_TASK_AND_PROGRAM_NAME, pTask1->GetName(), pTask2->GetProgram(), TRUE))
                        return(false);

                    if(!CompareNames(IDS_ERR_EQUAL_TASK_AND_PROGRAM_NAME, pTask1->GetProgram(), pTask2->GetName(), TRUE))
                        return(false);
                }
            }
        }
    }

    return(true);
}


//------------------------------------------------------------------*
/**
 * Compares two names and display an error message if they are equal
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
bool CProjectWizardData::CompareNames(UINT uiErrorMessage, CString strName1, CString strName2, bool bCompareEmptyStrings)
{
    CString strMessage;

    if(strName1.IsEmpty() && !bCompareEmptyStrings)
        return(true);

    if(strName1.CompareNoCase(strName2) != 0)
        return(true);

    strMessage.Format(uiErrorMessage, strName1);
    AfxMessageBox(strMessage);

    return(false);
}


//------------------------------------------------------------------*
/**
 * Checks whether the names entered by the user are correct identifiers
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
bool CProjectWizardData::CheckIdentifiers()
{
    int iResource;

    for(iResource = 0; iResource < GetResourceCount(); ++iResource)
    {
        CProjectWizardResource *pResource;
        int iTask;

        pResource = GetResource(iResource);
        ASSERT(pResource);
        if(pResource)
        {
            if(!CheckIdentifier(pResource->GetResourceName(), IDS_RESOURCE))
                return(false);

            if(!CheckIdentifier(pResource->GetTarget(), IDS_TARGET))
                return(false);

            for(iTask = 0; iTask < pResource->GetTaskCount(); ++iTask)
            {
                CProjectWizardTask *pTask;

                pTask = pResource->GetTask(iTask);

                if(!CheckIdentifier(pTask->GetName(), IDS_TASK))
                    return(false);

                if(!CheckIdentifier(pTask->GetProgram(), IDS_PROGRAM))
                    return(false);
            }
        }
    }

    return(true);
}


//------------------------------------------------------------------*
/**
 * Checks whether the given string is a correct identifier
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
bool CProjectWizardData::CheckIdentifier(CString strIdentifier, int iResourceIdIdentifierType)
{
    HRESULT hr;
    CString strIdentifierType;
    CString strMessage;

    hr = UTIL_CheckIdentifier(strIdentifier);

    if(SUCCEEDED(hr))
        return(true);

    strIdentifierType.LoadString(iResourceIdIdentifierType);

    switch(GetScode(hr))
    {
    case E_UTIL_ID_SYNTAX:
        strMessage.Format(IDS_ERR_IDENTIFIER_SYNTAX, strIdentifierType, strIdentifier);
        AfxMessageBox(strMessage);
    break;

    case E_UTIL_ID_IS_KEYWORD:
        strMessage.Format(IDS_ERR_IDENTIFIER_IS_KEYWORD, strIdentifierType, strIdentifier);
        AfxMessageBox(strMessage);
    break;

    case E_UTIL_ID_TOO_LONG:
        strMessage.Format(IDS_ERR_IDENTIFIER_TOO_LONG, strIdentifierType, strIdentifier);
        AfxMessageBox(strMessage);
    break;

    default:
    break;
    }

    return(false);
}


CString CProjectWizardData::GetExtensionForLanguage(TIecLanguage language)
{
    CString strExtension;

    switch(language)
    {
    case IEC_LANGUAGE_ST : strExtension = _T("st");  break;
    case IEC_LANGUAGE_FBD: strExtension = _T("4gr"); break;
    case IEC_LANGUAGE_SFC: strExtension = _T("4gr"); break;
    case IEC_LANGUAGE_LD: strExtension = _T("4gr"); break;
    case IEC_LANGUAGE_IL: strExtension = _T("il"); break;
    }

    return(strExtension);
}


CString CProjectWizardData::BuildProgramFileName(CString strProgramName, TIecLanguage language)
{
    CString strFileName;

    strFileName.Format(_T("%s.%s"), (LPCTSTR)strProgramName, (LPCTSTR)GetExtensionForLanguage(language));
    return(strFileName);
}


CString CProjectWizardData::BuildProgramFileName(CProjectWizardTask *pTask)
{
    return(BuildProgramFileName(pTask->GetProgram(), pTask->GetLanguage()));
}


HRESULT CProjectWizardData::CreateGuid (CString *sGuidString)
{
    GUID guid;
    HRESULT hrResult;
    LPOLESTR sGuid;

    hrResult = CoCreateGuid(&guid);
    if (hrResult == S_OK) 
    {
        hrResult = StringFromCLSID(guid,&sGuid);
        if (hrResult == S_OK) 
        {
            *sGuidString = sGuid;
            CoTaskMemFree(sGuid);
        }
    }
    return hrResult;
}

//------------------------------------------------------------------*
/**
 * create gh file.
 *
 * @param           -
 * @return          HRESULT
 * @exception       -
 * @see             -
*/
HRESULT CProjectWizardData::CreateGHFile()
{
    CGHFile tGHFile;
    CString strPassword = _T("123456");
    CString strFileName;
    strFileName.Format("%s\\Project.4gh",m_strPath);

    tGHFile.SetFileName(strFileName);
    tGHFile.SetPassword(strPassword);
    tGHFile.SetProjectGuid(m_strProjectGuid);

    if(!tGHFile.Write())
    {
        return E_FAIL;
    }

    return S_OK;
}


/* ---------------------------------------------------------------------------- */

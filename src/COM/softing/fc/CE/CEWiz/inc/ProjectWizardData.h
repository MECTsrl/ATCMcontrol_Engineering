
#ifndef __PROJECTWIZARDDATA_H_
#define __PROJECTWIZARDDATA_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "ProjectWizardLibrary.h"
#include "ProjectWizardResource.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


class CProjectWizardData
{
public:
    CProjectWizardData ();
    HRESULT Init (CString strPath, CString strProjectName);
    CString GetPath ();

    virtual void UseDefaultLibraries ();
    void UseAllLibraries ();
    void UseNoLibraries ();
    void UseLibrary (int i, bool bUse);

    int GetResourceCount ();
    CProjectWizardResource *GetResource (int i);
    void SetResourceCount (int iCount);

    int GetLibraryCount ();
    CProjectWizardLibrary *GetLibrary (int i);

    void SetSubSystems (TSubSystems subSystems);
    TSubSystems GetSubSystems ();

    void SetProperties (CString strAlias, CString strUserText1, CString strUserText2, CString strUserText3, CString strURL);

    HRESULT CreateProjectFiles ();
    bool CheckResources ();

    CString GetExtensionForLanguage (TIecLanguage language);
    CString BuildProgramFileName (CString strProgramName, TIecLanguage language);
    CString BuildProgramFileName (CProjectWizardTask *pTask);

protected:
    HRESULT SearchLibraries ();
    HRESULT CreateGuid (CString *sGuidString);
    HRESULT AddSystemComment (FILE *pFile);
    HRESULT CreateFile (CString sFileName, FILE **ppFile);
	HRESULT CreateLatcommFile();
    HRESULT CreateStProgramFile (FILE *pProgramFile, CString strProgramName);
    HRESULT CreateIlProgramFile (FILE *pProgramFile, CString strProgramName);
    HRESULT CreateFbdProgramFile (FILE *pProgramFile, CString strProgramName);
    HRESULT CreateSfcProgramFile (FILE *pProgramFile, CString strProgramName);
    HRESULT CreateLdProgramFile (FILE *pProgramFile, CString strProgramName);

    // Overridables for resource file creation
    virtual CString BuildTaskStatement (CProjectWizardTask *pTask);

    // Overridables for target file creation
    virtual CString BuildTargetFileName (CProjectWizardResource *pResource);
    virtual CString BuildTargetXML (CProjectWizardResource *pResource);

    // Name checking
    virtual bool CheckUniqueNames ();
    bool CompareNames (UINT uiErrorMessage, CString strName1, CString strName2, bool bCompareEmptyStrings);
    virtual bool CheckIdentifiers ();
    bool CheckIdentifier (CString strIdentifier, int iResourceIdIdentifierType);

    CString m_strPath;
    CString m_strProjectName;
    TSubSystems m_subSystems;
    CString m_strLibraryPath;
    CString m_strAlias;
    CString m_strUserText1;
    CString m_strUserText2;
    CString m_strUserText3;
    CString m_strURL;

    CArray<CProjectWizardLibrary, CProjectWizardLibrary &> m_Libraries;

    CArray<CProjectWizardResource, CProjectWizardResource &> m_Resources;

private:
    HRESULT CreateProjectDirectory ();
    HRESULT CreateProjectFile ();
    HRESULT CreateConfigurationFile ();
    HRESULT CreateGlobalVariableFile (CString strResourceName);
    HRESULT CreateResourceFile (CProjectWizardResource *pResource);
    HRESULT CreateConstantFile (CString strResourceName);
    HRESULT CreateProgramFiles (CProjectWizardResource *pResource);
    HRESULT CreateTargetFile (CProjectWizardResource *pResource);
    HRESULT CreateResourceDependentFiles (CProjectWizardResource *pResource);
    HRESULT CreateResourceDependentFiles ();
};

#endif // __PROJECTWIZARDDATA_H_


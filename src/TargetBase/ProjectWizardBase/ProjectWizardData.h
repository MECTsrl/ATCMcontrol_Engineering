/* $Header: /4CReleased/V2.20.00/TargetBase/PROJECTWIZARDBASE/ProjectWizardData.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: ProjectWizardData.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/PROJECTWIZARDBASE/ProjectWizardData.h $
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

#ifndef _PROJECTWIZARDDATA_H_
#define _PROJECTWIZARDDATA_H_

#include "ProjectWizardLibrary.h"
#include "ProjectWizardResource.h"

class CProjectWizardData
{
public:
    CProjectWizardData();
    virtual ~CProjectWizardData();

    virtual HRESULT Init (CString strPath, CString strProjectName);
    CString GetPath ();

    virtual void UseDefaultLibraries ();
    void UseAllLibraries ();
    void UseNoLibraries ();
    void UseLibrary (int i, bool bUse);

    int GetResourceCount ();
    CProjectWizardResource *GetResource (int i);
    BOOL SetResource(int i, CProjectWizardResource* pResource);
    void SetResourceCount (int iCount);
    virtual CProjectWizardResource* CreateResource();

    int GetLibraryCount ();
    CProjectWizardLibrary *GetLibrary (int i);

    void SetSubSystems (TSubSystems subSystems);
    TSubSystems GetSubSystems ();

    void SetProperties (CString strAlias, CString strUserText1, CString strUserText2, CString strUserText3, CString strURL);

    virtual HRESULT CreateProjectFiles ();
    virtual bool CheckResources ();

    CString GetExtensionForLanguage (TIecLanguage language);
    CString BuildProgramFileName (CString strProgramName, TIecLanguage language);
    CString BuildProgramFileName (CProjectWizardTask *pTask);

protected:
    virtual void ResetResources();
    virtual void InitResources();
    virtual BOOL CreateNewResources();

    virtual HRESULT SearchLibraries ();
    virtual HRESULT CreateGuid (CString *sGuidString);
    virtual HRESULT AddSystemComment (FILE *pFile);
    virtual HRESULT CreateFile (CString sFileName, FILE **ppFile);
	virtual HRESULT CreateLatcommFile();
    virtual HRESULT CreateStProgramFile (FILE *pProgramFile, CString strProgramName);
    virtual HRESULT CreateIlProgramFile (FILE *pProgramFile, CString strProgramName);
    virtual HRESULT CreateFbdProgramFile (FILE *pProgramFile, CString strProgramName);
    virtual HRESULT CreateSfcProgramFile (FILE *pProgramFile, CString strProgramName);
    virtual HRESULT CreateLdProgramFile (FILE *pProgramFile, CString strProgramName);

    virtual HRESULT CreateProjectDirectory ();
    virtual HRESULT CreateProjectFile ();
    virtual HRESULT CreateConfigurationFile ();
    virtual HRESULT CreateGlobalVariableFile (CString strResourceName);
    virtual HRESULT CreateResourceFile (CProjectWizardResource *pResource);
    virtual HRESULT CreateConstantFile (CString strResourceName);
    virtual HRESULT CreateProgramFiles (CProjectWizardResource *pResource);
    virtual HRESULT CreateTargetFile (CProjectWizardResource *pResource);
    virtual HRESULT CreateResourceDependentFiles (CProjectWizardResource *pResource);
    virtual HRESULT CreateResourceDependentFiles ();
    virtual HRESULT CreateGHFile();

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
    CString m_strProjectGuid;
    TSubSystems m_subSystems;
    CString m_strLibraryPath;
    CString m_strAlias;
    CString m_strUserText1;
    CString m_strUserText2;
    CString m_strUserText3;
    CString m_strURL;

    CArray<CProjectWizardLibrary, CProjectWizardLibrary &> m_Libraries;

    CArray<CProjectWizardResource*, CProjectWizardResource*> m_Resources;

};

#endif

/* ---------------------------------------------------------------------------- */

#ifndef __CEPROJECTMANAGER_H_
#define __CEPROJECTMANAGER_H_

#include "resource.h"       // main symbols
#include "ClassTree.h"
#include "TargetTree.h"
#include "InstanceTree.h"
#include "ProjectFile.h"

#include "CEBackparse.h"
#include "CEContainer.h"

#include "CEProjManCP.h"

#define CEPROJMAN_FACILITY 0x282
#define CEPROJMAN_TARGETID _T("CE_PROJMAN")


#ifndef PROJ_MAN_ASSERT
/**
* @def PROJ_MAN_ASSERT - for other assert mechanisms replace only the define here.
*/
#define PROJ_MAN_ASSERT(x) ASSERT((x))
#endif


//------------------------------------------------------------------*
// TBackparserInfo holds all info of the backparser objects
// created and hold by the project manager
// This struct is inserted in a map to quickly obtain the info for
// a backparser for a file extension.
//
enum EnumBPInfoError { 
    NO_BPERR=0,   // no error
    BPERR_CREATE, // backparser object could not be  created
    BPERR_KAD     // no backparser for file extension could be found in kad
};

typedef struct _BackparserInfo
{
    enum EnumBPInfoError error;                   
    ICEBackparse* pBackparser;   // pointer to the backparser object, 
                                 // null if backparser is not yet instantiated
    GUID guid;                   // the guid of the backparser
    CList<CString, CString&> strlFileExt;     // all file extensions using this backparser
    DWORD m_dwMessageAdviseCookie;
} TBackparserInfo;

typedef struct _ContainerInfo
{
    enum EnumBPInfoError error;                   
    ICEContainer* pContainer;    // pointer to the container object, 
                                 // null if container is not yet instantiated
    GUID guid;                   // the guid of the container
    CList<CString, CString&> strlFileExt;     // all file extensions using this container
    DWORD m_dwMessageAdviseCookie;
} TContainerInfo;


//------------------------------------------------------------------*
/**
 *  The com object representing the project manager
 *  this object implements the interfaces: 
 *     ICEProjInfo and ICEProjectManager
 *
 *  @see    ....
*/
class ATL_NO_VTABLE CCEProjectManager : 
    public CComObjectRootEx<CComMultiThreadModel>,
    public CComCoClass<CCEProjectManager, &CLSID_CEProjectManager>,
    public IDispatchImpl<ICEProjectManager, &IID_ICEProjectManager, &LIBID_CEPROJMANLib>,
    public ICEProjInfo,
    public CProxy_ICEProjectManagerEvents< CCEProjectManager >,
    public CProxy_ICEProjManInstanceEvents<CCEProjectManager>,
    public IConnectionPointContainerImpl<CCEProjectManager>,
    public CProxy_ICEMessageEvent< CCEProjectManager >,
    public ICEContainer,
    public CProxyICECompManEvents< CCEProjectManager >,
    public IDispatchImpl<_ICEMessageEvent, &IID__ICEMessageEvent, &LIBID_CEPROJMANLib>
{

public:
    // constructor
    CCEProjectManager();
    ~CCEProjectManager();

DECLARE_REGISTRY_RESOURCEID(IDR_CEPROJECTMANAGER)
DECLARE_NOT_AGGREGATABLE(CCEProjectManager)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CCEProjectManager)
    COM_INTERFACE_ENTRY(ICEProjectManager)
//DEL     COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(ICEProjInfo)
    COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
    COM_INTERFACE_ENTRY(ICEContainer)
    COM_INTERFACE_ENTRY(_ICEMessageEvent)
    COM_INTERFACE_ENTRY2(IDispatch, ICEProjectManager)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CCEProjectManager)
	CONNECTION_POINT_ENTRY(IID__ICEProjectManagerEvents)
	CONNECTION_POINT_ENTRY(IID__ICEMessageEvent)
	CONNECTION_POINT_ENTRY(IID_ICECompManEvents)
	CONNECTION_POINT_ENTRY(IID__ICEProjManInstanceEvents)
END_CONNECTION_POINT_MAP()


// interface: ICEProjectManager 
public:
    STDMETHOD(OpenProject)(/*[in, string]*/BSTR sProject);
    STDMETHOD(CloseProject)();
    STDMETHOD(ReparseCheck)();
    STDMETHOD(GetClassViewXML)(/*[out, string]*/BSTR* psClassViewXML);
    STDMETHOD(GetClassViewDOM)(/*[out]*/IUnknown** ppClassViewDOM);
    STDMETHOD(GetTargetViewXML)(/*[out, string]*/BSTR* psTargetViewXML);
    STDMETHOD(GetTargetViewDOM)(/*[out]*/IUnknown** ppTargetViewDOM);
	STDMETHOD(SaveViewData)();
   	STDMETHOD(GetAllFilesOfCategory)(/*[in]*/ enum E_FV_FileCategory eFileCat, 
                                     /*[out]*/SAFEARRAY **parrFileNames);
    STDMETHOD(GetFileCategory)(/*[in,string]*/BSTR sFileName,
                               /*[out]*/ enum E_FV_FileCategory* peFileCat);
    STDMETHOD(AddFile)(/*[in,string]*/BSTR sFileName,
                       /*[in]*/ enum E_FV_FileCategory eFileCat,
                       /*[in]*/ BOOL bTry);
    STDMETHOD(RemoveFile)(/*[in,string]*/BSTR sFileName,
                          /*[in]*/ enum E_FV_FileCategory eFileCat,
                          /*[in]*/ BOOL bTry);
    STDMETHOD(CompileReady)();

// interface: ICEProjInfo
public:
    STDMETHOD(getProjectPath)(BSTR * psProjectPath);
    STDMETHOD(getProjectName)(BSTR * psProjectName);
    STDMETHOD(getProjectGenPath)(BSTR * psGenPath);
    STDMETHOD(getLibraryPath)(BSTR * psPath);
    STDMETHOD(getInstallationPath)(BSTR * psPath);
    STDMETHOD(getXMLNodePtr)(BSTR sId, IUnknown** ppXMLPtr);    
    STDMETHOD(getContainerIF)(BSTR sSourceFile, BSTR sId, ICEContainer** ppICEContainer, BSTR * psParentSource, BSTR * psParentId); 
    STDMETHOD(getContainerIFforFile)(BSTR sContainerFile, ICEContainer** ppICEContainer);
    STDMETHOD(getAllTargetTypes)(SAFEARRAY** parrNames);
    STDMETHOD(getAllTargets)(SAFEARRAY** parrTargets);
    STDMETHOD(getAllTargetsAsIdPaths)(SAFEARRAY** parrTargets);
    STDMETHOD(getExtForTargetType)(BSTR sTargetType, BSTR* psExt);

    STDMETHOD(getAllClassTypes)(SAFEARRAY** parrNames);
    STDMETHOD(getTextForClassType)(BSTR sType, BSTR * psText);
    STDMETHOD(getAllClasses)(SAFEARRAY** parrFileNames);
    STDMETHOD(getAllClassesAsIdPaths)(SAFEARRAY** parrClasses);
    STDMETHOD(getClassesOfType)(BSTR sClassType, SAFEARRAY** parrFileNames);
    STDMETHOD(getClassesOfTypeAsIdPaths)(BSTR sClassType, SAFEARRAY** parrClasses);
    STDMETHOD(getAllLibraries)(SAFEARRAY** parrLibraries);
    STDMETHOD(getClassesOfTypeFromLib)(BSTR sLibraryName, BSTR sClassType, SAFEARRAY** parrFileNames);
    STDMETHOD(getTypeOfTarget)(BSTR sTargetName, BSTR * psTargetType);
    STDMETHOD(getAddressStringForTarget)(BSTR sTargetName, BSTR * psAddrString);
    STDMETHOD(getUserAddressStringForTarget)(BSTR sTargetName, BSTR * psAddrString);
    STDMETHOD(getTargetNameForAddr)(BSTR sAddr, BSTR * psTargetName);
    STDMETHOD(getResourceNameForTarget)(BSTR sTarget, BSTR * psResourceName);
    STDMETHOD(getFreeChannelNrForPC)(BSTR sPCName, long * plChannelNr);
    STDMETHOD(getGlobalNameProposal)(BSTR sBasis, BSTR * psName);
    STDMETHOD(fileChangedHint)(BSTR sSourceFile);
    STDMETHOD(fileParseNow)(BSTR sSourceFile);
    STDMETHOD(getFileNameFromIdPath)(BSTR  sIdPath, BSTR* psFileName); 
    STDMETHOD(getIdPathFromFileName)(BSTR  sFileName, BSTR* psIdPath); 
    STDMETHOD(CPInsertToSection)(BSTR sSection, BSTR sKey, BSTR sValue);
    STDMETHOD(CPDeleteFromSection)(BSTR sSection, BSTR sKey);
    STDMETHOD(CPDeleteKeyWithValueFromSection)(BSTR sSection, BSTR sKey, BSTR sValue);
    STDMETHOD(CPGetValueForKeyFromSection)(BSTR sSection, BSTR sKey, BSTR * psValue);
    STDMETHOD(CPGetAllValuesForKeyFromSection)(BSTR sSection, BSTR sKey, SAFEARRAY * * parrFileNames);
    STDMETHOD(GetNodesOfTypeUnderNode)(BSTR sStartNode, BSTR sType, SAFEARRAY** parrNodes);
    STDMETHOD(CheckIECIdentifier)(BSTR sIdentifier, enum E_IEC_IDENTIFIER_ERROR* pErrorFlag);
    STDMETHOD(getSupportedIECDataTypes)(SAFEARRAY** parrDataTypes);
    STDMETHOD(getSupportedIECDataTypesForTarget)(BSTR sTargetType, SAFEARRAY** parrDataTypes);
    STDMETHOD(getSupportedIECLanguages)(SAFEARRAY** parrLanguages);
    STDMETHOD(getSupportedIECLanguagesForTarget)(BSTR sTargetType, SAFEARRAY** parrLanguages);
    STDMETHOD(getSupportRetain)(BOOL* pbRetain);
    STDMETHOD(getSupportRetainForTarget)(BSTR sTargetType, BOOL* pbRetain);
    STDMETHOD(ReplaceMacros)(BSTR sIn,BSTR* psOut);
    STDMETHOD(GetFileAbsolute)(BSTR sFileName, BSTR* psAbsFileName);
    STDMETHOD(GetFileRelative)(BSTR sFileName, BSTR* psRelFileName);
    STDMETHOD(GetVersionOfProjectFile)(BSTR sPrjFileName, BSTR* psVersion);
    STDMETHOD(ShowContextHelp)(DWORD dwContextId);
    STDMETHOD(ProjectFileChanged)();


// ICEContainer
public :
	STDMETHOD(createLink)(BSTR sFileName, BSTR sLocation, BSTR sLinkName);
	STDMETHOD(renameLink)(BSTR sFileName, BSTR sLocation, BSTR sOldLinkName, BSTR sNewLinkName);
	STDMETHOD(deleteLink)(BSTR sFileName, BSTR sLocation, BSTR sLinkName);
    STDMETHOD(setProjectInfo)(IUnknown* pICEProjectInfo);

    

// _ICEMessageEvent
    STDMETHOD(OnNewMessage)(BSTR sMachine, USHORT wScrFacility, HRESULT hr, BSTR sDescr);


public:
    // some help functions
    BOOL    IsProjectLoaded();
    CString GetProjectPath();
    CString GetProjectName();
    CString GetProjectFileName();
    CString GetProjectFileNameWithPath();

    CTargetTree*  GetTargetTree();
	CClassTree*   GetClassTree();
    CInstanceTree* GetInstanceTree();
	CProjectFile* GetProjectFileHandler();
	IUnknown*     GetKadManager();
    ICEBackparse* GetBackparserForFileExt(CString strFileExt);
    ICEContainer* GetContainerForFileExt(CString strFileExt);
    void OutputErrorMessage(HRESULT hError,
                            LPCTSTR pszArg1 = NULL, 
                            LPCTSTR pszArg2 = NULL,
                            LPCTSTR pszArg3 = NULL, 
                            LPCTSTR pszArg4 = NULL);
    CString BuildErrorMsg(HRESULT hError, 
                          LPCTSTR pszArg1 = NULL, 
                          LPCTSTR pszArg2 = NULL,
                          LPCTSTR pszArg3 = NULL, 
                          LPCTSTR pszArg4 = NULL);


protected:
    BOOL    m_bProjectLoaded;   // is the project loaded, if not some members
                                // may not be initialized, request via interface 
                                // have to report an error
    BOOL    m_bProjectReady;    // project is loaded and navigator views are initialized?
                                // if TRUE, reparse checks are allowed
    CString m_strProjectPath;   // the absolute path of the project directory
    CString m_strProjectName;   // the name of the project = project file name without 
                                // file extension
    CString m_strProjectFileName; // the project file name with extension, no path info
    CString m_strProjectFileNameWithPath; // project filename with absolute path infos
    CProjectFile* m_pProjectFileHandler;  // The project file handler, 
                                        // does all actions in/gets all infos from the 
                                        // project file


private:
   void SplitNames(const CString& strNames, CList<CString, CString&> &nameList);
   BOOL InitKadManager();   // get an interface pointer to the kad manager
   BOOL InitBackparser();   // get info for all file extensions and backparser from the kad
   BOOL ClearBackparser();  // clear backparser info, and do release for all backparser
   BOOL InitContainer();    // get info for all file extensions and container from the kad
   BOOL ClearContainer();   // clear container info, and do release for all backparser
   void CleanUp();          // do some final clean up
   void ProjectManagerLock();   // wait until project manager is available
   void ProjectManagerUnlock(); // make project manager available

private:
   IUnknown*        m_pKadManager;  // remember pointer to KAD Manager
   CTargetTree*     m_pTargetTree;  // pointer to the class holding the Target View
                                    // this class only exists if a project is opened
   CClassTree*      m_pClassTree;   // pointer to the class holding the Class View
                                    // this class only exists if a project is opened
   // SIS, 15.03.01:
   CInstanceTree*   m_pInstanceTree; // pointer to instance tree; is created on open project


   // m_listBackparser holds a list with all backparsers
   CTypedPtrList<CPtrList,TBackparserInfo*> m_listBackparser;   
   //  m_mapBackparser holds a map between file extensions and backparser infos
   CMap<CString, LPCTSTR, TBackparserInfo*, TBackparserInfo*> m_mapBackparser;

   // m_listContainer holds a list with all container
   CTypedPtrList<CPtrList,TContainerInfo*> m_listContainer;   
   //  m_mapBackparser holds a map between file extensions and backparser infos
   CMap<CString, LPCTSTR, TContainerInfo*, TContainerInfo*> m_mapContainer;


   CComBSTR m_sTargetId;  // the target id for reporting error messages to the message view

	
   HANDLE   m_hMutex;       // to avoid concurrency problems with multiple threads

   HINSTANCE m_hInstance;

   BOOL m_bReparseClassesReady;
   BOOL m_bReparseTargetsReady;
   //BOOL m_bReparseInstancesReady;
   CTime m_timeReparse;
};




//------------------------------------------------------------------*
// inline definitions
//


//------------------------------------------------------------------*
/**
 * GetProjectPath returns the absolute path of the project.
 *
 * @return          The absolute path to the project directory
 * @exception       -
 * @see             
*/
inline CString CCEProjectManager::GetProjectPath() 
    { return m_strProjectPath; }


//------------------------------------------------------------------*
/**
 * GetProjectName returns the name of the project.
 *
 * @return          The name of the project, this is the name of the 
 *                  project file without file extension
 * @exception       -
 * @see             
*/
inline CString CCEProjectManager::GetProjectName() 
    { return m_strProjectName; }


//------------------------------------------------------------------*
/**
 * GetProjectFileName returns the project file name without 
 * path information.
 *
 * @param           
 * @return          The file name of the project file with file extension
 *                  but without path information
 * @exception       -
 * @see             
*/
inline CString CCEProjectManager::GetProjectFileName() 
    { return m_strProjectFileName; }


//------------------------------------------------------------------*
/**
 * GetProjectFileNameWithPath returns the file name of the project file
 * with file extension and full path information.
 *
 * @return          The project file name with absolute path information
 * @exception       -
 * @see             
*/
inline CString CCEProjectManager::GetProjectFileNameWithPath()
    { return m_strProjectFileNameWithPath; }


//------------------------------------------------------------------*
/**
 * IsProjectLoaded returns the information if a project is opened
 * If no project is open, no class,targetTree are available, and the 
 * requests via an interface have to return an error
 *
 * @return          (open)status of the project
 * @exception       -
 * @see             
*/
inline BOOL CCEProjectManager::IsProjectLoaded() 
    { return m_bProjectLoaded; }


//------------------------------------------------------------------*
/**
 * GetTargetTree returns a pointer to the object holding the target tree.
 *
 * @return          Pointer to the class holding the target tree
                    NULL - if no project is opened
 * @exception       -
 * @see             
*/
inline CTargetTree* CCEProjectManager::GetTargetTree()
    { return m_pTargetTree; }


//------------------------------------------------------------------*
/**
 * GetClassTree returns a pointer to the object holding the class tree.
 *
 * @return          Pointer to the class holding the class tree
 *                  NULL - if no project is opened
 * @exception       -
 * @see             
*/
inline CClassTree* CCEProjectManager::GetClassTree()
    { return m_pClassTree; }


//------------------------------------------------------------------*
/**
 * GetInstanceTree returns a pointer to the object holding the instance tree.
 *
 * @return          Pointer to the class holding the instance tree
 *                  NULL - if no project is opened
 * @exception       -
 * @see             
*/
inline CInstanceTree* CCEProjectManager::GetInstanceTree()
    { return m_pInstanceTree; }



//------------------------------------------------------------------*
/**
 * GetProjectFileHandler returns a pointer to the object handling the
 * project file.
 *
 * @return          Pointer to the class capsulating the project file 
 *                  operations
 *                  NULL - if no project is opened
 * @exception       -
 * @see             
*/
inline CProjectFile* CCEProjectManager::GetProjectFileHandler()
	{ return m_pProjectFileHandler; }


//------------------------------------------------------------------*
/**
 * GetKadManager returns an interface pointer to the kad manager.
 *
 * @return          return interface pointer to the Kad Manager
 * @exception       -
 * @see             
*/
inline IUnknown* CCEProjectManager::GetKadManager()
    { return m_pKadManager; }




#endif //__CEPROJECTMANAGER_H_


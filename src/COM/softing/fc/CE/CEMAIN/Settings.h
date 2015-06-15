#if !defined(AFX_SETTINGS_H__A32033FC_8BA6_4CFA_837A_05F811534BB9__INCLUDED_)
#define AFX_SETTINGS_H__A32033FC_8BA6_4CFA_837A_05F811534BB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

#include "XMLHelper\XMLDocument.h"
#include "XMLHelper\XMLNode.h"

#include "BrkPtMan.h"
#include "CEWindowPlacement.h"
#include "PrintSettings.h"

#define CE_PSET_FILENAME           _T("CEPrjSettings.4cs")

#define CE_PSET_ELEM_WATCH         _T("WATCH")
#define CE_PSET_ELEM_PANE          _T("PANE")
#define CE_PSET_ATTR_PANENAME      _T("NAME")
#define CE_PSET_ATTR_ACTIVE_TAB    _T("ACTIVETAB")

#define CE_PSET_ELEM_PRJSETTINGS   _T("CEPRJSETTINGS")

#define CE_PSET_ELEM_WPLC          _T ("WINDOWPLACEMENT")

#define CE_PSET_GC_CONTAINER       _T ("CONTAINER")
#define CE_PSET_GC_ELEM_DOCUMENT   _T ("DOCUMENT")
#define CE_PSET_GC_ATTR_NAME       _T ("NAME")
#define CE_PSET_GC_ELEM_INSTANCE   _T ("INSTANCE")
#define CE_PSET_GC_ELEM_WPLIST     _T ("WPLIST")
#define CE_PSET_GC_ATTR_PATH       _T ("PATH")
#define CE_PSET_GC_ELEM_WP         _T ("WP")
#define CE_PSET_GC_ATTR_POSITION   _T ("POSITION")
#define CE_PSET_GC_ATTR_VISIBLE    _T ("VISIBLE")
#define CE_PSET_GC_ATTR_TABBED     _T ("TABBED")
#define CE_PSET_GC_ATTR_NUMVIS     _T ("NUMVIS")
#define CE_PSET_GC_ATTR_ZORDER     _T ("ZORDER")
#define CE_PSET_GC_ATTR_ACTINST    _T ("ACTINST")
#define CE_PSET_GC_ATTR_INDEX      _T ("INDEX")

#define CE_PSET_BPS_BPLIST         _T ("BPLIST")
#define CE_PSET_BPS_ELEM_BP        _T ("BP")
#define CE_PSET_BPS_ATTR_INSTANCE  _T ("INSTANCE")
#define CE_PSET_BPS_ATTR_QUALIFIER _T ("QUALIFIER")
#define CE_PSET_BPS_ATTR_X         _T ("X")
#define CE_PSET_BPS_ATTR_Y         _T ("Y")
#define CE_PSET_BPS_ATTR_CX        _T ("CX")
#define CE_PSET_BPS_ATTR_CY        _T ("CY")
#define CE_PSET_BPS_ATTR_STATE     _T ("STATE")


#define CE_PSET_DOCKWND_WINDOW              _T ("WINDOWS")
#define CE_PSET_DOCKING_ATTR_FULLSCREEN     _T ("FULLSCREEN")
#define CE_PSET_DOCKING_ATTR_VISFLAGS       _T ("VISFLAGS")
#define CE_PSET_DOCKWND_ELEM_NAVIGATOR      _T ("NAVWND")
#define CE_PSET_DOCKWND_ELEM_MSGWND         _T ("MSGWND")
#define CE_PSET_DOCKWND_ELEM_WATCHWND       _T ("WATCHWND")
#define CE_PSET_DOCKWND_ELEM_BPOINTWND      _T ("BPOINTWND")
#define CE_PSET_DOCKWND_ELEM_POUVARSELWND   _T ("POUVARSELWND")
#define CE_PSET_DOCKWND_ELEM_XREFWND        _T ("XREFWND")
#define CE_PSET_DOCKWND_ATTR_VISIBLE        _T ("VISIBLE")
#define CE_PSET_DOCKWND_ATTR_FLOAT          _T ("FLOAT")
#define CE_PSET_DOCKWND_ATTR_XPOS           _T ("XPOS")
#define CE_PSET_DOCKWND_ATTR_YPOS           _T ("YPOS")
#define CE_PSET_DOCKWND_ATTR_WIDTH          _T ("WIDTH")
#define CE_PSET_DOCKWND_ATTR_HEIGHT         _T ("HEIGHT")
#define CE_PSET_DOCKWND_ATTR_MRUDOCKID      _T ("MRUDOCKID")
#define CE_PSET_DOCKWND_ATTR_ROW            _T ("ROW")
#define CE_PSET_DOCKWND_ATTR_COLUMN         _T ("COLUMN")
#define CE_PSET_DOCKWND_ATTR_DOCK_PCT_WIDTH _T ("DOCK_PCT_WIDTH")
#define CE_PSET_DOCKWND_ATTR_DOCK_HEIGHT    _T ("DOCK_HEIGHT")
#define CE_PSET_DOCKWND_ATTR_MDI_FLOAT      _T ("MDI_FLOAT")
#define CE_PSET_DOCKWND_ATTR_ALLOW_DOCKING  _T ("ALLOW_DOCKING")


#define CE_PSET_LIB_LIBRARY                 _T("LIBRARY")
#define CE_PSET_LIB_NAME                    _T("NAME")
#define CE_PSET_LIB_PATH                    _T("PATH")
#define CE_PSET_LIB_STANDARD_PATH           _T("STANDARD_PATH")
#define CE_PSET_LIB_TARGET                  _T("TARGET")

// project list view
#define CE_PSET_ELEM_PROJECT_LIST_VIEW      _T("PLV")

// attribute names for windows placement structure
#define CE_PSET_WP_FLAGS            _T("WPFLAGS")
#define CE_PSET_WP_SHOW             _T("WPSHOW")
#define CE_PSET_WP_MINPOSX          _T("WPMINPOSX")
#define CE_PSET_WP_MINPOSY          _T("WPMINPOSY")
#define CE_PSET_WP_MAXPOSX          _T("WPMAXPOSX")
#define CE_PSET_WP_MAXPOSY          _T("WPMAXPOSY")
#define CE_PSET_WP_NORMPOSBOTTOM    _T("WPNORMPOSBOTTOM")
#define CE_PSET_WP_NORMPOSLEFT      _T("WPNORMPOSLEFT")
#define CE_PSET_WP_NORMPOSRIGHT     _T("WPNORMPOSRIGHT")
#define CE_PSET_WP_NORMPOSTOP       _T("WPNORMPOSTOP")

// attributes and elements for engineering settings
#define CE_PSET_ENG_ELEM            _T("ENGINEERING")
#define CE_PSET_ENG_XREF            _T("XREF")
#define CE_PSET_ENG_VARSEL          _T("VARSEL")



//------------------------------------------------------------------*
/**
 *  class CWatchSettings
 *
 *  @see    CWatchPaneSettings
*/
class CWatchSettings
{
public:
    CWatchSettings();
    ~CWatchSettings();

    void Save(CXMLNode& node, CXMLDocument& document);
    void Load(CXMLNode& node);

    int GetPaneCount();

    void AddPane(const CString& crstrName, const CString& crstrXMLString);
    BOOL GetPaneInfo(int iIndex, CString& rstrName, CString& rstrXMLString);

    int GetActiveTab();
    void SetActiveTab(int iActiveTab);

    void Reset();

protected:
    CStringArray    m_astrName;       // name of pane
    CStringArray    m_astrXMLString;  // xml string representation for panes
    int             m_iActiveTab;     // index of active tab
};

//------------------------------------------------------------------*
/**
 * DockToolSettings: the settings for one docking window
 *
 * @see             
*/
class DockToolSettings
{
public:
    DockToolSettings ();
    ~DockToolSettings ();
    
    void Save(CXMLNode& node);
    void Load(CXMLNode& node);

    bool  m_bFloating;
    bool  m_bVisible;
    CRect m_geometry;
    int   m_uiMRUDockId;
    int   m_iRow;
    int   m_iColumn;
    float m_fPctWidth;
    int   m_iHeight;
    BOOL  m_bMDIChild;
    BOOL  m_bAllowDocking;
    DWORD m_dwMRUDockingState;
};



//------------------------------------------------------------------*
/**
 * CDockToolsPosSettings: container for the settings for all
 * available docking windows
 *
 * @see             
*/
class CDockToolsPosSettings 
{
public:
	CDockToolsPosSettings();
    virtual ~CDockToolsPosSettings();

    void Init();
    void Save(CXMLNode& parentNode, CXMLDocument& document);
    void Load(CXMLNode& parentNode);

// Attribute
public:
    BOOL            m_bFullScreen;          // full screen mode?
    UINT            m_uiVisFlags;           // visibility flags.
    DockToolSettings m_NavigatorSettings;
    DockToolSettings m_WatchSettings;
    DockToolSettings m_MessageSettings;
    DockToolSettings m_BPointSettings;
    DockToolSettings m_PouVarSelSettings;	
    DockToolSettings m_XRefSettings;
};




//------------------------------------------------------------------*
/**
 * CLibrarySettings contains the library settings
 *
 * @see             
*/
class CLibrarySettings 
{
public:
	CLibrarySettings();
    ~CLibrarySettings();

    void Init();
    void Save(CXMLNode& parentNode, CXMLDocument& document);
    void Load(CXMLNode& parentNode);

// Attributes
public:
    CString m_strLibName;
    CString m_strLibPath;
    BOOL    m_bStandardLibPath;
    CString m_strTargetType;
};





//------------------------------------------------------------------*
/**
 * CEngineeringSettings
 *
 * @see             
*/
class CEngineeringSettings
{
public:
    CEngineeringSettings();
    ~CEngineeringSettings();

    void Init();
    void Save(CXMLNode& parentNode, CXMLDocument& document);
    void Load(CXMLNode& parentNode);

    void SaveSettings();

public:
    BOOL    m_bXRefDeactivated;
    BOOL    m_bConsoleInfDeactivated;
};



//------------------------------------------------------------------*
/**
 * CBreakpointSettings is responsible to load and store breakpoint settings.
 *
 * @see             
*/
class CBreakpointSettings
{
public:
    CBreakpointSettings();
    ~CBreakpointSettings();

    void Init();
    void Save(CXMLNode& parentNode, CXMLDocument& document);
    void Load(CXMLNode& parentNode);

protected:
    CComPtr<IBreakPoint> GetBreakpointManager();

};



//------------------------------------------------------------------*
/**
 *  class CProjectListViewSettings
 *
 *  @see    ....
*/
class CProjectListViewSettings
{
public:
    CProjectListViewSettings();
    ~CProjectListViewSettings();

    void Save(CXMLNode& parentNode, CXMLDocument& document);
    void Load(CXMLNode& parentNode);

public:
    CCEWindowPlacement     m_tWindowPlacement;
};


//------------------------------------------------------------------*
/**
 *  class CInstanceSettings
 *
 *  @see    ....
*/
class CInstanceSettings
{
public:
    CInstanceSettings(const CString& crstrInstPath);
    ~CInstanceSettings();

    CStringArray* GetWatchpoints();
    void AddWatchpoint(const CString& crstrPosition);

    int GetWatchpointCount();

    CString GetInstancePath();
    void SetInstancePath(const CString& crstrInstPath);

    void SetIndex(int iIndex);
    int GetIndex();

protected:
    CString         m_strInstPath;      // instance path
    CStringArray    m_astrWatchpoints;  // watch points
    int             m_iIndex;           // to restore original order
};


//------------------------------------------------------------------*

typedef CMap<CString, LPCTSTR, CInstanceSettings*, CInstanceSettings*> TMapInstSettings;


//------------------------------------------------------------------*
/**
 * CDocumentSettings
 *
 * @see             
*/
class CDocumentSettings
{
public:
    CDocumentSettings();
    ~CDocumentSettings();

    void Init();
    void Save(CXMLNode& parentNode, CXMLDocument& document);
    void Load(CXMLNode& node);

    BOOL IsWatchpointListEmpty();

    void SetFilePath(const CString& crstrFilePath);
    CString GetFilePath();

    BOOL IsDocumentVisible();
    void SetDocumentVisible(BOOL bVisible = TRUE);

    BOOL IsTabbed();
    void SetTabbed(BOOL bTabbed = TRUE);

    BOOL IsInstancePresent(const CString& crstrInstance);
    CInstanceSettings* FindInstance(const CString& crstrInstance, BOOL bCreateNew = TRUE);
    int GetInstances(CStringArray& rastrInstances);
    BOOL DeleteInstance(const CString& crstrInstance);
    void DeleteAllInstances();
    int GetNumberOfInstances();
    POSITION GetInstanceStartPosition();
    void GetNextInstance(POSITION& pos, CString& rstrInstPath, CInstanceSettings*& pInstSettings);

    CInstanceSettings* GetInstanceSettings(const CString& crstrInstance);
    CStringArray* GetWatchpoints(const CString& crstrInstance, BOOL bCreate);

    void SetWindowPlacement(WINDOWPLACEMENT& rwndPlc);
    BOOL GetWindowPlacement(WINDOWPLACEMENT& rwndPlc);
    void ResetWindowPlacement();

    void SetZOrder(int iZOrder);
    int GetZOrder();

    void SetActiveInstance(int iInstance);
    int GetActiveInstance();

private:
    CString             m_strFilePath;          // file path name
    BOOL                m_bVisible;             // is document visible
    BOOL                m_bTabbed;              // is document tabbed
    int                 m_iZOrder;              // z order of visible document
    TMapInstSettings    m_mapInstances;         // map instance path -> instance settings
    CCEWindowPlacement  m_tWindowPlacement;     // window placement
    int                 m_iActiveInstance;      // index of active instance
};


typedef CMap<CString, LPCTSTR, CDocumentSettings*, CDocumentSettings*> TMapDocumentSettings;

//------------------------------------------------------------------*
/**
 * CContainerSettings
 *
 * @see             
*/
class CContainerSettings
{
public:
    CContainerSettings();
    ~CContainerSettings();

    void Init();
    void Save(CXMLNode& parentNode, CXMLDocument& document);
    void Load(CXMLNode& parentNode);
    
    void UpdateSettings();

    int GetNumVisibleDocs();

    CDocumentSettings* FindDocument(const CString& crstrFileName, BOOL bCreateNew = TRUE);
    POSITION GetDocumentStartPosition();
    void GetNextDocument(POSITION& pos, CString& strFileName, CDocumentSettings*& pDocSettings);

    BOOL MayRemoveDocument(CDocumentSettings* pDocSettings);

    BOOL RemoveDocument(const CString& crstrFileName);

private:
     TMapDocumentSettings   m_mapDocuments;
     int                    m_iNumVisibleDocs;  // number of visible documents
    
};



//------------------------------------------------------------------*
/**
 * CSettings: container for all settings in the project
 *
 * @see             
*/
class CSettings  
{
public:
	CSettings();
	virtual ~CSettings();

    void Init();
    void Save(const CString& strProjPath);
    void Load(const CString& strProjPath);

    void ResetDocking();

    CDockToolsPosSettings       m_dockToolsPosSettings;
    CLibrarySettings            m_librarySettings;
    CBreakpointSettings         m_breakpointSettings;
    CContainerSettings          m_containerSettings;
    CProjectListViewSettings    m_projectListViewSettings;
    CEngineeringSettings        m_engineeringSettings;
    CWatchSettings              m_watchSettings;
    CPrintSettings              m_printSettings;
};

#endif // !defined(AFX_SETTINGS_H__A32033FC_8BA6_4CFA_837A_05F811534BB9__INCLUDED_)

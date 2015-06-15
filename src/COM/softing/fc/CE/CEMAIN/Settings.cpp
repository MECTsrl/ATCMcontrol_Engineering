




#include "stdafx.h"
#include "Settings.h"

#include "XMLHelper\XMLNodeList.h"

#include "CEProjInfo.h"
#include "CECompMan.h"
#include "CEFileHandler.h"
#include "cemain.h"


//////////////////////////////////////////////////////////////////////
// WatchSettings
//////////////////////////////////////////////////////////////////////

CWatchSettings::CWatchSettings()
{
    m_iActiveTab = 0;
}

CWatchSettings::~CWatchSettings()
{
}

//------------------------------------------------------------------*
/**
 * save watch settings in xml format.
 *
 * @param           [in] node: watch settings root node
 * @param           [in] document: settings xml document
 * @return          -
 * @exception       -
 * @see             -
*/
void CWatchSettings::Save(CXMLNode& node, CXMLDocument &document)
{
    int iSize = m_astrXMLString.GetSize();
    CString strName;
    CXMLNode    tNodeWatch;

    if(document.CreateNode(tNodeWatch, CE_PSET_ELEM_WATCH))
    {
        if(node.AppendChild(tNodeWatch))
        {
            CString strActiveTab;
            strActiveTab.Format("%d", m_iActiveTab);
            tNodeWatch.SetAttribute(CE_PSET_ATTR_ACTIVE_TAB, strActiveTab);
            for(int iIndex = 0; iIndex < iSize; ++iIndex)
            {
                CXMLDocument    tXMLDocTmp;
                CXMLNode        tNodePage;
                tXMLDocTmp.LoadXMLString(m_astrXMLString[iIndex]);
                if(tXMLDocTmp.GetRootNode(tNodePage))
                {
                    strName = m_astrName[iIndex];
                    if(!strName.IsEmpty())
                    {
                        tNodePage.SetAttribute(CE_PSET_ATTR_PANENAME, strName);
                    }
                    tNodeWatch.AppendChild(tNodePage);
                }
                else
                {
                    ASSERT(FALSE);  // page node could not be created
                }
            }
        }
    }
    else
    {
        ASSERT(FALSE);  // watch node could not be created
    }
}


//------------------------------------------------------------------*
/**
 * reset.
 *
 *  delete all panes.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CWatchSettings::Reset()
{
    m_astrName.RemoveAll();
    m_astrXMLString.RemoveAll();
}

//------------------------------------------------------------------*
/**
 * load watch settings in xml node.
 *
 * @param           [in] node: settings root node
 * @return          -
 * @exception       -
 * @see             -
*/
void CWatchSettings::Load(CXMLNode& rootNode)
{
    // run over all children
    CXMLNodeList    tNodeList;
    CXMLNode        tNodeWatch;
    CXMLNode        tNodePage;
    CString         strName;
    CString         strXMLString;

    if(rootNode.GetFirstChildWithTagName(CE_PSET_ELEM_WATCH, tNodeWatch))
    {
        CString strActiveTab;
        if(tNodeWatch.GetAttribute(CE_PSET_ATTR_ACTIVE_TAB, strActiveTab))
        {
            m_iActiveTab = atoi(strActiveTab);
        }
        tNodeWatch.GetChildNodeList(tNodeList);
        tNodeList.Reset();
        while(tNodeList.Next(tNodePage))
        {
            tNodePage.GetAttribute(CE_PSET_ATTR_PANENAME, strName);
            tNodePage.GetXMLString(strXMLString);
            if(!strXMLString.IsEmpty())
            {
                AddPane(strName, strXMLString);
            }
        }
    }
}

//------------------------------------------------------------------*
/**
 * get pane count.
 *
 * @param           -
 * @return          number of panes.
 * @exception       -
 * @see             -
*/
int CWatchSettings::GetPaneCount()
{
    return(m_astrXMLString.GetSize());
}

//------------------------------------------------------------------*
/**
 * add pane.
 *
 * @param           [in] crstrName: name of pane (may be empty if automatic pane)
 * @param           [in] crstrXMLString: xml string representation of pane node
 * @return          -
 * @exception       -
 * @see             GetPaneInfo()
*/
void CWatchSettings::AddPane(const CString& crstrName, const CString& crstrXMLString)
{
    m_astrName.Add(crstrName);
    m_astrXMLString.Add(crstrXMLString);
}

int CWatchSettings::GetActiveTab()
{
    return m_iActiveTab;
}

void CWatchSettings::SetActiveTab(int iActiveTab)
{
    m_iActiveTab = iActiveTab;
}


//------------------------------------------------------------------*
/**
 * get pane information.
 *
 * @param           [in] iIndex: index of pane.
 * @param           [in/out] rstrName: name of pane
 * @param           [in/out] rstrXMLString: string representation of node
 * @return          -
 * @exception       -
 * @see             -
*/
BOOL CWatchSettings::GetPaneInfo(int iIndex, CString& rstrName, CString& rstrXMLString)
{
    int iSize = m_astrXMLString.GetSize();
    if(iIndex < 0 || iIndex >= iSize)
    {
        return FALSE;
    }

    rstrName = m_astrName[iIndex];
    rstrXMLString = m_astrXMLString[iIndex];

    return TRUE;
}



//////////////////////////////////////////////////////////////////////
// DockToolSettings
//////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------*
/**
 * constructor
 *
 * @see             
*/
DockToolSettings::DockToolSettings ()
{
}



//------------------------------------------------------------------*
/**
 * destructor
 *
 * @see             
*/
DockToolSettings::~DockToolSettings()
{
}



//------------------------------------------------------------------*
/**
 * Save saves the settings for one docking window
 *
 * @param           node: the xml node in the settings tree 
 *                        for that docking window
 * @return          
 * @exception       -
 * @see             
*/
void DockToolSettings::Save(CXMLNode& node)
{
    CString strValue;
    strValue.Format("%i", m_geometry.left);
    node.SetAttribute(CE_PSET_DOCKWND_ATTR_XPOS, strValue);
    strValue.Format("%i", m_geometry.top);
    node.SetAttribute(CE_PSET_DOCKWND_ATTR_YPOS, strValue);
    strValue.Format("%i", m_geometry.right-m_geometry.left);
    node.SetAttribute(CE_PSET_DOCKWND_ATTR_WIDTH, strValue);
    strValue.Format("%i", m_geometry.bottom-m_geometry.top);
    node.SetAttribute(CE_PSET_DOCKWND_ATTR_HEIGHT, strValue);
    strValue = _T("0");
    if (m_bVisible)
    {
        strValue = _T("1");
    }
    node.SetAttribute(CE_PSET_DOCKWND_ATTR_VISIBLE, strValue);
    strValue = _T("0");
    if (m_bFloating)
    {
        strValue = _T("1");
    }
    node.SetAttribute(CE_PSET_DOCKWND_ATTR_FLOAT, strValue);
    strValue.Format("%i", m_uiMRUDockId);
    node.SetAttribute(CE_PSET_DOCKWND_ATTR_MRUDOCKID, strValue);
    strValue.Format("%i", m_iRow);
    node.SetAttribute(CE_PSET_DOCKWND_ATTR_ROW, strValue);
    strValue.Format("%i", m_iColumn);
    node.SetAttribute(CE_PSET_DOCKWND_ATTR_COLUMN, strValue);
    strValue.Format("%i", (int)(m_fPctWidth*100.0f));
    node.SetAttribute(CE_PSET_DOCKWND_ATTR_DOCK_PCT_WIDTH, strValue);
    strValue.Format("%i", m_iHeight);
    node.SetAttribute(CE_PSET_DOCKWND_ATTR_DOCK_HEIGHT, strValue);
    strValue = _T("0");
    if (m_bMDIChild)
    {
        strValue = _T("1");
    }
    node.SetAttribute(CE_PSET_DOCKWND_ATTR_MDI_FLOAT, strValue);
    strValue = _T("0");
    if (m_bAllowDocking)
    {
        strValue = _T("1");
    }
    node.SetAttribute(CE_PSET_DOCKWND_ATTR_ALLOW_DOCKING, strValue);
}



//------------------------------------------------------------------*
/**
 * Load loads the settings for one docking window
 *
 * @param           node: the node in the xml tree for that window
 * @return          
 * @exception       -
 * @see             
*/
void DockToolSettings::Load(CXMLNode& node)
{
    CString strValue;
    BOOL ret;
    int iVal;
    
    ret = node.GetAttribute(CE_PSET_DOCKWND_ATTR_XPOS, strValue);
    if(ret)
    {
        iVal = _ttoi(strValue);
        m_geometry.left = iVal;
    }

    ret = node.GetAttribute(CE_PSET_DOCKWND_ATTR_YPOS, strValue);
    if(ret)
    {
        iVal = _ttoi(strValue);
        m_geometry.top = iVal;
    }

    ret = node.GetAttribute(CE_PSET_DOCKWND_ATTR_WIDTH, strValue);
    if(ret)
    {
        iVal = _ttoi(strValue);
        m_geometry.right = m_geometry.left + iVal;
    }

    ret = node.GetAttribute(CE_PSET_DOCKWND_ATTR_HEIGHT, strValue);
    if(ret)
    {
        iVal = _ttoi(strValue);
        m_geometry.bottom = m_geometry.top + iVal;
    }

    ret = node.GetAttribute(CE_PSET_DOCKWND_ATTR_VISIBLE, strValue);
    if(ret)
    {
        if (strValue.Compare(_T("0"))==0)
        {
            m_bVisible = FALSE;
        }
        else
        {
            m_bVisible = TRUE;
        }
    }

    ret = node.GetAttribute(CE_PSET_DOCKWND_ATTR_FLOAT, strValue);
    if(ret)
    {
        if (strValue.Compare(_T("0"))==0)
        {
            m_bFloating = FALSE;
        }
        else
        {
            m_bFloating = TRUE;
        }
    }

    ret = node.GetAttribute(CE_PSET_DOCKWND_ATTR_MRUDOCKID, strValue);
    if(ret)
    {
        iVal = _ttoi(strValue);
        m_uiMRUDockId = iVal;
    }

    ret = node.GetAttribute(CE_PSET_DOCKWND_ATTR_ROW, strValue);
    if(ret)
    {
        iVal = _ttoi(strValue);
        m_iRow = iVal;
    }

    ret = node.GetAttribute(CE_PSET_DOCKWND_ATTR_COLUMN, strValue);
    if(ret)
    {
        iVal = _ttoi(strValue);
        m_iColumn = iVal;
    }

    ret = node.GetAttribute(CE_PSET_DOCKWND_ATTR_DOCK_PCT_WIDTH, strValue);
    if(ret)
    {
        iVal = _ttoi(strValue);
        m_fPctWidth = ((float)iVal) / 100.0f;
    }

    ret = node.GetAttribute(CE_PSET_DOCKWND_ATTR_DOCK_HEIGHT, strValue);
    if(ret)
    {
        iVal = _ttoi(strValue);
        m_iHeight = iVal;
    }

    ret = node.GetAttribute(CE_PSET_DOCKWND_ATTR_MDI_FLOAT, strValue);
    if(ret)
    {
        if (strValue.Compare(_T("0"))==0)
        {
            m_bMDIChild = FALSE;
        }
        else
        {
            m_bMDIChild = TRUE;
        }
    }

    ret = node.GetAttribute(CE_PSET_DOCKWND_ATTR_ALLOW_DOCKING, strValue);
    if(ret)
    {
        if (strValue.Compare(_T("0"))==0)
        {
            m_bAllowDocking = FALSE;
        }
        else
        {
            m_bAllowDocking = TRUE;
        }
    }

}



//////////////////////////////////////////////////////////////////////
// CDockToolsPosSettings
//////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------*
/**
 * constructor
 *
 * @see             
*/
CDockToolsPosSettings::CDockToolsPosSettings()
{
}


//------------------------------------------------------------------*
/**
 * destructor
 *
 * @see             
*/
CDockToolsPosSettings::~CDockToolsPosSettings()
{
}



//------------------------------------------------------------------*
/**
 * Init initializes the settings of all docking windows
 *
 * @see             
*/
void CDockToolsPosSettings::Init()
{
    m_bFullScreen = FALSE;
    m_uiVisFlags = 3;           // navigator and message view

    CRect rectNav(0,0,300,400);
    m_NavigatorSettings.m_bVisible = TRUE;
    m_NavigatorSettings.m_bFloating = FALSE;
    m_NavigatorSettings.m_uiMRUDockId = AFX_IDW_DOCKBAR_LEFT;
    m_NavigatorSettings.m_geometry = rectNav;
    m_NavigatorSettings.m_iRow = 0;
    m_NavigatorSettings.m_iColumn = 0;
    m_NavigatorSettings.m_fPctWidth = 1.0;
    m_NavigatorSettings.m_iHeight = 300;
    m_NavigatorSettings.m_bMDIChild = FALSE;
    m_NavigatorSettings.m_bAllowDocking = TRUE;
    m_NavigatorSettings.m_dwMRUDockingState = CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT;

    CRect rectPOU(250-250,100,550+10-250,500+140); //EF increase width by 10 hight by 140, xpos -250
    m_PouVarSelSettings.m_bVisible = FALSE;	
    m_PouVarSelSettings.m_bFloating = TRUE;
    m_PouVarSelSettings.m_uiMRUDockId = AFX_IDW_DOCKBAR_LEFT;
    m_PouVarSelSettings.m_geometry = rectPOU;
    m_PouVarSelSettings.m_iRow = 1;
    m_PouVarSelSettings.m_iColumn = 0;
    m_PouVarSelSettings.m_fPctWidth = 1.0;
    m_PouVarSelSettings.m_iHeight = 300;
    m_PouVarSelSettings.m_bMDIChild = FALSE;
    m_PouVarSelSettings.m_bAllowDocking = FALSE;
    m_PouVarSelSettings.m_dwMRUDockingState = CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT;

    m_XRefSettings.m_bVisible = FALSE;	
    m_XRefSettings.m_bFloating = FALSE;
    m_XRefSettings.m_uiMRUDockId = AFX_IDW_DOCKBAR_LEFT;
    m_XRefSettings.m_geometry = rectNav;
    m_XRefSettings.m_iRow = 1;
    m_XRefSettings.m_iColumn = 0;
    m_XRefSettings.m_fPctWidth = 1.0;
    m_XRefSettings.m_iHeight = 300;
    m_XRefSettings.m_bMDIChild = FALSE;
    m_XRefSettings.m_bAllowDocking = TRUE;
    m_XRefSettings.m_dwMRUDockingState = CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT;

    CRect rectMsg(0,0,400,150);
    m_MessageSettings.m_bVisible = TRUE;
    m_MessageSettings.m_bFloating = FALSE;
    m_MessageSettings.m_uiMRUDockId = AFX_IDW_DOCKBAR_BOTTOM;
    m_MessageSettings.m_geometry = rectMsg;
    m_MessageSettings.m_iRow = 1;
    m_MessageSettings.m_iColumn = 0;
    m_MessageSettings.m_fPctWidth = 1.0;
    m_MessageSettings.m_iHeight = 150;
    m_MessageSettings.m_bMDIChild = FALSE;
    m_MessageSettings.m_bAllowDocking = TRUE;
    m_MessageSettings.m_dwMRUDockingState = CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM;

    m_WatchSettings.m_bVisible = FALSE;
    m_WatchSettings.m_bFloating = FALSE;
    m_WatchSettings.m_uiMRUDockId = AFX_IDW_DOCKBAR_BOTTOM;
    m_WatchSettings.m_geometry = rectMsg;
    m_WatchSettings.m_iRow = 0;
    m_WatchSettings.m_iColumn = 0;
    m_WatchSettings.m_fPctWidth = 1.0;
    m_WatchSettings.m_iHeight = 150;
    m_WatchSettings.m_bMDIChild = FALSE;
    m_WatchSettings.m_bAllowDocking = TRUE;
    m_WatchSettings.m_dwMRUDockingState = CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM;

    m_BPointSettings.m_bVisible = FALSE;
    m_BPointSettings.m_bFloating = FALSE;
    m_BPointSettings.m_uiMRUDockId = AFX_IDW_DOCKBAR_BOTTOM;
    m_BPointSettings.m_geometry = rectMsg;
    m_BPointSettings.m_iRow = 1;
    m_BPointSettings.m_iColumn = 1;
    m_BPointSettings.m_fPctWidth = 1.0;
    m_BPointSettings.m_iHeight = 150;
    m_BPointSettings.m_bMDIChild = FALSE;
    m_BPointSettings.m_bAllowDocking = TRUE;
    m_BPointSettings.m_dwMRUDockingState = CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM;
}



//------------------------------------------------------------------*
/**
 * Save: save the docking window settings
 *
 * @param           parentNode: the node in the xml tree under which the 
 *                              docking window settings node should be inserted
 * @param           document: the xml document, needed to create new nodes
 * @return          
 * @exception       -
 * @see             
*/
void CDockToolsPosSettings::Save(CXMLNode& parentNode, CXMLDocument& document)
{
    CXMLNode nodeNav;
    CXMLNode nodeWatch;
    CXMLNode nodeMsg;
    CXMLNode nodeBP;
    CXMLNode nodePou;
    CXMLNode nodeXRef;


    CXMLNode nodeWindows;
    CString  strTmp;

    document.CreateNode(nodeWindows, CE_PSET_DOCKWND_WINDOW);
    strTmp.Format("%d", m_bFullScreen);
    nodeWindows.SetAttribute(CE_PSET_DOCKING_ATTR_FULLSCREEN, strTmp);
    strTmp.Format("%d", m_uiVisFlags);
    nodeWindows.SetAttribute(CE_PSET_DOCKING_ATTR_VISFLAGS, strTmp);
    parentNode.AppendChild(nodeWindows);
    
    document.CreateNode(nodeNav, CE_PSET_DOCKWND_ELEM_NAVIGATOR);
    m_NavigatorSettings.Save(nodeNav);
    nodeWindows.AppendChild(nodeNav);

    document.CreateNode(nodeWatch, CE_PSET_DOCKWND_ELEM_WATCHWND);
    m_WatchSettings.Save(nodeWatch);
    nodeWindows.AppendChild(nodeWatch);

    document.CreateNode(nodeMsg, CE_PSET_DOCKWND_ELEM_MSGWND);
    m_MessageSettings.Save(nodeMsg);
    nodeWindows.AppendChild(nodeMsg);

    document.CreateNode(nodeBP, CE_PSET_DOCKWND_ELEM_BPOINTWND);
    m_BPointSettings.Save(nodeBP);
    nodeWindows.AppendChild(nodeBP);

    document.CreateNode(nodePou, CE_PSET_DOCKWND_ELEM_POUVARSELWND);
    m_PouVarSelSettings.Save(nodePou);
    nodeWindows.AppendChild(nodePou);

    document.CreateNode(nodeXRef, CE_PSET_DOCKWND_ELEM_XREFWND);
    m_XRefSettings.Save(nodeXRef);
    nodeWindows.AppendChild(nodeXRef);

}



//------------------------------------------------------------------*
/**
 * Load: load the settings for all docking windows
 *
 * @param           parentNode: the root node in the xml settings tree
 * @return          
 * @exception       -
 * @see             
*/
void CDockToolsPosSettings::Load(CXMLNode& parentNode)
{
    CXMLNode nodeWindows;
    CXMLNodeList nodeList;
    BOOL bFound = FALSE;

    // search windows node
    parentNode.GetChildNodeList(nodeList);
    nodeList.Reset();
    while(nodeList.Next(nodeWindows))
    {
        CString strTag;
        if (nodeWindows.GetTagName(strTag))
        {
            if (strTag.Compare(CE_PSET_DOCKWND_WINDOW)==0)
            {
                bFound = TRUE;
                break;
            }
        }
    }

    if (!bFound)
    {
        return;
    }

    // get fullscreen modus and visibility flags
    CString strTmp;
    if(nodeWindows.GetAttribute(CE_PSET_DOCKING_ATTR_FULLSCREEN, strTmp))
    {
        m_bFullScreen = (BOOL)atoi(strTmp);
    }
    if(nodeWindows.GetAttribute(CE_PSET_DOCKING_ATTR_VISFLAGS, strTmp))
    {
        m_uiVisFlags = (UINT)atoi(strTmp);
    }


    // look at each subnode and fill the corresponding settings
    CXMLNode childNode;
    nodeWindows.GetChildNodeList(nodeList);
    nodeList.Reset();
    while(nodeList.Next(childNode))
    {
        CString strTag;
        if (childNode.GetTagName(strTag))
        {
            if (strTag.Compare(CE_PSET_DOCKWND_ELEM_NAVIGATOR)==0)
            {
                m_NavigatorSettings.Load(childNode);
            }
            else if (strTag.Compare(CE_PSET_DOCKWND_ELEM_WATCHWND)==0)
            {
                m_WatchSettings.Load(childNode);
            }
            else if (strTag.Compare(CE_PSET_DOCKWND_ELEM_MSGWND)==0)
            {
                m_MessageSettings.Load(childNode);
            }
            else if (strTag.Compare(CE_PSET_DOCKWND_ELEM_BPOINTWND)==0)
            {
                m_BPointSettings.Load(childNode);
            }
            else if (strTag.Compare(CE_PSET_DOCKWND_ELEM_POUVARSELWND)==0)
            {
                m_PouVarSelSettings.Load(childNode);
            }
            else if (strTag.Compare(CE_PSET_DOCKWND_ELEM_XREFWND)==0)
            {
                m_XRefSettings.Load(childNode);
            }
        }
    }

}

//////////////////////////////////////////////////////////////////////
// CLibrarySettings
//////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------*
/**
 * constructor
 *
 * @see             
*/
CLibrarySettings::CLibrarySettings()
{
}


//------------------------------------------------------------------*
/**
 * destructor
 *
 * @see             
*/
CLibrarySettings::~CLibrarySettings()
{
}


//------------------------------------------------------------------*
/**
 * Init inits the library settings
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CLibrarySettings::Init()
{
    HRESULT hr;
    CComQIPtr<ICEProjInfo> pProjInfo;

    IUnknown* pUnk = NULL;
    pUnk = CEGetCOM(CE_COMPMAN_PRJMAN);
    if (!pUnk)
    {
        return;
    }

    pProjInfo = pUnk;
    pUnk->Release();

    // init lib name = project name
    CComBSTR sProjName;
    hr = pProjInfo->getProjectName(&sProjName);
    if (hr==S_OK)
    {
        m_strLibName = sProjName;
    }

    // get standard library path
    m_bStandardLibPath = FALSE;
    CComBSTR sLibPath;
    hr = pProjInfo->getLibraryPath(&sLibPath);
    if (hr==S_OK)
    {
        m_bStandardLibPath = TRUE;
        m_strLibPath = sLibPath;
    }

    // selected target is the first availabe target type,
    // if there is only one target installed, that type will be selected
    SAFEARRAY* psaTargets;
    hr = pProjInfo->getAllTargetTypes(&psaTargets);
    if (hr==S_OK)
    {
        long    lTargetCount;

        ::SafeArrayGetUBound(psaTargets, 0, &lTargetCount);

        if(lTargetCount == 1)
        {
            BSTR HUGEP *pbstr;
            BSTR bstr;

            hr = ::SafeArrayAccessData(psaTargets, (void HUGEP**)&pbstr);
            ASSERT(hr == S_OK);
            if (!FAILED(hr))
            {
                bstr = pbstr[0];
                m_strTargetType = bstr;            
            }

            ::SafeArrayUnaccessData(psaTargets);		
        }
        ::SafeArrayDestroy(psaTargets);
    }
}



//------------------------------------------------------------------*
/**
 * Load loads the library settings
 *
 * @param           parentNode: the root node of the xml settings
 * @return          
 * @exception       -
 * @see             
*/
void CLibrarySettings::Load(CXMLNode &parentNode)
{
    CXMLNode nodeLibrary;
    CXMLNodeList nodeList;
    BOOL bFound = FALSE;

    parentNode.GetChildNodeList(nodeList);
    nodeList.Reset();
    while(nodeList.Next(nodeLibrary))
    {
        CString strTag;
        if (nodeLibrary.GetTagName(strTag))
        {
            if (strTag.Compare(CE_PSET_LIB_LIBRARY)==0)
            {
                bFound = TRUE;
                break;
            }
        }
    }

    if (!bFound)
    {
        return;
    }

    CString strValue;
    BOOL ret;
    
    ret = nodeLibrary.GetAttribute(CE_PSET_LIB_NAME, strValue);
    if(ret)
    {
        m_strLibName = strValue;
    }
    
    ret = nodeLibrary.GetAttribute(CE_PSET_LIB_PATH, strValue);
    if(ret)
    {
        m_strLibPath = strValue;
    }
    
    ret = nodeLibrary.GetAttribute(CE_PSET_LIB_STANDARD_PATH, strValue);
    if(ret)
    {
        if (strValue.Compare(_T("0"))==0)
        {
            m_bStandardLibPath = FALSE;
        }
        else
        {
            m_bStandardLibPath = TRUE;
        }
    }
    
    ret = nodeLibrary.GetAttribute(CE_PSET_LIB_TARGET, strValue);
    if(ret)
    {
        m_strTargetType = strValue;
    }
}



//------------------------------------------------------------------*
/**
 * Save saves the library settings
 *
 * @param           parentNode: the root node of the settings tree
 * @param           document: the xml document
 * @return          
 * @exception       -
 * @see             
*/
void CLibrarySettings::Save(CXMLNode &parentNode, CXMLDocument &document)
{
    CXMLNode nodeLibrary;
    
    document.CreateNode(nodeLibrary, CE_PSET_LIB_LIBRARY);

    nodeLibrary.SetAttribute(CE_PSET_LIB_NAME, m_strLibName);
    nodeLibrary.SetAttribute(CE_PSET_LIB_PATH, m_strLibPath);

    if (m_bStandardLibPath)
    {
        nodeLibrary.SetAttribute(CE_PSET_LIB_STANDARD_PATH, _T("1"));
    }
    else
    {
        nodeLibrary.SetAttribute(CE_PSET_LIB_STANDARD_PATH, _T("0"));
    }

    nodeLibrary.SetAttribute(CE_PSET_LIB_TARGET, m_strTargetType);

    parentNode.AppendChild(nodeLibrary);
}





//////////////////////////////////////////////////////////////////////
// CEngineeringSettings
//////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------*
/**
 * constructor
 *
 * @see             
*/
CEngineeringSettings::CEngineeringSettings()
{
    Init();
}


//------------------------------------------------------------------*
/**
 * destructor
 *
 * @see             
*/
CEngineeringSettings::~CEngineeringSettings()
{
}


//------------------------------------------------------------------*
/**
 * Init inits the engineering settings
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CEngineeringSettings::Init()
{
    m_bXRefDeactivated = FALSE;
    m_bConsoleInfDeactivated = TRUE;
}



//------------------------------------------------------------------*
/**
 * Load loads the engineering settings
 *
 * @param           parentNode: the root node of the xml settings
 * @return          
 * @exception       -
 * @see             
*/
void CEngineeringSettings::Load(CXMLNode &parentNode)
{
    Init();

    // XRef settings from 4cp file
    CComQIPtr<ICEProjInfo>  pICEProjectInfo;
    CComPtr<IUnknown>       pTmpInfo;
    HRESULT hr;

    pTmpInfo.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pICEProjectInfo = pTmpInfo;
    ASSERT(pICEProjectInfo!=NULL);
    if(pICEProjectInfo!=NULL)
    {
        CComBSTR sSection = _T("Compiler");
        CComBSTR sKey = _T("opt");
        SAFEARRAY* parrKeys;
        
        hr = pICEProjectInfo->CPGetAllValuesForKeyFromSection(sSection, sKey, &parrKeys);
        if (hr==S_OK)
        {
            BSTR HUGEP *pbstr;
            BSTR bstr;
            unsigned long i;

            // Get a pointer to the elements of the array.
            hr = ::SafeArrayAccessData(parrKeys, (void HUGEP**)&pbstr);
            ASSERT(hr == S_OK);
            if (hr==S_OK)
            {
                for (i = 0; i < ((parrKeys)->rgsabound[0]).cElements; i++)
                {
                    CString str;
                    bstr = pbstr[i];
                    str = bstr;
                    if (str.CompareNoCase(_T("xref:0"))==0)
                    {
                        m_bXRefDeactivated = TRUE;
                    }
                    else if (str.CompareNoCase(_T("xref:1"))==0)
                    {
                        m_bXRefDeactivated = FALSE;
                    }
                    else if (str.CompareNoCase(_T("console:0")) == 0)
                    {
                        m_bConsoleInfDeactivated = TRUE;
                    }
                    else if (str.CompareNoCase(_T("console:1")) == 0)
                    {
                        m_bConsoleInfDeactivated = FALSE;
                    }
                }
            }
            ::SafeArrayUnaccessData(parrKeys);		
            ::SafeArrayDestroy(parrKeys);
        }
    }
}



//------------------------------------------------------------------*
/**
 * Save saves the engineering settings
 *
 * @param           parentNode: the root node of the settings tree
 * @param           document: the xml document
 * @return          
 * @exception       -
 * @see             
*/
void CEngineeringSettings::Save(CXMLNode &parentNode, CXMLDocument &document)
{

}




void CEngineeringSettings::SaveSettings()
{
    CComQIPtr<ICEProjInfo>  pICEProjectInfo;
    CComPtr<IUnknown>       pTmpInfo;
    HRESULT hr;

    pTmpInfo.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pICEProjectInfo = pTmpInfo;
    ASSERT(pICEProjectInfo!=NULL);
    if(pICEProjectInfo!=NULL)
    { 
        CComBSTR sSection = _T("Compiler");
        CComBSTR sKey = _T("opt");
        CComBSTR sXrefValue0 = _T("xref:0");
        CComBSTR sXrefValue1 = _T("xref:1");
		CComBSTR sConsoleValue0 = _T("console:0");
        CComBSTR sConsoleValue1 = _T("console:1");


        hr = pICEProjectInfo->CPDeleteKeyWithValueFromSection(sSection, sKey, sXrefValue0);
        hr = pICEProjectInfo->CPDeleteKeyWithValueFromSection(sSection, sKey, sXrefValue1);

		hr = pICEProjectInfo->CPDeleteKeyWithValueFromSection(sSection, sKey, sConsoleValue0);
        hr = pICEProjectInfo->CPDeleteKeyWithValueFromSection(sSection, sKey, sConsoleValue1);

        if (m_bXRefDeactivated)
        {
            hr = pICEProjectInfo->CPInsertToSection(sSection, sKey, sXrefValue0);
        }
        else
        {
            hr = pICEProjectInfo->CPInsertToSection(sSection, sKey, sXrefValue1);
        }

		if(m_bConsoleInfDeactivated)
		{
			hr = pICEProjectInfo->CPInsertToSection(sSection, sKey, sConsoleValue0);
		}
		else
		{
			hr = pICEProjectInfo->CPInsertToSection(sSection, sKey, sConsoleValue1);
		}
    }
}




//////////////////////////////////////////////////////////////////////
// CBreakpointSettings
//////////////////////////////////////////////////////////////////////



//------------------------------------------------------------------*
/**
 * constructore
 *
 * @see             
*/
CBreakpointSettings::CBreakpointSettings()
{
}


//------------------------------------------------------------------*
/**
 * destructor
 *
 * @see             
*/
CBreakpointSettings::~CBreakpointSettings()
{
}


//------------------------------------------------------------------*
/**
 * Init
 *
 * @see             
*/
void CBreakpointSettings::Init()
{
}


//------------------------------------------------------------------*
/**
 * Save: save breakpoint settings.
 *
 * @param           parentNode: the root node in the settings xml
 * @param           document: the xml document, is needed to create new nodes
 * @return          
 * @exception       -
 * @see             
*/
void CBreakpointSettings::Save(CXMLNode& parentNode, CXMLDocument& document)
{
    // create bp list node
    CXMLNode nodeBPList;
    
    document.CreateNode(nodeBPList, CE_PSET_BPS_BPLIST);
    parentNode.AppendChild(nodeBPList);

    // get bp manager and breakpoints
    CComPtr<IBreakPoint> pIBreakPoint;
    HRESULT hr;
    SAFEARRAY *pSafeArray=NULL;
    long lLowerBound;
    long lUpperBound;
    long lIndex;
    

    pIBreakPoint = GetBreakpointManager();
    if (pIBreakPoint == NULL)
    {
        return;
    }

    hr = pIBreakPoint->GetBPs (&pSafeArray);
    if (FAILED (hr))
    {
        return;
    }

    ::SafeArrayGetLBound (pSafeArray, 1, &lLowerBound);
    ::SafeArrayGetUBound (pSafeArray, 1, &lUpperBound);

    // for each breakpoint create a new node
    for (lIndex = lLowerBound; lIndex <= lUpperBound; ++lIndex)
    {
        CComPtr<IDispatch> pDispatch;
        CComQIPtr<IBreakPointInfo, &IID_IBreakPointInfo> pBPInfo;
        BSTR sInstance=NULL;
        CString strInstance;
        EDP_POS_QUALIFIER posQual;
        long lX;
        long lY;
        long lCX;
        long lCY;
        BOOL bActive;

        // get info for breakpoints
        ::SafeArrayGetElement (pSafeArray, &lIndex, (void *)&pDispatch);
        pBPInfo = pDispatch;

        hr = pBPInfo->get_Instance(&sInstance);
        if (FAILED (hr))
        {
            continue;
        }
        strInstance = sInstance;
        ::SysFreeString (sInstance);

        hr = pBPInfo->get_PosQualifier(&posQual);
        if (FAILED (hr))
        {
            continue;
        }

        hr = pBPInfo->get_X(&lX);
        if (FAILED (hr))
        {
            continue;
        }

        hr = pBPInfo->get_Y(&lY);
        if (FAILED (hr))
        {
            continue;
        }

        hr = pBPInfo->get_CX(&lCX);
        if (FAILED (hr))
        {
            continue;
        }

        hr = pBPInfo->get_CY(&lCY);
        if (FAILED (hr))
        {
            continue;
        }

        hr = pBPInfo->get_Active(&bActive);
        if (FAILED (hr))
        {
            continue;
        }

        // create a new node
        CXMLNode nodeBP;
        document.CreateNode(nodeBP, CE_PSET_BPS_ELEM_BP);
        nodeBPList.AppendChild(nodeBP);

        // set attributes
        CString strValue;
        nodeBP.SetAttribute(CE_PSET_BPS_ATTR_INSTANCE, strInstance);
        strValue.Format(_T("%li"), (long)posQual);
        nodeBP.SetAttribute(CE_PSET_BPS_ATTR_QUALIFIER, strValue);
        strValue.Format(_T("%li"), lX);
        nodeBP.SetAttribute(CE_PSET_BPS_ATTR_X, strValue);
        strValue.Format(_T("%li"), lY);
        nodeBP.SetAttribute(CE_PSET_BPS_ATTR_Y, strValue);
        strValue.Format(_T("%li"), lCX);
        nodeBP.SetAttribute(CE_PSET_BPS_ATTR_CX, strValue);
        strValue.Format(_T("%li"), lCY);
        nodeBP.SetAttribute(CE_PSET_BPS_ATTR_CY, strValue);
        if (bActive)
        {
            nodeBP.SetAttribute(CE_PSET_BPS_ATTR_STATE, _T("1"));
        }
        else
        {
            nodeBP.SetAttribute(CE_PSET_BPS_ATTR_STATE, _T("0"));
        }
    }

    ::SafeArrayDestroy (pSafeArray);
}


//------------------------------------------------------------------*
/**
 * Load: load breakpoint settings.
 *
 * @param           parentNode: the root node in the settings xml
 * @return          
 * @exception       -
 * @see             
*/
void CBreakpointSettings::Load(CXMLNode& parentNode)
{
    CXMLNode nodeBPList;
    CXMLNodeList nodeList;
    BOOL bFound = FALSE;

    parentNode.GetChildNodeList(nodeList);
    nodeList.Reset();
    while(nodeList.Next(nodeBPList))
    {
        CString strTag;
        if (nodeBPList.GetTagName(strTag))
        {
            if (strTag.Compare(CE_PSET_BPS_BPLIST)==0)
            {
                bFound = TRUE;
                break;
            }
        }
    }

    if (!bFound)
    {
        return;
    }


    CComPtr<IBreakPoint> pIBreakPoint;

    pIBreakPoint = GetBreakpointManager();
    if (pIBreakPoint == NULL)
    {
        return;
    }


    CXMLNode nodeBP;
    nodeBPList.GetChildNodeList(nodeList);
    nodeList.Reset();
    while(nodeList.Next(nodeBP))
    {
        // check node tag
        CString strTag;
        if (nodeBP.GetTagName(strTag))
        {
            if (strTag.Compare(CE_PSET_BPS_ELEM_BP)==0)
            {
                CString strInstance;
                long lQualifier;
                long lX;
                long lY;
                long lCX;
                long lCY;
                BOOL bState;
                CString strValue;

                if (!nodeBP.GetAttribute(CE_PSET_BPS_ATTR_INSTANCE, strInstance))
                {
                    continue;
                }

                if (!nodeBP.GetAttribute(CE_PSET_BPS_ATTR_QUALIFIER, strValue))
                {
                    continue;
                }
                lQualifier = _ttol(strValue);

                if (!nodeBP.GetAttribute(CE_PSET_BPS_ATTR_X, strValue))
                {
                    continue;
                }
                lX = _ttol(strValue);

                if (!nodeBP.GetAttribute(CE_PSET_BPS_ATTR_Y, strValue))
                {
                    continue;
                }
                lY = _ttol(strValue);


                if (!nodeBP.GetAttribute(CE_PSET_BPS_ATTR_CX, strValue))
                {
                    continue;
                }
                lCX = _ttol(strValue);

                if (!nodeBP.GetAttribute(CE_PSET_BPS_ATTR_CY, strValue))
                {
                    continue;
                }
                lCY = _ttol(strValue);



                if (!nodeBP.GetAttribute(CE_PSET_BPS_ATTR_STATE, strValue))
                {
                    continue;
                }
                if (strValue.Compare(_T("0"))==0)
                {
                    bState = FALSE;
                }
                else
                {
                    bState = TRUE;
                }

                pIBreakPoint->SetBP(CComBSTR (strInstance), 
                                    (EDP_POS_QUALIFIER)lQualifier, 
                                    lX, lY, lCX, lCY, bState);
            }
        }
    }
}



//------------------------------------------------------------------*
/**
 * GetBreakpointManager
 *
 * @return          pointer to breakpoint manager
 * @exception       -
 * @see             
*/
CComPtr<IBreakPoint> CBreakpointSettings::GetBreakpointManager ()
{
    HRESULT hr;
    CComPtr<IUnknown> pUnknown;
    CComPtr<IBreakPoint> pBreakpointManager;

    pUnknown.Attach(CEGetCOM(CE_COMPMAN_BREAKPTMAN, false));
    if (pUnknown == NULL)
    {
        return (NULL);
    }

    hr = pUnknown->QueryInterface (IID_IBreakPoint, (LPVOID *)&pBreakpointManager);
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        return (NULL);
    }

    return (pBreakpointManager);
}

//////////////////////////////////////////////////////////////////////
// CInstanceSettings
//////////////////////////////////////////////////////////////////////


CInstanceSettings::CInstanceSettings(const CString& crstrInstPath)
{
    m_strInstPath = crstrInstPath;
    m_iIndex = -1;
}

CInstanceSettings::~CInstanceSettings()
{
}

CStringArray* CInstanceSettings::GetWatchpoints()
{
    return &m_astrWatchpoints;
}

CString CInstanceSettings::GetInstancePath()
{
    return m_strInstPath;
}

void CInstanceSettings::SetInstancePath(const CString& crstrInstPath)
{
    m_strInstPath = crstrInstPath;
}

void CInstanceSettings::AddWatchpoint(const CString& crstrPosition)
{
    m_astrWatchpoints.Add(crstrPosition);
}

int CInstanceSettings::GetWatchpointCount()
{
    return m_astrWatchpoints.GetSize();
}

void CInstanceSettings::SetIndex(int iIndex)
{
    m_iIndex = iIndex;
}

int CInstanceSettings::GetIndex()
{
    return m_iIndex;
}


//////////////////////////////////////////////////////////////////////
// CDocumentSettings
//////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------*
/**
 * constructor
 *
 * @see             
*/
CDocumentSettings::CDocumentSettings()
{
    m_bVisible = FALSE;
    m_bTabbed = FALSE;
    m_iZOrder = 0;
    m_iActiveInstance = 0;
}



//------------------------------------------------------------------*
/**
 * destructor
 *
 * @see             
*/
CDocumentSettings::~CDocumentSettings()
{
    POSITION pos;
    pos = m_mapInstances.GetStartPosition();
    while(pos)
    {
        CString strInstance;
        CInstanceSettings* pInstSettings;
        m_mapInstances.GetNextAssoc(pos, strInstance, pInstSettings);
        delete pInstSettings;
    }
    m_mapInstances.RemoveAll();
}


//------------------------------------------------------------------*
/**
 * Init
 *
 * @see             
*/
void CDocumentSettings::Init()
{
}

//------------------------------------------------------------------*
/**
 * set z order.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CDocumentSettings::SetZOrder(int iZOrder)
{
    m_iZOrder = iZOrder;
}

//------------------------------------------------------------------*
/**
 * get z order.
 *
 * @param           -
 * @return          z order
 * @exception       -
 * @see             -
*/
int CDocumentSettings::GetZOrder()
{
    return m_iZOrder;
}

void CDocumentSettings::SetActiveInstance(int iInstance)
{
    m_iActiveInstance = iInstance;
}

int CDocumentSettings::GetActiveInstance()
{
    return m_iActiveInstance;
}


//------------------------------------------------------------------*
/**
 * Save
 *
 * @param           parentNode: the root node in the settings xml
 * @param           document: the xml document, is needed to create new nodes
 * @return          
 * @exception       -
 * @see             
*/
void CDocumentSettings::Save(CXMLNode& parentNode, CXMLDocument& document)
{
    CXMLNode nodeWPlc;      // window placement
    CXMLNode nodeWPList;
    CXMLNode nodeWP;
    CXMLNode nodeInst;

    CString strTmp;

    strTmp = m_bTabbed ? _T("TRUE") : _T("FALSE");
    parentNode.SetAttribute(CE_PSET_GC_ATTR_TABBED, strTmp);

    strTmp = m_bVisible ? _T("TRUE") : _T("FALSE");
    parentNode.SetAttribute(CE_PSET_GC_ATTR_VISIBLE, strTmp);

    strTmp.Format("%d", m_iZOrder);
    parentNode.SetAttribute(CE_PSET_GC_ATTR_ZORDER, strTmp);

    strTmp.Format("%d", m_iActiveInstance);
    parentNode.SetAttribute(CE_PSET_GC_ATTR_ACTINST, strTmp);

    // save window placement if valid
    if(m_tWindowPlacement.IsInitialized())
    {
        if(document.CreateNode(nodeWPlc, CE_PSET_ELEM_WPLC))
        {
            if(parentNode.AppendChild(nodeWPlc))
            {
                m_tWindowPlacement.Save(nodeWPlc);
            }
        }
    }

    // save watchpoints
    POSITION pos;
    pos = m_mapInstances.GetStartPosition();
    while(pos)
    {
        CString strInstance;
        CInstanceSettings* pInstSettings;

        m_mapInstances.GetNextAssoc(pos, strInstance, pInstSettings);
        ASSERT(pInstSettings);
        if (pInstSettings==NULL)
        {
            continue;
        }
        if (strInstance.IsEmpty())
        {
            continue;
        }
        if(!document.CreateNode(nodeInst, CE_PSET_GC_ELEM_INSTANCE))
        {
            continue;
        }

        parentNode.AppendChild(nodeInst);

        nodeInst.SetAttribute(CE_PSET_GC_ATTR_PATH, pInstSettings->GetInstancePath());

        strTmp.Format("%d", pInstSettings->GetIndex());
        nodeInst.SetAttribute(CE_PSET_GC_ATTR_INDEX, strTmp);

        CStringArray*       pWatchpoints = pInstSettings->GetWatchpoints();

        document.CreateNode(nodeWPList, CE_PSET_GC_ELEM_WPLIST);
        nodeInst.AppendChild(nodeWPList);

        for (int i=0; i<=pWatchpoints->GetUpperBound(); i++)
        {
            document.CreateNode(nodeWP, CE_PSET_GC_ELEM_WP);
            nodeWPList.AppendChild(nodeWP);
            CString strPosition;
            strPosition = pWatchpoints->GetAt(i);
            nodeWP.SetAttribute(CE_PSET_GC_ATTR_POSITION, strPosition);
        }
    }
}

void CDocumentSettings::SetWindowPlacement(WINDOWPLACEMENT& rwndPlc)
{
    m_tWindowPlacement.SetWindowPlacement(rwndPlc);
}

BOOL CDocumentSettings::GetWindowPlacement(WINDOWPLACEMENT& rwndPlc)
{
    return(m_tWindowPlacement.GetWindowPlacement(rwndPlc));
}

void CDocumentSettings::ResetWindowPlacement()
{
    m_tWindowPlacement.Reset();
}


//------------------------------------------------------------------*
/**
 * Load
 *
 * @param           node: the document node in the settings xml
 * @return          
 * @exception       -
 * @see             
*/
void CDocumentSettings::Load(CXMLNode& node)
{
    CXMLNodeList    childList;

    node.GetChildNodeList(childList);
    childList.Reset();

    CXMLNode        nodeInst;
    CXMLNode        nodeWPList;
    CXMLNode        nodeWP;
    CXMLNodeList    wpNodeList;
    CString         strTmp;
    int             iIndex;

    node.GetAttribute(CE_PSET_GC_ATTR_ZORDER, strTmp);
    m_iZOrder = atoi(strTmp);

    node.GetAttribute(CE_PSET_GC_ATTR_ACTINST, strTmp);
    m_iActiveInstance = atoi(strTmp);

    while(childList.Next(nodeInst))
    {
        CString strInstance;
        CInstanceSettings* pInstSettings;

        // windows placement node?
        if (nodeInst.GetTagName(strTmp))
        {
            if (strTmp.Compare(CE_PSET_ELEM_WPLC)==0)
            {
                // load it
                m_tWindowPlacement.Load(nodeInst);
                continue;
            }
        }

        nodeInst.GetAttribute(CE_PSET_GC_ATTR_PATH, strInstance);
        if(strInstance.IsEmpty())
        {
            continue;
        }

        pInstSettings = FindInstance(strInstance);
        if (pInstSettings == NULL)
        {
            continue;
        }

        nodeInst.GetAttribute(CE_PSET_GC_ATTR_INDEX, strTmp);
        iIndex = atoi(strTmp);
        pInstSettings->SetIndex(iIndex);

        if(nodeInst.GetFirstChildWithTagName(CE_PSET_GC_ELEM_WPLIST, nodeWPList))
        {
            nodeWPList.GetChildNodeList(wpNodeList);
            wpNodeList.Reset();
            while(wpNodeList.Next(nodeWP))
            {
                CString strPosition;

                nodeWP.GetAttribute(CE_PSET_GC_ATTR_POSITION, strPosition);

                pInstSettings->AddWatchpoint(strPosition);
            }
        }
    }

}


BOOL CDocumentSettings::IsWatchpointListEmpty()
{
    if (!m_mapInstances.IsEmpty())
    {
        POSITION pos;
        pos = m_mapInstances.GetStartPosition();
        while(pos)
        {
            CString strInstance;
            CInstanceSettings* pInstSettings;

            m_mapInstances.GetNextAssoc(pos, strInstance, pInstSettings);
            ASSERT(pInstSettings);
            if (pInstSettings!=NULL)
            {
                if (pInstSettings->GetWatchpoints()->GetUpperBound()>=0)
                {
                    return FALSE;
                }
            }
        }
    }
    return TRUE;
}

void CDocumentSettings::SetFilePath(const CString& crstrFilePath)
{
    m_strFilePath = crstrFilePath;
}


CString CDocumentSettings::GetFilePath()
{
    return m_strFilePath;
}


void CDocumentSettings::SetDocumentVisible(BOOL bVisible /*=TRUE*/)
{
    m_bVisible = bVisible;
}


BOOL CDocumentSettings::IsDocumentVisible()
{
    return m_bVisible;
}


void CDocumentSettings::SetTabbed(BOOL bTabbed /*=TRUE*/)
{
    m_bTabbed = bTabbed;
}

BOOL CDocumentSettings::IsTabbed()
{
    return m_bTabbed;
}


BOOL CDocumentSettings::IsInstancePresent(const CString& crstrInstance)
{
    CString strTemp = crstrInstance;
    CInstanceSettings* pInstSettings;
    strTemp.MakeLower();

    if (m_mapInstances.Lookup(strTemp, pInstSettings))
    {
        return TRUE;
    }
    return FALSE;
}

//------------------------------------------------------------------*
/**
 * FindInstance finds the settings of an instance.
 *
 * @param           [in] crstrInstance: instance path
 * @param           [in] bCreateNew: create new instance settings if not yet present
 * @return          pointer to instance settings
 * @exception       -
 * @see             
*/
CInstanceSettings* CDocumentSettings::FindInstance(const CString& crstrInstance, BOOL bCreateNew /*=TRUE*/)
{
    CString strTemp;
    CInstanceSettings* pInstSettings;
    strTemp = crstrInstance;

    strTemp.MakeLower();

    if(!m_mapInstances.Lookup(strTemp, pInstSettings))
    {
        if(bCreateNew)
        {
            pInstSettings = new CInstanceSettings(crstrInstance);
            if(!pInstSettings)
            {
                ASSERT(TRUE);
                return NULL;
            }
            m_mapInstances.SetAt(strTemp, pInstSettings);
        }
        else
        {
            return NULL;
        }
    }
    ASSERT(pInstSettings);
    return pInstSettings;
}

void CDocumentSettings::DeleteAllInstances()
{
    m_mapInstances.RemoveAll();
}

int CDocumentSettings::GetNumberOfInstances()
{
    return m_mapInstances.GetCount();
}

//------------------------------------------------------------------*
/**
 * GetInstanceStartPosition gets the position of the first instance
 *
 * @param           
 * @return          the position, to use in GetNextInstance
 *                  NULL if there are no instances
 * @exception       -
 * @see             
*/
POSITION CDocumentSettings::GetInstanceStartPosition()
{
    return m_mapInstances.GetStartPosition();
}



//------------------------------------------------------------------*
/**
 * GetNextInstance iterate through all instances
 *
 * @param           pos: the actual position in the list
 * @param           strFileName: the instance file name
 * @param           pDocSettings: the instance settings
 * @return          
 * @exception       -
 * @see             
*/
void CDocumentSettings::GetNextInstance(POSITION& pos, CString& rstrInstPath, 
                                         CInstanceSettings*& pInstSettings)
{
    m_mapInstances.GetNextAssoc(pos, rstrInstPath, pInstSettings);
}



int CDocumentSettings::GetInstances(CStringArray& rastrInstances)
{
    POSITION pos;
    pos = m_mapInstances.GetStartPosition();
    while(pos)
    {
        CString strInstance;
        CInstanceSettings* pInstSettings;
        m_mapInstances.GetNextAssoc(pos, strInstance, pInstSettings);
        rastrInstances.Add(pInstSettings->GetInstancePath());
    }
    return rastrInstances.GetSize();
}


BOOL CDocumentSettings::DeleteInstance(const CString& crstrInstance)
{
    CString strTemp = crstrInstance;
    CInstanceSettings* pInstSettings;
    strTemp.MakeLower();

    if (m_mapInstances.Lookup(strTemp, pInstSettings))
    {
        ASSERT(pInstSettings);
        delete pInstSettings;
        m_mapInstances.RemoveKey(strTemp);
        return TRUE;
    }
    return FALSE;
}


CStringArray* CDocumentSettings::GetWatchpoints(const CString& crstrInstance, BOOL bCreate)
{
    CInstanceSettings*  pInstSettings = FindInstance(crstrInstance, bCreate);
    if(pInstSettings)
    {
        return pInstSettings->GetWatchpoints();
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
// CProjectListViewSettings
//////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------*
/**
 * constructor
 *
 * @see             
*/
CProjectListViewSettings::CProjectListViewSettings()
{
}


//------------------------------------------------------------------*
/**
 * destructor
 *
 * @see             
*/
CProjectListViewSettings::~CProjectListViewSettings()
{
}



//------------------------------------------------------------------*
/**
 * Load loads the project list view settings
 *
 * @param           parentNode: the root node of the xml settings
 * @return          
 * @exception       -
 * @see             
*/
void CProjectListViewSettings::Load(CXMLNode &parentNode)
{
    CXMLNode nodeProjectListView;
    CXMLNode nodeWPlc;

    if(parentNode.GetFirstChildWithTagName(CE_PSET_ELEM_PROJECT_LIST_VIEW, nodeProjectListView))
    {
        // now get windows placement node
        if(nodeProjectListView.GetFirstChildWithTagName(CE_PSET_ELEM_WPLC, nodeWPlc))
        {
            m_tWindowPlacement.Load(nodeWPlc);
        }
    }
}



//------------------------------------------------------------------*
/**
 * Save saves the project list view settings
 *
 * @param           parentNode: the root node of the settings tree
 * @param           document: the xml document
 * @return          
 * @exception       -
 * @see             
*/
void CProjectListViewSettings::Save(CXMLNode &parentNode, CXMLDocument &document)
{
    CXMLNode nodeProjectListView;
    CXMLNode nodeWPlc;
    
    if(document.CreateNode(nodeProjectListView, CE_PSET_ELEM_PROJECT_LIST_VIEW))
    {
        if(parentNode.AppendChild(nodeProjectListView))
        {
            if(document.CreateNode(nodeWPlc, CE_PSET_ELEM_WPLC))
            {
                if(nodeProjectListView.AppendChild(nodeWPlc))
                {
                    m_tWindowPlacement.Save(nodeWPlc);
                }
            }
        }
    }

}



//////////////////////////////////////////////////////////////////////
// CSettings
//////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------*
/**
 * constructor
 *
 * @see             
*/
CContainerSettings::CContainerSettings() :
  m_iNumVisibleDocs(0)
{
}

//------------------------------------------------------------------*
/**
 * destructor
 *
 * @see             
*/
CContainerSettings::~CContainerSettings()
{
    POSITION pos;

    pos = m_mapDocuments.GetStartPosition();
    while(pos)
    {
        CString strFileName;
        CDocumentSettings* pDocSet;

        m_mapDocuments.GetNextAssoc(pos, strFileName, pDocSet);
        delete pDocSet;
    }

    m_mapDocuments.RemoveAll();
}


//------------------------------------------------------------------*
/**
 * Init
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CContainerSettings::Init()
{
}


//------------------------------------------------------------------*
/**
 * update settings.
 *
 *  remove all files that do not exist any more.
 *  set all documents to invisible
 *
 * @return          -
 * @exception       -
 * @see             -
*/
void CContainerSettings::UpdateSettings()
{
    // run over all instances
    POSITION        pos = GetDocumentStartPosition();
    POSITION        posold = pos;
    CDocumentSettings*  pDocSettings;
    CString         strFilePath;
    CStringArray    astrTmp;
    CFileStatus     tStatus;

    while(pos)
    {
        GetNextDocument(pos, strFilePath, pDocSettings);
        if(MayRemoveDocument(pDocSettings))
        {
            astrTmp.Add(strFilePath);
        }
        pDocSettings->SetDocumentVisible(FALSE);
        pDocSettings->ResetWindowPlacement();
    }
    int iNumDocs = astrTmp.GetSize();
    for(int iDoc = 0; iDoc < iNumDocs; ++iDoc)
    {
        RemoveDocument(astrTmp[iDoc]);
    }
}

//------------------------------------------------------------------*
/**
 * may remove document?
 *
 *  doc settings may be removed, if no watchpoints are stored.
 *
 * @param           [in] pDocSettings: document settings
 * @return          may remove?
 * @exception       -
 * @see             -
*/
BOOL CContainerSettings::MayRemoveDocument(CDocumentSettings* pDocSettings)
{
    BOOL bRemove = TRUE;
    CInstanceSettings*  pInstSettings;
    CString             strInstance;
    POSITION pos = pDocSettings->GetInstanceStartPosition();
    while(pos)
    {
        pDocSettings->GetNextInstance(pos, strInstance, pInstSettings);
        if(pInstSettings->GetWatchpointCount() > 0)
        {
            bRemove = FALSE;
            break;
        }
    }
    return bRemove;
}


//------------------------------------------------------------------*
/**
 * get number of visible documents.
 *
 *  this number is only valid immediately after Load()
 *  during a session it is not updated.
 *
 * @param           -
 * @return          number of visible documents
 * @exception       -
 * @see             
*/
int CContainerSettings::GetNumVisibleDocs()
{
    return m_iNumVisibleDocs;
}

//------------------------------------------------------------------*
/**
 * Save
 *
 * @param           parentNode: the root node in the settings xml
 * @param           document: the xml document, is needed to create new nodes
 * @return          
 * @exception       -
 * @see             
*/
void CContainerSettings::Save(CXMLNode& parentNode, CXMLDocument& document)
{
    CXMLNode nodeContainer;
    CXMLNode nodeDocument;

    document.CreateNode(nodeContainer, CE_PSET_GC_CONTAINER);
    parentNode.AppendChild(nodeContainer);

    POSITION pos;
    pos = m_mapDocuments.GetStartPosition();
    while(pos)
    {
        CString strFileName;
        CDocumentSettings* pDocSet;
        CFile file;

        m_mapDocuments.GetNextAssoc(pos, strFileName, pDocSet);
        if (pDocSet == NULL)
        {
            continue;
        }
        
        strFileName = pDocSet->GetFilePath();

        document.CreateNode(nodeDocument, CE_PSET_GC_ELEM_DOCUMENT);
        nodeContainer.AppendChild(nodeDocument);
        
        nodeDocument.SetAttribute(CE_PSET_GC_ATTR_NAME, strFileName);

        pDocSet->Save(nodeDocument, document);
    }

}


//------------------------------------------------------------------*
/**
 * Load
 *
 * @param           parentNode: the root node in the settings xml
 * @return          
 * @exception       -
 * @see             
*/
void CContainerSettings::Load(CXMLNode& parentNode)
{
    CXMLNode nodeContainer;
    CXMLNodeList nodeList;
    BOOL bFound = FALSE;
    int iMaxZOrder = -1;

    m_iNumVisibleDocs = 0;

    parentNode.GetChildNodeList(nodeList);
    nodeList.Reset();
    while(nodeList.Next(nodeContainer))
    {
        CString strTag;
        if (nodeContainer.GetTagName(strTag))
        {
            if (strTag.Compare(CE_PSET_GC_CONTAINER)==0)
            {
                bFound = TRUE;
                break;
            }
        }
    }

    if (!bFound)
    {
        return;
    }

    CString     strFileName;
    CString     strTag;
    CString     strVisibility;
    CString     strTabbed;
    CXMLNode    nodeDocument;

    nodeContainer.GetChildNodeList(nodeList);
    nodeList.Reset();
    while(nodeList.Next(nodeDocument))
    {
        // check node tag
        if (nodeDocument.GetTagName(strTag))
        {
            if (strTag.Compare(CE_PSET_GC_ELEM_DOCUMENT)==0)
            {
                if (!nodeDocument.GetAttribute(CE_PSET_GC_ATTR_NAME, strFileName))
                {
                    continue;
                }

                CDocumentSettings* pDocSet = NULL;
                pDocSet = new CDocumentSettings;
                ASSERT(pDocSet);
                if (!pDocSet)
                {
                    continue;
                }
                pDocSet->Load(nodeDocument);
                pDocSet->SetFilePath(strFileName);

                if(nodeDocument.GetAttribute(CE_PSET_GC_ATTR_VISIBLE, strVisibility))
                {
                    if(strVisibility.Compare(_T("TRUE")) == 0)
                    {
                        pDocSet->SetDocumentVisible(TRUE);
                        if(iMaxZOrder < pDocSet->GetZOrder())
                        {
                            iMaxZOrder = pDocSet->GetZOrder();
                        }
                        m_iNumVisibleDocs++;
                    }
                }
                if(nodeDocument.GetAttribute(CE_PSET_GC_ATTR_TABBED, strTabbed))
                {
                    if(strTabbed.Compare(_T("TRUE")) == 0)
                    {
                        pDocSet->SetTabbed(TRUE);
                    }
                }
                strFileName.MakeLower();
                m_mapDocuments.SetAt(strFileName, pDocSet);
            }
        }
    }
    ASSERT(m_iNumVisibleDocs > iMaxZOrder);
    if(m_iNumVisibleDocs <= iMaxZOrder)
    {
        m_iNumVisibleDocs = iMaxZOrder + 1;
    }
}



//------------------------------------------------------------------*
/**
 * FindDocument finds the settings of a document
 *
 * @param           [in] strFileName: the name of the document
 * @param           [in] bCreateNew: create new document settings, if the document 
 *                                   is not yet present
 * @return          pointer to document settings
 * @exception       -
 * @see             
*/
CDocumentSettings* CContainerSettings::FindDocument(const CString& crstrFileName, BOOL bCreateNew /*= TRUE*/)
{
    CString strTemp;
    CDocumentSettings* pDocSettings;
    CString strFileName(crstrFileName);
    CCEFileHandler* pFileHandler = ((CCEMAINApp*)AfxGetApp())->GetFileHandler();

    ASSERT(pFileHandler);
    if(!pFileHandler)
    {
        return NULL;
    }

    if(crstrFileName.IsEmpty())
    {
        return NULL;
    }

    strFileName = pFileHandler->GetFilePathOpt(crstrFileName);

    strTemp = strFileName;
    strTemp.MakeLower();

    if(!m_mapDocuments.Lookup(strTemp, pDocSettings))
    {
        if(bCreateNew)
        {
            pDocSettings = new CDocumentSettings;
            if(!pDocSettings)
            {
                ASSERT(TRUE);
                return NULL;
            }
            pDocSettings->SetFilePath(strFileName);
            m_mapDocuments.SetAt(strTemp, pDocSettings);
        }
        else
        {
            return NULL;
        }
    }
    ASSERT(pDocSettings);
    return pDocSettings;
}



//------------------------------------------------------------------*
/**
 * GetDocumentStartPosition gets the position of the first document
 *
 * @param           
 * @return          the position, to use in GetNextDocument
 *                  NULL if there are no documents
 * @exception       -
 * @see             
*/
POSITION CContainerSettings::GetDocumentStartPosition()
{
    return m_mapDocuments.GetStartPosition();
}



//------------------------------------------------------------------*
/**
 * GetNextDocument iterate through all documents
 *
 * @param           pos: the actual position in the list
 * @param           strFileName: the document file name
 * @param           pDocSettings: the document settings
 * @return          
 * @exception       -
 * @see             
*/
void CContainerSettings::GetNextDocument(POSITION& pos, CString& strFileName, 
                                         CDocumentSettings*& pDocSettings)
{
    CDocumentSettings* pDocSet;

    m_mapDocuments.GetNextAssoc(pos, strFileName, pDocSet);
    pDocSettings = pDocSet;
}


BOOL CContainerSettings::RemoveDocument(const CString& crstrFileName)
{
    CDocumentSettings* pDocSet;
    BOOL    bReturn(FALSE);

    if(m_mapDocuments.Lookup(crstrFileName, pDocSet))
    {
        if(pDocSet)
        {
            delete pDocSet;
        }
        m_mapDocuments.RemoveKey(crstrFileName);
        bReturn = TRUE;
    }
    return bReturn;
}







//////////////////////////////////////////////////////////////////////
// CSettings
//////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------*
/**
 * constructor
 *
 * @see             
*/
CSettings::CSettings() :
    m_dockToolsPosSettings()
{
    Init();
}


//------------------------------------------------------------------*
/**
 * destructor
 *
 * @see             
*/
CSettings::~CSettings()
{

}


//------------------------------------------------------------------*
/**
 * Init init settings
 *
 * @see             
*/
void CSettings::Init()
{
    m_dockToolsPosSettings.Init();
    m_librarySettings.Init();
    m_breakpointSettings.Init();
    m_containerSettings.Init();
    m_engineeringSettings.Init();
    //m_watchSettings.Init();   // not yet needed
    //m_printSettings.Init();   // not needed
}



//------------------------------------------------------------------*
/**
 * Save the settings file.
 *
 * @param           strProjPath: the project path
 * @return          
 * @exception       -
 * @see             
*/
void CSettings::Save(const CString& strProjPath)
{
    BOOL ret;
    CXMLDocument document;
    CXMLNode rootNode;
    CXMLNode docNode;

    ret = document.CreateNode(rootNode, (CString)CE_PSET_ELEM_PRJSETTINGS);
    ASSERT(ret); 
    if (!ret) 
    {
        return;
    }
    ret = document.GetDocNode(docNode);
    ASSERT(ret);
    if (!ret)
    {
        return;
    }
    ret = docNode.AppendChild(rootNode);
    ASSERT(ret);
    if (!ret)
    {
        return;
    }

    m_dockToolsPosSettings.Save(rootNode, document);
    m_librarySettings.Save(rootNode, document);
    m_breakpointSettings.Save(rootNode, document);
    m_containerSettings.Save(rootNode, document);
    m_projectListViewSettings.Save(rootNode, document);
    m_engineeringSettings.Save(rootNode, document);
    m_watchSettings.Save(rootNode, document);
    m_printSettings.Save(rootNode, document);

    ret = document.Save(strProjPath + CE_PSET_FILENAME);
    ASSERT(ret);


}



//------------------------------------------------------------------*
/**
 * Load: load the settings file
 *
 * @param           strProjPath: the project path
 * @return          
 * @exception       -
 * @see             
*/
void CSettings::Load(const CString& strProjPath)
{
    BOOL ret;
    CXMLDocument document;
    
    ret = document.Load(strProjPath + _T("\\") + CE_PSET_FILENAME);
    if (!ret)
    {
        Init();
        return;
    }

    CXMLNode rootNode;
    ret = document.GetRootNode(rootNode);
    ASSERT(ret);
    if (!ret)
    {
        Init();
        return;
    }

    m_dockToolsPosSettings.Load(rootNode);
    m_librarySettings.Load(rootNode);
    m_breakpointSettings.Load(rootNode);
    m_containerSettings.Load(rootNode);
    m_projectListViewSettings.Load(rootNode);
    m_engineeringSettings.Load(rootNode);
    m_watchSettings.Load(rootNode);
    m_printSettings.Load(rootNode);
}



//------------------------------------------------------------------*
/**
 * RestoreDocking inits only docking tools.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CSettings::ResetDocking()
{
    m_dockToolsPosSettings.Init();
}

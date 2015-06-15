
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "FileTreeCtrl.h"
#include "NavDef.h"
#include "cesysdef.h"
#include "DragItem.h"
#include "resource.h"
#include "cemain.h"
#include "KADHelper/KADFileExtInfo.h"
#include "cesysdef.h"
#include "CECompMan.h"
#include "FileStatusDlg.h"
#include "resource.h"
#include "fc_tools/fc_string.h"
#include "fc_tools/fc_fileIO.h"
#include "utilif.h"
#include "CEFileHandler.h"
#include "mainfrm.h"

#include <winerror.h>

//----  Static Initializations:   ----------------------------------*

const CString con_strSourceFileFolder       = "Source Files";
const CString con_strHelpFileFolder         = "Help Files";
const CString con_strAdditionalFileFolder   = "Additional Files";

const CString con_strTypeFileProject        = "FV_PROJECT";
const CString con_strTypeSourceGroup        = "FV_SOURCE_FILE_GROUP";
const CString con_strTypeHelpGroup          = "FV_HELP_FILE_GROUP";
const CString con_strTypeAdditionalGroup    = "FV_ADDITIONAL_FILE_GROUP";
const CString con_strTypeFolder             = "FV_FOLDER";
const CString con_strTypeFile               = "FV_FILE";

CFileTreeCtrl::CFileTreeCtrl()
{
    m_bLoaded           = FALSE;
    m_bChecking         = FALSE;
    m_iCheckDisableCount    = 1;    // file checking is disabled
    m_bCatChangeAllowed = TRUE;
    m_pThread           = NULL;
    m_bRejectUpdateMessages = FALSE;
}

CFileTreeCtrl::~CFileTreeCtrl()
{
    ClearTree();
    ResetFilters();
}

//------------------------------------------------------------------*
/**
 * return view index of file view
 *
 *  identification of view
 *  used as index value
 *
 * @return          view index of file view (=0)
*/
int CFileTreeCtrl::GetViewIndex()
{
    return(0);
}

BEGIN_MESSAGE_MAP(CFileTreeCtrl, CNavTreeCtrl)
	//{{AFX_MSG_MAP(CFileTreeCtrl)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_NAV_FV_FILE_CHANGED, OnFileChanged)
    ON_MESSAGE(WM_NAV_FV_DECR_CHECK_DISABLE_COUNT, OnDecrementCheckDisableCount)
END_MESSAGE_MAP()


//------------------------------------------------------------------*
/**
 * unload project data.
 *
 *  stop file change notification thread and call base class method
 *
 * @see             StopFileChangeNotificationThread(), CNavTreeCtrl::Unload()
*/
void CFileTreeCtrl::Unload()
{
    StopFileChangeNotificationThread();
    m_tFileAssoc.RemoveAll();
    CNavTreeCtrl::Unload();
}

//------------------------------------------------------------------*
/**
 * unload XML.
 *
 *  overwritten method of base class CNavTreeCtrl
 *  save XML in fileview.xml using SaveXML()
 *  calls base class method
 *
 * @see             CNavTreeCtrl::UnloadXML()
*/
void CFileTreeCtrl::UnloadXML()
{
    if(m_pXMLDoc)
    {
        SaveXML();
        m_bLoaded = FALSE;
    }
    CNavTreeCtrl::UnloadXML();
}


//------------------------------------------------------------------*
/**
 * load data for file view.
 *
 *  call CNavTreeCtrl::Load() to set project name and path
 *  call LoadFromXML() if fileview.xml is found
 *  else call LoadFromProject()
 *  set project name
 *
 * @return          successful?
 * @see             LoadFromXML(), LoadFromProject()
*/
BOOL CFileTreeCtrl::Load()
{
    // set project name and path
    CNavTreeCtrl::Load();

    CString strTmp;
    strTmp.Format("%s\\%s.4cp", m_strProjectPath, m_strProjectName);
    m_strProjectName = strTmp;

    // create DOM
	CXMLDocument* pXMLDoc = new CXMLDocument;

    // if fileview.xml exists: load XML
    WIN32_FIND_DATA tFindData;
    
    CString strXmlFile = m_strProjectPath + _T("\\") + CE_FILE_FILEVIEW;

    HANDLE hFile = ::FindFirstFile(strXmlFile, &tFindData);

    if (hFile != INVALID_HANDLE_VALUE)
    {
        if(!LoadFromXML(pXMLDoc))
        {
            LoadFromProject(pXMLDoc);
        }
        ::FindClose(hFile);
    }
    else
    {
        LoadFromProject(pXMLDoc);
    }

    m_pXMLDoc = pXMLDoc;

    SetProjectName();

    BuildTree();
    m_bLoaded = TRUE;
    m_iCheckDisableCount = 0;   // file checking is enabled now
    Check(TRUE);

    StartFileChangeNotificationThread(m_strProjectPath);

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * set project name.
 *
 *  if project was copied, the project name (contains path) is invalid
 *  this method ensures, that the right path is shown
 *
 * @return          -
 * @exception       -
 * @see             -
*/
void CFileTreeCtrl::SetProjectName()
{
    CXMLNode    tNodeRoot;
    if(!m_pXMLDoc->GetRootNode(tNodeRoot))
    {
        return;
    }
    CString strSource;

    tNodeRoot.SetAttribute(CE_XMLATTR_TEXT, m_strProjectName);
    tNodeRoot.SetAttribute(CE_XMLATTR_ID, m_strProjectName);
    if(tNodeRoot.GetAttribute(CE_XMLATTR_SOURCE, strSource))
    {
        tNodeRoot.SetAttribute(CE_XMLATTR_SOURCE, m_strProjectName);
    }
}

//------------------------------------------------------------------*
/**
 * build tree.
 *
 *  dont allow checking during build tree.
 *
 * @see             Check(), CNavTreeCtrl::BuildTree()
*/
BOOL CFileTreeCtrl::BuildTree()
{
    BOOL    bReturn;
    EnableFileCheck(FALSE);
    m_tFileAssoc.RemoveAll();
    bReturn = CNavTreeCtrl::BuildTree();
    EnableFileCheck(TRUE);
    return bReturn;
}


//------------------------------------------------------------------*
/**
 * load file view data from last session from file fileview.xml
 *
 * @param           [in] pXMLDoc: pointer to XML document
 * @return          successful?
 * @see             Load()
*/
BOOL CFileTreeCtrl::LoadFromXML(CXMLDocument* pXMLDoc)
{
    //CString strXmlFile = GetProjectPath() + rstrProjName + "/fileview.xml";
    CString strXmlFile = CE_FILE_FILEVIEW;

    // load XML-file
	if(!pXMLDoc->Load(strXmlFile))
    {
        return FALSE;
    }
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * set XML pointers
 *
 *  overwritten method for file view specifics
 *  node pointers for different file categories are set:
 *  create DOM-nodes for source, help and additional files folder
 *
 * @param           [in] pXMLDoc: pointer to XML document
 * @return          successful?
 * @see             CNavTreeCtrl::InitializeDOM()
*/
BOOL CFileTreeCtrl::InitializeDOM(CXMLDocument* pXMLDoc /*=NULL*/)
{
    CXMLNodeList    tChildList;
    CXMLNode        tNodeRoot;
    CXMLNode        tNodeSource;
    CXMLNode        tNodeHelp;
    CXMLNode        tNodeAdditional;

    // first initialize root pointer; this is done in base class
    CNavTreeCtrl::InitializeDOM(pXMLDoc);

    if(!m_pXMLDoc)
    {
        return FALSE;
    }

    if(!m_pXMLDoc->GetRootNode(tNodeRoot))
    {
        return FALSE;
    }

    // check, if all system nodes exist:
    if(!GetSystemNode(FV_FILE_SOURCE, tNodeSource) || 
        !GetSystemNode(FV_FILE_HELP, tNodeHelp) ||
        !GetSystemNode(FV_FILE_ADDITIONAL, tNodeAdditional))
    {
        return FALSE;
    }

    return TRUE;
}



//------------------------------------------------------------------*
/**
 * build up DOM from project
 *
 *  this is needed, if fileview.xml is not existent
 *  dom nodes are created for each category root (source, help ...)
 *  dom is initialized
 *  project manager is asked for all project files
 *  using GetAllFilesFromProjMan()
 *  BuildTree() is called
 *
 * @param           [in] pXMLDoc: pointer to XML document
 * @return          successful?
 * @see             GetAllFilesFromProjMan()
*/
BOOL CFileTreeCtrl::LoadFromProject(CXMLDocument* pXMLDoc)
{
    // initialize DOM
    if(!CreateDOMNodes(pXMLDoc))
        return FALSE;
    if(!InitializeDOM(pXMLDoc))
        return FALSE;

    //CXMLNode*           pNodeFile;
    CSBLLexTree<bool>       tFileNameTree;
    CStringArray            astrFileNames;
    long                    lNumFiles;
    CXMLNode                tNodeRoot;
    CXMLNode                tNodeFile;
    CXMLNode                tNodeSource;
    CXMLNode                tNodeHelp;
    CXMLNode                tNodeAdditional;
    CString                 strFileName;

    if(!pXMLDoc->GetRootNode(tNodeRoot))
    {
        return FALSE;
    }


    if(!GetSystemNode(FV_FILE_SOURCE, tNodeSource, pXMLDoc))
    {
        return FALSE;
    }

    // get all source files from project manager
    GetAllFilesFromProjMan(FV_FILE_SOURCE, tFileNameTree);
    tFileNameTree.getAllStringsFromSubstring("", astrFileNames);

    // insert all file names in DOM
    lNumFiles = astrFileNames.GetSize();

    for(long lFile=0; lFile < lNumFiles; ++lFile)
    {
        pXMLDoc->CreateNode(tNodeFile, CE_XMLTAG_NODE);
        // set name, id and source attribute to file name
        strFileName = astrFileNames[lFile];
        tNodeFile.SetAttribute(CE_XMLATTR_TEXT, strFileName);
        tNodeFile.SetAttribute(CE_XMLATTR_ID, strFileName);
        tNodeFile.SetAttribute(CE_XMLATTR_SOURCE, strFileName);
        tNodeFile.SetAttribute(CE_XMLATTR_TYPE, con_strTypeFile);
        if(!IsFileVisible(strFileName))
        {
            tNodeFile.SetAttribute(CE_XMLATTR_VISIBILITY, CE_XMLATTR_FALSE_VAL);
        }
        tNodeSource.AppendChild(tNodeFile);
    }

    // reset file tree and string array
    tFileNameTree.reset();
    astrFileNames.RemoveAll();

    if(!GetSystemNode(FV_FILE_HELP, tNodeHelp, pXMLDoc))
    {
        return FALSE;
    }
    // get all help files from project manager
    GetAllFilesFromProjMan(FV_FILE_HELP, tFileNameTree);
    tFileNameTree.getAllStringsFromSubstring("", astrFileNames);

    // insert all file names in DOM
    lNumFiles = astrFileNames.GetSize();

    for(lFile=0; lFile < lNumFiles; ++lFile)
    {
        pXMLDoc->CreateNode(tNodeFile, CE_XMLTAG_NODE);
        // set name attribute to project name
        strFileName = astrFileNames[lFile];
        tNodeFile.SetAttribute(CE_XMLATTR_TEXT, strFileName);
        tNodeFile.SetAttribute(CE_XMLATTR_ID, strFileName);
        tNodeFile.SetAttribute(CE_XMLATTR_SOURCE, strFileName);
        tNodeFile.SetAttribute(CE_XMLATTR_TYPE, con_strTypeFile);
        if(!IsFileVisible(strFileName))
        {
            tNodeFile.SetAttribute(CE_XMLATTR_VISIBILITY, CE_XMLATTR_FALSE_VAL);
        }
        tNodeSource.AppendChild(tNodeFile);
    }

    if(!GetSystemNode(FV_FILE_ADDITIONAL, tNodeAdditional, pXMLDoc))
    {
        return FALSE;
    }
    // get all additional files from project manager
    // get all help files from project manager
    GetAllFilesFromProjMan(FV_FILE_ADDITIONAL, tFileNameTree);
    tFileNameTree.getAllStringsFromSubstring("", astrFileNames);

    // insert all file names in DOM
    lNumFiles = astrFileNames.GetSize();

    for(lFile=0; lFile < lNumFiles; ++lFile)
    {
        pXMLDoc->CreateNode(tNodeFile, CE_XMLTAG_NODE);
        // set name attribute to project name
        strFileName = astrFileNames[lFile];
        tNodeFile.SetAttribute(CE_XMLATTR_TEXT, strFileName);
        tNodeFile.SetAttribute(CE_XMLATTR_ID, strFileName);
        tNodeFile.SetAttribute(CE_XMLATTR_SOURCE, strFileName);
        tNodeFile.SetAttribute(CE_XMLATTR_TYPE, con_strTypeFile);
        if(!IsFileVisible(strFileName))
        {
            tNodeFile.SetAttribute(CE_XMLATTR_VISIBILITY, CE_XMLATTR_FALSE_VAL);
        }
        tNodeSource.AppendChild(tNodeFile);
    }
    return TRUE;
}



//------------------------------------------------------------------*
/**
 * get system node.
 *
 * @param            [in] eFileCat: file category (source, help, additional, none)
 * @param            [out] rtNode: system node
 * @param            [in] pXMLDoc: XML-Document where to search (default is NULL)
 *                                  if this is NULL, member m_pXMLDoc is used
 * @return           system node found?
 *
*/
BOOL CFileTreeCtrl::GetSystemNode(E_FV_FileCategory eFileCat, CXMLNode& rtNode, CXMLDocument* pXMLDoc /*= NULL*/)
{
    if(!pXMLDoc)
    {
        if(m_pXMLDoc)
        {
            pXMLDoc = m_pXMLDoc;
        }
        else
        {
            return FALSE;
        }
    }

    CXMLNode    tNodeRoot;
    if(pXMLDoc->GetRootNode(tNodeRoot))
    {
        if(eFileCat == FV_FILE_UNKNOWN)
        {
            rtNode = tNodeRoot;
            return TRUE;
        }
        CString strGroupType = GetTypeForSystemNode(eFileCat);
        if(tNodeRoot.GetChildWithAttr(CE_XMLATTR_TYPE, strGroupType, rtNode))
        {
            return TRUE;
        }
    }
    return FALSE;
}

//------------------------------------------------------------------*
/**
 * get type for system node.
 *
 * @param           [in] eFileCat: file category (source, help, additional, none)
 * @return          KAD type of system node.
 *
*/
CString CFileTreeCtrl::GetTypeForSystemNode(E_FV_FileCategory eFileCat)
{
    switch(eFileCat)
    {
    case FV_FILE_SOURCE:
        return con_strTypeSourceGroup;
    case FV_FILE_HELP:
        return con_strTypeHelpGroup;
    case FV_FILE_ADDITIONAL:
        return con_strTypeAdditionalGroup;
    }
    return(con_strTypeFileProject);
}

//------------------------------------------------------------------*
/**
 * get file category of node
 *
 * @param           [in] pNode: pointer to XML node
 * @return          file category of node
 * @see             E_FV_FileCategory
*/
E_FV_FileCategory CFileTreeCtrl::GetFileCategory(CXMLNode* pNode)
{
    CXMLNode        tNodeParent;
    CString         strText;
    CString         strType;

    // get father node until node has attribute FIX (this is a system folder)
    pNode->GetParentWithAttr(CE_XMLATTR_SYSTEM, CE_XMLATTR_TRUE_VAL, tNodeParent, TRUE);

    tNodeParent.GetAttribute(CE_XMLATTR_TYPE, strType);

    if(strType == con_strTypeFileProject)
        return(FV_FILE_UNKNOWN);

    if(strType == con_strTypeSourceGroup)
        return(FV_FILE_SOURCE);
    if(strType == con_strTypeHelpGroup)
        return(FV_FILE_HELP);
    if(strType == con_strTypeAdditionalGroup)
        return(FV_FILE_ADDITIONAL);
    return(FV_FILE_UNKNOWN);
}


//------------------------------------------------------------------*
/**
 * get all file names from disk and add them to tLexTree
 *
 *  lex tree needs data to be associated with strings
 *  use bool in this case, i.e. for each string a bool is created with new
 *  but never used.
 *  destruction of created bools is delegated to lexical tree by making it owner
 *  (setModusOwner())
 *  used by Check()
 *
 * @param           [out] tLexTree: lexical tree to hold file names
 * @see             Check()
*/
void CFileTreeCtrl::GetAllFilesFromDisk(CSBLLexTree<bool>& tLexTree, BOOL bUseFilter /*=FALSE*/)
{
    // read all files in the directory and save file names in a lexical tree
    bool*       pbTmp = (bool*) 1;  // dummy pointer for insert
    CString     strFileName;

//    TRACE("GetAllFilesFromDisk\n");

    CString strFind_pattern = m_strProjectPath + "/*";

    // tLexTree is set to owner of data and deletes all bool* inserted
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;

    hFind = ::FindFirstFile(strFind_pattern, &FindFileData);

    if (hFind == INVALID_HANDLE_VALUE)
        return;

    // run over all files
    do
    {
        // if no directory and not hidden -> insert into tree
        if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
            !(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
        {
            strFileName = FindFileData.cFileName;
            if(!bUseFilter || (bUseFilter && IsPassingFilters(strFileName, con_strTypeFile)))
            {
                // hack for 4gh file
                // this should never be shown
                if(strFileName.Right(4).CompareNoCase(_T(".4gh")) != 0)
                {
                    tLexTree.insertData(strFileName, pbTmp);
                }
            }
//            TRACE("%s\n", FindFileData.cFileName);
        }
    } while(::FindNextFile(hFind, &FindFileData));

    ::FindClose(hFind);
}


//------------------------------------------------------------------*
/**
 * create DOM-nodes for root, source, help and additional files folder
 *
 *  is needed, when file view is loaded directly from project
 *  with LoadFromProject()
 *
 * @param           [in] pXMLDoc: pointer to XML document
 * @return          successful?
 * @see             LoadFromProject()
*/
BOOL CFileTreeCtrl::CreateDOMNodes(CXMLDocument* pXMLDoc)
{
    CXMLNode    tNodeDoc;
    CXMLNode    tNodeRoot;
    CXMLNode    tNodeChildSource;
    CXMLNode    tNodeChildHelp;
    CXMLNode    tNodeChildAdditional;
    if(!pXMLDoc->GetDocNode(tNodeDoc))
        return FALSE;

    CString strXMLFile(CE_FILE_FILEVIEW);

    VERIFY(pXMLDoc->CreateNode(tNodeRoot, CE_XMLTAG_NODE));
    tNodeRoot.SetAttribute(CE_XMLATTR_TEXT, m_strProjectName);
    tNodeRoot.SetAttribute(CE_XMLATTR_ID, m_strProjectName);
    tNodeRoot.SetAttribute(CE_XMLATTR_SOURCE, m_strProjectName);
    tNodeRoot.SetAttribute(CE_XMLATTR_TYPE, con_strTypeFileProject);
    tNodeRoot.SetAttribute(CE_XMLATTR_SYSTEM, CE_XMLATTR_TRUE_VAL);
    tNodeDoc.AppendChild(tNodeRoot);

    VERIFY(pXMLDoc->CreateNode(tNodeChildSource, CE_XMLTAG_NODE));
    tNodeChildSource.SetAttribute(CE_XMLATTR_TEXT, con_strSourceFileFolder);
    tNodeChildSource.SetAttribute(CE_XMLATTR_ID, con_strSourceFileFolder);
    tNodeChildSource.SetAttribute(CE_XMLATTR_TYPE, con_strTypeSourceGroup);
    tNodeChildSource.SetAttribute(CE_XMLATTR_SYSTEM, CE_XMLATTR_TRUE_VAL);
    tNodeRoot.AppendChild(tNodeChildSource);

    VERIFY(pXMLDoc->CreateNode(tNodeChildHelp, CE_XMLTAG_NODE));
    tNodeChildHelp.SetAttribute(CE_XMLATTR_TEXT, con_strHelpFileFolder);
    tNodeChildHelp.SetAttribute(CE_XMLATTR_ID, con_strHelpFileFolder);
    tNodeChildHelp.SetAttribute(CE_XMLATTR_TYPE, con_strTypeHelpGroup);
    tNodeChildHelp.SetAttribute(CE_XMLATTR_SYSTEM, CE_XMLATTR_TRUE_VAL);
    tNodeRoot.AppendChild(tNodeChildHelp);

    VERIFY(pXMLDoc->CreateNode(tNodeChildAdditional, CE_XMLTAG_NODE));
    tNodeChildAdditional.SetAttribute(CE_XMLATTR_TEXT, con_strAdditionalFileFolder);
    tNodeChildAdditional.SetAttribute(CE_XMLATTR_ID, con_strAdditionalFileFolder);
    tNodeChildAdditional.SetAttribute(CE_XMLATTR_TYPE, con_strTypeAdditionalGroup);
    tNodeChildAdditional.SetAttribute(CE_XMLATTR_SYSTEM, CE_XMLATTR_TRUE_VAL);
    tNodeRoot.AppendChild(tNodeChildAdditional);

    return TRUE;
}

// save DOM in file fileview.xml
BOOL CFileTreeCtrl::SaveXML()
{
    if(m_bLoaded)
    {
        CString strFileName = m_strProjectPath + _T("\\") + CE_FILE_FILEVIEW;
        m_pXMLDoc->Save(strFileName);
    }
    return TRUE;
}


// 
//------------------------------------------------------------------*
/**
 * check if files appeared or disappeared in project folder
 *
 *  this is triggered by CNavigator::OnTimer();
 *  uses GetAllFilesFromDisk(), CheckFilesExist() and InsertSortedSiblings()
 *
 * @param            [in] bSilent: if TRUE, no message box if deleted files are detected
 *                                 default is TRUE
 * @see              GetAllFilesFromDisk(), CheckFilesExist(), InsertSortedSiblings
*/
void CFileTreeCtrl::Check(BOOL bSilent /*=FALSE*/)
{
    CSBLLexTree<bool>    tFileNameTreeDisk;
    CSBLLexTree<bool>    tFileNameTreeTree;
//    CSBLLexTree<CXMLNode>   tFileNameTreeDOM;
//    CString             strFileNameTree;
//    CString             strFileNameDisk;
    CString             strFileNameDOM;
    CString             strMessage;
    CString             strFileExtension;

//    CXMLNode*           pNodeTmp;
    CXMLNode            tNodeRoot;
    
//    CTreeCursor         tTreeCursor;
//    CTreeCursor         tNextCursor;
//    CTreeCursor         tParentCursor;
    
    BOOL                bInvalidate = FALSE;
//    BOOL                bTmpFile;
//    int                 iFound;

    TRACE("Check\n");

    // Checking is not allowed or checking is not yet finished -> don't enter
    if(m_iCheckDisableCount || m_bChecking)
    {
        TRACE("Check disabled: -> refused\n");
        return;
    }

    // if no project is loaded -> nothing to do
    if(!m_bLoaded)
        return;

    m_bChecking = TRUE;    // check is running now

    // read all files from disk and store in disk name tree
    GetAllFilesFromDisk(tFileNameTreeDisk, TRUE);

    if(!m_pXMLDoc->GetRootNode(tNodeRoot))
    {
        m_bChecking = FALSE;
        return;
    }

    CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
    CString strStatusText;
    if(pMainFrame)
    {
        strStatusText.LoadString(IDS_CHECKING_SOURCE_FILES);
        pMainFrame->UpdateStatusBar(strStatusText);
    }

    // run over tree control and check files
    // files in tFileNameTreeDisk are deleted as they are found in tree
//    TRACE("Check Source Files\n");
//    TRACE("   No of disk files: %d\n", tFileNameTreeDisk.getNumStrings());
    bInvalidate |= CheckFiles(FV_FILE_SOURCE, tFileNameTreeDisk, bSilent);
//    TRACE("   No of disk files left: %d\n", tFileNameTreeDisk.getNumStrings());
//    TRACE("Check Help Files\n");
    if(pMainFrame)
    {
        strStatusText.LoadString(IDS_CHECKING_HELP_FILES);
        pMainFrame->UpdateStatusBar(strStatusText);
    }
    bInvalidate |= CheckFiles(FV_FILE_HELP, tFileNameTreeDisk, bSilent);
//    TRACE("   No of disk files left: %d\n", tFileNameTreeDisk.getNumStrings());
//    TRACE("Check Additional Files\n");
    if(pMainFrame)
    {
        strStatusText.LoadString(IDS_CHECKING_ADDITIONAL_FILES);
        pMainFrame->UpdateStatusBar(strStatusText);
    }
    bInvalidate |= CheckFiles(FV_FILE_ADDITIONAL, tFileNameTreeDisk, bSilent);
//    TRACE("   No of disk files left: %d\n", tFileNameTreeDisk.getNumStrings());
    // check remaining files
//    TRACE("Check Remaining Files\n");
    if(pMainFrame)
    {
        strStatusText.LoadString(IDS_CHECKING_REST_FILES);
        pMainFrame->UpdateStatusBar(strStatusText);
    }
    bInvalidate |= CheckFiles(FV_FILE_UNKNOWN, tFileNameTreeDisk, bSilent);
//    TRACE("   No of disk files left: %d\n", tFileNameTreeDisk.getNumStrings());

    if(pMainFrame)
    {
        strStatusText.LoadString(IDS_CHECKING_DONE);
        pMainFrame->UpdateStatusBar(strStatusText);
    }

//    if(bInvalidate)
//    {
//        CNavTreeCtrl::UpdateNode("");
//    }
    ExpandNodeFromDom(GetSystemCursor());
    m_bChecking = FALSE;
}


//------------------------------------------------------------------*
/**
 * check files.
 *
 *  first look for deleted files and remove corresponding nodes
 *  then look at new files and insert them
 *
 * @param           [in] eFileCat: file category (source, help, additional, unknown)
 * @param           [in/out] rtFileNameTreeDisk: file name tree containing all files on disk
 *                              file names are deleted as they are found in the tree
 * @param           [in] bSilent: if TRUE, no message box if deleted files are detected
 *                                default is TRUE
 * @return          
 * @exception       -
 * @see             CheckFilesExist(), CheckForNewFiles()
*/
BOOL CFileTreeCtrl::CheckFiles(E_FV_FileCategory eFileCat, CSBLLexTree<bool>& rtFileNameTreeDisk, BOOL bSilent /*=TRUE*/)
{
    BOOL bInvalidate = FALSE;
    CSBLLexTree<bool>    tFileNameTreeTree;
    CSBLLexTree<bool>    tFileNameTreeProject;
    CTreeCursor tTreeCursor;
    tTreeCursor = GetSystemCursor(eFileCat);
    if(!HTREEITEM(tTreeCursor))
    {
        return FALSE;
    }

    switch(eFileCat)
    {
    case FV_FILE_SOURCE:
    case FV_FILE_HELP:
    case FV_FILE_ADDITIONAL:
        GetAllFilesFromProjMan(eFileCat, tFileNameTreeProject);
        bInvalidate = CheckFilesExist(tTreeCursor, tFileNameTreeProject, rtFileNameTreeDisk, tFileNameTreeTree, TRUE, bSilent);
        bInvalidate |= CheckForNewFiles(eFileCat, tFileNameTreeProject, rtFileNameTreeDisk, tFileNameTreeTree);
        break;
    case FV_FILE_UNKNOWN:
        bInvalidate = CheckFilesExist(tTreeCursor, rtFileNameTreeDisk, rtFileNameTreeDisk, tFileNameTreeTree, FALSE, bSilent);
        bInvalidate |= CheckRootForNewFiles(rtFileNameTreeDisk, tFileNameTreeTree);
        break;
    }
    return bInvalidate;
}


//------------------------------------------------------------------*
/**
 * recursive check if all files in the tree exist also on disk and in project.
 *
 *   check if a file in tree also exists in the lexical tree containing files on disk
 *   files that are found are deleted from the tree
 *   files that are not found on disk are removed from the project file name tree
 *   file nodes to not existing files are deleted
 *
 * @param           [in] rtTreeCursor: cursor to tree item where to start
 * @param           [in] rtFileNameTreeProject: file name tree containing all files in project
 * @param           [in/out] rtFileNameTreeDisk: file name tree containing all files on disk
 *                              file names are deleted as they are found in the tree
 * @param           [out] rtFileNameTreeTree: file names found in the tree
 * @param           [in] bSilent: if TRUE, no message box if deleted files are detected
 *                                default is TRUE
 * @return          changes detected?
 * @see             Check()
*/
BOOL CFileTreeCtrl::CheckFilesExist(CTreeCursor& rtTreeCursor, 
                                    CSBLLexTree<bool>& rtFileNameTreeProject,
                                    CSBLLexTree<bool>& rtFileNameTreeDisk,
                                    CSBLLexTree<bool>& rtFileNameTreeTree, 
                                    BOOL bRecursive /*= TRUE*/, 
                                    BOOL bSilent /*=TRUE*/)
{
    CXMLNode*   pNodeTmp;
    CTreeCursor tNextCursor;
    CString     strFileNameTree;
    CString     strTypeName;
    CString     strMessage;
    CString     strType;        // type name of current node
    CString     strSystem;      // value of attribute SYSTEM
    CString     strFileExtension;
    CTreeCursor tTreeCursor;

    BOOL        bFileExist;     // file exists on disk

    BOOL        bInvalidate(FALSE);
    BOOL        bSystemFolder(FALSE);

    bool*       pbTmp = (bool*) 1;  // dummy pointer for insert

//    BOOL        bTmpFile;
//    int         iFound;

    tTreeCursor = rtTreeCursor.GetChild();

    // files removed?
    while(HTREEITEM(tTreeCursor))
    {
        tNextCursor = tTreeCursor.GetNextSibl();
        pNodeTmp = (CXMLNode*)tTreeCursor.GetData();
        pNodeTmp->GetAttribute(CE_XMLATTR_TYPE, strType);
        bSystemFolder = pNodeTmp->GetAttribute(CE_XMLATTR_SYSTEM, strSystem);
        // if folder, call function recursively
        if(bSystemFolder || strType == con_strTypeFolder)
        {
            if(bRecursive)
            {
                bInvalidate |= CheckFilesExist(tTreeCursor, rtFileNameTreeProject, rtFileNameTreeDisk, rtFileNameTreeTree, bRecursive, bSilent);
            }
        }
        else
        {
            bFileExist = TRUE;
            // get file name
            strFileNameTree = tTreeCursor.GetText();
            // if file name tree cannot be deleted from the file name disk tree
            // this file was removed from disk
            // check if file is existing on disk
            if(!rtFileNameTreeDisk.getData(strFileNameTree))
            {
                bFileExist = FALSE;
                TRACE("  CheckFilesExist: %s not existing on disk\n", strFileNameTree);
                // if not on disk, it should also not be in project list
                VERIFY(!rtFileNameTreeProject.deleteData(strFileNameTree));
            }
            if(!rtFileNameTreeProject.getData(strFileNameTree) || !bFileExist || !IsFileVisible(strFileNameTree))
            {
                bInvalidate = TRUE;

//                TRACE("  CheckFilesExist: Delete %s\n", strFileNameTree);
                DeleteXMLNode(pNodeTmp);
                DeleteTreeItem(tTreeCursor);
// TODO: message box, if file change is external
//   up to now we cannot distinguish between internal and external -> no messages
//                bTmpFile = FALSE;
//                iFound = strFileNameTree.ReverseFind(_T('.'));
//                if(iFound >= 0)
//                {
//                    strFileExtension = strFileNameTree.Right(strFileNameTree.GetLength() - iFound - 1);
//                }
//                if(strFileExtension.CompareNoCase(_T("tmp")) == 0)
//                {
//                    bTmpFile = TRUE;
//                }
//
//                if(!bSilent && !bTmpFile)
//                {
//                    // message to user
//                    strMessage.Format(IDS_FILE_REMOVED, strFileNameTree);
//                    AfxMessageBox(strMessage);
//                }
            }
            else
            {
//                TRACE("  CheckFilesExist: Add File to tree %s\n", strFileNameTree);
                rtFileNameTreeTree.insertData(strFileNameTree, pbTmp);
//                TRACE("  CheckFilesExist: Delete File from disk list %s\n", strFileNameTree);
                rtFileNameTreeDisk.deleteData(strFileNameTree);
            }
        }
        tTreeCursor = tNextCursor;
    }
    return bInvalidate;
}


//------------------------------------------------------------------*
/**
 * check for new files.
 *
 *  insert all files from rtFileNameTreeDisk that are not already present in 
 *  rtFileNameTreeTree.
 *  files that are inserted are removed from disk file name tree
 *
 * @param            [in] eFileCat: file category
 * @param            [in] rtFileNameTreeProject: lexical tree of file names in project
 * @param            [in/out] rtFileNameTreeDisk: lexical tree of file names on disk
 *                        for files not found by CheckFilesExist()  
 * @param            [in] rtFileNameTreeTree: lexical tree of file names in tree
 * @return           invalidate needed?
 * @exception        -
 * @see              CheckFilesExist()
*/
BOOL CFileTreeCtrl::CheckForNewFiles(   E_FV_FileCategory eFileCat,
                                        CSBLLexTree<bool>& rtFileNameTreeProject,
                                        CSBLLexTree<bool>& rtFileNameTreeDisk,
                                        CSBLLexTree<bool>& rtFileNameTreeTree,
                                        BOOL bSilent /*=TRUE*/)
{
    CSBLLexTree<bool>    tFileNameTreeProject;
    CStringArray            astrTreeFileNames;
    CString                 strTreeFileName;
    CTreeCursor tTreeCursor;

    BOOL        bInvalidate(FALSE);

    tTreeCursor = GetSystemCursor(eFileCat);
    if(!HTREEITEM(tTreeCursor))
    {
        return FALSE;
    }

    rtFileNameTreeTree.getAllStringsFromSubstring("", astrTreeFileNames);
    int iNumFiles = astrTreeFileNames.GetSize();
    int iFile;

    for(iFile = 0; iFile < iNumFiles; ++iFile)
    {
        strTreeFileName = astrTreeFileNames[iFile];
        rtFileNameTreeProject.deleteData(strTreeFileName);
    }

    CStringArray    astrNewFileNames;
    rtFileNameTreeProject.getAllStringsFromSubstring("", astrNewFileNames);

    iNumFiles = astrNewFileNames.GetSize();
//    TRACE("   CheckForNewFiles: Insert %d Files\n", iNumFiles);
    if(iNumFiles > 0)
    {
        InsertSortedChildren(tTreeCursor, astrNewFileNames);
        // now delete inserted files from disk file name tree
        iNumFiles = astrNewFileNames.GetSize();
        for(iFile = 0; iFile < iNumFiles; ++iFile)
        {
            rtFileNameTreeDisk.deleteData(astrNewFileNames[iFile]);
        }
        return TRUE;
    }
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * check root for new files.
 *
 *  insert all files from rtFileNameTreeDisk that are not already present in 
 *  rtFileNameTreeTree.
 *  files that are inserted are removed from disk file name tree
 *
 * @param            [in/out] rtFileNameTreeDisk: lexical tree of file names on disk
 *                        for files not found by CheckFilesExist()  
 * @param            [in] rtFileNameTreeTree: lexical tree of file names in tree
 * @return           invalidate needed?
 * @exception        -
 * @see              CheckFilesExist()
*/
BOOL CFileTreeCtrl::CheckRootForNewFiles(CSBLLexTree<bool>& rtFileNameTreeDisk,
                                        CSBLLexTree<bool>& rtFileNameTreeTree,
                                        BOOL bSilent /*=TRUE*/)
{
    CStringArray            astrTreeFileNames;
    CString                 strTreeFileName;
    CTreeCursor tTreeCursor;

    BOOL        bInvalidate(FALSE);

    tTreeCursor = GetSystemCursor();
    if(!HTREEITEM(tTreeCursor))
    {
        return FALSE;
    }

    rtFileNameTreeTree.getAllStringsFromSubstring("", astrTreeFileNames);
    int iNumFiles = astrTreeFileNames.GetSize();
    int iFile;

    for(iFile = 0; iFile < iNumFiles; ++iFile)
    {
        strTreeFileName = astrTreeFileNames[iFile];
        rtFileNameTreeDisk.deleteData(strTreeFileName);
    }

    CStringArray    astrNewFileNames;
    rtFileNameTreeDisk.getAllStringsFromSubstring("", astrNewFileNames);

    iNumFiles = astrNewFileNames.GetSize();
    TRACE("   CheckForNewFiles: Insert %d Files\n", iNumFiles);
    if(iNumFiles > 0)
    {
        InsertSortedChildren(tTreeCursor, astrNewFileNames);
        return TRUE;
    }
    return FALSE;
}


CTreeCursor CFileTreeCtrl::GetSystemCursor(E_FV_FileCategory eFileCat /*=FV_FILE_UNKNOWN*/)
{
    CTreeCursor   tCursorSystem;

    if(!HTREEITEM(m_tCursorRoot))
    {
        return tCursorSystem;
    }

    switch(eFileCat)
    {
    case FV_FILE_SOURCE:
        FindChildFromId(m_tCursorRoot, tCursorSystem, con_strSourceFileFolder);
        return tCursorSystem;
    case FV_FILE_HELP:
        FindChildFromId(m_tCursorRoot, tCursorSystem, con_strHelpFileFolder);
        return tCursorSystem;
    case FV_FILE_ADDITIONAL:
        FindChildFromId(m_tCursorRoot, tCursorSystem, con_strAdditionalFileFolder);
        return tCursorSystem;
    }
    return(m_tCursorRoot);
}

//------------------------------------------------------------------*
/**
 * insert sorted files as siblings starting from rtTreeCursor
 *
 *  XML file nodes are created for all files
 *  tree items are created and node pointers are set
 *  used in Check()
 *
 * @param           [in] rtTreeCursor: tree cursor to starting item
 * @param           [in] rastrFileNames: string array of sorted files to be inserted
 * @see             Check()
*/
void CFileTreeCtrl::InsertSortedSiblings(CTreeCursor& rtTreeCursor, CStringArray& rastrFileNames)
{
    CTreeCursor     tTreeCursor = rtTreeCursor;
    CTreeCursor     tNextCursor;
    CTreeCursor     tParentCursor;
    long            lFile;
    long            lNumFiles = rastrFileNames.GetSize();
    CString         strFileNameNew;
    CString         strFileNameTree;
    CXMLNode        tNodeParent;
    CXMLNode*       pNodeNext;
    CXMLNode*       pNodeTmp;
    int             iImageId;
    int             iOverlayId;
    CString         strState;

    tNextCursor = tTreeCursor.GetNextSibl();
    for(lFile = 0; lFile < lNumFiles; ++lFile)
    {
        strFileNameNew = rastrFileNames[lFile];
        // search insertion point:
        while(HTREEITEM(tNextCursor))
        {
            strFileNameTree = tNextCursor.GetText();
            if(strFileNameNew.CompareNoCase(strFileNameTree) > 0)
                break;
            tTreeCursor = tNextCursor;
            tNextCursor = tNextCursor.GetNextSibl();
        }
        
        // insert XML-DOM node
        pNodeTmp = (CXMLNode*)tTreeCursor.GetData();
        ASSERT(pNodeTmp);
        if(HTREEITEM(tNextCursor))
            pNodeNext = (CXMLNode*)tNextCursor.GetData();
        else
            pNodeNext = 0;
        pNodeTmp->GetParentNode(tNodeParent);
        pNodeTmp = InsertXMLSiblingNodeBefore(&tNodeParent, pNodeNext, strFileNameNew, con_strTypeFile);

        // if the file name is passing the filters, insert new tree item
        if(IsPassingFilters(strFileNameNew, con_strTypeFile))
        {
            tParentCursor = tTreeCursor.GetParent();
            m_pKADInfo->GetImageIdsForType(con_strTypeFile, strState, iImageId, iOverlayId);
            tNextCursor = InsertItemAfter(strFileNameNew, tParentCursor, HTREEITEM(tTreeCursor), *pNodeTmp, iImageId);
            pNodeTmp->GetIdPath(m_strInsertIdPath);
            // store XML-element pointer in node and in name tree
            //m_tNameTree.insertData(strFileNameNew, pNodeTmp);
        }
        else
        {
            if(pNodeTmp)
            {
                delete pNodeTmp;
            }
        }
    }
}


void CFileTreeCtrl::InsertSortedChildren(CTreeCursor& rtParent, CStringArray& rastrFileNames)
{
    CXMLNode*   pNodeParent = (CXMLNode*)rtParent.GetData();
    CXMLNode    tNewNodeTmp;
    long        lNumFiles = rastrFileNames.GetSize();
    long        lFile;
    CString     strFileNameNew;

    for(lFile = 0; lFile < lNumFiles; ++lFile)
    {
        strFileNameNew = rastrFileNames[lFile];
//        TRACE("    InsertSortedChildren: Insert %s\n", strFileNameNew);
        InsertNewXMLChildNode(pNodeParent, strFileNameNew, con_strTypeFile, tNewNodeTmp);
    }
    UpdateTreeNode(rtParent, *pNodeParent);
}


//------------------------------------------------------------------*
/**
 * is file visible.
 *
 * @param           [in] crstrFileName: file name
 * @return          is file visible?
 * @exception       -
 * @see             
*/
BOOL CFileTreeCtrl::IsFileVisible(const CString& crstrFileName)
{
    CString strExtension;
    int     iFound = crstrFileName.ReverseFind(_T('.'));
    CKADFileExtInfo*    pFileExtInfo;
    if(iFound >= 0)
    {
        strExtension = crstrFileName.Mid(iFound+1);
        pFileExtInfo = m_tFileInfo.GetFileExtInfo(strExtension);
        if(pFileExtInfo)
        {
            return(pFileExtInfo->m_bVisible);
        }
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * delete XML node.
 *
 *  removes node from DOM
 *
 * @param           [in] pNode: pointer to XML node to be deleted
 * @return          -
*/
BOOL CFileTreeCtrl::DeleteXMLNode(CXMLNode* pNode)
{
    CXMLNode        tNodeParent;

    ASSERT(pNode);
    if(pNode->GetParentNode(tNodeParent))
    {
        return tNodeParent.RemoveChild(*pNode);
    }
    return FALSE;
}

// 
//------------------------------------------------------------------*
/**
 * insert XML sibling node before a node
 *
 * @param           [in] pNodeParent: parent node
 * @param           [in] pBefore: child node before the new sibling node is inserted
 *                                if this one is NULL, the node is inserted as first
 *                  [in] rstrTextInsert: name = text of the new node
 *                  [in] rstrTypeInsert: type id string of the new node
 * @return          pointer to new node
*/
CXMLNode* CFileTreeCtrl::InsertXMLSiblingNodeBefore(CXMLNode* pNodeParent, CXMLNode* pBefore, const CString& rstrTextInsert, const CString& rstrTypeInsert)
{
    CXMLNode    tNodeNewTmp;
    CXMLNode*   pNodeNew;

    if(!m_pXMLDoc->CreateNode(tNodeNewTmp, CE_XMLTAG_NODE))
    {
        return 0;
    }

    // set name attribute
    tNodeNewTmp.SetAttribute(CE_XMLATTR_TEXT, rstrTextInsert);
    tNodeNewTmp.SetAttribute(CE_XMLATTR_ID, rstrTextInsert);
    tNodeNewTmp.SetAttribute(CE_XMLATTR_TYPE, rstrTypeInsert);
    if(rstrTypeInsert.Compare(con_strTypeFile) == 0)
    {
        tNodeNewTmp.SetAttribute(CE_XMLATTR_SOURCE, rstrTextInsert);
    }

    if(pBefore)
    {
        if(!pNodeParent->InsertChildBefore(tNodeNewTmp, *pBefore))
        {
            return 0;
        }
    }
    else
    {
        CXMLNode    tTmp;
        if(!pNodeParent->InsertChildBefore(tNodeNewTmp, tTmp))
        {
            return 0;
        }
    }
    pNodeNew = new CXMLNode;
    *pNodeNew = tNodeNewTmp;
    return(pNodeNew);
 }


/*
// check if files appeared or disappeared in project folder
// this is triggered by CNavigator::OnTimer();
// old version with BuildTree()
void CFileTreeCtrl::Check()
{
    CSBLLexTree<bool>           tFileNamesDisk;
    CStringArray                astrFileNamesDOMLower;
    CStringArray                astrFileNamesLeftLower;
    CString                     strFileName;
    HRESULT                     hr;
    CXMLNode*                pChildTmp;
    CXMLNode*                pNodeParent;
    CXMLNode*             pNodeTmp;
    BOOL                        bUpdate = FALSE;
    long                        lNumFiles_dom;
    long                        lNumFiles;
    long                        lFile;

    // read all files from disk and store in disk name tree
    GetAllFilesFromDisk(tFileNamesDisk);
    // get all file names from name tree (DOM contains the same)
    m_tNameTree.getAllStringsFromSubstring("", astrFileNamesDOMLower);

    // run over all files from the DOM and delete them from disk name tree
    lNumFiles_dom = astrFileNamesDOMLower.GetSize();
    for(lFile = 0; lFile < lNumFiles_dom; ++lFile)
    {
        strFileName = astrFileNamesDOMLower[lFile];
        // if DOM file name cannot be deleted in disk name tree, it
        // was removed from the project directory
        if(!tFileNamesDisk.deleteData(strFileName))
        {
            // tree has to be updated
            bUpdate = TRUE;
            // now remove file from the tree
            pNodeTmp = m_tNameTree.getData(strFileName);
            ASSERT(pNodeTmp);
            hr = pNodeTmp->get_parentNode(&pNodeParent);
            ASSERT(hr == S_OK);
            if(pNodeParent)
            {
                hr = tNodeParent.removeChild(pNodeTmp, &pChildTmp);
                ASSERT(hr == S_OK);
                tNodeParent.Release();
                m_tNameTree.deleteData(strFileName);
            }
            pChildTmp->Release();
            pNodeTmp->Release();
            
            CString strMessage;
            strMessage.Format("File %s was removed from outside", strFileName);
            AfxMessageBox(strMessage);
        }
    }

    // get all files not yet deleted from the disk file tree
    // these are new
    tFileNamesDisk.getAllStringsFromSubstring("", astrFileNamesLeftLower);
    lNumFiles = astrFileNamesLeftLower.GetSize();
    if(lNumFiles > 0)
        bUpdate = TRUE;    // new file(s) -> update required

    for(lFile = 0; lFile < lNumFiles; ++lFile)
    {
        strFileName = astrFileNamesLeftLower[lFile];
        // this is a new file -> add it to the name tree and put it into the root folder
        InsertNewXMLChildNode(m_pNodeRoot, strFileName, con_strTypeFile);
    }
    // now update tree if required
    if(bUpdate)
        BuildTree();
}*/

//------------------------------------------------------------------*
/**
 * insert XML child node
 *
 *  if child node with name already exists, it is not inserted
 *
 * @param           [in] pNodeParent: pointer to XML parent node
 * @param           [in] rstrTextInsert: name of the node (= attribute TEXT)
 * @param           [in] rtNodeNew: new node
 * @param           [in] bSorted: if bSorted == FALSE, insert as last else insert sorted
 *                                first folders then files, each group sorted
 * @return          succeeded?
 * @see             
*/
BOOL CFileTreeCtrl::InsertNewXMLChildNode(CXMLNode* pNodeParent, const CString& rstrTextInsert, const CString& rstrTypeInsert, CXMLNode& rtNodeNew, BOOL bSorted /*=TRUE*/)
{
    CString     strType;                // node name of current node = type (FILE, FOLDER, SYSTEMFOLDER)
    BOOL        bReturn = FALSE;        // succeeded?

    if(!m_pXMLDoc->CreateNode(rtNodeNew, CE_XMLTAG_NODE))
    {
        return(bReturn);
    }

    // set name attribute
    rtNodeNew.SetAttribute(CE_XMLATTR_TEXT, rstrTextInsert);
    rtNodeNew.SetAttribute(CE_XMLATTR_ID, rstrTextInsert);
    rtNodeNew.SetAttribute(CE_XMLATTR_TYPE, rstrTypeInsert);
    if(rstrTypeInsert.Compare(con_strTypeFile) == 0)
    {
        rtNodeNew.SetAttribute(CE_XMLATTR_SOURCE, rstrTextInsert);
        if(!IsFileVisible(rstrTextInsert))
        {
            rtNodeNew.SetAttribute(CE_XMLATTR_VISIBILITY, CE_XMLATTR_FALSE_VAL);
        }
    }

    return(InsertXMLChildNode(pNodeParent, &rtNodeNew, bSorted));
}



//------------------------------------------------------------------*
/**
 * get node type tree item (= type of corresponding XML-node)
 *
 * @param           [in] tTreeCursor: cursor to tree item
 * @return          type id string
*/
CString CFileTreeCtrl::GetNodeType(CTreeCursor& tTreeCursor)
{
    CXMLNode*   pNode;
    CString     strType;
    if(HTREEITEM(tTreeCursor))
    {
        pNode = (CXMLNode*)tTreeCursor.GetData();
        if(pNode)
        {
            pNode->GetAttribute(CE_XMLATTR_TYPE, strType);
        }
    }
    return(strType);
}


//------------------------------------------------------------------*
/**
 * add context menu and show it
 *
 *  overwritten method of base class CNavTreeCtrl
 *  context menu is hard coded and not loaded from KAD
 *
 * @param           [in] pMenu: pointer to context menu
 * @return          successful?
 * @see             CNavTreeCtrl::AddContextMenu()
*/
BOOL CFileTreeCtrl::AddContextMenu(CMenu* pMenu)
{
    BOOL    bOk = FALSE;
    CPoint  CurrPoint;
    CPoint	OrigPoint;
    CString strTmp;
    CTreeCursor tContextCursor;
    CString strType;
    CString strItem;    // menu item text
    DWORD   dwStyle = MF_STRING;
    BOOL    bExpanded;

	tContextCursor = GetSelectedItem();
    if(!HTREEITEM(tContextCursor))
    {
        return FALSE;
    }

    if(tContextCursor.GetState(TVIS_EXPANDED) & TVIS_EXPANDED)
    {
        bExpanded = TRUE;
    }
    else
    {
        bExpanded = FALSE;
    }

    if(!SetContextCursor(tContextCursor))
    {
        return FALSE;
    }

    if(IsProjectReadOnly())
    {
        dwStyle = MF_STRING | MF_GRAYED;
    }
    
    // insert expand all / collapse all
//    strTmp.LoadString(IDS_EXPAND_ALL);
//    pMenu->AppendMenu(MF_STRING, ID_NAV_CONTEXT_BASE + KSO_EXPAND_ALL, strTmp);
//    strTmp.LoadString(IDS_COLLAPSE_ALL);
//    pMenu->AppendMenu(MF_STRING, ID_NAV_CONTEXT_BASE + KSO_COLLAPSE_ALL, strTmp);

    switch(m_eContextType)
    {
    case E_NAV_FV_FILE:
        {
            BOOL bReadOnly = IsFileReadOnly(tContextCursor);
//            pMenu->AppendMenu(MF_SEPARATOR);
            if(bExpanded)
            {
                strItem.LoadString(IDS_FV_MENU_CLOSE);
            }
            else
            {
                strItem.LoadString(IDS_FV_MENU_OPEN);
            }
            pMenu->AppendMenu(MF_STRING, ID_NAV_FV_OPEN, strItem);
            strItem.LoadString(IDS_FV_MENU_OPEN_FILE);
            pMenu->AppendMenu(MF_STRING, ID_NAV_FV_OPEN_TEXT, strItem);
            pMenu->AppendMenu(MF_SEPARATOR);
            strItem.LoadString(IDS_FV_MENU_CUT);
            pMenu->AppendMenu(dwStyle, ID_NAV_FV_CUT, strItem);
//            strItem.LoadString(IDS_FV_MENU_COPY);
//            pMenu->AppendMenu(MF_STRING, ID_NAV_FV_COPY, "&Copy");
            if(bReadOnly)
            {
                pMenu->AppendMenu(MF_SEPARATOR);
                strItem.LoadString(IDS_FV_MENU_DELETE);
                pMenu->AppendMenu(dwStyle | MF_DISABLED | MF_GRAYED, ID_NAV_FV_DELETE, strItem);
            }
            else
            {
                pMenu->AppendMenu(MF_SEPARATOR);
                strItem.LoadString(IDS_FV_MENU_DELETE);
                pMenu->AppendMenu(dwStyle, ID_NAV_FV_DELETE, strItem);
            }
//            strItem.LoadString(IDS_FV_MENU_RENAME);
//            pMenu->AppendMenu(MF_STRING, ID_NAV_FV_RENAME, "&Rename");
            pMenu->AppendMenu(MF_SEPARATOR);
            strItem.LoadString(IDS_FV_MENU_PROPERTIES);
            pMenu->AppendMenu(MF_STRING, ID_NAV_FV_PROPERTIES, strItem);
            bOk = TRUE;
        }
        break;
    case E_NAV_FV_FOLDER:
//        pMenu->AppendMenu(MF_SEPARATOR);
        if(bExpanded)
        {
            strItem.LoadString(IDS_FV_MENU_CLOSE);
        }
        else
        {
            strItem.LoadString(IDS_FV_MENU_OPEN);
        }
        pMenu->AppendMenu(MF_STRING, ID_NAV_FV_OPEN, strItem);
        strItem.LoadString(IDS_FV_MENU_ADD_FILE);
        pMenu->AppendMenu(dwStyle, ID_NAV_FV_ADD_FILE, strItem);
        pMenu->AppendMenu(MF_SEPARATOR);
        strItem.LoadString(IDS_FV_MENU_CUT);
        pMenu->AppendMenu(dwStyle, ID_NAV_FV_CUT, strItem);
        strItem.LoadString(IDS_FV_MENU_PASTE);
        if(IsClipboardDataAvailable(m_uiCBXMLNodeFormat))
        {
            pMenu->AppendMenu(dwStyle, ID_NAV_FV_PASTE, strItem);
        }
        else
        {
            pMenu->AppendMenu(dwStyle | MF_GRAYED, ID_NAV_FV_PASTE, strItem);
        }
        pMenu->AppendMenu(MF_SEPARATOR);
        strItem.LoadString(IDS_FV_MENU_DELETE);
        pMenu->AppendMenu(dwStyle, ID_NAV_FV_DELETE, strItem);
        strItem.LoadString(IDS_FV_MENU_RENAME);
        pMenu->AppendMenu(dwStyle, ID_NAV_FV_RENAME, strItem);
        pMenu->AppendMenu(MF_SEPARATOR);
        strItem.LoadString(IDS_FV_MENU_CREATE_FOLDER);
        pMenu->AppendMenu(dwStyle, ID_NAV_FV_CREATE_FOLDER, strItem);
        bOk = TRUE;
        break;
    case E_NAV_FV_SOURCE_GROUP:
    case E_NAV_FV_HELP_GROUP:
    case E_NAV_FV_ADDITIONAL_GROUP:
//        pMenu->AppendMenu(MF_SEPARATOR);
        if(bExpanded)
        {
            strItem.LoadString(IDS_FV_MENU_CLOSE);
        }
        else
        {
            strItem.LoadString(IDS_FV_MENU_OPEN);
        }
        pMenu->AppendMenu(MF_STRING, ID_NAV_FV_OPEN, strItem);
        strItem.LoadString(IDS_FV_MENU_ADD_FILE);
        pMenu->AppendMenu(dwStyle, ID_NAV_FV_ADD_FILE, strItem);
        pMenu->AppendMenu(MF_SEPARATOR);
        strItem.LoadString(IDS_FV_MENU_PASTE);
        if(IsClipboardDataAvailable(m_uiCBXMLNodeFormat))
        {
            pMenu->AppendMenu(dwStyle, ID_NAV_FV_PASTE, strItem);
        }
        else
        {
            pMenu->AppendMenu(dwStyle | MF_GRAYED, ID_NAV_FV_PASTE, strItem);
        }
        pMenu->AppendMenu(MF_SEPARATOR);
        strItem.LoadString(IDS_FV_MENU_CREATE_FOLDER);
        pMenu->AppendMenu(dwStyle, ID_NAV_FV_CREATE_FOLDER, strItem);
        bOk = TRUE;
        break;
    case E_NAV_FV_ROOT:
//        pMenu->AppendMenu(MF_SEPARATOR);
//        strItem.LoadString(IDS_FV_MENU_OPEN);
//        pMenu->AppendMenu(MF_STRING, ID_NAV_FV_OPEN, strItem);
        strItem.LoadString(IDS_FV_MENU_ADD_FILE);
        pMenu->AppendMenu(dwStyle, ID_NAV_FV_ADD_FILE, strItem);
        pMenu->AppendMenu(MF_SEPARATOR);
        strItem.LoadString(IDS_FV_MENU_PASTE);
        if(IsClipboardDataAvailable(m_uiCBXMLNodeFormat))
        {
            pMenu->AppendMenu(dwStyle, ID_NAV_FV_PASTE, strItem);
        }
        else
        {
            pMenu->AppendMenu(dwStyle | MF_GRAYED, ID_NAV_FV_PASTE, strItem);
        }
        bOk = TRUE;
        break;
    }
    return bOk;
}

// get type id from type name
EFVNodeType CFileTreeCtrl::GetTypeFromTypeName(const CString& rstrType)
{
    CString strCmp;

    // files
    if(rstrType == con_strTypeFile)
        return(E_NAV_FV_FILE);

    // root folder
    if(rstrType == con_strTypeFileProject)
        return(E_NAV_FV_ROOT);

    // system folder
    if(rstrType == con_strTypeSourceGroup)
        return(E_NAV_FV_SOURCE_GROUP);
    if(rstrType == con_strTypeHelpGroup)
        return(E_NAV_FV_HELP_GROUP);
    if(rstrType == con_strTypeAdditionalGroup)
        return(E_NAV_FV_ADDITIONAL_GROUP);

    // normal folders
    if(rstrType == con_strTypeFolder)
        return(E_NAV_FV_FOLDER);

    // else
    return(E_NAV_FV_UNKNOWN);
}

//------------------------------------------------------------------*
/**
 * 
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CFileTreeCtrl::OnCommand(UINT nID)
{
    CString     strCommand;

    if(!HTREEITEM(m_tContextCursor))
        return;

    // check expand all / collapse all
    if(nID == ID_EXPAND_ALL)
    {
        ExpandTreeItem(m_tContextCursor, TVE_EXPAND, TRUE);
        return;
    }
    if(nID == ID_COLLAPSE_ALL)
    {
        ExpandTreeItem(m_tContextCursor, TVE_COLLAPSE, TRUE);
        return;
    }

    switch(nID)
    {
    case ID_NAV_FV_OPEN:
        {
            switch(m_eContextType)
            {
            case E_NAV_FV_FILE:
                {
                    CString strFileName = m_tContextCursor.GetText();
                    OpenSourceFile(strFileName);
                }
                break;
            case E_NAV_FV_FOLDER:
            case E_NAV_FV_SOURCE_GROUP:
            case E_NAV_FV_HELP_GROUP:
            case E_NAV_FV_ADDITIONAL_GROUP:
                ExpandTreeItem(m_tContextCursor, TVE_TOGGLE);
                Invalidate();
                break;
            }
        }
        break;
    case ID_NAV_FV_OPEN_TEXT:
        if (m_eContextType==E_NAV_FV_FILE)
        {
            CString strFileName = m_tContextCursor.GetText();
            OpenSourceFileAsText(strFileName);
        }
        break;
    case ID_NAV_FV_CUT:
        OnCut();
        break;
    case ID_NAV_FV_COPY:
        OnCopy();
        break;
    case ID_NAV_FV_PASTE:
        OnPaste();
        break;
    case ID_NAV_FV_DELETE:
        switch(m_eContextType)
        {
        case E_NAV_FV_FILE:
        case E_NAV_FV_FOLDER:
            DeleteContextNode();
            break;
        }
        break;
    case ID_NAV_FV_RENAME:
        // rename: only allowed for folders
        switch(m_eContextType)
        {
        case E_NAV_FV_FOLDER:
            {
                EditLabel(m_tContextCursor);
            }
            break;
        }
        break;
    case ID_NAV_FV_PROPERTIES:
        {
            CFileStatusDlg    tDlg;
            CString strFileName = m_tContextCursor.GetText();
            tDlg.ShowProperties(strFileName, m_strProjectPath);
        }
        break;
    case ID_NAV_FV_ADD_FILE:
        OnAddFile();
        break;
    case ID_NAV_FV_CREATE_FOLDER:
        CreateFolderNode();
        break;
    }
}


//------------------------------------------------------------------*
/**
 * on add file.
 *
 *  add file context menu message handler.
 *
*/
void CFileTreeCtrl::OnAddFile()
{
    CXMLNode*           pNode;
    E_FV_FileCategory   eFileCat;
    CString             strFilter;
    CString             strTitle;
    CString             strFileName;    // file name only
    CString             strFilePath;    // file incl path
    CString             strFilePathTmp; // path only
    CString             strInitDir;     // initial directory for file selection

    if(!HTREEITEM(m_tContextCursor))
    {
        return;
    }

    pNode = (CXMLNode*)m_tContextCursor.GetData();
    ASSERT(pNode);

    if(pNode)
    {
        CFileDialog tFileDlg(TRUE);

        eFileCat = GetFileCategory(pNode);
        tFileDlg.m_ofn.nMaxCustFilter = GetFileFilterForCategory(eFileCat, strFilter);

        strTitle.LoadString(IDS_ADD_FILE_TITLE);
	    tFileDlg.m_ofn.lpstrFilter = strFilter;
	    tFileDlg.m_ofn.lpstrTitle = strTitle;
        ::GetCurrentDirectory(_MAX_PATH, strInitDir.GetBuffer(_MAX_PATH));
        strInitDir.ReleaseBuffer();
        tFileDlg.m_ofn.lpstrInitialDir = strInitDir;
    
    	tFileDlg.m_ofn.Flags |= (OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT);

        tFileDlg.m_ofn.lpstrFile = new char[2048];
        tFileDlg.m_ofn.lpstrFile[0] = 0;
        tFileDlg.m_ofn.nMaxFile = 2048;

        // show dialog
        int nResult = tFileDlg.DoModal();
    	
        if(nResult == IDOK)
        {
            BOOL    bReparse = FALSE;
            CFile   tFile;
            EnableFileCheck(FALSE);

            SaveProjectFile();

            POSITION pos = tFileDlg.GetStartPosition();
            while(pos)
            {
                strFilePath = tFileDlg.GetNextPathName(pos);
                tFile.SetFilePath(strFilePath);
                strFileName = tFile.GetFileName();
                TRACE("add file: %s\n", strFilePath);
                // copy file in project directory
                if(CopyFileToProject(strFilePath))
                {
                    // now add file to project
                    E_FV_FileCategory   eFileCatProject;
                    CComBSTR    sFileName = strFileName;
                    m_pProjMan->GetFileCategory(sFileName, &eFileCatProject);
                    if(ChangeFileCategory(strFileName, eFileCatProject, eFileCat))
                    {
                        // remove node
                        CXMLNode   tNode;
                        if(GetSystemNode(FV_FILE_UNKNOWN, tNode))
                        {
                            if(FindFileNode(strFileName, tNode))
                            {
                                DeleteXMLNode(&tNode);
                            }
                        }

                        CTreeCursor tTreeCursor;
                        if(FindFileItem(strFileName, tTreeCursor))
                        {
                            DeleteTreeItem(tTreeCursor);
                        }
                        // then insert node
                        CXMLNode    tNodeNewTmp;
                        InsertNewXMLChildNode(pNode, strFileName, con_strTypeFile, tNodeNewTmp);
                        UpdateTreeNode(m_tContextCursor, *pNode);
                        bReparse = TRUE;
                    }
                }
            }
            if(bReparse)
            {
                m_pProjInfo->fileParseNow(NULL);
            }
            EnableFileCheck(TRUE);
        }
        delete [] tFileDlg.m_ofn.lpstrFile;
        tFileDlg.m_ofn.lpstrFile = NULL;
    }
}


//------------------------------------------------------------------*
/**
 * get file filter for category
 *
 * @param           [in] eCategory: file category
 * @return          filter for file dialog.
*/
int CFileTreeCtrl::GetFileFilterForCategory(E_FV_FileCategory eCategory, CString& rstrFilter)
{
    int         iNumFilters = 0;
   	CString     allFilter;

    rstrFilter = _T("");

    switch(eCategory)
    {
    case FV_FILE_SOURCE:
        iNumFilters = m_tFileInfo.GetFileFilterForSourceFiles(rstrFilter);
        return(iNumFilters);
    }

    iNumFilters = m_tFileInfo.GetFileFilterForAllFiles(rstrFilter);
    VERIFY(allFilter.LoadString(IDS_ALLFILTER));
	rstrFilter += allFilter;
	rstrFilter += (TCHAR)'\0';   // next string please
	rstrFilter += _T("*.*");
	rstrFilter += (TCHAR)'\0';   // last string
    ++iNumFilters;

    return(iNumFilters);
}


//------------------------------------------------------------------*
/**
 * rename file.
 *
 * @param           [in] rtTreeCursor: cursor to tree item, that has to be renamed
 * @param           [in] crstrNewFile: new file name
 * @return          successful?
*/
BOOL CFileTreeCtrl::RenameFile(CTreeCursor& rtTreeCursor, const CString& crstrNewFileName)
{
    BOOL        bReturn = FALSE;
    if(!HTREEITEM(rtTreeCursor))
    {
        return FALSE;
    }
    CXMLNode*   pNode = (CXMLNode*)rtTreeCursor.GetData();

    ASSERT(pNode);
    if(!pNode)
    {
        return FALSE;
    }

    SaveProjectFile();

    // first get file category
    E_FV_FileCategory   eFileCat;
    CString             strFileName;
    CString             strFilePath;
    CString             strInsertIdPath;

    pNode->GetAttribute(CE_XMLATTR_TEXT, strFileName);
    strFilePath = GetFilePath(strFileName);

    if(crstrNewFileName.CompareNoCase(strFileName) == 0)
    {
        return FALSE;
    }

    eFileCat = GetFileCategory(pNode);
    switch(eFileCat)
    {
    case FV_FILE_UNKNOWN:
        // just rename
        break;
    default:
        // warning
        {
            CString strMessage;
            strMessage.Format(IDS_RENAME_FILE_WARN, strFileName);
            if(::AfxMessageBox(strMessage, MB_YESNO) != IDYES)
            {
                return FALSE;
            }
        }
        break;
    }

    EnableFileCheck(FALSE);

    // now rename file
    CString     strNewFilePath = GetFilePath(crstrNewFileName);
    CComBSTR    sFile(strFileName);
    CComBSTR    sFileNew(crstrNewFileName);
    HRESULT     hr;
    BOOL        bReparse = FALSE;

    switch(eFileCat)
    {
    case FV_FILE_SOURCE:
    case FV_FILE_HELP:
    case FV_FILE_ADDITIONAL:
        // first try to remove file from project manager
        hr = m_pProjMan->RemoveFile(sFile, eFileCat, TRUE);
        if(hr == S_OK)
        {
            bReparse = TRUE;
            if(::MoveFile(strFilePath, strNewFilePath))
            {
                // now rename
                hr = m_pProjMan->RemoveFile(sFile, eFileCat, FALSE);
                hr = m_pProjMan->AddFile(sFileNew, eFileCat, FALSE);
                if(hr == S_OK)
                {
                    bReturn = TRUE;
                }
            }
        }
        if(bReturn == FALSE)
        {
            CString strMessage;
            strMessage.Format(IDS_RENAME_FILE_DENIED, strFileName);
            ::AfxMessageBox(strMessage);
        }
        break;
    case FV_FILE_UNKNOWN:
        ::MoveFile(strFilePath, strNewFilePath);
        bReturn = TRUE;
        break;
    }

    if(bReturn)
    {
        CXMLNode    tNodeParent;
        pNode->SetAttribute(CE_XMLATTR_TEXT, crstrNewFileName);
        pNode->SetAttribute(CE_XMLATTR_ID, crstrNewFileName);
        pNode->SetAttribute(CE_XMLATTR_SOURCE, crstrNewFileName);
        pNode->GetIdPath(strInsertIdPath);
        rtTreeCursor.SetText(crstrNewFileName);

        if(pNode->GetParentNode(tNodeParent))
        {
            DeleteXMLNode(pNode);
            InsertXMLChildNode(&tNodeParent, pNode);
            CTreeCursor tCursorParent = rtTreeCursor.GetParent();
            ASSERT(HTREEITEM(tCursorParent));
            if(HTREEITEM(tCursorParent))
            {
                UpdateTreeNode(tCursorParent, tNodeParent);
            }
        }
    }

    if(bReparse)
    {
        m_pProjInfo->fileParseNow(NULL);
    }

    EnableFileCheck(TRUE);

    m_strInsertIdPath = strInsertIdPath;
    SelectLastInsertNode();

    return bReturn;
}


//------------------------------------------------------------------*
/**
 * rename folder node
 *
 *  this is called to rename a folder node
 *  called by OnEndlabeledit()
 *  does not allow folder with already existing name in same parent folder
 *
 * @param           [in] rtTreeCursor: cursor to tree item to be renamed
 * @param           [in] rstrNewName: new name of the tree item
 * @return          successful?
 * @exception       -
 * @see             OnEndlabeledit()
*/
BOOL CFileTreeCtrl::RenameFolderNode(CTreeCursor& rtTreeCursor, const CString& rstrNewName)
{
    if(!HTREEITEM(rtTreeCursor))
    {
        return FALSE;
    }
    CXMLNode*   pNode = (CXMLNode*)rtTreeCursor.GetData();
    CXMLNode    tNodeParent;
    CTreeCursor tParentCursor;
    CTreeCursor tCursorNew;
    CXMLNode    tChildTmp;

    ASSERT(pNode);
    if(!pNode)
    {
        return FALSE;
    }
    pNode->GetParentNode(tNodeParent);
    // check whether folder with new name is already existing
    if(tNodeParent.GetChildWithAttr(CE_XMLATTR_ID, rstrNewName, tChildTmp))
    {
        CString     strType;
        tChildTmp.GetAttribute(CE_XMLATTR_TYPE, strType);
        if(strType == con_strTypeFolder)
        {
            return FALSE;
        }
    }

    pNode->SetAttribute(CE_XMLATTR_TEXT, rstrNewName);
    pNode->SetAttribute(CE_XMLATTR_ID, rstrNewName);

    // remove child
    tNodeParent.RemoveChild(*pNode);
    // insert child again
    InsertXMLChildNode(&tNodeParent, pNode);

    // remove old tree item
    tParentCursor = rtTreeCursor.GetParent();
    ASSERT(HTREEITEM(tParentCursor));
    DeleteTreeItem(rtTreeCursor);

    // update tree node
    UpdateTreeNode(tParentCursor, tNodeParent);

    SelectLastInsertNode();
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * create a new folder below context tree item
 *  
 *  the new folder is named IDS_NEW_FOLDER initially
 *  the user has to change the name afterwards
 *
 * @see             
*/
void CFileTreeCtrl::CreateFolderNode()
{
    // context item contains new text
    if(!HTREEITEM(m_tContextCursor))
    {
        return;
    }
    CString strTextNew;
    strTextNew.LoadString(IDS_NEW_FOLDER);
    CXMLNode*   pNode;
    CTreeCursor tNewChild;

    pNode = (CXMLNode*)m_tContextCursor.GetData();
    if(!pNode)
    {
        return;
    }
    int iCount = 1;
    CString strTextNewTmp = strTextNew;
    CXMLNode    tNodeNewTmp;
    while(!InsertNewXMLChildNode(pNode, strTextNewTmp, con_strTypeFolder, tNodeNewTmp))
    {
        ++iCount;
        strTextNewTmp.Format("%s%d", strTextNew, iCount);
        if(iCount > 100)
        {
            break;
        }
    }
    UpdateTreeNode(m_tContextCursor, *pNode);
    ExpandTreeItem(m_tContextCursor, TVE_EXPAND);
    FindChildFromId(m_tContextCursor, tNewChild, strTextNewTmp);
    Invalidate();
    SelectItem(tNewChild);
    EditLabel(HTREEITEM(tNewChild));
}


//------------------------------------------------------------------*
/**
 * delete context node
 *
 *  only delete tree item with data
 *  tree item without data is only text item "No project loaded"
 *  this should not be deleted
 *
*/
void CFileTreeCtrl::DeleteContextNode()
{
    BOOL    bReparse = FALSE;

    if(HTREEITEM(m_tContextCursor))
    {
        if(m_tContextCursor.GetData())
        {
            switch(m_eContextType)
            {
            case E_NAV_FV_FOLDER:
                {
                    CTreeCursor tParent = m_tContextCursor.GetParent();
                    CSBLLexTree<CXMLNode>   tLexTree;
                    CStringArray        astrFileNames;

                    ASSERT(HTREEITEM(tParent));
                    // run over all children of m_tContextCursor and move children to parent
                    CXMLNode*   pNode = (CXMLNode*)m_tContextCursor.GetData();
                    if(pNode)
                    {
                        CollectFiles(pNode, tLexTree);
                        DeleteXMLNode(pNode);
                        DeleteTreeItem(m_tContextCursor);
                        SetContextCursor(tParent);
                        tLexTree.getAllStringsFromSubstring("", astrFileNames);
                        InsertSortedChildren(tParent, astrFileNames);
                        Invalidate();
                    }
                }
                break;
            case E_NAV_FV_FILE:
                {
                    CString strFileName;
                    CString strFilePath;
                    CString strMessage;

                    CXMLNode*   pNode = (CXMLNode*)m_tContextCursor.GetData();
                    if(pNode)
                    {
                        E_FV_FileCategory   eFileCat;

                        pNode->GetAttribute(CE_XMLATTR_TEXT, strFileName);
                        strFilePath = GetFilePath(strFileName);

                        // is file readonly? -> do not delete it.
                        CFileStatus tStatus;
                        if(CFile::GetStatus(strFilePath, tStatus))
                        {
                            if(tStatus.m_attribute & CFile::readOnly)
                            {
                                ;;MessageBeep(MB_ICONASTERISK);
                                break;
                            }
                        }
                        // is source file?
                        eFileCat = GetFileCategory(pNode);
                        if(eFileCat == FV_FILE_UNKNOWN)
                        {
                            strMessage.Format(IDS_FV_DEL_FILE, strFileName);
                        }
                        else
                        {
                            strMessage.Format(IDS_FV_DEL_FILE_WARN, strFileName);
                        }

                        if(::AfxMessageBox(strMessage, MB_YESNO) == IDYES)
                        {
                            HRESULT     hr = S_OK;
                            if(eFileCat != FV_FILE_UNKNOWN)
                            {
                                CComBSTR    sFile(strFileName);

                                SaveProjectFile();
                                hr = m_pProjMan->RemoveFile(sFile, eFileCat, FALSE);
                                if(hr == S_OK)
                                {
                                    bReparse = TRUE;
                                }
                            }
                            if(hr == S_OK || hr == E_FAIL)
                            {
                                if(RemoveFile(strFilePath))
                                {
                                    DeleteXMLNode(pNode);
                                    DeleteTreeItem(m_tContextCursor);
                                }
                            }
                            else
                            {
                                strMessage.Format(IDS_FV_REFUSE_DEL_FILE, strFileName);
                                ::AfxMessageBox(strMessage);
                            }
                        }
                    }
                }
                break;
            case E_NAV_FV_SOURCE_GROUP:
            case E_NAV_FV_HELP_GROUP:
            case E_NAV_FV_ADDITIONAL_GROUP:
            case E_NAV_FV_ROOT:
                ::MessageBeep(MB_ICONEXCLAMATION);
                break;
            }
        }
    }
    if(bReparse)
    {
        m_pProjInfo->fileParseNow(NULL);
    }
}


//------------------------------------------------------------------*
/**
 * remove file.
 *
 * @param            [in] crstrPathName: file name incl. path
 * @param            [in] bRecycler: file is to put into recycle bin?
 * @return           successful?
 * @exception        -
 * @see              
*/
BOOL CFileTreeCtrl::RemoveFile(const CString& crstrPathName, BOOL bRecycler /*=TRUE*/)
{
//    if(bRecycler)
//    {
//        SHFILEOPSTRUCT  tFileOpStruct;
//        ::memset(&tFileOpStruct, 0, sizeof(tFileOpStruct));
//        tFileOpStruct.hwnd = GetSafeHwnd();
//        tFileOpStruct.wFunc = FO_DELETE;
//        tFileOpStruct.pFrom = crstrPathName;
//        tFileOpStruct.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION;
//        // move file to recycled bin
//        if(!SHFileOperation(&tFileOpStruct))
//        {
//            return FALSE;
//        }
//        return TRUE;
//    }
    // if file does not exist, return true to enable node to be deleted.
    CFileStatus tStatus;
    if(!CFile::GetStatus(crstrPathName, tStatus))
    {
        return TRUE;
    }

    BOOL bReturn = ::DeleteFile(crstrPathName);
    if(bReturn)
    {
        CCEFileHandler*     pFileHandler = ((CCEMAINApp*)AfxGetApp())->GetFileHandler();
        ASSERT(pFileHandler);
        if(pFileHandler)
        {
            pFileHandler->CloseFile(crstrPathName);
        }
    }
    return bReturn;
}

 
//------------------------------------------------------------------*
/**
 * get file path.
 *
 *  prepend project path if file name does not contain abs path
 *
 * @param           [in] crstrFileName: file name (or file path)
 * @return          file path
 *
*/
CString CFileTreeCtrl::GetFilePath(const CString& crstrFileName)
{
    CString strPath = crstrFileName;

    if (FC_StringIsAbsPath(strPath)==0)
    {
        strPath = m_strProjectPath + _T("\\") + strPath;
    }
    return(strPath);
}

//------------------------------------------------------------------*
/**
 * collect files recursively
 *
 * @param           [in] pNode: starting XML node
 * @return          [in/out] rtLexTree: lexical tree with collected file names
 * @see             DeleteContextNode()
*/
void CFileTreeCtrl::CollectFiles(CXMLNode* pNode, CSBLLexTree<CXMLNode>& rtLexTree, BOOL bUseFilter /*=FALSE*/)
{
    if(pNode)
    {
        CXMLNodeList            tChildList;
        CXMLNode                tNodeChild;
        CString                 strType;
        CString                 strText;
        CString                 strSource;

        pNode->GetAttribute(CE_XMLATTR_TYPE, strType);
        pNode->GetAttribute(CE_XMLATTR_SOURCE, strSource);
        if(strType == con_strTypeFile)
        {
            pNode->GetAttribute(CE_XMLATTR_TEXT, strText);
            if(!bUseFilter || (bUseFilter && IsPassingFilters(strText, strType)))
            {
                // if absolute path, don't add
                if(!FC_StringIsAbsPath(strSource))
                {
                    rtLexTree.insertData(strText, pNode);
                }
            }
        }
        if(pNode->GetChildNodeList(tChildList))
        {
            long lNumChildren = tChildList.GetLength();
            for(long lChild = 0; lChild < lNumChildren; ++lChild)
            {
                tChildList.Item(lChild, tNodeChild);
                CollectFiles(&tNodeChild, rtLexTree, bUseFilter);
            }
        }
    }
}

// double click on tree control -> excute kernel standard operation "Open"
void CFileTreeCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
//    GetCursorPos(&point);
//    ScreenToClient(&point);
    
    CString strType;
    UINT    uiHitFlags;
    CTreeCursor tContextCursor = HitTest(point, &uiHitFlags);

    if(!(uiHitFlags & TVHT_ONITEM))
	{
        CTreeCtrlEx::OnLButtonDblClk(nFlags, point);
        return;
    }

    if(SetContextCursor(tContextCursor))
    {
        if(!HTREEITEM(m_tContextCursor))
        {
            m_eContextType = E_NAV_FV_UNKNOWN;
            return;
        }
    }

    SelectItem(m_tContextCursor);
    strType = GetNodeType(m_tContextCursor);
    m_eContextType = GetTypeFromTypeName(strType);

    OnCommand(ID_NAV_FV_OPEN);
}

//------------------------------------------------------------------*
/**
 * initialize filters
 *
 * @see             CNavTreeCtrl::InitFilters()
*/
void CFileTreeCtrl::InitFilters()
{
    CNavTreeCtrl::InitFilters();
    // get all file extensions from KAD
    CComPtr<IUnknown>   pKadManager;
    CComPtr<IUnknown>   pTmp;

    pKadManager.Attach(CEGetCOM(CE_COMPMAN_KADMAN));
    if(!pKadManager)
    {
        return;
    }

    m_tFileInfo.Load(pKadManager);

    CStringArray    astrTmp;
    m_tFileInfo.GetAllExtensions(astrTmp);

    long                lNumExt = astrTmp.GetSize();
    long                lExt;
    CString             strExt;
    CString             strText;
    CString             strExpr;
    CKADFileExtInfo*    pFileExtInfo;
    CSBLLexTree<CKADFileExtInfo>    tFileExtTree;   // for sorting

    // set filter categories
    //m_tFileInfo.GetAllTexts(astrTmp);
    for(lExt = 0; lExt < lNumExt; ++lExt)
    {
        strExt = astrTmp[lExt];
        pFileExtInfo = m_tFileInfo.GetFileExtInfo(strExt);
        ASSERT(pFileExtInfo);
        if(!pFileExtInfo->m_strOrderKey.IsEmpty())
        {
            VERIFY(tFileExtTree.insertData(pFileExtInfo->m_strOrderKey, pFileExtInfo));
        }
    }

    // run over lex tree and append filters
    CStringArray    astrExt;
    int             iNumExt;
    tFileExtTree.getAllStringsFromSubstring("", astrExt);
    iNumExt = astrExt.GetSize();

    for(int iExt = 0; iExt < iNumExt; ++iExt)
    {
        pFileExtInfo = tFileExtTree.getData(astrExt[iExt]);
        if(pFileExtInfo)
        {
            strExpr.Format("*.%s", pFileExtInfo->m_strExt);
            strText.Format("%s (%s)", pFileExtInfo->m_strFilterText, strExpr);
            m_pCatFilter->AddNewFilterItem(strText, strExpr);
        }
    }

    CNavTreeCtrl::InitFilters();
}


//------------------------------------------------------------------*
/**
 * get files of category from project manager.
 *
 *  files starting with $ are not added.
 *
 * @param           [in] eFC: file category (FV_FILE_SOURCE, FV_FILE_HELP)
 * @param           [out] rastrFiles: file category (FV_FILE_SOURCE, FV_FILE_HELP)
 * @return          successful?
 * @see             
*/
BOOL CFileTreeCtrl::GetAllFilesFromProjMan(enum E_FV_FileCategory eFC, CSBLLexTree<bool>& rtFileNameTree)
{
    SAFEARRAY*          parrFiles;
    bool*               pbTmp = (bool*) 1;  // dummy pointer for insert

    HRESULT hr = m_pProjMan->GetAllFilesOfCategory(eFC ,&parrFiles);
    if(hr != S_OK)
    {
        return FALSE;        
    }   

    BSTR HUGEP *pbstr;
    BSTR bstr;
    CString str;
    unsigned long i;

    // Get a pointer to the elements of the array.
    hr = ::SafeArrayAccessData(parrFiles, (void HUGEP**)&pbstr);
    if (FAILED(hr))
    {
        return FALSE;
    }
    unsigned long iSize = ((parrFiles)->rgsabound[0]).cElements;
    TRACE("    GetAllFilesFromProjMan: %d Files\n", iSize);

    for (i = 0; i < iSize; i++)
    {
        bstr = pbstr[i];
        str = bstr;
        if(str[0] != _T('$'))
        {
//        TRACE("  GetAllFilesFromProjMan: Add %s\n", str);
            rtFileNameTree.insertData(str, pbTmp);
        }
    }
    ::SafeArrayUnaccessData(parrFiles);
    ::SafeArrayDestroy(parrFiles);
    return TRUE;
}


// 
//------------------------------------------------------------------*
/**
 * start editing label
 *
 *  only allow folders to be renamed
 *
 * @param           [in] pDispInfo: not used
 * @param           [in] pResult: prevent renaming by setting to TRUE
 *
*/
void CFileTreeCtrl::OnBeginlabeledit(TV_DISPINFO* pDispInfo, LRESULT* pResult)
{
    CXMLNode*       pNode;
    CString         strText;
    CString         strType;
    CString         strSource;

    *pResult = 1;   // generally prevent rename

    // editing is only allowed, if standard operation rename is available
    CTreeCursor tTreeCursor = GetSelectedItem();
    if(!HTREEITEM(tTreeCursor))
    {
        return;
    }
    pNode = (CXMLNode*)tTreeCursor.GetData();
    if(!pNode)
    {
        return;
    }

    SetContextCursor(tTreeCursor);
    m_strRenameText = m_tContextCursor.GetText();

    pNode->GetAttribute(CE_XMLATTR_TYPE, strType);
    pNode->GetAttribute(CE_XMLATTR_TEXT, strText);
    m_eContextType = GetTypeFromTypeName(strType);

    m_tContextCursor.SetText(m_strRenameText);

    // rename allowed for user folders and files
    switch(m_eContextType)
    {
    case E_NAV_FV_FOLDER:
        *pResult = 0;
        break;
    case E_NAV_FV_FILE:
        if(strText.Right(4).CompareNoCase(_T(".4cp")) != 0)
        {
            CCEFileHandler* pFileHandler = ((CCEMAINApp*)AfxGetApp())->GetFileHandler();
            if(pFileHandler)
            {
                pNode->GetAttribute(CE_XMLATTR_SOURCE, strSource, TRUE);
                if(pFileHandler->GetFileStatus(strSource) != eFSReadOnly)
                {
                    *pResult = 0;
                }
            }
        }
        break;
    }
}

//------------------------------------------------------------------*
/**
 * end editing label
 *
 *  first change item text, then invoke OnCommand() for changing node text
 *
 * @param           [in] pDispInfo: not used
 * @param           [in] pResult: not changed
*/
void CFileTreeCtrl::OnEndlabeledit(TV_DISPINFO* pDispInfo, LRESULT* pResult)
{
    if(HTREEITEM(m_tContextCursor))
    {
        if(pDispInfo)
        {
            if(pDispInfo->item.pszText)
            {
                CString     strText = pDispInfo->item.pszText;
                CString     strTextNew;
                CString     strMessage;
                switch(m_eContextType)
                {
                case E_NAV_FV_FOLDER:
                    if(!RenameFolderNode(m_tContextCursor, strText))
                    {
                        strMessage.LoadString(IDS_FOLDER_EXISTING);
                        AfxMessageBox(strMessage);
                        EditLabel(m_tContextCursor);
                    }
                    break;
                case E_NAV_FV_FILE:
                    RenameFile(m_tContextCursor, strText);
//                    if(RenameFile(m_tContextCursor, strText))
//                    {
//                        SelectLastInsertNode();
//                    }
                    break;
                }
            }
        }
    }
}



//------------------------------------------------------------------*
/**
 * on key down.
 *
 * @param           [in] nChar: virtual key code
 * @param           [in] nRepCnt: repetition count
 * @param           [in] nFlags: flags see CWnd::OnKeyDown
*/
void CFileTreeCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    BOOL    bDone = FALSE;
    switch(nChar)
    {
    // escape: cancel cut operation
    case VK_ESCAPE:
        if(HTREEITEM(m_tDragCursor))
        {
            m_tDragCursor.SetState(0, TVIS_CUT);
        }
        break;
    case VK_RETURN:
        {
            if(SetContextCursor(GetSelectedItem()))
            {
                OnCommand(ID_NAV_FV_OPEN);
                bDone = TRUE;
            }
        }
        break;
    case VK_INSERT:
        bDone = TRUE;   // eat return and insert operations
        break;
    case VK_DELETE:
        {
            if(SetContextCursor(GetSelectedItem()))
            {
                DeleteContextNode();
            }
            bDone = TRUE;
        }
        break;
    }
    if(!bDone)
    {
        CNavTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
    }
}


//------------------------------------------------------------------*
/**
 * set data for drag operation
 *
 *  overwrites CNavTreeCtrl::SetClipboardData()
 *  copy data to clipboard using CXMLNodeDragItem::CopyToClipboard
 *
 * @param           [in] pNode: pointer to XML node with data
 * @return          successful?
 * @see             CXMLNodeDragItem
 *
*/
BOOL CFileTreeCtrl::SetClipboardData(CXMLNode* pNode, BOOL bDragDrop /*=FALSE*/)
{
    if(!pNode)
    {
        return FALSE;
    }
    CString             strText;
    CString             strType;
    CString             strXMLString;
    CXMLNodeDragItem    tDragItem;

    pNode->GetAttribute(CE_XMLATTR_TEXT, strText);
    pNode->GetAttribute(CE_XMLATTR_TYPE, strType);
    pNode->GetXMLString(strXMLString);

    tDragItem.SetData(strText, strText, strType, strText, m_strProjectPath, strXMLString);
    tDragItem.SetCBFormatId(m_uiCBXMLNodeFormat);

    COleDataSource*  pDataSource;
    pDataSource = new COleDataSource;
    if(pDataSource == NULL)
    {
        ::AfxThrowMemoryException();
    }

    if(tDragItem.CopyToClipboard(pDataSource))
    {
        if(bDragDrop)
        {
            pDataSource->DoDragDrop();
           	pDataSource->InternalRelease();
        }
        else
        {
            pDataSource->SetClipboard();
        }
    }
    else
    {
        ::MessageBeep(MB_ICONEXCLAMATION);
        delete pDataSource;
        return FALSE;
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * is drag of node allowed?
 *
 * @param           [in] pNode: pointer to XML node to be dragged
 * @return          drag allowed?
 * @see             OnBegindrag()
*/
BOOL CFileTreeCtrl::IsDragAllowed(CXMLNode* pNode)
{
    if(!pNode)
    {
        return FALSE;
    }

    CString strType;
    BOOL    bDragAllowed = FALSE;

    pNode->GetAttribute(CE_XMLATTR_TYPE, strType);
    EFVNodeType eType = GetTypeFromTypeName(strType);
    switch(eType)
    {
    case E_NAV_FV_FILE:
    case E_NAV_FV_FOLDER:
        bDragAllowed = TRUE;
        break;
    }
    return bDragAllowed;
}


//------------------------------------------------------------------*
/**
 * move tree item
 *
 * @param           [in] rtCursorSource: item to be moved
 * @param           [in] rtCursorTarget: item where to move
 * @param           [in] bUpdate: update category (category has changed)
 * @return          reparse needed?
*/
BOOL CFileTreeCtrl::MoveNode(CTreeCursor& rtCursorSource, CTreeCursor& rtCursorTarget, BOOL bUpdate /*=FALSE*/)
{
    if(!HTREEITEM(rtCursorSource) || !HTREEITEM(rtCursorTarget))
    {
        return FALSE;
    }

    CXMLNode*   pNodeSource = (CXMLNode*) rtCursorSource.GetData();
    CXMLNode*   pNodeTarget = (CXMLNode*) rtCursorTarget.GetData();

    ASSERT(pNodeSource && pNodeTarget);

    m_strInsertIdPath.Empty();

    CString strTargetType;
    pNodeTarget->GetAttribute(CE_XMLATTR_TYPE, strTargetType);

    EFVNodeType     eTargetType = GetTypeFromTypeName(strTargetType);

    CXMLNode    tNodeInsert;            // parent where node is to be inserted
    CXMLNode    tNodeSourceParent;      // parent of source node
    CTreeCursor tCursorInsert;          // tree cursor where node is to be inserted
    CTreeCursor tCursorNew;             // tree cursor of new child element
    CString     strIdPathSourceParent;  // id path to tNodeSourceParent
    CString     strIdPathSource;        // id path to source node
    CString     strIdPathInsert;        // id path to insert node
    CString     strIdSource;            // id of insert node
    BOOL        bParse(FALSE);          // parsing needed?

    if(!pNodeSource->GetParentNode(tNodeSourceParent))
    {
        return FALSE;
    }
    pNodeSource->GetAttribute(CE_XMLATTR_ID, strIdSource);

    tNodeSourceParent.GetIdPath(strIdPathSourceParent);

    // if target type is file, insert node is parent node (this must be folder)
    if(eTargetType == E_NAV_FV_FILE)
    {
        if(!pNodeTarget->GetParentNode(tNodeInsert))
        {
            return FALSE;
        }
        tCursorInsert = rtCursorTarget.GetParent();
        ASSERT(HTREEITEM(tCursorInsert));
    }
    else
    {
        tNodeInsert = *pNodeTarget;
        tCursorInsert = rtCursorTarget;
    }

    pNodeSource->GetIdPath(strIdPathSource);

    tNodeInsert.GetIdPath(strIdPathInsert);
    // if source and insert node are the same or source is already child
    // of insert node, no move
    if(strIdPathSource == strIdPathInsert || strIdPathSourceParent == strIdPathInsert)
    {
        return FALSE;
    }

    E_FV_FileCategory   eTargetCat = GetFileCategory(&tNodeInsert);
    E_FV_FileCategory   eSourceCat = GetFileCategory(pNodeSource);

    if(!m_bCatChangeAllowed && !bUpdate && (eTargetCat != eSourceCat))
    {
        TRACE("Category change not allowed\n");
        ::MessageBeep(MB_ICONEXCLAMATION);
        return FALSE;
    }

    // first try to change file cat
    if(!bUpdate)
    {
        if(eTargetCat != eSourceCat)
        {
            CSBLLexTree<CXMLNode>   tLexTree;
            CStringArray            astrFiles;

            CollectFiles(pNodeSource, tLexTree);
            tLexTree.getAllStringsFromSubstring("", astrFiles);

            EnableFileCheck(FALSE);
            if(!ChangeFileCategory(astrFiles, eSourceCat, eTargetCat, TRUE))
            {
                EnableFileCheck(TRUE);
                return FALSE;
            }

            bParse = ChangeFileCategory(astrFiles, eSourceCat, eTargetCat);
        }
    }

    // unlink node
    tNodeSourceParent.RemoveChild(*pNodeSource);

    // then insert node
    if(!InsertXMLChildNode(&tNodeInsert, pNodeSource, TRUE))
    {
        EnableFileCheck(TRUE);
        return FALSE;
    }

    DeleteTreeItem(rtCursorSource);

    UpdateTreeNode(tCursorInsert, tNodeInsert);
    ExpandNodeFromDom(tCursorInsert);
    ExpandTreeItem(tCursorInsert, TVE_EXPAND);
    if(!bUpdate)
    {
        if(FindChildFromId(tCursorInsert, tCursorNew, strIdSource))
        {
            SelectItem(tCursorNew);
        }
        Invalidate();
    }
    EnableFileCheck(TRUE);

    // select last insert node
//    SelectLastInsertNode();

    if(bParse)
    {
        m_bRejectUpdateMessages = TRUE;
        m_pProjInfo->fileParseNow(NULL);
        m_bRejectUpdateMessages = FALSE;
    }

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * change file category
 *
 * @param           [in] crastrFiles: file names
 * @param           [in] eFromCat: current file category
 * @param           [in] eToCat: file category to change to
 * @param           [in] eTry: just try to do it
 * @return          reparse needed?
 *                  in try mode: successful for all files?
 *
*/
BOOL CFileTreeCtrl::ChangeFileCategory(const CStringArray& crastrFiles, E_FV_FileCategory eFromCat, E_FV_FileCategory eToCat, BOOL bTry /*=FALSE*/)
{
    BOOL        bReparse = FALSE;
    BOOL        bSuccessful = TRUE;

    SaveProjectFile();

    if(eFromCat == eToCat)
    {
        if(bTry)
        {
            return bSuccessful;
        }
        else
        {
            return bReparse;
        }
    }

    long        lNumFiles = crastrFiles.GetSize();

    for(long lFile = 0; lFile < lNumFiles; ++lFile)
    {
        bReparse |= ChangeFileCategory(crastrFiles[lFile], eFromCat, eToCat, bTry);
        if(bTry && !bReparse)
        {
            bSuccessful = FALSE;
            break;
        }
    }
    if(bTry)
    {
        return bSuccessful;
    }
    return bReparse;
}


//------------------------------------------------------------------*
/**
 * save project file.
 *
 * @param           -
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CFileTreeCtrl::SaveProjectFile()
{
    CString strProjectFileName(m_strProjectName + _T(".4cp"));
    CCEFileHandler*     pFileHandler = ((CCEMAINApp*)AfxGetApp())->GetFileHandler();
    if(!pFileHandler)
    {
        return FALSE;
    }
    pFileHandler->SaveFile(strProjectFileName);
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * change file category of a single file.
 *
 * @param           [in] crstrFileName: file name without path
 * @param           [in] eFromCat: current file category
 * @param           [in] eToCat: file category to change to
 * @param           [in] bTry: just try to change file cat but don't? (default FALSE)
 * @return          successful?
 *
*/
BOOL CFileTreeCtrl::ChangeFileCategory(const CString& crstrFileName, E_FV_FileCategory eFromCat, E_FV_FileCategory eToCat, BOOL bTry /*=FALSE*/)
{

    if(eFromCat == eToCat)
    {
        return TRUE;
    }

    HRESULT     hr = S_OK;
    CComBSTR    sFile(crstrFileName);
    hr = E_FAIL;
    BOOL    bSuccessful = TRUE;

    switch(eFromCat)
    {
    case FV_FILE_SOURCE:
        hr = m_pProjMan->RemoveFile(sFile, FV_FILE_SOURCE, bTry);
        if(hr != S_OK)
        {
            bSuccessful = FALSE;
        }
        break;
    case FV_FILE_HELP:
        hr = m_pProjMan->RemoveFile(sFile, FV_FILE_HELP, bTry);
        if(hr != S_OK)
        {
            bSuccessful = FALSE;
        }
        break;
    case FV_FILE_ADDITIONAL:
        hr = m_pProjMan->RemoveFile(sFile, FV_FILE_ADDITIONAL, bTry);
        if(hr != S_OK)
        {
            bSuccessful = FALSE;
        }
        break;
    }
    if(bSuccessful)
    {
        switch(eToCat)
        {
        case FV_FILE_SOURCE:
        case FV_FILE_HELP:
        case FV_FILE_ADDITIONAL:
            hr = m_pProjMan->AddFile(sFile, eToCat, bTry);
            if(hr != S_OK)
            {
                bSuccessful = FALSE;
            }
            break;
        }
    }
    return bSuccessful;
}


//------------------------------------------------------------------*
/**
 * insert XML child node
 *
 *  if child node with name already exists, it is not inserted
 *
 * @param           [in] pNodeParent: pointer to XML parent node
 * @param           [in] pNodeInsert: pointer to XML node to be inserted as child
 * @param           [in] bSorted: if bSorted == FALSE, insert as last else insert sorted
 *                                first folders then files, each group sorted
 * @return          succeeded?
 * @see             
*/
BOOL CFileTreeCtrl::InsertXMLChildNode(CXMLNode* pNodeParent, CXMLNode* pNodeInsert, BOOL bSorted /*=TRUE*/)
{
    CString     strTypeInsert;      // node type id string of insert node (FILE, FOLDER, SYSTEMFOLDER)
    CString     strTextInsert;      // node text of insert node
    BOOL        bReturn = FALSE;    // succeeded?

    ASSERT(pNodeParent && pNodeInsert);

    pNodeInsert->GetAttribute(CE_XMLATTR_TYPE, strTypeInsert);
    pNodeInsert->GetAttribute(CE_XMLATTR_TEXT, strTextInsert);

    if(!bSorted)
    {
        // now create new node as copy of tNodeNewTmp
        pNodeParent->AppendChild(*pNodeInsert);
        pNodeInsert->GetIdPath(m_strInsertIdPath);
        bReturn = TRUE;
    }
    else
    {
        // get all children
        CXMLNodeList            tChildList;
        CXMLNode                tNodeChild;
        CString                 strChildName;
        CString                 strChildType;
        BOOL                    bFolder = TRUE;

        if(pNodeParent->GetChildNodeList(tChildList))
        {
            long lNumChildren = tChildList.GetLength();
            for(long lChild = 0; lChild < lNumChildren; ++lChild)
            {
                tChildList.Item(lChild, tNodeChild);
                tNodeChild.GetAttribute(CE_XMLATTR_TEXT, strChildName);
                tNodeChild.GetAttribute(CE_XMLATTR_TYPE, strChildType);

                if(strChildType != con_strTypeFile)
                    bFolder = TRUE;
                else
                    bFolder = FALSE;
                // if file is to be inserted, skip all folders
                if(strTypeInsert == con_strTypeFile && bFolder)
                    continue;
                // if folder is to be inserted and next node is already file, insert here
                if(strTypeInsert == con_strTypeFolder && !bFolder)
                {
                    break;
                }
                // if file / folder already exists, it cannot be inserted
                if(strChildName.CompareNoCase(strTextInsert) == 0)
                {
                    return(FALSE);
                }
                // if child name alphabetically after file/folder name, insert before
                if(strChildName.CompareNoCase(strTextInsert) > 0)
                {
                    break;
                }
            }
            if(lChild == lNumChildren)
            {
                tNodeChild.SetIXMLNode((IXMLDOMNode*)NULL);
            }
            // if file is to be inserted and all children are folders
            // insert at last, i.e. set tNodeChild IXMLDOMNode pointer to NULL
            if(strTypeInsert == con_strTypeFile && bFolder)
            {
                tNodeChild.SetIXMLNode((IXMLDOMNode*)NULL);
            }
        }
        if(pNodeParent->InsertChildBefore(*pNodeInsert, tNodeChild))
        {
            bReturn = TRUE;
        }
    }
    return(bReturn);
}


//------------------------------------------------------------------*
/**
 * handle drop message
 *
 * @see             COleDropTarget::OnDrop()
*/
BOOL CFileTreeCtrl::OnDrop( CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
    BOOL bReturn = FALSE;

	if(pDataObject->IsDataAvailable(m_uiCBXMLNodeFormat))
    {
        CXMLNodeDragItem    tDragItem;

        EnableFileCheck(FALSE);
        tDragItem.SetCBFormatId(m_uiCBXMLNodeFormat);
        if(tDragItem.LoadFromClipboard(pDataObject))
        {

            CTreeCursor tDropCursor = GetNewInsertItem(point);
            if(HTREEITEM(tDropCursor) && HTREEITEM(m_tDragCursor))
            {
                if(!MoveNode(m_tDragCursor, tDropCursor))
                {
                    ::AfxMessageBox(IDS_CANNOT_MOVE_NODE);
                }
                bReturn = TRUE;
            }
        // if drag started outside file tree view
            else
            {
                CXMLDocument    tXMLDocTmp;
                tXMLDocTmp.LoadXMLString(tDragItem.m_strXMLString);
                CXMLNode        tNodeTmp;
                if(tXMLDocTmp.GetRootNode(tNodeTmp))
                {
                    bReturn = InsertNodeFromExtern(tDropCursor, &tNodeTmp, tDragItem.m_strProjPath);
                }
            }
        }
        if(bReturn)
        {
            m_pProjInfo->fileParseNow(NULL);
        }
        EnableFileCheck(TRUE);
    }
    m_tDragCursor = CTreeCursor();    // set item drag to NULL
    OnDragLeave(this);
    return bReturn;
}


//------------------------------------------------------------------*
/**
 * is clipboard data available?
 *
 * @return          is clipboard data available?
*/
BOOL CFileTreeCtrl::IsClipboardDataAvailable(UINT uiFormat)
{
    if(::IsClipboardFormatAvailable(uiFormat))
    {
        return TRUE;
    }
    return FALSE;
}

//------------------------------------------------------------------*
/**
 * insert node from external source
 *
 *  called from OnDrop()
 *
 * @param           [in] rtItemInsert: item where node is to be inserted
 * @param           [in] pNodeToInsert: pointer to node to be inserted
 * @return          successful?
 * @see             OnDrop()
 *
*/
BOOL CFileTreeCtrl::InsertNodeFromExtern(CTreeCursor& rtItemInsert, CXMLNode* pNodeToInsert, const CString& crstrProjPath)
{
    if(!HTREEITEM(rtItemInsert))
    {
        return FALSE;
    }

    CSBLLexTree<CXMLNode>   tLexTree;
    CStringArray        astrFiles;
    CString             strFileName;        // file name without path
    CString             strFilePathName;    // file name with path
    E_FV_FileCategory   eDropCat;
    long                lNumFiles;
    long                lFile;
    CComBSTR            sFile;
    CXMLNode            tNodeInsert;
    CXMLNode*           pNodeTmp = (CXMLNode*)rtItemInsert.GetData();
    CString             strType;
    BOOL                bReturn = TRUE;
    CCEFileHandler*     pFileHandler = ((CCEMAINApp*)AfxGetApp())->GetFileHandler();

    ASSERT(pFileHandler);
    if(!pFileHandler)
    {
        return FALSE;
    }



    ASSERT(pNodeTmp);
    ASSERT(pNodeToInsert);

    // get insert node: this must not be a file type node
    pNodeTmp->GetAttribute(CE_XMLATTR_TYPE, strType);
    if(strType == con_strTypeFile)
    {
        if(!pNodeTmp->GetParentNode(tNodeInsert))
        {
            return FALSE;
        }
    }
    else
    {
        tNodeInsert = *pNodeTmp;
    }

    CollectFiles(pNodeToInsert, tLexTree);
    tLexTree.getAllStringsFromSubstring("", astrFiles);
    eDropCat = GetFileCategory(&tNodeInsert);

    SaveProjectFile();

    CString strProblemFiles;
    lNumFiles = astrFiles.GetSize();
    // try to add files to project manager
    for(lFile = 0; lFile < lNumFiles; ++lFile)
    {
        HRESULT hr = S_OK;
        strFileName = astrFiles[lFile];
        strFilePathName = pFileHandler->GetFilePathName(strFileName, crstrProjPath);
        sFile = strFilePathName;
        switch(eDropCat)
        {
        case FV_FILE_SOURCE:
        case FV_FILE_HELP:
        case FV_FILE_ADDITIONAL:
            hr = m_pProjMan->AddFile(sFile, eDropCat, TRUE);
            break;
        }
        if(hr != S_OK)
        {
            strProblemFiles += _T("\r\n") + strFilePathName;
        }
    }


    if(!strProblemFiles.IsEmpty())
    {
        CString strMessage;
        strFilePathName = pFileHandler->GetFilePathName(strFileName, crstrProjPath);
        strMessage.Format(IDS_CAN_NOT_ADD_FILES, strProblemFiles);
        ::AfxMessageBox(strMessage);
        return FALSE;
    }

    // look for files to be overwritten and warn user
    CFileStatus     tStatus;
    strProblemFiles.Empty();
    for(lFile = 0; lFile < lNumFiles; ++lFile)
    {
        strFileName = astrFiles[lFile];
        strFilePathName = pFileHandler->GetFilePathName(strFileName);
        if(CFile::GetStatus(strFilePathName, tStatus))
        {
            strProblemFiles += _T("\r\n") + strFilePathName;
        }
    }

    if(!strProblemFiles.IsEmpty())
    {
        CString strMessage;
        strMessage.Format(IDS_OVERWRITE_DOUBLE_FILES, strProblemFiles);
        if(::AfxMessageBox(strMessage, MB_YESNO) != IDYES)
        {
            return FALSE;
        }
    }

    // copy each of these files into the project folder
    strProblemFiles.Empty();
    for(lFile = 0; lFile < lNumFiles; ++lFile)
    {
        strFileName = astrFiles[lFile];
        strFilePathName = pFileHandler->GetFilePathName(strFileName, crstrProjPath);
        if(!CopyFileToProject(strFilePathName, TRUE, TRUE))
        {
            strProblemFiles += _T("\r\n") + strFilePathName;
        }
    }

    if(!strProblemFiles.IsEmpty())
    {
        CString strMessage;
        strMessage.Format(IDS_COULD_NOT_COPY_FILES, strProblemFiles);
        ::AfxMessageBox(strMessage);
    }

    // now add files to project manager
    strProblemFiles.Empty();
    for(lFile = 0; lFile < lNumFiles; ++lFile)
    {
        HRESULT hr = S_OK;
        strFileName = astrFiles[lFile];
        sFile = strFileName;
        switch(eDropCat)
        {
        case FV_FILE_SOURCE:
        case FV_FILE_HELP:
        case FV_FILE_ADDITIONAL:
            hr = m_pProjMan->AddFile(sFile, eDropCat, FALSE);
            break;
        }
        if(hr != S_OK)
        {
            strProblemFiles += _T("\r\n") + strFilePathName;
        }
    }
    ASSERT(strProblemFiles.IsEmpty());

    pNodeToInsert->GetIdPath(m_strInsertIdPath);
    InsertXMLChildNode(&tNodeInsert, pNodeToInsert);
    UpdateTreeNode(rtItemInsert, tNodeInsert);

    // files could not be added to project -> check
    if(!strProblemFiles.IsEmpty())
    {
        Check(TRUE);
    }

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * copy file to project
 *
 *  copy file into project directory
 *  if file already exists and not silent mode, user is asked
 *  if he wants to overwrite existing project file
 *
 * @param           [in] crstrFilePathName: file name incl path
 * @param           [in] bSilent: silent mode? (user is not asked)
 *                                  default is FALSE
 * @param           [in] bOverwrite: overwrite existing file? (only silent mode)
 *                                  default is FALSE
 * @return          successful?
 * @see             
*/
BOOL CFileTreeCtrl::CopyFileToProject(const CString& crstrFilePathName, BOOL bSilent /*=FALSE*/, BOOL bOverwrite /*=FALSE*/)
{
    CString     strExistingFile(crstrFilePathName);  // file name inclusive full path of foreign file
    CString     strInsertFile;      // file name inclusive project path
    CString     strFileName;        // file name without path

    // check for 4cp files
    if(crstrFilePathName.Right(4).CompareNoCase(_T(".4cp")) == 0)
    {
        CString strMessage;
        strMessage.Format(IDS_ERR_PROJECT_FILE_DROP, crstrFilePathName);
        ::AfxMessageBox(strMessage);
        return FALSE;
    }

    // now get file name from file path name:
    strFileName = FC_StringGetFilePart(crstrFilePathName);
    strInsertFile = m_strProjectPath + "\\" + strFileName;

    if(strExistingFile.CompareNoCase(strInsertFile) == 0)
    {
        return TRUE;
    }

    // if file exists
    CFileStatus tStatus;
    if(CFile::GetStatus(strInsertFile, tStatus))
    {
        // and if not silent, ask user
        if(!bSilent)
        {
            CString     strMessage;
            strMessage.Format(IDS_FILE_EXISTING, strInsertFile);
            if(::AfxMessageBox(strMessage, MB_YESNO) == IDNO)
            {
                return FALSE;
            }
            bOverwrite = TRUE;  // overwrite
        }
    }

    // if overwrite
    if(bOverwrite)
    {
        if(!::CopyFile(strExistingFile, strInsertFile, FALSE) && !bSilent)
        {
            DWORD   dwLastError = ::GetLastError();
            UTIL_DisplaySysError(dwLastError, strInsertFile, NULL);
            return FALSE;
        }
    }
    else
    {
        if(!::CopyFile(strExistingFile, strInsertFile, TRUE) && !bSilent)
        {
            DWORD   dwLastError = ::GetLastError();
            UTIL_DisplaySysError(dwLastError, strInsertFile, NULL);
            return FALSE;
        }
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * copy external file to project.
 *
 * @param            [in] crstrFilePathName: file name inclusive path
 * @param            [in] eFileCat: file category for new file
 * @return           reparse needed?
*/
BOOL CFileTreeCtrl::CopyExternalFileToProject(const CString& crstrFilePathName, E_FV_FileCategory eFileCat /*= FV_FILE_UNKNOWN*/)
{
    CString strFileName;
    CFile   tFile;
    BOOL    bReparse = FALSE;
    tFile.SetFilePath(crstrFilePathName);
    strFileName = tFile.GetFileName();

    // copy file to project folder
    if(CopyFileToProject(crstrFilePathName))
    {
        // if file cat is unknown (i.e. no special file cat), ok
        if(eFileCat == FV_FILE_UNKNOWN)
        {
            return FALSE;
        }
        // if file category changed to desired category, ok
        bReparse = ChangeFileCategory(strFileName, FV_FILE_UNKNOWN, eFileCat);
    }
    // else not ok
    return bReparse;
}


//
//CString CFileTreeCtrl::GetFullPath(const CString& crstrFile, const CString& crstrPath)
//{
//
//}

//------------------------------------------------------------------*
/**
 * handle drag over message
 *
 * @see             COleDropTarget::OnDragOver()
*/
DROPEFFECT CFileTreeCtrl::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	if(pDataObject && pDataObject->IsDataAvailable(m_uiCBXMLNodeFormat))
    {
	    CTreeCursor			tItem;

        if(m_pDragImageList)
        {
		    m_pDragImageList->DragMove(point);
            tItem = GetNewInsertItem(point);
            //tItem = HitTest(point);
		    if(HTREEITEM(tItem))
		    {
			    m_pDragImageList->DragLeave(this);
			    SelectDropTarget(tItem);
			    m_pDragImageList->DragEnter(this, point);
                return GetDropEffect(tItem, dwKeyState);
		    }
        }
        else
        {
            //tItem = GetNewInsertItem(point);
            tItem = HitTest(point);
		    if(HTREEITEM(tItem))
		    {
			    SelectDropTarget(tItem);
                return GetDropEffect(tItem, dwKeyState);
		    }
        }
    }
    return DROPEFFECT_NONE;
}


//------------------------------------------------------------------*
/**
 * get insert item
 *
 *  if item type is file -> get parent item
 *
 * @param           [in] rtPoint: cursor position
 * @return          cursor to insert item
*/
CTreeCursor CFileTreeCtrl::GetNewInsertItem(CPoint& rtPoint)
{
    UINT		flags;
    CTreeCursor tHitCursor;
    CTreeCursor tDropCursor;

    tHitCursor = HitTest(rtPoint, &flags);
    tDropCursor = tHitCursor;
    if(HTREEITEM(tHitCursor))
    {
        CXMLNode* pNode = (CXMLNode*)tHitCursor.GetData();

        ASSERT(pNode);
        if(pNode)
        {
            CString     strType;
            pNode->GetAttribute(CE_XMLATTR_TYPE, strType);
            if(strType == con_strTypeFile)
            {
                tDropCursor = tHitCursor.GetParent();
            }
        }
    }
    return(tDropCursor);
}



//------------------------------------------------------------------*
/**
 * update node.
 *
 *  delete tree item and fill it again from DOM
 *  uses protected method UpdateTreeNode()
 *
 * @param           [in] rcs_id_path: ID-path to node
 * @return          successful?
 * @see             UpdateTreeNode()
*/
BOOL CFileTreeCtrl::UpdateNode(const CString& rcs_id_path)
{
    if(rcs_id_path == "")
    {
        Check(TRUE);
        //return TRUE;
    }
    return TRUE;
    //return(CNavTreeCtrl::UpdateNode(rcs_id_path));
}

//------------------------------------------------------------------*
/**
 * check categories
 *
 *  run over all files and check if category has changed
 *
 * @param           [in] rtTreeCursor: start item
 * @return          view changed?
*/
BOOL CFileTreeCtrl::CheckCategories(CTreeCursor& rtTreeCursor)
{
    CTreeCursor         tTreeCursorDest;
    CXMLNode*           pNode;
    CString             strType;
    CString             strSource;
    E_FV_FileCategory   eFileCatView;
    E_FV_FileCategory   eFileCatProject;
    BOOL                bInvalidate = FALSE;

    if(HTREEITEM(rtTreeCursor))
    {
        strType = GetNodeType(rtTreeCursor);
        if(strType == con_strTypeFile)
        {
            pNode = (CXMLNode*)rtTreeCursor.GetData();
            ASSERT(pNode);
            if(pNode)
            {
                pNode->GetAttribute(CE_XMLATTR_TEXT, strSource);
                CComBSTR    sSource = strSource;
                m_pProjMan->GetFileCategory(sSource, &eFileCatProject);
                eFileCatView = GetFileCategory(pNode);
                if(eFileCatView != eFileCatProject)
                {
                    // now move file to system folder
                    tTreeCursorDest = GetSystemCursor(eFileCatProject);
                    MoveNode(rtTreeCursor, tTreeCursorDest, TRUE);
                    bInvalidate = TRUE;
                }
            }
        }
        CTreeCursor tTreeCursorChild = rtTreeCursor.GetChild();
        if(HTREEITEM(tTreeCursorChild))
        {
            do
            {
                if(CheckCategories(tTreeCursorChild))
                {
                    bInvalidate = TRUE;
                }
                tTreeCursorChild = tTreeCursorChild.GetNextSibl();
            } while(HTREEITEM(tTreeCursorChild));
        }
    }
    return bInvalidate;
}

//------------------------------------------------------------------*
/**
 * cut command (STRG + X)
 *
 *  is called from navigator
 *
 * @see             CNavigator::OnNavCut()
*/
void CFileTreeCtrl::OnCut()
{
    CXMLNode*   pNode;
    CString     strType;

    CEdit*  pEdit = GetEditControl();
    if(pEdit)
    {
        pEdit->Cut();
        return;
    }

    if(HTREEITEM(m_tDragCursor))
    {
        m_tDragCursor.SetState(0, TVIS_CUT);
    }
    m_tDragCursor = GetSelectedItem();
    m_tDragCursor.SetState(TVIS_CUT, TVIS_CUT);
    if(HTREEITEM(m_tDragCursor))
    {
        pNode = (CXMLNode*)m_tDragCursor.GetData();
        if(pNode)
        {
            SetClipboardData(pNode);
            m_eContextAction = eCut;
            if(!m_pDragImageList)
            {
                m_pDragImageList = CreateDragImage(m_tDragCursor);
            }
   	        m_pDragImageList->DragShowNolock(TRUE);
        }
    }
}


//------------------------------------------------------------------*
/**
 * copy command (STRG + C)
 *
 *  is called from navigator
 *
 * @see             CNavigator::OnNavCopy()
*/
void CFileTreeCtrl::OnCopy()
{
    CXMLNode*   pNode;
    CString     strType;

    CEdit*  pEdit = GetEditControl();
    if(pEdit)
    {
        pEdit->Copy();
        return;
    }

    m_tDragCursor = GetSelectedItem();
    if(HTREEITEM(m_tDragCursor))
    {
        pNode = (CXMLNode*)m_tDragCursor.GetData();
        if(pNode)
        {
            SetClipboardData(pNode);
            m_eContextAction = eCopy;
        }
    }
}

//------------------------------------------------------------------*
/**
 * paste command (STRG + V)
 *
 *  is called from navigator
 *  m_tDragCursor is starting point of cut or copy
 *
 * @see             CNavigator::OnNavPaste()
*/
void CFileTreeCtrl::OnPaste()
{
    COleDataObject  tDataObject;
    BOOL            bReparse = FALSE;

    CEdit*  pEdit = GetEditControl();
    if(pEdit)
    {
        pEdit->Paste();
        return;
    }

    if(!tDataObject.AttachClipboard())
    {
        return;
    }

    if(HTREEITEM(m_tDragCursor))
    {
        m_tDragCursor.SetState(0, TVIS_CUT);
    }

    SetContextCursor(GetSelectedItem());
    if(HTREEITEM(m_tContextCursor))
    {
	    if(tDataObject.IsDataAvailable(m_uiCBXMLNodeFormat))
        {
            CXMLNodeDragItem    tDragItem;

            tDragItem.SetCBFormatId(m_uiCBXMLNodeFormat);
            if(tDragItem.LoadFromClipboard(&tDataObject))
            {
                CTreeCursor tDropCursor;

                if(m_eContextType == E_NAV_FV_FILE)
                {
                    tDropCursor = m_tContextCursor.GetParent();
                }
                else
                {
                    tDropCursor = m_tContextCursor;
                }
                if(HTREEITEM(tDropCursor) && HTREEITEM(m_tDragCursor))
                {
                    if(m_eContextAction == eCut)
                    {
                        if(!MoveNode(m_tDragCursor, tDropCursor))
                        {
                            ::AfxMessageBox(IDS_CANNOT_MOVE_NODE);
                        }
                        bReparse = TRUE;
                    }
//                    else
//                    {
//
//                    }
                }
                // if drag started outside file tree view
                else
                {
                    CXMLDocument    tXMLDocTmp;
                    tXMLDocTmp.LoadXMLString(tDragItem.m_strXMLString);
                    CXMLNode        tNodeTmp;
                    if(tXMLDocTmp.GetRootNode(tNodeTmp))
                    {
                        if(InsertNodeFromExtern(tDropCursor, &tNodeTmp, tDragItem.m_strProjPath))
                        {
                            bReparse = TRUE;
                        }
                    }
                }
            }
        }
        if(bReparse)
        {
            m_pProjInfo->fileParseNow(NULL);
        }
    }
}


//------------------------------------------------------------------*
/**
 * expand XML node
 *
 *  changes folder icon to open or closed folder
 *
 * @param            [in] pNode: pointer to XML node
 * @param            [in] bExpand: is node to be expanded?
 *                                  default TRUE
 * @param            [in] bRecursive: expand recursively? default FALSE
*/
BOOL CFileTreeCtrl::ExpandXMLNode(CXMLNode* pNode, BOOL bExpand /*=TRUE*/, BOOL bRecursive /*=FALSE*/)
{
    BOOL    bUpdateState = FALSE;
    ASSERT(pNode);
    if(pNode)
    {
        CNavTreeCtrl::ExpandXMLNode(pNode, bExpand);
        CString strType;
        pNode->GetAttribute(CE_XMLATTR_TYPE, strType);
        EFVNodeType eType = GetTypeFromTypeName(strType);
        switch(eType)
        {
        case E_NAV_FV_FOLDER:
        case E_NAV_FV_SOURCE_GROUP:
        case E_NAV_FV_HELP_GROUP:
        case E_NAV_FV_ADDITIONAL_GROUP:
            if(bExpand)
            {
                pNode->SetAttribute(CE_XMLATTR_STATE, "1");
            }
            else
            {
                pNode->RemoveAttribute(CE_XMLATTR_STATE);
            }
            bUpdateState = TRUE;
            break;
        case E_NAV_FV_ROOT:
            break;
        }
        if(bRecursive)
        {
            // run over all children and expand them
            long            lNumChildren;       // number of children of current node
            long            lChild;             // number of current child in child list
            CXMLNodeList    tNodeList;          // list of child nodes
            CXMLNode        tNodeChildTmp;      // temporary child node
            if(pNode->GetChildNodeList(tNodeList))
            {
                lNumChildren = tNodeList.GetLength();
                for(lChild = 0; lChild < lNumChildren; ++lChild)
                {
                    // get child
                    tNodeList.Item(lChild, tNodeChildTmp);
                    bUpdateState |= ExpandXMLNode(&tNodeChildTmp, bExpand, bRecursive);
                }
            }
        }
    }
    return bUpdateState;
}

//------------------------------------------------------------------*
/**
 * file changed notification.
 *
 * @param           [in] lParam: not used
 * @param           [in] wParam: not used
 *
*/
void CFileTreeCtrl::OnFileChanged(LPARAM lParam, WPARAM wParam)
{
    Check();
}

//------------------------------------------------------------------*
/**
 * start file change notification thread.
 *
 *  to stop thread call StopFileChangeNotificationThread()
 *
 * @param           [in] crstrFolder: folder to be checked (= project folder)
 * @see             StopFileChangeNotificationThread()
*/
void CFileTreeCtrl::StartFileChangeNotificationThread(const CString& crstrFolder)
{
    CFileThreadInfo*    pThreadInfo = new CFileThreadInfo;
    pThreadInfo->m_strFolder = crstrFolder;
    pThreadInfo->m_hEvent = m_event.m_hObject;
    pThreadInfo->m_pFileTreeCtrl = this;

    CWinThread* pThread = AfxBeginThread (ThreadFunc, pThreadInfo,
        THREAD_PRIORITY_IDLE);

    pThread->m_bAutoDelete = FALSE;
    m_hThread = pThread->m_hThread;
    m_pThread = pThread;
}


//------------------------------------------------------------------*
/**
 * stop file check thread.
 *
*/
void CFileTreeCtrl::StopFileChangeNotificationThread()
{
    // Kill running file change notification thread.
    m_event.SetEvent();
    ::WaitForSingleObject(m_hThread, INFINITE);
    delete m_pThread;
}

/////////////////////////////////////////////////////////////////////////
// Thread function for detecting file system changes
//EF TODO: consider to use this function also to update the 
//file readonly flag in the fileview, this is currently done in OnIdle
//of main frame.
#define BUILD_NO_RELEVANT_FILE_PAT _T("~$*|fileview.xml|CEPrjSettings.4cs")

UINT CFileTreeCtrl::ThreadFunc(LPVOID pParam)
{
    CFileThreadInfo* pThreadInfo = (CFileThreadInfo*)pParam;
    CString strFolder = pThreadInfo->m_strFolder;
    HANDLE hEvent = pThreadInfo->m_hEvent;
    CFileTreeCtrl* pFileTreeCtrl = pThreadInfo->m_pFileTreeCtrl;
    delete pThreadInfo;

    ASSERT(pFileTreeCtrl);
    if(!pFileTreeCtrl)
    {
        return 1;
    }

    // Get a handle to a file change notification object.
    HANDLE hChange = ::FindFirstChangeNotification ((LPCTSTR) strFolder,
        FALSE, FILE_NOTIFY_CHANGE_FILE_NAME|FILE_NOTIFY_CHANGE_LAST_WRITE);

    // Return now if ::FindFirstChangeNotification failed.
    if (hChange == INVALID_HANDLE_VALUE)
        return 1;

    HANDLE aHandles[2];
    aHandles[0] = hChange;
    aHandles[1] = hEvent;
    BOOL bContinue = TRUE;

    // Sleep until a file change notification wakes this thread or
    // m_event becomes set indicating it's time for the thread to end.
    while (bContinue)
    {
        if (::WaitForMultipleObjects (2, aHandles, FALSE, INFINITE) - WAIT_OBJECT_0 == 0)
        { // Respond to a change notification.
            if(pFileTreeCtrl->IsCheckAllowed())
            {
                pFileTreeCtrl->PostMessage(WM_NAV_FV_FILE_CHANGED);
            }
            //EF TODO: we want to force a rebuild if any file changed in the project folder
            //consider usage of DeleteProjectBuildSem(strFileName) in other placed of the 
            //4CE and this code: it is somehow duplicate...
            //Hack: since the gen folder content has changed by  the compiler we
            //reach here after every build, no use FC_FileLastWrt to find out
            //the last write file time of any project file (not including any subdir)
            //As long as we don't hav real dependency info this as not a 100% solution
            //because there maybe sourcefile that are not in the project folder !!
            __int64 FTLatestWrt = 0;
            TCHAR szFileName[MAX_PATH];
            VERIFY(FC_FileLastWrt(strFolder, NULL, NULL, _T("*"), BUILD_NO_RELEVANT_FILE_PAT, 
                szFileName, MAX_PATH, &FTLatestWrt));

            CString strSemFile = strFolder +_T("\\")  CE_GEN_PATH   _T("\\project.sem");
            if(FTLatestWrt>FC_FileGetLastWriteTime(strSemFile))
                DeleteFile(strSemFile);

            ::FindNextChangeNotification (hChange);
        }
        else // Kill this thread (m_event became signaled).
            bContinue = FALSE;
    }

    // Close the file change notification handle and return.
    ::FindCloseChangeNotification (hChange);
    return 0;
}

BOOL CFileTreeCtrl::SetContextCursor(CTreeCursor& rtContextCursor)
{
    if(!HTREEITEM(rtContextCursor))
    {
        return FALSE;
    }

    CXMLNode*   pNode = (CXMLNode*)rtContextCursor.GetData();
    if(!pNode)
    {
        return FALSE;
    }
    CString strType;

    pNode->GetIdPath(m_strContextIdPath);
    m_tContextCursor = rtContextCursor;
    strType = GetNodeType(m_tContextCursor);
    m_eContextType = GetTypeFromTypeName(strType);

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * enable file check.
 *
 *  caution:
 *  dont forget to enable file check after disable it
 *  otherwise file checking is never enabled again.
 *
 * @param            [in] bFileCheck: enable file check?
 *
*/
void CFileTreeCtrl::EnableFileCheck(BOOL bFileCheck /*= TRUE*/)
{
    if(bFileCheck)
    {
        // do not decrement directly but send message
        PostMessage(WM_NAV_FV_DECR_CHECK_DISABLE_COUNT);
    }
    else
    {
        ++m_iCheckDisableCount;
    }
    TRACE("CheckDisableCount: %d\n", m_iCheckDisableCount);
}

void CFileTreeCtrl::OnDecrementCheckDisableCount(LPARAM lParam, WPARAM wParam)
{
    if(m_iCheckDisableCount > 0)
    {
        --m_iCheckDisableCount;
    }
}

BOOL CFileTreeCtrl::IsCheckAllowed()
{
    if(m_iCheckDisableCount)
    {
        return FALSE;
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * on file added.
 *
 *  this is called by project manager whenever a file was added to a
 *  certain category.
 *
 * @param           [in] crstrFilePath: file name or path, if not project relative
 * @param           [in] eFileCat: file category of new file
 * @return          -
 * @exception       -
 * @see             OnFileRemoved()
*/
void CFileTreeCtrl::OnFileAdded(const CString& crstrFilePath, enum E_FV_FileCategory eFileCat)
{
    // reject update
    if(!m_bLoaded || m_bRejectUpdateMessages || crstrFilePath[0] == _T('$'))
    {
        return;
    }

    CXMLNode    tNode;
    CXMLNode    tNewNodeTmp;
    BOOL        bFileExistsOnDisk = IsFileExisting(crstrFilePath);

    ASSERT(bFileExistsOnDisk);

    // look if file is already in tree
    CXMLNode    tFileNode;
    if(GetSystemNode(FV_FILE_UNKNOWN, tFileNode))
    {
        if(FindFileNode(crstrFilePath, tFileNode))
        {
            if(GetFileCategory(&tFileNode) != eFileCat)
            {
                DeleteXMLNode(&tFileNode);
                CTreeCursor tFileCursor;
                if(FindFileItem(crstrFilePath, tFileCursor))
                {
                    DeleteTreeItem(tFileCursor);
                }
            }
            else
            {
                return;     // nothing to do
            }
        }
    }

    // insert tree item
    if(GetSystemNode(eFileCat, tNode))
    {
        CString strIdPath;
        InsertNewXMLChildNode(&tNode, crstrFilePath, con_strTypeFile, tNewNodeTmp);
        tNewNodeTmp.GetIdPath(strIdPath);
        UpdateInsert(strIdPath);
    }
}

//------------------------------------------------------------------*
/**
 * on file removed.
 *
 *  this is called by project manager whenever a file was removed from a
 *  certain category.
 *
 * @param           [in] crstrFilePath: file name or path, if not project relative
 * @param           [in] eFileCat: file category of file that was removed
 * @return          -
 * @exception       -
 * @see             OnFileAdded()
*/
void CFileTreeCtrl::OnFileRemoved(const CString& crstrFilePath, enum E_FV_FileCategory eFileCat)
{
    // reject update
    if(!m_bLoaded || m_bRejectUpdateMessages || crstrFilePath[0] == _T('$'))
    {
        return;
    }

    // find file node
    CXMLNode    tNodeParent;
    BOOL        bFileExistsOnDisk = IsFileExisting(crstrFilePath);

    CXMLNode    tFileNode;
    if(GetSystemNode(FV_FILE_UNKNOWN, tFileNode))
    {
        if(FindFileNode(crstrFilePath, tFileNode))
        {
            DeleteXMLNode(&tFileNode);
        }
    }

    CTreeCursor tFileCursor;
    if(FindFileItem(crstrFilePath, tFileCursor))
    {
        DeleteTreeItem(tFileCursor);
    }

    if(bFileExistsOnDisk)
    {
        // get file cat from project manager
        CComBSTR    sFileName(crstrFilePath);
        E_FV_FileCategory   eFileCat;
        if(m_pProjMan->GetFileCategory(sFileName, &eFileCat) != S_OK)
        {
            eFileCat = FV_FILE_UNKNOWN;
        }

        CXMLNode    tNodeSystem;
        CXMLNode    tNodeNewTmp;
        CTreeCursor tCursorSystem = GetSystemCursor(eFileCat);
        if(GetSystemNode(eFileCat, tNodeSystem))
        {
            InsertNewXMLChildNode(&tNodeSystem, crstrFilePath, con_strTypeFile, tNodeNewTmp);
        }
        if(HTREEITEM(tCursorSystem))
        {
            UpdateTreeNodeInsertRun(tCursorSystem, tNodeSystem);
        }
    }
}


//------------------------------------------------------------------*
/**
 * is file existing on disk?.
 *
 *  uses file handler to get full path.
 *  project must be loaded and file handler created.
 *
 * @param           [in] crstrFileString: file string (name, rel path or abs path)
 * @return          exists?
 * @exception       -
 * @see             -
*/
BOOL CFileTreeCtrl::IsFileExisting(const CString& crstrFileString)
{
    BOOL    bFileExists = FALSE;
    
    CString strFilePath = GetFilePath(crstrFileString);
    CFileStatus tStatus;
    if(CFile::GetStatus(strFilePath, tStatus))
    {
        bFileExists = TRUE;
    }
    return bFileExists;
}

//------------------------------------------------------------------*
/**
 * IsFileReadOnly
 *
 * @param           [in] crstrFileString: file string (name, rel path or abs path)
 * @return          read only?
 * @exception       -
 * @see             -
*/
BOOL CFileTreeCtrl::IsFileReadOnly(const CString& crstrFileString)
{
    BOOL    bReadOnly = FALSE;
    CString strFilePath = GetFilePath(crstrFileString);

    CFileStatus tStatus;
    if(CFile::GetStatus(strFilePath, tStatus) && (tStatus.m_attribute & CFile::readOnly))
    {
        bReadOnly = TRUE;
    }
    return bReadOnly;
}



//------------------------------------------------------------------*
/**
 * IsFileReadOnly
 *
 *  is file read only?
 *  Version for tree cursor
 *  Uses IsFileReadOnly(file name)
 *
 * @param           rtTreeCursor: tree cursor to file node
 * @return          read only?
 * @exception       -
 * @see             IsFileReadOnly(file name)
*/
BOOL CFileTreeCtrl::IsFileReadOnly(CTreeCursor& rtTreeCursor)
{
    if(!HTREEITEM(rtTreeCursor))
    {
        return FALSE;
    }

    CXMLNode*   pNode = (CXMLNode*)rtTreeCursor.GetData();
    if(!pNode)
    {
        return FALSE;
    }
    CString strFileName;
    pNode->GetAttribute(CE_XMLATTR_TEXT, strFileName);

    return IsFileReadOnly(strFileName);
}

//------------------------------------------------------------------*
/**
 * find file node.
 *
 * @param           [in] rtParent: parent node
 * @param           [in] crstrFilePath: file name or path, if not project relative
 * @return          found?
 * @exception       -
 * @see             -
*/
BOOL CFileTreeCtrl::FindFileItem(const CString& crstrFilePath, CTreeCursor& rtCursorFile)
{
    HTREEITEM hItem;
    CString strKey(crstrFilePath);
    strKey.MakeLower();

    if(m_tFileAssoc.Lookup(strKey, hItem))
    {
        rtCursorFile = CTreeCursor(hItem, this);
        return TRUE;
    }
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * find file node.
 *
 * @param           crstrFilePath: file name
 * @param           rtNode: xml node
 * @return          found?
 * @exception       -
 * @see             -
*/
BOOL CFileTreeCtrl::FindFileNode(const CString& crstrFilePath, CXMLNode& rtNode)
{
    CString strSource;
    if(rtNode.GetAttribute(CE_XMLATTR_SOURCE, strSource))
    {
        if(crstrFilePath.CompareNoCase(strSource) == 0)
        {
            return TRUE;
        }
    }

    // run over children
    CXMLNode        tNodeChild;
    CXMLNodeList    tChildList;
    long            lNumChildren;
    long            lChild;

    if(rtNode.GetChildNodeList(tChildList))
    {
        lNumChildren = tChildList.GetLength();
        for(lChild = 0; lChild < lNumChildren; ++lChild)
        {
            tChildList.Item(lChild, tNodeChild);
            if(FindFileNode(crstrFilePath, tNodeChild))
            {
                rtNode = tNodeChild;
                return TRUE;
            }
        }
    }
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * insert item after.
 *
 *  call base class method and insert tree item in file assoc
 *
 * @param           [in] crstrText: text for new item
 * @param           [in] rtCursorParent: cursor to parent item
 * @param           [in] hItemAfter: tree item after which the new item is inserted
 * @param           [in] rtNode: corresponding xml node
 * @param           [in] iImageIdx: image index of new item
 * @return          cursor to new tree item
 * @exception       -
 * @see             CNavTreeCtrl::InsertItemAfter()
*/
CTreeCursor CFileTreeCtrl::InsertItemAfter(const CString& crstrText, CTreeCursor& rtCursorParent, HTREEITEM hItemAfter, CXMLNode& rtNode, int iImageIdx /*= -1*/)
{
    CTreeCursor tTreeCursor;
    tTreeCursor = CNavTreeCtrl::InsertItemAfter(crstrText, rtCursorParent, hItemAfter, rtNode, iImageIdx);
    if(HTREEITEM(tTreeCursor))
    {
        if(con_strTypeFile.Compare(GetNodeType(tTreeCursor)) == 0)
        {
            CString strKey(crstrText);
            strKey.MakeLower();
            m_tFileAssoc.SetAt(strKey, HTREEITEM(tTreeCursor));
        }
    }
    return tTreeCursor;
}

//------------------------------------------------------------------*
/**
 * add item tail.
 *
 *  call base class method and insert tree item in file assoc
 *
 * @param           [in] crstrText: text for new item
 * @param           [in] rtCursorParent: cursor to parent item
 * @param           [in] rtNode: corresponding xml node
 * @param           [in] iImageIdx: image index of new item
 * @return          cursor to new tree item
 * @exception       -
 * @see             CNavTreeCtrl::AddItemTail()
*/
CTreeCursor CFileTreeCtrl::AddItemTail(const CString& crstrText, CTreeCursor& rtCursorParent, CXMLNode& rtNode, int iImageIdx /*= -1*/)
{
    CTreeCursor tTreeCursor;
    tTreeCursor = CNavTreeCtrl::AddItemTail(crstrText, rtCursorParent, rtNode, iImageIdx);
    if(HTREEITEM(tTreeCursor))
    {
        if(con_strTypeFile.Compare(GetNodeType(tTreeCursor)) == 0)
        {
            CString strKey(crstrText);
            strKey.MakeLower();
            m_tFileAssoc.SetAt(strKey, HTREEITEM(tTreeCursor));
        }
    }
    return tTreeCursor;
}

//------------------------------------------------------------------*
/**
 * add item head.
 *
 *  call base class method and insert tree item in file assoc
 *
 * @param           [in] crstrText: text for new item
 * @param           [in] rtCursorParent: cursor to parent item
 * @param           [in] rtNode: corresponding xml node
 * @param           [in] iImageIdx: image index of new item
 * @return          cursor to new tree item
 * @exception       -
 * @see             CNavTreeCtrl::AddItemHead()
*/
CTreeCursor CFileTreeCtrl::AddItemHead(const CString& crstrText, CTreeCursor& rtCursorParent, CXMLNode& rtNode, int iImageIdx /*= -1*/)
{
    CTreeCursor tTreeCursor;
    tTreeCursor = CNavTreeCtrl::AddItemHead(crstrText, rtCursorParent, rtNode, iImageIdx);
    if(HTREEITEM(tTreeCursor))
    {
        if(con_strTypeFile.Compare(GetNodeType(tTreeCursor)) == 0)
        {
            CString strKey(crstrText);
            strKey.MakeLower();
            m_tFileAssoc.SetAt(strKey, HTREEITEM(tTreeCursor));
        }
    }
    return tTreeCursor;
}

//------------------------------------------------------------------*
/**
 * %function%
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CFileTreeCtrl::DeleteTreeItem(CTreeCursor& rtCursor)
{
    if(con_strTypeFile.Compare(GetNodeType(rtCursor)) == 0)
    {
        CString strText = rtCursor.GetText();
        strText.MakeLower();
        m_tFileAssoc.RemoveKey(strText);
    }
    CNavTreeCtrl::DeleteTreeItem(rtCursor);
}

//------------------------------------------------------------------*
/**
 * add source files.
 *
 * @param           [in] rastrFiles: string array with file names
 * @param           [in] crstrForeignProjPath: foreign project path where to be copied from
 * @return          number of copied files
 * @exception       -
 * @see             -
*/
int CFileTreeCtrl::AddSourceFiles(CStringArray& rastrFiles, const CString& crstrForeignProjPath)
{
    HRESULT     hr;
    int         iNumFiles = rastrFiles.GetSize();
    int         iNumFilesCopied = 0;
    CString     strFileName;
    CString     strFilePathName;
    CComBSTR    sFileName;
    CComBSTR    sFilePathName;

    CCEFileHandler* pFileHandler = ((CCEMAINApp*)AfxGetApp())->GetFileHandler();
    if(!pFileHandler)
    {
        return 0;
    }

    EnableFileCheck(FALSE);
    SaveProjectFile();

    for(int iFile = 0; iFile < iNumFiles; ++iFile)
    {
        strFileName = rastrFiles[iFile];
        strFilePathName = pFileHandler->GetFilePathName(strFileName, crstrForeignProjPath);
        sFileName = strFileName;
        sFilePathName = strFilePathName;

        hr = m_pProjMan->AddFile(sFilePathName, FV_FILE_SOURCE, TRUE);
        if(hr == S_OK)
        {
            if(CopyFileToProject(strFilePathName))
            {
                hr = m_pProjMan->AddFile(sFileName, FV_FILE_SOURCE, FALSE);
                if(hr == S_OK)
                {
                    ++iNumFilesCopied;
                }
            }
        }
        sFileName.Empty();
        sFilePathName.Empty();
    }
    EnableFileCheck(TRUE);
    return iNumFilesCopied;
}


//------------------------------------------------------------------*
/**
 * check source state
 *
 *  special version for file tree ctrl.
 *  if old fileview.xml no source attribute is available ->
 *  get file name via id.
 *
 * @param           rtTreeCursor: cursor to tree item
 * @param           bRecursive: recursive (default is TRUE)
 * @return          -
 * @exception       -
 * @see             -
*/
void CFileTreeCtrl::CheckNodeSourceState(CTreeCursor& rtTreeCursor, BOOL bRecursive /*=TRUE*/)
{
    BOOL    bExpanded = FALSE;

    CCEFileHandler* pFileHandler = ((CCEMAINApp*)AfxGetApp())->GetFileHandler();
    if(!pFileHandler)
    {
        return;
    }

    CString     strSource;
    CXMLNode*   pNode = (CXMLNode*)rtTreeCursor.GetData();
    if(pNode)
    {
        bExpanded = IsXMLNodeExpanded(pNode);
        if(pNode->GetAttribute(CE_XMLATTR_SOURCE, strSource))
        {
            EFileStatus eStatus = pFileHandler->GetFileStatus(strSource);
            SetItemState(rtTreeCursor, INDEXTOSTATEIMAGEMASK(eStatus), TVIS_STATEIMAGEMASK);
        }
        else
        {
            CString strType;
            if(pNode->GetAttribute(CE_XMLATTR_TYPE, strType) && pNode->GetAttribute(CE_XMLATTR_ID, strSource))
            {
                if(strType.Compare(con_strTypeFile) == 0)
                {
                    EFileStatus eStatus = pFileHandler->GetFileStatus(strSource);
                    SetItemState(rtTreeCursor, INDEXTOSTATEIMAGEMASK(eStatus), TVIS_STATEIMAGEMASK);
                }
            }
        }
    }

    if(bRecursive && bExpanded)
    {
        CTreeCursor tTreeCursorChild = rtTreeCursor.GetChild();
        if(HTREEITEM(tTreeCursorChild))
        {
            do
            {
                CheckNodeSourceState(tTreeCursorChild, bRecursive);
                tTreeCursorChild = tTreeCursorChild.GetNextSibl();
            } while(HTREEITEM(tTreeCursorChild));
        }
    }
}



//------------------------------------------------------------------*
/**
 * handle drag scroll message
 *
 * @see             COleDropTarget::OnDragScroll()
*/
//DROPEFFECT CFileTreeCtrl::OnDragScroll(CWnd* pWnd, DWORD dwKeyState, CPoint point)
//{
//    return(DROPEFFECT_NONE);
//}


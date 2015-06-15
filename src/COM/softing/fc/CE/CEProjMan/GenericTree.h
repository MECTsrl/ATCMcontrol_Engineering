#if !defined(AFX_GENERICTree_H__EFC2AEA1_E2D9_11D4_9506_0008C779D265__INCLUDED_)
#define AFX_GENERICTree_H__EFC2AEA1_E2D9_11D4_9506_0008C779D265__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include "FileHelper.h"



#define PRJMAN_VERSION _T("VERSION")




class CCEProjectManager;

////////////////////////////////////////////////////////////////////
// global defines


//------------------------------------------------------------------*
/**
 *  TSourceFileInfo containing all necessary information for a source file
 */
typedef struct _SourceFileInfo {
    CString  strFileName;      // the file nameof the source file
    CXMLNode xmlNode;          // dom node for that file
    CTime    parseSourceTime;  // modify file time of the source file 
                               //   when the source file has been parsed
    CString  strParentFileName;// the name of the parent source file
    CList<CString, CString&> strlChildFileNames; // a list with all child files, referenced via
                                    // a link tag in this source file
    CString strIdPath;              // the id path for the xml node (all id to the root
                                    // root node connected with /)
    BOOL bFileMissing;
} TSourceFileInfo;



//------------------------------------------------------------------*
/**
 *  CGenericTree contains all operations and information that all
 *  trees (target, class and instance tree) share. 
 *
 *  @see    CClassTree, CTargetTree
*/
class CGenericTree  
{

public:
    CGenericTree(CCEProjectManager* pProjMan);
    virtual ~CGenericTree();

    virtual BOOL InitTree()=0;
    virtual BOOL ReparseProjFile() { return FALSE; };
    BOOL Reparse(BOOL updateInfo = TRUE);
    BOOL CheckSourceFilesToReparse();
    BOOL CheckSourceFilesToReparseIncrementally();
    BOOL AddSourceFileToReparse(const CString& strFileName);
    CString GetTreeXML();
    BOOL SaveViewData();
    BOOL GetIXMLDoc(IUnknown** ppDoc);
    virtual BOOL IsViewPath(const CString& strIdPath)=0;
    virtual BOOL CreateLink(const CString& strLocation, 
                            const CString& strLinkName) { return FALSE; };
    virtual BOOL RenameLink(const CString& strLocation, 
                            const CString& strOldLinkName,
                            const CString& strNewLinkName) { return FALSE; };
    virtual BOOL DeleteLink(const CString& strLocation,
                            const CString& strLinkName) {return FALSE; };
    BOOL GetParentSourceAndId(const CString& strSourceFile, 
                              CString& strParentSourceFile,
                              CString& strParentId);
    BOOL IsTreeSourceFile(const CString& strFileName);
    BOOL GetAllTreeSourceFiles(CList<CString, CString&>& listSourceFiles);
    BOOL GetFileNameFromIdPath(const CString& strIdPath, CString& strFileName);
    BOOL GetIdPathFromFileName(const CString& strFileName, CString& strIdPath);
    BOOL GetAllClassesOfTypeUnderNode(const CString strStartNode, const CString strType, 
                                      SAFEARRAY** parrNodes);
    BOOL GetXMLNodePtr(const CString& strIdPath, IUnknown** ppXMLPtr);

    virtual BOOL FVAddFile(const CString& strFileName, const CString& strBackparsed, BOOL bTry = FALSE) {return FALSE;};
    virtual BOOL FVRemoveFile(const CString& strFileName, BOOL bTry = FALSE) {return FALSE;};

    void DeleteProjectBuildSem(const CString& strFileName);

protected:
    TSourceFileInfo* CreateSourceFileInfo(const CString& strFileName,
                                          const CXMLNode& xmlNode,
                                          const CString& strParentFileName);
    BOOL SetSourceTime(TSourceFileInfo* pFileInfo, const CString& strFile);
    BOOL RemoveSourceFileAndChilds(const CString& strFileName);
    virtual BOOL BuildErrorNode(TSourceFileInfo* pFileInfo);

	BOOL ReparseFile(TSourceFileInfo* pFileInfo, BOOL bAddToParent = TRUE);
    BOOL CheckForLinkedFiles(CXMLNode& node, CString& strFileName, CList<CString, CString&>& strlChildList);

    BOOL SetModTimeInfo(CXMLNode& node, const CTime& time);
    BOOL GetModTimeInfo(const CXMLNode& node, CTime& time);

    BOOL LoadViewData();
   
    virtual void FireUpdateTree(const CString& strChangedNode, enum E_UPDATE_MODE eMode)=0;
    //BOOL FillChangedNodesSafeArray(SAFEARRAY** pparrChangedNodes);
    //void AddChangedNode(CString& strChangedNode);
    void Reparse4CPAttributes(CXMLNode* pProjectNode);

    virtual void ParentCheck(const CXMLNode& parentNode, const CXMLNode& newNode);


protected:
    CMap<CString, LPCTSTR, TSourceFileInfo*, TSourceFileInfo*> m_mapSourceFiles;
    CList<CString, CString&> m_listFilesToReparse;

    CXMLDocument       m_domDocument;     // the xml dom document containing 
                                          // the whole xml tree for the view
    CCEProjectManager* m_pProjectManager;
    CString m_strFileName;                // the name of the file where the xml 
                                          // should be stored
    //CList<CString, CString&> m_listChangedNodes; // list will be filled during reparsing
                                          // of changed files, it holds all ID-Paths
                                          // of all nodes under which something 
                                          // has been changed
    BOOL m_bProjFileToReparse;
    BOOL m_bFileViewChanged;
    BOOL m_bDeleteSem;           // delete project build semaphore, this should not be done in the instance view

    BOOL m_bInit;                // will be true during init process, there should be no fire
                                 // update trees, because there has been no init yet
    BOOL m_bMsgMissingFiles;

private:
    BOOL UpdateFileInfo(const CXMLNode& node, const CString& strParentFile);
    BOOL UpdateChildFileInfo(const CXMLNode& node, const CString& strParentFile,
                             CList<CString, CString&>& childList);

    CList<CString, CString&> m_listFilesToCheck;
    POSITION m_posNextPosToCheck;

public:
    BOOL StringList_Find(CString strSearch, CList<CString, CString&>& list);
    void SourceFileMap_SetAt(CString strId, TSourceFileInfo* pSourceInfo, 
                CMap<CString, LPCTSTR, TSourceFileInfo*, TSourceFileInfo*>& map);
    BOOL SourceFileMap_Lookup(CString strId, TSourceFileInfo*& pSourceInfo,
                CMap<CString, LPCTSTR, TSourceFileInfo*, TSourceFileInfo*>& map);
    BOOL SourceFileMap_RemoveKey(CString strId,
                CMap<CString, LPCTSTR, TSourceFileInfo*, TSourceFileInfo*>& map);
};

#endif // !defined(AFX_GENERICTree_H__EFC2AEA1_E2D9_11D4_9506_0008C779D265__INCLUDED_)


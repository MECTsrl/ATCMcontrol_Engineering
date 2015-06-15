#if !defined(AFX_CLASSTree_H__EFC2AEA2_E2D9_11D4_9506_0008C779D265__INCLUDED_)
#define AFX_CLASSTree_H__EFC2AEA2_E2D9_11D4_9506_0008C779D265__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


 


//------------------------------------------------------------------*
// Includes
//
#include "GenericTree.h"


#define ID_PATH_CLASSVIEW _T("CLASSVIEW")

/**
 * TClassInfo holds information for a class in the class view
 */
typedef struct _ClassesInfo
{
    CString strText;        // the text shown in the navigator for the class group
    CString strSection;     // the section in the 4cp file 
    CString strType;        // the type of the class group
    CString strClassesType; // the type of the classes under the class group
    BOOL    bVisible;       // special case for the configuration: the group node 
                            // should be invisible
    CXMLNode groupNode;     // holding the node of that class group, 
                            // if bVisible == FALSE, groupNode holds the parent node
} TClassesInfo;


//------------------------------------------------------------------*
/**
 *  CClassTree is a derived class from CGeneralTree 
 *  this class contains specialized things for the class tree.
 *
 *  @see    CGenericTree.h (for base class description)
*/
class CClassTree : public CGenericTree  
{
public:
    CClassTree(CCEProjectManager* pProjMan);
    virtual ~CClassTree();

    virtual BOOL InitTree();
    virtual BOOL ReparseProjFile();
    virtual BOOL IsViewPath(const CString& strIdPath);
    virtual BOOL CreateLink(const CString& strLocation, 
                            const CString& strLinkName);
    virtual BOOL RenameLink(const CString& strLocation, 
                            const CString& strOldLinkName,
                            const CString& strNewLinkName);
    virtual BOOL DeleteLink(const CString& strLocation,
                            const CString& strLinkName);

    BOOL GetStaticInfos();

    BOOL GetAllClassTypes(SAFEARRAY** pparrClassTypes);
    BOOL GetTextForClassType(const CString& strClassType, CString& strText);
    BOOL GetAllClasses(SAFEARRAY** pparrClasses, BOOL bIdPaths);
    BOOL GetAllClassesOfType(const CString& strClass, SAFEARRAY** pparrClasses, BOOL bIdPaths);

    virtual BOOL FVAddFile(const CString& strFileName, const CString& strBackparsed, BOOL bTry = FALSE);
    virtual BOOL FVRemoveFile(const CString& strFileName, BOOL bTry = FALSE);


protected:
    virtual void FireUpdateTree(const CString& strChangedNode, enum E_UPDATE_MODE eMode);

    BOOL IsIdExisting(const CString& crstrId, const CString& crstrParentId);
    
    virtual void ParentCheck(const CXMLNode& parentNode, const CXMLNode& newNode);

private:
    CString m_strClassesType;
    CList<TClassesInfo, TClassesInfo&> m_listClassInfo;
};

#endif // !defined(AFX_CLASSTree_H__EFC2AEA2_E2D9_11D4_9506_0008C779D265__INCLUDED_)
#ifndef __INSTANCETREE_H_
#define __INSTANCETREE_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*
typedef CList<CString, CString&>                    TStringList;
typedef CMap<CString, LPCTSTR, CString, CString&>   TStringMap;

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


#include "GenericTree.h"

#define ID_PATH_INSTANCEVIEW _T("INSTANCEVIEW")


typedef struct _InstanceTargetInfo {
    CString  strTargetId;      // the Id of the target
    CXMLNode xmlNode;          // dom node for that file
    CString  strIdPath;             // the id path for the xml node (all id to the root
                                    // root node connected with /)
    BOOL     bUsed;
} TInstanceTargetInfo;



//------------------------------------------------------------------*
/**
 *  CInstanceTree is a derived class from CGeneralTree 
 *  this class contains specialized things for the instance tree.
 *
 *  @see    CGenericTree.h (for base class description)
*/
class CInstanceTree : public CGenericTree  
{
public:
    CInstanceTree(CCEProjectManager* pProjMan);
    virtual ~CInstanceTree();

    virtual BOOL InitTree();
    virtual BOOL ReparseProjFile();
    virtual BOOL IsViewPath(const CString& strIdPath);

    BOOL GetStaticInfos();

    BOOL GetAllInstancesOfType(const CString& strInstance, TStringList& tInstanceList, BOOL bIdPaths);

    BOOL UpdateTargets();

protected:
     // inform other modules about target instance changes
    virtual void FireUpdateTree(const CString& strChangedNode, enum E_UPDATE_MODE eMode);
    virtual BOOL BuildErrorNode(TSourceFileInfo* pFileInfo);
    BOOL GetTargetsFromTargetTree(TStringList& listTargets);

    void ExpandFromViewData();
    void ExpandParallel(CXMLNode& rtNodeNew, CXMLNode& rtNodeOld);

protected:
    CString                     m_strInstancesType;     // type id name of instance root node
    CMap<CString, LPCTSTR, TInstanceTargetInfo*, TInstanceTargetInfo*> m_mapInstanceTargets;
};

#endif // __INSTANCETREE_H_


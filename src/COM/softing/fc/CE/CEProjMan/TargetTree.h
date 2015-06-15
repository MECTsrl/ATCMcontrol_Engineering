#if !defined(AFX_TARGETTree_H__EFC2AEA3_E2D9_11D4_9506_0008C779D265__INCLUDED_)
#define AFX_TARGETTree_H__EFC2AEA3_E2D9_11D4_9506_0008C779D265__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



//------------------------------------------------------------------*
//  Includes
//
#include "GenericTree.h"

#define ID_PATH_TARGETVIEW _T("TARGETVIEW")


/**
 * TTargetInfo holds information for a target in the target view
 */
typedef struct _TargetInfo
{
    CString                  strType;           // the target type
    CString                  strFileExt;        // file extension
    CList<CString, CString&> listSuppDataTypes; // the list of supported data types
    CList<CString, CString&> listSuppLanguages; // the list of supported languages
    BOOL                     bSuppRetain;       // does target support retain modifier
} TTargetInfo;




//------------------------------------------------------------------*
/**
 *  class CTargetTree
 *  holds information for the target tree view.
 *
 *  @see    CGenericTree.h (for base class definition)
*/
class CTargetTree : public CGenericTree  
{
public:
    CTargetTree(CCEProjectManager* pProjMan);
    virtual ~CTargetTree();

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
    BOOL GetAllTargetTypes(SAFEARRAY** pparrTargetTypes);
    BOOL GetAllTargets(SAFEARRAY** pparrTargets, BOOL bIdPaths);

    virtual BOOL FVAddFile(const CString& strFileName, const CString& strBackparsed, BOOL bTry = FALSE);
    virtual BOOL FVRemoveFile(const CString& strFileName, BOOL bTry = FALSE);

    BOOL GetTypeOfTarget(const CString& strTargetId, CString& strType);
    BOOL GetAddressStringForTarget(const CString& strTargetId, CString& strAddr);
    BOOL GetUserAddressStringForTarget(const CString& strTargetId, CString& strUserAddr);
    BOOL GetTargetNameForAddr(const CString& strAddr, CString& strTarget);
    BOOL GetResourceNameForTarget(const CString& strTargetId, CString& strResName);
    BOOL GetFreeChannelNrForPC(const CString& strPCId, long& lChannelNr);
    BOOL GetSupportedDataTypes(SAFEARRAY** pparrDataTypes);
    BOOL GetSupportedDataTypesForTarget(const CString& strTargetType, SAFEARRAY** pparrDataTypes);
    BOOL GetSupportedLanguages(SAFEARRAY** pparrLanguages);
    BOOL GetSupportedLanguagesForTarget(const CString& strTargetType, SAFEARRAY** pparrLanguages);
    BOOL GetSupportRetain();
    BOOL GetSupportRetainForTarget(const CString& strTargetType);
    BOOL GetExtForTarget(const CString& strTargetType, CString& strExt);

    BOOL m_bTargetTreeChanged;  // Flag for instance view: if TRUE: target view has changed
                                //  instance view will reset the flag if the changes have been 
                                //  realized in the instance view.

protected:
    BOOL FindTargetType(const CString& strType);
    void FillTargetDataTypes(TTargetInfo* pTargetInfo, const CString strDataTypes);
    void CreateMaxSupportedDataTypes();
    void FillTargetLanguages(TTargetInfo* pTargetInfo, const CString strLanguages);
    void CreateMaxSupportedLanguages();

protected:
    virtual void FireUpdateTree(const CString& strChangedNode, enum E_UPDATE_MODE eMode);
    CList<TTargetInfo*, TTargetInfo*> m_listTargetInfo;
    CList<CString, CString&>          m_listMaxSuppDataTypes;
    CList<CString, CString&>          m_listMaxSuppLanguages;
    BOOL                              m_bSuppRetain;
};

#endif // !defined(AFX_TARGETTree_H__EFC2AEA3_E2D9_11D4_9506_0008C779D265__INCLUDED_)

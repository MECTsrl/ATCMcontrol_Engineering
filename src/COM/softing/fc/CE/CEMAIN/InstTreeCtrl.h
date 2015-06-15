#ifndef __INSTTREECTRL_H_
#define __INSTTREECTRL_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "NavTreeControl.h"
#include "CSC_Online.h"
#include "SBLLexTree.h"
#include "Subscription.h"

//----  Forward Class Definitions:   -------------------------------*
class CTargetOnlBase;
class CTargetOnl;
class CSubscriptionManager;
class CNavigator;
class CSubscrElement;

//----  Defines:    ------------------------------------------------*
typedef CSBLLexTree<CSubscrElement>                 TSubscrTree;
typedef CMap<CString, LPCTSTR, CString, LPCTSTR>    TStrStrMap;

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


//------------------------------------------------------------------*
/**
 *  class CInstTreeCtrl
 *
 *  - class for instance view
 *  - main functionality inherited from CNavTreeCtrl
 *  - target online object creation and destruction
 *  - target connect states
 *  - resource and task states (subscription and update via callbacks)
 *  - open console
 *
 *  @see    CNavTreeCtrl
 */
class CInstTreeCtrl : public CNavTreeCtrl
{
private:
    FC_CALLBACK(SubscriptionModifiedCB, CInstTreeCtrl) m_VarCB;

public:
    CInstTreeCtrl();
    virtual ~CInstTreeCtrl();

    // return view index (3 for instance view)
    int GetViewIndex();

    void AddTargetInstance(const CString& crstrId, const CString& crstrIdPath);
    void RemoveTargetInstance(const CString& crstrId);

    void OnSessionStateChange(CTargetOnlBase *pTarget, CSC_SESSION_STATE sessionState, HRESULT hrError);
    void OnDebugSessionStateChange(CTargetOnl *pTarget, CSC_SESSION_STATE sessionState, HRESULT hrError);

    void OnConnectStateChanged(CTargetOnlBase *pTarget);
    
    // unload project data
    virtual void Unload();

    // update attributes of node
    virtual BOOL UpdateAttributes(const CString& crstrIdPath);

    // d-2048 24.03.04 SIS >>
    virtual BOOL UpdateInsert(const CString& crstrIdPath);
    // d-2048 24.03.04 SIS <<
    
protected:
//    int GetStateFromSessionState(CSC_SESSION_STATE sessionState);

    void SubscribeStateVariables(CXMLNode& rtNode);
    void UnsubscribeStateVariables(CXMLNode& rtNode, BOOL bCheck = FALSE);
    void UnsubscribeStateVariables(const CString& crstrIdPath);

    void OnVarNotify(CSubscription* pSubscription);

    CString GetIdPathForTarget(const CString& crstrTarget);
    CString GetTargetIdFromIdPath(const CString& crstrIdPath);

    void UpdateTargetFromNodeData(CTargetOnlBase* pTargetOnl, CXMLNode* pNode);

    BOOL OpenInstance();

    virtual void UpdateTreeNode(CTreeCursor& rtTreeCursor, CXMLNode& rtNode);

    virtual BOOL IsDragAllowed(CXMLNode* pNode);

    // is kernel standard operation possible
    virtual BOOL IsKSOPossible(int iKSO, CXMLNode* pNode);

    // execute kernel standard operation (open console)
    virtual BOOL DoKernelStandardOperation(int iKSO, CXMLNode* pNode);

    virtual BOOL SetClipboardData(CXMLNode* pNode, BOOL bDragDrop = FALSE);
    
    HRESULT SetWatchClipboardData(CXMLNode* pNode, COleDataSource* pDataSource);
    
protected:
    //  Variable subsciption management of navigator
    CSubscriptionManager  m_tSubscrMan;  //subscription manager 
    // 
    TSubscrTree     m_tStateIntVarSubscrTree;   // mapping from id-path of nodes to subscription elements
    TSubscrTree     m_tStateTextVarSubscrTree;  // mapping from id-path of nodes to subscription elements
    TStrStrMap      m_tStateIntVarIdPathMap;    // mapping from variable to id-path
    TStrStrMap      m_tStateTextVarIdPathMap;   // mapping from variable to id-path
    UINT            m_uiWatchExpressionCBFormat;// cookie for watch expression
};

#endif // __INSTTREECTRL_H_


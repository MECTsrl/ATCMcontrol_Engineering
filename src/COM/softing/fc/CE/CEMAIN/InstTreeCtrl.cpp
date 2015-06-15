
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "InstTreeCtrl.h"
#include "CECompMan.h"
#include "CESysDef.h"
#include "TargetSet.h"
#include "TargetOnlBase.h"
#include "TargetOnl.h"
#include "navigator.h"
#include "cemain.h"
#include "SubscrElement.h"
#include "XMLHelper/IdPathHelp.h"
#include "MsgWnd.h"
#include "dragitem.h"
#include "utilif.h"
#include "CEFileHandler.h"
#include "Mainfrm.h"

//----  Static Initializations:   ----------------------------------*


CInstTreeCtrl::CInstTreeCtrl() :
    m_VarCB(this,OnVarNotify),
    m_tSubscrMan()
{
    m_tStateIntVarSubscrTree.setModusOwner();
    m_tStateTextVarSubscrTree.setModusOwner();
    m_uiWatchExpressionCBFormat = RegisterClipboardFormat(CB_FORMAT_4CWATCHEXPR);
}


CInstTreeCtrl::~CInstTreeCtrl()
{
}

//------------------------------------------------------------------*
/**
 * unload project data.
 *
 *  before unloading data, remove all subscriptions and stop
 *  subscription manager.
 *
 * @see             CNavTreeCtrl::Unload()
*/
void CInstTreeCtrl::Unload()
{
    CXMLNode    tRootNode;

    ASSERT(m_pXMLDoc);
    if(m_pXMLDoc)
    {
        if(m_pXMLDoc->GetRootNode(tRootNode))
        {
            UnsubscribeStateVariables(tRootNode);
        }
    }
    if(m_tSubscrMan.IsInitialized())
    {
        m_tSubscrMan.Stop();
    }

    CNavTreeCtrl::Unload();
}



int CInstTreeCtrl::GetViewIndex()
{
    return(3);
}

//------------------------------------------------------------------*
/**
 * get id path for target.
 *
 *  reconstructs id path from target id (name).
 *
 * @param           crstrTarget: target id (name)
 * @return          instance tree id path to target
*/
CString CInstTreeCtrl::GetIdPathForTarget(const CString& crstrTarget)
{
    CString strIdPath;
    strIdPath = (CString)CE_XML_IDPATH_SEP + m_strProjectName + (CString)CE_XML_IDPATH_SEP_DOT +
        (CString)CE_XML_IDPATH_INSTANCEVIEW + (CString)CE_XML_IDPATH_SEP + crstrTarget;
    return strIdPath;
}

//------------------------------------------------------------------*
/**
 * subscribe state variables.
 *
 *  runs over node and all subnode recursively and subscribes all state
 *  variables (STATE_VAR attribute)
 *
 * @param           rtNode: start node
 * @see             UnsubscribeStateVars()
*/
void CInstTreeCtrl::SubscribeStateVariables(CXMLNode& rtNode)
{
    // look for STATE_VAR
    CString         strStateVar;
    CString         strIdPath;
    CSubscription*  pSubscription;
    BOOL            bAddKey;
    CSubscrElement* pSubscrElement;
    CSubscrElement* pSubscrElTmp;

    if(!m_tSubscrMan.IsInitialized())
    {
        HRESULT hr;
        hr = m_tSubscrMan.Start();
        ASSERT(hr == S_OK);
    }

    if(rtNode.GetAttribute(CE_XMLATTR_STATE_VAR, strStateVar))
    {
        pSubscription = NULL;
        bAddKey = TRUE;

        rtNode.GetIdPath(strIdPath);
        m_tSubscrMan.GetSubscription(strStateVar, BL_DINT, pSubscription);

        if(pSubscription)
        {
            pSubscrElement = new CSubscrElement(pSubscription, strStateVar);
            pSubscrElTmp = m_tStateIntVarSubscrTree.getData(strIdPath);
            if(pSubscrElTmp)
            {
                // correct subscription already exists -> don't add it
                if(*pSubscrElTmp == *pSubscrElement)
                {
                    bAddKey = FALSE;
                }
                else    // state var is the same, but node changed -> remove it
                {
                    m_tStateIntVarSubscrTree.deleteData(strIdPath);
                    m_tStateIntVarIdPathMap.RemoveKey(strStateVar);
                }
            }
            if(bAddKey)
            {
                m_tStateIntVarSubscrTree.insertData(strIdPath, pSubscrElement);
                m_tStateIntVarIdPathMap.SetAt(strStateVar, strIdPath);
                pSubscription->RegisterModifiedCB(&m_VarCB);
                //TRACE("   >>>>>>> Subscribed %s\n", strIdPath);
            }
            else
            {
                delete pSubscrElement;
            }
        }
    }


    if(rtNode.GetAttribute(CE_XMLATTR_STATETEXT_VAR, strStateVar))
    {
        pSubscription = NULL;
        bAddKey = TRUE;

        rtNode.GetIdPath(strIdPath);
        m_tSubscrMan.GetSubscription(strStateVar, BL_WSTRING, pSubscription);

        if(pSubscription)
        {
            pSubscrElement = new CSubscrElement(pSubscription, strStateVar);
            pSubscrElTmp = m_tStateTextVarSubscrTree.getData(strIdPath);
            if(pSubscrElTmp)
            {
                // correct subscription already exists -> don't add it
                if(*pSubscrElTmp == *pSubscrElement)
                {
                    bAddKey = FALSE;
                }
                else    // state var is the same, but node changed -> remove it
                {
                    m_tStateTextVarSubscrTree.deleteData(strIdPath);
                    m_tStateTextVarIdPathMap.RemoveKey(strStateVar);
                }
            }
            if(bAddKey)
            {
                m_tStateTextVarSubscrTree.insertData(strIdPath, pSubscrElement);
                m_tStateTextVarIdPathMap.SetAt(strStateVar, strIdPath);
                pSubscription->RegisterModifiedCB(&m_VarCB);
                //TRACE("   >>>>>>> Subscribed %s\n", strIdPath);
            }
            else
            {
                delete pSubscrElement;
            }
        }
    }


    // run over all children and make recursive call
    CXMLNodeList    tNodeList;
    long            lNumChildren;
    long            lChild;
    CXMLNode        tNodeChildTmp;

    if(rtNode.GetChildNodeList(tNodeList))
    {
        lNumChildren = tNodeList.GetLength();
        for(lChild = 0; lChild < lNumChildren; ++lChild)
        {
            // get child
            tNodeList.Item(lChild, tNodeChildTmp);
            SubscribeStateVariables(tNodeChildTmp);
        }
    }
}


//------------------------------------------------------------------*
/**
 * unsubscribe state vars for target.
 *
 *  runs over all state vars and delete them from tree and map
 *
 * @param           rtNode: target node
 * @param           bCheck: if TRUE, only variables without correspoding node are unsubscribed
 *                                  default is FALSE
*/
void CInstTreeCtrl::UnsubscribeStateVariables(CXMLNode& rtNode, BOOL bCheck /*=FALSE*/)
{
    CString         strIdPath;
    CSubscrElement* pSubscrElement;
    CSubscription*  pSubscription;
    CString         strVar;
    CTreeCursor     tTreeCursor;
    CXMLNode        tNodeTmp;
    BOOL            bNodeExisting;
    CString         strIdPathTmp;

    VERIFY(rtNode.GetIdPath(strIdPath));

    CStringArray    astrIdPaths;
    m_tStateIntVarSubscrTree.getAllStringsFromSubstring(strIdPath, astrIdPaths);
    
    int iNumNodes = astrIdPaths.GetSize();
    for(int iNode = 0; iNode < iNumNodes; ++iNode)
    {
        strIdPathTmp = astrIdPaths[iNode];
        bNodeExisting = rtNode.GetNodeFromIdPath(strIdPathTmp, tNodeTmp);
        // if check mode and node exists, update state and tree item
        if(bCheck && bNodeExisting)
        {
            pSubscrElement = m_tStateIntVarSubscrTree.getData(strIdPathTmp);
            ASSERT(pSubscrElement);
            if(pSubscrElement)
            {
                CTreeCursor tTreeCursor;
                CString     strStateValue;
                pSubscription = pSubscrElement->GetSubscription();
                strStateValue = pSubscription->GetValue();
                tNodeTmp.SetAttribute(CE_XMLATTR_STATE, strStateValue);
                if(GetTreeCursorFromNodeIdPath(strIdPathTmp, tTreeCursor))
                {
                    UpdateItem(tTreeCursor);
                }
                continue;
            }
        }
        pSubscrElement = m_tStateIntVarSubscrTree.getData(strIdPathTmp);
        ASSERT(pSubscrElement);
        if(pSubscrElement)
        {
            pSubscription = pSubscrElement->GetSubscription();
            ASSERT(pSubscription);
            strVar = pSubscription->GetObject();
            m_tStateIntVarIdPathMap.RemoveKey(strVar);
            //TRACE("   >>>>>>> Unsubscribed %s\n", strIdPathTmp);
        }
        m_tStateIntVarSubscrTree.deleteData(strIdPathTmp);
        if(bNodeExisting)
        {
            tNodeTmp.RemoveAttribute(CE_XMLATTR_STATE);
            if(GetTreeCursorFromNodeIdPath(strIdPathTmp, tTreeCursor))
            {
                UpdateTreeItemState(tTreeCursor, &tNodeTmp);
            }
        }
    }

    astrIdPaths.RemoveAll();
    m_tStateTextVarSubscrTree.getAllStringsFromSubstring(strIdPath, astrIdPaths);

    iNumNodes = astrIdPaths.GetSize();
    for(iNode = 0; iNode < iNumNodes; ++iNode)
    {
        strIdPathTmp = astrIdPaths[iNode];
        bNodeExisting = rtNode.GetNodeFromIdPath(strIdPathTmp, tNodeTmp);
        // if check mode and node exists, update state and tree item
        if(bCheck && bNodeExisting)
        {
            pSubscrElement = m_tStateTextVarSubscrTree.getData(strIdPathTmp);
            ASSERT(pSubscrElement);
            if(pSubscrElement)
            {
                CTreeCursor tTreeCursor;
                CString     strStateValue;
                pSubscription = pSubscrElement->GetSubscription();
                strStateValue = pSubscription->GetValue();
                tNodeTmp.SetAttribute(CE_XMLATTR_STATETEXT, strStateValue);
                if(GetTreeCursorFromNodeIdPath(strIdPathTmp, tTreeCursor))
                {
                    UpdateItem(tTreeCursor);
                }
                continue;
            }
        }
        pSubscrElement = m_tStateTextVarSubscrTree.getData(strIdPathTmp);
        ASSERT(pSubscrElement);
        if(pSubscrElement)
        {
            pSubscription = pSubscrElement->GetSubscription();
            ASSERT(pSubscription);
            strVar = pSubscription->GetObject();
            m_tStateTextVarIdPathMap.RemoveKey(strVar);
        }
        m_tStateTextVarSubscrTree.deleteData(strIdPathTmp);
        if(bNodeExisting)
        {
            tNodeTmp.RemoveAttribute(CE_XMLATTR_STATETEXT);
            if(GetTreeCursorFromNodeIdPath(strIdPathTmp, tTreeCursor))
            {
                UpdateTreeItemState(tTreeCursor, &tNodeTmp);
            }
        }
    }
}


//------------------------------------------------------------------*
/**
 * unsubscribe state vars for deleted target.
 *
 * @param           crstrIdPath: id path to target
*/
void CInstTreeCtrl::UnsubscribeStateVariables(const CString& crstrIdPath)
{
    CString         strIdPath = crstrIdPath;
    CSubscrElement* pSubscrElement;
    CSubscription*  pSubscription;
    CString         strVar;
    CString         strIdPathTmp;

{
    CStringArray    astrIdPaths;
    m_tStateIntVarSubscrTree.getAllStringsFromSubstring(strIdPath, astrIdPaths);

    int iNumNodes = astrIdPaths.GetSize();
    for(int iNode = 0; iNode < iNumNodes; ++iNode)
    {
        strIdPathTmp = astrIdPaths[iNode];
        pSubscrElement = m_tStateIntVarSubscrTree.getData(strIdPathTmp);
        ASSERT(pSubscrElement);
        if(pSubscrElement)
        {
            pSubscription = pSubscrElement->GetSubscription();
            ASSERT(pSubscription);
            strVar = pSubscription->GetObject();
            m_tStateIntVarIdPathMap.RemoveKey(strVar);
        }
        m_tStateIntVarSubscrTree.deleteData(strIdPathTmp);
    }
}

{
    CStringArray    astrIdPaths;
    m_tStateTextVarSubscrTree.getAllStringsFromSubstring(strIdPath, astrIdPaths);

    int iNumNodes = astrIdPaths.GetSize();
    for(int iNode = 0; iNode < iNumNodes; ++iNode)
    {
        strIdPathTmp = astrIdPaths[iNode];
        pSubscrElement = m_tStateTextVarSubscrTree.getData(strIdPathTmp);
        ASSERT(pSubscrElement);
        if(pSubscrElement)
        {
            pSubscription = pSubscrElement->GetSubscription();
            ASSERT(pSubscription);
            strVar = pSubscription->GetObject();
            m_tStateTextVarIdPathMap.RemoveKey(strVar);
        }
        m_tStateTextVarSubscrTree.deleteData(strIdPathTmp);
    }
}

}


//------------------------------------------------------------------*
/**
 * insert target into target set.
 *
 * @param           crstrId: id of target (name)
 * @param           crstrIdPath: id path of target in instance tree
 * @see             RemoveTargetInstance()
*/
void CInstTreeCtrl::AddTargetInstance(const CString& crstrId, const CString& crstrIdPath)
{
    CTargetSet* pTargetSet = CEGetTargetSet(_T("Project Targets"));
    if(!pTargetSet)
    {
        TRACE("CInstTreeCtrl::AddTargetInstance: could'nt get target\n");
        return;
    }
    
    // get node and connect info
    CXMLNode    tRootNode;
    CXMLNode    tTargetInstNode;
    CString     strAddress;
    CString     strUserAddress;
    CString     strAssignedResource;
    CString     strType;
    CString     strTmp;
    CString     strWaitForTargetConnection;
    BOOL        bWaitForTargetConnection = TRUE;

    CTargetOnlBase* pTarget;

    if(!m_pXMLDoc)
    {
        return;
    }
    else
    {
        if(!m_pXMLDoc->GetRootNode(tRootNode))
        {
            return;
        }
    }
    
    if(!tRootNode.GetNodeFromIdPath(crstrIdPath, tTargetInstNode))
    {
        ASSERT(0);
        return;
    }

    tTargetInstNode.GetAttribute(CE_XMLATTR_ADDR, strAddress);
    tTargetInstNode.GetAttribute(CE_XMLATTR_USERADDR, strUserAddress);
    tTargetInstNode.GetAttribute(CE_XMLATTR_RESOURCE, strAssignedResource);
    tTargetInstNode.GetAttribute(CE_XMLATTR_TARGET_TYPE, strType);
    if (tTargetInstNode.GetAttribute(CE_XMLATTR_WAIT_TARGET_CONN, strWaitForTargetConnection))
    {
        if (strWaitForTargetConnection.Compare(_T("0"))==0)
        {
            bWaitForTargetConnection = FALSE;
        }
    }
    pTarget = pTargetSet->GetTarget(crstrId);
    if(pTarget)
    {
        strTmp = pTarget->GetAssignedResource();
        if(strAssignedResource.CompareNoCase(strTmp))
        {
            pTarget->SetAssignedResource(strAssignedResource);
        }
        strTmp = pTarget->GetIdPath();
        if(crstrIdPath.CompareNoCase(strTmp) != 0)
        {
            pTarget->SetIdPath(crstrIdPath);
        }
        strTmp = pTarget->GetControlAddress();
        if(strAddress.CompareNoCase(strTmp) != 0)
        {
            pTarget->SetControlAddress(strAddress);
            pTarget->SetUserControlAddress(strUserAddress);

            // now inform others about address change
            CCEStateManager* pStateManager = CEGetStateManager();
            if(pStateManager)
            {
                pStateManager->TriggerTargetAddressModify(pTarget);
            }
        }
        pTarget->SetWaitForTargetConnection(bWaitForTargetConnection);
    }
    else
    {
        pTarget = pTargetSet->AddTarget(crstrId, crstrIdPath, strType, strAssignedResource, strAddress, strUserAddress);
        if (pTarget)
        {
            pTarget->SetWaitForTargetConnection(bWaitForTargetConnection);
        }
    }
}

//------------------------------------------------------------------*
/**
 * remove target instance from target set.
 *
 * @param           crstrId: id of target (name)
 * @see             AddTargetInstance()
*/
void CInstTreeCtrl::RemoveTargetInstance(const CString& crstrId)
{
    CTargetSet* pTargetSet = CEGetTargetSet(_T("Project Targets"));
    pTargetSet->RemoveTarget(crstrId);
}


//------------------------------------------------------------------*
/**
 * on session state change.
 *
 *  notification handler of CTargetOnlBase.
 *  called whenever the session state of the target changes
 *
 * @param           pTarget: target onl base object
 * @param           sessionState: session state
 * @param           hrError: error HRESULT from target onl base object
 * @see             OnDebugSessionStateChange()
*/
void CInstTreeCtrl::OnSessionStateChange(CTargetOnlBase *pTarget, CSC_SESSION_STATE sessionState, HRESULT hrError)
{
    CString strIdPath = pTarget->GetIdPath();
    CTreeCursor tTreeCursor;
    CXMLNode    tRootNode;
    CXMLNode    tNode;
//    int         iState = GetStateFromSessionState(sessionState);

    if(m_pXMLDoc)
    {
        if(m_pXMLDoc->GetRootNode(tRootNode))
        {
            if(tRootNode.GetNodeFromIdPath(strIdPath, tNode))
            {
//                CString strState;
//                strState.Format("%d", sessionState);
//                strState.Format("%d", iState);
//                tNode.SetAttribute(CE_XMLATTR_STATE, strState);
                if(GetTreeCursorFromNodeIdPath(strIdPath, tTreeCursor))
                {
                    if(sessionState == RW_established)
                    {
                        SubscribeStateVariables(tNode);
                    }
                    if(sessionState == regular_shutdown)
                    {
                        UnsubscribeStateVariables(tNode);
                    }
                    UpdateTreeItemState(tTreeCursor, &tNode);
                }
            }
            else
            {
                // node is not existing, i. e. target was removed
                if(sessionState == regular_shutdown)
                {
                    TRACE("Unsubscribe state variables for %s\n", strIdPath);
                    UnsubscribeStateVariables(strIdPath);
                }
            }
        }
    }
}

//------------------------------------------------------------------*
/**
 * on debug session state change.
 *
 *  notification handler of CTargetOnlBase.
 *  called whenever the exclusive (debug) session state of the target changes
 *
 * @param           pTarget: target onl object
 * @param           sessionState: session state
 * @param           hrError: error HRESULT from target onl object
 * @see             OnSessionStateChange()
*/
void CInstTreeCtrl::OnDebugSessionStateChange(CTargetOnl *pTarget, CSC_SESSION_STATE sessionState, HRESULT hrError)
{
    CString strIdPath = pTarget->GetIdPath();
    CTreeCursor tTreeCursor;
    CXMLNode    tRootNode;
    CXMLNode    tNode;
    int         iState = sessionState;

    if(m_pXMLDoc)
    {
        if(m_pXMLDoc->GetRootNode(tRootNode))
        {
            if(tRootNode.GetNodeFromIdPath(strIdPath, tNode))
            {
                CString strState;
                strState.Format("%d", iState);
                tNode.SetAttribute(CE_XMLATTR_STATE, strState);
                if(GetTreeCursorFromNodeIdPath(strIdPath, tTreeCursor))
                {
                    UpdateTreeItemState(tTreeCursor, &tNode);
                }
            }
            else
            {
                TRACE("CInstTreeCtrl::OnSessionCB: node not found\n");
            }
        }
    }
}

//------------------------------------------------------------------*
/**
 * on debug session state change.
 *
 *  notification handler of CTargetOnlBase.
 *  called whenever the exclusive (debug) session state of the target changes
 *
 * @param           pTarget: target onl object
*/
void CInstTreeCtrl::OnConnectStateChanged(CTargetOnlBase *pTarget)
{
    CString strIdPath = pTarget->GetIdPath();
    CTreeCursor tTreeCursor;
    CXMLNode    tRootNode;
    CXMLNode    tNode;
    int         iState = (int) pTarget->GetConnectState();

    if(m_pXMLDoc)
    {
        if(m_pXMLDoc->GetRootNode(tRootNode))
        {
            if(tRootNode.GetNodeFromIdPath(strIdPath, tNode))
            {
                CString strState;
                strState.Format("%d", iState);
                tNode.SetAttribute(CE_XMLATTR_STATE, strState);
                if(GetTreeCursorFromNodeIdPath(strIdPath, tTreeCursor))
                {
                    UpdateTreeItemState(tTreeCursor, &tNode);
                }
            }
            else
            {
                TRACE("CInstTreeCtrl::OnConnectStateChanged: node not found\n");
            }
        }
    }
}



//------------------------------------------------------------------*
/**
 * on var notify.
 *
 *  callback handler for variable changes
 *
 * @param           pSubscription: subscription object
 * @return          -
 * @exception       -
 * @see             -
*/
void CInstTreeCtrl::OnVarNotify(CSubscription* pSubscription)
{
    TRACE("CInstTreeCtrl::OnVarNotify\n");
    CString strVal = pSubscription->GetValue();
    CString strVar = pSubscription->GetObject();
    CString strIdPath;
    CXMLNode    tRootNode;
    CXMLNode    tStateNode;
    CTreeCursor tTreeCursor;
    
    if(m_tStateIntVarIdPathMap.Lookup(strVar, strIdPath))
    {
        if ((pSubscription->GetQuality() & 192) != 192)
             strVal = _T("");
        TRACE("   IdPath = %s\n", strIdPath);
        if(m_pXMLDoc)
        {
            VERIFY(m_pXMLDoc->GetRootNode(tRootNode));
            tRootNode.GetNodeFromIdPath(strIdPath, tStateNode);
            tStateNode.SetAttribute(CE_XMLATTR_STATE, strVal);
            if(GetTreeCursorFromNodeIdPath(strIdPath, tTreeCursor))
            {
                UpdateTreeItemState(tTreeCursor, &tStateNode);
            }
        }
    }
    else if(m_tStateTextVarIdPathMap.Lookup(strVar, strIdPath))
    {
        if ((pSubscription->GetQuality() & 192) != 192)
        {
             //would benice to see this as info but not yet written variables seem to have bad 
            //quality 
            strVal.Format(_T("!!Quality bad=0x%x"), pSubscription->GetQuality());
        }

        TRACE("   IdPath = %s\n", strIdPath);
        if(m_pXMLDoc)
        {
            VERIFY(m_pXMLDoc->GetRootNode(tRootNode));
            tRootNode.GetNodeFromIdPath(strIdPath, tStateNode);
            tStateNode.SetAttribute(CE_XMLATTR_STATETEXT, strVal);
            if(GetTreeCursorFromNodeIdPath(strIdPath, tTreeCursor))
            {
                UpdateTreeItemState(tTreeCursor, &tStateNode);
            }
        }
    }
}


//------------------------------------------------------------------*
/**
 * update tree node.
 *
 *  Check subscriptions
 *
 * @param           rtTreeCursor: cursor to tree item to be updated
 * @return          -
 * @exception       -
 * @see             
*/
void CInstTreeCtrl::UpdateTreeNode(CTreeCursor& rtTreeCursor, CXMLNode& rtNode)
{
    CNavTreeCtrl::UpdateTreeNode(rtTreeCursor, rtNode);

    CTargetSet* pTargetSet;
    pTargetSet = CEGetTargetSet(_T("Project Targets"));
    ASSERT(pTargetSet);
    if(pTargetSet)
    {

        // first get target id
        CString         strIdPath;
        CString         strTargetId;
        CTargetOnlBase* pTargetOnl;

        VERIFY(rtNode.GetIdPath(strIdPath));
        strTargetId = GetTargetIdFromIdPath(strIdPath);
        pTargetOnl = pTargetSet->GetTarget(strTargetId);
        if(pTargetOnl)
        {
            // TODO: only preliminary as long as no project manager event
            // like target modified
            UpdateTargetFromNodeData(pTargetOnl, &rtNode);
            if(pTargetOnl->IsConnected())
            {
                UnsubscribeStateVariables(rtNode, TRUE);
                SubscribeStateVariables(rtNode);
            }
        }
    }
}



//------------------------------------------------------------------*
/**
 * update target from node data.
 *
 * @param           pTargetOnl: pointer to target online object
 * @param           pNode: pointer to target node
 * @return          -
*/
void CInstTreeCtrl::UpdateTargetFromNodeData(CTargetOnlBase* pTargetOnl, CXMLNode* pNode)
{
    if(!pTargetOnl || !pNode)
    {
        return;
    }

    CString         strAddress;
    CString         strUserAddress;
    BOOL            bUpdateMsgVw = FALSE;


    if(pNode->GetAttribute(CE_XMLATTR_ADDR, strAddress))
    {
        if(strAddress.CompareNoCase(pTargetOnl->GetControlAddress())!=0)
        {
            CString strMessage;
            if(pTargetOnl->IsConnected())
            {
                // address change of connected target should not happen
                ASSERT(0);
                strMessage.LoadString(IDS_WARNING_ADDRESS_CHANGED);
                AfxMessageBox(strMessage);
            }
            pTargetOnl->SetControlAddress(strAddress);
            bUpdateMsgVw = TRUE;
        }
    }
    if(pNode->GetAttribute(CE_XMLATTR_USERADDR, strUserAddress))
    {
        pTargetOnl->SetUserControlAddress(strUserAddress);
        bUpdateMsgVw = TRUE;
    }
    if(bUpdateMsgVw)
    {
        // inform message window
        CMsgWnd *pMsgWnd = dynamic_cast<CMsgWnd *> (CEGetDockingTool("Message View", false));
        if(pMsgWnd != NULL)
        {
            pMsgWnd->UpdateTargetConnectState(pTargetOnl);
        }
    }
}


//------------------------------------------------------------------*
/**
 * update attributes.
 *
 * @param           crstrIdPath: id path of node
 * @return          successful?
 * @exception       -
 * @see             CNavTreeCtrl::UpdateAttributes()
*/
BOOL CInstTreeCtrl::UpdateAttributes(const CString& crstrIdPath)
{
    // state var attributes are not changed up to now
    // -> just call base class
    return CNavTreeCtrl::UpdateAttributes(crstrIdPath);
}



//------------------------------------------------------------------*
/**
 * get target id from id path
 *
 * @param           crstrIdPath: node id path
 * @return          target id.
 * @exception       -
 * @see             -
*/
CString CInstTreeCtrl::GetTargetIdFromIdPath(const CString& crstrIdPath)
{
    // id path looks like this: /ProjName.INSTANCEVIEW/target_id/...
    CString strIdPath = crstrIdPath;
    CString strId;

    for(int i=0; i<3; ++i)
    {
        StripIdFromIdPath(strIdPath, strId);
    }
    return strId;
}


//------------------------------------------------------------------*
/**
 * is drag allowed.
 *
 *  base class method overwritten to allow all nodes to be dragged.
 *
 * @param           pNode: node pointer
 * @return          is drag allowed?
 * @exception       -
 * @see             CNavTreeCtrl::IsDragAllowed()
*/
BOOL CInstTreeCtrl::IsDragAllowed(CXMLNode* pNode)
{
    if(!pNode)
    {
        return FALSE;
    }
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * set clipboard data.
 *
 *  base class method overwritten to create drag / drop format for watch.
 *
 * @param           pNode: pointer to node
 * @param           bDragDrop: start drag drop operation
 * @return          successful?
*/
BOOL CInstTreeCtrl::SetClipboardData(CXMLNode* pNode, BOOL bDragDrop /*=FALSE*/)
{
    HRESULT hr;
    BOOL    bDataCached = FALSE;

    COleDataSource* pDataSource = new COleDataSource;
    if(!pDataSource)
    {
        ::AfxThrowMemoryException();
    }

    // cache xml format for editor
    hr = SetEditorClipboardData(pNode, pDataSource);
    if(!SUCCEEDED(hr))
    {
        delete pDataSource;
        return FALSE;
    }
    if(hr == S_OK)
    {
        bDataCached = TRUE;
    }

    // cache xml node format for navigator
    hr = SetInternalClipboardData(pNode, pDataSource);
    if(!SUCCEEDED(hr))
    {
        delete pDataSource;
        return FALSE;
    }
    if(hr == S_OK)
    {
        bDataCached = TRUE;
    }

    // cache xml format for watch
    hr = SetWatchClipboardData(pNode, pDataSource);
    if(!SUCCEEDED(hr))
    {
        delete pDataSource;
        return FALSE;
    }
    if(hr == S_OK)
    {
        bDataCached = TRUE;
    }

    // if no data was cached -> return false
    if(!bDataCached)
    {
        delete pDataSource;
        return FALSE;
    }

    // copy cached data to clipboard
    if(bDragDrop)
    {
        pDataSource->DoDragDrop();
        pDataSource->InternalRelease();
    }
    else
    {
        pDataSource->SetClipboard();
    }

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * set clipboard data for watch.
 *
 *  if exception occurs, data source will be destroyed
 *
 * @param           pNode: pointer to node
 * @param           pDataSource: pointer to data source object
 * @return          S_OK:       data was copied successfully
 *                  S_FALSE:    no data was copied
 *                  E_FAIL:     error occurred
*/
HRESULT CInstTreeCtrl::SetWatchClipboardData(CXMLNode* pNode, COleDataSource* pDataSource)
{
    CString strExpression;

    if(!pNode)
    {
        return FALSE;
    }
    if(!pNode->GetAttribute(CE_XMLATTR_INSTANCE_PATH, strExpression))
    {
        return FALSE;
    }
    if(strExpression.IsEmpty())
    {
        return FALSE;
    }

	try
	{
        CWatchExpressionDragItem dragItem(strExpression, _T(""));
        CSharedFile file;
        CArchive ar(&file, CArchive::store);

        dragItem.Serialize(ar);

        ar.Close();

        pDataSource->CacheGlobalData(m_uiWatchExpressionCBFormat, file.Detach());
	}
	catch(CMemoryException *e)
	{
		//out of memory
		//TODO status output
        delete pDataSource;
		e->Delete();
        return FALSE;
	}
	catch(CFileException* e)
	{
		//file op failed
		//TODO status output
        delete pDataSource;
		e->Delete();
        return FALSE;
	}
	catch(CArchiveException* e)
	{
		//archive op failed
		//TODO status output
        delete pDataSource;
		e->Delete();
        return FALSE;
	}
	catch(HRESULT hrRes)
	{
		//hr exception
		//TODO status output
        delete pDataSource;
		HRESULT hr = hrRes;
        return FALSE;
	}
    return TRUE;
}



//------------------------------------------------------------------*
/**
 * is kernel standard operation possible?.
 *
 *  each kernel standard operation is dependent on a certain attribute
 *  if this attribute is existing, the operation can be performed.
 *
 * @param           iKSO: number of kernel standard operation (see kso.h)
 * @param           pNode: pointer to currently selected XML node
 * @return          successful?
 *
 * @see             AddContextMenuItem()
*/
BOOL CInstTreeCtrl::IsKSOPossible(int iKSO, CXMLNode* pNode)
{
    BOOL    bReturn = FALSE;
    CTargetSet* pTargetSet = CEGetTargetSet("Project Targets", false);
    if(!pTargetSet)
    {
        return FALSE;
    }

    switch(iKSO)
    {
    case KSO_OPEN_CONSOLE:
        {
            CString         strIdPath;
            CString         strTargetId;
            CString         strId;
            CString         strType;
            CString         strUrl;
            CTargetOnlBase* pTarget = NULL;

            // get target online object
            pNode->GetIdPath(strIdPath);
            strTargetId = GetTargetIdFromIdPath(strIdPath);
            pTarget = pTargetSet->GetTarget(strTargetId);
            bReturn = pTarget->IsConsoleType();
        }
        break;

//    case KSO_OPEN_CONSOLE:      // connected to attribute CONSOLE_URL
//        pNode->GetAttribute(CE_XMLATTR_CONSOLE_URL, strAttribValue);
//        break;
    case KSO_OPEN_INSTANCE:
        {
            CString strAttribValue;
            if(pNode->GetAttribute(CE_XMLATTR_INSTANCE_PATH, strAttribValue)
               //state text may containd BP: or WAIT: location
              || pNode->GetAttribute(CE_XMLATTR_STATETEXT, strAttribValue)
              ) 
            {
                bReturn = TRUE;
            }
        }
        break;
    }
    if(!bReturn)
    {
        return CNavTreeCtrl::IsKSOPossible(iKSO, pNode);
    }
    return bReturn;
}

//------------------------------------------------------------------*
/**
 * execute kernel standard operation
 *
 * @param           iKSO: number of kernel standard operation
 * @param           rtTreeCursor: cursor to current tree item
 * @return          operation performed?
*/
BOOL CInstTreeCtrl::DoKernelStandardOperation(int iKSO, CXMLNode* pNode)
{
    CString     strSource;
    BOOL        bReturn = TRUE;     // operation found?
    if(!pNode)
    {
        return FALSE;
    }

    CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
    if(!pMainFrame)
    {
        return FALSE;
    }

    // is there only one target defined?
    CTargetSet* pTargetSet = CEGetTargetSet("Project Targets", false);
    if(!pTargetSet)
    {
        return FALSE;
    }

    switch(iKSO)
    {
    case KSO_OPEN_CONSOLE:
//        {
//            CString strConsoleURL;
//            pNode->GetAttribute(CE_XMLATTR_CONSOLE_URL, strConsoleURL);
//            CreatePanelView (strConsoleURL);
//        }
//        break;
        {
            CString         strIdPath;
            CString         strTargetId;
            CString         strId;
            CString         strType;
            CString         strUrl;
            CTargetOnlBase* pTarget = NULL;
            CString         strControlAddr;

            // get target online object
            pNode->GetIdPath(strIdPath);
            strTargetId = GetTargetIdFromIdPath(strIdPath);
            pTarget = pTargetSet->GetTarget(strTargetId);

            // get type and id
            pNode->GetAttribute(CE_XMLATTR_ID, strId);
            pNode->GetAttribute(CE_XMLATTR_TYPE, strType);
            ASSERT(pTarget);
            if(pTarget)
            {
                strControlAddr = pTarget->GetControlAddress();
                // remove brackets
                strControlAddr = strControlAddr.Left(strControlAddr.GetLength()-1);
                strControlAddr = strControlAddr.Mid(1);
                if(strType.CompareNoCase(_T("RESOURCE_INST")) == 0)
                {
                    strUrl.Format(CONSOLE_TASKLIST_FACEPLATE, (LPCTSTR)strControlAddr, UTIL_GetMasterPassword(), (LPCTSTR)strId);
                }
                else if(strType.CompareNoCase(_T("TASK_INST")) == 0)
                {
                    strUrl.Format(CONSOLE_TASK_FACEPLATE, (LPCTSTR)strControlAddr, UTIL_GetMasterPassword(), (LPCTSTR)strId);
                }
                else
                {
                    strUrl.Format(CONSOLE_MAIN_FACEPLATE, (LPCTSTR)strControlAddr, UTIL_GetMasterPassword());
                }
            }
            pMainFrame->CreatePanelView(strUrl);
        }
        break;
    case KSO_OPEN_INSTANCE:
        OpenInstance();
        break;
    default:
        bReturn = FALSE;
        break;
    }

    if(bReturn == FALSE)
    {
        return CNavTreeCtrl::DoKernelStandardOperation(iKSO, pNode);
    }
    return bReturn;
}



//------------------------------------------------------------------*
/**
 * open instance.
 *
 *  get editor container and call ShowInstance()
 *
*/
BOOL CInstTreeCtrl::OpenInstance()
{
    CTreeCursor tContextCursor;
    if(!GetTreeCursorFromNodeIdPath(m_strContextIdPath, tContextCursor))
    {
        return FALSE;
    }
    CString strSource;
    CString strInstPath;

    CXMLNode*   pNode = (CXMLNode*)tContextCursor.GetData();
    ASSERT(pNode);
    pNode->GetAttribute(CE_XMLATTR_SOURCE, strSource);
    pNode->GetAttribute(CE_XMLATTR_INSTANCE_PATH, strInstPath);

    CString strLocation;
    CString strStateValue;
    if(pNode->GetAttribute(CE_XMLATTR_STATETEXT, strStateValue))
    {
        int idx = strStateValue.Find('@');
        if(idx>0)
        {
            CXMLNode tParNode;
            //make inst path complete HACK: assume that it is not complete?
            //works only if task node:
            if(pNode->GetParentNode(tParNode))
                tParNode.GetAttribute(CE_XMLATTR_INSTANCE_PATH, strInstPath);
            strInstPath += _T(".");    
            strInstPath += strStateValue.Right(strStateValue.GetLength()-idx-1);

            strSource    = strStateValue.Left(idx);

            idx = strSource.Find('(');
            if(idx>0)
            {
                strLocation = strSource.Right(strSource.GetLength()-idx-1);
                strLocation.TrimRight(")");
                strSource = strSource.Left(idx);
            }
        }
    }



    CCEFileHandler* pFileHandler = ((CCEMAINApp*)AfxGetApp())->GetFileHandler();
    if(pFileHandler)
    {
        CGenEdDoc *pDoc = NULL;
        pFileHandler->ShowInstance(strSource, strInstPath);
        if(!strLocation.IsEmpty())
            pFileHandler->ShowFileAt(strSource, strLocation, pDoc, true);
        return TRUE;
    }
    return FALSE;
}


// d-2048 24.03.04 SIS >>
// overwrite base class to subscribe state variables
// this is necessary when the user is connected exclusively to a target
// that has not been compiled yet and a compilation is started.
BOOL CInstTreeCtrl::UpdateInsert(const CString& crstrIdPath)
{
    BOOL bRet = CNavTreeCtrl::UpdateInsert(crstrIdPath);

    CTreeCursor tTreeCursor;
    if(GetTreeCursorFromNodeIdPath(crstrIdPath, tTreeCursor))
    {
        CXMLNode* pNode = (CXMLNode*)tTreeCursor.GetData();
        if(pNode)
        {
            CTargetSet* pTargetSet;
            pTargetSet = CEGetTargetSet(_T("Project Targets"));
            ASSERT(pTargetSet);
            if(pTargetSet)
            {
                // first get target id
                CString         strIdPath;
                CString         strTargetId;
                CTargetOnlBase* pTargetOnl;

                VERIFY(pNode->GetIdPath(strIdPath));
                strTargetId = GetTargetIdFromIdPath(strIdPath);
                pTargetOnl = pTargetSet->GetTarget(strTargetId);
                if(pTargetOnl)
                {
                    // TODO: only preliminary as long as no project manager event
                    // like target modified
                    UpdateTargetFromNodeData(pTargetOnl, pNode);
                    if(pTargetOnl->IsConnected())
                    {
                        UnsubscribeStateVariables(*pNode, TRUE);
                        SubscribeStateVariables(*pNode);
                    }
                }
            }
        }
    }
    return bRet;
}
// d-2048 24.03.04 SIS <<



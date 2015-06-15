

// msgdoc.cpp : implementation file
//

#include "stdafx.h"
#include "msgdoc.h"
#include "msgthrd.h"
#include "MsgWnd.h"
#include "cecompman.h"
#include "CESysDef.h"

#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMsgData
CMsgData::CMsgData()
{
    m_hChildStdoutRd = NULL;
    m_hChildStdoutWr = NULL;
    m_hChildStdinRd = NULL;
    m_hChildStdinWr = NULL;
    m_pMsgThrd = NULL;
    m_pTarget = NULL;
    m_strText = _T("");
    m_iLines = 0;
    m_dwEditPos = 0;
    m_bEndPos = TRUE;
    m_bAlarm = FALSE;
    m_MsgViewerType = MSG_CmdLineMsgViewer;
    m_hwndClient = NULL;
    m_pIFCMessageSession = NULL;
    m_dwCookie = NULL;
    return;
}

CMsgData::CMsgData(CMsgData & other)
{
    if(this == &other)
    {
        return;
    }

    m_hChildStdoutRd = other.m_hChildStdoutRd;
    m_hChildStdoutWr = other.m_hChildStdoutWr;
    m_hChildStdinRd = other.m_hChildStdinRd;
    m_hChildStdinWr = other.m_hChildStdinWr;
    m_pMsgThrd = other.m_pMsgThrd;
    m_pTarget = other.m_pTarget;
    m_strText = other.m_strText;
    m_iLines = other.m_iLines;
    m_dwEditPos = other.m_dwEditPos;
    m_bAlarm = other.m_bAlarm;
    m_MsgViewerType = other.m_MsgViewerType;
    m_hwndClient = other.m_hwndClient;
    m_pIFCMessageSession = other.m_pIFCMessageSession;
    m_dwCookie = other.m_dwCookie;
	return;
}

CMsgData & CMsgData::operator = (CMsgData & other)
{
    if(this == &other)
    {
        return *this;
    }

    m_hChildStdoutRd = other.m_hChildStdoutRd;
    m_hChildStdoutWr = other.m_hChildStdoutWr;
    m_hChildStdinRd = other.m_hChildStdinRd;
    m_hChildStdinWr = other.m_hChildStdinWr;
    m_pMsgThrd = other.m_pMsgThrd;
    m_pTarget = other.m_pTarget;
    m_strText = other.m_strText;
    m_iLines = other.m_iLines;
    m_dwEditPos = other.m_dwEditPos;
    m_bAlarm = other.m_bAlarm;
    m_MsgViewerType = other.m_MsgViewerType;
    m_hwndClient = other.m_hwndClient;
    m_pIFCMessageSession = other.m_pIFCMessageSession;
    m_dwCookie = other.m_dwCookie;
    return *this;
}

CMsgData::~CMsgData()
{
}


/////////////////////////////////////////////////////////////////////////////
// CMsgDoc

IMPLEMENT_DYNCREATE(CMsgDoc, CDocument)

const int CMsgDoc::TextModifiedHint = 1000;


CMsgDoc::CMsgDoc()
{
  m_arrMsgData.SetSize(0, CE_MAXTARGET + 2);
}

BOOL CMsgDoc::OnNewDocument() 
{
    if(!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CMsgDoc::~CMsgDoc() 
{
}


BEGIN_MESSAGE_MAP(CMsgDoc, CDocument)
	//{{AFX_MSG_MAP(CMsgDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgDoc diagnostics

#ifdef _DEBUG
void CMsgDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMsgDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMsgDoc serialization

void CMsgDoc::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

HRESULT CMsgDoc::RegisterSessionCB(unsigned int uiIndex, FC_CALLBACK_BASE(TargetOnlSessionCB) *pSessionCB)
{
	CTargetOnlBase *pTargetBase;
	HRESULT hr;

	pTargetBase = GetTarget(uiIndex);
	assert(pTargetBase != NULL);
	if(pTargetBase == NULL)
	{
		return(E_FAIL);
	}

	hr = pTargetBase->RegisterSessionCB(pSessionCB);
	return(hr);
}


HRESULT CMsgDoc::UnregisterSessionCB(unsigned int uiIndex, FC_CALLBACK_BASE(TargetOnlSessionCB) *pSessionCB)
{
	CTargetOnlBase *pTargetBase;
	HRESULT hr;

	pTargetBase = GetTarget(uiIndex);
	assert(pTargetBase != NULL);
	if(pTargetBase == NULL)
	{
		return(E_FAIL);
	}

	hr = pTargetBase->UnregisterSessionCB(pSessionCB);
	return(hr);
}


HRESULT CMsgDoc::RegisterExclusiveSessionCB(unsigned int uiIndex, FC_CALLBACK_BASE(TargetOnlExclusiveSessionCB) *pExclusiveSessionCB)
{
	CTargetOnlBase *pTargetBase;
    CTargetOnl* pTarget;
	HRESULT hr;

	pTargetBase = GetTarget(uiIndex);
	assert(pTargetBase != NULL);
	if(pTargetBase == NULL)
	{
		return(E_FAIL);
	}

    pTarget = dynamic_cast<CTargetOnl*> (pTargetBase);
    if(pTarget)
    {
	    hr = pTarget->RegisterExclusiveSessionCB(pExclusiveSessionCB);
    }
    else
    {
        return(E_FAIL);
    }
	return(hr);
}


HRESULT CMsgDoc::UnregisterExclusiveSessionCB(unsigned int uiIndex, FC_CALLBACK_BASE(TargetOnlExclusiveSessionCB) *pExclusiveSessionCB)
{
	CTargetOnlBase *pTargetBase;
    CTargetOnl* pTarget;
	HRESULT hr;

	pTargetBase = GetTarget(uiIndex);
	assert(pTargetBase != NULL);
	if(pTargetBase == NULL)
	{
		return(E_FAIL);
	}

    pTarget = dynamic_cast<CTargetOnl*> (pTargetBase);
    if(pTarget)
    {
	    hr = pTarget->UnregisterExclusiveSessionCB(pExclusiveSessionCB);
    }
    else
    {
        return(E_FAIL);
    }
	return(hr);
}


HRESULT CMsgDoc::RegisterConnectStateCB (unsigned int uiIndex, FC_CALLBACK_BASE (TargetOnlConnectStateCB) *pConnectStateCB)
{
	CTargetOnlBase *pTargetBase;
	HRESULT hr;

	pTargetBase = GetTarget(uiIndex);
	assert(pTargetBase != NULL);
	if(pTargetBase == NULL)
	{
		return(E_FAIL);
	}

    hr = pTargetBase->RegisterConnectStateCB(pConnectStateCB);
	return(hr);
}


HRESULT CMsgDoc::SubscribeVariable(unsigned int uiIndex, const CString &strName, FC_CALLBACK_BASE(SubscriptionModifiedCB) *pModifiedCB, CSubscription *&pSubscription)
{
	CTargetOnlBase *pTargetBase;
	HRESULT hr;

	pTargetBase = GetTarget(uiIndex);
	ASSERT(pTargetBase != NULL);
	if(pTargetBase == NULL)
	{
		return(E_FAIL);
	}

	hr = pTargetBase->SubscribeVariable(strName, pModifiedCB, pSubscription);
	return(hr);
}


HRESULT CMsgDoc::UnsubscribeVariable(unsigned int uiIndex, CSubscription *pSubscription, FC_CALLBACK_BASE(SubscriptionModifiedCB) *pModifiedCB)
{
	CTargetOnlBase *pTargetBase;
	HRESULT hr;

	pTargetBase = GetTarget(uiIndex);
	ASSERT(pTargetBase != NULL);
	if(pTargetBase == NULL)
	{
		return(E_FAIL);
	}

	hr = pTargetBase->UnsubscribeVariable(pSubscription, pModifiedCB);
	return(hr);
}


HRESULT CMsgDoc::ReadVariable(unsigned int uiIndex, CSubscription *pSubscription, short &sQuality, CComVariant &varValue)
{
	CTargetOnlBase *pTargetBase;
	HRESULT hr;

	if((pTargetBase = GetTarget(uiIndex)) == NULL)
	{
		return(S_FALSE);
	}

	hr = pTargetBase->ReadVariable(pSubscription, sQuality, varValue);
	return(hr);
}


CTargetOnlBase *CMsgDoc::GetTarget(unsigned int uiIndex)
{
    ASSERT((int)uiIndex < m_arrMsgData.GetSize());
    if((int)uiIndex >= m_arrMsgData.GetSize())
    {
        return(NULL);
    }

    ASSERT(m_arrMsgData[uiIndex].m_MsgViewerType == MSG_ControlMsgViewer);
    if(m_arrMsgData[uiIndex].m_MsgViewerType != MSG_ControlMsgViewer)
    {
        return(NULL);
    }

	return(m_arrMsgData[uiIndex].m_pTarget);
}

void CMsgDoc::Clear(unsigned int uiIndex)
{
    CMsgWnd *pMessageWindow;

    m_arrMsgData[uiIndex].m_strText.Empty();
    m_arrMsgData[uiIndex].m_iLines = 0;
    m_arrMsgData[uiIndex].m_bEndPos = TRUE;

    UpdateAllViews(NULL, TextModifiedHint, (CObject *)uiIndex);
    
    pMessageWindow = dynamic_cast<CMsgWnd *> (CEGetDockingTool("Message View"));
    if(pMessageWindow == NULL)
    {
        return;
    }

    // Reset alarm for corresponding tab
    pMessageWindow->ResetAlarm(uiIndex);
}

void CMsgDoc::ClearAll()
{
    int iIndex;
    for (iIndex=0; iIndex<m_arrMsgData.GetSize(); iIndex++)
    {
        m_arrMsgData[iIndex].m_strText.Empty();
        m_arrMsgData[iIndex].m_iLines = 0;
        m_arrMsgData[iIndex].m_bEndPos = TRUE;
    }
    UpdateAllViews(NULL, TextModifiedHint, NULL);
}

CMsgData *CMsgDoc::FindControlMsgData(const CString &strTargetName, int *iIndex)
{
    return(FindMsgData(MSG_ControlMsgViewer, &strTargetName, iIndex));
}


CMsgData *CMsgDoc::FindControlMsgData(CTargetOnlBase *pTargetBase, int *iIndex)
{
    CMsgData *pMsgData;

    // Lookup message data by Target name.
    //////////////////////////////////

    pMsgData = FindControlMsgData(pTargetBase->GetId(), iIndex);
    if(pMsgData == NULL)
        return(NULL);


    // Target names should be unique. But for safety check pointer equality.
    ////////////////////////////////////////////////////////////////////

    ASSERT(pMsgData->m_pTarget == pTargetBase);
    if(pMsgData->m_pTarget != pTargetBase)
        return(NULL);

    return(pMsgData);
}


CMsgData *CMsgDoc::FindMsgData(MSG_VIEWER_TYPE viewerType, const CString *pstrName, int *iIndex)
{
    int i;

    // Must have a Target name if looking for a control message viewer.
    ///////////////////////////////////////////////////////////////

    ASSERT(viewerType != MSG_ControlMsgViewer || pstrName != NULL);
    if(viewerType == MSG_ControlMsgViewer && pstrName == NULL)
        return(NULL);

    for(i = 0; i < m_arrMsgData.GetSize(); ++i)
    {

        // Not the requested kind of message viewer? Continue!
        //////////////////////////////////////////////////////

        if(m_arrMsgData[i].m_MsgViewerType != viewerType)
            continue;


        if(viewerType == MSG_ControlMsgViewer)
        {

            // Must have a CTargetOnlBase object if it is a control message viewer.
            ///////////////////////////////////////////////////////////////
        
            ASSERT(m_arrMsgData[i].m_pTarget != NULL);
            if(m_arrMsgData[i].m_pTarget == NULL)
                continue;


            // Not the Target we are looking for? Continue!
            ///////////////////////////////////////////

            if(pstrName->CompareNoCase(m_arrMsgData[i].m_pTarget->GetId()) != 0)
                continue;
        }

        if(iIndex != NULL)
        {
            *iIndex = i;
        }

        return(&m_arrMsgData[i]);
    }

    return(NULL);
}


BOOL CMsgDoc::SaveModified() 
{
	//return CDocument::SaveModified();
	return TRUE;
}


//------------------------------------------------------------------*
/**
 * get target insert position.
 *
 * @param           [in] crstrTargetId: target id of new target
 * @return          index where to insert new target
 * @exception       -
 * @see             
*/
int CMsgDoc::GetTargetInsertPosition(const CString& crstrTargetId)
{
    int         iReturn;
    CMsgData    tMsgData;
    int         iMax = m_arrMsgData.GetSize() - 2;
    CString     strNewTargetId(crstrTargetId);
    CString     strTargetId;

    ASSERT(iMax >= 0);

    strNewTargetId.MakeLower();

    iReturn = iMax;

    // run over all target tabs
    for(int i=2; i < iMax; ++i)
    {
        tMsgData = m_arrMsgData.GetAt(i);
        if(tMsgData.m_pTarget)
        {
            strTargetId = tMsgData.m_pTarget->GetId();
            strTargetId.MakeLower();
            if(strTargetId > strNewTargetId)
            {
                iReturn = i;
                break;
            }
        }
    }
    return iReturn;
}

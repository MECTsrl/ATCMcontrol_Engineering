#if !defined(AFX_MSGDOC_H__24241912_93C0_11D1_AE0E_02608C7C2DDE__INCLUDED_)
#define AFX_MSGDOC_H__24241912_93C0_11D1_AE0E_02608C7C2DDE__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// msgdoc.h : header file
//

#include "msgthrd.h"
#include "CSC_Online.h"
#include "fccallback.h"
#include "targetonlbase.h"
#include "targetonl.h"

/////////////////////////////////////////////////////////////////////////////
// CMsgData

typedef enum tagMSG_VIEWER_TYPE 
{
    MSG_CmdLineMsgViewer = 0,
    MSG_CompileMsgViewer,
    MSG_DownLoadMsgViewer,
    MSG_ControlMsgViewer,
    MSG_FindInFilesMsgViewer,
    MSG_SystemMsgViewer
} MSG_VIEWER_TYPE;

class CMsgData
{
public:
    CMsgThrd*       m_pMsgThrd;	
    HANDLE		    m_hChildStdoutRd;
    HANDLE		    m_hChildStdoutWr;
    HANDLE		    m_hChildStdinRd;
    HANDLE		    m_hChildStdinWr;
    CTargetOnlBase* m_pTarget;
    CString         m_strText;
    int             m_iLines;
    DWORD           m_dwEditPos;
    BOOL            m_bEndPos;
    BOOL            m_bAlarm;
    MSG_VIEWER_TYPE	m_MsgViewerType;	// Type of the MsgViewer to handle such a MsgData
    HWND		    m_hwndClient;			// window handle of the client which receive the MsgText
    CComPtr<IFCConnect>  m_pIFCMessageSession;
                                        // only used for ControlMsgViewer
                                        // COM-Pointer to hold the Targets MessageSession IF
    DWORD       m_dwCookie;             // only used for ControlMsgViewer
                                        // holds the cookie from the advice of the MsgSource

    CMsgData();
    CMsgData(CMsgData & other);
    CMsgData & operator = (CMsgData & other);
    virtual ~CMsgData();
};


/////////////////////////////////////////////////////////////////////////////
// CMsgDoc

class CMsgDoc : public CDocument
{
protected:
    CMsgDoc();           // protected constructor used by dynamic creation
    DECLARE_DYNCREATE(CMsgDoc)

// Attributes
    CArray<CMsgData, CMsgData &> m_arrMsgData;

public:
    static const int TextModifiedHint;

    CTargetOnlBase* GetTarget (unsigned int uiIndex);
    HRESULT RegisterSessionCB (unsigned int uiIndex, FC_CALLBACK_BASE (TargetOnlSessionCB) *pSessionCB);
    HRESULT UnregisterSessionCB (unsigned int uiIndex, FC_CALLBACK_BASE (TargetOnlSessionCB) *pSessionCB);
    HRESULT RegisterExclusiveSessionCB (unsigned int uiIndex, FC_CALLBACK_BASE (TargetOnlExclusiveSessionCB) *pExclusiveSessionCB);
    HRESULT RegisterConnectStateCB (unsigned int uiIndex, FC_CALLBACK_BASE (TargetOnlConnectStateCB) *pConnectStateCB);
    HRESULT UnregisterExclusiveSessionCB (unsigned int uiIndex, FC_CALLBACK_BASE (TargetOnlExclusiveSessionCB) *pExclusiveSessionCB);
    HRESULT SubscribeVariable (unsigned int uiIndex, const CString &strName, FC_CALLBACK_BASE (SubscriptionModifiedCB) *pModifiedCB, CSubscription *&pSubscription);
    HRESULT UnsubscribeVariable (unsigned int uiIndex, CSubscription *pSubscription, FC_CALLBACK_BASE (SubscriptionModifiedCB) *pModifiedCB);
    HRESULT ReadVariable (unsigned int uiIndex, CSubscription *pSubscription, short &sQuality, CComVariant &varValue);
    CMsgData *FindControlMsgData (const CString &strTargetName, int *iIndex=NULL);
    CMsgData *FindControlMsgData (CTargetOnlBase *pTarget, int *iIndex=NULL);
    CMsgData *FindMsgData (MSG_VIEWER_TYPE viewerType, const CString *pstrName, int *iIndex=NULL);

    int GetTargetInsertPosition(const CString& crstrTargetId);

    void Clear (unsigned int uiIndex);
    void ClearAll();

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMsgDoc)
	public:
    virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
    virtual BOOL OnNewDocument();
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CMsgDoc();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
    //{{AFX_MSG(CMsgDoc)
        // NOTE - the ClassWizard will add and remove member functions here.
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGDOC_H__24241912_93C0_11D1_AE0E_02608C7C2DDE__INCLUDED_)
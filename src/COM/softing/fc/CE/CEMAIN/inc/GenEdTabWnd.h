
#ifndef __GENEDTABWND_H_
#define __GENEDTABWND_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "MsgTabWnd.h"
#include "FcDataTip.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CGenEdCont;


class CGenEdTabWnd : public CMsgTabWnd
{
//   friend class CwndTab;
//	DECLARE_DYNCREATE(CGenEdTabWnd)
public:
	CGenEdTabWnd();           
	virtual ~CGenEdTabWnd();

//    SECTab* AddTab(CRuntimeClass* pViewClass,
//                    LPCTSTR lpszLabel,
//                    CCreateContext* pContext =NULL,
//                    CInstTabInfo* pInstTabData = NULL,
//                    UINT nID = -1);
//
//    SECTab* InsertTab(CRuntimeClass* pViewClass,
//                    int nIndex,
//                    LPCTSTR lpszLabel,
//                    CCreateContext* pContext =NULL,
//                    CInstTabInfo* pInstTabData = NULL,
//                    UINT nID = -1);

    SECTab* AddTab(CRuntimeClass* pViewClass,
                    LPCTSTR lpszLabel,
                    CCreateContext* pContext =NULL,
                    UINT nID = -1);

    SECTab* InsertTab(CRuntimeClass* pViewClass,
                    int nIndex,
                    LPCTSTR lpszLabel,
                    CCreateContext* pContext =NULL,
                    UINT nID = -1);

    void SetContainer(CGenEdCont* pContainer);

    CString GetToolTipTextForTab(int iTab);

    HRESULT EnableTips(CWnd *pTabWindow);
    void DisableTips();

    void ActivateTips();
    void DeactivateTips();

    void RelayMessage(MSG *pMessage);

protected:
    virtual void OnDataTipPopup(bool &bAllow);
    virtual void OnDataTipActionQuery(CPoint point, FCDataTipAction &action);

    int TabHitTest(CPoint point);

    CFCDataTip m_DataTip;

    int m_iDataTipTab;

private:
	FC_CALLBACK (FCDataTipPopupCB, CGenEdTabWnd)       m_DataTipPopupCB;
	FC_CALLBACK (FCDataTipActionQueryCB, CGenEdTabWnd) m_DataTipActionQueryCB;

    CGenEdCont*     m_pContainer;           // pointer to editor container for get active document
    int             m_uSelectedViewIndex;   // Index for current selected view

    int             m_iReplaceMode;

    CImageList*     m_pImageList;


// Overrides
public:

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGenEdTabWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
	//{{AFX_MSG(CGenEdTabWnd)
    afx_msg LRESULT OnTabSelect(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
    afx_msg void OnContextMenu( CWnd* pWnd, CPoint pos );
    afx_msg void OnTabContextMenu(WPARAM tab, LPARAM pos);
	DECLARE_MESSAGE_MAP()
};


#endif // __GENEDTABWND_H_


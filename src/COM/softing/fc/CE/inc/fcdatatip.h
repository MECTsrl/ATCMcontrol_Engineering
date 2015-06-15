
#ifndef _DATATIP_H
#define _DATATIP_H

#include "fccallback.h"


enum FCDataTipAction
{
	FCShowDataTip,
	FCHideDataTip,
	FCMoveDataTip
};


FC_CALLBACK_DEF1 (FCDataTipPopupCB, bool &);

FC_CALLBACK_DEF2 (FCDataTipActionQueryCB, CPoint, FCDataTipAction &)


class CFCDataTip : public CToolTipCtrl
{
public:
	enum PopupState
	{
		Sleeping,
		Triggered,
		PoppedUp
	};

	CFCDataTip ();
	~CFCDataTip ();

	void SetPopupCB (FC_CALLBACK_BASE (FCDataTipPopupCB) *pPopupCB);
	void SetActionQueryCB (FC_CALLBACK_BASE (FCDataTipActionQueryCB) *pActionQueryCB);

	HRESULT Create (CWnd *pParent, DWORD dwStyle, CWnd *pTool);
	HRESULT Destroy ();
	bool IsCreated ();

	HRESULT Activate ();
	HRESULT Deactivate ();
	bool IsActive ();

	HRESULT Show ();
	HRESULT Hide ();
	HRESULT Move ();
	PopupState GetPopupState ();
	bool IsSleeping ();
	bool IsTriggered ();
	bool IsPoppedUp ();

	HRESULT SetText (const CString &strText);

	void RelayMessage (LPMSG pMessage);

protected:
	DECLARE_MESSAGE_MAP()

	void PropagateText ();

	afx_msg void OnNeedText (NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPop (NMHDR *pNMHDR, LRESULT *pResult);

	FC_CALLBACK_BASE (FCDataTipPopupCB)       *m_pPopupCB;
	FC_CALLBACK_BASE (FCDataTipActionQueryCB) *m_pActionQueryCB;

	bool       m_bCreated;
	bool       m_bActive;
	PopupState m_PopupState;
	bool       m_bInCallback;
	CWnd       *m_pTool;
	CString    m_strText;
	bool       m_bDelayedDelTool;
};


#endif

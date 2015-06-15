
#ifndef __GENEDDOC_H_
#define __GENEDDOC_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

// GenEdDoc.h : interface of the CGenEdDoc class
//
/////////////////////////////////////////////////////////////////////////////

#include "cesysdef.h"
#include "gened.h"
#include "StdEditorIF.h"
#include "fccallback.h"
#include "geneddef.h"
#include "BrkPtMan.h"
#include "CEStateMan.h"
#include "csc_online.h"
#include <atlbase.h>

class CGenEdView;
class CGenEdCntrItem;
class CGenEdDoc;
class CDocumentSettings;
class CGenEdFrame;

enum GenEdAction { GenEdDocumentSaved, GenEdDocumentClosed };

FC_CALLBACK_DEF2 (GenEdActionCB, GenEdAction, CGenEdDoc *)


class CGenEdDoc : public COleDocument
{
public:
//	enum LoadMode { SimpleLoad, SplitLoad };

	virtual ~CGenEdDoc ();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void RegisterActionCB (FC_CALLBACK_BASE (GenEdActionCB) *pActionCB);
	void UnregisterActionCB (FC_CALLBACK_BASE (GenEdActionCB) *pActionCB);

	void SetBreakpointManager (CComPtr<IBreakPoint> pIBreakPoint);
//	void SetSettings (CGCDocSettings *pGCSettings);
	void UpdateSettings ();
	HRESULT SetPrintInfo (const CString &strPrinterName, long lWidth, long lHeight);

	CString GetDocumentFile ();

	virtual BOOL OnOpenDocument (LPCTSTR pszPathName);
	virtual void OnCloseDocument ();
	virtual BOOL OnSaveDocument (LPCTSTR pszPathName);
	void SaveFailed ();

    bool SaveIfModified (bool bAskBeforeSave);
	virtual BOOL DoFileSave ();
	virtual BOOL DoSave (LPCTSTR pszPathName, BOOL bReplace);

    LRESULT OnParseNow(WPARAM wParam, LPARAM lParam);


	CGenEdCntrItem *GetEditorServer();
//    CGenEdCntrItem *GetSecondaryServer (unsigned int uiIndex);
	CGenEdCntrItem *GetActiveServer();

//	CGenEdCntrItem *CreateServer (const CLSID &classId, bool bPrimary);
	CGenEdCntrItem *CreateServer (const GUID& rtGUID);

//	HRESULT LoadServers (LoadMode loadMode);
	HRESULT LoadServer();
	HRESULT ReloadServer();

    void SetDelayedPositioning (bool bDelay);

	void GetInstance (CString &strInstance, CString &strMachineName, CString &strMachineAddress) const;
	void SetInstance (const CString &strInstanceName, const CString &strMachineName, const CString &strMachineAddress);

	void NotifyConnect (const CString &strMachineAddress);
	void NotifyDisconnect (const CString &strMachineAddress);
	void NotifyDebugState (CSC_SESSION_STATE state);

	void ForceReadOnly (bool bOn);

	HRESULT GotoPosition (CGenEdCntrItem *pServer, EDP_POS_QUALIFIER posQual, long lX, long lY, long lCX, long lCY);
	HRESULT GotoLocation (const CString &strLocation);
	HRESULT ServerFromLocation (const CString &strLocation, CGenEdCntrItem *&pServer);

	bool GetModified ();
	void ClearModified ();
	CString GetPositionText ();

	HRESULT ShowBreakpoint (EDP_POS_QUALIFIER posQual, long lX, long lY, long lCX, long lCY, short sState);
	HRESULT HideBreakpoint (EDP_POS_QUALIFIER posQual, long lX, long lY, long lCX, long lCY);
	HRESULT ShowStoppoint  (EDP_POS_QUALIFIER posQual, long lX, long lY, long lCX, long lCY);
	HRESULT HideStoppoint ();

	HRESULT InsertFBCall  (const CString &strFB);
    HRESULT InsertVariable(const CString &strVar);

	HRESULT StoreWatchpoints ();
	HRESULT RestoreWatchpoints ();

    HRESULT GetSelectedText(CString &strText);

    bool ModifyCheck ();
    void ResetCheckedFileTime();

    CDocumentSettings* GetDocumentSettings();

    void EnableParsing(BOOL bParse = TRUE);

    BOOL IsReadOnly();

	// SIS 22.07.04 >>
	// maximum count of open documents
	void IncreaseOpenDocumentCount();
	void DecreaseOpenDocumentCount();
	// SIS 22.07.04 <<

protected:
	CString m_strDocumentFile;
	CGenEdCntrItem *m_pEditorServer;
	FC_CALLBACK_HOLDER (GenEdActionCB) m_ActionCBs;
	CString m_strInstance;
	CString m_strMachineName;
    CString m_strMachineAddress;
    CString m_strInstanceWithTargetName;
	CComPtr<IBreakPoint> m_pIBreakPoint;
//	LoadMode m_LoadMode;
//	CGCDocSettings *m_pGCSettings;
	CString m_strPrinterName;
	long    m_lPrintWidth;
	long    m_lPrintHeight;
    CTime m_CheckedFileTime;
    BOOL    m_bParse;
    BOOL    m_bReadOnly;

    FC_CALLBACK (CEStateChangeCB, CGenEdDoc) m_StateChangeCB;

	CGenEdDoc ();

	virtual void UpdateModifiedFlag ();
	virtual void OnUpdateMruList (CCmdUI *pCmdUI);

private:
	BOOL CreateTemporaryDocFile ();
	HRESULT LoadEditorServer (const CString &strFile);
    HRESULT DeleteTemporaryFiles ();
    bool CheckWriteable (const CString &strPath);
    void OnStateChange (CEStateNotify stateNotify, unsigned int uiStage);
    void OnTargetAddressModified();

    CStringArray* GetWatchpoints(const CString &strInstance, BOOL bCreate);

    CGenEdFrame* GetGenEdFrame();

//wizard:
	DECLARE_DYNCREATE(CGenEdDoc)

public:
	//{{AFX_VIRTUAL(CGenEdDoc)
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();
	//}}AFX_VIRTUAL

	DECLARE_MESSAGE_MAP()
	//{{AFX_MSG(CGenEdDoc)
    afx_msg void OnFileClose();
    afx_msg void OnFileSave();
	afx_msg void OnSaveAs();
	//}}AFX_MSG

	DECLARE_DISPATCH_MAP()
	//{{AFX_DISPATCH(CGenEdDoc)
	afx_msg SCODE GetContainer (LPDISPATCH FAR* pContainerDispatch);
	afx_msg SCODE SetBreakpoint(long lQualifier, long lX, long lY, long lCX, long lCY);
	afx_msg SCODE ClearBreakpoint(long lQualifier, long lX, long lY, long lCX, long lCY);
	afx_msg SCODE ActivateBreakpoint(long lQualifier, long lX, long lY, long lCX, long lCY);
	afx_msg SCODE DeactivateBreakpoint(long lQualifier, long lX, long lY, long lCX, long lCY);
	afx_msg SCODE ContinueTask();
    //mode=0: single step, mode=1: step over, mode=2: step out 
	afx_msg SCODE StepTask(long mode);
	afx_msg SCODE ClearInstanceBreakpoints();
	afx_msg SCODE ClearAllBreakpoints();
	afx_msg SCODE OnEditEvent();
	//}}AFX_DISPATCH

	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.


#endif // __GENEDDOC_H_



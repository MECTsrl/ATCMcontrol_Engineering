
#ifndef __GENEDCONT_H_
#define __GENEDCONT_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "CEWindowPlacement.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

// GenEdCont.h : Header-Datei
//

class CGenEdDocTemplate;
class CGenEdFrame;
class CGenEdView;
class CGenEdCntrItem;
class CGCSettings;
class CPCOnl;
class CGenEdDoc;
struct CE_SRCDOCPOS_TYP;


typedef CMap<CString, LPCTSTR, CGenEdDoc*, CGenEdDoc*>    TExtDocMap;

#include "GenEdDoc.h"
#include "GenEdFrame.h"
#include "CEProjMan.h"
#include "KADHelper/KADFileInfo.h"
#include "BrkPtMan.h"
#include "DbgInfo.h"
#include "settings.h"

class CGenEdCont;

/////////////////////////////////////////////////////////////////////////////
// Befehlsziel CGenEdCont 

FC_CALLBACK_DEF (DocumentsReloadedCB);


class CGenEdCont : public CCmdTarget
{
public:
//	enum EEditorServer { STEditor, GREditor, Librarian, HtmlEditor };

	CGenEdCont ();
	virtual ~CGenEdCont ();

    // initialization
	HRESULT Init ();
	void Cleanup ();

    void LoadPrintServers();
	CGenEdDoc* LoadPrintServer(const CString& crstrExt);
	void UnloadPrintServers();
    CGenEdDoc* GetFirstPrintServer();
    CGenEdDoc* GetNextPrintServer();

	CGenEdDocTemplate *GetDocTemplate (bool bTabbed);

	CGenEdDoc* GetFirstDocument (CE_SRCDOCPOS_TYP &position, BOOL bTabbed = TRUE);
	CGenEdDoc* GetNextDocument (CE_SRCDOCPOS_TYP &position, BOOL bTabbed = TRUE);
	CGenEdDoc* GetActiveDocument ();
	CGenEdDoc* FindDocument (const CString &strFilePath);

	CGenEdDoc* CreateUntypedDocument (bool bVisible=true);
	CGenEdDoc* CreateTypedDocument (const CString &strExtension, bool bNeverTabbed, bool bVisible=true);
	CGenEdDoc* OpenDocument (const CString &strFileName, bool bNeverTabbed, bool bVisible = true, E_ActivateAction eAction = eActivateClass, bool bForceAsText = false);
    CGenEdDoc* RecoverDocument(const CString& crstrDocFilePath, bool bNeverTabbed);


	bool SaveDocument (CGenEdDoc* pDoc);
	bool LoadDocument (CGenEdDoc* pDoc);
	void ActivateDocument (CGenEdDoc* pDoc, E_ActivateAction eAction = eActivateClass);
	void CloseDocument (CGenEdDoc* pDoc);

	void SetFocusToServer (CGenEdCntrItem* pServer);

	HRESULT ShowInstance (CGenEdDoc* pDoc, const CString &strInstance);
	HRESULT ShowLocation (CGenEdDoc* pDoc, const CString &strLocation);

	void UpdateTabs ();
	void UpdateDocumentTabs (CGenEdDoc* pDoc);

	void CloseAll ();
	bool SaveAll (bool bAskBeforeSave=true);
	void ClearModified ();
    bool HaveModifiedDocuments ();

    bool HaveOpenDocuments();

//	void SetSettings (CGCSettings* pGCSettings);
	void UpdateSettings ();

	void SetPrintInfo (const CString &strPrinterName, long lWidth, long lHeight);

    bool OnIdle ();

    void RegisterReloadedCB (FC_CALLBACK_BASE (DocumentsReloadedCB)* pReloadedCB);
    void UnregisterReloadedCB (FC_CALLBACK_BASE (DocumentsReloadedCB)* pReloadedCB);

    BOOL IsEditFile(const CString& rstrFileName);

    static CGenEdCont* GetMe ();

    HRESULT GetDebugInfo(IDebugInfo** ppDebugInfo);

    void SaveSettings(CContainerSettings& rtContSettings);
    void LoadSettings(CContainerSettings& rtContSettings);

    CGenEdDoc* GetServerForFile(const CString& crstrFilePath);
    CGenEdDoc* GetServerForPrintOptions(const CString &strExtension);

    void ReloadProjectFile();

	// SIS 22.07.04 >>
	// maximum count of open documents
	void IncreaseOpenDocumentCount();
	void DecreaseOpenDocumentCount();
	int GetOpenDocumentCount();
	// SIS 22.07.04 <<
	// SIS 08.11.04 >>
	BOOL TryCloseUnusedDocument();
	BOOL CloseUnusedDocument(BOOL bForce);
	// SIS 08.11.04 <<

protected:
	CComPtr<IBreakPoint> m_pIBreakPoint;

	CGenEdDocTemplate* m_pSimpleTemplate;
	CGenEdDocTemplate* m_pTabbedTemplate;

	FC_CALLBACK (GenEdFrameActionCB, CGenEdCont)   m_FrameActionCB;
    FC_CALLBACK (TargetOnlConnectActionCB, CGenEdCont) m_ConnectActionCB;
    FC_CALLBACK (CEStateChangeCB, CGenEdCont)      m_StateChangeCB;

	CGenEdFrame*   m_pActiveFrame;

	CGCSettings*   m_pGCSettings;

    // map between server CLSID and print documents:
    TExtDocMap     m_tPrintDocsMap;

	CString         m_strPrinterName;
	long            m_lPrintWidth;
	long            m_lPrintHeight;

    CList<CGenEdDoc* , CGenEdDoc* >         m_ModifyCheckDocuments;

    FC_CALLBACK_HOLDER (DocumentsReloadedCB) m_ReloadedCBs;
    bool                                     m_bReloadedAny;

    UINT                                    m_uiResubscribeCookie;
    bool                                    m_bResubscribeAnotherTime;

    bool            m_bKadLoaded;

    static CGenEdCont*                      m_pMe;

    CKADFileInfo     m_tKadFileInfo;

    CComPtr<IDebugInfo> m_pIDebugInfo;

    POSITION        m_PrintDocPos;

	// SIS 22.07.04 >>
	// maximum number of open documents
	int				m_iOpenDocumentCount;
	int				m_iMaxOpenDocumentCount;
	// SIS 22.07.04 <<
	// SIS 08.11.04 <<
	int				m_iMaxRecoverDocumentCount;
	// SIS 08.11.04 <<

private:
	CGenEdFrame* GetActiveFrame ();
	bool GetCreateParameters(const CString &strFileName, bool bNeverTabbed, CGenEdDocTemplate *&pDocTemplate, GUID* pGUID, bool bForceAsText = false);
	CGenEdDoc* CreateDocument(CGenEdDocTemplate* pDocTemplate, const CString &strFileName, bool bVisible, GUID& rtGUID, E_ActivateAction eAction = eActivateClass, bool bPrintOpDoc = false);
	CGenEdFrame* FindFrame (CGenEdDoc* pGenEdDoc);
	bool CreateServer(CGenEdDoc* pDoc, CGenEdCntrItem* pServerItem);
	bool LoadServer(CGenEdDoc* pDoc);
	bool ShowServer(CGenEdView* pServerView, CGenEdCntrItem* pServerItem);
	void OnFrameAction (GenEdFrameAction action, CGenEdFrame* pFrame);
    void OnConnectAction (CTargetOnlBase* pTarget, TargetConnectAction action);
//	void OpenSettingsDocuments ();
    void OnStateChange (CEStateNotify stateNotify, unsigned int uiStage);
    bool ModifyCheck ();
    void GetAllDocuments (CList<CGenEdDoc* , CGenEdDoc* > &documents);
    void SetBreakpointManager (const CComPtr<IBreakPoint> &pBreakpointManager);
	void OnAddTarget(CTargetOnlBase* pTarget);
	void OnRemoveTarget(CTargetOnlBase* pTarget);
	void OnConnectTarget(CTargetOnlBase* pTarget);
	void OnDisconnectTarget(CTargetOnlBase* pTarget);
    void OnDebugConnectTarget(CTargetOnl* pTarget);
    void OnDebugDisconnectTarget(CTargetOnl* pTarget);

    bool LoadFileInfoFromKAD();

    void SaveFrameSettings(CGenEdFrame* pFrame, CContainerSettings& rtContSettings, int& riZOrder, BOOL bMaximized = FALSE);

    CString GetClassIdStringForFileExtension(const CString& crstrFileExt);

    void Parse();


//wizard:
	DECLARE_DYNCREATE(CGenEdCont)

	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CGenEdCont)
	public:
	//}}AFX_VIRTUAL

	DECLARE_MESSAGE_MAP()
	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CGenEdCont)
		// HINWEIS - Der Klassen-Assistent fügt hier Member-Funktionen ein und entfernt diese.
	//}}AFX_MSG

	DECLARE_DISPATCH_MAP()
	// Generierte OLE-Dispatch-Zuordnungsfunktionen
	//{{AFX_DISPATCH(CGenEdCont)
    public:
	afx_msg SCODE ShowURL(LPCTSTR szURL);
	afx_msg SCODE ShowFile(LPCTSTR szFile);
	afx_msg SCODE SelectFBForCall();
    afx_msg SCODE ShowVarSelection(LPCTSTR szPouName);
    afx_msg SCODE SetVarSelectionHint(LPCTSTR szPouName,
                                      LPCTSTR szVarNamePrefix,
                                      LPCTSTR szDataType);
    afx_msg SCODE ShowHelp(LPCTSTR szPOU);
    afx_msg SCODE ShowHeader(LPCTSTR szPOU);
    afx_msg SCODE ShowHeaderFile(LPCTSTR szFileName);

    afx_msg SCODE GetLRUPous(VARIANT FAR* parrPous);
    afx_msg SCODE GetPouXML(LPCTSTR szPouName, BSTR FAR* pszPouDef);
    afx_msg SCODE GetVarList(VARIANT FAR* parrVars);
    afx_msg SCODE GetPouList(VARIANT FAR* parrPous);
    afx_msg SCODE GetPouDefinition(LPCTSTR szPouName, BSTR FAR* pszPouDef);
    afx_msg SCODE GetPouMember(LPCTSTR szPouName, VARIANT FAR* parrMembers);
    afx_msg SCODE XRefChangeRefPosition(BOOL bForward);
	afx_msg SCODE XRefSetActiveQuery(LPCTSTR sVariableName);
	afx_msg SCODE ShowVarSelectionBox(LONG lXPos, LONG lYPos,
                                      LPCTSTR szLine, LONG lColumn,
                                      LPCTSTR szTypeHint,
                                      LPCTSTR szPouName,
                                      BSTR FAR* pszSelection,
                                      LONG* plStartColumn,
                                      LONG* plEndColumn);
    afx_msg SCODE GetProjectManager(IUnknown** pProjManUnk);
	//}}AFX_DISPATCH

	DECLARE_INTERFACE_MAP()
};


CGenEdCont* CEGetContainer ();

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // __GENEDCONT_H_


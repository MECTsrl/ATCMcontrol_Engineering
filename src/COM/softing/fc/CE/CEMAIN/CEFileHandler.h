#ifndef __CEFILEHANDLER_H_
#define __CEFILEHANDLER_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

enum E_CE_FileStringType
{
    eCEFileName,    // file name
    eCERelPath,     // relative path
    eCEAbsPath      // absolute path
};


#include "fccallback.h"
#include "GenEdDef.h"
//#include "BrkPtMan.h"
#include "GenEdCont.h"
#include "GenEdDoc.h"
#include "CeSysDef.h"

class CCEFileHandler : public CCmdTarget
{
public:
    CCEFileHandler ();
    virtual ~CCEFileHandler ();

    HRESULT Init (CGenEdCont *pContainer);
    void Cleanup ();
	void EnableParsing(bool bEnable);
	bool IsParseEnabled();
    void Parse(CGenEdDoc *pDoc);

    HRESULT ShowFile (const CString &strPath, CGenEdDoc *&pDoc, bool bActivateInst = false, bool bForceAsText = false);
    HRESULT ShowFileAt (const CString &strPath, const CString& crstrLocation, CGenEdDoc *&pDoc, bool bActivateInst = false);
    HRESULT LoadFile (const CString &strPath);
    HRESULT SaveFile (const CString &strPath);
    HRESULT CloseFile (const CString &strPath);
    HRESULT IsFileOpen (const CString &strPath, CGenEdDoc *&pDoc);

    BOOL ShowInstance(const CString& crstrFileString, const CString& crstrInst);

    // some helper functions for file names and path names
    // get file name without path:
    CString GetFileName(const CString& crstrFileString);
    // get file name with full path:
    CString GetFilePathName(const CString& crstrFileString);
    // get file name with full path:
    CString GetFilePathName(const CString& crstrFileString, const CString& crstrPath);
    // get file name with relative path to project or return path if not in project
    CString GetFilePathOpt(const CString& crstrPath);
    // get file name with relative path to other path
    CString GetFilePathRelative(const CString& crstrFilePathName, const CString& crstrPath);
    // analyse file string: file name | file name with rel path | file name with abs path
    E_CE_FileStringType AnalyseFileString(const CString& crstrFileString);
    // is within project path
    BOOL IsWithinProjectPath(const CString& crstrFilePathName);

    // file status: normal, readonly, missing
    EFileStatus GetFileStatus(const CString& crstrFileString);
    EFileStatus GetLibraryHeaderStatus(const CString& crstrFileString);

    // is library header existing
    BOOL IsLibraryHeaderExisting(const CString& crstrFileString);


protected:
    struct StoppointData
    {
        BPM_STPPT_STATE          m_State;
        CString                  m_strInstance;
        CComPtr<IBreakPointInfo> m_pBPInfo;
        bool                     m_bUpdate;
    };

    bool m_bInitialized;
    bool m_bProjectLoaded;

    CString m_strProjPath;
    CString m_strLibPath;

//    FC_CALLBACK (GenEdActionCB, CCEFileHandler)       m_ActionCB;
//    FC_CALLBACK (DocumentsReloadedCB, CCEFileHandler) m_ReloadedCB;
    FC_CALLBACK (CEStateChangeCB, CCEFileHandler)     m_StateChangeCB;

    CGenEdCont *m_pContainer;

    CComPtr<IBreakPoint> m_pIBreakPoint;
    DWORD                m_dwBPNotifyCookie;

    bool m_bParsingEnabled;

    unsigned int m_uiStoppointDelayCookie;
    CList<StoppointData *, StoppointData *> m_StoppointDatas;

    HRESULT OnBPStateChanged (BPM_BRKPT_STATE newState, const CString &strInstance, CComPtr<IBreakPointInfo>& pBPInfo, bool bUpdate);
    HRESULT OnSPStateChanged (BPM_STPPT_STATE newState, const CString &strInstance, CComPtr<IBreakPointInfo>& pBPInfo, bool bUpdate);
    HRESULT OnTaskStopped (const CString &strInstance, CComPtr<IBreakPointInfo>& pBPInfo, bool bUpdate);
    HRESULT OnTaskContinued (const CString &strInstance, CComPtr<IBreakPointInfo>& pBPInfo, bool bUpdate);
    HRESULT OnShowBP (const CString &strInstance, CComPtr<IBreakPointInfo>& pBPInfo);


private:
    HRESULT ShowDocument (const CString &strPath, CGenEdDoc *&pDoc, E_ActivateAction eAction = eActivateClass, bool bForceText = false);
    HRESULT LoadDocument (const CString &strPath);
    HRESULT SaveDocument (const CString &strPath);
    HRESULT CloseDocument (const CString &strPath);
//    void OnGenEdAction (GenEdAction action, CGenEdDoc *pDoc);
//    void OnDocumentsReloaded ();
    CGenEdDoc *FindFileDocument (const CString &strPath);
    HRESULT CheckRelative (const CString &strPath);
    CGenEdDoc *FindInstanceDocument (const CString &strPath, const CString &strInstance);
    HRESULT ReadBreakpointInfo(
        CComPtr<IBreakPointInfo>& pInfo, 
        CString&                  strPath, 
        EDP_POS_QUALIFIER&        posQual,
        long&                     lX, 
        long&                     lY, 
        long&                     lCX, 
        long&                     lCY
    );
    void SetBreakpointManager (const CComPtr<IBreakPoint>& pBreakpointManager);
    void OnStateChange (CEStateNotify stateNotify, unsigned int uiStage);
    void HandleStoppoint (StoppointData *pData);
    BOOL GetProject(CComQIPtr<ICEProjectManager>& pProjMan);
    BOOL GetProjectInfo(CComQIPtr<ICEProjInfo>& pProjInfo);

    BOOL Is4CEEditFile(const CString &strPath);

    CString GetProjectPath();

    BOOL IsLibraryHeader(const CString& crstrFileString);
    HRESULT ShowLibraryHeader(const CString& crstrFileString, CGenEdDoc*& pDoc);

//wizard:
    DECLARE_DYNCREATE(CCEFileHandler)

    // Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
    //{{AFX_VIRTUAL(CCEFileHandler)
    public:
    //}}AFX_VIRTUAL

    DECLARE_MESSAGE_MAP()
    // Generierte Nachrichtenzuordnungsfunktionen
    //{{AFX_MSG(CCEFileHandler)
        // HINWEIS - Der Klassen-Assistent fügt hier Member-Funktionen ein und entfernt diese.
    //}}AFX_MSG

//    DECLARE_DISPATCH_MAP()
    // Generierte OLE-Dispatch-Zuordnungsfunktionen
    //{{AFX_DISPATCH(CCEFileHandler)
    //}}AFX_DISPATCH

    DECLARE_INTERFACE_MAP()
    BEGIN_INTERFACE_PART (_IBreakPointNotifyObj, _IBreakPointNotify)
        STDMETHOD (OnBPStateChanged)(/*[in]*/ BPM_BRKPT_STATE newState, /*[in, string]*/ BSTR sInstance, /*[in]*/ IUnknown *pUnk, BOOL bUpdate);
        STDMETHOD (OnSPStateChanged)(/*[in]*/ BPM_STPPT_STATE newState, /*[in, string]*/ BSTR sInstance, /*[in]*/ IUnknown *pUnk, BOOL bUpdate);
        STDMETHOD (OnShowBP)(/*[in]*/ BSTR sInstance, /*[in]*/ IUnknown *pUnk);
    END_INTERFACE_PART(_IBreakPointNotifyObj)
};

#endif // __CEFILEHANDLER_H_

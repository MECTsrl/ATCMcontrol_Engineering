

#ifndef PRGEDITORDOC_H
#define PRGEDITORDOC_H

class CWatchpoint;
interface IDebugInfo;
interface IInstanceDBI;
interface IStepDBI;
interface IActionDBI;
interface ITransitionDBI;
interface ITypeDBI;

#include "StdEditorDoc.h"
#include "fccallback.h"
#include "Breakpoint.h"
#include "CESession.h"
#include "Symbols.h"
#include "CEWatchBe.h"

#include "SECEditor.h"

//-------------------------------------

class CPrgWPClientData :public CObject   // program editor default client data
{
public:
    CPrgWPClientData(const CString &strVariableName, const CStdEditorPosition &startPosition, const CStdEditorPosition &endPosition);

    CString GetVariableName () const;

    CStdEditorPosition GetStartPosition () const;
    CStdEditorPosition GetEndPosition () const;

    CString GetText () const;
    void SetText (const CString &strText);

    CRect GetArea () const;
    void SetArea (const CRect &rect);

    CRect GetLargestArea () const;

    bool GetActiveVisibility () const;
    void SetActiveVisibility (bool bVisible);

    bool GetInactiveVisibility () const;
    void SetInactiveVisibility (bool bVisible);

    void GetDisplay (long &lDisplayX, long &lDisplayY, long &lDisplayOffsetX, long &lDisplayOffsetY) const;
    void SetDisplay (long lDisplayX, long lDisplayY, long lDisplayOffsetX, long lDisplayOffsetY);

    unsigned int GetZ ();
    void SetZ (unsigned int uiZ);

    COLORREF GetBackgroundColor ();
    void SetBackgroundColor (COLORREF backgroundColor);

    bool GetCrossedOut ();
    void SetCrossedOut (bool bOn);

protected:
    CString            m_strVariableName;
    CStdEditorPosition m_StartPosition;
    CStdEditorPosition m_EndPosition;
    CString            m_strText;
    long               m_lDisplayX;
    long               m_lDisplayY;
    long               m_lDisplayOffsetX;
    long               m_lDisplayOffsetY;
    CRect              m_Area;
    long               m_lLargestWidth;
    long               m_lLargestHeight;
    bool               m_bActiveVisibility;
    bool               m_bInactiveVisibility;
    unsigned int       m_uiZ;
    COLORREF           m_BackgroundColor;
    bool               m_bCrossedOut;
};

//-------------------------------------

class CPrgEdStep : public CObject
{
public:
    CPrgEdStep (const CString &strName, long lNumber, long lX, long lY);

    CString GetName ();
    long GetNumber ();
    long GetX ();
    long GetY ();

    void *GetClientData ();
    void SetClientData (void *pClientData);

    bool IsActive ();
    void SetActive (bool bOn);

protected:
    CString m_strName;
    long    m_lNumber;
    long    m_lX;
    long    m_lY;

    void *m_pClientData;

    bool m_bActive;
};

//-------

class CSTStepClientData : public CObject
{
public:
    CSTStepClientData (const CStdEditorPosition &keywordPosition, const CStdEditorPosition &behindIdPosition);

    CStdEditorPosition GetKeywordPosition ();
    CStdEditorPosition GetBehindIdPosition ();

protected:
    CStdEditorPosition m_KeywordPosition;
    CStdEditorPosition m_BehindIdPosition;
};

//-------------------------------------

class CPrgEdAction : public CObject
{
public:
    CPrgEdAction (const CString &strName, long lNumber, long lX, long lY);

    CString GetName ();
    long GetNumber ();
    long GetX ();
    long GetY ();

    void *GetClientData ();
    void SetClientData (void *pClientData);

    bool IsActive ();
    void SetActive (bool bOn);

    bool IsForced ();

    bool IsForcedPermanently ();
    void SetForcedPermanently (bool bOn);

    bool IsForcedOnce ();
    void SetForcedOnce (bool bOn);

    bool IsBlocked ();
    void SetBlocked (bool bOn);

protected:
    CString m_strName;
    long    m_lNumber;
    long    m_lX;
    long    m_lY;

    void *m_pClientData;

    bool m_bActive;
    bool m_bForcedOnce;
    bool m_bForcedPermanently;
    bool m_bBlocked;
};

//-------

class CSTActionClientData : public CObject
{
public:
    CSTActionClientData (const CStdEditorPosition &keywordPosition, const CStdEditorPosition &behindIdPosition);

    CStdEditorPosition GetKeywordPosition ();
    CStdEditorPosition GetBehindIdPosition ();

protected:
    CStdEditorPosition m_KeywordPosition;
    CStdEditorPosition m_BehindIdPosition;
};

//-------------------------------------

class CPrgEdTransition : public CObject
{
public:
    CPrgEdTransition (const CString &strName, long lNumber, long lX, long lY);
    
    CString GetName ();
    long GetNumber ();
    long GetX ();
    long GetY ();
    
    void *GetClientData ();
    void SetClientData (void *pClientData);
    
    bool IsEnabled ();
    void SetEnabled (bool bOn);
    
    bool IsForced ();
    
    bool IsForcedPermanently ();
    void SetForcedPermanently (bool bOn);
    
    bool IsForcedOnce ();
    void SetForcedOnce (bool bOn);
    
    bool IsBlocked ();
    void SetBlocked (bool bOn);
    
protected:
    CString m_strName;
    long    m_lNumber;
    long    m_lX;
    long    m_lY;
    
    void *m_pClientData;
    
    bool m_bEnabled;
    bool m_bForcedOnce;
    bool m_bForcedPermanently;
    bool m_bBlocked;
};

//-------

class CSTTransitionClientData : public CObject
{
public:
    CSTTransitionClientData (const CStdEditorPosition &keywordPosition, const CStdEditorPosition &behindIdPosition);

    CStdEditorPosition GetKeywordPosition ();
    CStdEditorPosition GetBehindIdPosition ();

protected:
    CStdEditorPosition m_KeywordPosition;
    CStdEditorPosition m_BehindIdPosition;
};

//-------------------------------------

class TTNfyData:public CObject
{
public:
   UINT     id;
   NMHDR    *pTTTStruct;
   LRESULT  *pResult;
   CPoint   Pmouse;
   bool     bRes;
};

//-------------------------------------

class CPrgEdSessions
{
public:
    CPrgEdSessions ();
    
    void Reference ();
    void Unreference ();
    
    CCESessionManager *GetSessionManager ();
    
protected:
    unsigned int      m_uiReferences;
    CCESessionManager m_SessionManager;
    
    ~CPrgEdSessions ();
};


class CPrgEditorDoc : public CStdEditorDoc
{
    
    // View update hints
    ////////////////////
    
public:
    static const int PreConnectHint;
    static const int PostConnectHint;
    static const int PreDisconnectHint;
    static const int PostDisconnectHint;
    
    static const int ShowErrorHint;
    
    static const int WatchpointDataHint;
    
    static const int ShowBreakpointHint;
    static const int HideBreakpointHint;
    
    static const int ShowWatchpointHint;
    static const int HideWatchpointHint;
    
    static const int ShowStoppointHint;
    static const int HideStoppointHint;
    
    static const int InsertFBCallHint;
    
    static const int ExpressionChangedHint;
    
    static const int BeginSFCUpdateHint;
    static const int EndSFCUpdateHint;
    
    static const int StepAttributeChangedHint;
    static const int ActionAttributeChangedHint;
    static const int TransitionAttributeChangedHint;
    
    static const int AllActionsOffToggledHint;
    
    static const int AllTransitionsOnToggledHint;
    
    static const int ManualModeToggledHint;
    static const int PerformingCycleToggledHint;

    static const int InsertVariableHint;
    static const int IdleHint;
    static const int ToolTipNfyHint;
    static const int ViewActivatedHint;
    static const int ViewDeactivatedHint;
    
    enum StepAttribute { StepActive };
    enum ActionAttribute { ActionActive, ActionForcedOnce, ActionForcedPermanently, ActionBlocked, ActionForced=ActionForcedPermanently };
    enum TransitionAttribute { TransitionEnabled, TransitionForcedOnce, TransitionForcedPermanently, TransitionBlocked, TransitionForced=TransitionForcedPermanently };
    
    
    // Construction/destruction/initialization
    //////////////////////////////////////////
    
protected: // create from serialization only
    CPrgEditorDoc ();
    
private:
    // helper to override the watchpoint display position (after restoring)
    bool m_bSpecialWPDisplay;
    long m_lWPDisplayX;
    long m_lWPDisplayY;
    long m_lWPDisplayOffsetX;
    long m_lWPDisplayOffsetY;
    long m_lWPContext;

public:
    virtual ~CPrgEditorDoc ();
    
    // abstract methods
    virtual class CSECEditor* GetEditor(UINT id)=0; // return the document's member (if any)
    virtual bool  CheckInPlaceActive()=0;           // COleServerDoc::IsInPlaceActive(..) is NOT virtual
    virtual COleServerItem* OnGetEmbeddedItem()=0;  // cover non-virtual method
    
    // Convenience methods used by subclasses and views
    ///////////////////////////////////////////////////
    
public:
    bool CanMonitor ();
    bool CanDebug ();
    
    bool IsConnected ();
    bool IsDebugConnected ();
    
//    bool m_bHadReadOnlyWarning;
//    bool HadReadOnlyWarning();
//    void HadReadOnlyWarning(bool b);

    
    POSITION GetFirstWPPosition ();
    CWatchpoint *GetNextWP (POSITION &pPosition);
    CWatchpoint *LookupWatchpoint (long lHandle);
    HRESULT SetWatchpoint (const CStdEditorPosition &position, 
                           bool bFailIfBad=true);
    HRESULT SetExpressionWatchpoint (const CStdEditorPosition &startPosition, 
                                     const CStdEditorPosition &endPosition, 
                                     bool bFailIfBad=true);
    HRESULT ClearWatchpoint (CWatchpoint *pWP);
    void ClearAllWatchpoints ();
    HRESULT SuspendWatchpoint (CWatchpoint *pWP);
    HRESULT ResumeWatchpoint (CWatchpoint *pWP);
    bool RaiseWatchpoint (CWatchpoint *pWP);
    void ZOrderWatchpoints (CArray<CWatchpoint *, CWatchpoint *> &zOrdered);
    
    POSITION GetFirstBPPosition ();
    CBreakpoint* GetNextBP (POSITION &pPosition);
    CBreakpoint* LookupBreakpoint (const CStdEditorPosition& position);
    HRESULT SetBreakpoint (const CStdEditorPosition& position);
    HRESULT ClearBreakpoint (const CStdEditorPosition& position);
    HRESULT ClearInstanceBreakpoints ();
    HRESULT ClearAllBreakpoints ();
    HRESULT ActivateBreakpoint (const CStdEditorPosition& position);
    HRESULT DeactivateBreakpoint (const CStdEditorPosition& position);
    void HideAllBreakpoints ();
    
    bool GetStopPosition (CStdEditorPosition& position);
    
    HRESULT ContinueTask ();
    //mode: 0=continue task, 1=single step, 2=step over, 3=step out 
    HRESULT StepTask (long mode);
    
    virtual BOOL PrepareEdit();

    virtual void RecreateToolTip();

    // CGenEdContDriver



	
    //    CGenEdContDriver
    //    cemaindriver
    HRESULT ShowURL(const CString& strURL);
    HRESULT ShowFile (const CString &strFile);
    HRESULT ShowUserHelp (const CString &strText);  // == show help
    HRESULT ShowHeader(const CString &strPOU);
    HRESULT ShowHeaderFile(const CString &strFileName);

    HRESULT SelectFBForCall ();
    HRESULT ShowVarSelection(const CString& strPouName);
    HRESULT SetVarSelectionHint(const CString& strPouName, 
                                const CString& strVarNamePrefix,
                                const CString& strDataType);

    HRESULT GetLRUPous(CList<CString, CString&>& listPous);
    HRESULT GetPouXML(const CString& strPouName, CString& strPouDef);
    HRESULT GetVarList(CList<CString, CString&>& listVars);
    HRESULT GetPouList(CList<CString, CString&>& listPous);
    HRESULT GetPouDefinition(const CString& strPouName, CString& strPouDef);
    HRESULT GetPouMember(const CString& strPouName, CList<CString, CString&>& listMembers);
    HRESULT XRefChangeRefPosition(BOOL bForward);
	HRESULT XRefSetActiveQuery(const CString& strVariableName);
    HRESULT ShowVarSelectionBox(LONG lXPos, LONG lYPos, 
                                const CString& strLine, LONG lColumn,
                                const CString& strTypeHint,
                                const CString& strPouName,
                                CString& strSelection,
                                LONG& lStartColumn,
                                LONG& lEndColumn);
    HRESULT GetProjectManager(IUnknown** pProjManUnk);
    // end CGenEdContDriver
    
    CString GetConfiguration ();
    CString GetWithoutConfiguration ();
    CString GetResource ();
    CString StripInstanceQualifier (CString &strPath);
    
    void CacheWatchExpression (const CString &strExpression, COleDataSource *pDataSource);
    
    void EnableSFC (bool bOn);
    void DisableSFC(); // called

    void SetLastActiveView(CView* pView);
    CView* GetLastActiveView();

    
    // Watch backend handling methods
    /////////////////////////////////
    
    virtual HRESULT AddWatchItem (const CString &strItem, LONG &lHandle);
    virtual HRESULT RemoveWatchItem (LONG lHandle);
    virtual HRESULT ConnectWatchItem (LONG lHandle);
    virtual HRESULT DisconnectWatchItem (LONG lHandle);
    virtual HRESULT WriteWatchItem (LONG lHandle, const CComVariant &varValue);
    bool IsUnknownWatchItemType (LONG lHandle);
    bool IsSimpleWatchItemType (LONG lHandle, BL_IEC_TYP &iecType);
    CEExpressionState GetWatchExpressionState (LONG lHandle);
    CString GetWatchExpressionValue (LONG lHandle);
    CString GetWatchExpressionValue (LONG lHandle, short &sQuality, DATE &timestamp);
    
    
    // Ladder tracing methods
    /////////////////////////
    
public:
    bool IsLadderActive (long lX, long lY);


    // SFC tracing methods
    //////////////////////
    
public:
    CString GetSFCVisValue ();
    
    CString GetOnceForcedActionsValue ();
    CString GetForcedActionsValue ();
    CString GetBlockedActionsValue ();
    
    CString GetOnceForcedTransitionsValue ();
    CString GetForcedTransitionsValue ();
    CString GetBlockedTransitionsValue ();
    
    HRESULT SetManualMode (bool bOn);
    bool InManualMode (bool &bIn);
    
    HRESULT PerformSFCCycle ();
    bool InSFCCycle (bool &bIn);
    
    bool IsStepPosition (long lX, long lY);
    bool IsStepActive (long lX, long lY);
    
    bool IsActionPosition (long lX, long lY);
    bool IsActionActive (long lX, long lY);
    bool IsActionForced (long lX, long lY);
    bool IsActionForcedOnce (long lX, long lY);
    bool IsActionForcedPermanently (long lX, long lY);
    bool IsActionBlocked (long lX, long lY);
    bool AreAllActionsBlocked ();
    bool AreAllActionsBlockedGlobally ();
    
    HRESULT ForceActionOnce (bool bOn, long lX, long lY);
    HRESULT ForceAction (bool bOn, long lX, long lY);
    HRESULT BlockAction (bool bOn, long lX, long lY);
    HRESULT ForceAllActionsOnce (bool bOn);
    HRESULT ForceAllActions (bool bOn);
    HRESULT BlockAllActions (bool bOn);
    HRESULT GloballyBlockAllActions (bool bOn);
    
    bool IsTransitionPosition (long lX, long lY);
    bool IsTransitionEnabled (long lX, long lY);
    bool IsTransitionForced (long lX, long lY);
    bool IsTransitionForcedOnce (long lX, long lY);
    bool IsTransitionForcedPermanently (long lX, long lY);
    bool IsTransitionBlocked (long lX, long lY);
    bool AreAllTransitionsForced ();
    bool AreAllTransitionsForcedGlobally ();
    
    HRESULT ForceTransitionOnce (bool bOn, long lX, long lY);
    HRESULT ForceTransition (bool bOn, long lX, long lY);
    HRESULT BlockTransition (bool bOn, long lX, long lY);
    HRESULT ForceAllTransitionsOnce (bool bOn);
    HRESULT ForceAllTransitions (bool bOn);
    HRESULT BlockAllTransitions (bool bOn);
    HRESULT GloballyForceAllTransitions (bool bOn);
    
    bool IsSFCDebugging ();
    
    void GetSteps       (CList<CPrgEdStep *, CPrgEdStep *> &steps, StepAttribute attribute);
    void GetStepsAtLine (CList<CPrgEdStep *, CPrgEdStep *> &steps, int iLine);
    void GetAllSteps    (CList<CPrgEdStep *, CPrgEdStep *> &steps);

    void GetActions      (CList<CPrgEdAction *, CPrgEdAction *> &actions, ActionAttribute attribute);
    void GetActionsAtLine(CList<CPrgEdAction *, CPrgEdAction *> &actions, int iLine);
    void GetAllActions   (CList<CPrgEdAction *, CPrgEdAction *> &actions);

    void GetTransitions      (CList<CPrgEdTransition *, CPrgEdTransition *> &transitions, TransitionAttribute attribute);
    void GetTransitionsAtLine(CList<CPrgEdTransition *, CPrgEdTransition *> &transitions, int iLine);
    void GetAllTransitions   (CList<CPrgEdTransition *, CPrgEdTransition *> &transitions);
    
    void OnViewObjHelpPosition  (const CStdEditorPosition &position);
    void OnViewObjHelpSelection (const CStdEditorPosition &startPosition,
                                 const CStdEditorPosition &endPosition);

    CString GetContextStringPosition(const CStdEditorPosition &position);
    CString GetContextStringSelection(const CStdEditorPosition &startPosition,
                                  const CStdEditorPosition &endPosition);


protected:
    virtual void OnCreateStep (CPrgEdStep *pStep);
    virtual void OnDestroyStep (CPrgEdStep *pStep);
    virtual void OnCreateAction (CPrgEdAction *pAction);
    virtual void OnDestroyAction (CPrgEdAction *pAction);
    virtual void OnCreateTransition (CPrgEdTransition *pTransition);
    virtual void OnDestroyTransition (CPrgEdTransition *pTransition);
    
    virtual HRESULT StartSFCTracing ();
    virtual HRESULT StopSFCTracing ();
    virtual void BeginSFCUpdate ();
    virtual void EndSFCUpdate ();
    
    virtual HRESULT StartLDTracing (void);
    virtual HRESULT StopLDTracing (void);

private:
    void ReadSFCDBI ();
    bool CreateStep (const CComPtr<IStepDBI> &pStepDBI);
    void DestroyStep (CPrgEdStep *pStep);
    void DestroyAllSteps ();
    bool CreateAction (const CComPtr<IActionDBI> &pActionDBI);
    void DestroyAction (CPrgEdAction *pAction);
    void DestroyAllActions ();
    bool CreateTransition (const CComPtr<ITransitionDBI> &pTransitionDBI);
    void DestroyTransition (CPrgEdTransition *pTransition);
    void DestroyAllTransitions ();
    
    HRESULT StartVisMonitoring ();
    HRESULT StopVisMonitoring ();
    HRESULT StartManualModeMonitoring ();
    HRESULT StopManualModeMonitoring ();
    HRESULT StartDoNextMonitoring ();
    HRESULT StopDoNextMonitoring ();
    HRESULT StartActionForcingMonitoring ();
    HRESULT StopActionForcingMonitoring ();
    HRESULT StartTransitionForcingMonitoring ();
    HRESULT StopTransitionForcingMonitoring ();
    HRESULT StartVariableMonitoring (const CString &strName, long &lHandle);
    HRESULT StopVariableMonitoring (long lHandle);
    void SFCVisChanged ();
    void ManualModeChanged ();
    void DoNextChanged ();
    void OnceForcedActionsChanged ();
    void ForcedActionsChanged ();
    void BlockedActionsChanged ();
    void AllActionsOffChanged ();
    void OnceForcedTransitionsChanged ();
    void ForcedTransitionsChanged ();
    void BlockedTransitionsChanged ();
    void AllTransitionsOnChanged ();
    void UpdateSteps (StepAttribute attribute, const CArray<long, long> &stepNumbers);
    CPrgEdStep *LookupStepByPosition (long lX, long lY);
    CComPtr<IStepDBI> LookupStepDBIByPosition (long lX, long lY);
    void ResetAllSteps ();
    void UpdateActions (ActionAttribute attribute, const CArray<long, long> &actionNumbers);
    CPrgEdAction *LookupActionByPosition (long lX, long lY);
    CComPtr<IActionDBI> LookupActionDBIByPosition (long lX, long lY);
    void ResetAllActions ();
    void UpdateTransitions (TransitionAttribute attribute, const CArray<long, long> &transitionNumbers);
    CPrgEdTransition *LookupTransitionByPosition (long lX, long lY);
    CComPtr<ITransitionDBI> LookupTransitionDBIByPosition (long lX, long lY);
    void ResetAllTransitions ();
    HRESULT WriteTransitionCommand (bool bSet, long lNumber);
    
    
    // ICEPrgEdit functionality
    ///////////////////////////
    
protected:
    virtual void SetInstance (const CString &strInstanceName, const CString &strMachineName, const CString &strMachineAddress);
    virtual void SetSymbolFile (const CString &strSymbolFile);
    virtual HRESULT NotifyConnect (const CString &strMachineName);
    virtual HRESULT NotifyDisconnect (const CString &strMachineName);
    virtual void ShowError (CStdEditorPosition position);
    virtual HRESULT InsertFBCall (const CString &strFB);
    virtual HRESULT NotifyDebugState (CSC_SESSION_STATE state);
    virtual HRESULT Resubscribe ();
    virtual HRESULT InsertVariable(const CString &strVarName);
    
    
    // ICEMonitor functionality
    ///////////////////////////
    
protected:
    virtual HRESULT AddWatchpoint (const CStdEditorPosition &position, bool bFailIfBad, long &lHandle);
    virtual HRESULT RemoveWatchpoint (long lHandle);
    virtual HRESULT LoadWatchpoints (SAFEARRAY *pWPInfos);
    virtual HRESULT StoreWatchpoints (SAFEARRAY *&pWPInfos);
    virtual HRESULT EnableStepMonitoring (bool bOn);
    virtual HRESULT AddExpressionWatchpoint (const CStdEditorPosition &startPosition, const CStdEditorPosition &endPosition, bool bFailIfBad, long &lHandle);
    
    
    // ICEDebug functionality
    /////////////////////////
    
protected:
    virtual HRESULT ShowBreakpoint (const CStdEditorPosition& position, short sState);
    virtual HRESULT HideBreakpoint (const CStdEditorPosition& position);
    virtual HRESULT ShowStoppoint  (const CStdEditorPosition& position);
    virtual HRESULT HideStoppoint ();
    
    
    // Event handlers
    /////////////////
public:
    virtual CString GetSmartText   (const CStdEditorPosition &position, CStdEditorPosition &startPosition, CStdEditorPosition &endPosition);

protected:
    // override the WP display position (only once - for the next WP creation)
    void ForceWPDisplayPos(long X, long Y, long Xofs, long Yofs);

    virtual CString GetTrimmedText (const CStdEditorPosition &startPosition, const CStdEditorPosition &endPosition, CStdEditorPosition &trimmedStart, CStdEditorPosition &trimmedEnd);
    virtual HRESULT OnCreateWatchpoint (CWatchpoint *pWP);
    virtual void OnDestroyWatchpoint (CWatchpoint *pWP);

    virtual HRESULT OnConnectWatchpoint (CWatchpoint *pWP);
    virtual HRESULT OnConnectWatchpoint (CWatchpoint *pWP, bool bFailIfBad);
    virtual void OnDisconnectWatchpoint (CWatchpoint *pWP);
    virtual void OnShowWatchpoint (CWatchpoint *pWP);
    virtual void OnHideWatchpoint (CWatchpoint *pWP);
    virtual void OnNewWatchpointData (CWatchpoint *pWP);
    virtual CString OnStoreWatchpoint (CWatchpoint *pWP);
    virtual void OnRestoreWatchpoint (const CString &strWPInfo);
    HRESULT OnRestoreSmartWatchpoint (const CString &strWPInfo);
    HRESULT OnRestoreExpressionWatchpoint (const CString &strWPInfo);

    bool IsDuplicateWP (const CStdEditorPosition &startPosition, const CStdEditorPosition &endPosition);
    
    virtual void OnShowBreakpoint (CBreakpoint *pBP);
    virtual void OnHideBreakpoint (CBreakpoint *pBP);
    virtual void OnShowStoppoint ();
    virtual void OnHideStoppoint ();
    
    virtual void OnStepAttributeChanged (CPrgEdStep *pStep);
    virtual void OnActionAttributeChanged (CPrgEdAction *pAction);
    virtual void OnTransitionAttributeChanged (CPrgEdTransition *pTransition);
    
    virtual void OnAllActionsOffToggled ();
    virtual void OnAllTransitionsOnToggled ();
    virtual void OnManualModeToggled ();
    virtual void OnPerformingCycleToggled ();
    
    virtual void OnExpressionChanged (LONG lHandle);
    
    virtual void OnInstancePathModified ();
    
    
    // Superclass queries
    /////////////////////
    
protected:
    virtual bool QueryDelayClose ();
    
    
    // Protected attributes
    ///////////////////////
    
protected:
    CString m_strInstance;
    CString m_strMachineName;
    CString m_strMachineAddress;
    
    CString      m_strSymbolFile;
    CSymbolTable *m_pSymbolTable;
    CString      m_strLocalPrefix;
    CString      m_strGlobalPrefix;
    CView       *m_pLastActiveView;

    CList<CBreakpoint *, CBreakpoint *>   m_BreakpointList;
    CList<CWatchpoint *, CWatchpoint *>   m_WatchpointList;
    
    CList<CPrgEdStep *, CPrgEdStep *>             m_StepList;
    CList<CPrgEdAction *, CPrgEdAction *>         m_ActionList;
    CList<CPrgEdTransition *, CPrgEdTransition *> m_TransitionList;
    
    bool                m_bStopped;
    CStdEditorPosition  m_StopPosition;
    
    CComPtr<ICEWatch> m_pWatch;
    bool              m_bWatchInitialized;
    DWORD             m_dwWatchCookie;
    
    CComPtr<IDebugInfo>   m_pDebugInfo;
    CComPtr<IInstanceDBI> m_pInstanceDBI;
    CComPtr<ITypeDBI>     m_pTypeDBI;
    
    long m_lNextWPHandle;
    
    UINT m_uiWatchExpressionCBFormat;
    
    bool m_bSFCEnabled;
    bool m_bInternalSFCEnabled;
    
    long              m_lSFCVisHandle;
    CEExpressionState m_SFCVisState;
    CString           m_strSFCVisValue;
    
    long              m_lManualModeHandle;
    CEExpressionState m_ManualModeState;
    bool              m_bManualModeValue;
    
    long              m_lDoNextHandle;
    CEExpressionState m_DoNextState;
    bool              m_bDoNextValue;
    
    long              m_lOnceForcedActionsHandle;
    CEExpressionState m_OnceForcedActionsState;
    CString           m_strOnceForcedActionsValue;
    
    long              m_lForcedActionsHandle;
    CEExpressionState m_ForcedActionsState;
    CString           m_strForcedActionsValue;
    
    long              m_lBlockedActionsHandle;
    CEExpressionState m_BlockedActionsState;
    CString           m_strBlockedActionsValue;
    
    long              m_lEnabledTransitionsHandle;
    CEExpressionState m_EnabledTransitionsState;
    CString           m_strEnabledTransitionsValue;
    
    long              m_lOnceForcedTransitionsHandle;
    CEExpressionState m_OnceForcedTransitionsState;
    CString           m_strOnceForcedTransitionsValue;
    
    long              m_lForcedTransitionsHandle;
    CEExpressionState m_ForcedTransitionsState;
    CString           m_strForcedTransitionsValue;
    
    long              m_lBlockedTransitionsHandle;
    CEExpressionState m_BlockedTransitionsState;
    CString           m_strBlockedTransitionsValue;
    
    long              m_lAllTransitionsOnHandle;
    CEExpressionState m_AllTransitionsOnState;
    bool              m_bAllTransitionsOnValue;
    
    long              m_lAllActionsOffHandle;
    CEExpressionState m_AllActionsOffState;
    bool              m_bAllActionsOffValue;
    
    long              m_lManCmdHandle;
    CEExpressionState m_ManCmdState;

    bool m_bEnableWPChanges;

    
    // Private attributes
    /////////////////////
    
private:
    CCESession           *m_pSession;
    CSC_SESSION_STATE    m_DebugSessionState;
    bool                 m_bReferencedWatch;
    bool                 m_bDoNotCallStop;
    
    FC_CALLBACK (SessionModifiedCB, CPrgEditorDoc) m_SessionModifiedCB;
    
    bool m_bDelayClose;
    
    static CPrgEdSessions *m_pSessions;
    
    
    // Private helpers
    //////////////////
    
private:
    CCESessionManager *GetSessionManager ();
    
    HRESULT OpenVarSession ();
    void CloseVarSession ();
    HRESULT OpenDebugSession ();
    void CloseDebugSession ();

    HRESULT GetInstanceDBI ();

    void ReadSymbols ();

    void OnSessionModified (CCESession *pSession, bool bDebug);

    HRESULT FinishAddWatchpoint (CWatchpoint *pWP, bool bFailIfBad);

    bool GetWatchItemStateAndValue (long lHandle, int iType, CEExpressionState &state, CComVariant &varValue);

    void IncludeNumberArray (CArray<long, long> &numbers, const CArray<long, long> &moreNumbers);


	// Wizard generated
	///////////////////

	DECLARE_DYNAMIC (CPrgEditorDoc)

	//{{AFX_VIRTUAL(CPrgEditorDoc)
	public:
	virtual void DeleteContents();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CPrgEditorDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CPrgEditorDoc)
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

    BEGIN_INTERFACE_PART(CEPrgEditObj, ICEPrgEdit)
	    STDMETHOD(SetInstance)(/*[in, string]*/ BSTR sInstanceName, /*[in, string]*/ BSTR sMachineName, /*[in, string]*/ BSTR sMachineAddress);
		STDMETHOD(SetSymbolFile)(/*[in, string]*/ BSTR sSymbolFile);
	    STDMETHOD(NotifyConnect)(/*[in, string]*/ BSTR sMachineName);
	    STDMETHOD(NotifyDisconnect)(/*[in, string]*/ BSTR sMachineName);
		STDMETHOD(ShowError)(/*[in]*/ EDP_POS_QUALIFIER posQual, /*[in]*/ long lX, /*[in]*/ long lY, /*[in]*/ long lCX, /*[in]*/ long lCY);
		STDMETHOD(InsertFBCall)(/*[in, string]*/ BSTR sFB);
	    STDMETHOD(NotifyDebugState)(/*[in]*/ short state);
        STDMETHOD(Resubscribe)();
   		STDMETHOD(InsertVariable)(/*[in, string]*/ BSTR sVarName);
    END_INTERFACE_PART(CEPrgEditObj)

    BEGIN_INTERFACE_PART(CEDebugObj, ICEDebug)
		STDMETHOD(ShowBreakpoint)(/*[in]*/ EDP_POS_QUALIFIER posQual, /*[in]*/ long lX, /*[in]*/ long lY, /*[in]*/ long lCX, /*[in]*/ long lCY, /*[in]*/ short sState);
		STDMETHOD(HideBreakpoint)(/*[in]*/ EDP_POS_QUALIFIER posQual, /*[in]*/ long lX, /*[in]*/ long lY, /*[in]*/ long lCX, /*[in]*/ long lCY);
		STDMETHOD(ShowStoppoint) (/*[in]*/ EDP_POS_QUALIFIER posQual, /*[in]*/ long lX, /*[in]*/ long lY, /*[in]*/ long lCX, /*[in]*/ long lCY);
		STDMETHOD(HideStoppoint)();
    END_INTERFACE_PART(CEDebugObj)

    BEGIN_INTERFACE_PART(CEMonitorObj, ICEMonitor)
		STDMETHOD(AddWatchpoint)(/*[in]*/ long lX, /*[in]*/ long lY, /*[out]*/ long *plHandle);
		STDMETHOD(RemoveWatchpoint)(/*[in]*/ long lHandle);
		STDMETHOD(LoadWatchpoints)(/*[in]*/ SAFEARRAY/*(BSTR)*/ *pWPInfos);
		STDMETHOD(StoreWatchpoints)(/*[out]*/ SAFEARRAY/*(BSTR)*/ **ppWPInfos);
		STDMETHOD(EnableStepMonitoring)(BOOL bOn);
        STDMETHOD(AddExpressionWatchpoint)(/*[in]*/ long lStartX, /*[in]*/ long lStartY, /*[in]*/ long lEndX, /*[in]*/ long lEndY, /*[out]*/ long *plHandle);
    END_INTERFACE_PART(CEMonitorObj)

    BEGIN_INTERFACE_PART(CEWatchNotifyObj, ICEWatchNotify)
        STDMETHOD(OnExpressionChanged)(/*[in]*/ LONG lHandle);
    END_INTERFACE_PART(CEWatchNotifyObj)

	DECLARE_INTERFACE_MAP()
};

#endif


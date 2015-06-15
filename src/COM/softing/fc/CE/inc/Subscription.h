#ifndef Subscription_H
#define Subscription_H

class CSubscription;
class CSubscriber;

#include "CSC_Online.h"
#include "fccallback.h"
#include "baselib.h"


// Callback definition for clients of CSubscriptionManager
FC_CALLBACK_DEF1 (SubscriptionNotifyCB, CSubscription *);
FC_CALLBACK_DEF (ConnectionDroppedCB);

// Callback definition for clients of CSubscription
FC_CALLBACK_DEF1 (SubscriptionModifiedCB, CSubscription *);

// Callback definitions for internal use
FC_CALLBACK_DEF1 (SubscriptionReleaseCB, CSubscription *)


class CSubscription : public CObject
{
public:
    static CSubscription *Create (const CString &strObject, BL_IEC_TYP type, CSubscriber *pSubscriber);

    void RegisterModifiedCB (FC_CALLBACK_BASE (SubscriptionModifiedCB) *pModifiedCB);
    void UnregisterModifiedCB (FC_CALLBACK_BASE (SubscriptionModifiedCB) *pModifiedCB);

    void SetReleaseCB (FC_CALLBACK_BASE (SubscriptionReleaseCB) *pReleaseCB);

    CString GetObject () const;
    BL_IEC_TYP GetType () const;
    LONG GetServer () const;

    bool IsKnown () const;
    bool HaveData () const;

    CString GetValue () const;
    CComVariant GetVariantValue () const;
    short GetQuality () const;
    DATE GetTimestamp () const;
    void SetData (CComVariant varValue, short sQuality, DATE timestamp);

    HRESULT Reference (unsigned int uiSuspensions=0);
    HRESULT Unreference (unsigned int uiSuspensions=0);
    unsigned int GetReferences ();

    HRESULT Suspend ();
    HRESULT Resume ();
    unsigned int GetSuspensions ();

    HRESULT Read ();
    HRESULT Write (const CComVariant &varValue);

    HRESULT Resubscribe ();

protected:
    FC_CALLBACK_HOLDER (SubscriptionModifiedCB) m_ModifiedCBs;

    FC_CALLBACK_BASE (SubscriptionReleaseCB) *m_pReleaseCB;

    CString    m_strObject;
    BL_IEC_TYP m_Type;

    CSubscriber *m_pSubscriber;

    LONG        m_hServer;
    bool        m_bKnown;
    bool        m_bHaveData;
    CComVariant m_varValue;
    CString     m_strValue;
    short       m_sQuality;
    DATE        m_Timestamp;

    unsigned int m_uiReferences;
    unsigned int m_uiSuspensions;

    CSubscription (const CString &strObject, BL_IEC_TYP type, CSubscriber *pSubscriber);

    void *operator new(size_t size);
    void operator delete(void *memory);

private:
    HRESULT DoSubscribing ();
    HRESULT Subscribe ();
    HRESULT Unsubscribe ();
};


class CSubscriber : public CObject
{
public:
    static CSubscriber *Create (CComPtr<IFCVariableCollection> pIFCVarCollection);
    void Delete ();

    HRESULT Subscribe (const CString &strObject, BL_IEC_TYP type, long &hServer, bool &bKnown);
    HRESULT Unsubscribe (const CString &strObject, long hServer);

    HRESULT Read (const CString &strObject, long hServer, CComVariant &varValue, short &sQuality, DATE &timestamp);
    HRESULT Write (const CString &strObject, long hServer, const CComVariant &varValue);

protected:
    CComPtr<IFCVariableCollection> m_pIFCVarCollection;
    bool                           m_bEntered;
    bool                           m_bDelayedDelete;

    CSubscriber (CComPtr<IFCVariableCollection> pIFCVarCollection);
    ~CSubscriber ();

    void Enter ();
    void Leave ();
};


class CSubscriptionManager : public CObject
{
    friend class CVariableNotify;

public:
    CSubscriptionManager ();

    void SetNotifyCB (FC_CALLBACK_BASE (SubscriptionNotifyCB) *pNotifyCB);

    HRESULT Start ();
    HRESULT Stop ();
    bool IsInitialized ();

    HRESULT GetSubscription (const CString &strObject, BL_IEC_TYP type, CSubscription *&pSubscription);

    POSITION GetHeadPosition ();
    CSubscription *GetNext (POSITION &pPosition);

    void ResubscribeAll ();

protected:
    bool m_bInitialized;

    CSubscriber *m_pSubscriber;

    CComPtr<IFCVariableCollection> m_pIFCVarCollection;

    CList<CSubscription *, CSubscription *> m_SubscriptionList;

    FC_CALLBACK (SubscriptionReleaseCB, CSubscriptionManager) m_ReleaseCB;

    FC_CALLBACK_BASE (SubscriptionNotifyCB) *m_pNotifyCB;

    CComObject<CVariableNotify> *m_pVariableNotify;
    DWORD                       m_dwVarCookie;

    virtual void OnNewData (LONG hServer, const CComVariant &varValue, short sQuality, DATE date);

    virtual void OnSubscriptionReleased (CSubscription *pSubscription);
};


class CVariableNotify : public IDispatchImpl<_FCVariableNotification, &IID__FCVariableNotification, &LIBID_CSC_ONLINELib,2,0>,
                        public CComObjectRoot
{
public:
    CVariableNotify::CVariableNotify ();

    void Init (CSubscriptionManager *pSubscriptionManager);

protected:
    CSubscriptionManager *m_pSubscriptionManager;

private:
    STDMETHOD_(HRESULT, OnChanged)();
    STDMETHOD_(HRESULT, OnNewData)(/*[in]*/ long hServer, /*[in]*/ VARIANT newVal, /*[in]*/ short Quality, /*[in]*/ DATE Date);

    BEGIN_COM_MAP(CVariableNotify)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(_FCVariableNotification)
    END_COM_MAP()
};


class SFCHelper
{
public:
    static const LPCTSTR VisName;
    static const LPCTSTR ManualModeName;
    static const LPCTSTR DoNextName;
    static const LPCTSTR OnceForcedActionsName;
    static const LPCTSTR ForcedActionsName;
    static const LPCTSTR BlockedActionsName;
    static const LPCTSTR AllActionsOffName;
    static const LPCTSTR EnabledTransitionsName;
    static const LPCTSTR OnceForcedTransitionsName;
    static const LPCTSTR ForcedTransitionsName;
    static const LPCTSTR BlockedTransitionsName;
    static const LPCTSTR AllTransitionsOnName;
    static const LPCTSTR ManCmdName;

    static const TCHAR ListSeparator;

    static const LPCTSTR StepFieldX;
    static const LPCTSTR ActionFieldA;

    static bool IsStepInVis (const CString &strSFCVis, long lStepNumber);
    static bool IsActionInVis (const CString &strSFCVis, long lActionNumber);
    static bool IsTransitionInVis (const CString &strSFCVis, long lTransitionNumber);

    static bool StepsInVis (const CString &strSFCVis, CArray<long, long> &steps);
    static bool ActionsInVis (const CString &strSFCVis, CArray<long, long> &actions);
    static bool TransitionsInVis (const CString &strSFCVis, CArray<long, long> &transitions);

    static bool StepsInList (const CString &strList, CArray<long, long> &steps);
    static bool ActionsInList (const CString &strList, CArray<long, long> &actions);
    static bool TransitionsInList (const CString &strList, CArray<long, long> &transitions);

    static bool AddStepToList (CString &strList, long lStepNumber);
    static bool AddActionToList (CString &strList, long lActionNumber);
    static bool AddTransitionToList (CString &strList, long lTransitionNumber);

    static bool RemoveStepFromList (CString &strList, long lStepNumber);
    static bool RemoveActionFromList (CString &strList, long lStepNumber);
    static bool RemoveTransitionFromList (CString &strList, long lStepNumber);

    static bool SplitVis (const CString &strSFCVis, CString &strSteps, CString &strActions, CString &strTransitions);
    static bool ExtractCountedList (CString &strItems, CString &strList);
    static bool IsNumberInList (const CString &strList, long lNumber);
    static bool NumbersFromList (const CString &strList, CArray<long, long> &numbers);
    static bool ListFromNumbers (CString &strList, const CArray<long, long> &numbers);
    static bool ExtractNumberFromList (CString &strItems, long &lItem);
    static bool AddNumberToList (CString &strList, long lNumber);
    static bool RemoveNumberFromList (CString &strList, long lNumber);
};

#endif
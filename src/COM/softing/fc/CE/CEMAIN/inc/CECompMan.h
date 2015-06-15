
#ifndef CECOMPMAN_H
#define CECOMPMAN_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>
#include "CEStateMan.h"
#include "cecompmanev.h"
#include "CEAddonActionEventsComObj.h"

//----  Forward Class Definitions:   -------------------------------*
class CTargetSet;
class CCEComponentManagerEvents;
struct IMenu;
struct IToolbar;
//----  Defines:    ------------------------------------------------*
//------------------------------------------------------------------*
/**
 * @def COMP_MGR_NEXT_ARRAY_SIZE - size of array for enumeration 
 * functions AddFromFilesSection() AddFromMainMenuSection().
 * Will be used with the IEnumXXX::Next() functions to fetch more 
 * than one interface pointer with a call to Next()
 *
 * @see IEnumXXX::Next()
*/
#define COMP_MGR_NEXT_ARRAY_SIZE    (10)

#ifndef COMP_MGR_ASSERT
/**
* @def COMP_MGR_ASSERT - for other assert mechanisms replace only the define here.
*/
#define COMP_MGR_ASSERT(x) ASSERT((x))
#endif
//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


//------------------------------------------------------------------*
/**
 *  type COMComponentEntry to describe a COM component
 *
 *  @see    GetCOMComponentList()
*/
typedef struct
{
    IUnknown* m_pUnk;   /**<unknown pointer to component NULL means not created*/
    CLSID     m_clsid;  /**<CLSID of component */
    CString   m_strName; /**<name of component */
}COMComponentEntry;

//------------------------------------------------------------------*
/**
 *  class CCEComponentManager - manages 4CE's components.
 *  Holds a map of references to the components associated with a
 *  unique identifier string. COM components, simple and dialog based
 *  docking windows and target objects are managed by the component
 *  manager. Additionally the creation and destruction of the components 
 *  will be handled by the component manager according to defined life times.
 *  3 defined life times exist as listed below:<br>
 * - LifetimeSystem: creation during startup of system, destroy
 * during shutdown of system <br>
 * - LifetimeFrame: creation during create of frame window, destroy 
 * during close of frame window <br>
 * - LifetimeProject: creation if project is opened, destroy if
 * project is closed.<br>
 * For COM components despite of the lifetime parameter, lifetime can
 * additionally be controlled by the argument bCreateOnDemand. 
 * If the flag is set the COM component will be created if it is 
 * first used, e.g. during the first call of GetCOM. The destruction 
 * of the COM component (final release) is in contrast to the creation
 * bound to the parameter lifetime.
 *
 *  @see    Architecture of component/state manager - 4CE_CompManArch(1.20).doc
*/
class CCEComponentManager : public CCmdTarget
{
public:
    enum Lifetime { LifetimeSystem, LifetimeFrame, LifetimeProject };

    CCEComponentManager ();
    ~CCEComponentManager ();

    void SetDefaultParent (CWnd *pParent);

    HRESULT AddCOM (const CString &strName,
                    Lifetime lifetime,
                    const CLSID &classId,bool bCreateOnDemand = false);
    
    HRESULT AddSimpleDockingTool (const CString &strName,
                                  Lifetime lifetime,
                                  CRuntimeClass *pDockingToolClass,
                                  UINT uiId,
                                  DWORD dwStyle,
                                  const CString &strTitle,
                                  DWORD dwExStyle = 0UL);

    HRESULT AddDialogDockingTool (const CString &strName,
                                  Lifetime lifetime,
                                  CRuntimeClass *pDockingToolClass,
                                  UINT uiId,
                                  DWORD dwStyle,
                                  UINT uiIDD,
                                  DWORD dwExStyle = CBRS_EX_STDCONTEXTMENU);
    
    HRESULT AddTargetSet (const CString &strName);
    HRESULT AddFromKAD();

    HRESULT RemoveComponent (const CString &strName);

    HRESULT GetCOM (const CString &, IUnknown *&, bool bForceCreate = false);
    HRESULT GetCOMByCLSID(const CString &,IUnknown *&, bool bForceCreate = false);
    HRESULT GetDockingTool (const CString &strName, SECControlBar *&pDockingTool);
    bool    IsDockingTool (const CString &strName);

    HRESULT GetTargetSet (const CString &strName, CTargetSet *&pTargetSet);

    HRESULT GetCOMComponentList(COMComponentEntry* & pList,int & iCount);
    void FreeCOMComponentList(COMComponentEntry * pList,int iCount);

    bool IsLifetimeActive (Lifetime lifetime);

    static CCEComponentManager *GetMe ();

protected:
    class LogicalComponent
    {
    public:
        LogicalComponent (const CString &strName, Lifetime lifetime, bool bCreateOnDemand = false)
            : m_strName (strName),
              m_Lifetime (lifetime)
        {
            m_bCreateOnDemand = bCreateOnDemand;
        }

        virtual ~LogicalComponent () { }

        CString GetName () { return (m_strName); }
        Lifetime GetLifetime () { return (m_Lifetime); }
        bool OnDemand() { return m_bCreateOnDemand; }

        virtual HRESULT Create () = 0;
        virtual HRESULT Destroy () = 0;
        virtual bool IsCreated () = 0;


    protected:
        CString  m_strName;
        Lifetime m_Lifetime;
        bool     m_bCreateOnDemand;
    };

    class COMComponent : public LogicalComponent
    {
    public:
        COMComponent (const CString &strName, Lifetime lifetime, const CLSID &classId, bool bCreateOnDemand)
            : LogicalComponent (strName, lifetime,bCreateOnDemand),
              m_ClassId (classId)
        {
//            m_dwROT = 0UL;
            m_dwAdvise = 0UL;
            m_dwAddonAdvise = 0UL;
            m_dwAddonAdvise2 = 0UL;
        }

        virtual ~COMComponent () { }

        CLSID GetClassId () { return (m_ClassId); }
        void  GetClassId(CString &);
        IUnknown *GetInterface () { return (m_pUnknown); }

        virtual HRESULT Create ();
        virtual HRESULT Destroy ();
        virtual bool IsCreated ();

    protected:
        CLSID             m_ClassId;
        CComPtr<IUnknown> m_pUnknown;
        DWORD             m_dwAdvise;
        DWORD             m_dwAddonAdvise;
        DWORD             m_dwAddonAdvise2;
        CComPtr<IMoniker> m_pMoniker;
//        DWORD   m_dwROT;

    private:
        HRESULT Advise();
        HRESULT UnAdvise();
        HRESULT AdviseAddonEvents();
        HRESULT UnAdviseAddonEvents();
        HRESULT CreateMoniker(IMoniker **);
//        HRESULT GetFromROT(IUnknown *& pUnk);
//        HRESULT RegisterROT();
//        void    UnRegisterROT();
    };

    friend class COMComponent;

    class DockingToolComponent : public LogicalComponent
    {
    public:
        DockingToolComponent (const CString &strName, Lifetime lifetime,
                              CRuntimeClass *pClass, CWnd *pParent, UINT uiId, 
                              DWORD dwStyle, DWORD dwExStyle)
            : LogicalComponent (strName, lifetime),
              m_pClass (pClass),
              m_pParent (pParent),
              m_uiId (uiId),
              m_dwStyle (dwStyle),
              m_dwExStyle (dwExStyle),
              m_pControlBar (NULL)
        {
            ASSERT (pClass != NULL);
        }

        virtual ~DockingToolComponent () { }

        CRuntimeClass *GetClass () { return (m_pClass); }
        CWnd *GetParent () { return (m_pParent); }
        UINT GetId () { return (m_uiId); }
        DWORD GetStyle () { return (m_dwStyle); }
        SECControlBar *GetDockingTool () { return (m_pControlBar); }

        virtual HRESULT Create ();
        virtual HRESULT Destroy ();
        virtual bool IsCreated ();

    protected:
        CRuntimeClass      *m_pClass;
        CWnd               *m_pParent;
        UINT               m_uiId;
        DWORD              m_dwStyle;
        DWORD              m_dwExStyle;
        SECControlBar      *m_pControlBar;
    };

    class SimpleDockingToolComponent : public DockingToolComponent
    {
    public:
        SimpleDockingToolComponent (const CString &strName, Lifetime lifetime,
                                    CRuntimeClass *pClass, CWnd *pParent, UINT uiId, DWORD dwStyle,
                                    const CString &strTitle,DWORD dwExStyle = 0UL)
            : DockingToolComponent (strName, lifetime, pClass, pParent, uiId, dwStyle, dwExStyle),
              m_strTitle (strTitle)
        {
        }

        virtual ~SimpleDockingToolComponent () { }

        CString GetTitle () { return (m_strTitle); }

        virtual HRESULT Create ();

    protected:
        CString m_strTitle;
    };

    class DialogDockingToolComponent : public DockingToolComponent
    {
    public:
        DialogDockingToolComponent (const CString &strName, Lifetime lifetime,
                                    CRuntimeClass *pClass, CWnd *pParent, UINT uiId, DWORD dwStyle,
                                    UINT uiIDD, DWORD dwExStyle = CBRS_EX_STDCONTEXTMENU )
            : DockingToolComponent (strName, lifetime, pClass, pParent, uiId, dwStyle, dwExStyle),
              m_uiIDD (uiIDD)
        {
        }

        virtual ~DialogDockingToolComponent () { }

        UINT GetIDD () { return (m_uiIDD); }

        virtual HRESULT Create ();

    protected:
        UINT m_uiIDD;
    };

    class TargetSetComponent : public LogicalComponent
    {
    public:
        TargetSetComponent (const CString &strName, Lifetime lifetime)
            : LogicalComponent (strName, lifetime),
              m_pTargetSet (NULL)
        {
        }

        virtual ~TargetSetComponent () { }

        CTargetSet *GetTargetSet () { return (m_pTargetSet); }

        virtual HRESULT Create ();
        virtual HRESULT Destroy ();
        virtual bool IsCreated ();

    protected:
        CTargetSet *m_pTargetSet;
    };

    CCEStateManager *m_pStateManager;
    CWnd *m_pDefaultParent;
    CComObject<CCEComponentManagerEvents>* m_pEvents;
    CComObject<CCEAddOnActionEvents>* m_pAddonEvents;

    CMap<CString,LPCTSTR , LogicalComponent *, LogicalComponent *> m_Components;
    CMap<CString,LPCTSTR, COMComponent *, COMComponent *> m_CLSIDMap;

    FC_CALLBACK (CEStateChangeCB, CCEComponentManager) m_StateChangeCB;

    bool m_bLifetimeSystemActive;
    bool m_bLifetimeFrameActive;
    bool m_bLifetimeProjectActive;

    LogicalComponent *m_pAccessibleComponent;

    static CCEComponentManager *m_pMe;

private:
    void OnStateChange (CEStateNotify stateNotify, unsigned int uiState);
    void BeginLifetime (Lifetime lifetime);
    void EndLifetime (Lifetime lifetime);

    void LookupComponent (const CString &strName, LogicalComponent *&pComponent);
    void AddComponent (LogicalComponent *pComponent);
    void RemComponent (LogicalComponent *pComponent);
    void AddComponent (COMComponent *pComponent);
    void RemComponent (COMComponent *pComponent);

    void MaybeCreateComponent (LogicalComponent *pComponent);
    void MaybeDestroyComponent (LogicalComponent *pComponent);

    void CreateComponents (Lifetime lifetime);
    void ComponentsAccessible (Lifetime lifetime);
    void HandsOffComponents (Lifetime lifetime);
    void DestroyComponents (Lifetime lifetime);
    HRESULT CreateComponent (LogicalComponent *pComponent);
    void DestroyComponent (LogicalComponent *pComponent);
    void Quit();

    //KAD related private methods
    void AddFromFilesSection(IUnknown*); //throws _com_error exception
    void AddFromMainMenuSection(IUnknown*); //throws _com_error exception
    HRESULT AddMenuItems(IMenu *);

// SIS, 08.05.01: commented out, no handlers any more
//    void AddFromToolbarsSection(IUnknown*); //throws _com_error exception
//    HRESULT AddToolbarItems(IToolbar *);

    void AddFromTypesSection(IUnknown*);//throws _com_error exception

};

//------------------------------------------------------------------*
/**
 *  class CCEComponentManagerEvents - implements interface ICECompManEvents.
 *  Allows COM components to query for other COM components maintained
 *  by the component manager residing in 4CE. The corresponding comp-
 *  onents will be created on demand if they are not active.
 *
*/
class CCEComponentManagerEvents : public CComObjectRoot,
                         public ICECompManEvents
{
public:
    CCEComponentManagerEvents() {}
    virtual ~CCEComponentManagerEvents() {}

    //------------------------------------------------------------------*
    /**
     * Will be called during final construction of the object. Overwritten
     * to provide an initial reference count of one for the object. Hence
     * calls AddRef() to increment reference count.
     * @return          S_OK;
     * 
    */
    HRESULT FinalConstruct()
    { 
        AddRef(); 
        return S_OK;
    }
public:

protected:
    DECLARE_PROTECT_FINAL_CONSTRUCT();
private:
	BEGIN_COM_MAP(CCEComponentManagerEvents)
   	    COM_INTERFACE_ENTRY(IUnknown)
   	    COM_INTERFACE_ENTRY(ICECompManEvents)
	END_COM_MAP()

	STDMETHOD(OnComponentByCLSID)(BSTR sCLSID, IUnknown ** ppUnk);
    STDMETHOD(OnComponentById)(BSTR sId, IUnknown * * ppUnk);
};


CCEComponentManager *CEGetComponentManager ();

IUnknown *CEGetCOM (const CString &strName, bool bMustLive=false);
IUnknown *CEGetCOMByCLSID(const CString & strCLSID, bool bMustLive=false);

SECControlBar *CEGetDockingTool (const CString &strName, bool bMustLive=true);
bool CEIsDockingTool(const CString & strName);

CTargetSet *CEGetTargetSet (const CString &strName, bool bMustLive=true);
bool CESystemMessage(LPCTSTR pszMessage);

#endif


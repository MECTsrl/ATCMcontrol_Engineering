

//----  Includes:   -------------------------------------------*
#include "stdafx.h" 
#include "utilif.h"
#include "cemainerror.h"
#include "CECompMan.h"
#include "TargetSet.h"
#include "MainFrm.h"
#include "GenEdCont.h"

//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

//------------------------------------------------------------------*
/**
 * Do not provide HashKey anymore. a different declaration of CMap
 * will solve the problem. Leave the function here for information.
 * @see             class CMap
UINT AFXAPI HashKey (const CString &strKey)
{
    LPCTSTR pszKey;
    UINT uiHash=0;

    for (pszKey = strKey; *pszKey != _T ('\0'); ++pszKey)
    {
        uiHash = (uiHash << 5) + uiHash + *pszKey;
    }

	return (uiHash);
}
*/

CCEComponentManager *CCEComponentManager::m_pMe;

CCEComponentManager *CCEComponentManager::GetMe ()
{
    return (m_pMe);
}

//------------------------------------------------------------------*
/**
 * constructor - resets members and registers the state 
 * manager if existent. State manager must exist prior to the 
 * construction of the component manager. Creates also the 
 * event sink object of class CCEComponentManagerEvents. This
 * object implements the interfaces ICECompManEvents and ICEAddonActionEvents.
 *
 * @see     class CCEComponentManagerEvents, interface ICECompManEvents
 * @see     interface ICEAddonActionEvents
 * @see     ~CCEComponentManager() - dtor
 *
*/
CCEComponentManager::CCEComponentManager ()
: m_StateChangeCB (this, OnStateChange),
  m_pDefaultParent (NULL),
  m_bLifetimeSystemActive (false),
  m_bLifetimeFrameActive (false),
  m_bLifetimeProjectActive (false),
  m_pAccessibleComponent (NULL)
{
    
    COMP_MGR_ASSERT (m_pMe == NULL);
    if (m_pMe == NULL)
    {
        m_pMe = this;
    }

    m_pStateManager = CEGetStateManager ();
    if (m_pStateManager != NULL)
    {
        m_pStateManager->RegisterCallback (&m_StateChangeCB);
    }

    m_pEvents = NULL;
    m_pAddonEvents = NULL;
    CComObject<CCEComponentManagerEvents>::CreateInstance(&m_pEvents);
    CComObject<CCEAddOnActionEvents>::CreateInstance(&m_pAddonEvents);
}

//------------------------------------------------------------------*
/**
 * Destructor of component manager. Deletes stored component containers
 * in the list of maintained components. Unregisters the state manager
 * if it has been registered with construction. Releases the 
 * event sink object of class CCEComponentManagerEvents. This
 * object implements the interface ICECompManEvents and ICEAddonActionEvents.
 *
 * @see  class CCEComponentManagerEvents, interface ICECompManEvents
 * @see  interface ICEAddonActionEvents
 * @see  CCEComponentManager() - ctor
*/
CCEComponentManager::~CCEComponentManager ()
{
    POSITION pPosition;

    if (m_pStateManager != NULL)
    {
        m_pStateManager->UnregisterCallback (&m_StateChangeCB);
    }

    m_CLSIDMap.RemoveAll();
    pPosition = m_Components.GetStartPosition ();
    while (pPosition != NULL)
    {
        CString strName;
        LogicalComponent *pComponent;

        m_Components.GetNextAssoc (pPosition, strName, pComponent);
        delete pComponent;
    }

    m_Components.RemoveAll();

    if(m_pEvents) 
    {
        m_pEvents->Release();
        m_pEvents = NULL;
    }
    if(m_pAddonEvents)
    {
        m_pAddonEvents->Release();
        m_pAddonEvents = NULL;
    }
}

//------------------------------------------------------------------*
/**
 * Sets the default parent window into the component manager. The
 * default parent window will be used as parent window when creating
 * a simple docking window or a dialog based docking window.
 *
 * @param           pParent - reference to the parent CWnd window 
 *                  encapsulation.
*/
void CCEComponentManager::SetDefaultParent (CWnd *pParent)
{
    m_pDefaultParent = pParent;
}

//------------------------------------------------------------------*
/**
 * Adds a COM component with the id strName to the administration of
 * the component manager. strName shall be a unique identifier for
 * lookups in the component manager. For a description of the life time 
 * enumerations see description of class CCEComponentManager. 
 * Despite of the lifetime parameter, lifetime can additionally be 
 * controlled by the argument bCreateOnDemand. If the flag is set
 * the COM component will be created if it is first used, e.g. during
 * the first call of GetCOM. The destruction of the COM component (final
 * release) is in contrast to the creation bound to the parameter lifetime.
 *
 * <b>If the component could not be created due to an fatal error, the application
 * will be closed.</b>
 * 
 * @param           strName - unique identifier to identify the component.
 * @param           lifetime - argument determining the life time of the 
 *                  component (see description above)
 * @param           classId - class id of the component
 * @param           bCreateOnDemand - determines additionally life time of
 *                  the component (see description above).
 * @return          HRESULT - S_OK if successful call,<br>
 *                  E_OUTOFMEMORY - out of memory situation
 *
 * @see             GetCOM()             
*/
HRESULT CCEComponentManager::AddCOM (const CString &strName, Lifetime lifetime, 
                                     const CLSID &classId, bool bCreateOnDemand)
{
    LogicalComponent *pDuplicate;
    COMComponent *pComponent;


    // Equally named logical component must not exist.
    //////////////////////////////////////////////////

    LookupComponent (strName, pDuplicate);
    if (pDuplicate != NULL)
    {
        return (S_OK);
    }


    // Create component list entry that describes a
    // COM component. Add it to the component map.
    ///////////////////////////////////////////////

    pComponent = new COMComponent (strName, lifetime, classId, bCreateOnDemand);
    COMP_MGR_ASSERT (pComponent != NULL);
    if (pComponent == NULL)
    {
        return (E_OUTOFMEMORY);
    }

    AddComponent (pComponent);


    // If lifetime that applies for our logical
    // component is active create the component.
    ////////////////////////////////////////////

    MaybeCreateComponent (pComponent);
    return S_OK;
}


//------------------------------------------------------------------*
/**
 * Adds a simple docking window with the id strName to the administration of
 * the component manager. strName shall be a unique identifier for
 * lookups in the component manager. For a description of the life time 
 * enumerations see description of class CCEComponentManager. The method
 * creates the corresponding docking window if the life time has been 
 * reached. Otherwise if the life time is not reached the docking window
 * will only be registered with the component manager.
 * 
 * @param           strName - unique identifier to identify the component.
 * @param           lifetime - argument determining the life time of the 
 *                  component (see description of class CCEComponentManager)
 * @param           pDockingToolClass - runtime class of the window class
 *                  The window class must be a subclass of SECControlBar.
 * @param           uiId - identifier for command dispatches.
 * @param           dwStyle - window style for ::CreateWindow(), 
 *                  ::CreateWindowEx()
 * @param           strTitle - title for the title bar of the docking tool
 *                  window.
 * @param           dwExStyle - ext. window style for ::CreateWindow(), 
 *                  ::CreateWindowEx()
 * @return          HRESULT - S_OK if successful call,<br>
 * @return          E_OUTOFMEMORY - out of memory situation<br>
 * @return          E_CECOMPMAN_COMPONENT_ALREADY_ADDED - component has
 *                  already been created.
 *
 * @see             GetDockingTool(), CEGetDockingTool()             
*/
HRESULT CCEComponentManager::AddSimpleDockingTool (const CString &strName,
                                                   Lifetime lifetime,
                                                   CRuntimeClass *pDockingToolClass,
                                                   UINT uiId,
                                                   DWORD dwStyle,
                                                   const CString &strTitle,
                                                   DWORD dwExStyle)
{
    LogicalComponent *pDuplicate;
    DockingToolComponent *pComponent;


    // Equally named logical component must not exist.
    //////////////////////////////////////////////////

    LookupComponent (strName, pDuplicate);
    if (pDuplicate != NULL)
    {
        return (E_CECOMPMAN_COMPONENT_ALREADY_ADDED);
    }


    // Create component list entry that describes a docking
    // tool component. Add it to the component map.
    ///////////////////////////////////////////////////////

    pComponent = new SimpleDockingToolComponent (strName, lifetime, pDockingToolClass, m_pDefaultParent, uiId, dwStyle, strTitle);
    COMP_MGR_ASSERT (pComponent != NULL);
    if (pComponent == NULL)
    {
        return (E_OUTOFMEMORY);
    }

    AddComponent (pComponent);


    // If lifetime that applies for our logical
    // component is active create the component.
    ////////////////////////////////////////////

    MaybeCreateComponent (pComponent);
    return S_OK;
}


//------------------------------------------------------------------*
/**
 * Adds a dialog based docking window with the id strName to the 
 * administration of the component manager. strName shall be a unique 
 * identifier for lookups in the component manager.
 * For a description of the life time enumerations see description of
 * class CCEComponentManager. The method creates the corresponding 
 * docking window if the life time has been reached. Otherwise if the
 * life time is not reached the docking window will only be registered 
 * with the component manager.
 * 
 * @param           strName - unique identifier to identify the component.
 * @param           lifetime - argument determining the life time of the 
 *                  component (see description of class CCEComponentManager)
 * @param           pDockingToolClass - runtime class of the window class
 *                  The window class must be a subclass of SECControlBar.
 * @param           uiId - identifier for command dispatches.
 * @param           dwStyle - window style for ::CreateWindow(), 
 *                  ::CreateWindowEx()
 * @param           strTitle - title for the title bar of the docking tool
 *                  window.
 * @param           dwExStyle - ext. window style for ::CreateWindow(), 
 *                  ::CreateWindowEx()
 * @param           uiIDD - identifier for the dialog template.
 * @return          HRESULT - S_OK if successful call,<br>
 * @return          E_OUTOFMEMORY - out of memory situation<br>
 * @return          E_CECOMPMAN_COMPONENT_ALREADY_ADDED - component has
 *                  already been created.
 *
 * @see             GetDockingTool(), CEGetDockingTool()             
*/
HRESULT CCEComponentManager::AddDialogDockingTool (const CString &strName,
                                                   Lifetime lifetime,
                                                   CRuntimeClass *pDockingToolClass,
                                                   UINT uiId,
                                                   DWORD dwStyle,
                                                   UINT uiIDD,
                                                   DWORD dwExStyle)
{
    LogicalComponent *pDuplicate;
    DockingToolComponent *pComponent;


    // Equally named logical component must not exist.
    //////////////////////////////////////////////////

    LookupComponent (strName, pDuplicate);
    if (pDuplicate != NULL)
    {
        return (E_CECOMPMAN_COMPONENT_ALREADY_ADDED);
    }


    // Create component list entry that describes a docking
    // tool component. Add it to the component map.
    ///////////////////////////////////////////////////////

    pComponent = new DialogDockingToolComponent (strName, lifetime, pDockingToolClass, m_pDefaultParent, uiId, dwStyle, uiIDD, dwExStyle);
    COMP_MGR_ASSERT (pComponent != NULL);
    if (pComponent == NULL)
    {
        return (E_OUTOFMEMORY);
    }

    AddComponent (pComponent);

    // If lifetime that applies for our logical
    // component is active create the component.
    ////////////////////////////////////////////

    MaybeCreateComponent (pComponent);
    return S_OK;
}


HRESULT CCEComponentManager::AddTargetSet (const CString &strName)
{
    LogicalComponent *pDuplicate;
    TargetSetComponent *pComponent;


    // Equally named logical component must not exist.
    //////////////////////////////////////////////////

    LookupComponent (strName, pDuplicate);
    if (pDuplicate != NULL)
    {
        return (E_CECOMPMAN_COMPONENT_ALREADY_ADDED);
    }


    // Create component list entry that describes a Target
    // set component. Add it to the component map.
    //////////////////////////////////////////////////

    pComponent = new TargetSetComponent (strName, LifetimeProject);
    COMP_MGR_ASSERT (pComponent != NULL);
    if (pComponent == NULL)
    {
        return (E_OUTOFMEMORY);
    }

    AddComponent (pComponent);


    // If lifetime that applies for this logical
    // component is active create the component.
    ////////////////////////////////////////////

    MaybeCreateComponent (pComponent);
    return S_OK;
}

//------------------------------------------------------------------*
/**
 * Uses the KAD manager to iterate over all entries in the KAD 
 * section and to add all relevant components to the component manager.
 *
 *  @return     HRESULT of a call to AddFromFilesSection(),
 *              AddFromMainMenuSection()
 *  @see        AddFromFilesSection(),AddFromMainMenuSection()
 */
HRESULT CCEComponentManager::AddFromKAD()
{
    HRESULT hr = S_OK;
    IUnknown *pUnk = NULL;

    try {
        hr = GetCOM("KadManager",pUnk);
        if(FAILED(hr)) {
            _com_raise_error(hr);
        }
        FC_PCHECK_THROW(pUnk,_com_error(E_POINTER));
        AddFromFilesSection(pUnk);
        AddFromMainMenuSection(pUnk);
//        AddFromToolbarsSection(pUnk);     // SIS, 08.05.01: no handlers any more
        AddFromTypesSection(pUnk);

    }
    catch(_com_error err) {
        hr = err.Error();
    }

    FC_RELEASE_PTR(pUnk);
    return hr;
}

//------------------------------------------------------------------*
/**
 * Removes a component with the identifier strName from the 
 * administration of the component manager. strName shall be a unique 
 * identifier for lookups in the component manager. The component must
 * be registered prior through a call to AddCOM(), AddSimpleDockingTool()
 * or AddDialogDockingTool().
 *
 * @param           strName - unique identifier to identify the component.
 * @return          E_CECOMPMAN_UNKNOWN_COMPONENT - unknown component,
 *                  AddCOM(), AddSimpleDockingTool(),AddDialogDockingTool()
 *                  has not been called prior to invoking this function.
 *
 * @see             AddCOM(), AddSimpleDockingTool(),AddDialogDockingTool()
*/
HRESULT CCEComponentManager::RemoveComponent (const CString &strName)
{
    LogicalComponent *pComponent;

    // Lookup component by name.
    ////////////////////////////

    LookupComponent (strName, pComponent);
    if (pComponent == NULL)
    {
        return (E_CECOMPMAN_UNKNOWN_COMPONENT);
    }


    // If lifetime that applies for this logical
    // component is active destroy the component.
    /////////////////////////////////////////////

    MaybeDestroyComponent (pComponent);


    // Remove component from component map and delete it.
    /////////////////////////////////////////////////////

    RemComponent (pComponent);

    delete pComponent;

    return (S_OK);
}

//------------------------------------------------------------------*
/**
 * Gets a COM component by the unique identifier. The component has
 * to be added to the component manager by the function AddCOM() prior
 * to calling this function. In the event that the component has been
 * added with the bCreateOnDemand flag set to true the component will
 * be created with the first call to this function if bForceCreate has
 * been set. Returns a strong reference with the IUnknown ref.
 *
 * @param           strName - unique identifier to identify the component.
 * @param           pUnknown - reference to an IUnknown pointer receiving
 *                  the default interface pointer of the component. Will
 *                  be set to NULL if component has not been created.
 * @param           bForceCreate - enforces creation.
 * @return          E_CECOMPMAN_UNKNOWN_COMPONENT - the component
 *                  cannot be found in the component manager.<br>
 *                  E_CECOMPMAN_COMPONENT_NOT_ALIVE - the bCreateOnDemand flag
 *                  is set to false during AddCOM() and the component is not
 *                  in the current lifetime period.<br>
 *                  E_CECOMPMAN_NOT_COM_COMPONENT - component is not a COM component.
 *                  E_FAIL - Inteface pointer of component is NULL.<br>
 *                  S_OK - successful call.<br>
 * @see             AddCOM()            
*/
HRESULT CCEComponentManager::GetCOM (const CString &strName, IUnknown *&pUnknown,bool bForceCreate)
{
    LogicalComponent *pComponent;
    COMComponent *pCOMComponent;
    DWORD dwRef;

    pUnknown = NULL;

    LookupComponent (strName, pComponent);
    if (pComponent == NULL)
    {
        return (E_CECOMPMAN_UNKNOWN_COMPONENT);
    }

    if (pComponent != m_pAccessibleComponent && !IsLifetimeActive (pComponent->GetLifetime ()))
    {
        if(pComponent->OnDemand() == false) {
            return (E_CECOMPMAN_COMPONENT_NOT_ALIVE);
        }
        //create on demand flag == true - fall thru'
    }

    //the component may not be created if the demand flag is set - create it now
    if(!pComponent->IsCreated()) {
        if(!bForceCreate) {
            return E_FAIL; //not created and not enforced
        }
        CreateComponent(pComponent);
    }

    pCOMComponent = dynamic_cast<COMComponent *> (pComponent);
    if (pCOMComponent == NULL)
    {
        return (E_CECOMPMAN_NOT_COM_COMPONENT);
    }

    pUnknown = pCOMComponent->GetInterface ();
    if (pUnknown == NULL)
    {
        return (E_FAIL);
    }

    dwRef = pUnknown->AddRef();
    return (S_OK);
}


//------------------------------------------------------------------*
/**
 * Gets a COM component by the CLSID. The component has
 * to be added to the component manager by the function AddCOM() prior
 * to calling this function. In the event that the component has been
 * added with the bCreateOnDemand flag set to true the component will
 * be created with the first call to this function if bForceCreate has
 * been set. Returns a strong reference with the IUnknown ref.
 *
 * @param           strGuid - CLSID to identify the component.
 * @param           pUnknown - reference to an IUnknown pointer receiving
 *                  the default interface pointer of the component. Will
 *                  be set to NULL if component has not been created.
 * @param           bForceCreate - enforces creation.
 * @return          E_CECOMPMAN_UNKNOWN_COMPONENT - the component
 *                  cannot be found in the component manager.<br>
 *                  E_CECOMPMAN_COMPONENT_NOT_ALIVE - the bCreateOnDemand flag
 *                  is set to false during AddCOM() and the component is not
 *                  in the current lifetime period.<br>
 *                  E_FAIL - Inteface pointer of component is NULL.<br>
 *                  S_OK - successful call.<br>
 * @see             AddCOM()            
*/
HRESULT CCEComponentManager::GetCOMByCLSID(const CString &strGuid,IUnknown *&pUnknown,bool bForceCreate)
{
    COMComponent *pCOMComponent;
    DWORD dwRef;

    pUnknown = NULL;

    if(!m_CLSIDMap.Lookup(strGuid,pCOMComponent)) {
        return (E_CECOMPMAN_UNKNOWN_COMPONENT);
    }

    if (pCOMComponent == NULL) {
        return (E_CECOMPMAN_UNKNOWN_COMPONENT);
    }

    if (pCOMComponent != m_pAccessibleComponent && !IsLifetimeActive (pCOMComponent->GetLifetime ()))
    {
        if(pCOMComponent->OnDemand() == false) {
            return (E_CECOMPMAN_COMPONENT_NOT_ALIVE);
        }
        //create on demand flag == true - fall thru'
    }

    //the component may not be created if the demand flag is set - create it now
    if(!pCOMComponent->IsCreated()) {
        if(!bForceCreate) { //not created and not enforced
            return E_FAIL;
        }
        CreateComponent(pCOMComponent);
    }

    pUnknown = pCOMComponent->GetInterface ();
    if (pUnknown == NULL)
    {
        return (E_FAIL);
    }

    dwRef = pUnknown->AddRef();
    return (S_OK);
}

//------------------------------------------------------------------*
/**
 * Gets a docking tool by the unique identifier. The component has
 * to be added to the component manager by the function 
 * AddSimpleDockingTool() or AddDialogDockingTool() prior
 * to calling this function. Returns a reference to the SECControlBar
 * super class.
 *
 * @param           strName - unique identifier to identify the component.
 * @param           pDockingTool- SECControlBar reference or NULL in case 
 *                  of an error.<br>
 *                  Possible error cases are: Docking tool has not been
 *                  added to the component manager using the methods 
 *                  mentioned above or Docking tool is not created resp.
 *                  alive due to another life time.
 * @return          E_CECOMPMAN_UNKNOWN_COMPONENT - unknown component - 
 *                  AddSimpleDockingTool(), AddDialogDockingTool() has not
 *                  been called.<br>
 * @return          E_CECOMPMAN_COMPONENT_NOT_ALIVE - component not alive
 *                  due to the life time specified with the add functions.<br>
 * @return          E_FAIL - component window not created - fatal<br>
 * @return          S_OK - success.
 *
 * @see             AddSimpleDockingTool(), AddDialogDockingTool() 
*/
HRESULT CCEComponentManager::GetDockingTool (const CString &strName, SECControlBar *&pDockingTool)
{
    LogicalComponent *pComponent;
    DockingToolComponent *pDockingToolComponent;

    pDockingTool = NULL;

    LookupComponent (strName, pComponent);
    if (pComponent == NULL)
    {
        return (E_CECOMPMAN_UNKNOWN_COMPONENT);
    }

    if (pComponent != m_pAccessibleComponent && !IsLifetimeActive (pComponent->GetLifetime ()))
    {
        return (E_CECOMPMAN_COMPONENT_NOT_ALIVE);
    }

    pDockingToolComponent = dynamic_cast<DockingToolComponent *> (pComponent);
    if (pDockingToolComponent == NULL)
    {
        return (E_CECOMPMAN_NOT_DOCKINGTOOL_COMPONENT);
    }

    pDockingTool = pDockingToolComponent->GetDockingTool ();
    if (pDockingTool == NULL)
    {
        return (E_FAIL);
    }

    return (S_OK);
}

bool CCEComponentManager::IsDockingTool (const CString &strName)
{
    LogicalComponent *pComponent;
    DockingToolComponent *pDockingToolComponent;

    LookupComponent (strName, pComponent);
    if (pComponent == NULL)
    {
        return false;
    }

    pDockingToolComponent = dynamic_cast<DockingToolComponent *> (pComponent);
    if (pDockingToolComponent == NULL)
    {
        return false;
    }

    return true;
}

HRESULT CCEComponentManager::GetTargetSet (const CString &strName, CTargetSet *&pTargetSet)
{
    LogicalComponent *pComponent;
    TargetSetComponent *pTargetSetComponent;

    pTargetSet = NULL;

    LookupComponent (strName, pComponent);
    if (pComponent == NULL)
    {
        return (E_CECOMPMAN_UNKNOWN_COMPONENT);
    }

    if (pComponent != m_pAccessibleComponent && !IsLifetimeActive (pComponent->GetLifetime ()))
    {
        return (E_CECOMPMAN_COMPONENT_NOT_ALIVE);
    }

    pTargetSetComponent = dynamic_cast<TargetSetComponent *> (pComponent);
    if (pTargetSetComponent == NULL)
    {
        return (E_CECOMPMAN_NOT_TARGETSET_COMPONENT);
    }

    pTargetSet = pTargetSetComponent->GetTargetSet ();
    if (pTargetSet == NULL)
    {
        return (E_FAIL);
    }

    return (S_OK);
}


bool CCEComponentManager::IsLifetimeActive (Lifetime lifetime)
{
    return (lifetime == LifetimeSystem && m_bLifetimeSystemActive ||
            lifetime == LifetimeFrame && m_bLifetimeFrameActive ||
            lifetime == LifetimeProject && m_bLifetimeProjectActive);
}


void CCEComponentManager::OnStateChange (CEStateNotify stateNotify, unsigned int uiState)
{
	// SIS, 05.03.01: editor container activated
    CGenEdCont *pContainer;

    switch (stateNotify)
    {
    case CESystemStartupNotify:
        BeginLifetime (LifetimeSystem);
        break;

    case CESystemShutdownNotify:
        EndLifetime (LifetimeSystem);
        break;

    case CECreateFrameNotify:
        BeginLifetime (LifetimeFrame);
        break;

    case CEDestroyFrameNotify:
        EndLifetime (LifetimeFrame);
        break;

    case CEPreOpenProjectNotify:
//        pContainer = CEGetContainer ();
//        if (pContainer != NULL)
//        {
//            pContainer->LoadPrintServers();
//        }
        BeginLifetime (LifetimeProject);
        break;

    case CEPostCloseProjectNotify:
        EndLifetime (LifetimeProject);
        pContainer = CEGetContainer ();
        if (pContainer != NULL)
        {
            pContainer->UnloadPrintServers();
        }
        break;
    }
}


void CCEComponentManager::BeginLifetime (Lifetime lifetime)
{
    if (IsLifetimeActive (lifetime))
    {
        return;
    }

    CreateComponents (lifetime);

    switch (lifetime)
    {
    case LifetimeSystem:
        m_bLifetimeSystemActive = true;
        break;

    case LifetimeFrame:
        m_bLifetimeFrameActive = true;
        break;

    case LifetimeProject:
        m_bLifetimeProjectActive = true;
        break;
    }

    ComponentsAccessible (lifetime);
}


void CCEComponentManager::EndLifetime (Lifetime lifetime)
{
    if (!IsLifetimeActive (lifetime))
    {
        return;
    }

    HandsOffComponents (lifetime);

    switch (lifetime)
    {
    case LifetimeSystem:
        m_bLifetimeSystemActive = false;
        break;

    case LifetimeFrame:
        m_bLifetimeFrameActive = false;
        break;

    case LifetimeProject:
        m_bLifetimeProjectActive = false;
        break;
    }

    DestroyComponents (lifetime);
}


void CCEComponentManager::LookupComponent (const CString &strName, LogicalComponent *&pComponent)
{
    if (!m_Components.Lookup (strName, pComponent))
    {
        pComponent = NULL;
    }
}


void CCEComponentManager::AddComponent (LogicalComponent *pComponent)
{
    m_Components.SetAt (pComponent->GetName (), pComponent);
}


void CCEComponentManager::RemComponent (LogicalComponent *pComponent)
{
    m_Components.RemoveKey (pComponent->GetName ());
}

void CCEComponentManager::AddComponent (COMComponent *pComponent)
{
    CString str;
    m_Components.SetAt (pComponent->GetName (), pComponent);
    pComponent->GetClassId(str);
    m_CLSIDMap.SetAt(str, pComponent);
}

void CCEComponentManager::RemComponent (COMComponent *pComponent)
{
    CString str;
    pComponent->GetClassId(str);
    m_Components.RemoveKey (pComponent->GetName ());
    m_CLSIDMap.RemoveKey(str);
}


void CCEComponentManager::MaybeCreateComponent (LogicalComponent *pComponent)
{
    if (IsLifetimeActive (pComponent->GetLifetime ()) && 
        (pComponent->OnDemand() == false))
    {
        CreateComponent (pComponent);
    }
}


void CCEComponentManager::MaybeDestroyComponent (LogicalComponent *pComponent)
{
    if (IsLifetimeActive (pComponent->GetLifetime ()))
    {
        DestroyComponent (pComponent);
    }
}


void CCEComponentManager::CreateComponents (Lifetime lifetime)
{
    POSITION pPosition;
    CString strName;
    LogicalComponent *pComponent;

    pPosition = m_Components.GetStartPosition ();
    while (pPosition != NULL)
    {
        m_Components.GetNextAssoc (pPosition, strName, pComponent);

        if (pComponent->GetLifetime () == lifetime &&
            pComponent->OnDemand() == false)
        {
            CreateComponent (pComponent);
        }
    }
}


void CCEComponentManager::ComponentsAccessible (Lifetime lifetime)
{
    POSITION pPosition;
    CString strName;
    LogicalComponent *pComponent;

    pPosition = m_Components.GetStartPosition ();
    while (pPosition != NULL)
    {
        m_Components.GetNextAssoc (pPosition, strName, pComponent);

        if (pComponent->GetLifetime () == lifetime && pComponent->IsCreated ())
        {
            m_pStateManager->TriggerComponentAccessible (pComponent->GetName ());
        }
    }
}


void CCEComponentManager::HandsOffComponents (Lifetime lifetime)
{
    POSITION pPosition;
    CString strName;
    LogicalComponent *pComponent;

    pPosition = m_Components.GetStartPosition ();
    while (pPosition != NULL)
    {
        m_Components.GetNextAssoc (pPosition, strName, pComponent);

        if (pComponent->GetLifetime () == lifetime && pComponent->IsCreated ())
        {
            m_pStateManager->TriggerHandsOffComponent (pComponent->GetName ());
        }
    }
}


void CCEComponentManager::DestroyComponents (Lifetime lifetime)
{
    POSITION pPosition;
    CString strName;
    LogicalComponent *pComponent;

    pPosition = m_Components.GetStartPosition ();
    while (pPosition != NULL)
    {
        m_Components.GetNextAssoc (pPosition, strName, pComponent);

        if (pComponent->GetLifetime () == lifetime)
        {
            DestroyComponent (pComponent);
        }
    }
}


HRESULT CCEComponentManager::CreateComponent (LogicalComponent *pComponent)
{
    HRESULT hr;
    LogicalComponent *pSavedAccessibleComponent;

    hr = pComponent->Create ();
    if (FAILED (hr))
    {
        CString strMessage;

        strMessage.Format(IDS_NO_COMPONENT, (LPCTSTR)pComponent->GetName ());
        UTIL_FatalError (strMessage, hr);
        return hr;
    }

    pSavedAccessibleComponent = m_pAccessibleComponent;
    m_pAccessibleComponent = pComponent;

    m_pStateManager->TriggerCreateComponent (pComponent->GetName ());

    m_pAccessibleComponent = pSavedAccessibleComponent;
    return S_OK;
}


void CCEComponentManager::DestroyComponent (LogicalComponent *pComponent)
{
    HRESULT hr;
    LogicalComponent *pSavedAccessibleComponent;

    pSavedAccessibleComponent = m_pAccessibleComponent;
    m_pAccessibleComponent = pComponent;

    m_pStateManager->TriggerDestroyComponent (pComponent->GetName ());

    m_pAccessibleComponent = pSavedAccessibleComponent;

    hr = pComponent->Destroy ();
    if (FAILED (hr))
    {
        // Message box
    }
}

void CCEComponentManager::Quit()
{
    if(m_pDefaultParent == NULL) {
        PostQuitMessage(1);
    }
    else{
        m_pDefaultParent->PostMessage(WM_CLOSE);
    }
}


HRESULT CCEComponentManager::COMComponent::Create ()
{
    HRESULT hr = S_OK;
    hr = CoCreateInstance (m_ClassId, NULL, CLSCTX_ALL, IID_IUnknown, (LPVOID *)&m_pUnknown);
    
    if(SUCCEEDED(hr)) 
    {
        Advise();
        AdviseAddonEvents();
    }
    return (hr);
}


HRESULT CCEComponentManager::COMComponent::Destroy ()
{
    UnAdvise();
    UnAdviseAddonEvents();

#ifdef _DEBUG
    if(m_pUnknown)
    {
        ULONG       ulRef;
        IUnknown*   pUnk = m_pUnknown;

        ulRef = pUnk->AddRef();
        ulRef = pUnk->Release();
        if(ulRef > 1)
        {
            TRACE("CCEComponentManager: Memory leak warning: %s not released (RefCount %d)\n", m_strName, ulRef);
        }
    }
#endif

    m_pUnknown = NULL;
    return S_OK;
}


//------------------------------------------------------------------*
/**
 * Advises an event sink object implementing the interface ICECompManEvents
 * to the COM object being created by component manager. The object being
 * created must not implement the interface ICECompManEvents as an
 * outgoing interface. Advise behaves tolerant if the query for 
 * ICECompManEvents returns E_NOINTERFACE. If the COM object implements
 * the interface it object is able to call back to the component manager
 * to retrieve or create other COM objects in turn. The interface 
 * Allows COM components to query for other COM components maintained
 * by the component manager residing in 4CE. The corresponding comp-
 * onents will be created on demand if they are not active. Asserts if
 * the COM object has not been created successfully and Advise is called
 * anyhow.
 *
 * @return          HRESULT - S_OK upon successful advise of the 
 *                  sink object respectively callback object<br>.
 * @return          E_FAIL  - Component manager has not successfully
 *                  created the event sink object.
 *
 * @see             UnAdvise(), AtlAdvise()
*/
HRESULT CCEComponentManager::COMComponent::Advise()
{
    HRESULT hr = S_OK;
    CCEComponentManager *pMgr = GetMe();

    if(!pMgr->m_pEvents) {
        return E_FAIL; //no events interface
    }

    COMP_MGR_ASSERT(m_pUnknown); //shall not be null
    COMP_MGR_ASSERT(m_dwAdvise == 0UL); //has to be 0
    m_dwAdvise = 0UL;

    hr = AtlAdvise(m_pUnknown,pMgr->m_pEvents->GetUnknown(),
        __uuidof(ICECompManEvents),&m_dwAdvise);
    //do not worry about advise failure

    return S_OK;
}


//------------------------------------------------------------------*
/**
 * UnAdvises an event sink object implementing the interface ICECompManEvents
 * with the COM object being created by component manager. The object being
 * created must not implement the interface ICECompManEvents as an
 * outgoing interface. UnAdvise() behaves tolerant if the event sink object
 * has not been advised successfully by a previous call to Advise(). 
 * If the COM object implements the interface it is able to call back 
 * to the component manager to retrieve or create other COM objects in turn. 
 * The interface allows COM components to query for other COM components 
 * maintained by the component manager residing in 4CE. The corresponding
 * components will be created on demand if they are not active.
 *
 * @return          HRESULT - S_OK upon successful unadvise of the 
 *                  sink object respectively callback object<br>.
 * @return          hr  - of AtlUnadvise()
 *
 * @see             Advise(), AtlUnadvise()
*/
HRESULT CCEComponentManager::COMComponent::UnAdvise()
{
    HRESULT hr = S_OK;

    if(m_dwAdvise) {
        //if it is advised be sensitive for errors.
        COMP_MGR_ASSERT(m_pUnknown);
        hr = AtlUnadvise(m_pUnknown,__uuidof(ICECompManEvents),m_dwAdvise);
        m_dwAdvise = 0UL;
    }

    return hr;
}


//------------------------------------------------------------------*
/**
 * Advises an event sink object implementing the interface ICEAddonActionEvents
 * to the COM object being created by component manager. The object being
 * created must not implement the interface ICEAddonActionEvents as an
 * outgoing interface. Advise behaves tolerant if the query for 
 * ICEAddonActionEvents returns E_NOINTERFACE. If the COM object implements
 * the interface it object is able to call back to the component manager
 * to retrieve or create other COM objects in turn. The interface 
 * Allows COM components to query for other COM components maintained
 * by the component manager residing in 4CE. The corresponding comp-
 * onents will be created on demand if they are not active. Asserts if
 * the COM object has not been created successfully and Advise is called
 * anyhow.
 *
 * @return          HRESULT - S_OK upon successful advise of the 
 *                  sink object respectively callback object<br>.
 * @return          E_FAIL  - Component manager has not successfully
 *                  created the event sink object.
 *
 * @see             UnAdvise(), AtlAdvise()
*/
HRESULT CCEComponentManager::COMComponent::AdviseAddonEvents()
{
    HRESULT hr = S_OK;
    CCEComponentManager *pMgr = GetMe();

    if(!pMgr->m_pAddonEvents) 
    {
        return E_FAIL; //no events interface
    }

    COMP_MGR_ASSERT(m_pUnknown); //shall not be null
    COMP_MGR_ASSERT(m_dwAddonAdvise == 0UL); //has to be 0
    COMP_MGR_ASSERT(m_dwAddonAdvise2 == 0UL);
    m_dwAddonAdvise = 0UL;
    m_dwAddonAdvise2 = 0UL;

    hr = AtlAdvise(m_pUnknown,pMgr->m_pAddonEvents->GetUnknown(),
        __uuidof(ICEAddOnActionEvents),&m_dwAddonAdvise);
    //do not worry about advise failure

    hr = AtlAdvise(m_pUnknown, pMgr->m_pAddonEvents->GetUnknown(),
        __uuidof(ICEAddOnActionEvents2), &m_dwAddonAdvise2);

    return S_OK;
}

//------------------------------------------------------------------*
/**
 * Unadvises an event sink object implementing the interface ICEAddonActionEvents
 * with the COM object being created by component manager. The object being
 * created must not implement the interface ICEAddonActionEvents as an
 * outgoing interface. UnAdvise() behaves tolerant if the event sink object
 * has not been advised successfully by a previous call to Advise(). 
 * If the COM object implements the interface it is able to call back 
 * to the component manager to retrieve or create other COM objects in turn. 
 * The interface allows COM components to query for other COM components 
 * maintained by the component manager residing in 4CE. The corresponding
 * components will be created on demand if they are not active.
 *
 * @return          HRESULT - S_OK upon successful unadvise of the 
 *                  sink object respectively callback object<br>.
 * @return          hr  - of AtlUnadvise()
 *
 * @see             Advise(), AtlUnadvise()
*/
HRESULT CCEComponentManager::COMComponent::UnAdviseAddonEvents()
{
    HRESULT hr = S_OK;

    if(m_dwAddonAdvise)
    {
        //if it is advised be sensitive for errors.
        COMP_MGR_ASSERT(m_pUnknown);
        hr = AtlUnadvise(m_pUnknown,__uuidof(ICEAddOnActionEvents),m_dwAddonAdvise);
        m_dwAddonAdvise = 0UL;
    }

    if(m_dwAddonAdvise2)
    {
        //if it is advised be sensitive for errors.
        COMP_MGR_ASSERT(m_pUnknown);
        hr = AtlUnadvise(m_pUnknown,__uuidof(ICEAddOnActionEvents2),m_dwAddonAdvise2);
        m_dwAddonAdvise2 = 0UL;
    }

    return hr;
}

//HRESULT CCEComponentManager::COMComponent::RegisterROT()
//{
//    CComPtr<IRunningObjectTable> pROT;
//    HRESULT hrRes = CreateMoniker(&m_pMoniker);
//
//    hrRes = ::GetRunningObjectTable(0UL,&pROT);
//    if(pROT && m_pMoniker) {
//        m_dwROT = 0UL;
//        hrRes = pROT->Register(ROTFLAGS_REGISTRATIONKEEPSALIVE,m_pUnknown,m_pMoniker,&m_dwROT);
//    }
//
//    pROT = NULL;
//    return hrRes;
//}

//void CCEComponentManager::COMComponent::UnRegisterROT()
//{
//    HRESULT hr = S_OK;
//    CComPtr<IRunningObjectTable> pROT;
//
//    if(m_dwROT) {
//        hr = ::GetRunningObjectTable(0UL,&pROT);
//        if(pROT) {
//            hr = pROT->Revoke(m_dwROT);
//        }
//    }
//
//    pROT = NULL;
//    m_pMoniker = NULL;
//}

//HRESULT CCEComponentManager::COMComponent::GetFromROT(IUnknown *& pUnk)
//{
//    CComPtr<IRunningObjectTable> pROT;
//    CComPtr<IMoniker>pMoniker;
//
//    pUnk = NULL;
//
//    HRESULT hr = ::GetRunningObjectTable(0UL,&pROT);
//    if(FAILED(hr)) {
//        return hr;
//    }
//    hr = CreateMoniker(&pMoniker);
//    if(FAILED(hr)) {
//        return hr;
//    }
//    if(!pROT) {
//        return E_FAIL;
//    }
//
//    hr = pROT->GetObject(pMoniker,&pUnk);
//    return hr;
//}

HRESULT CCEComponentManager::COMComponent::CreateMoniker(IMoniker** ppMoniker)
{
    DWORD dwId;
    CString str,strHelp;
    CComBSTR sGuid;

    USES_CONVERSION;
    
    COMP_MGR_ASSERT(ppMoniker != NULL);
    *ppMoniker = NULL;


    /**
    *   Build the moniker path with the following rule:
    *   PROCESS_ID\CLASSID of component
    */
    sGuid = m_ClassId; //does the appropriate conversion
    strHelp = sGuid; //assign it to the string

    dwId = GetCurrentProcessId(); 
    str.Format(_T("%lx\\%s"),dwId,strHelp);
    return ::CreateFileMoniker(T2COLE(str),ppMoniker);
}

bool CCEComponentManager::COMComponent::IsCreated ()
{
    return (m_pUnknown != NULL);
}

void CCEComponentManager::COMComponent::GetClassId(CString & str)
{
    CGuid guid;
    LPTSTR psz;

    str.Empty();
    guid = m_ClassId;
    psz = (LPTSTR)guid;
    COMP_MGR_ASSERT(psz);
    str = psz;
}

HRESULT CCEComponentManager::DockingToolComponent::Create ()
{
    CObject *pObject;

    COMP_MGR_ASSERT (m_pClass != NULL);
    if (m_pClass == NULL)
    {
        return (E_FAIL);
    }

    pObject = m_pClass->CreateObject ();
    if (pObject == NULL)
    {
        return (E_OUTOFMEMORY);
    }

    m_pControlBar = dynamic_cast<SECControlBar*> (pObject);
    if (m_pControlBar == NULL)
    {
        delete pObject;
        return (E_CECOMPMAN_INVALID_DOCKINGTOOL_CLASS);
    }

    return (S_OK);
}


HRESULT CCEComponentManager::DockingToolComponent::Destroy ()
{
    COMP_MGR_ASSERT (m_pControlBar != NULL);
    if (m_pControlBar == NULL)
    {
        return (E_FAIL);
    }

    m_pControlBar->DestroyWindow ();
    delete m_pControlBar;
    m_pControlBar = NULL;
    return (S_OK);
}


bool CCEComponentManager::DockingToolComponent::IsCreated ()
{
    return (m_pControlBar != NULL);
}


HRESULT CCEComponentManager::SimpleDockingToolComponent::Create ()
{
    HRESULT hr;

    hr = DockingToolComponent::Create ();
    if (FAILED (hr))
    {
        return (hr);
    }

    COMP_MGR_ASSERT (m_pControlBar != NULL);
    if (m_pControlBar == NULL)
    {
        return (E_FAIL);
    }

    if (!m_pControlBar->Create (m_pParent, m_strTitle, m_dwStyle, m_dwExStyle, m_uiId, NULL)) {
        // parent window, window name, style , ext. style, id, create context
        DockingToolComponent::Destroy ();
        return (E_CECOMPMAN_FAILED_TO_CREATE_DOCKINGTOOL_WINDOW);
    }

    // LifetimeFrame begins before call of main frames LoadFrame
    // method which will send WM_INITIALUPDATE to all descendants.
    // If we create the docking tool later we have to do this
    // manually.

    if (m_Lifetime != LifetimeFrame)
    {
        m_pControlBar->SendMessage (WM_INITIALUPDATE);
        m_pControlBar->SendMessageToDescendants (WM_INITIALUPDATE);
    }

    return (S_OK);
}


HRESULT CCEComponentManager::DialogDockingToolComponent::Create ()
{
    HRESULT hr;
    SECDialogBar* pDialogBar;

    hr = DockingToolComponent::Create ();
    if (FAILED (hr))
    {
        return (hr);
    }

    COMP_MGR_ASSERT (m_pControlBar != NULL);
    if (m_pControlBar == NULL)
    {
        return (E_FAIL);
    }

    pDialogBar = dynamic_cast<SECDialogBar*>(m_pControlBar);
    COMP_MGR_ASSERT (pDialogBar != NULL);
    if (pDialogBar == NULL)
    {
        return (E_FAIL);
    }

    
    if (!pDialogBar->Create (m_pParent, m_uiIDD, m_dwStyle, m_dwExStyle, m_uiId)){
        // parent window, dialog template id, style, ext. style, id
        DockingToolComponent::Destroy ();
        return (E_CECOMPMAN_FAILED_TO_CREATE_DOCKINGTOOL_WINDOW);
    }

    // LifetimeFrame begins before call of main frames LoadFrame
    // method which will send WM_INITIALUPDATE to all descendants.
    // If we create the docking tool later we have to do this
    // manually.

    if (m_Lifetime != LifetimeFrame)
    {
        m_pControlBar->SendMessage (WM_INITIALUPDATE);
        m_pControlBar->SendMessageToDescendants (WM_INITIALUPDATE);
    }

    return (S_OK);
}


HRESULT CCEComponentManager::TargetSetComponent::Create ()
{
    m_pTargetSet = new CTargetSet;
    if (m_pTargetSet == NULL)
    {
        return (E_OUTOFMEMORY);
    }

    return m_pTargetSet->LoadFromKAD();
}



HRESULT CCEComponentManager::TargetSetComponent::Destroy ()
{
    delete m_pTargetSet;
    m_pTargetSet = NULL;
    return (S_OK);
}


bool CCEComponentManager::TargetSetComponent::IsCreated ()
{
    return (m_pTargetSet != NULL);
}

//------------------------------------------------------------------*
/**
 * Uses the KAD manager to iterate over all entries in the KAD FILES
 * section and to add all parser components to the component manager.
 *
 * @param           pKAD - IUnknown reference to the KAD managers
 *                  root object
 *
 * @exception       _com_error(E_NOINTERFACE) if one of the interfaces
 *                  IFiles or IEnumFiles could not retrieved from KAD manager.<br>
 *                  _com_error(hr) from the call to IEnumFiles::Next if call
 *                  fails.<br>
 *                  _com_error(hr) if an fatal error to AddCOM() occurred.
 * 
 * @see             AddCOM(), class _com_error, _com_raise_error()
*/
void CCEComponentManager::AddFromFilesSection(IUnknown *pKAD)
{
    HRESULT hr = S_OK;
    HRESULT hrEnum = S_OK;
    CComQIPtr<IFiles> pFiles;
    CComPtr<IEnumFiles> pEnum;
    IFile* arr[COMP_MGR_NEXT_ARRAY_SIZE];
    CGuid guid;
    bool bFail;
    ULONG ulFetched;
    CString str;

    pFiles = pKAD;
    FC_PCHECK_THROW(pFiles,_com_error(E_NOINTERFACE));

    hr = pFiles->GetFileEnumerator(&pEnum);
    switch(hr) {
        case S_OK:
            break;
        default: //TODO evaluate error codes: no KAD, node not found
            return;
    }

    FC_PCHECK_THROW(pEnum,_com_error(E_POINTER));

    do {
        hrEnum = pEnum->Next(FC_ARRAY_LEN(arr),arr,&ulFetched);
        if(FAILED(hrEnum)) {
            _com_raise_error(hrEnum);
        }

        if(hrEnum == S_OK) {
            ulFetched = FC_ARRAY_LEN(arr);
        }

        bFail = false;
        for(ULONG ul = 0; ul < ulFetched; ul++) {
            if(!bFail) {
                hr = arr[ul]->GetParser(guid);
                switch(hr) {
                    case S_OK:
                        str = (LPCTSTR) guid;
                        hr = AddCOM(str,LifetimeProject,guid,true);
                        if(hr == E_OUTOFMEMORY) {
                            bFail = true;
                        }
                        guid.Empty();
                        break;
                    case S_FALSE:
                        break;
                    default:
                        bFail = true;
                        break;
                }
            }
            FC_RELEASE_PTR(arr[ul]);
        }

        if(bFail) {
            _com_raise_error(hr);
        }

    }
    while(hrEnum == S_OK);
}

//------------------------------------------------------------------*
/**
 * Uses the KAD manager to iterate over all entries in the KAD MAINMENU
 * section and to add all handler components to the component manager.
 *
 * @param           pKAD - IUnknown reference to the KAD managers
 *                  root object
 *
 * @exception       _com_error(E_NOINTERFACE) if one of the interfaces
 *                  IMainMenu or IEnumMenus could not retrieved from KAD manager.<br>
 *                  _com_error(hr) from the call to IEnumMenus::Next() if call
 *                  fails.<br>
 *                  _com_error(hr) if an fatal error to AddCOM() occurred.
 * 
 * @see             AddCOM(), class _com_error, _com_raise_error(), 
 *                  IEnumMenus::Next()
*/
void CCEComponentManager::AddFromMainMenuSection(IUnknown *pKAD)
{
    HRESULT hr = S_OK;
    HRESULT hrEnum = S_OK;
    CComQIPtr<IMainMenu> pMM;
    CComPtr<IEnumMenus> pEnum;
    IMenu* arr[COMP_MGR_NEXT_ARRAY_SIZE];
    CGuid guid;
    bool bFail;
    ULONG ulFetched;
    CString str;

    pMM = pKAD;
    FC_PCHECK_THROW(pMM,_com_error(E_NOINTERFACE));

    hr = pMM->GetMenuEnumerator(&pEnum);
    switch(hr) {
        case S_OK:
            break;
        default: //TODO evaluate error codes: no KAD, node not found
            return;
    }

    FC_PCHECK_THROW(pEnum,_com_error(E_POINTER));
    
    do {
        hrEnum = pEnum->Next(FC_ARRAY_LEN(arr),arr,&ulFetched);
        if(FAILED(hrEnum)) {
            _com_raise_error(hrEnum);
        }

        if(hrEnum == S_OK) {
            ulFetched = FC_ARRAY_LEN(arr);
        }

        bFail = false;
        for(ULONG ul = 0; ul < ulFetched; ul++) {
            if(!bFail) {
                //get the global handler if any present
                hr = arr[ul]->GetHandler(guid);
                switch(hr) {
                    case S_OK:
                        str = (LPCTSTR) guid;
                        hr = AddCOM(str,LifetimeProject,guid,true);
                        if(hr == E_OUTOFMEMORY) {
                            bFail = true;
                        }
                        guid.Empty();
                        break;
                    case S_FALSE:
                        break;
                    default:
                        bFail = true;
                        break;
                }

                if(!bFail) {
                    //get the menu items and associated handlers
                    hr = AddMenuItems(arr[ul]);
                }
            }
            FC_RELEASE_PTR(arr[ul]);
        }

        if(bFail) {
            _com_raise_error(hr);
        }

    }
    while(hrEnum == S_OK);
}


//------------------------------------------------------------------*
/**
 * Uses the KAD manager to iterate over all menu item entries in the KAD MENU
 * section identified by the passed IMenu interface and to add all handler 
 * components to the component manager.
 *
 * @param           pMenu - IMenu reference to the KAD managers
 *                  menu object.
 *
 * @return          E_POINTER if parameter pMenu is NULL.<br>
 *                  hr of IMenu::GetMenuItemEnumerator() if function call fails.<br>
 *                  E_POINTER if returned IEnumMenuItems enumerator is NULL.<br>
 *                  HRESULT from the call to IEnumMenuItems::Next() if call
 *                  fails.<br>
 *                  HRESULT if an fatal error to AddCOM() occurred.
 * 
 * @see             AddCOM(), IEnumMenuItems::Next()
*/
HRESULT CCEComponentManager::AddMenuItems(IMenu *pMenu)
{
    HRESULT hr = S_OK,hrEnum = S_OK;
    CComPtr<IEnumMenuItems> pEnum;
    IMenuItem* arr[COMP_MGR_NEXT_ARRAY_SIZE];
    CGuid guid;
    bool bFail;
    ULONG ulFetched;
    CString str;
    
    COMP_MGR_ASSERT(pMenu);
    FC_PCHECK_HR(pMenu,E_POINTER);

    hr = pMenu->GetMenuItemEnumerator(&pEnum);
    if(FAILED(hr) || !pEnum) {
        //no enumerator - return silently
        return S_OK;
    }

    do {
        hrEnum = pEnum->Next(FC_ARRAY_LEN(arr),arr,&ulFetched);
        if(FAILED(hrEnum)) {
            return hrEnum;
        }

        if(hrEnum == S_OK) {
            ulFetched = FC_ARRAY_LEN(arr);
        }

        bFail = false;
        for(ULONG ul = 0; ul < ulFetched; ul++) {
            if(!bFail) {
                hr = arr[ul]->GetHandler(guid);
                switch(hr) {
                    case S_OK:
                        str = (LPCTSTR) guid;
                        hr = AddCOM(str,LifetimeProject,guid,true);
                        if(hr == E_OUTOFMEMORY) {
                            bFail = true;
                        }
                        guid.Empty();
                        break;
                    case S_FALSE:
                        break;
                    default:
                        bFail = true;
                        break;
                }
            }
            FC_RELEASE_PTR(arr[ul]);
        }

        if(bFail) {
            return hr;
        }

    }
    while(hrEnum == S_OK);
    return S_OK;
}

// SIS, 08.05.01: commented out, no handlers any more
//------------------------------------------------------------------*
/**
 * Uses the KAD manager to iterate over all entries in the KAD TOOLBARS
 * section and to add all handler components to the component manager.
 *
 * @param           pKAD - IUnknown reference to the KAD managers
 *                  root object
 *
 * @exception       _com_error(E_NOINTERFACE/E_POINTER) if one of the interfaces
 *                  IToolbars or IEnumToolbars could not retrieved from KAD manager.<br>
 *                  _com_error(hr) from the call to IEnumToolbars::Next() if call
 *                  fails.<br>
 *                  _com_error(hr) if an fatal error to AddCOM() occurred.<br>
 * 
 * @see             AddCOM(), class _com_error, _com_raise_error(), 
 *                  IEnumToolbars::Next(), AddToolbarItems()
*/
//void CCEComponentManager::AddFromToolbarsSection(IUnknown *pKAD)
//{
//    HRESULT hr = S_OK;
//    HRESULT hrEnum = S_OK;
//    CComQIPtr<IToolbars> pTB;
//    CComPtr<IEnumToolbars> pEnum;
//    IToolbar* arr[COMP_MGR_NEXT_ARRAY_SIZE];
//    CGuid guid;
//    bool bFail;
//    ULONG ulFetched;
//    CString str;
//
//    pTB = pKAD;
//    FC_PCHECK_THROW(pTB,_com_error(E_NOINTERFACE));
//
//    hr = pTB->GetToolbarEnumerator(&pEnum);
//    switch(hr) {
//        case S_OK:
//            break;
//        default: //TODO evaluate error codes: no KAD, node not found
//            return;
//    }
//
//    FC_PCHECK_THROW(pEnum,_com_error(E_POINTER));
//
//    do {
//        hrEnum = pEnum->Next(FC_ARRAY_LEN(arr),arr,&ulFetched);
//        if(FAILED(hrEnum)) {
//            _com_raise_error(hrEnum);
//        }
//
//        if(hrEnum == S_OK) {
//            ulFetched = FC_ARRAY_LEN(arr);
//        }
//
//        bFail = false;
//        for(ULONG ul = 0; ul < ulFetched; ul++) {
//            if(!bFail) {
//                //get the global handler if any present
//                hr = arr[ul]->GetHandler(guid);
//                switch(hr) {
//                    case S_OK:
//                        str = (LPCTSTR) guid;
//                        hr = AddCOM(str,LifetimeProject,guid,true);
//                        if(hr == E_OUTOFMEMORY) {
//                            bFail = true;
//                        }
//                        guid.Empty();
//                        break;
//                    case S_FALSE:
//                        break;
//                    default:
//                        bFail = true;
//                        break;
//                }
//
//                if(!bFail) {
//                    //get the menu items and associated handlers
//                    hr = AddToolbarItems(arr[ul]);
//                }
//            }
//            FC_RELEASE_PTR(arr[ul]);
//        }
//
//        if(bFail) {
//            _com_raise_error(hr);
//        }
//
//    }
//    while(hrEnum == S_OK);
//}


// SIS, 08.05.01: commented out, no handlers any more
//------------------------------------------------------------------*
/**
 * Uses the KAD manager to iterate over all toolbar item entries in the KAD TOOLBAR
 * section identified by the passed IToolbar interface and to add all handler 
 * components to the component manager.
 *
 * @param           pMenu - IToolbar reference to the KAD managers
 *                  toolbar object.
 *
 * @return          E_POINTER if parameter pToolbar is NULL.<br>
 *                  hr of IToolbar::GetToolbarItemEnumerator() if function call fails.<br>
 *                  E_POINTER if returned IEnumToolbarItems enumerator is NULL.<br>
 *                  HRESULT from the call to IEnumToolbarItems::Next() if call
 *                  fails.<br>
 *                  HRESULT if an fatal error to AddCOM() occurred.
 * 
 * @see             AddCOM(), IEnumToolbarItems::Next()
*/
//HRESULT CCEComponentManager::AddToolbarItems(IToolbar* pToolbar)
//{
//    HRESULT hr = S_OK,hrEnum = S_OK;
//    CComPtr<IEnumToolbarItems> pEnum;
//    IToolbarItem* arr[COMP_MGR_NEXT_ARRAY_SIZE];
//    CGuid guid;
//    bool bFail;
//    ULONG ulFetched;
//    CString str;
//    
//    COMP_MGR_ASSERT(pToolbar);
//    FC_PCHECK_HR(pToolbar,E_POINTER);
//
//    hr = pToolbar->GetToolbarItemEnumerator(&pEnum);
//    if(FAILED(hr) || !pEnum) {
//        //no enumerator - return silently
//        return S_OK;
//    }
//
//    do {
//        hrEnum = pEnum->Next(FC_ARRAY_LEN(arr),arr,&ulFetched);
//        if(FAILED(hrEnum)) {
//            return hrEnum;
//        }
//
//        if(hrEnum == S_OK) {
//            ulFetched = FC_ARRAY_LEN(arr);
//        }
//
//        bFail = false;
//        for(ULONG ul = 0; ul < ulFetched; ul++) {
//            if(!bFail) {
//                hr = arr[ul]->GetHandler(guid);
//                switch(hr) {
//                    case S_OK:
//                        str = (LPCTSTR) guid;
//                        hr = AddCOM(str,LifetimeProject,guid,true);
//                        if(hr == E_OUTOFMEMORY) {
//                            bFail = true;
//                        }
//                        guid.Empty();
//                        break;
//                    case S_FALSE:
//                        break;
//                    default:
//                        bFail = true;
//                        break;
//                }
//            }
//            FC_RELEASE_PTR(arr[ul]);
//        }
//
//        if(bFail) {
//            return hr;
//        }
//
//    }
//    while(hrEnum == S_OK);
//    return S_OK;
//}

//------------------------------------------------------------------*
/**
 * Uses the KAD manager to iterate over all entries in the KAD TYPES
 * section in order to add all menu item related handler components 
 * to the component manager. Queries for the associated MENU entry of
 * an TYPE entry and calls AddMenuItems() to add all handlers to the 
 * component manager.
 *
 * @param           pKAD - IUnknown reference to the KAD managers
 *                  root object
 *
 * @exception       _com_error(E_NOINTERFACE/E_POINTER) if one of the interfaces
 *                  ITypes or IEnumTypes could not retrieved from KAD manager.<br>
 *                  _com_error(hr) from the call to IEnumTypes::Next() if call
 *                  fails.<br>
 *                  _com_error(hr) if an fatal error to AddCOM() occurred.
 * 
 * @see             AddCOM(), class _com_error, _com_raise_error(), 
 *                  IEnumTypes::Next(), AddMenuItems()
*/
void CCEComponentManager::AddFromTypesSection(IUnknown* pKAD) //throws _com_error exception
{
    HRESULT hr = S_OK;
    HRESULT hrEnum = S_OK;
    CComQIPtr<ITypes> pT;
    CComPtr<IEnumTypes> pEnum;
    CComPtr<IMenu> pMenu;
    IType* arr[COMP_MGR_NEXT_ARRAY_SIZE];
    CGuid guid;
    bool bFail;
    ULONG ulFetched;
    CString str;

    pT = pKAD;
    FC_PCHECK_THROW(pT,_com_error(E_NOINTERFACE));

    hr = pT->GetTypeEnumerator(&pEnum);
    switch(hr) {
        case S_OK:
            break;
        default: //TODO evaluate error codes: no KAD, node not found
            return;
    }

    FC_PCHECK_THROW(pEnum,_com_error(E_POINTER));

    do {
        hrEnum = pEnum->Next(FC_ARRAY_LEN(arr),arr,&ulFetched);
        if(FAILED(hrEnum)) {
            _com_raise_error(hrEnum);
        }

        if(hrEnum == S_OK) {
            ulFetched = FC_ARRAY_LEN(arr);
        }

        bFail = false;
        for(ULONG ul = 0; ul < ulFetched; ul++) {
            if(!bFail) {
                //get the global handler if any present
                pMenu = NULL;
                arr[ul]->GetMenu(&pMenu);

                if(pMenu) {
                    hr = pMenu->GetHandler(guid);
                    switch(hr) {
                        case S_OK:
                            str = (LPCTSTR) guid;
                            hr = AddCOM(str,LifetimeProject,guid,true);
                            if(hr == E_OUTOFMEMORY) {
                                bFail = true;
                            }
                            guid.Empty();
                            break;
                        case S_FALSE:
                            break;
                        default:
                            bFail = true;
                            break;
                    }
                    hr = AddMenuItems(pMenu);
                }
            }
            FC_RELEASE_PTR(arr[ul]);
        }

        if(bFail) {
            _com_raise_error(hr);
        }

    }
    while(hrEnum == S_OK);
}

//------------------------------------------------------------------*
/**
 * Get a list of all COM components. Allocates and returns a simple array
 * of all COM components registered with the component manager. The
 * returned array should be deleted using FreeCOMComponentList(). The
 * COMComponentEntry includes IUnknown reference, CLSID and name of 
 * the component. If the IUnknown reference is null the component has
 * been registered with one of the functions AddCOM(), 
 * AddSimpleDockingTool(), AddDialogDockingTool() but is not created yet.
 * 
 * @param           reference to COMComponentEntry* - the allocated array of
 *                  COMComponentEntry entries will be returned with this parameter
 * @param           iCount - number of entries in the array for iteration.
 * @return          E_OUTOFMEMORY  - array could not be created due to an
 *                  out of memory situation.
 * @see             FreeCOMComponentList()
*/
HRESULT CCEComponentManager::GetCOMComponentList(COMComponentEntry* & pList,int & iCount)
{
    POSITION pPosition;
    int i=0;
    
    iCount = m_CLSIDMap.GetCount();

    pList = NULL;
    pList = new COMComponentEntry[iCount];
    if(!pList) {
        return E_OUTOFMEMORY;
    }

    pPosition = m_CLSIDMap.GetStartPosition ();
    while (pPosition != NULL)
    {
        CString strCLSID;
        COMComponent *pComponent;

        m_CLSIDMap.GetNextAssoc (pPosition, strCLSID, pComponent);
        pList[i].m_pUnk = pComponent->GetInterface();
        if(pList[i].m_pUnk) {
            pList[i].m_pUnk->AddRef();
        }
        pList[i].m_clsid = pComponent->GetClassId();
        pList[i].m_strName = pComponent->GetName();
        i++;
    }
    return S_OK;   
}

//------------------------------------------------------------------*
/**
 * Free the list of all COM components. Frees the array of 
 * COMComponentEntry entries allocated with GetCOMComponentList().
 * Always use this method to free the array returned with GetCOMComponentList().
 * 
 * @param           COMComponentEntry* - the array COMComponentEntry entries.
 * @param           iCount - number of entries in the array.
 * @see             GetCOMComponentList()
*/
void CCEComponentManager::FreeCOMComponentList(COMComponentEntry * pList,int iCount)
{
    for(int i = 0;i< iCount;i++) {
        if(pList[i].m_pUnk) {
            pList[i].m_pUnk->Release();
            pList[i].m_pUnk = NULL;
        }
    }

    delete [] pList;
}

//------------------------------------------------------------------*
/**
 * OnComponentByCLSID returns the component identified by the class
 * id of the component. A strong reference will be returned with
 * the IUnknown pointer. If no COM component with the class id is registered
 * with the component manager, an error will be returned. If the COM
 * component with the class id is registered but not activated 
 * the activation will be performed.
 *
 * @param       sId - unique identifier to identify the component.
 * @param       ppUnk - reference to an IUnknown pointer receiving
 *              the default interface pointer of the component. Will
 *              be set to NULL in case of an error.
 *
 * @see			OnComponentById()			
*/
STDMETHODIMP CCEComponentManagerEvents::OnComponentByCLSID(BSTR sCLSID, IUnknown ** ppUnk)
{
    CCEComponentManager *pComponentManager = NULL;
    HRESULT hr = S_OK;

    AFX_MANAGE_STATE (AfxGetAppModuleState());
    try {
        CString str;
        CGuid guid;
        FC_PCHECK_THROW(ppUnk,_com_error(E_POINTER));
        *ppUnk = NULL;

        pComponentManager = CEGetComponentManager ();
        //TODO error code 
        FC_PCHECK_THROW(pComponentManager,_com_error(E_POINTER));

        guid = sCLSID;
        str = (LPCTSTR)guid;
        hr = pComponentManager->GetCOMByCLSID(str,*ppUnk,true);
    }
    catch(CMemoryException* e) {
        e->Delete();
        return E_OUTOFMEMORY;
    }
    catch(_com_error err) {
        hr = err.Error();
    }
    return hr;
}

//------------------------------------------------------------------*
/**
 * OnComponentById returns the component identified by the unique
 * identifier string sId. A strong reference will be returned with
 * the IUnknown pointer. If no COM component with the id is registered
 * with the component manager, an error will be returned. If the COM
 * component with the id is registered but not activated the activation
 * will be performed.
 *
 * @param       sId - unique identifier to identify the component.
 * @param       ppUnk - reference to an IUnknown pointer receiving
 *              the default interface pointer of the component. Will
 *              be set to NULL in case of an error.
 *
 * @return		
 *
 * @see			OnComponentByCLSID()	
*/
STDMETHODIMP CCEComponentManagerEvents::OnComponentById(BSTR sId, IUnknown ** ppUnk)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState());
    try {
        FC_PCHECK_THROW(ppUnk,_com_error(E_POINTER));
        *ppUnk = NULL;
        
        CString str = sId;
        *ppUnk = CEGetCOM(str);
    }
    catch(CMemoryException* e) {
        e->Delete();
        return E_OUTOFMEMORY;
    }
    catch(_com_error err) {
        return err.Error();
    }
    return S_OK;
}

//------------------------------------------------------------------*
/**
 * Returns the reference respectively pointer to the one and only 
 * instance of the component manager. Do not create more than one
 * instance of the component manager. Asserts if the component manager
 * has not been created at all (e.g.) the component manager reference
 * is NULL.
 *
 * @return          Reference resp. pointer to the component manager<br>
 *                  NULL if component manager has not been created yet.
 *  
 * @see             description of class CCEComponentManager
*/
CCEComponentManager *CEGetComponentManager ()
{
    CCEComponentManager *pComponentManager;

    pComponentManager = CCEComponentManager::GetMe ();
    COMP_MGR_ASSERT (pComponentManager != NULL);
    return (pComponentManager);
}

//------------------------------------------------------------------*
/**
 * Gets a COM component by the unique identifier. The component has
 * to be added to the component manager by the function AddCOM() prior
 * to calling this function. In the event that the component has been
 * added with the bCreateOnDemand flag set to true the component will
 * be created with the first call to this function. Returns a strong
 * reference with the IUnknown ref.
 *
 * @param           strName - unique identifier to identify the component.
 * @param           bMustLive - true, creation will be enforced if create
 *                  on demand flag has been set. Otherwise creation is
 *                  determined by life time enumation.
 * @return          IUnknown reference or NULL in case of an error. Returns
 *                  a strong reference with the IUnknown pointer. Hence a 
 *                  call to IUnknown::Release() is mandatory to release the
 *                  reference.
 * @see             AddCOM(), IUnknown::Release()          
*/
IUnknown *CEGetCOM (const CString &strName, bool bMustLive)
{
    CCEComponentManager *pComponentManager;
    HRESULT hr;
    IUnknown *pUnknown;

    pComponentManager = CEGetComponentManager ();
    if (pComponentManager == NULL) {
        return NULL;
    }

    hr = pComponentManager->GetCOM (strName, pUnknown,bMustLive);
    if (!bMustLive && hr == E_CECOMPMAN_COMPONENT_NOT_ALIVE){
        //must live is false but component not alive
        return NULL;
    }
    
    if (FAILED (hr)) {
        return NULL;
    }

    return pUnknown;
}

//------------------------------------------------------------------*
/**
 * Gets a COM component by the CLSID. The component has
 * to be added to the component manager by the function AddCOM() prior
 * to calling this function. In the event that the component has been
 * added with the bCreateOnDemand flag set to true the component will
 * be created with the first call to this function if bMustLive has
 * been set. Returns a strong reference with the IUnknown ref.
 *
 * @param           strGuid - CLSID to identify the component.
 * @param           bMustLive - true, creation will be enforced if create
 *                  on demand flag has been set. Otherwise creation is
 *                  determined by life time enumation.
 * @return          pUnknown - IUnknown pointer receiving
 *                  the default interface pointer of the component. Will
 *                  be set to NULL if component has not been created or 
 *                  has not been registered with AddCOM().
 * @see             AddCOM()            
*/
IUnknown *CEGetCOMByCLSID(const CString & strCLSID, bool bMustLive)
{
    CCEComponentManager *pComponentManager;
    HRESULT hr;
    IUnknown *pUnknown;

    pComponentManager = CEGetComponentManager ();
    if (pComponentManager == NULL)
    {
        return (NULL);
    }

    hr = pComponentManager->GetCOMByCLSID(strCLSID,pUnknown,bMustLive);
    if (!bMustLive && hr == E_CECOMPMAN_COMPONENT_NOT_ALIVE) {
        //must live is false but component not alive
        return NULL;
    }
    
    if (FAILED (hr)) {
        return NULL;
    }

    return pUnknown;
}


//------------------------------------------------------------------*
/**
 * Gets a docking tool by the unique identifier. 
 * The component has to be added to the component manager by the function 
 * AddSimpleDockingTool() or AddDialogDockingTool() prior
 * to calling this function. Returns a pointer to the SECControlBar
 * super class or NULL if not existing.
 * If the asked docking tool is not existing, and bMustLive is false
 * this is not regarded to be an error.
 *
 * @param           strName - unique identifier to identify the component.
 *                  bMustLive: 
 * @return          SECControlBar reference or NULL in case of an error.<br>
 *                  Possible error cases are: Docking tool has not been
 *                  added to the component manager using the methods 
 *                  mentioned above or Docking tool is not created resp.
 *                  alive due to another life time.
 * @see             AddSimpleDockingTool(), AddDialogDockingTool() 
*/
SECControlBar *CEGetDockingTool (const CString &strName, bool bMustLive)
{
    CCEComponentManager *pComponentManager;
    HRESULT hr;
    SECControlBar *pDockingTool = NULL;

    pComponentManager = CEGetComponentManager ();
    if (pComponentManager == NULL)
    {
        return (NULL);
    }

    hr = pComponentManager->GetDockingTool (strName, pDockingTool);
    if (bMustLive)
    {
        COMP_MGR_ASSERT (SUCCEEDED (hr));
    }
    if (FAILED (hr))
    {
        return (NULL);
    }

    return (pDockingTool);
}


//------------------------------------------------------------------*
/**
 * Returns true if the component identified by the argument strName
 * is existing and is a docking tool component. Otherwise returns
 * false.
 * @param			const CString & strName - identifier of component
 * @return			bool - true - component exists and is a docking
 *                  tool component, false otherwise.
 * @see			    CEGetComponentManager()
*/
bool CEIsDockingTool(const CString & strName)
{
    CCEComponentManager *pComponentManager;
    SECControlBar *pDockingTool = NULL;

    pComponentManager = CEGetComponentManager ();
    if (pComponentManager == NULL)
    {
        return false;
    }

    return pComponentManager->IsDockingTool (strName);
}


CTargetSet *CEGetTargetSet (const CString &strName, bool bMustLive)
{
    CCEComponentManager *pComponentManager;
    HRESULT hr;
    CTargetSet *pTargetSet;

    pComponentManager = CEGetComponentManager ();
    if (pComponentManager == NULL)
    {
        return (NULL);
    }

    hr = pComponentManager->GetTargetSet (strName, pTargetSet);
    if (!bMustLive && hr == E_CECOMPMAN_COMPONENT_NOT_ALIVE)
    {
        return (NULL);
    }
    COMP_MGR_ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        return (NULL);
    }

    return (pTargetSet);
}


//------------------------------------------------------------------*
/**
 * Issues a system message to the system tab of the message view
 * component. Returns true if the message referred by argument
 * pzsMessage has been added successfully to the message view.
 * Returns false if the message view does not exist in the scope
 * of the component manager or if the retrieved component is not
 * of the type of the message view component.
 * @param			LPCTSTR pszMessage - message to issue to the
 *                  system tab of the message view component.
 * @return			bool - see above.
*/
bool CESystemMessage(LPCTSTR pszMessage)
{
    CMsgWnd * pWnd = NULL;
    SECControlBar *pBar = CEGetDockingTool (CE_COMPMAN_MSGVIEW, false);
    if(!pBar) {
        //sorry message view window not created
        return false;
    }

    pWnd = dynamic_cast<CMsgWnd*>(pBar);
    if(!pWnd) {
        //no message window - assert it
        COMP_MGR_ASSERT(pWnd);
        return false;
    }

    pWnd->AddMessage (_T (""), E_FACILITY_CESYSTEM, pszMessage);
    return true;
}


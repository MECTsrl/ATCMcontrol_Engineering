/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/FCSession.h 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: FCSession.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/FCSession.h $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CSC
 *
 * =COMPONENT           CSC Online Control
 *
 * =CURRENT      $Date: 28.02.07 19:00 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *





 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  15.03.2001  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/
#ifndef _FCSESSION_H_
#define _FCSESSION_H_

//----  Aggregate Includes:   --------------------------------------*
#include "CSC_Online.h"     // interfaces
#include "resource.h"       // main symbols
#include "CpSessionNotification.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*
//----  Typedefs:    -----------------------------------------------*
typedef CMap<CString, LPCTSTR, IUnknown*, IUnknown*&>   sessionNameMap;
typedef CMap<DWORD, DWORD, IUnknown*, IUnknown*&>       sessionCookieMap;

#define _SESSION_FULL_ACTIVEX_CTRL

//------------------------------------------------------------------*
/**
 *  class CFCSessions
 *  collection of sessions to 4C Control
 *
 *  @see    ....
*/
class ATL_NO_VTABLE CFCSessions : 
	public CComObjectRootEx<CComGlobalsThreadModel>,
	public CComCoClass<CFCSessions, &CLSID_FCSessions>,
	public CComControl<CFCSessions>,
	public IDispatchImpl<IFCSessions, &IID_IFCSessions, &LIBID_CSC_ONLINELib>,
    public IProvideClassInfoImpl<&CLSID_FCSessions, &LIBID_CSC_ONLINELib>,

#ifdef _SESSION_FULL_ACTIVEX_CTRL
    public IPersistStreamInitImpl<CFCSessions>,
	public IOleControlImpl<CFCSessions>,
	public IOleObjectImpl<CFCSessions>,
	public IOleInPlaceActiveObjectImpl<CFCSessions>,
	public IViewObjectExImpl<CFCSessions>,
//	public ISupportErrorInfo,
	public IOleInPlaceObjectWindowlessImpl<CFCSessions>,
	public IObjectWithSiteImpl<CFCSessions>,
	public IPersistStorageImpl<CFCSessions>,
	public IQuickActivateImpl<CFCSessions>,
	public IDataObjectImpl<CFCSessions>,
	public ISpecifyPropertyPagesImpl<CFCSessions>,
#endif // _SESSION_FULL_ACTIVEX_CTRL
    public IObjectSafetyImpl<CFCSessions, INTERFACESAFE_FOR_UNTRUSTED_CALLER>
{
public:
//--------------------------
// =Constructor
	                        CFCSessions();
	                        ~CFCSessions();

DECLARE_CLASSFACTORY_SINGLETON(CFCSessions)
DECLARE_REGISTRY_RESOURCEID(IDR_FCSESSION)

//--------------------------
// =Interface map
BEGIN_COM_MAP(CFCSessions)
	COM_INTERFACE_ENTRY(IFCSessions)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
    COM_INTERFACE_ENTRY(IObjectSafety)

#ifdef _SESSION_FULL_ACTIVEX_CTRL
	COM_INTERFACE_ENTRY_IMPL(IObjectWithSite)
	COM_INTERFACE_ENTRY_IMPL(IViewObjectEx)
	COM_INTERFACE_ENTRY_IMPL_IID(IID_IViewObject2, IViewObjectEx)
	COM_INTERFACE_ENTRY_IMPL_IID(IID_IViewObject, IViewObjectEx)
	COM_INTERFACE_ENTRY_IMPL(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY_IMPL_IID(IID_IOleInPlaceObject, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY_IMPL_IID(IID_IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY_IMPL(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY_IMPL(IOleControl)
	COM_INTERFACE_ENTRY_IMPL(IOleObject)
	COM_INTERFACE_ENTRY_IMPL(IQuickActivate)
	COM_INTERFACE_ENTRY_IMPL(IPersistStorage)
	COM_INTERFACE_ENTRY_IMPL(IPersistStreamInit)
	COM_INTERFACE_ENTRY_IMPL(ISpecifyPropertyPages)
	COM_INTERFACE_ENTRY_IMPL(IDataObject)
//	COM_INTERFACE_ENTRY(ISupportErrorInfo)
#endif //_SESSION_FULL_ACTIVEX_CTRL
END_COM_MAP()

#ifdef _SESSION_FULL_ACTIVEX_CTRL
//--------------------------
// =Property map
BEGIN_PROPERTY_MAP(CFCSessions)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	PROP_PAGE(CLSID_StockColorPage)
END_PROPERTY_MAP()

//--------------------------
// =Message map
BEGIN_MSG_MAP(CFCSessions)
	MESSAGE_HANDLER(WM_PAINT, OnPaint)
//	MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
//	MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocus)
END_MSG_MAP()
#endif // _SESSION_FULL_ACTIVEX_CTRL


//--------------------------
// =Operations
public:
    typedef enum {
        varSession,
        ibtSession,
        msgSession
    } SessionType;
    
    

//--------------------------
// =Interface
// IFCSessions
public:
	STDMETHOD(OpenVarSession)(BSTR sAddress, /*[out, retval]*/ LPUNKNOWN *pUnk);
	STDMETHOD(LookupVarSession)(/*[in, string]*/ BSTR sAddress, /*[out, retval]*/ LPUNKNOWN *pUnk);
	STDMETHOD(OpenCommissioningSession)(BSTR sAddress, /*[out, retval]*/ LPUNKNOWN *pUnk);
	STDMETHOD(LookupCommissioningSession)(/*[in, string]*/ BSTR sAddress, /*[out, retval]*/ LPUNKNOWN *pUnk);
	STDMETHOD(OpenMessageSession)(BSTR sAddress, /*[out, retval]*/ LPUNKNOWN *pUnk);
	STDMETHOD(LookupMessageSession)(/*[in, string]*/ BSTR sAddress, /*[out, retval]*/ LPUNKNOWN *pUnk);
	STDMETHOD(Shutdown_Gracefully)();

//--------------------------
// =Overrides
public:
    HRESULT FinalConstruct();
    void    FinalRelease();
    static void ParseAddress(CString & strAddress,CString &, CGuid &, CString &);
//--------------------------
// =Helpers
private:
    void    CleanoutLists();
    CString BuildSessionName(SessionType type, LPCTSTR pszAddress);
    HRESULT LookupSession(SessionType type, BSTR sAddress, LPUNKNOWN*);
    void    LookupSession(SessionType type, LPCTSTR pszAddress, LPUNKNOWN *);
    
    //HRESULT RemoveFromList(SessionType, LPCTSTR);
//--------------------------
// =Attributes
private:
    sessionNameMap          m_sessionNameMap;   /**< map with sessions key = address string*/
    CCriticalSection        m_cs;               /**< to protect maps*/
    DWORD                   m_dwThreadId;       /**< thread context creating this object*/
};

/////////////////////////////////////////////////////////////////////
#endif // _FCSESSION_H_

/*
 *----------------------------------------------------------------------------*
 *  $History: FCSession.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 5  *****************
 * User: Su           Date: 10.03.03   Time: 10:02
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 4  *****************
 * User: Su           Date: 30.04.01   Time: 14:46
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * Use whole address as key.
 * 
 * *****************  Version 3  *****************
 * User: Su           Date: 19.03.01   Time: 19:02
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Revision with respect to new target connection information
 * 
 * *****************  Version 2  *****************
 * User: Su           Date: 15.03.01   Time: 19:43
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Changed IFCSessions interface to have only addess string and no config
 * name. 
 *  New header, removed private trace files, replaced by tracea.lib.
 *==
 *----------------------------------------------------------------------------*
*/

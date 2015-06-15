/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/4CVariables.h 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: 4CVariables.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/4CVariables.h $
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
#ifndef _4CVARIABLES_H_
#define _4CVARIABLES_H_

//----  Aggregate Includes:   --------------------------------------*
#include "resource.h"       // main symbols
#include "CSC_Online.h"
#include "CPtVarNotification.h"
#include "4CVariable.h"

//----  Forward Class Definitions:   -------------------------------*
class   CFCVariable;

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

//----  Typedefs:    -----------------------------------------------*
typedef CMap<long, long, CComObject<CFCVariable>*, CComObject<CFCVariable>*&>           varHandleMap;

#ifdef _DEBUG
#define _VAR_COLL_FULL_ACTIVEX_CTRL
#endif // _DEBUG

//------------------------------------------------------------------*
/**
 *  class CFCVariableCollection
 *  collection of 4C variables to be accessed via OPC
 *
 *  @see    ....
*/
class ATL_NO_VTABLE CFCVariableCollection : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CFCVariableCollection,&CLSID_FCVariableCollection>,
	public IDispatchImpl<IFCVariableCollection, &IID_IFCVariableCollection, &LIBID_CSC_ONLINELib>,
    public IObjectSafetyImpl<CFCVariableCollection, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
	public IConnectionPointContainerImpl<CFCVariableCollection>,
    public CProxy_FCVariableNotification<CFCVariableCollection>,
#ifdef _VAR_COLL_FULL_ACTIVEX_CTRL
	public CComControl<CFCVariableCollection>,
    public IPersistStreamInitImpl<CFCVariableCollection>,
	public IOleControlImpl<CFCVariableCollection>,
	public IOleObjectImpl<CFCVariableCollection>,
	public IOleInPlaceActiveObjectImpl<CFCVariableCollection>,
	public IViewObjectExImpl<CFCVariableCollection>,
	public ISupportErrorInfo,
	public IOleInPlaceObjectWindowlessImpl<CFCVariableCollection>,
	public IObjectWithSiteImpl<CFCVariableCollection>,
	public IPersistStorageImpl<CFCVariableCollection>,
	public IQuickActivateImpl<CFCVariableCollection>,
	public IDataObjectImpl<CFCVariableCollection>,
	public ISpecifyPropertyPagesImpl<CFCVariableCollection>,
#endif // _VAR_COLL_FULL_ACTIVEX_CTRL
    public IProvideClassInfo2Impl<&CLSID_FCVariableCollection, &IID__FCVariableNotification, &LIBID_CSC_ONLINELib>
{
public:
//--------------------------
// =Constructor
	                        CFCVariableCollection();
	                        ~CFCVariableCollection();

// DECLARE_GET_CONTROLLING_UNKNOWN()
DECLARE_REGISTRY_RESOURCEID(IDR_4CVARIABLES)

//--------------------------
// =Interface map
BEGIN_COM_MAP(CFCVariableCollection)
	COM_INTERFACE_ENTRY2(IDispatch, IFCVariableCollection)
	COM_INTERFACE_ENTRY(IFCVariableCollection)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
    COM_INTERFACE_ENTRY(IConnectionPointContainer)
    COM_INTERFACE_ENTRY(IObjectSafety)

#ifdef _VAR_COLL_FULL_ACTIVEX_CTRL
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
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
#endif //_VAR_COLL_FULL_ACTIVEX_CTRL
END_COM_MAP()

//--------------------------
// =Connection point map
BEGIN_CONNECTION_POINT_MAP(CFCVariableCollection)
    CONNECTION_POINT_ENTRY(IID__FCVariableNotification)
END_CONNECTION_POINT_MAP()

#ifdef _VAR_COLL_FULL_ACTIVEX_CTRL
//--------------------------
// =Property map
BEGIN_PROPERTY_MAP(CFCVariableCollection)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	PROP_PAGE(CLSID_StockColorPage)
END_PROPERTY_MAP()

//--------------------------
// =Message map
BEGIN_MSG_MAP(CFCVariableCollection)
	MESSAGE_HANDLER(WM_PAINT, OnPaint)
	MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
	MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocus)
END_MSG_MAP()
#endif // _VAR_COLL_FULL_ACTIVEX_CTRL

//--------------------------
// =Operations
public:

//--------------------------
// =Overrides
public:
	HRESULT                 FinalConstruct();

	void                    FinalRelease();

#ifdef _VAR_COLL_FULL_ACTIVEX_CTRL
    HRESULT OnDrawAdvanced(ATL_DRAWINFO& di);
#endif // _VAR_COLL_FULL_ACTIVEX_CTRL



#ifdef _VAR_COLL_FULL_ACTIVEX_CTRL
//--------------------------
// =Interface
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IViewObjectEx
	STDMETHOD(GetViewStatus)(DWORD* pdwStatus)
	{
		ATLTRACE(_T("IViewObjectExImpl::GetViewStatus\n"));
		*pdwStatus = VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE;
		return S_OK;
	}
#endif // _VAR_COLL_FULL_ACTIVEX_CTRL

//--------------------------
// =Interface
// IFCVariableCollection
public:
	STDMETHOD(Subscribe)(/*[in]*/ BSTR sItem, /*[out, retval]*/ long *phServer);
	STDMETHOD(UnsubscribeWithHandle)(/*[in]*/ long hServer);
	STDMETHOD(WriteWithHandle)(/*[in]*/ long hServer, /*[in]*/ VARIANT newVal);
	STDMETHOD(ReadWithHandle)(/*[in]*/ long hServer, /*[out, optional]*/ short *pQuality, /*[out, optional]*/ DATE *pDate, /*[out, retval]*/ VARIANT *pVal);

//--------------------------
// =Helpers
private:
    CComObject<CFCVariable>*    FindVariable(long hServer);
    // find variable by handle; returns NULL if not found
    void    ParseAddress(BSTR sItem,CString & strAddress, CString & strItemPath);

//--------------------------
// =Attributes
private:
    varHandleMap            m_varHandleMap;
                                // my server handle to variable object, used to verify handed-in handles

    CComPtr<IFCSessions>    m_pSessions;

    CCriticalSection        m_cs;
};

#endif //__4CVARIABLES_H_
/////////////////////////////////////////////////////////////////////

/*
 *----------------------------------------------------------------------------*
 *  $History: 4CVariables.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 3  *****************
 * User: Su           Date: 19.03.01   Time: 19:02
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Revision with respect to new target connection information
 * 
 * *****************  Version 2  *****************
 * User: Su           Date: 15.03.01   Time: 19:41
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * New header, removed private trace files, replaced by tracea.lib.
 * Removed not implemented IF methods.
 * 
 * *****************  Version 1  *****************
 * User: Su           Date: 14.03.01   Time: 17:21
 * Created in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * Initial rev.
 * 
 * *****************  Version 40  *****************
 * User: Ed           Date: 11.02.00   Time: 16:16
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Variablen-Connection mit eigenem Thread
 *==
 *----------------------------------------------------------------------------*
*/

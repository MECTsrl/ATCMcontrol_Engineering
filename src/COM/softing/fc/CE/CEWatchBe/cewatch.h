#ifndef __CEWATCH_H_
#define __CEWATCH_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


// CEWatch.h : Declaration of the CCEWatch


class CExpressionNode;

#include "fccallback.h"
#include "cewatchbe.h"
#include "resource.h"
#include "cewatchbeCP.h"
#include "watchexpr.h"
#include "watchexprman.h"


/////////////////////////////////////////////////////////////////////////////
// CCEWatch
class ATL_NO_VTABLE CCEWatch : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CCEWatch, &CLSID_CEWatchBE>,
	public IConnectionPointContainerImpl<CCEWatch>,
	public IDispatchImpl<ICEWatch, &IID_ICEWatch, &LIBID_CEWATCHBELib>,
	public CProxyICEWatchNotify< CCEWatch >
{
public:
    CCEWatch ();
    ~CCEWatch ();

DECLARE_REGISTRY_RESOURCEID(IDR_CEWATCHBE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CCEWatch)
	COM_INTERFACE_ENTRY(ICEWatch)
//DEL 	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(CCEWatch)
CONNECTION_POINT_ENTRY(IID_ICEWatchNotify)
END_CONNECTION_POINT_MAP()


// ICEWatch
public:
	STDMETHOD(Init)(/*[in, string]*/ BSTR sSymbolFile);
	STDMETHOD(Cleanup)();
	STDMETHOD(GetDebugInfo)(/*[out]*/ IUnknown **ppDebugInfo);
    STDMETHOD(AddItem)(/*[in, string]*/ BSTR sText, /*[out]*/ long *plHandle);
    STDMETHOD(RemoveItem)(/*[in]*/ long lHandle);
    STDMETHOD(GetItemText)(/*[in]*/ long lHandle, /*[out, string]*/ BSTR *psText);
	STDMETHOD(GetItemState)(/*[in]*/ long lHandle, /*[out]*/ CEWatchItemState *pState);
	STDMETHOD(GetExpressionText)(/*[in]*/ long lHandle, /*[out, string]*/ BSTR *psText);
	STDMETHOD(GetExpressionState)(/*[in]*/ long lHandle, /*[out]*/ CEExpressionState *pState);
	STDMETHOD(GetFormatText)(/*[in]*/ long lHandle, /*[out, string]*/ BSTR *psText);
	STDMETHOD(GetFormatState)(/*[in]*/ long lHandle, /*[out]*/ CEFormatState *pState);
	STDMETHOD(GetFormatBase)(/*[in]*/ long lHandle, /*[out]*/ CENumberBase *pBase);
	STDMETHOD(GetRangesText)(/*[in]*/ long lHandle, /*[out, string]*/ BSTR *psText);
	STDMETHOD(GetRangesState)(/*[in]*/ long lHandle, /*[out]*/ CERangesState *pState);
	STDMETHOD(GetRangeCount)(/*[in]*/ long lHandle, /*[out]*/ unsigned long *puiCount);
	STDMETHOD(GetRange)(/*[in]*/ long lHandle, /*[in]*/ unsigned long lIndex, /*[out]*/ long *plLower, /*[out]*/ long *plUpper);
	STDMETHOD(GetType)(/*[in]*/ long lHandle, /*[out]*/ IUnknown **ppType);
    STDMETHOD(GetSubItemCount)(/*[in]*/ long lHandle, /*[out]*/ unsigned long *pulCount);
    STDMETHOD(GetSubItem)(/*[in]*/ long lHandle, /*[in]*/ unsigned long ulIndex, /*[out]*/ long *plSubItemHandle);
    STDMETHOD(Connect)(/*[in]*/ long lHandle);
    STDMETHOD(Disconnect)(/*[in]*/ long lHandle);
	STDMETHOD(GetData)(/*[in]*/ long lHandle, /*[out]*/ VARIANT *pvarValue, /*[out]*/ short *psQuality, /*[out]*/ DATE *pDate);
	STDMETHOD(GetDataText)(/*[in]*/ long lHandle, /*[in]*/ BOOL bFormatted, /*[out, string]*/ BSTR *psValue, /*[out]*/ short *psQuality, /*[out]*/ DATE *pTimestamp);
	STDMETHOD(GetWriteState)(/*[in]*/ long lHandle, /*[out]*/ CEWriteState *pState);
    STDMETHOD(Write)(/*[in]*/ long lHandle, /*[in]*/ VARIANT varValue);
	STDMETHOD(AddItemWithScope)(/*[in, string]*/ BSTR sText, /*[in, string]*/ BSTR sScope, /*[out]*/ long *plHandle);
	STDMETHOD(GetClientData)(/*[in]*/ long lHandle, /*[out]*/ long *plClientData);
	STDMETHOD(SetClientData)(/*[in]*/ long lHandle, /*[in]*/ long lClientData);
	STDMETHOD(SetFormatText)(/*[in]*/ long lHandle, /*[in, string]*/ BSTR sText);
	STDMETHOD(AddItemExpand)(/*[in, string]*/ BSTR sText, /*[in]*/ CEExpandMode expandMode, /*[out]*/ long *plHandle);

protected:
    typedef CList<long, long> HandleList;

    CMap<long, long, CExpressionHolder *, CExpressionHolder *> m_ExpressionByHandle; 

    CExpressionFactory *m_pExpressionFactory;

    FC_CALLBACK (ExpressionChangedCB, CCEWatch) m_ExpressionChangedCB;

    CComPtr<IDebugInfo> m_pDebugInfo;

private:
    HRESULT ParseSimple (const CString &strText, CExpressionHolder *&pExpression);
    HRESULT ParseExpand (const CString &strText, CEExpandMode expandMode, CExpressionHolder *&pExpression);
    HRESULT ParseWithScope (const CString &strText, const CString &strScope, CExpressionHolder *&pExpression);
    CExpressionHolder *GetHolder (long lHandle);
    void OnExpressionChanged (CExpressionHolder *pHolder);
    void DisposeExpressions ();
};


class ATL_NO_VTABLE CCEWatchAdmin : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CCEWatchAdmin, &CLSID_CEWatchAdminBE>,
	public IDispatchImpl<ICEWatchAdmin, &IID_ICEWatchAdmin, &LIBID_CEWATCHBELib>
{
public:
    CCEWatchAdmin ();
    ~CCEWatchAdmin ();

DECLARE_REGISTRY_RESOURCEID(IDR_CEWATCHBE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CCEWatchAdmin)
	COM_INTERFACE_ENTRY(ICEWatchAdmin)
//DEL 	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


// ICEWatchAdmin
public:
    STDMETHOD(Resubscribe)();
};


#endif // __CEWATCH_H_


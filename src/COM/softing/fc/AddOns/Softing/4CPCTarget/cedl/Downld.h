#ifndef __DOWNLOAD_H_
#define __DOWNLOAD_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//----  Aggregate Includes:   --------------------------------------*
#include "resource.h"       // main symbols
#include "cedl.h"
#include "cemsg.h"
#include "cpcedl.h"
#include "dlevnt.h"

//----  Forward Class Definitions:   -------------------------------*
class CStationList;		//station list object
//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


/////////////////////////////////////////////////////////////////////////////
// CImpCEDL
class ATL_NO_VTABLE CImpCEDL : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CImpCEDL, &CLSID_CEDL>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CImpCEDL>,
	public IDispatchImpl<IDL, &IID_IDL, &LIBID_CEDLLib,2,0>,
    public CProxyIDLEvents<CImpCEDL>,
    public CProxy_ICEMessageEvent<CImpCEDL>,
    public IDispatchImpl<IDLProperties, &IID_IDLProperties, &LIBID_CEDLLib,2,0>,
    public IDispatchImpl<IDLEx, &IID_IDLEx, &LIBID_CEDLLib,2,0>
{
	HANDLE			m_hEvntAbort;		//abort event
	CStationList*	m_pStationList;		//pointer to station list
	BOOL			m_bInit;			//Init flag
    DWORD           m_dwEvents;

    CComObject<CImpDLEvents>* m_pEvents;//own Event sink

private:
    void CreateEvntSink(void) throw(); //init own Event sink;
    void DestroyEvntSink(void); //destroy own event sink;
    void Cleanup();
public:
	CImpCEDL(void);
	HRESULT FinalConstruct();
	void	FinalRelease();

DECLARE_REGISTRY_RESOURCEID(IDR_DOWNLOAD)

BEGIN_COM_MAP(CImpCEDL)
	COM_INTERFACE_ENTRY(IDL)
	COM_INTERFACE_ENTRY(IDLProperties)
    COM_INTERFACE_ENTRY2(IDispatch,IDL)
    COM_INTERFACE_ENTRY(IDLEx)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CImpCEDL)
    CONNECTION_POINT_ENTRY(IID_IDLEvents)
    CONNECTION_POINT_ENTRY(IID__ICEMessageEvent)
END_CONNECTION_POINT_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

//  ICEDL
public:
	STDMETHOD(Init)(BSTR sTargetName,BSTR sGenPath,BOOL bSilent);
	STDMETHOD(Exit)();
    STDMETHOD(DownloadTarget)(/*[in]*/BSTR sUser,/*[in]*/BSTR sPassWord,
        /*[in]*/CEDL_DOWNLD_TYPE Type,/*[in]*/CEDL_DOWNLD_OPTIONS options);
    STDMETHOD(Abort)();
	STDMETHOD(Attach)(LPUNKNOWN pUnk);
	STDMETHOD(Detach)();
//  IDLProperties
    STDMETHOD(GetTargetDLVersion)(/*[out]*/BSTR* psVersion,/*[out]*/BSTR* psProjectName);
    STDMETHOD(get_Address)(/*[out]*/BSTR* psAddress);
//  IDLEx
    STDMETHOD(DownloadSelect)(long lHWND,BSTR sUser);
    STDMETHOD(ShowDownloadState)(/*[in]*/long lHWND);
};

typedef CProxyIDLEvents<CImpCEDL>* PIDLEvents;
typedef CProxy_ICEMessageEvent<CImpCEDL>* PICEMsg;

#endif //__DOWNLOAD_H_

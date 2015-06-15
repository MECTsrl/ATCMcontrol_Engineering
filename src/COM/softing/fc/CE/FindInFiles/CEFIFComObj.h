// CEFIFComObj.h : Declaration of the CCEFIFComObj

#ifndef __CEFIFCOMOBJ_H_
#define __CEFIFCOMOBJ_H_

#include "resource.h"       // main symbols
#include "CEMsg.h"
#include "FindInFiles.h"
#include "FindInFilesCP.h"
#include "FIFThread.h"

#include "cfacility.h"

/////////////////////////////////////////////////////////////////////////////
// CCEFIFComObj

//------------------------------------------------------------------*
/**
 *  class CCEFIFComObj
 *
 *  - COM class to encapsulate find in files functionality
 *  - implements ICEFindInFiles
 *  - implements connection point ICEMessageEvent
 *  - IFIFMsgInternal is used to generate marshaling code for access to MTA 
 *    (Multi Threaded Apartment)
 *  - contains find in files thread CEFIFThread
 *
 *  @see    CEFIFThread
 */
class ATL_NO_VTABLE CCEFIFComObj : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCEFIFComObj, &CLSID_CEFindInFiles>,
	public IDispatchImpl<ICEFindInFiles, &IID_ICEFindInFiles, &LIBID_FINDINFILESLib>,
	public CProxy_ICEMessageEvent< CCEFIFComObj >,
	public IConnectionPointContainerImpl<CCEFIFComObj>,
	public IDispatchImpl<IFIFMsgInternal, &IID_IFIFMsgInternal, &LIBID_FINDINFILESLib>
{
public:
	CCEFIFComObj();

DECLARE_REGISTRY_RESOURCEID(IDR_CEFIFCOMOBJ)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CCEFIFComObj)
	COM_INTERFACE_ENTRY(ICEFindInFiles)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IFIFMsgInternal)
END_COM_MAP()

public:
    void FireMessage(const CString& strMessage, HRESULT hr = S_OK);

    virtual void FinalRelease();

    HRESULT GetInterfacePointer(IFIFMsgInternal** ppIFIFMsg);

// interface: ICEFindInFiles 
public:


protected:
    HRESULT Initialize();

public :

BEGIN_CONNECTION_POINT_MAP(CCEFIFComObj)
	CONNECTION_POINT_ENTRY(IID__ICEMessageEvent)
END_CONNECTION_POINT_MAP()



// _ICEMessageEvent
	STDMETHOD(OnNewMessage)(BSTR sMachine, USHORT wScrFacility, HRESULT hr, BSTR sDescr)
	{
		return E_NOTIMPL;
	}
// IFIFMsgInternal
	STDMETHOD(NewFIFMessage)(BSTR sMessage)
	{
        CComBSTR    sMachine;
        HRESULT     hr = S_OK;

		Fire_OnNewMessage(sMachine, E_FACILITY_CEFINDINFILES, hr, sMessage);
        return S_OK;
	}

// ICEFindInFiles
    STDMETHOD (AddFileType)(/*[in,string]*/ BSTR    sExtension,
                            /*[in,string]*/ BSTR    sEditGUID);

    STDMETHOD (StartFind)(/*[in,string]*/   BSTR    sExpression,
                          /*[in,string]*/   BSTR    sDirectory,
                          /*[in]*/          DWORD   dwOptions);

    STDMETHOD (StopFind)();

    STDMETHOD (IsFindActive)(/*[out]*/      BOOL*   pbActive);

    STDMETHOD (Reset)();

protected:
    CEFIFThread         m_tFIFThread;       /**< find in files thread member */
    CEFIFThreadData     m_tFIFThreadData;   /**< find in files thread data */
    BOOL                m_bInitialized;     /**< initialized? */

};

#endif //__CEFIFCOMOBJ_H_

/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_simpleOPC/SyncAccess.h 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: SyncAccess.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_simpleOPC/SyncAccess.h $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CSC
 *
 * =COMPONENT           simple sync OPC client
 *
 * =CURRENT      $Date: 28.02.07 19:00 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  07.08.01  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/

#ifndef __SYNCACCESS_H_
#define __SYNCACCESS_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "resource.h"
#include "csc_simpleOPC.h"
#include <asptlb.h>         // Active Server Pages Definitions
#include "ItemContainer.h"

//----  Forward Class Definitions:   -------------------------------*
class   CSyncAccess;
interface IControlMgt;

//----  Defines:    ------------------------------------------------*
typedef CMap<CString, LPCTSTR, CItemContainer*, CItemContainer*&>   serverNameMap;

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*
HRESULT ConnectToControlMgt(LPCTSTR pszNodeName, BSTR sPassword, IControlMgt **ppUnk, long* plSessionKey);

/////////////////////////////////////////////////////////////////////////////
// CSyncAccess
class ATL_NO_VTABLE CSyncAccess : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CSyncAccess, &CLSID_SyncAccess>,
	public ISupportErrorInfo,
	public IDispatchImpl<ISyncAccess, &IID_ISyncAccess, &LIBID_CSC_SIMPLEOPCLib>,
    public IObjectSafetyImpl<CSyncAccess, INTERFACESAFE_FOR_UNTRUSTED_CALLER>
{
public:
	                        CSyncAccess();
  	virtual                 ~CSyncAccess();
    void                    FinalRelease();

public:

DECLARE_REGISTRY_RESOURCEID(IDR_SYNCACCESS)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CSyncAccess)
	COM_INTERFACE_ENTRY(ISyncAccess)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
    COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// ISyncAccess
public:
	STDMETHOD(ReadStringListTyped)(/*[in, string]*/ BSTR sItems, /*[in,string]*/ BSTR sCommonPath, /*[out,string,retval]*/ BSTR *psValues);
	STDMETHOD(ReadStringList)(/*[in, string]*/ BSTR sItems, /*[in,string]*/ BSTR sCommonPath, /*[out,string,retval]*/ BSTR *psValues);
	STDMETHOD(UnsubscribeStringList)(/*[in,string]*/ BSTR sItems, /*[in,string]*/ BSTR sCommonPath);
	STDMETHOD(SubscribeStringList)(/*[in,string]*/ BSTR sItems, /*[in,string]*/ BSTR sCommonPath);
	STDMETHOD(Subscribe)(/*[in, string]*/ BSTR sItem);
	STDMETHOD(Unsubscribe)(/*[in, string]*/ BSTR sItem);
	STDMETHOD(get_Value)(/*[in, string]*/ BSTR sItem, /*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(put_Value)(/*[in, string]*/ BSTR sItem, /*[in]*/ VARIANT newVal);
	//Active Server Pages Methods
	STDMETHOD(OnStartPage)(IUnknown* IUnk);
	STDMETHOD(OnEndPage)();

public:
	STDMETHOD(ReadCachedStringListTyped)(/*[in, string]*/ BSTR sItems, /*[in,string]*/ BSTR sCommonPath, /*[out,string,retval]*/ BSTR *psValues);
	STDMETHOD(Write)(/*[in,string]*/ BSTR sItem, /*[in]*/ VARIANT newVal);
/**
 * the following helper function are used by the console to start/stop resources and tasks.
 * purpose: replace CSC_Online.ocx completely by CSC_SimpleOPC
 */
public:
    STDMETHOD(StartResource)(/* [string][in] */ BSTR sFullResourcePath, /* [in][string] */ BSTR sPassword);
    STDMETHOD(StopResource)(/* [string][in] */ BSTR sFullResourcePath, /* [in][string] */ BSTR sPassword);
    STDMETHOD(StartAllTasks)(/* [string][in] */ BSTR sFullResourcePath, /* [in][string] */ BSTR sPassword);
    STDMETHOD(StopAllTasks)(/* [string][in] */ BSTR sFullResourcePath, /* [in][string] */ BSTR sPassword);
    STDMETHOD(StartTask)(/* [string][in] */ BSTR sFullTaskPath, /* [in][string] */ BSTR sPassword);
    STDMETHOD(StopTask)(/* [string][in] */ BSTR sFullTaskPath, /* [in][string] */ BSTR sPassword);

public:
	HRESULT DoSubscribe(const CString& strItem);
	HRESULT DoSubscribeStringList(const CString& strItems, const CString& strCommonPath);
	HRESULT DoSubscribeStringList(const CString& strItems);
	HRESULT DoSubscribeStringListOpt(const CString& strItems);

	HRESULT DoUnsubscribe(const CString& strItem);
	HRESULT DoUnsubscribeStringList(const CString& strItems, const CString& strCommonPath);
	HRESULT DoUnsubscribeStringList(const CString& strItems);
	HRESULT DoUnsubscribeStringListOpt(const CString& strItems);

	HRESULT DoReadString(const CString& strItem, CString& strValue);
	HRESULT DoReadStringList(const CString& strItems, const CString& strCommonPath, CString& strValues);
	HRESULT DoReadStringList(const CString& strItems, CString& strValues);
	HRESULT DoReadStringListOpt(const CString& strItems, CString& strValues);
	HRESULT DoReadStringTyped(const CString& strItem,const CString& strType,  CString& strValue);
	HRESULT DoReadStringListTyped(const CString& strItems, const CString& strCommonPath, CString& strValues);
	HRESULT DoReadStringListTyped(const CString& strItems, CString& strValues);

	HRESULT DoReadCachedString(const CString& strItem, CString& strValue);
	HRESULT DoReadCachedStringList(const CString& strItems, const CString& strCommonPath, CString& strValues);
	HRESULT DoReadCachedStringList(const CString& strItems, CString& strValues);

	HRESULT DoReadCachedStringTyped(const CString& strItem,const CString& strType,  CString& strValue);
	HRESULT DoReadCachedStringListTyped(const CString& strItems, const CString& strCommonPath, CString& strValues);
	HRESULT DoReadCachedStringListTyped(const CString& strItems, CString& strValues);


	HRESULT DoWrite(const CString& strValue, VARIANT newVal);


// =Helpers
private:
    CItemContainer *        LookupContainer(LPCTSTR pszVarName, CString& strPureName, BOOL bCreate = FALSE);
    CItemContainer *        LookupContainer(BSTR sVarName, CString& strPureName, BOOL bCreate = FALSE);

// =Attributes
private:
    serverNameMap           m_configNameMap;     // map of configuration names to container
    CString                 m_strServerNameOPC;
    CComPtr<IControlMgt>    m_pControlMgt;
    long                    m_lSessionKey;
	BOOL                    m_bOnStartPageCalled; //OnStartPage successful?
};

#endif // __SYNCACCESS_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: SyncAccess.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_simpleOPC
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSC/CSC_simpleOPC
 * 
 * *****************  Version 3  *****************
 * User: Ed           Date: 9/03/01    Time: 3:23p
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_simpleOPC
 * handle session password
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 8.08.01    Time: 18:15
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_simpleOPC
 * adapted to changes of V2.0
 * 
 * *****************  Version 1  *****************
 * User: Ca           Date: 8.08.01    Time: 9:59
 * Created in $/4Control/COM/softing/fc/CSC/CSC_simpleOPC
 * copied from V1.3
 *==
 *----------------------------------------------------------------------------*
*/

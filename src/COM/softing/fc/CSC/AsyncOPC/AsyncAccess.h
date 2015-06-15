/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/AsyncOPC/AsyncAccess.h 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: AsyncAccess.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/AsyncOPC/AsyncAccess.h $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CSC
 *
 * =COMPONENT           AsyncOPC
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
 *    AsyncAccess.h : Declaration of the CAsyncAccess
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  07.08.01  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/

#ifndef __ASYNCACCESS_H_
#define __ASYNCACCESS_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "resource.h"       // main symbols
#include "ValueFormatter.h"
#include "Item.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

typedef CMap<CString, LPCTSTR, CItem*, CItem*&>   itemNameMap;

/////////////////////////////////////////////////////////////////////////////
// CAsyncAccess
/////////////////////////////////////////////////////////////////////////////
// CAsyncAccess
class ATL_NO_VTABLE CAsyncAccess : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CAsyncAccess, &CLSID_AsyncAccess>,
    public IProvideClassInfoImpl<&CLSID_AsyncAccess>,
	public ISupportErrorInfoImpl<&IID_IAsyncAccess>,
	public IDispatchImpl<IAsyncAccess, &IID_IAsyncAccess, &LIBID_ASYNCOPCLib>,
    public IObjectSafetyImpl<CAsyncAccess, INTERFACESAFE_FOR_UNTRUSTED_CALLER>
{
public:
	                        CAsyncAccess();
  	virtual                 ~CAsyncAccess();

public:

DECLARE_REGISTRY_RESOURCEID(IDR_ASYNCACCESS)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CAsyncAccess)
	COM_INTERFACE_ENTRY(IAsyncAccess)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
    COM_INTERFACE_ENTRY(IProvideClassInfo)
    COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

// IAsyncAccess
public:
	STDMETHOD(ReadStringListTyped)(/*[in, string]*/ BSTR sItems, /*[in,string]*/ BSTR sCommonPath, /*[out,string,retval]*/ BSTR *psValues);
	STDMETHOD(ReadStringList)(/*[in, string]*/ BSTR sItems, /*[in,string]*/ BSTR sCommonPath, /*[out,string,retval]*/ BSTR *psValues);
	STDMETHOD(UnsubscribeStringList)(/*[in,string]*/ BSTR sItems, /*[in,string]*/ BSTR sCommonPath);
	STDMETHOD(SubscribeStringList)(/*[in,string]*/ BSTR sItems, /*[in,string]*/ BSTR sCommonPath);
	STDMETHOD(Subscribe)(/*[in, string]*/ BSTR sItem);
	STDMETHOD(Unsubscribe)(/*[in, string]*/ BSTR sItem);
	STDMETHOD(get_Value)(/*[in, string]*/ BSTR sItem, /*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(put_Value)(/*[in, string]*/ BSTR sItem, /*[in]*/ VARIANT newVal);

	STDMETHOD(ReadStringListTypedAndRaw)(/*[in, string]*/ BSTR sItems, /*[in,string]*/ BSTR sCommonPath, /*[out,string,retval]*/ BSTR *psValues);
	STDMETHOD(Write)(/*[in,string]*/ BSTR sItem, /*[in]*/ VARIANT newVal);

    STDMETHOD(StartResource)(/* [string][in] */ BSTR sFullResourcePath, /* [string][in] */ BSTR sPassword);
    STDMETHOD(StopResource)(/* [string][in] */ BSTR sFullResourcePath, /* [string][in] */ BSTR sPassword);
    STDMETHOD(StartAllTasks)(/* [string][in] */ BSTR sFullResourcePath, /* [string][in] */ BSTR sPassword);
    STDMETHOD(StopAllTasks)(/* [string][in] */ BSTR sFullResourcePath, /* [string][in] */ BSTR sPassword);
    STDMETHOD(StartTask)(/* [string][in] */ BSTR sFullTaskPath, /* [string][in] */ BSTR sPassword);
    STDMETHOD(StopTask)(/* [string][in] */ BSTR sFullTaskPath, /* [string][in] */ BSTR sPassword);


// =Helpers
private:
	HRESULT DoSubscribe(const CString& strItem);
	HRESULT DoUnsubscribe(const CString& strItem);

    HRESULT DoRead(const CString& strItem, VARIANT *pValue);
	HRESULT DoReadString(const CString& strItem, CString& strValue);
	HRESULT DoReadStringTyped(const CString& strItem,const CString& strType,  CString& strValue);
	HRESULT DoReadStringTypedAndRaw(const CString& strItem,const CString& strType,  CString& strValue);
	HRESULT DoWrite(const CString& strItem, VARIANT newVal);

// =Attributes
private:
    CCriticalSection        m_cs;
    itemNameMap             m_itemNameMap;
    CValueFormatter         m_valueFormatter;
};

#endif // __ASYNCACCESS_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: AsyncAccess.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSC/AsyncOPC
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSC/AsyncOPC
 * 
 * *****************  Version 2  *****************
 * User: Ed           Date: 9/03/01    Time: 3:22p
 * Updated in $/4Control/COM/softing/fc/CSC/AsyncOPC
 * handle session password
 * 
 * *****************  Version 1  *****************
 * User: Ca           Date: 8.08.01    Time: 9:41
 * Created in $/4Control/COM/softing/fc/CSC/AsyncOPC
 * copied from V1.3
 *==
 *----------------------------------------------------------------------------*
*/
	

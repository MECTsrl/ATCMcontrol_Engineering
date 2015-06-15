
#ifndef __INSTANCEDBI_H_
#define __INSTANCEDBI_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXMLArchive;
class CDBIContext;
class CTypeDBI;

#include "DbgInfo.h"
#include "AnyDBI.h"


/////////////////////////////////////////////////////////////////////////////
// CInstanceDBI

class ATL_NO_VTABLE CInstanceDBI : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CInstanceDBI, &CLSID_InstanceDBI>,
	public IDispatchImpl<IInstanceDBI, &IID_IInstanceDBI, &LIBID_DBGINFOLib>,
    public CAnyDBI
{
public:
DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CInstanceDBI)
	COM_INTERFACE_ENTRY(IInstanceDBI)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

public:
    HRESULT Init (CDBIContext *pContext, CTypeDBI *pType, const CString &strPath);
    HRESULT Init (CDBIContext *pContext, CTypeDBI *pType, CXMLArchive &ar);

    CTypeDBI *GetType ();
    CString GetPath ();

	STDMETHOD(get_Type)(/*[out, retval]*/ IUnknown **ppType);
	STDMETHOD(get_Path)(/*[out, string, retval]*/ BSTR *psPath);

protected:
    CTypeDBI    *m_pType;
    CString     m_strPath;

    CInstanceDBI ();
    virtual ~CInstanceDBI ();

    friend class CResourceDBI;
};

#endif

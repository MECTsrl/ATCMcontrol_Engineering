
#ifndef __RESOURCEDBI_H_
#define __RESOURCEDBI_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXMLArchive;
class CDBIContext;
class CTypeDBI;
class CInstanceDBI;

#include "DbgInfo.h"
#include "AnyDBI.h"
#include "Oaidl.h"


/////////////////////////////////////////////////////////////////////////////
// CResourceDBI

class ATL_NO_VTABLE CResourceDBI : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CResourceDBI, &CLSID_ResourceDBI>,
	public IDispatchImpl<IResourceDBI, &IID_IResourceDBI, &LIBID_DBGINFOLib>,
    public CAnyDBI
{
public:
DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CResourceDBI)
	COM_INTERFACE_ENTRY(IResourceDBI)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

public:
    HRESULT Init (CDBIContext *pContext, const CString &strName, CTypeDBI *pType);
    HRESULT Init (CDBIContext *pContext, CXMLArchive &ar);

    CString GetName ();
    CTypeDBI *GetType ();
    CInstanceDBI *LookupInstanceByPath (const CString &strPath);
    void AddInstancesForDataType(const CString& crstrDataType, CString& rstrInstPath, CStringArray& rastrInstancePaths);
    void AddGvlInstance(const CString& crstrFileName, CString& rstrInstPath, CStringArray& rastrInstancePaths);

	STDMETHOD(get_Name)(/*[out, string, retval]*/ BSTR *psName);
	STDMETHOD(get_Type)(/*[out, retval]*/ IUnknown **ppType);
	STDMETHOD(get_VariableCount)(/*[out, retval]*/ unsigned long *pulCount);
	STDMETHOD(get_VariableByIndex)(/*[in]*/ unsigned long ulIndex, /*[out, retval]*/ IUnknown **ppVariable);
	STDMETHOD(get_VariableByName)(/*[in, string]*/ BSTR sName, /*[out, retval]*/ IUnknown **ppVariable);
	STDMETHOD(get_InstanceCount)(/*[in, string]*/ IUnknown *pType, /*[out, retval]*/ unsigned long *pulCount);
	STDMETHOD(get_InstanceByIndex)(/*[in]*/ IUnknown *pType, /*[in]*/ unsigned long ulIndex, /*[out, retval]*/ IUnknown **ppInstance);
	STDMETHOD(get_InstanceByPath)(/*[in, string]*/ BSTR sPath, /*[out, retval]*/ IUnknown **ppInstance);
    STDMETHOD(GetInstancesForDataType)(/*[in, string]*/ BSTR sDataType, /*[out]*/ SAFEARRAY** pparrInstancePaths);

protected:
    CString     m_strName;
    CTypeDBI    *m_pType;

    CResourceDBI ();
    virtual ~CResourceDBI ();
};

#endif //__RESOURCEDBI_H_


#ifndef __CONFIGDBI_H_
#define __CONFIGDBI_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXMLArchive;
class CDBIContext;
class CResourceDBI;
class CInstanceDBI;
class CTypeDBI;

#include "DbgInfo.h"
#include "AnyDBI.h"


/////////////////////////////////////////////////////////////////////////////
// CConfigDBI

class ATL_NO_VTABLE CConfigurationDBI : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CConfigurationDBI, &CLSID_ConfigurationDBI>,
	public IDispatchImpl<IConfigurationDBI, &IID_IConfigurationDBI, &LIBID_DBGINFOLib>,
    public CAnyDBI
{
public:
DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CConfigurationDBI)
	COM_INTERFACE_ENTRY(IConfigurationDBI)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

public:
    HRESULT Init (CDBIContext *pContext, const CString &strName, const CString &strMachineName, const CString &strMachineAddress);
    HRESULT Init (CDBIContext *pContext, CXMLArchive &ar);

    CString GetName ();
    CTypeDBI *GetType ();
    CString GetMachineName ();
    CString GetMachineAddress ();
    unsigned long GetResourceCount ();
    CResourceDBI *LookupResourceByIndex (unsigned long ulIndex);
    CResourceDBI *LookupResourceByName (const CString &strName);
    bool LookupInstanceByPath (const CString &strPath, CResourceDBI *&pResourceDBI, CInstanceDBI *&pInstanceDBI);
    void AddInstancesForDataType(const CString& crstrDataType, CStringArray& rastrInstancePaths);

	STDMETHOD(get_Name)(/*[out, string]*/ BSTR *psName);
	STDMETHOD(get_Type)(/*[out, retval]*/ IUnknown **ppType);
	STDMETHOD(get_MachineName)(/*[out, retval]*/ BSTR *psName);
	STDMETHOD(get_MachineAddress)(/*[out, retval]*/ BSTR *psAddress);
	STDMETHOD(get_ResourceCount)(/*[out, retval]*/ unsigned long *pulCount);
	STDMETHOD(get_ResourceByIndex)(/*[in]*/ unsigned long ulIndex, /*[out, retval]*/ IUnknown **ppResource);
	STDMETHOD(get_ResourceByName)(/*[in, string]*/ BSTR sName, /*[out]*/ IUnknown **ppResource);
	STDMETHOD(get_InstanceByPath)(/*[in, string]*/ BSTR sPath, /*[out, retval]*/ IUnknown **ppInstance);

protected:
    CString     m_strName;
    CTypeDBI    *m_pType;
    CString     m_strMachineName;
    CString     m_strMachineAddress;

    CArray<CResourceDBI *, CResourceDBI *> m_Resources;

    CConfigurationDBI ();
    virtual ~CConfigurationDBI ();
};

#endif //__CONFIGDBI_H_


#ifndef __PROJECTDBI_H_
#define __PROJECTDBI_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CConfigurationDBI;
class CDBIContext;
class CXMLArchive;
class CResourceDBI;
class CInstanceDBI;

#include "DbgInfo.h"
#include "AnyDBI.h"


#define PROJECT_DBI_FILE _T ("project.dbi")


/////////////////////////////////////////////////////////////////////////////
// CProjectDBI

class ATL_NO_VTABLE CProjectDBI : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CProjectDBI, &CLSID_ProjectDBI>,
	public IDispatchImpl<IProjectDBI, &IID_IProjectDBI, &LIBID_DBGINFOLib>,
    public CAnyDBI
{
public:
DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CProjectDBI)
	COM_INTERFACE_ENTRY(IProjectDBI)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

    HRESULT Init (CDBIContext *pContext);
    HRESULT Load ();

    CString GetName ();
    unsigned long GetConfigurationCount ();
    CConfigurationDBI *LookupConfigurationByIndex (unsigned long ulIndex);
    CConfigurationDBI *LookupConfigurationByName (const CString &strName);
    CConfigurationDBI *LookupConfigurationByMachine (const CString &strMachine);
    CConfigurationDBI *LookupConfigurationByAddress (const CString &strAddress);
    bool LookupInstanceByPath (const CString &strPath, CConfigurationDBI *&pConfigurationDBI, CResourceDBI *&pResourceDBI, CInstanceDBI *&pInstanceDBI);

	STDMETHOD(get_Name)(/*[out, string]*/ BSTR *psName);
	STDMETHOD(get_ConfigurationCount)(/*[out, retval]*/ unsigned long *pulCount);
	STDMETHOD(get_ConfigurationByIndex)(/*[in]*/ unsigned long ulIndex, /*[out, retval]*/ IUnknown **ppConfiguration);
	STDMETHOD(get_ConfigurationByName)(/*[in, string]*/ BSTR sName, /*[out, retval]*/ IUnknown **ppConfiguration);
	STDMETHOD(get_ConfigurationByMachine)(/*[in, string]*/ BSTR sMachine, /*[out, retval]*/ IUnknown **ppConfiguration);
	STDMETHOD(get_ConfigurationByAddress)(/*[in, string]*/ BSTR sAddress, /*[out, retval]*/ IUnknown **ppConfiguration);
	STDMETHOD(get_InstanceByPath)(BSTR sPath, /*[out, retval]*/ IUnknown **ppInstance);
    STDMETHOD(GetInstancesForDataType)(/*[in, string]*/ BSTR sDataType, /*[out]*/ SAFEARRAY** pparrInstancePaths);

protected:
    CString     m_strName;
    bool        m_bLoaded;
    bool        m_bLoading;
    CTime       m_Timestamp;

    CArray<CConfigurationDBI *, CConfigurationDBI *> m_Configurations;

    CProjectDBI ();
    virtual ~CProjectDBI ();

private:
    HRESULT LoadPOUs (CXMLArchive &ar);
    void Unload ();
    CString StripConfiguration (const CString &strPath, CString &strRemainder);
};

#endif //__PROJECTDBI_H_

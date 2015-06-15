
#ifndef __VARIABLEDBI_H_
#define __VARIABLEDBI_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXMLArchive;
class CDBIContext;
class CTypeDBI;

#include "DbgInfo.h"
#include "AnyDBI.h"


/////////////////////////////////////////////////////////////////////////////
// CVariableDBI

class ATL_NO_VTABLE CVariableDBI : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CVariableDBI, &CLSID_VariableDBI>,
	public IDispatchImpl<IVariableDBI, &IID_IVariableDBI, &LIBID_DBGINFOLib>,
    public CAnyDBI
{
public:
DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CVariableDBI)
	COM_INTERFACE_ENTRY(IVariableDBI)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

public:
    HRESULT Init (CDBIContext *pContext, DBIVariableKind kind, const CString &strName, CTypeDBI *pType, const TCHAR* pszOnlString);
    HRESULT Init (CDBIContext *pContext, const CString &strName, CTypeDBI *pType, const CString &strValue);
    HRESULT Init (CDBIContext *pContext, DBIVariableKind kind, CXMLArchive &ar);

    CString GetName ();
    CTypeDBI *GetType ();
    DBIVariableKind GetKind ();
    CComVariant GetValue ();

	STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *psName);
	STDMETHOD(get_Type)(/*[out, retval]*/ IUnknown **ppType);
	STDMETHOD(get_Kind)(/*[out, retval]*/ DBIVariableKind *pKind);
	STDMETHOD(get_Value)(/*[out, retval]*/ VARIANT *pvarValue);
	STDMETHOD(get_OnlString)(/*[out, retval]*/ BSTR *psOnlString);

protected:
    DBIVariableKind m_Kind;
    CString         m_strName;
    CString*        m_pStrOnlString;
    CTypeDBI        *m_pType;
    CComVariant     m_varValue;

    CVariableDBI ();
    virtual ~CVariableDBI ();
};


class CVariableList
{
public:
    ~CVariableList ();

    HRESULT Load (CDBIContext *pContext, CXMLArchive &ar, CAnyDBI *pParent);
    void Unload (CAnyDBI *pParent);

    unsigned long GetCount ();
    CVariableDBI *GetVariable (unsigned long ulIndex);

    CVariableDBI *LookupByName (const CString &strName);

    HRESULT AddVariable (CDBIContext *pContext, DBIVariableKind kind, const CString &strName, CTypeDBI *pType, CAnyDBI *pParent);

protected:
    CArray<CVariableDBI *, CVariableDBI *> m_Variables;

private:
    HRESULT LoadVariables (CDBIContext *pContext, CXMLArchive &ar, DBIVariableKind kind, CAnyDBI *pParent);
};

#endif

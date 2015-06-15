
#ifndef __TYPEDBI_H_
#define __TYPEDBI_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CDBIContext;
class CVariableDBI;
class CBreakpointDBI;
class CInstanceDBI;
class CXMLArchive;

#include "DbgInfo.h"
#include "baselib.h"
#include "GvlFileDBI.h"
#include "VariableDBI.h"
#include "GvlFileDBI.h"
#include "BreakpointDBI.h"
#include "StepDBI.h"
#include "ActionDBI.h"
#include "TransitionDBI.h"
#include "CG_AutoDecl.h"


/////////////////////////////////////////////////////////////////////////////
// CTypeDBI

class ATL_NO_VTABLE CTypeDBI : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CTypeDBI, &CLSID_TypeDBI>,
	public IDispatchImpl<ITypeDBI, &IID_ITypeDBI, &LIBID_DBGINFOLib>,
    public CAnyDBI
{
public:
DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTypeDBI)
	COM_INTERFACE_ENTRY(ITypeDBI)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

public:
    HRESULT Init (CDBIContext *pContext, BL_IEC_TYP iecType);
    HRESULT Init (CDBIContext *pContext, BL_IEC_TYP iecType, unsigned long ulStringLength);
    HRESULT Init (CDBIContext *pContext, CTypeDBI *pElementType, long lLowerBound, long lUpperBound);
    HRESULT Init (CDBIContext *pContext, const CString &strTypeName, const CString &strNamespace, const CString &strDBIFile);
    HRESULT Init (CDBIContext *pContext, DBITypeKind kind);

    DBITypeKind GetKind ();
    BL_IEC_TYP GetIECType ();
    unsigned long GetStringLength ();
    CTypeDBI *GetElementType ();
    long GetLowerBound ();
    long GetUpperBound ();
    CString GetSource ();
    CString GetTypeName ();
    CString GetDBIFile ();
    CString GetNamespace ();
    DBILanguage GetLanguage ();

    unsigned long GetVariableCount ();
    CVariableDBI *LookupVariableByIndex (unsigned long ulIndex);
    CVariableDBI *LookupVariableByName (const CString &strName);

    CBreakpointDBI *LookupBreakpointByNumber (long lNumber);
    CBreakpointDBI *LookupBreakpointByPosition (
        EDP_POS_QUALIFIER qualifier, 
        long lX, long lY, long lCX, long lCY);

    unsigned long GetInstanceCount (IUnknown *pType);
    CInstanceDBI *LookupInstanceByIndex (IUnknown *pType, unsigned long ulIndex);
    CInstanceDBI *LookupInstanceByPath (const CString &strPath);

    CString GetText ();

    unsigned long GetStepCount ();
    CStepDBI *LookupStepByIndex (unsigned long ulIndex);
    CStepDBI *LookupStepByName (const CString &strName);
    CStepDBI *LookupStepByNumber (long lNumber);
    CStepDBI *LookupStepByPosition (long lX, long lY);

    unsigned long GetActionCount ();
    CActionDBI *LookupActionByIndex (unsigned long ulIndex);
    CActionDBI *LookupActionByName (const CString &strName);
    CActionDBI *LookupActionByNumber (long lNumber);
    CActionDBI *LookupActionByPosition (long lX, long lY);

    unsigned long GetTransitionCount ();
    CTransitionDBI *LookupTransitionByIndex (unsigned long ulIndex);
    CTransitionDBI *LookupTransitionByName (const CString &strName);
    CTransitionDBI *LookupTransitionByNumber (long lNumber);
    CTransitionDBI *LookupTransitionByPosition (long lX, long lY);

    CGvlFileList* GetGvlFileList();

	STDMETHOD(get_Kind)(/*[out, retval]*/ DBITypeKind *pKind);
	STDMETHOD(get_Source)(/*[out, retval]*/ BSTR *psSource);
	STDMETHOD(get_TypeName)(/*[out, retval]*/ BSTR *psTypeName);
	STDMETHOD(get_IECType)(/*[out, retval]*/ short *psIECType);
	STDMETHOD(get_ElementType)(/*[out, retval]*/ IUnknown **ppElementType);
	STDMETHOD(get_LowerBound)(/*[out, retval]*/ long *plLowerBound);
	STDMETHOD(get_UpperBound)(/*[out, retval]*/ long *plUpperBound);

	STDMETHOD(get_VariableCount)(/*[out, retval]*/ unsigned long *pulCount);
	STDMETHOD(get_VariableByIndex)(/*[in]*/ unsigned long ulIndex, /*[out, retval]*/ IUnknown **ppVariable);
	STDMETHOD(get_VariableByName)(/*[in, string]*/ BSTR sName, /*[out, retval]*/ IUnknown **ppVariable);

    STDMETHOD(get_BreakpointByNumber)(/*[in]*/ long lNumber, /*[out, retval]*/ IUnknown **ppBreakpoint);
	STDMETHOD(get_BreakpointByPositionEx)(/*[in]*/ EDP_POS_QUALIFIER qualifier, /*[in]*/ long lX, /*[in]*/ long lY, 
        /*[in]*/ long lCX, /*[in]*/ long lCY, /*[out, retval]*/ IUnknown **ppBreakpoint);

	STDMETHOD(get_InstanceCount)(/*[in, string]*/ IUnknown *pType, /*[out, retval]*/ unsigned long *pulCount);
	STDMETHOD(get_InstanceByIndex)(/*[in]*/ IUnknown *pType, /*[in]*/ unsigned long ulIndex, /*[out, retval]*/ IUnknown **ppInstance);
	STDMETHOD(get_InstanceByPath)(/*[in, string]*/ BSTR sPath, /*[out, retval]*/ IUnknown **ppInstance);

	STDMETHOD(get_Text)(/*[out, retval]*/ BSTR *psText);

	STDMETHOD(get_StepCount)(/*[out, retval]*/ unsigned long *pulCount);
	STDMETHOD(get_StepByIndex)(/*[in]*/ unsigned long ulIndex, /*[out, retval]*/ IUnknown **ppStep);
	STDMETHOD(get_StepByName)(/*[in, string]*/ BSTR sName, /*[out, retval]*/ IUnknown **ppStep);
    STDMETHOD(get_StepByNumber)(/*[in]*/ long lNumber, /*[out, retval]*/ IUnknown **ppStep);
	STDMETHOD(get_StepByPosition)(/*[in]*/ long lX, /*[in]*/ long lY, /*[out, retval]*/ IUnknown **ppStep);

	STDMETHOD(get_ActionCount)(/*[out, retval]*/ unsigned long *pulCount);
	STDMETHOD(get_ActionByIndex)(/*[in]*/ unsigned long ulIndex, /*[out, retval]*/ IUnknown **ppAction);
	STDMETHOD(get_ActionByName)(/*[in, string]*/ BSTR sName, /*[out, retval]*/ IUnknown **ppAction);
    STDMETHOD(get_ActionByNumber)(/*[in]*/ long lNumber, /*[out, retval]*/ IUnknown **ppAction);
	STDMETHOD(get_ActionByPosition)(/*[in]*/ long lX, /*[in]*/ long lY, /*[out, retval]*/ IUnknown **ppAction);

	STDMETHOD(get_TransitionCount)(/*[out, retval]*/ unsigned long *pulCount);
	STDMETHOD(get_TransitionByIndex)(/*[in]*/ unsigned long ulIndex, /*[out, retval]*/ IUnknown **ppTransition);
	STDMETHOD(get_TransitionByName)(/*[in, string]*/ BSTR sName, /*[out, retval]*/ IUnknown **ppTransition);
    STDMETHOD(get_TransitionByNumber)(/*[in]*/ long lNumber, /*[out, retval]*/ IUnknown **ppTransition);
	STDMETHOD(get_TransitionByPosition)(/*[in]*/ long lX, /*[in]*/ long lY, /*[out, retval]*/ IUnknown **ppTransition);

    STDMETHOD(get_Language)(/*[out, retval]*/ DBILanguage *pLanguage);

protected:
    struct InstanceList
    {
        InstanceList (CTypeDBI *pType) : m_pType (pType) { }

        CTypeDBI *m_pType;

        CArray<CInstanceDBI *, CInstanceDBI *> m_Instances;
    };

    DBITypeKind m_Kind;
    bool        m_bLoaded;
    bool        m_bLoading;

    // In case of DBISimpleType
    BL_IEC_TYP m_IECType;

    // In case of DBISimpleType and m_IECType==BL_WSTRING.
    // 0 means no string size given.
    unsigned long m_ulStringLength;

    // In case of DBIArrayType
    CTypeDBI *m_pElementType;
    long     m_lLowerBound;
    long     m_lUpperBound;

    // In case of DBIAliasType
    CString m_strTypeName;
    CString m_strNamespace;
    CString m_strDBIFile;
    CTime   m_Timestamp;

    // In case of DBIProgramType, DBIFunctionBlockType or DBIStructType
    CString         m_strSource;
    CGvlFileList    m_Gvls;
    CVariableList   m_Variables;
    DBILanguage     m_Language;

    // In case of DBIProgramType or DBIFunctionBlockType
    CBreakpointList m_Breakpoints;

    // In case of DBIResourceType
    CString                                m_strResourceType;
    CArray<InstanceList *, InstanceList *> m_Instances;

    // In case of DBIStepType
    CStepList m_Steps;

    // In case of DBIActionType
    CActionList m_Actions;

    // In case of DBITransitionType
    CTransitionList m_Transitions;

    ICG_AutoDecl*  m_pIAutoDecl;
    
    CTypeDBI ();
    virtual ~CTypeDBI ();

private:
    HRESULT Load ();
    void Unload ();
    HRESULT LoadPOUs (CXMLArchive &ar);
    HRESULT LoadAttributes (CXMLArchive &ar);
    HRESULT LoadResourceAttributes (CXMLArchive &ar);
    HRESULT LoadGvls (CXMLArchive &ar);
    HRESULT LoadCustomGlobals (CXMLArchive &ar);
    HRESULT LoadVariables (CXMLArchive &ar);
    HRESULT LoadVariables (CXMLArchive &ar, DBIVariableKind kind);
    HRESULT LoadBreakpoints (CXMLArchive &ar);
    HRESULT AddVariable (CVariableDBI *pVariableDBI);
    HRESULT LoadInstances (CXMLArchive &ar);
    HRESULT LoadInstList (CTypeDBI *pType, CXMLArchive &ar);
    HRESULT AddInstance (CInstanceDBI *pInstanceDBI);
    void UnloadInstances ();
    InstanceList* LookupInstanceList (IUnknown *pType);
    // 18.09.01: SIS: lookup instance list by type name
    InstanceList* LookupInstanceList(const CString& crstrTypeName);
    HRESULT LoadSFC (CXMLArchive &ar);

    friend class CResourceDBI;
};

#endif

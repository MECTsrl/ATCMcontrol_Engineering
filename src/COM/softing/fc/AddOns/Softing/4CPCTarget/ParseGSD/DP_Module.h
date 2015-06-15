#ifndef __DP_MODULE_H_
#define __DP_MODULE_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//----  Aggregate Includes:   ------------------------------------------------*
#include "pgsd.h"       // interface definition
#include "DP_ExtParam.h"
#include "dp_unitdiag.h"
//----  Forward Class Definitions:   -----------------------------------------*
class   yy_parse;
class   CStationCallback;
//----  Defines:    ----------------------------------------------------------*

//----  Globals:    ----------------------------------------------------------*

//----  Prototypes:    -------------------------------------------------------*

//----------------------------------------------------------------------------*
/**
 *  class CDP_Module
 *  DP Slave Module representation. 
 *  The module object can be accessed via an enumerator from the station object. 
 *  The module object is represented by this class and exposes IModule,IModule2
 *  interfaces to retrieve the data of an module.
 *
 *  @see    CDP_Station.
*/
class ATL_NO_VTABLE CDP_Module : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CDP_Module, &CLSID_DP_Module>,
	public IDispatchImpl<IModule, &IID_IModule, &LIBID_PARSEGSDLib,2,0>,
	public IDispatchImpl<IModule2, &IID_IModule2, &LIBID_PARSEGSDLib,2,0>
{
    friend  yy_parse;       // needed to comfortably set private members from parser!
    friend  class gsd_Parser;

//--------------------------
// =Constructor
public:
	                        CDP_Module();
	virtual                 ~CDP_Module();
    HRESULT                 FinalConstruct();
    void                    FinalRelease();
    void                    Check(long, CStationCallback*);

//--------------------------
// =Interface map
BEGIN_COM_MAP(CDP_Module)
	COM_INTERFACE_ENTRY(IModule)
    COM_INTERFACE_ENTRY(IModule2)
    COM_INTERFACE_ENTRY2(IDispatch, IModule)
END_COM_MAP()

//--------------------------
// =Interface
// IModule
public:
	STDMETHOD(get_LenUserParams)(/*[out, retval]*/ long *pVal);
    STDMETHOD(get_HasExtUserParam)(/*[out, retval]*/ BOOL *pVal);
    STDMETHOD(get_ModName)(/*[out, retval]*/ BSTR *pVal);
    STDMETHOD(get_ConfigBytes)(/*[out, retval]*/ VARIANT *pVal);
    STDMETHOD(CreateExtUserParamEnumerator)(/*[out, retval]*/ LPUNKNOWN *pVal);
// IModule2
	STDMETHOD(get_ModuleReference)(LONG * pVal);
	STDMETHOD(get_InfoText)(BSTR * psVal);
    STDMETHOD(get_ExtUserParamCollection)(IExtUserParamCollection** pp);
    STDMETHOD(get_DiagnosticCollection)(IDiagnosticInfoCollection** pp);

private:
// = Operations

// =Attributes
    /** Name of the module */
    CString                 m_strName;
	CByteArray              m_configBytes;
    /** module reference */
    LONG                    m_lModuleReference;
    /** info text as of GSD revision 3*/
    CString                 m_strInfoText;
    /** length of ext user parameters.*/
    LONG                    m_lExtModParamLen;
    /** list of ext user parameters of the module */
    listExtUserParamObject  m_listExtUserParamObject;
    /** list of channel diagnostic definitions */
    listUnitDiag            m_listDiag;
};

typedef CComObject<CDP_Module> CDPModuleObject;
typedef CMFCObjStore<CDPModuleObject> listModuleObject;

//------------------------------------------------------------------*
/**
 *  class CEnumModules
 *      enumerate modules by IUnknown
 *
 *  @see    ....
*/
class CEnumModules :
        public CComObjectRoot,
    	public IEnumUnknown
{
//--------------------------
// =Constructor
public:
							CEnumModules();
							~CEnumModules();

//--------------------------
// =Interface map
BEGIN_COM_MAP(CEnumModules)
    COM_INTERFACE_ENTRY(IEnumUnknown)
END_COM_MAP()

//--------------------------
// =Operations
public:
	void					Initialize(listModuleObject& listModules);

//--------------------------
// =Interface
// IEnumIUnknown
	STDMETHODIMP			Next(ULONG		celt,
								 LPUNKNOWN*	ppUnk,
								 ULONG*		pceltFetched);
    STDMETHODIMP            Skip(ULONG celt);
    STDMETHODIMP            Reset(void);
	STDMETHODIMP            Clone(IEnumUnknown** ppEnumUnknown);


//--------------------------
// =Attributes
private:
    /** iterator for modules */
	ULONG                   m_pos;
    /** count of modules */
    ULONG                   m_count;
    /** list of modules in enumerator (snapshot) */								
	listModuleObject        m_listModules;
};

typedef CComObject<CEnumModules> CEnumModuleObject;

/////////////////////////////////////////////////////////////////////
#endif // __DP_MODULE_H_



#ifndef __DP_EXT_PARAM_H_
#define __DP_EXT_PARAM_H_

//----  Aggregate Includes:   ------------------------------------------------*
#include "defs.h"
#include "ExtParamDB.h"
#include "PrmTextDB.h"

//----  Forward Class Definitions:   -----------------------------------------*
class   yy_parse;

//----  Defines:    ----------------------------------------------------------*

//----  Globals:    ----------------------------------------------------------*

//----  Prototypes:    -------------------------------------------------------*


//------------------------------------------------------------------*
/**
 *  class CDP_ExtUserParam
 *      DP extended user parameter
 *
 *  @see    ....
*/
class ATL_NO_VTABLE CDP_ExtUserParam : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CDP_ExtUserParam, &CLSID_DP_ExtUserParam>,
	public IDispatchImpl<IExtUserParam, &IID_IExtUserParam, &LIBID_PARSEGSDLib,2,0>,
	public IDispatchImpl<IExtUserParam2, &IID_IExtUserParam2, &LIBID_PARSEGSDLib,2,0>
{
    friend  yy_parse;       // needed to comfortably set private members from parser!
    friend class gsd_Parser;

//--------------------------
// =Constructor
public:
	                        CDP_ExtUserParam();
	virtual                 ~CDP_ExtUserParam();
    HRESULT                 FinalConstruct();
    void                    FinalRelease();

//--------------------------
// =Interface map
BEGIN_COM_MAP(CDP_ExtUserParam)
	COM_INTERFACE_ENTRY(IExtUserParam)
    COM_INTERFACE_ENTRY(IExtUserParam2)
    COM_INTERFACE_ENTRY2(IDispatch, IExtUserParam)
END_COM_MAP()

//--------------------------
// =Interface
// IExtUserParam
public:
    STDMETHOD(get_Offset)(/*[out, retval]*/ LONG *pVal);
    STDMETHOD(get_Description)(/*[out, retval]*/ BSTR *pVal);
    STDMETHOD(get_IsConstantParam)(/*[out, retval]*/ BOOL *pVal);
    STDMETHOD(get_IsVariableEnumParam)(/*[out, retval]*/ BOOL *pVal);
    STDMETHOD(get_IsVariableMinMaxParam)(/*[out, retval]*/ BOOL *pVal);
    STDMETHOD(get_IsBitArea)(/*[out, retval]*/ BOOL *pVal);
    STDMETHOD(get_BitAreaStart)(/*[out, retval]*/ LONG *pVal);
    STDMETHOD(get_BitAreaLength)(/*[out, retval]*/ LONG *pVal);
    STDMETHOD(get_DefaultValue)(/*[out, retval]*/ LONG *pVal);
    STDMETHOD(get_MinValue)(/*[out, retval]*/ LONG *pVal);
    STDMETHOD(get_MaxValue)(/*[out, retval]*/ LONG *pVal);
    STDMETHOD(get_ConstPrmBytes)(/*[out, retval]*/ VARIANT *pVal);
    STDMETHOD(NamedEnumValues)(/*[out]*/ VARIANT *pValues, /*[out]*/ VARIANT *pNames);
    STDMETHOD(EnumValues)(/*[out]*/ VARIANT *pValues);
// IExtUserParam2
	STDMETHOD(get_Datatype)(tagGSDDatatype * pVal);
    
// =Operations
public:
    BOOL    CopyDefinitionValues(CExtParamDefinition *pDefinition);
    void    Check(CStationCallback *);

// =Attributes
private:
    // the DP GSD features:
    //----  IExtUserParam
    GSDDATATYPE             m_dataType;
                                // octets or bit area
    CExtParamDefinition::paramType  m_paramType;
                                // const, range or enums
    CString                 m_strName;
                                // textual description to this definition
    
    long                    m_lRefNum;
                                // Key into this entry

    long                    m_lOffset;
                                // starting byte offset

    long                    m_lDefaultValue;
                                // default value if not constantValue
    long                    m_lMinValue;
                                // minimal value if variableRange
    long                    m_lMaxValue;
                                // minimal value if variableRange
	CByteArray              m_usrParamBytes;
                                // octets if constantValue
    long                    m_lAreaStart;
                                // start bit if bitArea
    long                    m_lAreaLength;
                                // number of bits in bitArea
    CPrmTextDef             m_textDef;
                                // holds set of values and text strings; may be empty!
    listEnumValues          m_listEnumValues;
                                // list of enumerated values (anonymous values!)
};

typedef CComObject<CDP_ExtUserParam> CDPExtUserParamObject;
typedef CMFCObjStore<CDPExtUserParamObject> listExtUserParamObject;


//------------------------------------------------------------------*
/**
 *  class CEnumExtUserParam
 *      enumerate modules by string or IUnknown
 *
 *  @see    ....
*/
class CEnumExtUserParam :
        public CComObjectRoot,
    	public IEnumUnknown
{
//--------------------------
// =Constructor
public:
							CEnumExtUserParam();
							~CEnumExtUserParam();

//--------------------------
// =Interface map
BEGIN_COM_MAP(CEnumExtUserParam)
    COM_INTERFACE_ENTRY(IEnumUnknown)
END_COM_MAP()

//--------------------------
// =Operations
public:
	void					Initialize(listExtUserParamObject& listExtUserParams);

//--------------------------
// =Interface
// IEnumIUnknown
	STDMETHODIMP			Next(ULONG		celt,
								 LPUNKNOWN*	prgpUnk,
								 ULONG*		pceltFetched);
    STDMETHODIMP            Skip(ULONG celt);
    STDMETHODIMP            Reset(void);
	STDMETHODIMP            Clone(IEnumUnknown** ppEnumUnknown);

//--------------------------
// =Attributes
private:
    /** ext user params */
	ULONG                   m_pos;
    /** count of ext user params */
    ULONG                   m_count;
    /** list of ext user params in enumerator */
	listExtUserParamObject  m_listExtUserParams;
};

typedef CComObject<CEnumExtUserParam> CEnumExtUserParamObject;

/////////////////////////////////////////////////////////////////////
#endif //__DP_EXT_PARAM_H_



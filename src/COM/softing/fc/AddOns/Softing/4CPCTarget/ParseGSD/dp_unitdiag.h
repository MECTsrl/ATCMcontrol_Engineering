#ifndef __DP_UNITDIAG_H_
#define __DP_UNITDIAG_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "defs.h"
//----  Forward Class Definitions:   -------------------------------*
class yy_parse;
//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


//------------------------------------------------------------------*
/**
 *  class CDiagVal
 *  container for diagnostic value and associated diagnostic text
*/
class CDiagVal
{
public:
    CDiagVal();
    CDiagVal(long lValue, LPCTSTR pszDiagText);
    virtual ~CDiagVal()
    {}

    CDiagVal(const CDiagVal &);
    CDiagVal & operator =(const CDiagVal &);

    /** Unit_Diag_Area/Unit_Diag_Bit - value */
    long m_lValue;
    /** Unit_Diag_Area/Unit_Diag_Bit - assoc'd diag text */
    CString m_strDiagText;
private:
    void CopyInternal(const CDiagVal &);
};


typedef CList<CDiagVal,CDiagVal&> DiagValues;

//------------------------------------------------------------------*
/**
 *  class CDP_UnitDiag
 *  Represents unit diagnostic or channel diagnostic definition from 
 *  the GSD file. The information of a single Unit_Diag_Bit a Unit_Diag_Area 
 *  definition or a channel diagnostic definition from the GSD file 
 *  is repesented by this object. The interface IDiagnosticInfo is 
 *  used to access the information. Call get_DiagnosticCollection on 
 *  the IDP_Slave2 interface to obtain a collection of all diagnostic 
 *  definitions.
*/
class ATL_NO_VTABLE CDP_UnitDiag : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public IDispatchImpl<IDiagnosticInfo, &IID_IDiagnosticInfo, &LIBID_PARSEGSDLib,2,0>
{
    friend  yy_parse;       // needed to comfortably set private members from parser!
    friend  class gsd_Parser;

//--------------------------
// =Constructor
public:
            CDP_UnitDiag();
	virtual ~CDP_UnitDiag();

//--------------------------
// =Interface map
BEGIN_COM_MAP(CDP_UnitDiag)
COM_INTERFACE_ENTRY(IDispatch)
COM_INTERFACE_ENTRY(IDiagnosticInfo)
END_COM_MAP()

// IDiagnosticInfo
	STDMETHOD(get_DiagnosticType)(tagDiagType * pVal);
	STDMETHOD(get_BitAreaStart)(LONG * pVal);
	STDMETHOD(get_BitAreaEnd)(LONG * pVal);
	STDMETHOD(get_Values)(VARIANT * pValues, VARIANT * pText);

    void AddValue(long,LPCTSTR);
private:
    DIAG_TYPE m_Type;
    long m_lBitAreaStart;
    long m_lBitAreaEnd;
    DiagValues m_Values;
};

typedef CComObject<CDP_UnitDiag> CDP_UnitDiagObject;
typedef CMFCObjStore<CDP_UnitDiagObject> listUnitDiag;


#endif // __DP_UNITDIAG_H_



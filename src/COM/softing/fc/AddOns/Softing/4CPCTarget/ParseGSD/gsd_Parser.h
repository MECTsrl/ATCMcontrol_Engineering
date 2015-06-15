
#ifndef __GSD_PARSER_H_
#define __GSD_PARSER_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//----  Aggregate Includes:   ------------------------------------------------*
#include "PrmTextDB.h"
#include "ExtParamDB.h"
#include "Parser.h" 
#include "DP_Station.h"
#include "PrmTextDB.h"

//----  Forward Class Definitions:   -----------------------------------------*

//----  Defines:    ----------------------------------------------------------*
#define DEVICE_LEVEL        reinterpret_cast<CDPModuleObject*>(0xFFFFFFFF)

//----  Globals:    ----------------------------------------------------------*

//----  Prototypes:    -------------------------------------------------------*

class gsd_Parser : public yy_parse
///////////////////////////////////////////////////////////////////////////////
//  class gsd_Parser
//      
///////////////////////////////////////////////////////////////////////////////
{
    friend  yy_parse;
// =Construction
public:
    gsd_Parser(int sz);
    ~gsd_Parser();

private:
    // copy ctor
    gsd_Parser(gsd_Parser const & other);
    // assignment                                
    gsd_Parser const & operator = (gsd_Parser const & other);

// =Operations:
public:
    HRESULT GetDP_Station(IUnknown **pUnk);
    void PostProcess(void);
    bool CheckRevision(long lRevision);
    void SetSlaveFamily(long, CString *);
    HRESULT CreateModule(LPCTSTR pszName,CByteArray *pOctets);
    HRESULT CreatePIObject(long lInterfaceId);
    HRESULT CreateSlot(long lSlotNo, CString* pstrName, long lDefault);
    HRESULT CreateUnitDiag(DIAG_TYPE type, long lBitStart,long lBitEnd);
    HRESULT CreateChannelDiag(long lErrorType, LPCTSTR pszDiagText);
    void ClearModRefList();
    void AddModRef(long lStart, long lEnd = 0);
    void Message(HRESULT hr,LPCTSTR);

// =Helpers
private:
    void processObjectList(listExtUserParamObject* exList);
    void processMessages();
private:
    /** Station reference */
    CComObject<CDP_Station> *m_pStation;

    /** maps PrmTextRef number to correct PrmText definition */
    mapTextDefDB            m_mapTextDefDB;
    /** lookup database of extended user parameter definitions */
    CExtParamDB             m_extParamDB;
    /** if != NULL: current ext user param definition in scope */
    CExtParamDefinition    *m_pCurrUserParamDef;
    /** if != NULL and != DEVICE_LEVEL */
    CDPModuleObject        *m_pCurrModule;
    /** current physical interface object to process*/
    CDP_PIObject*           m_pPI;
    /** list of module references captured */
    CList<long,long>        m_ModRefList;
    /** current unit diagnostic definition */
    CDP_UnitDiagObject*     m_pUnitDiag;
};

#endif // __GSD_PARSER_H_



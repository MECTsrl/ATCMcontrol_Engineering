#ifndef __EXTPARAMDB_H_
#define __EXTPARAMDB_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "PrmTextDB.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

//----  Typedefs:    -----------------------------------------------*
typedef CList<LONG, LONG&>              listEnumValues;


//------------------------------------------------------------------*
/**
*  class CExtParamDefinition
*
*  @see    ....
*/
class CExtParamDefinition
{
    friend  class yy_parse;       // needed to comfortably set private members from parser!
    friend  class CDP_ExtUserParam;     // eases impl. of copyDefinitionValues
// =Construction
public:
                            CExtParamDefinition(long lReference = 0);

                                // default ctor
    virtual                 ~CExtParamDefinition() {};
                                // dtor
private:
                            CExtParamDefinition(CExtParamDefinition const & other);
                                // copy ctor
    CExtParamDefinition const &    operator = (CExtParamDefinition const & other);
                                // assignment
// =Typedefs
public:
    typedef enum {
        constantValue = 1,
        variableEnum,
        variableRange,
        referenceValue
    } paramType;

// =Access
public:
    long                    GetReference(void) const
                            {
                                return m_lReference;
                            }
    CString const &         GetDescription()
                            {
                                return m_strName;
                            }

private:
    long                    m_lReference;
                                // referenced from Ext_User_Prm_Data_Ref() / _Const() block by this value
    GSDDATATYPE             m_dataType;
                                // octets or bit area
    paramType               m_paramType;
                                // const, range or enums
    CString                 m_strName;
                                // textual description to this definition
    long                    m_lOffset;
                                // starting byte offset

    long                    m_lDefaultValue;
                                // default value if not constantValue
    long                    m_lMinValue;
                                // minimal value if variableRange
    long                    m_lMaxValue;
                                // minimal value if variableRange
    CByteArray              m_configBytes;
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

//----  Typedefs:    -----------------------------------------------*
typedef CMap<long, long&, CExtParamDefinition*, CExtParamDefinition*&>    mapParamDef;

//------------------------------------------------------------------*
/**
*  class CExtParamDB
*
*  @see    ....
*/
class CExtParamDB
{
// =Construction
public:
                            CExtParamDB();
                                // default ctor
    virtual                 ~CExtParamDB();
                                // dtor
private:
                            CExtParamDB(CExtParamDB const & other);
                                // copy ctor
    CExtParamDB const &    operator = (CExtParamDB const & other);
                                // assignment

// =Access
public:
    BOOL                    GetDescription(long lReference, CString &strDescr);
    BOOL                    GetDefinition(long lReference, CExtParamDefinition *&pItem);

// =Operations
public:
    BOOL                    AddDefinition(CExtParamDefinition *pItem);

// =Overrides
public:

protected:

// =Helpers
private:

// =Attributes
protected:

private:
    mapParamDef             m_mapParamDef;
                                // map of ExtUserParamDef (CExtParamDefinition)

};

//----  Typedefs:    -----------------------------------------------*
typedef CList<CExtParamDB*, CExtParamDB*&>                      listTextDefDB;


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.
#endif // __EXTPARAMDB_H_



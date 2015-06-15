
#if !defined(AFX_POUVAROBJECT_H__95DA6231_15A3_46FC_BFB3_1D4A9AFA8681__INCLUDED_)
#define AFX_POUVAROBJECT_H__95DA6231_15A3_46FC_BFB3_1D4A9AFA8681__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "LinkFile.h"
#include "PouVarDefs.h"


// typedef for pou objects
typedef enum _EnumIECType
{
    IEC_UNK,    // unknown
    IEC_FB,     // FunctionBlock
    IEC_FUN,    // Function
    IEC_SDT,    // Struct
    IEC_PRG     // Program
} TIECType;


// typedefs for variable objects
typedef enum _EnumEdgeModifier
{
    EDGE_NO,       // no edge modifier
    EDGE_R,     // rising edge modifier
    EDGE_F      // falling edge modifier
} TEdgeModifier;


typedef enum _EnumModifier
{
    MOD_NONE,
    MOD_RETAIN,
    MOD_NON_RETAIN,
    MOD_CONSTANT
} TModifier;

//  I,O,X,IE,OE,XE,L,G,C,E
typedef enum _EnumVarClass
{
    VARCLASS_UNKNOWN,
    VARCLASS_VAR,
    VARCLASS_INPUT,
    VARCLASS_OUTPUT,
    VARCLASS_INOUT,
    VARCLASS_INEXT,
    VARCLASS_OUTEXT,
    VARCLASS_GLOBAL,
    VARCLASS_EXTERNAL,
    VARCLASS_TEMP,
    VARCLASS_CONSTANT
} TVarClass;





// foreward declarations
class CPouObject;
class CVarObject;


//------------------------------------------------------------------*
/**
 * CPouVarObject is the base class for all objects.
 *
 * @see             
*/
class CPouVarObject  
{
public:
    // which kind of object is it
    CPouVarObject() {};
    virtual ~CPouVarObject() {};

    virtual CString GetDragAndDropString() = 0;
    virtual CString GetHeader() = 0;
    virtual void Clear() = 0;
    virtual int GetDBObjectType() = 0;

    CString  m_strName;  // the name of the object
    CString  m_strNativeType;  // the native type of the pou
   
    CString  m_strOrigin; // the library path if the pou belongs
                          // to a library, empty if the pou belongs 
                          // to the project

    // some infos for projects objects, needed to fasten updating process
    CString  m_strProjectFile;
    CString  m_strIdPath;
    CString  m_strLastParseTime;
};


//------------------------------------------------------------------*
/**
 * CPouObject holds all objects of IEC type Struct, FB, Fun, Prog.
 *
 * @see             
*/
class CPouObject : public CPouVarObject
{
public:
    CPouObject() { Clear(); };
    virtual ~CPouObject();

    TIECType    m_iecType;        // the type of the pou: FB, FUN, SDT, PRG

    CString m_strUserHelp;          // the user help file for the pou
    CString m_strHeaderFile;        // the header file for the pou
    int m_iHeaderEndPos;          // the end position in the header file
    CList<CVarObject*, CVarObject*> m_listVars;  // all vars declared in the pou,
                                   // interface vars if pou comes from a library

    BOOL InitFromLinkFile(CLinkFile* pLinkFile);
    BOOL InitFromMangledName(const CString& strMangledName);
    BOOL InitFromXMLNode(CXMLNode* pNode);

    virtual CString GetDragAndDropString();
    virtual CString GetHeader();
    virtual void Clear();

    virtual CString GetDefinition();
    virtual BOOL GetMembers(CList<CString, CString&>& listMembers);

    virtual int GetDBObjectType();

    CList<CString, CString&> m_listTargets;

};





//------------------------------------------------------------------*
/**
 * CVarObject holds all objects of type Var or Const..
 *
 * @see             
*/
class CVarObject : public CPouVarObject
{
public:
    CVarObject() { Clear(); };
    virtual ~CVarObject() {};

    TVarClass     m_varClass;  // the class of the variable: input, output ...
    TModifier     m_modifier;     // modifier: retain, constant ...

    CString m_strUserData; //can be used by class user as needed
    
    CString m_strAddress;  
    CString m_strInitValue;  
    CString m_strAlias;
    CString m_strComment;

    CString m_strDataType;
    TEdgeModifier m_edgeModifier;
    PVS_BITSET m_bsetDataType;
    BOOL m_bIsArray;
    BOOL m_bIsFBorStructDataType;

    BOOL InitFromMangledNamePart(const CString& strMangledNamePart);
    BOOL InitFromXMLNode(CXMLNode* pNode);

    virtual CString GetDragAndDropString();
    virtual CString GetHeader();
    virtual void Clear();

    CString GetDragAndDropPart();

    CString GetVarClassText();
    CString GetModifierText();
    CString GetVisModifierText();
    CString GetVarClassWithModifierText();

    int GetDBObjectType();

    static PVS_BITSET GetDataTypeCodeFromText(const CString& strDataType);
    static BOOL GetDataTypeTextFromCode(const PVS_BITSET bsetCode, 
                                        CList<CString, CString&>& listDataTypes);
    static void InitDataTypeMap();
    static BOOL m_gbDataTypeMapInit;

    static CMap<CString, LPCTSTR, PVS_BITSET, PVS_BITSET> m_gmapDataTypes;

};



#endif // !defined(AFX_POUVAROBJECT_H__95DA6231_15A3_46FC_BFB3_1D4A9AFA8681__INCLUDED_)

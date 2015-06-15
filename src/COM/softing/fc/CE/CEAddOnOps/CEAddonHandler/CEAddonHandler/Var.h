
#ifndef __VAR_H_
#define __VAR_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "keyrc.h"
#include "CESysDef.h"
#include "cedb.h"
#include "afxtempl.h"
#include "pou.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*
#define SPACETAB (_T("    "))
#define NEWLINE _T('\n')
#define BLANK	_T(' ')
#define CEPARSE_POS_INVALID  -1
#define CCONST_TYPE CVAR_TYPE_CONST

typedef enum 
{
	GVAR_TYPE,			//global variable
	GVAR_CONST_TYPE,	//global const variable 
	GVAR_RET_TYPE,		//global retain variable
	TASK_TYPE,			//task inside resource
	PRG_TYPE,			//program instance inside resource
	GVL_TYPE,			//global var list import inside resource
	VAR_TYPE,			//normal var inside program/fb
	VAR_RET_TYPE,		//retain var inside program/fb
	VAR_CONST_TYPE,		//constant var inside program/fb
	VAR_IN_TYPE,		//input var inside program/fb
	VAR_INRET_TYPE,		//input retain inside program/fb
	VAR_OUT_TYPE,		//output var inside program/fb
	VAR_OUTRET_TYPE,	//output retain var inside program/fb
	VAR_INOUT_TYPE,		//inout var inside program/fb
	VAR_EXT_TYPE,		//external var inside program/fb
	CONST_TYPE,			//constant
    GVAR_NRET_TYPE,     //global non retain var
    VAR_NRET_TYPE,      //non retain var inside program/fb
    VAR_IN_NRET_TYPE,   //non retain input var inside program/fb
    VAR_OUT_NRET_TYPE,  //non retain output var inside program/fb
    TASK_INTERVAL_TYPE, //interval of a task
    TASK_PRIO_TYPE,     //priority of a task
    STDATTRIB_TYPE,     //standard attributes
    OBJECT_TYPE,        // whole object
    PC_ADDR_TYPE,       //address of the pc
    PRG_TASKASSOC,      //task association of a program instance
    PRG_CLASS,          //class association of a program instance
  	VAR_EXTERNAL_TYPE,  //external var inside program
	INVALID_EXTYPE = -1	//invalid type
} SRCGEN_EXTTYPE;

typedef struct 
{
    SRCGEN_EXTTYPE ExtType;	//external type to resolve ambigous situations
	int iIdsKeySect;		//startkey of section iff no section available (VAR ... END_VAR)
	int iIdsKeyQualifier;	//optional qualifier string like CONSTANT
	int iIdsKeyEndSect;		//endkey of section
}SRCGEN_EXTYPE_DESC;

static SRCGEN_EXTYPE_DESC ExtTypeTbl[] = {
    {GVAR_TYPE, IDS_KEY_VARGLOBAL, 0, IDS_KEY_ENDVAR},
    {GVAR_CONST_TYPE, IDS_KEY_VARGLOBAL, IDS_KEY_CONSTANT, IDS_KEY_ENDVAR},
    {GVAR_RET_TYPE, IDS_KEY_VARGLOBAL, IDS_KEY_RETAIN, IDS_KEY_ENDVAR},
    {GVAR_NRET_TYPE, IDS_KEY_VARGLOBAL,IDS_KEY_NON_RETAIN,IDS_KEY_ENDVAR},
    {TASK_TYPE, 0, 0, 0,},
    {PRG_TYPE, 0, 0, 0},
	{GVL_TYPE, 0, 0, 0},
    {VAR_TYPE, IDS_KEY_VAR, 0, IDS_KEY_ENDVAR},
    {VAR_RET_TYPE, IDS_KEY_VAR, IDS_KEY_RETAIN, IDS_KEY_ENDVAR},
    {VAR_CONST_TYPE, IDS_KEY_VAR, IDS_KEY_CONSTANT, IDS_KEY_ENDVAR},
    {VAR_IN_TYPE, IDS_KEY_VARIN, 0, IDS_KEY_ENDVAR},
    {VAR_INRET_TYPE, IDS_KEY_VARIN, IDS_KEY_RETAIN, IDS_KEY_ENDVAR},
	{VAR_OUT_TYPE, IDS_KEY_VAROUT, 0, IDS_KEY_ENDVAR},
	{VAR_OUTRET_TYPE, IDS_KEY_VAROUT, IDS_KEY_RETAIN, IDS_KEY_ENDVAR},
	{VAR_INOUT_TYPE, IDS_KEY_VARINOUT, 0, IDS_KEY_ENDVAR},
	{VAR_EXT_TYPE, IDS_KEY_VAREXT, 0, IDS_KEY_ENDVAR},
    {VAR_NRET_TYPE,IDS_KEY_VAR,IDS_KEY_NON_RETAIN,IDS_KEY_ENDVAR},
    {VAR_IN_NRET_TYPE,IDS_KEY_VARIN,IDS_KEY_NON_RETAIN,IDS_KEY_ENDVAR},
    {VAR_OUT_NRET_TYPE,IDS_KEY_VAROUT,IDS_KEY_NON_RETAIN,IDS_KEY_ENDVAR},
	{CONST_TYPE, IDS_KEY_CONST, 0, IDS_KEY_ENDCONST},
    {TASK_INTERVAL_TYPE,0,0,0},
    {TASK_PRIO_TYPE,0,0,0},
    {STDATTRIB_TYPE,0,0,0},
    {OBJECT_TYPE,0,0,0},
    {PRG_TASKASSOC,0,0,0},
    {PRG_CLASS,0,0,0},
	{VAR_EXTERNAL_TYPE, IDS_KEY_VAR_EXTERNAL, 0, IDS_KEY_ENDVAR},
};

typedef struct
{
    int iStorageClass;
    SRCGEN_EXTTYPE ExtType; 
} CLASS_EXTTYPE_DESC;

static CLASS_EXTTYPE_DESC StorageClassTbl[] = 
{
	{CVAR_TYPE_LOCAL,VAR_TYPE},
	{CVAR_TYPE_GLOBAL,GVAR_TYPE},
	{CVAR_TYPE_IN,VAR_IN_TYPE},
	{CVAR_TYPE_OUT,VAR_OUT_TYPE},
	{CVAR_TYPE_INOUT,VAR_INOUT_TYPE},
	{CVAR_TYPE_LOCAL | CVAR_TYPE_RETAIN,VAR_RET_TYPE},
	{CVAR_TYPE_GLOBAL | CVAR_TYPE_RETAIN,GVAR_RET_TYPE},
	{CVAR_TYPE_IN | CVAR_TYPE_RETAIN,VAR_INRET_TYPE},
	{CVAR_TYPE_OUT | CVAR_TYPE_RETAIN,VAR_OUTRET_TYPE},
	{CVAR_TYPE_LOCAL | CVAR_TYPE_NONRETAIN,VAR_NRET_TYPE},
	{CVAR_TYPE_GLOBAL | CVAR_TYPE_NONRETAIN,GVAR_NRET_TYPE},
	{CVAR_TYPE_IN | CVAR_TYPE_NONRETAIN,VAR_IN_NRET_TYPE},
	{CVAR_TYPE_OUT | CVAR_TYPE_NONRETAIN,VAR_OUT_NRET_TYPE},
	{CVAR_TYPE_LOCAL | CVAR_TYPE_CONST,VAR_CONST_TYPE},
	{CVAR_TYPE_GLOBAL | CVAR_TYPE_CONST,GVAR_CONST_TYPE},
    {CCONST_TYPE, CONST_TYPE},    
	{CVAR_TYPE_EXTERNAL,VAR_EXTERNAL_TYPE}
};

//Edge detection
enum EDGE_DETECT_TYP
{
  TYP_EDGE_NOTHING,
  TYP_EDGE_F_EDGE,
  TYP_EDGE_R_EDGE
};

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*



class CVar  //: public COperation
{
protected:
    CString     m_sVarName; //Name
    CString     m_sVarAlias; //Alias
    CString     m_sVarComment; //VarComment
	CString     m_sClass; //???
    CString     m_sVarType; //BOOL, LREAL, ...
    CString     m_sAddress; //%IX0.0
    CString     m_sInitVal; //Initial value
    UINT        m_uiVarAttr;
    CString     m_sVarStorageClass; //VAR, VAR_INPUT, ...
    LONG        m_lFatherClass; //Name of the POU object
    EDGE_DETECT_TYP m_Edge; //???
    StringPos   m_IP; //Insertion point in the text string
    int  m_iVarDeclRange; //Range of the variable declaration
    CString     m_sHint; //"Add" or "Mod" or "Del" or "No"

public:
    void    GetInsertString(CString & strText, BOOL bHeader = TRUE);//, ST_FILE_TYP FileType = fileType_st);
    CVar();
	CVar(CVar & other);
    virtual ~CVar();

	CVar & operator = (CVar & other);
    BOOL operator == (CVar & other);

    void SetObjName(CString &StrName);
    CString & GetObjName();
    void SetObjAlias(CString &StrAlias);
    CString & GetObjAlias();

    void SetObjVarComment(CString &StrVarComment);
    CString & GetObjVarComment();
    
    void SetVarType(CString & strConstType);
    CString & GetVarType();

    void SetAddress(CString & strAddress);
    CString & GetAddress();

    void SetVarAttr(UINT uiVarAttr);
    UINT GetVarAttr();

    void SetVarStorageClass(CString & StrVarStorageClass);
    CString & GetVarStorageClass();
    void SetIP(StringPos IP);
    StringPos GetIP();

    void SetHint(CString & StrHint);
    CString & GetHint();
    void SetVarDeclRange(int  VarDeclRange);
    int GetVarDeclRange();

	void SetClass(CString & StrIdent);
	CString & GetClass();
	void SetVarInitVal(CString & strInitVal);
	CString & GetVarInitVal();
	
    void SetFatherClass(LONG lFatherClass);
    void SetEdge(EDGE_DETECT_TYP);
    EDGE_DETECT_TYP GetEdge();

    void AddQuotation(LPCTSTR pszText, CString & strQuotedText);
    BOOL KeysByExtType(SRCGEN_EXTTYPE ExtType, int & iIdsKeySect, int &  iIdsKeyQualifier, int & iIdsKeyEndSect);
    SRCGEN_EXTTYPE StorageClass2ExtType();
};

#endif // __VAR_H_


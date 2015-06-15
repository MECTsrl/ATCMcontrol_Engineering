


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "Var.h"
#include "Operation.h"
//----  Static Initializations:   ----------------------------------*



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CVar::CVar() : 	m_sClass(_T("")), m_sVarType(_T("")), 
                m_sInitVal(_T("")), m_sHint(_T("No"))
{
	m_lFatherClass = 0;
    m_Edge = TYP_EDGE_NOTHING;
}

CVar::CVar(CVar & other)
{
    if(this == &other)
        return;

    m_sVarName = other.m_sVarName;
    m_sVarAlias = other.m_sVarAlias;
    m_sVarComment = other.m_sVarComment;
	m_sClass = other.m_sClass;
	m_sVarType = other.m_sVarType;
	m_sAddress = other.m_sAddress;
	m_sInitVal = other.m_sInitVal;
	m_uiVarAttr = other.m_uiVarAttr;
    m_sVarStorageClass = other.m_sVarStorageClass;
	m_lFatherClass = other.m_lFatherClass;
    m_Edge = other.m_Edge;
    m_IP = other.m_IP;
    m_iVarDeclRange = other.m_iVarDeclRange;
    m_sHint = other.m_sHint;
}

CVar::~CVar()
{
}

CVar & CVar::operator = (CVar & other)
{
    if(this == &other)
    {
        return *this;
    }

    m_sVarName = other.m_sVarName;
    m_sVarAlias = other.m_sVarAlias;
    m_sVarComment = other.m_sVarComment;
	m_sClass = other.m_sClass;
    m_sAddress = other.m_sAddress;
	m_sVarType = other.m_sVarType;
	m_sInitVal = other.m_sInitVal;
	m_uiVarAttr = other.m_uiVarAttr;
    m_sVarStorageClass = other.m_sVarStorageClass;
	m_lFatherClass = other.m_lFatherClass;
    m_Edge = other.m_Edge;
    m_IP = other.m_IP;
    m_iVarDeclRange = other.m_iVarDeclRange;
    m_sHint = other.m_sHint;

	return *this; 
}

BOOL CVar::operator == (CVar & other)
{
    BOOL bRet = TRUE;

    if(this == &other)
    {
        return TRUE;
    }

    bRet = bRet && (m_sClass == other.m_sClass);
    bRet = bRet && (m_sVarType == other.m_sVarType);
    bRet = bRet && (m_sInitVal == other.m_sInitVal);
    bRet = bRet && (m_uiVarAttr == other.m_uiVarAttr);
    bRet = bRet && (m_lFatherClass == other.m_lFatherClass);
    bRet = bRet && (m_Edge == other.m_Edge);
    bRet = bRet && (m_IP = other.m_IP);
    bRet = bRet && (m_iVarDeclRange = other.m_iVarDeclRange);
    bRet = bRet && (m_sHint = other.m_sHint);

    return bRet;
}

void CVar::SetObjName(CString &StrName)
{
    m_sVarName = StrName;
}

CString & CVar::GetObjName() 
{
    return m_sVarName;
}

void CVar::SetObjAlias(CString &StrAlias)
{
    m_sVarAlias = StrAlias;
}

CString & CVar::GetObjAlias() 
{
    return m_sVarAlias;
}

void CVar::SetObjVarComment(CString &StrVarComment)
{
    m_sVarComment = StrVarComment;
}

CString & CVar::GetObjVarComment() 
{
    return m_sVarComment;
}

void CVar::SetVarType(CString & strVarType)
{
    m_sVarType = strVarType;
}

CString & CVar::GetVarType()
{
    return m_sVarType;
}

void CVar::SetAddress(CString & strAddress)
{
    m_sAddress = strAddress;
}
CString & CVar::GetAddress()
{
    return m_sAddress;
}

void CVar::SetHint(CString & strHint)
{
    m_sHint = strHint;
}

CString & CVar::GetHint()
{
    return m_sHint;
}

void CVar::SetVarDeclRange(int  VarDeclRange)
{
    m_iVarDeclRange = VarDeclRange;
}

int CVar::GetVarDeclRange()
{
    return m_iVarDeclRange;
}

void CVar::SetVarStorageClass(CString & StrVarStorageClass)
{
    m_sVarStorageClass = StrVarStorageClass;
}
void CVar::SetIP(StringPos IP)
{
    m_IP = IP;
}

StringPos CVar::GetIP()
{
    return m_IP;
}

CString & CVar::GetVarStorageClass()
{
    return m_sVarStorageClass;
}

void CVar::SetVarAttr(UINT uiVarAttr)
{
    m_uiVarAttr = uiVarAttr;
}

UINT CVar::GetVarAttr()
{
    return m_uiVarAttr;
}

void CVar::SetClass(CString & StrIdent)
{
    m_sClass = StrIdent;
}

CString & CVar::GetClass()
{
    return m_sClass;
}

void CVar::SetVarInitVal(CString & strInitVal)
{
    m_sInitVal = strInitVal;
}

CString & CVar::GetVarInitVal()
{
    return m_sInitVal;
}

void CVar::SetFatherClass(LONG lFatherClass)
{
	m_lFatherClass = lFatherClass;
}

void CVar::SetEdge(EDGE_DETECT_TYP typ)
{
    m_Edge = typ;
}

EDGE_DETECT_TYP CVar::GetEdge()
{
    return m_Edge;
}

void CVar::GetInsertString(CString & StrOut, BOOL bHeader)
{
    CString StrHelp,strAlias,strQuoted;
    //CString Strlf = _T("\r\n");
    CString StrIndent = SPACETAB;
	CString StrBlank = _T(' ');

    if(bHeader)
    {
        StrOut = StrIndent;
    }
    else
    {
        StrOut = _T("");
    }
	
	StrOut += m_sVarName;
	StrOut += StrBlank;

	if(!m_sVarAlias.IsEmpty()) 
    {
		strAlias.LoadString(IDS_KEY_COMMENT);
		StrHelp.LoadString(IDS_KEY_SYSCOMMENT);
		strAlias += StrHelp;
		AddQuotation(m_sVarAlias,strQuoted);
		StrHelp.Format(IDS_KEY_ALIAS,strQuoted);
		strAlias += StrHelp;
		StrHelp.LoadString(IDS_KEY_ENDSYSCOMMENT);
		strAlias += StrHelp;
		StrHelp.LoadString(IDS_KEY_ENDCOMMENT);
		strAlias += StrHelp;
		StrOut += strAlias;
		StrOut += StrBlank;
	}

	if(!m_sAddress.IsEmpty()) 
    {
		StrOut += _T("AT ");
		StrOut += m_sAddress;
		StrOut += StrBlank;
    }

    StrHelp.LoadString(IDS_KEY_COLON);
	StrOut += StrHelp;
	StrOut += StrBlank;
	StrOut += m_sVarType;

    //OH: Edge ???
    /*if(m_uiVarAttr & CVAR_TYPE_IN) 
    {
        //edge only if var type input
        switch(m_Edge) 
        {
            case TYP_EDGE_F_EDGE:
                StrOut += StrBlank;
                StrHelp.LoadString(IDS_KEY_FEDGE);
                StrOut += StrHelp;
                break;
            case TYP_EDGE_R_EDGE:
                StrOut += StrBlank;
                StrHelp.LoadString(IDS_KEY_REDGE);
                StrOut += StrHelp;
                break;
            case TYP_EDGE_NOTHING:
                break;
            default:
                ASSERT(!"CEOBJBASE> CVar: invalid edge type");
                break;
        }
    }*/

    if (!m_sInitVal.IsEmpty())
    {
	    StrOut += StrBlank;
	    StrHelp.LoadString(IDS_KEY_ASSIGN);
	    StrOut += StrHelp;
	    StrOut += StrBlank;
	    StrOut += m_sInitVal;
    }

	StrHelp.LoadString(IDS_KEY_SEMICOL);
    StrOut += StrHelp;

    if (!m_sVarComment.IsEmpty())
    {
        StrOut += StrBlank;
	    StrHelp.LoadString(IDS_KEY_OPENVARCOMMENT);
        StrOut += StrHelp;

        StrOut += m_sVarComment;

        StrHelp.LoadString(IDS_KEY_CLOSEVARCOMMENT);
        StrOut += StrHelp;
    }

    //StrOut += Strlf;

    return;
}

void CVar::AddQuotation(LPCTSTR pszText, CString & strQuotedText)
{
    strQuotedText = pszText;
    if (strQuotedText.GetLength() == 1 || strQuotedText.Left(1) != _T("\"") || 
        strQuotedText.Right(1) != _T("\""))
    {
        strQuotedText = _T("\"") + strQuotedText;
        strQuotedText += _T("\"");
    }
}
///////////////////////////////////////////////////
//OH: Hauptarbeit

SRCGEN_EXTTYPE CVar::StorageClass2ExtType()
{
    int nEntries  = sizeof(StorageClassTbl)/sizeof(CLASS_EXTTYPE_DESC);

    for(int i = 0;i<nEntries;i++) 
    {
		if(StorageClassTbl[i].iStorageClass == m_uiVarAttr) 
        {
			return StorageClassTbl[i].ExtType;
		}
    }
	return VAR_TYPE;
}

BOOL CVar::KeysByExtType(SRCGEN_EXTTYPE ExtType, int & iIdsKeySect, int &  iIdsKeyQualifier, int & iIdsKeyEndSect)
{
	//set ids invalid first
	iIdsKeySect = 0;
	iIdsKeyQualifier = 0; 
	iIdsKeyEndSect = 0;

    int nEntries  = sizeof(ExtTypeTbl)/sizeof(SRCGEN_EXTYPE_DESC);

    for(int i = 0;i<nEntries;i++) 
    {
        if(ExtTypeTbl[i].ExtType == ExtType) 
        {
			iIdsKeySect = ExtTypeTbl[i].iIdsKeySect;
			iIdsKeyQualifier = ExtTypeTbl[i].iIdsKeyQualifier;
			iIdsKeyEndSect = ExtTypeTbl[i].iIdsKeyEndSect;
			return TRUE;
		}
    }
	return FALSE;	
}




#include "stdafx.h"
#include "cewatchview.h"
#include "CeWatchType.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCeWatchType::CCeWatchType()
{
	Init();
}

CCeWatchType::~CCeWatchType()
{

}

void CCeWatchType::Init()
{
	m_strName.Empty();
	m_iBaseType = -1;

	m_IECType = -1;
	m_IECModifier = 0;

	SetVarKind(NoVar);

	m_ArrayLower = m_ArrayUpper = 1;
}

bool CCeWatchType::SetByName(const CString& strName)
{
	bool rv = false;

	Init();
	
	CString strName2;
	int iVarKind = -1;
	int iSep = strName.Find(';');
	if (iSep >= 0)
	{
		strName2 = strName.Left(iSep);
		iVarKind = atoi(strName.Mid(iSep+1));
	}
	else
	{
		strName2 = strName;
	}

	strName2.TrimLeft();
	strName2.TrimRight();

	// parse name, e.g.: "ARRAY [1..100] OF MyStruct"
	if (strName2.Find("ARRAY ") == 0)
	{
		int i1 = strName2.Find('[');
		int i2 = strName2.Find('.', i1+2);
		int i3 = strName2.Find(']', i2+3);
		int i4 = strName2.Find(" OF ");
		if (i1 > 0 && i2 > 0 && i3 > 0 && i4 > 0)
		{
			CString strLower = strName2.Mid(i1+1, i2-i1-1);
			CString strUpper = strName2.Mid(i2+2, i3-i2-2);
			CString strName3 = strName2.Mid(i4+4);

			strName3.TrimLeft();
			rv = SetArrayType(strName3, atoi(strLower), atoi(strUpper));
		}
	}
	else
	{
		rv = SetSimpleType(strName2);
	}
	
	if (rv && (iVarKind > NoVar && iVarKind <= Const))
		SetVarKind((ECeWatchVarKind)iVarKind);
	return rv;
}

bool CCeWatchType::Set(int iBaseType, const CString& strName)
{
	Init();

	ASSERT(iBaseType >= CEWATCH_BASETYPE_PROJECT && iBaseType <=CEWATCH_BASETYPE_SIMPLE);
	if (iBaseType >= CEWATCH_BASETYPE_PROJECT && iBaseType <= CEWATCH_BASETYPE_FUNCTIONBLOCK)
	{
		m_iBaseType = iBaseType;
		m_strName = strName;
		SetVarKind(NoVar);
		return true;
	}
	else if (iBaseType >= CEWATCH_BASETYPE_ARRAY && iBaseType <= CEWATCH_BASETYPE_SIMPLE)
	{
		return SetByName(strName);
	}
	return false;
}

bool CCeWatchType::SetUnknown()
{
    Init();

    SetVarKind(VarInOut);

    m_strName = _T("Unknown");

    return true;
}

bool CCeWatchType::SetSimpleType(const CString& strName)
{
	Init();
		
	BL_IEC_TYP iType;
	DWORD iModifier;

    BSTR bstrName = strName.AllocSysString();
	HRESULT hr = BL_TypenameToDatatype(bstrName, &iType, &iModifier);
    SysFreeString(bstrName);

	if (SUCCEEDED(hr))
	{
		m_IECType = iType;
		m_IECModifier = (short)iModifier;
	}
	else
	{
		m_IECType = -1;
		m_IECModifier = 0;
	}

	m_iBaseType = CEWATCH_BASETYPE_SIMPLE;
	SetVarKind(Var);
	m_strName = strName;	
	return true;
}


bool CCeWatchType::SetArrayType(const CString& strName, long iArrayLower, long iArrayUpper)
{
	Init();

	if (iArrayUpper < iArrayLower)
	{
		ASSERT(false);
		return false;
	}

	m_iBaseType = CEWATCH_BASETYPE_ARRAY;
	m_ArrayLower = iArrayLower;
	m_ArrayUpper = iArrayUpper;

	m_strName.Format("ARRAY [%ld..%ld] OF ", m_ArrayLower, m_ArrayUpper);
	m_strName += strName; 
		
	SetVarKind(Var);
	return true;
}

CString CCeWatchType::GetArrayTypeName() const
{
	ASSERT(IsArray());
	CString strArrayTypeName;
	if (IsArray())
	{
		int i1 = m_strName.Find(" OF ");
		ASSERT(i1 > 0);
		if (i1 > 0)
		{
			strArrayTypeName = m_strName.Mid(i1+4);		
			strArrayTypeName.TrimLeft();
		}
	}
	return strArrayTypeName;
}

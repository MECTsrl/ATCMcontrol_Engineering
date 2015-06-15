// ExtParam.cpp: implementation of the ExtParam class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConfigGSD.h"
#include "ExtParam.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ExtParam::ExtParam() :
m_dwByteOffset(0),
m_dwDefaultVal(0),
m_dwBitFieldStart(0),
m_dwBitFieldLength(0),
m_dwLowerVal(0),
m_dwUpperVal(0),
m_dwUsedVal(0),
m_bIsUsed(FALSE),
m_bBitArea(FALSE),
m_bConstantParam(FALSE),
m_bVariableEnumParam(FALSE)
{
	m_ParamType = Byte;
	m_ConstBytes.RemoveAll();
	m_NamedEnumList.RemoveAll();
}

//------------------------- COPY - CONSTRUCTOR -------------------------------*
ExtParam::ExtParam(ExtParam const& other)
{
	*this = other;	// just use assignement
}

ExtParam::~ExtParam()
{
	// delete all 'NamedEnumValues' 
	POSITION pos = m_NamedEnumList.GetHeadPosition();
	while (pos)	{
		NamedEnumValues* pElement = (NamedEnumValues*)m_NamedEnumList.GetNext(pos);
		delete pElement;
	}
}

//---------------------------- ASSIGNMENT ------------------------------------*
ExtParam const&
	ExtParam::operator = (ExtParam const & other)
{
	m_ParamType = other.m_ParamType;
	m_bIsUsed = FALSE;
	m_szDescription = other.m_szDescription;
	m_dwByteOffset = other.m_dwByteOffset;
	m_bVariableEnumParam = other.m_bVariableEnumParam;
	m_bBitArea = other.m_bBitArea;
	m_bConstantParam = other.m_bConstantParam;

	m_dwBitFieldStart = other.m_dwBitFieldStart;
	m_dwBitFieldLength = other.m_dwBitFieldLength;
	m_dwLowerVal = other.m_dwLowerVal;
	m_dwUpperVal = other.m_dwUpperVal;
	m_dwDefaultVal = other.m_dwDefaultVal;
	m_dwUsedVal = m_dwDefaultVal;
	
	CByteArray* pBytes = (CByteArray*)&(other.m_ConstBytes);
	m_ConstBytes.RemoveAll();
	m_ConstBytes.InsertAt(0, pBytes);

	m_NamedEnumList.RemoveAll();
	POSITION pos = other.m_NamedEnumList.GetHeadPosition();
	while (pos)	{
		NamedEnumValues* pElement = new NamedEnumValues(*((NamedEnumValues*)other.m_NamedEnumList.GetNext(pos)));
		m_NamedEnumList.AddTail((CObject*)pElement);
	}

	return *this;
}

//------------------------------- Function -----------------------------------+
void ExtParam::SetDescription(CString szDescription)
{
	m_szDescription = szDescription;
}

CString ExtParam::GetDescription()	{
	return m_szDescription;
}

//----------------------------- FUNCTION -------------------------------------*
void ExtParam::SetConstantBytes(unsigned char* pucArray /*=NULL*/, DWORD dwLength /*=0*/)
{
	unsigned char* pucTemp = pucArray;
	m_ConstBytes.RemoveAll();
	if (dwLength == 0 || pucArray == NULL)
		return;
	
	for (DWORD i = 0; i < dwLength; i++)	{
		m_ConstBytes.Add(*pucTemp);
		pucTemp++;
	}
}

//----------------------------- FUNCTION -------------------------------------*
void ExtParam::AddNamedEnumVal(NamedEnumValues* pElement)	{
	if (pElement != NULL) {
		m_NamedEnumList.AddTail((CObject*)pElement);
        m_bVariableEnumParam = TRUE;
        if ((LONG)pElement->Value < (LONG)m_dwLowerVal)
            m_dwLowerVal = pElement->Value;
        if ((LONG)pElement->Value > (LONG)m_dwUpperVal)
            m_dwUpperVal = pElement->Value;
    }
}

//----------------------------- FUNCTION -------------------------------------*
void ExtParam::SetDefaultValue(DWORD dwVal)	{
	m_dwDefaultVal = dwVal;
	m_dwUsedVal = m_dwLowerVal = m_dwUpperVal = dwVal;
}

//----------------------------- FUNCTION -------------------------------------*
void ExtParam::SetUsedValue(DWORD dwVal)	{
	m_dwUsedVal = dwVal;
}

//----------------------------- FUNCTION -------------------------------------*
CString ExtParam::GetState()
{
    CString retval;
    retval.Format(_T("%d;"), GetUsedValue());
    return retval;
}

//----------------------------- FUNCTION -------------------------------------*
BOOL ExtParam::SetState(LPCTSTR &buffer)
{
    DWORD dwValue = GetNextValue(buffer);
	if ((LONG)dwValue < (LONG)GetLowerValue() ||
        (LONG)dwValue > (LONG)GetUpperValue())
        return FALSE;

    if (IsVariableEnumParam()) {
		CObList* pEnumList = GetNamedEnumList();
		BOOL bOK = FALSE;
		POSITION pos = pEnumList->GetHeadPosition();
		while (pos != NULL) {
			NamedEnumValues* pEnumVal = (NamedEnumValues*)pEnumList->GetNext(pos);

			if (pEnumVal->Value == dwValue) {
                bOK = TRUE;
                break;
            }
		}
        if (!bOK)
            return FALSE;
    }

    SetUsedValue(dwValue);
    return TRUE;
}

//----------------------------- FUNCTION -------------------------------------*
LONG GetNextValue(LPCTSTR &buffer)
{
    LONG retval = _ttol(buffer);
    while (*buffer && *buffer != _T(';'))
        buffer++;
    if (*buffer)
        buffer++;
    return retval;
}


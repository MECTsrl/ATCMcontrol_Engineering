

#include "stdafx.h"
#include "cewatchview.h"
#include "CeWatchIndices.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCeWatchIndices::CCeWatchIndices()
{
	Clear();
}

CCeWatchIndices::~CCeWatchIndices()
{
	Clear();
}

void CCeWatchIndices::Clear()
{
	m_Indices.RemoveAll();
	m_IndicesAsString.Empty();
	m_BlockIndex = 0;
	m_Index = 0;
	m_iTotMin = 1;
	m_iTotMax = 1;
}

CString CCeWatchIndices::_ToString() const
{
	CString str, str2;

	for (int ii = 0; ii < m_Indices.GetSize(); ii += 2)
	{
		long iMin = m_Indices.GetAt(ii);
		long iMax = m_Indices.GetAt(ii+1);
		if (iMin != iMax)
			str2.Format("%ld..%ld", iMin, iMax);
		else
			str2.Format("%ld", iMin);
		str += str2;

		if (ii != m_Indices.GetSize()-2)
			str += CString(", ");
	}
	return str;
}

// 15.12.05 SIS >>
CString CCeWatchIndices::AsString(BOOL bWithBrackets /*=FALSE*/) const
{
	CString strIndicesAsString = m_IndicesAsString;
	if(bWithBrackets && !m_IndicesAsString.IsEmpty())
	{
		strIndicesAsString.Format(_T("[%s]"), (LPCTSTR)m_IndicesAsString);
	}
	return strIndicesAsString;
}
// 15.12.05 SIS <<


bool CCeWatchIndices::GetNextIndex(long& Index, bool bFirst)
{
	int nEntries = m_Indices.GetSize() / 2;
	if (nEntries > 0)
	{
		if (bFirst)
		{
			m_BlockIndex = 0;
			m_Index = m_Indices.GetAt(2 * m_BlockIndex);
			Index = m_Index;
			return true;
		}
		else
		{
			if (m_BlockIndex < nEntries)
			{
				m_Index++;
				if (m_Index <= m_Indices.GetAt(2 * m_BlockIndex + 1))
				{
					Index = m_Index;
					return true;
				}
				else
				{
					m_BlockIndex++;
					if (m_BlockIndex < nEntries)
					{
						m_Index = m_Indices.GetAt(2 * m_BlockIndex);
						Index = m_Index;
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool CCeWatchIndices::Inc(bool bDec/*=false*/)
{
	if (m_Indices.GetSize() > 0)
	{
		long d = m_Indices.GetAt(1) - m_Indices.GetAt(0) + 1;

		for (int ii = 0; ii < m_Indices.GetSize(); ii += 2)
		{
			long iMin = m_Indices.GetAt(ii);
			long iMax = m_Indices.GetAt(ii+1);
			long d2 = iMax - iMin;
			if (bDec)
			{
				iMin -= d;
				iMin = max(iMin, m_iTotMin);
				iMax = iMin + d2;
			}
			else
			{
				iMax += d;
				iMax = min(iMax, m_iTotMax);
				iMin = iMax - d2;
			}
			m_Indices.SetAt(ii, iMin);
			m_Indices.SetAt(ii+1, iMax);
		}
		m_IndicesAsString = _ToString();
	}
	return false;
}

bool CCeWatchIndices::Create(long iMin, long iMax, long iTotMin/*=1*/, long iTotMax/*=1*/)
{
	if (iMin <= iMax && iMin >= iTotMin && iMax <= iTotMax)
	{
		Clear();
		m_iTotMin = iTotMin;
		m_iTotMax = iTotMax;
		m_Indices.Add(iMin);
		m_Indices.Add(iMax);
		m_IndicesAsString = _ToString();
		return true;
	}
	return false;
}

bool CCeWatchIndices::Create(const CString& str, long iTotMin/*=1*/, long iTotMax/*=1*/)
{
	int i1 = str.Find('[');
	int i2 = str.ReverseFind(']');
	CString str2;
	if (i1 > 0 && i2 > i1)
	{
		str2 = str.Mid(i1 + 1, i2 - i1 - 1);
	}
	else if (i1 < 0 && i2 < 0)
		str2 = str;
	else
		return false;
	
	str2.TrimLeft();
	str2.TrimRight();
	if (str2.GetLength() == 0)
	{
		return Create(iTotMin, iTotMax, iTotMin, iTotMax);
	}

	CArray<long, long> iArray;
	if (CreateIndexArray(str2, iArray))
	{
		// check boundaries
		for (int ii = 0; ii < iArray.GetSize(); ii += 2)
		{
			if (iArray.GetAt(ii) < iTotMin || iArray.GetAt(ii+1) > iTotMax)
				return false;
		}
		
		Clear();

		m_iTotMin = iTotMin;
		m_iTotMax = iTotMax;
		m_Indices.Copy(iArray);
		m_IndicesAsString = _ToString();
		return true;
	}
	return false;
}

bool CCeWatchIndices::CreateIndexArray(const CString& str, CArray<long, long>& iArray) const
{
	iArray.RemoveAll();
	int ll = str.GetLength();
	if (ll != 0)
	{
		int i1 = 0;
		int i2 = 0;
		CString expr;
		long iMin, iMax;
		do 
		{
			i2 = str.Find(',', i1);
			if (i2 < 0)
				i2 = ll;
			expr = str.Mid(i1, i2-i1);
			if (EvalExpr(expr, iMin, iMax))
			{
				iArray.Add(iMin);
				iArray.Add(iMax);
			}
			else
			{
				iArray.RemoveAll();
				return false;
			}
			i1 = i2 + 1;	
		}
		while (i1 < ll);
	}
	return true;
}

bool CCeWatchIndices::EvalExpr(const CString& str, long& iMin, long& iMax) const
{
	int ll = str.GetLength();
	if (ll == 0) 
		return false;

	CString strMin, strMax;
	int ii = str.Find("..");
	if (ii < 0)
	{
		strMin = str;
		strMax = str;
	}
	else
	{
		if ((ii < 1) || (ii > ll-3))
			return false;

		strMin = str.Left(ii);
		strMax = str.Right(ll - ii - 2);
	}
	bool bRv = String2Int(strMin, iMin) && String2Int(strMax, iMax);
	bRv &= (iMin <= iMax);

	return  bRv;
}

bool CCeWatchIndices::String2Int(const CString& strNum, long& iNum) const
{
	iNum = 0;
	CString str = strNum;
	str.TrimLeft();
	str.TrimRight();
	int ll = str.GetLength();
	if (ll <= 0)
		return false;

	int ii = 0;
	bool bLowerZero = false;
	if (str.GetAt(0) == _T('-'))
	{
		bLowerZero = true;
		ii++;
	}

	for (; ii < ll; ii++)
	{
		TCHAR c = str.GetAt(ii);
		if (c >= _T('0') && c <= _T('9'))
			iNum = 10L * iNum + (long)(c - _T('0'));
		else
			return false;
	}
	iNum = bLowerZero ? -iNum : iNum;

	return true;
}

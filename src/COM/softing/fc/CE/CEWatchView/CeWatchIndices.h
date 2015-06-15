#if !defined(AFX_CEWATCHINDICES_H__FDFA70C1_FC64_11D2_A017_00A024363DFC__INCLUDED_)
#define AFX_CEWATCHINDICES_H__FDFA70C1_FC64_11D2_A017_00A024363DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCeWatchIndices  
{
public:
	CCeWatchIndices();
	virtual ~CCeWatchIndices();

	bool Create(const CString& str, long iTotMin=1, long iTotMax=1);
	bool Create(long iMin, long iMax, long iTotMin=1, long iTotMax=1);
	void Clear();
	bool Inc(bool bDec=false);
	// 15.12.05 SIS >>
	// added default parameter
	CString AsString(BOOL bWithBrackets = FALSE) const;
	// 15.12.05 SIS <<
	bool GetNextIndex(long& Index, bool bFirst);
	inline long GetMinIndex() const { return m_iTotMin; }
	inline long GetMaxIndex() const { return m_iTotMax; }

private:
	bool EvalExpr(const CString& str, long& iMin, long& iMax) const;
	bool String2Int(const CString& strNum, long& iNum) const;
	CString _ToString() const;
	bool CreateIndexArray(const CString& str, CArray<long, long>& iArray) const;

private:
	CArray<long, long> m_Indices;
	CString				 m_IndicesAsString;
	int					 m_BlockIndex;
	long                 m_Index;
	long                 m_iTotMin;
	long                 m_iTotMax;
};

#endif // !defined(AFX_CEWATCHINDICES_H__FDFA70C1_FC64_11D2_A017_00A024363DFC__INCLUDED_)

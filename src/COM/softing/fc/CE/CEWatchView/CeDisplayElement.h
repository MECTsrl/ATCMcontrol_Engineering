#if !defined(AFX_CEDISPLAYELEMENT_H__E92B7803_549A_11D3_A053_00A024363DFC__INCLUDED_)
#define AFX_CEDISPLAYELEMENT_H__E92B7803_549A_11D3_A053_00A024363DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCeDisplayElement  
{
public:
	CCeDisplayElement();
	virtual ~CCeDisplayElement();


	inline bool IsExpanded() const { return m_bExpanded; }
	inline void SetExpanded(bool bExpanded) { m_bExpanded = bExpanded; }

	inline CCeWatchIndices& GetWatchIndices()  { return m_Indices; }

	unsigned long GetVerticalLineMask() const {return m_VertLineMask; }
	void SetVerticalLineMask(unsigned long Mask)  { m_VertLineMask = Mask; }

	CCeWatchElement * GetWatchElement() const {return m_pVar; }
	void SetWatchElement(CCeWatchElement *pVar) { m_pVar = pVar; }

	LONG GetExpressionHandle() const {return m_hBackEnd; }
	void SetExpressionHandle(LONG handle)  { m_hBackEnd = handle; }

// serialization
	bool WriteData(CStdioFile& file) const;
	bool ReadData(CStdioFile& file);

    void SaveExpandInfo();

private :
	bool               m_bExpanded;
	CCeWatchIndices    m_Indices;
	unsigned long      m_VertLineMask;
	CCeWatchElement*   m_pVar;


	LONG m_hBackEnd; // registration handle for backend


// debug checking
public :
	unsigned m_DebugId;
};

bool IsValidDisplayElement(const CCeDisplayElement* pElem);

#endif // !defined(AFX_CEDISPLAYELEMENT_H__E92B7803_549A_11D3_A053_00A024363DFC__INCLUDED_)

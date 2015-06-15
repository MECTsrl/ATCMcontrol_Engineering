#if !defined(AFX_FCHTMLEDOPTIONS_H__E8883BA3_D581_11D2_A002_00A024363DFC__INCLUDED_)
#define AFX_FCHTMLEDOPTIONS_H__E8883BA3_D581_11D2_A002_00A024363DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FcHtmlEdOptions.h : header file
//


class CFcHtmlEdOptions
{
// Construction
public:
	CFcHtmlEdOptions();

// Attributes
public:
	inline BOOL IsGridActive() { return m_bGridOn; }
	inline unsigned GetGridHorizontal() { return m_GridHorz; }
	inline unsigned GetGridVertical() { return m_GridVert; }

	inline void SetGridActive(BOOL bActivate=TRUE) { m_bGridOn = bActivate; }
	inline void SetGridHorizontal(unsigned Horz) { m_GridHorz = Horz; }
	inline void SetGridVertical(unsigned Vert) { m_GridVert = Vert; }

// Operations
public:
	BOOL LoadOptions();
	BOOL SaveOptions();

// Implementation
public:
	virtual ~CFcHtmlEdOptions();

private:
	BOOL	 m_bGridOn;
	unsigned m_GridHorz;
	unsigned m_GridVert;

};


#endif // !defined(AFX_FCHTMLEDOPTIONS_H__E8883BA3_D581_11D2_A002_00A024363DFC__INCLUDED_)

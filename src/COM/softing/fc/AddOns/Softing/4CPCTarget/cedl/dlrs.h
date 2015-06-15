#if !defined(AFX_DLRS_H__FF101EC2_F622_11D1_89F6_0060080993FF__INCLUDED_)
#define AFX_DLRS_H__FF101EC2_F622_11D1_89F6_0060080993FF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// dlrs.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfRS DAO recordset

class CConfRS : public CDaoRecordset
{
public:
	CConfRS(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CConfRS)

// Field/Param Data
	//{{AFX_FIELD(CConfRS, CDaoRecordset)
	long	m_ConfId;
	CString	m_ConfName;
	CString	m_ResourceName;
	CString	m_Adress;
	CString	m_Version;
	//}}AFX_FIELD

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfRS)
	public:
	virtual CString GetDefaultDBName();		// Default database name
	virtual CString GetDefaultSQL();		// Default SQL for Recordset
	virtual void DoFieldExchange(CDaoFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////
// CDomRS DAO recordset

class CDomRS : public CDaoRecordset
{
public:
	CDomRS(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CDomRS)

// Field/Param Data
	//{{AFX_FIELD(CDomRS, CDaoRecordset)
	long	m_DomainId;
	long	m_ConfId;
	CString	m_DomainName;
	CString	m_VersionId;
	short	m_Flags;
	CString	m_Path;
	CString	m_DLFilePath;
	//}}AFX_FIELD

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDomRS)
	public:
	virtual CString GetDefaultDBName();		// Default database name
	virtual CString GetDefaultSQL();		// Default SQL for Recordset
	virtual void DoFieldExchange(CDaoFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
/////////////////////////////////////////////////////////////////////////////
// CSysAttrRS DAO recordset

class CSysAttrRS : public CDaoRecordset
{
public:
	CSysAttrRS(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CSysAttrRS)

// Field/Param Data
	//{{AFX_FIELD(CSysAttrRS, CDaoRecordset)
	CString	m_SysAttrName;
	CString	m_SysAttrValue;
	//}}AFX_FIELD

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSysAttrRS)
	public:
	virtual CString GetDefaultDBName();		// Default database name
	virtual CString GetDefaultSQL();		// Default SQL for Recordset
	virtual void DoFieldExchange(CDaoFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLRS_H__FF101EC2_F622_11D1_89F6_0060080993FF__INCLUDED_)

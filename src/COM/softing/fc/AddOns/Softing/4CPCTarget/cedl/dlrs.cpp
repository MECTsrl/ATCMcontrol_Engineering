// dlrs.cpp : implementation file
//

#include "stdafx.h"
#include "dlrs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfRS

IMPLEMENT_DYNAMIC(CConfRS, CDaoRecordset)

CConfRS::CConfRS(CDaoDatabase* pdb)
	: CDaoRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CConfRS)
	m_ConfId = 0;
	m_ConfName = _T("");
	m_ResourceName = _T("");
	m_Adress = _T("");
	m_Version = _T("");
	m_nFields = 5;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dbOpenDynaset;
}


CString CConfRS::GetDefaultDBName()
{
	return _T("C:\\SRC\\COM\\softing\\fc\\CE\\cedl\\CEDL_Base.mdb");
}

CString CConfRS::GetDefaultSQL()
{
	return _T("[Configuration]");
}

void CConfRS::DoFieldExchange(CDaoFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CConfRS)
	pFX->SetFieldType(CDaoFieldExchange::outputColumn);
	DFX_Long(pFX, _T("[ConfId]"), m_ConfId);
	DFX_Text(pFX, _T("[ConfName]"), m_ConfName);
	DFX_Text(pFX, _T("[ResourceName]"), m_ResourceName);
	DFX_Text(pFX, _T("[Adress]"), m_Adress);
	DFX_Text(pFX, _T("[Version]"), m_Version);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CConfRS diagnostics

#ifdef _DEBUG
void CConfRS::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void CConfRS::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}
#endif //_DEBUG
/////////////////////////////////////////////////////////////////////////////
// CDomRS

IMPLEMENT_DYNAMIC(CDomRS, CDaoRecordset)

CDomRS::CDomRS(CDaoDatabase* pdb)
	: CDaoRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CDomRS)
	m_DomainId = 0;
	m_ConfId = 0;
	m_DomainName = _T("");
	m_VersionId = _T("");
	m_Flags = 0;
	m_Path = _T("");
	m_DLFilePath = _T("");
	m_nFields = 7;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dbOpenDynaset;
}


CString CDomRS::GetDefaultDBName()
{
	return _T("C:\\SRC\\COM\\softing\\fc\\CE\\cedl\\CEDL_Base.mdb");
}

CString CDomRS::GetDefaultSQL()
{
	return _T("[Domain]");
}

void CDomRS::DoFieldExchange(CDaoFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CDomRS)
	pFX->SetFieldType(CDaoFieldExchange::outputColumn);
	DFX_Long(pFX, _T("[DomainId]"), m_DomainId);
	DFX_Long(pFX, _T("[ConfId]"), m_ConfId);
	DFX_Text(pFX, _T("[DomainName]"), m_DomainName);
	DFX_Text(pFX, _T("[VersionId]"), m_VersionId);
	DFX_Short(pFX, _T("[Flags]"), m_Flags);
	DFX_Text(pFX, _T("[Path]"), m_Path);
	DFX_Text(pFX, _T("[DLFilePath]"), m_DLFilePath);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CDomRS diagnostics

#ifdef _DEBUG
void CDomRS::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void CDomRS::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSysAttrRS

IMPLEMENT_DYNAMIC(CSysAttrRS, CDaoRecordset)

CSysAttrRS::CSysAttrRS(CDaoDatabase* pdb)
	: CDaoRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CSysAttrRS)
	m_SysAttrName = _T("");
	m_SysAttrValue = _T("");
	m_nFields = 2;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dbOpenDynaset;
}


CString CSysAttrRS::GetDefaultDBName()
{
	return _T("C:\\SRC\\COM\\softing\\fc\\CE\\cedl\\CEDL_Base.mdb");
}

CString CSysAttrRS::GetDefaultSQL()
{
	return _T("[SysAttribute]");
}

void CSysAttrRS::DoFieldExchange(CDaoFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CSysAttrRS)
	pFX->SetFieldType(CDaoFieldExchange::outputColumn);
	DFX_Text(pFX, _T("[SysAttrName]"), m_SysAttrName);
	DFX_Text(pFX, _T("[SysAttrValue]"), m_SysAttrValue);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CSysAttrRS diagnostics

#ifdef _DEBUG
void CSysAttrRS::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void CSysAttrRS::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}
#endif //_DEBUG

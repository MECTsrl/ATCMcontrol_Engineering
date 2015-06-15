
#include "stdafx.h"
#include "DBIHolder.h"
#include "DBIContext.h"
#include "ProjectDBI.h"
#include "XML_Wrapper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern void DBIAddHolder (CDBIHolder *pDBIHolder);
extern void DBIRemoveHolder (CDBIHolder *pDBIHolder);


CDBIHolder::CDBIHolder (const CString &strDebugInfoPath)
: m_strDebugInfoPath (strDebugInfoPath),
  m_pContext (NULL),
  m_pProjectDBI (NULL),
  m_uiReferences (1),
  m_bReloading (false),
  m_ProjectDBITimestamp (0)
{
    DBIAddHolder (this);
}


CDBIHolder::~CDBIHolder ()
{
    if (m_pProjectDBI != NULL)
    {
        Cleanup ();
    };

    DBIRemoveHolder (this);
}


void CDBIHolder::Reference ()
{
    ++m_uiReferences;
}


void CDBIHolder::Unreference ()
{
    ASSERT (m_uiReferences != 0);
    if (m_uiReferences == 0)
    {
        return;
    };

    if (--m_uiReferences == 0)
    {
        delete this;
    };
}


HRESULT CDBIHolder::Init ()
{
    HRESULT hr;
    CString strDBIFile;
    CFileStatus fileStatus;
    CComObject<CProjectDBI> *pProjectDBI;

    ASSERT(m_strDebugInfoPath.GetLength()>0);
    strDBIFile.Format ("%s\\%s", (LPCTSTR)m_strDebugInfoPath, PROJECT_DBI_FILE);
    
    if (CFile::GetStatus (strDBIFile, fileStatus))
    {
        m_ProjectDBITimestamp = fileStatus.m_mtime;
    };

    m_pContext = new CDBIContext (m_strDebugInfoPath);
    ASSERT (m_pContext != NULL);
    if (m_pContext == NULL)
    {
        return (E_OUTOFMEMORY);
    };

    hr = CComObject<CProjectDBI>::CreateInstance (&pProjectDBI);
    if (FAILED (hr))
    {
        m_pContext->Unreference ();
        m_pContext = NULL;

        return (hr);
    };
    ASSERT (pProjectDBI != NULL);
    if (pProjectDBI == NULL)
    {
        m_pContext->Unreference ();
        m_pContext = NULL;

        return (E_UNEXPECTED);
    };
    pProjectDBI->AddRef ();

    hr = pProjectDBI->Init (m_pContext);
    if (FAILED (hr))
    {
        pProjectDBI->Release ();

        m_pContext->Unreference ();
        m_pContext = NULL;

        return (hr);
    };

    pProjectDBI->AddParent (this);
    m_pProjectDBI = pProjectDBI;
    return (S_OK);
}


void CDBIHolder::Cleanup ()
{
    if (m_pProjectDBI != NULL)
    {
        m_pProjectDBI->RemoveParent (this);
        m_pProjectDBI->Release ();
        m_pProjectDBI = NULL;
    };

    if (m_pContext != NULL)
    {
        m_pContext->Unreference ();
        m_pContext = NULL;
    };
}


void CDBIHolder::RegisterModifiedCB (FC_CALLBACK_BASE (DebugInfoModifiedCB) *pModifiedCB)
{
    m_ModifiedCBs.Add (pModifiedCB);
}


void CDBIHolder::UnregisterModifiedCB (FC_CALLBACK_BASE (DebugInfoModifiedCB) *pModifiedCB)
{
    m_ModifiedCBs.Remove (pModifiedCB);
}


CString CDBIHolder::GetDebugInfoPath () const
{
    return (m_strDebugInfoPath);
}


CProjectDBI *CDBIHolder::GetProject ()
{
    HRESULT hr;

    hr = m_pProjectDBI->Load ();
    if (FAILED (hr))
    {
        throw (hr);
    };

    if (IsModified ())
    {
        Reload ();
    };

    return (m_pProjectDBI);
}


CTypeDBI *CDBIHolder::GetSimpleType (BL_IEC_TYP iecType)
{
    CTypeDBI *pTypeDBI;

    pTypeDBI = m_pContext->GetSimpleType (iecType);
    ASSERT (pTypeDBI != NULL);
    if (pTypeDBI == NULL)
    {
        throw (E_UNEXPECTED);
    };

    return (pTypeDBI);
}


CTypeDBI *CDBIHolder::GetConfigurationType ()
{
    CTypeDBI *pTypeDBI;

    pTypeDBI = m_pContext->GetConfigurationType ();
    ASSERT (pTypeDBI != NULL);
    if (pTypeDBI == NULL)
    {
        throw (E_UNEXPECTED);
    };

    return (pTypeDBI);
}


bool CDBIHolder::IsCorrupt ()
{
    return (m_pContext->IsCorrupt ());
}


bool CDBIHolder::IsModified ()
{
    CString strDBIFile;
    CFileStatus fileStatus;

    strDBIFile.Format ("%s\\%s", (LPCTSTR)m_strDebugInfoPath, PROJECT_DBI_FILE);

    if (!CFile::GetStatus (strDBIFile, fileStatus))
    {
        return (false);
    };

    if (fileStatus.m_mtime == m_ProjectDBITimestamp)
    {
        return (false);
    };

    return (true);
}


void CDBIHolder::Reload ()
{
    if (!m_bReloading)
    {
        m_bReloading = true;
        Cleanup ();
        Init ();
        m_bReloading = false;
        m_ModifiedCBs.Execute ();
    };
}

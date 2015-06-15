
#ifndef __DBIHOLDER_H_
#define __DBIHOLDER_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXMLArchive;
class CDBIContext;
class CProjectDBI;
class CTypeDBI;

#include "baselib.h"
#include "AnyDBI.h"
#include "fccallback.h"

FC_CALLBACK_DEF (DebugInfoModifiedCB)


/////////////////////////////////////////////////////////////////////////////
// CDBIContext

class CDBIHolder : public CAnyDBI
{
public:
    CDBIHolder (const CString &strDebugInfoPath);
    ~CDBIHolder ();

    HRESULT Init ();

    void Reference ();
    void Unreference ();

    void RegisterModifiedCB (FC_CALLBACK_BASE (DebugInfoModifiedCB) *pModifiedCB);
    void UnregisterModifiedCB (FC_CALLBACK_BASE (DebugInfoModifiedCB) *pModifiedCB);

    CString GetDebugInfoPath () const;

    CProjectDBI *GetProject ();
    CTypeDBI *GetSimpleType (BL_IEC_TYP iecType);
    CTypeDBI *GetConfigurationType ();

    bool IsCorrupt ();

    bool IsModified ();
    void Reload ();

protected:
    unsigned int            m_uiReferences;
    CString                 m_strDebugInfoPath;
    CDBIContext             *m_pContext;
    CComObject<CProjectDBI> *m_pProjectDBI;
    bool                    m_bReloading;
    CTime                   m_ProjectDBITimestamp;

    FC_CALLBACK_HOLDER (DebugInfoModifiedCB) m_ModifiedCBs;

private:
    void Cleanup ();
};

#endif

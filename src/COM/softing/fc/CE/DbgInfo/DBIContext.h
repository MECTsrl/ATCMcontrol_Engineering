
#ifndef __DBICONTEXT_H_
#define __DBICONTEXT_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CTypeDBI;
class CXMLArchive;
class CBreakpointDBI;
class CVariableDBI;

#include "baselib.h"
#include "DbgInfo.h"


/////////////////////////////////////////////////////////////////////////////
// CDBIContext

class CDBIContext
{
public:
    CDBIContext (const CString &strDebugInfoPath);

    void Reference ();
    void Unreference ();

    CString GetDebugInfoPath () const;

    CTypeDBI *GetSimpleType (BL_IEC_TYP iecType);
    CTypeDBI *GetConfigurationType ();
    CTypeDBI *GetStepType ();
    CTypeDBI *GetActionType ();
    CTypeDBI *GetTransitionType ();

    HRESULT BuildDataType (CString strText, const CString &strNamespace, const CString &strDBIFile, CTypeDBI *&pType);
    void TypeDieing (CTypeDBI *pType);

    bool IsCorrupt ();
    void Corrupt ();

protected:
    unsigned int                   m_uiReferences;
    CString                        m_strDebugInfoPath;
    CArray<CTypeDBI *, CTypeDBI *> m_UserTypes;
    CTypeDBI                       *m_SimpleTypes[BL_LASTTYPE+1];
    CTypeDBI                       *m_pStepType;
    CTypeDBI                       *m_pActionType;
    CTypeDBI                       *m_pTransitionType;
    bool                           m_bCorrupt;

    virtual ~CDBIContext ();

private:
    HRESULT BuildArrayType (CString strRangeList, const CString &strNamespace, CString strText, const CString &strDBIFile, CTypeDBI *&pType);
    HRESULT BuildSimpleType (BL_IEC_TYP iecType, CString strText, CTypeDBI *&pType);
    HRESULT BuildNamedType (CString strText, const CString &strNamespace, const CString &strName, const CString &strDBIFile, CTypeDBI *&pType);
};

#endif

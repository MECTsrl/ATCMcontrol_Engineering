#ifndef __GVLFILEDBI_H_
#define __GVLFILEDBI_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "DbgInfo.h"
#include "AnyDBI.h"

//----  Forward Class Definitions:   -------------------------------*
class CXMLArchive;
class CDBIContext;

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


/////////////////////////////////////////////////////////////////////////////
// CGvlFileDBI

class ATL_NO_VTABLE CGvlFileDBI : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CGvlFileDBI, &CLSID_GvlFileDBI>,
	public IDispatchImpl<IGvlFileDBI, &IID_IGvlFileDBI, &LIBID_DBGINFOLib>,
    public CAnyDBI
{
public:
DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CGvlFileDBI)
	COM_INTERFACE_ENTRY(IGvlFileDBI)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

public:
    HRESULT Init (CDBIContext *pContext, const CString &strFileName, const CString& strId);
    HRESULT Init (CDBIContext *pContext, CXMLArchive &ar);

    CString     GetFileName();
    CString     GetId();

    STDMETHOD(get_FileName)(/*[out, string, retval]*/ BSTR *psFileName);
    STDMETHOD(get_Id)(/*[out, string, retval]*/ BSTR *psId);

protected:
    CString     m_strId;            // ID: "1"
    CString     m_strFileName;      // file name of gvl

    CGvlFileDBI();
    virtual ~CGvlFileDBI();
    
    friend class CResourceDBI;
};


class CGvlFileList
{
public:
    ~CGvlFileList ();

    HRESULT Load (CDBIContext *pContext, CXMLArchive &ar, CAnyDBI *pParent);
    void Unload (CAnyDBI *pParent);

    unsigned long GetCount ();
    CGvlFileDBI *GetGvlFile (unsigned long ulIndex);

    CGvlFileDBI *LookupByFileName (const CString &strFileName);

    HRESULT AddGvlFile (CDBIContext *pContext, const CString &strFileName, const CString& strId, CAnyDBI *pParent);

protected:
    CArray<CGvlFileDBI*, CGvlFileDBI*> m_GvlFiles;
};

#endif // __GVLFILEDBI_H_
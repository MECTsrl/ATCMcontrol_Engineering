
#ifndef __LOWLEVELLIB_H_
#define __LOWLEVELLIB_H_

#include "resource.h"       // main symbols



/////////////////////////////////////////////////////////////////////////////
// CLibObject
/////////////////////////////////////////////////////////////////////////////
class ATL_NO_VTABLE CLibObject : 
	public CComObjectRootEx<CComSingleThreadModel>,
    public I4CLAccess
{
BEGIN_COM_MAP(CLibObject)
	COM_INTERFACE_ENTRY(I4CLAccess)
END_COM_MAP()


private:
    FC_CArchive  m_ar;
    FC_CArDir*   m_pRoot;

    HRESULT getPouDir(
        BSTR        sPOUName, 
        BSTR        sTargetType, 
        FC_CArDir** ppPOU,
        BSTR*       psErrDescr,
        bool        bForceNew
    );
public:
    CLibObject(){m_pRoot = NULL;}
    ~CLibObject(){m_ar.close(false, NULL);}
    
    HRESULT onArError(
        FC_ARSTAT    ret, 
        BSTR*        psErrDescr
    );
    HRESULT openAr(
        const TCHAR*    pszArchive, 
        bool            bWrite,
        BSTR*           psErrorDescr
    );
    HRESULT closeAr(
        BSTR* psErrDescr
    );



/////////////////////////////////////////////////////////////////////
// I4CLAccess
/////////////////////////////////////////////////////////////////////
    STDMETHOD(GetLibFileName)(
        /*[out, string]*/ BSTR* sFileName
    );
    STDMETHOD(GetLastModifiedFileTime)(
        /*[out]*/ __int64* pftLastModified
    );
    STDMETHOD(GetAllPOUs)(
        /*[out]*/ SAFEARRAY** parrPOUs
    );
    STDMETHOD(GetTargetTypesOfPOU)(
        /*[in, string]*/ BSTR        sPOUName, 
        /*[out]*/        SAFEARRAY** pasTrgTypes
    );
    STDMETHOD(ExistsPOU)(
        /*[in, string]*/ BSTR sPOUName, 
        /*[in, string]*/ BSTR sTargetType
    );
    STDMETHOD(GetFileNames)(
        /*[in, string]*/ BSTR      sPOUName,
        /*[in, string]*/ BSTR      sTargetType,
        /*[out]*/ SAFEARRAY**      parrFiles
    );
    STDMETHOD(ExistsFile)(
        /*[in, string]*/BSTR   sPOUName,
        /*[in, string]*/BSTR   sTargetType,
        /*[in, string]*/BSTR   sFileName,
        /*[full][out] */DWORD* plFileByteSize
    );
    STDMETHOD(GetFileRaw)(
        /*[in, string]*/ BSTR   sPOUName, 
        /*[in, string]*/ BSTR   sTargetType,
        /*[in, string]*/ BSTR   sFileName,
        /*[out,ptr]   */ byte*  pBytes,
        /*[in]        */ DWORD  nBytes
    );
    STDMETHOD(PutFileRaw)(
        /*[in, string]*/ BSTR  sPOUName, 
        /*[in, string]*/ BSTR  sTargetType,
        /*[in, string]*/ BSTR  sFileName,
        /*[in, ptr]*/    byte* pBytes,
        /*[in, ptr]*/    DWORD nBytes
    );
    STDMETHOD(GetFileAsBSTR)(
        /* [string][in] */ BSTR     sPOUName,
        /* [string][in] */ BSTR     sTargetType,
        /* [string][in] */ BSTR     sFileName,
        /* [out] */ BSTR __RPC_FAR *sTextFile
    );
    STDMETHOD(CheckOut)(
        /*[in, string]*/   BSTR          sPOUName,
        /*[in, string]*/   BSTR          sTargetType,
        /*[in, string]*/   BSTR          sFileSpec,
        /*[in]*/           BSTR          sCheckOutDir,
        /*[in, out,ptr]*/  SAFEARRAY**   parrFiles,
        /*[out]*/          BSTR*         sErrorDescr
    );
    STDMETHOD(CheckIn)(
        /*[in, string]*/ BSTR  sPOUName,
        /*[in, string]*/ BSTR  sTargetType,
        /*[in, string]*/ BSTR  sFileSpec,
        /*[out]*/        BSTR* sErrorDescr
    );
    STDMETHOD(DeleteFiles)(
        /*[in, string]*/ BSTR  sPOUName,
        /*[in, string]*/ BSTR  sTargetType,
        /*[in, string]*/ BSTR  sFileSpec
    );
};




/////////////////////////////////////////////////////////////////////////////
// C4CLAccessLib
/////////////////////////////////////////////////////////////////////////////
class ATL_NO_VTABLE C4CLAccessLib : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<C4CLAccessLib, &CLSID_FC4CLAccess>,
    public I4CLOpen
{
public:
DECLARE_REGISTRY_RESOURCEID(IDR_LOWLEVELLIB)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(C4CLAccessLib)
	COM_INTERFACE_ENTRY(I4CLOpen)
END_COM_MAP()


/////////////////////////////////////////////////////////////////////
// I4CLOpen
/////////////////////////////////////////////////////////////////////
public:
    STDMETHOD(Open)(
        /*[in]*/         BOOL          bWrite, 
        /*[in, string]*/ BSTR          sFileName,
        /*[out,string]*/ BSTR*         psErrorDescr,
        /*[out]*/        I4CLAccess**  ppIAcc   
    );

    STDMETHOD(Close)(
        /*[in]*/        I4CLAccess* pIAccess
    );
};


#endif //__LOWLEVELLIB_H_



///////////////////////////////////////////////////////////////////////////////
// EOF
///////////////////////////////////////////////////////////////////////////////

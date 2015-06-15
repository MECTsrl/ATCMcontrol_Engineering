

// TargetOnlineComObj.h : Declaration of the CTargetOnlineComObj

#ifndef __TARGETONLINECOMOBJ_H_
#define __TARGETONLINECOMOBJ_H_

#include "resource.h"       // main symbols

#include "XmlNodeRef.h"

/////////////////////////////////////////////////////////////////////////////
// CTargetOnlineComObj
class ATL_NO_VTABLE CTargetOnlineComObj : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public ITargetOnline,
	public IXmlNodeRef
{
    ALLOC_CHECK
public:
	CTargetOnlineComObj()
	{
	}

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTargetOnlineComObj)
COM_INTERFACE_ENTRY(ITargetOnline)
COM_INTERFACE_ENTRY(IXmlNodeRef)
END_COM_MAP()

public:
// ITargetOnline
	STDMETHOD(GetCustomConnect)(GUID *pCustomConnectId);
	STDMETHOD(GetDownload)(GUID *pDownloadId);
//	STDMETHOD(GetDownloadChanges)(GUID *pDownloadChangesId);
//	STDMETHOD(GetDownloadSelection)(GUID *pDownloadSelectionId);
    STDMETHOD(GetConsoleFlag)(BOOL* pbConsoleFlag);
    STDMETHOD(GetDebugFlag)(BOOL* pbDebugFlag);
    STDMETHOD(GetMultitaskFlag)(BOOL* pbMultitaskFlag);
    STDMETHOD(GetWarmstartFlag)(BOOL* pbWarmstartFlag);
    STDMETHOD(GetInitClearFlag)(BOOL* pbInitClearFlag);
    STDMETHOD(GetDLChangesFlag)(BOOL* pbDLChangesFlag);
    STDMETHOD(GetPreDLStep)(GUID* pPreDLStep);
    STDMETHOD(GetPostDLStep)(GUID* pPostDLStep);
    STDMETHOD(GetPreConnectStep)(GUID* pPreConnectStep);
// IXmlNodeRef
    XML_NODE_REF_DECL
};

#endif //__TARGETONLINECOMOBJ_H_


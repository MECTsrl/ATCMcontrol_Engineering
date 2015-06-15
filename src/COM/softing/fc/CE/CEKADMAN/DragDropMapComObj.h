
#ifndef __DRAGDROPMAPCOMOBJ_H_
#define __DRAGDROPMAPCOMOBJ_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "resource.h"       // main symbols
#include "CEKadMan.h"
#include "KADManager.h"
#include "XmlNodeRef.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

/////////////////////////////////////////////////////////////////////////////
// CDragDropMapComObj
class ATL_NO_VTABLE CDragDropMapComObj : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDragDropMap,
	public IXmlNodeRef
{
    ALLOC_CHECK
public:
	CDragDropMapComObj()
	{
	}

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDragDropMapComObj)
COM_INTERFACE_ENTRY(IDragDropMap)
COM_INTERFACE_ENTRY(IXmlNodeRef)
END_COM_MAP()

public:
// IDragDropMap
	STDMETHOD(GetDragDropActionEnumerator)(IEnumDragDropActions **ppEnumDragDropActions);
// IXmlNodeRef
    XML_NODE_REF_DECL
};

#endif // __DRAGDROPMAPCOMOBJ_H_


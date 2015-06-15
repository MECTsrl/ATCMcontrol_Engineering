


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

#include "stdafx.h"
#include "DragDropMapComObj.h"
#include "EnumDragDropActionsComObj.h"

/////////////////////////////////////////////////////////////////////////////
// CDragDropMapComObj

XML_NODE_REF_IMPL(CDragDropMapComObj)

STDMETHODIMP CDragDropMapComObj::GetDragDropActionEnumerator(IEnumDragDropActions **ppEnumDragDropActions)
{
    return TFCreateAndInitEnumObject<CEnumDragDropActionsComObj,IEnumDragDropActions>(
        IID_IEnumDragDropActions,IID_IDragDropAction,L"DRAGDROPACTION",
        m_XmlNodeRef.GetXmlNode(),ppEnumDragDropActions);
}



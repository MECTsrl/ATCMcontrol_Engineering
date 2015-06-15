
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*


#include "stdafx.h"
#include "DragItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CDragItem, CObject,CB_FORMAT_4CLIB_VERSION)
IMPLEMENT_SERIAL(CXMLDragItem, CObject,CB_FORMAT_4CXML_VERSION)
IMPLEMENT_SERIAL(CWatchExpressionDragItem, CObject, CB_FORMAT_4CWATCHEXPR_VERSION)
IMPLEMENT_SERIAL(CXMLNodeDragItem, CObject, CB_FORMAT_4CXMLNODE_VERSION)


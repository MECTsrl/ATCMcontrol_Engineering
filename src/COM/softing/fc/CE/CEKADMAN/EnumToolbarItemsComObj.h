// EnumToolbarItemsComObj.h : Declaration of the CEnumToolbarItemsComObj

#ifndef __ENUMTOOLBARITEMSCOMOBJ_H_
#define __ENUMTOOLBARITEMSCOMOBJ_H_

#include "resource.h"       // main symbols
#include "EnumComObj.h"
#include "ToolbarItemComObj.h"

/////////////////////////////////////////////////////////////////////////////
// CEnumToolbarItemsComObj

typedef TCEnumComObj<IEnumToolbarItems,IToolbarItem,CToolbarItemComObj>
    CEnumToolbarItemsComObj;

#endif //__ENUMTOOLBARITEMSCOMOBJ_H_

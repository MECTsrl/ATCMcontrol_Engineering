// EnumMenuItemsComObj.h : Declaration of the CEnumMenuItemsComObj

#ifndef __ENUMMENUITEMSCOMOBJ_H_
#define __ENUMMENUITEMSCOMOBJ_H_

#include "resource.h"       // main symbols
#include "EnumComObj.h"
#include "MenuItemComObj.h"

/////////////////////////////////////////////////////////////////////////////
// CEnumMenuItemsComObj

typedef TCEnumComObj<IEnumMenuItems,IMenuItem,CMenuItemComObj> 
    CEnumMenuItemsComObj;

#endif //__ENUMMENUITEMSCOMOBJ_H_

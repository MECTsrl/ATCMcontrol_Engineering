// EnumMenusComObj.h : Declaration of the CEnumMenusComObj

#ifndef __ENUMMENUSCOMOBJ_H_
#define __ENUMMENUSCOMOBJ_H_

#include "resource.h"       // main symbols
#include "EnumComObj.h"
#include "MenuComObj.h"

/////////////////////////////////////////////////////////////////////////////
// CEnumMenusComObj

typedef TCEnumComObj<IEnumMenus,IMenu,CMenuComObj>
    CEnumMenusComObj;

#endif //__ENUMMENUSCOMOBJ_H_

// EnumToolbarsComObj.h : Declaration of the CEnumToolbarsComObj

#ifndef __ENUMTOOLBARSCOMOBJ_H_
#define __ENUMTOOLBARSCOMOBJ_H_

#include "resource.h"       // main symbols
#include "EnumComObj.h"
#include "ToolbarComObj.h"

/////////////////////////////////////////////////////////////////////////////
// CEnumToolbarsComObj

typedef TCEnumComObj<IEnumToolbars,IToolbar,CToolbarComObj> 
    CEnumToolbarsComObj;

#endif //__ENUMTOOLBARSCOMOBJ_H_

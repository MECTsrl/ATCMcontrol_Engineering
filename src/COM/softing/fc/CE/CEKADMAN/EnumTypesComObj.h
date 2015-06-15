// EnumTypesComObj.h : Declaration of the CEnumTypesComObj

#ifndef __ENUMTYPESCOMOBJ_H_
#define __ENUMTYPESCOMOBJ_H_

#include "resource.h"       // main symbols
#include "EnumComObj.h"
#include "TypeComObj.h"

/////////////////////////////////////////////////////////////////////////////
// CEnumTypesComObj

typedef TCEnumComObj<IEnumTypes,IType,CTypeComObj>
    CEnumTypesComObj;

#endif //__ENUMTYPESCOMOBJ_H_

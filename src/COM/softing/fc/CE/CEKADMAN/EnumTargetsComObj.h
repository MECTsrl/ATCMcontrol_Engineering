// EnumTargetComObj.h : Declaration of the CEnumTargetComObj

#ifndef __ENUMTARGETCOMOBJ_H_
#define __ENUMTARGETCOMOBJ_H_

#include "resource.h"       // main symbols
#include "EnumComObj.h"
#include "TargetComObj.h"

/////////////////////////////////////////////////////////////////////////////
// CEnumTargetComObj

typedef TCEnumComObj<IEnumTargets,ITarget,CTargetComObj>
    CEnumTargetsComObj;

#endif //__ENUMTARGETCOMOBJ_H_

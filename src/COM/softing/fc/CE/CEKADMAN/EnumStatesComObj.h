// EnumStatesComObj.h : Declaration of the CEnumStatesComObj

#ifndef __ENUMSTATESCOMOBJ_H_
#define __ENUMSTATESCOMOBJ_H_

#include "resource.h"       // main symbols
#include "EnumComObj.h"
#include "StateComObj.h"

/////////////////////////////////////////////////////////////////////////////
// CEnumStatesComObj

typedef TCEnumComObj<IEnumStates,IState,CStateComObj>
    CEnumStatesComObj;

#endif //__ENUMSTATESCOMOBJ_H_

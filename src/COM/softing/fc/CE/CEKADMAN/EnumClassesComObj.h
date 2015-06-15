// EnumClassesComObj.h : Declaration of the CEnumClassesComObj

#ifndef __ENUMCLASSESCOMOBJ_H_
#define __ENUMCLASSESCOMOBJ_H_

#include "resource.h"       // main symbols
#include "EnumData.h"
#include "EnumComObj.h"
#include "ClassComObj.h"

/////////////////////////////////////////////////////////////////////////////
// CEnumClassesComObj

typedef TCEnumComObj<IEnumClasses,IClass,CClassComObj>
    CEnumClassesComObj;

#endif //__ENUMCLASSESCOMOBJ_H_

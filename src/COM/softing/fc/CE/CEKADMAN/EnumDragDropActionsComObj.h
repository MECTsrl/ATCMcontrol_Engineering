#ifndef __ENUMDRAGDROPACTIONSCOMOBJ_H_
#define __ENUMDRAGDROPACTIONSCOMOBJ_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

#include "resource.h"       // main symbols
#include "EnumComObj.h"
#include "DragDropActionComObj.h"

/////////////////////////////////////////////////////////////////////////////
// CEnumDragDropActionsComObj

typedef TCEnumComObj<IEnumDragDropActions,IDragDropAction,CDragDropActionComObj> 
    CEnumDragDropActionsComObj;

#endif // __ENUMDRAGDROPACTIONSCOMOBJ_H_



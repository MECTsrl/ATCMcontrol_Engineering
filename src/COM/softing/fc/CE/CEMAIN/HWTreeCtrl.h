#ifndef __HWTREECTRL_H_
#define __HWTREECTRL_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "NavTreeControl.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

class CHWTreeCtrl : public CNavTreeCtrl
{
public:
    // return view index (2 for hardware view)
    int GetViewIndex();

};

#endif // __HWTREECTRL_H_


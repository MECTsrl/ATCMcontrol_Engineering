#ifndef __CLASSTREECTRL_H_
#define __CLASSTREECTRL_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "NavTreeControl.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

class CClassTreeCtrl : public CNavTreeCtrl
{
public:
    //CClassTreeCtrl() {};
    //virtual ~CClassTreeCtrl() {};

    // returns tree index (1 for class view)
    int GetViewIndex();

protected:

};

#endif // __CLASSTREECTRL_H_
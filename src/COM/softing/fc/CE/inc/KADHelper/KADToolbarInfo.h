#ifndef __KADTOOLBARINFO_H_
#define __KADTOOLBARINFO_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "KADToolbar.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

class CKADToolbarInfo
{
public:
    CKADToolbarInfo();
    ~CKADToolbarInfo();

    void AddToolbar(CKADToolbar* pToolbar);

    CKADToolbar* GetFirstToolbar();
    CKADToolbar* GetNextToolbar();

public:
    CList<CKADToolbar*, CKADToolbar*>   m_tToolbarList;
protected:
    POSITION    m_tPos;
};

#endif // __KADTOOLBARINFO_H_


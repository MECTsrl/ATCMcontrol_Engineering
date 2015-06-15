
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"

#include "KADToolbarInfo.h"
#include "KADToolbar.h"

//----  Static Initializations:   ----------------------------------*


CKADToolbarInfo::CKADToolbarInfo()
{
    m_tPos = 0;
}

CKADToolbarInfo::~CKADToolbarInfo()
{
    CKADToolbar*    pToolbar;
    // delete all toolbars
    POSITION    pos = m_tToolbarList.GetHeadPosition();
    while(pos)
    {
        pToolbar = m_tToolbarList.GetNext(pos);
        ASSERT(pToolbar);
        if(pToolbar)
        {
            delete pToolbar;
        }
    }
    m_tToolbarList.RemoveAll();
}

void CKADToolbarInfo::AddToolbar(CKADToolbar* pToolbar)
{
    m_tToolbarList.AddTail(pToolbar);
}

CKADToolbar* CKADToolbarInfo::GetFirstToolbar()
{
    m_tPos = m_tToolbarList.GetHeadPosition();

    return(GetNextToolbar());
}

CKADToolbar* CKADToolbarInfo::GetNextToolbar()
{
    if(!m_tPos)
    {
        return NULL;
    }
    return(m_tToolbarList.GetNext(m_tPos));
}


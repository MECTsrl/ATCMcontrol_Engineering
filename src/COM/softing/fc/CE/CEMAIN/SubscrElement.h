
#ifndef __SUBSCRELEMENT_H_
#define __SUBSCRELEMENT_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "Subscription.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

/////////////////////////////////////////////////////////////////////////////
// CSubscrElement subscription container for identifiing and managing
// variable subscriptions
//
class CSubscrElement
{
    CSubscription*  m_pSubscr;
    CString         m_strIdPath;
    bool            m_bResourceState;

public:
    CSubscrElement();
    CSubscrElement(CSubscription * pScr, const CString& crstrIdPath, bool bResourceState = false);
    CSubscrElement(const CSubscrElement& other);
    ~CSubscrElement(void);

    const CSubscrElement& operator=(const CSubscrElement& other);

    bool operator==(const CSubscrElement& other) const;
    
    inline CSubscription* GetSubscription()
    {
        return m_pSubscr;
    }

    inline const CString& GetIdPath()
    {   
        return m_strIdPath;
    }

    inline void SetIdPath(const CString& crstrIdPath)
    {
        m_strIdPath = crstrIdPath;
    }

    inline bool IsResourceState ()
    {
        return (m_bResourceState);
    }

    inline void SetResourceState (bool bResourceState)
    {
        m_bResourceState = bResourceState;
    }
};


#endif // __SUBSCRELEMENT_H_


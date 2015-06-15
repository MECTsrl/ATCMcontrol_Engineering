


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "SubscrElement.h"

//----  Static Initializations:   ----------------------------------*

/////////////////////////////////////////////////////////////////////////////
// CSubscrElement subscription container for identifiing and managing
// variable subscriptions
//
CSubscrElement::CSubscrElement()
{
    m_pSubscr = NULL;
    m_bResourceState = false;
}

CSubscrElement::CSubscrElement(CSubscription * pScr, const CString& crstrIdPath, bool bResourceState)
{
    m_pSubscr = pScr;
    m_strIdPath = crstrIdPath;
    m_bResourceState = bResourceState;
}

CSubscrElement::~CSubscrElement()
{
    if(m_pSubscr)
    {
        m_pSubscr->Unreference();
        m_pSubscr = NULL;
    }
}

CSubscrElement::CSubscrElement(const CSubscrElement& other)
{
    if(&other == this)
    {
        return;
    }

    m_strIdPath = other.m_strIdPath;

    if(other.m_pSubscr)
    {
        //increase this first
        other.m_pSubscr->Reference();
    }

    if(m_pSubscr)
    {
        m_pSubscr->Unreference();
        m_pSubscr = NULL;
    }

    m_pSubscr = other.m_pSubscr;

    m_bResourceState = other.m_bResourceState;
}

const CSubscrElement& CSubscrElement::operator=(const CSubscrElement& other)
{
    if(&other == this) {
        return *this;
    }

    m_strIdPath = other.m_strIdPath;

    if(other.m_pSubscr)
    {
        //increase this first
        other.m_pSubscr->Reference();
    }

    if(m_pSubscr) 
    {
        m_pSubscr->Unreference();
        m_pSubscr = NULL;
    }

    m_pSubscr = other.m_pSubscr;

    m_bResourceState = other.m_bResourceState;
    return *this;
}

bool CSubscrElement::operator==(const CSubscrElement& other) const
{
    if(m_pSubscr != other.m_pSubscr)
    {
        return false;
    }

    if(m_strIdPath != other.m_strIdPath)
    {
        return false;
    }

    if(m_bResourceState != other.m_bResourceState)
    {
        return false;
    }
    return true;
}


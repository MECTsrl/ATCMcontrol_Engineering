#include "stdafx.h"
#include "AnyDBI.h"
#include "DBIContext.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CAnyDBI::CAnyDBI ()
: m_pContext (NULL)
{
}


CAnyDBI::~CAnyDBI ()
{
    if (m_pContext != NULL)
    {
        m_pContext->Unreference ();
    };
}


void CAnyDBI::SetContext (CDBIContext *pContext)
{
    if (m_pContext != NULL)
    {
        m_pContext->Unreference ();
        m_pContext = NULL;
    };

    if (pContext != NULL)
    {
        pContext->Reference ();
        m_pContext = pContext;
    };
}


void CAnyDBI::AddParent (CAnyDBI *pParent)
{
    ASSERT (m_Parents.Find (pParent) == NULL);
    if (m_Parents.Find (pParent) != NULL)
    {
        return;
    };

    m_Parents.AddTail (pParent);
}


void CAnyDBI::RemoveParent (CAnyDBI *pParent)
{
    POSITION pPosition;

    pPosition = m_Parents.Find (pParent);
    ASSERT (pPosition != NULL);
    if (pPosition == NULL)
    {
        return;
    };

    m_Parents.RemoveAt (pPosition);
}


void CAnyDBI::Corrupt ()
{
    if (m_pContext != NULL)
    {
        m_pContext->Corrupt ();
    };
}

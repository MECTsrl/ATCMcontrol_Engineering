


#include "stdafx.h"
#include "Watchpoint.h"
#include "Subscription.h"


CWatchpoint::CWatchpoint (const CStdEditorPosition &position, long lHandle)
: m_lHandle (lHandle),
  m_bExpressionWP (false),
  m_Position (position),
  m_lWatchHandle (0),
  m_IECType (BL_ANY),
  m_pClientData (NULL),
  m_bSuspended (false)
{
}


CWatchpoint::CWatchpoint (const CStdEditorPosition &startPosition, const CStdEditorPosition &endPosition, long lHandle)
: m_lHandle (lHandle),
  m_bExpressionWP (true),
  m_Position (startPosition),
  m_EndPosition (endPosition),
  m_lWatchHandle (0),
  m_IECType (BL_ANY),
  m_pClientData (NULL),
  m_bSuspended (false)
{
}


CWatchpoint::~CWatchpoint ()
{
    (void)DetachWatchItem ();
}


long CWatchpoint::GetHandle () const
{
    return (m_lHandle);
}


bool CWatchpoint::IsExpressionWP () const
{
    return (m_bExpressionWP);
}


CStdEditorPosition CWatchpoint::GetPosition () const
{
   ASSERT (!m_bExpressionWP);
   return (m_Position);
}


CStdEditorPosition CWatchpoint::GetStartPosition () const
{
   ASSERT (m_bExpressionWP);
   return (m_Position);
}


CStdEditorPosition CWatchpoint::GetEndPosition () const
{
   ASSERT (m_bExpressionWP);
   return (m_EndPosition);
}


LONG CWatchpoint::GetWatchItem () const
{
   return (m_lWatchHandle);
}


BL_IEC_TYP CWatchpoint::GetIECType () const
{
   return (m_IECType);
}


void *CWatchpoint::GetClientData () const
{
   return (m_pClientData);
}


void CWatchpoint::SetClientData (void *pClientData)
{
   m_pClientData = pClientData;
}


void CWatchpoint::AttachWatchItem (LONG lHandle, BL_IEC_TYP iecType)
{
    DetachWatchItem ();
    m_lWatchHandle = lHandle;
    m_IECType = iecType;
}


LONG CWatchpoint::DetachWatchItem ()
{
    LONG lHandle;

    lHandle = m_lWatchHandle;

    m_lWatchHandle = 0;
    m_IECType = BL_ANY;

    return (lHandle);
}


bool CWatchpoint::IsSuspended () const
{
	return (m_bSuspended);
}


HRESULT CWatchpoint::Suspend ()
{
	if (m_bSuspended)
	{
		return (S_FALSE);
	};

	m_bSuspended = true;

	return (S_OK);
}


HRESULT CWatchpoint::Resume ()
{
	if (!m_bSuspended)
	{
		return (S_FALSE);
	};

	m_bSuspended = false;

	return (S_OK);
}

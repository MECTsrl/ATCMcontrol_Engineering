/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/CpSessionNotification.h 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CpSessionNotification.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/CpSessionNotification.h $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CSC
 *
 * =COMPONENT           CSC Online Control
 *
 * =CURRENT      $Date: 28.02.07 19:00 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *





 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  15.03.2001  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/
#ifndef _C_PT_SESSION_NOTIFICATION_H_
#define _C_PT_SESSION_NOTIFICATION_H_

//////////////////////////////////////////////////////////////////////////////
// CProxy_FCSessionNotification
template <class T>
class CProxy_FCSessionNotification : public IConnectionPointImpl<T, &IID__FCSessionNotification, CComDynamicUnkArray>
{
public:

//_FCSessionNotification : IDispatch
public:
    //------------------------------------------------------------------*
    /**
    *   fire callback to client, that any of the session states has changed
    *   @param  void
    *   @return void
    */
	void Fire_OnChanged()
	{
		T* pT = (T*)this;
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			_FCSessionNotification* p_FCSessionNotification = reinterpret_cast<_FCSessionNotification*>(sp.p);
			if (p_FCSessionNotification != NULL)
			{
				p_FCSessionNotification->OnChanged();
			}
		}
	}

    //------------------------------------------------------------------*
    /**
    *   fire callback to client, that given session's state has changed
    *   @param  pSession    ptr to IUnknown of session which has changed
    *   @param  newState    new state value
    *   @return void
    */
	void Fire_OnStateChanged(
		IUnknown * pSession,
		tagCSC_SESSION_STATE newState,
        HRESULT hrError)
	{
		T* pT = (T*)this;
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			_FCSessionNotification* p_FCSessionNotification = reinterpret_cast<_FCSessionNotification*>(sp.p);
			if (p_FCSessionNotification != NULL)
			{
				p_FCSessionNotification->OnStateChanged(pSession, newState, hrError);
			}
		}
	}
};

#endif //_C_PT_SESSION_NOTIFICATION_H_
/*
 *----------------------------------------------------------------------------*
 *  $History: CpSessionNotification.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 2  *****************
 * User: Su           Date: 15.03.01   Time: 19:42
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * New header
 *==
 *----------------------------------------------------------------------------*
*/

/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/CpSimpleMsgNotification.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: CpSimpleMsgNotification.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/CpSimpleMsgNotification.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlLibServ
 *
 * =CURRENT 	 $Date: 28.02.07 19:06 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _CPSIMPLEMSGNOTIFICATION_H_
#define _CPSIMPLEMSGNOTIFICATION_H_

template <class T>
    class CProxy_ICERemoteMessageEvent :
        public IConnectionPointImpl<T, &IID__ICERemoteMessageEvent, CComDynamicUnkArray>
{
public:
    BOOL Is_Proxy_Connected()
    {
        BOOL retval = FALSE;
        T* pT = static_cast<T*>(this);
        pT->Lock();
        int i = m_vec.GetSize();
        while (i-->0)
            if (m_vec.GetAt(i) != NULL)
                retval = TRUE;
        pT->Unlock();
        return retval;
    }

//_FCSimpleMessageNotification : IDispatch
public:
    HRESULT Fire_OnNewMessage(BSTR sDescr)
    {
		T* pT = static_cast<T*>(this);
		int nConnections;
		CComPtr<IUnknown> *sp;
        pT->Lock();
        nConnections = m_vec.GetSize();
        sp = new CComPtr<IUnknown>[nConnections];
        if (!sp)
        {
            pT->Unlock();
            return E_FAIL;
        }
		for (int i = 0; i < nConnections; i++)
            sp[i] = m_vec.GetAt(i);
        pT->Unlock();
        HRESULT ret = E_FAIL;
		while (nConnections-->0)
		{
			_ICERemoteMessageEvent* p_ICEMessageEvent = reinterpret_cast<_ICERemoteMessageEvent*>(sp[nConnections].p);
            if (p_ICEMessageEvent != NULL) {
                ret = p_ICEMessageEvent->OnNewMessage(sDescr);
                if (FAILED(ret))
                {
                    TRACE(_T("Fire_OnNewMessage(): callback returned error 0x%08X; is removed!\n"), ret);
                    TrcPrint(TRC_ERROR, _T("Fire_OnNewMessage(): callback returned error 0x%08X; is removed!\n"), ret);
                    pT->Lock();
                    for (IUnknown **pp = m_vec.begin(); pp<m_vec.end(); pp++)
                        if ((IUnknown *)p_ICEMessageEvent == *pp)
                            if (m_vec.Remove(m_vec.GetCookie(pp)))
                                p_ICEMessageEvent->Release();
                    pT->Unlock();
                }
            }
        }
        delete []sp;
        return ret;
    }
};

#endif

/* ---------------------------------------------------------------------------- */

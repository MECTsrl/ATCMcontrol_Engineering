/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/CpBreakpointNotification.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: CpBreakpointNotification.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/CpBreakpointNotification.h $
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

#ifndef __CPBREAKPOINTNOTIFICATION_H_
#define __CPBREAKPOINTNOTIFICATION_H_

template <class T>
    class CProxy_FCRemoteBreakpointNotification :
        public IConnectionPointImpl<T, &IID__FCRemoteBreakpointNotification, CComDynamicUnkArray>
{
	//Warning this class may be recreated by the wizard.
public:
	VOID Fire_OnBreakpointReached(BSTR sTaskName, LONG bpQuality, SAFEARRAY *arrsInstances, SAFEARRAY *arrlBreakpointNumber)
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
            return;
        }
		for (int i = 0; i < nConnections; i++)
            sp[i] = m_vec.GetAt(i);
        pT->Unlock();
		while (nConnections-->0)
		{
			_FCRemoteBreakpointNotification* p_FCBreakpointNotification = reinterpret_cast<_FCRemoteBreakpointNotification*>(sp[nConnections].p);
            if (p_FCBreakpointNotification != NULL) {
				p_FCBreakpointNotification->OnBreakpointReached(sTaskName, bpQuality, arrsInstances, arrlBreakpointNumber);
                if (!CoIsHandlerConnected(p_FCBreakpointNotification))
                {
                    TRACE(_T("Fire_OnBreakpointReached(): callback disconnected; is removed!\n"));
                    TrcPrint(TRC_ERROR, _T("Fire_OnBreakpointReached(): callback disconnected; is removed!\n"));
                    pT->Lock();
                    for (IUnknown **pp = m_vec.begin(); pp<m_vec.end(); pp++)
                        if ((IUnknown *)p_FCBreakpointNotification == *pp)
                            if (m_vec.Remove(m_vec.GetCookie(pp)))
                                p_FCBreakpointNotification->Release();
                    pT->Unlock();
                }
            }
		}
        delete []sp;
	}

	VOID Fire_OnConfigurationChanged()
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
            return;
        }
		for (int i = 0; i < nConnections; i++)
            sp[i] = m_vec.GetAt(i);
        pT->Unlock();
		while (nConnections-->0)
		{
			_FCRemoteBreakpointNotification* p_FCBreakpointNotification = reinterpret_cast<_FCRemoteBreakpointNotification*>(sp[nConnections].p);
            if (p_FCBreakpointNotification != NULL) {
				p_FCBreakpointNotification->OnConfigurationChanged();
                if (!CoIsHandlerConnected(p_FCBreakpointNotification))
                {
                    TRACE(_T("Fire_OnConfigurationChanged(): callback disconnected; is removed!\n"));
                    TrcPrint(TRC_ERROR, _T("Fire_OnConfigurationChanged(): callback disconnected; is removed!\n"));
                    pT->Lock();
                    for (IUnknown **pp = m_vec.begin(); pp<m_vec.end(); pp++)
                        if ((IUnknown *)p_FCBreakpointNotification == *pp)
                            if (m_vec.Remove(m_vec.GetCookie(pp)))
                                p_FCBreakpointNotification->Release();
                    pT->Unlock();
                }
            }
		}
        delete []sp;
	}

};

#endif

/* ---------------------------------------------------------------------------- */

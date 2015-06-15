/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/CpCommissioningMgtNotification.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: CpCommissioningMgtNotification.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/CpCommissioningMgtNotification.h $
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

#ifndef _CPCOMMISSIONINGMGTNOTIFICATION_H_
#define _CPCOMMISSIONINGMGTNOTIFICATION_H_

template <class T>
    class CProxy_FCCommissioningMgtNotification :
       public IConnectionPointImpl<T, &IID__FCCommissioningMgtNotification, CComDynamicUnkArray>
{
public:

//_FCCommissioningMgtNotification : IDispatch
public:
    HRESULT Fire_OnUserLoggedin(
        BSTR sUserName,
        BSTR sEngMachineName,
        long bLoggingIn)
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
        HRESULT ret = S_OK;
		while (nConnections-->0)
        {
             _FCCommissioningMgtNotification* p_FCCommissioningMgtNotification = reinterpret_cast<_FCCommissioningMgtNotification*>(sp[nConnections].p);
             if (p_FCCommissioningMgtNotification != NULL) {
                ret = p_FCCommissioningMgtNotification->OnUserLoggedin(sUserName, sEngMachineName, bLoggingIn);
                if (FAILED(ret))
                {
                    TRACE(_T("Fire_OnUserLoggedin(): callback returned error 0x%08X; is removed!\n"), ret);
                    TrcPrint(TRC_ERROR, _T("Fire_OnUserLoggedin(): callback returned error 0x%08X; is removed!\n"), ret);
                    pT->Lock();
                    for (IUnknown **pp = m_vec.begin(); pp<m_vec.end(); pp++)
                        if ((IUnknown *)p_FCCommissioningMgtNotification == *pp)
                            if (m_vec.Remove(m_vec.GetCookie(pp)))
                                p_FCCommissioningMgtNotification->Release();
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

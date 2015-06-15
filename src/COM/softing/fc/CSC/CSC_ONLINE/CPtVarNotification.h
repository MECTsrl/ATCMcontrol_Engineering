/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/CPtVarNotification.h 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CPtVarNotification.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/CPtVarNotification.h $
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
#ifndef _C_PT_VAR_NOTIFICATION_H_
#define _C_PT_VAR_NOTIFICATION_H_

//////////////////////////////////////////////////////////////////////////////
// CProxy_FCVariableNotification
template <class T>
class CProxy_FCVariableNotification : public IConnectionPointImpl<T, &IID__FCVariableNotification, CComDynamicUnkArray>
{
public:
//methods:
//_FCVariableNotification : IDispatch
public:
    //------------------------------------------------------------------*
    /**
    *   fire callback to client, that any of the variables has changed
    *   @param  void
    *   @return void
    */
    void Fire_OnChanged()
    {
        HRESULT hr;
        int nConnectionIndex;
        int nConnections = m_vec.GetSize();
		
        for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
        {
            pT->Lock();
            CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
            pT->Unlock();
            IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
            if (pDispatch != NULL)
            {
                DISPPARAMS disp = { NULL, NULL, 0, 0 };
                hr = pDispatch->Invoke(0x1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL);
            }
        }
    }


    //------------------------------------------------------------------*
    /**
    *   fire callback to client, that one variable has changed
    *       send new values with it
    *   @param  hServer     server (proxy) supplied handle of variable
    *   @param  newVal      new value
    *   @param  Quality     new quality
    *   @param  Date        date of value/quality change
    *   @return HRESULT
    */
	void Fire_OnNewData(
		long hServer,
		VARIANT newVal,
		short Quality,
		DATE Date)
	{
		VARIANTARG pvars[4];
		T* pT = (T*)this;
        int nConnectionIndex;
        int nConnections = m_vec.GetSize();
		
        for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
        {
            pT->Lock();
            CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
            pT->Unlock();
            IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
            if (pDispatch != NULL)
            {
                pvars[3].vt = VT_I4;
                pvars[3].lVal= hServer;
                pvars[2] = newVal;
                pvars[1].vt = VT_I2;
                pvars[1].iVal= Quality;
                pvars[0].vt = VT_DATE;
                pvars[0].date= Date;
                DISPPARAMS disp = { pvars, NULL, 4, 0 };
                pDispatch->Invoke(0x2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL);
            }
        }
    }


    //------------------------------------------------------------------*
    /**
    *   fire callback to client when connection to variables was dropped
    *   @param  void
    *   @return void
    */
    void Fire_OnConnDropped()
	{
        HRESULT hr;
        T* pT = (T*)this;
        int nConnectionIndex;
        int nConnections = m_vec.GetSize();
		
        for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
        {
            pT->Lock();
            CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
            pT->Unlock();
            IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
            if (pDispatch != NULL)
            {
                DISPPARAMS disp = { NULL, NULL, 0, 0 };
                hr = pDispatch->Invoke(0x3, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL);
            }
        }
    }
};

#endif //_C_PT_VAR_NOTIFICATION_H_

/*
 *----------------------------------------------------------------------------*
 *  $History: CPtVarNotification.h $
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

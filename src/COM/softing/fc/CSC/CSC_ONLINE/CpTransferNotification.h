/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/CpTransferNotification.h 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CpTransferNotification.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/CpTransferNotification.h $
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
#ifndef __CPTRANSFERNOTIFICATION_H_
#define __CPTRANSFERNOTIFICATION_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*



//////////////////////////////////////////////////////////////////////////////
// CProxy_FCTransferNotification
template <class T>
class CProxy_FCTransferNotification : 
    public IConnectionPointImpl<T, &IID__FCTransferNotification, CComDynamicUnkArray>
{
public:

//_FCTransferNotification : IDispatch
public:
//------------------------------------------------------------------*
/**
 * Fire_OnUserLoggedIn
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
	HRESULT Fire_OnUserLoggedIn(
		BSTR sUserName,
		BSTR sHostName,
		long bLoggedIn)
	{
		T* pT = (T*)this;
		HRESULT ret = S_OK;
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			_FCTransferNotification* p_FCTransferNotification = reinterpret_cast<_FCTransferNotification*>(sp.p);
			if (p_FCTransferNotification != NULL)
			{
				ret = p_FCTransferNotification->OnUserLoggedIn(sUserName, sHostName, bLoggedIn);
			}
		}
		return ret;
	}

//------------------------------------------------------------------*
/**
 * Fire_OnTransferStateChanged
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
	HRESULT Fire_OnTransferStateChanged(
		unsigned long dwCookie,
		BSTR sCurrentDomainName,
		tagCSC_TRANSFER_STATE state,
		float fltPercentDone)
	{
		T* pT = (T*)this;
		HRESULT ret = S_OK;
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			_FCTransferNotification* p_FCTransferNotification = reinterpret_cast<_FCTransferNotification*>(sp.p);
			if (p_FCTransferNotification != NULL)
			{
				ret = p_FCTransferNotification->OnTransferStateChanged(dwCookie, sCurrentDomainName, state, fltPercentDone);
			}
		}
		return ret;
	}

//------------------------------------------------------------------*
/**
 * Fire_OnTransferStateChanged
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
	HRESULT Fire_OnTransferError(
		unsigned long dwCookie,
		BSTR sCurrentDomainName,
		tagCSC_TRANSFER_STATE state,
		HRESULT hError)
	{
		T* pT = (T*)this;
		HRESULT ret = S_OK;
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			_FCTransferNotification* p_FCTransferNotification = reinterpret_cast<_FCTransferNotification*>(sp.p);
			if (p_FCTransferNotification != NULL)
			{
				ret = p_FCTransferNotification->OnTransferError(dwCookie, sCurrentDomainName, state, hError);
			}
		}
		return ret;
	}
};

#endif // __CPTRANSFERNOTIFICATION_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: CpTransferNotification.h $
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

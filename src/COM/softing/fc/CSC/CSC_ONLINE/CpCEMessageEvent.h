/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/CpCEMessageEvent.h 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CpCEMessageEvent.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/CpCEMessageEvent.h $
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
#ifndef __CPCEMESSAGEEVENT_H_
#define __CPCEMESSAGEEVENT_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

//////////////////////////////////////////////////////////////////////////////
// CProxy_ICEMessageEvent
template <class T>
class CProxy_ICEMessageEvent : public IConnectionPointImpl<T, &IID__ICEMessageEvent, CComDynamicUnkArray>
{
public:

//_ICEMessageEvent : IDispatch
public:
	HRESULT Fire_OnNewMessage(
		BSTR sMachine,
        WORD wSrcFacility,
		HRESULT hr,
		BSTR sDescr)
	{
        HRESULT ret = S_OK;
		T* pT = (T*)this;
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			_ICEMessageEvent* p_ICEMessageEvent = reinterpret_cast<_ICEMessageEvent*>(sp.p);
			if (p_ICEMessageEvent != NULL)
			{
//				ret = p_ICEMessageEvent->OnNewMessage(sMachine, hr, sDescr);
				p_ICEMessageEvent->OnNewMessage(sMachine, wSrcFacility, hr, sDescr);
			}
		}
		return ret;
	}
};

#endif // __CPCEMESSAGEEVENT_H_

/*
 *----------------------------------------------------------------------------*
 *  $History: CpCEMessageEvent.h $
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

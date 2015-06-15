/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/CpBreakpointNotification.h 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CpBreakpointNotification.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/CpBreakpointNotification.h $
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
#ifndef __CPBREAKPOINTNOTIFICATION_H_
#define __CPBREAKPOINTNOTIFICATION_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


template <class T>
class CProxy_FCBreakpointNotification :
    public IConnectionPointImpl<T, &IID__FCBreakpointNotification, CComDynamicUnkArray>
{
	//Warning this class may be recreated by the wizard.
public:
	VOID Fire_OnBreakpointReached(
            BSTR sInstanceName,
            LONG lBreakpointNumber,
            LONG bpQuality,
            BSTR sTaskName)
	{
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			_FCBreakpointNotification* p_FCBreakpointNotification = reinterpret_cast<_FCBreakpointNotification*>(sp.p);
			if (p_FCBreakpointNotification != NULL)
				p_FCBreakpointNotification->OnBreakpointReached(sInstanceName, lBreakpointNumber, bpQuality, sTaskName);
		}
	}


	VOID Fire_OnConfigurationChanged(
            BSTR sAddress)
	{
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			_FCBreakpointNotification* p_FCBreakpointNotification = reinterpret_cast<_FCBreakpointNotification*>(sp.p);
			if (p_FCBreakpointNotification != NULL)
				p_FCBreakpointNotification->OnConfigurationChanged(sAddress);
		}
	}
};

#endif // __CPBREAKPOINTNOTIFICATION_H_

/*
 *----------------------------------------------------------------------------*
 *  $History: CpBreakpointNotification.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 5  *****************
 * User: Su           Date: 12.09.01   Time: 15:27
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Renamed OnNewDownload to OnConfigurationChanged and removed parameters
 * really not used.
 * 
 * *****************  Version 4  *****************
 * User: Su           Date: 17.07.01   Time: 17:59
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Removed Fire_OnConfigurationChanged(), Fire_OnNewDownload has address
 * as argument.
 * 
 * *****************  Version 3  *****************
 * User: Su           Date: 13.07.01   Time: 17:47
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Revised interfaces for Version 2.0
 * 
 * *****************  Version 2  *****************
 * User: Su           Date: 15.03.01   Time: 19:42
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * New header
 *==
 *----------------------------------------------------------------------------*
*/


#ifndef __CPCEDL_H_
#define __CPCEDL_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

/////////////////////////////////////////////////////////////////////////////
// CProxyIDLEvents
template <class T>
class CProxyIDLEvents : public IConnectionPointImpl<T, &IID_IDLEvents, CComDynamicUnkArray>
{
public:
//IDLEvents : IDispatch
public:
	void Fire_OnDownloadState(
		BSTR sTarget,
		BSTR sDomain,
		float fPercentage,
		CEDL_DOWNLD_STATE State)
	{
		T* pT = (T*)this;
		pT->Lock();
		IUnknown** pp = m_vec.begin();
		while (pp < m_vec.end())
		{
			if (*pp != NULL)
			{
				IDLEvents* pIDLEvents = reinterpret_cast<IDLEvents*>(*pp);
				pIDLEvents->OnDownloadState(sTarget, sDomain, fPercentage, State);
			}
			pp++;
		}
		pT->Unlock();
	}
	void Fire_OnDownloadError(
        BSTR sTarget,
		HRESULT hr,
		BSTR sDescr)
	{
		T* pT = (T*)this;
		pT->Lock();
		IUnknown** pp = m_vec.begin();
		while (pp < m_vec.end())
		{
			if (*pp != NULL)
			{
				IDLEvents* pIDLEvents = reinterpret_cast<IDLEvents*>(*pp);
				pIDLEvents->OnDownloadError(sTarget, hr, sDescr);
			}
			pp++;
		}
		pT->Unlock();
	}
};

//////////////////////////////////////////////////////////////////////////////
// CProxy_ICEMessageEvent
template <class T>
class CProxy_ICEMessageEvent : public IConnectionPointImpl<T, &IID__ICEMessageEvent, CComDynamicUnkArray>
{
public:

//_ICEMessageEvent : IDispatch
public:
	void Fire_OnNewMessage(
		BSTR sMachine,
        WORD wScrFacility,
		HRESULT hr,
		BSTR sDescr)
	{
		T* pT = (T*)this;
		pT->Lock();
		IUnknown** pp = m_vec.begin();
		while (pp < m_vec.end())
		{
			if (*pp != NULL)
			{
				_ICEMessageEvent* p_ICEMessageEvent = reinterpret_cast<_ICEMessageEvent*>(*pp);
				p_ICEMessageEvent->OnNewMessage(sMachine,wScrFacility, hr, sDescr);
			}
			pp++;
		}
		pT->Unlock();
	}
};

#endif // __CPCEDL_H_

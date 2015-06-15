
#ifndef __BRKPTMANCP_H_
#define __BRKPTMANCP_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*



template <class T>
class CProxy_ICEMessageEvent : public IConnectionPointImpl<T, &IID__ICEMessageEvent, CComDynamicUnkArray>
{
	//Warning this class may be recreated by the wizard.
public:
	HRESULT Fire_OnNewMessage(BSTR sMachine, USHORT wScrFacility, HRESULT hr, BSTR sDescr)
	{
		HRESULT ret;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			_ICEMessageEvent* p_ICEMessageEvent = reinterpret_cast<_ICEMessageEvent*>(sp.p);
			if (p_ICEMessageEvent != NULL)
				ret = p_ICEMessageEvent->OnNewMessage(sMachine, wScrFacility, hr, sDescr);
		}	return ret;
	
	}
};

template <class T>
class CProxy_IBreakPointNotify : public IConnectionPointImpl<T, &IID__IBreakPointNotify, CComDynamicUnkArray>
{
	//Warning this class may be recreated by the wizard.
public:
	HRESULT Fire_OnBPStateChanged(tagBPM_BREAKPOINT_STATE newState, BSTR sInstance, IUnknown * pBP, LONG bUpdate)
	{
		HRESULT ret;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			_IBreakPointNotify* p_IBreakPointNotify = reinterpret_cast<_IBreakPointNotify*>(sp.p);
			if (p_IBreakPointNotify != NULL)
				ret = p_IBreakPointNotify->OnBPStateChanged(newState, sInstance, pBP, bUpdate);
		}	return ret;
	
	}
	HRESULT Fire_OnSPStateChanged(tagBPM_STOPPOINT_STATE newState, BSTR sInstance, IUnknown * pSP, LONG bUpdate)
	{
		HRESULT ret;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			_IBreakPointNotify* p_IBreakPointNotify = reinterpret_cast<_IBreakPointNotify*>(sp.p);
			if (p_IBreakPointNotify != NULL)
				ret = p_IBreakPointNotify->OnSPStateChanged(newState, sInstance, pSP, bUpdate);
		}	return ret;
	
	}
	HRESULT Fire_OnShowBP(BSTR sInstance, IUnknown * pBP)
	{
		HRESULT ret;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			_IBreakPointNotify* p_IBreakPointNotify = reinterpret_cast<_IBreakPointNotify*>(sp.p);
			if (p_IBreakPointNotify != NULL)
				ret = p_IBreakPointNotify->OnShowBP(sInstance, pBP);
		}	return ret;
	
	}
};

#endif // __BRKPTMANCP_H_


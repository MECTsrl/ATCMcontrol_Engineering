#ifndef _XREFCP_H_
#define _XREFCP_H_




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
class CProxy_IXRefCtrlEvents : public IConnectionPointImpl<T, &IID__IXRefCtrlEvents, CComDynamicUnkArray>
{
	//Warning this class may be recreated by the wizard.
public:
	HRESULT Fire_GetProjectManager(IUnknown * * ppUnk)
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
			_IXRefCtrlEvents* p_IXRefCtrlEvents = reinterpret_cast<_IXRefCtrlEvents*>(sp.p);
			if (p_IXRefCtrlEvents != NULL)
				ret = p_IXRefCtrlEvents->GetProjectManager(ppUnk);
		}	return ret;
	
	}
	HRESULT Fire_ShowInEditor(BSTR sEdiPosInfo)
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
			_IXRefCtrlEvents* p_IXRefCtrlEvents = reinterpret_cast<_IXRefCtrlEvents*>(sp.p);
			if (p_IXRefCtrlEvents != NULL)
				ret = p_IXRefCtrlEvents->ShowInEditor(sEdiPosInfo);
		}	return ret;
	
	}
	HRESULT Fire_OnPrint()
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
			_IXRefCtrlEvents* p_IXRefCtrlEvents = reinterpret_cast<_IXRefCtrlEvents*>(sp.p);
			if (p_IXRefCtrlEvents != NULL)
				ret = p_IXRefCtrlEvents->OnPrint();
		}	return ret;
	
	}
};
#endif

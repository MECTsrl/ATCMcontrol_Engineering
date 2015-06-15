#ifndef _POUVARSELECTIONCP_H_
#define _POUVARSELECTIONCP_H_













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
class CProxy_IPouVarSelCtrlEvents : public IConnectionPointImpl<T, &IID__IPouVarSelCtrlEvents, CComDynamicUnkArray>
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
			_IPouVarSelCtrlEvents* p_IPouVarSelCtrlEvents = reinterpret_cast<_IPouVarSelCtrlEvents*>(sp.p);
			if (p_IPouVarSelCtrlEvents != NULL)
				ret = p_IPouVarSelCtrlEvents->GetProjectManager(ppUnk);
		}	return ret;
	
	}
	HRESULT Fire_GetKadManager(IUnknown * * ppUnk)
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
			_IPouVarSelCtrlEvents* p_IPouVarSelCtrlEvents = reinterpret_cast<_IPouVarSelCtrlEvents*>(sp.p);
			if (p_IPouVarSelCtrlEvents != NULL)
				ret = p_IPouVarSelCtrlEvents->GetKadManager(ppUnk);
		}	return ret;
	
	}
	HRESULT Fire_InsertFBCall(BSTR sFB)
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
			_IPouVarSelCtrlEvents* p_IPouVarSelCtrlEvents = reinterpret_cast<_IPouVarSelCtrlEvents*>(sp.p);
			if (p_IPouVarSelCtrlEvents != NULL)
				ret = p_IPouVarSelCtrlEvents->InsertFBCall(sFB);
		}	return ret;
	
	}
	HRESULT Fire_InsertVariable(BSTR sVar)
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
			_IPouVarSelCtrlEvents* p_IPouVarSelCtrlEvents = reinterpret_cast<_IPouVarSelCtrlEvents*>(sp.p);
			if (p_IPouVarSelCtrlEvents != NULL)
				ret = p_IPouVarSelCtrlEvents->InsertVariable(sVar);
		}	return ret;
	
	}
	HRESULT Fire_ShowHelpFile(BSTR sHelpFile)
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
			_IPouVarSelCtrlEvents* p_IPouVarSelCtrlEvents = reinterpret_cast<_IPouVarSelCtrlEvents*>(sp.p);
			if (p_IPouVarSelCtrlEvents != NULL)
				ret = p_IPouVarSelCtrlEvents->ShowHelpFile(sHelpFile);
		}	return ret;
	
	}
	HRESULT Fire_ShowHeaderFile(BSTR sHeaderFile)
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
			_IPouVarSelCtrlEvents* p_IPouVarSelCtrlEvents = reinterpret_cast<_IPouVarSelCtrlEvents*>(sp.p);
			if (p_IPouVarSelCtrlEvents != NULL)
				ret = p_IPouVarSelCtrlEvents->ShowHeaderFile(sHeaderFile);
		}	return ret;
	
	}
	HRESULT Fire_SaveEditorFile(BSTR sEditorFile)
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
			_IPouVarSelCtrlEvents* p_IPouVarSelCtrlEvents = reinterpret_cast<_IPouVarSelCtrlEvents*>(sp.p);
			if (p_IPouVarSelCtrlEvents != NULL)
				ret = p_IPouVarSelCtrlEvents->SaveEditorFile(sEditorFile);
		}	return ret;
	
	}
	HRESULT Fire_ReloadEditorFile(BSTR sEditorFile)
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
			_IPouVarSelCtrlEvents* p_IPouVarSelCtrlEvents = reinterpret_cast<_IPouVarSelCtrlEvents*>(sp.p);
			if (p_IPouVarSelCtrlEvents != NULL)
				ret = p_IPouVarSelCtrlEvents->ReloadEditorFile(sEditorFile);
		}	return ret;
	
	}
	HRESULT Fire_ShowPouVarSelection()
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
			_IPouVarSelCtrlEvents* p_IPouVarSelCtrlEvents = reinterpret_cast<_IPouVarSelCtrlEvents*>(sp.p);
			if (p_IPouVarSelCtrlEvents != NULL)
				ret = p_IPouVarSelCtrlEvents->ShowPouVarSelection();
		}	return ret;
	
	}
};
#endif
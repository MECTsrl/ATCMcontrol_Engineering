#ifndef _ADDONBASECP_H_
#define _ADDONBASECP_H_

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
class CProxyICEAddOnActionEvents : public IConnectionPointImpl<T, &IID_ICEAddOnActionEvents, CComDynamicUnkArray>
{
	//Warning this class may be recreated by the wizard.
public:
	HRESULT Fire_OnEnsureTargetDisconnected(BSTR sTargetId, LONG bRebuildHint)
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
			ICEAddOnActionEvents* pICEAddOnActionEvents = reinterpret_cast<ICEAddOnActionEvents*>(sp.p);
			if (pICEAddOnActionEvents != NULL)
				ret = pICEAddOnActionEvents->OnEnsureTargetDisconnected(sTargetId, bRebuildHint);
		}	return ret;
	
	}
	HRESULT Fire_OnFileRemoved(BSTR sFilePath)
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
			ICEAddOnActionEvents* pICEAddOnActionEvents = reinterpret_cast<ICEAddOnActionEvents*>(sp.p);
			if (pICEAddOnActionEvents != NULL)
				ret = pICEAddOnActionEvents->OnFileRemoved(sFilePath);
		}	return ret;
	
	}
	HRESULT Fire_OnFileRenamed(BSTR sOldFilePath, BSTR sNewFilePath)
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
			ICEAddOnActionEvents* pICEAddOnActionEvents = reinterpret_cast<ICEAddOnActionEvents*>(sp.p);
			if (pICEAddOnActionEvents != NULL)
				ret = pICEAddOnActionEvents->OnFileRenamed(sOldFilePath, sNewFilePath);
		}	return ret;
	
	}
};

template <class T>
class CProxyICEAddOnActionEvents2 : public IConnectionPointImpl<T, &IID_ICEAddOnActionEvents2, CComDynamicUnkArray>
{
	//Warning this class may be recreated by the wizard.
public:
	HRESULT Fire_BuildAll()
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
			ICEAddOnActionEvents2* pICEAddOnActionEvents2 = reinterpret_cast<ICEAddOnActionEvents2*>(sp.p);
			if (pICEAddOnActionEvents2 != NULL)
				ret = pICEAddOnActionEvents2->BuildAll();
		}	return ret;
	
	}
	HRESULT Fire_BuildTarget(BSTR sTargetId)
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
			ICEAddOnActionEvents2* pICEAddOnActionEvents2 = reinterpret_cast<ICEAddOnActionEvents2*>(sp.p);
			if (pICEAddOnActionEvents2 != NULL)
				ret = pICEAddOnActionEvents2->BuildTarget(sTargetId);
		}	return ret;
	
	}
	HRESULT Fire_ShowTargetMsgTab(BSTR sTargetId)
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
			ICEAddOnActionEvents2* pICEAddOnActionEvents2 = reinterpret_cast<ICEAddOnActionEvents2*>(sp.p);
			if (pICEAddOnActionEvents2 != NULL)
				ret = pICEAddOnActionEvents2->ShowTargetMsgTab(sTargetId);
		}	return ret;
	
	}
	HRESULT Fire_ShowMsgTab(E_FC_MsgTab msgTab)
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
			ICEAddOnActionEvents2* pICEAddOnActionEvents2 = reinterpret_cast<ICEAddOnActionEvents2*>(sp.p);
			if (pICEAddOnActionEvents2 != NULL)
				ret = pICEAddOnActionEvents2->ShowMsgTab(msgTab);
		}	return ret;
	
	}
	HRESULT Fire_IsConnected(BSTR sTargetId, BOOL * pbStatus)
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
			ICEAddOnActionEvents2* pICEAddOnActionEvents2 = reinterpret_cast<ICEAddOnActionEvents2*>(sp.p);
			if (pICEAddOnActionEvents2 != NULL)
				ret = pICEAddOnActionEvents2->IsConnected(sTargetId, pbStatus);
		}	return ret;
	
	}
	HRESULT Fire_IsExclusiveConnected(BSTR sTargetId, BOOL * pbStatus)
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
			ICEAddOnActionEvents2* pICEAddOnActionEvents2 = reinterpret_cast<ICEAddOnActionEvents2*>(sp.p);
			if (pICEAddOnActionEvents2 != NULL)
				ret = pICEAddOnActionEvents2->IsExclusiveConnected(sTargetId, pbStatus);
		}	return ret;
	
	}
	HRESULT Fire_GetResourceState(BSTR sTargetId, LONG * plStatus)
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
			ICEAddOnActionEvents2* pICEAddOnActionEvents2 = reinterpret_cast<ICEAddOnActionEvents2*>(sp.p);
			if (pICEAddOnActionEvents2 != NULL)
				ret = pICEAddOnActionEvents2->GetResourceState(sTargetId, plStatus);
		}	return ret;
	
	}
	HRESULT Fire_Connect(BSTR sTargetId)
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
			ICEAddOnActionEvents2* pICEAddOnActionEvents2 = reinterpret_cast<ICEAddOnActionEvents2*>(sp.p);
			if (pICEAddOnActionEvents2 != NULL)
				ret = pICEAddOnActionEvents2->Connect(sTargetId);
		}	return ret;
	
	}
	HRESULT Fire_Disconnect(BSTR sTargetId)
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
			ICEAddOnActionEvents2* pICEAddOnActionEvents2 = reinterpret_cast<ICEAddOnActionEvents2*>(sp.p);
			if (pICEAddOnActionEvents2 != NULL)
				ret = pICEAddOnActionEvents2->Disconnect(sTargetId);
		}	return ret;
	
	}
	HRESULT Fire_ConnectExclusive(BSTR sTargetId)
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
			ICEAddOnActionEvents2* pICEAddOnActionEvents2 = reinterpret_cast<ICEAddOnActionEvents2*>(sp.p);
			if (pICEAddOnActionEvents2 != NULL)
				ret = pICEAddOnActionEvents2->ConnectExclusive(sTargetId);
		}	return ret;
	
	}
	HRESULT Fire_DisconnectExclusive(BSTR sTargetId)
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
			ICEAddOnActionEvents2* pICEAddOnActionEvents2 = reinterpret_cast<ICEAddOnActionEvents2*>(sp.p);
			if (pICEAddOnActionEvents2 != NULL)
				ret = pICEAddOnActionEvents2->DisconnectExclusive(sTargetId);
		}	return ret;
	
	}
	HRESULT Fire_Download(BSTR sTargetId)
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
			ICEAddOnActionEvents2* pICEAddOnActionEvents2 = reinterpret_cast<ICEAddOnActionEvents2*>(sp.p);
			if (pICEAddOnActionEvents2 != NULL)
				ret = pICEAddOnActionEvents2->Download(sTargetId);
		}	return ret;
	
	}
	HRESULT Fire_Flash(BSTR sTargetId)
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
			ICEAddOnActionEvents2* pICEAddOnActionEvents2 = reinterpret_cast<ICEAddOnActionEvents2*>(sp.p);
			if (pICEAddOnActionEvents2 != NULL)
				ret = pICEAddOnActionEvents2->Flash(sTargetId);
		}	return ret;
	
	}
	HRESULT Fire_StartControl(BSTR sTargetId)
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
			ICEAddOnActionEvents2* pICEAddOnActionEvents2 = reinterpret_cast<ICEAddOnActionEvents2*>(sp.p);
			if (pICEAddOnActionEvents2 != NULL)
				ret = pICEAddOnActionEvents2->StartControl(sTargetId);
		}	return ret;
	
	}
	HRESULT Fire_StopControl(BSTR sTargetId)
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
			ICEAddOnActionEvents2* pICEAddOnActionEvents2 = reinterpret_cast<ICEAddOnActionEvents2*>(sp.p);
			if (pICEAddOnActionEvents2 != NULL)
				ret = pICEAddOnActionEvents2->StopControl(sTargetId);
		}	return ret;
	
	}
	HRESULT Fire_StartAllTasks(BSTR sTargetId)
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
			ICEAddOnActionEvents2* pICEAddOnActionEvents2 = reinterpret_cast<ICEAddOnActionEvents2*>(sp.p);
			if (pICEAddOnActionEvents2 != NULL)
				ret = pICEAddOnActionEvents2->StartAllTasks(sTargetId);
		}	return ret;
	
	}
	HRESULT Fire_StopAllTasks(BSTR sTargetId)
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
			ICEAddOnActionEvents2* pICEAddOnActionEvents2 = reinterpret_cast<ICEAddOnActionEvents2*>(sp.p);
			if (pICEAddOnActionEvents2 != NULL)
				ret = pICEAddOnActionEvents2->StopAllTasks(sTargetId);
		}	return ret;
	
	}
	HRESULT Fire_ColdStart(BSTR sTargetId)
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
			ICEAddOnActionEvents2* pICEAddOnActionEvents2 = reinterpret_cast<ICEAddOnActionEvents2*>(sp.p);
			if (pICEAddOnActionEvents2 != NULL)
				ret = pICEAddOnActionEvents2->ColdStart(sTargetId);
		}	return ret;
	
	}
	HRESULT Fire_WarmStart(BSTR sTargetId)
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
			ICEAddOnActionEvents2* pICEAddOnActionEvents2 = reinterpret_cast<ICEAddOnActionEvents2*>(sp.p);
			if (pICEAddOnActionEvents2 != NULL)
				ret = pICEAddOnActionEvents2->WarmStart(sTargetId);
		}	return ret;
	
	}
	HRESULT Fire_InitClear(BSTR sTargetId)
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
			ICEAddOnActionEvents2* pICEAddOnActionEvents2 = reinterpret_cast<ICEAddOnActionEvents2*>(sp.p);
			if (pICEAddOnActionEvents2 != NULL)
				ret = pICEAddOnActionEvents2->InitClear(sTargetId);
		}	return ret;
	
	}
	HRESULT Fire_CustomOnlCmd(BSTR sTargetId, SAFEARRAY * * dataDown, SAFEARRAY * * dataUp)
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
			ICEAddOnActionEvents2* pICEAddOnActionEvents2 = reinterpret_cast<ICEAddOnActionEvents2*>(sp.p);
			if (pICEAddOnActionEvents2 != NULL)
				ret = pICEAddOnActionEvents2->CustomOnlCmd(sTargetId, dataDown, dataUp);
		}	return ret;
	
	}
	HRESULT Fire_ProjectInfoUpload(BSTR sTargetId, BSTR* psProjVers, BSTR* psProjName, BSTR* psModified, BSTR* psCreated, BSTR* psOwner, BSTR* psComment1, BSTR* psComment2, BSTR* psComment3)
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
			ICEAddOnActionEvents2* pICEAddOnActionEvents2 = reinterpret_cast<ICEAddOnActionEvents2*>(sp.p);
			if (pICEAddOnActionEvents2 != NULL)
				ret = pICEAddOnActionEvents2->ProjectInfoUpload(sTargetId, psProjVers, psProjName, psModified, psCreated, psOwner, psComment1, psComment2, psComment3);
		}	return ret;
	
	}
	HRESULT Fire_ProjectDownload(BSTR sTargetId, SAFEARRAY * * dataDown)
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
			ICEAddOnActionEvents2* pICEAddOnActionEvents2 = reinterpret_cast<ICEAddOnActionEvents2*>(sp.p);
			if (pICEAddOnActionEvents2 != NULL)
				ret = pICEAddOnActionEvents2->ProjectDownload(sTargetId, dataDown);
		}	return ret;
	
	}
	HRESULT Fire_ProjectUpload(BSTR sTargetId, SAFEARRAY * * dataUp)
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
			ICEAddOnActionEvents2* pICEAddOnActionEvents2 = reinterpret_cast<ICEAddOnActionEvents2*>(sp.p);
			if (pICEAddOnActionEvents2 != NULL)
				ret = pICEAddOnActionEvents2->ProjectUpload(sTargetId, dataUp);
		}	return ret;
	
	}
	HRESULT Fire_CustCmd1(BSTR sTargetId, SAFEARRAY * * dataDown, SAFEARRAY * * dataUp)
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
			ICEAddOnActionEvents2* pICEAddOnActionEvents2 = reinterpret_cast<ICEAddOnActionEvents2*>(sp.p);
			if (pICEAddOnActionEvents2 != NULL)
				ret = pICEAddOnActionEvents2->CustCmd1(sTargetId, dataDown, dataUp);
		}	return ret;
	
	}
	HRESULT Fire_CustCmd2(BSTR sTargetId, SAFEARRAY * * dataDown, SAFEARRAY * * dataUp)
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
			ICEAddOnActionEvents2* pICEAddOnActionEvents2 = reinterpret_cast<ICEAddOnActionEvents2*>(sp.p);
			if (pICEAddOnActionEvents2 != NULL)
				ret = pICEAddOnActionEvents2->CustCmd2(sTargetId, dataDown, dataUp);
		}	return ret;
	
	}
	HRESULT Fire_CustCmd3(BSTR sTargetId, SAFEARRAY * * dataDown, SAFEARRAY * * dataUp)
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
			ICEAddOnActionEvents2* pICEAddOnActionEvents2 = reinterpret_cast<ICEAddOnActionEvents2*>(sp.p);
			if (pICEAddOnActionEvents2 != NULL)
				ret = pICEAddOnActionEvents2->CustCmd3(sTargetId, dataDown, dataUp);
		}	return ret;
	
	}
	HRESULT Fire_CustCmd4(BSTR sTargetId, SAFEARRAY * * dataDown, SAFEARRAY * * dataUp)
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
			ICEAddOnActionEvents2* pICEAddOnActionEvents2 = reinterpret_cast<ICEAddOnActionEvents2*>(sp.p);
			if (pICEAddOnActionEvents2 != NULL)
				ret = pICEAddOnActionEvents2->CustCmd4(sTargetId, dataDown, dataUp);
		}	return ret;
	
	}
	HRESULT Fire_CustCmd5(BSTR sTargetId, SAFEARRAY * * dataDown, SAFEARRAY * * dataUp)
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
			ICEAddOnActionEvents2* pICEAddOnActionEvents2 = reinterpret_cast<ICEAddOnActionEvents2*>(sp.p);
			if (pICEAddOnActionEvents2 != NULL)
				ret = pICEAddOnActionEvents2->CustCmd5(sTargetId, dataDown, dataUp);
		}	return ret;
	
	}
    HRESULT Fire_LoadFile(BSTR sTargetId, BSTR sName, SAFEARRAY * * data)
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
			ICEAddOnActionEvents2* pICEAddOnActionEvents2 = reinterpret_cast<ICEAddOnActionEvents2*>(sp.p);
			if (pICEAddOnActionEvents2 != NULL)
				ret = pICEAddOnActionEvents2->LoadFile(sTargetId, sName, data);
		}	return ret;
	
	}
    HRESULT Fire_SaveFile(BSTR sTargetId, BSTR sName, SAFEARRAY * * data)
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
			ICEAddOnActionEvents2* pICEAddOnActionEvents2 = reinterpret_cast<ICEAddOnActionEvents2*>(sp.p);
			if (pICEAddOnActionEvents2 != NULL)
				ret = pICEAddOnActionEvents2->SaveFile(sTargetId, sName, data);
		}	return ret;
	
	}
    HRESULT Fire_Dir(BSTR sTargetId, SAFEARRAY * * fileNames)
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
			ICEAddOnActionEvents2* pICEAddOnActionEvents2 = reinterpret_cast<ICEAddOnActionEvents2*>(sp.p);
			if (pICEAddOnActionEvents2 != NULL)
				ret = pICEAddOnActionEvents2->Dir(sTargetId, fileNames);
		}	return ret;
	
	}
};

template <class T>
class CProxyICECompManEvents : public IConnectionPointImpl<T, &IID_ICECompManEvents, CComDynamicUnkArray>
{
	//Warning this class may be recreated by the wizard.
public:
	HRESULT Fire_OnComponentByCLSID(BSTR sCLSID, IUnknown * * ppUnk)
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
			ICECompManEvents* pICECompManEvents = reinterpret_cast<ICECompManEvents*>(sp.p);
			if (pICECompManEvents != NULL)
				ret = pICECompManEvents->OnComponentByCLSID(sCLSID, ppUnk);
		}	return ret;
	
	}
	HRESULT Fire_OnComponentById(BSTR sId, IUnknown * * ppUnk)
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
			ICECompManEvents* pICECompManEvents = reinterpret_cast<ICECompManEvents*>(sp.p);
			if (pICECompManEvents != NULL)
				ret = pICECompManEvents->OnComponentById(sId, ppUnk);
		}	return ret;
	
	}
};

#endif
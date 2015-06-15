#ifndef _CEADDONHANDLERCP_H_
#define _CEADDONHANDLERCP_H_

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
#endif
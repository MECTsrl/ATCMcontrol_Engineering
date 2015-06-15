#ifndef _DBGINFOCP_H_
#define _DBGINFOCP_H_





template <class T>
class CProxyIDebugInfoNotify : public IConnectionPointImpl<T, &IID_IDebugInfoNotify, CComDynamicUnkArray>
{
	//Warning this class may be recreated by the wizard.
public:
	HRESULT Fire_OnDebugInfoModified()
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
			IDebugInfoNotify* pIDebugInfoNotify = reinterpret_cast<IDebugInfoNotify*>(sp.p);
			if (pIDebugInfoNotify != NULL)
				ret = pIDebugInfoNotify->OnDebugInfoModified();
		}	return ret;
	
	}
};
#endif
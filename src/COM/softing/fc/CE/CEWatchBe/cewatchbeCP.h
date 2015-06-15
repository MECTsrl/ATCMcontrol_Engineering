
#ifndef __CEWATCHBECP_H_
#define __CEWATCHBECP_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*







template <class T>
class CProxyICEWatchNotify : public IConnectionPointImpl<T, &IID_ICEWatchNotify, CComDynamicUnkArray>
{
	//Warning this class may be recreated by the wizard.
public:
	HRESULT Fire_OnExpressionChanged(LONG lHandle)
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
			ICEWatchNotify* pICEWatchNotify = reinterpret_cast<ICEWatchNotify*>(sp.p);
			if (pICEWatchNotify != NULL)
				ret = pICEWatchNotify->OnExpressionChanged(lHandle);
		}	return ret;
	
	}
};

#endif // __CEWATCHBECP_H_


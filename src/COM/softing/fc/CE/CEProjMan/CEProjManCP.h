#ifndef _CEPROJMANCP_H_
#define _CEPROJMANCP_H_




#include "stdafx.h"



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
class CProxy_ICEProjectManagerEvents : public IConnectionPointImpl<T, &IID__ICEProjectManagerEvents, CComDynamicUnkArray>
{
	//Warning this class may be recreated by the wizard.
public:
	HRESULT Fire_InitClassTree(IUnknown * pXMLDocument)
	{
        TRACE("PROJ_MAN> Fire_InitClassTree\n");
        
        HRESULT ret;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			_ICEProjectManagerEvents* p_ICEProjectManagerEvents = reinterpret_cast<_ICEProjectManagerEvents*>(sp.p);
			if (p_ICEProjectManagerEvents != NULL)
				ret = p_ICEProjectManagerEvents->InitClassTree(pXMLDocument);
		}	return ret;
	
	}
	HRESULT Fire_InitTargetTree(IUnknown * pXMLDocument)
	{
        TRACE("PROJ_MAN> Fire_InitTargetTree\n");
		
        HRESULT ret;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			_ICEProjectManagerEvents* p_ICEProjectManagerEvents = reinterpret_cast<_ICEProjectManagerEvents*>(sp.p);
			if (p_ICEProjectManagerEvents != NULL)
				ret = p_ICEProjectManagerEvents->InitTargetTree(pXMLDocument);
		}	return ret;
	
	}
	HRESULT Fire_UpdateClassTree(BSTR sChangedIdPath, E_UPDATE_MODE eMode)
	{
        TRACE("PROJ_MAN> Fire_UpdateClassTree: %s, %i\n", (CString)sChangedIdPath, (int)eMode);
        
        HRESULT ret;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			_ICEProjectManagerEvents* p_ICEProjectManagerEvents = reinterpret_cast<_ICEProjectManagerEvents*>(sp.p);
			if (p_ICEProjectManagerEvents != NULL)
				ret = p_ICEProjectManagerEvents->UpdateClassTree(sChangedIdPath, eMode);
		}	return ret;
	
	}
	HRESULT Fire_UpdateTargetTree(BSTR sChangedIdPath, E_UPDATE_MODE eMode)
	{
        TRACE("PROJ_MAN> Fire_UpdateTargetTree: %s, %i\n", (CString)sChangedIdPath, (int)eMode);
		
        HRESULT ret;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			_ICEProjectManagerEvents* p_ICEProjectManagerEvents = reinterpret_cast<_ICEProjectManagerEvents*>(sp.p);
			if (p_ICEProjectManagerEvents != NULL)
				ret = p_ICEProjectManagerEvents->UpdateTargetTree(sChangedIdPath, eMode);
		}	return ret;
	
	}
	HRESULT Fire_OnFileAdded(BSTR sFilePath, E_FV_FileCategory eFileCat)
	{
        TRACE("PROJ_MAN> Fire_OnFileAdded: %s, %i\n", (CString)sFilePath, (int)eFileCat);
        
        HRESULT ret;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			_ICEProjectManagerEvents* p_ICEProjectManagerEvents = reinterpret_cast<_ICEProjectManagerEvents*>(sp.p);
			if (p_ICEProjectManagerEvents != NULL)
				ret = p_ICEProjectManagerEvents->OnFileAdded(sFilePath, eFileCat);
		}	return ret;
	
	}
	HRESULT Fire_OnFileRemoved(BSTR sFilePath, E_FV_FileCategory eFileCat)
	{
        TRACE("PROJ_MAN> Fire_OnFileRemoved: %s, %i\n", (CString)sFilePath, (int)eFileCat);
		
        HRESULT ret;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			_ICEProjectManagerEvents* p_ICEProjectManagerEvents = reinterpret_cast<_ICEProjectManagerEvents*>(sp.p);
			if (p_ICEProjectManagerEvents != NULL)
				ret = p_ICEProjectManagerEvents->OnFileRemoved(sFilePath, eFileCat);
		}	return ret;
	
	}
	HRESULT Fire_InitInstanceTree(IUnknown * pXMLDocument)
	{
        TRACE("PROJ_MAN> Fire_InitInstanceTree\n");
        
        HRESULT ret;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			_ICEProjectManagerEvents* p_ICEProjectManagerEvents = reinterpret_cast<_ICEProjectManagerEvents*>(sp.p);
			if (p_ICEProjectManagerEvents != NULL)
				ret = p_ICEProjectManagerEvents->InitInstanceTree(pXMLDocument);
		}	return ret;
	
	}
	HRESULT Fire_UpdateInstanceTree(BSTR sChangedIdPath, E_UPDATE_MODE eMode)
	{
        TRACE("PROJ_MAN> Fire_UpdateInstanceTree: %s, %i\n", (CString)sChangedIdPath, (int)eMode);

        HRESULT ret;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			_ICEProjectManagerEvents* p_ICEProjectManagerEvents = reinterpret_cast<_ICEProjectManagerEvents*>(sp.p);
			if (p_ICEProjectManagerEvents != NULL)
				ret = p_ICEProjectManagerEvents->UpdateInstanceTree(sChangedIdPath, eMode);
		}	return ret;
	
	}
	HRESULT Fire_ShowContextHelp(ULONG dwContextId)
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
			_ICEProjectManagerEvents* p_ICEProjectManagerEvents = reinterpret_cast<_ICEProjectManagerEvents*>(sp.p);
			if (p_ICEProjectManagerEvents != NULL)
				ret = p_ICEProjectManagerEvents->ShowContextHelp(dwContextId);
		}	return ret;
	
	}
	HRESULT Fire_ProjectFileChanged()
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
			_ICEProjectManagerEvents* p_ICEProjectManagerEvents = reinterpret_cast<_ICEProjectManagerEvents*>(sp.p);
			if (p_ICEProjectManagerEvents != NULL)
				ret = p_ICEProjectManagerEvents->ProjectFileChanged();
		}	return ret;
	
	}
};

template <class T>
class CProxy_ICEProjManInstanceEvents : public IConnectionPointImpl<T, &IID__ICEProjManInstanceEvents, CComDynamicUnkArray>
{
	//Warning this class may be recreated by the wizard.
public:
	HRESULT Fire_TargetInstanceAdded(BSTR sId, BSTR sIdPath)
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
			_ICEProjManInstanceEvents* p_ICEProjManInstanceEvents = reinterpret_cast<_ICEProjManInstanceEvents*>(sp.p);
			if (p_ICEProjManInstanceEvents != NULL)
				ret = p_ICEProjManInstanceEvents->TargetInstanceAdded(sId, sIdPath);
		}	return ret;
	
	}
	HRESULT Fire_TargetInstanceRemoved(BSTR sId)
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
			_ICEProjManInstanceEvents* p_ICEProjManInstanceEvents = reinterpret_cast<_ICEProjManInstanceEvents*>(sp.p);
			if (p_ICEProjManInstanceEvents != NULL)
				ret = p_ICEProjManInstanceEvents->TargetInstanceRemoved(sId);
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
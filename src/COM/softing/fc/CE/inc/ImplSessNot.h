#ifndef _IMPLSESSNOT_H
#define _IMPLSESSNOT_H

#include "CSC_Online.h"
#include "fccallback.h"


FC_CALLBACK_DEF2 (SessionNotifyCB, CSC_SESSION_STATE, HRESULT)

class CImplSessionNotify : 
	public IDispatchImpl<_FCSessionNotification, &IID__FCSessionNotification, &LIBID_CSC_ONLINELib,2,0>, 
	public CComObjectRoot
{
private:
	FC_CALLBACK_BASE (SessionNotifyCB) *m_pCallback;

public:
	CImplSessionNotify() { m_pCallback = NULL; };
	~CImplSessionNotify() {};

    STDMETHOD_(HRESULT, OnChanged)();
	STDMETHOD_(HRESULT, OnStateChanged)(/*[in]*/ LPUNKNOWN pSession, /*[in]*/ CSC_SESSION_STATE newState, /*[in]*/ HRESULT hrError);

	void SetCallback (FC_CALLBACK_BASE (SessionNotifyCB) *pCallback) { m_pCallback = pCallback; }

	BEGIN_COM_MAP(CImplSessionNotify)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(_FCSessionNotification)
	END_COM_MAP()
};


#endif  // _IMPLSESSNOT_H
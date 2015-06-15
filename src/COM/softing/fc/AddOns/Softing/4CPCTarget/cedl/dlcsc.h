
#ifndef __DLCSC_H_
#define __DLCSC_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "CSC_Online.h"
//----  Forward Class Definitions:   -------------------------------*
class CStation;
//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

class CImpTransferNotify : 
	public IDispatchImpl<_FCTransferNotification, &IID__FCTransferNotification, &LIBID_CSC_ONLINELib,2,0>, 
	public CComObjectRoot
{
public:
	CImpTransferNotify(void);
	virtual ~CImpTransferNotify(void);

	STDMETHOD(OnUserLoggedIn) (/*[in, string]*/ BSTR sUserName,/*[in]*/ BSTR sHostName,/*[in]*/ BOOL bLoggedIn);
	STDMETHOD(OnTransferStateChanged) (/*[in]*/ DWORD dwCookie, /*[in, string]*/ BSTR sCurrentDomainName, /*[in]*/ CSC_TRANSFER_STATE state, /*[in]*/ float fltPercentDone);
    STDMETHOD(OnTransferError)(/*[in]*/ DWORD dwCookie, /*[in, string]*/ BSTR sCurrentDomainName, /*[in]*/ CSC_TRANSFER_STATE state, /*[in]*/HRESULT hError);

	BEGIN_COM_MAP(CImpTransferNotify)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(_FCTransferNotification)
	END_COM_MAP()

public: //attributes
	CStation * m_pStation;
private:
    void OnTransferDomain(BSTR sDomain, float fPercentage);
    void OnDeleteDomain(BSTR sDomain, float fPercentage);
    void OnCleanup(void);
	void OnAborting(DWORD dwCookie);

    bool IsRequestPending(void);

};

#endif // __DLCSC_H_

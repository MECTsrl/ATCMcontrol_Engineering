// CompManEventsImpl.h: interface for the CCompManEventsImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPMANEVENTSIMPL_H__ADC03BDF_F92B_4688_9A54_A97912AE69FC__INCLUDED_)
#define AFX_COMPMANEVENTSIMPL_H__ADC03BDF_F92B_4688_9A54_A97912AE69FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cecompmanev.h"
#include "CEProjMan.h"
#include <atlbase.h>
#include <atlcom.h>


class CCompManEventsImpl  : public IDispatchImpl<ICECompManEvents, 
                                            &IID_ICECompManEvents, 
                                            &LIBID_CEPROJMANLib>,
						   public CComObjectRoot
{
public:
	CCompManEventsImpl();
	virtual ~CCompManEventsImpl();

    

private:
    IUnknown* m_pKadManag;

	STDMETHOD(OnComponentByCLSID)(BSTR sCLSID, IUnknown** ppUnk);
    STDMETHOD(OnComponentById)(BSTR sId, IUnknown** ppUnk); 

	BEGIN_COM_MAP(CCompManEventsImpl)
	    COM_INTERFACE_ENTRY(ICECompManEvents)
   	    COM_INTERFACE_ENTRY(IUnknown)
	END_COM_MAP()

};

#endif // !defined(AFX_COMPMANEVENTSIMPL_H__ADC03BDF_F92B_4688_9A54_A97912AE69FC__INCLUDED_)

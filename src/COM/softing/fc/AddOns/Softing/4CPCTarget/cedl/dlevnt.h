
#ifndef __DLEVNT_H_
#define __DLEVNT_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "dl.h"
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

class CImpDLEvents : 
	public IDispatchImpl<IDLEvents, &IID_IDLEvents, &LIBID_CEDLLib,2,0>, 
	public CComObjectRoot
{
public:
	CImpDLEvents();
	virtual ~CImpDLEvents();

    STDMETHOD(OnDownloadState) (BSTR sTarget, BSTR sDomain, float fPercentage,CEDL_DOWNLD_STATE State);
    STDMETHOD(OnDownloadError)(BSTR sTarget, HRESULT hr, BSTR sDescr);

	BEGIN_COM_MAP(CImpDLEvents)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IDLEvents)
	END_COM_MAP()

public: //attributes
    void SetListClient(HWND hWnd) {m_hwndListClient = hWnd;};
protected:
    HWND   m_hwndListClient; 
};
#endif // __DLEVNT_H_


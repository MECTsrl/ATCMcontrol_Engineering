
#ifndef __PARSEMESSAGE_H_
#define __PARSEMESSAGE_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "defs.h"
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*
//----------------------------------------------------------------------------*
/**
 *  class CParseMessage
 *  Represents GSD Parser message occured during the 
 *  parse process. The message string, HRESULT error code can
 *  be obtained via this interface IParseMessage. 
 *  It can also be queried wether the message is an hard error 
 *  message or not. 
 *
 *  @see    
*/
class ATL_NO_VTABLE CParseMessage : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public IDispatchImpl<IParseMessage, &IID_IParseMessage, &LIBID_PARSEGSDLib,2,0>
{

//--------------------------
// =Constructor
public:
            CParseMessage();
	virtual ~CParseMessage()
    {}

    void Initialize(HRESULT, LPCTSTR);

//--------------------------
// =Interface map
BEGIN_COM_MAP(CParseMessage)
COM_INTERFACE_ENTRY(IDispatch)
COM_INTERFACE_ENTRY(IParseMessage)
END_COM_MAP()

//--------------------------
// Interface IParseMessage
    STDMETHOD(get_Message)(BSTR * psVal);
    STDMETHOD(get_Code)(LONG * pVal);
    STDMETHOD(IsError)(VARIANT_BOOL * pVal);

private:
// =Operations
// =Attributes
    /** Parser message (localized) */
    CString m_strMessage;
    /** error code of message, can also be a warning, informational.*/
    HRESULT m_code;
};

typedef CComObject<CParseMessage> ParseMessage;
typedef CMFCObjStore<ParseMessage> MessageList;

ParseMessage* CreateParseMessage(HRESULT, LPCTSTR );

#endif // __PARSEMESSAGE_H_



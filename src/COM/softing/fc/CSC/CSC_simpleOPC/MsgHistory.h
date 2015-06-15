/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_simpleOPC/MsgHistory.h 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: MsgHistory.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_simpleOPC/MsgHistory.h $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CSC
 *
 * =COMPONENT           simple sync OPC client
 *
 * =CURRENT      $Date: 28.02.07 19:00 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  07.08.01  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/

#ifndef __MSGHISTORY_H_
#define __MSGHISTORY_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//----  Aggregate Includes:   --------------------------------------*
#include "resource.h"       // main symbols
#include "CSC_OnlineServer.h"
#include "CSC_simpleOPC.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

DATE VariantTimeToLocalTime(DATE dt);



/////////////////////////////////////////////////////////////////////////////
// CFCMessageHistory
class ATL_NO_VTABLE CFCMessageHistory : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CFCMessageHistory, &CLSID_FCMessageHistory>,
	public IDispatchImpl<IFCMessageHistoryScripted, &IID_IFCMessageHistoryScripted, &LIBID_CSC_SIMPLEOPCLib>,
    public IObjectSafetyImpl<CFCMessageHistory, INTERFACESAFE_FOR_UNTRUSTED_CALLER>
{
//--------------------------
// =Constructor
public:
            	            CFCMessageHistory();
            	            ~CFCMessageHistory();

DECLARE_REGISTRY_RESOURCEID(IDR_FCMESSAGES)

//--------------------------
// =Interface map
BEGIN_COM_MAP(CFCMessageHistory)
	COM_INTERFACE_ENTRY(IFCMessageHistoryScripted)
	COM_INTERFACE_ENTRY2(IDispatch, IFCMessageHistoryScripted)
    COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

//--------------------------
// =Connection point map
BEGIN_CONNECTION_POINT_MAP(CFCMessageHistory)
END_CONNECTION_POINT_MAP()

//--------------------------
// =Operations
public:

//--------------------------
// =Overrides
public:
    HRESULT                 FinalConstruct();
    void                    FinalRelease();

//--------------------------
// =Interface
// IFCMessageHistoryScripted
public:
	STDMETHOD(Connect)(/*[in, string]*/ BSTR sPassword);
	STDMETHOD(Disconnect)(void);
	STDMETHOD(get_MachineName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_MachineName)(/*[in, string]*/ BSTR newVal);
	STDMETHOD(get_ConfigName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_ConfigName)(/*[in, string]*/ BSTR newVal);
	STDMETHOD(get_IsConnected)(/*[out, retval]*/ BOOL *pVal);

	STDMETHOD(ClearHistoryBuffer)(/*[in, string]*/ BSTR sResource);
	STDMETHOD(get_MaxHistoryLength)(/*[in, string]*/ BSTR sResource, /*[out, retval]*/ long *pVal);
	STDMETHOD(put_MaxHistoryLength)(/*[in, string]*/ BSTR sResource, /*[in]*/ long newVal);

    STDMETHOD(get_MinHistoryID)(/*[in, string]*/ BSTR sResource, /*[out, retval]*/ long *pVal);
	STDMETHOD(get_MaxHistoryID)(/*[in, string]*/ BSTR sResource, /*[out, retval]*/ long *pVal);
    STDMETHOD(get_MsgText)(/*[in, string]*/ BSTR sResource, /*[in]*/ long idHistory, /*[out, string, retval]*/ BSTR *sMessage);
    STDMETHOD(get_MsgErrorNo)(/*[in, string]*/ BSTR sResource, /*[in]*/ long idHistory, /*[out, retval]*/ HRESULT *phError);
    STDMETHOD(get_MsgDate)(/*[in, string]*/ BSTR sResource, /*[in]*/ long idHistory, /*[out, retval]*/ DATE *dt);


//--------------------------
// Helpers
private:
	HRESULT                 DoConnect(BSTR sPassword);
	HRESULT                 DoDisconnect(void);

//--------------------------
// =Attributes
private:
    CString                 m_strMachineName;
    CString                 m_strConfigName;
    CComPtr<IMessages>      m_pMessageIndirect;
    long                    m_cachedHistoryID;
    CComBSTR                m_cachedMessage;
    DATE                    m_cachedDate;
    CComPtr<IControlMgt>    m_pControlMgt;
    long                    m_lSessionKey;
};

/////////////////////////////////////////////////////////////////////

#endif // __MSGHISTORY_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: MsgHistory.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_simpleOPC
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSC/CSC_simpleOPC
 * 
 * *****************  Version 3  *****************
 * User: Ed           Date: 9/03/01    Time: 3:23p
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_simpleOPC
 * handle session password
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 8.08.01    Time: 18:15
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_simpleOPC
 * adapted to changes of V2.0
 * 
 * *****************  Version 1  *****************
 * User: Ca           Date: 8.08.01    Time: 9:59
 * Created in $/4Control/COM/softing/fc/CSC/CSC_simpleOPC
 * copied from V1.3
 *==
 *----------------------------------------------------------------------------*
*/

/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/AsyncOPC/OPCConnection.h 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: OPCConnection.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/AsyncOPC/OPCConnection.h $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CSC
 *
 * =COMPONENT           AsyncOPC
 *
 * =CURRENT      $Date: 28.02.07 19:00 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *
 *==
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

#ifndef __OPCCONNECTION_H_
#define __OPCCONNECTION_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

/////////////////////////////////////////////////////////////////////////////
// CWriteRequest
class ATL_NO_VTABLE CWriteRequest : 
	public CComObjectRootEx<CComMultiThreadModel>,
    public IUnknown
{
public:
	                        CWriteRequest();
  	virtual                 ~CWriteRequest();

BEGIN_COM_MAP(CWriteRequest)
END_COM_MAP()

    HRESULT                 Init(CItem *pItem, VARIANT Value);
    HRESULT                 GetItem(CItem **ppItem);
    HRESULT                 GetValue(VARIANT *pValue);

private:
    CComPtr<CItem>          m_pItem;
    CComVariant             m_Value;
};

typedef CList<CWriteRequest*, CWriteRequest*>    writeRequestList;
typedef CMap<CString, LPCTSTR, COPCConnection*, COPCConnection*>   connectionMap;
typedef CList<CItem*, CItem*>    itemList;

/////////////////////////////////////////////////////////////////////////////
// CConnection
class ATL_NO_VTABLE COPCConnection : 
	public CComObjectRootEx<CComMultiThreadModel>,
    public IUnknown
{
public:
                            COPCConnection();
  	virtual                 ~COPCConnection();

BEGIN_COM_MAP(COPCConnection)
END_COM_MAP()

    static HRESULT          LookupConnection(LPCTSTR strMachineName, CItem *pItem, COPCConnection**ppConnection);
    static HRESULT          ReleaseConnection(COPCConnection *pConnection, CItem *pItem);

    HRESULT                 SubmitWriteRequest(CItem *pItem, VARIANT Value);

private:
    HRESULT                 StartThread();
    HRESULT                 StopThread();
    HRESULT                 AddItem(CItem *pItem);
    HRESULT                 RemoveItem(CItem *pItem);
    BOOL                    IsEmpty() const { return m_activeItemList.IsEmpty(); }

private:
    static DWORD WINAPI     ThreadMain(LPVOID lpParam);
    HRESULT                 ConnectThread();
    HRESULT                 DoConnect();
    HRESULT                 DoDisconnect();
    HRESULT                 DoWorkItem();
    HRESULT                 DoAddItems(itemList &addItems);
    HRESULT                 DoReadItems(itemList &activeItems);
    HRESULT                 DoWriteItems(writeRequestList &writeRequests);
    HRESULT                 DoRemoveItems(itemList &removedItems);

private:
    IOPCServer             *m_pOPCServer;
    OPCHANDLE               m_hGroup1;
    IOPCItemMgt            *m_pItemMgt;
    IOPCSyncIO             *m_pSyncIO;

private:
    CString                 m_sMachineName;
    itemList                m_activeItemList;
    itemList                m_removedItemList;
    writeRequestList        m_writeRequestList;
    bool                    m_bShutdownRequest;
    bool                    m_bConnected;
    CCriticalSection        m_cs;
    static CCriticalSection sm_guardConnectionMap;
    static connectionMap    sm_connectionMap;
};

#endif // __OPCCONNECTION_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: OPCConnection.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSC/AsyncOPC
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSC/AsyncOPC
 * 
 * *****************  Version 1  *****************
 * User: Ca           Date: 8.08.01    Time: 9:41
 * Created in $/4Control/COM/softing/fc/CSC/AsyncOPC
 * copied from V1.3
 *==
 *----------------------------------------------------------------------------*
*/
	

// OPCGroup.h
//
// (c) Copyright 1998 The OPC Foundation
// ALL RIGHTS RESERVED.
//
// DISCLAIMER:
//  This sample code is provided by the OPC Foundation solely to assist
//  in understanding the OPC Specifications and may be used
//  as set forth in the License Grant section of the OPC Specification.
//  This code is provided as-is and without warranty or support of any sort
//  and is subject to the Warranty and Liability Disclaimers which appear
//  in the printed OPC Specification.
//
// CREDITS:
//  This code was generously provided to the OPC Foundation by
//  Jim Hansen, FactorySoft Inc.
//
// CONTENTS:
//  This file is part of the OPC Data Access Automation 2.0 wrapper dll.
//  This is the Group Object.
//
//
// Modification Log:
// Vers    Date   By    Notes
// ----  -------- ---   -----
// 1.00  03/7/98  JH    First Release
// 1.01  09/2/98  JH    AddItem & AddItems
//

#ifndef __OPCGROUP_H__              // Only Include Once
#define __OPCGROUP_H__

#include "OPCItem.h"
#include "OPCGroupEvent.h"

class COPCAutoServer;

/////////////////////////////////////////////////////////////////////
class ATL_NO_VTABLE COPCGroupImpl :
   public IAdviseSink,
   public IOPCDataCallback,
   public CComDualImpl<IOPCGroup, &IID_IOPCGroup, &LIBID_OPCAutomation>,
   public CComDualImpl<OPCItems, &IID_OPCItems, &LIBID_OPCAutomation>,
   public IConnectionPointContainerImpl<COPCGroupImpl>,
   public CProxyDIOPCGroupEvent<COPCGroupImpl>,
   public CComObjectRootEx<CComMultiThreadModel>
{
public:
BEGIN_COM_MAP(COPCGroupImpl)
   COM_INTERFACE_ENTRY(IAdviseSink)
   COM_INTERFACE_ENTRY(IOPCDataCallback)
   COM_INTERFACE_ENTRY2(IDispatch, IOPCGroup)
   COM_INTERFACE_ENTRY(IOPCGroup)
   COM_INTERFACE_ENTRY(OPCItems)
   COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(COPCGroupImpl)
   CONNECTION_POINT_ENTRY(DIID_DIOPCGroupEvent)
END_CONNECTION_POINT_MAP()

DECLARE_NOT_AGGREGATABLE(OPCGroup)

   COPCGroupImpl();
   ~COPCGroupImpl();

   // OPCGroup interface
   STDMETHOD(get_Parent)(IOPCAutoServer **ppParent);
   STDMETHOD(get_Name)(BSTR *pName);
   STDMETHOD(put_Name)(BSTR Name);
   STDMETHOD(get_IsPublic)(VARIANT_BOOL *pIsPublic);
   STDMETHOD(get_IsActive)(VARIANT_BOOL *pIsActive);
   STDMETHOD(put_IsActive)(VARIANT_BOOL IsActive);
   STDMETHOD(get_IsSubscribed)(VARIANT_BOOL *pIsSubscribed);
   STDMETHOD(put_IsSubscribed)(VARIANT_BOOL IsSubscribed);
   STDMETHOD(get_ClientHandle)(LONG *pClientHandle);
   STDMETHOD(put_ClientHandle)(LONG ClientHandle);
   STDMETHOD(get_ServerHandle)(LONG *pServerHandle);
   STDMETHOD(get_LocaleID)(LONG *pLocaleID);
   STDMETHOD(put_LocaleID)(LONG LocaleID);
   STDMETHOD(get_TimeBias)(LONG *pTimeBias);
   STDMETHOD(put_TimeBias)(LONG TimeBias);
   STDMETHOD(get_DeadBand)(FLOAT *pDeadBand);
   STDMETHOD(put_DeadBand)(FLOAT DeadBand);
   STDMETHOD(get_UpdateRate)(LONG *pUpdateRate);
   STDMETHOD(put_UpdateRate)(LONG UpdateRate);
   STDMETHOD(get_OPCItems)(OPCItems **ppItems);
   STDMETHOD(SyncRead)( SHORT Source,
                        LONG NumItems,
                        SAFEARRAY ** ppServerHandles,
                        SAFEARRAY ** ppValues,
                        SAFEARRAY ** ppErrors,
                        VARIANT * pQualities,
                        VARIANT * pTimeStamps);
   STDMETHOD(SyncWrite)(LONG NumItems,
                        SAFEARRAY ** ppServerHandles,
                        SAFEARRAY ** ppValues,
                        SAFEARRAY ** ppErrors);
   STDMETHOD(AsyncRead)(LONG NumItems,
                        SAFEARRAY ** ppServerHandles,
                        SAFEARRAY ** ppErrors,
                        LONG TransactionID,
                        LONG * pCancelID);
   STDMETHOD(AsyncWrite)(LONG NumItems,
                        SAFEARRAY ** ppServerHandles,
                        SAFEARRAY ** ppValues,
                        SAFEARRAY ** ppErrors,
                        LONG TransactionID,
                        LONG * pCancelID);
   STDMETHOD(AsyncRefresh)(SHORT Source,
                        LONG TransactionID,
                        LONG * pCancelID);
   STDMETHOD(AsyncCancel)(LONG CancelID);

   // OPCItems interface
   STDMETHOD(get_Parent)(OPCGroup **ppParent);
   STDMETHOD(get_DefaultRequestedDataType)(SHORT *pDefaultRequestedDataType);
   STDMETHOD(put_DefaultRequestedDataType)(SHORT DefaultRequestedDataType);
   STDMETHOD(get_DefaultAccessPath)(BSTR *pDefaultAccessPath);
   STDMETHOD(put_DefaultAccessPath)(BSTR DefaultAccessPath);
   STDMETHOD(get_DefaultIsActive)(VARIANT_BOOL *pDefaultIsActive);
   STDMETHOD(put_DefaultIsActive)(VARIANT_BOOL DefaultIsActive);
   STDMETHOD(get_Count)(LONG *pCount);
   STDMETHOD(get__NewEnum)(IUnknown **ppUnk);
   STDMETHOD(Item)(VARIANT ItemSpecifier,
                        OPCItem **ppItem);
   STDMETHOD(GetOPCItem)(LONG ServerHandle,
                        OPCItem **ppItem);
   STDMETHOD(AddItem)(  BSTR ItemID,
                        LONG ClientHandle,
                        OPCItem **ppItem);
   STDMETHOD(AddItems)( LONG NumItems,
                        SAFEARRAY ** ppItemIDs,
                        SAFEARRAY ** ppClientHandles,
                        SAFEARRAY ** ppServerHandles,
                        SAFEARRAY ** ppErrors,
                        VARIANT RequestedDataTypes,
                        VARIANT AccessPaths);
   STDMETHOD(Remove)(LONG NumItems,
                        SAFEARRAY ** ppServerHandles,
                        SAFEARRAY ** ppErrors);
   STDMETHOD(Validate)(LONG NumItems,
                        SAFEARRAY ** ppItemIDs,
                        SAFEARRAY ** ppErrors,
                        VARIANT RequestedDataTypes,
                        VARIANT AccessPaths);
   STDMETHOD(SetActive)(LONG NumItems,
                        SAFEARRAY ** ppServerHandles,
                        VARIANT_BOOL ActiveState,
                        SAFEARRAY ** ppErrors);
   STDMETHOD(SetClientHandles)(
                        LONG NumItems,
                        SAFEARRAY ** ppServerHandles,
                        SAFEARRAY ** ppClientHandles,
                        SAFEARRAY ** ppErrors);
   STDMETHOD(SetDataTypes)(
                        LONG NumItems,
                        SAFEARRAY ** ppServerHandles,
                        SAFEARRAY ** ppRequestedDataTypes,
                        SAFEARRAY ** ppErrors);

   // The OPC Callback functions
   STDMETHODIMP OnDataChange(
                        DWORD       Transid,
                        OPCHANDLE   grphandle,
                        HRESULT     masterquality,
                        HRESULT     mastererror,
                        DWORD       count,
                        OPCHANDLE * clienthandles,
                        VARIANT   * values,
                        WORD      * quality,
                        FILETIME  * time,
                        HRESULT   * errors);

   STDMETHODIMP OnReadComplete(
                        DWORD       Transid,
                        OPCHANDLE   grphandle,
                        HRESULT     masterquality,
                        HRESULT     mastererror,
                        DWORD       count,
                        OPCHANDLE * clienthandles,
                        VARIANT   * values,
                        WORD      * quality,
                        FILETIME  * time,
                        HRESULT   * errors);

   STDMETHODIMP OnWriteComplete(
                        DWORD       Transid,
                        OPCHANDLE   grphandle,
                        HRESULT     mastererr,
                        DWORD       count,
                        OPCHANDLE * clienthandles,
                        HRESULT   * errors);

   STDMETHODIMP OnCancelComplete(
                        DWORD       transid,
                        OPCHANDLE   grphandle);
   // IData interface
   STDMETHODIMP_(void)  OnDataChange(LPFORMATETC, LPSTGMEDIUM);
   STDMETHODIMP_(void)  OnViewChange(DWORD, LONG);
   STDMETHODIMP_(void)  OnRename(LPMONIKER);
   STDMETHODIMP_(void)  OnSave(void);
   STDMETHODIMP_(void)  OnClose(void);

   STDMETHODIMP_(void)  OnReadComplete(LPFORMATETC, LPSTGMEDIUM);
   STDMETHODIMP_(void)  OnWriteComplete(LPFORMATETC, LPSTGMEDIUM);

   // Non-COM operations
   HRESULT         Initialize( COPCAutoServer* pParent, IUnknown* pUnk );
   const CComBSTR& GetName() const {return m_name;}
   const LONG      GetServerHandle() const {return m_server;}

private:
   // Group Status members
   CComBSTR    m_name;
   DWORD       m_rate;
   BOOL        m_active;
   LONG        m_timebias;
   FLOAT       m_deadband;
   DWORD       m_LCID;
   OPCHANDLE   m_client;
   OPCHANDLE   m_server;
   DWORD       m_DataConnection;
   DWORD       m_WriteConnection;
   BOOL        m_usingCP;     // Flag if using ConnectionPoints for data
   VARIANT_BOOL m_subscribed;
   list<OPCHANDLE> m_readIDs;  // list of outstanding AsyncRead transaction IDs
   BOOL        m_asyncReading;

   // Main Group Interface Pointer
   CComQIPtr<IOPCGroupStateMgt,
            &IID_IOPCGroupStateMgt> m_pOPCGroup;
   // hold all of these interfaces rather than query as needed
   // (each query is a round trip to the server)
   CComQIPtr<IOPCSyncIO,
            &IID_IOPCSyncIO> m_pSyncIO;
   CComQIPtr<IOPCAsyncIO,
            &IID_IOPCAsyncIO> m_pAsyncIO;
   CComQIPtr<IOPCAsyncIO2,
            &IID_IOPCAsyncIO2> m_pAsyncIO2;
   CComQIPtr<IOPCItemMgt,
            &IID_IOPCItemMgt> m_pOPCItem;
   CComQIPtr<IDataObject,
            &IID_IDataObject> m_pIDataObject;

   // OPCItems collection members
   list<COPCItem*> m_items;  // list of OPCItem objects

   VARTYPE        m_defaultDataType;
   CComBSTR       m_defaultAccessPath;
   VARIANT_BOOL   m_defaultActive;

   // pointer to parent
   COPCAutoServer* m_pParent;
};

typedef CComObject<COPCGroupImpl> COPCGroup;

#endif
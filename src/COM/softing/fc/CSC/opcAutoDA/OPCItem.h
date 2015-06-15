// OPCItem.h
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
//  This is the Item Object.
//
//
// Modification Log:
// Vers    Date   By    Notes
// ----  -------- ---   -----
// 1.00  03/7/98  JH    First Release
//

#ifndef __OPCITEM_H__               // Only Include Once
#define __OPCITEM_H__

class COPCGroupImpl;

/////////////////////////////////////////////////////////////////////
class ATL_NO_VTABLE COPCItemImpl :
   public CComDualImpl<OPCItem, &IID_OPCItem, &LIBID_OPCAutomation>,
   public CComObjectRootEx<CComMultiThreadModel>
{
public:
BEGIN_COM_MAP(COPCItemImpl)
   COM_INTERFACE_ENTRY(OPCItem)
END_COM_MAP()
DECLARE_NOT_AGGREGATABLE(OPCItem)

   COPCItemImpl();
   ~COPCItemImpl();
   HRESULT Initialize(  OPCITEMDEF* pIdef,
                        OPCITEMRESULT* pResult,
                        IOPCGroupStateMgt* pGroup,
                        COPCGroupImpl* pParent );

   // OPCItem interface
   STDMETHOD(get_Parent)(OPCGroup **ppParent);
   STDMETHOD(get_ClientHandle)(LONG *pClientHandle);
   STDMETHOD(put_ClientHandle)(LONG ClientHandle);
   STDMETHOD(get_ServerHandle)(LONG *pServerHandle);
   STDMETHOD(get_AccessPath)(BSTR *pAccessPath);
   STDMETHOD(get_AccessRights)(LONG *pAccessRights);
   STDMETHOD(get_ItemID)(BSTR *pItemID);
   STDMETHOD(get_IsActive)(VARIANT_BOOL *pIsActive);
   STDMETHOD(put_IsActive)(VARIANT_BOOL IsActive);
   STDMETHOD(get_RequestedDataType)(SHORT *pRequestedDataType);
   STDMETHOD(put_RequestedDataType)(SHORT RequestedDataType);
   STDMETHOD(get_Value)(VARIANT *pCurrentValue);
   STDMETHOD(get_Quality)(LONG *pQuality);
   STDMETHOD(get_TimeStamp)(DATE *pTimeStamp);
   STDMETHOD(get_CanonicalDataType)(SHORT *pCanonicalDataType);
   STDMETHOD(get_EUType)(SHORT * pEUType);
   STDMETHOD(get_EUInfo)(VARIANT * pEUInfo);
   STDMETHOD(Read)(
                  SHORT Source,
                  VARIANT *pValue,
                  VARIANT *pQuality,
                  VARIANT *pTimeStamp);
   STDMETHOD(Write)(
                  VARIANT Value);

   // Non-COM interface
   void     Update( const OPCITEMSTATE* pItemState );
   void     Update( const VARIANT *pValue,
                    const DWORD quality,
                    const FILETIME timestamp );
   const BSTR GetItemID() const {return m_itemID;}
   const VARIANT* GetValue() const {return &m_value;}
   const OPCHANDLE GetServerHandle() const {return m_server;}
   const OPCHANDLE GetClientHandle() const {return m_client;}
   void     SetClientHandle( OPCHANDLE clientHandle ) {m_client = clientHandle;}
   void     SetActiveState( BOOL active ) {m_active = active;}
   void     SetDataType( VARTYPE datatype ) {m_datatype = datatype;}

private:
   CComBSTR    m_itemID;
   CComBSTR    m_accessPath;
   BOOL        m_active;
   DWORD       m_accessRights;
   VARTYPE     m_datatype;
   VARTYPE     m_nativeDatatype;
   CComVariant m_value;
   DWORD       m_quality;
   FILETIME    m_timestamp;
   OPCHANDLE   m_client;
   OPCHANDLE   m_server;

   // Interface Pointers
   CComQIPtr<IOPCItemMgt,
            &IID_IOPCItemMgt> m_pOPCItem;
   CComQIPtr<IOPCSyncIO,
            &IID_IOPCSyncIO> m_pOPCSyncIO;

   // pointer to parent
   COPCGroupImpl*      m_pParent;
};

typedef CComObject<COPCItemImpl> COPCItem;

#endif
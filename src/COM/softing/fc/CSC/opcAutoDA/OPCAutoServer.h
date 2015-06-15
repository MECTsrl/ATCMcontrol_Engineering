// OPCAutoServer.h
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
//  This is the Server Object.
//  The server manages a connection to an OPC Server,
//  a collection of groups, and creates Browse objects.
//
// Modification Log:
// Vers    Date   By    Notes
// ----  -------- ---   -----
// 1.00  03/7/98  JH    First Release
//
//

#include "OPCGroup.h"

/////////////////////////////////////////////////////////////////////
class OPCServerInfo
{
public:
    OPCServerInfo( LPOLESTR ProgID, LPOLESTR Description, CLSID& clsid )
        : m_ProgID(ProgID), m_Description(Description), m_clsid(clsid) {}
    CComBSTR     m_ProgID;
    CComBSTR     m_Description;
    CLSID        m_clsid;
};

class ATL_NO_VTABLE COPCAutoServer :
   public IOPCShutdown,
   public CComDualImpl<IOPCAutoServer, &IID_IOPCAutoServer, &LIBID_OPCAutomation>,
   public CComDualImpl<IOPCGroups, &IID_IOPCGroups, &LIBID_OPCAutomation>,
   public ISupportErrorInfo,
   public IConnectionPointContainerImpl<COPCAutoServer>,
   public CProxyDIOPCServerEvent<COPCAutoServer>,
   public CProxyDIOPCGroupsEvent<COPCAutoServer>,
   public CComObjectRootEx<CComMultiThreadModel>,
   public CComCoClass<COPCAutoServer,&CLSID_OPCServer>
{
public:
BEGIN_COM_MAP(COPCAutoServer)
   COM_INTERFACE_ENTRY(IOPCShutdown)
   COM_INTERFACE_ENTRY2(IDispatch, IOPCAutoServer)
   COM_INTERFACE_ENTRY(IOPCAutoServer)
   COM_INTERFACE_ENTRY(IOPCGroups)
   COM_INTERFACE_ENTRY(ISupportErrorInfo)
   COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(COPCAutoServer)
   CONNECTION_POINT_ENTRY(DIID_DIOPCServerEvent)
   CONNECTION_POINT_ENTRY(DIID_DIOPCGroupsEvent)
END_CONNECTION_POINT_MAP()

DECLARE_NOT_AGGREGATABLE(COPCAutoServer)
DECLARE_REGISTRY(COPCAutoServer, _T("Softing.OPC_Automation.1"), _T("Softing.OPC_Automation"), IDS_PROJNAME, THREADFLAGS_BOTH)

   COPCAutoServer();
   ~COPCAutoServer();

// ISupportsErrorInfo
   STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IOPCShutdown
   STDMETHOD(ShutdownRequest)(LPCWSTR szReason);

// OPCServer
    STDMETHOD(get_StartTime)(DATE * pStartTime);
    STDMETHOD(get_CurrentTime)(DATE * pCurrentTime);
    STDMETHOD(get_LastUpdateTime)(DATE * pLastUpdateTime);
    STDMETHOD(get_MajorVersion)(short * pMajorVersion);
    STDMETHOD(get_MinorVersion)(short * pMinorVersion);
    STDMETHOD(get_BuildNumber)(short * pBuildNumber);
    STDMETHOD(get_VendorInfo)(BSTR * pVendorInfo);
    STDMETHOD(get_ServerState)(LONG * pServerState);
    STDMETHOD(get_ServerName)(BSTR * pServerName);
    STDMETHOD(get_ServerNode)(BSTR * pServerNode);
    STDMETHOD(get_ClientName)(BSTR * pClientName);
    STDMETHOD(put_ClientName)(BSTR ClientName);
    STDMETHOD(get_LocaleID)(LONG * pLocaleID);
    STDMETHOD(put_LocaleID)(LONG LocaleID);
    STDMETHOD(get_Bandwidth)(LONG * pBandwidth);
    STDMETHOD(get_OPCGroups)(OPCGroups ** ppGroups);
    STDMETHOD(get_PublicGroupNames)(VARIANT * pPublicGroups);
    STDMETHOD(GetOPCServers)(VARIANT Node,
                        VARIANT * pOPCServers);
    STDMETHOD(Connect)( BSTR ProgID,
                        VARIANT Node);
    STDMETHOD(Disconnect)(void);
    STDMETHOD(CreateBrowser)(OPCBrowser ** ppBrowser);
    STDMETHOD(GetErrorString)(LONG ErrorCode,
                              BSTR *ErrorString);
    STDMETHOD(QueryAvailableLocaleIDs)(VARIANT * LocaleIDs);
    STDMETHOD(QueryAvailableProperties)(
                        BSTR ItemID,
                        LONG * pCount,
                        SAFEARRAY ** PropertyIDs,
                        SAFEARRAY ** Descriptions,
                        SAFEARRAY ** ppDataTypes);
    STDMETHOD(GetItemProperties)(BSTR ItemID,
                        LONG Count,
                        SAFEARRAY ** PropertyIDs,
                        SAFEARRAY ** PropertyValues,
                        SAFEARRAY ** Errors);
    STDMETHOD(LookupItemIDs)(BSTR ItemID,
                        LONG Count,
                        SAFEARRAY ** PropertyIDs,
                        SAFEARRAY ** NewItemIDs,
                        SAFEARRAY ** Errors);

// OPCGroups
    STDMETHOD(get_Parent)(IOPCAutoServer ** ppParent);
    STDMETHOD(get_DefaultGroupIsActive)(VARIANT_BOOL * pDefaultGroupIsActive);
    STDMETHOD(put_DefaultGroupIsActive)(VARIANT_BOOL DefaultGroupIsActive);
    STDMETHOD(get_DefaultGroupUpdateRate)(LONG * pDefaultGroupUpdateRate);
    STDMETHOD(put_DefaultGroupUpdateRate)(LONG DefaultGroupUpdateRate);
    STDMETHOD(get_DefaultGroupDeadband)(float * pDefaultGroupDeadband);
    STDMETHOD(put_DefaultGroupDeadband)(float DefaultGroupDeadband);
    STDMETHOD(get_DefaultGroupLocaleID)(LONG * pDefaultGroupLocaleID);
    STDMETHOD(put_DefaultGroupLocaleID)(LONG DefaultGroupLocaleID);
    STDMETHOD(get_DefaultGroupTimeBias)(LONG * pDefaultGroupTimeBias);
    STDMETHOD(put_DefaultGroupTimeBias)(LONG DefaultGroupTimeBias);
    STDMETHOD(get_Count)(LONG *pCount);
    STDMETHOD(get__NewEnum)(IUnknown ** ppUnk);
    STDMETHOD(Item)(VARIANT ItemSpecifier, OPCGroup ** ppGroup);
    STDMETHOD(Add)(VARIANT Name,
                     OPCGroup ** ppGroup);
    STDMETHOD(GetOPCGroup)(VARIANT ItemSpecifier, OPCGroup ** ppGroup);
    STDMETHOD(RemoveAll)(void);
    STDMETHOD(Remove)(VARIANT ItemSpecifier);
    STDMETHOD(ConnectPublicGroup)(
                     BSTR Name,
                     OPCGroup ** ppGroup);

    STDMETHOD(RemovePublicGroup)(VARIANT ItemSpecifier);

private:
   CComBSTR    m_name;
   CComBSTR    m_node;
   CComBSTR    m_client;
   LONG        m_localeID;
   CComQIPtr<IOPCServer, &IID_IOPCServer> m_pOPCServer;
   CComQIPtr<IOPCItemProperties, &IID_IOPCItemProperties> m_pOPCItemParams;
   CComQIPtr<IOPCCommon, &IID_IOPCCommon> m_pOPCCommon;
   DWORD       m_ShutdownConnection;

   // OPCGroups data
   VARIANT_BOOL   m_defaultActive;
   LONG           m_defaultUpdate;
   float          m_defaultDeadband;
   LONG           m_defaultLocale;
   LONG           m_defaultTimeBias;

   // Collection of groups used by enumerator
   list<COPCGroup*> m_groups;  // list of OPCGroup objects

   // OPC Servers data
   list<OPCServerInfo*> m_servers;
};

typedef CComObject<COPCAutoServer> COPCAutoServerObject;









// OPCBrowser.h
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
//  This is the Browser Object.
//
//
// Modification Log:
// Vers    Date   By    Notes
// ----  -------- ---   -----
// 1.00  03/7/98  JH    First Release
//

#ifndef __OPCBROWSER_H__               // Only Include Once
#define __OPCBROWSER_H__

/////////////////////////////////////////////////////////////////////
class ATL_NO_VTABLE COPCBrowserImpl :
   public CComDualImpl<OPCBrowser, &IID_OPCBrowser, &LIBID_OPCAutomation>,
   public CComObjectRootEx<CComMultiThreadModel>
{
public:
BEGIN_COM_MAP(COPCBrowserImpl)
   COM_INTERFACE_ENTRY(OPCBrowser)
END_COM_MAP()
DECLARE_NOT_AGGREGATABLE(OPCBrowser)

   COPCBrowserImpl();
   ~COPCBrowserImpl();

   STDMETHOD(get_Organization)( LONG *pOrganization);
   STDMETHOD(get_Filter)( BSTR *pFilter);
   STDMETHOD(put_Filter)( BSTR Filter);
   STDMETHOD(get_DataType)( SHORT *pDataType);
   STDMETHOD(put_DataType)( SHORT DataType);
   STDMETHOD(get_AccessRights)( LONG *pAccessRights);
   STDMETHOD(put_AccessRights)( LONG AccessRights);
   STDMETHOD(get_CurrentPosition)( BSTR *pCurrentPosition);
   STDMETHOD(get_Count)( LONG *pCount);
   STDMETHOD(get__NewEnum)( IUnknown **ppUnk);
   STDMETHOD(Item)( VARIANT ItemSpecifier, BSTR *pItem);
   STDMETHOD(ShowBranches)();
   STDMETHOD(ShowLeafs)( VARIANT Flat);
   STDMETHOD(MoveUp)();
   STDMETHOD(MoveToRoot)();
   STDMETHOD(MoveDown)( BSTR Branch);
   STDMETHOD(MoveTo)( SAFEARRAY ** ppBranches);
   STDMETHOD(GetItemID)( BSTR Leaf, BSTR *pItemID);
   STDMETHOD(GetAccessPaths)( BSTR ItemID, VARIANT *pAccessPaths);

   // Browse Interface Pointer
   CComQIPtr<IOPCBrowseServerAddressSpace,
            &IID_IOPCBrowseServerAddressSpace> m_pOPCBrowser;
private:
   CComBSTR    m_filter;
   VARTYPE     m_dataType;
   LONG        m_accessRights;
   CComBSTR    m_currentPosition;
   list<CComBSTR*> m_names;  // list of names
   list<CComBSTR*>::iterator m_listPosition;

   void        ClearNames();
};

typedef CComObject<COPCBrowserImpl> COPCBrowser;

#endif
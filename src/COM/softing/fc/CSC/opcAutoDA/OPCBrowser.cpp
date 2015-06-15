// OPCBrowser.cpp
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
//  This is the Browser object.
//
//
// Modification Log:
// Vers    Date   By    Notes
// ----  -------- ---   -----
// 1.00  03/7/98  JH    First Release
//

#include "stdafx.h"
#include "OPCBrowser.h"

/////////////////////////////////////////////////////////////////////
// Constructor
COPCBrowserImpl::COPCBrowserImpl()
: m_filter( "" )
{
   m_dataType = VT_EMPTY;
   m_accessRights = OPC_WRITEABLE|OPC_READABLE;
   m_listPosition = NULL;
}

COPCBrowserImpl::~COPCBrowserImpl()
{
   ClearNames();
}

/////////////////////////////////////////////////////////////////////
// OPCBrowser::Organization property
STDMETHODIMP COPCBrowserImpl::get_Organization( LONG *pOrganization)
{
   if( pOrganization == NULL )
      return E_INVALIDARG;

   OPCNAMESPACETYPE  nameSpaceType;
   HRESULT hr = m_pOPCBrowser->QueryOrganization(&nameSpaceType);
   if( FAILED(hr) )
      return hr;

   *pOrganization = nameSpaceType;
   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCBrowser::Filter property
STDMETHODIMP COPCBrowserImpl::get_Filter( BSTR *pFilter)
{
   if( pFilter == NULL )
      return E_INVALIDARG;

   *pFilter = m_filter.Copy();

   return S_OK;
}

STDMETHODIMP COPCBrowserImpl::put_Filter( BSTR Filter)
{
   if( Filter==NULL )
      m_filter = _T("");
   else
      m_filter = Filter;
   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCBrowser::DataType property
STDMETHODIMP COPCBrowserImpl::get_DataType( SHORT *pDataType)
{
   if( pDataType == NULL )
      return E_INVALIDARG;

   *pDataType = m_dataType;
   return S_OK;
}

STDMETHODIMP COPCBrowserImpl::put_DataType( SHORT DataType)
{
   m_dataType = DataType;
   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCBrowser::AccessRights property
STDMETHODIMP COPCBrowserImpl::get_AccessRights( LONG *pAccessRights)
{
   if( pAccessRights == NULL )
      return E_INVALIDARG;

   *pAccessRights = m_accessRights;
   return S_OK;
}

STDMETHODIMP COPCBrowserImpl::put_AccessRights( LONG AccessRights)
{
   m_accessRights = AccessRights;
   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCBrowser::CurrentPosition property
// Servers should return the position when GetItemID is called with no name
STDMETHODIMP COPCBrowserImpl::get_CurrentPosition( BSTR *pCurrentPosition)
{
   if( pCurrentPosition == NULL )
      return E_INVALIDARG;

   LPWSTR pName = NULL;
   HRESULT hr = m_pOPCBrowser->GetItemID( empty, &pName );
   if( SUCCEEDED(hr) )
   {
      *pCurrentPosition = SysAllocString( pName );
      CoTaskMemFree( pName );
   }

   return hr;
}

/////////////////////////////////////////////////////////////////////
// OPCBrowser::Count property
STDMETHODIMP COPCBrowserImpl::get_Count( LONG *pCount)
{
   if( pCount == NULL )
      return E_INVALIDARG;

   *pCount = m_names.size();

   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCBrowser::_NewEnum property
typedef CComObject<CComEnum<IEnumVARIANT, &IID_IEnumVARIANT, VARIANT, _Copy<VARIANT> > > enumvar;

STDMETHODIMP COPCBrowserImpl::get__NewEnum( IUnknown **ppUnk)
{
   if( ppUnk == NULL )
      return E_INVALIDARG;
   *ppUnk = NULL;

   if( m_names.size()==0 )
   {
      // create an IEnumVariant object initialized with a variant
      CComVariant var( "" );
      enumvar* p = new enumvar;
      _ASSERTE(p);
      HRESULT hr = p->Init(&var, &var, NULL, AtlFlagCopy);
      if (SUCCEEDED(hr))
         hr = p->QueryInterface(IID_IEnumVARIANT, (void**)ppUnk);
      if (FAILED(hr))
         delete p;
      return hr;
   }


   // Create an array of variants containing BSTRs
   VARIANT* pArray = (VARIANT*)_alloca(m_names.size()*sizeof(VARIANT));

   m_listPosition = m_names.begin();
   long index=0;
   while( m_listPosition!=m_names.end() )
   {
      CComBSTR* pName = *m_listPosition;
      // and put it into a variant
      VariantInit( &pArray[index] );
      pArray[index].vt = VT_BSTR;
      pArray[index].bstrVal = *pName;
      m_listPosition++;
      index++;
   }


   // create an IEnumVariant object initialized with these variants
   enumvar* p = new enumvar;
   _ASSERTE(p);
   HRESULT hr = p->Init(&pArray[0], &pArray[m_names.size()], NULL, AtlFlagCopy);
   if (SUCCEEDED(hr))
      hr = p->QueryInterface(IID_IEnumVARIANT, (void**)ppUnk);
   if (FAILED(hr))
      delete p;

   return hr;
}

/////////////////////////////////////////////////////////////////////
// OPCBrowser::Item method
// Item index is 1-based for VB
STDMETHODIMP COPCBrowserImpl::Item( VARIANT ItemSpecifier, BSTR *pItem)
{
   if( ItemSpecifier.vt != VT_I4 && ItemSpecifier.vt != VT_I2 )
      return E_INVALIDARG;
   if( ItemSpecifier.vt == VT_I2 )
      ItemSpecifier.lVal = ItemSpecifier.iVal;
   if( ItemSpecifier.lVal < 1 || ItemSpecifier.lVal > m_names.size() )
      return E_INVALIDARG;
   if( pItem == NULL )
      return E_INVALIDARG;

   LONG index=1;
   m_listPosition = m_names.begin();
   while( m_listPosition!=m_names.end() && index < ItemSpecifier.lVal )
   {
      m_listPosition++;
      index++;
   }
   if( index == ItemSpecifier.lVal )
   {
      CComBSTR* pName = *m_listPosition;
      *pItem = pName->Copy();
      return S_OK;
   }

   return E_FAIL;
}

/////////////////////////////////////////////////////////////////////
// OPCBrowser::ShowBranches method
#define NEXT_COUNT 100  // the number of elements per Next() call

STDMETHODIMP COPCBrowserImpl::ShowBranches()
{
   // Clear out the list
   ClearNames();

   // Get the Branch names
   IEnumString* pEnumString = NULL;
   HRESULT hr =
   m_pOPCBrowser->BrowseOPCItemIDs(OPC_BRANCH,
                               m_filter,
                               m_dataType,
                               m_accessRights,
                               &pEnumString);
   if( SUCCEEDED(hr) )
   {
      // Enumerate all the names, saving them in the list
      LPWSTR pName[NEXT_COUNT];
      ULONG count = 0;
      do
      {
         hr = pEnumString->Next(NEXT_COUNT, &pName[0], &count);
         for( ULONG index=0; index<count; index++ )
         {
            m_names.push_back( new CComBSTR( pName[index] ) );
            CoTaskMemFree( pName[index] );
         }
      }
      while( hr == S_OK );

      pEnumString->Release();

   }

   return hr;
}


/////////////////////////////////////////////////////////////////////
// OPCBrowser::ShowLeafs method

STDMETHODIMP COPCBrowserImpl::ShowLeafs( VARIANT Flat)
{
   // Clear out the list
   ClearNames();

   // Get the Leaf names
   IEnumString* pEnumString = NULL;
   HRESULT hr = S_OK;
   if ((Flat.vt==VT_BOOL) && (Flat.boolVal))
   {
      hr = m_pOPCBrowser->BrowseOPCItemIDs(OPC_FLAT,
                               m_filter,
                               m_dataType,
                               m_accessRights,
                               &pEnumString);
   }
   else
   {
      hr = m_pOPCBrowser->BrowseOPCItemIDs(OPC_LEAF,
                               m_filter,
                               m_dataType,
                               m_accessRights,
                               &pEnumString);
   }
   if( SUCCEEDED(hr) )
   {
      // Enumerate all the names, saving them in the list
      LPWSTR pName[NEXT_COUNT];
      ULONG count = 0;
      do
      {
         hr = pEnumString->Next(NEXT_COUNT, &pName[0], &count);
         for( ULONG index=0; index<count; index++ )
         {
            m_names.push_back( new CComBSTR( pName[index] ) );
            CoTaskMemFree( pName[index] );
         }
      }
      while( hr == S_OK );

      pEnumString->Release();
   }

   return hr;
}

/////////////////////////////////////////////////////////////////////
// OPCBrowser::MoveUp method
STDMETHODIMP COPCBrowserImpl::MoveUp()
{
   // Clear out the list
   ClearNames();

   HRESULT hr = m_pOPCBrowser->ChangeBrowsePosition( OPC_BROWSE_UP, empty );
   return hr;
}

/////////////////////////////////////////////////////////////////////
// OPCBrowser::MoveToRoot method
STDMETHODIMP COPCBrowserImpl::MoveToRoot()
{
   // Clear out the list
   ClearNames();

   HRESULT hr = S_OK;
   for( int i=0; i<50 && SUCCEEDED(hr); i++ )
      hr = m_pOPCBrowser->ChangeBrowsePosition( OPC_BROWSE_UP, empty );
   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCBrowser::MoveDown method
STDMETHODIMP COPCBrowserImpl::MoveDown( BSTR Branch)
{
   // Clear out the list
   ClearNames();

   HRESULT hr = m_pOPCBrowser->ChangeBrowsePosition( OPC_BROWSE_DOWN, Branch );
   return hr;
}

/////////////////////////////////////////////////////////////////////
// OPCBrowser::MoveTo method
STDMETHODIMP COPCBrowserImpl::MoveTo( SAFEARRAY ** ppBranches )
{
   if(*ppBranches == NULL)
      return E_INVALIDARG;

   // Clear out the list
   ClearNames();

   HRESULT hr = S_OK;
   LONG lBound=0;
   LONG uBound=0;
   // ServerHandles
   hr = SafeArrayGetLBound(*ppBranches, 1, &lBound);
   if( FAILED(hr) )
      return hr;
   hr = SafeArrayGetUBound(*ppBranches, 1, &uBound);
   if( FAILED(hr) )
      return hr;

   hr = MoveToRoot();
   if( FAILED(hr) )
      return hr;

   for( LONG index=lBound; index<=uBound; index++ )
   {
      BSTR Branch;
      hr = SafeArrayGetElement(*ppBranches, &index, &Branch);
      if( FAILED(hr) )
         return hr;
      if(*Branch != 0 )
      {
         hr = m_pOPCBrowser->ChangeBrowsePosition( OPC_BROWSE_DOWN, Branch );
         SysFreeString( Branch );
         if( FAILED(hr) )
            return hr;
      }
   }
   return hr;
}

/////////////////////////////////////////////////////////////////////
// OPCBrowser::GetItemID method
STDMETHODIMP COPCBrowserImpl::GetItemID( BSTR Leaf, BSTR *pItemID)
{
   if( pItemID == NULL )
      return E_INVALIDARG;

   LPWSTR pName = NULL;
   HRESULT hr = m_pOPCBrowser->GetItemID( Leaf, &pName );
   if( SUCCEEDED(hr) )
   {
      *pItemID = SysAllocString( pName );
      CoTaskMemFree( pName );
   }

   return hr;
}

/////////////////////////////////////////////////////////////////////
// OPCBrowser::GetAccessPaths method
STDMETHODIMP COPCBrowserImpl::GetAccessPaths( BSTR ItemID, VARIANT *pAccessPaths)
{
   if( pAccessPaths == NULL )
      return E_INVALIDARG;

   return E_NOTIMPL;
}

/////////////////////////////////////////////////////////////////////
// ClearNames function
void COPCBrowserImpl::ClearNames()
{
   m_listPosition = m_names.begin();
   for( ; m_listPosition!=m_names.end(); m_listPosition++ )
   {
      CComBSTR* pName = *m_listPosition;
      delete pName;
   }
   m_names.clear();
   m_listPosition = NULL;
}

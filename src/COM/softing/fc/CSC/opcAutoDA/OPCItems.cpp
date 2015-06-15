// OPCItems.cpp
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
//  This is the OPCItems collection in the Group object.
//
//
// Modification Log:
// Vers    Date   By    Notes
// ----  -------- ---   -----
// 1.00  03/7/98  JH    First Release
// 1.01  09/2/98  JH    AddItem & AddItems
//       25/11/99 RT    check NumItems Parameter in AddItems
//
//

#include "stdafx.h"

LPWSTR empty = {L""};
CComBSTR emptyString( empty );

/////////////////////////////////////////////////////////////////////
// OPCItems::Parent property
STDMETHODIMP COPCGroupImpl::get_Parent(OPCGroup **ppParent)
{
   if( ppParent == NULL )
      return E_INVALIDARG;
   *ppParent = NULL;

   // get Interface pointer
   ((COPCGroup*)this)->QueryInterface(IID_IOPCGroup, (void**)ppParent);
   _ASSERTE(*ppParent);

   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCItems::DefaultRequestedDataType property
STDMETHODIMP COPCGroupImpl::get_DefaultRequestedDataType(SHORT *pDefaultRequestedDataType)
{
   if(pDefaultRequestedDataType == NULL)
      return E_INVALIDARG;

   *pDefaultRequestedDataType = m_defaultDataType;
   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCItems::DefaultRequestedDataType property
STDMETHODIMP COPCGroupImpl::put_DefaultRequestedDataType(SHORT DefaultRequestedDataType)
{
   m_defaultDataType = DefaultRequestedDataType;
   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCItems::DefaultAccessPath property
STDMETHODIMP COPCGroupImpl::get_DefaultAccessPath(BSTR *pDefaultAccessPath)
{
   if(pDefaultAccessPath == NULL)
      return E_INVALIDARG;

   *pDefaultAccessPath = m_defaultAccessPath.Copy();
   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCItems::DefaultAccessPath property
STDMETHODIMP COPCGroupImpl::put_DefaultAccessPath(BSTR DefaultAccessPath)
{
   m_defaultAccessPath = DefaultAccessPath;
   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCItems::DefaultIsActive property
STDMETHODIMP COPCGroupImpl::get_DefaultIsActive(VARIANT_BOOL *pDefaultIsActive)
{
   if(pDefaultIsActive == NULL)
      return E_INVALIDARG;

   *pDefaultIsActive = (m_defaultActive ? VARIANT_TRUE : VARIANT_FALSE);
   return S_OK;
}

STDMETHODIMP COPCGroupImpl::put_DefaultIsActive(VARIANT_BOOL DefaultIsActive)
{
   m_defaultActive = DefaultIsActive;
   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCItems::Count property
STDMETHODIMP COPCGroupImpl::get_Count(LONG *pCount)
{
   if(pCount == NULL)
      return E_INVALIDARG;

   *pCount = m_items.size();
   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCItems::_NewEnum property
typedef CComObject<CComEnum<IEnumVARIANT, &IID_IEnumVARIANT, VARIANT, _Copy<VARIANT> > > enumvar;

STDMETHODIMP COPCGroupImpl::get__NewEnum(IUnknown **ppUnk)
{
   if(ppUnk == NULL)
      return E_INVALIDARG;
   *ppUnk = NULL;

   if( m_items.size()==0 )
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


   // Create an array of variants containing IDispatch pointers
   VARIANT* pArray = (VARIANT*)_alloca(m_items.size()*sizeof(VARIANT));

   list<COPCItem*>::iterator listPosition = m_items.begin();
   long index=0;
   while( listPosition!=m_items.end() )
   {
      COPCItem* pItem = *listPosition;

      // get IDispatch pointer for each group
      OPCItem* pDisp=NULL;
      pItem->QueryInterface(IID_OPCItem, (void**)&pDisp);
      _ASSERTE(pDisp);

      // and put it into a variant
      VariantInit( &pArray[index] );
      pArray[index].vt = VT_DISPATCH;
      pArray[index].pdispVal = pDisp;
      listPosition++;
      index++;
   }


   // create an IEnumVariant object initialized with these variants
   enumvar* p = new enumvar;
   _ASSERTE(p);
   HRESULT hr = p->Init(&pArray[0], &pArray[m_items.size()], NULL, AtlFlagCopy);
   if (SUCCEEDED(hr))
      hr = p->QueryInterface(IID_IEnumVARIANT, (void**)ppUnk);
   if (FAILED(hr))
      delete p;

   // Clear the local variant array (to release the COM pointers)
   for( index=0; index < m_items.size(); index++ )
   {
      VariantClear( &pArray[index] );
   }
   return hr;
}

/////////////////////////////////////////////////////////////////////
// OPCItems::Item method
// Item index is 1-based for VB
STDMETHODIMP COPCGroupImpl::Item(VARIANT ItemSpecifier,
                        OPCItem **ppItem)
{
   if( ItemSpecifier.vt != VT_I4 && ItemSpecifier.vt != VT_I2 && ItemSpecifier.vt!=VT_BSTR )
      return E_INVALIDARG;
   if( ItemSpecifier.vt == VT_I4 )
      if( ItemSpecifier.lVal < 1 || ItemSpecifier.lVal > m_items.size() )
         return E_INVALIDARG;
   if( ItemSpecifier.vt == VT_I2 )
      if( ItemSpecifier.iVal < 1 || ItemSpecifier.iVal > m_items.size() )
         return E_INVALIDARG;
   if( ppItem == NULL )
      return E_INVALIDARG;
   *ppItem = NULL;

   LONG itemNum = ItemSpecifier.lVal;
   if( ItemSpecifier.vt == VT_I2 )
      itemNum = ItemSpecifier.iVal;

   LONG index=1;
   list<COPCItem*>::iterator m_listPosition = m_items.begin();
   while( m_listPosition!=m_items.end() )
   {
      COPCItem* pItem = *m_listPosition;
      if( ItemSpecifier.vt==VT_BSTR )
      {
         if( wcscmp(pItem->GetItemID(), ItemSpecifier.bstrVal)==0 )
         {
            HRESULT hr = pItem->QueryInterface(IID_OPCItem, (LPVOID*)ppItem);
            _ASSERTE(*ppItem);
            return hr;
         }
      }
      else if( ItemSpecifier.vt==VT_I4 || ItemSpecifier.vt==VT_I2 )
      {
         if( index == itemNum )
         {
            HRESULT hr = pItem->QueryInterface(IID_OPCItem, (LPVOID*)ppItem);
            _ASSERTE(*ppItem);
            return hr;
         }
      }
      m_listPosition++;
      index++;
   }
   return E_FAIL;
}

/////////////////////////////////////////////////////////////////////
// OPCItems::GetOPCItem method
STDMETHODIMP COPCGroupImpl::GetOPCItem(LONG ServerHandle,
                        OPCItem **ppItem)
{
   if(ppItem == NULL)
      return E_INVALIDARG;
   *ppItem = NULL;

   // loop to find the item
   list<COPCItem*>::iterator m_listPosition = m_items.begin();
   long index=0;
   while( m_listPosition!=m_items.end() )
   {
      COPCItem* pItem = *m_listPosition;
      if( (OPCHANDLE)pItem == ServerHandle )
      {
         pItem->QueryInterface(IID_OPCItem, (LPVOID*)ppItem);
         return S_OK;
      }
      m_listPosition++;
   }

   return E_FAIL;
}

/////////////////////////////////////////////////////////////////////
// OPCItems::AddItems method
STDMETHODIMP COPCGroupImpl::AddItem( BSTR ItemID,
                        LONG ClientHandle,
                        OPCItem **ppItem)
{
   if(ItemID == NULL)
      return E_INVALIDARG;
   if(ppItem == NULL)
      return E_INVALIDARG;
   *ppItem = NULL;

   HRESULT hr = S_OK;
   if( !m_pOPCItem )
   {
      m_pOPCItem = m_pOPCGroup;   // get Item interface
      if( !m_pOPCItem )
         return E_FAIL;
   }

   // Create an OPCITEMDEF
   OPCITEMDEF idef;
   memset( &idef, 0, sizeof(OPCITEMDEF) );

   // Create OPCItems
   COPCItem* pItem;
   HRESULT hRes = COPCItem::CreateInstance(&pItem);
   _ASSERTE(SUCCEEDED(hRes));

   idef.szItemID = ItemID;
   idef.dwBlobSize = 0;
   idef.pBlob = NULL;
   idef.bActive = m_defaultActive ? TRUE : FALSE;
   idef.hClient = (OPCHANDLE)pItem;  // use item's address as client handle
   idef.szAccessPath = m_defaultAccessPath.Copy();
   idef.vtRequestedDataType = m_defaultDataType;


   // add the items in the server
   OPCITEMRESULT * pResults=0;
   HRESULT *pErr=0;
   hr = m_pOPCItem->AddItems(1, &idef, &pResults, &pErr);
   if( FAILED( hr ) || FAILED(pErr[0]) )
   {
      delete pItem;
      return hr;
   }
   _ASSERTE( pItem = (COPCItem*)idef.hClient );

   // Add item to the list
   idef.hClient = ClientHandle;
   pItem->Initialize( &idef, pResults, m_pOPCGroup, this );
   pItem->AddRef();  // hold our reference

   m_items.push_back( pItem );

   // also delete the strings from the client
//   SysFreeString( idef.szItemID );
   SysFreeString( idef.szAccessPath );

   hr = pItem->QueryInterface(IID_OPCItem, (LPVOID*)ppItem);

   return hr;
}

/////////////////////////////////////////////////////////////////////
// OPCItems::AddItems method
STDMETHODIMP COPCGroupImpl::AddItems( LONG NumItems,
                        SAFEARRAY ** ppItemIDs,
                        SAFEARRAY ** ppClientHandles,
                        SAFEARRAY ** ppServerHandles,
                        SAFEARRAY ** ppErrors,
                        VARIANT RequestedDataTypes,   // \/ Optional \/
                        VARIANT AccessPaths)
{
   // RT: crashes from VB without this check if zero items are added
   if (NumItems == 0)
      return E_INVALIDARG;
   if(*ppItemIDs == NULL)
      return E_INVALIDARG;
   if(*ppClientHandles == NULL)
      return E_INVALIDARG;
   if(ppServerHandles == NULL)
      return E_INVALIDARG;
   if(ppErrors == NULL)
      return E_INVALIDARG;

   // Validate input types
   if(RequestedDataTypes.vt != VT_EMPTY && RequestedDataTypes.vt != VT_ERROR )
      if(RequestedDataTypes.vt != (VT_I2 | VT_ARRAY))
         return E_INVALIDARG;

   if(AccessPaths.vt != VT_EMPTY && AccessPaths.vt != VT_ERROR )
      if(AccessPaths.vt != (VT_BSTR | VT_ARRAY))
         return E_INVALIDARG;

   HRESULT hr = S_OK;
   // check safearray bounds
   LONG lBound=0;
   LONG uBound=0;
   // ItemID
   hr = SafeArrayGetLBound(*ppItemIDs, 1, &lBound);
   if( FAILED(hr) )
      return hr;
   hr = SafeArrayGetUBound(*ppItemIDs, 1, &uBound);
   if( FAILED(hr) )
      return hr;
   if( lBound > 1 )
      return E_INVALIDARG;
   if( uBound < NumItems )
      return E_INVALIDARG;

   // ClientHandles
   hr = SafeArrayGetLBound(*ppClientHandles, 1, &lBound);
   if( FAILED(hr) )
      return hr;
   hr = SafeArrayGetUBound(*ppClientHandles, 1, &uBound);
   if( FAILED(hr) )
      return hr;
   if( lBound > 1 )
      return E_INVALIDARG;
   if( uBound < NumItems )
      return E_INVALIDARG;

   if( !m_pOPCItem )
   {
      m_pOPCItem = m_pOPCGroup;   // get Item interface
      if( !m_pOPCItem )
         return E_FAIL;
   }

   // Create array of OPCITEMDEFs
   OPCITEMDEF* pIdef = (OPCITEMDEF*)_alloca(NumItems*sizeof(OPCITEMDEF));
   memset( pIdef, 0, NumItems*sizeof(OPCITEMDEF) );
   for( LONG index=0; index<NumItems; index++ )
      {
      LONG vbIndex = index+1;
      // Create OPCItems
      COPCItem* pItem;
      HRESULT hRes = COPCItem::CreateInstance(&pItem);
      _ASSERTE(SUCCEEDED(hRes));

      hr = SafeArrayGetElement(*ppItemIDs, &vbIndex, &pIdef[index].szItemID);
      if( FAILED(hr) )
         return hr;
      pIdef[index].dwBlobSize = 0;
      pIdef[index].pBlob = NULL;
      pIdef[index].bActive = m_defaultActive ? TRUE : FALSE;
      pIdef[index].hClient = (OPCHANDLE)pItem;  // use item's address as client handle
      if( AccessPaths.vt == (VT_BSTR | VT_ARRAY) )
      {
         hr = SafeArrayGetElement(AccessPaths.parray, &vbIndex, &pIdef[index].szAccessPath);
         if( FAILED(hr) )
            return hr;
      }
      else  // use default
         pIdef[index].szAccessPath = m_defaultAccessPath.Copy();

      if( RequestedDataTypes.vt == (VT_I2 | VT_ARRAY) )
      {
         hr = SafeArrayGetElement(RequestedDataTypes.parray, &vbIndex, &pIdef[index].vtRequestedDataType);
         if( FAILED(hr) )
            return hr;
      }
      else  // use default
         pIdef[index].vtRequestedDataType = m_defaultDataType;
      }

   // add the items in the server
   OPCITEMRESULT * pResults=0;
   HRESULT *pErr=0;
   hr = m_pOPCItem->AddItems(NumItems, pIdef, &pResults, &pErr);
   if( FAILED( hr ) )
   {
      for( long index=0; index<NumItems; index++ )
      {
         COPCItem* pItem = (COPCItem*)pIdef[index].hClient;
         delete pItem;
      }
      return hr;
   }

   SAFEARRAYBOUND bound;
   bound.lLbound = 1;
   bound.cElements = NumItems;

   // Return Server handles in a SAFEARRAY
   {
      SAFEARRAY *pArray = SafeArrayCreate(VT_I4, 1, &bound);
      if(pArray == NULL)
         return E_FAIL;    // temp
      for( long index=0; index<NumItems; index++ )
      {
         LONG vbIndex = index+1;
         DWORD handle = pIdef[index].hClient;
         if( FAILED(pErr[index]) )
            handle = 0;     // item will be deleted 
         SafeArrayPutElement(pArray, &vbIndex, (void *)&handle);
      }
      *ppServerHandles = pArray;
   }

   // Add items to the list
   {
      for( long index=0; index<NumItems; index++ )
      {
         // Get Items and initialize them
         COPCItem* pItem = (COPCItem*)pIdef[index].hClient;
         if( FAILED(pErr[index]) )
            delete pItem;
         else
         {
            LONG vbIndex = index+1;
            // substitute the client's handle
            SafeArrayGetElement(*ppClientHandles, &vbIndex, &pIdef[index].hClient);
            pItem->Initialize( &pIdef[index], &pResults[index], m_pOPCGroup, this );
            pItem->AddRef();  // hold our reference

            m_items.push_back( pItem );
         }

         // also delete the strings from the client
         SysFreeString( pIdef[index].szItemID );
         SysFreeString( pIdef[index].szAccessPath );
      }
   }

   // Return errors in a SAFEARRAY
   {
      SAFEARRAY *pArray = SafeArrayCreate(VT_I4, 1, &bound);
      if(pArray == NULL)
         return E_FAIL;
      for( long index=0; index<NumItems; index++ )
      {
         LONG vbIndex = index+1;
         SafeArrayPutElement(pArray, &vbIndex, (void *)&pErr[index]);
      }
      *ppErrors = pArray;
   }
   if( pResults->pBlob != NULL )
      CoTaskMemFree( pResults->pBlob );
   CoTaskMemFree( pResults );
   CoTaskMemFree( pErr );

   return hr;
}

/////////////////////////////////////////////////////////////////////
// OPCItems::Remove method
STDMETHODIMP COPCGroupImpl::Remove(LONG NumItems,
                        SAFEARRAY ** ppServerHandles,
                        SAFEARRAY ** ppErrors)
{
   if(*ppServerHandles == NULL)
      return E_INVALIDARG;
   if(ppErrors == NULL)
      return E_INVALIDARG;

   HRESULT hr = S_OK;
   LONG lBound=0;
   LONG uBound=0;
   // ServerHandles
   hr = SafeArrayGetLBound(*ppServerHandles, 1, &lBound);
   if( FAILED(hr) )
      return hr;
   hr = SafeArrayGetUBound(*ppServerHandles, 1, &uBound);
   if( FAILED(hr) )
      return hr;
   if( lBound > 1 )
      return E_INVALIDARG;
   if( uBound < NumItems )
      return E_INVALIDARG;

   if( !m_pOPCItem )
   {
      m_pOPCItem = m_pOPCGroup;   // get Item interface
      if( !m_pOPCItem )
         return E_FAIL;
   }

   // Create array of handles
   OPCHANDLE* pSHandles = (OPCHANDLE*)_alloca(NumItems*sizeof(OPCHANDLE));
   memset( pSHandles, 0, NumItems*sizeof(OPCHANDLE) );
   for( long index=0; index<NumItems; index++ )
      {
      OPCHANDLE handle=0;
      LONG vbIndex = index+1;
      hr = SafeArrayGetElement(*ppServerHandles, &vbIndex, &handle);
      if( FAILED(hr) )
         return hr;
      if( handle == 0 )
         return E_INVALIDARG;
      COPCItem* pItem = (COPCItem*)handle;
      pSHandles[index] = pItem->GetServerHandle();
      m_items.remove( pItem );   // remove from list
      pItem->Release();          // and release our reference
      }

   // call the server
   HRESULT *pErr=0;
   hr = m_pOPCItem->RemoveItems(NumItems, pSHandles, &pErr);
   if( FAILED( hr ) )
   {
      return hr;
   }

   // Return errors in a SAFEARRAY
   {
      SAFEARRAYBOUND bound;
      bound.lLbound = 1;
      bound.cElements = NumItems;
      SAFEARRAY *pArray = SafeArrayCreate(VT_I4, 1, &bound);
      if(pArray == NULL)
         return E_FAIL;    // temp
      for( long index=0; index<NumItems; index++ )
      {
         LONG vbIndex = index+1;
         SafeArrayPutElement(pArray, &vbIndex, (void *)&pErr[index]);
      }
      *ppErrors = pArray;
   }
   CoTaskMemFree( pErr );

   return hr;
}

/////////////////////////////////////////////////////////////////////
// OPCItems::Validate method
STDMETHODIMP COPCGroupImpl::Validate(LONG NumItems,
                        SAFEARRAY ** ppItemIDs,
                        SAFEARRAY ** ppErrors,
                        VARIANT RequestedDataTypes,   // \/ Optional \/
                        VARIANT AccessPaths)
{
   if(*ppItemIDs == NULL)
      return E_INVALIDARG;
   if(ppErrors == NULL)
      return E_INVALIDARG;

   if(RequestedDataTypes.vt != VT_EMPTY && RequestedDataTypes.vt != VT_ERROR )
      if(RequestedDataTypes.vt != (VT_I2 | VT_ARRAY))
         return E_INVALIDARG;

   if(AccessPaths.vt != VT_EMPTY && AccessPaths.vt != VT_ERROR )
      if(AccessPaths.vt != (VT_BSTR | VT_ARRAY))
         return E_INVALIDARG;

   HRESULT hr = S_OK;
   LONG lBound=0;
   LONG uBound=0;
   // ItemIDs
   hr = SafeArrayGetLBound(*ppItemIDs, 1, &lBound);
   if( FAILED(hr) )
      return hr;
   hr = SafeArrayGetUBound(*ppItemIDs, 1, &uBound);
   if( FAILED(hr) )
      return hr;
   if( lBound > 1 )
      return E_INVALIDARG;
   if( uBound < NumItems )
      return E_INVALIDARG;

   if( !m_pOPCItem )
   {
      m_pOPCItem = m_pOPCGroup;   // get Item interface
      if( !m_pOPCItem )
         return E_FAIL;
   }

   // Create array of OPCITEMDEFs
   OPCITEMDEF* pIdef = (OPCITEMDEF*)_alloca(NumItems*sizeof(OPCITEMDEF));
   memset( pIdef, 0, NumItems*sizeof(OPCITEMDEF) );
   for( LONG index=0; index<NumItems; index++ )
      {
      LONG vbIndex = index+1;
      hr = SafeArrayGetElement(*ppItemIDs, &vbIndex, &pIdef[index].szItemID);
      if( FAILED(hr) )
         return hr;
      pIdef[index].dwBlobSize = 0;
      pIdef[index].pBlob = NULL;
      pIdef[index].bActive = TRUE;
      pIdef[index].hClient = index;
      if( AccessPaths.vt == (VT_BSTR | VT_ARRAY) )
      {
         hr = SafeArrayGetElement(AccessPaths.parray, &vbIndex, &pIdef[index].szAccessPath);
         if( FAILED(hr) )
            return hr;
      }
      else
         pIdef[index].szAccessPath = emptyString.Copy();

      if( RequestedDataTypes.vt == (VT_I2 | VT_ARRAY) )
      {
         hr = SafeArrayGetElement(RequestedDataTypes.parray, &vbIndex, &pIdef[index].vtRequestedDataType);
         if( FAILED(hr) )
            return hr;
      }
      else  // use default
         pIdef[index].vtRequestedDataType = m_defaultDataType;
      }

   // call the server
   OPCITEMRESULT * pResults=0;
   HRESULT *pErr=0;
   hr = m_pOPCItem->ValidateItems(NumItems, pIdef, FALSE, &pResults, &pErr);
   if( FAILED( hr ) )
   {
      return hr;
   }

   // Return errors in a SAFEARRAY
   {
      SAFEARRAYBOUND bound;
      bound.lLbound = 1;
      bound.cElements = NumItems;
      SAFEARRAY *pArray = SafeArrayCreate(VT_I4, 1, &bound);
      if(pArray == NULL)
         return E_FAIL;    // temp
      for( long index=0; index<NumItems; index++ )
      {
         LONG vbIndex = index+1;
         SafeArrayPutElement(pArray, &vbIndex, (void *)&pErr[index]);

         // also delete the strings from the client
         SysFreeString( pIdef[index].szItemID );
         SysFreeString( pIdef[index].szAccessPath );
      }
      *ppErrors = pArray;
   }
   if( pResults->pBlob != NULL )
      CoTaskMemFree( pResults->pBlob );
   CoTaskMemFree( pResults );
   CoTaskMemFree( pErr );

   return hr;
}

/////////////////////////////////////////////////////////////////////
// OPCItems::SetActive method
STDMETHODIMP COPCGroupImpl::SetActive(LONG NumItems,
                        SAFEARRAY ** ppServerHandles,
                        VARIANT_BOOL ActiveState,
                        SAFEARRAY ** ppErrors)
{
   if(*ppServerHandles == NULL)
      return E_INVALIDARG;
   if(ppErrors == NULL)
      return E_INVALIDARG;

   BOOL active = (ActiveState==VARIANT_TRUE ? TRUE : FALSE);

   HRESULT hr = S_OK;
   LONG lBound=0;
   LONG uBound=0;
   // Array bounds checking
   hr = SafeArrayGetLBound(*ppServerHandles, 1, &lBound);
   if( FAILED(hr) )
      return hr;
   hr = SafeArrayGetUBound(*ppServerHandles, 1, &uBound);
   if( FAILED(hr) )
      return hr;
   if( lBound > 1 )
      return E_INVALIDARG;
   if( uBound < NumItems )
      return E_INVALIDARG;

   if( !m_pOPCItem )
   {
      m_pOPCItem = m_pOPCGroup;   // get Item interface
      if( !m_pOPCItem )
         return E_FAIL;
   }

   // Create array of handles
   OPCHANDLE* pSHandles = (OPCHANDLE*)_alloca(NumItems*sizeof(OPCHANDLE));
   memset( pSHandles, 0, NumItems*sizeof(OPCHANDLE) );
   for( long index=0; index<NumItems; index++ )
      {
      LONG vbIndex = index+1;
      DWORD localHandle=0;
      hr = SafeArrayGetElement(*ppServerHandles, &vbIndex, &localHandle);
      if( FAILED(hr) )
         return hr;
      if( localHandle == 0 )
         return E_INVALIDARG;
      COPCItem* pItem = (COPCItem*)localHandle;
//      if( !m_items.Lookup( (LPVOID)localHandle, pItem ) )
//         return E_INVALIDARG;
      pSHandles[index] = pItem->GetServerHandle();
      }

   // call the server
   HRESULT *pErr=0;
   hr = m_pOPCItem->SetActiveState(NumItems, pSHandles, active, &pErr);
   if( FAILED( hr ) )
   {
      return hr;
   }

   // Return errors in a SAFEARRAY
   {
      SAFEARRAYBOUND bound;
      bound.lLbound = 1;
      bound.cElements = NumItems;
      SAFEARRAY *pArray = SafeArrayCreate(VT_I4, 1, &bound);
      if(pArray == NULL)
         return E_FAIL;    // temp
      for( long index=0; index<NumItems; index++ )
      {
         LONG vbIndex = index+1;
         SafeArrayPutElement(pArray, &vbIndex, (void *)&pErr[index]);
         if( SUCCEEDED(pErr[index]) )
         {
            DWORD localHandle=0;
            hr = SafeArrayGetElement(*ppServerHandles, &vbIndex, &localHandle);
            COPCItem* pItem = (COPCItem*)localHandle;
            pItem->SetActiveState(active);
         }
      }
      *ppErrors = pArray;
   }
   CoTaskMemFree( pErr );

   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCItems::SetClientHandles method
STDMETHODIMP COPCGroupImpl::SetClientHandles(
                        LONG NumItems,
                        SAFEARRAY ** ppServerHandles,
                        SAFEARRAY ** ppClientHandles,
                        SAFEARRAY ** ppErrors)
{
   if(*ppServerHandles == NULL)
      return E_INVALIDARG;
   if(*ppClientHandles == NULL)
      return E_INVALIDARG;
   if(ppErrors == NULL)
      return E_INVALIDARG;

   HRESULT hr = S_OK;
   LONG lBound=0;
   LONG uBound=0;
   // Array bounds checking
   hr = SafeArrayGetLBound(*ppServerHandles, 1, &lBound);
   if( FAILED(hr) )
      return hr;
   hr = SafeArrayGetUBound(*ppServerHandles, 1, &uBound);
   if( FAILED(hr) )
      return hr;
   if( lBound > 1 )
      return E_INVALIDARG;
   if( uBound < NumItems )
      return E_INVALIDARG;

   hr = SafeArrayGetLBound(*ppClientHandles, 1, &lBound);
   if( FAILED(hr) )
      return hr;
   hr = SafeArrayGetUBound(*ppClientHandles, 1, &uBound);
   if( FAILED(hr) )
      return hr;
   if( lBound > 1 )
      return E_INVALIDARG;
   if( uBound < NumItems )
      return E_INVALIDARG;

   // Set Item's client handles
   for( long index=0; index<NumItems; index++ )
      {
      LONG vbIndex = index+1;
      DWORD localHandle=0;
      hr = SafeArrayGetElement(*ppServerHandles, &vbIndex, &localHandle);
      if( FAILED(hr) )
         return hr;
      if( localHandle == 0 )
         return E_INVALIDARG;
      COPCItem* pItem = (COPCItem*)localHandle;
//      if( !m_items.Lookup( (LPVOID)localHandle, pItem ) )
//         return E_INVALIDARG;
      DWORD clientHandle = 0;
      hr = SafeArrayGetElement(*ppClientHandles, &vbIndex, &clientHandle);
      pItem->SetClientHandle( clientHandle );
      }

   // Return errors in a SAFEARRAY
   HRESULT ok = S_OK;
   {
      SAFEARRAYBOUND bound;
      bound.lLbound = 1;
      bound.cElements = NumItems;
      SAFEARRAY *pArray = SafeArrayCreate(VT_I4, 1, &bound);
      if(pArray == NULL)
         return E_FAIL;    // temp
      for( long index=0; index<NumItems; index++ )
      {
         LONG vbIndex = index+1;
         SafeArrayPutElement(pArray, &vbIndex, (void *)&ok);
      }
      *ppErrors = pArray;
   }

   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCItems::SetDataTypes method
STDMETHODIMP COPCGroupImpl::SetDataTypes(
                        LONG NumItems,
                        SAFEARRAY ** ppServerHandles,
                        SAFEARRAY ** ppRequestedDataTypes,
                        SAFEARRAY ** ppErrors)
{
   if(*ppServerHandles == NULL)
      return E_INVALIDARG;
   if(*ppRequestedDataTypes == NULL)
      return E_INVALIDARG;
   if(ppErrors == NULL)
      return E_INVALIDARG;

   HRESULT hr = S_OK;
   LONG lBound=0;
   LONG uBound=0;
   // Array bounds checking
   hr = SafeArrayGetLBound(*ppServerHandles, 1, &lBound);
   if( FAILED(hr) )
      return hr;
   hr = SafeArrayGetUBound(*ppServerHandles, 1, &uBound);
   if( FAILED(hr) )
      return hr;
   if( lBound > 1 )
      return E_INVALIDARG;
   if( uBound < NumItems )
      return E_INVALIDARG;

   hr = SafeArrayGetLBound(*ppRequestedDataTypes, 1, &lBound);
   if( FAILED(hr) )
      return hr;
   hr = SafeArrayGetUBound(*ppRequestedDataTypes, 1, &uBound);
   if( FAILED(hr) )
      return hr;
   if( lBound > 1 )
      return E_INVALIDARG;
   if( uBound < NumItems )
      return E_INVALIDARG;

   if( !m_pOPCItem )
   {
      m_pOPCItem = m_pOPCGroup;   // get Item interface
      if( !m_pOPCItem )
         return E_FAIL;
   }

   // Create array of handles
   OPCHANDLE* pSHandles = (OPCHANDLE*)_alloca(NumItems*sizeof(OPCHANDLE));
   memset( pSHandles, 0, NumItems*sizeof(OPCHANDLE) );
   for( long index=0; index<NumItems; index++ )
      {
      LONG vbIndex = index+1;
      DWORD localHandle=0;
      hr = SafeArrayGetElement(*ppServerHandles, &vbIndex, &localHandle);
      if( FAILED(hr) )
         return hr;
      if( localHandle == 0 )
         return E_INVALIDARG;
      COPCItem* pItem = (COPCItem*)localHandle;
//      if( !m_items.Lookup( (LPVOID)localHandle, pItem ) )
//         return E_INVALIDARG;
      pSHandles[index] = pItem->GetServerHandle();
      }

   // Create array of datatypes
   VARTYPE* pTypes = (VARTYPE*)_alloca(NumItems*sizeof(VARTYPE)+2);
   for( index=0; index<NumItems; index++ )
   {
      LONG vbIndex = index+1;
      hr = SafeArrayGetElement(*ppRequestedDataTypes, &vbIndex, &pTypes[index]);
   }

   // call the server
   HRESULT *pErr=0;
   hr = m_pOPCItem->SetDatatypes(NumItems, pSHandles, pTypes, &pErr);
   if( FAILED( hr ) )
   {
      return hr;
   }

   // Return errors in a SAFEARRAY
   {
      SAFEARRAYBOUND bound;
      bound.lLbound = 1;
      bound.cElements = NumItems;
      SAFEARRAY *pArray = SafeArrayCreate(VT_I4, 1, &bound);
      if(pArray == NULL)
         return E_FAIL;    // temp
      for( long index=0; index<NumItems; index++ )
      {
         LONG vbIndex = index+1;
         SafeArrayPutElement(pArray, &vbIndex, (void *)&pErr[index]);
         if( SUCCEEDED(pErr[index]) )
         {
            DWORD localHandle=0;
            hr = SafeArrayGetElement(*ppServerHandles, &vbIndex, &localHandle);
            COPCItem* pItem = (COPCItem*)localHandle;
            pItem->SetDataType(pTypes[index]);
         }
      }
      *ppErrors = pArray;
   }
   CoTaskMemFree( pErr );

   return S_OK;
}

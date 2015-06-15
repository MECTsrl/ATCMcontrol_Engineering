// OPCGroups.cpp
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
//  This is the Server Object's OPCGroups interface
//
//
// Modification Log:
// Vers    Date   By    Notes
// ----  -------- ---   -----
// 1.00  03/7/98  JH    First Release
//

#include "stdafx.h"

//**************************************************************************
STDMETHODIMP COPCAutoServer::get_Parent(IOPCAutoServer ** ppParent)
{
   if( ppParent == NULL )
      return E_INVALIDARG;
   *ppParent = NULL;

   // get Interface pointer
   ((COPCAutoServerObject*)this)->QueryInterface(IID_IOPCAutoServer, (void**)ppParent);
   _ASSERTE(*ppParent);

   return S_OK;
}

//**************************************************************************
STDMETHODIMP COPCAutoServer::get_DefaultGroupIsActive(VARIANT_BOOL * pDefaultGroupIsActive)
{
   if( pDefaultGroupIsActive == NULL )
      return E_INVALIDARG;

   *pDefaultGroupIsActive = m_defaultActive;

   return S_OK;
}

//**************************************************************************
STDMETHODIMP COPCAutoServer::put_DefaultGroupIsActive(VARIANT_BOOL DefaultGroupIsActive)
{
   m_defaultActive = DefaultGroupIsActive;
   return S_OK;
}

//**************************************************************************
STDMETHODIMP COPCAutoServer::get_DefaultGroupUpdateRate(LONG * pDefaultGroupUpdateRate)
{
   if( pDefaultGroupUpdateRate == NULL )
      return E_INVALIDARG;

   *pDefaultGroupUpdateRate = m_defaultUpdate;

   return S_OK;
}

//**************************************************************************
STDMETHODIMP COPCAutoServer::put_DefaultGroupUpdateRate(LONG DefaultGroupUpdateRate)
{
   m_defaultUpdate = DefaultGroupUpdateRate;
   return S_OK;
}

//**************************************************************************
STDMETHODIMP COPCAutoServer::get_DefaultGroupDeadband(float *pDefaultGroupDeadband)
{
   if( pDefaultGroupDeadband == NULL )
      return E_INVALIDARG;

   *pDefaultGroupDeadband = m_defaultDeadband;

   return S_OK;
}

//**************************************************************************
STDMETHODIMP COPCAutoServer::put_DefaultGroupDeadband(float DefaultGroupDeadband)
{
   m_defaultDeadband = DefaultGroupDeadband;
   return S_OK;
}

//**************************************************************************
STDMETHODIMP COPCAutoServer::get_DefaultGroupLocaleID(LONG * pDefaultGroupLocaleID)
{
   if( pDefaultGroupLocaleID == NULL )
      return E_INVALIDARG;

   *pDefaultGroupLocaleID = m_defaultLocale;

   return S_OK;
}

//**************************************************************************
STDMETHODIMP COPCAutoServer::put_DefaultGroupLocaleID(LONG DefaultGroupLocaleID)
{
   m_defaultLocale = DefaultGroupLocaleID;
   return S_OK;
}

//**************************************************************************
STDMETHODIMP COPCAutoServer::get_DefaultGroupTimeBias(LONG * pDefaultGroupTimeBias)
{
   if( pDefaultGroupTimeBias == NULL )
      return E_INVALIDARG;

   *pDefaultGroupTimeBias = m_defaultTimeBias;

   return S_OK;
}

//**************************************************************************
STDMETHODIMP COPCAutoServer::put_DefaultGroupTimeBias(LONG DefaultGroupTimeBias)
{
   m_defaultTimeBias = DefaultGroupTimeBias;
   return S_OK;
}

//**************************************************************************
STDMETHODIMP COPCAutoServer::get_Count(LONG *pCount)
{
   if( pCount == NULL )
      return E_INVALIDARG;

   *pCount = m_groups.size();

   return S_OK;
}

//**************************************************************************
typedef CComObject<CComEnum<IEnumVARIANT, &IID_IEnumVARIANT, VARIANT, _Copy<VARIANT> > > enumvar;

STDMETHODIMP COPCAutoServer::get__NewEnum(IUnknown ** ppUnk)
{
   if( ppUnk == NULL )
      return E_INVALIDARG;
   *ppUnk = NULL;

   if( m_groups.size()==0 )
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
   VARIANT* pArray = (VARIANT*)_alloca(m_groups.size()*sizeof(VARIANT));

   list<COPCGroup*>::iterator m_listPosition = m_groups.begin();
   long index=0;
   while( m_listPosition!=m_groups.end() )
   {
      COPCGroup* pGroup = *m_listPosition;

      // get IDispatch pointer for each group
      IOPCGroup* pDisp=NULL;
      pGroup->QueryInterface(IID_IOPCGroup, (void**)&pDisp);
      _ASSERTE(pDisp);

      // and put it into a variant
      VariantInit( &pArray[index] );
      pArray[index].vt = VT_DISPATCH;
      pArray[index].pdispVal = pDisp;
      m_listPosition++;
      index++;
   }


   // create an IEnumVariant object initialized with these variants
   enumvar* p = new enumvar;
   _ASSERTE(p);
   HRESULT hr = p->Init(&pArray[0], &pArray[m_groups.size()], NULL, AtlFlagCopy);
   if (SUCCEEDED(hr))
      hr = p->QueryInterface(IID_IEnumVARIANT, (void**)ppUnk);
   if (FAILED(hr))
      delete p;

   // Clear the local variant array (to release the COM pointers)
   for( index=0; index < m_groups.size(); index++ )
   {
      VariantClear( &pArray[index] );
   }
   return hr;
}

//**************************************************************************
// Item accepts 1-based index or name
STDMETHODIMP COPCAutoServer::Item(VARIANT ItemSpecifier, OPCGroup ** ppGroup)
{
   if( ItemSpecifier.vt != VT_I4 && ItemSpecifier.vt != VT_I2 && ItemSpecifier.vt!=VT_BSTR )
      return E_INVALIDARG;
   if( ItemSpecifier.vt == VT_I4 )
      if( ItemSpecifier.lVal < 1 || ItemSpecifier.lVal > m_groups.size() )
         return E_INVALIDARG;
   if( ItemSpecifier.vt == VT_I2 )
      if( ItemSpecifier.iVal < 1 || ItemSpecifier.iVal > m_groups.size() )
         return E_INVALIDARG;
   if( ppGroup == NULL )
      return E_INVALIDARG;
   *ppGroup = NULL;

   LONG itemNum = ItemSpecifier.lVal;
   if( ItemSpecifier.vt == VT_I2 )
      itemNum = ItemSpecifier.iVal;

   // loop to find the group
   list<COPCGroup*>::iterator m_listPosition = m_groups.begin();
   long index=1;
   while( m_listPosition!=m_groups.end() )
   {
      COPCGroup* pGroup = *m_listPosition;
      if( ItemSpecifier.vt==VT_BSTR )
      {
         if( wcscmp(pGroup->GetName(), ItemSpecifier.bstrVal)==0 )
         {
            HRESULT hr = pGroup->QueryInterface(IID_IOPCGroup, (LPVOID*)ppGroup);
            _ASSERTE(*ppGroup);
            return hr;
         }
      }
      else if( ItemSpecifier.vt==VT_I4 || ItemSpecifier.vt==VT_I2 )
      {
         if( index == itemNum )
         {
            HRESULT hr = pGroup->QueryInterface(IID_IOPCGroup, (LPVOID*)ppGroup);
            _ASSERTE(*ppGroup);
            return hr;
         }
      }
      m_listPosition++;
      index++;
   }

   return E_FAIL;
}

//**************************************************************************
STDMETHODIMP COPCAutoServer::Add(VARIANT Name,
                     OPCGroup ** ppGroup)
{
   if( ppGroup == NULL )
      return E_INVALIDARG;
   *ppGroup = NULL;

   if( !m_pOPCServer )
      return E_FAIL;

   // Create an Automation group object
   COPCGroup* pGroup;
   HRESULT hr = COPCGroup::CreateInstance(&pGroup);
   _ASSERTE(SUCCEEDED(hr));
   if( FAILED(hr) )
   {
      return hr;
   }

   // Create a group in the server
   CComBSTR groupName(L"");
   if( Name.scode != DISP_E_PARAMNOTFOUND
    && Name.vt == VT_BSTR
    && Name.bstrVal != NULL )
      groupName = Name.bstrVal;
   DWORD RevisedUpdateRate=0;
   OPCHANDLE ServerHandle=0;
   IUnknown *pUnk;
   hr = m_pOPCServer->AddGroup(  groupName,
                                 m_defaultActive,
                                 m_defaultUpdate,
                                 (OPCHANDLE)pGroup, // Our pointer is client handle
                                 &m_defaultTimeBias,
                                 &m_defaultDeadband,
                                 m_defaultLocale,
                                 &ServerHandle,
                                 &RevisedUpdateRate,
                                 IID_IOPCGroupStateMgt,
                                 &pUnk );
   if( FAILED(hr) )
   {
      delete pGroup;
      return hr;
   }

   hr = pGroup->Initialize( this, pUnk );
   pUnk->Release();
   pUnk = NULL;
   if( FAILED(hr) )
   {
      delete pGroup;
      return hr;
   }

   m_groups.push_back( pGroup );

   // get Automation interface pointer
   pGroup->QueryInterface(IID_IOPCGroup, (void**)ppGroup);
   pGroup->AddRef(); // our reference
   _ASSERTE(*ppGroup);

   return S_OK;
}

//**************************************************************************
STDMETHODIMP COPCAutoServer::GetOPCGroup(VARIANT ItemSpecifier, OPCGroup ** ppGroup)
{
   if( ppGroup == NULL )
      return E_INVALIDARG;
   *ppGroup = NULL;
   if( ItemSpecifier.vt!=VT_I4 && ItemSpecifier.vt!=VT_BSTR )
      return E_INVALIDARG;

   // loop to find the group
   list<COPCGroup*>::iterator m_listPosition = m_groups.begin();
   long index=0;
   while( m_listPosition!=m_groups.end() )
   {
      COPCGroup* pGroup = *m_listPosition;
      if( ItemSpecifier.vt==VT_BSTR )
      {
         if( wcscmp(pGroup->GetName(), ItemSpecifier.bstrVal)==0 )
         {
            pGroup->QueryInterface(IID_IOPCGroup, (LPVOID*)ppGroup);
            return S_OK;
         }
      }
      else if( ItemSpecifier.vt==VT_I4 )
      {
         if( (OPCHANDLE)pGroup == ItemSpecifier.lVal )
         {
            pGroup->QueryInterface(IID_IOPCGroup, (LPVOID*)ppGroup);
            return S_OK;
         }
      }
      m_listPosition++;
   }

   return E_FAIL;
}

//**************************************************************************
STDMETHODIMP COPCAutoServer::RemoveAll(void)
{
   while( m_groups.size() )
   {
      COPCGroup* pGroup = *m_groups.begin();
      _ASSERTE(pGroup!=NULL);
      CComVariant var( (LONG)pGroup ); // Remove takes a variant
      HRESULT hr = Remove( var );
   }

   return S_OK;
}

//**************************************************************************
STDMETHODIMP COPCAutoServer::Remove(VARIANT ItemSpecifier)
{
   if( ItemSpecifier.vt!=VT_I4 && ItemSpecifier.vt!=VT_BSTR )
      return E_INVALIDARG;

   // loop to find the specified group
   COPCGroup* pRemoveGroup = NULL;
   list<COPCGroup*>::iterator m_listPosition = m_groups.begin();
   long index=0;
   while( m_listPosition!=m_groups.end() )
   {
      COPCGroup* pGroup = *m_listPosition;
      if( ItemSpecifier.vt==VT_BSTR )
      {
         if( wcscmp(pGroup->GetName(), ItemSpecifier.bstrVal)==0 )
         {
            pRemoveGroup = pGroup;
         }
      }
      else if( ItemSpecifier.vt==VT_I4 )
      {
         if( (OPCHANDLE)pGroup == ItemSpecifier.lVal )
         {
            pRemoveGroup = pGroup;
         }
      }
      // if a group is found, remove it
      if( pRemoveGroup != NULL )
      {
         pRemoveGroup->put_IsSubscribed( VARIANT_FALSE );
         HRESULT hr = m_pOPCServer->RemoveGroup(pRemoveGroup->GetServerHandle(), FALSE);
         m_groups.erase( m_listPosition );
         pRemoveGroup->Release();
         return hr;
      }
      m_listPosition++;
   }

   return E_INVALIDARG;
}

//**************************************************************************
STDMETHODIMP COPCAutoServer::ConnectPublicGroup(
                     BSTR Name,
                     OPCGroup ** ppGroup)
{
   if( ppGroup == NULL )
      return E_INVALIDARG;
   *ppGroup = NULL;

   return E_NOTIMPL;
}

//**************************************************************************
STDMETHODIMP COPCAutoServer::RemovePublicGroup(VARIANT ItemSpecifier)
{
   return E_NOTIMPL;
}
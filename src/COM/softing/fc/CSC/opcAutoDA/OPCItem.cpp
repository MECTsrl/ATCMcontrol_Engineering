// OPCItem.cpp
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
//  This is the OPCItem object.
//
//
// Modification Log:
// Vers    Date   By    Notes
// ----  -------- ---   -----
// 1.00  03/7/98  JH    First Release
//
// ToDo:
//    EUxxx

#include "stdafx.h"

/////////////////////////////////////////////////////////////////////
// Constructor
COPCItemImpl::COPCItemImpl()
{
   m_active = TRUE;
   m_accessRights = 0;
   m_datatype = VT_EMPTY;
   m_nativeDatatype = VT_EMPTY;
   m_quality = 0;
   CoFileTimeNow( &m_timestamp );
   m_client = 0;
   m_server = 0;
}

COPCItemImpl::~COPCItemImpl()
{
   m_pOPCItem.Release();
   m_pOPCSyncIO.Release();
}

/////////////////////////////////////////////////////////////////////
// Initialize function
HRESULT COPCItemImpl::Initialize( OPCITEMDEF* pIdef,
                                  OPCITEMRESULT* pResult,
                                  IOPCGroupStateMgt* pGroup,
                                  COPCGroupImpl* pParent )
{
   m_pParent = pParent;
   m_itemID = pIdef->szItemID;
   m_accessPath = pIdef->szAccessPath;
   m_active = pIdef->bActive;
   m_accessRights = pResult->dwAccessRights;
   m_datatype = pIdef->vtRequestedDataType;
   m_nativeDatatype = pResult->vtCanonicalDataType;
   m_client = pIdef->hClient;
   m_server = pResult->hServer;
   m_quality = 0;
   CoFileTimeNow( &m_timestamp );

   // get interface pointer
   m_pOPCItem = pGroup;
   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCItem Update function
void COPCItemImpl::Update( const OPCITEMSTATE* pItemState )
{
   VariantClear( &m_value );
   m_quality = pItemState->wQuality;
   m_value = pItemState->vDataValue;
   VariantToAutomation( &m_value );
   m_timestamp = pItemState->ftTimeStamp;
}

/////////////////////////////////////////////////////////////////////
// OPCItem Update function
void COPCItemImpl::Update(const VARIANT *pValue,
                          const DWORD quality,
                          const FILETIME timestamp )
{
   VariantClear( &m_value );
   m_quality = quality;
   m_value = *pValue;
   VariantToAutomation( &m_value );
   m_timestamp = timestamp;
}

/////////////////////////////////////////////////////////////////////
// OPCItem::Parent property
STDMETHODIMP COPCItemImpl::get_Parent(OPCGroup **ppParent)
{
   if( ppParent == NULL )
      return E_INVALIDARG;
   *ppParent = NULL;

   // get Interface pointer
   ((COPCGroup*)m_pParent)->QueryInterface(IID_IOPCGroup, (void**)ppParent);
   _ASSERTE(*ppParent);

   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCItem::ClientHandle property
STDMETHODIMP COPCItemImpl::get_ClientHandle(LONG *pClientHandle)
{
   if(pClientHandle == NULL)
      return E_INVALIDARG;

   *pClientHandle = m_client;
   return S_OK;
}

STDMETHODIMP COPCItemImpl::put_ClientHandle(LONG ClientHandle)
{
   m_client = ClientHandle;
   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCItem::ServerHandle property
STDMETHODIMP COPCItemImpl::get_ServerHandle(LONG *pServerHandle)
{
   if(pServerHandle == NULL)
      return E_INVALIDARG;

   *pServerHandle = (LONG)this;
   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCItem::AccessPath property
STDMETHODIMP COPCItemImpl::get_AccessPath(BSTR *pAccessPath)
{
   if(pAccessPath == NULL)
      return E_INVALIDARG;

   *pAccessPath = m_accessPath.Copy();
   return S_OK;
}

STDMETHODIMP COPCItemImpl::get_AccessRights(LONG *pAccessRights)
{
   if(pAccessRights == NULL)
      return E_INVALIDARG;

   *pAccessRights = m_accessRights;
   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCItem::ItemID property
STDMETHODIMP COPCItemImpl::get_ItemID(BSTR *pItemID)
{
   if(pItemID == NULL)
      return E_INVALIDARG;

   *pItemID = m_itemID.Copy();
   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCItem::IsActive property
STDMETHODIMP COPCItemImpl::get_IsActive(VARIANT_BOOL *pIsActive)
{
   if(pIsActive == NULL)
      return E_INVALIDARG;

   *pIsActive = (m_active ? VARIANT_TRUE : VARIANT_FALSE);
   return S_OK;
}

STDMETHODIMP COPCItemImpl::put_IsActive(VARIANT_BOOL IsActive)
{
   m_active = IsActive;
   HRESULT *pErr=0;
   HRESULT hr = m_pOPCItem->SetActiveState(1, &m_server, m_active, &pErr);
   if( SUCCEEDED(hr) )
      CoTaskMemFree( pErr );

   return hr;
}

/////////////////////////////////////////////////////////////////////
// OPCItem::RequestedDataType property
STDMETHODIMP COPCItemImpl::get_RequestedDataType(SHORT *pRequestedDataType)
{
   if(pRequestedDataType == NULL)
      return E_INVALIDARG;

   *pRequestedDataType = m_datatype;
   return S_OK;
}

STDMETHODIMP COPCItemImpl::put_RequestedDataType(SHORT RequestedDataType)
{
   m_datatype = RequestedDataType;
   HRESULT *pErr=0;
   HRESULT hr = m_pOPCItem->SetDatatypes(1, &m_server, &m_datatype, &pErr);
   if( SUCCEEDED(hr) )
      CoTaskMemFree( pErr );

   return hr;
}

/////////////////////////////////////////////////////////////////////
// OPCItem::Value property
STDMETHODIMP COPCItemImpl::get_Value(VARIANT *pCurrentValue)
{
   if(pCurrentValue == NULL)
      return E_INVALIDARG;

   VariantClear( pCurrentValue );
   HRESULT hr = VariantCopy( pCurrentValue, &m_value );
   return hr;
}

/////////////////////////////////////////////////////////////////////
// OPCItem::Quality property
STDMETHODIMP COPCItemImpl::get_Quality(LONG *pQuality)
{
   if(pQuality == NULL)
      return E_INVALIDARG;

   *pQuality = m_quality;
   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCItem::TimeStamp property
STDMETHODIMP COPCItemImpl::get_TimeStamp(DATE *pTimeStamp)
{
   if(pTimeStamp == NULL)
      return E_INVALIDARG;

   FILETIME filetimeLocal;
   if( FileTimeToLocalFileTime( &m_timestamp, &filetimeLocal) )
   {
      SYSTEMTIME systime;
      if( FileTimeToSystemTime(&filetimeLocal, &systime) )
      {
	      SystemTimeToVariantTime( &systime, pTimeStamp);
	   }
   }

   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCItem::CanonicalDataType property
STDMETHODIMP COPCItemImpl::get_CanonicalDataType(SHORT *pCanonicalDataType)
{
   if(pCanonicalDataType == NULL)
      return E_INVALIDARG;

   *pCanonicalDataType = m_nativeDatatype;
   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCItem::EUType property
STDMETHODIMP COPCItemImpl::get_EUType(SHORT * pEUType)
{
   if(pEUType == NULL)
      return E_INVALIDARG;

   return E_NOTIMPL;
}

/////////////////////////////////////////////////////////////////////
// OPCItem::EUInfo property
STDMETHODIMP COPCItemImpl::get_EUInfo(VARIANT * pEUInfo)
{
   if(pEUInfo == NULL)
      return E_INVALIDARG;

   return E_NOTIMPL;
}

/////////////////////////////////////////////////////////////////////
// OPCItem::Read method
STDMETHODIMP COPCItemImpl::Read( SHORT Source,
                                 VARIANT *pValue,
                                 VARIANT *pQuality,
                                 VARIANT *pTimeStamp)
{
   if( !m_pOPCSyncIO )
   {
      m_pOPCSyncIO = m_pOPCItem;   // get Sync interface
      if( !m_pOPCSyncIO )
         return E_FAIL;
   }

   OPCITEMSTATE* pItemState;
   HRESULT* pErrors;
   HRESULT hr = m_pOPCSyncIO->Read((OPCDATASOURCE)Source, 1, &m_server, &pItemState, &pErrors);
   if( SUCCEEDED(hr) )
   {
      hr = pErrors[0];
      if( SUCCEEDED(hr) )
      {
         _ASSERTE( pItemState->hClient == (OPCHANDLE)this );
         Update( pItemState );

         // return values
         if(pValue != NULL && pValue->vt != VT_ERROR && pValue->scode != DISP_E_PARAMNOTFOUND )
         {
            VariantClear( pValue );
            hr = VariantCopy( pValue, &pItemState->vDataValue );
            if( SUCCEEDED(hr) )
               hr = VariantToAutomation( pValue );
         }
         if(pQuality != NULL && pQuality->vt != VT_ERROR && pQuality->scode != DISP_E_PARAMNOTFOUND )
         {
            VariantClear( pQuality );
            pQuality->vt = VT_I2;
            pQuality->iVal = (short)m_quality;
         }
         if(pTimeStamp != NULL && pTimeStamp->vt != VT_ERROR && pTimeStamp->scode != DISP_E_PARAMNOTFOUND )
         {
            VariantClear( pTimeStamp );
            DATE date = NULL;
            FILETIME filetimeLocal;
            if( FileTimeToLocalFileTime( &m_timestamp, &filetimeLocal) )
            {
               SYSTEMTIME systime;
               if( FileTimeToSystemTime(&filetimeLocal, &systime) )
               {
	               SystemTimeToVariantTime( &systime, &date);
	            }
            }
            pTimeStamp->vt = VT_DATE;
            pTimeStamp->date = date;
         }
      }
      VariantClear( &pItemState->vDataValue );
      CoTaskMemFree( pItemState );
      CoTaskMemFree( pErrors );
   }
   return hr;
}

/////////////////////////////////////////////////////////////////////
// OPCItem::Write method
STDMETHODIMP COPCItemImpl::Write(VARIANT Value)
{
   if( !m_pOPCSyncIO )
   {
      m_pOPCSyncIO = m_pOPCItem;   // get Sync interface
      if( !m_pOPCSyncIO )
         return E_FAIL;
   }

   HRESULT* pErrors;
   m_value = Value;
   HRESULT hr = m_pOPCSyncIO->Write( 1, &m_server, &m_value, &pErrors);
   if( SUCCEEDED(hr) )
   {
      if( hr == S_FALSE )
         hr = pErrors[0];
      CoTaskMemFree( pErrors );
   }
   return hr;
}

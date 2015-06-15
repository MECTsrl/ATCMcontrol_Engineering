// OPCGroup.cpp
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
//  This is the Group Object's OPCGroup interface.
//
//
// Modification Log:
// Vers    Date   By    Notes
// ----  -------- ---   -----
// 1.00  03/7/98  JH    First Release
//

#include "stdafx.h"

extern UINT OPCSTMFORMATDATATIME;
extern UINT OPCSTMFORMATWRITECOMPLETE;
/////////////////////////////////////////////////////////////////////
// Constructor
COPCGroupImpl::COPCGroupImpl()
{
   m_rate = 0;
   m_active = FALSE;
   m_timebias = 0;
   m_deadband = 0.0F;
   m_LCID = LOCALE_SYSTEM_DEFAULT;
   m_client = (OPCHANDLE)this;
   m_server = 0;

   m_defaultDataType = VT_EMPTY;
   m_defaultActive = VARIANT_TRUE;
   m_pParent = NULL;
   m_DataConnection = 0;
   m_WriteConnection = 0;
   m_usingCP = FALSE;
   m_subscribed = VARIANT_FALSE;
   m_asyncReading = FALSE;
}

COPCGroupImpl::~COPCGroupImpl()
{
   put_IsSubscribed( VARIANT_FALSE );

   // Create array of handles
   int NumItems = m_items.size();
   if( NumItems > 0 )
   {
       OPCHANDLE* pSHandles = (OPCHANDLE*)_alloca(NumItems*sizeof(OPCHANDLE));
       memset( pSHandles, 0, NumItems*sizeof(OPCHANDLE) );

       list<COPCItem*>::iterator m_listPosition = m_items.begin();
       int index = 0;
       while( m_listPosition!=m_items.end() )
       {
          COPCItem* pItem = *m_listPosition;
          pSHandles[index] = pItem->GetServerHandle();
          pItem->Release();          // and release our reference
          m_listPosition++;
          index++;
       }
       // call the server
       HRESULT *pErr=0;
       HRESULT hr = m_pOPCItem->RemoveItems(NumItems, pSHandles, &pErr);
   }

   if (m_pParent != (COPCAutoServerObject*)0)
	   ((COPCAutoServerObject*)m_pParent)->Release();
   // destructors will release all interface pointers
}

/////////////////////////////////////////////////////////////////////
// Initialize function
HRESULT COPCGroupImpl::Initialize( COPCAutoServer* pParent,
                                   IUnknown* pUnk )
{
   if(pUnk == NULL)
      return E_INVALIDARG;

   HRESULT hr=S_OK;

   m_pParent = pParent;
   ((COPCAutoServerObject*)m_pParent)->AddRef();
   m_pOPCGroup = pUnk;  // get a group interface pointer

   if( !m_pOPCGroup )
      return E_FAIL;    // get QI error

   // get initial values for state variables
   LPWSTR name;
   OPCHANDLE   localclient;
   hr = m_pOPCGroup->GetState( &m_rate, &m_active, &name, &m_timebias,
                               &m_deadband, &m_LCID, &localclient, &m_server );
   if( SUCCEEDED(hr) )
   {
      m_name = name;
      CoTaskMemFree( name );
   }
   else
   {
      m_rate = 0;
      m_active = TRUE;
   }
   _ASSERTE(m_client == (OPCHANDLE)this);

   // See if this server supports OPC 2.0 ConnectionPoints
   IConnectionPointContainer *pCPC = 0;
   HRESULT hrCP = m_pOPCGroup->QueryInterface(IID_IConnectionPointContainer, (void**)&pCPC);
   if( SUCCEEDED(hrCP) )
   {
      m_usingCP = TRUE;  // This server supports 2.0
      pCPC->Release();
   }

   return hr;
}

/////////////////////////////////////////////////////////////////////
// OPCGroup::Parent property
STDMETHODIMP COPCGroupImpl::get_Parent(IOPCAutoServer **ppParent)
{
   if( ppParent == NULL )
      return E_INVALIDARG;
   *ppParent = NULL;

   // get Interface pointer
   ((COPCAutoServerObject*)m_pParent)->QueryInterface(IID_IOPCAutoServer, (void**)ppParent);
   _ASSERTE(*ppParent);

   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCGroup::Name property
STDMETHODIMP COPCGroupImpl::get_Name(BSTR *pName)
{
   if(pName == NULL)
      return E_INVALIDARG;

   if( !m_pOPCGroup )
      return E_FAIL;

   LPWSTR name;
   OPCHANDLE   localclient;
   HRESULT hr = m_pOPCGroup->GetState( &m_rate, &m_active, &name, &m_timebias,
                                       &m_deadband, &m_LCID, &localclient, &m_server );
   if( SUCCEEDED(hr) )
   {
      // need to duplicate the name into a BSTR!!
      m_name = name;
      *pName = m_name.Copy();
      CoTaskMemFree( name );
   }

   return hr;
}

STDMETHODIMP COPCGroupImpl::put_Name(BSTR Name)
{
   if( !m_pOPCGroup )
      return E_FAIL;

   HRESULT hr = m_pOPCGroup->SetName( (LPWSTR)Name );
   if( SUCCEEDED(hr) )
      m_name = Name;

   return hr;
}

/////////////////////////////////////////////////////////////////////
// OPCGroup::IsPublic property
STDMETHODIMP COPCGroupImpl::get_IsPublic(VARIANT_BOOL *pIsPublic)
{
   if(pIsPublic == NULL)
      return E_INVALIDARG;

//      *pIsPublic = (xx ? VARIANT_TRUE : VARIANT_FALSE);
   return E_NOTIMPL;
}

/////////////////////////////////////////////////////////////////////
// OPCGroup::IsActive property
STDMETHODIMP COPCGroupImpl::get_IsActive(VARIANT_BOOL *pIsActive)
{
   if(pIsActive == NULL)
      return E_INVALIDARG;

   if( !m_pOPCGroup )
      return E_FAIL;

   LPWSTR name;
   OPCHANDLE   localclient;
   HRESULT hr = m_pOPCGroup->GetState( &m_rate, &m_active, &name, &m_timebias,
                                       &m_deadband, &m_LCID, &localclient, &m_server );
   if( SUCCEEDED(hr) )
   {
      *pIsActive = (m_active ? VARIANT_TRUE : VARIANT_FALSE);
      CoTaskMemFree( name );
   }
   return hr;
}

STDMETHODIMP COPCGroupImpl::put_IsActive(VARIANT_BOOL IsActive)
{
   if( !m_pOPCGroup )
      return E_FAIL;

   m_active = IsActive;
   HRESULT hr = m_pOPCGroup->SetState( &m_rate, &m_rate, &m_active, NULL,
                                       NULL, NULL, NULL );
   return hr;
}

/////////////////////////////////////////////////////////////////////
// OPCGroup::IsSubscribed property
STDMETHODIMP COPCGroupImpl::get_IsSubscribed(VARIANT_BOOL *pIsSubscribed)
{
   if(pIsSubscribed == NULL)
      return E_INVALIDARG;

   if( !m_pOPCGroup )
      return E_FAIL;

   *pIsSubscribed = m_subscribed;
   return S_OK;
}

STDMETHODIMP COPCGroupImpl::put_IsSubscribed(VARIANT_BOOL IsSubscribed)
{
   if( !m_pOPCGroup )
      return E_FAIL;

   HRESULT hr=S_OK;
   if( IsSubscribed == m_subscribed )
      return hr;

   if( IsSubscribed )
   {
      if( m_usingCP )
      {
         IConnectionPointContainer *pCPC = 0;
         hr = m_pOPCGroup->QueryInterface(IID_IConnectionPointContainer, (LPVOID*)&pCPC);
         if( SUCCEEDED(hr) )
         {
            IConnectionPoint *pCallbackCP = 0;
            hr = pCPC->FindConnectionPoint(IID_IOPCDataCallback, &pCallbackCP);
            pCPC->Release();
            if( FAILED(hr) )
            {
               return E_FAIL;
            }

            IUnknown* pUnk = NULL;
            ((COPCGroup*)this)->QueryInterface( IID_IUnknown, (LPVOID*)&pUnk);
            hr = pCallbackCP->Advise(pUnk, &m_DataConnection);
            pCallbackCP->Release();
            pUnk->Release();
            if( FAILED(hr) )
            {
               return E_FAIL;
            }
         }
      }
      else
      {
         if( !m_pIDataObject )
         {
            m_pIDataObject = m_pOPCGroup;   // get IDataObject Interface
            if( !m_pIDataObject )
               return E_FAIL;
         }

         IAdviseSink *pAdviseSink = NULL;

         hr = ((COPCGroup*)this)->QueryInterface(IID_IAdviseSink, (LPVOID *)&pAdviseSink);
         if (FAILED(hr))
         {
            return E_FAIL;
         }

         FORMATETC formatEtc ;
         formatEtc.cfFormat = OPCSTMFORMATDATATIME;
         formatEtc.tymed =  TYMED_HGLOBAL;
         formatEtc.ptd = NULL;
         formatEtc.dwAspect = DVASPECT_CONTENT;
         formatEtc.lindex = -1;

         hr = m_pIDataObject->DAdvise(&formatEtc,
                                    (DWORD)ADVF_PRIMEFIRST,
                                    pAdviseSink,
                                    &m_DataConnection);

         if( SUCCEEDED(hr) )  // also advise write complete
         {
            formatEtc.cfFormat = OPCSTMFORMATWRITECOMPLETE;
            hr = m_pIDataObject->DAdvise(&formatEtc,
                                       (DWORD)ADVF_PRIMEFIRST,
                                       pAdviseSink,
                                       &m_WriteConnection);
         }
         // DAdvise will hold its own reference
         pAdviseSink->Release();
      }
      m_subscribed = VARIANT_TRUE;
   }
   else  // Unsubscribing
   {
      if( m_usingCP )
      {
         // OPC 2.0 ConnectionPoints
         IConnectionPointContainer *pCPC = 0;
         hr = m_pOPCGroup->QueryInterface(IID_IConnectionPointContainer, (LPVOID*)&pCPC);
         if( SUCCEEDED(hr) )
         {
            IConnectionPoint *pCallbackCP = 0;
            hr = pCPC->FindConnectionPoint(IID_IOPCDataCallback, &pCallbackCP);
            if( SUCCEEDED(hr) )
            {
               hr = pCallbackCP->Unadvise(m_DataConnection);
               pCallbackCP->Release();
            }
            pCPC->Release();
         }
      }
      else
      {
         if( m_pIDataObject.p )   // if a valid interface pointer
         {
            hr = m_pIDataObject->DUnadvise(m_DataConnection);
            if( SUCCEEDED(hr) )  // also unadvise write complete
               hr = m_pIDataObject->DUnadvise(m_WriteConnection);
            m_pIDataObject.Release();
         }
      }
      m_subscribed = VARIANT_FALSE;
   }

   return hr;
}

/////////////////////////////////////////////////////////////////////
// OPCGroup::ClientHandle property
STDMETHODIMP COPCGroupImpl::get_ClientHandle(LONG *pClientHandle)
{
   if(pClientHandle == NULL)
      return E_INVALIDARG;

   *pClientHandle = m_client;
   return S_OK;
}

STDMETHODIMP COPCGroupImpl::put_ClientHandle(LONG ClientHandle)
{
   m_client = ClientHandle;
   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCGroup::ServerHandle property
STDMETHODIMP COPCGroupImpl::get_ServerHandle(LONG *pServerHandle)
{
   if(pServerHandle == NULL)
      return E_INVALIDARG;

   *pServerHandle = (LONG)this;
   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCGroup::LocaleID property
STDMETHODIMP COPCGroupImpl::get_LocaleID(LONG *pLocaleID)
{
   if(pLocaleID == NULL)
      return E_INVALIDARG;

   if( !m_pOPCGroup )
      return E_FAIL;

   LPWSTR name;
   OPCHANDLE   localclient;
   HRESULT hr = m_pOPCGroup->GetState( &m_rate, &m_active, &name, &m_timebias,
                                       &m_deadband, &m_LCID, &localclient, &m_server );
   if( SUCCEEDED(hr) )
   {
      *pLocaleID = m_LCID;
      CoTaskMemFree( name );
   }
   return hr;
}

STDMETHODIMP COPCGroupImpl::put_LocaleID(LONG LocaleID)
{
   if( !m_pOPCGroup )
      return E_FAIL;

   m_LCID = LocaleID;
   HRESULT hr = m_pOPCGroup->SetState( &m_rate, &m_rate, NULL, NULL,
                                       NULL, &m_LCID, NULL );
   return hr;
}

/////////////////////////////////////////////////////////////////////
// OPCGroup::TimeBias property
STDMETHODIMP COPCGroupImpl::get_TimeBias(LONG *pTimeBias)
{
   if(pTimeBias == NULL)
      return E_INVALIDARG;

   if( !m_pOPCGroup )
      return E_FAIL;

   LPWSTR name;
   OPCHANDLE   localclient;
   HRESULT hr = m_pOPCGroup->GetState( &m_rate, &m_active, &name, &m_timebias,
                                       &m_deadband, &m_LCID, &localclient, &m_server );
   if( SUCCEEDED(hr) )
   {
      *pTimeBias = m_timebias;
      CoTaskMemFree( name );
   }
   return hr;
}

STDMETHODIMP COPCGroupImpl::put_TimeBias(LONG TimeBias)
{
   if( !m_pOPCGroup )
      return E_FAIL;

   m_timebias = TimeBias;
   HRESULT hr = m_pOPCGroup->SetState( &m_rate, &m_rate, NULL, &m_timebias,
                                       NULL, NULL, NULL );
   return hr;
}

/////////////////////////////////////////////////////////////////////
// OPCGroup::DeadBand property
STDMETHODIMP COPCGroupImpl::get_DeadBand(FLOAT *pDeadBand)
{
   if(pDeadBand == NULL)
      return E_INVALIDARG;

   if( !m_pOPCGroup )
      return E_FAIL;

   LPWSTR name;
   OPCHANDLE   localclient;
   HRESULT hr = m_pOPCGroup->GetState( &m_rate, &m_active, &name, &m_timebias,
                                       &m_deadband, &m_LCID, &localclient, &m_server );
   if( SUCCEEDED(hr) )
   {
      *pDeadBand = m_deadband;
      CoTaskMemFree( name );
   }
   return hr;
}

STDMETHODIMP COPCGroupImpl::put_DeadBand(FLOAT DeadBand)
{
   if( !m_pOPCGroup )
      return E_FAIL;

   m_deadband = DeadBand;
   HRESULT hr = m_pOPCGroup->SetState( &m_rate, &m_rate, NULL, NULL,
                                       &m_deadband, NULL, NULL );
   return hr;
}

/////////////////////////////////////////////////////////////////////
// OPCGroup::UpdateRate property
STDMETHODIMP COPCGroupImpl::get_UpdateRate(LONG *pUpdateRate)
{
   if(pUpdateRate == NULL)
      return E_INVALIDARG;

   if( !m_pOPCGroup )
      return E_FAIL;

   LPWSTR name;
   OPCHANDLE   localclient;
   HRESULT hr = m_pOPCGroup->GetState( &m_rate, &m_active, &name, &m_timebias,
                                       &m_deadband, &m_LCID, &localclient, &m_server );
   if( SUCCEEDED(hr) )
   {
      *pUpdateRate = m_rate;
      CoTaskMemFree( name );
   }
   return hr;
}

STDMETHODIMP COPCGroupImpl::put_UpdateRate(LONG UpdateRate)
{
   if( !m_pOPCGroup )
      return E_FAIL;

   m_rate = UpdateRate;
   HRESULT hr = m_pOPCGroup->SetState( &m_rate, &m_rate, NULL, NULL,
                                       NULL, NULL, NULL );
   return hr;
}

/////////////////////////////////////////////////////////////////////
// OPCGroup::OPCItems property
STDMETHODIMP COPCGroupImpl::get_OPCItems(OPCItems **ppItems)
{
   if(ppItems == NULL)
      return E_INVALIDARG;

   // get Interface pointer
   ((COPCGroup*)this)->QueryInterface(IID_OPCItems, (void**)ppItems);
   _ASSERTE(*ppItems);

   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCGroup::SyncRead method
STDMETHODIMP COPCGroupImpl::SyncRead( SHORT Source,
                        LONG NumItems,
                        SAFEARRAY ** ppServerHandles,
                        SAFEARRAY ** ppValues,
                        SAFEARRAY ** ppErrors,
                        VARIANT *pQualities,    // \/ Optional \/
                        VARIANT *pTimeStamps)
{
   // Validate input types
   if(*ppServerHandles == NULL)
      return E_INVALIDARG;
   if(ppValues == NULL)
      return E_INVALIDARG;
   if(ppErrors == NULL)
      return E_INVALIDARG;

   if( !m_pSyncIO )
   {
      m_pSyncIO = m_pOPCGroup;   // get Sync interface
      if( !m_pSyncIO )
         return E_FAIL;
   }

   HRESULT hr = S_OK;
   LONG lBound=0;
   LONG uBound=0;
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
      COPCItem* pItem = (COPCItem*)localHandle;
//      if( !m_items.Lookup( (LPVOID)localHandle, pItem ) )
//         return E_INVALIDARG;
      pSHandles[index] = pItem->GetServerHandle();
      }

   // call the server
   OPCITEMSTATE* pItemState;
   HRESULT *pErrors=0;
   hr = m_pSyncIO->Read( (OPCDATASOURCE)Source,
                         (DWORD)NumItems,
                         pSHandles,
                         &pItemState,
                         &pErrors);
   if( FAILED( hr ) )
   {
      return hr;
   }

   // Return Values in a SAFEARRAY and update each COPCItem
   {
      SAFEARRAYBOUND bound;
      bound.lLbound = 1;
      bound.cElements = NumItems;
      SAFEARRAY *pArray = SafeArrayCreate(VT_VARIANT, 1, &bound);
      if(pArray == NULL)
         return E_FAIL;    // temp
      for( long index=0; index<NumItems; index++ )
      {
         LONG vbIndex = index+1;
         VariantToAutomation(&pItemState[index].vDataValue);
         SafeArrayPutElement(pArray, &vbIndex, (void *)&pItemState[index].vDataValue);
         if( SUCCEEDED(pErrors[index]) )
         {
            COPCItem* pItem = (COPCItem*)pItemState[index].hClient;
            pItem->Update( &pItemState[index] );
         }
         VariantClear( &pItemState[index].vDataValue );
      }
      *ppValues = pArray;
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
         SafeArrayPutElement(pArray, &vbIndex, (void *)&pErrors[index]);
      }
      *ppErrors = pArray;
   }

   // Return qualities in a SAFEARRAY
   if(pQualities != NULL && pQualities->scode != DISP_E_PARAMNOTFOUND )
   {
      VariantClear( pQualities );
      SAFEARRAYBOUND bound;
      bound.lLbound = 1;
      bound.cElements = NumItems;
      SAFEARRAY *pArray = SafeArrayCreate(VT_I2, 1, &bound);
      if(pArray == NULL)
         return E_FAIL;    // temp
      for( long index=0; index<NumItems; index++ )
      {
         LONG vbIndex = index+1;
         SafeArrayPutElement(pArray, &vbIndex, (void *)&pItemState[index].wQuality);
      }
      pQualities->vt = VT_I2 | VT_ARRAY;
      pQualities->parray = pArray;
   }

   // Return timestamps in a SAFEARRAY
   if(pTimeStamps != NULL && pTimeStamps->scode != DISP_E_PARAMNOTFOUND )
   {
      VariantClear( pTimeStamps );
      SAFEARRAYBOUND bound;
      bound.lLbound = 1;
      bound.cElements = NumItems;
      SAFEARRAY *pArray = SafeArrayCreate(VT_DATE, 1, &bound);
      if(pArray == NULL)
         return E_FAIL;    // temp
      for( long index=0; index<NumItems; index++ )
      {
         LONG vbIndex = index+1;
         DATE timeStamp = NULL; 
         FILETIME filetimeLocal;
         if( FileTimeToLocalFileTime( &pItemState[index].ftTimeStamp, &filetimeLocal) )
         {
            SYSTEMTIME systime;
            if( FileTimeToSystemTime(&filetimeLocal, &systime) )
            {
	            SystemTimeToVariantTime( &systime, &timeStamp);
	         }
         }
         SafeArrayPutElement(pArray, &vbIndex, (void *)&timeStamp);
      }
      pTimeStamps->vt = VT_DATE | VT_ARRAY;
      pTimeStamps->parray = pArray;
   }

   CoTaskMemFree( pItemState );
   CoTaskMemFree( pErrors );

   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCGroup::SyncWrite method
STDMETHODIMP COPCGroupImpl::SyncWrite(LONG NumItems,
                        SAFEARRAY ** ppServerHandles,
                        SAFEARRAY ** ppValues,
                        SAFEARRAY ** ppErrors)
{
   // Validate input types
   if(*ppServerHandles == NULL)
      return E_INVALIDARG;
   if(*ppValues == NULL)
      return E_INVALIDARG;

   if( !m_pSyncIO )
   {
      m_pSyncIO = m_pOPCGroup;   // get Sync interface
      if( !m_pSyncIO )
         return E_FAIL;
   }

   HRESULT hr = S_OK;
   LONG lBound=0;
   LONG uBound=0;
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

   // Values
   hr = SafeArrayGetLBound(*ppValues, 1, &lBound);
   if( FAILED(hr) )
      return hr;
   hr = SafeArrayGetUBound(*ppValues, 1, &uBound);
   if( FAILED(hr) )
      return hr;
   if( lBound > 1 )
      return E_INVALIDARG;
   if( uBound < NumItems )
      return E_INVALIDARG;

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
      COPCItem* pItem = (COPCItem*)localHandle;
//      if( !m_items.Lookup( (LPVOID)localHandle, pItem ) )
//         return E_INVALIDARG;
      pSHandles[index] = pItem->GetServerHandle();
      }

   VARIANT* pValues = (VARIANT*)_alloca(NumItems*sizeof(VARIANT));
   for( index=0; index<NumItems; index++ )
   {
      LONG vbIndex = index+1;
      VariantInit( &pValues[index] );
      hr = SafeArrayGetElement(*ppValues, &vbIndex, &pValues[index]);
   }

   // call the server
   HRESULT *pErr=0;
   hr = m_pSyncIO->Write((DWORD)NumItems,
                         pSHandles,
                         pValues,
                         &pErr);

   for( index=0; index<NumItems; index++ )
   {
      VariantClear( &pValues[index] );
   }

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

   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCGroup::AsyncRead method
STDMETHODIMP COPCGroupImpl::AsyncRead( LONG NumItems,
                        SAFEARRAY ** ppServerHandles,
                        SAFEARRAY ** ppErrors,
                        LONG TransactionID,
                        LONG * pCancelID)
{
   // Validate input types
   if(*ppServerHandles == NULL)
      return E_INVALIDARG;
   if(ppErrors == NULL)
      return E_INVALIDARG;
   if(pCancelID == NULL)
      return E_INVALIDARG;

   HRESULT hr = S_OK;
   LONG lBound=0;
   LONG uBound=0;
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
      COPCItem* pItem = (COPCItem*)localHandle;
//      if( !m_items.Lookup( (LPVOID)localHandle, pItem ) )
//         return E_INVALIDARG;
      pSHandles[index] = pItem->GetServerHandle();
      }

   // call the server
   HRESULT *pErr=0;
   if( m_usingCP )
   {
      if( !m_pAsyncIO2 )
      {
         m_pAsyncIO2 = m_pOPCGroup;   // get Async interface
         if( !m_pAsyncIO2 )
            return E_FAIL;
      }

      hr = m_pAsyncIO2->Read((DWORD)NumItems,
                            pSHandles,
                            TransactionID,
                            (DWORD*)pCancelID,
                            &pErr);
   }
   else
   {
      if( !m_pAsyncIO )
      {
         m_pAsyncIO = m_pOPCGroup;   // get Async interface
         if( !m_pAsyncIO )
            return E_FAIL;
      }
      m_asyncReading = TRUE;
      hr = m_pAsyncIO->Read(m_DataConnection,
                            OPC_DS_DEVICE,
                            (DWORD)NumItems,
                            pSHandles,
                            (DWORD*)pCancelID,
                            &pErr);
      m_asyncReading = FALSE;

      // Need to save the cancel ID to figure out if the callback
      // is a read or refresh
      if( SUCCEEDED( hr ) )
      {
         BOOL found = FALSE;
         list<OPCHANDLE>::iterator listPosition = m_readIDs.begin();
         while( listPosition!=m_readIDs.end() && !found )
         {
            if( *listPosition == *pCancelID )
            {
               m_readIDs.remove(*pCancelID);
               found = TRUE;
            }
            listPosition++;
         }
         if( !found )
         {
            m_readIDs.push_back(*pCancelID);
         }
      }
   }

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

   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCGroup::AsyncWrite method
STDMETHODIMP COPCGroupImpl::AsyncWrite(LONG NumItems,
                        SAFEARRAY ** ppServerHandles,
                        SAFEARRAY ** ppValues,
                        SAFEARRAY ** ppErrors,
                        LONG TransactionID,
                        LONG * pCancelID)
{
   // Validate input types
   if(*ppServerHandles == NULL)
      return E_INVALIDARG;
   if(ppErrors == NULL)
      return E_INVALIDARG;
   if(pCancelID == NULL)
      return E_INVALIDARG;

   HRESULT hr = S_OK;
   LONG lBound=0;
   LONG uBound=0;
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
      COPCItem* pItem = (COPCItem*)localHandle;
//      if( !m_items.Lookup( (LPVOID)localHandle, pItem ) )
//         return E_INVALIDARG;
      pSHandles[index] = pItem->GetServerHandle();
      }

   VARIANT* pValues = (VARIANT*)_alloca(NumItems*sizeof(VARIANT));
   for( index=0; index<NumItems; index++ )
   {
      LONG vbIndex = index+1;
      VariantInit( &pValues[index] );
      hr = SafeArrayGetElement(*ppValues, &vbIndex, &pValues[index]);
   }

   // call the server
   HRESULT *pErr=0;
   if( m_usingCP )
   {
      if( !m_pAsyncIO2 )
      {
         m_pAsyncIO2 = m_pOPCGroup;   // get Async interface
         if( !m_pAsyncIO2 )
            return E_FAIL;
      }

      hr = m_pAsyncIO2->Write((DWORD)NumItems,
                            pSHandles,
                            pValues,
                            TransactionID,
                            (DWORD*)pCancelID,
                            &pErr);
   }
   else
   {
      if( !m_pAsyncIO )
      {
         m_pAsyncIO = m_pOPCGroup;   // get Async interface
         if( !m_pAsyncIO )
            return E_FAIL;
      }

      hr = m_pAsyncIO->Write(m_WriteConnection,
                            (DWORD)NumItems,
                            pSHandles,
                            pValues,
                            (DWORD*)pCancelID,
                            &pErr);
   }

   for( index=0; index<NumItems; index++ )
   {
      VariantClear( &pValues[index] );
   }

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

   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCGroup::AsyncRefresh method
STDMETHODIMP COPCGroupImpl::AsyncRefresh(SHORT Source,
                        LONG TransactionID,
                        LONG * pCancelID)
{
   HRESULT hr = S_OK;
   if( m_usingCP )
   {
      if( !m_pAsyncIO2 )
      {
         m_pAsyncIO2 = m_pOPCGroup;   // get Async interface
         if( !m_pAsyncIO2 )
            return E_FAIL;
      }

      hr = m_pAsyncIO2->Refresh2( (OPCDATASOURCE)Source,
                                 TransactionID,
                                 (DWORD*)pCancelID );
   }
   else
   {
      if( !m_pAsyncIO )
      {
         m_pAsyncIO = m_pOPCGroup;   // get Async interface
         if( !m_pAsyncIO )
            return E_FAIL;
      }

      hr = m_pAsyncIO->Refresh( m_DataConnection,
                               (OPCDATASOURCE)Source,
                               (DWORD*)pCancelID );
   }
   return hr;
}

/////////////////////////////////////////////////////////////////////
// OPCGroup::AsyncCancel method
STDMETHODIMP COPCGroupImpl::AsyncCancel(LONG CancelID)
{
   HRESULT hr = S_OK;
   if( m_usingCP )
   {
      if( !m_pAsyncIO2 )
      {
         m_pAsyncIO2 = m_pOPCGroup;   // get Async interface
         if( !m_pAsyncIO2 )
            return E_FAIL;
      }

      hr = m_pAsyncIO2->Cancel2( (DWORD)CancelID );
   }
   else
   {
      if( !m_pAsyncIO )
      {
         m_pAsyncIO = m_pOPCGroup;   // get Async interface
         if( !m_pAsyncIO )
            return E_FAIL;
      }

      hr = m_pAsyncIO->Cancel( (DWORD)CancelID );
   }
   return hr;
}


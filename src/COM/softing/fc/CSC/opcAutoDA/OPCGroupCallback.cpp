// OPCGroupCallback.cpp
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
//  These are the Group Object's data advise callback functions.
//
//
// Modification Log:
// Vers    Date   By    Notes
// ----  -------- ---   -----
// 1.00  06/10/98  JH    First Release
//

#include "stdafx.h"

extern UINT OPCSTMFORMATDATATIME;
extern UINT OPCSTMFORMATWRITECOMPLETE;
/////////////////////////////////////////////////////////////////////
// OPCDataCallback::OnDataChange
STDMETHODIMP COPCGroupImpl::OnDataChange(
                                    DWORD       Transid, 
                                    OPCHANDLE   grphandle, 
                                    HRESULT     masterquality,
                                    HRESULT     mastererror,
                                    DWORD       count, 
                                    OPCHANDLE * clienthandles, 
                                    VARIANT   * values, 
                                    WORD      * quality,
                                    FILETIME  * time,
                                    HRESULT   * errors)
{
   _ASSERTE(grphandle == (OPCHANDLE)this);
   // Return SAFEARRAYs of handles, values, qualities, and timestamps
   SAFEARRAYBOUND bound;
   bound.lLbound = 1;
   bound.cElements = count;
   SAFEARRAY *pHandles = SafeArrayCreate(VT_I4, 1, &bound);
   SAFEARRAY *pValues  = SafeArrayCreate(VT_VARIANT, 1, &bound);
   SAFEARRAY *pQualities = SafeArrayCreate(VT_I4, 1, &bound);
   SAFEARRAY *pTimestamps = SafeArrayCreate(VT_DATE, 1, &bound);
   SAFEARRAY *pErrors = SafeArrayCreate(VT_I4, 1, &bound);

   for( DWORD index=0; index<count; index++ )
   {
      LONG vbIndex = index+1;
      COPCItem* pItem = (COPCItem*)clienthandles[index];
      pItem->Update( &values[index], quality[index], time[index]);

      LONG clientHandle = pItem->GetClientHandle();
      SafeArrayPutElement(pHandles, &vbIndex, (void *)&clientHandle);
      SafeArrayPutElement(pValues,   &vbIndex, (void *)pItem->GetValue());
      LONG longquality = quality[index];
      SafeArrayPutElement(pQualities, &vbIndex, (void *)&longquality);
      DATE timeStamp = NULL; 
      FILETIME filetimeLocal;
      if( FileTimeToLocalFileTime( &time[index], &filetimeLocal) )
      {
         SYSTEMTIME systime;
         if( FileTimeToSystemTime(&filetimeLocal, &systime) )
         {
	         SystemTimeToVariantTime( &systime, &timeStamp);
	      }
      }
      SafeArrayPutElement(pTimestamps, &vbIndex, (void *)&timeStamp);
      LONG longerror = errors[index];
      SafeArrayPutElement(pErrors, &vbIndex, (void *)&longerror);
   }

   Fire_DataChange( Transid, count, pHandles, pValues, pQualities, pTimestamps);

   // Also the Groups collection event
   m_pParent->Fire_GlobalDataChange( Transid, m_client, count, pHandles, pValues, pQualities, pTimestamps);

   SafeArrayDestroy( pHandles );
   SafeArrayDestroy( pValues );
   SafeArrayDestroy( pQualities );
   SafeArrayDestroy( pTimestamps );
   SafeArrayDestroy( pErrors );

   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCDataCallback::OnReadComplete
STDMETHODIMP COPCGroupImpl::OnReadComplete(
                                    DWORD       Transid, 
                                    OPCHANDLE   grphandle, 
                                    HRESULT     masterquality,
                                    HRESULT     mastererror,
                                    DWORD       count, 
                                    OPCHANDLE * clienthandles, 
                                    VARIANT   * values, 
                                    WORD      * quality,
                                    FILETIME  * time,
                                    HRESULT   * errors)
{
   _ASSERTE(grphandle == (OPCHANDLE)this);
   // Return SAFEARRAYs of handles, values, qualities, and timestamps
   SAFEARRAYBOUND bound;
   bound.lLbound = 1;
   bound.cElements = count;
   SAFEARRAY *pHandles = SafeArrayCreate(VT_I4, 1, &bound);
   SAFEARRAY *pValues  = SafeArrayCreate(VT_VARIANT, 1, &bound);
   SAFEARRAY *pQualities = SafeArrayCreate(VT_I4, 1, &bound);
   SAFEARRAY *pTimestamps = SafeArrayCreate(VT_DATE, 1, &bound);
   SAFEARRAY *pErrors = SafeArrayCreate(VT_I4, 1, &bound);

   for( DWORD index=0; index<count; index++ )
   {
      LONG vbIndex = index+1;
      COPCItem* pItem = (COPCItem*)clienthandles[index];
      pItem->Update( &values[index], quality[index], time[index]);

      LONG clientHandle = pItem->GetClientHandle();
      SafeArrayPutElement(pHandles, &vbIndex, (void *)&clientHandle);
      SafeArrayPutElement(pValues,   &vbIndex, (void *)pItem->GetValue());
      LONG longquality = quality[index];
      SafeArrayPutElement(pQualities, &vbIndex, (void *)&longquality);
      DATE timeStamp = NULL; 
      FILETIME filetimeLocal;
      if( FileTimeToLocalFileTime( &time[index], &filetimeLocal) )
      {
         SYSTEMTIME systime;
         if( FileTimeToSystemTime(&filetimeLocal, &systime) )
         {
	         SystemTimeToVariantTime( &systime, &timeStamp);
	      }
      }
      SafeArrayPutElement(pTimestamps, &vbIndex, (void *)&timeStamp);
      SafeArrayPutElement(pErrors, &vbIndex, (void *)&errors[index]);
   }

   Fire_AsyncReadComplete( Transid,
                           count,
                           pHandles,
                           pValues,
                           pQualities,
                           pTimestamps,
                           pErrors);

   SafeArrayDestroy( pHandles );
   SafeArrayDestroy( pValues );
   SafeArrayDestroy( pQualities );
   SafeArrayDestroy( pTimestamps );

   return S_OK;
}


/////////////////////////////////////////////////////////////////////
// OPCDataCallback::OnWriteComplete
STDMETHODIMP COPCGroupImpl::OnWriteComplete(
                                    DWORD       Transid, 
                                    OPCHANDLE   grphandle, 
                                    HRESULT     mastererr, 
                                    DWORD       count, 
                                    OPCHANDLE * clienthandles, 
                                    HRESULT   * errors)
{
   _ASSERTE(grphandle == (OPCHANDLE)this);
   // Return SAFEARRAYs of handles and errors
   SAFEARRAYBOUND bound;
   bound.lLbound = 1;
   bound.cElements = count;
   SAFEARRAY *pHandles = SafeArrayCreate(VT_I4, 1, &bound);
   SAFEARRAY *pErrors = SafeArrayCreate(VT_I4, 1, &bound);

   for( DWORD index=0; index<count; index++ )
   {
      LONG vbIndex = index+1;
      COPCItem* pItem = (COPCItem*)clienthandles[index];

      LONG clientHandle = pItem->GetClientHandle();
      SafeArrayPutElement(pHandles, &vbIndex, (void *)&clientHandle);
      SafeArrayPutElement(pErrors, &vbIndex, (void *)&errors[index]);
   }

   Fire_AsyncWriteComplete(Transid,
                           count,
                           pHandles,
                           pErrors );

   SafeArrayDestroy( pHandles );
   SafeArrayDestroy( pErrors );

   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCDataCallback::OnCancelComplete
STDMETHODIMP COPCGroupImpl::OnCancelComplete(
                                    DWORD       transid, 
                                    OPCHANDLE   grphandle)
{
   _ASSERTE(grphandle == (OPCHANDLE)this);
   Fire_AsyncCancelComplete( transid );
   return S_OK;
}


/////////////////////////////////////////////////////////////////////
// IDataSink functions
/////////////////////////////////////////////////////////////////////
// Data returned from the OPC server
STDMETHODIMP_(void) COPCGroupImpl::OnDataChange(LPFORMATETC pFE, LPSTGMEDIUM pSTM)
{
   // Verify the format follows the OPC spec
   if( TYMED_HGLOBAL != pFE->tymed )
      return;
   if( pSTM->hGlobal == 0 )
      return;
   if( OPCSTMFORMATDATATIME != pFE->cfFormat )
   {
      OnWriteComplete(pFE, pSTM);
      return;
   }

   // It must be a data advise
   const BYTE* buffer = (BYTE*)GlobalLock( pSTM->hGlobal );
   if( !buffer )
      return;

   const OPCGROUPHEADER* pHeader = (OPCGROUPHEADER*)buffer;
   _ASSERTE( pHeader->hClientGroup == (OPCHANDLE)this );
   if( pHeader->dwTransactionID != 0 )
   {
      GlobalUnlock( pSTM->hGlobal );
      OnReadComplete(pFE, pSTM);
      return;
   }

   if( pHeader->dwItemCount == 0 )
   {
      GlobalUnlock( pSTM->hGlobal );
      return;
   }

   // Return SAFEARRAYs of handles, values, qualities, and timestamps
   SAFEARRAYBOUND bound;
   bound.lLbound = 1;
   bound.cElements = pHeader->dwItemCount;
   SAFEARRAY *pHandles = SafeArrayCreate(VT_I4, 1, &bound);
   SAFEARRAY *pValues  = SafeArrayCreate(VT_VARIANT, 1, &bound);
   SAFEARRAY *pQualities = SafeArrayCreate(VT_I4, 1, &bound);
   SAFEARRAY *pTimestamps = SafeArrayCreate(VT_DATE, 1, &bound);

   int offset = sizeof(OPCGROUPHEADER);
   // for each item in the data stream, get the value
   for( DWORD index=0; index<pHeader->dwItemCount; index++, offset += sizeof(OPCITEMHEADER1) )
   {
      LONG vbIndex = index+1;
      const OPCITEMHEADER1* pItemHeader = (OPCITEMHEADER1*)&buffer[offset];
      COPCItem* pItem = (COPCItem*)pItemHeader->hClient;

      VARIANT* pValue = (VARIANT*)&buffer[pItemHeader->dwValueOffset];
      // Strings and arrays are packed in the stream
      // requiring unpacking
      if( pValue->vt == VT_BSTR )
      {
         pValue->bstrVal = (BSTR)((BYTE*)pValue + sizeof(VARIANT) + sizeof(DWORD));
      }
      else if( (pValue->vt & VT_ARRAY) == VT_ARRAY )
      {
         pValue->parray = (SAFEARRAY*)((BYTE*)pValue + sizeof(VARIANT));
         pValue->parray->pvData = ((BYTE*)pValue->parray + sizeof(SAFEARRAY));
      }
      pItem->Update( pValue, pItemHeader->wQuality, pItemHeader->ftTimeStampItem);

      LONG clientHandle = pItem->GetClientHandle();
      SafeArrayPutElement(pHandles, &vbIndex, (void *)&clientHandle);
      SafeArrayPutElement(pValues,   &vbIndex, (void *)pItem->GetValue());
      LONG longquality = pItemHeader->wQuality;
      SafeArrayPutElement(pQualities, &vbIndex, (void *)&longquality);
      DATE timeStamp = NULL; 
      FILETIME filetimeLocal;
      if( FileTimeToLocalFileTime( &pItemHeader->ftTimeStampItem, &filetimeLocal) )
      {
         SYSTEMTIME systime;
         if( FileTimeToSystemTime(&filetimeLocal, &systime) )
         {
	         SystemTimeToVariantTime( &systime, &timeStamp);
	      }
      }
      SafeArrayPutElement(pTimestamps, &vbIndex, (void *)&timeStamp);
   }

   Fire_DataChange( pHeader->dwTransactionID,
                    pHeader->dwItemCount,
                    pHandles,
                    pValues,
                    pQualities,
                    pTimestamps);

   // Also the Groups collection event
   m_pParent->Fire_GlobalDataChange( pHeader->dwTransactionID, 
                                     m_client,
                                     pHeader->dwItemCount,
                                     pHandles,
                                     pValues,
                                     pQualities,
                                     pTimestamps);

   SafeArrayDestroy( pHandles );
   SafeArrayDestroy( pValues );
   SafeArrayDestroy( pQualities );
   SafeArrayDestroy( pTimestamps );

   GlobalUnlock( pSTM->hGlobal );
   return;
}

/////////////////////////////////////////////////////////////////////
// Data returned from the OPC server
STDMETHODIMP_(void) COPCGroupImpl::OnReadComplete(LPFORMATETC pFE, LPSTGMEDIUM pSTM)
{
   const BYTE* buffer = (BYTE*)GlobalLock( pSTM->hGlobal );
   if( !buffer )
      return;

   const OPCGROUPHEADER* pHeader = (OPCGROUPHEADER*)buffer;
   _ASSERTE( pHeader->hClientGroup == (OPCHANDLE)this );
   if( pHeader->dwItemCount == 0 )
   {
      GlobalUnlock( pSTM->hGlobal );
      return;
   }

   // Return SAFEARRAYs of handles, values, qualities, and timestamps
   SAFEARRAYBOUND bound;
   bound.lLbound = 1;
   bound.cElements = pHeader->dwItemCount;
   SAFEARRAY *pHandles = SafeArrayCreate(VT_I4, 1, &bound);
   SAFEARRAY *pValues  = SafeArrayCreate(VT_VARIANT, 1, &bound);
   SAFEARRAY *pQualities = SafeArrayCreate(VT_I4, 1, &bound);
   SAFEARRAY *pTimestamps = SafeArrayCreate(VT_DATE, 1, &bound);
   SAFEARRAY *pErrors = SafeArrayCreate(VT_I4, 1, &bound);

   int offset = sizeof(OPCGROUPHEADER);
   // for each item in the data stream, get the value
   for( DWORD index=0; index<pHeader->dwItemCount; index++, offset += sizeof(OPCITEMHEADER1) )
   {
      LONG vbIndex = index+1;
      const OPCITEMHEADER1* pItemHeader = (OPCITEMHEADER1*)&buffer[offset];
      COPCItem* pItem = (COPCItem*)pItemHeader->hClient;

      VARIANT* pValue = (VARIANT*)&buffer[pItemHeader->dwValueOffset];
      // Strings and arrays are packed in the stream
      // requiring unpacking
      if( pValue->vt == VT_BSTR )
      {
         pValue->bstrVal = (BSTR)((BYTE*)pValue + sizeof(VARIANT) + sizeof(DWORD));
      }
      else if( (pValue->vt & VT_ARRAY) == VT_ARRAY )
      {
         pValue->parray = (SAFEARRAY*)((BYTE*)pValue + sizeof(VARIANT));
         pValue->parray->pvData = ((BYTE*)pValue->parray + sizeof(SAFEARRAY));
      }
      pItem->Update( pValue, pItemHeader->wQuality, pItemHeader->ftTimeStampItem);

      LONG clientHandle = pItem->GetClientHandle();
      SafeArrayPutElement(pHandles, &vbIndex, (void *)&clientHandle);
      SafeArrayPutElement(pValues,   &vbIndex, (void *)pItem->GetValue());
      LONG longquality = pItemHeader->wQuality;
      SafeArrayPutElement(pQualities, &vbIndex, (void *)&longquality);
      SafeArrayPutElement(pErrors, &vbIndex, (void *)&pItemHeader->wQuality);
      DATE timeStamp = NULL; 
      FILETIME filetimeLocal;
      if( FileTimeToLocalFileTime( &pItemHeader->ftTimeStampItem, &filetimeLocal) )
      {
         SYSTEMTIME systime;
         if( FileTimeToSystemTime(&filetimeLocal, &systime) )
         {
	         SystemTimeToVariantTime( &systime, &timeStamp);
	      }
      }
      SafeArrayPutElement(pTimestamps, &vbIndex, (void *)&timeStamp);
   }

   BOOL itsARead = FALSE;
   if( m_asyncReading )    // must be reading
   {
      m_readIDs.push_back( pHeader->dwTransactionID );
      itsARead = TRUE;
   }
   else
   {
      list<OPCHANDLE>::iterator listPosition = m_readIDs.begin();
      while( listPosition!=m_readIDs.end() && !itsARead )
      {
         if( *listPosition == pHeader->dwTransactionID )
         {
            m_readIDs.remove(pHeader->dwTransactionID);
            itsARead = TRUE;
         }
      listPosition++;
      }
   }

   if( itsARead )
   {
      Fire_AsyncReadComplete( pHeader->dwTransactionID,
                              pHeader->dwItemCount,
                              pHandles,
                              pValues,
                              pQualities,
                              pTimestamps,
                              pErrors);
   }
   else
   {
      Fire_DataChange( pHeader->dwTransactionID,
                       pHeader->dwItemCount,
                       pHandles,
                       pValues,
                       pQualities,
                       pTimestamps);

      // Also the Groups collection event
      m_pParent->Fire_GlobalDataChange( pHeader->dwTransactionID, 
                                        m_client,
                                        pHeader->dwItemCount,
                                        pHandles,
                                        pValues,
                                        pQualities,
                                        pTimestamps);
   }

   SafeArrayDestroy( pHandles );
   SafeArrayDestroy( pValues );
   SafeArrayDestroy( pQualities );
   SafeArrayDestroy( pTimestamps );
   SafeArrayDestroy( pErrors );

   GlobalUnlock( pSTM->hGlobal );
   return;
}

/////////////////////////////////////////////////////////////////////
// Return Write complete status
STDMETHODIMP_(void) COPCGroupImpl::OnWriteComplete(LPFORMATETC pFE, LPSTGMEDIUM pSTM)
{
   if( OPCSTMFORMATWRITECOMPLETE != pFE->cfFormat )
   {
      return;
   }

   const BYTE* buffer = (BYTE*)GlobalLock( pSTM->hGlobal );
   if( !buffer )
      return;

   const OPCGROUPHEADERWRITE* pHeader = (OPCGROUPHEADERWRITE*)buffer;
   _ASSERTE( pHeader->hClientGroup == (OPCHANDLE)this );
   // pHeader->hrStatus

   // Return SAFEARRAYs of handles and errors
   SAFEARRAYBOUND bound;
   bound.lLbound = 1;
   bound.cElements = pHeader->dwItemCount;
   SAFEARRAY *pHandles = SafeArrayCreate(VT_I4, 1, &bound);
   SAFEARRAY *pErrors = SafeArrayCreate(VT_I4, 1, &bound);

   int offset = sizeof(OPCGROUPHEADERWRITE);
   // for each item in the data stream, get the value
   for( DWORD index=0; index<pHeader->dwItemCount; index++, offset += sizeof(OPCITEMHEADERWRITE) )
   {
      LONG vbIndex = index+1;
      OPCITEMHEADERWRITE* pItemHeader = (OPCITEMHEADERWRITE*)&buffer[offset];
      COPCItem* pItem = (COPCItem*)pItemHeader->hClient;

      LONG clientHandle = pItem->GetClientHandle();
      SafeArrayPutElement(pHandles, &vbIndex, (void *)&clientHandle);
      SafeArrayPutElement(pErrors,  &vbIndex, (void *)&pItemHeader->dwError);
   }

   Fire_AsyncWriteComplete(pHeader->dwTransactionID, pHeader->dwItemCount, pHandles, pErrors );

   SafeArrayDestroy( pHandles );
   SafeArrayDestroy( pErrors );

   GlobalUnlock( pSTM->hGlobal );
}

/////////////////////////////////////////////////////////////////////
// Required IAdviseSink interface
STDMETHODIMP_(void) COPCGroupImpl::OnViewChange(DWORD dwAspect, LONG lindex)
{
   return;
}

STDMETHODIMP_(void) COPCGroupImpl::OnRename(LPMONIKER pmk)
{
   return;
}

STDMETHODIMP_(void) COPCGroupImpl::OnSave(void)
{
   return;
}

STDMETHODIMP_(void) COPCGroupImpl::OnClose(void)
{
   return;
}

/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/iecVar.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: iecVar.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/iecVar.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlLibServ
 *
 * =CURRENT 	 $Date: 28.02.07 19:06 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "OpcError.h"
#include "Security.h"
#include "opcItem.h"
#include "iecResource.h"
#include "srtResource.h"
#include "iecVar.h"
#include "SysVar.h"
#include "SysVarPool.h"
#include "NSpaceHolder.h"
#include "connection.h"
#include "ControlMgt.h"

//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////
//------------- MEMBER FUNCTIONS OF CLASS IEC_Variable
/////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------*
/**
 * constructor
 *
 * @param           void
 * @return          void
 * @exception       -
 */
IEC_Variable::IEC_Variable()
{
    setUserData(iecVarType);
    setName(_T(""));
    setAccessRights(OPC_READABLE | OPC_WRITEABLE);

    // determine access rights and native data type
    m_bValid = FALSE;
    m_downloadCounter = -1;
    m_pParent = NULL;
}

//------------------------------------------------------------------*
/**
 * destructor
 *
 * @param           void
 * @return          void
 * @exception       -
*/
IEC_Variable::~IEC_Variable()
{
    if (m_pParent != NULL)
        m_pParent->release();
}






//------------------------------------------------------------------*
/**
 * read value from device
 *
 * @param           req     request
 * @return          void
 * @exception       -
 */
HRESULT
    IEC_Variable::updateReadCache(IN GenericCache *pCache, IN GenericItem *item)
{
    HRESULT hr;
    UINT readPos    = 0;
    UINT requestLen = 0;
    BYTE *request   = NULL;

    hr = BuildReadRequest(pCache, item, &request, &requestLen);
    if (FAILED(hr))
        return hr;

	hr = myConnection.TransferData(TRUE, CMD_GET_VALUE, &request, &requestLen);

	if (FAILED(hr)) {
		requestLen = 0;
	}

    hr = updateReadCache(pCache, item, request, requestLen, &readPos);
    
	HEAP_FREE(request,requestLen);

    return hr;
}

//------------------------------------------------------------------*
/**
 * build a read request
 *
 * @param           req     request
 * @return          void
 * @exception       -
 */
HRESULT IEC_Variable::BuildReadRequest(IN GenericCache *pCache, IN GenericItem *item, IN OUT BYTE **request, IN OUT UINT *requestLen)
{
    const UINT ALLOC_REQUEST_SIZE = 50;
    const UINT ALLOC_NEW_REQUEST_SIZE = 500;
    HRESULT hr;

    CheckForNamespaceUpdate();

    if (!m_bValid) {
        pCache->setQuality(OPC_QUALITY_BAD | OPC_QUALITY_NOT_CONNECTED);
        return OPC_E_NOTFOUND;
    }

    // check for user rights:
    hr = CheckAccessRights(OPC_READABLE, item);
    if (FAILED(hr)) {
        pCache->setQuality(OPC_QUALITY_LAST_KNOWN);
        return hr;
    }

	if (CConnection::GetFirmware() < 21000)
	{
		if (*requestLen % (ALLOC_REQUEST_SIZE * sizeof(XVariable)) == 0) {
			LPVOID p = NULL;
			UINT len = *requestLen + ALLOC_REQUEST_SIZE * sizeof(XVariable);
			if (*request == NULL)
				p = HeapAlloc(GetProcessHeap(), 0, len);
			else
				p = HeapReAlloc(GetProcessHeap(), 0, *request, len);

			if (p == NULL) {
				pCache->setQuality(OPC_QUALITY_LAST_KNOWN);
				return E_OUTOFMEMORY;
			}
			*request = (BYTE *)p;
		}

		XVariable *pVariable = (XVariable *)(*request + *requestLen);
		*requestLen += sizeof(XVariable);
		pVariable->uSegment = TRANSLATE_SHORT((IEC_UINT)m_varAddr.segment);
		pVariable->ulOffset = TRANSLATE_LONG(m_varAddr.offset);
		pVariable->byBit = 0;
		pVariable->byDummy = 0;
		pVariable->uLen = 1;
		switch(m_varAddr.iecType) {
		  case BL_BOOL:
			pVariable->byBit = 1 + m_varAddr.bitnr;
			break;
		  case BL_BYTE:
			pVariable->uLen = 1;
			break;
		  case BL_WORD:
          case BL_INT:
          case BL_UINT:
			pVariable->uLen = 2;
			break;
		  case BL_DWORD:
		  case BL_DINT:
		  case BL_UDINT:
		  case BL_TOD:
		  case BL_DATE:
		  case BL_REAL:
		  case BL_TIME:
			pVariable->uLen = 4;
			break;
		  case BL_LREAL:
		  case BL_LINT:
		  case BL_ULINT:
		  case BL_LWORD:
		  case BL_DT:
			pVariable->uLen = 8;
			break;
		  case BL_STRING:
			pVariable->uLen = 2 + m_varAddr.strlen;
			break;
		}
		pVariable->uLen = TRANSLATE_SHORT(pVariable->uLen);

		return S_OK;
	}

	/* ------------------------------------------------------------------------ */


	IEC_UDINT ulOffset	= m_varAddr.offset;
	IEC_UINT  uLen		= 0;
	IEC_UINT  uSegment	= (IEC_UINT)m_varAddr.segment;
	IEC_USINT usBit		= 0;

	switch(m_varAddr.iecType) 
	{
	  case BL_BOOL:
		usBit = 1 + m_varAddr.bitnr;
		break;
	  case BL_BYTE:
		uLen = 1;
		break;
	  case BL_WORD:
	  case BL_INT:
	  case BL_UINT:
		uLen = 2;
		break;
	  case BL_DWORD:
	  case BL_DINT:
	  case BL_UDINT:
	  case BL_TOD:
	  case BL_DATE:
	  case BL_REAL:
	  case BL_TIME:
		uLen = 4;
		break;
	  case BL_LREAL:
	  case BL_LINT:
	  case BL_ULINT:
	  case BL_LWORD:
	  case BL_DT:
		uLen = 8;
		break;
	  case BL_STRING:
		uLen = 2 + m_varAddr.strlen;
		break;
	  default:
		uLen  = 0;
		usBit = 0;
		break;
	}

	IEC_USINT usVType = VMM_XV_LARGE;
	IEC_UINT  uVSize = sizeof(XVariableL);

	if (ulOffset <= 0xffu && uLen <= 0xffu && uSegment <= 0xffu)
	{
		usVType = VMM_XV_SMALL;
		uVSize  = sizeof(XVariableS);
	}
	else if (ulOffset <= 0xffffu && uSegment <= 0xffu)
	{
		usVType = VMM_XV_MEDIUM;
		uVSize  = sizeof(XVariableM);
	}
	else
	{
		usVType = VMM_XV_LARGE;
		uVSize  = sizeof(XVariableL);
	}

	if (*request == NULL || (*requestLen + uVSize) >= ((*requestLen / ALLOC_NEW_REQUEST_SIZE) + 1) * ALLOC_NEW_REQUEST_SIZE)
	{
		LPVOID p = NULL;
		UINT len = *requestLen + ALLOC_NEW_REQUEST_SIZE + sizeof(XVariableL);
		if (*request == NULL)
			p = HeapAlloc(GetProcessHeap(), 0, len);
		else
			p = HeapReAlloc(GetProcessHeap(), 0, *request, len);

		if (p == NULL) {
			pCache->setQuality(OPC_QUALITY_LAST_KNOWN);
			return E_OUTOFMEMORY;
		}
		*request = (BYTE *)p;
	}

	switch(usVType)
	{
		case VMM_XV_SMALL:
		{
			XVariableS *pxVar = (XVariableS *)(*request + *requestLen);

			pxVar->usType = (IEC_USINT)(VMM_XV_SMALL | (usBit & VMM_XV_BITMASK));

			pxVar->usOffset = (IEC_USINT)ulOffset;
			pxVar->usLen	= (IEC_USINT)uLen;
			pxVar->usSegment= (IEC_USINT)uSegment;

			*requestLen += sizeof(XVariableS);

			break;
		}

		case VMM_XV_MEDIUM:
		{
			XVariableM *pxVar = (XVariableM *)(*request + *requestLen);

			pxVar->usType = (IEC_USINT)(VMM_XV_MEDIUM | (usBit & VMM_XV_BITMASK));

			pxVar->uOffset	= TRANSLATE_SHORT((IEC_UINT)ulOffset);
			pxVar->uLen		= TRANSLATE_SHORT((IEC_UINT)uLen);
			pxVar->usSegment= (IEC_USINT)uSegment;

			*requestLen += sizeof(XVariableM);

			break;
		}

		case VMM_XV_LARGE:
		{
			XVariableL *pxVar = (XVariableL *)(*request + *requestLen);

			pxVar->usType = (IEC_USINT)(VMM_XV_LARGE | (usBit & VMM_XV_BITMASK));

			pxVar->ulOffset = TRANSLATE_LONG(ulOffset);
			pxVar->uLen		= TRANSLATE_SHORT(uLen);
			pxVar->uSegment = TRANSLATE_SHORT(uSegment);

			*requestLen += sizeof(XVariableL);

			break;
		}

		default:
			return E_FAIL;
	}

	return S_OK;
}

//------------------------------------------------------------------*
/**
 * read value from device
 *
 * @param           req     request
 * @return          void
 * @exception       -
 */
HRESULT
    IEC_Variable::updateReadCache(IN GenericCache *pCache, IN GenericItem *item, IN BYTE *request, IN UINT requestLen, IN OUT UINT *readPos)
{
    HRESULT hr = S_OK;


    UINT uLen = 1;
    switch(m_varAddr.iecType) {
      case BL_WORD:
      case BL_INT:
      case BL_UINT:
        uLen = 2;
        break;
      case BL_DWORD:
      case BL_DINT:
      case BL_UDINT:
      case BL_TOD:
      case BL_DATE:
      case BL_REAL:
      case BL_TIME:
        uLen = 4;
        break;
      case BL_LREAL:
      case BL_LINT:
      case BL_ULINT:
      case BL_LWORD:
      case BL_DT:
        uLen = 8;
        break;
      case BL_STRING:
        uLen = 2 + m_varAddr.strlen;
        break;
    }

    CComVariant newVal;
    BYTE *pData = request + *readPos;

    if (requestLen < *readPos + uLen) {
        hr = OPC_E_UNKNOWNITEMID;
        WORD wQual = pCache->getQuality();
        FILETIME timestamp;
        ::CoFileTimeNow(&timestamp);
        if ((wQual & OPC_QUALITY_MASK) != OPC_QUALITY_BAD) {
            // flag time of disconnection
            pCache->setTimeStamp(&timestamp);
        }
        pCache->setQuality(OPC_QUALITY_BAD | OPC_QUALITY_OUT_OF_SERVICE);
    }
    else {
        // it's not a QVT, add quality here
        switch(m_varAddr.iecType) {
          case BL_BOOL:
            newVal.vt = VT_BOOL;
            newVal.boolVal = *pData ? VARIANT_TRUE : VARIANT_FALSE;
            break;
          case BL_BYTE:
            newVal.vt = VT_I4;
            newVal.lVal = *(BYTE *)pData;
            break;
          case BL_WORD:
          case BL_UINT:
            newVal.vt = VT_I4;
            newVal.lVal = (WORD)TRANSLATE_SHORT(*(WORD *)pData);
            break;
          case BL_INT:
            newVal.vt = VT_I4;
            newVal.lVal = TRANSLATE_SHORT(*(short *)pData);
            break;
          case BL_DINT:
          case BL_UDINT:
          case BL_DWORD:
          case BL_TOD:
          case BL_DATE:
            newVal.vt = VT_I4;
            newVal.lVal = TRANSLATE_LONG(*(LONG *)pData);
            break;
          case BL_REAL:
            newVal.vt = VT_R4;
            TRANSLATE_FLOAT((FLOAT *)pData, &newVal.fltVal);
            break;
          case BL_LREAL:
            newVal.vt = VT_R8;
            TRANSLATE_DOUBLE((DOUBLE *)pData, &newVal.dblVal);
            break;
          case BL_LINT:
          case BL_ULINT:
          case BL_LWORD:
          case BL_DT:
            newVal.vt = VT_DECIMAL;
            newVal.decVal.sign = 0;
            newVal.decVal.scale = 0;
            newVal.decVal.Lo64 = TRANSLATE_I64(*(__int64 *)pData);
            newVal.decVal.Hi32 = 0;
            if ((long)newVal.decVal.Mid32<0)
              {
                newVal.decVal.sign = DECIMAL_NEG;
                newVal.decVal.Lo64 = -(__int64)newVal.decVal.Lo64;
              }
            break;
          case BL_TIME:
            {   long val = TRANSLATE_LONG(*(LONG *)pData);
                newVal.vt = VT_DECIMAL;
                newVal.decVal.sign = 0;
                newVal.decVal.scale = 0;
                newVal.decVal.Hi32 = 0;
                newVal.decVal.Mid32 = 0;
                newVal.decVal.Lo32 = val;

                if(val < 0) {
                   newVal.decVal.Lo32 = -val;
                   newVal.decVal.sign = DECIMAL_NEG;
                }

            }
            break;
          case BL_STRING:
            newVal.vt = VT_BSTR;
            int len = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCSTR)pData+2, min(pData[0],m_varAddr.strlen), NULL, 0);
            newVal.bstrVal = SysAllocStringLen(NULL, len);
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCSTR)pData+2, m_varAddr.strlen, newVal.bstrVal, len);
            break;
        }

        pCache->setQuality(OPC_QUALITY_GOOD);
        pCache->setValue(&newVal);
    }

    *readPos += uLen;

    return hr;
}

//------------------------------------------------------------------*
/**
 * write value to device
 *
 * @param           req     request
 * @return          void
 * @exception       -
 */
HRESULT
    IEC_Variable::writeToVar(IN VARIANT vtNew, IN GenericItem *item)
{
    HRESULT hr;
    UINT requestLen = 0;
    BYTE *request   = NULL;

    hr = BuildWriteRequest(vtNew, item, &request, &requestLen);
    if (FAILED(hr))
        return hr;

	hr = myConnection.TransferData(TRUE, CMD_SET_VALUE, &request, &requestLen);
	HEAP_FREE(request, requestLen);

    return hr;
}

//------------------------------------------------------------------*
/**
 * build a write request
 *
 * @param           req     request
 * @return          void
 * @exception       -
 */
HRESULT IEC_Variable::BuildWriteRequest(IN VARIANT vtNew, IN GenericItem *item, IN OUT BYTE **request, IN OUT UINT *requestLen)
{
    const UINT ALLOC_REQUEST_SIZE = 1024;
    HRESULT hr;

    CheckForNamespaceUpdate();

    if (!m_bValid)
        return OPC_E_NOTFOUND;

    // check for user rights:
    hr = CheckAccessRights(OPC_WRITEABLE, item);
    if (FAILED(hr)) {
        return hr;
    }

    CComVariant newVal = vtNew;
    hr = newVal.ChangeType(getNativeDatatype());
    if (FAILED(hr))
    {
        switch (hr) {
        case DISP_E_OVERFLOW:
            hr = OPC_E_RANGE;
            break;
        case DISP_E_BADVARTYPE:
        case DISP_E_TYPEMISMATCH:
            hr = OPC_E_BADTYPE;
            break;
        case E_OUTOFMEMORY:
            hr = E_OUTOFMEMORY;
            break;
        default:
            break;
        }
        return hr;
    }

    UINT cbData = sizeof(XVariable);
    switch(m_varAddr.iecType) {
      case BL_BOOL:
      case BL_BYTE:
        cbData += 1;
        break;
      case BL_WORD:
      case BL_INT:
      case BL_UINT:
        cbData += 2;
        break;
      case BL_DWORD:
      case BL_UDINT:
      case BL_DINT:
      case BL_TOD:
      case BL_DATE:
      case BL_REAL:
      case BL_TIME:
        cbData += 4;
        break;
      case BL_LREAL:
      case BL_LINT:
      case BL_ULINT:
      case BL_LWORD:
      case BL_DT:
        cbData += 8;
        break;
      case BL_STRING:
        cbData += 2 + m_varAddr.strlen;
        break;
    }
    if (*requestLen + cbData >  (*requestLen + ALLOC_REQUEST_SIZE - 1) / ALLOC_REQUEST_SIZE * ALLOC_REQUEST_SIZE) {
        LPVOID p = NULL;
        UINT len = (*requestLen + cbData + ALLOC_REQUEST_SIZE - 1) / ALLOC_REQUEST_SIZE * ALLOC_REQUEST_SIZE;
        if (*request == NULL)
            p = HeapAlloc(GetProcessHeap(), 0, len);
        else
            p = HeapReAlloc(GetProcessHeap(), 0, *request, len);
        if (p == NULL) {
            return E_OUTOFMEMORY;
        }
        *request = (BYTE *)p;
    }
    XVariable *pVariable = (XVariable*)(*request + *requestLen);
    IEC_DATA *pValue = (IEC_DATA*)(pVariable+1);
    *requestLen += cbData;
    pVariable->uSegment = TRANSLATE_SHORT((IEC_UINT)m_varAddr.segment);
    pVariable->ulOffset = TRANSLATE_LONG(m_varAddr.offset);
    pVariable->byBit = 0;
    pVariable->byDummy = 0;
    pVariable->uLen = 1;
    switch(m_varAddr.iecType) {
      case BL_BOOL:
        pVariable->byBit = 1 + m_varAddr.bitnr;
        *pValue = vtNew.boolVal != VARIANT_FALSE;
        break;
      case BL_BYTE:
        pVariable->uLen = 1;
        *pValue = vtNew.bVal;
        break;
      case BL_WORD:
      case BL_INT:
      case BL_UINT:
        pVariable->uLen = 2;
        *(short *)pValue = TRANSLATE_SHORT(vtNew.iVal);
        break;
      case BL_DWORD:
      case BL_DINT:
      case BL_UDINT:
      case BL_DATE:
      case BL_TOD:
        pVariable->uLen = 4;
        *(long *)pValue = TRANSLATE_LONG(vtNew.lVal);
        break;
      case BL_REAL:
        pVariable->uLen = 4;
        TRANSLATE_FLOAT(&vtNew.fltVal, (FLOAT *)pValue);
        break;
      case BL_LREAL:
        pVariable->uLen = 8;
        TRANSLATE_DOUBLE(&vtNew.dblVal, (DOUBLE *)pValue);
        break;
      case BL_LINT:
      case BL_ULINT:
      case BL_LWORD:
      case BL_DT:
        pVariable->uLen = 8;
        if (vtNew.decVal.sign & DECIMAL_NEG)
          *(__int64 *)pValue = TRANSLATE_I64(-(__int64)vtNew.decVal.Lo64);
        else
          *(__int64 *)pValue = TRANSLATE_I64(vtNew.decVal.Lo64);
        break;
      case BL_TIME:
        {   long val;
            pVariable->uLen = 4;
            val = vtNew.decVal.Lo32;
            if (vtNew.decVal.sign & DECIMAL_NEG)
                val = -val;
            *(long *)pValue = TRANSLATE_LONG(val);
        }
        break;
      case BL_STRING:
        pVariable->uLen = 2 + m_varAddr.strlen;
        pValue[1] = m_varAddr.strlen;
        pValue[0] = WideCharToMultiByte(CP_ACP, 0, vtNew.bstrVal, -1, (LPSTR)&pValue[2], m_varAddr.strlen, NULL, NULL);
        if (pValue[0] != 0)
            pValue[0]--;
        else
            pValue[0] = m_varAddr.strlen;
        break;
    }

    pVariable->uLen = TRANSLATE_SHORT(pVariable->uLen);
    return S_OK;
}

//------------------------------------------------------------------*
/**
 * attach to control's object interface
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT
    IEC_Variable::Attach(CSC_VARADDR* pVarAddr, long downloadCounter)
{
    m_downloadCounter = downloadCounter;
    m_bValid = pVarAddr != NULL;
    if (m_bValid)
    {
      m_varAddr = *pVarAddr;
      VARTYPE vt = VT_EMPTY;
      switch(m_varAddr.iecType) {
          case BL_BOOL:
              vt = VT_BOOL;
              break;
          case BL_BYTE:
          case BL_WORD:
          case BL_INT:
          case BL_DWORD:
          case BL_UINT:
          case BL_UDINT:
          case BL_DINT:
          case BL_DATE:
          case BL_TOD:
              vt = VT_I4;
              break;
          case BL_LWORD:
          case BL_LINT:
          case BL_ULINT:
          case BL_TIME:
          case BL_DT:
              vt = VT_DECIMAL;
              break;
          case BL_REAL:
              vt = VT_R4;
              break;
          case BL_LREAL:
              vt = VT_R8;
              break;
          case BL_STRING:
              vt = VT_BSTR;
              break;
      }

      setNativeDatatype(vt);
    }
    return S_OK;
}


//------------------------------------------------------------------*
HRESULT 
    IEC_Variable::SetParent(SRT_Resource *pParent)
{
    if (m_pParent != NULL)
        m_pParent->release();
    m_pParent = pParent;
    if (m_pParent != NULL)
        m_pParent->addRef();
    return S_OK;
}


//------------------------------------------------------------------*
HRESULT
    IEC_Variable::CheckForNamespaceUpdate()
{
    if (m_pParent == NULL)
        return E_FAIL;

    if (m_downloadCounter != m_pParent->GetCachedDownloadCounter())
        m_pParent->RefreshVariable(this);

    return S_OK;
}


//------------------------------------------------------------------*
HRESULT
    IEC_Variable::VariantToFileTime(DATE dt, FILETIME *pFileTime)
{
    if (pFileTime == NULL) {
        return E_POINTER;
    }

    unsigned __int64 ms_since_1900 = (unsigned __int64)((dt - 2.0) * (24*3600*1000.0) + 0.5);

    *(unsigned __int64 *)pFileTime = ms_since_1900 * 10000
                                   + 24*3600*10000000ui64 * (299*365+299/4-2);

    return S_OK;
}

/* ---------------------------------------------------------------------------- */

/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/AsyncOPC/Item.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: Item.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/AsyncOPC/Item.cpp $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CSC
 *
 * =COMPONENT           AsyncOPC
 *
 * =CURRENT      $Date: 28.02.07 19:00 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  07.08.01  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/



//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "opcDA.h"
#include "Item.h"
#include "OPCConnection.h"
#include "SyncAccessGlobals.h"

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CItem
CItem::CItem()
{
    m_Quality = OPC_QUALITY_BAD;
    CoFileTimeNow(&m_Time);
    m_hServer = 0;
}

//------------------------------------------------------------------*
CItem::~CItem()
{
}

//------------------------------------------------------------------*
HRESULT CItem::Init(LPCTSTR strName)
{
    CString strPureMachineName;
    HRESULT hr = SplitMachineName(strName, strPureMachineName, m_pureName);
    if (FAILED(hr))
        return hr;
    hr = COPCConnection::LookupConnection(strPureMachineName, this, &m_pConnection);
    if (FAILED(hr))
        return hr;
    _ASSERTE(m_pConnection != NULL);
    return S_OK;
}


//------------------------------------------------------------------*
HRESULT CItem::Cleanup()
{
    COPCConnection::ReleaseConnection(m_pConnection, this);
    m_pConnection->Release();
    m_pConnection = NULL;
    return S_OK;
}

//------------------------------------------------------------------*
HRESULT CItem::GetQVT(VARIANT *pValue, WORD *pQuality, FILETIME *pTime)
{
    CSingleLock lock(&m_cs, TRUE);
    if (pValue == NULL)
        return E_POINTER;

    VariantInit(pValue);
    HRESULT hr = VariantCopy(pValue, &m_Value);

    if (pQuality != NULL)
        *pQuality = m_Quality;

    if (pTime != NULL)
        *pTime = m_Time;

    return hr;
}

//------------------------------------------------------------------*
HRESULT CItem::SetQVT(VARIANT vValue, WORD wQuality, FILETIME fTime)
{
    CSingleLock lock(&m_cs, TRUE);
    HRESULT hr = VariantCopy(&m_Value, &vValue);
    m_Quality = wQuality;
    m_Time = fTime;
    return hr;
}

//------------------------------------------------------------------*
HRESULT CItem::WriteValue(VARIANT vValue)
{
    return m_pConnection->SubmitWriteRequest(this, vValue);
}

//------------------------------------------------------------------*
HRESULT CItem::GetTagName(BSTR *pName)
{
    if (pName == NULL)
        return E_POINTER;
    *pName = m_pureName.AllocSysString();
    if (*pName == NULL)
        return E_OUTOFMEMORY;
    return S_OK;
}



/*
 *----------------------------------------------------------------------------*
 *  $History: Item.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSC/AsyncOPC
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSC/AsyncOPC
 * 
 * *****************  Version 1  *****************
 * User: Ca           Date: 8.08.01    Time: 9:41
 * Created in $/4Control/COM/softing/fc/CSC/AsyncOPC
 * copied from V1.3
 *==
 *----------------------------------------------------------------------------*
*/

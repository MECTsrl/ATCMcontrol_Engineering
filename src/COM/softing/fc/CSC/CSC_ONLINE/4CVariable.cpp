/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/4CVariable.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: 4CVariable.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/4CVariable.cpp $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CSC
 *
 * =COMPONENT           CSC Online Control
 *
 * =CURRENT      $Date: 28.02.07 19:00 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *





 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  15.03.2001  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/
#include "stdafx.h"
#include "baselib.h"
#include "CSC_Online.h"
#include "opcDA.h"

#include "CSC_OnlineErr.h"
#include "4CVariable.h"
#include "4CVariables.h"
#include "VarSession.h"


//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CFCVariable

//------------------------------------------------------------------*
CFCVariable::CFCVariable()
{
   FILETIME  now;
   CoFileTimeNow(&now);
   FileTimeToVariantTime(&now, &m_TimeStamp);
   m_wQuality = OPC_QUALITY_BAD;
   m_pVarColl = NULL;
   m_pSession = NULL;
   m_hServer = 0;
   m_hClient = 0;
}

//------------------------------------------------------------------*
CFCVariable::~CFCVariable()
{
   if (m_pVarColl != NULL)
      m_pVarColl->Release();

   if (m_pSession != NULL)
      m_pSession->Release();
}


//------------------------------------------------------------------*
void CFCVariable::Init(LPCTSTR name, CFCVariableCollection *pVarColl)
{
    ASSERT(name != NULL);
    ASSERT(pVarColl != NULL);
    ASSERT(m_pVarColl == NULL);
    m_name = name;
    pVarColl->AddRef();
    m_pVarColl = pVarColl;
}

//------------------------------------------------------------------*
void CFCVariable::Init2(OPCHANDLE hClient, CFCVarSession *pSession)
{
    ASSERT(pSession != NULL);
    ASSERT(m_pSession == NULL);
    m_hClient = hClient;
    pSession->AddRef();
    m_pSession = pSession;
}

//------------------------------------------------------------------*
HRESULT CFCVariable::getContent(VARIANT *pVal, short *pQuality, DATE *pTimeStamp)
{
    ASSERT(pVal != NULL);

    VariantInit(pVal);
    HRESULT hr = VariantCopy(pVal, &m_Value);

    if (pQuality != NULL)
       *pQuality = m_wQuality;
    if (pTimeStamp != NULL)
       *pTimeStamp = m_TimeStamp;

    return hr;
}

//------------------------------------------------------------------*
HRESULT CFCVariable::setContent(VARIANT value, short quality, DATE timestamp)
{
    HRESULT hr = VariantCopy(&m_Value, &value);

    m_wQuality = quality;
    m_TimeStamp = timestamp;

    return hr;
}


/**
 * convert FILETIME to DATE more accurate than
 * SystemTimeToVariantTime() would do it.
 * @param IN  pFileTime
 * @param OUT pDT
 */
HRESULT FileTimeToVariantTime(const FILETIME *pFileTime, DATE *pDT)
{
    if (pFileTime == NULL || pDT == NULL) {
        return E_POINTER;
    }

    __int64 ms_since_1900 = (*(__int64 *)pFileTime - 24*3600*10000000i64*(299*365+299/4-2)) / 10000;
    *pDT = 2.0 + ms_since_1900/(24*3600*1000.0);

    return S_OK;
}


/*
 *----------------------------------------------------------------------------*
 *  $History: 4CVariable.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 2  *****************
 * User: Su           Date: 15.03.01   Time: 19:34
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * New header, removed private trace files, replaced by tracea.lib
 * 
 * *****************  Version 1  *****************
 * User: Su           Date: 14.03.01   Time: 17:21
 * Created in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * Initial rev.
 * 
 * *****************  Version 26  *****************
 * User: Ed           Date: 11.02.00   Time: 16:16
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Variablen-Connection mit eigenem Thread
 * 
 *==
 *----------------------------------------------------------------------------*
*/


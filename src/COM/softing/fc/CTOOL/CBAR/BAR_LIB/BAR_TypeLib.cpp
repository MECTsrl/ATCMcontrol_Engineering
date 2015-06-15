/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_TypeLib.cpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_TypeLib.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_TypeLib.cpp $
 *
 * =PROJECT             CAK1020  ATCMControl V2.0
 *
 * =SWKE                CBar
 *
 * =COMPONENT           bar
 *
 * =CURRENT      $Date: 28.02.07 19:01 $
 *           $Revision: 1 $
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
 *  02.07.01  KT      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/

//#include <>
#include <assert.h>

#include <atlbase.h>

#include "BAR_TypeLib.hpp"
#include "BAR_Callback.hpp"
#include "BAR_Trace.hpp"
#include "BAR_Error.hpp"
#include "BAR_Warning.hpp"
#include "BAR_Util.hpp"

//====  Local Defines:   ======================================================*

//====  Static Initializations:   =============================================*


//=============================================================================*
// MEMBER FUNCTIONS OF CLASS BAR_TypeLib
//=============================================================================*

//=============================================================================*
/**
 * <B>constructor</B>
 * @param           
 * @param           
 * @param           
 * @exception       -
 */
//=============================================================================*
BAR_TypeLib::BAR_TypeLib(BAR_Callback& callback, const string& sLabel,
                         const IID& iidTlb,
                         const string& sTlbHost,
                         const string& sResourceIndex)
: m_callback(callback),
  m_sLabel(sLabel),
  m_iidTlb(iidTlb),
  m_pITlb(NULL),
  m_pTlbAttr(NULL)
{
    //=== search for type library host
    char* lpBuf = new char[1024];
    string sTlbPath;
    if ( 0 < ::SearchPath(NULL, sTlbHost.c_str(), NULL, 1024, lpBuf, NULL) )
    {
        sTlbPath = lpBuf;
    }
    else
    {
        Throw_RegisteringTlbFailed(E_FAIL, 
                                   wstring(L"could not find type library host")
                                   + L", name=" + ToWS(sTlbHost));
    }
    BAR_TRACE1(2, "found type library host, %s", sTlbPath.c_str());

    //=== load type library
    sTlbPath += "\\" + sResourceIndex;
    HRESULT hr = ::LoadTypeLib(CComBSTR(sTlbPath.c_str()).m_str, &m_pITlb);
    if ( FAILED(hr) )
    {
        Throw_RegisteringTlbFailed(hr, wstring(L"loading type library failed") 
                                       + L", path=" + ToWS(sTlbHost));
    }
    if ( m_pITlb == NULL )
    {
        Throw_FatalError(L"type library interface pointer is null");
    }

    //=== get tlb  attributes ... 
    hr = m_pITlb->GetLibAttr(&m_pTlbAttr);
    if ( FAILED(hr) )
    {
        Throw_RegisteringTlbFailed(hr, 
                                   wstring(L"could not access type library")
                                   + L" attributes, path=" 
                                   + ToWS(sTlbPath));
    }
    assert(m_pTlbAttr != NULL);
    if ( m_pTlbAttr == NULL )
    {
        Throw_FatalError(L"typelibrary attribute pointer is null");
    }

    //=== convert found and expected guid into strings
    CompareGuids(m_pTlbAttr->guid, m_iidTlb);

    //=== register type library
    const wchar_t* wsTlbPath = CComBSTR(sTlbPath.c_str()).m_str;
    hr = ::RegisterTypeLib(m_pITlb, (wchar_t*)wsTlbPath, NULL);
    if ( FAILED(hr) )
    {
        Throw_RegisteringTlbFailed(hr, wstring(L"path=") + ToWS(sTlbPath));
    }

    //== finished
    BAR_TRACE(2, "registering type library succeeded");
}

//=============================================================================*
/**
 * <B>destructor</B>
 * @param           
 * @param           
 * @param           
 * @exception       -
 */
//=============================================================================*
BAR_TypeLib::~BAR_TypeLib()
{
   BAR_TRACE1(2, "unregistering type library, tlb=%s ", m_sLabel.c_str());


   if ( m_pTlbAttr != NULL )
   {
        HRESULT hr = ::UnRegisterTypeLib(m_iidTlb,
                                         m_pTlbAttr->wMajorVerNum,
                                         m_pTlbAttr->wMinorVerNum,
                                         m_pTlbAttr->lcid,
                                         m_pTlbAttr->syskind);
        if ( FAILED(hr) )
        {
            Warning_UnregisteringTlbFailed(hr);
        }
   }

    //=== free type library resources
    if ( m_pITlb != NULL )
    {
        if ( m_pTlbAttr != NULL )
        {
            m_pITlb->ReleaseTLibAttr(m_pTlbAttr);
        }
        m_pITlb->Release();
    }
    m_pITlb    = NULL;
    m_pTlbAttr = NULL;

    //=== finished
}

//=============================================================================*
/**
 * <B>compare guids</B>
 * @param           
 * @param           
 * @param           
 * @exception       -
 */
//=============================================================================*
void BAR_TypeLib::CompareGuids(const GUID& guidFound, const GUID& guidExpected) const
{
    //=== convert found and expected guid into strings
    const string sGuidFound    = GUIDToString(guidFound);
    const string sGuidExpected = GUIDToString(guidExpected);
    
    //=== ... and compare if they are equal
    if ( sGuidFound == sGuidExpected )
    {
        BAR_TRACE1(2, "type library has correct guid, guid=%s", sGuidFound.c_str());
        return;
    }
    else
    {
        Throw_RegisteringTlbFailed(E_FAIL, 
                                   wstring(L"type library has wrong guid")
                                   + L", found=" 
                                   + ToWS(sGuidFound)
                                   + L", expected=" 
                                   + ToWS(sGuidExpected)
                                  );
    }
}


//=============================================================================*
/**
 * <B></B>
 * @param           
 * @param           
 * @param           
 * @exception       -
 */
//=============================================================================*
void BAR_TypeLib::Throw_FatalError(const wstring& wsDetails) const
{
    throw BAR_Error(ERR_0400, wsDetails);
}

//=============================================================================*
/**e
 * <B></B>
 * @param           
 * @param           
 * @param           
 * @exception       -
 */
//=============================================================================*
void BAR_TypeLib::Throw_RegisteringTlbFailed(HRESULT hr, 
                                             const wstring& wsDetails) const
{
    throw BAR_Error(ERR_0401,
                    wstring(L"could not register ")
                    + ToWS(m_sLabel) + L" type library",
                    wsDetails + L", " + HRToWString(hr));
}

//=============================================================================*
/**
 * <B></B>
 * @param           
 * @param           
 * @param           
 * @exception       -
 */
//=============================================================================*
void BAR_TypeLib::Warning_UnregisteringTlbFailed(HRESULT hr) const
{
    m_callback.SendWarning(BAR_Warning(ERR_0402, 
                                       wstring(L"could not unregister ")
                                       + ToWS(m_sLabel) + L" type library",
                                       HRToWString(hr)));
}

/*
 *----------------------------------------------------------------------------*
 *  $History: BAR_TypeLib.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * 
 * *****************  Version 1  *****************
 * User: Kt           Date: 10.07.01   Time: 16:38
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_LIB
 * first draft
 *==
 *----------------------------------------------------------------------------*
*/

// ***************************  E O F  ******************************
// $Workfile: BAR_TypeLib.cpp $

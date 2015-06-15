/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_ArcConfigRestore.cpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_ArcConfigRestore.cpp $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_ArcConfigRestore.cpp $
 *
 * =PROJECT             CAK1010 ATCMControl
 *
 * =SWKE                4CBAR
 *
 * =COMPONENT           4CBAR_LIB
 *
 * =CURRENT      $Date: 28.02.07 19:01 $
 *           $Revision: 1 $
 *
 * =ENVIRONMENT             Win32:
 *                      MS Windows NT 4.0
 *                      MSVC++ V5.0 / MFC 4.2 / ATL 2.0 / DAO 3.5
 *                      MS Java 2.0
 *                      MS Visual SourceSafe 5.0
 *
 * =REFERENCES
 *==
 *----------------------------------------------------------------------------*
 *

 

 

 *==
 *
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  $History: BAR_ArcConfigRestore.cpp $
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
 * User: Kt           Date: 10.07.01   Time: 16:27
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_LIB
 * first draft
 * 
 * *****************  Version 1  *****************
 * User: Ff           Date: 8.03.00    Time: 15:28
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_LIB
*==
*******************************************************************************
H<<*/

//====  Local Defines:   ======================================================*

//====  Static Initializations:   =============================================*


#include <assert.h>

#include "BAR_ArcConfigRestore.hpp"
#include "BAR_Callback.hpp"
#include "BAR_ZIP.hpp"
#include "BAR_Trace.hpp"
#include "BAR_Util.hpp"

//=============================================================================*
// MEMBER FUNCTIONS OF CLASS BAR_ArcConfigRestore
//=============================================================================*

//=============================================================================*
BAR_ArcConfigRestore::BAR_ArcConfigRestore(BAR_Callback& callback)
//=============================================================================*
    : BAR_ArcConfig(callback)
{}


//=============================================================================*
void BAR_ArcConfigRestore::Open(const string& sArcFileName)
//=============================================================================*
{
    if ( IsOpened() )
    {
        Throw_FatalError(L"configuration restore archive opened twice");
    }

    //=== remember name of compressed target file
    SetCompressedFileName(sArcFileName);

    //=== get a temporary file name
    CreateTempFileName(); 
    BAR_TRACE2(2, "opening configuration restore archive, "
                   "tmp=%s, compressed=%s",
                   GetTempFileName().c_str(), 
                   GetCompressedFileName().c_str());
    
    //=== uncompress source file into temporary storage
    BAR_TRACE2(2, "uncompressing configuration restore archive, "
                  "tmp=%s, compressed=%s",
                   GetTempFileName().c_str(),
                   GetCompressedFileName().c_str());
    string errUncompress = BAR_ZIP::uncompressFile(GetCallback(),
                                                   GetCompressedFileName(),
                                                   GetTempFileName());
    if ( ! errUncompress.empty() )
    {
        Throw_UncompressingFailed(ToWS(errUncompress));
    }

    //=== try to open archive file for reading
    HRESULT hr = StgOpenStorage(CComBSTR(GetTempFileName().c_str()),
                                NULL,
                                m_dwModeStgRead,
                                NULL,
                                0,
                                &m_pStg);
    if ( FAILED(hr) )
    {
        Cleanup();
        Throw_CouldNotOpenCfgArc(hr, ToWS(GetTempFileName()),
                                     L"opening storage failed");
    }
    assert( m_pStg != NULL );
    if ( m_pStg == NULL )
    {
        Cleanup();
        Throw_FatalError(L"root-storage is null in open()");
    }

    
    //=== finished
    SetOpened(true);
}

//=============================================================================*
void BAR_ArcConfigRestore::Close()
//=============================================================================*
{
    BAR_TRACE2(2, "closing configuration restore archive, "
                  "tmp=%s, compressed=%s",
                  GetTempFileName().c_str(), GetCompressedFileName().c_str());

    //=== cleanup
    Cleanup();

    //=== finished
    SetOpened(false);
}

//=============================================================================*
IStorage* BAR_ArcConfigRestore::OpenDomainListStorage(CSC_TRANSFER_TARGET trg)
//=============================================================================*
{
    if ( trg == ProgramDomains )
    {
        //=== open sub storage for program domains
        HRESULT hr = m_pStg->OpenStorage(CComBSTR(m_sPrgStgName.c_str()),
                                         NULL,
                                         m_dwModeStgRead,
                                         NULL,
                                         0,
                                         &m_pStgPrgDomains);
        if ( FAILED(hr) )
        {
            Cleanup();
            Throw_CouldNotOpenCfgArc(hr, ToWS(GetTempFileName()), 
                                         L"opening program-substorage failed");
        }
        assert( m_pStgPrgDomains != NULL );
        if ( m_pStgPrgDomains == NULL )
        {
            Cleanup();
            Throw_FatalError(L"program-substorage is null in open()");
        }
    
        return m_pStgPrgDomains;
    }
    else if ( trg == VisualDomains )
    {
        //=== open sub storage for visual domains
        HRESULT hr = m_pStg->OpenStorage(CComBSTR(m_sVisStgName.c_str()),
                                         NULL,
                                         m_dwModeStgRead,
                                         NULL,
                                         0,
                                         &m_pStgVisDomains);
        if ( FAILED(hr) )
        {
            Cleanup();
            Throw_CouldNotOpenCfgArc(hr, ToWS(GetTempFileName()), 
                                         L"opening visual-substorage failed");
        }
        assert( m_pStgVisDomains != NULL );
        if ( m_pStgVisDomains == NULL )
        {
            Cleanup();
            Throw_FatalError(L"visual-substorage is null in open()");
        }

        return m_pStgVisDomains;
    }
    else
    {
        //=== should never happen
        assert(false);
        return NULL;
    }
}


//=============================================================================*
CComBSTR BAR_ArcConfigRestore::GetAttr(const string& sAttrName)
//=============================================================================*
{
    const string sDetails = string("name=") + sAttrName; 

    BAR_TRACE1(4, "reading backup attribute, %s", sDetails.c_str());

    if ( ! IsOpened() )
    {
        Throw_FatalError(L"restore archive not opened, can not read attribute");
    }
    
    //=== open attribute stream
    IStream* pStmAttr = NULL;
    assert(m_pStg != NULL);
    HRESULT hr = m_pStg->OpenStream(CComBSTR(sAttrName.c_str()),
                                    NULL,
                                    m_dwModeStmRead,
                                    0,
                                    &pStmAttr);
    if ( FAILED(hr) )
    {
        Throw_ReadingFailed(hr, wstring(L"could not open stream "
                                        L"for attribute, ")
                                + ToWS(sDetails));
    }
    assert( pStmAttr != NULL );
    if ( pStmAttr== NULL )
    {
        Throw_FatalError(L"stream is null in GetAttr()");
    }

    //=== read attribute value from attribute stream
    CComBSTR strAttrVal;
    hr = strAttrVal.ReadFromStream(pStmAttr);
    if ( FAILED(hr) )
    {
        pStmAttr->Release();
        Throw_StoringFailed(hr,
                            wstring(L"reading attribute from stream failed")
                            + ToWS(sDetails));
    }

    //=== close attribute stream
    pStmAttr->Release();

    //=== finished
    return strAttrVal;
}

//=============================================================================*
void BAR_ArcConfigRestore::GetDomainNodes(CSC_TRANSFER_TARGET trg,
                                          vector<CComBSTR>& vecDomainNodes)
//=============================================================================*
{
    const wstring wsDetails = wstring(L"trg=") + ToWS(TrgToString(trg));

    if ( ! IsOpened() )
    {
        Throw_FatalError(L"restore archive not opened, "
                         L"can not read domain nodes");
    }

    //=== clear return vector
    vecDomainNodes.clear();
    
    //=== get enumerator interface
    IEnumSTATSTG* pEnum = NULL;
    assert( ( trg == ProgramDomains ) || ( trg == VisualDomains ) );
    IStorage* pStgDomains = OpenDomainListStorage(trg); //=== will be released
                                                        //=== by destructor
                                                        //=== in error case
    HRESULT hr = pStgDomains->EnumElements(0, NULL, 0, &pEnum);
    if ( FAILED(hr) )
    {
        Throw_FatalError(wstring(L"could not enumerate domain nodes, ")
                         + wsDetails + L", "
                         + HRToWString(hr));
    }
    assert( pEnum != NULL );
    if ( pEnum == NULL )
    {
        Throw_FatalError(L"enumerator is null in GetDomainNodes()");
    }

    //=== reset enumerator
    hr = pEnum->Reset();
    if ( FAILED(hr) )
    {
        pEnum->Release();
        Throw_FatalError(wstring(L"could not reset domain enumerator, ")
                        + wsDetails + L", "
                        + HRToWString(hr));
    }

    //=== loop over all elements
    //=== and append their names
    //=== to domain name vector
	STATSTG statstg;
    while(true)
	{
        ULONG celtFetched = 0;
		hr = pEnum->Next(1, &statstg, &celtFetched);
        if ( FAILED(hr) )
        {
            pEnum->Release();
            Throw_FatalError(wstring(L"next failed in GetDomainNodes(), ")
                            + wsDetails + L", "
                            + HRToWString(hr));
        }
        if ( celtFetched != 1 )
            break;
		if ( statstg.type == STGTY_STORAGE )
            vecDomainNodes.push_back(statstg.pwcsName);

        CoTaskMemFree(statstg.pwcsName);
    }
    
    //=== finished
    pEnum->Release();
    ReleaseDomainListStorage(trg);
}

//=============================================================================*
void BAR_ArcConfigRestore::GetDomain(CSC_TRANSFER_TARGET trg,
                                     CComBSTR strDomainNode,
                                     CComBSTR& strDomainName,
                                     SAFEARRAY** ppsa)
//=============================================================================*
{
    const wstring wsDetails = wstring(L"trg=") + ToWS(TrgToString(trg))
                          + L", node=" + wstring(strDomainNode.m_str).c_str();
    BAR_WTRACE1(4, L"reading domain from archive, %s", wsDetails.c_str());

    if ( ! IsOpened() )
    {
        Throw_FatalError(wstring(L"restore archive not opened, "
                                 L"can not read domain, ")
                         + wsDetails);
    }
        
    //=== select parent storage
    assert( ( trg == ProgramDomains ) || ( trg == VisualDomains ) );
    IStorage* pStgParent = OpenDomainListStorage(trg); //=== will be released
                                                       //=== by destructor
                                                       //=== in error case

    //=== open domain storage
    IStorage* pStgDomain = NULL;
    HRESULT hr = pStgParent->OpenStorage(strDomainNode, 
                                         NULL,
                                         m_dwModeStgRead,
                                         NULL,
                                         0,
                                         &pStgDomain);
    if ( FAILED(hr) )
    {
        Throw_ReadingFailed(hr, wstring(L"could not open stream for domain, ") + wsDetails);
    }
    assert( pStgDomain != NULL );
    if ( pStgDomain == NULL )
    {
        Throw_FatalError(L"domain-storage is null in GetDomain()");
    }

    //=== open domain name stream
    IStream* pStmDomainName = NULL;
    hr = pStgDomain->OpenStream(CComBSTR(m_sNodeName.c_str()),
                                NULL,
                                m_dwModeStmRead,
                                0,
                                &pStmDomainName);
    if ( FAILED(hr) )
    {
        pStgDomain->Release();
        Throw_ReadingFailed(hr,
                            wstring(L"could not open domain-name-stream, ")
                            + wsDetails);
    }
    assert( pStmDomainName != NULL );
    if ( pStmDomainName == NULL )
    {
        pStgDomain->Release();
        Throw_FatalError(L"domain-name-stream is null in GetDomain()");
    }

    //=== read domain name
    hr = strDomainName.ReadFromStream(pStmDomainName);
    if ( FAILED(hr) )
    {
        pStgDomain->Release();
        pStmDomainName->Release();
        Throw_ReadingFailed(hr, 
                            wstring(L"could not read "
                                    L"from domain-name-stream, ")
                            + wsDetails);
    }

    //=== close domain name stream
    pStmDomainName->Release();    

    //=== open domain content stream 
    IStream* pStmDomainCont = NULL;
    hr = pStgDomain->OpenStream(CComBSTR(m_sNodeCont.c_str()),
                                NULL,
                                m_dwModeStmRead,
                                0,
                                &pStmDomainCont);
    if ( FAILED(hr) )
    {
        pStgDomain->Release();
        Throw_ReadingFailed(hr,
                            wstring(L"could not create "
                                    L"domain-content-stream, ")
                            + wsDetails);
    }
    assert( pStmDomainCont != NULL );
    if ( pStmDomainCont == NULL )
    {
        pStgDomain->Release();
        Throw_FatalError(L"domain-content-stream is null in GetDomain()");
    }

    //=== get stream stats
    STATSTG statstg;
    hr = pStmDomainCont->Stat(&statstg, STATFLAG_NONAME);
    if ( FAILED(hr) )
    {
        pStgDomain->Release();
        pStmDomainCont->Release();
        Throw_ReadingFailed(hr, wstring(L"could not read "
                                        L"domain content stream stats")
                                + wsDetails);
    }
    assert(statstg.cbSize.HighPart == 0); //=== only Lowpart is used later on
    if (statstg.cbSize.HighPart != 0)
    {
        Throw_FatalError(L"domain content stream size too big");
    }

    //=== create a safearray of appropriate size
    *ppsa = SafeArrayCreateVector(VT_UI1, 0, statstg.cbSize.LowPart);
    if ( *ppsa == NULL )
    {
        pStgDomain->Release();
        pStmDomainCont->Release();
        Throw_FatalError(wstring(L"could not create safe array, ")
                         + HRToWString(hr));
    }

    //=== access safe array data
    void HUGEP* pData = NULL;
    hr = SafeArrayAccessData(*ppsa, &pData);
    if ( FAILED(hr) )
    {
        pStgDomain->Release();
        pStmDomainCont->Release();
        Throw_FatalError(wstring(L"could not access domain content "
                                 L"safe array data, ")
                         + HRToWString(hr));
    }
    assert( pData != NULL);
    if ( pData == NULL )
    {
        pStgDomain->Release();
        pStmDomainCont->Release();
        Throw_FatalError(L"accessed domain content data "
                         L"is null in GetDomain()");
    }

    //=== copy stream into safearray
    hr = pStmDomainCont->Read(pData, statstg.cbSize.LowPart, NULL);
    if ( FAILED(hr) )
    {
        pStgDomain->Release();
        pStmDomainCont->Release();
        Throw_FatalError(wstring(L"could not read "
                                 L"domain content data from stream, ")
                         + HRToWString(hr));
             
    }

    //=== unaccess safe array data
    hr = SafeArrayUnaccessData(*ppsa);

    //=== close content stream
    pStmDomainCont->Release();

    //== close domain node storage
    pStgDomain->Release();

    //=== close domain list storage
    ReleaseDomainListStorage(trg);

    //=== finished
}

// ***************************  E O F  ******************************
// $Workfile: BAR_ArcConfigRestore.cpp $

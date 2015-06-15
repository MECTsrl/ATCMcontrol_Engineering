/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_ArcConfigBackup.cpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_ArcConfigBackup.cpp $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_ArcConfigBackup.cpp $
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
 *  $History: BAR_ArcConfigBackup.cpp $
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
 * User: Kt           Date: 10.07.01   Time: 16:25
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_LIB
 * first draft
 * 
 * *****************  Version 1  *****************
 * User: Ff           Date: 8.03.00    Time: 15:27
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_LIB
*==
*******************************************************************************
H<<*/

//====  Local Defines:   ======================================================*

//====  Static Initializations:   =============================================*


#include <assert.h>

#include "BAR_ArcConfigBackup.hpp"
#include "BAR_ZIP.hpp"
#include "BAR_Trace.hpp"
#include "BAR_Util.hpp"

//=============================================================================*
// MEMBER FUNCTIONS OF CLASS BAR_ArcConfigBackup
//=============================================================================*

//=============================================================================*
BAR_ArcConfigBackup::BAR_ArcConfigBackup(BAR_Callback& callback)
//=============================================================================*
: BAR_ArcConfig(callback),
  m_ulVisDomId(0),
  m_ulPrgDomId(0),
  m_bProgramDomainListStorageCreated(false),
  m_bVisualDomainListStorageCreated(false)
{
}

//=============================================================================*
void BAR_ArcConfigBackup::Cleanup()
//=============================================================================*
{
    m_ulPrgDomId = 0;
    m_ulVisDomId = 0;

    BAR_ArcConfig::Cleanup();
}

//=============================================================================*
void BAR_ArcConfigBackup::Open(const string& sArcFile)
//=============================================================================*
{
    if ( IsOpened() )
    {
        Throw_FatalError(L"configuration backup archive opened twice");
    }

    //=== remember name of compressed target file
    SetCompressedFileName(sArcFile);

    //=== check if target file already exists
    if ( ExistsFile(sArcFile) )
    {
        //== and if so, ask for permission to remove it
        if ( IsOverwriteAllowed("The target file already exists. "
                                "Do you want to overwrite it ?") )
        {
            ::remove(sArcFile.c_str());
        }
        else
        {
            Throw_ArchiveFileAlreadyExists(ToWS(sArcFile));
        }
    }

    //=== get a temporary file name
    CreateTempFileName(); 
    BAR_TRACE2(2, "opening configuration backup archive, "
                   "tmp=%s, compressed=%s",
                   GetTempFileName().c_str(), 
                   GetCompressedFileName().c_str());

    //=== try to open new archive file for writing
    //=== fails if the file already exists
    HRESULT hr = ::StgCreateDocfile(CComBSTR(GetTempFileName().c_str()),
                                    m_dwModeStgWrite,
                                    0,
                                    &m_pStg);
    if ( FAILED(hr) )
    {
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
void BAR_ArcConfigBackup::Close()
//=============================================================================*
{
    BAR_TRACE2(2, "closing configuration backup archive, "
                  "tmp=%s, compressed=%s",
                  GetTempFileName().c_str(), GetCompressedFileName().c_str());

    //=== close storage
    ReleaseInterfaces();
    
    //=== compress temporary file into target file
    string errCompress = "";
    if ( IsOpened() )
    {
        BAR_TRACE2(2, "compressing configuration backup archive, "
                      "tmp=%s, compressed=%s",
                       GetTempFileName().c_str(),
                       GetCompressedFileName().c_str());
        errCompress = BAR_ZIP::compressFile(GetCallback(),
                                            GetTempFileName(),
                                            GetCompressedFileName());
    }

    //=== cleanup
    Cleanup();

    m_bProgramDomainListStorageCreated = false;
    m_bVisualDomainListStorageCreated  = false;

    //=== archive is now closed
    SetOpened(false);
    
    //=== finished
    if ( ! errCompress.empty() )
    {
        Throw_CompressingFailed(ToWS(errCompress));
    }
}

//=============================================================================*
void BAR_ArcConfigBackup::CreateDomainListStorage(CSC_TRANSFER_TARGET trg)
//=============================================================================*
{
    IStorage* pStgDomainList = NULL;
    if ( trg == ProgramDomains )
    {
        HRESULT hr = m_pStg->CreateStorage(CComBSTR(m_sPrgStgName.c_str()),
                                           m_dwModeStgWrite,
                                           0,
                                           0,
                                           &pStgDomainList);
        if ( FAILED(hr) )
        {
            Cleanup();
            Throw_CouldNotOpenCfgArc(hr, ToWS(GetTempFileName()), 
                                         L"creating program-substorage failed");
        }
        m_bProgramDomainListStorageCreated = true;
    }
    else if ( trg == VisualDomains )
    {
        //=== create sub storage for visual domains
        HRESULT hr = m_pStg->CreateStorage(CComBSTR(m_sVisStgName.c_str()),
                                           m_dwModeStgWrite,
                                           0,
                                           0,
                                           &pStgDomainList);
        if ( FAILED(hr) )
        {
            Cleanup();
            Throw_CouldNotOpenCfgArc(hr, ToWS(GetTempFileName()), 
                                         L"creating visual-substorage failed");
        }
        m_bVisualDomainListStorageCreated = true;
    }
    else
    {
        //=== should never happen
        assert(false);
    }

    if ( pStgDomainList != NULL )
    {
        pStgDomainList->Release();
    }
}

//=============================================================================*
IStorage* BAR_ArcConfigBackup::OpenDomainListStorage(CSC_TRANSFER_TARGET trg)
//=============================================================================*
{
    if ( trg == ProgramDomains )
    {
        if ( ! m_bProgramDomainListStorageCreated )
        {
            CreateDomainListStorage(trg);
        }

        HRESULT hr = m_pStg->OpenStorage(CComBSTR(m_sPrgStgName.c_str()),
                                         NULL,
                                         m_dwModeStgWrite,
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
            Throw_FatalError(L"program-substorage is null");
        }
        return m_pStgPrgDomains;
    }
    else if ( trg == VisualDomains )
    {
        if ( ! m_bVisualDomainListStorageCreated )
        {
            CreateDomainListStorage(trg);
        }

        //=== create sub storage for visual domains
        HRESULT hr = m_pStg->OpenStorage(CComBSTR(m_sVisStgName.c_str()),
                                         NULL,
                                         m_dwModeStgWrite,
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
            Throw_FatalError(L"visual-substorage is null");
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
void BAR_ArcConfigBackup::StoreAttr(const string& sAttrName,
                                    CComBSTR strAttrVal)
//=============================================================================*
{
    const string sDetails = string("name=") + sAttrName; 

    BAR_TRACE1(4, "storing backup attribute, %s", sDetails.c_str());

    if ( ! IsOpened() )
    {
        Throw_FatalError(L"backup archive not opened, can not store attribute");
    }
    
    //=== create new attribute stream
    IStream* pStmAttr = NULL;
    HRESULT hr = m_pStg->CreateStream(CComBSTR(sAttrName.c_str()),
                                      m_dwModeStmWrite,
                                      0,
                                      0,
                                      &pStmAttr);
    if ( FAILED(hr) )
    {
        Throw_StoringFailed(hr, wstring(L"could not create stream "
                                        L"for attribute, ")
                                + ToWS(sDetails));
    }
    assert( pStmAttr != NULL );
    if ( pStmAttr== NULL )
    {
        Throw_FatalError(L"stream is null in StoreAttr()");
    }

    //=== store attribute value into attribute stream
    hr = strAttrVal.WriteToStream(pStmAttr);
    if ( FAILED(hr) )
    {
        pStmAttr->Release();
        Throw_StoringFailed(hr,
                            wstring(L"writing attribute to stream failed")
                            + ToWS(sDetails));
    }

    //=== close attribute stream
    pStmAttr->Release();

    //=== finished
}


//=============================================================================*
void BAR_ArcConfigBackup::StoreDomain(CSC_TRANSFER_TARGET trg,
                                      const string& sDomainName,
                                      const void* pDomainCont,
                                      ULONG ulDomainSize,
                                      ULONG& ulBytesWritten)
//=============================================================================*
{
    const string sDetails = string("trg=") + TrgToString(trg)
                          + ", domain=" + sDomainName;
    BAR_TRACE1(4, "storing domain, %s", sDetails.c_str());

    if ( ! IsOpened() )
    {
        Throw_FatalError(wstring(L"backup archive not opened, "
                                 L"can not store domain, ")
                         + ToWS(sDetails));
    }
    

    //=== select parent storage
    assert( ( trg == ProgramDomains ) || ( trg == VisualDomains ) );
    IStorage* pStgParent = OpenDomainListStorage(trg); //=== will be released
                                                       //=== by destructor
                                                       //=== in error case

    //=== create new domain storage
    IStorage* pStgDomain = NULL;
    const string sId = GetNextDomainId(trg);
    HRESULT hr = pStgParent->CreateStorage(CComBSTR(sId.c_str()), 
                                           m_dwModeStmWrite,
                                           0,
                                           0,
                                           &pStgDomain);
    if ( FAILED(hr) )
    {
        Throw_StoringFailed(hr,
                            wstring(L"could not create stream for domain, ")
                            + ToWS(sDetails));
    }
    assert( pStgDomain != NULL );
    if ( pStgDomain == NULL )
    {
        Throw_FatalError(L"domain-storage is null in StoreDomain()");
    }

    //=== create new domain name stream
    IStream* pStmDomainName = NULL;
    hr = pStgDomain->CreateStream(CComBSTR(m_sNodeName.c_str()),
                                  m_dwModeStmWrite,
                                  0,
                                  0,
                                  &pStmDomainName);
    if ( FAILED(hr) )
    {
        pStgDomain->Release();
        Throw_StoringFailed(hr,
                            wstring(L"could not create domain-name-stream, ")
                            + ToWS(sDetails));
    }
    assert( pStmDomainName != NULL );
    if ( pStmDomainName == NULL )
    {
        pStgDomain->Release();
        Throw_FatalError(L"domain-name-stream is null in StoreDomain()");
    }

    //=== store domain name into domain name stream
    hr = CComBSTR(sDomainName.c_str()).WriteToStream(pStmDomainName);
    if ( FAILED(hr) )
    {
        pStgDomain->Release();
        pStmDomainName->Release();
        Throw_StoringFailed(hr,
                            wstring(L"could not write to  domain-name-stream, ")
                            + ToWS(sDetails));
    }

    //=== close domain name stream
    pStmDomainName->Release();

    //=== create new domain content stream
    IStream* pStmDomainCont = NULL;
    hr = pStgDomain->CreateStream(CComBSTR(m_sNodeCont.c_str()),
                                  m_dwModeStmWrite,
                                  0,
                                  0,
                                  &pStmDomainCont);
    if ( FAILED(hr) )
    {
        pStgDomain->Release();
        Throw_StoringFailed(hr,
                            wstring(L"could not create domain-content-stream, ")
                            + ToWS(sDetails));
    }
    assert( pStmDomainCont != NULL );
    if ( pStmDomainCont == NULL )
    {
        pStgDomain->Release();
        Throw_FatalError(L"domain-content-stream is null in StoreDomain()");
    }

    //=== store domain content into domain content stream
    hr = pStmDomainCont->Write(pDomainCont,
                               ulDomainSize,
                               &ulBytesWritten);
    if ( FAILED(hr) )
    {
        pStgDomain->Release();
        pStmDomainCont->Release();
        Throw_StoringFailed(hr,
                            wstring(L"could not write to  domain-content-stream, ")
                            + ToWS(sDetails));
    }

    //=== close domain content stream
    pStmDomainCont->Release();

    //=== close domain storage
    pStgDomain->Release();

    //=== close domain list storage
    ReleaseDomainListStorage(trg);

    //=== finished
}

//=============================================================================*
string BAR_ArcConfigBackup::GetNextDomainId(CSC_TRANSFER_TARGET trg)
//=============================================================================*
{
    ostringstream ossBuf;
    ossBuf.width(6);
    ossBuf.fill('0');
    if ( trg == ProgramDomains )
    {
        ossBuf << m_ulPrgDomId;
        m_ulPrgDomId++;
    }
    else if ( trg == VisualDomains )
    {
        ossBuf << m_ulVisDomId;
        m_ulVisDomId++;
    }
    else
    {
        assert(false);
        return "";
    }

    return ossBuf.str();
}

// ***************************  E O F  ******************************
// $Workfile: BAR_ArcConfigBackup.cpp $

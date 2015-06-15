/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_ArcConfig.cpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_ArcConfig.cpp $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_ArcConfig.cpp $
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
 *  $History: BAR_ArcConfig.cpp $
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
 * User: Kt           Date: 10.07.01   Time: 16:24
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_LIB
 * first draft
 * 
 * *****************  Version 1  *****************
 * User: Ff           Date: 8.03.00    Time: 15:26
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_LIB
*==
*******************************************************************************
H<<*/

//====  Local Defines:   ======================================================*

//====  Static Initializations:   =============================================*


#include <atlbase.h>

#include <assert.h>

#include "BAR_Trace.hpp"
#include <ctime>

#include "BAR_ArcConfig.hpp"
#include "BAR_Callback.hpp"
#include "BAR_Util.hpp"

//=============================================================================*
// MEMBER FUNCTIONS OF CLASS BAR_ArcConfig
//=============================================================================*

//=============================================================================*
BAR_ArcConfig::BAR_ArcConfig(BAR_Callback& callback)
//=============================================================================*
: m_callback(callback),
  m_bIsOpened(false),
  m_sPasswordName("password"),
  m_pStg(NULL),
  m_pStgPrgDomains(NULL),
  m_pStgVisDomains(NULL),
  m_dwModeStgRead(STGM_READ | STGM_SHARE_EXCLUSIVE),
  m_dwModeStmRead(STGM_READ | STGM_SHARE_EXCLUSIVE),
  m_dwModeStgWrite(STGM_READWRITE | STGM_SHARE_EXCLUSIVE ),
  m_dwModeStmWrite(STGM_WRITE | STGM_SHARE_EXCLUSIVE | STGM_FAILIFTHERE | STGM_DIRECT),
  m_sPrgStgName(TrgToString(ProgramDomains)),
  m_sVisStgName(TrgToString(VisualDomains)),
  m_sNodeName("name"),
  m_sNodeCont("content"),
  m_sTmpFile(""),
  m_sCompressedFile("")
{
    srand( (unsigned)time( NULL ) );
}

//=============================================================================*
BAR_ArcConfig::~BAR_ArcConfig()
//=============================================================================*
{
    Cleanup();
}

//=============================================================================*
void BAR_ArcConfig::Cleanup()
//=============================================================================*
{
    ReleaseInterfaces();

    //=== remove temporary file
    if ( ! GetTempFileName().empty() )
    {
        int err = ::remove(GetTempFileName().c_str());
        if ( err == 0 )
        {
            BAR_TRACE1(2, 
                       "removed temporary file \"%s\"",
                       GetTempFileName().c_str());
        }
        else
        {
            Warning_CouldNotRemoveTempFile(ToWS(GetTempFileName()));
        }
    }

    SetTempFileName("");
    SetCompressedFileName("");
}

//=============================================================================*
void BAR_ArcConfig::ReleaseInterfaces()
//=============================================================================*
{
    ReleaseDomainListStorage(ProgramDomains);
    ReleaseDomainListStorage(VisualDomains);

    if ( m_pStg != NULL )
    {
        m_pStg->Release();
        m_pStg = NULL;
    }
}

//=============================================================================*
void BAR_ArcConfig::ReleaseDomainListStorage(CSC_TRANSFER_TARGET trg)
//=============================================================================*
{
    if ( trg == ProgramDomains )
    {
        if ( m_pStgPrgDomains != NULL )
        {
            m_pStgPrgDomains->Release();
            m_pStgPrgDomains = NULL;
        }
    }
    else if ( trg == VisualDomains )
    {
        if ( m_pStgVisDomains != NULL )
        {
            m_pStgVisDomains->Release();
            m_pStgVisDomains = NULL;
        }
    }
    else
    {
        //=== should NEVER happen
        assert(false);
    }
}

//=============================================================================*
bool BAR_ArcConfig::IsOverwriteAllowed(const string& sQuestion)
//=============================================================================*
{
    return GetCallback().IsOverwriteAllowed(sQuestion); 
}

//=============================================================================*
wstring BAR_ArcConfig::GetStgErrTextW(HRESULT hr)
//=============================================================================*
{
    wstring errText = HRToWString(hr);

    if ( hr == S_OK )
        errText += L", no error occured";
    else if ( hr == STG_E_ACCESSDENIED )
        errText += L", access denied";
    else if ( hr == STG_E_FILEALREADYEXISTS )
        errText += L", file already exists";
    else if ( hr == STG_E_FILENOTFOUND)
        errText += L", file not found";
    else if ( hr == STG_E_PATHNOTFOUND)
        errText += L", path not found";
    else if ( hr == STG_E_INSUFFICIENTMEMORY  )
        errText += L", insufficient memory";
    else if ( hr == STG_E_INVALIDNAME )
        errText += L", invalid file name";
    else if ( hr == STG_E_LOCKVIOLATION )
        errText += L", lock violation";
    else if ( hr == STG_E_SHAREVIOLATION )
        errText += L", share violation";
    else if ( hr == STG_E_TOOMANYOPENFILES )
        errText += L", too many open files";
    else if ( hr == STG_E_MEDIUMFULL )
        errText += L", medium full";
    else  if ( hr == STG_E_CANTSAVE )
        errText += L", can't save";
    else  if ( hr == STG_E_WRITEFAULT )
        errText += L", write fault";

    return errText;
}

//=============================================================================*
void BAR_ArcConfig::CreateTempFileName()
//=============================================================================*
{
    char szBuf[MAX_PATH];

    //=== get path to temp dir
    string sTmpDir(".");
    if ( 0 != ::GetTempPath(MAX_PATH, szBuf) )
        sTmpDir = szBuf;
  
    //=== get a unique tmp file name
    if ( 0 == ::GetTempFileName(sTmpDir.c_str(),
                                "4CBAR",
                                (UINT)rand()%1000,
                                szBuf)
       )
    {
        Throw_FatalError(L"could not get a temporary file name");
    }

    //=== finished
    SetTempFileName(szBuf);
}

//=============================================================================*
void BAR_ArcConfig::Warning_CouldNotRemoveTempFile(const wstring& wsFileName)
//=============================================================================*
{
    m_callback.SendWarning(BAR_Warning(WRN_0300, 
                                       L"could not remove temporary file",
                                       wstring(L"file=") + wsFileName));
}

//=============================================================================*
void BAR_ArcConfig::Throw_FatalError(const wstring& wsDetails)
//=============================================================================*
{
    throw BAR_Error(ERR_0301, L"fatal error", wsDetails);
}

//=============================================================================*
void BAR_ArcConfig::Throw_CouldNotOpenCfgArc(HRESULT hr,
                                             const wstring& wsStgFile,
                                             const wstring& wsDetails)
//=============================================================================*
{
    throw BAR_Error(ERR_0302, 
                    L"could not open configuration archive",
                    wsDetails 
                    + L", file=" + wsStgFile
                    + L", " + GetStgErrTextW(hr));
}

//=============================================================================*
void BAR_ArcConfig::Throw_StoringFailed(HRESULT hr,
                                        const wstring& wsDetails)
//=============================================================================*
{
    throw BAR_Error(ERR_0303, 
                    L"storing data to configuration archive failed",
                    wsDetails + L", " + GetStgErrTextW(hr));
}

//=============================================================================*
void BAR_ArcConfig::Throw_CompressingFailed(const wstring& wsDetails)
//=============================================================================*
{
    throw BAR_Error(ERR_0304, L"compressing archive failed", wsDetails);
}

//=============================================================================*
void BAR_ArcConfig::Throw_ArchiveFileAlreadyExists(const wstring& wsArcFile)
//=============================================================================*
{
    throw BAR_Error(ERR_0305, L"archive file already exists", 
                              wstring(L"file=") + wsArcFile);
}

//=============================================================================*
void BAR_ArcConfig::Throw_UncompressingFailed(const wstring& wsDetails)
//=============================================================================*
{
    throw BAR_Error(ERR_0306, L"uncompressing archive failed", wsDetails);
}

//=============================================================================*
void BAR_ArcConfig::Throw_ReadingFailed(HRESULT hr,
                                        const wstring& wsDetails)
//=============================================================================*
{
    throw BAR_Error(ERR_0307, 
                    L"reading data from configuration archive failed",
                    wsDetails + L", " + GetStgErrTextW(hr));
}

// ***************************  E O F  ******************************
// $Workfile: BAR_ArcConfig.cpp $

/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_ArcConfig.hpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_ArcConfig.hpp $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_ArcConfig.hpp $
 *
 * =PROJECT             CAK1020  ATCMControl V2.0
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
 *                      MSVC++ V6.0 / / ATL 2.0
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
 *  $History: BAR_ArcConfig.hpp $
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

#ifndef _BAR_ARCCONFIG_H_
#define _BAR_ARCCONFIG_H_

//====  Aggregate Includes:   =================================================*
#include <windows.h>
#include <string>
using namespace ::std;

#include "CSC_Online.h"

//====  Forward Class Definitions:   ==========================================*
class BAR_Callback;

//====  Defines:    ===========================================================*

//====  Globals:    ===========================================================*

//====  Prototypes:    ========================================================*



//=============================================================================*
/**
 *  class BAR_ArcConfig
 *
 *  @see    ....
*/
//=============================================================================*
class BAR_ArcConfig
{

public: //=== con-, destruction and assignment

   BAR_ArcConfig(BAR_Callback& callback);
   virtual ~BAR_ArcConfig();

    virtual void Open(const string& sFileName)=0;
    virtual void Close();
    
public: //=== access

public: //=== operations

public: //=== overrides

protected: //=== helpers

    enum { WRN_0300 = 300,
           ERR_0301 = 301,
           ERR_0302 = 302,
           ERR_0303 = 303,
           ERR_0304 = 304,
           ERR_0305 = 305,
           ERR_0306 = 306,
           ERR_0307 = 307
    };
    
    virtual void Cleanup();

    wstring GetStgErrTextW(HRESULT hr);

    void CreateTempFileName();
    const string& GetTempFileName() const;
    void SetTempFileName(const string& sTmpFileName);

    const string& GetCompressedFileName() const;
    void SetCompressedFileName(const string& sComprFile);

    virtual IStorage* OpenDomainListStorage(CSC_TRANSFER_TARGET trg) = 0;
    void ReleaseDomainListStorage(CSC_TRANSFER_TARGET trg);

    void ReleaseInterfaces();

    void Warning_CouldNotRemoveTempFile(const wstring& wsFileName);
    void Throw_FatalError(const wstring& wsDetails);
    void Throw_CouldNotOpenCfgArc(HRESULT hr,
                                  const wstring& wsStgFile,
                                  const wstring& wsDetails);
    void Throw_StoringFailed(HRESULT hr, const wstring& wsDetails);
    void Throw_CompressingFailed(const wstring& wsDetails);
    void Throw_ArchiveFileAlreadyExists(const wstring& wsArcFile);
    void Throw_UncompressingFailed(const wstring& wsDetails);
    void Throw_ReadingFailed(HRESULT hr, const wstring& wsDetails);

    BAR_Callback& GetCallback() const;

    bool IsOpened() const;
    void SetOpened(bool bIsOpened);

    bool IsOverwriteAllowed(const string& sQuestion);
    
    const DWORD m_dwModeStgWrite;
    const DWORD m_dwModeStmWrite;
    const DWORD m_dwModeStgRead;
    const DWORD m_dwModeStmRead;

    const string m_sPasswordName;
    const string m_sPrgStgName;
    const string m_sVisStgName;
    const string m_sNodeName;
    const string m_sNodeCont;

    IStorage* m_pStg;
    IStorage* m_pStgPrgDomains;
    IStorage* m_pStgVisDomains;

private: //=== attributes

    BAR_Callback& m_callback;

    bool m_bIsOpened;


    string m_sTmpFile;
    string m_sCompressedFile;
};

//=============================================================================*
inline void BAR_ArcConfig::Close()
//=============================================================================*
{ Cleanup(); }

//=============================================================================*
inline BAR_Callback& BAR_ArcConfig::GetCallback() const
//=============================================================================*
{ return m_callback; }

//=============================================================================*
inline bool BAR_ArcConfig::IsOpened() const
//=============================================================================*
{ return m_bIsOpened; }

//=============================================================================*
inline void BAR_ArcConfig::SetOpened(bool bIsOpened)
//=============================================================================*
{ m_bIsOpened = bIsOpened; }

//=============================================================================*
inline const string& BAR_ArcConfig::GetTempFileName() const
//=============================================================================*
{ return m_sTmpFile; }

//=============================================================================*
inline void BAR_ArcConfig::SetTempFileName(const string& sTmpFileName)
//=============================================================================*
{ m_sTmpFile = sTmpFileName; }

//=============================================================================*
inline const string& BAR_ArcConfig::GetCompressedFileName() const
//=============================================================================*
{ return m_sCompressedFile; }

//=============================================================================*
inline void BAR_ArcConfig::SetCompressedFileName(const string& sComprFile)
//=============================================================================*
{ m_sCompressedFile = sComprFile; }


//=============================================================================*
#endif // _BAR_ARCCONFIG_H

// ***************************  E O F  ******************************
// $Workfile: BAR_ArcConfig.hpp $

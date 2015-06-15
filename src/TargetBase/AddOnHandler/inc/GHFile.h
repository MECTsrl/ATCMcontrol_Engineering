#ifndef _GHFILE_H_
#define _GHFILE_H_

//----  Aggregate Includes:   --------------------------------------*
#include "PouProtectData.h"

//------------------------------------------------------------------*
/**
 *  class CGHFile
 *
 *  @see    CPouProtectData
 */
class CGHFile
{
public:
    CGHFile();
    virtual ~CGHFile();

    void SetFileName(const CString& crstrFileName);

    BOOL Read();
    BOOL Write();

    CString GetPassword();
    void SetPassword(const CString& crstrPassword);

    CString GetProjectGuid();
    void SetProjectGuid(const CString& crstrProjectGuid);

    POSITION GetFirstPouProtectDataPosition();
    void GetNextPouProtectData(POSITION& rtPos, CPouProtectData& rtData);

    void AddPouProtectData(const CString& crstrFileName, const CString& crstrFingerprint);

    void ResetPouProtectData();
    
protected:
    BOOL StripLine(CString& rstrLine, CString& rstrRest, BOOL bFromEnd = FALSE);
    BOOL AddPouProtectData(const CString& crstrLine);

protected:
                                        // pou protection data list:
    CList<CPouProtectData, CPouProtectData&>    m_tPouProtectDataList;

    CString     m_strFileName;          // file name of gh file incl path
    CString     m_strPassword;          // password
    CString     m_strProjectGuid;       // project guid

};

#endif


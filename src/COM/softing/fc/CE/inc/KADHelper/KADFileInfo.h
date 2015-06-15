#ifndef __KADFILEINFO_H_
#define __KADFILEINFO_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "CEKadMan.h"
//#include "KadManager.h"

#include <atlbase.h>

//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*
class CKADFileExtInfo;

//----  Defines:    ------------------------------------------------*
typedef CMap<CString, LPCTSTR, CKADFileExtInfo*, CKADFileExtInfo*> T_FileExtMap;

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


//------------------------------------------------------------------*
/**
 *  class CKADFileInfo
 *
 *  - encapsulates file extension information from KAD
 *  - load information using Load()
 *  - get information for extension using GetFileExtInfo()
 *
 *  @see    class CKADFileExtInfo
 */
class CKADFileInfo
{
public:
    CKADFileInfo();
    ~CKADFileInfo();

    BOOL Load(CComPtr<IUnknown>& pKadMan);

    CKADFileExtInfo* GetFileExtInfo(CString strExt);

    void GetAllExtensions(CStringArray& rastrExtensions);

    int GetFileFilterForAllFiles(CString& rstrFilter);

    int GetFileFilterForSourceFiles(CString& rstrFilter);


protected:
    BOOL LoadFileExtInfo(CComPtr<IFile>& pIKadFile);

    void AddFileFilter(CString& rstrFilter, const CString& crstrExtension, const CString& crstrText);

protected:
    T_FileExtMap    m_tFileExtMap;      // map file extension -> file extension data
};

#endif // __KADFILEINFO_H_
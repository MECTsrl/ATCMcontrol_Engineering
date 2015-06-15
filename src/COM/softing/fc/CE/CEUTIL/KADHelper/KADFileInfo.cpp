
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "KADFileInfo.h"
#include "KADFileExtInfo.h"
#include "SBLLexTree.h"
#include "..\..\CeMain\resource.h"

//----  Static Initializations:   ----------------------------------*

CKADFileInfo::CKADFileInfo()
{
}

CKADFileInfo::~CKADFileInfo()
{
    // delete file extension info
    POSITION            tPosition;
    CString             strExt;
    CKADFileExtInfo*    pFileExtInfo;

    tPosition = m_tFileExtMap.GetStartPosition();
    while(tPosition)
    {
        m_tFileExtMap.GetNextAssoc(tPosition, strExt, pFileExtInfo);
        delete pFileExtInfo;
    }
}

//------------------------------------------------------------------*
/**
 * load file info from KAD
 *
 *  uses LoadFileExtInfo() to load information for a single file extension
 *
 * @param           [in] pKadMan: COM interface pointer to KAD Manager
 * @return          successful?
 * @see             LoadFileExtInfo()
 *
*/
BOOL CKADFileInfo::Load(CComPtr<IUnknown>& pKadMan)
{
    HRESULT hr;
    CComQIPtr<IFiles, &IID_IFiles> pIKadFiles;
    pIKadFiles = pKadMan;

    if (!pIKadFiles)
    {
        return FALSE;
    }

    CComPtr<IEnumFiles> pKadEnumFiles;
    hr = pIKadFiles->GetFileEnumerator(&pKadEnumFiles);
    if((hr != S_OK) || !pKadEnumFiles)
    {
        return FALSE;
    }

    pKadEnumFiles->Reset();
    ULONG n;
    CComPtr<IFile> pIKadFile;
    hr = pKadEnumFiles->Next(1, &pIKadFile, &n);
    while (SUCCEEDED(hr) && pIKadFile)
    {
        LoadFileExtInfo(pIKadFile);
        pIKadFile.Release();
        hr = pKadEnumFiles->Next(1, &pIKadFile, &n);
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * load file extension info from KAD
 *
 * @param           [in] pIKadFile: COM interface pointer to IFile of KAD
 * @return          successful?
 * @see             Load()
*/
BOOL CKADFileInfo::LoadFileExtInfo(CComPtr<IFile>& pIKadFile)
{
    CKADFileExtInfo*   pFileExtInfo;
    // create new type-info object
    pFileExtInfo = new CKADFileExtInfo;

    CComBSTR    sTmp;
    CComBSTR    sOrderKey;

    HRESULT hr = pIKadFile->GetExtension(&sTmp);
    if(hr == S_OK)
    {
        GUID    tGUIDTmp;
        BOOL    bTmp;

        pFileExtInfo->SetExtension(CString(sTmp));

        sTmp.Empty();
        hr = pIKadFile->GetIcon(&sTmp);
        if(hr == S_OK)
        {
            pFileExtInfo->m_strIcon = sTmp;
        }

        hr = pIKadFile->GetStandardEditor(&tGUIDTmp);
        if(hr == S_OK)
        {
            pFileExtInfo->SetEditorGUID(tGUIDTmp);
        }

        hr = pIKadFile->GetProgramEditor(&tGUIDTmp);
        if(hr == S_OK)
        {
            pFileExtInfo->SetEditorGUID(tGUIDTmp);
            pFileExtInfo->m_bProgEditor = TRUE;
        }

        sTmp.Empty();
        hr = pIKadFile->GetExeEditor(&sTmp);
        if(hr == S_OK)
        {
            pFileExtInfo->m_strExeEditor = sTmp;
        }

        sTmp.Empty();
        hr = pIKadFile->GetFilterText(&sTmp);
        if(hr == S_OK)
        {
            pFileExtInfo->m_strFilterText = sTmp;
        }

        hr = pIKadFile->GetParser(&tGUIDTmp);
        if(hr == S_OK)
        {
            pFileExtInfo->SetParserGUID(tGUIDTmp);
        }

        hr = pIKadFile->GetVisibilityFlag(&bTmp);
        if(hr == S_OK)
        {
            pFileExtInfo->m_bVisible = bTmp;
        }

        hr = pIKadFile->GetFindInFilesFlag(&bTmp);
        if(hr == S_OK)
        {
            pFileExtInfo->m_bFindInFiles = bTmp;
        }

        hr = pIKadFile->GetOrderKey(&sOrderKey);
        if(hr == S_OK)
        {
            pFileExtInfo->m_strOrderKey = sOrderKey;
        }

        hr = pIKadFile->GetPrintFlag(&bTmp);
        if(hr == S_OK)
        {
            pFileExtInfo->m_bPrint = bTmp;
        }

        // add file extension -> extension info to map
        m_tFileExtMap.SetAt(pFileExtInfo->m_strExt, pFileExtInfo);
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * get file extension info
 *
 * @param           [in] crstrExt: file extension without '.'
 * @return          pointer to file extension info object
 *
*/
CKADFileExtInfo* CKADFileInfo::GetFileExtInfo(CString strExt)
{
    CKADFileExtInfo*   pFileExtInfo;

    strExt.MakeLower();
    if(!m_tFileExtMap.Lookup(strExt, pFileExtInfo))
        return 0;
    return(pFileExtInfo);
}

//------------------------------------------------------------------*
/**
 * get all file extensions
 *
 * @param           [out] rastrExtensions: string array with file extensions
*/
void CKADFileInfo::GetAllExtensions(CStringArray& rastrExtensions)
{
    POSITION            pos;
    CString             strKey;
    CKADFileExtInfo*    pFileExtInfo;

    rastrExtensions.RemoveAll();
    pos = m_tFileExtMap.GetStartPosition();
    while(pos!=NULL)
    {
        m_tFileExtMap.GetNextAssoc(pos, strKey, pFileExtInfo);
        rastrExtensions.Add(strKey);
    }
}

//------------------------------------------------------------------*
/**
 * add file filters for all files.
 *
 *  used for file dialogs.
 *
 * @param           [out] rstrFilter: file filter
 * @return          number of filters
 *
*/
int CKADFileInfo::GetFileFilterForAllFiles(CString& rstrFilter)
{
    int                 iCount = 0;
    POSITION            pos;
    CString             strKey;
    CKADFileExtInfo*    pFileExtInfo;
    CString             strCombinationFilterText; 
    CString             strCombinationFilter;
    CString             strFilterTmp;

    CSBLLexTree<CKADFileExtInfo>         tFileExtTree;

    strCombinationFilterText.LoadString(IDS_KAD_HELPER_FILTER_SOURCE_FILES);

    // get all filters and put them into lex tree to get them sorted
    pos = m_tFileExtMap.GetStartPosition();
    while(pos!=NULL)
    {
        m_tFileExtMap.GetNextAssoc(pos, strKey, pFileExtInfo);
        if(pFileExtInfo)
        {
            if(!pFileExtInfo->m_strOrderKey.IsEmpty() && !pFileExtInfo->m_strFilterText.IsEmpty() && pFileExtInfo->m_pParserGUID)
            {
                VERIFY(tFileExtTree.insertData(pFileExtInfo->m_strOrderKey, pFileExtInfo));
            }
        }
    }

    // run over lex tree and append filters
    CStringArray    astrExt;
    int             iNumExt;
    tFileExtTree.getAllStringsFromSubstring("", astrExt);
    iNumExt = astrExt.GetSize();

    for(int iExt = 0; iExt < iNumExt; ++iExt)
    {
        if(!strCombinationFilter.IsEmpty())
        {
            strCombinationFilter += _T(";");
        }
        pFileExtInfo = tFileExtTree.getData(astrExt[iExt]);
        if(pFileExtInfo)
        {
            AddFileFilter(rstrFilter, pFileExtInfo->m_strExt, pFileExtInfo->m_strFilterText);
            strFilterTmp.Format("*.%s", pFileExtInfo->m_strExt);
            strCombinationFilter += strFilterTmp;
            ++iCount;
        }
    }

    if(!strCombinationFilter.IsEmpty())
    {
        // add combination filter as first
        rstrFilter = strCombinationFilterText + TCHAR('\0') + 
            strCombinationFilter + TCHAR('\0') + rstrFilter;
        ++iCount;
    }
    return iCount;
}



//------------------------------------------------------------------*
/**
 * add file filters for source files.
 *
 *  used for file dialogs.
 *
 * @param           [out] rstrFilter: file filter
 * @return          number of filters
 *
*/
int CKADFileInfo::GetFileFilterForSourceFiles(CString& rstrFilter)
{
    int                 iCount = 0;
    POSITION            pos;
    CString             strKey;
    CKADFileExtInfo*    pFileExtInfo;
    CString             strCombinationFilterText; 
    CString             strCombinationFilter;
    CString             strFilterTmp;

    strCombinationFilterText.LoadString(IDS_KAD_HELPER_FILTER_SOURCE_FILES);

    CSBLLexTree<CKADFileExtInfo>         tFileExtTree;

    pos = m_tFileExtMap.GetStartPosition();
    while(pos!=NULL)
    {
        m_tFileExtMap.GetNextAssoc(pos, strKey, pFileExtInfo);
        if(pFileExtInfo)
        {
            if(!pFileExtInfo->m_strOrderKey.IsEmpty() && !pFileExtInfo->m_strFilterText.IsEmpty() && pFileExtInfo->m_pParserGUID)
            {
                VERIFY(tFileExtTree.insertData(pFileExtInfo->m_strOrderKey, pFileExtInfo));
            }
        }
    }

    // run over lex tree and append filters
    CStringArray    astrExt;
    int             iNumExt;
    tFileExtTree.getAllStringsFromSubstring("", astrExt);
    iNumExt = astrExt.GetSize();

    for(int iExt = 0; iExt < iNumExt; ++iExt)
    {
        if(!strCombinationFilter.IsEmpty())
        {
            strCombinationFilter += _T(";");
        }
        pFileExtInfo = tFileExtTree.getData(astrExt[iExt]);
        if(pFileExtInfo)
        {
            AddFileFilter(rstrFilter, pFileExtInfo->m_strExt, pFileExtInfo->m_strFilterText);
            strFilterTmp.Format("*.%s", pFileExtInfo->m_strExt);
            strCombinationFilter += strFilterTmp;
            ++iCount;
        }
    }

    if(!strCombinationFilter.IsEmpty())
    {
        // add combination filter as first
        rstrFilter = strCombinationFilterText + TCHAR('\0') + 
            strCombinationFilter + TCHAR('\0') + rstrFilter;
        ++iCount;
    }
    return iCount;
}


//------------------------------------------------------------------*
/**
 * add file filter.
 *
 * @param           [in/out] rstrFilter: filter string
 * @param           [in] crstrExtension: file extension
 * @param           [in] crstrText: file filter text
 *
*/
void CKADFileInfo::AddFileFilter(CString& rstrFilter, const CString& crstrExtension, const CString& crstrText)
{
    CString strTmpText;
    CString strTmpFilter;

    strTmpText.Format("%s (*.%s)", crstrText, crstrExtension);
    strTmpFilter.Format("*.%s", crstrExtension);
    rstrFilter += strTmpText;
    rstrFilter += (TCHAR)'\0';
    rstrFilter += strTmpFilter;
    rstrFilter += (TCHAR)'\0';
}

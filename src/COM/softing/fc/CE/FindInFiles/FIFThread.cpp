
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "FIFThread.h"
#include "CEFIFComObj.h"
#include "StdEditorIF.h"
#include "StdEditorIF_i.c"
#include "Comhelper.h"
#include "CESysDef.h"
#include "regex.h"
#include "fc_todebug\fc_mem.h"

//----  Static Initializations:   ----------------------------------*

#define BUFSIZE 4096

//------------------------------------------------------------------*
/**
 * constructor.
 *
 *  create run event.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
CEFIFThread::CEFIFThread()
{
    // create run event with initital state TRUE
    m_hRunEvent = ::CreateEvent(NULL, FALSE, TRUE, NULL);
    m_bExit     = FALSE;
    m_pIFIFMsg  = NULL;
    m_iMatchCount = 0;
}

//------------------------------------------------------------------*
/**
 * destructor.
 *
 *  release edit interfaces used for search and close run event.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
CEFIFThread::~CEFIFThread()
{
    ReleaseEditInterfaces();
    ::CloseHandle(m_hRunEvent);
}

//------------------------------------------------------------------*
/**
 * release edit interfaces.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CEFIFThread::ReleaseEditInterfaces()
{
    // now release all edit
    CStringArray    astrGUID;
    int             iNumGUIDs;
    ICEEdit*        pEdit;

    m_tEditTree.getAllStringsFromSubstring(_T(""), astrGUID);
    iNumGUIDs = astrGUID.GetSize();
    for(int iGUID = 0; iGUID < iNumGUIDs; ++iGUID)
    {
        pEdit = m_tEditTree.getData(astrGUID[iGUID]);
        ASSERT(pEdit);
        if(pEdit)
        {
            pEdit->Release();
        }
    }
    m_tEditTree.reset();
}

//------------------------------------------------------------------*
/**
 * on initialize.
 *
 *  this function is called once at the beginning from within the thread.
 *
 * @param           -
 * @return          thread_noerror: OK
 *                  thread_error: error occurred.
 * @exception       -
 * @see             -
*/
int CEFIFThread::OnInitialize()
{
    HRESULT hr;
    m_bExit = FALSE;

    hr = ::CoInitializeEx(NULL,COINIT_MULTITHREADED);
    if(hr != S_OK)
    {
        return thread_error;
    }
    hr = ::CoGetInterfaceAndReleaseStream(m_pIStream, IID_IFIFMsgInternal, (void**)&m_pIFIFMsg);
    if(hr != S_OK)
    {
        return thread_error;
    }

    ASSERT(m_pIFIFMsg);

    // to be shure it is not blocking
    ::SetEvent(m_hRunEvent);

    return thread_noerror;
}


//------------------------------------------------------------------*
/**
 * on exit.
 *
 *  this function is called when the thread is to be terminated from 
 *  within the thread.
 *
 * @param           -
 * @return          thread_noerror: OK
 * @exception       -
 * @see             -
*/
int CEFIFThread::OnExit()
{
    m_pIFIFMsg->Release();
    ::CoUninitialize();
    return thread_noerror;
}

//------------------------------------------------------------------*
/**
 * schedule.
 *
 *  This is the worker function called within the thread.
 *  The search is started for all registered extensions.
 *  At the end the search results are sended to the message view.
 *
 * @param           -
 * @return          thread_break: thread is to be stopped.
 * @exception       -
 * @see             -
*/
int CEFIFThread::Schedule()
{
    CEFIFExtData    tData;
//    DWORD           dwResult;
    m_iMatchCount = 0;


    // run over all extensions
    POSITION    pos = m_pData->m_tExtDataList.GetHeadPosition();
    while(pos && !m_bExit)
    {
        tData = m_pData->m_tExtDataList.GetNext(pos);
        FindInFiles(m_pData->m_strDirectory, tData.m_strExtension, tData.m_strGUID, m_pData->m_strExpression, m_pData->m_dwFlags, m_iMatchCount);
//        dwResult = ::WaitForSingleObject(m_hRunEvent, INFINITE);
    }
    ReleaseEditInterfaces();
    OutputResult();
    return thread_break;
}

//------------------------------------------------------------------*
/**
 * output result.
 *
 *  send message with search result to message view.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CEFIFThread::OutputResult()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    CString     strMessage(_T(" "));
    CComBSTR    sEmptyMessage(strMessage);
    m_pIFIFMsg->NewFIFMessage(sEmptyMessage);

    strMessage.Format(IDS_FIF_MATCH, m_iMatchCount);
    CComBSTR    sResultMessage(strMessage);
    m_pIFIFMsg->NewFIFMessage(sResultMessage);
}



//------------------------------------------------------------------*
/**
 * find in files.
 *
 *  searches in files with a certain extension.
 *  uses FindStringInFile() to do real search.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             FindStringInFile()
*/
HRESULT CEFIFThread::FindInFiles
(
    const CString& crstrDirectory,
    const CString& crstrExtension,
    const CString& crstrGUID,
    const CString& crstrExpression,
    DWORD dwFlags,
    int& riMatchCount
)
{
    HRESULT hRes = S_OK;
    HRESULT hr;
    // get all files with extension
    WIN32_FIND_DATA fData;
    CString strFileTemplate;
    CString strFile;
    strFileTemplate.Format("%s\\*.%s", crstrDirectory, crstrExtension);

    HANDLE hFindFile = FindFirstFile(strFileTemplate, &fData);
    if (INVALID_HANDLE_VALUE != hFindFile)
    {
        do
        {
            CString strFile;

            if(m_bExit)
            {
                // abort was signaled
                FindClose(hFindFile);
                return -1;
            }

            if (FILE_ATTRIBUTE_DIRECTORY & fData.dwFileAttributes)
            {
                continue;
            }
            strFile = crstrDirectory + _T("\\") + fData.cFileName;

            // do again a check for the file extension: 
            //  FindFiles("*.4cl", ...) does also find the file afilename.4clc
            // it seems to compare only three letters of the file extension
            int iExtPos = strFile.ReverseFind(_T('.'));
            CString strExt;
            if (iExtPos>0)
            {
                strExt = strFile.Mid(iExtPos+1);
            }
            if (strExt.CompareNoCase(crstrExtension)==0)
            {
                hr = FindStringInFile(strFile, crstrExpression, crstrGUID, dwFlags, riMatchCount);
                if(hr != S_OK)
                {
                    hRes = hr;
                }
            }
        }
        while (FindNextFile(hFindFile, &fData));
        FindClose(hFindFile);
    }
    return hRes;
}


//------------------------------------------------------------------*
/**
 * find string in file.
 *
 *  use ICEEdit interface for search
 *  if FindFirst is not implemented -> call text search FindStringInTextFile()
 *
 * @param           crstrFilePathName: file name incl full path to be searched in
 * @param           crstrExpression: search expression
 * @param           crstrGUID: editor interface GUID
 * @param           dwFlags: search flags
 * @param           riMatchCount: total count of matches (in/out)
 * @return          S_OK: ok
 *                  error return value of ICEEdit::LoadDocument() or FindStringInTextFile()
 * @exception       -
 * @see             FindStringInTextFile()
*/
HRESULT CEFIFThread::FindStringInFile
(
    const CString& crstrFilePathName,
    const CString& crstrExpression,
    const CString& crstrGUID,
    DWORD dwFlags, 
    int& riMatchCount
)
{
    CString strMessage;
    HRESULT hr;
    BSTR    sLocation;
    CString strFileName;
    CString strLocation;

    CFile   tFileTmp;
    tFileTmp.SetFilePath(crstrFilePathName);
    strFileName = tFileTmp.GetFileName();

    // get edit handler interface
    ICEEdit*    pEdit = GetEditInterface(crstrGUID);
//    ASSERT(pEdit);
    if(pEdit)
    {
        // load document
        CComBSTR    sFilePathName(crstrFilePathName);
        CComBSTR    sExpression(crstrExpression);

        hr = pEdit->LoadDocument(sFilePathName);
        if(hr != S_OK)
        {
            return hr;
        }

        // search file with FindFirst / FindNext methods
        hr = pEdit->FindFirst(sExpression, dwFlags, &sLocation);
        // if find functionality not implemented -> use text search
        if(hr == E_NOTIMPL)
        {
            hr = FindStringInTextFile(crstrFilePathName, crstrExpression, dwFlags, riMatchCount);
            return hr;
        }
        if(hr != S_OK)
        {
            return (hr);
        }

        strLocation = sLocation;
        SysFreeString(sLocation);

        while(S_OK == hr)
        {
            strMessage.Format(_T("%s(%s):%s"), strFileName, strLocation, crstrExpression);
            CComBSTR    sMessage(strMessage);

            m_pIFIFMsg->NewFIFMessage(sMessage);

            riMatchCount++;
            hr = pEdit->FindNext(&sLocation);
            strLocation = sLocation;
            SysFreeString(sLocation);
        }
    }
    else
    {
        hr = FindStringInTextFile(crstrFilePathName, crstrExpression, dwFlags, riMatchCount);
    }
    return hr;
}

//------------------------------------------------------------------*
/**
 * find string in text file.
 *
 *  open file as text file.
 *  read each line.
 *  evaluate regular expression.
 *
 * @param           crstrFilePathName: file name incl full path to be searched in
 * @param           crstrExpression: search expression
 * @param           dwFlags: search flags
 * @param           riMatchCount: total count of matches (in/out)
 * @return          S_OK:
 *                  S_FALSE;
 *                  E_FAIL:
 *                  E_OUTOFMEMORY
 * @exception       -
 * @see             -
*/
HRESULT CEFIFThread::FindStringInTextFile
(
    const CString& crstrFilePathName,
    const CString& crstrExpression,
    DWORD dwFlags, 
    int& riMatchCount
)
{
    CString strMessage;
    CString strFileName;
    CString strBuffer;
    CString strText = crstrExpression;
    int iTextLen = strText.GetLength();
    int iLine = 1;
    int iOffset = 0;
    regex_t    Regex;      
    regmatch_t * pRegMatch = NULL;
    int iRegError;
    int iGroups = 0;
    int iLastMatchedLine = -1;
    CStdioFile File;

    File.SetFilePath(crstrFilePathName);
    strFileName = File.GetFileName();

    if (!File.Open(crstrFilePathName, CFile::modeRead | CFile::typeText))
    {
        return E_FAIL;
    }

    if (!(dwFlags & FIF_OPTION_CASESENSITIVE))
    {
        strText.MakeLower();
    }

    // compile regular expression
    if (dwFlags & FIF_OPTION_REGEXP)
    {
        iRegError = regcomp(&Regex, strText, REG_EXTENDED);
        if (iRegError)
        {
            TRACE("FIF-Error: compiling regular expression (errval = %d)\n", iRegError);
            File.Close();
            return E_FAIL;
        }

        iGroups = Regex.re_nsub + 1;
        if (!FC_NewMemory((void **)&pRegMatch, sizeof(*pRegMatch) * iGroups))
        {
            TRACE("FIF-Error: out of memory\n");
            regfree(&Regex);
            File.Close();
            return E_OUTOFMEMORY;
        }

    }

    // read a single line
    while (File.ReadString(strBuffer))
    {
        if(m_bExit)
        // check for termination of thread
        {
            regfree(&Regex);
            if(pRegMatch)
            {
                FC_FreeMemory(pRegMatch);
            }
            File.Close();
            return S_FALSE;
        }

        CString strCmpBuf = strBuffer;

        if (!(dwFlags & FIF_OPTION_CASESENSITIVE))
        {
            strCmpBuf.MakeLower();
        }

        do
        {   
            int iMatch;
            if (dwFlags & FIF_OPTION_REGEXP)
            {
                // check match of regular expression
                iRegError = regexec(&Regex, strCmpBuf, iGroups, pRegMatch, 0);
                if (iRegError != 0 && iRegError != REG_NOMATCH)
                {
                    TRACE("FIF-Error: executing regular expression (errval = %d)\n", iRegError);
                    regfree(&Regex);
                    FC_FreeMemory(pRegMatch);
                    File.Close();
                    return E_FAIL;
                }

                iMatch = (REG_NOMATCH != iRegError) ? pRegMatch[0].rm_so: -1;
            }
            else
            {
                // find match string in text line
                iMatch = strCmpBuf.Find(strText);
            }

            int iMatchLen = 0;
            BOOL bMatchInLine = FALSE;
            if (iMatch >= 0)
            {
                iMatchLen =  (dwFlags & FIF_OPTION_REGEXP) ? 
                              pRegMatch[0].rm_eo - pRegMatch[0].rm_so :
                              iTextLen;

                // check for word boundaries
                BOOL bLeft = (0 == iMatch || !__iscsym(strCmpBuf[iMatch - 1]));
                BOOL bRight = (iMatch + iMatchLen >= strCmpBuf.GetLength() || 
                               0 == strCmpBuf[iMatch + iMatchLen] || 
                               !__iscsym(strCmpBuf[iMatch + iMatchLen]));

                if (!(dwFlags & FIF_OPTION_WHOLEWORD) ||
                    (bLeft && bRight))
                {
                    bMatchInLine = TRUE;
                    riMatchCount++;
                    if (iLastMatchedLine != iLine)
                    {
                        // first match found in current line
                        iLastMatchedLine = iLine;
                        CString strMessage;
                        strMessage.Format(_T("%s(%d):%s"), strFileName, iLine, strBuffer);
                        CComBSTR    sMessage(strMessage);
                        m_pIFIFMsg->NewFIFMessage(sMessage);
                    }
                }
            }
            if (!bMatchInLine && iMatchLen > 0)
            {
                // match found but not as a whole word: check rest of line
                strCmpBuf = strCmpBuf.Mid(iMatch + iMatchLen);
            }
            else
            {
                iLine++;
                break;
            }
        }
        while (TRUE);
    }

    File.Close();

    if (dwFlags & FIF_OPTION_REGEXP)
    {
        FC_FreeMemory(pRegMatch);
        regfree(&Regex);
    }

    return S_OK;
}


//------------------------------------------------------------------*
/**
 * get edit interface.
 *
 *  get ICEEdit interface for GUID.
 *  if not yet created, create it using CreateEdit()
 *
 * @param           crstrGUID: editor interface GUID string
 * @return          editor interface pointer or NULL if failed to create
 * @exception       -
 * @see             CreateEdit()
*/
ICEEdit* CEFIFThread::GetEditInterface(const CString& crstrGUID)
{
    // find interface
    ICEEdit*    pEdit = m_tEditTree.getData(crstrGUID);
    if(!pEdit)
    {
        pEdit = CreateEdit(crstrGUID);
        m_tEditTree.insertData(crstrGUID, pEdit);
    }
    return pEdit;
}

//------------------------------------------------------------------*
/**
 * create edit.
 *
 * @param           crstrGUID: editor interface GUID string.
 * @return          editor interface pointer or NULL if failed.
 * @exception       -
 * @see             GetEditInterface()
*/
ICEEdit* CEFIFThread::CreateEdit(const CString& crstrGUID)
{
    HRESULT     hr;
    ICEEdit*    pEdit = NULL;
    IUnknown*   pUnk = NULL;
    CGuid       tGUID(crstrGUID);
    
    hr = CoCreateInstance(tGUID, NULL, CLSCTX_SERVER,
                          IID_IUnknown, (LPVOID *)&pUnk);
    if(hr != S_OK)
    {
        TRACE("Fehler1 in CEFIFThread::CreateEdit: hr = %d\n", hr);
    }
    hr = pUnk->QueryInterface(IID_ICEEdit, (LPVOID *)&pEdit);
    if(hr != S_OK)
    {
        TRACE("Fehler2 in CEFIFThread::CreateEdit: hr = %d\n", hr);
    }
    pUnk->Release();
    return pEdit;
}


//------------------------------------------------------------------*
/**
 * is thread running?.
 *
 * @param           -
 * @return          is thread running?
 * @exception       -
 * @see             -
*/
BOOL CEFIFThread::IsThreadRunning()
{
    DWORD   dwExitCode;
    if(m_hThread)
    {
        if(::GetExitCodeThread(m_hThread, &dwExitCode))
        {
            if(dwExitCode == STILL_ACTIVE)
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}

//------------------------------------------------------------------*
/**
 * break thread.
 *
 *  reset run event to stop thread.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CEFIFThread::Break()
{
    ::ResetEvent(m_hRunEvent);
}

//------------------------------------------------------------------*
/**
 * continue thread.
 *
 *  set run event to continue thread.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CEFIFThread::Continue()
{
    ::SetEvent(m_hRunEvent);
}

//------------------------------------------------------------------*
/**
 * stop thread.
 *
 *  set exit flag.
 *
 * @param           -
 * @return          TRUE: OK
 * @exception       -
 * @see             -
*/
BOOL CEFIFThread::Stop()
{
    m_bExit = TRUE;
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * get match count.
 *
 * @param           -
 * @return          number of matches.
 * @exception       -
 * @see             -
*/
int CEFIFThread::GetMatchCount()
{
    return m_iMatchCount;
}
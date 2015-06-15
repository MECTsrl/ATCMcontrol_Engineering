// Extract.cpp : Implementierung von CExtract
#include "stdafx.h"
#include "CeExtract.h"
#include "Extract.h"
//#include "toassert.h"
//#include "to_mem.h"
#include "utilif.h"
#include "regex.h"

//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CExtract


STDMETHODIMP CExtract::Extract(BSTR sSrcFile, BSTR *sHeaderFile)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    CComBSTR sTagName(CEEX_HEADER);
    CComBSTR sAttrName("");
    CComBSTR sAttrValue("");
//    CComBSTR sFileExt(UTIL_GetFileExtFromType(fileType_st));
    CComBSTR sFileExt(_T("st"));

    return ExtractTaggedBlock(sSrcFile, 0, sTagName, sAttrName, 
                              sAttrValue, sFileExt,
                              sHeaderFile, TRUE);
}

STDMETHODIMP CExtract::Merge(BSTR sMergeFile, BSTR sHeaderFile, BOOL bDelHeaderFile)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    CComBSTR sTagName(CEEX_HEADER);
    CComBSTR sAttrName("");
    CComBSTR sAttrValue("");
//    CComBSTR sFileExt(UTIL_GetFileExtFromType(fileType_st));
    CComBSTR sFileExt(_T("st"));

    return MergeTaggedBlock(sMergeFile, 0, sTagName, sAttrName, 
						    sAttrValue, sHeaderFile, TRUE, bDelHeaderFile, FALSE);
}

STDMETHODIMP CExtract::FindTaggedBlock(BSTR sSrcFile, long lOffset,
                                       BSTR sTagName, BSTR sAttrName,
                                       BSTR sAttrValue, long * plStartPos, 
                                       long * plLength)
{
    return FindXMLTag(sSrcFile, lOffset, sTagName, sAttrName, 
                      sAttrValue, plStartPos, plLength, enumFindBlockBetween);
}

STDMETHODIMP CExtract::FindTag(BSTR sSrcFile, long lOffset,
                               BSTR sTagName, BSTR sAttrName,
                               BSTR sAttrValue, long * plStartPos, 
                               long * plLength)
{
    return FindXMLTag(sSrcFile, lOffset, sTagName, sAttrName, 
                      sAttrValue, plStartPos, plLength, enumFindBeginToEndTag);
}

STDMETHODIMP CExtract::FindBeginTag(BSTR sSrcFile, long lOffset,
                                    BSTR sTagName, BSTR sAttrName,
                                    BSTR sAttrValue, long * plStartPos, 
                                    long * plLength)
{
    return FindXMLTag(sSrcFile, lOffset, sTagName, sAttrName, 
                      sAttrValue, plStartPos, plLength, enumFindBeginTag, FALSE);
}

HRESULT CExtract::FindXMLTag(BSTR sSrcFile, long lOffset,
                             BSTR sTagName, BSTR sAttrName,
                             BSTR sAttrValue, long * plStartPos, 
                             long * plLength, int iFindMode, BOOL bCRLF /* TRUE */)
{
    // bCRLF == TRUE: regard CRLF characters at the end of the tag
    // bCRLF == FALSE: disregard CRLF characters at the end of the tag
    BOOL bIsReadFileOpen = FALSE;
    CStdioFile InFile;
    CString strSrcFile;
    CString strBeginTagRegEx;
    CString strEndTagRegEx;
    HRESULT hr = S_FALSE;
    LPBYTE pByte = NULL;
    regex_t reTagBegin;
    regmatch_t rmTagBegin;
    regex_t reTagEnd;
    regmatch_t rmTagEnd;
    regex_t reEmptyTag;
    regmatch_t * prmEmptyTag = NULL;;
    BOOL bTagBeginRegExCompiled = FALSE;
    BOOL bTagEndRegExCompiled = FALSE;
    BOOL bEmptyTagRegExCompiled = FALSE;

	AFX_MANAGE_STATE(AfxGetStaticModuleState())
 
    try
    {
        CString strAttrName = sAttrName;
        CString strStartTagRegEx;
        CString strEmptyTagRegEx;
        int iRegExError;
        int iStart;
        int iEnd;
        BOOL bInTag = FALSE;

        strSrcFile = sSrcFile;

        if (strAttrName.IsEmpty())
        {
            strBeginTagRegEx.Format(CE_EXTRACT_BEGINTAGREGEX_NOATTR, sTagName);
            strEmptyTagRegEx.Format(CE_EXTRACT_EMPTYTAGREGEX_NOATTR, sTagName);
        }
        else
        {
            BSTR sEncodedAttrValue;
            hr = UTIL_XMLEncodeString(sAttrValue, &sEncodedAttrValue, XML_ENCODE_DEFAULT);
            if (FAILED(hr))
            {
                throw hr;
            }

            strBeginTagRegEx.Format(CE_EXTRACT_BEGINTAGREGEX_ATTR, 
                                    sTagName, sAttrName, sEncodedAttrValue);
            strEmptyTagRegEx.Format(CE_EXTRACT_EMPTYTAGREGEX_ATTR, 
                                    sTagName, sAttrName, sEncodedAttrValue);
            SysFreeString(sEncodedAttrValue);
        }

        if (bCRLF)
        {
            strBeginTagRegEx += CE_EXTRACT_CRLF;
            strEmptyTagRegEx += CE_EXTRACT_CRLF;
        }

        iRegExError = regcomp(&reTagBegin, strBeginTagRegEx, REG_EXTENDED);
        if (iRegExError)
        {
//            TO_DEBUGPRINT1(_T("CEEXTRACT> error %d compiling regular expression\n"), iRegExError);
            hr = E_FAIL;
            throw(hr);
        }

        bTagBeginRegExCompiled = TRUE;

        iRegExError = regcomp(&reEmptyTag, strEmptyTagRegEx, REG_EXTENDED);
        if (iRegExError)
        {
//            TO_DEBUGPRINT1(_T("CEEXTRACT> error %d compiling regular expression\n"), iRegExError);
            hr = E_FAIL;
            throw(hr);
        }

        bEmptyTagRegExCompiled = TRUE;

//        if (!TO_NewMemory((void **)&prmEmptyTag, sizeof(*prmEmptyTag) * (reEmptyTag.re_nsub + 1)))
        if (! (prmEmptyTag = (regmatch_t*)HeapAlloc(GetProcessHeap(),0,sizeof(*prmEmptyTag) * (reEmptyTag.re_nsub + 1))))
        {
            AfxThrowMemoryException();
        }

        strEndTagRegEx.Format(CE_EXTRACT_ENDTAGREGEX, sTagName);
        iRegExError = regcomp(&reTagEnd, strEndTagRegEx, REG_EXTENDED);
        if (iRegExError)
        {
//            TO_DEBUGPRINT1(_T("CEEXTRACT> error %d compiling regular expression\n"), iRegExError);
            hr = E_FAIL;
            throw(hr);
        }

        bTagEndRegExCompiled = TRUE;

        // !! restriction: element tag must not be larger than CEEXTRACT_COPYBUF_SIZE
//        if (!TO_NewMemory((void **)&pByte, CEEXTRACT_COPYBUF_SIZE + 2))
        if (! (pByte = (LPBYTE)HeapAlloc(GetProcessHeap(),0,CEEXTRACT_COPYBUF_SIZE + 2)))
        {
            AfxThrowMemoryException();
        }

        // open source file
        if (!InFile.Open(strSrcFile, 
                    CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary))
        {
            AfxThrowFileException(CFileException::fileNotFound, -1, strSrcFile);
        }

        bIsReadFileOpen = TRUE;


        long lPos = lOffset;
        UINT uiRead = 0;
        DWORD dwLength = InFile.GetLength();
        hr = S_FALSE;
        do
        {
            InFile.Seek(lPos, CFile::begin);
            uiRead = InFile.Read(pByte, CEEXTRACT_COPYBUF_SIZE);
            if (0 == uiRead)
            {
                break;
            }

            *(TCHAR *)(pByte + uiRead) = 0;

            if (!bInTag)
            {
                // check for element begin tag
                iRegExError = regexec(&reTagBegin, (LPCTSTR)pByte, reTagBegin.re_nsub + 1, &rmTagBegin, 0);
                if (!iRegExError)
                {
                    iStart = rmTagBegin.rm_so;
                    iEnd = rmTagBegin.rm_eo;
                    lPos += iEnd;
                    if (enumFindBeginTag == iFindMode ||
                        enumFindBeginToEndTag == iFindMode)
                    {
                        *plStartPos = lPos - iEnd + iStart;
                        if (enumFindBeginTag == iFindMode)
                        {
                            *plLength = iEnd - iStart;
                            hr = S_OK;
                            break;
                        }
                        else
                        {
                            bInTag = TRUE;
                            continue;
                        }
                    }
                    else
                    {
                        *plStartPos = lPos;
                        bInTag = TRUE;
                        continue;
                    }
                }
                else
                {
                    // Check for empty element tag
                    iRegExError = regexec(&reEmptyTag, (LPCTSTR)pByte, reEmptyTag.re_nsub + 1, prmEmptyTag, 0);
                    if (!iRegExError)
                    {
                    if (enumFindBeginTag == iFindMode ||
                        enumFindBeginToEndTag == iFindMode)
                        {
                            *plStartPos = iStart;
                            *plLength = iEnd - iStart;
                        }
                        else
                        {
                            lPos += prmEmptyTag[1].rm_so;
                            *plStartPos = lPos;
                            *plLength = -1;
                            hr = S_OK;
                            break;
                        }
                    }
                    else
                    {
                        // set file position variable for overlapped reading
                        lPos += CEEXTRACT_COPYBUF_SIZE / 2;
                    }
                }
            }

            if (bInTag)
            {
                // check for element end tag
                iRegExError = regexec(&reTagEnd, (LPCTSTR)pByte, reTagEnd.re_nsub + 1, &rmTagEnd, 0);
                if (!iRegExError)
                {
                    iStart = rmTagEnd.rm_so;
                    iEnd = rmTagEnd.rm_eo;

                    if (enumFindBeginTag == iFindMode ||
                        enumFindBeginToEndTag == iFindMode)
                    {
                        *plLength = lPos + iEnd - (*plStartPos);
                    }
                    else
                    {
                        *plLength = lPos + iStart - (*plStartPos);
                    }

                    bInTag = FALSE;
                    hr = S_OK;
                    break;
                }
                else
                {
                    // set file position variable for overlapped reading
                    lPos += CEEXTRACT_COPYBUF_SIZE / 2;
                }
            }
        }
        while (0 != uiRead && lPos < (long)dwLength);
    }

    catch (HRESULT hresult)
    {
//        TO_DEBUGPRINT(_T("CEEXTRACT> Decode error\n"));
        hr = hresult;
    }

    catch (CMemoryException *e)
    {
        e->Delete();
        hr = E_OUTOFMEMORY;
    }

    catch (CFileException *e)
    {
        switch (e->m_cause)
        {
            case CFileException::fileNotFound:   
//                TO_DEBUGPRINT1(_T("CEEXTRACT> File not found\n"), e->m_strFileName);
                break;

            default:
//                TO_DEBUGPRINT(_T("CEEXTRACT> File exception\n"));
                break;
        }
        e->Delete();
        hr = E_FAIL;
    }

    // free used resources

    if (NULL != pByte)
    {
//        TO_FreeMemory(pByte);
        HeapFree(GetProcessHeap(),0,pByte);
        pByte = NULL;
    }

    if (NULL != prmEmptyTag)
    {
//        TO_FreeMemory(prmEmptyTag);
        HeapFree(GetProcessHeap(),0,prmEmptyTag);
        prmEmptyTag = NULL;
    }

    if (bTagBeginRegExCompiled)
    {
        regfree(&reTagBegin);
    }

    if (bTagEndRegExCompiled)
    {
        regfree(&reTagEnd);
    }

    if (bEmptyTagRegExCompiled)
    {
        regfree(&reEmptyTag);
    }

    if (bIsReadFileOpen)
    {
        InFile.Close();
    }

	return hr;
}

STDMETHODIMP CExtract::ExtractTaggedBlock(BSTR sSrcFile, long lOffset, 
		                                  BSTR sTagName, BSTR sAttrName, 
                                          BSTR sAttrValue, BSTR sFileExt,
                                          BSTR * sExtractedFile, BOOL bCDataOnly)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    long lStart;
    long lLength;
    HRESULT hr;
    BOOL bIsReadFileOpen = FALSE;
    BOOL bIsWriteFileOpen = FALSE;
    CStdioFile InFile;
    CStdioFile OutFile;
    CString strTempFile;
    CString strExtractedFile;
    LPBYTE pByte = NULL;
    BSTR sDecodedText = NULL;
    BOOL bCDataRegExCompiled = FALSE;
    regex_t reCData;
    regmatch_t rmCData;


    hr = FindTaggedBlock(sSrcFile, lOffset, sTagName, sAttrName, sAttrValue, 
                         &lStart, &lLength);
    if (S_OK != hr)
    {
        return hr;
    }

    if (lLength < 0)
    {
        // empty element tag
        lLength = 0;
    }

    try
    {
        CString strSrcFile = sSrcFile;
        CString strPath;
        int iRegExError;
        CString strCDataRegEx;

        LPTSTR pszPath = strPath.GetBufferSetLength(MAX_PATH);
        if (!pszPath)
        {
            AfxThrowMemoryException();
        }


        LPTSTR pszTempFile = strTempFile.GetBufferSetLength(MAX_PATH);
        if (!pszTempFile)
        {
            strPath.ReleaseBuffer();
            AfxThrowMemoryException();
        }

        GetTempPath(MAX_PATH, pszPath);
        if (!GetTempFileName(pszPath, CEEX_TEMPFILE_PREFIX, 0, pszTempFile))
        {
//            TO_DEBUGPRINT(_T("CEEXTRACT> No temporary file created\n"));
            strTempFile.ReleaseBuffer();
            hr = E_FAIL;
            throw(hr);
        }

        // GetTempFileName created a file, we only need its name
        DeleteFile(pszTempFile);
        strPath.ReleaseBuffer();
        strTempFile.ReleaseBuffer();

        int iFileExtSep = strTempFile.ReverseFind(_T('.'));
        if (iFileExtSep < 0)
        {
//            TO_DEBUGPRINT(_T("CEEXTRACT> Temporary file has no extension\n"));
            hr = E_FAIL;
            throw(hr);
        }

        strTempFile = strTempFile.Left(iFileExtSep);
        strExtractedFile.Format(_T("%s.%S"), strTempFile, sFileExt);


//        if (!TO_NewMemory((void **)&pByte, lLength + 2))
        if (! (pByte = (LPBYTE)HeapAlloc(GetProcessHeap(),0,lLength + 2)))
//*lplpTmp = (LPBYTE)HeapAlloc(GetProcessHeap(),0,SizeNew);
        {
            AfxThrowMemoryException();
        }

        memset(pByte, 0, lLength + 2);

        iRegExError = regcomp(&reCData, CE_EXTRACT_CDATAREGEX, REG_EXTENDED);
        if (iRegExError)
        {
//            TO_DEBUGPRINT1(_T("CEEXTRACT> error %d compiling regular expression\n"), iRegExError);
            hr = E_FAIL;
            throw(hr);
        }

        bCDataRegExCompiled = TRUE;

        // open source file
        if (!InFile.Open(strSrcFile, 
                         CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary))
        {
            AfxThrowFileException(CFileException::fileNotFound, -1, strSrcFile);
        }

        bIsReadFileOpen = TRUE;

        // position file pointer in source file and read data
        InFile.Seek(lStart, CFile::begin);
        if (InFile.Read(pByte, lLength) != (unsigned long)lLength)
        {
//            TO_DEBUGPRINT2(_T("CEEXTRACT> Can't read %ld bytes from file %s\n"),
//                           lLength, strSrcFile);
            hr = E_FAIL;
            throw(hr);
        }

        // open target file
        if (!OutFile.Open(strExtractedFile, CFile::modeCreate | CFile::modeWrite | 
                     CFile::shareDenyWrite | CFile::typeBinary))
        {
            AfxThrowFileException(CFileException::fileNotFound, -1, strExtractedFile);
        }

        bIsWriteFileOpen = TRUE;

        // length may be 0 in case of an empty element tag.
        if (lLength > 0)
        {
            iRegExError = regexec(&reCData, (LPCTSTR)pByte, reCData.re_nsub + 1, &rmCData, 0);
            //##d-2126 beg: do decode only if bCDataOnly and if bCDataOnly==true only return 
            //decoded cdata section. This code still cannot handle multiple cdata sektions!
            if (!iRegExError && bCDataOnly)
            {
                int iStart = rmCData.rm_so + sizeof(CEEX_CDATA) - sizeof(TCHAR);
                int iEnd = rmCData.rm_eo - sizeof(CEEX_CDATA_END) + sizeof(TCHAR);

                CString strText = (LPCTSTR)(pByte + iStart);
                strText = strText.Left(iEnd - iStart / sizeof(TCHAR));

                CComBSTR sText(strText);
                hr = UTIL_XMLDecodeString(sText, &sDecodedText, XML_ENCODE_DEFAULT);
                if (FAILED(hr))
                {
                    throw(hr);
                }
        
                strText = sDecodedText;

                // write CDATA part to target file
                OutFile.Write(strText, strText.GetLength() * sizeof(TCHAR));
            }
            //##d-2126 end
            else
            {
                // no CDATA in block: write data to target file
                OutFile.Write(pByte, lLength);
            }
        }

        *sExtractedFile = strExtractedFile.AllocSysString();
    }

    catch (HRESULT hresult)
    {
//        TO_DEBUGPRINT(_T("CEEXTRACT> Decode error\n"));
        hr = hresult;
    }

    catch (CMemoryException *e)
    {
        e->Delete();
        hr = E_OUTOFMEMORY;
    }

    catch (CFileException *e)
    {
        switch (e->m_cause)
        {
            case CFileException::fileNotFound:   
//                TO_DEBUGPRINT1(_T("CEEXTRACT> File not found\n"), e->m_strFileName);
                break;

            default:
//                TO_DEBUGPRINT(_T("CEEXTRACT> File exception\n"));
                break;
        }
        e->Delete();
        hr = E_FAIL;
    }

    // free used resources

    if (NULL != pByte)
    {
//        TO_FreeMemory(pByte);
        HeapFree(GetProcessHeap(),0,pByte);
        pByte = NULL;
    }

    if (NULL != sDecodedText)
    {
        SysFreeString(sDecodedText);
        sDecodedText = NULL;
    }

    if (bCDataRegExCompiled)
    {
        regfree(&reCData);
    }

    if (bIsReadFileOpen)
    {
        InFile.Close();
    }


    if (bIsWriteFileOpen)
    {
        OutFile.Close();
        if (FAILED(hr))
        {
            DeleteFile(strExtractedFile);
        }
    }

	return hr;
}

STDMETHODIMP CExtract::MergeTaggedBlock(BSTR sSrcFile, long lOffset, 
								        BSTR sTagName, BSTR sAttrName, 
								        BSTR sAttrValue, BSTR sMergeFile,
								        BOOL bAsCData, BOOL bDelFile, 
                                        BOOL bRemoveXMLVersion)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    long lStart;
    long lLength;
    HRESULT hr;
    BOOL bIsReadFileOpen = FALSE;
    BOOL bIsMergeFileOpen = FALSE;
    BOOL bIsWriteFileOpen = FALSE;
    CStdioFile InFile;
    CStdioFile MergeFile;
    CStdioFile OutFile;
    CString strTempFile;
    LPBYTE pByte = NULL;
    CString strSrcFile;
    CString strMergeFile;
    regex_t reXmlVersion;
    regmatch_t rmXmlVersion;
    int iRegExError;
    BOOL bXMLVersionRegExCompiled = FALSE;

    hr = FindTaggedBlock(sSrcFile, lOffset, sTagName, sAttrName, sAttrValue, 
                         &lStart, &lLength);
    if (S_OK != hr)
    {
        return hr;
    }

    try
    {
        CString strPath;
        CString strCData;

        strSrcFile = sSrcFile;
        strMergeFile = sMergeFile;

        LPTSTR pszPath = strPath.GetBufferSetLength(MAX_PATH);
        if (!pszPath)
        {
            AfxThrowMemoryException();
        }


        LPTSTR pszTempFile = strTempFile.GetBufferSetLength(MAX_PATH);
        if (!pszTempFile)
        {
            strPath.ReleaseBuffer();
            AfxThrowMemoryException();
        }

        GetTempPath(MAX_PATH, pszPath);
        if (!GetTempFileName(pszPath, CEEX_TEMPFILE_PREFIX, 0, pszTempFile))
        {
//            TO_DEBUGPRINT(_T("CEEXTRACT> No temporary file created\n"));
            strTempFile.ReleaseBuffer();
            hr = E_FAIL;
            throw(hr);
        }

        // GetTempFileName created a file, we only need its name
        DeleteFile(pszTempFile);
        strPath.ReleaseBuffer();
        strTempFile.ReleaseBuffer();

//        if (!TO_NewMemory((void **)&pByte, CEEXTRACT_COPYBUF_SIZE + 2))  
        if (! (pByte = (LPBYTE)HeapAlloc(GetProcessHeap(),0,CEEXTRACT_COPYBUF_SIZE + 2)))
        {
            AfxThrowMemoryException();
        }

        // open source file
        if (!InFile.Open(strSrcFile, 
                         CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary))
        {
            AfxThrowFileException(CFileException::fileNotFound, -1, strSrcFile);
        }

        bIsReadFileOpen = TRUE;

        // open merge file
        if (!MergeFile.Open(strMergeFile, 
                         CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary))
        {
            AfxThrowFileException(CFileException::fileNotFound, -1, strMergeFile);
        }

        bIsMergeFileOpen = TRUE;

        // if merge file is empty nothing to do
        if (0 != MergeFile.GetLength() || bAsCData)
        {
            // open target file
            if (!OutFile.Open(strTempFile, CFile::modeCreate | CFile::modeWrite | 
                              CFile::shareDenyWrite | CFile::typeBinary))
            {
                AfxThrowFileException(CFileException::fileNotFound, -1, strTempFile);
            }

            bIsWriteFileOpen = TRUE;

            iRegExError = regcomp(&reXmlVersion, CE_EXTRACT_XML_VERSION, REG_EXTENDED);
            if (iRegExError)
            {
//                TO_DEBUGPRINT1(_T("CEEXTRACT> error %d compiling regular expression\n"), iRegExError);
                hr = E_FAIL;
                throw(hr);
            }

            bXMLVersionRegExCompiled = TRUE;

            long lCopied = 0;
            long lChunk = 0;
            UINT uiRead = 0;

            // copy beginning of source file to target file
            do
            {
                lChunk = min(CEEXTRACT_COPYBUF_SIZE, lStart - lCopied);
                if (lChunk > 0)
                {
                    uiRead = InFile.Read(pByte, lChunk);
                    if (uiRead > 0)
                    {
                        OutFile.Write(pByte, uiRead);
                        lCopied += uiRead;
                    }
                }
            }
            while (lChunk > 0 && 0 != uiRead);

            // if it is an empty element tag in source file
            // add a closing bracket
            if (lLength < 0)
            {
                CString strTemp = _T(">\r\n");
                OutFile.Write(strTemp, strTemp.GetLength() * sizeof(TCHAR));
            }

            // copy merge file to target file

            // append CDATA opening bracket if desired
            if (bAsCData)
            {
                strCData = CEEX_CDATA;
                OutFile.Write(strCData, strCData.GetLength() * sizeof(TCHAR));
            }

            do
            {
                uiRead = MergeFile.Read(pByte, CEEXTRACT_COPYBUF_SIZE);
                *(TCHAR *)(pByte + uiRead) = 0;

                if (bAsCData)
                {

                    CString strText = (LPCTSTR)pByte;
                    if (strText.GetLength() > 0)
                    {
                        CComBSTR sText = strText;
                        CComBSTR sEncodedText;

                        hr = UTIL_XMLEncodeString(sText, &sEncodedText, XML_ENCODE_DEFAULT);
                        if (FAILED(hr))
                        {
                            throw(hr);
                        }

                        strText = sEncodedText;

                        OutFile.Write(strText, 
                                      strText.GetLength() * sizeof(TCHAR));
                    }
                }
                else
                {
                    if (uiRead > 0)
                    {
                        if (bRemoveXMLVersion)
                        {
                            // check for XML Version tag
                            // this is usually located at the beginning of a file
                            // so we don't have to read the merge file in overlapped blocks
                            *(TCHAR *)(pByte + uiRead) = 0;
                            iRegExError = regexec(&reXmlVersion, (LPCTSTR)pByte, reXmlVersion.re_nsub + 1, &rmXmlVersion, 0);
                            if (!iRegExError)
                            {
                                int iStart = rmXmlVersion.rm_so;
                                int iEnd = rmXmlVersion.rm_eo;
                                memcpy(pByte + iStart, pByte + iEnd,
                                       uiRead - iEnd);
                                uiRead -= (iEnd - iStart);
                                *(TCHAR *)(pByte + uiRead) = 0;
                            }
                        }

                        OutFile.Write(pByte, uiRead);
                    }
                }

                lCopied += uiRead;
            }
            while (0 != uiRead);

            // append CDATA closing bracket if desired
            if (bAsCData)
            {
                strCData = CEEX_CDATA_END;
                OutFile.Write(strCData, strCData.GetLength() * sizeof(TCHAR));
            }


            // if it is an empty element tag in source file
            // add an element end tag /the '>' will be copied later
            // from the source file
            if (lLength < 0)
            {
                CString strTemp;
                strTemp.Format(_T("\r\n</%S"), sTagName);
                OutFile.Write(strTemp, strTemp.GetLength() * sizeof(TCHAR));
                lLength = 1;
            }

            InFile.Seek(lStart + lLength, CFile::begin);

            // copy rest of source file to target file
            do
            {
                uiRead = InFile.Read(pByte, CEEXTRACT_COPYBUF_SIZE);
                if (uiRead > 0)
                {
                    OutFile.Write(pByte, uiRead);
                    lCopied += uiRead;
                }
            }
            while (0 != uiRead);
        } 
    }

    catch (HRESULT hresult)
    {
//        TO_DEBUGPRINT(_T("CEEXTRACT> Decode error\n"));
        hr = hresult;
    }

    catch (CMemoryException *e)
    {
        e->Delete();
        hr = E_OUTOFMEMORY;
    }

    catch (CFileException *e)
    {
        switch (e->m_cause)
        {
            case CFileException::fileNotFound:   
//                TO_DEBUGPRINT1(_T("CEEXTRACT> File not found\n"), e->m_strFileName);
                break;

            default:
//                TO_DEBUGPRINT(_T("CEEXTRACT> File exception\n"));
                break;
        }
        e->Delete();
        hr = E_FAIL;
    }

    // free used resources

    if (NULL != pByte)
    {
//        TO_FreeMemory(pByte);
        HeapFree(GetProcessHeap(),0,pByte);
    }

    if (bIsReadFileOpen)
    {
        InFile.Close();
    }

    if (bIsMergeFileOpen)
    {
        MergeFile.Close();
    }

    if (bIsWriteFileOpen)
    {
        OutFile.Close();
        if (FAILED(hr))
        {
            DeleteFile(strTempFile);
        }
    }

    if (bXMLVersionRegExCompiled)
    {
        regfree(&reXmlVersion);
    }

    if (SUCCEEDED(hr) && bIsWriteFileOpen)
    {
        if (!CopyFile(strTempFile, strSrcFile, FALSE))
        {
            hr = E_FAIL;
        }

        DeleteFile(strTempFile);
    }

    if (SUCCEEDED(hr) && bDelFile)
    {
        DeleteFile(strMergeFile);
    }

	return hr;
}

STDMETHODIMP CExtract::GetXMLAttributeValue(BSTR sSrcFile, long lOffset, 
		                                    BSTR sTagName, 
                                            BSTR sKeyAttrName, BSTR sKeyAttrValue, 
                                            BSTR sAttrName,BSTR * sAttrValue,
                                            long  * plPos)
{
    long lStartPos;
    long lLength;
    HRESULT hr;
    BOOL bIsReadFileOpen = FALSE;
    CStdioFile InFile;
    LPBYTE pByte = NULL;
    regex_t reTag;
    regmatch_t * prmTag = FALSE;
    BOOL bTagRegExCompiled = FALSE;
    int iRegExError;
    CString strAttrValue;

    // search for begin tag
    hr = FindXMLTag(sSrcFile, lOffset, sTagName, sKeyAttrName, 
                    sKeyAttrValue, &lStartPos, &lLength, enumFindBeginTag);
    if (S_OK != hr)
    {
        return hr;
    }

    try
    {
        CString strSrcFile = sSrcFile;

//        if (!TO_NewMemory((void **)&pByte, lLength + 2))
        if (! (pByte = (LPBYTE)HeapAlloc(GetProcessHeap(),0,lLength + 2)))
        {
            AfxThrowMemoryException();
        }

        // open source file
        if (!InFile.Open(strSrcFile, 
                         CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary))
        {
            AfxThrowFileException(CFileException::fileNotFound, -1, strSrcFile);
        }

        bIsReadFileOpen = TRUE;

        // position file pointer in source file and read data
        InFile.Seek(lStartPos, CFile::begin);
        if (InFile.Read(pByte, lLength) != (unsigned long)lLength)
        {
//            TO_DEBUGPRINT2(_T("CEEXTRACT> Can't read %ld bytes from file %s\n"),
//                           lLength, strSrcFile);
            hr = E_FAIL;
            throw(hr);
        }

        *(TCHAR *)(pByte + lLength) = 0;

        CString strTagRegEx;

        // search for attribute and extract its value
        strTagRegEx.Format(CE_EXTRACT_ATTR, sAttrName);
        iRegExError = regcomp(&reTag, strTagRegEx, REG_EXTENDED);
        if (iRegExError)
        {
//            TO_DEBUGPRINT1(_T("CEEXTRACT> error %d compiling regular expression\n"), iRegExError);
            hr = E_FAIL;
            throw(hr);
        }

        bTagRegExCompiled = TRUE;

//        if (!TO_NewMemory((void **)&prmTag, sizeof(*prmTag) * (reTag.re_nsub + 1)))
        if (! (prmTag = (regmatch_t*)HeapAlloc(GetProcessHeap(),0,sizeof(*prmTag) * (reTag.re_nsub + 1))))
        {
            AfxThrowMemoryException();
        }

        iRegExError = regexec(&reTag, (LPCTSTR)pByte, reTag.re_nsub + 1, prmTag, 0);
        if (!iRegExError)
        {
            BSTR sEncodedAttrValue;
            pByte[prmTag[1].rm_eo] = 0;
            CString strAttrValue = &pByte[prmTag[1].rm_so];
            sEncodedAttrValue = strAttrValue.AllocSysString();
            hr = UTIL_XMLDecodeString(sEncodedAttrValue, sAttrValue, XML_ENCODE_DEFAULT);
            SysFreeString(sEncodedAttrValue);
            if (FAILED(hr))
            {
                throw(hr);
            }

            *plPos = lStartPos + prmTag[1].rm_so;
        }
        else
        {
            hr = S_FALSE;
        }
    }

    catch (HRESULT hresult)
    {
//        TO_DEBUGPRINT(_T("CEEXTRACT> Decode error\n"));
        hr = hresult;
    }

    catch (CMemoryException *e)
    {
        e->Delete();
        hr = E_OUTOFMEMORY;
    }

    catch (CFileException *e)
    {
        switch (e->m_cause)
        {
            case CFileException::fileNotFound:   
//                TO_DEBUGPRINT1(_T("CEEXTRACT> File not found\n"), e->m_strFileName);
                break;

            default:
//                TO_DEBUGPRINT(_T("CEEXTRACT> File exception\n"));
                break;
        }
        e->Delete();
        hr = E_FAIL;
    }

    // free used resources

    if (NULL != pByte)
    {
//        TO_FreeMemory(pByte);
        HeapFree(GetProcessHeap(),0,pByte);
        pByte = NULL;
    }

    if (NULL != prmTag)
    {
//        TO_FreeMemory(prmTag);
        HeapFree(GetProcessHeap(),0,prmTag);
        prmTag = NULL;
    }

    if (bTagRegExCompiled)
    {
        regfree(&reTag);
    }

    if (bIsReadFileOpen)
    {
        InFile.Close();
    }

    return hr;
}

STDMETHODIMP CExtract::SetXMLAttributeValue(BSTR sSrcFile, long lOffset, 
		                                    BSTR sTagName, 
                                            BSTR sKeyAttrName, BSTR sKeyAttrValue, 
                                            BSTR sAttrName,BSTR sAttrValue)
{
    long lPos;
    long lLength;
    HRESULT hr;
    BSTR sOldAttrValue;

    hr = GetXMLAttributeValue(sSrcFile, lOffset, sTagName, sKeyAttrName, 
                              sKeyAttrValue, sAttrName, &sOldAttrValue, &lPos);
    if (FAILED(hr))
    {
        return hr;
    }

    // attribute does not exist, so we have to add it
    // get position of key attribute value and calculate a position to
    // insert the attribute
    if (S_OK != hr)
    {
        hr = GetXMLAttributeValue(sSrcFile, lOffset, sTagName, sKeyAttrName, 
                                  sKeyAttrValue, sKeyAttrName, &sOldAttrValue, &lPos);

        if (S_OK != hr)
        {
            return hr;
        }

        lPos = lPos + wcslen(sOldAttrValue) + 1;
        lLength = 0;
    }
    else
    {
        lLength = wcslen(sOldAttrValue);
    }


    ::SysFreeString(sOldAttrValue);

    BOOL bIsReadFileOpen = FALSE;
    BOOL bIsWriteFileOpen = FALSE;
    CStdioFile InFile;
    CStdioFile OutFile;
    CString strTempFile;
    LPBYTE pByte = NULL;
    CString strSrcFile;

    try
    {
        CString strPath;
        CString strAttrValue;
        BSTR sEncodedAttrValue;

        hr = UTIL_XMLEncodeString(sAttrValue, &sEncodedAttrValue, XML_ENCODE_DEFAULT);
        if (FAILED(hr))
        {
            throw hr;
        }
        
        if (0 != lLength)
        {
            strAttrValue = sEncodedAttrValue;
        }
        else
        {
            // create new attribute
            strAttrValue.Format(_T(" %S = \"%S\""), sAttrName, sEncodedAttrValue);
        }

        SysFreeString(sEncodedAttrValue);

        strSrcFile = sSrcFile;

        LPTSTR pszPath = strPath.GetBufferSetLength(MAX_PATH);
        if (!pszPath)
        {
            AfxThrowMemoryException();
        }


        LPTSTR pszTempFile = strTempFile.GetBufferSetLength(MAX_PATH);
        if (!pszTempFile)
        {
            strPath.ReleaseBuffer();
            AfxThrowMemoryException();
        }

        GetTempPath(MAX_PATH, pszPath);
        if (!GetTempFileName(pszPath, CEEX_TEMPFILE_PREFIX, 0, pszTempFile))
        {
//            TO_DEBUGPRINT(_T("CEEXTRACT> No temporary file created\n"));
            strTempFile.ReleaseBuffer();
            hr = E_FAIL;
            throw(hr);
        }

        // GetTempFileName created a file, we only need its name
        DeleteFile(pszTempFile);
        strPath.ReleaseBuffer();
        strTempFile.ReleaseBuffer();

//        if (!TO_NewMemory((void **)&pByte, CEEXTRACT_COPYBUF_SIZE + 2))
        if (! (pByte = (LPBYTE)HeapAlloc(GetProcessHeap(),0,CEEXTRACT_COPYBUF_SIZE + 2)))
        {
            AfxThrowMemoryException();
        }

        // open source file
        if (!InFile.Open(strSrcFile, 
                         CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary))
        {
            AfxThrowFileException(CFileException::fileNotFound, -1, strSrcFile);
        }

        bIsReadFileOpen = TRUE;

        // open target file
        if (!OutFile.Open(strTempFile, CFile::modeCreate | CFile::modeWrite | 
                          CFile::shareDenyWrite | CFile::typeBinary))
        {
            AfxThrowFileException(CFileException::fileNotFound, -1, strTempFile);
        }

        bIsWriteFileOpen = TRUE;

        long lCopied = 0;
        long lChunk = 0;
        UINT uiRead = 0;

        // copy beginning of source file to target file
        do
        {
            lChunk = min(CEEXTRACT_COPYBUF_SIZE, lPos - lCopied);
            if (lChunk > 0)
            {
                uiRead = InFile.Read(pByte, lChunk);
                if (uiRead > 0)
                {
                    OutFile.Write(pByte, uiRead);
                    lCopied += uiRead;
                }
            }
        }
        while (lChunk > 0 && 0 != uiRead);

        // copy merge file to target file

        OutFile.Write(strAttrValue, strAttrValue.GetLength() * sizeof(TCHAR));

        // copy rest of source file to target file
        InFile.Seek(lLength, CFile::current);
        do
        {
            uiRead = InFile.Read(pByte, CEEXTRACT_COPYBUF_SIZE);
            if (uiRead > 0)
            {
                OutFile.Write(pByte, uiRead);
                lCopied += uiRead;
            }
        }
        while (0 != uiRead);
    } 

    catch (HRESULT hresult)
    {
//        TO_DEBUGPRINT(_T("CEEXTRACT> Decode error\n"));
        hr = hresult;
    }

    catch (CMemoryException *e)
    {
        e->Delete();
        hr = E_OUTOFMEMORY;
    }

    catch (CFileException *e)
    {
        switch (e->m_cause)
        {
            case CFileException::fileNotFound:   
//                TO_DEBUGPRINT1(_T("CEEXTRACT> File not found\n"), e->m_strFileName);
                break;

            default:
//                TO_DEBUGPRINT(_T("CEEXTRACT> File exception\n"));
                break;
        }
        e->Delete();
        hr = E_FAIL;
    }

    // free used resources

    if (NULL != pByte)
    {
//        TO_FreeMemory(pByte);
        HeapFree(GetProcessHeap(),0,pByte);
    }

    if (bIsReadFileOpen)
    {
        InFile.Close();
    }

    if (bIsWriteFileOpen)
    {
        OutFile.Close();
        if (FAILED(hr))
        {
            DeleteFile(strTempFile);
        }
    }

    if (SUCCEEDED(hr) && bIsWriteFileOpen)
    {
        if (!CopyFile(strTempFile, strSrcFile, FALSE))
        {
            hr = E_FAIL;
        }

        DeleteFile(strTempFile);
    }

    return hr;
}
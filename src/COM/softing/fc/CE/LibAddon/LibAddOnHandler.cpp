// LibAddOnHandler.cpp : Implementation of CLibAddOnHandler
#include "stdafx.h"
#include "LibAddon.h"
#include "LibAddOnHandler.h"

/////////////////////////////////////////////////////////////////////////////
// CLibAddOnHandler


STDMETHODIMP CLibAddOnHandler::doAction(BSTR sMethodName, BSTR sSourceFile, BSTR sId, ICEProjInfo *pICEProjInfo, BSTR sAdditionalInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

    try
    {
        CString strMethodName(sMethodName);
        CString strSourceFile(sSourceFile);
        CString strSourceId(sId);
        CString strAddInfo(sAdditionalInfo);

        if (strMethodName.Compare(DISP_ADDLIB)==0)
        {
            return AddLib(strSourceFile, strSourceId, pICEProjInfo, strAddInfo);
        }
        if (strMethodName.Compare(DISP_REMOVELIB)==0)
        {
            return RemoveLib(strSourceFile, strSourceId, pICEProjInfo, strAddInfo);
        }

	    return S_OK;
    }
    catch(...)
    {
        return E_FAIL;
    }
}



HRESULT CLibAddOnHandler::AddLib(CString strSourceFile, 
                                 CString strSourceId, 
                                 ICEProjInfo *pICEProjInfo, 
                                 CString strAddInfo)
{
    HRESULT hr;

    ASSERT(pICEProjInfo);
    if (!pICEProjInfo)
    {
        return E_FAIL;
    }

    // get libraries already added
    CList<CString, CString&> listLibs;
    hr = GetAllLibs(listLibs, pICEProjInfo);
    if (!SUCCEEDED(hr))
    {
        return hr;
    }

    // get standard lib path
    CString strStdLibPath;
    CComBSTR sStdLibPath;
    hr = pICEProjInfo->getLibraryPath(&sStdLibPath);
    if (hr==S_OK)
    {
        strStdLibPath = sStdLibPath;
    }
    
    // show dialog box
    CString strNewLib;
    CString strShortLib;

    CString strFilter;
    CString strTitle;
    CString strUrl;
    CStringArray    astrPresent;    //libraries that are already present in project

    strFilter.LoadString(IDS_FILE_FILTER);
    strTitle.LoadString(IDS_FILE_TITLE);
    CFileDialog tFileDlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT, strFilter, NULL);
    tFileDlg.m_ofn.lpstrTitle = strTitle;
    tFileDlg.m_ofn.lpstrInitialDir = strStdLibPath;
    tFileDlg.m_ofn.lpstrFile = new char[1024];
    tFileDlg.m_ofn.lpstrFile[0] = 0;
    tFileDlg.m_ofn.nMaxFile = 1024;
    if (tFileDlg.DoModal() == IDOK) 
    {
        POSITION posdlg = tFileDlg.GetStartPosition();
        while(posdlg)
        {
            strNewLib = tFileDlg.GetNextPathName(posdlg);
            // get short lib name
            CComBSTR sShortLib;
            CComBSTR sLib(strNewLib);
            strShortLib = strNewLib;

            hr = pICEProjInfo->GetFileRelative(sLib, &sShortLib);
            if (hr == S_OK)
            {
                strShortLib = sShortLib;
            }
            if(!IsLibPresent(strShortLib, listLibs, pICEProjInfo))
            {
                // add library
                CComBSTR sSection = LIB_CPLIB;
                CComBSTR sKey = CE_CP_FILE;
                CComBSTR sValue = strShortLib;

                hr = pICEProjInfo->CPInsertToSection(sSection, sKey, sValue);

                CComBSTR sEmpty = _T(""); // --> project file

                pICEProjInfo->fileChangedHint(sEmpty);
            }
            else
            {
                astrPresent.Add(strShortLib);
            }
        }
    }
    delete [] tFileDlg.m_ofn.lpstrFile;
    tFileDlg.m_ofn.lpstrFile = NULL;

    if(astrPresent.GetSize() > 0)
    {
        CString strTmp;
        CString strMessage;

        strTmp = astrPresent[0];
        for(int i = 1; i < astrPresent.GetSize(); ++i)
        {
            strTmp += _T("\n") + astrPresent[i];
        }
        strMessage.Format(IDS_LIBS_EXIST, strTmp);
        ::AfxMessageBox(strMessage, MB_OK|MB_ICONERROR);
    }
    return S_OK;
}


//------------------------------------------------------------------*
/**
 * is library present.
 *
 * @param           [in] crstrShortLib: short name of library
 * @param           [in] listLibs: list of library names
 * @param           [in] pICEProjInfo: project info interface pointer
 * @return          is library present?
 * @exception       -
 * @see             -
*/
BOOL CLibAddOnHandler::IsLibPresent(const CString& crstrShortLib, CList<CString, CString&>& listLibs, ICEProjInfo* pICEProjInfo)
{
    HRESULT hr;
    // check if lib is already added
    POSITION pos = listLibs.GetHeadPosition();
    
    while (pos)
    {
        CString strOldLib = listLibs.GetNext(pos);
        CString strShortOldLib;
        CComBSTR sShortOldLib;
        CComBSTR sOldLib = strOldLib;

        hr = pICEProjInfo->GetFileRelative(sOldLib, &sShortOldLib);
        if (hr==S_OK)
        {
            strShortOldLib = sShortOldLib;
            if (crstrShortLib.CompareNoCase(strShortOldLib)==0)
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}



HRESULT CLibAddOnHandler::RemoveLib(CString strSourceFile, 
                                    CString strSourceId, 
                                    ICEProjInfo *pICEProjInfo, 
                                    CString strAddInfo)
{
    HRESULT hr;

    ASSERT(pICEProjInfo);
    if (!pICEProjInfo)
    {
        return E_FAIL;
    }

    // get libraries already added
    CList<CString, CString&> listLibs;
    hr = GetAllLibs(listLibs, pICEProjInfo);
    if (hr!=S_OK)
    {
        return hr;
    }

    // find library
    CString strShortLib = strSourceFile;
    CComBSTR sShortLib;
    CComBSTR sLib = strSourceFile;

    hr = pICEProjInfo->GetFileRelative(sLib, &sShortLib);

    if (hr == S_OK)
    {
        strShortLib = sShortLib;
    }

    POSITION pos = listLibs.GetHeadPosition();
    while(pos)
    {
        CString strOldLib = listLibs.GetNext(pos);
        CString strShortOldLib;
        CComBSTR sShortOldLib;
        CComBSTR sOldLib = strOldLib;

        hr = pICEProjInfo->GetFileRelative(sOldLib, &sShortOldLib);
        if (hr==S_OK)
        {
            strShortOldLib = sShortOldLib;
            if (strShortLib.CompareNoCase(strShortOldLib)==0)
            {
                // delete from 4cp file
                CComBSTR sSection = LIB_CPLIB;
                CComBSTR sKey = CE_CP_FILE;
                CComBSTR sValue = strOldLib;

                hr = pICEProjInfo->CPDeleteKeyWithValueFromSection(sSection, sKey, sValue);

                CComBSTR sEmpty = _T(""); // --> project file

                pICEProjInfo->fileChangedHint(sEmpty);

                return S_OK;
            }
        }
    }

    return S_OK;
}


HRESULT CLibAddOnHandler::GetAllLibs(CList<CString, CString&>& listLibs, ICEProjInfo *pICEProjInfo)
{
    SAFEARRAY* parrFileNames;
    CComBSTR sSection = LIB_CPLIB;
    CComBSTR sKey     = CE_CP_FILE;

    HRESULT hr;

    hr = pICEProjInfo->CPGetAllValuesForKeyFromSection(sSection, sKey, &parrFileNames);
    if (hr!=S_OK)
    {
        return hr;
    }

    BSTR HUGEP *pbstr;
    BSTR bstr;
    unsigned long i;

    // Get a pointer to the elements of the array.
    hr = ::SafeArrayAccessData(parrFileNames, (void HUGEP**)&pbstr);
    if (FAILED(hr))
    {
        return hr;
    }
    for (i = 0; i < ((parrFileNames)->rgsabound[0]).cElements; i++)
    {
        // returned have been id paths: 
        // the last part of the id paths is the object name
        CString str;
        bstr = pbstr[i];
        str = (CString)bstr;
        listLibs.AddTail(str);
    }

    ::SafeArrayUnaccessData(parrFileNames);
    ::SafeArrayDestroy(parrFileNames);

    return S_OK;
}



STDMETHODIMP CLibAddOnHandler::doBackparse(BSTR sFileName, ICEProjInfo * pICEProjInfo, BSTR * psXmlText)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    HRESULT hr;

	if (psXmlText == NULL)
    {
		return E_POINTER;
    }
    *psXmlText = NULL;

    CString strFileName(sFileName);
    CString strBPOut;

    CXMLDocument doc;
    CXMLNode rootNode;
    BOOL ret;

    ret = doc.CreateNode(rootNode, CE_XMLTAG_NODE);
    ASSERT(ret);
    if (!ret)
    {
        return E_FAIL;
    }

    ASSERT(pICEProjInfo);

    // set std attributes for node
    // SOURCE

    CString strShortFileName = strFileName;

    if (pICEProjInfo)
    {   
        CComBSTR sFileName(strFileName);
        CComBSTR sRelFileName;
        hr = pICEProjInfo->GetFileRelative(sFileName, &sRelFileName);
        if (hr==S_OK)
        {
            strShortFileName = sRelFileName;
        }
    }
    
    rootNode.SetAttribute(CE_XMLATTR_SOURCE, strShortFileName);

    // TEXT attribute
    // get name of library only: cut file ext and path
    CString outText;
    outText = strFileName;
    int i = outText.ReverseFind(_T('.'));
    if (i>0)
    {
        outText = outText.Left(i);
    }
    i = outText.ReverseFind(_T('\\'));
    if (i>0)
    {
        ASSERT(outText.GetLength()>i+1);
        outText = outText.Mid(i+1);
    }
    i = outText.ReverseFind(_T('/'));
    if (i>0)
    {
        ASSERT(outText.GetLength()>i+1);
        outText = outText.Mid(i+1);
    }
    
    rootNode.SetAttribute(CE_XMLATTR_TEXT, outText);
    rootNode.SetAttribute(CE_XMLATTR_ID, outText);
    rootNode.SetAttribute(CE_XMLATTR_TYPE, LIB_XMLTYPE);

    CFileStatus fileStatus;
    CString strFullFileName;
    CComBSTR sFullFileName;
    CString m_strCreateDate;
    CString m_strLastModDate;

    if (pICEProjInfo)
    {
        pICEProjInfo->GetFileAbsolute(sFileName, &sFullFileName);

        strFullFileName = sFullFileName;

        // does library file exist?
        if (!CFile::GetStatus(strFullFileName, fileStatus))
        {
            CString strError;
            strError.LoadString(IDS_ERR_FILE_NOT_EXIST);
            outText = outText + strError;
            rootNode.SetAttribute(CE_XMLATTR_TEXT, outText);
        }
        else
        {
            CTime ctime = fileStatus.m_ctime;
            CTime mtime = fileStatus.m_mtime;

            // get create and modified date
            m_strCreateDate.Format(_T("DT#%04d-%02d-%02d-%02d:%02d:%02d"),
                                    ctime.GetYear(),
                                    ctime.GetMonth(),
                                    ctime.GetDay(),
                                    ctime.GetHour(),
                                    ctime.GetMinute(),
                                    ctime.GetSecond());
            m_strLastModDate.Format(_T("DT#%04d-%02d-%02d-%02d:%02d:%02d"),
                                    mtime.GetYear(),
                                    mtime.GetMonth(),
                                    mtime.GetDay(),
                                    mtime.GetHour(),
                                    mtime.GetMinute(),
                                    mtime.GetSecond());
        }
    }
  
    // CREATE/MODIFIED
    if (!m_strCreateDate.IsEmpty())
    {
        rootNode.SetAttribute(CE_XMLATTR_CREATED, m_strCreateDate);
    }
  
    if (!m_strLastModDate.IsEmpty())
    {
        rootNode.SetAttribute(CE_XMLATTR_MODIFIED, m_strLastModDate);
    }
   
  

    CXMLNode docNode;
    doc.GetDocNode(docNode);
    docNode.AppendChild(rootNode);

    doc.GetXMLString(strBPOut);

    *psXmlText = strBPOut.AllocSysString();
		
	return S_OK;
}


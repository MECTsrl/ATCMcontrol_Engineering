
#include "stdafx.h"
#include "Operation.h"
#include "CEProjInfo_i.c"
#include "CEContainer.h"
#include "resource.h"
#include "utilif.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define STRSYSCMTEMPTY _T("(*[[\n]]*)\n")


COperation::COperation(CPropertySheetBase *Dialog,
                       BSTR sSourceFile,
                       BSTR sId,
                       ICEProjInfo *pICEProjectInfo,
                       BSTR sAdditionalInfo)
{

    if(CString(sAdditionalInfo).Compare(_T("SILENT")) == 0)
    {
        m_bSilent = TRUE;
    }
    else
    {
        m_bSilent = FALSE;
    }

    m_pDialog = Dialog;
    m_sSourceFile = sSourceFile;
    m_sId = sId;
    m_sAdditionalInfo = sAdditionalInfo;
    m_pMembers = NULL;
    m_aVarList.RemoveAll();

    m_sAlias = _T("");
    m_sHelpUrl = _T("");
    m_sUrl = _T("");
    for (int i = 0; i < 3; i++)
    {
        m_sUsrText[i] = _T("");
    }
    m_sObjGUID = _T("");

    HRESULT hRes = 0;

    m_pICEProjectInfo = pICEProjectInfo;
    m_pICEProjectInfo->AddRef();
    hRes = m_pICEProjectInfo->getProjectPath(&m_ProjectPath);
    ASSERT(SUCCEEDED(hRes));

    m_bSourceReadOnly = FALSE;
    m_bAutomationMode = FALSE;

    CComBSTR    sProjPath;
    HRESULT     hr;

    hr = pICEProjectInfo->getProjectPath(&sProjPath);
    ASSERT(hr == S_OK);

    if(hr == S_OK)
    {
        CString     strSourceFilePathName = GetAbsPath((CString)m_sSourceFile, (CString)sProjPath);
        CFileStatus tStatus;
        if(CFile::GetStatus(strSourceFilePathName, tStatus))
        {
            m_bSourceReadOnly = (tStatus.m_attribute & CFile::readOnly) ? TRUE : FALSE;
        }
    }
}


COperation::~COperation()
{
    m_pICEProjectInfo->Release();
    if(m_pMembers)
    {
        ::SafeArrayDestroy(m_pMembers);
        m_pMembers = NULL;
    }
}


void COperation::SetAutomationMode(BOOL bMode)
{
    m_bAutomationMode = bMode;
}


//------------------------------------------------------------------*
/**
 * get abs path.
 *
 * @param           crstrFileString: file name resp file name incl relative or absolute path
 * @return          file name incl absolute path.
 * @exception       -
 * @see             -
*/
CString COperation::GetAbsPath(const CString& crstrFileString, const CString& crstrProjPath)
{
    HRESULT hr = UTIL_IsAbsolutePath(crstrFileString);
    if(hr == S_OK)
    {
        return crstrFileString;
    }

    CString strFilePathName;
    strFilePathName.Format("%s\\%s", crstrProjPath, crstrFileString);

    return strFilePathName;
}

//------------------------------------------------------------------*
/**
 * Function name			COperation::Execute
 * Description		Called after the user clicks on the OK button of a dialog	
 * @return			BOOL: TRUE if succesful else FALSE
 * @exception			
 * @see			
*/
BOOL COperation::Execute()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * Function name			COperation::GetProjInfo
 * Description			
 * @return			ICEProjInfo*
 * @exception			
 * @see			
*/
ICEProjInfo* COperation::GetProjInfo()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    return m_pICEProjectInfo;
}

//------------------------------------------------------------------*
/**
 * Function name	COperation::WriteHeader
 * Description			
 * @param			CString sPouText
 * @return			void 
 * @exception			
 * @see			
*/
void COperation::WriteHeader(CString sPouText)
{
    if (m_FileType == FILETYPE_ST)
    {
        WriteFile(sPouText);
    }
    else
    {
        MergeHeaderIntoFile(sPouText);
    }
}

//------------------------------------------------------------------*
/**
 * Function name	COperation::MergeHeader
 * Description			
 * @param			CString sPouText
 * @return			void 
 * @exception			
 * @see			
*/
void COperation::MergeHeaderIntoFile(CString sPouText)
{
    wchar_t *hfn; 
    wchar_t *dir = L"."; 
    wchar_t *prefix = L"CE"; 
    hfn = _wtempnam(dir, prefix);
    CString HeaderFileName;
    if (hfn != NULL)
    {
        HeaderFileName = hfn;
        delete hfn;
    }
    else
    {
        HeaderFileName = _T("TemporaryHeaderFile");
    }

    CString strFullFileName = m_ProjectPath;
    strFullFileName += _T("\\") + m_sFileName;

    try
    {
        CStdioFile File(HeaderFileName,
                   CFile::modeWrite | CFile::modeCreate);
        File.Write(sPouText, sPouText.GetLength());
        File.Close();
        MergeHeader(string(strFullFileName), string(HeaderFileName), TRUE);
    }
    catch(CFileException* pFileException)
    {
        pFileException->ReportError();
        pFileException->Delete();
    }   
}

//------------------------------------------------------------------*
/**
 * Function name	COperation::WriteFile
 * Description		Writes the contents of the modified string in the given file	
 * @param			CString sPouText
 * @return			void 
 * @exception			
 * @see			
*/
void COperation::WriteFile(CString sPouText)
{
    try
    {
        CStdioFile File((CString)m_ProjectPath + _T("\\") + m_sFileName,
                   CFile::modeWrite | CFile::modeCreate);
        File.Write(sPouText, sPouText.GetLength());
        File.Close();
    }
    catch(CFileException* pFileException)
    {
        pFileException->ReportError();
        pFileException->Delete();
    }   
}


//------------------------------------------------------------------*
/**
 * Function name			COperation::DoesFileExist
 * Description		Tests if the file already exists. If it's the case
	                the user will be warned, otherwise the return value
                    is FALSE.
 * @param			CString strFileName
 * @return			BOOL 
 * @exception			
 * @see			
*/
BOOL COperation::DoesFileExist(CString strName, BOOL bWithExt)
{
    if(m_bSilent)
    {
        return FALSE;
    }

    CString strFileName;
    CFileStatus fileStatus;
    if(bWithExt)
    {
        if (CFile::GetStatus((CString)m_ProjectPath + _T("\\") + strName, fileStatus))
        {
            // file does already exist
            CString errorMsg;
            //errorMsg.Format(IDS_OVERWRITE_FILE, strName);
            //int Res = AfxMessageBox(errorMsg, MB_YESNO|MB_ICONWARNING);
            //if(Res == IDYES)
            //{
            //    //Remove the old file
            //    CFile::Remove((CString)m_ProjectPath + _T("\\") + strName);
            //    return FALSE; 
            //}
            //else
            //{
            //    return TRUE;
            //}
            errorMsg.Format(IDS_FILE_ALREADY_EXISTS, strName);
            AfxMessageBox(errorMsg, MB_OK|MB_ICONWARNING);
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        for(int iIndex = 0; iIndex < 3; iIndex++)
        {
            if(iIndex == 0)
            {
                strFileName = strName + _T(".st");
            }
            else if(iIndex == 1)
            {
                strFileName = strName + _T(".4gr");
            }
            else if(iIndex == 2)
            {
                strFileName = strName + _T(".sdt");
            }
            
            // test if file already exists
            if (CFile::GetStatus((CString)m_ProjectPath + _T("\\") + strFileName, fileStatus))
            {
                // file does already exist
                CString errorMsg;
                //errorMsg.Format(IDS_OVERWRITE_FILE, strName);
                //int Res = ::AfxMessageBox(errorMsg, MB_YESNO|MB_ICONWARNING);
                //if(Res == IDYES)
                //{
                    //Remove the old file
                //    CFile::Remove((CString)m_ProjectPath + _T("\\") + strFileName);
                //    return FALSE; 
                //}
                //else
                //{
                //    return TRUE;
                //}
                errorMsg.Format(IDS_FILE_ALREADY_EXISTS, strName);
                AfxMessageBox(errorMsg, MB_OK|MB_ICONWARNING);
                return TRUE;
            }
        }
    }
    return FALSE;
}

//------------------------------------------------------------------*
/**
 * Function name	COperation::SetFileName
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void COperation::SetFileName()
{
    m_sFileName = _T(" ");
}


//------------------------------------------------------------------*
/**
 * Function name	COperation::FillDialog
 * Description		Fills the dialog with the varaible declarations	
 * @return			void 
 * @exception			
 * @see			    CVar
*/
void COperation::FillDialog()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    //create the safe array for the CVar to write them in the dialogs
	SAFEARRAYBOUND bound[2];
    HRESULT hr = S_OK;
	long arrIndex[2];
    
    bound[0].lLbound = 0;
    bound[0].cElements = m_aVarList.GetCount(); 
    bound[1].lLbound = 0;
    bound[1].cElements = 9; //OH: was 5, now + IP and Hint and Range  
                            //SN: was 8, now + VarComment
    
    //create the safe array for the CVar list m_aVarList
    m_pMembers = ::SafeArrayCreate(VT_BSTR , 2, bound);
    if (!m_pMembers)
    {
        return;
    }

    EDGE_DETECT_TYP eEdgeTyp;
    POSITION pos = m_aVarList.GetHeadPosition();
    for (long i=bound[0].lLbound; i < bound[0].cElements; i++) 
    {
        arrIndex[0] = (bound[0].cElements - i - 1);//Convert the order of the variables
        //copy from the CVar array into a safe array
        CVar var1(m_aVarList.GetNext(pos));
        CString strTmp;
        eEdgeTyp = var1.GetEdge();
        switch(eEdgeTyp)
        {
        case TYP_EDGE_F_EDGE:
            strTmp.Format("%s F_EDGE", var1.GetVarType());
            break;
        case TYP_EDGE_R_EDGE:
            strTmp.Format("%s R_EDGE", var1.GetVarType());
            break;
        default:
            strTmp.Format("%s", var1.GetVarType());
            break;
        }
        CArray<CString, CString> astrDummy;//for converting CVar -> Element of SAFEARRAY
        astrDummy.Add(var1.GetVarStorageClass());
        astrDummy.Add(var1.GetObjName());
        astrDummy.Add(var1.GetObjAlias());
        astrDummy.Add(strTmp);
        astrDummy.Add(var1.GetVarInitVal());
        astrDummy.Add(var1.GetObjVarComment());
        strTmp.Format("%d", var1.GetIP());
        astrDummy.Add(strTmp);
        strTmp.Format("%d", var1.GetVarDeclRange());
        astrDummy.Add(strTmp);
        astrDummy.Add(var1.GetHint());
        for (long k = bound[1].lLbound; k < bound[1].cElements; k++)
        {
            arrIndex[1] = k;
            CString strDummy = astrDummy[k];
            BSTR bstrDummy = strDummy.AllocSysString();
            hr = ::SafeArrayPutElement(m_pMembers, arrIndex, (void*)bstrDummy);
            SysFreeString(bstrDummy);
            if(FAILED(hr)) 
            {
                return;
            }
        }
    }
}


//------------------------------------------------------------------*
/**
 * Function name			COperation::InformProjectManager
 * Description		Imforms the project manager about changes in the 4cp file
	                and other files
 * @return			void 
 * @exception			
 * @see			
*/
void COperation::InformProjectManager()
{
    HRESULT hRes = 0;
    BSTR strFileName;
//    BSTR sId;
    CString strHelp;
//    int iPos;

    ICEContainer *pICEContainer = NULL;
    hRes = m_pICEProjectInfo->getContainerIFforFile(m_sSourceFile,
                                                    &pICEContainer);
   	ASSERT(hRes==S_OK);
    
    //Construct the ID
//    strHelp = m_sId;
//    strHelp = strHelp + _T("/") + m_sFileName;
//    iPos = strHelp.ReverseFind('.');
//    strHelp = strHelp.Left(iPos);
//    sId = strHelp.AllocSysString();
//    //Look for the link
//    hRes = m_pICEProjectInfo->getFileNameFromIdPath(sId, &strFileName);
//    if(hRes == S_OK)
//    {
//        strHelp = strFileName;
//        if(!strHelp.IsEmpty()) 
//        {//if there is one delete it
//            hRes = pICEContainer->deleteLink(m_sSourceFile, m_sId, strFileName);
//            ASSERT(hRes == S_OK);
//        }
//    }

    // SIS: leave it to project manager where to add node
    m_sId.Empty();
    //Create the link
    strFileName = m_sFileName.AllocSysString();
    hRes = pICEContainer->createLink(m_sSourceFile,     
                                     m_sId, 
                                     strFileName);
    SysFreeString(strFileName);
//    SysFreeString(sId);

    if(!m_sHelpUrl.IsEmpty())
    {
        CreateUsrHelpLink();
    }

    if(!m_bSilent)
    {
        m_pICEProjectInfo->fileParseNow(NULL);
    }
    
    //ASSERT(hRes==S_OK);

    pICEContainer->Release();
}


//------------------------------------------------------------------*
/**
 * Function name			COperation::SetGUID
 * Description			
 * @param			CString &StrGUID
 * @return			void 
 * @exception			
 * @see			
*/
void COperation::SetGUID(CString &StrGUID) 
{
    m_sObjGUID = StrGUID;
    return;
}

//------------------------------------------------------------------*
/**
 * Function name	COperation::CreateGUID
 * Description		creates the GUID	
 * @return			void 
 * @exception			
 * @see			StringFromGUID2, CoCreateGuid
*/
void COperation::CreateGUID() 
{
    GUID Guid;
    OLECHAR GuidString[CEOBJ_GUIDLEN];
    HRESULT hr;
    hr = CoCreateGuid(&Guid);
    ASSERT(SUCCEEDED(hr));
    if (!SUCCEEDED(hr))
    {
        m_sObjGUID = _T("");
    }
    else
    {
        StringFromGUID2(Guid, GuidString, sizeof(GuidString));
        m_sObjGUID = GuidString;
    }
    return;
}


//------------------------------------------------------------------*
/**
 * Function name			COperation::GetGUID
 * Description			
 * @return			CString & 
 * @exception			
 * @see			
*/
CString & COperation::GetGUID() 
{
    return(m_sObjGUID);
}

//------------------------------------------------------------------*
/**
 * Function name			COperation::AddQuotation
 * Description			
 * @param			LPCTSTR pszText
 * @param			CString & strQuotedText
 * @return			void 
 * @exception			
 * @see			
*/
void COperation::AddQuotation(LPCTSTR pszText, CString & strQuotedText)
{
    strQuotedText = pszText;
    if (strQuotedText.GetLength() == 1 || strQuotedText.Left(1) != _T("\"") || 
        strQuotedText.Right(1) != _T("\""))
    {
        strQuotedText = _T("\"") + strQuotedText;
        strQuotedText += _T("\"");
    }
}

//------------------------------------------------------------------*
/**
 * Function name	COperation::AppendCommentBegin
 * Description		Adds the comment begin to system comment	
 * @param			CString & strText
 * @param			int iTabs
 * @return			void 
 * @exception			
 * @see			
*/
void COperation::AppendCommentBegin(CString & strText, int iTabs)
{
    CString StrHelp;
    CString StrOut(_T(""));
    CString Strlf(NEWLINE);
    CString StrTab(SPACETAB);
    CString StrIndent(_T(""));
    int i;

    for (i=0; i < iTabs; i++)
    {
        StrIndent += StrTab;
    }

    StrOut += StrIndent;
    StrHelp.LoadString(IDS_KEY_COMMENT);
    StrOut += StrHelp;
    StrHelp.LoadString(IDS_KEY_SYSCOMMENT);
    StrOut += StrHelp;
    StrOut += Strlf;

    strText += StrOut;
}

//------------------------------------------------------------------*
/**
 * Function name	COperation::AppendCommentEnd
 * Description		Adds the comment end to system comment	
 * @param			CString & strText
 * @param			int iTabs
 * @return			void 
 * @exception			
 * @see			
*/
void COperation::AppendCommentEnd(CString & strText, int iTabs)
{
    CString StrHelp,StrOut(_T("")),Strlf(NEWLINE),StrTab(SPACETAB),StrIndent(_T(""));
    int i;

    for (i=0; i < iTabs; i++)
    {
        StrIndent += StrTab;
    }

    StrOut += StrIndent;
    StrHelp.LoadString(IDS_KEY_ENDSYSCOMMENT);
    StrOut += StrHelp;
    StrHelp.LoadString(IDS_KEY_ENDCOMMENT);
    StrOut += StrHelp;
    //StrOut += Strlf;      // 25.08.06 SIS

    strText += StrOut;
}

//------------------------------------------------------------------*
/**
 * Function name	COperation::AppendGuid
 * Description		Adds the guid to system comment	
 * @param			CString & strText
 * @param			int iTabs
 * @return			void 
 * @exception			
 * @see			
*/
void COperation::AppendGuid(CString & strText, int iTabs)
{
    CString StrHelp,StrOut(_T("")),Strlf(NEWLINE),StrTab(SPACETAB),StrIndent(_T(""));
    CString Strco, strGuid = _T("");
    int i;
    
    for (i=0; i < iTabs; i++)
    {
        StrIndent += StrTab;
    }
    if(!m_sObjGUID.IsEmpty())
    {
        
        StrOut += StrIndent;
        StrHelp.Format(IDS_KEY_GUID, m_sObjGUID);
        StrOut += StrHelp;
        Strco.LoadString(IDS_KEY_COMMA);
        StrOut += Strco;
        StrOut += Strlf;
        
        strText += StrOut;
    }
}

//------------------------------------------------------------------*
/**
 * Function name	COperation::AppendUsrText
 * Description		Adds the usertext to system comment	
 * @param			CString & strText
 * @param			int iTabs
 * @param			bool bComma
 * @return			void 
 * @exception			
 * @see			
*/
void COperation::AppendUsrText(CString & strText, int iTabs, bool bComma)
{
    CString StrHelp,StrOut(_T("")),Strlf(NEWLINE),StrTab(SPACETAB),StrIndent(_T(""));
    CString Strco, strQuote;
    int nErr = 0;
    int i;

    for (i=0; i < iTabs; i++)
    {
        StrIndent += StrTab;
    }

    Strco.LoadString(IDS_KEY_COMMA);
    
    for (i=1; i <= CE_NUMUSRTXT; i++)
    {
        if (!m_sUsrText[i-1].IsEmpty())
        {
            if (!StrOut.IsEmpty())
            {
                if(bComma) {
                    StrOut += Strco;
                }

                StrOut += Strlf;
            }

            StrOut += StrIndent;
            AddQuotation(m_sUsrText[i-1], strQuote);
            StrHelp.Format(IDS_KEY_USERTEXT, i, strQuote);
            StrOut += StrHelp;
        }
    }

    if (!StrOut.IsEmpty())
    {
        if(bComma) {
            StrOut += Strco;
        }
        StrOut += Strlf;
    }

    strText += StrOut;
}

//------------------------------------------------------------------*
/**
 * Function name	COperation::AppendAlias
 * Description		Adds the alias to system comment	
 * @param			CString & strText
 * @param			int iTabs
 * @return			void 
 * @exception			
 * @see			
*/
void COperation::AppendAlias(CString & strText, int iTabs)
{
    CString StrHelp,StrOut(_T("")),Strlf(NEWLINE),StrTab(SPACETAB),StrIndent(_T(""));
    CString Strco, strGuid = _T(""), strQuote;
    int i;

    for (i=0; i < iTabs; i++)
    {
        StrIndent += StrTab;
    }

    Strco.LoadString(IDS_KEY_COMMA);

    if (!m_sAlias.IsEmpty())
    {
        StrOut += StrIndent;
        AddQuotation(m_sAlias, strQuote);
        StrHelp.Format(IDS_KEY_ALIAS, strQuote);
        StrOut += StrHelp;
        StrOut += Strco;
        StrOut += Strlf;

        strText += StrOut;
    }
}

//------------------------------------------------------------------*
/**
 * Function name	COperation::AppendUsrURL
 * Description		Adds the userurl to system comment	
 * @param			CString & strText
 * @param			int iTabs
 * @return			void 
 * @exception			
 * @see			
*/
void COperation::AppendUsrURL(CString & strText, int iTabs)
{
    CString StrHelp,StrOut(_T("")),Strlf(NEWLINE),StrTab(SPACETAB),StrIndent(_T(""));
    CString Strco, strGuid = _T(""), strQuote;
    int i;

    for (i=0; i < iTabs; i++)
    {
        StrIndent += StrTab;
    }

    Strco.LoadString(IDS_KEY_COMMA);

    if (!m_sUrl.IsEmpty())
    {
        StrOut += StrIndent;
        AddQuotation(m_sUrl, strQuote);
        StrHelp.Format(IDS_KEY_USRURL, strQuote);
        StrOut += StrHelp;
        StrOut += Strco;
        StrOut += Strlf;

        strText += StrOut;
    }
}

//------------------------------------------------------------------*
/**
 * Function name			COperation::AppendHelpURL
 * Description		Adds the helpurl to system comment	
 * @param			CString & strText
 * @param			int iTabs
 * @return			void 
 * @exception			
 * @see			
*/
void COperation::AppendHelpURL(CString & strText, int iTabs)
{
    CString StrHelp,StrOut(_T("")),Strlf(NEWLINE),StrTab(SPACETAB),StrIndent(_T(""));
    CString Strco, strGuid = _T(""), strQuote;
    int i;

    for (i=0; i < iTabs; i++)
    {
        StrIndent += StrTab;
    }

    Strco.LoadString(IDS_KEY_COMMA);

    if (!m_sHelpUrl.IsEmpty())
    {
        StrOut += StrIndent;
        AddQuotation(m_sHelpUrl, strQuote);
        StrHelp.Format(IDS_KEY_HLPURL, strQuote);
        StrOut += StrHelp;
        StrOut += Strco;
        StrOut += Strlf;

        strText += StrOut;
        
        if(!m_sHelpTemplate.IsEmpty())
        {
            CreateUsrHelp();
        }
    }
}


//------------------------------------------------------------------*
/**
 * Appends attribute - key and value to system comment.
 * @param			CString & strOut - system comment to modify
 * @param			string & key - key of attribute
 * @param			string & value -  value of attribute
 * @param			int iTabs - no of tabs for indenting
 * @exception		CMemoryException
*/
void COperation::AppendAttrib(CString & strOut, string & key, string & value, int iTabs)
{
    CString strIndent,strComma,strQuote,strlf(NEWLINE),strTab(SPACETAB);
    int i;
    

    for (i=0; i < iTabs; i++)
    {
        strIndent += strTab;
    }

    strComma.LoadString(IDS_KEY_COMMA);
    AddQuotation(value.c_str(), strQuote);
    strOut += strIndent;
    strOut += key.c_str();
    strOut += _T("= ");
    strOut += strQuote;
    strOut += strComma;
    strOut += strlf;

}

//------------------------------------------------------------------*
/**
 * Function name	COperation::GetStdAttribString
 * Description		Builds the system commnent string	
 * @param			CString & strText: text to be abdated
 * @param			int iTabs: tabs count to insert in the beginning of each line
 * @return			void 
 * @exception			
 * @see			AppendCommentBegin, AppendGuid, AppendUsrText, AppendUsrURL, AppendHelpURL
                KillLastComma, AppendCommentEnd
*/
void COperation::GetStdAttribString(CString & strText, int iTabs)
{
    CString Strlf = NEWLINE;
    
    if((m_sObjGUID.IsEmpty()) && (m_sUsrText[0].IsEmpty()) && (m_sUsrText[1].IsEmpty())
        && (m_sUsrText[2].IsEmpty()) && (m_sAlias.IsEmpty()) && (m_sUrl.IsEmpty()) && (m_sHelpUrl.IsEmpty()))

    {
        strText += STRSYSCMTEMPTY;    
    }
    else
    {
        AppendCommentBegin(strText, (iTabs - 1));
        AppendGuid(strText, iTabs);
        AppendAlias(strText, iTabs);
        AppendUsrText(strText, iTabs);
        AppendUsrURL(strText, iTabs);
        AppendHelpURL(strText, iTabs);
        KillLastComma(strText);
        AppendCommentEnd(strText, (iTabs - 1));
        strText += Strlf;
    }
}



//------------------------------------------------------------------*
/**
 * Function name			COperation::VarStorageClassByString
 * Description		iterates through the table StorageClassTbl1 and gets
                    the index of the given storage class
 * @param			LPCTSTR pszStorage: storage class
 * @return			int : index in the table
 * @exception			
 * @see		        StorageClassTbl1	
*/
int COperation::VarStorageClassByString(LPCTSTR pszStorage)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CString strHelp;
    int nEntries  = sizeof(StorageClassTbl1)/sizeof(STORAGETYPE_DESC);

    for(int i = 0;i<nEntries;i++) 
    {
		if(!strHelp.LoadString(StorageClassTbl1[i].iIDS)) 
        {
			strHelp = StorageClassTbl1[i].szDefault;
		}

		if(!strHelp.CompareNoCase(pszStorage)) 
        {
			return StorageClassTbl1[i].iStorageClass;
		}
    }
	return -1;
}

//------------------------------------------------------------------*
/**
 * Function name	COperation::Init
 * Description		Initialises the user name, the date and the variable list
	                if there is a safearray list (this returned from the dialogs)
 * @return			void 
 * @exception			
 * @see			    CVar, AddVar
*/
void COperation::Init()
{
    GetCurrentUserName(m_sCreateUser);
    m_sLastModUser = m_sCreateUser;
    // extract variables from safe array
    if (m_pMembers)
    {
        long lSize;
        long lLBound;
        long lUBound;
        long arrIndex[2];
        UINT uiDim = ::SafeArrayGetDim(m_pMembers);
        CVar var;

        ASSERT(2 == uiDim);
        ::SafeArrayGetLBound(m_pMembers, 1, &lLBound);
        ::SafeArrayGetUBound(m_pMembers, 1, &lUBound);
        lSize = lUBound - lLBound + 1;
        
        ::SafeArrayGetLBound(m_pMembers, 2, &lLBound);
        ASSERT(0 == lLBound);
        ::SafeArrayGetUBound(m_pMembers, 2, &lUBound);
        ASSERT(8 == lUBound);
        
        for (long li=0; li<lSize; li++)
        {
            BSTR sVarClass;
            BSTR sVarName;
            BSTR sVarAlias;
            BSTR sVarComment;
            BSTR sVarType;
            BSTR sVarInitVal;
            BSTR sIP;
            BSTR sLength;
            BSTR sHint;
            CString strVarClass;
            CString strVarName;
            CString strVarAlias;
            CString strVarComment;
            CString strVarType;
            CString strVarInitVal;
            CString strIP;
            CString strLength;
            CString strHint;
            
            arrIndex[0] = li;
            
            arrIndex[1] = 0;
            ::SafeArrayGetElement(m_pMembers, arrIndex, &sVarClass);
            strVarClass = sVarClass;
            SysFreeString(sVarClass);
            
            arrIndex[1] = 1;
            ::SafeArrayGetElement(m_pMembers, arrIndex, &sVarName);
            strVarName = sVarName;
            SysFreeString(sVarName);
            
            arrIndex[1] = 2;
            ::SafeArrayGetElement(m_pMembers, arrIndex, &sVarAlias);
            strVarAlias = sVarAlias;
            SysFreeString(sVarAlias);

            arrIndex[1] = 3;
            ::SafeArrayGetElement(m_pMembers, arrIndex, &sVarType);
            strVarType = sVarType;
            SysFreeString(sVarType);
            
            arrIndex[1] = 4;
            ::SafeArrayGetElement(m_pMembers, arrIndex, &sVarInitVal);
            strVarInitVal = sVarInitVal;
            SysFreeString(sVarInitVal);

            arrIndex[1] = 5;
            ::SafeArrayGetElement(m_pMembers, arrIndex, &sVarComment);
            strVarComment = sVarComment;
            SysFreeString(sVarComment);
            
            arrIndex[1] = 6;
            ::SafeArrayGetElement(m_pMembers, arrIndex, &sIP);
            strIP = sIP;
            SysFreeString(sIP);

            arrIndex[1] = 7;
            ::SafeArrayGetElement(m_pMembers, arrIndex, &sLength);
            strLength = sLength;
            SysFreeString(sLength);

            arrIndex[1] = 8;
            ::SafeArrayGetElement(m_pMembers, arrIndex, &sHint);
            strHint = sHint;
            SysFreeString(sHint);
            
            var.SetObjName(strVarName);
            var.SetObjAlias(strVarAlias);
            var.SetObjVarComment(strVarComment);
            var.SetVarType(strVarType);
            int iClass = VarStorageClassByString(strVarClass);
            ASSERT(iClass != -1);
            var.SetVarAttr(iClass);
            var.SetVarStorageClass(strVarClass);
            var.SetVarInitVal(strVarInitVal);
            var.SetIP(_ttoi((LPCTSTR)strIP));
            var.SetVarDeclRange(_ttoi((LPCTSTR)strLength));
            var.SetHint(strHint);
            AddVar(var);
        }
    }
}

//------------------------------------------------------------------*
/**
 * Function name			COperation::InitFromFile
 * Description		Parses the file and initialises the system comment and
                    the variable declaration list.
 * @param			POU* Pou
 * @return			void 
 * @exception			
 * @see			    CVar, Init, SetPouDT, SysComment, POU, var_decl_sections_type
                    and other BackParser classes
*/
void COperation::InitFromFile(POU* Pou)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    ASSERT(Pou);
    //Init
    COperation::Init();
    SetPouDT(FALSE); //TRUE for Insert (FALSE for Prop)
    //Name
    m_sName           = Pou->getName().c_str();
    //System commentar
    m_iSysCmtPos = 0;
    m_iSysCmtLength = 0;
    const SysComment* pSysCmt = Pou->getFileSystemComment();
    if(pSysCmt)
    {
        CString strDummy;
        string dummy;
        strDummy.LoadString(IDS_KEY_ALIAS1);
        dummy = strDummy;
        m_sAlias          = (pSysCmt->avm_get(dummy)).c_str();
        strDummy.LoadString(IDS_KEY_USERTEXT1);
        dummy = strDummy;
        m_sUsrText[0]     = (pSysCmt->avm_get(dummy)).c_str();
        strDummy.LoadString(IDS_KEY_USERTEXT2);
        dummy = strDummy;
        m_sUsrText[1]     = (pSysCmt->avm_get(dummy)).c_str();
        strDummy.LoadString(IDS_KEY_USERTEXT3);
        dummy = strDummy;
        m_sUsrText[2]     = (pSysCmt->avm_get(dummy)).c_str();
        strDummy.LoadString(IDS_KEY_HELPURL);
        dummy = strDummy;
        m_sHelpUrl        = (pSysCmt->avm_get(dummy)).c_str();
        strDummy.LoadString(IDS_KEY_URL);
        dummy = strDummy;
        m_sUrl            = (pSysCmt->avm_get(dummy)).c_str();
        strDummy.LoadString(IDS_KEY_GUID1);
        dummy = strDummy;
        m_sObjGUID        = (pSysCmt->avm_get(dummy)).c_str();

        //Get the insert position and the length of the system comment
        StringPosRange SysCmtRange = pSysCmt->GetRange();
        m_iSysCmtPos = SysCmtRange.getFirst();
        m_iSysCmtLength = SysCmtRange.getLength();
    }

    //Variables
    const var_decl_sections_type VarDeclSection = Pou->getVarDeclSections();
    if(VarDeclSection.size() != 0)
    {
        //Variables from the pou object into the variable list
        var_decl_sections_citer iter;
        for (iter = VarDeclSection.begin(); iter < VarDeclSection.end(); iter++)
        {
            const string SectionType = (*iter)->getType();
            const string SectionModifier = (*iter)->getModifier();
            const var_declarations_type VarDeclList = (*iter)->getVarDeclList();
            if(VarDeclList.size() != 0)
            {
                var_declarations_citer iter1;
                for(iter1 = VarDeclList.begin(); iter1 < VarDeclList.end(); iter1++)
                {
                    CVar var;
                    CString strDummy;
                    strDummy = SectionType.c_str();
                    var.SetVarStorageClass(strDummy);
                    //var.SetModifier(strDummy(SectionModifier));
                    const string VarElemtsType = (*iter1)->getType();
                    strDummy = VarElemtsType.c_str();
                    var.SetVarType(strDummy);
                    const string VarElemtsInit = (*iter1)->getInitializer();
                    strDummy = VarElemtsInit.c_str();
                    var.SetVarInitVal(strDummy);
                    const string VarComment = (*iter1)->getVarComment();
                    strDummy = VarComment.c_str();
                    var.SetObjVarComment(strDummy);
                    StringPosRange VarDeclRange1 = (*iter1)->getRange();
                    StringPos Total = VarDeclRange1.getLength();
                    var.SetVarDeclRange(Total);
                    StringPos IP = VarDeclRange1.getFirst() - 1; 
                    var.SetIP(IP);
                    vars_type VarElemts = (*iter1)->getVariables();
                    strDummy = (*iter1)->getEdge().c_str();
                    if(strDummy.CompareNoCase(_T("R_EDGE")) == 0)
                    {
                        var.SetEdge(TYP_EDGE_R_EDGE);
                    }
                    else if(strDummy.CompareNoCase(_T("F_EDGE")) == 0)
                    {
                        var.SetEdge(TYP_EDGE_F_EDGE);
                    }
                    int iVarListSize = VarElemts.size();
                    if( iVarListSize > 0)
                    {
                        vars_citer iter2;
                        int iVarNumber = 0;
                        for(iter2 = VarElemts.begin(); iter2 < VarElemts.end(); iter2++)
                        {
                            if(iVarListSize == 1)
                            {
                                strDummy = _T("No");
                                var.SetHint(strDummy);
                                const string VarName = (*iter2)->getName();
                                strDummy = VarName.c_str();
                                var.SetObjName(strDummy);
                                const SysComment* pSysCmt = (*iter2)->getSystemComment();
                                if (pSysCmt)
                                {
                                    string dummy;
                                    strDummy.LoadString(IDS_KEY_ALIAS1);
                                    dummy = strDummy;
                                    const string VarAlias = pSysCmt->avm_get(dummy);
                                    strDummy = VarAlias.c_str();
                                    var.SetObjAlias(strDummy);
                                }
                                else
                                {
                                    strDummy = _T("");
                                    var.SetObjAlias(strDummy);
                                }
                            }
                            else if(iVarNumber == 0)
                            {
                                strDummy = _T("Mod");
                                var.SetHint(strDummy);
                                const string VarName = (*iter2)->getName();
                                strDummy = VarName.c_str();
                                var.SetObjName(strDummy);
                                const SysComment* pSysCmt = (*iter2)->getSystemComment();
                                if (pSysCmt)
                                {
                                    string dummy;
                                    strDummy.LoadString(IDS_KEY_ALIAS1);
                                    dummy = strDummy;
                                    const string VarAlias = pSysCmt->avm_get(dummy);
                                    strDummy = VarAlias.c_str();
                                    var.SetObjAlias(strDummy);
                                }
                                else
                                {
                                    strDummy = _T("");
                                    var.SetObjAlias(strDummy);
                                }
                            }
                            else
                            {
                                strDummy = _T("Add");
                                var.SetHint(strDummy);
                                var.SetIP(-1);
                                const string VarName = (*iter2)->getName();
                                strDummy = VarName.c_str();
                                var.SetObjName(strDummy);
                                const SysComment* pSysCmt = (*iter2)->getSystemComment();
                                if (pSysCmt)
                                {
                                    string dummy;
                                    strDummy.LoadString(IDS_KEY_ALIAS1);
                                    dummy = strDummy;
                                    const string VarAlias = pSysCmt->avm_get(dummy);
                                    strDummy = VarAlias.c_str();
                                    var.SetObjAlias(strDummy);
                                }
                                else
                                {
                                    strDummy = _T("");
                                    var.SetObjAlias(strDummy);
                                }
                            }
                            AddVar(var);
                            iVarNumber ++;
                        }
                    }
                }
            }
        }
    }
}

//------------------------------------------------------------------*
/**
 * Function name	COperation::GetCurrentUserName
 * Description			
 * @param			CString & UserName
 * @return			void 
 * @exception			
 * @see			
*/
void COperation::GetCurrentUserName(CString & UserName)
{
    TCHAR szUserName[CE_MAXUSERNAMELEN] = _T("");
    DWORD Size = sizeof(szUserName) / sizeof(szUserName[0]);
    GetUserName(szUserName, &Size);
    UserName = szUserName;
}


//------------------------------------------------------------------*
/**
 * Function name			COperation::SetCreateAuthor
 * Description			
 * @param			CString &StrAuthor
 * @return			void 
 * @exception			
 * @see			
*/
void COperation::SetCreateAuthor(CString &StrAuthor)
{
    m_sCreateUser = StrAuthor;
}


//------------------------------------------------------------------*
/**
 * Function name			COperation::GetCreateAuthor
 * Description			
 * @return			CString & 
 * @exception			
 * @see			
*/
CString & COperation::GetCreateAuthor()
{
    return m_sCreateUser;
}


//------------------------------------------------------------------*
/**
 * Function name			COperation::SetModifyAuthor
 * Description			
 * @param			CString &StrAuthor
 * @return			void 
 * @exception			
 * @see			
*/
void COperation::SetModifyAuthor(CString &StrAuthor)
{
    m_sLastModUser = StrAuthor;
}


//------------------------------------------------------------------*
/**
 * Function name			COperation::GetModifyAuthor
 * Description			
 * @return			CString & 
 * @exception			
 * @see			
*/
CString & COperation::GetModifyAuthor()
{
    return m_sLastModUser;
}


//------------------------------------------------------------------*
/**
 * Function name			COperation::SetPouDT
 * Description		Stes the creation and modification date	
 * @param			BOOL bAdd:  TRUE it's an insert daialog-> don't set the dates
                                FALSE it's a proporties dailog -> set the dates
 * @return			void 
 * @exception			
 * @see			    GetDates
*/
void COperation::SetPouDT(BOOL bAdd)
{
    string sCreateDate, sLastModDate, sFileName;
    
    if(bAdd) 
    { 
        // Insert
        m_sCreateDate.Empty();
        m_sLastModDate.Empty();
        return;
    }

    //Prop 
    m_sCreateDate.Empty();
    m_sLastModDate.Empty();
    sFileName = (LPCTSTR) m_sFileName;

    GetDates(sFileName, sCreateDate, sLastModDate);
    
    m_sCreateDate = sCreateDate.c_str();
    m_sLastModDate = sLastModDate.c_str();
}

//------------------------------------------------------------------*
/**
 * Function name			COperation::KillLastComma
 * Description		Kills the last komma in the system comment
 * @param			CString & strText
 * @return			void 
 * @exception			
 * @see			
*/
void COperation::KillLastComma(CString & strText)
{
    CString StrHelp,Strlf(NEWLINE),StrCom(_T(","));

    StrCom.LoadString(IDS_KEY_COMMA);
    StrCom += Strlf;

    StrHelp = strText.Right(3);
    if (StrHelp == StrCom)
    {
        strText = strText.Left(strText.GetLength() - 3);
        strText += Strlf;
    }
}

//------------------------------------------------------------------*
/**
 * Function name	COperation::AddVar
 * Description		Adds a CVar to the variable list. Depending on it's insert 
                    position. Variables with greater insert position's are inserted
                    at the begin of the list. In this way they are modified or deleted
                    before other variables with smaller insert positions and we don't
                    loose the position information in the string. Variable's with 
                    "Add" as a hint may stay at the end of the list because they have
                    no position information.
 * @param			CVar & var: storeges the info's of the variable declaration
 * @return			void 
 * @exception			
 * @see			    CVar
*/
void COperation::AddVar(CVar & var)
{
	var.SetFatherClass(m_lObjClassId);
	var.SetClass(m_sName);
    if(m_aVarList.IsEmpty())
    {
        m_aVarList.AddHead(var);
    }
    else if(var.GetHint() == _T("Add"))
    {
        m_aVarList.AddTail(var);
    }
    else
    {
        POSITION OldPos = NULL;
        POSITION pos = m_aVarList.GetHeadPosition();
        CVar var1 = m_aVarList.GetHead();
        for (int i=0;i <= m_aVarList.GetCount();i++)
        {
            if( (var.GetIP() > var1.GetIP()) && (pos != NULL))
            {
                m_aVarList.InsertBefore(pos, var);
                break;
            }
            else if((var.GetIP() > var1.GetIP()) && (pos == NULL))
            {
                ASSERT(OldPos);
                m_aVarList.InsertBefore(OldPos, var);
                break;
            }
            else if(pos == NULL)
            {
                m_aVarList.AddTail(var);
                break;
            }
            else
            {
                OldPos = pos;
                var1 = m_aVarList.GetNext(pos);
            }
        }
    }
}


//------------------------------------------------------------------*
/**
 * Function name	COperation::InsertVarList
 * Description		Inserts the text generated for the variable 
                    declaration to the body of the POU object.
                    For this perpuse, we iterate through m_aVarList
                    and insert each variable. if the storage class of 
                    the variable was changed, it's deleted from the text
                    and added at the end of the list with "Add" as a hint.
                    otherwise depending on it's hint other operation's are done 
 * @param			CString & sPouText: text to abdated
 * @param			POU*  Pou: returned from the parser
 * @return			int 0 if succesful else -1
 * @exception			
 * @see			    CVar, ModifyVar, ModifyVarStrClass, DeleteVar, InsertNewVar 
*/
int COperation::InsertVarList(CString & sPouText, POU*  Pou)
{
	int iRet;
	CVar var;

	if(m_aVarList.GetCount() == 0)
    {
		return 0;
    }

    POSITION pos = m_aVarList.GetHeadPosition();
    int Count = m_aVarList.GetCount();
	for(int i = 0; i< Count; i++) 
    {
        POSITION Oldpos;
        if(pos)
        {
            Oldpos = pos; //Needed if the strClass of the last variable in the list is modified
            var = m_aVarList.GetNext(pos);
            //var.SetClass(m_sName);
            if((var.GetHint() == _T("Mod")) && (var.GetIP() != -1))
            {
                iRet = ModifyVar(var, sPouText, Pou);
            }
            else if(var.GetHint() == _T("ModStrClass"))
            {
                iRet = ModifyVarStrClass(var, sPouText, Pou);
            }
            else if ((var.GetHint() == _T("Del")) && (var.GetIP() != -1))
            {
                iRet = DeleteVar(var, sPouText, Pou);
            }
            else if((var.GetHint() == _T("Add")) || ((var.GetHint() == _T("Mod")) && (var.GetIP() == -1)))
            {
                iRet = InsertNewVar(var, sPouText, Pou);
            }
            else if(var.GetHint() == _T("No"))
            {
                ;
            }
        }
        int NewCount = m_aVarList.GetCount();
        if(NewCount > Count)
        {
            Count = NewCount; //the strCalss of a var in the list has been modified
            if(!pos)
            {
                pos = Oldpos; // the last strClass of the last var in the list has been modified
                var = m_aVarList.GetNext(pos); //Um pos noch um 1 zu erhöhen (var wird nicht verwendet)
            }
        }
	}
	return 0;
}

//------------------------------------------------------------------*
/**
 * Function name	COperation::InsertNewVar
 * Description		Generates the text for the variable declaration
	                and inserts it in the appropriate place depending
                    on if the variable declaration section allready 
                    exists or not.
 * @param			CVar & var
 * @param			CString & sPouText
 * @param			POU*  Pou
 * @return			int: 0 if succesful else -1 
 * @exception			
 * @see			    CVar
*/
int COperation::InsertNewVar(CVar & var, CString & sPouText, POU*  Pou)
{
    CString strOut;
	SRCGEN_EXTTYPE Type;
	CString strHelp,strInsert;
	BSTR sHelp = NULL;
	bool bSection;
	StringPos IP;
	int iIdsKeySect,iIdsKeyQualifier,iIdsKeyEndSect;

	var.GetInsertString(strInsert);
	Type = var.StorageClass2ExtType();
    
    //Get the actual Pou with the actual Text
    Pou = COperation::ParsePou(sPouText, Pou);
    //Get the Insertion point
    IP = GetIP(var, bSection, Pou);

    delete Pou;
    //Set the insert position in the variable object
    //var.SetIP(IP);

        strHelp.Empty();
	    if(!bSection) 
        {
		    if(!var.KeysByExtType(Type,iIdsKeySect,iIdsKeyQualifier,iIdsKeyEndSect)) 
            {
			    ASSERT(!"CESRCGEN > External type table corrupt");
			    throw E_FAIL; //external type table corrupt.
		    }

		    strHelp.LoadString(iIdsKeySect);
            strOut += NEWLINE;
		    strOut += strHelp;
		    
		    if(iIdsKeyQualifier) 
            {
			    strOut += BLANK;
			    strHelp.LoadString(iIdsKeyQualifier);
			    strOut += strHelp;
		    }

		    strOut += NEWLINE;
		    strHelp = strInsert;
		    strOut += strHelp;
		    strOut += NEWLINE;
		    strHelp.LoadString(iIdsKeyEndSect);
		    strOut += strHelp;
		    strOut += NEWLINE;
	    }
	    else 
        {
		    strOut = strInsert;
		    strOut += NEWLINE;
	    }

        sPouText.Insert(IP, strOut);

    return 0;
}

//------------------------------------------------------------------*
/**
 * Function name	COperation::ModifyVar
 * Description	    Modifíes a variable declaration. It deletes the old one and
                    inserts the new one at the same position.
 * @param			CVar & var: Stores all info's about the variable declaration
 * @param			CString & sPouText: the text to be abdated
 * @param			POU*  Pou: returned from the parser
 * @return			int : 0 if successful else -1
 * @exception			
 * @see			    CVar
*/
int COperation::ModifyVar(CVar & var, CString & sPouText, POU*  Pou)
{
	CString strInsert;

    var.GetInsertString(strInsert, FALSE);

    int OldTextLength = sPouText.GetLength();
    int length = var.GetVarDeclRange();
    int pos = var.GetIP();// - 5;

    int NewTextLength = sPouText.Delete(pos, length);
    //strInsert += NEWLINE;
    sPouText.Insert(pos, strInsert);

    return 0;
}

//------------------------------------------------------------------*
/**
 * Function name			COperation::ModifyVarStrClass
 * Description		Modifies the storage calss of a variable. It deletes the
                    variable declaration from the old variable declaration section
                    and inserts it into the new one.
 * @param			CVar & var: the CVar class including all info's about the variable
 * @param			CString & sPouText: the text to be abdated
 * @param			POU*  Pou: pou object returned from the parser
 * @return			int : 0 if succesfull else -1
 * @exception			
 * @see			    CVar, DeleteVar, AddVar
*/
int COperation::ModifyVarStrClass(CVar & var, CString & sPouText, POU*  Pou)
{
    CString sHint = _T("Add");
    var.SetHint(sHint);
    AddVar(var);
    if(var.GetIP() != -1)
    {
        DeleteVar(var, sPouText, Pou);
    }
    return 0;
}

//------------------------------------------------------------------*
/**
 * Function name			COperation::DeleteVar
 * Description			
 * @param			CVar & var
 * @param			CString & sPouText
 * @param			POU*  Pou
 * @return			int 
 * @exception			
 * @see			
*/
int COperation::DeleteVar(CVar & var, CString & sPouText, POU*  Pou)
{
    CString strInsert;
    int OldTextLength = sPouText.GetLength();
    int length = var.GetVarDeclRange();
    int pos = var.GetIP();// - 5;

    //strInsert = SPACETAB;
    strInsert = _T("");
    int NewTextLength = sPouText.Delete(pos, length);
    sPouText.Insert(pos, strInsert);

    return 0;
}


//------------------------------------------------------------------*
/**
 * Function name	COperation::GetIP
 * Description		Uses ParsePou to create a POU object and 
                    getVarInsertPos from the created object to get the 
                    insert position for actual variable.
 * @param			CVar & var
 * @param			bool & bSection
 * @param			POU*  Pou
 * @return			StringPos 
 * @exception			
 * @see			    BackParserDll, POU
*/
StringPos COperation::GetIP(CVar & var, bool & bSection, POU*  Pou)
{
    StringPos Ret;
    bSection = false;
    CString VarStorageClass = var.GetVarStorageClass();
    string Type, Modifier;

    int iSpace = VarStorageClass.Find(" ");
    if (iSpace == -1)
    {
        Type = (LPCTSTR)VarStorageClass;
        Modifier = _T("");
    }
    else
    {
        Type = (LPCTSTR)(VarStorageClass.Left(iSpace));
        Modifier = (LPCTSTR)(VarStorageClass.Right(VarStorageClass.GetLength() - (iSpace + 1)));
        if(Modifier == _T("CONST")) //Used in key.rc
        {
            Modifier = _T("CONSTANT"); //Used in the BackParser
        }
    }

    if(Pou)
    {
        Pou->getVarInsertPos(Type, Modifier, Ret, bSection);
    }
    
    return Ret;
}


//------------------------------------------------------------------*
/**
 * Function name			COperation::InsertSysComment
 * Description			
 * @param			CString & PouText
 * @return			void 
 * @exception			
 * @see			
*/
void COperation::InsertSysComment(CString & PouText)
{
    CString SysCmt = _T("");
    GetStdAttribString(SysCmt);
    if(SysCmt != STRSYSCMTEMPTY)
    {
        //SysCmt.Delete(SysCmt.GetLength() - 2, 2); //d-351
    }
    else
    {
        SysCmt.Delete(SysCmt.GetLength() - 1, 1);
    }
    PouText.Delete(m_iSysCmtPos, m_iSysCmtLength);
    PouText.Insert(m_iSysCmtPos, SysCmt);
}

//------------------------------------------------------------------*
/**
 * Inserts system comment for POU that appears on the beginning of
 * the file (file system comment). Recognizes other comments within
 * the system comment section.
 *
 * @param		CString & PouText - pou text to write to file
 * @param		POU* pou - pou object from back parser
 * @exception	CMemoryException.
 * @see			InsertSysComment();
*/
void COperation::InsertSysComment2(CString & PouText, POU* pou)
{
    CString SysCmt;
    CString strDummy;
    string key,value;

    if(!pou) {
        InsertSysComment(PouText);
        return;
    }

    SysComment* pSysCmt = const_cast<SysComment*> (pou->getFileSystemComment());
    if(!pSysCmt) {
        InsertSysComment(PouText);
        return;
    }

    //first set all new values if applicable
    //ALIAS
    strDummy.LoadString(IDS_KEY_ALIAS1);
    key = strDummy;
            
    if(!m_sAlias.IsEmpty()) {
        value = m_sAlias;
        pSysCmt->avm_set(key,value);
    }
    else {
        pSysCmt->avm_remove(key);
    }

    //USERTEXT1
    strDummy.LoadString(IDS_KEY_USERTEXT1);
    key = strDummy;
    
    if(!m_sUsrText[0].IsEmpty()) {
        value = m_sUsrText[0];
        pSysCmt->avm_set(key,value);
    }
    else {
        pSysCmt->avm_remove(key);
    }
    
    //USERTEXT1
    strDummy.LoadString(IDS_KEY_USERTEXT2);
    key = strDummy;
    
    if(!m_sUsrText[1].IsEmpty()) {
        value = m_sUsrText[1];
        pSysCmt->avm_set(key,value);
    }
    else {
        pSysCmt->avm_remove(key);
    }

    //USERTEXT3
    strDummy.LoadString(IDS_KEY_USERTEXT3);
    key = strDummy;
    
    if(!m_sUsrText[2].IsEmpty()) {
        value = m_sUsrText[2];
        pSysCmt->avm_set(key,value);
    }
    else {
        pSysCmt->avm_remove(key);
    }

    //URL    
    strDummy.LoadString(IDS_KEY_URL);
    key = strDummy;

    if(!m_sUrl.IsEmpty()) {
        value = m_sUrl;
        pSysCmt->avm_set(key,value);
    }
    else {
        pSysCmt->avm_remove(key);
    }

    //HELP_URL    
    strDummy.LoadString(IDS_KEY_HELPURL);
    key = strDummy;

    if(!m_sHelpUrl.IsEmpty()) {
        value = m_sHelpUrl;
        pSysCmt->avm_set(key,value);
    }
    else {
        pSysCmt->avm_remove(key);
    }

    //GUID
    strDummy.LoadString(IDS_KEY_GUID1);
    key = strDummy;

    if(!m_sObjGUID.IsEmpty()) {
        value = m_sObjGUID;
        pSysCmt->avm_set(key,value);
    }
    else {
        pSysCmt->avm_remove(key);
    }

    AppendCommentBegin(SysCmt,0);
    for(bool bRet = pSysCmt->GetFirstAttrib(key,value); bRet == true;
        bRet = pSysCmt->GetNextAttrib(key,value))
    {
        AppendAttrib(SysCmt,key,value);
    }
    AppendCommentEnd(SysCmt,0);

    PouText.Delete(m_iSysCmtPos, m_iSysCmtLength);
    PouText.Insert(m_iSysCmtPos, SysCmt);
}


//------------------------------------------------------------------*
/**
 * Function name	COperation::ParsePou
 * Description		Uses parse_string from the backparser to create 
                    the	associate POU object to the given PouText.
 * @param			CString & sPouText
 * @param			POU*  Pou
 * @return			POU* 
 * @exception			
 * @see			    parse_string()
*/
POU* COperation::ParsePou(CString & sPouText, POU*  Pou)
{
    string buf = sPouText;
    //delete Pou;
    string err = parse_string(buf, &Pou);
    return Pou;
}


//------------------------------------------------------------------*
/**
 * Function name			COperation::ReadFileContent
 * Description			
 * @param			const CString fileName
 * @param			string& buf
 * @return			void 
 * @exception			
 * @see			
*/
void COperation::ReadFileContent(const CString fileName, string& buf)
{
    CString SourceFileName;

    if (fileName.Right(4).CompareNoCase(_T(".4gr")) == 0)
    {
        string HeaderFileName;
        ExtractHeader(string(fileName), HeaderFileName);
        SourceFileName = HeaderFileName.c_str();
        m_FileType = FILETYPE_4GR;
    }
    else
    {
        SourceFileName = fileName;
        m_FileType = FILETYPE_ST;
    }
    
    CStdioFile cFile(SourceFileName, CFile::modeRead);
    int iLength = cFile.GetLength();
    char* pbuf;
    pbuf = new char[iLength+1];
    int iCount;
    iCount = cFile.Read(pbuf, iLength);
    pbuf[iCount]=0;
    ASSERT(iCount > 0);
    buf = pbuf;
    delete [] pbuf;
    cFile.Close();
    if (m_FileType == FILETYPE_4GR)
    {
        CFile::Remove(SourceFileName);
    }
    return;
}

//------------------------------------------------------------------*
/**
 * Function name	COperation::GetNameProposal
 * Description			
 * @param			CString sBasis
 * @return			CString 
 * @exception			
 * @see			
*/
CString COperation::GetNameProposal(CString sBasis)
{
    CString sName;
    BSTR  strBasis= sBasis.AllocSysString();
    BSTR  strName;
    HRESULT hr = m_pICEProjectInfo->getGlobalNameProposal(strBasis, &strName);
    if(hr != S_OK)
    {
        SysFreeString(strBasis);
        return _T("");
    }
    else
    {
        sName = strName;
        SysFreeString(strBasis);
        SysFreeString(strName);
        return sName;
    }
}


//------------------------------------------------------------------*
/**
 * Function name			COperation::GetNameFromId
 * Description		this function is used to get the name of the selected object
	                if it's inserted in another object (like a task in a resource)
                    In these case the Backparser gives a list of objects and you don't
                    know wich one is actually selected. Use this function to get the name
                    from the Id and compare it to the names in the list.
 * @return			CString 
 * @exception			
 * @see			
*/
CString COperation::GetNameFromId()
{
    CString Id = _T("");
    CString Name = _T("");
    
    Id = m_sId;
    int iPos = Id.ReverseFind('/');
    Name = Id.Right(Id.GetLength() - (iPos + 1));

    return Name;
}


//------------------------------------------------------------------*
/**
 * Function name			COperation::GetVarPos
 * Description		Loocks up the variable list for a variable with the given
	                name.
 * @param			CString strVarName: varaiable name to look for
 * @return			POSITION : position
 * @exception			
 * @see			CVar
*/
POSITION COperation::GetVarPos(CString strVarName)
{
    ASSERT(!strVarName.IsEmpty());
    int iCount = m_aVarList.GetCount();
    CVar var;
    
    if(m_aVarList.IsEmpty())
    {
        return NULL;
    }
    
    POSITION pos = m_aVarList.GetHeadPosition();
    ASSERT(pos);
    POSITION posOld;
    
	for(int i = 0; i< iCount; i++) 
    {
        if(pos)
        {
            posOld = pos;
            var = m_aVarList.GetNext(pos);
        }
        if(!var.GetObjName().CompareNoCase(strVarName))
        {//this is the variable
            return posOld;
        }
    }
    //ASSERT(FALSE); // must be a mistake 
    // no, no, no: can happen if variable has been deleted in the editor, caller has to handle this case.
    return NULL;
}


//------------------------------------------------------------------*
/**
 * Function name			COperation::GetExtentionFromFileName
 * Description			
 * @param			CString strFileName
 * @return			CString 
 * @exception			
 * @see			
*/
CString COperation::GetExtentionFromFileName(CString strFileName)
{
    CString strFileExtention;
    int iPos = strFileName.ReverseFind('.');
    strFileExtention = strFileName.Right(strFileName.GetLength() - iPos);
    return strFileExtention;
}

// get position after next newline (if not exists, crlf is appended)
StringPos COperation::GetNextInsertPosition(string& text, StringPos startpos)
{
    StringPos   pos = text.find('\n', startpos);
    if(pos < 0)
    {
        text += "\r\n";
        pos = text.length() - 1;
    }
    return pos;
}

//------------------------------------------------------------------*
/**
 * Function name			COperation::CreateUsrHelp
 * Description		Copies the UserHelp Template choosen in the Help sheet
	                to the project path and renames it to the POU name with 
                    the correspending extention.
 * @return			BOOL : TRUE if succeeded else false.
 * @exception			
 * @see			
*/
BOOL COperation::CreateUsrHelp()
{
    int iPos = m_sHelpUrl.ReverseFind(_T(':'));
    CString strHelp1 = m_sHelpUrl.Right(m_sHelpUrl.GetLength()- iPos - 1);
    CString strHelp2 = m_ProjectPath;
    strHelp1 = strHelp2 + _T("\\") + strHelp1;
    
    /*CopyFile: If the last parameter is TRUE and the new file already exists, 
    the function fails. If this parameter is FALSE and the new file 
    already exists, the function overwrites the existing file and succeeds.*/
    // SIS: bug fix d-2019
    // do not overwrite file automatically
    // ask user if file is existing
    CFileStatus tStatus;
    BOOL        bOverwrite = TRUE;
    BOOL        bFileCopied = FALSE;

    if(CFile::GetStatus(strHelp1, tStatus))
    {
        CString strMessage;
        strMessage.Format(IDS_OVERWRITE_FILE, strHelp1);
        bOverwrite = (AfxMessageBox(strMessage, MB_YESNO) == IDYES) ? TRUE : FALSE;
    }
    if(bOverwrite)
    {
        bFileCopied = CopyFile(m_sHelpTemplate, strHelp1, FALSE);
        ASSERT(bFileCopied == TRUE);
    }
    
    return bFileCopied;
}

//------------------------------------------------------------------*
/**
 * Function name			COperation::CreateUsrHelpLink
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void COperation::CreateUsrHelpLink()
{
    HRESULT hRes = 0;
    BSTR    sSection, sKey, sValue;
    CString strHelp;
    int iPos = 0;
    CString strExtension;

    iPos = m_sHelpUrl.ReverseFind(_T('.'));
    if(iPos >= 0)
    {
        strExtension = m_sHelpUrl.Mid(iPos);
    }

    strHelp = _T("USER_HELP");
    sSection = strHelp.AllocSysString();
    strHelp = _T("file");
    sKey = strHelp.AllocSysString();
    iPos = m_sFileName.ReverseFind(_T('.'));
    strHelp = m_sFileName.Left(iPos) + strExtension;
    sValue = strHelp.AllocSysString();

    hRes = m_pICEProjectInfo->CPInsertToSection(sSection, sKey, sValue);
    ASSERT(hRes == S_OK);

    ::SysFreeString(sSection);
    ::SysFreeString(sKey);
    ::SysFreeString(sValue);
}



BOOL COperation::IsSourceReadOnly()
{
    return m_bSourceReadOnly;
}


void COperation::SetAlias(const CString& strAlias)
{
    m_sAlias = strAlias;
}


void COperation::SetUsrText(int i, const CString& strText)
{
    if (i<CE_NUMUSRTXT && i>=0)
        m_sUsrText[i] = strText;
}


void COperation::SetHelpUrl(const CString& strUrl)
{
    m_sHelpUrl = strUrl;
}


void COperation::SetUrl(const CString & strUrl)
{
    m_sUrl = strUrl;
}



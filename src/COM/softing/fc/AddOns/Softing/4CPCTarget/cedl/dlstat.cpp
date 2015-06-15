//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "cedlerror.h"
#include "dlrs.h"
#include "dlext.h"
#include "Downld.h"
#include "dlstat.h"
#include "utilif.h"

CHeaderInfo::CHeaderInfo() 
: m_strAddress() , m_strConfName() , 
  m_strPrjName() , m_strResName() , m_strVersion()
{ 
	m_Type = CC;
}

CStation::CStation(CHeaderInfo & Info, PIDLEvents pEvntSink, PICEMsg pMsgSink, bool bSilent)
	: m_strGenPath(), 
    m_strConfigPath(), 
    m_Parse(), 
    m_strUsrName(), 
    m_strDLGuid(),
    m_strVisDLGuid(),
    m_StationInfo(),
    m_bSilent(bSilent),
    m_Header(Info)
{
	m_lConfId = -1L;
	m_State = CEDL_STAT_STRT;
    m_Signal = 0;
    m_options = DOWNLD_NO_OPTIONS;
	m_dwThreadId = ::GetCurrentThreadId();
	m_pTransNotify = NULL;
	m_pDLSession = NULL;
	m_pConnect = NULL;
	m_dwTransferCookie = 0;
	m_dwUploadCookie = 0;
    m_dwDownloadCookie = 0;
    m_dwDelCookie = 0;
	m_hr = S_OK;
    m_pDB = NULL;
    m_pEvntSink = pEvntSink;
    m_pMsgSink = pMsgSink;
    m_StationInfo.SetStation(this);

	if(Info.m_Type == CONSOLE) {
		m_Target = VisualDomains;
		m_strVersionDomain = CEDL_VISVERSIONDOMAIN;
	}
	else {
		m_Target = ProgramDomains;
		m_strVersionDomain = CEDL_VERSIONDOMAIN;
	}

}

CStation::~CStation()
{
    m_StationInfo.Disconnect();
}

BOOL CStation::CheckThreadId(DWORD dwThreadId)
{
	if(dwThreadId != m_dwThreadId) {
		FC_DEBUGPRINT2(_T("CEDL> different thread context 0x%x <-> 0x%x\n"),
			m_dwThreadId,dwThreadId);
		return FALSE;
	}
	return TRUE;
}


void CStation::UpdateConfigurations(CHeaderInfo & Info, CDaoDatabase* pDB, CString& strGenPath)
//throws CMemoryException and CDaoException
{
	CConfRS rs(pDB);
	CString strTemp,strSQL;

    m_pDB = pDB;
    m_strGenPath = strGenPath;
    m_strConfigPath = strGenPath + _T('\\') + Info.m_strTargetName;

    m_Header = Info;

    assert(!m_strGenPath.IsEmpty());
    assert(!m_Header.m_strAddress.IsEmpty());
    assert(!m_Header.m_strResName.IsEmpty());

	strTemp = _T("SELECT * FROM Configuration WHERE ConfName = '%s' AND ResourceName = '%s'");
	strSQL.Format(strTemp,m_Header.m_strConfName,m_Header.m_strResName);

	rs.Open(dbOpenDynaset, strSQL);
	if(rs.IsEOF()) { //no records try to add
		if(rs.CanAppend()) {
			rs.AddNew();
			rs.m_ConfName = m_Header.m_strConfName;
			rs.m_ResourceName = m_Header.m_strResName;
			rs.m_Adress = m_Header.m_strAddress;
			rs.m_Version = m_Header.m_strVersion;
			rs.Update();
			rs.SetBookmark(rs.GetLastModifiedBookmark());
			m_lConfId = rs.m_ConfId;
		}
	}        
    else { //update it
		if(rs.CanUpdate()) {
			rs.Edit();
			rs.m_Adress = m_Header.m_strAddress;
            rs.m_ResourceName = m_Header.m_strResName;
			rs.m_Version = m_Header.m_strVersion;
			rs.Update();
			m_lConfId = rs.m_ConfId;
		}
	}
	rs.Close();
}

void CStation::OnEmptyDomainTable()
{
	assert(m_lConfId != -1);
    CHeaderInfo Info;
	HRESULT hr = S_OK;
	BOOL bEmpty;
	CDomRS rs(m_pDB),rsTemp,rsHelp;
	CString strTemp,strSQL,strFile,strPath;
	
	strTemp = _T("SELECT * FROM Domain WHERE ConfId = %d");
	strSQL.Format(strTemp,m_lConfId);

	try {
		rs.Open(dbOpenDynaset, strSQL);
		bEmpty = rs.IsEOF();
		rs.Close();

		if(bEmpty)
		{ //no records try to add
			strFile = m_strConfigPath + _T('\\');
			strFile += CEDL_LISTFILE;

			hr = m_Parse.Open(strFile);
			if(FAILED(hr)) {
                CFileException::ThrowOsError(hr,strFile);
			}

            ParseHeader(Info,m_Parse);

            while(ReadDomainLine(rsTemp)) {
				if(rsTemp.m_DomainName.IsEmpty() ||
					rsTemp.m_Path.IsEmpty()) {
					m_Parse.Close();
					ComThrow(CLSID_CEDL,__uuidof(IDL),E_FAIL);
				}
                
                if(GetDomain(rsTemp.m_DomainName, rsHelp)) {
                    //it is there - avoid a duplication
                    continue;
                }

				if(!AddNewDomain(rsTemp,
                    (CEDL_DOM_FLAG) (DOM_CHANGED | DOM_NEW | DOM_SELECTED))) {
					m_Parse.Close();
                    ComThrow(CLSID_CEDL,__uuidof(IDL),E_CE_DL_ADD_DOM);
				}
				
			}
            m_Parse.Close();

            //force an upload due to an empty domain table
            m_strDLGuid.Empty();
            m_strVisDLGuid.Empty();
		}

		//set the download guid
		if(GetDomain(m_strVersionDomain, rsTemp)) {
			//version domain available
            m_strDLGuid = rsTemp.m_VersionId;
        }

	}
	catch(CDaoException* e) {
        m_Parse.Close();
        throw (e); //propagate it
    }
    
    m_State = CEDL_STAT_IDLE;
}



//------------------------------------------------------------------*
/**
 * Reads one domain line from the loader.list file. The fields will
 * be stored in a database object of type CDomRS.
 * @param			CDomRS & rs - reference to database entry object.
 *					The fields of the domain line will be stored there.
 * @return			BOOL - TRUE - domain line found, otherwise FALSE.
 * @see				ProcessNextDomain(), OnEmptyDomainTable()
*/
BOOL CStation::ReadDomainLine(CDomRS & rs)
{
	CEDL_DFILE_FIELD field;
	CString strField;

	if(!m_Parse.IsEndOfLine()) {
		assert(m_Parse.IsEndOfLine());
		return FALSE;
	}

	//start with field domain path
	field = DFILE_FIELD_DOMPATH;

	do { // as long as end of line not reached
		if(!m_Parse.ParseNextField(strField)) {
			return FALSE;
		}

		switch(field) {
			case DFILE_FIELD_DOMPATH: //domain path field
				rs.m_Path = strField;
				//next expected field 
				field = DFILE_FIELD_DOMNAME;
				break;
			case DFILE_FIELD_DOMNAME: //domain name field
				rs.m_DomainName = strField;
				//next expected field 
				field = DFILE_FIELD_INVALID;
				break;
			default:
				assert(!"CEDL> unexpected field type");
				break;
		}
	}while(!m_Parse.IsEndOfLine());

	return TRUE;
}

BOOL CStation::AdaptOnlineChangeCycle2(void)
{
	CHeaderInfo Info;
	CString     reason;
	int         res;
	CString     strFile;
	HRESULT		hr;

	MarkDomains(false);
	strFile = m_strConfigPath + _T('\\');
	strFile += CEDL_LISTFILE;
	
	hr = m_Parse.Open(strFile);
	if(FAILED(hr)) {
		ComThrow(CLSID_CEDL,__uuidof(IDL),E_FAIL);
	}

	ParseHeader(Info, m_Parse);
	do {
		res = OnlineChangeProcessNextDomain(reason);
	} while(res == 1);					
	m_Parse.Close();

	if (res == -1) {
		CString strTmp;
		BSTR bstr;

		m_State = CEDL_STAT_ABORT;

		strTmp.Format(IDS_OC_IMPOSSIBLE, reason);
		bstr = strTmp.AllocSysString();
		FireErrorMessage(MAKE_HRESULT(1, E_FACILITY_CEDL, IDS_OC_IMPOSSIBLE), bstr);
		::SysFreeString(bstr);

        if(!isSilent()) 
		{
			CString strCaption;
			strCaption.Format("Download Changes to %s", m_Header.m_strTargetName);

            HWND hWnd = AfxGetMainWnd()->GetSafeHwnd();

            ::MessageBox(hWnd, strTmp, strCaption, MB_SETFOREGROUND | MB_ICONSTOP | MB_OK);
        }
		return FALSE;
	}

	return TRUE;
}


void CStation::AdaptCycle2()
{
	CString strFile;
    CHeaderInfo Info;
	HRESULT hr = S_OK;
    DUMP_STATIONSTATE(_T("AdaptCycle2"),m_State);

	MarkDomains(false);

	strFile = m_strConfigPath + _T('\\');
	strFile += CEDL_LISTFILE;

	hr = m_Parse.Open(strFile);
	if(FAILED(hr)) {
		ComThrow(CLSID_CEDL,__uuidof(IDL),E_FAIL);
	}

    ParseHeader(Info, m_Parse);
	

	while(ProcessNextDomain()) 
	{}

	m_Parse.Close();

	ResetSignal(BUILD_DL_LIST);
    m_State = CEDL_STAT_ADAPT_2; //adaption cycle 2

    if(m_pEvntSink) {
        CComBSTR sTarget = m_Header.m_strTargetName;
        m_pEvntSink->Fire_OnDownloadState(sTarget,NULL,0.0f,DOWNLD_LIST_READY);
    }
}



//------------------------------------------------------------------*
/**
 * Processes parsing of one domain line within target specific 
 * loader list file and performs binary comparision of 4CCG 
 * generated file with downloader database if the domain is not new.
 * @return			BOOL - FALSE: last domain in loader list file reached
 *					TRUE - more domain lines in loader list to process.
 * @exception		CMemoryException , CDaoException , CFileException
 * @see			
*/
BOOL CStation::ProcessNextDomain()
{	
	CDomRS rs(m_pDB), rsTemp;
	CString strTemp,strSQL,strPath,strDLPath;
	HRESULT hr;
	BOOL bDiff = FALSE;
	
	if(!ReadDomainLine(rsTemp)) 
		return FALSE;
	
	strTemp= _T("SELECT * FROM Domain WHERE DomainName = '%s' AND ConfId = %d");
	strSQL.Format(strTemp,rsTemp.m_DomainName,m_lConfId);

	rs.Open(dbOpenDynaset, strSQL);
	
	if(rs.IsEOF()) { //new Domain
		rs.Close();
		AddNewDomain(rsTemp,
			(CEDL_DOM_FLAG) (DOM_CHANGED | DOM_NEW | DOM_SELECTED));

		FC_DEBUGPRINT1(_T("CEDL> Adapt cycle 2: new domain %s\n"),
			rsTemp.m_DomainName);

		return TRUE;
	}
	

    //update the path
    strPath = rsTemp.m_Path;
    //domain not new 
	if(!(rs.m_Flags & DOM_NEW)) {

		hr = UTIL_IsAbsolutePath(strPath);
		if(GetScode(hr) == S_FALSE) { //no absolute path
			strPath = m_strGenPath + _T('\\') + strPath;	
		}

		strDLPath = m_strConfigPath + _T('\\');
		strDLPath = strDLPath + rs.m_DLFilePath;
		
		hr = CompareFiles(strPath,strDLPath);
		
		switch(GetScode(hr)) {
			case S_OK:
				bDiff = FALSE;
				break;
			case S_FALSE:
				bDiff = TRUE;
				break;
			case E_OUTOFMEMORY:
				rs.Close();
				AfxThrowMemoryException();
				return FALSE;
			default:
                //file cannot be opened
                bDiff = TRUE;
				break;
		}

		FC_DEBUGPRINT3(_T("CEDL> Adapt cycle 2: compare %s : %s => %d\n")
			,rs.m_DomainName,strDLPath,bDiff);
	}
	else {
        FC_DEBUGPRINT1(_T("CEDL> Adapt cycle 2: new domain %s\n"),
			rs.m_DomainName);

		bDiff = TRUE;
	}


    if(!rs.CanUpdate()) {
        return FALSE;
    }
	
    rs.Edit();

    if(bDiff) {//files not the same - changed & pre selected
        rs.m_Path = rsTemp.m_Path;
        rs.m_DomainName = rsTemp.m_DomainName;
	    rs.m_Flags |= DOM_CHANGED;
	    rs.m_Flags |= DOM_SELECTED;
	    rs.m_Flags &= ~DOM_DELETED;
        /*  mark as new if the upload detected a new domain means
            a domain in the cedl database is there but not on the control
        */
        if(rs.m_Flags & DOM_UPNEW) {
            rs.m_Flags &= ~DOM_UPNEW;
            rs.m_Flags |= DOM_NEW;
        }
        rs.Update();
	}
	else {
        rs.m_Path = rsTemp.m_Path;
        rs.m_DomainName = rsTemp.m_DomainName;
		rs.m_Flags &= ~DOM_DELETED;

        /*  mark as new if the upload detected a new domain means
            a domain in the cedl database is there but not on the control
        */
        if(rs.m_Flags & DOM_UPNEW) {
            rs.m_Flags &= ~DOM_UPNEW;
            rs.m_Flags |= DOM_NEW;
	        rs.m_Flags |= DOM_CHANGED;
	        rs.m_Flags |= DOM_SELECTED;
        }
		rs.Update();
	}
	
	rs.Close();
	
	return TRUE;
}


void CStation::StartDownload(void)
{
    CString strTemp;
    CComBSTR sListFile;
    HRESULT hr = S_OK;

    assert(m_pDLSession);
	WriteVersionDomain();

    if(!IsSignalSet(CHANGED)) {//nothing really changed
		if(IsSignalSet(CC_ONL)) {
            CommitDownload(FALSE);
        }
        ResetSignal(DOWN_LD);
        ResetSignal(DOWN_LD_ALL);
        ResetSignal(PRJ_NEW);
        ResetSignal(CHANGED);
        FireDLState(NULL,0.0,DOWNLD_READY); 

        if(!isSilent()) 
		{
			CString strCaption;
			strCaption.Format("Download Changes to %s", m_Header.m_strTargetName);

			CString strTmp;
			strTmp.LoadString(IDS_OC_NOCHANGE);

            HWND hWnd = AfxGetMainWnd()->GetSafeHwnd();

            ::MessageBox(hWnd, strTmp, strCaption, MB_SETFOREGROUND | MB_ICONINFORMATION | MB_OK);
        }

        return;
    }

    WriteDownloadFile(CEDL_DLFILE1,DOM_SELECTED);
    
    if(!IsSignalSet(CC_ONL)) { //start online change done ?
        //No, check for state interrupted ?
        hr = m_pDLSession->StartOnlineChange(m_Target);
        if(FAILED(hr)) {
           ThrowCSCException(hr);
        }

        SetSignal(CC_ONL);
    }

	GetSessionFileName(strTemp,CEDL_DLFILE1);
	sListFile = strTemp;
    m_State = CEDL_STAT_DLCC; //start with download

	hr = m_pDLSession->DownloadDomainList(m_Target, sListFile, &m_dwDownloadCookie);
    if(FAILED(hr)) {
       ThrowCSCException(hr);
    }
}

void CStation::GetSessionFileName(CString & strFile, LPCTSTR pszFileTemplate)
{
	CString strTemp;

	strTemp = m_strGenPath + _T('\\') + m_Header.m_strTargetName 
        + _T('\\') + CEDL_DOMDIR;
	strTemp = strTemp + _T('\\');
    strTemp = strTemp + pszFileTemplate;
	strFile.Format(strTemp,m_lConfId);

}

bool CStation::DeleteDomains(LPCTSTR pszDelFile)
//false indicates nothing to delete or deletealldomains
{
    CString strTemp;
    CComBSTR sListFile;
    HRESULT hr = S_OK;
    CFileStatus fState;

    assert(m_pDLSession);

    if(IsSignalSet(PRJ_NEW) || IsSignalSet(DOWN_LD_ALL)) {
        hr = m_pDLSession->StartOnlineChange(m_Target);
        if(FAILED(hr)) {
            ThrowCSCException(hr);
        }

        SetSignal(CC_ONL);

        hr = m_pDLSession->DeleteAllDomains(m_Target);
        if(FAILED(hr)) {
            ThrowCSCException(hr);
        }

        DeleteMarkedDomains();
        //continue with the next step
        return false;
    }
    
    WriteDownloadFile(pszDelFile,DOM_DELETED);

	GetSessionFileName(strTemp,pszDelFile);

    //check if upload file is empty
    if(!CFile::GetStatus(strTemp,fState)) {
        CFileException::ThrowOsError(ERROR_FILE_NOT_FOUND,strTemp);
    }

    if(!fState.m_size) {//nothing to load
        FC_DEBUGPRINT(_T("CEDL> No domains to delete\n"));
        //continue with the next step
        return false; 
    }

    /*  force a changed to write the correct __.list,__vislist
        domain to the target
    */
    SetSignal(CHANGED);

    hr = m_pDLSession->StartOnlineChange(m_Target);
    if(FAILED(hr)) {
        ThrowCSCException(hr);
    }

    SetSignal(CC_ONL);
	sListFile = strTemp;

	hr = m_pDLSession->DeleteDomainList(m_Target,sListFile,&m_dwDelCookie);
    if(FAILED(hr)) {
        ThrowCSCException(hr);
    }
    return true;
}


void CStation::CommitDownload(BOOL bFinalize)
{
    HRESULT hr = S_OK;
    CComBSTR sResource,sProjectGUID,sDLGUID,sProjectName;
    assert(m_pDLSession);
    
    sResource = m_Header.m_strResName;
    sProjectGUID = m_Header.m_strVersion;
    sProjectName = m_Header.m_strPrjName;
    sDLGUID = m_strDLGuid;

	int firmware;
	firmware = m_Header.m_strAddress.Find(_T("VERSION"));
	firmware = m_Header.m_strAddress.Find(_T('('), firmware);
	firmware = atol(LPCTSTR(m_Header.m_strAddress) + firmware + 1);

	// for embedded targets move flash after download
    if(IsOptionSet(DOWNLD_FLASH) && m_Target == ProgramDomains && 
		(firmware < 21000 || CString(_T("4cpc")) == m_Header.m_strTargetType)) {
        hr = m_pDLSession->FlashResource();
    }

    if(FAILED(hr)) {
        //Flash failed
        FireErrorMessage(S_CE_DL_ONLCHANGE_FAIL);

    }
    else {
        hr = m_pDLSession->CommitOnlineChange(m_Target,sResource,
            sProjectName,sProjectGUID,sDLGUID);

        if (m_Target == ProgramDomains) {
            CString strSemFile = m_strGenPath + _T("\\project.sem");
            WritePrivateProfileString("downloaded", m_Header.m_strTargetName, SUCCEEDED(hr) ? _T("1") : _T("0"), strSemFile);

            VARIANT vari;
            VariantInit(&vari);
            BSTR sVarName = ::SysAllocString(L"?sUpdataInfo");
            HRESULT hrAttrGet = m_pDLSession->get_DLoadAttribute(m_Target, sVarName, &vari);
            SysFreeString(sVarName);
            assert(FAILED(hrAttrGet) || vari.vt==VT_BSTR);
            if(SUCCEEDED(hrAttrGet) && vari.vt==VT_BSTR && vari.bstrVal[0]!=0)
            {
                const wchar_t* pLineBeg = vari.bstrVal;
                for(const wchar_t* pszw = vari.bstrVal; *pszw; pszw++)
                {
                    if(*pszw=='\n')
                    {
                        CString sLine(pLineBeg, pszw - pLineBeg);
                        pLineBeg = pszw;
                        sLine.TrimLeft();
                        sLine.TrimRight();
                        if(!sLine.IsEmpty())
                            FireErrorMessage(S_CE_DL_FIRMWARE_UPDATE_INFO, sLine);
                    }
                }
            }
            VariantClear(&vari);
        }

        if(FAILED(hr)) {
            switch(hr) {
                case E_FAIL: //configuration change failed in control
                    FireErrorMessage(S_CE_DL_ONLCHANGE_FAIL);
                    FireDLState(NULL,0.0,DOWNLD_ERROR); 
                    hr = S_OK;
                    break;
                default:
                    //any other error
                    FireErrorMessage(S_CE_DL_ONLCHANGE_FAIL);
                    break;
            }
        }

		// for embedded targets flash happens download
		if(IsOptionSet(DOWNLD_FLASH) && m_Target == ProgramDomains && 
			(firmware >= 21000 && CString(_T("4cpc")) != m_Header.m_strTargetType)) {
			hr = m_pDLSession->FlashResource();
		}
    }

    ResetSignal(CC_ONL);    

    if(!bFinalize) {
        if(FAILED(hr)) {
            ThrowCSCException(hr);
        }
        return;
    }
    ResetSignal(DOWN_LD);
    ResetSignal(DOWN_LD_ALL);
    ResetSignal(PRJ_NEW);
    ResetSignal(CHANGED);
    ResetSignal(BUILD_DL_LIST);
    FireDLState(NULL,0.0,DOWNLD_READY); 
}

HRESULT CStation::CancelDownload()
{
    HRESULT hr;
    CComBSTR sResource;
    assert(m_pDLSession);
    
    sResource = m_Header.m_strResName;
    hr = m_pDLSession->CancelOnlineChange(m_Target);
    ResetSignal(CC_ONL);
    return hr;
}


void CStation::WriteVersionDomain(void)
//writes the __.list file
{
	CDomRS rs(m_pDB);
    CGuid guid;
	CString strTemp,strSQL,strFile,strPath,strDLPath;
	CCsvExp ExpList;
	HRESULT hr;
    DWORD dwError;

	assert(m_lConfId != -1);

    //open the __.list file
	strFile = m_strConfigPath + _T('\\');
	strFile += m_strVersionDomain;

	ExpList.Open(strFile);

	strTemp = _T("SELECT * FROM Domain WHERE ConfId = %d");
	strSQL.Format(strTemp,m_lConfId);

	rs.Open(dbOpenDynaset, strSQL);
	while(!rs.IsEOF()) {

        if(rs.m_DomainName == m_strVersionDomain) {
			if(!rs.IsEOF())		
				rs.MoveNext();
			continue;
		}
		//changed includes changed and new
		short flag = rs.m_Flags;
		strTemp = rs.m_VersionId;

		if(flag & DOM_SELECTED) {	

            if(flag & DOM_CHANGED) {
                FC_DEBUGPRINT1(_T("CEDL> New GUID for domain %s\n"),
					rs.m_DomainName);

				//generate a new guid for domain
				guid.Create();
				strTemp = guid;
				guid.Empty();

				//copy new domain to cedl domain file
			    hr = UTIL_IsAbsolutePath(rs.m_Path);
			    if(GetScode(hr) == S_FALSE) { //no absolute path
				    strPath = m_strGenPath + _T('\\') + rs.m_Path;	
			    }
			    else {//absolute path
				    strPath = rs.m_Path;
			    }

			    strDLPath = m_strConfigPath + _T('\\');
			    strDLPath = strDLPath + rs.m_DLFilePath;

			    if(!::CopyFile(strPath,strDLPath,FALSE)) {
                    dwError = ::GetLastError();

				    FC_DEBUGPRINT2(_T("CEDL> Cannot copy domain file %s => %s\n"),
					    strPath,rs.m_DLFilePath);
				    rs.Close();

                    if(dwError == ERROR_FILE_NOT_FOUND) {
                        ComThrow(CLSID_CEDL,__uuidof(IDL),E_CE_DL_DOMFILE_NOTFOUND,m_Header.m_strAddress,strPath);
                    }
                    else {
                        CFileException::ThrowOsError(dwError,strPath);
                    }
                    
			    }
            }
			//iff domain new - clear it
			if(flag & DOM_NEW) {
				flag &= ~DOM_NEW;
			}

		    SetSignal(CHANGED);
        }

		ExpList.WriteField(rs.m_DomainName);
		ExpList.WriteField(strTemp);
		ExpList << COleDateTime::GetCurrentTime();
		ExpList.WriteField(_T(""),TRUE);

		if(rs.CanUpdate()) {
			rs.Edit();
			rs.m_Flags = flag;
			rs.m_VersionId = strTemp;
			rs.Update();
		}

		if(!rs.IsEOF())		
			rs.MoveNext();

	}
	rs.Close();
	ExpList.Close();
	
    
    //build up the version domain (__.list-domain) entry in the database
	rs.m_ConfId = m_lConfId;
	rs.m_DomainName = m_strVersionDomain;
	rs.m_Flags = DOM_NEW | DOM_CHANGED | DOM_SELECTED;
	rs.m_Path = m_Header.m_strTargetName + _T('\\') + m_strVersionDomain;

	rs.m_DLFilePath = "";

    if(IsSignalSet(CHANGED)) {
	    //generate a new guid for the download (version guid of the version domain
        guid.Create();
        rs.m_VersionId = guid;
	    FC_DEBUGPRINT1(_T("CEDL> New GUID for domain %s\n"),rs.m_DomainName);
        m_strDLGuid = rs.m_VersionId;
        guid.Empty();
    }
    else {
        //no download at all: use previous version ID (makes the different version go away)
        m_strDLGuid = m_StationInfo.GetVar(DloadGUID);
        rs.m_VersionId = m_strDLGuid;
    }

	UpdateDomain(rs);
	strDLPath = m_strConfigPath + _T('\\');
	strDLPath = strDLPath + rs.m_DLFilePath;

	if(!::CopyFile(strFile,strDLPath,FALSE)) {
        dwError = ::GetLastError();

        if(dwError == ERROR_FILE_NOT_FOUND) {
            ComThrow(CLSID_CEDL,__uuidof(IDL),E_CE_DL_DOMFILE_NOTFOUND,m_Header.m_strAddress,strFile);
        }
        else {
            CFileException::ThrowOsError(dwError,strFile);
        }
	}
}

void CStation::WriteDownloadFile(LPCTSTR pszFileName,short nFlag)
{
	CDomRS rs(m_pDB);
	CString strTemp,strSQL;
	CCsvExp Exp;

	bool objectsinit = FALSE;
	CString objectsinitName1, objectsinitName2;
	
	assert(m_lConfId != -1);

	GetSessionFileName(strTemp,pszFileName);

	Exp.Open(strTemp);

	strTemp = _T("SELECT * FROM Domain WHERE ConfId = %d");
	strSQL.Format(strTemp,m_lConfId);

	rs.Open(dbOpenDynaset, strSQL);
	while(!rs.IsEOF()) {
		if(rs.m_Flags & (nFlag)) {
            strTemp = m_strConfigPath + _T('\\') + rs.m_DLFilePath;

			// for embedded targets move __objects.init to end 
			// it need to appear in the list after all .def files
			if(rs.m_DomainName.CompareNoCase(_T("__objects.init")) == 0)
			{
				assert(objectsinit == FALSE);
				objectsinitName1 = rs.m_DomainName;
				objectsinitName2 = strTemp;
				objectsinit = true;
			}
			else
			{
    			Exp.WriteField(rs.m_DomainName);
    			Exp.WriteField(strTemp,TRUE);
			}
        }

		if(!rs.IsEOF())		
			rs.MoveNext();
    }

	if(objectsinit)
	{
    	Exp.WriteField(objectsinitName1);
    	Exp.WriteField(objectsinitName2,TRUE);
	}

    rs.Close();
    Exp.Close();
}

void CStation::AdaptCycle0(void)
{
    HRESULT hr = S_OK;
    int iState;
	
	assert(m_pDLSession);

    if(IsSignalSet(PRJ_NEW)) //transfer error sets this signal too
        return;

    ResetSignal(PRJ_NEW);
    ResetSignal(PRJ_CHANGED);

    hr = m_StationInfo.GetDLoadAttributes(m_Header.m_strResName,m_pDLSession);
    if (FAILED(hr)) {
        ComThrow(CLSID_CEDL,__uuidof(IDL),hr);    
    }

    //do the checks
    iState = m_StationInfo.State();
    switch(iState) {
        case CStationInfo::cleared:
        case CStationInfo::project:
            SetSignal(PRJ_NEW);
            break;
        case CStationInfo::unknown: // fresh installation found?
        case CStationInfo::unsync:
            SetSignal(PRJ_CHANGED);
            break;
        case CStationInfo::sync:
            break;
        case CStationInfo::notconnected:
        default :
            assert(!"CEDL> Invalid project state");
            SetSignal(PRJ_NEW);
            break;
    }
}

void CStation::Attach(LPUNKNOWN pUnk)
{
	IFCDownload* pISession = NULL;
    IFCConnect*	 pConnect = NULL;
	HRESULT hr = S_OK;

	try{

		hr = pUnk->QueryInterface(IID_IFCDownload,(void **)&pISession);
		if(FAILED(hr)) {
			FC_DEBUGPRINT1(_T("CEDL> QueryIf failed hr=0x%x\n"),hr);
			_com_raise_error(hr);
		}

		hr = pUnk->QueryInterface(IID_IFCConnect,(void **)&pConnect);
		if(FAILED(hr)) {
			FC_DEBUGPRINT1(_T("CEDL> QueryIf failed hr=0x%x\n"),hr);
			_com_raise_error(hr);
		}
        
		if(m_pDLSession) {
			//attached but an old session ?
			if(pISession == m_pDLSession) {
				//same session - return but check target first
				SetSignal(ATTACHED);
                FC_RELEASE_PTR(pConnect);
                FC_RELEASE_PTR(pISession);
				return;
			}

			//not the same session
			Detach();
		}

		m_pDLSession = pISession;
        m_pConnect = pConnect;
		pISession = NULL;
        pConnect = NULL;

		//now try to attach
		hr = AdviseSession();
		if(FAILED(hr)) {
            _com_raise_error(hr);
		}

		SetSignal(ATTACHED);

	}
    catch(_com_error err){

		UnAdviseSession();

        FC_RELEASE_PTR(pConnect);
        FC_RELEASE_PTR(pISession);
		FC_RELEASE_PTR(m_pConnect);
		FC_RELEASE_PTR(m_pDLSession);
		ResetSignal(ATTACHED);
        hr = err.Error();
	}

    if(FAILED(hr)) {
        _com_raise_error(hr);
    }
}

bool CStation::IsConnected(void)
{
	CSC_SESSION_STATE CurState;
	bool bConn;
	
	if(!m_pDLSession || !m_pConnect) {
		return false;
	}

	m_pConnect->get_SessionState(&CurState);
	bConn = false;

	switch(CurState) {
		case waiting_for_connect:
		case waiting_for_exclusive:
		case not_connected:
			break;
		case interrupted: //interrupted but connection logically est.
			bConn = true; 
			break;
		case exclusive_established:
			bConn = true;
			break;
	}

	return bConn;
}


//------------------------------------------------------------------*
/**
 * Detaches session pointer of the download session from downloader
 * CStation object. If the session is not attached and bForce is false
 * (the default value) the detach operation will not be performed.
 * If the session is not attached (signal ATTACHED reset) and bForce
 * is true the session will be detached anyway. bForce is usually only
 * set during final release of the downloader COM object.
 * @param			bool bForce - see above (default value is false).
 * @return			void 
 * @exception		_com_error, if UnAdviseSession() failed.
 * @see             UnAdviseSession().
*/
void CStation::Detach(bool bForce)
{
	HRESULT hr = S_OK;
	ULONG ulRef = 0;

	if(!IsSignalSet(ATTACHED) && bForce == false) {//not attached - return
		return;
	}

	hr = UnAdviseSession();
	
	FC_RELEASE_PTR(m_pDLSession);
	FC_RELEASE_PTR(m_pConnect);
	ResetSignal(ATTACHED);
    
    if(FAILED(hr)) {
        _com_raise_error(hr);
    }
    return;
}

HRESULT CStation::AdviseSession(void)
{
	HRESULT hr;

	if(!m_pDLSession || !m_pConnect)
		return S_OK;

	if(!m_pTransNotify) {
		//create the transfer notification interface
		hr = CComObject<CImpTransferNotify>::CreateInstance(&m_pTransNotify);
		if(FAILED(hr)) {
			FC_DEBUGPRINT1(_T("CEDL> create of tranfer notification if. failed hr=0x%x\n"),hr);
			return hr;
		}
		
		m_pTransNotify->AddRef(); //stay with one reference

		AddRef(); //give another ref to the notification interface
		m_pTransNotify->m_pStation = this;
	}

	if(!m_dwTransferCookie) {
		hr = AtlAdvise(m_pDLSession,m_pTransNotify->GetUnknown(),
			IID__FCTransferNotification, &m_dwTransferCookie);
		if(FAILED(hr)) {
			FC_DEBUGPRINT1(_T("CEDL> Advise failed hr=0x%x\n"),hr);
			return hr;
		}
	}

	return S_OK;
}

HRESULT CStation::UnAdviseSession(void)
{
	HRESULT hr,hrRes = S_OK;

	if(!m_pDLSession) {
		return S_OK;
	}
	
	// unadvise the notification object from connection point
	if(m_dwTransferCookie) {

		hr = AtlUnadvise(m_pDLSession,IID__FCTransferNotification, m_dwTransferCookie);

		if(FAILED(hr)) {
			FC_DEBUGPRINT1(_T("CEDL> Unadvise failed hr=0x%x\n"),hr);
			hrRes = (hrRes == S_OK) ? hr : hrRes;
			//overwrite hrRes only iff S_OK - first error counts
		}
		
		m_dwTransferCookie = 0;
	}

	if(m_pTransNotify) {
		m_pTransNotify->Release();
		m_pTransNotify = NULL;
	}

	return hrRes;
}

HRESULT CStation::ConnectVars(void)
{
    HRESULT hr = m_StationInfo.Connect();
    return hr;
}

HRESULT CStation::ReconnectVars(void)
{
    if(m_Header.m_strAddress != m_StationInfo.GetAddress()) {
        //different target machine address - reconnect
        m_StationInfo.Disconnect();
        return m_StationInfo.Connect();
    }

    return S_OK;
}


HRESULT CStation::DoStateMachine()
{
    HRESULT hr = S_OK;
    if(!IsRequestPending())
        return E_FAIL;

    try {
        switch(m_State) {
            case CEDL_STAT_WAIT_CONNECT:
                if(CheckConnection()) {
                    hr = DoStateMachine();
                    break;
                }
                break;
            case CEDL_STAT_DIR:
                //upload the domain dir e.g version domain
                m_State = CEDL_STAT_GETDV;
		        UploadDirectory(CEDL_UPFILE1);
                break;
            case CEDL_STAT_GETDV:
                //requests have been resetted
			    ResetCookies();

                if (IsSignalSet(PRJ_NEW)) { // upload of list domain fails...
                    m_State = CEDL_STAT_ADAPT_1;
                    hr = DoStateMachine();
                    break;
                }

                CompareDomDirs(CEDL_UPFILE1);

                //can upload control domains now
                if(UploadDomains(CEDL_UPFILE1)) {
                    //upload target domains started
                    break;
                }
                /*false no domains to upload
                */
                m_State = CEDL_STAT_ADAPT_1;
            case CEDL_STAT_ADAPT_1:
                FireUploadReady();

                if(IsSignalSet(PRJ_NEW)) {
				    
				    FireProjectNew();
				    MarkDomainsNew();

				    if(  IsSignalSet(DOWN_LD) &&
					    !IsSignalSet(DOWN_LD_ALL)) {
                        if (isSilent()) {
						    OnAborting();
						    break;
                        }

					    //Download changes 
						CString strCaption;
						strCaption.Format("Download Changes to %s", m_Header.m_strTargetName);
                    
                        CString strText;
						strText.LoadString(IDS_QUERY_NEW_PROJECT);
                        
						HWND hWnd = AfxGetMainWnd()->GetSafeHwnd();
                    
                        if(::MessageBox(hWnd, strText, strCaption, MB_SETFOREGROUND | MB_ICONQUESTION | MB_OKCANCEL) == IDCANCEL) 
						{
						    OnAborting();
						    break;
					    }
				    }
                }

				if(IsOptionSet(DOWNLD_FLASH_ONLY))
				{
					m_pDLSession->FlashResource();
					ResetSignal(DOWN_LD);
					ResetSignal(BUILD_DL_LIST);
					OnAborting();
					break;
				}

				if(!IsSignalSet(DOWN_LD_ALL) && IsSignalSet(DOWN_LD) 
					&& CString(_T("4cpc")) != m_Header.m_strTargetType 
					&& AdaptOnlineChangeCycle2() != TRUE) 
				{
					OnAborting();
					break;
				}

                AdaptCycle2(); //adaption cycle 2
                //fall thru
            case CEDL_STAT_ADAPT_2:
                //iff download not set
                if(!IsSignalSet(DOWN_LD)) {
                    //m_State = CEDL_STAT_IDLE;
                    break;
                }

                /*
                    start with the deletion cycle
                */
                m_State = CEDL_STAT_DELCC;
                if(DeleteDomains(CEDL_DELFILE)) {
                    //deletion cycle started
                    break;
                }

                /*
                    no target domains to delete or delete all
                    start download target domains
                */
                StartDownload();
                break;
            case CEDL_STAT_UPCC: //control domain upload
			    ResetCookies();

                FinalizeCycle1();

                m_State = CEDL_STAT_ADAPT_1;
                hr = DoStateMachine();
                break;
            case CEDL_STAT_DLCC:
                FC_DEBUGPRINT(_T("CEDL> Download Control cleanup\n"));
                ResetCookies();
                if(m_Target == ProgramDomains)
                {
                    VARIANT vari;
                    VariantInit(&vari);
                    BSTR sVarName = ::SysAllocString(L"?bMustRestart");
                    HRESULT hrAttrGet = m_pDLSession->get_DLoadAttribute(m_Target, sVarName, &vari);
                    SysFreeString(sVarName);
                    
                    assert(FAILED(hrAttrGet) || vari.vt==VT_BOOL);
                    if(SUCCEEDED(hrAttrGet) && vari.vt==VT_BOOL && vari.boolVal)
                    {
                        FireErrorMessage(S_CE_DL_FIRMWARECHANGE);
                        if(!isSilent())
                        {
                            CString strText,strCaption;
                            HWND hWnd = (AfxGetMainWnd())->GetSafeHwnd();
                            strText.LoadString(IDS_QUERY_FRMWARE_CHANGES);
                            strCaption.Format(IDS_DL_CAPTION, GetAddress());

                            if(::MessageBox(hWnd,strText,strCaption, 
                                MB_SETFOREGROUND|MB_ICONEXCLAMATION|MB_OKCANCEL) == IDCANCEL) 
                            {
						        OnAborting();
						        break;
					        }
                        }
                    }
                    //else: not a version supporting this 
                    VariantClear(&vari);
                }
                FireErrorMessage(S_CE_DL_CONFIG_CHANGE);
                CommitDownload(TRUE);
                break;
            case CEDL_STAT_DELCC:
			    ResetCookies();
                DeleteMarkedDomains();
                /*
                    start with download to target
                */
                StartDownload();
                break;
            case CEDL_STAT_ABORT: //download is aborted... do nothing!
                break;
            default:
                assert(!"Bad State");
                break;
        }
    }
    catch(_com_error err) {
        hr = err.Error();
        FireErrorMessage(hr,(BSTR)err.Description());
    }
	catch(CDaoException* e) {
        hr = e->m_scode;
        FireDaoErrorMessage(*e);
        e->Delete();
	}
	catch(CMemoryException * e) {
		hr = E_OUTOFMEMORY;
		e->Delete();
	}
	catch(CFileException * e) {
		hr = HRESULT_FROM_WIN32(e->m_lOsError);
        FireFileErrorMessage(*e);
		e->Delete();
	}

    if(m_Parse.IsOpen()) {
        m_Parse.Close();
    }

    if(FAILED(hr)) {
        FireError(hr);
    }
    return hr;
}

void CStation::UploadDirectory(LPCTSTR pszUpldFile)
/*  upload request for the __.list (version-guid) domain 
*/
{
	CDomRS rs;
	CString strTemp,strDLPath;
    CCsvExp Exp;
	CComBSTR sListFile;
	HRESULT hr = S_OK;

	GetSessionFileName(strTemp,pszUpldFile);

	Exp.Open(strTemp);        
	//version domain first
	if(!GetDomain(m_strVersionDomain,rs)) {
		rs.m_DomainName = m_strVersionDomain;
		rs.m_Path = m_Header.m_strConfName + _T('\\') + m_Header.m_strResName + 
			_T('\\') + m_strVersionDomain;
		AddNewDomain(rs,(CEDL_DOM_FLAG) (DOM_CHANGED | DOM_NEW | DOM_SELECTED));
	}
	strDLPath = m_strConfigPath + _T('\\');
	strDLPath = strDLPath + rs.m_DLFilePath;
	
	Exp.WriteField(rs.m_DomainName);
	Exp.WriteField(strDLPath,TRUE);
    Exp.Close();

	sListFile = strTemp;

    hr = m_pDLSession->UploadDomainList(m_Target, sListFile, &m_dwUploadCookie);
    if(FAILED(hr)) {
        ThrowCSCException(hr);
    }
    Exp.Close();
}



bool CStation::UploadDomains(LPCTSTR pszUpldFile)
{
	CString strTemp;
	CComBSTR sListFile;
    CFileStatus fState;
	HRESULT hr = S_OK;

	GetSessionFileName(strTemp,pszUpldFile);

    //check if upload file is empty
    if(!CFile::GetStatus(strTemp,fState)) {
        CFileException::ThrowOsError(ERROR_FILE_NOT_FOUND,strTemp);
    }

    m_State = CEDL_STAT_UPCC;

    if(!fState.m_size) {//nothing to load
        FC_DEBUGPRINT(_T("CEDL> AdaptCycle1: No changed domains to upload\n"));
        return false; 
    }

	sListFile = strTemp;

	hr = m_pDLSession->UploadDomainList(m_Target, sListFile, &m_dwUploadCookie);
	if(FAILED(hr)) {
        ThrowCSCException(hr);
	}
    return true;
}

void CStation::CompareDomDirs(LPCTSTR pszUpldFile)
{
	CDomRS rs;
	CString strTemp,strDomName,strGUID;
 	CCsvExp Exp;
	BOOL bUpload;
	HRESULT hr = S_OK;

	MarkDomains(true);

    if(!GetDomain(m_strVersionDomain,rs)) {
        ComThrow(CLSID_CEDL,__uuidof(IDL),E_CE_DL_DOM_NOT_FOUND,m_Header.m_strAddress);
	}

	strTemp = m_strConfigPath + _T('\\');
	strTemp = strTemp + rs.m_DLFilePath;

	//this must be the former uploaded version domain e.g. domain dir of 4CC
	hr = m_Parse.Open(strTemp);
	if(FAILED(hr)) {
		ComThrow(CLSID_CEDL,__uuidof(IDL),E_FAIL);
	}

	//get the transfer file for CSC_Online
	GetSessionFileName(strTemp,pszUpldFile);
	Exp.Open(strTemp);
	
	while(1) {
		bUpload = FALSE;

		if(!m_Parse.ParseNextField(strDomName)) {
			break;
		}

		if(!m_Parse.ParseNextField(strGUID)) {
			break;
		}

		if(!m_Parse.ParseNextField(strTemp)) {
			break;
		}

		if(!m_Parse.ParseNextField(strTemp)) {
			break;
		}

		if(!GetDomain(strDomName,rs)) {
			//do not update the version guid here
			rs.m_DomainName = strDomName;
			rs.m_Path = _T("");
			AddNewDomain(rs,(CEDL_DOM_FLAG) (DOM_CHANGED | DOM_NEW | DOM_SELECTED));
			bUpload = TRUE;
		}
		else { //domain found
			if(strGUID != rs.m_VersionId) {
				bUpload = TRUE;
			}

			rs.m_Flags &= ~DOM_DELETED;
            rs.m_Flags &= ~DOM_UPNEW;
			UpdateDomain(rs);
		}

		if(bUpload) {
			FC_DEBUGPRINT1(_T("CEDL> AdaptCycle1 upload domain %s\n"),
                rs.m_DomainName);
            
            CString strDLPath = m_strConfigPath + _T('\\');
			strDLPath = strDLPath + rs.m_DLFilePath;

			Exp.WriteField(rs.m_DomainName);
			Exp.WriteField(strDLPath,TRUE);

            ClearFile(strDLPath);
		}
	}

    Exp.Close();
	m_Parse.Close();
}

void CStation::FinalizeCycle1()
{
	CDomRS rs;
	CString strTemp,strDomName,strGUID,strDLPath;
	CCsvExp Exp;
	HRESULT hr = S_OK;
    CFileStatus Status;
	
    DUMP_STATIONSTATE(_T("FinalizeCycle1"),m_State);
    
	if(!GetDomain(m_strVersionDomain,rs)) {
		ComThrow(CLSID_CEDL,__uuidof(IDL),E_CE_DL_DOM_NOT_FOUND,m_Header.m_strAddress);
	}

	strTemp = m_strConfigPath + _T('\\');
	strTemp = strTemp + rs.m_DLFilePath;
	//this must be the former uploaded version domain e.g. domain dir of 4CC
	
	hr = m_Parse.Open(strTemp);
	if(FAILED(hr)) {
		ComThrow(CLSID_CEDL,__uuidof(IDL),E_FAIL);
	}
	
	while(1) {
		if(!m_Parse.ParseNextField(strDomName)) {
			break;
		}

		
		if(!m_Parse.ParseNextField(strGUID)) {
			break;
		}

		if(!m_Parse.ParseNextField(strTemp)) {
			break;
		}

		
		if(!m_Parse.ParseNextField(strTemp)) {
			break;
		}

		if(!GetDomain(strDomName,rs)) { //domain must be there
			m_Parse.Close();
			ComThrow(CLSID_CEDL,__uuidof(IDL),E_CE_DL_DOM_NOT_FOUND,m_Header.m_strAddress);
		}

        strDLPath = m_strConfigPath + _T('\\');
		strDLPath = strDLPath + rs.m_DLFilePath;

        if(!CFile::GetStatus(strDLPath,Status)) {
            continue;
        }

		//domain found update the version guid
        if(strGUID != rs.m_VersionId) {
			rs.m_VersionId = strGUID;
            rs.m_Flags &= ~DOM_NEW;
			UpdateDomain(rs);
        }
	}

	Exp.Close();
	m_Parse.Close();
}

void CStation::OnError(HRESULT hr)
//error handling
{
    HRESULT hrRes;

    hrRes = Abort();

    if(FAILED(hrRes)) {
        FireErrorMessage(hrRes);
    }
}

HRESULT CStation::Abort(void)
{
	DWORD dwCookie = 0;
	HRESULT hr;

	m_State = CEDL_STAT_ABORT;
	
	if(m_dwDownloadCookie) {
		dwCookie = m_dwDownloadCookie;
		m_dwDownloadCookie = 0;
	}

	if(m_dwUploadCookie) {
		dwCookie = m_dwUploadCookie;
		m_dwUploadCookie = 0;
	}

	if(m_dwDelCookie) {
		dwCookie = m_dwDelCookie;
		m_dwDelCookie = 0;
	}

	if(dwCookie) {
		hr = m_pDLSession->AbortTransfer(dwCookie);
	}

	hr = OnAborting();
	return hr;
}



HRESULT CStation::OnAborting(void)
{
    HRESULT hr,hrRes = S_OK;
    bool bRequest;
    
    //Is there any request pending ?
    bRequest = IsRequestPending();
    
    if(IsSignalSet(CC_ONL)) {

        hr = CancelDownload();
        if(FAILED(hr)) { //let the first error count but continue...
            hrRes = (hrRes == S_OK) ? hr : hrRes;
        }
    }

    ResetSignal(DOWN_LD);
    ResetSignal(DOWN_LD_ALL);
    ResetSignal(PRJ_NEW);
    ResetSignal(PRJ_CHANGED);
    ResetSignal(CHANGED);
	ResetSignal(BUILD_DL_LIST);

    if(bRequest) {
        //fire abort message iff there was a request pending
	    FireDLState(NULL,0.0,DOWNLD_ABORTED,S_OK,true);
    }
    else {
        FireDLState(NULL,0.0,DOWNLD_ABORTED,S_OK,false);
    }
    m_State = CEDL_STAT_IDLE;
    return hrRes;
}

void CStation::GetLoginUser(CString & strUser)
{
	HRESULT hr;
	BSTR sUser = NULL;

	strUser.Empty();

	if(!m_pDLSession) {
		return;
	}

	if(!m_pConnect) {
		return;
	}

	hr = m_pConnect->get_CurrentUser(&sUser);
	if(SUCCEEDED(hr)) {
		strUser = sUser;
	}
	
	if(sUser) {
		::SysFreeString(sUser);
	}

}


//------------------------------------------------------------------*
/**
 * Parses the header of the 4CCG generated loader.list file for a 
 * specific target. The header information is stored in a CHeaderInfo
 * reference. In case of an exception CCsvParse::Close() is called to
 * close the file. On normal return the CCsvParse::Close() has to be
 * called after invocation of this method
 * @param			CHeaderInfo & Info - reference to CHeaderInfo object
 *                  for storage of header information.
 * @param			CCsvParse & Parse - reference to CCsvParse object
 *                  for parsing the loader.list (csv-typed) file.
 * @return			void 
 * @exception       _com_error() - syntax error.
*/
void CStation::ParseHeader(CHeaderInfo & Info, CCsvParse & Parse)
{
    CString strField;

    if(Parse.ParseNextField(strField) == FALSE) {
        Parse.Close();
        _com_raise_error(E_FAIL);
    }
	//address
    if(Parse.ParseNextField(strField) == FALSE) {
        Parse.Close();
		_com_raise_error(E_FAIL);
    }
    Info.m_strAddress = strField;
    //project;
    if(Parse.ParseNextField(strField) == FALSE) {
        Parse.Close();
        _com_raise_error(E_FAIL);
    }
    //project name
    if(Parse.ParseNextField(strField) == FALSE) {
        Parse.Close();
        _com_raise_error(E_FAIL);
    }
    Info.m_strPrjName = strField;
    //version 
    if(Parse.ParseNextField(strField) == FALSE) {
        Parse.Close();
        _com_raise_error(E_FAIL);
    }
    //version string
    if(Parse.ParseNextField(strField) == FALSE) {
        Parse.Close();
        _com_raise_error(E_FAIL);
    }
    Info.m_strVersion = strField;
    //config
    if(Parse.ParseNextField(strField) == FALSE) {
        Parse.Close();
        _com_raise_error(E_FAIL);
    }
    //config name
    if(Parse.ParseNextField(strField) == FALSE) {
        Parse.Close();
        _com_raise_error(E_FAIL);
    }
    Info.m_strConfName = strField;
    //resource
    if(Parse.ParseNextField(strField) == FALSE) {
        Parse.Close();
        _com_raise_error(E_FAIL);
    }

    //resource name
    if(Parse.ParseNextField(strField) == FALSE) {
        Parse.Close();
        _com_raise_error(E_FAIL);
    }
    Info.m_strResName = strField;

    //target type
    if(Parse.ParseNextField(strField) == FALSE) {
        Parse.Close();
        _com_raise_error(E_FAIL);
    }

    //target type id
    if(Parse.ParseNextField(strField) == FALSE) {
        Parse.Close();
        _com_raise_error(E_FAIL);
    }
	Info.m_strTargetType = strField;

	if(!strField.CompareNoCase(_T("4cconsole"))) {
		Info.m_Type = CONSOLE;
	}
	else {
		Info.m_Type = CC;
	}
}



//------------------------------------------------------------------*
/**
 * Checks the connection, detemines new state with connection state
 * and requested download operation. The return value indicates whether
 * DoStateMachine has to be called again or not
 * @return			true , call DoStateMachine() with the new state
 *                  determined by this function.<br>
 *                  false , do not call DoStateMachine().<br>
 * @exception       _com_error
 * @see			    DoStateMachine()
*/
bool CStation::CheckConnection()
{
    CSC_SESSION_STATE CurState;
    BOOL bUpload;
    HRESULT hr;

    if(!m_pDLSession || !m_pConnect) {
        m_State = CEDL_STAT_WAIT_CONNECT;
		ComThrow(CLSID_CEDL,__uuidof(IDL),E_CE_DL_STATION_NOT_CONN,m_Header.m_strAddress);
    }

	m_pConnect->get_SessionState(&CurState);
    switch(CurState) {
	    case waiting_for_connect:
	    case waiting_for_exclusive:
	    case interrupted:
	    case not_connected:
		    m_State = CEDL_STAT_WAIT_CONNECT;
            ComThrow(CLSID_CEDL,__uuidof(IDL),E_CE_DL_STATION_NOT_CONN,m_Header.m_strAddress);
		    return false;
	    case exclusive_established:
            if(IsSignalSet(DOWN_LD_ALL)) {
                m_State = CEDL_STAT_ADAPT_1;
                return true;
            }

            assert(!IsSignalSet(DOWN_LD_ALL));
            AdaptCycle0();

            bUpload = !IsSignalSet(PRJ_NEW) &&
                       IsSignalSet(PRJ_CHANGED);

            if(bUpload) {
                hr = CancelDownload();
                if(FAILED(hr)) {
                    FC_DEBUGPRINT1(_T("CEDL> CancelOnlineChange failed 0x%x\n"),hr);
                    ThrowCSCException(hr);
                }
                m_State = CEDL_STAT_DIR;
                return true;
            }

            m_State = CEDL_STAT_ADAPT_1;
		    return true;
	}

    return false;
}

void CStation::ThrowCSCException(HRESULT hr)
{
    CString strDescr;
    IErrorInfo* pInfo = NULL;

    if(hr == E_OUTOFMEMORY) {
        _com_raise_error(hr);
    }

    FormatMsg(strDescr.GetBuffer(_MAX_PATH),hr);
    strDescr.ReleaseBuffer();

    if(strDescr.IsEmpty()) {
	    GetErrorDescription(hr,strDescr.GetBuffer(_MAX_PATH),_MAX_PATH);
        strDescr.ReleaseBuffer();
    }

    if(strDescr.IsEmpty()) {
        strDescr.Format(IDS_CEDL_DEFERROR,hr);
    }

    ComErrorInfo(CLSID_FCCommissioningSession,__uuidof(IFCDownload),&pInfo,strDescr);
    _com_raise_error(hr,pInfo);
}

//------------------------------------------------------------------*
/**
 * Function called only for EMBEDDED targets
 *
 * Processes parsing of one domain line within target specific 
 * loader list file and performs binary comparision of 4CCG 
 * generated file with downloader database if the domain is not new,
 * and checks if online change is possible
 * @return			0: last domain in loader list file reached
 *					1  more domain lines in loader list to process.
 *				   -1  new domain contains to many changes 
					   online changes not possible 
 * @exception		CMemoryException , CDaoException , CFileException
 * @see			
*/
int CStation::OnlineChangeProcessNextDomain(CString& sReason)
{
	CDomRS rs(m_pDB), rsTemp;
	CString strTemp,strSQL,strPath,strDLPath;
	HRESULT hr;
	BOOL bDiff = FALSE;

	if(!ReadDomainLine(rsTemp)) 
		return 0;
	
	strTemp= _T("SELECT * FROM Domain WHERE ConfId = %d AND DomainName = '%s'");
	strSQL.Format(strTemp,m_lConfId,rsTemp.m_DomainName);

	rs.Open(dbOpenDynaset, strSQL);
	if(rs.IsEOF()) { //new Domain
		rs.Close();
		return 1;
	}

    //update the path
    strPath = rsTemp.m_Path;
	strDLPath = m_strConfigPath + _T('\\');
	strDLPath = strDLPath + rs.m_DLFilePath;

	rs.Close();
    
	//domain not new 
	if(!(rs.m_Flags & DOM_NEW)) {
		hr = UTIL_IsAbsolutePath(strPath);
		if(GetScode(hr) == S_FALSE) { //no absolute path
			strPath = m_strGenPath + _T('\\') + strPath;	
		}

		hr = CompareFilesForOnlineChange(strPath, strDLPath);
		switch(GetScode(hr)) {
			case S_OK:
				return 1;				
			case S_FALSE:
				sReason.Format(_T("Changes in domain %s are not supported."), rsTemp.m_DomainName);
				return -1;
			case E_OUTOFMEMORY:
				AfxThrowMemoryException();
				return 0;
			default:
				sReason.Format(_T("Error during computation on domain %s."), rsTemp.m_DomainName);
				return -1;
		}
	}
	return 0;
}



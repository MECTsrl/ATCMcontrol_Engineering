
//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "cedlerror.h"
#include "utilif.h"
#include "dlrs.h"
#include "dlext.h"
#include "Downld.h"
#include "dlstat.h"
#include "dlstlist.h"

CStationList::CStationList(LPCTSTR pszGenPath,LPCTSTR pszDBPath)
	: m_strGenPath(pszGenPath), m_strDBPath(pszDBPath) , m_strVersion(), m_strPrjName()
{
	m_pActDB = NULL;
}

CStationList::~CStationList(void)
{
	DisconnectStations();
	RemoveStations();
	CloseDB();
}

void CStationList::RemoveStations(void)
{
	CStation *pStation;

	while(m_StationList.GetSize()) {
		pStation = m_StationList[0];
		m_StationList.RemoveAt(0);
		pStation->Release();
	}
}

CStation* CStationList::FindStationByAddress(LPCTSTR pszAddress)
{
	CStation *pStation;
	CString strAddr,strConfigName;
	int iSize = m_StationList.GetSize();

	for(int i = 0; i < iSize; i++) {
		pStation = m_StationList[i];
		
		strConfigName = pStation->GetConfigName();
        //TODO rename
		strAddr = pStation->GetAddress();

		if( strAddr == pszAddress) {
			pStation->AddRef();
			return pStation;
		}

	}
	return NULL;
}


void CStationList::DisconnectStations(void)
{
	CStation *pStation;
	int iSize = m_StationList.GetSize();

	for(int i = 0; i < iSize; i++) {
		pStation = m_StationList[i];
		pStation->Detach(true);
	}
}

int CStationList::FindStationIndex(CStation * pStation)
{ //Function does not Addref a station ptr
	CStation *pStat;
	int iSize = m_StationList.GetSize();

	for(int i = 0; i < iSize; i++) {
		pStat = m_StationList[i];
		
		if(pStat == pStation) {
            return i;
        }
	}
	return -1;
}

CStation * CStationList::FindStationByIndex(int iIndex,bool bAddRef)
{ //Function does not Addref a station ptr
    CStation* pStation = NULL;
	int iSize = m_StationList.GetSize();

    if(iIndex < 0 || iIndex >= iSize) {
        return NULL;
    }

    pStation = m_StationList[iIndex];
    if(pStation && bAddRef) { //Station found and bAddRef == true
        pStation->AddRef();
    }
    return pStation;
}

POSITION CStationList::GetHeadPosition ()
{
	if (m_StationList.GetSize () == 0)
	{
		return (NULL);
	};

	return ((POSITION)1);
}


CStation *CStationList::GetNext (POSITION &pPosition)
{
	CStation *pStation;

	pStation = m_StationList[(int)pPosition - 1];

	if ((int)pPosition == m_StationList.GetSize ())
	{
		pPosition = NULL;
	}
	else
	{
		pPosition = (POSITION)((int)pPosition + 1);
	};

	return (pStation);
}

BOOL CStationList::IsDownloadPending(void)
{
    CStation *pStation;
	int iSize = m_StationList.GetSize();

	for(int i = 0; i < iSize; i++) {
		pStation = m_StationList[i];
		
        if(pStation->IsRequestPending()) {
            return TRUE;
        }
    
    }
    return FALSE;
}


/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*   Opens the downloader database for the target.
*   Returns with no action if database for current target is already opened.
*
*   @exception - com_error exception
*   @see        CloseDB()
*/
void CStationList::OpenDB(void)
{
	HRESULT hr = S_OK;

    if(m_strDBPath.IsEmpty()) {
		assert(!m_strDBPath.IsEmpty());
		_com_raise_error(E_FAIL);
	}

	if(m_pActDB)
		return;

	m_pActDB = new CDaoDatabase;

	try	{
		m_pActDB->Open(m_strDBPath);
	}
	catch(CDaoException* e) {
		delete m_pActDB;
        m_pActDB = NULL; //don't forget to reset the pointer
        throw (e); //and propagate it
	}
}



//------------------------------------------------------------------*
/**
 * Closes the downloader database and resets database reference to 
 * NULL. No action if the database is already closed
 * @see	    OpenDB()
*/
void CStationList::CloseDB(void)
{
	if(!m_pActDB) {
		return;
	}

	m_pActDB->Close();
	delete m_pActDB;
	m_pActDB = NULL;

}


/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*   Checks the version of the downloader database for the current target.
*   @return -   true if the version equals the database version defined
*               false if not.
*   @exception - _com_error, CMemoryException, CDaoException
*/
bool CStationList::CheckDBVersion(void)
{
    CString strVersion;

    if(m_strDBPath.IsEmpty()) {
	    assert(!m_strDBPath.IsEmpty());
        _com_raise_error(E_FAIL);
    }

    if(!IsDBOpen()) {
        _com_raise_error(E_FAIL);
    }


    GetDBVersion(strVersion);
    if(_ttoi((LPCTSTR)strVersion) == CE_DLDatabaseVersion) 
        return true;
    else 
        return false;
}

bool CStationList::GetDBVersion(CString & strVersion)
{
    bool bResult = false;
    assert(m_pActDB);

    CSysAttrRS  SysAttribRS(m_pActDB);

    SysAttribRS.m_strFilter.Format(_T("SysAttrName = 'CE_DatabaseVersion'")); 
    SysAttribRS.Open();
    if(!SysAttribRS.IsEOF()) {
        strVersion = SysAttribRS.m_SysAttrValue;
        bResult = true;
    }

    SysAttribRS.Close();
    return bResult;
}

//------------------------------------------------------------------*
/**
 * Determines whether the database is empty or not.
 * @return          BOOL - TRUE if database is empty, otherwise FALSE.
 * @exception       CMemoryException,CDaoException
 * @see             
*/
BOOL CStationList::IsDBEmpty()
{
	CDaoTableDefInfo tabledefinfo;

	m_pActDB->GetTableDefInfo(CEDL_CONFIGTABLE,tabledefinfo,AFX_DAO_ALL_INFO);
	if (!tabledefinfo.m_lRecordCount)
		return TRUE;
	else
		return FALSE;			
}


/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*   Create or initialize station list.
*   Description = If no station list exists it will be created.
*   An existing station list will be incrementally reconfigured.
*   @param      Event interface pointer
*   @param      Message interface pointer.
*   @param      bSilent - display message boxes ?
*   @exception  _com_error, CMemoryException
*/
void CStationList::BuildStationList(BSTR sTargetName,PIDLEvents pEvents, PICEMsg pMsgSink, bool bSilent)
{
	CString strFile;
    CHeaderInfo Info;
	HRESULT hr = S_OK;
    int i;
	CStation *pStation = NULL;

	MarkStationsDelete();

    Info.m_strTargetName = sTargetName;

	strFile = m_strGenPath + _T('\\');
    strFile += Info.m_strTargetName + _T('\\');
	strFile += CEDL_LISTFILE;

	CCsvParse Parse(strFile);
	hr = Parse.Open();
	if(FAILED(hr)) {
        CFileException::ThrowOsError(hr,strFile);
	}

    CStation::ParseHeader(Info,Parse);
    Parse.Close();

	m_strPrjName = Info.m_strPrjName;
	m_strVersion = Info.m_strVersion;

    pStation = FindStationByAddress(Info.m_strAddress);
    if(!pStation) {//not found
        pStation = new CStation(Info,pEvents,pMsgSink,bSilent);
		if(!pStation) {
			AfxThrowMemoryException();
		}

        pStation->AddRef();
		m_StationList.Add(pStation);
        pStation->UpdateConfigurations(Info,m_pActDB,m_strGenPath);
        pStation->ConnectVars();
    }
    else { //station found - AddRef'd station
        pStation->UpdateConfigurations(Info,m_pActDB,m_strGenPath);
        pStation->ResetSignal(DEL_STATION);
        pStation->ReconnectVars();
        pStation->Release();
        
    }
	
    //check wether a station found - otherwise this is an error
    if(!pStation) {
        ComThrow(CLSID_CEDL,__uuidof(IDL),E_CE_DL_STATION_NOT_FOUND);
    }

	/*delete the marked configs that don't have the prj guid in the
	PrjGuid field */
	DeleteUnusedStations();

	/* check whether the domain table is empty */
	for(i=0;i< m_StationList.GetSize(); i++) {
		pStation = m_StationList[i];
		assert(pStation);
		pStation->OnEmptyDomainTable();
	}
}

void CStationList::MarkStationsDelete(void)
//throws CMemoryException and CDaoException
{
	CConfRS rs(m_pActDB);
	CString strTemp,strSQL;
    CStation *pStation;
	int iSize = m_StationList.GetSize();

	for(int i = 0; i < iSize; i++) {
		pStation = m_StationList[i];
        assert(pStation);
        
        pStation->SetSignal(DEL_STATION);
    }

	strSQL = _T("SELECT * FROM Configuration");
	rs.Open(dbOpenDynaset, strSQL);
	if((!rs.IsEOF()) && rs.CanAppend()) {
        // set the ChildIds of the InstanceTable to 0
        strSQL=  _T("UPDATE Configuration SET Version = 'DEL'");
        m_pActDB->Execute(strSQL);

	}  
	rs.Close();
}

void CStationList::DeleteUnusedStations(void)
//throws CMemoryException and CDaoException
{
	CConfRS rs(m_pActDB);
	CString strTemp,strSQL;
    CStation *pStation;

	for(int i = 0; i < m_StationList.GetSize();) {
		pStation = m_StationList[i];
        assert(pStation);
        
        if(pStation->IsSignalSet(DEL_STATION)) {
		    m_StationList.RemoveAt(i);
            //pStation->Disconnect();
            pStation->Detach();
		    pStation->Release();
        }
        else {
            i++;
        }
    }
	
	strTemp = _T("SELECT * FROM Configuration WHERE (Version <> '%s')");
	strSQL.Format(strTemp,m_strVersion);

	rs.Open(dbOpenDynaset, strSQL);
	while(!rs.IsEOF()) {
		if(rs.CanAppend()) {
		// Now delete the file (the DataBase will also delete all related records)
			rs.Delete();
		}
		if(!rs.IsEOF())		
			rs.MoveNext();
	}  
	rs.Close();
}


/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*   Copies the base template database in the downloader directory of the current project.
*   Description = Copies the base template database in the downloader directory 
*   of the current project. If the database already exists it will only be overwritten
*   iff bForce is set.
*   @param  - target specific gen path. shall not be NULL
*   @param  - bForce - see above.
*   @exception - CMemoryException, _com_error exception
*   @see
*/
void CStationList::CopyDatabase(CString & strTarget,BSTR sGenPath,bool bForce)
{
    CString strSrc,strDest;
    CFileStatus rStatus;
    bool bCopy;
    DWORD dwError;

    if(sGenPath == NULL) {
        _com_raise_error(E_POINTER);
    }

    GetDBBasePath(strSrc);

    //check wether the downloader database CEDL.MDB exists
    GetDBPath(strDest,strTarget,sGenPath);

    bCopy = false;
	if(!CFile::GetStatus(strDest,rStatus)) {
        //does not exist
        bCopy = true;
	}
    else { //database found delete it if bForce is true
        if(bForce) {
            ::DeleteFile(strDest);
            bCopy = true;
        }
    }

    if(bCopy) {
	    //database not found copy template database from install path
	    if(!CopyFile(strSrc,strDest,TRUE)) {
            dwError = ::GetLastError();
            CFileException::ThrowOsError(dwError,strSrc);
	    }
    }

}

/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*   Returns path to the (base)template for the downloader database.
*   @exception - CMemoryException,_com_error
*/
void CStationList::GetDBBasePath(CString & strPath)
{
    HRESULT hr = UTIL_GetModulePath(AfxGetInstanceHandle(),
        strPath.GetBuffer(_MAX_PATH),_MAX_PATH);
    strPath.ReleaseBuffer();

	if(FAILED(hr))
		_com_raise_error(hr);

    strPath += CEDL_BASEDB;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*   Returns path to the projects downloader database.
*   @param      strPath - the path to the downloader database will be returned there.
*   @param      sGenPath - path to 4CCG generation path.
*   @exception - CMemoryException
*/
void CStationList::GetDBPath(CString & strPath,CString & strTarget,BSTR sGenPath)
{
	strPath = sGenPath;
	strPath = strPath + _T('\\') + strTarget + _T('\\') + CEDL_DOMDIR + 
        _T('\\') + CEDL_DB;
}


//TODO Obsolete ?
HRESULT CStationList::DelUnusedDomainFiles(CString & strPath)
/*	deletes unused domain files from the internal domain dir.
	the internal domain dir is the dir "CEDL" under gen path.

	throws CFileException,CDaoException,CMemoryException.
*/
{
	WIN32_FIND_DATA FindData;
    HANDLE hFind;
	HRESULT hr;
    DWORD dwErr;
	CString strTemp,strSQL,strHelp;
	LPTSTR pszExt;
    CDomRS rs(m_pActDB);

	strHelp = strPath + _T("*.*");		
	strTemp = _T("SELECT * FROM Domain WHERE DLFilePath = '%s'");

    hFind = FindFirstFile(strHelp,&FindData);
    if(hFind == INVALID_HANDLE_VALUE) {
		CFileException::ThrowOsError(GetLastError());
    }

    do{
        if(!(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {

			pszExt = strHelp.GetBuffer(_MAX_EXT);
			
			hr = UTIL_GetFileExt(FindData.cFileName,pszExt,_MAX_EXT);
			
			strHelp.ReleaseBuffer();
			if(FAILED(hr)) {
				FindClose(hFind);
				return hr;
			}

            if(!strHelp.CompareNoCase(_T("tmp"))) { //it is a temp downloader file
				strHelp = strPath + FindData.cFileName;
				
				//try to find in domain table
				strSQL.Format(strTemp,strHelp);
				rs.Open(dbOpenDynaset, strSQL);			
				
				if(rs.IsEOF()) {//not found
					
					FC_DEBUGPRINT1(_T("CEDL> Delete domain file %s\n"),strHelp);
			
					if(!DeleteFile(strHelp)) {
                        dwErr = ::GetLastError();
						rs.Close();
						FindClose(hFind);
						CFileException::ThrowOsError((LONG) dwErr);
					}
				}
				rs.Close();
			}
        }     

        if(!FindNextFile(hFind,&FindData)) {
            if((dwErr = GetLastError()) == ERROR_NO_MORE_FILES)
                break;
            else {
				FindClose(hFind);
				CFileException::ThrowOsError(dwErr);
            }
        }
    } while(TRUE);

    FindClose(hFind);
    return S_OK;
}


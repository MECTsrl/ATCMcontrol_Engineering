
//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "dlext.h"
#include "downld.h"
#include "dlstat.h"

BOOL CStation::AddNewDomain(CDomRS & rsVal,CEDL_DOM_FLAG flag)
//throws CMemoryException , CDaoException , CFileException
//adds a new domain to the domain table
{
	CDomRS rs(m_pDB);
	CString strTemp,strPath;
	long lDomainId;
	
	rs.Open(); //open recordset
	if(!rs.CanAppend()) {
		rs.Close();
		return FALSE;
	}

	rs.m_DomainId = 0;
	rs.AddNew();
	rs.m_ConfId = m_lConfId;
	rs.m_DomainName = rsVal.m_DomainName;
	rs.m_Flags = flag;
	rs.m_Path = rsVal.m_Path;
	rs.m_VersionId = rsVal.m_VersionId;
	rs.m_DLFilePath = _T("");
	rs.Update();
	rs.SetBookmark(rs.GetLastModifiedBookmark());
	lDomainId = rs.m_DomainId;

	if(!rs.CanUpdate()) {
		rs.Close();
		return FALSE;
	}

	strTemp.Format(_T("%s%d.tmp"),CEDL_DOMFILE,lDomainId);
		//update the DLFilePath member
	rs.Edit();
	rs.m_DLFilePath = CEDL_DOMDIR;
	rs.m_DLFilePath += _T('\\') + strTemp;
	rs.Update();

	//give back the downloader file path
	rsVal.m_DLFilePath = rs.m_DLFilePath;
	//and the version id
	rsVal.m_VersionId = rs.m_VersionId;
	rs.Close();
	return TRUE;
}

BOOL CStation::UpdateDomain(CDomRS & rsVal)
{
	CDomRS rs(m_pDB);
	CString strTemp,strSQL;

	strTemp= _T("SELECT * FROM Domain WHERE ConfId = %d AND DomainName = '%s'");
	strSQL.Format(strTemp,m_lConfId,rsVal.m_DomainName);

	rs.Open(dbOpenDynaset, strSQL);
	if(rs.IsEOF()) { //new Domain
		rs.Close();
		return 
			AddNewDomain(rsVal,
				(CEDL_DOM_FLAG) (DOM_CHANGED | DOM_NEW | DOM_SELECTED));
	}

	if(!rs.CanUpdate()) {
		return FALSE;
	}

	rs.Edit();
	rs.m_DomainName = rsVal.m_DomainName;
	rs.m_Path = rsVal.m_Path;

	//only iff the version id is not empty update it
	if(!rsVal.m_VersionId.IsEmpty()) {
		rs.m_VersionId = rsVal.m_VersionId;
	}
	
	rs.m_Flags = rsVal.m_Flags;
	rs.Update();
	//give back the downloader file path
	rsVal.m_DLFilePath = rs.m_DLFilePath;
	rsVal.m_VersionId = rs.m_VersionId;
	rs.Close();
	return TRUE;
}

BOOL CStation::GetDomain(LPCTSTR pszDomainName, CDomRS & rsVal)
{
	CDomRS rs(m_pDB);
	CString strTemp,strSQL;

	strTemp= _T("SELECT * FROM Domain WHERE ConfId = %d AND DomainName = '%s'");
	strSQL.Format(strTemp,m_lConfId,pszDomainName);

	rs.Open(dbOpenDynaset, strSQL);
	if(rs.IsEOF()) { //new Domain
		rs.Close();
		return FALSE;
	}

	rsVal.m_ConfId = rs.m_ConfId ;
	rsVal.m_DomainId = rs.m_DomainId;
	rsVal.m_DLFilePath = rs.m_DLFilePath;
	rsVal.m_DomainName = rs.m_DomainName;
	rsVal.m_Flags = rs.m_Flags;
	rsVal.m_Path = rs.m_Path;
	rsVal.m_VersionId = rs.m_VersionId;
	rs.Close();
	return TRUE;
}




//------------------------------------------------------------------*
/**
 * Marks all domains with flag DOM_DELETED and resets flags
 * DOM_CHANGED and DOM_SELECTED. If parameter pUpNew is set
 * flag DOM_UPNEW will also be set.
 * @param			bool bUpNew
 * @return			void 
 * @exception		CDaoException,CMemoryException
 * @see				MarkDomainsNew()
*/
void CStation::MarkDomains(bool bUpNew)
{
	CDomRS rs(m_pDB), rsTemp;
	CString strTemp,strSQL;
	
	assert(m_lConfId != -1);

	strTemp = _T("SELECT * FROM Domain WHERE ConfId = %d");
	strSQL.Format(strTemp,m_lConfId);

	rs.Open(dbOpenDynaset, strSQL);
	while(!rs.IsEOF()) {
		//skip the version domain
		if((rs.m_DomainName != m_strVersionDomain) &&
			rs.CanUpdate()) {

			rs.Edit();
			rs.m_Flags |= DOM_DELETED;
			rs.m_Flags &= ~DOM_CHANGED;
			rs.m_Flags &= ~DOM_SELECTED;
            if(bUpNew) {
                rs.m_Flags |= DOM_UPNEW;
            }
			rs.Update();
		}

		if(!rs.IsEOF())		
			rs.MoveNext();
	}
	rs.Close();
}



//------------------------------------------------------------------*
/**
 * Marks all domains for new domains by setting domain flag DOM_NEW.
 * @exception	    CDaoException, CMemoryException		
 * @see			MarkDomains()
*/
void CStation::MarkDomainsNew()
{
	CDomRS rs(m_pDB), rsTemp;
	CString strTemp,strSQL;
	
	assert(m_lConfId != -1);

	strTemp = _T("SELECT * FROM Domain WHERE ConfId = %d");
	strSQL.Format(strTemp,m_lConfId);

	rs.Open(dbOpenDynaset, strSQL);
	while(!rs.IsEOF()) {
		//skip the version domain
		if(rs.CanUpdate()) {
			rs.Edit();
            rs.m_Flags |= DOM_NEW;
            rs.Update();
		}
		if(!rs.IsEOF())		
			rs.MoveNext();
	}
	rs.Close();
}


void CStation::DeleteMarkedDomains()
{
	CDomRS rs(m_pDB);
	CString strTemp,strSQL;
    CFileStatus FileStatus;
    LONG lErr;
	
	assert(m_lConfId != -1);
	strTemp = _T("SELECT * FROM Domain WHERE ConfId = %d AND (Flags = %d OR Flags = %d)");
	strSQL.Format(strTemp,m_lConfId,DOM_DELETED,(DOM_NEW | DOM_DELETED));

	rs.Open(dbOpenDynaset, strSQL);
    while(!rs.IsEOF()) {
		if(rs.CanAppend()) {

			FC_DEBUGPRINT1(_T("CEDL> Delete domain %s\n"),
				rs.m_DomainName);
			//try to delete the file
			strTemp = m_strConfigPath + _T('\\');
			strTemp = strTemp + rs.m_DLFilePath;
        
            // Now delete the file (the DataBase will also delete all related records)
            if(CFile::GetStatus(strTemp,FileStatus)) {
			    if(!DeleteFile(strTemp)) {
                    lErr = (LONG)::GetLastError();
				    rs.Close();
				    CFileException::ThrowOsError(lErr);
			    }	
            }
			
			rs.Delete();
		}
		if(!rs.IsEOF())		
			rs.MoveNext();
	}  
	rs.Close();
}

BOOL CStation::GetDLGUID(LPCTSTR pszVersionDomain,CString & strGUID)
{
    CDomRS rsTemp;

    strGUID.Empty();
    if(!GetDomain(pszVersionDomain, rsTemp)) {
        return FALSE;
    }

    strGUID = rsTemp.m_VersionId;
    return TRUE;
}


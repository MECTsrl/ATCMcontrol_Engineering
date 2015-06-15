
#ifndef DLSTLIST_H
#define DLSTLIST_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

class CStationList
{
    /** list of stations*/
	CArray<CStation *,CStation *> m_StationList;	
    /** path of downloader database*/		
	CString			m_strDBPath;		
	/**gen path from 4CCG */
    CString			m_strGenPath;
    /**project version*/
	CString			m_strVersion;
    /**project name*/
	CString			m_strPrjName;
    /**database*/
	CDaoDatabase*	m_pActDB;			
public: 
	CStationList(LPCTSTR pszGenPath,LPCTSTR pszDBPath);
	virtual ~CStationList(void);

	void    OpenDB(void);
	void	CloseDB(void);
	BOOL	IsDBEmpty(void);
    inline BOOL IsDBOpen(void)
    {
        if(!m_pActDB) {
            return FALSE;
        }
        return m_pActDB->IsOpen();
    }

    bool    CheckDBVersion(void);


	void        BuildStationList(BSTR sTargetName, PIDLEvents pEvntSink,PICEMsg pMsgSink, bool bSilent);
	CStation*   FindStationByAddress(LPCTSTR pszCCAddr);
    int         FindStationIndex(CStation *); //index of a certain station
    CStation*   FindStationByIndex(int iIndex,bool bAddRef = false);//find station by index

	POSITION GetHeadPosition ();
	CStation *GetNext (POSITION &pPosition);

    inline LPCTSTR GetVersion(void) const
    {
        return m_strVersion;
    }

    BOOL    IsDownloadPending(void);

    static void CopyDatabase(CString & strTarget,BSTR sGenPath,bool bForce);
    static void GetDBBasePath(CString &strPath);
    static void GetDBPath(CString & strPath,CString & strTarget,BSTR sGenPath);
private:
	void	RemoveStations(void);	//deletes station list
	void	DeleteUnusedStations(void); //removes unused stations/configs
	void	MarkStationsDelete(void);	//mark all stations/configs for delete
	HRESULT	DelUnusedDomainFiles(CString & strPath);
	void    DisconnectStations(void);
    bool    GetDBVersion(CString&);

}
;

#endif //DLSTLIST_H

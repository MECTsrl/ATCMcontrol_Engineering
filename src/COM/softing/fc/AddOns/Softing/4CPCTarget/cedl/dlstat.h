
#ifndef DLSTAT_H
#define DLSTAT_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "dlrs.h"
#include "dlcsc.h"
#include "dlstinfo.h"
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*
class CHeaderInfo
{
public:
	CString m_strVersion;
    CString m_strConfName;
	CString m_strResName;
	CString m_strAddress;
	CString m_strPrjName;
	CEDL_TARGET_TYPE m_Type;
    CString m_strTargetName;
    CString m_strTargetType;
    CHeaderInfo();
};

class CStation : public CRefObj
{
	long	m_lConfId;
    CHeaderInfo m_Header;
    CString m_strDLGuid;
    CString m_strVisDLGuid;
    /**gen path*/
	CString m_strGenPath;
    /** path to loader.list*/
    CString m_strConfigPath;    
	CString m_strUsrName;
    /** station info block */
    CStationInfo m_StationInfo; 
    /** internal version domain*/
	CString m_strVersionDomain;	
    //type of transfer target
	CSC_TRANSFER_TARGET m_Target;
	
    /** database reference*/
	CDaoDatabase* m_pDB;
    /** CSV parser object */
	CCsvParse m_Parse;		
    /**signal for pending commands etc.*/
    unsigned short m_Signal;
    /** options for download */
    unsigned short m_options; 
    /** download state */
	CEDL_STAT m_State;
    /** thread id of the thread constructing the object*/
	DWORD m_dwThreadId;	
    /** event sink*/
    PIDLEvents m_pEvntSink; 
    /** message event sink if: _ICEMessageEvent */
    PICEMsg m_pMsgSink;     
	/** notification sink object for file transfer*/
	CComObject<CImpTransferNotify>* m_pTransNotify;
	/** session object of 4CSC - CSC_Online */
    IFCDownload*  m_pDLSession;
	IFCConnect*	m_pConnect;
	DWORD m_dwTransferCookie;
	DWORD m_dwUploadCookie;
    DWORD m_dwDownloadCookie;
    DWORD m_dwDelCookie;

    /** failed flag for notifications*/
	HRESULT m_hr;
    /** if true open no message boxes (used fom command line) */
    bool   m_bSilent;
public:
	CStation(CHeaderInfo & Info, PIDLEvents pEvntSink, PICEMsg pMsgSink, bool bSilent);
	virtual ~CStation();

    void UpdateConfigurations(CHeaderInfo &,CDaoDatabase*,CString& strGenPath);

    static void ParseHeader(CHeaderInfo & , CCsvParse &);

	void OnEmptyDomainTable(void);

    HRESULT DoStateMachine(void);

    //connection handling
	void Attach(LPUNKNOWN pUnk);
	void Detach(bool bForce = false);
	bool IsConnected(void);

    HRESULT ConnectVars(void);
    HRESULT ReconnectVars(void);

	//Download
    void MarkDomainsNew();

	//misc
    BOOL GetDLGUID(LPCTSTR pszVersionDomain,CString &);
    BOOL isSilent()const{return m_bSilent;}
    HRESULT Abort(void);
    //event methods
    void FireError(HRESULT hr);
    void FireErrorMessage(HRESULT hr,LPCTSTR pszDescr = NULL);
    void FireErrorMessage(HRESULT hr,BSTR);
    void FireStartDownload();

    //get methods
	LPCTSTR GetVersion(void) const
    {
        return m_Header.m_strVersion;
    }

    LPCTSTR CStation::GetAddress(void) const
    {
	    return m_Header.m_strAddress;
    }

	LPCTSTR CStation::GetConfigName(void) const
    {
	    return m_Header.m_strConfName;
    }

    LPCTSTR CStation::GetProjectName() const
    {
        return m_Header.m_strPrjName;
    }

    CStationInfo & CStation::GetStationInfo(void)
    {
        return m_StationInfo;
    }

    const long CStation::GetConfId(void) const
    {
	    return m_lConfId;
    }

    CDaoDatabase* CStation::GetDomainDBPtr(void)
    {
        return m_pDB;
    }

    LPCTSTR CStation::GetDLGuid () const
    {
	    return m_strDLGuid;
    }

    //set methods
    void CStation::SetSignal(CEDL_SIGNAL signal)
    {
        m_Signal |= signal;
    }

    void CStation::ResetSignal(CEDL_SIGNAL signal)
    {
        m_Signal &= ~signal;
    }

    BOOL CStation::IsSignalSet(CEDL_SIGNAL signal)
    {
        return m_Signal & signal;
    }

    void CStation::SetState(CEDL_STAT state)
    {
        m_State = state;
    }

    bool CStation::IsRequestPending(void)
    {
        if(IsSignalSet(BUILD_DL_LIST) || 
           IsSignalSet(DOWN_LD)) {
            return true;
        }
        return false;
    }


    //------------------------------------------------------------------*
    /**
     * Sets download options passed with IDL::DownloadTarget().
     * @param			CEDL_DOWNLD_OPTIONS options - options according
     *                  to enumeration CEDL_DOWNLD_OPTIONS (see cedl.idl).
     * @see			    enumeration CEDL_DOWNLD_OPTIONS in cedl.idl.
    */
    void CStation::SetOptions(CEDL_DOWNLD_OPTIONS options)
    {
        m_options = options;
    }


    //------------------------------------------------------------------*
    /**
     * Checks to see whether a specific option is set with the options 
     * passed to IDL::DownloadTarget().
     * @param			CEDL_DOWNLD_OPTIONS option  - option according
     *                  to enumeration CEDL_DOWNLD_OPTIONS (see cedl.idl).
     * @return			BOOL - TRUE if options set, FALSE otherwise.
     * @see			    enumeration CEDL_DOWNLD_OPTIONS in cedl.idl.
    */
    BOOL CStation::IsOptionSet(CEDL_DOWNLD_OPTIONS option)
    {
        return m_options & option;
    }

private:
	void GetSessionFileName(CString & strFile, LPCTSTR pszFileTemplate);
	void MarkDomains(bool);
	BOOL AddNewDomain(CDomRS & rsVal,CEDL_DOM_FLAG flag);
	BOOL UpdateDomain(CDomRS & rsVal);
    BOOL GetDomain(LPCTSTR pszDomainName, CDomRS & rsVal);

	void WriteVersionDomain(void);
    void WriteDownloadFile(LPCTSTR pszFileName,short nFlag);
    
    //connection handling
	HRESULT AdviseSession(void);
	HRESULT UnAdviseSession(void);
    bool    CheckConnection();

    /** Adaption cycle 0 - compare loaded project-GUID against initted 
    project-GUID*/
    void AdaptCycle0(void);

	//Adaption cycle 1 (Upload from control)
	void    UploadDirectory(LPCTSTR);  //upload version domain __.list from control
    bool    UploadDomains(LPCTSTR);
	void    CompareDomDirs(LPCTSTR pszUpldFile);
	void    FinalizeCycle1();

	//Adaption cycle 2
	void AdaptCycle2(void);
	BOOL AdaptOnlineChangeCycle2(void);
	BOOL ProcessNextDomain();
	BOOL ReadDomainLine(CDomRS & rs);

	//OnlineChanges helper funtions
	int     OnlineChangeProcessNextDomain(CString& sReason);

	//Download
    void    StartDownload();
    bool    DeleteDomains(LPCTSTR pszDelFile);
    void    CommitDownload(BOOL);
    HRESULT CancelDownload();
    void    DeleteMarkedDomains();

    //event methods and error handling
    void FireUploadReady(void);
    void FireProjectNew();
    void FireDLState(BSTR sDomain, float fPercentage, CEDL_DOWNLD_STATE State,
        HRESULT hrErr = S_OK,bool bMessage = true);
    void FireTransferError(LPCTSTR, CSC_TRANSFER_STATE,HRESULT);
    void FireFileErrorMessage(const CFileException & e);
    void FireDaoErrorMessage(const CDaoException & e);

    static void ThrowCSCException(HRESULT);
	//misc
    void FormatMsgDispatch(CString &,HRESULT,bool bHeader = true,LPCTSTR pszDescr = NULL);
	BOOL CheckThreadId(DWORD dwThreadId);
    void OnError(HRESULT hr);
	HRESULT OnAborting(void);
	void GetLoginUser(CString & strUser);

    void CStation::ResetCookies(void)
    {
	    m_dwUploadCookie = 0;
        m_dwDownloadCookie = 0;
        m_dwDelCookie = 0;
    }
   

friend class CImpTransferNotify;
};








#endif //DLSTAT_H


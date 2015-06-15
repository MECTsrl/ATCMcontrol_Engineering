
//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "cfacility.h"
#include "cedlerror.h"
#include "dlrs.h"
#include "dlext.h"
#include "Downld.h"
#include "dlstat.h"
#include "utilif.h"

void CStation::FireError(HRESULT hr)
{
    m_hr = hr;
    OnError(hr);
}

void CStation::FireErrorMessage(HRESULT hr,LPCTSTR pszDescr)
{
    CString strDescr;
    CComBSTR sDescr,sTarget;

    if(m_pMsgSink) {
        FormatMsgDispatch(strDescr,hr,true,pszDescr);
        sDescr = strDescr;
        sTarget = m_Header.m_strTargetName;
        m_pMsgSink->Fire_OnNewMessage(sTarget,E_FACILITY_CEDL,hr,sDescr);
    }

}

void CStation::FireErrorMessage(HRESULT hr,BSTR sDescr)
{
    CComBSTR s;
    if(m_pMsgSink) {
        s = m_Header.m_strTargetName;
        m_pMsgSink->Fire_OnNewMessage(s,HRESULT_FACILITY(hr),hr,sDescr);
    }
}


void CStation::FormatMsgDispatch(CString & strOut,HRESULT hr, bool bHeader, LPCTSTR pszDescr)
{
    CString strDescr;
    CString strTemp;

    strDescr.Empty();
    strTemp.Empty();

    switch(HRESULT_FACILITY(hr)) {
        case E_FACILITY_CEDL: 
            if(!pszDescr) {
                FormatMsg(strDescr.GetBuffer(_MAX_PATH),hr,m_Header.m_strTargetName);
            }
            else {
                FormatMsg(strDescr.GetBuffer(_MAX_PATH),hr,m_Header.m_strTargetName,pszDescr);
            }
    
            strDescr.ReleaseBuffer();
            break;
        case E_FACILITY_SYSCOM:
			GetErrorDescription(hr,strDescr.GetBuffer(_MAX_PATH),_MAX_PATH);
            strDescr.ReleaseBuffer();

            if(strDescr.IsEmpty()) {
                strDescr.Format(IDS_CEDL_DEFERROR,hr);
            }

            if(bHeader) {
                strTemp.Format(IDS_CEDL_MSGHEADER,m_Header.m_strTargetName);
            }
            strDescr = strTemp + strDescr;
            break;
        default:
            FormatMsg(strDescr.GetBuffer(_MAX_PATH),hr);
            strDescr.ReleaseBuffer();

            if(strDescr.IsEmpty()) {
                strDescr.Format(IDS_CEDL_DEFERROR,hr);
            }

            if(bHeader) {
                strTemp.Format(IDS_CEDL_MSGHEADER,m_Header.m_strTargetName);
            }
            strDescr = strTemp + strDescr;
            break;
    }

    strOut = strDescr;
}

void CStation::FireTransferError(LPCTSTR pszDomain, CSC_TRANSFER_STATE state,HRESULT hrErr)
{
    CString strHelp,strDescr;
    CComBSTR sDescr,sTarget;
    HRESULT hr;

    if(m_pMsgSink) {
        FormatMsgDispatch(strHelp,hrErr,false);

        switch(state) {    
            case deleted_domain:
                hr = S_CE_DL_DEL_ERROR;
                FormatMsg(strDescr.GetBuffer(_MAX_PATH),hr,m_Header.m_strTargetName,
                    pszDomain,strHelp);
                strDescr.ReleaseBuffer();
                break;
            case transfering_domain: 
                hr = S_CE_DL_TRANSFER_ERROR;
                FormatMsg(strDescr.GetBuffer(_MAX_PATH),hr,m_Header.m_strTargetName,
                    pszDomain,strHelp);
                strDescr.ReleaseBuffer();
                break;
            default:
                //?? try to message this 
                hr = hrErr;
                strDescr = strHelp;
                break;
        }

        sDescr = strDescr;
        sTarget = m_Header.m_strTargetName;
        m_pMsgSink->Fire_OnNewMessage(sTarget,E_FACILITY_CEDL,hr,sDescr);
    }
}


void CStation::FireUploadReady(void)
{
    if(m_pMsgSink) {
        CString strDescr;
        CComBSTR sDescr;
        CComBSTR sTarget;

        FormatMsg(strDescr.GetBuffer(_MAX_PATH),S_CE_DL_DOWNLD_UPLD_READY,m_Header.m_strTargetName);
        strDescr.ReleaseBuffer();

        sDescr = strDescr;
        sTarget = m_Header.m_strTargetName;
        m_pMsgSink->Fire_OnNewMessage(sTarget,E_FACILITY_CEDL,
            S_CE_DL_DOWNLD_UPLD_READY,sDescr);
    }
}

void CStation::FireDLState(BSTR sDomain, float fPercentage, CEDL_DOWNLD_STATE State, HRESULT hrErr, bool bMessage)
{
    CString strDomain,strDescr;
    CComBSTR sHelp;
    CComBSTR sTarget;
    LPTSTR pszBuffer = NULL;
    HRESULT hr = S_OK;

    if(State == DOWNLD_READY ) {
        m_State = CEDL_STAT_IDLE;
    }
    
    sTarget = m_Header.m_strTargetName;

    if(m_pMsgSink && bMessage) {
        pszBuffer = strDescr.GetBuffer(_MAX_PATH);
        if (sDomain) {
            strDomain = sDomain;
        }
    
        switch(State) {
            case DOWNLD_READY:
                hr = S_CE_DL_DOWNLD_READY;
                FormatMsg(pszBuffer,hr,m_Header.m_strTargetName);
                break;
            case DOWNLD_ABORTED:
                hr = S_CE_DL_DOWNLD_ABORT;
                FormatMsg(pszBuffer,hr,m_Header.m_strTargetName);
                break;
            case DOWNLD_TRANSFER:
                hr = S_CE_DL_DOWNLD_TRANSFER;
                FormatMsg(pszBuffer,hr,m_Header.m_strTargetName,strDomain);
                break;
            case DOWNLD_UPLOAD:
                hr = S_CE_DL_DOWNLD_UPLD;
                FormatMsg(pszBuffer,hr,m_Header.m_strTargetName,strDomain);
                break;
            case DOWNLD_INTR:
				hr = S_CE_DL_DOWNLD_CONN;
				FormatMsg(pszBuffer,hr,m_Header.m_strTargetName);
                break;
            case DOWNLD_ERROR:
                {   CString strHelp;
            
                    hr = S_CE_DL_DOWNLD_ERROR;
                    strHelp.Format(_T("%x"),hrErr);
                    FormatMsg(pszBuffer,hr,m_Header.m_strTargetName,strHelp);
                }
                break;
            case DOWNLD_DEL:
                hr = S_CE_DL_DOWNLD_DEL;
                FormatMsg(pszBuffer,hr,m_Header.m_strTargetName,strDomain);
                break;
            default:
                break;
        }

        strDescr.ReleaseBuffer();

        sHelp = strDescr;
        m_pMsgSink->Fire_OnNewMessage(sTarget,E_FACILITY_CEDL,hr,sHelp);
    }

    if(m_pEvntSink) {
        sHelp.Empty();
        if(!sDomain) {
            sHelp = strDomain;
        }
        else {
            sHelp = sDomain;
        }
        m_pEvntSink->Fire_OnDownloadState(sTarget,sHelp,fPercentage,State);
    }
}

void CStation::FireStartDownload()
{
    if(m_pMsgSink) {
        CString strDescr;
        CComBSTR sDescr,sTarget;
        HRESULT hr;

        if(IsSignalSet(DOWN_LD_ALL)) {
            hr = S_CE_DL_START;
        }
        else {
            hr = S_CE_DL_STARTCHANGES;
        }

        FormatMsg(strDescr.GetBuffer(_MAX_PATH),hr, m_Header.m_strTargetName,
            m_Header.m_strAddress);
        strDescr.ReleaseBuffer();

        sDescr = strDescr;
        sTarget = m_Header.m_strTargetName;
        m_pMsgSink->Fire_OnNewMessage(sTarget,E_FACILITY_CEDL,
            hr,sDescr);
    }
}

void CStation::FireProjectNew()
{
    CComBSTR sTarget = m_Header.m_strTargetName;

    if(m_pMsgSink) {
        CString strDescr;
        CComBSTR sDescr;

        FormatMsg(strDescr.GetBuffer(_MAX_PATH),S_CE_DL_DOWNLD_NEWPRJ,m_Header.m_strTargetName);
        strDescr.ReleaseBuffer();

        sDescr = strDescr;
        m_pMsgSink->Fire_OnNewMessage(sTarget,E_FACILITY_CEDL,
            S_CE_DL_DOWNLD_NEWPRJ,sDescr);
    }

    if(m_pEvntSink) {
        m_pEvntSink->Fire_OnDownloadState(sTarget,NULL,0.0,DOWNLD_NEW_PROJECT);
    }
}

void CStation::FireFileErrorMessage(const CFileException & e)
{
    CString strHelp,strDescr;
    CComBSTR sDescr,sTarget;
    HRESULT hr = S_OK;

    if(m_pMsgSink) {
        hr = HRESULT_FROM_WIN32(e.m_lOsError);
        FormatMsgDispatch(strHelp,hr,false);

        strHelp.TrimRight(_T("\r\n"));
        FormatMsg(strDescr.GetBuffer(_MAX_PATH),E_CE_DL_FILE_ERROR,m_Header.m_strTargetName,
            strHelp,e.m_strFileName);
        strDescr.ReleaseBuffer();

        sDescr = strDescr;
        sTarget= m_Header.m_strTargetName;
        m_pMsgSink->Fire_OnNewMessage(sTarget,E_FACILITY_CEDL,hr,sDescr);
    }
}

void CStation::FireDaoErrorMessage(const CDaoException & e)
{
    CComBSTR s,sTarget;
    CString str;

    if(e.m_pErrorInfo) {
        s = e.m_pErrorInfo->m_strDescription;
    }
    else {
        str.Format(IDS_CEDL_DEFERROR,e.m_scode);
        s = str;
    }

    sTarget= m_Header.m_strTargetName;
    m_pMsgSink->Fire_OnNewMessage(sTarget,E_FACILITY_CEDL,e.m_scode,s);
}


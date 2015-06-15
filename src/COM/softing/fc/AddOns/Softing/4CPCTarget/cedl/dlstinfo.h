#ifndef DLSTINFO_H
#define DLSTINFO_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//----  Aggregate Includes:   --------------------------------------*
#include "CSC_Online.h"
#include "Subscription.h"

//----  Forward Class Definitions:   -------------------------------*
class CStation;
class CImpVarNotify;
//----  Defines:    ------------------------------------------------*
#define DL_SYSVAR_SIZE  6
//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

class CStationInfo
{
    CSubscriptionManager                             m_SubscriptionManager;
    FC_CALLBACK (SubscriptionNotifyCB, CStationInfo) m_NotifyCB;

	CSubscription                   *m_pSubscriptionArr[DL_SYSVAR_SIZE];
	bool							m_bConnect;
    bool                            m_bCleared;
    HWND                            m_hWnd;
    CStation*						m_pStation;
    CString                         m_strAddress;
   	//indices inside the server handle array m_hServerArr
    CString                         m_strRead[DL_SYSVAR_SIZE];

public:
    //read data
    enum {
        unknown,
        notconnected,
        cleared,
        project,
        unsync,
        sync
    };

	CStationInfo(void);
	virtual ~CStationInfo(void);

	HRESULT Connect(void); //connect vars
	void Disconnect(void); //disconnect vars

    inline void SetStation(CStation *pStation)
    {
        m_pStation = pStation;
    }

    //register notify window
    inline void RegisterWnd(HWND hWnd) 
    {
        m_hWnd = hWnd;
    }

    //unregister notify window
    inline void UnRegisterWnd(void) 
    {
        m_hWnd = NULL;
    }

    //get the current connected machine address
    inline LPCTSTR GetAddress(void) const
    {
        return m_strAddress;
    }
    
    //is the station connected ?
    inline const bool IsConnected(void) const
    {
        return m_bConnect;
    }

    //is the station / resource cleared ?
    inline const bool IsCleared(void) const
    {
        return m_bCleared;
    }
    
    inline LPCTSTR GetVar(int iIndex) const
    {
        assert(iIndex >= 0 && iIndex < DL_SYSVAR_SIZE);
        return m_strRead[iIndex];
    }

    //number of vars
    int GetNumberOfVars(void);
    //get the status
    int State(void);
    //is the server handle one of my subscribed vars ?
    int CheckVar(CSubscription *pSubscription);
    void UpdateVar(CEDL_VAR iVarId,CString& strValue,short nQuality);
    HRESULT GetDLoadAttributes(CString & strResource,IFCDownload* );

private:
    HRESULT Subscribe(void);
    HRESULT UnSubscribe(void);

    LPCTSTR GetVarAddress(CEDL_VAR iVarId);
    LPCTSTR GetVarName(CEDL_VAR iVarId);

    void OnSubscriptionNotify (CSubscription *pSubscription);
};

#endif //DLSTINFO_H


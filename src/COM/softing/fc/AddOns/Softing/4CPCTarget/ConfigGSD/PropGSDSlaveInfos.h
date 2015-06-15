#if !defined(AFX_PROPGSDSLAVEINFOS_H__BF508B73_FB79_11D1_81D3_00104B43B0E0__INCLUDED_)
#define AFX_PROPGSDSLAVEINFOS_H__BF508B73_FB79_11D1_81D3_00104B43B0E0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PropGSDSlaveInfos.h : header file
//

#include "stdafx.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// PropGSDSlaveInfos dialog

class PropGSDSlaveInfos : public CPropertyPage
{
	DECLARE_DYNCREATE(PropGSDSlaveInfos)

// Construction
public:
	BOOL SetPin24V(LONG lVal);
	BOOL SetRepeaterSignal(LONG lVal);
	BOOL SetRedundancy(BOOL bVal);
	BOOL SetFailSafe(BOOL bFailSafe);
	PropGSDSlaveInfos();
	~PropGSDSlaveInfos();

	LONG GetIdentNumber();
			// returns the Device-Identnumber
	BOOL SetSlaveAddrSetSupported(BOOL bSetAddr);
			// Sets if Set Slaveaddress is supported
	BOOL SetAutoBaudSupp(BOOL bAuto);
			// Sets if find Baudrate by itself
	BOOL SetFreezeRequest(BOOL bFreeze);
			// Sets if Freezerequest is supported
	BOOL SetSyncRequest(BOOL bSync);
			// Sets if Syncrequest is supported
	BOOL SetFMSSupported(BOOL bFMSSupport);
			// Sets if FMS-Protocoll is supported
	BOOL SetRevisionString(CString szRevision);
			// Sets the Revision of the Device
	BOOL SetGSDRevision(LONG lGSDRev);
			// Sets the Revision of GSD
	BOOL SetSWRelease(CString szSWRelease);
			// Sets SW-Release
	BOOL SetHWRelease(CString szHWRelease);
			// Sets HW-Release
	BOOL SetStationType(WORD wStationType);
			// Sets Stationtype (0=Slave, 1=Master)
	BOOL SetFileDate(DATE DateGSD);
			// Sets the Filedate of GSD
	BOOL SetModelName(CString szModelName);
			// Set the device modelname
	BOOL SetIdentNmb(LONG lIdent);
			// Set the Device Identnumber
	BOOL SetVendorName(CString szName);
			// Set the Device Vendorname
	BOOL SetFileName(CString& szFilename);
			// Set the GSD-Filename for visualization
	BOOL SetSupportedBaudRates(WORD wBRates);
			// Set the Supported Baudrates

// Dialog Data
	//{{AFX_DATA(PropGSDSlaveInfos)
	enum { IDD = IDD_PROP_GSD_SLAVE_INFOS };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(PropGSDSlaveInfos)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(PropGSDSlaveInfos)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
//	private Funktion prototypes
	BOOL SetUsableBaudrates(CListBox* pLBox);

private:
	WORD m_wPin_24V;
	WORD m_wRepeater;
	BOOL m_bRedundancy;
	BOOL m_bFailSafe;
//	private Membervariables
	BOOL	m_bSetSlaveAddrSupp;		// Slave supports SetSlaveAddress
	BOOL	m_bAutoBaud;				// Slave finds Baudrate by itself
	BOOL	m_bFreezeRequest;			// Slave supports Freezemode
	BOOL	m_bSyncRequest;				// Slave supports Syncmode
	WORD	m_wStationType;				// Stationtype: 0=DP-Slave, 1=DP-Master
	WORD	m_wIdent;					// Identnumber as int-Number

	CString m_szFiledate;				// Date of GSD-File
	CString m_szFilename;				// Location of GSD-File
	CString m_szGSDRevision;			// Version of GSD-File
	CString m_szVendorName;				// Name of Company
	CString m_szModelName;				// Name of Device / Example: "S7-300 / CP 342-5 DP"
	CString m_szRevision;				// Productversion of Device
	CString m_szIdentNmb;				// Identnumber of Device
	CString m_szFMSSupport;				// Device supports FMS
	CString m_szHWRelease;				// HW-Version
	CString m_szSWRelease;				// SW-Version

//	  F   E   D   C   B    A    9    8     7      6       5        4        3        2       1       0
//	+--------------------------------------------------------------------------------------------------------+
//	| n | n | n | n | n | 12M | 6M | 3M | 1.5M | 500k | 187_5k | 93_75k | 45_45k | 31_25k | 19_2k | 9_6k |
//	+--------------------------------------------------------------------------------------------------------+
	WORD m_wSuppBaudRates;				// Supported Baudrates
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPGSDSLAVEINFOS_H__BF508B73_FB79_11D1_81D3_00104B43B0E0__INCLUDED_)

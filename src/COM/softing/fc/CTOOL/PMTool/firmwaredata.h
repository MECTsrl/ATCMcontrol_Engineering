/* $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/PMTool/firmwaredata.h 1     28.02.07 19:01 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: firmwaredata.h $
 *						 $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/PMTool/firmwaredata.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / AddOnHandler
 *
 * =COMPONENT			AddOnBase
 *
 * =CURRENT 	 $Date: 28.02.07 19:01 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ------------------------------------------------------------------------------
 * Change Log
 *
 * Ref.		Date	 Author	Comment
 *
 * ##
 * ------------------------------------------------------------------------------
 */

#ifndef _FIRMWAREDATA_H_
#define _FIRMWAREDATA_H_

#include <afxtempl.h>

#define STD_VERSION_NUMBER  20000

// helper class for CFirmwareData
class CCommChannelData
{
public:
    CString     m_strId;            // ID of comm channel = comm channel prefix
    CString     m_strStdConnPar;    // standard connect parameters
    CString     m_strProtocol;      // protocol
    CString     m_strProtocolPar;   // protocol parameter
};

typedef CArray<CCommChannelData, CCommChannelData&> TCCArray;


//------------------------------------------------------------------*
/**
 *  class CFirmwareData
 *  
 *  - encapsulates data of one firmware entry in target adaption KAD
 *  - contains communication channels plus data
 *
 *  @see    CBaseAddonParameter
 */
class CFirmwareData
{
public:
    CFirmwareData();
    ~CFirmwareData();

    CFirmwareData& operator = (CFirmwareData& rtOther);

    BOOL GetCommChannelDataFromId(const CString& crstrCommChannelId, CCommChannelData& rtCommChannelData);

    int GetCommChannels(CStringArray& rastrCommChannels);

public:
    UINT        m_uiVersion;                // internal version number
    CString     m_strVersionDescription;    // version description string
    CString     m_strCustomConnectGuid;     // custom connect GUID
    CString     m_strDownloadGuid;          // download GUID
    TCCArray    m_tCommChannelArr;          // comm channel data array
};

#endif

/* ---------------------------------------------------------------------------- */

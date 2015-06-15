/* $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/PMTool/firmwaredata.cpp 1     28.02.07 19:01 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: firmwaredata.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/PMTool/firmwaredata.cpp $
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

/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "FirmwareData.h"


//----  Static Initializations:   ----------------------------------*

CFirmwareData::CFirmwareData() : m_uiVersion(STD_VERSION_NUMBER)
{
}

CFirmwareData::~CFirmwareData()
{
}


CFirmwareData& CFirmwareData::operator = (CFirmwareData& rtOther)
{
    m_strCustomConnectGuid = rtOther.m_strCustomConnectGuid;
    m_strDownloadGuid = rtOther.m_strDownloadGuid;
    m_strVersionDescription = rtOther.m_strVersionDescription;
    m_uiVersion = rtOther.m_uiVersion;
    m_tCommChannelArr.RemoveAll();
    m_tCommChannelArr.Copy(rtOther.m_tCommChannelArr);

    return *this;
}

//------------------------------------------------------------------*
/**
 * get comm channel data from id.
 *
 * @param           crstrCommChannelId: id of communication channel
 * @param           rtCommChannelData: communication channel data (out)
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CFirmwareData::GetCommChannelDataFromId(const CString& crstrCommChannelId, CCommChannelData& rtCommChannelData)
{
    int iNumCommChannels = m_tCommChannelArr.GetSize();
    for(int iCommChannel = 0; iCommChannel < iNumCommChannels; ++iCommChannel)
    {
        rtCommChannelData = m_tCommChannelArr[iCommChannel];
        if(rtCommChannelData.m_strId.CompareNoCase(crstrCommChannelId) == 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}

//------------------------------------------------------------------*
/**
 * get comm channel data from description.
 *
 * @param           crstrCommChannelId: id of communication channel
 * @param           rtCommChannelData: communication channel data (out)
 * @return          successful?
 * @exception       -
 * @see             -
*/
//BOOL CFirmwareData::GetCommChannelDataFromDescription(const CString& crstrCommChannelDescription, CCommChannelData& rtCommChannelData)
//{
//    int iNumCommChannels = m_tCommChannelArr.GetSize();
//    for(int iCommChannel = 0; iCommChannel < iNumCommChannels; ++iCommChannel)
//    {
//        rtCommChannelData = m_tCommChannelArr[iCommChannel];
//        if(rtCommChannelData.m_strDescription.CompareNoCase(crstrCommChannelDescription) == 0)
//        {
//            return TRUE;
//        }
//    }
//    return FALSE;
//}


//------------------------------------------------------------------*
/**
 * get communication channels.
 *
 * @param           rastrCommChannels: string array of communication channels.
 * @return          number of comm channels.
 * @exception       -
 * @see             -
*/
int CFirmwareData::GetCommChannels(CStringArray& rastrCommChannels)
{
    CCommChannelData    tCommChannelData;
    int iNumCommChannels = m_tCommChannelArr.GetSize();
    for(int iCommChannel = 0; iCommChannel < iNumCommChannels; ++iCommChannel)
    {
        rastrCommChannels.Add(m_tCommChannelArr[iCommChannel].m_strId);
    }
    return rastrCommChannels.GetSize();
}

/* ---------------------------------------------------------------------------- */

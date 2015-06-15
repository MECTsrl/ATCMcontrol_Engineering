/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "CommPropertySheet.h"
#include "PropPageSerialComm.h"
#include "PropPageSocketComm.h"
#include "proppagenetbuicomm.h"	// NFTASKHANDLING 19.05.05 SIS
#include "BaseAddonParameter.h"
#include "CommDefinitions.h"

#include "utilif.h"


IMPLEMENT_DYNAMIC(CCommPropertySheet, CPropertySheet)



//------------------------------------------------------------------*
/**
 * constructor
 *
 * @param           astrCommChannels: string array of supported communication channel ids
 * @param           crstrCommChannelSel: communication channel id for selected page
 * @exception       -
 * @see             -
*/
CCommPropertySheet::CCommPropertySheet()
  : CPropertySheet(IDS_COMM_PROP_CAP)
{
    // remove apply now button:
    m_psh.dwFlags |= PSH_NOAPPLYNOW;
}



//------------------------------------------------------------------*
/**
 * destructor.
 *
 *  delete property pages.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
CCommPropertySheet::~CCommPropertySheet()
{
    // delete property pages
    POSITION        pos = m_tPageList.GetHeadPosition();
    CBaseCommPropertyPage*  pPropPage;

    while(pos)
    {
        pPropPage = m_tPageList.GetNext(pos);
        delete pPropPage;
    }
}


BEGIN_MESSAGE_MAP(CCommPropertySheet, CPropertySheet)
	//{{AFX_MSG_MAP(CCommPropertySheet)
	//}}AFX_MSG_MAP
    ON_COMMAND_EX(IDOK, OnOK)
END_MESSAGE_MAP()



//------------------------------------------------------------------*
/**
 * on ok.
 *
 *  validate data.
 *
 * @param           nID: id of control
 * @return          -
 * @exception       -
 * @see             -
*/
BOOL CCommPropertySheet::OnOK(UINT nID)
{
    return FALSE;
}

//------------------------------------------------------------------*
/**
 * set connect parameter.
 *
 * @param           crstrCommChannel: communication channel
 * @param           crstrConnectPar: connect parameter string to be set.
 * @return          -
 * @exception       -
 * @see             GetConnectParameter()
*/
void CCommPropertySheet::SetConnectParameter(const CString& crstrCommChannel, const CString& crstrConnectPar)
{
    CBaseCommPropertyPage*  pCommPage = GetPageForChannel(crstrCommChannel);
    if(pCommPage)
    {
        pCommPage->SetConnectParameter(crstrConnectPar);
    }
}

//------------------------------------------------------------------*
/**
 * get connect parameter.
 *
 * @param           crstrCommChannel: communication channel
 * @return          connect parameter string.
 * @exception       -
 * @see             SetConnectParameter()
*/
CString CCommPropertySheet::GetConnectParameter(const CString& crstrCommChannel)
{
    CString strConnectPar;
    CBaseCommPropertyPage*  pCommPage = GetPageForChannel(crstrCommChannel);
    if(pCommPage)
    {
        strConnectPar = pCommPage->GetConnectParameter();
    }
    return strConnectPar;
}

//------------------------------------------------------------------*
/**
 * set active communication channel.
 *
 * @param           crstrCommChannel: communication channel of page to be activated
 * @return          -
 * @exception       -
 * @see             -
*/
void CCommPropertySheet::SetActiveCommChannel(const CString& crstrCommChannel)
{
    CBaseCommPropertyPage*  pCommPage = GetPageForChannel(crstrCommChannel);
    if(pCommPage)
    {
        SetActivePage(pCommPage);
    }
}

//------------------------------------------------------------------*
/**
 * get page for channel.
 *
 *  find property page for certain communication channel.
 *
 * @param           crstrCommChannel: communication channel
 * @return          -
 * @exception       -
 * @see             -
*/
CBaseCommPropertyPage* CCommPropertySheet::GetPageForChannel(const CString& crstrCommChannel)
{
    CBaseCommPropertyPage*  pCommPage;
    int iPageCount = GetPageCount();
    for(int iPage = 0; iPage < iPageCount; ++iPage)
    {
        pCommPage = dynamic_cast<CBaseCommPropertyPage*> (GetPage(iPage));
        ASSERT(pCommPage);
        if(pCommPage)
        {
            if(crstrCommChannel.CompareNoCase(pCommPage->GetCommChannel()) == 0)
            {
                return pCommPage;
            }
        }
    }
    return NULL;
}


//------------------------------------------------------------------*
/**
 * add page for channel
 *
 * @param           crstrCommChannel: communication channel
 * @return          comm channel recognized?
 * @exception       throws memory exception
 * @see             -
*/
BOOL CCommPropertySheet::AddPageForChannel(const CString& crstrCommChannel)
{
    BOOL bAdded = FALSE;
    if(crstrCommChannel.CompareNoCase(COMM_CHANNEL_TCP) == 0)
    {
        CPropPageSocketComm* pPage = new CPropPageSocketComm;
        AddPropertyPage(pPage);
        bAdded = TRUE;
    }
    else if(crstrCommChannel.CompareNoCase(COMM_CHANNEL_RS232) == 0)
    {
        CPropPageSerialComm* pPage = new CPropPageSerialComm;
        AddPropertyPage(pPage);
        bAdded = TRUE;
    }
	// NFTASKHANDLING 19.05.05 SIS >>
    else if(crstrCommChannel.CompareNoCase(COMM_CHANNEL_NETBUI) == 0)
    {
        CPropPageNetBUIComm* pPage = new CPropPageNetBUIComm;
        AddPropertyPage(pPage);
        bAdded = TRUE;
    }
	// NFTASKHANDLING 19.05.05 SIS <<

    return bAdded;
}

//------------------------------------------------------------------*
/**
 * add property page.
 *
 * @param           pPage: property page
 * @return          -
 * @exception       -
 * @see             -
*/
void CCommPropertySheet::AddPropertyPage(CBaseCommPropertyPage* pPage)
{
    m_tPageList.AddTail(pPage);     // store pointer for later destruction

    AddPage(pPage);
}

/* ---------------------------------------------------------------------------- */

/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "TargetPropertySheet.h"
#include "PropPageSerialComm.h"
#include "PropPageSocketComm.h"
#include "BaseTargetData.h"
#include "CommDefinitions.h"
#include "PropPageCustomDownload.h"
#include "fc_tools\fc_fileIO.h"

#include "utilif.h"


//----  Static Initializations:   ----------------------------------*

// TargetPropertySheet.cpp: implementation of the CTargetPropertySheet class.
//
//////////////////////////////////////////////////////////////////////


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CTargetPropertySheet, CPropertySheet)



//------------------------------------------------------------------*
/**
 * constructor
 *
 * @param           pBaseTargetData: base target data
 * @param           bNewDlg: new dialog?
 * @param           nIDCaption: resource id of caption string
 * @param           pParentWnd: parent window
 * @param           iSelectPage: index of page to be shown
 * @exception       -
 * @see             
*/
CTargetPropertySheet::CTargetPropertySheet
(
    CBaseTargetData*    pBaseTargetData,
    BOOL                bNewDlg,
    UINT                nIDCaption,
    CWnd*               pParentWnd,
    UINT                iSelectPage
)
  : CPropertySheet(nIDCaption, pParentWnd, iSelectPage),
    m_pTargetData(pBaseTargetData),
    m_bNewDlg(bNewDlg)
{
    // remove apply now button:
    m_psh.dwFlags |= PSH_NOAPPLYNOW;
    m_pPageGeneral = NULL;
    m_pPageInfo = NULL;
    m_pPageCustDownload = NULL;
}


//------------------------------------------------------------------*
/**
 * constructor
 *
 * @param           pBaseTargetData: base target data
 * @param           bNewDlg: new dialog?
 * @param           pszCaption: caption string
 * @param           pParentWnd: parent window
 * @param           iSelectPage: index of page to be shown
 * @exception       -
 * @see             
*/
CTargetPropertySheet::CTargetPropertySheet
(
    CBaseTargetData*    pBaseTargetData,
    BOOL                bNewDlg,
    LPCTSTR             pszCaption,
    CWnd*               pParentWnd,
    UINT                iSelectPage
) 
  : CPropertySheet(pszCaption, pParentWnd, iSelectPage),
    m_pTargetData(pBaseTargetData),
    m_bNewDlg(bNewDlg)
{
    // remove apply now button:
    m_psh.dwFlags |= PSH_NOAPPLYNOW;
    m_pPageGeneral = NULL;
    m_pPageInfo = NULL;
    m_pPageCustDownload = NULL;
}

//------------------------------------------------------------------*
/**
 * initialize pages.
 *
 *  first static initialization (not target dependent)
 *  then target dependent initialization (InitNewTarget or ReadTargetData)
 *
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CTargetPropertySheet::InitPages()
{
    InitPagesStatic();

    if(m_bNewDlg)
    {
        InitNewTarget();
    }
    else
    {
        ReadTargetData();
    }
}


//------------------------------------------------------------------*
/**
 * init pages static.
 *
 *  target independent initialization.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CTargetPropertySheet::InitPagesStatic()
{
    CPropPageTargetGeneral* pPageGeneral = dynamic_cast<CPropPageTargetGeneral*> (m_pPageGeneral);
    ASSERT(pPageGeneral);
    if(!pPageGeneral)
    {
        TRACE("TargetPropertySheet: m_pPageGeneral is of wrong type\n");
        return;
    }
    CPropPageInfo* pPageInfo = dynamic_cast<CPropPageInfo*> (m_pPageInfo);
    ASSERT(pPageInfo);
    if(!pPageInfo)
    {
        TRACE("TargetPropertySheet: m_pPageInfo is of wrong type\n");
        return;
    }
    //////////////////////////////////////////////////////////////////////////
    ////////////////////////////// general page //////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    CStringArray            astrResources;
    CBaseAddonParameter*    pAddonPar = m_pTargetData->GetAddonParameter();
    ICEProjInfo*            pProjInfo = m_pTargetData->GetProjInfo();

    m_pTargetData->GetAllResourcesFromProject(astrResources);

    pPageGeneral->Init(pProjInfo, pAddonPar, astrResources, m_bNewDlg);

    //////////////////////////////////////////////////////////////////////////
    ///////////////////////////// communication pages //////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    // do not require target independent initialization up to now

    //////////////////////////////////////////////////////////////////////////
    /////////////////////////////// info page ////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    CString strProjectPath;
    CComBSTR    sProjectPath;
    pProjInfo->getProjectPath(&sProjectPath);
    strProjectPath = sProjectPath;

    pPageInfo->Init(strProjectPath);
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
CTargetPropertySheet::~CTargetPropertySheet()
{
    // delete general property page
    if (m_pPageGeneral)
    {
        delete m_pPageGeneral;
        m_pPageGeneral = NULL;
    }

    // delete property pages
    POSITION        pos = m_tPageList.GetHeadPosition();
    CBaseCommPropertyPage*  pPropPage;

    while(pos)
    {
        pPropPage = m_tPageList.GetNext(pos);
        delete pPropPage;
    }

    // delete info page
    if (m_pPageInfo)
    {
        delete m_pPageInfo;
        m_pPageInfo = NULL;
    }

    if (m_pPageCustDownload)
    {
        delete m_pPageCustDownload;
        m_pPageCustDownload = NULL;
    }
}


BEGIN_MESSAGE_MAP(CTargetPropertySheet, CPropertySheet)
	//{{AFX_MSG_MAP(CTargetPropertySheet)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
    ON_COMMAND_EX(IDOK, OnOK)
END_MESSAGE_MAP()


//------------------------------------------------------------------*
/**
 * add pages.
 *
 *  caution:
 *  If you overwrite this function, you must create
 *  at least m_pPageGeneral and m_pPageInfo.
 *  Target base does not verify if these pages exist
 *  thus it will crash
 *  m_pPageCustDownload may be NULL
 *
 * @param           -
 * @return          -
 * @exception       throws memory exceptions
 * @see             -
*/
void CTargetPropertySheet::AddPages()
{
    // general property page
    m_pPageGeneral = new CPropPageTargetGeneral;
    AddPage(m_pPageGeneral);

    m_pPageCustDownload = new CPropPageCustomDownload;
    BOOL bShowDLGui = FALSE;
    if (m_pTargetData && 
        m_pTargetData->GetAddonParameter() &&
        m_pTargetData->GetAddonParameter()->m_bCustDLGui)
    {
        bShowDLGui = TRUE;
    }
    if (bShowDLGui)
    {
        AddPage(m_pPageCustDownload);
    }

    // communication pages
    AddCommunicationPages();

    // info property page
    m_pPageInfo = new CPropPageInfo;
    AddPage(m_pPageInfo);
}

//------------------------------------------------------------------*
/**
 * add communication pages.
 *
 *  
 *
 * @param           -
 * @return          -
 * @exception       throws memory exceptions
 * @see             -
*/
void CTargetPropertySheet::AddCommunicationPages()
{
    CStringArray    astrCommChannels;
    CBaseAddonParameter*    pAddonPar = m_pTargetData->GetAddonParameter();
    ASSERT(pAddonPar);
    pAddonPar->GetAllCommChannels(astrCommChannels);

    int iNumCommChannels = astrCommChannels.GetSize();
    for(int iCommChannel = 0; iCommChannel < iNumCommChannels; ++iCommChannel)
    {
        AddCommPageForChannel(astrCommChannels[iCommChannel]);
    }
}



//------------------------------------------------------------------*
/**
 * add communication page for communication channel.
 *
 *  overwrite this function to introduce new communication channels.
 *
 * @param           crstrCommChannel: communication channel
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CTargetPropertySheet::AddCommPageForChannel(const CString& crstrCommChannel)
{
    BOOL bAdded = FALSE;
    if(crstrCommChannel.CompareNoCase(COMM_CHANNEL_TCP) == 0)
    {
        CPropPageSocketComm*  pPropPage = new CPropPageSocketComm;
        AddCommPropertyPage(pPropPage);
        bAdded = TRUE;
    }
    else if(crstrCommChannel.CompareNoCase(COMM_CHANNEL_RS232) == 0)
    {
        CPropPageSerialComm*  pPropPage = new CPropPageSerialComm;
        AddCommPropertyPage(pPropPage);
        bAdded = TRUE;
    }
    return bAdded;
}



//------------------------------------------------------------------*
/**
 * get communication page for certain communication channel.
 *
 * @param           crstrCommChannel: communication channel
 * @return          communication property page for channel or NULL if not found
 * @exception       -
 * @see             -
*/
CBaseCommPropertyPage* CTargetPropertySheet::GetPageForCommChannel(const CString& crstrCommChannel)
{
    // run over all communication pages and find communication page for channel
    POSITION        pos = m_tPageList.GetHeadPosition();
    CBaseCommPropertyPage*  pPage;

    while(pos)
    {
        pPage = m_tPageList.GetNext(pos);
        if(crstrCommChannel.CompareNoCase(pPage->GetCommChannel()) == 0)
        {
            return pPage;
        }
    }
    return NULL;
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
void CTargetPropertySheet::AddCommPropertyPage(CBaseCommPropertyPage* pPage)
{
    m_tPageList.AddTail(pPage);

    AddPage(pPage);
}


int CTargetPropertySheet::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}


//------------------------------------------------------------------*
/**
 * OK button command handler.
 *
 *  caution: returning TRUE means, that command is handled, i.e. the
 *  dialog will not be closed.
 *  OnOK notifications of property pages are sent later, see Validate()
 *
 * @param           nID: control ID
 * @return          command handled?
 * @exception       -
 * @see             -
*/
BOOL CTargetPropertySheet::OnOK(UINT nID)
{

    UpdatePageData();

    // validate it
    if(!Validate())
    {
        return TRUE;
    }

    // write target data
    WriteTargetData();

    return FALSE;
}


//------------------------------------------------------------------*
/**
 * update page data.
 *
 *  call update page data for all pages.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CTargetPropertySheet::UpdatePageData()
{

    int iPageCount = GetPageCount();
    CBasePropertyPage*  pPage;
    for(int iPage = 0; iPage < iPageCount; ++iPage)
    {
        pPage = dynamic_cast<CBasePropertyPage*>(GetPage(iPage));
        ASSERT(pPage);
        if(pPage)
        {
            pPage->UpdatePageData();
        }
    }
}


//------------------------------------------------------------------*
/**
 * read target data.
 *
 *  fill dialog data from target data.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CTargetPropertySheet::ReadTargetData()
{
    CPropPageTargetGeneral* pPageGeneral = dynamic_cast<CPropPageTargetGeneral*> (m_pPageGeneral);
    ASSERT(pPageGeneral);
    if(!pPageGeneral)
    {
        TRACE("TargetPropertySheet: m_pPageGeneral is of wrong type\n");
        return;
    }
    CPropPageInfo* pPageInfo = dynamic_cast<CPropPageInfo*> (m_pPageInfo);
    ASSERT(pPageInfo);
    if(!pPageInfo)
    {
        TRACE("TargetPropertySheet: m_pPageInfo is of wrong type\n");
        return;
    }
    CPropPageCustomDownload* pPageDownload = dynamic_cast<CPropPageCustomDownload*> (m_pPageCustDownload);

    CBaseAddonParameter* pAddonPar = m_pTargetData->GetAddonParameter();

    //////////////////////////////////////////////////////////////////////////
    ////////////////////////////// general page //////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    pPageGeneral->m_strName           = m_pTargetData->m_strName;
    pPageGeneral->m_strOPCServer      = m_pTargetData->m_strOPCServer;
    pPageGeneral->m_strResource       = m_pTargetData->m_strResource;
    pPageGeneral->m_uiFirmwareIndex   = m_pTargetData->m_uiActFirmware;
    pPageGeneral->m_strCommChannel    = m_pTargetData->m_strCommChannel;

    pPageGeneral->m_strCreateDate     = m_pTargetData->m_strCreateDate;
    pPageGeneral->m_strCreateUser     = m_pTargetData->m_strCreateUser;
    pPageGeneral->m_strLastModDate    = m_pTargetData->m_strModifyDate;
    pPageGeneral->m_strLastModUser    = m_pTargetData->m_strModifyUser;

    //////////////////////////////////////////////////////////////////////////
    ////////////////////////////// download page /////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    if(pPageDownload)
    {
        CString strFileRel = m_pTargetData->m_strCustomDownloadDir;
        CComBSTR sPathAbs;
        CComBSTR sPathRel(strFileRel);
        HRESULT hr = m_pTargetData->GetProjInfo()->GetFileAbsolute(sPathRel, &sPathAbs);
        CString strFileAbs(sPathAbs);

        m_strDLDir = strFileAbs;

        pPageDownload->SetDir(m_strDLDir);
    }

    //////////////////////////////////////////////////////////////////////////
    ////////////////////////// communication pages ///////////////////////////
    //////////////////////////////////////////////////////////////////////////
    // run over all communication property pages and read connect parameters
    POSITION        pos = m_tPageList.GetHeadPosition();
    CBaseCommPropertyPage*  pPropPage;
    CString         strCommChannel;
    CString         strConnectPar;

    while(pos)
    {
        pPropPage = m_tPageList.GetNext(pos);
        strCommChannel = pPropPage->GetCommChannel();
        strConnectPar = m_pTargetData->GetConnectParameter(strCommChannel);
        pPropPage->SetConnectParameter(strConnectPar);
    }

    //////////////////////////////////////////////////////////////////////////
    /////////////////////////////// info page ////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    pPageInfo->m_strAlias  = m_pTargetData->m_strAlias;
    pPageInfo->m_strText1  = m_pTargetData->m_strText1;
    pPageInfo->m_strText2  = m_pTargetData->m_strText2;
    pPageInfo->m_strText3  = m_pTargetData->m_strText3;
    pPageInfo->m_strURL    = m_pTargetData->m_strUrl;
}

//------------------------------------------------------------------*
/**
 * write target data.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CTargetPropertySheet::WriteTargetData()
{
    CPropPageTargetGeneral* pPageGeneral = dynamic_cast<CPropPageTargetGeneral*> (m_pPageGeneral);
    ASSERT(pPageGeneral);
    if(!pPageGeneral)
    {
        TRACE("TargetPropertySheet: m_pPageGeneral is of wrong type\n");
        return;
    }
    CPropPageInfo* pPageInfo = dynamic_cast<CPropPageInfo*> (m_pPageInfo);
    ASSERT(pPageInfo);
    if(!pPageInfo)
    {
        TRACE("TargetPropertySheet: m_pPageInfo is of wrong type\n");
        return;
    }

    if(!m_bNewDlg)
    {
    // if control string data or assignment changes -> ensure that target is disconnected
        BOOL    bDisconnectRequired = FALSE;
        if(m_pTargetData->m_strOPCServer.CompareNoCase(pPageGeneral->m_strOPCServer) != 0)
        {
            bDisconnectRequired = TRUE;
        }
        if(m_pTargetData->m_strResource.CompareNoCase(pPageGeneral->m_strResource) != 0)
        {
            bDisconnectRequired = TRUE;
        }
        if(m_pTargetData->m_uiActFirmware != pPageGeneral->m_uiFirmwareIndex)
        {
            bDisconnectRequired = TRUE;
        }
        if(m_pTargetData->m_strCommChannel.CompareNoCase(pPageGeneral->m_strCommChannel) != 0)
        {
            bDisconnectRequired = TRUE;
        }
        else    // comm channel not changed -> check connect parameter
        {
            CString strOldConnectPar;
            CString strNewConnectPar;
            CBaseCommPropertyPage*  pPropPage = GetPageForCommChannel(m_pTargetData->m_strCommChannel);
            if(pPropPage)
            {
                strOldConnectPar = m_pTargetData->GetConnectParameter(m_pTargetData->m_strCommChannel);
                strNewConnectPar = pPropPage->GetConnectParameter();
                if(strOldConnectPar.CompareNoCase(strNewConnectPar) != 0)
                {
                    bDisconnectRequired = TRUE;
                }
            }
        }
        if(bDisconnectRequired)
        {
            if(!m_pTargetData->EnsureTargetDisconnected(m_pTargetData->m_strName, TRUE))
            {
                return;
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////
    ////////////////////////////// general page //////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    CBaseAddonParameter* pAddonPar = m_pTargetData->GetAddonParameter();

    m_pTargetData->m_strName        =   pPageGeneral->m_strName;
    m_pTargetData->m_strOPCServer   =   pPageGeneral->m_strOPCServer;
    m_pTargetData->m_strResource    =   pPageGeneral->m_strResource;
    m_pTargetData->m_uiActFirmware  =   pPageGeneral->m_uiFirmwareIndex;
    m_pTargetData->m_strCommChannel =   pPageGeneral->m_strCommChannel;

    m_pTargetData->m_strCreateDate  =   pPageGeneral->m_strCreateDate;
    m_pTargetData->m_strCreateUser  =   pPageGeneral->m_strCreateUser;
    m_pTargetData->m_strModifyDate  =   pPageGeneral->m_strLastModDate;
    m_pTargetData->m_strModifyUser  =   pPageGeneral->m_strLastModUser;

    // resource == no assignment -> delete string
    CString strNoResource;
    strNoResource.LoadString(IDS_NO_ASSIGN);
    if(m_pTargetData->m_strResource.Compare(strNoResource) == 0)
    {
        m_pTargetData->m_strResource.Empty();
    }

    //////////////////////////////////////////////////////////////////////////
    ///////////////////////////// download page //////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    if(m_pPageCustDownload)
    {
        CString strDirFull = m_pPageCustDownload->GetDir();

        if (m_bNewDlg && strDirFull.CompareNoCase(m_strDLDir)==0)
        {
            // exchange target name at the end of the path
            int ip = strDirFull.ReverseFind(_T('\\'));
            if (ip>0)
            {
                strDirFull = strDirFull.Left(ip);
                strDirFull += _T("\\") + m_pTargetData->m_strName;
            }
        }

        CComBSTR sPathAbs(strDirFull);
        CComBSTR sPathRel;
        HRESULT hr = m_pTargetData->GetProjInfo()->GetFileRelative(sPathAbs, &sPathRel);

        CString strDirRel(sPathRel);

        m_pTargetData->m_strCustomDownloadDir = strDirRel;


        CFileStatus status;
        if (!strDirFull.IsEmpty() &&
            !CFile::GetStatus(strDirFull, status))
        {
            // create custom download directory if it does not exist
            FC_CreateSubDirA(strDirFull);
        }
    }



    //////////////////////////////////////////////////////////////////////////
    ////////////////////////// communication pages ///////////////////////////
    //////////////////////////////////////////////////////////////////////////
    // run over all communication property pages and save connect parameters
    POSITION        pos = m_tPageList.GetHeadPosition();
    CBaseCommPropertyPage*  pPropPage;
    CString         strCommChannel;
    CString         strConnectPar;

    while(pos)
    {
        pPropPage = m_tPageList.GetNext(pos);
        strCommChannel = pPropPage->GetCommChannel();
        strConnectPar = pPropPage->GetConnectParameter();
        m_pTargetData->SetConnectParameter(strCommChannel, strConnectPar);
    }

    //////////////////////////////////////////////////////////////////////////
    /////////////////////////////// info page ////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    m_pTargetData->m_strAlias   = pPageInfo->m_strAlias;
    m_pTargetData->m_strText1   = pPageInfo->m_strText1;
    m_pTargetData->m_strText2   = pPageInfo->m_strText2;
    m_pTargetData->m_strText3   = pPageInfo->m_strText3;
    m_pTargetData->m_strUrl     = pPageInfo->m_strURL;
}

//------------------------------------------------------------------*
/**
 * validate data of all property pages.
 *
 *  UpdateData() must be called before Validate()
 *
 * @param           -
 * @return          data OK?
 * @exception       -
 * @see             -
*/
BOOL CTargetPropertySheet::Validate()
{
    if(!m_pPageGeneral->Validate())
    {
        return FALSE;
    }

    // run over all pages and call their Validate() function
    POSITION        pos = m_tPageList.GetHeadPosition();
    CBaseCommPropertyPage*  pPropPage;

    while(pos)
    {
        pPropPage = m_tPageList.GetNext(pos);
        if(!pPropPage->Validate())
        {
            return FALSE;
        }
    }

    if(!m_pPageInfo->Validate())
    {
        return FALSE;
    }
    return TRUE;
}



//------------------------------------------------------------------*
/**
 * init new.
 *
 *  initialize dialog members for new target.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CTargetPropertySheet::InitNewTarget()
{
    CPropPageTargetGeneral* pPageGeneral = dynamic_cast<CPropPageTargetGeneral*> (m_pPageGeneral);
    ASSERT(pPageGeneral);
    if(!pPageGeneral)
    {
        TRACE("TargetPropertySheet: m_pPageGeneral is of wrong type\n");
        return;
    }

    //////////////////////////////////////////////////////////////////////////
    ////////////////////////////// general page //////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    CBaseAddonParameter* pAddonPar = m_pTargetData->GetAddonParameter();

    // get a new name proposal from project info
    CString strBaseName = pAddonPar->m_strKADType;
    CComBSTR sNameProp;
    CComBSTR sBaseName(strBaseName);
    HRESULT hr = m_pTargetData->GetProjInfo()->getGlobalNameProposal(sBaseName, &sNameProp);
    if(hr == S_OK)
    {
        pPageGeneral->m_strName = sNameProp;
    }
    else
    {
        // if no proposal has been returned, just take the base name,
        // user can change this
        pPageGeneral->m_strName = strBaseName;
    }

    int    iMaxFirmwareVersionIdx;

    pAddonPar->GetMaxFirmwareVersion(iMaxFirmwareVersionIdx);

    // get local machine name as OPC-Server
//    CString strCompName;
//    UTIL_GetLocalMachineName(strCompName.GetBuffer(UTIL_MAX_OPC_SERVER_LEN), UTIL_MAX_OPC_SERVER_LEN);
//    strCompName.ReleaseBuffer();

//    pPageGeneral->m_strOPCServer = strCompName;
    pPageGeneral->m_strOPCServer = _T("localhost");
    pPageGeneral->m_strResource.LoadString(IDS_NO_ASSIGN);
    pPageGeneral->m_uiFirmwareIndex = iMaxFirmwareVersionIdx;
    pPageGeneral->m_strCommChannel = pAddonPar->GetStandardCommChannel(0);

    //////////////////////////////////////////////////////////////////////////
    ////////////////////////// communication pages ///////////////////////////
    //////////////////////////////////////////////////////////////////////////
    POSITION        pos = m_tPageList.GetHeadPosition();
    CBaseCommPropertyPage*  pPropPage;
    CString         strCommChannel;
    CString         strConnectPar;

    while(pos)
    {
        pPropPage = m_tPageList.GetNext(pos);
        strCommChannel = pPropPage->GetCommChannel();
        strConnectPar = m_pTargetData->GetConnectParameter(strCommChannel);
        pPropPage->SetConnectParameter(strConnectPar);
    }

    m_strDLDir = m_pTargetData->GetProjectPath() + _T("\\") + 
                       pAddonPar->m_strCustDLDir + _T("\\") +
                       pPageGeneral->m_strName;

    if(m_pPageCustDownload)
    {
        m_pPageCustDownload->SetDir(m_strDLDir);
    }

    //////////////////////////////////////////////////////////////////////////
    /////////////////////////////// info page ////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    // no initialization needed
}




//------------------------------------------------------------------*
/**
 * check communication channels.
 *
 *  disable not available communication channels
 *
 * @param           uiFirmwareVersionIndex: index of firmware data in firmware array
 * @return          -
 * @exception       -
 * @see             -
*/
void CTargetPropertySheet::CheckCommChannels(UINT uiFirmwareVersionIndex)
{
    CBaseAddonParameter* pAddonPar = m_pTargetData->GetAddonParameter();
    CStringArray        astrCommChannels;
    CFirmwareData*      pFirmwareData = pAddonPar->GetFirmwareData(uiFirmwareVersionIndex);
    ASSERT(pFirmwareData);
    if(!pFirmwareData)
    {
        return;
    }
    pFirmwareData->GetCommChannels(astrCommChannels);
    int iNumCommChannels = astrCommChannels.GetSize();
    CBaseCommPropertyPage*  pPropPage;
    // first disable all pages
    POSITION    pos = m_tPageList.GetHeadPosition();
    while(pos)
    {
        pPropPage = dynamic_cast<CBaseCommPropertyPage*>(m_tPageList.GetNext(pos));
        if(pPropPage)
        {
            pPropPage->EnablePage(FALSE);
        }
    }
    for(int iCommChannel = 0; iCommChannel < iNumCommChannels; ++iCommChannel)
    {
        pPropPage = GetPageForCommChannel(astrCommChannels[iCommChannel]);
        if(pPropPage)
        {
            pPropPage->EnablePage(TRUE);
        }
    }
}

/* ---------------------------------------------------------------------------- */



//----  Includes:   -------------------------------------------*
#include "fc_tools\fc_fileIO.h"
#include "cgt_xmlparse.h"
#include "cgt_address.h"
#include "cgt_tools.h"
#include "Parse4CH.h"
#include "globctx.h"
#include "helper.h"
#include "io_config.h"
#include "io_config_i.c"
#include "4cpc.h"
#include "io_configdp.h"
#include "io_configdp_i.c"


PC_CIOComp::PC_CIOComp(
    const TCHAR* pszTrgFile,   //pointer valid longer than this
    const TCHAR* pszMasterFile //the <FBUS LINK="master.typ">
)
{
    FC_CString  jot;

    m_masterFile.load(pszMasterFile);
    m_pszIOTyp = FC_StringGetFileExt(m_masterFile);

    jot.load(FC_StringGetFilePart(pszMasterFile)).changeFileExt(NULL);
    m_masterName.load(jot);
    
    m_pICompile = NULL;
	m_pDPCompile = NULL;
    m_channel = -1; //-1 means not initialized , -2 XML error

    CG_InitSTEdipos(&m_edp, pszTrgFile);//TODO line info in XML shit?
}

PC_CIOComp::~PC_CIOComp()
{
    FC_RELEASE_PTR(m_pICompile);
    FC_RELEASE_PTR(m_pDPCompile);
}


bool PC_CIOComp::compile(
    CGT_CDiagMsg*   pdm,
    const CKADInfo* pkad,
    const TCHAR*    pszPrjPath,
    const TCHAR*    pszTrgGenPath
)
{
    HRESULT         hr;
    TCHAR           fileName[2*MAX_PATH];
    FC_CString      sFileName(MAX_PATH);
    BSTR            sBSTR;
    SAFEARRAY*      pDomainFiles = NULL;
    GUID            clsID;
    BSTR            sIOLayerRef;

    //get the clsid from KAD:
    if(!pkad->getIOCompByExt(m_pszIOTyp, &clsID))
    {
        pdm->msg2(CGB_E_NO_IOCONFIG, &m_edp, m_pszIOTyp, m_masterFile);
        return false;
    }
    hr = CoCreateInstance(clsID, NULL, CLSCTX_ALL, IID_ICompile, (void**)&m_pICompile);
    if(FAILED(hr))
    {
        pdm->sysErr(hr, NULL, _T("CoCreateInstance for interface IID_ICompile, for file: "), m_masterFile);
        return false;
    }

    // load the config file
    FC_FileFullPath(pszPrjPath, m_masterFile, fileName, 2*MAX_PATH);
    if ( ! FC_FileExists(NULL, fileName)) // check whether file exists
    {
        pdm->msg1(CG_E_FILE_NOT_EXIST, &m_edp, m_masterFile);
        return false;
    }

    sBSTR = FC_StringToBSTR(fileName);
    LCID lcid = MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),SORT_DEFAULT);//TODO lang 
    hr = m_pICompile->Load(sBSTR, lcid);
    FC_FREE_BSTR(sBSTR);
    
    if(FAILED(hr))
    {
        pdm->msg1(CG_E_BACKEND_ERROR, NULL, m_masterFile);//TODO better diag...
        return false;
    }


    sFileName.load(pszTrgGenPath)<<_T("\\.")<<m_pszIOTyp;
    sFileName.toLower();
    sBSTR = FC_StringToBSTR(sFileName);
    hr = m_pICompile->Compile(sBSTR, &pDomainFiles, &m_channel, &sIOLayerRef);
    FC_FREE_BSTR(sBSTR);
    
    if(FAILED(hr))
    {
        pdm->msg1(CG_E_BACKEND_ERROR, NULL, m_masterFile);//TODO better diag...
        return false;
    }

    assert(sIOLayerRef[0]==L'{');
    m_IOLayerClsid.load(sIOLayerRef);

    if(pDomainFiles)
    {
        //build the m_domainList, the file names must be relative to gen dir:
        long lMax = pDomainFiles->rgsabound[0].cElements;
        sFileName.load(FC_StringGetFilePart(pszTrgGenPath))<<_T("\\.")<<m_pszIOTyp;
        for (int i=0; i<lMax; i++)
        {
            sFileName.addc(_T('\\'))<<((BSTR*)pDomainFiles->pvData)[i];
            sFileName.toLower();
            m_domainList.add(sFileName);
            sFileName.stripFilePart();
        }
        
        SafeArrayDestroy(pDomainFiles);
    }
	
	hr = m_pICompile->QueryInterface(IID_IDPCompile, (void**)&m_pDPCompile);
    if(FAILED(hr))
    {
        m_pDPCompile = NULL;
    }

    return true;
}



bool PC_CIOComp::checkAddr(
    CGT_CDiagMsg*    pdm,
    IOCOMP_CHK_ADDR* pData
)const
{
    HRESULT hr;
    BSTR    sAddress;
    BSTR    sIECType;
    BSTR    sInstancePath;
    BSTR    sJavaClass;
    BSTR    sInitString;
    BSTR    sDiagMsg;
    long    lErrorPos;
    bool    bOk;
    
    sAddress      = FC_StringToBSTR(pData->pszAddr);
    sIECType      = FC_StringToBSTR(pData->pszType);
    sInstancePath = FC_StringToBSTR(pData->pszInst); 
    sJavaClass    = NULL;
    sInitString   = NULL;
    sDiagMsg      = NULL;
    lErrorPos     = 0;
    pData->javaClass.load(_T("%err"));
    pData->initString.load(_T("%err"));
    
    assert(CGT_GetAddrChannelNumber(pData->pszAddr) == m_channel);

    hr = m_pICompile->CheckAddress(
            sAddress,
            sIECType,
            sInstancePath,
            &sJavaClass,
            &sInitString,
            &sDiagMsg,
            &lErrorPos
        );
        

    if(SUCCEEDED(hr))
    {
        assert(!sDiagMsg && sJavaClass && sInitString);

        bOk = true;
        pData->javaClass.load(sJavaClass);
        pData->initString.load(sInitString);
        
        if(sDiagMsg)
        {
            //could warning:
            assert(lErrorPos>=0);
            CG_Edipos edp = *pData->pEdp;
            CG_IncColEdipos(&edp, lErrorPos);

            pdm->msg2(CG_W_INVALID_ADDR, &edp, sAddress, sDiagMsg);
        }
    }
    else
    {
        if(!sDiagMsg)
        {
            pdm->sysErr(hr, NULL, _T("IID_ICompile::CheckAddress for address: "), pData->pszAddr);            
            bOk = false;
        }
        else
        {
            assert(lErrorPos>=0);
            CG_Edipos edp = *pData->pEdp;
            CG_IncColEdipos(&edp, lErrorPos);

            bOk = false;
            pdm->msg2(CG_E_INVALID_ADDR, &edp, sAddress, sDiagMsg);
        }
    }


    FC_FREE_BSTR(sAddress);
    FC_FREE_BSTR(sIECType);
    FC_FREE_BSTR(sInstancePath);
    FC_FREE_BSTR(sJavaClass);
    FC_FREE_BSTR(sInitString);
    FC_FREE_BSTR(sDiagMsg);

    
    return bOk;
}
    
long PC_CIOComp::getDPMasterNum() const 
{ 
	long lMasterAddress;
	
	assert(isDP());
	if (SUCCEEDED(m_pDPCompile->GetMasterAddress(m_channel, &lMasterAddress)))
		return lMasterAddress;
	else 
		return -1;
}

bool PC_CIOComp::getDPAddrByteOffset(const TCHAR* pszAddr, long* plByteOffset, long* plBitOffset) const
{
	BSTR sVarAddress = FC_StringToBSTR(pszAddr);

	assert(isDP());

	HRESULT hr = m_pDPCompile->GetVarOffset(sVarAddress, plByteOffset, plBitOffset);

	FC_FREE_BSTR(sVarAddress);
	return SUCCEEDED(hr);
}

bool PC_CIOComp::getDPIOSize(long* plInBytes, long* plOutBytes, long* plStatusBytes) const 
{
	assert(isDP());
	HRESULT hr;

	hr = m_pDPCompile->GetIOReadLength(m_channel, plInBytes);
	if (FAILED(hr))
		return false;

	hr = m_pDPCompile->GetIOWriteLength(m_channel, plOutBytes);
	if (FAILED(hr))
		return false;

    //TODO ??????????? status size:
    *plStatusBytes = 0;

	return true;
}



CTarget::CTarget()
  :  m_domList(true) 
{
    CG_InitCMDLEdipos(&m_edp);
    m_pres = NULL;
    m_lVersion = 20000; //assume old version
    m_bDidAddrSimuWarn = false;
    m_bDemoVersion     = false;
    m_NumberOfIOPoints = 0;
    m_nRetainItems     = -1;
}


CTarget::~CTarget()
{
    m_masters.deletePtrs();
}



/*
<?xml version="1.0" encoding="UTF-8"?>
<PC  CONTROL="W-EF"  CONSOLE="W-ED" RESOURCE="Resource1" >
        <FBUS LINK="Profiboard0.iodp"/>
        <FBUS LINK="myPAMS\NestedPAM.pam"/>
</PC>
*/

bool CTarget::loadTarget(
    CGlobCtx*      pgctx,
    const TCHAR*   pszTrgFile 
)
{
    long                i;
    long                len;
    IXMLDOMNodeList*    pNList;
    IXMLDOMNode*        pNode;
    int                 nCurrErrors;
    FC_CString          jot(MAX_PATH);
    CGT_CDiagMsg*       pdm;

    pdm = &pgctx->m_dm;
    nCurrErrors = pdm->getTotalErrors();

	//don't load twice:
    assert(m_masters.getUsed()==0);

    m_bDemoVersion = pgctx->getLicData()->bDemo;


    //init file name and edipos:
    m_trgFile.load(pszTrgFile);
    CG_InitSTEdipos(&m_edp, m_trgFile);

    jot.load(pszTrgFile);
    jot.changeFileExt(NULL);
    m_trgName.load(FC_StringGetFilePart(jot));

    jot.load(pgctx->m_pIF->FCPgetGenPath()).appendDir(m_trgName);
    m_trgGenPath.load(jot);

    //open xml file:
    pNode = CGT_XMLLoadFile(pdm, pgctx->m_pIF->FCPgetPrjPath(),
        m_trgFile, L"PC");
    if(!pNode)
        return false;

    if(CGT_XMLGetStrAttrTrim(pdm, pNode, L"CONTROL", &jot, true, true))
        m_control.load(jot);
    
    if(CGT_XMLGetStrAttrTrim(pdm, pNode, L"CONSOLE", &jot, true, true))
        m_console.load(jot);
    
    if(CGT_XMLGetStrAttrTrim(pdm, pNode, L"RESOURCE", &jot, true, true))
        m_resName.load(jot);

    if(CGT_XMLGetStrAttrTrim(pdm, pNode, L"VERSION", &jot, true, true))
        m_lVersion = atol(jot);
    else
        m_lVersion = 20000; //assume old version if not present

    pNList = NULL;
    len = 0;
    pNode->selectNodes(L"FBUS", &pNList);
    assert(pNList);
    FC_RELEASE_PTR(pNode);

    pNList->get_length(&len);
    for(i=0;i<len;i++)
    {
        pNList->get_item(i, &pNode);

        if(CGT_XMLGetStrAttrTrim(pdm, pNode, L"LINK", &jot, true, false))
            m_masters.add(new PC_CIOComp(m_trgFile, jot));

        FC_RELEASE_PTR(pNode);
    }
    FC_RELEASE_PTR(pNList);


    return nCurrErrors==pdm->getTotalErrors();
}



bool CTarget::compile(
    CGT_CDiagMsg*   pdm,
    const CKADInfo* pkad,
    const TCHAR*    pszPrjPath
)
{
    int         i;
    int         j;
    bool        bError;
    PC_CIOComp* pFBM;

    bError = false;
    for(i=0; i<m_masters.getUsed(); i++)
    {
        pFBM = m_masters.get(i);

        if(pFBM->compile(pdm, pkad, pszPrjPath, m_trgGenPath))
        {
            for(j=0; j<i; j++)
            {
                if(pFBM->getChannel() == m_masters.get(j)->getChannel())
                {
                    pdm->msg3(CG_E_FBM_CHANNEL_NUMBER_REUSE, &pFBM->m_edp, 
                        pFBM->m_masterName, pdm->jot1()<<pFBM->getChannel(), 
                        m_masters.get(j)->m_masterName);
                }
            }
        }
        else
        {
            bError = true;
        }
    }
    return !bError;
}


void CTarget::warnDisableIO(
    CGT_CDiagMsg*    pdm,
    const CG_Edipos* pEdp
)
{
    if(!m_bDidAddrSimuWarn)
    {
        m_bDidAddrSimuWarn = true;
        if(m_bDemoVersion)
            pdm->msg1(CG_W_DEMO_USED_IO, pEdp, m_trgName);
        else 
            pdm->msg2(CG_W_ADDR_SIMULATION, pEdp, m_trgName, m_control);
    }
    assert(pdm->getDisplayedWarnings()>0);
}


bool CTarget::checkAddr(
    CGT_CDiagMsg*    pdm,
    IOCOMP_CHK_ADDR* pData
)
{
    int               masterNum;
    const PC_CIOComp* pFBM;

    m_NumberOfIOPoints++;
    
    pFBM = getFBMasterByAddr(pData->pszAddr, &masterNum);
    if(!pFBM)
    {
        if(masterNum>=0)
            pdm->msg3(CG_E_ADDR_NO_MASTER, pData->pEdp, pData->pszAddr, pdm->jot1()<<masterNum, m_trgName);
        else
            pdm->msg2(CG_E_INVALID_ADDR, pData->pEdp, pData->pszAddr, _T("missing channel number"));
        return false;
    }

    return pFBM->checkAddr(pdm, pData);
}


const PC_CIOComp* CTarget::getFBMasterByAddr(
    const TCHAR* pszAddr, 
    int*         pnChannelNum
)const
{
    int nCh;
    
    nCh = CGT_GetAddrChannelNumber(pszAddr);//return -1 if no channel number e.g. %IS!diag
    if(pnChannelNum)
        *pnChannelNum = nCh;

    return getFBMasterByChannel(nCh);
}
const PC_CIOComp* CTarget::getFBMasterByChannel(
    int nChannelNum
)const
{
    int         i;
    PC_CIOComp* pFBM;

    for(i=0; i<m_masters.getUsed(); i++)
    {
        pFBM = m_masters.get(i);
        if(nChannelNum == pFBM->getChannel())
            return pFBM;
    }

    return NULL;
}


void CTarget::addDomain(
    const TCHAR* pszGenRelDomFile
)
{
    m_domList.insertEx(pszGenRelDomFile, NULL, false, NULL);
}



/* 
control;w-ed            <-address exactly as defined in pc1.4cpc
project;<project name>
version;<user defined project version string>
config;<config name>
resource;<resource type>
targettype;4cpc
<domain source path and name, relative to gen>;<domain dest name>
pc1\program1.class;program1.class
pc1\__.init;__.init
*/
 
void CTarget::dumpLoaderList(
    CGlobCtx*           pgctx,
    const FC_CStrList&  usrClassDom,  
    const FC_CStrList&  cmnClassDom,  
    const FC_CString&   cmnLibDomains,
    const FC_CString&   anyOtherDomains
)
{
    int                           i;
    CGT_CDmp                      dmp;
    const CG_KeyValPair*          pAttrVers;
    const FC_CHashStr<int>::Pair* pPair;
    const TCHAR*                  pszFileName;
    FC_CString                    trgDirBeg(MAX_PATH);
    const SL_4CPCLicData*         pLicData;
    const TCHAR*                  pszGenPath;
    const TCHAR*                  pszClsGenPath;
    TCHAR                         szPath[2*MAX_PATH];
    
    //only call if:
    assert(hasControl() && getCGRes() && pgctx->m_pConfig);

    pszGenPath = pgctx->getGenPath();
    pszClsGenPath= pgctx->getClsGenPath();


    //check number of ios:
    pLicData = pgctx->getLicData();
    if(pLicData->bDemo)
    {
        if(m_NumberOfIOPoints!=0)
            CGT_IERR(&pgctx->m_dm, _T("license restrictions"));
    }
    else if(pLicData->bIsBundle)
    {
        if(pLicData->nIOs < m_NumberOfIOPoints)
        {
            pgctx->m_dm.msg2(CG_E_LICENSE_TO_MANY_IOS, &m_edp,
                pgctx->m_dm.jot1()<<pLicData->nIOs,
                pgctx->m_dm.jot2()<<m_NumberOfIOPoints);
        }
    }

    //user defined version string, might not exist:
    pAttrVers = pgctx->m_pIF->lookUpAttrib(
        pgctx->m_pIF->FCPgetSection(CG_SEC_4CP_ATTRIBUTES), CG_KEY_4CP_VERSION); 
    

    
    //standard header for control loader.list:
    (dmp<<_T("control;")).dumpAsCSV(m_control, -1, true)<<_T('\n');
    (dmp<<_T("project;")).dumpAsCSV(pgctx->m_pIF->FCPgetPrjName(), -1, true)<<_T('\n');
    dmp<<_T("version;");
    if(pAttrVers)
        dmp.dumpAsCSV(pAttrVers->pszVal, -1, true);
    dmp<<_T('\n');
    //config and resource are not tolower case, use as typed by user:
    dmp<<_T("config;")<<pgctx->m_pConfig->getName()<<_T('\n');
    dmp<<_T("resource;")<<m_resName<<_T('\n');
    dmp<<_T("targettype;")TARGET_EXT _T("\n");


 
    if(!CGT_DumpSecurityFile(&pgctx->m_dm, pgctx->m_pIF))
        return;
    dmp<<CGT_SECURTY_FILE_NAME _T(";") CGT_SECURTY_FILE_NAME _T("\n");


    //common source file leading string of all domain files in loader.list:
    trgDirBeg.appendLo(m_trgName).addc(_T('\\'));

    for(pPair=m_domList.firstPair(); pPair; pPair=pPair->pNext)
    {
        pszFileName = FC_StringGetFilePart(pPair->pszKey);

        //HACK copy all domains from the .4cpc\.classes to the target inst folder:
        if(FC_StringStartsWithI(pPair->pszKey, trgDirBeg)<0)
        {
            //pPair->pszKey is not already in gen\<target>, copy it including its path, if any:
            FC_FileFullPath(pszGenPath, pPair->pszKey, szPath, FC_ARRAY_LEN(szPath));
            if(!CGT_CopyFile(&pgctx->m_dm, NULL, szPath, m_trgGenPath, pszFileName))
            {
                assert(pgctx->m_dm.hasErr());
                return;
            }
            
            if(!_tcsicmp(FC_StringGetFileExt(pszFileName), _T("class")))//HACK better analys class file in detail, we count also debug symbols!
            {
                if(FC_FileGetSize(szPath) > 65536)
                {
                    //error only if not SFC:
                    FC_CString pouName(MAX_PATH);
                    pouName.load(pszFileName).changeFileExt(NULL);
                    const CG_Symtab* pSym = pgctx->m_pIF->loadHeader(pouName);
                    assert(pSym);
                    if(pSym && (pSym->flags & CG_STAB_BDY_IS_SFC)==0)
                        pgctx->m_dm.msg1(CG_E_CLASS_FILE_TOO_BIG, NULL, pszFileName);
                }
            }

            dmp<<trgDirBeg<<pszFileName<<_T(';')<<pszFileName<<_T('\n');
        }
        else
        {
            dmp<<pPair->pszKey<<_T(';')<<pszFileName<<_T('\n');
        }
    }


    //same HACK as for gen java file's classes: copy to gen\<target>
    for(i=0; i<usrClassDom.getUsed(); i++)
    {
        pszFileName = usrClassDom.get(i);
        if(!CGT_CopyFile(&pgctx->m_dm, pszClsGenPath, pszFileName, m_trgGenPath, pszFileName))
        {
            assert(pgctx->m_dm.hasErr());
            return;
        }
        //DO NOT tolower, class names are user-defined !!
        dmp<<trgDirBeg<<pszFileName<<';'<<_T("..\\classes\\")<<pszFileName<<'\n';
    }
    for(i=0; i<cmnClassDom.getUsed(); i++)
    {
        pszFileName = cmnClassDom.get(i);
        if(!CGT_CopyFile(&pgctx->m_dm, pszClsGenPath, pszFileName, m_trgGenPath, pszFileName))
        {
            assert(pgctx->m_dm.hasErr());
            return;
        }
        //DO NOT tolower, class names are user-defined !!
        dmp<<trgDirBeg<<pszFileName<<';'<<_T("..\\classes\\")<<pszFileName<<'\n';
    }

    if(!cmnLibDomains.isEmpty())
        dmp<<cmnLibDomains;

    if(!anyOtherDomains.isEmpty())
        dmp<<anyOtherDomains;

    //dump to disk:
    CGT_DmpToFile(&pgctx->m_dm, m_trgGenPath, PC_TRG_LOADER_LIST, dmp, false);
}


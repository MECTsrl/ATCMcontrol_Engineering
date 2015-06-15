/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/Parse4CH.cpp 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: Parse4CH.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/Parse4CH.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / CCG
 *
 * =COMPONENT			4CVMLib
 *
 * =CURRENT 	 $Date: 28.02.07 19:04 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "utilif.h"
#include "fc_tools\fc_fileIO.h"
#include "fc_tools\fc_fileIO.h"

#include "cgt_xmlparse.h"
#include "cgt_address.h"
#include "Parse4CH.h"
#include "globctx.h"
#include "helper.h"
#include "vmasm.h"
#include "vmConstants.h"

#include "io_4cvm_config_i.c"
#include "resource.h"
#include "4cvmAdapt.h"
#include "BuildNr.h"
#include "Product.h"

#include <sys/types.h> 
#include <sys/stat.h>
#include <time.h>




CChannelConfig::CChannelConfig(const TCHAR* pszMasterFile)
{
    FC_CString  jot;

    m_masterFile.load(pszMasterFile);
    CG_InitSTEdipos(&m_edp, m_masterFile, 1);

    m_pszIOTyp = FC_StringGetFileExt(m_masterFile);

    jot.load(FC_StringGetFilePart(pszMasterFile)).changeFileExt(NULL);
    m_masterName.load(jot);
    
    m_pICompile = NULL;
    m_channel = -1; //-1 is means not initialized
}
CChannelConfig::~CChannelConfig()
{
    FC_RELEASE_PTR(m_pICompile);
}

bool CChannelConfig::compile(
    CGlobCtx*      pgctx,
    const CTarget* pTrg
)
{
    HRESULT             hr;
    TCHAR               fileName[2*MAX_PATH];
    const CHANNEL_TYP*  pKADInfo; 
    bool                bMultible;

    pKADInfo = pgctx->m_kad.getChannelConfiguratorByExt(m_pszIOTyp, &bMultible);
    if (!pKADInfo)
    {
        pgctx->m_dm.msg1(CG_E_TEXT, &m_edp, 
            pgctx->m_dm.jot1()
            <<"no IO-Configurator found for file type '" //TODO: diag in front end?
            <<m_pszIOTyp
            <<"', known IO-types are: "
            <<pgctx->m_kad.getListOfFBUSTypes()
        );
        return false;
    }

    //make full path and check existence of config file:
    FC_FileFullPath(pgctx->m_pIF->FCPgetPrjPath(), m_masterFile, fileName, 2*MAX_PATH);
    if (!FC_FileExists(NULL, fileName))
    {
        pgctx->m_dm.msg1(CG_E_FILE_NOT_EXIST, pTrg->getEdp(), fileName);
        return false;
    }

    //get I4CVMCompile2 interface:
    hr = CoCreateInstance(pKADInfo->clsidFBUS,
                          NULL, 
                          CLSCTX_ALL,
                          IID_I4CVMCompile2, 
                          (void**)&m_pICompile);
    if(FAILED(hr))
    {
        pgctx->m_dm.sysErr(hr, &m_edp, _T("CoCreateInstance for interface IID_I4CVMCompile2, for file: "), m_masterFile);
        return false;
    }

    //call load(), this yields the m_channel:
    BSTR sCFGfile = FC_StringToBSTR(fileName);
    hr = m_pICompile->Load(sCFGfile, &m_channel);
    FC_FREE_BSTR(sCFGfile);

    if(FAILED(hr))
    {
        pgctx->m_dm.sysErr(hr, &m_edp, _T("failed: Load() of interface IID_I4CVMCompile2, for file: "), m_masterFile);
        FC_RELEASE_PTR(m_pICompile);
        return false;
    }

    //now we know the m_channel number that is stored in the IOComponent1.fcdp file
    //with this info we get the correct pKADInfo for that m_channel. The above pKADInfo
    //was only to get the pKADInfo->clsidFBUS, there may be more than one <CHANNEL>
    //with the same EXT= (then bMultible==true). 
    pKADInfo = pgctx->m_kad.getChannelInfoByChannelNum(m_channel);

    //check the returned channel number: pKADInfo must be !=NULL
    //and pKADInfo->szExt must still map to the file extension: TODO: check also CLSID match!
    if(!pKADInfo || _tcsicmp(pKADInfo->szExt, m_pszIOTyp))
    {
        pgctx->m_dm.msg1(CG_E_TEXT, &m_edp, 
            pgctx->m_dm.jot1()
            <<"invalid IO-channel number: "
            <<m_channel
            <<" is not assigned to IO-Type '"//TODO: diag in front end?
            <<m_pszIOTyp
            <<"' (that would be "
            <<pgctx->m_kad.getChannelNumberForExt(m_pszIOTyp)
            <<")" 
        );
        FC_RELEASE_PTR(m_pICompile);
        return false;
    }

    //initialize the variables of the base class that can be set from static KAD info:
    m_ext.load(pKADInfo->szExt);
    m_bInitialise     = pKADInfo->bInitialise;

    m_ulISegOffs      = pKADInfo->ulISegOffs;
    m_ulIChnSize      = pKADInfo->ulIChnSize;

    m_ulOSegOffs      = pKADInfo->ulOSegOffs;
    m_ulOChnSize      = pKADInfo->ulOChnSize;

    m_ulGMSegOffs     = pKADInfo->ulGMSegOffs;
    m_ulGMChnSize     = pKADInfo->ulGMChnSize;



    long        lInputSize  =0;
    long        lOutputSize =0;
    long        lMemorySize =0;

    FC_CString fullPathDest(MAX_PATH);
    
    fullPathDest.load(pTrg->getCDownloadDir());
    if(fullPathDest.isEmpty())
    {   //maybe a very old target file, if no cust dl dir given => error: 
        CG_Edipos edp;
        CG_InitSTEdipos(&edp, pTrg->getTrgFile(), 1);
        pgctx->m_dm.msg1(CG_E_TEXT, &edp, _T("custom download direcotry is not specified"));

        //use some path after error:
        fullPathDest.load(pTrg->getTrgGenPath())<<_T("\\.")<<m_pszIOTyp;
    }

    if(!FC_StringIsAbsPath(fullPathDest))
        fullPathDest.prepend(pgctx->m_pIF->FCPgetPrjPath(), _T("\\"));


    BSTR          sDestDir = FC_StringToBSTR(fullPathDest);
    BSTR          sDiagMsg = NULL;
    BOOL          bIsError = TRUE;
    SAFEARRAY*    pDomainFiles = NULL;
    unsigned long ulTrgVer = pTrg->getTrgVer();

    if(!_tcsicmp(pTrg->getTrgType(),_T("4cwin")))
        ulTrgVer += 100000; //HACK: the Compile method must also know the target type
                            //by adding 100000 we signal "win" target else is bc target...

    //initialize the member variables of the base class that need output of Compile():
    hr = m_pICompile->Compile(
        //inputs
            ulTrgVer,
            sDestDir,

            m_ulISegOffs,
            m_ulIChnSize,

            m_ulOSegOffs,
            m_ulOChnSize,

            m_ulGMSegOffs,
            m_ulGMChnSize,

        //Outputs:
            &pDomainFiles,
            &lInputSize,   //lInputSize:  currently we do not use this info 
            &lOutputSize,  //lOutputSize: currently we do not use this info 
            &lMemorySize,  //lMemorySize: currently we do not use this info 

            &sDiagMsg,
            &bIsError
        );

    FC_FREE_BSTR(sDestDir);
    
    //HACK: we do not need loader.list any longer, delete now, better solution
    //is not to generate it at all in the IOConfigurator.
    fullPathDest<<'\\'<<_T("loader.list");
    DeleteFile(fullPathDest); 

    if(sDiagMsg || bIsError)
    {
        pgctx->m_dm.msg1(bIsError ? CG_E_TEXT : CG_W_TEXT, &m_edp,
            pgctx->m_dm.jot1()<<(sDiagMsg ? sDiagMsg : L"unqualified error"));
    }
    
    if(FAILED(hr))
    {
        pgctx->m_dm.sysErr(hr, NULL, _T("failed: Compile() of interface IID_I4CVMCompile2, for file: "), m_masterFile);
        bIsError = true;
    }

    if(sDiagMsg)
        SysFreeString(sDiagMsg);

    // we used the custom download fature, no need to precess the domainlist file...
    //    long lMax = pDomainFiles->rgsabound[0].cElements;
    //    for (int i=0; i<lMax; i++)
    //        sFileName.load(((BSTR*)pDomainFiles->pvData)[i]);

    if(pDomainFiles)
        SafeArrayDestroy(pDomainFiles);

    return !bIsError;
}

//return false only if address has errors not if only a warning!
bool CChannelConfig::checkAddr(
    CGlobCtx*		 pgctx,
    CGT_CDiagMsg*    pdm,
    const CG_Edipos* pEdp,
    const TCHAR*     pszIECAddr,
    const CG_DType*  pType,
    const TCHAR*     pszInstPath,
    VM_Address*      pVMAddr
)const
{
    BSTR        sADR;
    BSTR        sTYP;
    BSTR        sINST;
    BSTR        sDiagMsg;
    BOOL        bIsError;
    HRESULT     hr;
    long        lByteOff;
    long        lBitOff;
    long        lBitSize;
    long        lErrorPos;


    // calculate the VM segment and segment offset for that channel
    char          chLoc       = CGT_GetAddrLocation(pszIECAddr);
    long          lSegment    = getVMSegm(chLoc);
    unsigned long ulChByteOff = getOff(chLoc);
    unsigned long ulChnSize   = getSize(chLoc);

    if(ulChnSize==0)//0 identifies that this channel does not have that segment
    {               //do not bother the configurator with this probelm. Also 
                    //submitting the Diag here has the advantage that all 
                    //<CHANNELS> produce the same diag msg.
        pdm->msg2(CG_E_INVALID_ADDR, pEdp, pszIECAddr, 
            pdm->res1(VM_RES_STR_INVALID_CHANNEL)
            .fmtStr3(NULL, pdm->jot1()<<m_channel, 
                           pdm->jot2()<<_T(" (= '")<<m_masterFile<<_T("')"), 
                           pdm->jot3()<<chLoc) 
        );
        return false;
    }



    sADR  = FC_StringToBSTR(pszIECAddr);
    sTYP  = FC_StringToBSTR(pType->pSym ? pType->pSym->pszId : 
                            CGT_DTYPES_ToString(pType->type));
    sINST = FC_StringToBSTR(pszInstPath);
    lBitSize = VMDTypeFullBitSize(pgctx, pEdp, pType);
    bIsError = TRUE;
    sDiagMsg = NULL;
    lErrorPos= 0;

    hr = m_pICompile->CheckAddress(
        //inputs:
            sADR,
            sTYP,
            pType->type==CG_STRUCT || pType->type==CG_INSTANCE,
            pType->bIsArray,
            pType->bIsArray ? pType->nElems : -1,
            pType->lStrLen,
            lBitSize,
            sINST,
        //outputs:
            &lByteOff,
            &lBitOff,
            &sDiagMsg,
            &bIsError,
            &lErrorPos
    );


    if(sDiagMsg || bIsError)
    {
        CG_Edipos edp;
        edp = *pEdp;
        CG_IncColEdipos(&edp, lErrorPos);

        pdm->msg2(bIsError ? CG_E_INVALID_ADDR : CG_W_INVALID_ADDR, &edp, sADR, 
            (sDiagMsg ? sDiagMsg : L"unqualified error")
        );
    }
    
    if(FAILED(hr))
    {
        //must be a hard COM error like out of mem:
        pdm->sysErr(hr, NULL, _T("failed: Compile() of interface IID_I4CVMCompile2, for file: "), m_masterFile);
        bIsError = true;
    }

    if(sDiagMsg)
        SysFreeString(sDiagMsg);

    FC_FREE_BSTR(sDiagMsg);
    FC_FREE_BSTR(sADR);
    FC_FREE_BSTR(sTYP);
    FC_FREE_BSTR(sINST);


    if(!bIsError)
    {
        lByteOff +=  lBitOff/8;
        lBitOff  %=  8;

        pVMAddr->ulByteOffs = lByteOff + lBitOff/8 + ulChByteOff; // ad channel offset
        
        if(!(lBitOff>=0 && lBitOff<=7))
            CGT_IERR2(pdm, pEdp, _T("IID_I4CVMCompile2::Compile() returns invalid bit offset")); 

        pVMAddr->ucBitPos   = (unsigned char)lBitOff;

        switch(CGT_GetAddrBitSize(pszIECAddr))
        {
        case 1:    pVMAddr->chSize = 'X'; break;
        case 8:    pVMAddr->chSize = 'B'; break;
        case 16:   pVMAddr->chSize = 'W'; break;
        case 32:   pVMAddr->chSize = 'D'; break;
        case 64:   pVMAddr->chSize = 'L'; break;
        case -1:   pVMAddr->chSize = 'S'; break;
        default:
            CGT_IERR2(pdm, pEdp, _T("unexpected invalid direct address")); 
        }
        pVMAddr->bitSize = lBitSize;
        // address seems to be OK ==> set channel, SEGMENT and offset:
        pVMAddr->lChannel = m_channel;
        pVMAddr->vmSegEnum= getVMSegm(chLoc);
        pVMAddr->ulChOff  = ulChByteOff;


        //Test channel bounds: should be done by configurator, but be sure:
        //get byte size of type, a simple BOOL has size 1 and an array of 
        //BOOL has size  (VM_ARRAY_LIMIT_SIZE + nElems*1 +7)/8:
        //therefore we can ignore the bit pos in the max segment size check:
        unsigned long ulTypeBytes = VMDTypeFullByteSize(pgctx, pEdp, pType);
    
        //since a BOOL has byte size 1 next > compare is ok also for BOOL: 
        if((unsigned __int64)lByteOff + (unsigned __int64)ulTypeBytes > (unsigned __int64) ulChnSize)
        {                                          //comparison is 100% ok also for BOOLs
		    //address and it's data type exceeds channel size:
            pdm->msg2(CG_E_INVALID_ADDR, pEdp, pdm->jot1()
                <<pszInstPath<<_T(" AT ")<<pszIECAddr<<" : "<<pType->pszName, 
			    pdm->res1(VM_RES_STR_ADDR_CHANNEL_SIZE)
            );
            bIsError = true;;
	    }
    }

    return !bIsError;
}


static bool CheckVMAddrSyntax(const TCHAR* pszAddr)
{
    LPCTSTR psz = pszAddr;
    bool bBit = false;

    if (*psz != _T('%'))
        return false;

    psz = _tcsinc(psz);
    switch(*psz)
    {
    case _T('I'):
    case _T('Q'):
    case _T('M'):
        break;
    default:
        return false;
    }

    psz = _tcsinc(psz);
    switch(*psz)
    {
    case _T('X'):
        bBit = true;
    case _T('B'):
    case _T('W'):
    case _T('D'):
    case _T('L'):
    case _T('S'):
        break;
    default:
        return false;
    }

    psz = _tcsinc(psz);
    while(*psz)
    {
        if (_istdigit(*psz) || *psz == '_')
            psz = _tcsinc(psz);
        else
            break;
    }

    if (*psz != _T('.'))
        return false;

    psz = _tcsinc(psz);
    if ( ! isdigit(*psz))
        return false;

    if (bBit)
    {
        psz = strchr(psz, _T('.'));
        if (psz == NULL)
            return false;

        psz = _tcsinc(psz);
        if ( ! isdigit(*psz))
            return false;
    }
    else
    {
        psz = strchr(psz, _T('.'));
        if (psz != NULL)
            return false;
    }

    return true;
}


bool CChannelNoConfig::checkAddr(
    CGlobCtx*		 pgctx,
    CGT_CDiagMsg*    pdm,
    const CG_Edipos* pEdp,
    const TCHAR*     pszIECAddr,
    const CG_DType*  pType,
    const TCHAR*     pszInstPath,
    VM_Address*      pVMAddr
)const
{
    unsigned long  ulChByteOff = 0;
    VM_SEGM_TYP    vmSegment = VM_SEGM_UNDEF;
    unsigned long  ulChnSize = 0;
    char           chLoc;
    unsigned long  ulAddrByteOff;
    unsigned long  ulAddrBitOff;
    unsigned long  ulTypeBytes;
    const TCHAR*   psz;


    //check:
    if(!CheckVMAddrSyntax(pszIECAddr))
	{
        pdm->msg2(CG_E_INVALID_ADDR, pEdp, pszIECAddr, 
			pdm->res1(VM_RES_STR_INV_ADDR_FMT));
        return false;
	}

    chLoc = CGT_GetAddrLocation(pszIECAddr);

    // calculate the VM segment and segment offset for that channel
    vmSegment   = getVMSegm(chLoc);
    ulChByteOff = getOff(chLoc);
    ulChnSize   = getSize(chLoc);

    if(ulChnSize==0)//0 identifies that this channel does not have that segment
    {
        pdm->msg2(CG_E_INVALID_ADDR, pEdp, pszIECAddr, 
            pdm->res1(VM_RES_STR_INVALID_CHANNEL).fmtStr3(NULL, 
                        pdm->jot1()<<m_channel, 
                        _T(""), 
                        pdm->jot3()<<chLoc) 
        );
        return false;
    }

    //skip channel num:
    psz = _tcschr(pszIECAddr, _T('.'));

    // byteoffset
    ulAddrByteOff = strtoul(psz + 1, (TCHAR**)&psz, 10); // +1: overread '.'
    
    // bitoffset:
    ulAddrBitOff = 0;
    if(*psz)
    {
        assert(*psz=='.');
        ulAddrBitOff   = strtoul(psz + 1, (TCHAR**)&psz, 10); // +1: overread '.'
        ulAddrByteOff += ulAddrBitOff / 8;
        ulAddrBitOff   = ulAddrBitOff % 8;

        assert(pType->type==CG_BOOL);//assert: should already be checked, but
        if(ulAddrBitOff!=0 && pType->bIsArray)//an array of bool may reach here
            pdm->errNS1(pEdp, pdm->res1(VM_RES_STR_ARRAY_AT_BITPOS));
    }


    //get byte size of type, a simple BOOL has size 1 and an array of 
    //BOOL has size  (VM_ARRAY_LIMIT_SIZE + nElems*1 +7)/8:
    //therefore we can ignore the bit pos in the max segment size check:
    ulTypeBytes = VMDTypeFullByteSize(pgctx, pEdp, pType);
    
    //since a BOOL has byte size 1 next > compare is ok also for BOOL: 
    if((unsigned __int64)ulAddrByteOff + (unsigned __int64)ulTypeBytes > (unsigned __int64) ulChnSize)
    {                                          //comparison is 100% ok also for BOOLs
		//address and it's data type exceeds channel size:
        pdm->msg2(CG_E_INVALID_ADDR, pEdp, pdm->jot1()
            <<pszInstPath<<_T(" AT ")<<pszIECAddr<<" : "<<pType->pszName, 
			pdm->res1(VM_RES_STR_ADDR_CHANNEL_SIZE)
        );
        return false;
	}

    // size char:
    switch(CGT_GetAddrBitSize(pszIECAddr))
    {
    case 1:    pVMAddr->chSize = 'X'; break;
    case 8:    pVMAddr->chSize = 'B'; break;
    case 16:   pVMAddr->chSize = 'W'; break;
    case 32:   pVMAddr->chSize = 'D'; break;
    case 64:   pVMAddr->chSize = 'L'; break;
    case -1:   pVMAddr->chSize = 'S'; break;
    default:
        CGT_IERR2(pdm, pEdp, _T("unexpected invalid direct address")); 
        return false;
    }
    //bitsize:
    pVMAddr->bitSize    = VMDTypeFullBitSize(pgctx, pEdp, pType);
    // segment
    pVMAddr->vmSegEnum  = vmSegment;
    // channel offset:
    pVMAddr->ulChOff    = ulChByteOff;
    // channel
    pVMAddr->lChannel   = m_channel;
    // final byte offset:
    pVMAddr->ulByteOffs = ulAddrByteOff + ulChByteOff; // ad channel offset

    // bit off if X:
    if(pVMAddr->chSize == 'X')
    {
        assert(ulAddrBitOff<=7);
        pVMAddr->ucBitPos = (unsigned char)ulAddrBitOff;
    }
    else
    {
        assert(ulAddrBitOff==0);
        pVMAddr->ucBitPos = 0;
    }
        
    return true;
}




CTarget::CTarget()
  :  
  m_ppChannelArray(NULL),
  m_channelArraySize(0),
  m_ulTrgVer(0),
  m_iNumIOUsed(0),
  m_bIOErrMsgAlreadyDsp(false),
  m_bDEMOErrMsgAlreadyDsp(false)
{
    CG_InitCMDLEdipos(&m_edp);
    m_pres = NULL;
}


CTarget::~CTarget()
{
    for(int i=0; i<m_channelArraySize; i++)
        delete(m_ppChannelArray[i]);

    delete m_ppChannelArray;
}



/*
Example target file:

<?xml version="1.0" encoding="UTF-8"?>
<WIN CONTROL="W-EF,126a81fd-4b8e-4a44-96cb-e78c0c741258,W-EF,TCP(W-EF),SIMPLE(1000),VERSION(20000)"
  RESOURCE="Resource1"
  TCP="W-EF"
  COMMCHANNEL="TCP">
  <CDOWNLOAD DIR="__custdl.bin"/>
</WIN>
*/
bool CTarget::loadTarget(
    CGlobCtx*      pgctx,
    const TCHAR*   pszTrgFile 
)
{
    long                len;
    IXMLDOMNodeList*    pNList;
    IXMLDOMNode*        pNode;
    IXMLDOMNode*        pNode2;
    int                 nCurrErrors;
    FC_CString          jot(MAX_PATH);
    FC_CString          jot2(MAX_PATH);
    CGT_CDiagMsg*       pdm;

    pdm = &pgctx->m_dm;
    nCurrErrors = pdm->getTotalErrors();
    
    //init file name and edipos:
    m_trgFile.load(pszTrgFile);
    CG_InitSTEdipos(&m_edp, m_trgFile);

    jot.load(pszTrgFile);
    jot.changeFileExt(NULL);
    m_trgName.load(FC_StringGetFilePart(jot));

    jot.load(FC_StringGetFileExt(pszTrgFile)).toLower();
    m_trgType.load(jot);

    jot.load(pgctx->m_pIF->FCPgetGenPath()).appendDir(m_trgName);
    m_trgGenPath.load(jot);

    //open xml file:
    pNode = CGT_XMLLoadFile(pdm, pgctx->m_pIF->FCPgetPrjPath(), m_trgFile, NULL);
    if(!pNode)
        return false;
    
    //next three attr are optional:
    if(CGT_XMLGetStrAttrTrim(pdm, pNode, L"CONTROL", &jot, true, true))
        m_control.load(jot);

    if(CGT_XMLGetStrAttrTrim(pdm, pNode, L"CONSOLE", &jot, true, true))
        m_console.load(jot);// TODO : file must contain an empty string : CONSOLE=""
    
    if(CGT_XMLGetStrAttrTrim(pdm, pNode, L"RESOURCE", &jot, true, true))
        m_resName.load(jot);
    
    TCHAR szFirmwareVersion[UTIL_MAX_FIRMWARE_VERSION_LEN+1];
    szFirmwareVersion[0] = 0;
    
    if(UTIL_ParseControlString(
        m_control, 
        NULL, // pszOPCServer 
        NULL, // pszCustomConnectGuid 
        NULL, // pszOPCInstName
        NULL, // pszCommChannel 
        NULL, // pszConnectPar
        NULL, // pszProtocol
        NULL, // pszProtocolPar
        szFirmwareVersion) != S_OK)
    {
        //TODO diag ?
    }

    if(szFirmwareVersion[0])
    {
        TCHAR* pszEnd;
        m_ulTrgVer = _tcstoul(szFirmwareVersion, &pszEnd, 10);
        //TODO: diag ?
        assert(m_ulTrgVer>=20000 && *pszEnd==0);
    }
    else
    {
        //version "20000" has no VERSION(), set default:
        m_ulTrgVer = 20000;
    }


    // project info
    if(CGT_XMLGetStrAttrTrim(pdm, pNode, L"USERTEXT1", &jot, false, true))
        m_strPIComment1.load(jot);
    if(CGT_XMLGetStrAttrTrim(pdm, pNode, L"USERTEXT2", &jot, false, true))
        m_strPIComment2.load(jot);
    if(CGT_XMLGetStrAttrTrim(pdm, pNode, L"USERTEXT3", &jot, false, true))
        m_strPIComment3.load(jot);

    FC_CString strBase;
	FC_CString strVersion;
    strBase.load(PROD_VERSION_STR);

    strVersion<<_T("B")<<Adapt4CVM_GetBuildNumber();
	strVersion<<_T(".V")<<strBase;

    m_strPIVersion.load(strVersion);

    FC_CString strFull;
    strFull.load(pgctx->m_pIF->FCPgetPrjPath());
    strFull.appendDir(m_trgFile);
    struct _tstat buf;
    int result;
    result = _tstat( strFull, &buf );

    if( result == 0 )
    {
        FC_CString strTime;
        strTime.load(_tctime(&buf.st_mtime));
        strTime.removeLastChar(_T("\n"));
        m_strPIModified.load(strTime);

        strTime.load(_tctime(&buf.st_ctime));
        strTime.removeLastChar(_T("\n"));
        m_strPICreated.load(strTime);
    }
 
    TCHAR szUserName[MAX_PATH] = _T("");
    DWORD Size = sizeof(szUserName) / sizeof(szUserName[0]);
    ::GetUserName(szUserName, &Size);
    m_strPIOwner.load(szUserName);


    // read FBUS
    pNList = NULL;
    len = 0;
    pNode->selectNodes(L"FBUS", &pNList);
    assert(pNList);

    pNList->get_length(&len);
    for(int i=0;i<len;i++)
    {
        pNList->get_item(i, &pNode2);

        if(CGT_XMLGetStrAttrTrim(pdm, pNode2, L"LINK", &jot, true, false) && !jot.isEmpty())
            m_FBUSConfigFiles.add(jot);

        FC_RELEASE_PTR(pNode2);
    }
    FC_RELEASE_PTR(pNList);


    // read custom download stuff (may be optional on some targets)
    pNList = NULL;
    len = 0;
    pNode->selectNodes(L"CDOWNLOAD", &pNList);
    assert(pNList);
    pNList->get_length(&len);

    if (len > 1)
    {
        pdm->intErr(m_trgFile, 0, _T("more than one custom download directory specified"));
        return false;
    }

    if (len == 1)
    {
        pNList->get_item(0, &pNode2);
        
        if(CGT_XMLGetStrAttrTrim(pdm, pNode2, L"DIR", &jot, true, false) && !jot.isEmpty())
            m_CDownloadDir.load(jot);
        
        FC_RELEASE_PTR(pNode2);
        FC_RELEASE_PTR(pNList);
    }

    return nCurrErrors==pdm->getTotalErrors();
}



bool CTarget::compile(
    CGT_CDiagMsg* pdm,
    CGlobCtx*     pgctx
)
{
    int           i;
    int           n;
    int           nMax;
    const FC_CGArray<CHANNEL_TYP*>* pIOCompListFromKAD;
    FC_CGArray<CChannelConfig*>     configList;
    FC_CGArray<CChannelNoConfig*>   noConfigList;
    

    //get all IOComponent1.fcdp, IOComponent1.bcbac files from the 
    //target defintion file and instantiate a CChannelConfig object:
    nMax = m_FBUSConfigFiles.getUsed();
    for(i=0; i<nMax; i++)
        configList.add(new CChannelConfig(m_FBUSConfigFiles.get(i)));
    
    //call the compile() routines for all CChannelConfig objects, after Compile()
    //we know especially the channel numbers of 
    //these <FBUS LINK="IOComponent1.fcdp">:
    for(i=0; i<nMax; i++)
        configList.get(i)->compile(pgctx, this);


    //now get all <CHANNEL> definitions that don't have a configurator
    //from the KAD and convert them to CChannelNoConfig objects: 
    pIOCompListFromKAD = pgctx->m_kad.getChannels();
    nMax = pIOCompListFromKAD->getUsed();
    for(i=0; i<nMax; i++)
    {
        CHANNEL_TYP* pCHDef = pIOCompListFromKAD->get(i);
        if(!pCHDef->bUseConfigurator)
            noConfigList.add(new CChannelNoConfig(pCHDef));
    }


    if(!pdm->hasErr())
    {
        //get the maximal used channel number to build the m_ppChannelArray for fast access:
        m_channelArraySize = 0;
        nMax = configList.getUsed();
        for(i=0; i<nMax; i++)
        {
            if((n = configList.get(i)->m_channel) < 0) //TODO for Wago we will need this feature again, at the moment not supported diag:
                pdm->errNS1(&m_edp, _T("<CHANNEL> with configurator but no channel number"));
            if(n+1 > m_channelArraySize)
                m_channelArraySize = n+1;
        }

        nMax = noConfigList.getUsed();
        for(i=0; i<nMax; i++)
        {
            if((n = noConfigList.get(i)->m_channel) < 0) //should not happen...
                pdm->errNS1(&m_edp, _T("<CHANNEL> without configurator and without channel number"));
            if(n+1 > m_channelArraySize)
                m_channelArraySize = n+1;
        }
    }

    //allocate the m_ppChannelArray member and fill in the CChannelBase objects, 
    //use this opportunity to check that all channel numbers are unique:
    //we now copy all CChannel[No]Config object from the local tmp arrays 'configList'
    //and 'noConfigList' into m_ppChannelArray, therefore call
    //[no]configList.set(i, NULL) after getting the pointer!
    if(!pdm->hasErr() && m_channelArraySize>0)
    {
        m_ppChannelArray = new CChannelBase*[m_channelArraySize];
        memset(m_ppChannelArray, 0, m_channelArraySize*sizeof(CChannelBase*));

        nMax = configList.getUsed();
        for(i=0; i<nMax; i++)
        {
            CChannelBase* pioc = configList.get(i);
                                 configList.set(i, NULL);  
                                 
            assert(pioc->m_channel>=0 && pioc->m_channel<m_channelArraySize);
            if(m_ppChannelArray[pioc->m_channel])
            {
                pdm->msg1(CG_E_TEXT, &m_edp, pdm->jot1()<<_T("reuse of channel number ")<<pioc->m_channel);
                delete(pioc);
            }
            else
            {
                m_ppChannelArray[pioc->m_channel] = pioc;
            }
        }
    
        nMax = noConfigList.getUsed();
        for(i=0; i<nMax; i++)
        {
            CChannelBase* pioc = noConfigList.get(i);
                                 noConfigList.set(i, NULL);  

            assert(pioc->m_channel>=0 && pioc->m_channel<m_channelArraySize);
            if(m_ppChannelArray[pioc->m_channel])
            {
                pdm->msg1(CG_E_TEXT, &m_edp, pdm->jot1()<<_T("reuse of channel number ")<<pioc->m_channel);
                delete(pioc);
            }
            else
            {
                m_ppChannelArray[pioc->m_channel] = pioc;
            }
        }
    }

    //if no errors above there should be nothing to delete, 
    //but if errors we must clean up now:
    configList.deletePtrs();
    noConfigList.deletePtrs();

    //CTarget::Compile() done...
    return pdm->hasNoErr();
}





bool CTarget::checkAddr(
    CGlobCtx*          pgctx,
    const CG_SymEntry* pEntry,
    VM_Address*        pVMAddr
)const
{
    bool                bOk;
    const CChannelBase* pChBase;
    int                 nChannelNum;

    pChBase = getIOCompByAddr(pEntry->pszAddr, &nChannelNum);
    if(!pChBase)
    {
        // invalid channel number
        pgctx->m_dm.msg3(CG_E_ADDR_NO_MASTER, &pEntry->edpAddr, pEntry->pszAddr, 
            pgctx->m_dm.jot1()<<nChannelNum, m_trgName);
        return false;
    }

    bOk = pChBase->checkAddr(
        pgctx,
        &pgctx->m_dm, 
        &pEntry->edpAddr, 
        pEntry->pszAddr, 
        pEntry->pType, 
        pEntry->pszId, 
        pVMAddr
    );

    if(bOk)
    {
        //is it allowed ? 
        assert(nChannelNum == pVMAddr->lChannel && pChBase->m_channel==nChannelNum);
        if(pEntry->pInit && !pChBase->m_bInitialise)
        {
            pgctx->m_dm.errNS1(&pEntry->edp, pgctx->m_dm.res1(CG_RES_STR_INIT_VAR_AT));
            bOk = false;
        }
    }

    return bOk;
}

const CChannelBase* CTarget::getIOCompByAddr(
    const TCHAR* pszAddr, 
    int*         pnChannelNum
)const
{
    LPCTSTR psz     = pszAddr;
    *pnChannelNum   = -1;

    if (*psz != _T('%'))
        return NULL;

    psz = _tcsinc(psz);
    if (*psz != _T('I') && *psz != _T('Q') && *psz != _T('M'))
        return NULL;
    
    psz = _tcsinc(psz);
    if (*psz != _T('X') && *psz != _T('B')&& *psz != _T('W')&& *psz != _T('D')&& *psz != _T('L')&& *psz != _T('S'))
        return NULL;

    psz = _tcsinc(psz);
    *pnChannelNum = _ttoi(psz);
    
    return getIOCompByChannel(*pnChannelNum);
}


void CTarget::dumpChannelInfo(CGT_CDiagMsg* pdm, const TCHAR* pszAsmFile)
{
    int       i;
    CGT_CDmp  dmp;


    dmp << _T(".channels\n");
    dmp.incrIndent();

    for(i=0; i<m_channelArraySize; i++)
    {
        const CChannelBase* pCh = m_ppChannelArray[i];
        if(!pCh)
            continue;

        dmp << _T(".channel ")<<pCh->m_channel<<'\n';
        dmp.incrIndent();
            (dmp<<"CHname '").lo(pCh->m_ext)<<"'\n";
            (dmp<<"Ioffs  dword#16#").dumpULONGAsHex(pCh->getOff ('I'), 8)<<'\n';
            (dmp<<"Isize  dword#16#").dumpULONGAsHex(pCh->getSize('I'), 8)<<'\n';

            (dmp<<"Qoffs  dword#16#").dumpULONGAsHex(pCh->getOff ('Q'), 8)<<'\n';
            (dmp<<"Qsize  dword#16#").dumpULONGAsHex(pCh->getSize('Q'), 8)<<'\n';

            (dmp<<"Moffs  dword#16#").dumpULONGAsHex(pCh->getOff ('M'), 8)<<'\n';
            (dmp<<"Msize  dword#16#").dumpULONGAsHex(pCh->getSize('M'), 8)<<'\n';
        dmp.decrIndent();
        dmp<<_T(".end\n");
    }

    dmp.decrIndent();
    dmp<<_T(".end\n\n");

    CGT_DmpToFile(pdm, NULL, pszAsmFile, dmp, true);
}



void CTarget::dumpLoaderList(
    CGlobCtx* pgctx
)
{
    CGT_CDmp             dmp;
    const CG_KeyValPair* pAttrVers;

    // debug 
    if ( ! pgctx->m_popt->bCallAsm)
    {
        // 4asm was not called build no loader list
        return;
    }

    //only call if:
    assert(hasControl() && getCGRes() && pgctx->m_pConfig);

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
    //config and resource are not tolower case, use as typyed ba user:
    dmp<<_T("config;")<<pgctx->m_pConfig->getName()<<_T('\n');
    dmp<<_T("resource;")<<m_resName<<_T('\n');
    dmp<<_T("targettype;") << pgctx->m_popt->szCurTrgTyp << _T("\n");


    //append 4asm domains from __domain.list to loader.list:
    CGT_CSVFile csvRead;

    //oopen list file in append mode:
    FC_CString fileName;
    fileName << m_trgGenPath << _T("\\");
    fileName << FILE_NAME_DOMAIN_LIST;

    // HACK for OSAI target
    bool bOsai = ! _tcsicmp(pgctx->m_popt->szCurTrgTyp, _T("4cosai"));

    CG_Edipos edp;
    CG_InitSTEdipos(&edp, fileName, csvRead.getLineno());
    
    if ( ! CGT_LoadCSVFile(&pgctx->m_dm, &csvRead, m_trgGenPath, FILE_NAME_DOMAIN_LIST))
    {
        pgctx->m_dm.msg1(CG_E_FILE_NOT_EXIST, &edp, fileName);
        CGT_IERR(&pgctx->m_dm, _T("ASM domain list not created"));
        return;
    }

    if(!m_paramList.isEmpty())
        dmp.lo(getTrgName())<<'\\'<<m_paramList<<';'<<m_paramList<<'\n';


    while( ! csvRead.isEOF())
    {
        LPCTSTR SmplFileName = NULL;
        LPCTSTR FullFileName = NULL;
        LPCTSTR Section;
        if (   ! csvRead.getNextField(&Section) 
            || ! csvRead.getNextField(&SmplFileName)
            || ! csvRead.isLastFieldEOL())
        {
            pgctx->m_dm.msg1(CG_E_SYNTAX_EXPECTED, &edp, _T("EOL"));
            CGT_IERR(&pgctx->m_dm, _T("bad format in ASM domain list"));
            return;
        }
        
        if (bOsai || _tcscmp(SmplFileName, FILE_NAME_CALL_STACK))
        {
            // dump file '__callstack.bin' only for OSAI
            dmp.lo(getTrgName()) << _T("\\");
            (dmp.lo(SmplFileName) << _T(';')).lo(SmplFileName) << '\n';
        }
    }

    // process cutom download stuff
    if ( ! m_CDownloadDir.isEmpty())
    {
        // loader.list
        dmp.lo(getTrgName()) << _T("\\") FILE_NAME_CUSTOM_BIN;
        dmp << _T(";") << FILE_NAME_CUSTOM_BIN _T("\n");

        // __domain.list
        CGT_CDmp dmp2;
        dmp2 << DL_SECTION_CUSTOM _T(";") << FILE_NAME_CUSTOM_BIN _T("\n");
        CGT_DmpToFile(&pgctx->m_dm, m_trgGenPath, FILE_NAME_DOMAIN_LIST, dmp2, true);

        // write an empty "__custdl.bin"
        fileName.clear();
        fileName << m_trgGenPath << _T("\\");
        fileName << FILE_NAME_CUSTOM_BIN;
        FILE* pFile = fopen(fileName, "wb");
        fclose(pFile);
    }

    CGT_DmpToFile(&pgctx->m_dm, m_trgGenPath, PC_TRG_LOADER_LIST, dmp, false);
}

/* ---------------------------------------------------------------------------- */

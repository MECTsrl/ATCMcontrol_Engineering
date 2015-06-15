/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/parseKAD.cpp 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: parseKAD.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/parseKAD.cpp $
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

#include <limits.h>
#include <errno.h>

#include "parseKAD.h"
#include "defines.h"
#include "cgt_xmlparse.h"
#include "cgt_CDiagMsg.h"
#include "helper.h" //GetModulePath()



class CGT_CXMLEdp{ //TODO put into cg_tools.lib
public:
    FC_CString xmlFile;
    CG_Edipos  m_edp;

    CGT_CXMLEdp(IXMLDOMNode*  pNode)
    {
        IXMLDOMDocument* pdoc;
        BSTR             bstr;

        pNode->get_ownerDocument(&pdoc);
        pdoc->get_url(&bstr);
        xmlFile.load(bstr);
        FC_RELEASE_PTR(pdoc);
        FC_FREESYSTRING(bstr);

        xmlFile.stripBeginI(_T("file:"));
        while(xmlFile.stripBeginI("/"))
            ;

        CG_InitSTEdipos(&m_edp, xmlFile, 1);//TODO impossible to get (line,col), use line 1...??
    }
};

static void CGT_XMLDiagAttrValue(//TODO put into cg_tools.lib
    CGT_CDiagMsg* pdm,
    IXMLDOMNode*  pNode, 
    const TCHAR*  pszAttr,
    const TCHAR*  pszValue,
    const TCHAR*  pszExpected
)
{
    CGT_CXMLEdp xedp(pNode);
    BSTR        bstrName;
    FC_CString  jot(255);


    pNode->get_nodeName(&bstrName);
    if(bstrName)
    {
        jot.load(bstrName);
        FC_FREESYSTRING(bstrName);
    }

    pdm->msg3(CG_E_XML_STRUCTURE_ERROR_VALUE, &xedp.m_edp, jot, pszValue, pszAttr);

    if(pszExpected)
        pdm->userMsg(jot.load(_T("    expected value: "))<<pszExpected<<_T("\n"));
}
int CGT_ParseUINT32(const char* pszTok, unsigned long* pul)//TODO put into cg_tools.lib
{
    char* pszEnd;

    //leading zeros are optional ! don't use strtoul(pszTok, &pszEnd, 0) 
    //otherwise strtod thinks it is octal
    if(pszTok[0]=='0' && (pszTok[1] == 'x'|| pszTok[1] == 'X'))
       *pul = strtoul(pszTok+2, &pszEnd, 16);
    else
       *pul = strtoul(pszTok, &pszEnd, 10);

    if(errno == ERANGE || *pszEnd)
    {
        *pul = 0;
        return 0;
    }
    return 1;
}
int CGT_ParseINT32(const char* pszTok, long* pl)//TODO put into cg_tools.lib
{
    char* pszEnd;
    
    if(pszTok[0]=='0' && (pszTok[1] == 'x'|| pszTok[1] == 'X'))
        *pl = strtol(pszTok+2, &pszEnd, 16);
    else
        *pl = strtol(pszTok, &pszEnd, 10);

    if(errno == ERANGE || *pszEnd)
    {
        *pl = 0;
        return 0;
    }
    return 1;
}




static void ReadChannels(
    CGT_CDiagMsg*            pdm,
    IXMLDOMNodeList*         pNList,
    FC_CGArray<CHANNEL_TYP*>*pChannels
);

static void ValidateChannels(
    CGT_CDiagMsg*             pdm,
    const CG_Edipos*          pEdp,
    FC_CGArray<CHANNEL_TYP*>* pChannels,
    const VM_SETTINGS*        pVMSettings
);

static bool isCurrentTrgType(
    CGT_CDiagMsg* pdm,
    LPCTSTR       szTargetName,
    IXMLDOMNode*  pNode
);

static void ReadVmSettings(
    CGT_CDiagMsg*    pdm,
    IXMLDOMNodeList* pNList,
    VM_SETTINGS*     pVmSettings
);

static void ReadIecFeatures(
    CGT_CDiagMsg*    pdm,
    LPCTSTR          pszKADFileAndPath,
    IXMLDOMNodeList* pNList,
    FC_CString*      pIecLanguages,
    CG_BITSET*       pbsIecTypes,
    CG_BITSET*       pbsArrayIdxIectypes,
    long*            plMaxTasks,
    bool*            pbLocalRetain,
    bool*            pCheckAddressSpace
);

static void ReadTaskImageFeature(
    CGT_CDiagMsg*    pdm,
    LPCTSTR          pszKADFileAndPath,
    IXMLDOMNodeList* pNList,
    VM_BITSET*       pbsUseTIforSegm,     // TI: TaskImage
    VM_BITSET*       pbsOptimizeTIforSegm,  // TI: TaskImage
    long*            plMaxRegionsWR,
    long*            plMaxRegionsRD,
    unsigned long    ulTrgVer
);

static void ReadCgOptions(
    CGT_CDiagMsg*    pdm,
    IXMLDOMNodeList* pNList,
    CG_OPTIONS*      pCgOptions
);

void CKADInfo::clear()
{
    CG_InitEdipos(&m_KADEdp);
    m_szKADFile.load(_T(""));
    m_channels.deletePtrs();
    //TODO init m_VmSettings;
    //TODO init m_CgOptions
    m_AllowedIECtypes = 0;
    m_CheckAddressSpace = false;
    m_AllowedArrayIndexIECtypes = 0;
    m_AllowedIEClanguages.clear();
    m_useTIforSegm = 0;
    m_optimizeTIforSegm = 0;
    memset(m_lMaxRegionsWR, 0, sizeof(m_lMaxRegionsWR));
    memset(m_lMaxRegionsRD, 0, sizeof(m_lMaxRegionsRD));
    m_lMaxTasks = -1; // -1 if no maximum
    m_bLocalRetain = false;
    m_listOfFBUSTypes.clear();
}


bool CKADInfo::load(
    CGT_CDiagMsg*   pdm,
	const TCHAR*    pszKADFileAndPath,
	const TCHAR*    pszTargetType,
    unsigned long   ulTrgVer
)
{
    long                i, o;
    long                len;
    long                len2;
    long                len3;
    IXMLDOMNodeList*    pNList;
    IXMLDOMNodeList*    pNList2;
    IXMLDOMNodeList*    pNList3;
    IXMLDOMNode*        pNode;
    IXMLDOMNode*        pNode2;
    int                 nCurrErrors;
    FC_CString          jot(MAX_PATH);
    bool                bTargetTypeFound = false;


    nCurrErrors = pdm->getTotalErrors();
    
	//clear possible old content:
    clear();

    m_szKADFile.load(pszKADFileAndPath);
    CG_InitSTEdipos(&m_KADEdp, m_szKADFile, 1);


    pNode = CGT_XMLLoadFile(pdm, NULL, m_szKADFile, L"KAD4VM");
    if(!pNode)
        return false;

    
    // read targets
    pNList = NULL;
    len = 0;
    pNode->selectNodes(L"TRGTYP", &pNList);
    assert(pNList);
    FC_RELEASE_PTR(pNode);

    pNList->get_length(&len);
    for(i=0;i<len;i++)
    {
        pNList->get_item(i, &pNode);

        if ( ! isCurrentTrgType(pdm, pszTargetType, pNode))
            continue;

        bTargetTypeFound = true;


        // <IEC_FEATURES> : read IEC features : dadta types , languages
        pNList2 = NULL;
        pNode->selectNodes(L"IEC_FEATURES", &pNList2);
        assert(pNList2);

        ReadIecFeatures(pdm, 
            m_szKADFile, 
            pNList2, 
            &m_AllowedIEClanguages, 
            &m_AllowedIECtypes, 
            &m_AllowedArrayIndexIECtypes,
            &m_lMaxTasks,
            &m_bLocalRetain,
            &m_CheckAddressSpace
        );

        FC_RELEASE_PTR(pNList2);

        

        // <TASKIMAGE> : read task image features : segments, size, optimize
        pNList2 = NULL;
        pNode->selectNodes(L"TASKIMAGE", &pNList2);
        assert(pNList2);

        ReadTaskImageFeature(pdm, 
            m_szKADFile, 
            pNList2, 
            &m_useTIforSegm,
            &m_optimizeTIforSegm,
            m_lMaxRegionsWR,
            m_lMaxRegionsRD,
            ulTrgVer);

        FC_RELEASE_PTR(pNList2);



        // <IOCOMPONENTS>
        pNList2 = NULL;
        pNode->selectNodes(L"IOCOMPONENTS", &pNList2);
        assert(pNList2);

        len2 = 0;
        pNList2->get_length(&len2);
        for(o=0;o<len2;o++)
        {
            pNList2->get_item(o, &pNode2);
            // <CHANNEL>
            pNList3 = NULL;
            pNode2->selectNodes(L"CHANNEL", &pNList3);
            ReadChannels(pdm, pNList3, &m_channels);
            FC_RELEASE_PTR(pNList3);
        }
        FC_RELEASE_PTR(pNList2);


        // <VM_SETTINGS> : read target features 
        pNList2 = NULL;
        pNode->selectNodes(L"VM_SETTINGS", &pNList2);
        assert(pNList2);

        ReadVmSettings(pdm, pNList2, &m_VmSettings);
        FC_RELEASE_PTR(pNList2);


        // <CG_OPTIONS> : read target code generator features 
        pNList2 = NULL;
        pNode->selectNodes(L"CG_OPTIONS", &pNList2);
        assert(pNList2);
        pNList2->get_length(&len3);
        if (len3 != 0)
        {
            // <CG_OPTIONS> is optional
            ReadCgOptions(pdm, pNList2, &m_CgOptions);
            FC_RELEASE_PTR(pNList2);
        }

        FC_RELEASE_PTR(pNode);
    }
    FC_RELEASE_PTR(pNList);

    if ( ! bTargetTypeFound)
    {
        pdm->msg1(CG_E_XML_TARGET_NOT_FOUND_IN_KAD, &m_KADEdp, pszTargetType);
        return false;
    }

    //finally check usage of channels if no error so far:
    if(nCurrErrors==pdm->getTotalErrors())
        ValidateChannels(pdm, &m_KADEdp, &m_channels, &m_VmSettings);

    return nCurrErrors==pdm->getTotalErrors();
}

const CHANNEL_TYP* CKADInfo::getChannelConfiguratorByExt(const TCHAR* pszExt, bool* pbMultible)const
{
    int                i;
    const CHANNEL_TYP* pIOC;
    const CHANNEL_TYP* pIOCret = NULL;
    int                nMax = m_channels.getUsed();

    *pbMultible = false;

    for(i=0;i<nMax;i++)
    {
        pIOCret = m_channels.get(i);
        if(pIOCret->bUseConfigurator && !_tcsicmp(pszExt, pIOCret->szExt))
        {
            for(i++ ;i<nMax;i++)
            {
                pIOC = m_channels.get(i);
                if(pIOC->bUseConfigurator && !_tcsicmp(pszExt, pIOC->szExt))
                {
                    *pbMultible = true;
                    return pIOCret;
                }
            }
            return pIOCret;
        }
    }
    return NULL;
}
const CHANNEL_TYP* CKADInfo::getChannelInfoByChannelNum(int nChNum)const
{
    int                 i;
    const CHANNEL_TYP*  pIOC;
    int                 nMax = m_channels.getUsed();
    
    assert(nChNum>=0); //TODO: implement NUM="userdefined"|"notused" ?? must change code here??
    for(i=0;i<nMax;i++)
    {
        pIOC = m_channels.get(i);
        if(pIOC->lChannelNum==nChNum)
            return pIOC;
    }
    return NULL;
}

const TCHAR* CKADInfo::getListOfFBUSTypes()
{

    if(m_listOfFBUSTypes.isEmpty())
    {
        for(int i=0; i<m_channels.getUsed(); i++)
        {
            CHANNEL_TYP* pioc = m_channels.get(i);
            if(pioc->bUseConfigurator)
            {
                if(!m_listOfFBUSTypes.isEmpty())
                    m_listOfFBUSTypes<<_T(", ");
                m_listOfFBUSTypes<<pioc->szExt;
            }
        }
    }
    return m_listOfFBUSTypes;
}
FC_CStr CKADInfo::getChannelNumberForExt(const TCHAR* pszExt)const
{
    FC_CString jot(MAX_PATH);

    for(int i=0; i<m_channels.getUsed(); i++)
    {
        CHANNEL_TYP* pioc = m_channels.get(i);
        if(!_tcsicmp(pioc->szExt, pszExt))
        {
            if(!jot.isEmpty())
                jot<<_T(", ");
            jot<<pioc->lChannelNum;
        }
    }
    return jot;
}


static bool isCurrentTrgType(
    CGT_CDiagMsg* pdm,
    LPCTSTR       szTargetName,
    IXMLDOMNode*  pNode)
{
    FC_CString jot(MAX_PATH);

    if(!CGT_XMLGetStrAttrTrim(pdm, pNode, L"EXT", &jot, true, false))
        return false;

    if ( ! stricmp(jot, szTargetName))
        return true;

    return false;
}


static bool MakeDataTypeBitset(CGT_CDiagMsg*      pdm, 
                               LPCTSTR            pszKADFileAndPath,
                               IXMLDOMNode*       pNode,
                               const TCHAR*       pszAttrName,
                               const TCHAR*       pszTypes,
                               CG_BITSET*         pbsIecTypes)
{
    LPCTSTR    pszInput = pszTypes;
    FC_CString sType;

    while(pszTypes && *pszTypes != 0)
    {
        sType.clear();
        sType.appendUpTo(pszTypes, _T(","), &pszTypes);
        sType.trim();
        CG_DTYPES type = CGT_TypeKeywordToCG_DTYPES(sType);
        if (type == CG_DTYPES_UNDEF)
            CGT_XMLDiagAttrValue(pdm,pNode,pszAttrName,pszInput,NULL);

        *pbsIecTypes |= CGT_DTypesToTBS(type);

        if (*pszTypes != 0)
            pszTypes = _tcsinc(pszTypes);
    }

    return true;
}
static VM_BITSET VM_SegmentKeywordToSegmentBS(const TCHAR* pszSegmentKW)
{
    if ( ! _tcscmp(VM_SEGNAME_INPUT, pszSegmentKW))
        return VM_BS_SEGM_INPUT;

    if ( ! _tcscmp(VM_SEGNAME_OUTPUT, pszSegmentKW))
        return VM_BS_SEGM_OUTPUT;

    if ( ! _tcscmp(VM_SEGNAME_GLOBAL, pszSegmentKW))
        return VM_BS_SEGM_GLOBAL;

    if ( ! _tcscmp(VM_SEGNAME_RETAIN, pszSegmentKW))
        return VM_BS_SEGM_RETAIN;

    return VM_BS_SEGM_UNDEF;
}

static bool MakeSegmentTypeBitset(CGT_CDiagMsg*      pdm, 
                                  LPCTSTR            pszKADFileAndPath,
                                  IXMLDOMNode*       pNode,
                                  const TCHAR*       pszAttrName,
                                  const TCHAR*       pszSegments,
                                  VM_BITSET*         pbsSegmentTypes)
{
    LPCTSTR    pszInput = pszSegments;
    FC_CString sSegment;

    while(pszSegments && *pszSegments != 0)
    {
        sSegment.clear();
        sSegment.appendUpTo(pszSegments, _T(","), &pszSegments);
        sSegment.trim();
        VM_BITSET segm = VM_SegmentKeywordToSegmentBS(sSegment);
        if (segm == VM_BS_SEGM_UNDEF)
        {
            CGT_XMLDiagAttrValue(pdm,pNode,pszAttrName,pszInput,NULL);
            return false;
        }

        *pbsSegmentTypes |= segm;

        if (*pszSegments != 0)
            pszSegments = _tcsinc(pszSegments);
    }

    return true;
}

static VM_SEGM_TYP VM_SegmentKeywordToSegmentEnum(const TCHAR* pszSegmentKW)
{
    if ( ! _tcscmp(VM_SEGNAME_INPUT, pszSegmentKW))
        return VM_SEGM_INPUT;

    if ( ! _tcscmp(VM_SEGNAME_OUTPUT, pszSegmentKW))
        return VM_SEGM_OUTPUT;

    if ( ! _tcscmp(VM_SEGNAME_GLOBAL, pszSegmentKW))
        return VM_SEGM_GLOBAL;

    if ( ! _tcscmp(VM_SEGNAME_RETAIN, pszSegmentKW))
        return VM_SEGM_RETAIN;

    return VM_SEGM_UNDEF;
}

static void ReadNumberPerSegment(CGT_CDiagMsg*      pdm, 
                                 LPCTSTR            pszKADFileAndPath,
                                 IXMLDOMNode*       pNode,
                                 const TCHAR*       pszAttrName,
                                 const TCHAR*       pszNumPerSegments,
                                 long*              plMaxRegions)
{
    LPCTSTR    pszInput  = pszNumPerSegments;
    LPCTSTR    pszNum    = NULL;
    bool       bNumValid = true;
    FC_CString sNum;
    FC_CString sSegm;

    while(pszNumPerSegments && *pszNumPerSegments != 0)
    {
        sSegm.clear();
        sSegm.appendUpTo(pszNumPerSegments, _T(":"), &pszNumPerSegments);
        sSegm.trim();
        if (*(pszNumPerSegments+1) == 0)
            goto Error;

        sNum.clear();
        sNum.appendUpTo(pszNumPerSegments+1, _T(","), &pszNumPerSegments);
        sNum.trim();

        // evaluate segment and number
        VM_SEGM_TYP segm = VM_SegmentKeywordToSegmentEnum(sSegm);
        if (segm == VM_SEGM_UNDEF)
            goto Error;

        for (pszNum = sNum; *pszNum; pszNum++)
            if ( ! _istdigit(*pszNum))
                goto Error;

        plMaxRegions[segm] = strtol(sNum, NULL, 10);

        if (*pszNumPerSegments != 0)
            pszNumPerSegments = _tcsinc(pszNumPerSegments);
    }

    return;

Error:
    CGT_XMLDiagAttrValue(pdm,pNode,pszAttrName,pszInput,NULL);
    return;
}

static void ReadIecFeatures(CGT_CDiagMsg*    pdm,
                            LPCTSTR          pszKADFileAndPath,
                            IXMLDOMNodeList* pNList,
                            FC_CString*      pIecLanguages,
                            CG_BITSET*       pbsIecTypes,
                            CG_BITSET*       pbsArrayIdxIectypes,
                            long*            plMaxTasks,
                            bool*            pbLocalRetain, 
                            bool*            pCheckAddressSpace)
{
    IXMLDOMNode*          pNode;
    FC_CString            jot, jot2, jot3;


    pNList->get_item(0, &pNode);
    assert(pNode);
    
    if ( ! CGT_XMLGetStrAttrTrim(pdm, pNode, L"DATA_TYPES", &jot, true, false))
        goto Error;
    if ( ! CGT_XMLGetStrAttrTrim(pdm, pNode, L"ARR_IDX_DATA_TYPES", &jot3, true, false))
        goto Error;
    if ( ! CGT_XMLGetStrAttrTrim(pdm, pNode, L"LANGUAGES", pIecLanguages, true, false))
        goto Error;

    if (CGT_XMLGetStrAttrTrim(pdm, pNode, L"MAX_TASKS", &jot2, true, true))
        *plMaxTasks = _ttol(jot2);

    if(CGT_XMLGetBoolAttr(pdm, pNode, L"LOCAL_RETAIN", pbLocalRetain, true)<0)
        goto Error;
        

    // allowed iec data types
    *pbsIecTypes = 0;    // reset
    if ( ! MakeDataTypeBitset(pdm, pszKADFileAndPath, pNode, _T("DATA_TYPES"), jot, pbsIecTypes))
        goto Error;

    // read the allowed array index data types
    *pbsArrayIdxIectypes = 0;   // reset
    if ( ! MakeDataTypeBitset(pdm, pszKADFileAndPath, pNode, _T("ARR_IDX_DATA_TYPES"), jot3, pbsArrayIdxIectypes))
        goto Error;

    if(CGT_XMLGetBoolAttr(pdm, pNode, L"CHECK_ADDRESS_SPACE", pCheckAddressSpace, true)<0)
        goto Error;

Error:
    FC_RELEASE_PTR(pNode);
}

/* --------------------------------------------------------------------------- */
/*
 *  ReadNumberPerTask
 */
static void ReadNumberPerTask(CGT_CDiagMsg*     pdm,
                              IXMLDOMNode*      pNode,
                              const TCHAR*      pszAttrName,
                              FC_CString&       NumPerTask,
                              long*             plMaxRegions)
{
    LPCTSTR pszNum = NULL;

    for (pszNum = NumPerTask; *pszNum; pszNum++)
    {
        if ( !_istdigit(*pszNum))
        {
            CGT_XMLDiagAttrValue(pdm, pNode, pszAttrName, NumPerTask, NULL);
            return;
        }
    }
    *plMaxRegions = strtol(NumPerTask, NULL, 10);
    
}

static void ReadTaskImageFeature(CGT_CDiagMsg*    pdm,
                                 LPCTSTR          pszKADFileAndPath,
                                 IXMLDOMNodeList* pNList,
                                 VM_BITSET*       pbsUseTIforSegm,     // TI: TaskImage
                                 VM_BITSET*       pbsOptimizeTIforSegm,  // TI: TaskImage
                                 long*            plMaxRegionsWR,
                                 long*            plMaxRegionsRD,
                                 unsigned long    ulTrgVer)
{
    IXMLDOMNode*          pNode;
    FC_CString            jot2, jot3, jot4, jot5;


    pNList->get_item(0, &pNode);
    assert(pNode);
    

    if (ulTrgVer < 21040)
    {
        CGT_XMLGetStrAttrTrim(pdm, pNode, L"USE_FOR_SEGMENT", &jot4, true, true);
    
        if ( ! jot4.isEmpty())
        {
            if ( ! CGT_XMLGetStrAttrTrim(pdm, pNode, L"OPTIMIZE_FOR_SEGMENT", &jot5, true, false))
                goto Error;
            if ( ! CGT_XMLGetStrAttrTrim(pdm, pNode, L"MAX_WRITE_REGIONS_PER_SEGM", &jot2, true, false))
                goto Error;
            if ( ! CGT_XMLGetStrAttrTrim(pdm, pNode, L"MAX_READ_REGIONS_PER_SEGM", &jot3, true, false))
                goto Error;
        
            memset(plMaxRegionsWR, 0, sizeof(long)*(VM_SEGM_INST+1));   // reset
            ReadNumberPerSegment(pdm, pszKADFileAndPath, pNode, 
                _T("MAX_WRITE_REGIONS_PER_SEGM"), jot2, plMaxRegionsWR);
    
            memset(plMaxRegionsRD, 0, sizeof(long)*(VM_SEGM_INST+1));   // reset
            ReadNumberPerSegment(pdm, pszKADFileAndPath, pNode, 
                _T("MAX_READ_REGIONS_PER_SEGM") , jot3, plMaxRegionsRD);
        }


        // read segments which use a TASKIMAGE
        *pbsUseTIforSegm = 0;    // reset
        if ( ! MakeSegmentTypeBitset(pdm, pszKADFileAndPath, pNode, _T("USE_FOR_SEGMENT"), jot4, pbsUseTIforSegm))
            goto Error;

        // read the allowed array index data types
        *pbsOptimizeTIforSegm = 0;   // reset
        if ( ! MakeSegmentTypeBitset(pdm, pszKADFileAndPath, pNode, _T("OPTIMIZE_FOR_SEGMENT"), jot5, pbsOptimizeTIforSegm))
            goto Error;
    }
    else
    { // firmware version >= 21040
        memset(plMaxRegionsWR, 0, sizeof(long)*(VM_SEGM_INST+1));   // reset
        memset(plMaxRegionsRD, 0, sizeof(long)*(VM_SEGM_INST+1));   // reset

        *pbsUseTIforSegm = 0;       // reset
        *pbsOptimizeTIforSegm = 0;  // reset
        if (CGT_XMLGetStrAttrTrim(pdm, pNode, L"MAX_WRITE_REGIONS_PER_TASK", &jot2, true, true))
        {
            // evaluate number
            ReadNumberPerTask(pdm, pNode, _T("MAX_WRITE_REGIONS_PER_TASK"), jot2, 
                plMaxRegionsWR + VM_SEGM_OUTPUT);
        }

        if (CGT_XMLGetStrAttrTrim(pdm, pNode, L"MAX_READ_REGIONS_PER_TASK", &jot3, true, true))
        {
            // evaluate number
            ReadNumberPerTask(pdm, pNode, _T("MAX_READ_REGIONS_PER_TASK"), jot3, 
                plMaxRegionsRD + VM_SEGM_OUTPUT);
            plMaxRegionsRD[VM_SEGM_INPUT] = plMaxRegionsRD[VM_SEGM_OUTPUT];
        }

        if (plMaxRegionsRD[VM_SEGM_OUTPUT] > 0 && plMaxRegionsWR[VM_SEGM_OUTPUT] > 0) 
        {
            *pbsUseTIforSegm        |= VM_BS_SEGM_OUTPUT;
            *pbsOptimizeTIforSegm   |= VM_BS_SEGM_OUTPUT;
        }

        if (plMaxRegionsRD[VM_SEGM_INPUT] > 0 /*&& plMaxRegionsWR[VM_SEGM_INPUT] > 0*/) 
        {
            *pbsUseTIforSegm        |= VM_BS_SEGM_INPUT;
            *pbsOptimizeTIforSegm   |= VM_BS_SEGM_INPUT;
        }
    }

Error:
    FC_RELEASE_PTR(pNode);
}

//TODO: dieses file wegwerfen !
static void ReadVmSettings(CGT_CDiagMsg*    pdm,
                           IXMLDOMNodeList* pNList,
                           VM_SETTINGS*     pVmSettings)
{
    IXMLDOMNode*          pNode;
    TCHAR*                psz;
    FC_CString            strval;

    pNList->get_item(0, &pNode);

   
    if ( ! CGT_XMLGetStrAttrTrim(pdm, pNode, L"ALIGN", &strval, true, false))
        goto Error;
    pVmSettings->instAlignBytes = _tcstoul(strval, &psz, 10);


    if ( ! CGT_XMLGetStrAttrTrim(pdm, pNode, L"OP_MINSIZE", &strval, true, false))
        goto Error;
    pVmSettings->opMinSize = _tcstoul(strval, &psz, 10);

    if ( ! CGT_XMLGetStrAttrTrim(pdm, pNode, L"ENDIAN", &strval, true, false))
        goto Error;
    pVmSettings->bigEndian = !_tcsicmp(strval, "BIG");


    if ( ! CGT_XMLGetStrAttrTrim(pdm, pNode, L"DATATYPE_TIME", &strval, true, false))
        goto Error;
    pVmSettings->timeSizeRes = 32;

    if ( ! CGT_XMLGetStrAttrTrim(pdm, pNode, L"STACK", &strval, true, false))
        goto Error;
    pVmSettings->stackAlignBytes = _tcstoul(strval, &psz, 10) / 8;


    if(!CGT_XMLGetStrAttrTrim(pdm, pNode, L"MAX_OBJECTS", &strval, true, false))
        goto Error;
    pVmSettings->ulMaxObjects = _tcstoul(strval, &psz, 0);

    
    //TODO make non optional !!!
    pVmSettings->ulMaxCodeObjects = 0xffffffff;
    if(CGT_XMLGetStrAttrTrim(pdm, pNode, L"MAX_CODE_OBJECTS", &strval, true, true))
        pVmSettings->ulMaxCodeObjects = _tcstoul(strval, &psz, 0);


    // ISEG_START is optional
    CGT_XMLGetStrAttrTrim(pdm, pNode, L"ISEG_START", &pVmSettings->szISegStart, true, true);

    // ISEG_SIZE is optional
    CGT_XMLGetStrAttrTrim(pdm, pNode, L"ISEG_SIZE", &pVmSettings->szISegSize, true, true);
    if(pVmSettings->szISegSize.len())
        pVmSettings->ulISegSize = _tcstoul(pVmSettings->szISegSize, &psz, 16);
    else
        pVmSettings->ulISegSize = 0;

    // OSEG_START is optional
    CGT_XMLGetStrAttrTrim(pdm, pNode, L"OSEG_START", &pVmSettings->szOSegStart, true, true);
    
    // OSEG_SIZE is optional
    CGT_XMLGetStrAttrTrim(pdm, pNode, L"OSEG_SIZE", &pVmSettings->szOSegSize, true, true);
    if(pVmSettings->szOSegSize.len())
        pVmSettings->ulOSegSize = _tcstoul(pVmSettings->szOSegSize, &psz, 16);
    else
        pVmSettings->ulOSegSize = 0;
    
    // GMSEG_START is optional
    CGT_XMLGetStrAttrTrim(pdm, pNode, L"GMSEG_START", &pVmSettings->szGMSegStart, true, true);
    
    // GMSEG_SIZE is optional
    CGT_XMLGetStrAttrTrim(pdm, pNode, L"GMSEG_SIZE", &pVmSettings->szGMSegSize, true, true);
    if(pVmSettings->szGMSegSize.len())
        pVmSettings->ulGMSegSize = _tcstoul(pVmSettings->szGMSegSize, &psz, 16);
    else
        pVmSettings->ulGMSegSize = 0;
    
    // GMRSEG_START is optional
    CGT_XMLGetStrAttrTrim(pdm, pNode, L"GMRSEG_START", &pVmSettings->szGMRSegStart, true, true);
    
    // GMRSEG_SIZE is optional
    CGT_XMLGetStrAttrTrim(pdm, pNode, L"GMRSEG_SIZE", &pVmSettings->szGMRSegSize, true, true);
    
    // LOCDATASEG_START is optional
    CGT_XMLGetStrAttrTrim(pdm, pNode, L"LOCDATASEG_START", &pVmSettings->szLocDataSegStart, true, true);
    
    // LOCDATASEG_SIZE is optional
    CGT_XMLGetStrAttrTrim(pdm, pNode, L"LOCDATASEG_SIZE", &pVmSettings->szLocDataSegSize, true, true);
    
    // CODESEG_START is optional
    CGT_XMLGetStrAttrTrim(pdm, pNode, L"CODESEG_START", &pVmSettings->szCodeSegStart, true, true);
    
    // CODESEG_SIZE is optional
    CGT_XMLGetStrAttrTrim(pdm, pNode, L"CODESEG_SIZE", &pVmSettings->szCodeSegSize, true, true);
    
    // GMIEC_OFFSET is optional
    CGT_XMLGetStrAttrTrim(pdm, pNode, L"GMIEC_OFFSET", &pVmSettings->szGMiecOffs, true, true);

    // GMIEC_SIZE is optional
    CGT_XMLGetStrAttrTrim(pdm, pNode, L"GMIEC_SIZE", &pVmSettings->szGMiecSize, true, true);

    if ( ! pVmSettings->szGMiecOffs.isEmpty())
        pVmSettings->ulGMiecOffset = _tcstoul(pVmSettings->szGMiecOffs, &psz, 16);
    else
        pVmSettings->ulGMiecOffset = 0;

Error:
    FC_RELEASE_PTR(pNode);
}

// parse: <CG_OPTIONS  POST_COMPILER="path+exe-name" POST_CALL="[%targetgenpath %postopt]|[<text>]" CUSTOM_DECLARATION="{AF69A710-4874-11D5-8546-009027A31158}" CUSTOM_DECL_CONFIG="4cOsai\\AutoDecl.ini" /> 
static void ReadCgOptions(CGT_CDiagMsg*    pdm,
                          IXMLDOMNodeList* pNList,
                          CG_OPTIONS*      pCgOptions)
{
    FC_CString              strval(MAX_PATH);
    IXMLDOMNode*            pNode;

    pNList->get_item(0, &pNode);

    if(CGT_XMLGetStrAttrTrim(pdm, pNode, L"MEMINFO", &strval, true, true))
        pCgOptions->bMemInfo = !_tcsicmp(strval, "1");
    
    CGT_XMLGetStrAttrTrim(pdm, pNode, L"POST_COMPILER", &pCgOptions->szPostCompileExe, true, true);

    // if POST_COMPILER is given POST_CALL is not optional
    CGT_XMLGetStrAttrTrim(pdm, 
                         pNode,
                         L"POST_CALL",
                         &pCgOptions->szPostCall,
                         true,
                         pCgOptions->szPostCompileExe.isEmpty());

    CGT_XMLGetCLSIDAttr(pdm, pNode, L"CUSTOM_DECLARATION", &pCgOptions->clsidAutoDecl, true);

    // if CUSTOM_DECLARATION is given CUSTOM_DECL_CONFIG is not optional
    CGT_XMLGetStrAttrTrim(pdm, 
                         pNode,
                         L"CUSTOM_DECL_CONFIG",
                         &pCgOptions->szAutoDeclIniFile,
                         true,
                         pCgOptions->clsidAutoDecl.Data1 == 0);

    FC_RELEASE_PTR(pNode);
}




static void ReadOffSize(
    CGT_CDiagMsg*  pdm,
    IXMLDOMNode*   pNode,
    const wchar_t* pszPrFix, 
    unsigned long* plOff, 
    unsigned long* plSize
)
{
    FC_CString jot(16);
    wchar_t    xSEG_OFFS[64];
    wchar_t    xCHN_SIZE[64];

    wcscat(wcscpy(xSEG_OFFS, pszPrFix), L"SEG_OFFS");
    wcscat(wcscpy(xCHN_SIZE, pszPrFix), L"CHN_SIZE");

    if(CGT_XMLGetStrAttrTrim(pdm, pNode, xSEG_OFFS, &jot, true, false))
    {
        if(!CGT_ParseUINT32(jot, plOff))
            CGT_XMLDiagAttrValue(pdm, pNode, FC_CStr(xSEG_OFFS), jot, _T("unsigned long"));
    }

    if(CGT_XMLGetStrAttrTrim(pdm, pNode, xCHN_SIZE, &jot, true, false))
    {
        if(!CGT_ParseUINT32(jot, plSize))
            CGT_XMLDiagAttrValue(pdm, pNode, FC_CStr(xCHN_SIZE), jot, _T("unsigned long"));
    }

    if(*plSize == 0) //if size is zero the segment type is not used at all
        *plOff = 0;  //to avoid any ambiguity force the offset to zero
                     //it is considered a feature that the KAD user needs only to 
                     //set the size to 0x0 in order to "comment out" a segment type
                     //for a channel.
}  


static void ReadChannels(
    CGT_CDiagMsg*            pdm,
    IXMLDOMNodeList*         pNList,
    FC_CGArray<CHANNEL_TYP*>*pChannels
)
{
    long                  i;
    long                  len;
    FC_CString            jot(MAX_PATH);
    IXMLDOMNode*          pNode;
    CHANNEL_TYP*          pIOC;

    pNList->get_length(&len);
    for(i=0;i<len;i++)
    {
        pNList->get_item(i, &pNode);

        pIOC = new CHANNEL_TYP;
        pChannels->add(pIOC);


        CGT_XMLGetStrAttrTrim(pdm, pNode, L"NUM", &jot, true, false);
        if(!_tcsicmp(jot, _T("userdefined")))
            pIOC->lChannelNum = -1;
        else if(!_tcsicmp(jot, _T("notused")))
            pIOC->lChannelNum = -2;
        else if(! (CGT_ParseINT32(jot, &pIOC->lChannelNum)) || pIOC->lChannelNum<0)
            CGT_XMLDiagAttrValue(pdm, pNode, "NUM", jot, _T("userdefined|notused|0..N"));


        //EXT and CONFIGURATOR are not optional any more (since V2.1.3):
        if(!CGT_XMLGetStrAttrTrim(pdm, pNode, L"EXT", &pIOC->szExt, true, false))
            return;
        if(!CGT_XMLGetStrAttrTrim(pdm, pNode, L"CONFIGURATOR", &jot, true, false))
            return;

        if(!jot.isEmpty() || pIOC->lChannelNum<0)
        {   //force CONFIGURATOR="<clsid>" valid clsid if NUM="userdefined"|"notused"
            pIOC->bUseConfigurator = true;
            if(!CGT_XMLGetCLSIDAttr(pdm, pNode, L"CONFIGURATOR", &pIOC->clsidFBUS, false))
                return;
        }
        //if CONFIGURATOR is given we need always an EXT:
        if(pIOC->bUseConfigurator && pIOC->szExt.isEmpty())
            CGT_XMLDiagAttrValue(pdm, pNode, "EXT", "", _T("<file extention>"));


        if(CGT_XMLGetStrAttrTrim(pdm, pNode, L"INIT", &jot, true, false))
        {
            if(!_tcsicmp(jot, _T("0")))
                pIOC->bInitialise = false;
            else if(!_tcsicmp(jot, _T("1")))
                pIOC->bInitialise = true;
            else
                CGT_XMLDiagAttrValue(pdm, pNode, "INIT", jot, _T("0|1"));
        }

        // [I|O|GM]SEG_OFFS,[I|O|GM]CHN_SIZE are never optional:
        ReadOffSize(pdm, pNode, L"I",  &pIOC->ulISegOffs, &pIOC->ulIChnSize);
        ReadOffSize(pdm, pNode, L"O",  &pIOC->ulOSegOffs, &pIOC->ulOChnSize);
        ReadOffSize(pdm, pNode, L"GM", &pIOC->ulGMSegOffs, &pIOC->ulGMChnSize);
        
        FC_RELEASE_PTR(pNode);
    }
}


static void SortChannelsByOffsetAndTest(
    CGT_CDiagMsg*       pdm,
    const CG_Edipos*    pEdp,
    CHANNEL_TYP*        a[/*n*/], 
    int                 n, 
    char                cIQM,
    unsigned long       ulMaxIQM 
)
{
    int i, j;
    
    //this algorithm works also if some Sizes AND offsets 
    //are zero (=means not used), see also: static void ReadOffSize

    //bubble sort the array to increasing values:
    #define TSWAP(T, a,b){T t; t=a; a=b; b=t; }
    for(i=0;i<n;i++)//Make n passes through the array
    {
        for(j=1;j<n-i;j++)//From the first element to the end of the unsorted section
        {
           if(a[j-1]->getOff(cIQM) > a[j]->getOff(cIQM)) 
              TSWAP(CHANNEL_TYP*, a[j-1],a[j]);//If adjacent items are out of order, swap them
        }
    }


    //test for overlapping offset and size values:
//    for(i=0;i<n-1;i++)
//    {
//       if(a[i+1]->getSize(cIQM)>0) //special case: if size is 0 the channel is not used at all
//       {                           //would be nicer to exclude those channels at beginn of this algorithm...
//           if(a[i]->getOff(cIQM) + a[i]->getSize(cIQM) > a[i+1]->getOff(cIQM))
//           {
//               pdm->msg1(CG_E_TEXT, pEdp, pdm->jot1()
//                <<"overlapping channels: %"<<cIQM<<" area of channels "
//                <<a[i]->lChannelNum <<" and " <<a[i+1]->lChannelNum
//                <<" do overlapp");
//           }
//       }
//    }

    //test that the highest reached byte of all channel is in range of segment:
    if(n>0)
    {
        unsigned __int64 highestOff  = a[n-1]->getOff(cIQM);
                         highestOff += a[n-1]->getSize(cIQM);
        if(highestOff > ulMaxIQM)
        {
           pdm->msg1(CG_E_TEXT, pEdp, pdm->jot1()
             <<"channels number "<<a[n-1]->lChannelNum
             <<": %"<<cIQM<<" area exceeds total segment size");
        }
    }
}

#define VM_MAXCHANNELNUM 512
static void ValidateChannels(
    CGT_CDiagMsg*             pdm,
    const CG_Edipos*          pEdp,
    FC_CGArray<CHANNEL_TYP*>* pChannels,
    const VM_SETTINGS*        pVMSettings
)
{
    int           i;
    int           nCh;
    bool          bUsedChNumbers[VM_MAXCHANNELNUM+1];
    CHANNEL_TYP** ppCh;
    int           lChannelNum;


    memset(bUsedChNumbers, 0, sizeof(bUsedChNumbers));

    nCh =  pChannels->getUsed();
    ppCh = pChannels->getElements();


    //test: only exact one Channel if channel number shall not be used:
    for(i=0; i<nCh; i++)
    {
        lChannelNum = ppCh[i]->lChannelNum;
        if(lChannelNum == -2)
        {
            if(nCh != 1)
            {
               pdm->msg1(CG_E_TEXT, pEdp, "channel number NUM=\"notused\" but more than one <CHANNEL>");
               return;
            }

            //TODO: we need this for wago:
            pdm->errNS1(pEdp, _T("<CHANNEL> without channel number"));
        }
        else if(lChannelNum == -1)
        {
            if(!ppCh[i]->bUseConfigurator || ppCh[i]->szExt.isEmpty())
               pdm->msg1(CG_E_TEXT, pEdp, "channel number is user defined but no configurator or file extension set");
        }
        else if(lChannelNum<0 || lChannelNum>VM_MAXCHANNELNUM)
        {
            pdm->msg1(CG_E_TEXT, pEdp, pdm->jot1()
                <<"channel number '"<<lChannelNum
                <<"' out of range, allowed range [0.."<<VM_MAXCHANNELNUM<<"0]");
        }
        else if(bUsedChNumbers[lChannelNum])
        {
            pdm->msg1(CG_E_TEXT, pEdp, pdm->jot1()
              <<pdm->jot1()<<"channel number '"<<lChannelNum<<"' used twice");
        }
        else
        {
            bUsedChNumbers[lChannelNum] = true;
        }
    }

    SortChannelsByOffsetAndTest(pdm, pEdp, ppCh, nCh, 'I', pVMSettings->ulISegSize);
    SortChannelsByOffsetAndTest(pdm, pEdp, ppCh, nCh, 'Q', pVMSettings->ulOSegSize);
    SortChannelsByOffsetAndTest(pdm, pEdp, ppCh, nCh, 'M', pVMSettings->ulGMSegSize);
}

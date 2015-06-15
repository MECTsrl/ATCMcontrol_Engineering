/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/asm/globalContext.cpp 9     13.06.07 18:36 Ef $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: globalContext.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/asm/globalContext.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / CCG
 *
 * =COMPONENT			ASM
 *
 * =CURRENT 	 $Date: 13.06.07 18:36 $
 *			 $Revision: 9 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

static char szversioninfo[] = "$Header: /4CReleased/V2.20.00/TargetBase/CCG/asm/globalContext.cpp 9     13.06.07 18:36 Ef $";


#include <time.h>
#include <limits.h>

#include "asmcps.h"
#include "asmtools.h"
#include "globalContext.h"
#include "AsmParse.h"
#include "vmSharedDef.h"

#define OPC_CODE_GEN 1
#include "intOpcds.h"
#undef  OPC_CODE_GEN

#include "VmConstants.h"

static void ASM_DumpUserText(const TCHAR* pszText)
{
    _fputts(pszText, stdout);
    fflush(stdout);
}

static int ASM_GetObjIdxFromSeg(ASM_SEGM_TYP seg)
{
	if(GetGC()->getTrgAttr()->trg_version >= 21000)
	{
		switch(seg)
		{
		case ASM_SEGM_GLOBAL: return MAX_SEGMENTS;
		case ASM_SEGM_RETAIN: return MAX_SEGMENTS+1;
		default:  	 		  return seg;
		}
	}
	else
		return seg;	
}


FileContext_C::FileContext_C(const TCHAR* pszFile) : m_fileName(pszFile)
{
    m_errorInFile = 0;
}



ASMGlobalContext_C* ASMGlobalContext_C::pTheGC;
ASMGlobalContext_C* GetGC(){return ASMGlobalContext_C::pTheGC;}
ASMGlobalContext_C::ASMGlobalContext_C()
 : m_hashFuns(false, false),
   m_hashPrgFB(false, false),
   m_hashGlobComp(false, false),
   m_hashTypes(false, false),
   m_hashObjects(false, false),
   m_hashOpCodes(false, false),
   m_hashOPC(false, false, 1013),
   m_hashClasses(true, false),
   m_hashTasks(false, false),
   m_hashPrgInstTasks(false)
{

    int            i;
    ASM_DATA_TYP*  pbt;

    assert(ASMGlobalContext_C::pTheGC==NULL);

    m_clsid_1st   = 0;

    assert(ASM_TYPE_UNDEF==0);
    for(i=ASM_TYPE_UNDEF; i<=ASM_LAST_TYPE_ENUM; i++)
    {
        pbt = new ASM_DATA_TYP;
        pbt->type    =  (ASM_TYPE_ENUM)i;
        pbt->name    = registerId(ASM_TypeEnumToString((ASM_TYPE_ENUM)i));
        pbt->pFBIDef = NULL;
        pbt->lStrlen = LONG_MIN;    
        pbt->nElems  = 1;
        pbt->bArray  = false;
        
        m_registeredTypes.add(pbt);
        m_hashTypes.put(pbt, pbt->name.psz, false);
    }

    m_objectNr = MAX_SEGMENTS;
   
    pTheGC = this;
    m_pClsIdFile = NULL;

    m_ulBitOff_GM  =0;
    m_ulBitOff_GMR =0;
    m_usedGlobalRetainSize = -1;
    m_usedLocalRetainSize  = -1;
    m_ulUsedGlobalSize = -1;
    m_ulUsedGlobalSimpleSize = -1;
    m_ulTotalCodeBytes = 0;
    m_ulLocalDataSize = 0;
    m_lTotCopyRegionCount  = 0;
    m_bCreateMapFile = false;
    // für unseren abdeckungstest
    memset(m_opcodes, 0, 1024*64*4);
}

ASMGlobalContext_C::~ASMGlobalContext_C()
{
    int        i;

    pTheGC = NULL;

    for(i=0; i<m_funList.getMaxUsed(); i++)
        delete(m_funList.get(i));

    for(i=0; i<m_prgFBList.getMaxUsed(); i++)
        delete(m_prgFBList.get(i));

    for(i=0; i<m_globComp.getMaxUsed(); i++)
        delete(m_globComp.get(i));

    for(i=0; i<m_fileContext.getMaxUsed(); i++)
        delete(m_fileContext.get(i));

    for(i=0; i<m_registeredTypes.getMaxUsed(); i++)
        delete(m_registeredTypes.get(i));

    for(i=0; i<m_objectList.getMaxUsed(); i++)
        delete(m_objectList.get(i));

    for(i=0; i<m_taskList.getMaxUsed(); i++)
        delete(m_taskList.get(i));

    m_ListOfInitLists.deletePtrs();
    m_ListOfLitLists.deletePtrs();
    m_listOfIdLists.deletePtrs();
    m_copySection.deletePtrs();
    m_channelList.deletePtrs();
}


void ASMGlobalContext_C::parseCommandLine(int argc_in, TCHAR** argv_in)
{
    int          i;
    const TCHAR* pszArg;
    const TCHAR* pszExt;
    bool         bNologo = false; //TODO print logo
    FC_CString   jot(MAX_PATH);
    

    for(i=1;i<argc_in; )
    {
        pszArg = argv_in[i++]; 

        if(*pszArg=='-' || *pszArg=='/')
        {
            pszArg++;
            if(!_tcsicmp(pszArg, _T("nologo")))
               bNologo = true;
            else if(!_tcsicmp(pszArg, _T("map")))
               m_bCreateMapFile = true;
            else
                ST_DiagMsg1(ASM_CMDLEdiPos(), E_4CCG_ERR_BAD_CMD_UNKNOWN_OPT, pszArg);

            //ST_DiagMsg1(pEdp, E_4CCG_WARN_CMD_OPT_TWICE, pszArg);
        }
        else
        {
            //pure file argument must be the 4asm file
            //check extension:
            pszExt = FC_StringGetFileExt(pszArg);
            if(!pszExt || _tcsicmp(pszExt, _T("4asm")))
            {
                ST_DiagMsg2(ASM_CMDLEdiPos(), E_4CCG_ERR_BAD_CML_FILETYPE, pszArg, _T("4asm"));
                return;
            }

            if(m_asmFileName[0])
                ST_DiagMsg0(ASM_CMDLEdiPos(), E_4CCG_ERR_BAD_CMD_LINE);
            else
                m_asmFileName.load(pszArg);
        }
    }

    if(m_asmFileName[0]==0)
    {
        ST_DiagMsg0(NULL, E_4CCG_ERR_CML_NOASM);
        return;
    }

    //check here: existance of 4asm file: 
    if(!FC_Exist(m_asmFileName))
    {
        ST_DiagMsg1(ASM_CMDLEdiPos(), E_4CCG_ERR_FILE_NOT_EXIST, m_asmFileName);
    }
    else
    {
        //build target gen path:
        TCHAR  fullPath[2*MAX_PATH];
        FC_FileFullPath(NULL, m_asmFileName, fullPath, FC_ARRAY_LEN(fullPath));
        FC_StringCutFilePart(fullPath);
        strcat(fullPath, _T("\\"));// HACK
        m_GenPath.load(fullPath);

        jot.load(m_asmFileName).stripFilePart();
        jot<<_T("\\meminfo.txt");
        m_memInfoFileName.load(jot);
        DeleteFile(m_memInfoFileName);
    }

    // open map file
    if(m_bCreateMapFile)
    {
        jot.load(m_asmFileName);
        jot.changeFileExt(FILE_EXT_MAPFILE);
        TO_OpenMapFile(jot);
    }
}

void ASMGlobalContext_C::addTargetAttribute(const ST_EdiPos* pEdp, ST_IDENT_TYP key, const ASM_LITERAL_TYP& val)
{
    FC_CString    text;
    __int64       minval = 0;
    __int64       maxval = 0;
    bool          bCheckRange;
    ASM_TYPE_ENUM expectedType = ASM_DINT;

    
    bCheckRange = true; 
    if(!_tcscmp(key.psz, TAG_TARGETTYPE))
    {
        if(!m_trgAttr.trg_type.isEmpty())
            goto err_redef;
        if(val.type != ASM_STRING)
            goto err_type;
 
        bCheckRange = false;
        m_trgAttr.trg_type.loadLen(val.pszIECStr+1, _tcslen(val.pszIECStr)-2);
    }
    else if(!_tcscmp(key.psz, TAG_FIRMWAREVERS))
    {
        if(m_trgAttr.trg_version != 0x0)
            goto err_redef;
        if(val.type != ASM_DWORD)
            goto err_type;
        
        minval = 20000;
        maxval = 0xffffffff;
        m_trgAttr.trg_version = (unsigned long)val.any64;
    }
    else if(!_tcscmp(key.psz, _T("maxobjects")))
    {
        if(val.type != ASM_DWORD)
            goto err_type;
        bCheckRange = false;
        m_trgAttr.trg_maxObjects = (unsigned long)val.any64;
    }
    else if(!_tcscmp(key.psz, _T("dumpinfo")))
    {
        if(val.type != ASM_BOOL)
            goto err_type;
        minval = 0;
        maxval = 0x1;
        m_trgAttr.trg_dumpinfo = val.any64 ? true : false;
    }
    else if(!_tcscmp(key.psz, _T("instAlignBytes")))
    {
        if(m_trgAttr.trg_instAlignBytes > 0)
            goto err_redef;
        if(val.type != ASM_DINT)
            goto err_type;
                        
        minval = 1;
        maxval = 8;
        m_trgAttr.trg_instAlignBytes = (unsigned long)val.any64;
    }
    else if(!_tcscmp(key.psz, _T("stackAlignBytes")))
    {
        if(m_trgAttr.trg_stackAlignBytes > 0)
            goto err_redef;
        if(val.type != ASM_DINT)
            goto err_type;
        
        minval = 1;
        maxval = 8;
        m_trgAttr.trg_stackAlignBytes = (unsigned long)val.any64;
    }
    else if(!_tcscmp(key.psz, _T("bigEndian")))
    {
        if(m_trgAttr.trg_bigEndian >= 0)
            goto err_redef;
        if(val.type != ASM_DINT)
            goto err_type;
                        
        maxval = 1;
        m_trgAttr.trg_bigEndian = (unsigned long)val.any64;
    }
    else if(!_tcscmp(key.psz, _T("minOperandSize")))
    {
        if(m_trgAttr.trg_minOperandSize > 0)
            goto err_redef;
        if(val.type != ASM_DINT)
            goto err_type;
                        
        minval = 1;
        maxval = 4;
        m_trgAttr.trg_minOperandSize = (unsigned long)val.any64;
    }
    else if(!_tcscmp(key.psz, TAG_DBGSETTING))
    {
        if(m_trgAttr.trg_dgbSetting != 0xffffffff)
            goto err_redef;
        if(val.type != ASM_DINT)
            goto err_type;

        maxval = 2;
        m_trgAttr.trg_dgbSetting = (unsigned long)val.any64;
    }
    else if(!_tcscmp(key.psz, TAG_ISEGSTART))
    {
        if(m_trgAttr.trg_ISegStart != 0xffffffff)
            goto err_redef;
        if(val.type != ASM_LINT)
            goto err_type;

        maxval = UINT_MAX;//?????????
        m_trgAttr.trg_ISegStart = (unsigned long)val.any64;
    }
    else if(!_tcscmp(key.psz, TAG_ISEGSIZE))
    {
        if(m_trgAttr.trg_ISegSize != 0xffffffff)
            goto err_redef;
        if(val.type != ASM_LINT)
            goto err_type;

        maxval = UINT_MAX;//?????????
        m_trgAttr.trg_ISegSize = (unsigned long)val.any64;
    }
    else if(!_tcscmp(key.psz, TAG_OSEGSTART))
    {
        if(m_trgAttr.trg_OSegStart != 0xffffffff)
            goto err_redef;
        if(val.type != ASM_LINT)
            goto err_type;

        maxval = UINT_MAX;//?????????
        m_trgAttr.trg_OSegStart = (unsigned long)val.any64;
    }
    else if(!_tcscmp(key.psz, TAG_OSEGSIZE))
    {
        if(m_trgAttr.trg_OSegSize != 0xffffffff)
            goto err_redef;
        if(val.type != ASM_LINT)
            goto err_type;

        maxval = UINT_MAX;//?????????
        m_trgAttr.trg_OSegSize = (unsigned long)val.any64;
    }
    else if(!_tcscmp(key.psz, TAG_GMSEGSTART))
    {
        if(m_trgAttr.trg_GMSegStart != 0xffffffff)
            goto err_redef;
        if(val.type != ASM_LINT)
            goto err_type;

        maxval = UINT_MAX;//?????????
        m_trgAttr.trg_GMSegStart = (unsigned long)val.any64;
    }
    else if(!_tcscmp(key.psz, TAG_GMSEGSIZE))
    {
        if(m_trgAttr.trg_GMSegSize != 0xffffffff)
            goto err_redef;
        if(val.type != ASM_LINT)
            goto err_type;

        maxval = UINT_MAX;//?????????
        m_trgAttr.trg_GMSegSize = (unsigned long)val.any64;
    }
    else if(!_tcscmp(key.psz, TAG_GMRSEGSTART))
    {
        if(m_trgAttr.trg_GMRSegStart != 0xffffffff)
            goto err_redef;
        if(val.type != ASM_LINT)
            goto err_type;

        maxval = UINT_MAX;//?????????
        m_trgAttr.trg_GMRSegStart = (unsigned long)val.any64;
    }
    else if(!_tcscmp(key.psz, TAG_GMRSEGSIZE))
    {
        if(m_trgAttr.trg_GMRSegSize != 0xffffffff)
            goto err_redef;
        if(val.type != ASM_LINT)
            goto err_type;

        maxval = UINT_MAX;//?????????
        m_trgAttr.trg_GMRSegSize = (unsigned long)val.any64;
    }
    else if(!_tcscmp(key.psz, TAG_CODESEGSTART))
    {
        if(m_trgAttr.trg_CodeSegStart != 0xffffffff)
            goto err_redef;
        if(val.type != ASM_LINT)
            goto err_type;

        maxval = UINT_MAX;//?????????
        m_trgAttr.trg_CodeSegStart = (unsigned long)val.any64;
    }
    else if(!_tcscmp(key.psz, TAG_CODESEGSIZE))
    {
        if(m_trgAttr.trg_CodeSegSize != 0xffffffff)
            goto err_redef;
        if(val.type != ASM_LINT)
            goto err_type;

        maxval = UINT_MAX;//?????????
        m_trgAttr.trg_CodeSegSize = (unsigned long)val.any64;
    }
    else if(!_tcscmp(key.psz, TAG_LOCDATASEGSTART))
    {
        if(m_trgAttr.trg_LocDataSegStart != 0xffffffff)
            goto err_redef;
        if(val.type != ASM_LINT)
            goto err_type;

        maxval = UINT_MAX;//?????????
        m_trgAttr.trg_LocDataSegStart = (unsigned long)val.any64;
    }
    else if(!_tcscmp(key.psz, TAG_LOCDATASEGSIZE))
    {
        if(m_trgAttr.trg_LocDataSegSize != 0xffffffff)
            goto err_redef;
        if(val.type != ASM_LINT)
            goto err_type;

        maxval = UINT_MAX;//?????????
        m_trgAttr.trg_LocDataSegSize = (unsigned long)val.any64;
    }
    else if(!_tcscmp(key.psz, TAG_GMIECOFFS))
    {
        if(m_trgAttr.trg_OSegStart != 0xffffffff)
            goto err_redef;
        if(val.type != ASM_LINT)
            goto err_type;

        maxval = UINT_MAX;//?????????
        m_trgAttr.trg_GmIecOffs = (unsigned long)val.any64;
    }
    else if(!_tcscmp(key.psz, TAG_GMIECSIZE))
    {
        if(m_trgAttr.trg_OSegSize != 0xffffffff)
            goto err_redef;
        if(val.type != ASM_LINT)
            goto err_type;

        maxval = UINT_MAX;//?????????
        m_trgAttr.trg_GmIecSize = (unsigned long)val.any64;
    }
    else if(!_tcscmp(key.psz, TAG_PI_VERSION))
    {
        if(!m_trgAttr.trg_piVersion.isEmpty())
            goto err_redef;
        if(val.type != ASM_STRING)
            goto err_type;
 
        bCheckRange = false;
        m_trgAttr.trg_piVersion.loadLen(val.pszIECStr+1, _tcslen(val.pszIECStr)-2);
    }
    else if(!_tcscmp(key.psz, TAG_PI_NAME))
    {
        if(!m_trgAttr.trg_piName.isEmpty())
            goto err_redef;
        if(val.type != ASM_STRING)
            goto err_type;
 
        bCheckRange = false;
        m_trgAttr.trg_piName.loadLen(val.pszIECStr+1, _tcslen(val.pszIECStr)-2);
    }
    else if(!_tcscmp(key.psz, TAG_PI_MODIFIED))
    {
        if(!m_trgAttr.trg_piModified.isEmpty())
            goto err_redef;
        if(val.type != ASM_STRING)
            goto err_type;
 
        bCheckRange = false;
        m_trgAttr.trg_piModified.loadLen(val.pszIECStr+1, _tcslen(val.pszIECStr)-2);
    }
    else if(!_tcscmp(key.psz, TAG_PI_CREATED))
    {
        if(!m_trgAttr.trg_piCreated.isEmpty())
            goto err_redef;
        if(val.type != ASM_STRING)
            goto err_type;
 
        bCheckRange = false;
        m_trgAttr.trg_piCreated.loadLen(val.pszIECStr+1, _tcslen(val.pszIECStr)-2);
    }
    else if(!_tcscmp(key.psz, TAG_PI_OWNER))
    {
        if(!m_trgAttr.trg_piOwner.isEmpty())
            goto err_redef;
        if(val.type != ASM_STRING)
            goto err_type;
 
        bCheckRange = false;
        m_trgAttr.trg_piOwner.loadLen(val.pszIECStr+1, _tcslen(val.pszIECStr)-2);
    }
    else if(!_tcscmp(key.psz, TAG_PI_COMMENT1))
    {
        if(!m_trgAttr.trg_piComment1.isEmpty())
            goto err_redef;
        if(val.type != ASM_STRING)
            goto err_type;
 
        bCheckRange = false;
        m_trgAttr.trg_piComment1.loadLen(val.pszIECStr+1, _tcslen(val.pszIECStr)-2);
    }
    else if(!_tcscmp(key.psz, TAG_PI_COMMENT2))
    {
        if(!m_trgAttr.trg_piComment2.isEmpty())
            goto err_redef;
        if(val.type != ASM_STRING)
            goto err_type;
 
        bCheckRange = false;
        m_trgAttr.trg_piComment2.loadLen(val.pszIECStr+1, _tcslen(val.pszIECStr)-2);
    }
    else if(!_tcscmp(key.psz, TAG_PI_COMMENT3))
    {
        if(!m_trgAttr.trg_piComment3.isEmpty())
            goto err_redef;
        if(val.type != ASM_STRING)
            goto err_type;
 
        bCheckRange = false;
        m_trgAttr.trg_piComment3.loadLen(val.pszIECStr+1, _tcslen(val.pszIECStr)-2);
    }
    else
    {
        ST_DiagMsg1(pEdp, E_4CCG_WARN_UNKNOWN_ATTRIB, key.psz);
        return;
    }

   
    if(bCheckRange && (val.any64 < minval || val.any64 > maxval))
    {
        ASM_LitToString(text, &val);
        ST_DiagMsg4(pEdp, E_4CCG_ERR_ATTR_VAL_RANGE, key.psz, text, Tostr_C(minval), Tostr_C(maxval));
    }
    return;

err_redef:
    ST_DiagMsg1(pEdp, E_4CCG_ERR_DOUBLE_ATTRIP, key.psz);
    return;
err_type:
    ASM_LitToString(text, &val);
    ST_DiagMsg2(pEdp, E_4CCG_ERR_EXPECTED_LIT_OF_TYPE, ASM_TypeEnumToString(expectedType), text);
}



ASM_INST_PATH_TYP ASMGlobalContext_C::registerInstPathPlusMember(const ASM_INST_PATH_TYP* pPath,
                                                                 ST_IDENT_TYP             member)
{
    ST_String_C       newpath;
    ASM_INST_PATH_TYP ret;
    
    ret.depth = 1;
    if(pPath)
    {
        ret.depth += pPath->depth;
        newpath.load(pPath->ids.psz);
        newpath<<_T(".");
    }
    
    newpath<<member.psz;
    
    ret.ids = registerId(newpath);
    
    return ret;
}

ASM_INST_PATH_TYP ASMGlobalContext_C::registerInstArrayPath(const ASM_INST_PATH_TYP& path,
                                                            const ASM_LITERAL_TYP*   pArrayIdx)
{
    FC_CString        newpath;
    ASM_INST_PATH_TYP ret;
    //TODO Check data type of lit, may be is string or float !
    newpath.load(path.ids.psz);
    newpath << _T(".") << pArrayIdx->any64;
    ret.depth = path.depth + 1;
    ret.ids = registerId(newpath);
    
    return ret;
}

ASM_INST_PATH_TYP ASMGlobalContext_C::registerInstPathPlusArrayMember(const ASM_INST_PATH_TYP* pPath,
                                                                      ST_IDENT_TYP             member,
                                                                      int                      Index)
{
    ST_String_C       newpath;
    ASM_INST_PATH_TYP ret;
    
    ret.depth = 1;
    if(pPath)
    {
        ret.depth += pPath->depth;
        newpath.load(pPath->ids.psz);
        newpath<<_T(".");
    }
    
    newpath << member.psz;
    newpath << _T(".");
    newpath << Index;
    
    ret.ids = registerId(newpath);
    
    return ret;
}

const ASM_DATA_TYP* ASMGlobalContext_C::registerType(
    ASM_TYPE_ENUM      type, 
    const ST_IDENT_TYP name, 
    const ASMPROC_TYP* pProcDef
)
{
    assert( type == ASM_PRGI || type == ASM_FBI|| type == ASM_STRI);
    const ASM_DATA_TYP* pPrevious;
    ASM_DATA_TYP* pNewType;

    pPrevious = m_hashTypes.get(name.psz);
    if(pPrevious)
        return pPrevious;

    pNewType = new ASM_DATA_TYP;
    pNewType->type    = type;
    pNewType->name    = name;
    pNewType->pFBIDef = pProcDef;
    pNewType->lStrlen = LONG_MIN;
    pNewType->nElems  = 1;
    pNewType->bArray  = false;

    m_registeredTypes.add(pNewType);

    pPrevious = m_hashTypes.put(pNewType, name.psz, false);
    assert(!pPrevious);
    return pNewType;
}

const ASM_DATA_TYP* ASMGlobalContext_C::registerType(
    ASM_TYPE_ENUM type, 
    ST_IDENT_TYP name, 
    const ASMPROC_TYP* pProcDef,
    bool bArray,
    const ASM_LITERAL_TYP* pArrayDim, 
    const ASM_LITERAL_TYP* pStrLen)
{
    ST_String_C TypeName(name.psz);
    if (bArray)
    {
        TypeName << _T("[");
        if (pArrayDim)
        {
            Tostr_C ArrayDim(pArrayDim->any64);
            TypeName << ArrayDim;
        }
        TypeName << _T("]");
    }

    if (pStrLen != NULL)
    {
        Tostr_C StrLen(pStrLen->any64);
        TypeName << _T("(");
        TypeName << StrLen;
        TypeName << _T(")");
    }

    const ASM_DATA_TYP* pPrevious = m_hashTypes.get(TypeName);
    if(pPrevious)
        return pPrevious;

    ASM_DATA_TYP* pNewType = new ASM_DATA_TYP;
    pNewType->type    = type;
    pNewType->name    = registerId(TypeName);
    pNewType->bArray  = bArray;

    if (pStrLen != NULL)
    {
        pNewType->pFBIDef = NULL;
        pNewType->lStrlen = (unsigned) pStrLen->any64;
    }
    else
    {
        pNewType->pFBIDef = pProcDef;
        pNewType->lStrlen = LONG_MIN;
    }

    if (pArrayDim != NULL)
        pNewType->nElems = (unsigned) pArrayDim->any64;
    else
        pNewType->nElems = (unsigned) 1;

    m_registeredTypes.add(pNewType);

    pPrevious = m_hashTypes.put(pNewType, pNewType->name.psz, false);
    assert(!pPrevious);
    return pNewType;
}

long ASMGlobalContext_C::registerObject(const ASM_INST_PATH_TYP&   path,
                                        const ASM_DATA_TYP*        pType,
                                        const ASM_INITIALISER_TYP* pInit,
                                        ASM_SEGM_TYP               segm,
                                        ASM_BITOFF                 bitOffs)
{
    ASM_OBJECT_TYP* pObj = new ASM_OBJECT_TYP;

    pObj->Index   = m_objectNr;
    pObj->path    = path;
    pObj->pType   = pType;
    pObj->segm    = segm;
    pObj->byteSize= ASM_GetVMStoreByteSize(pType);
    pObj->bitOffs = bitOffs;
    pObj->bInit   = false;

    if (pType->bArray && pInit)
        pObj->ListOfListOfArrayInits.add(pInit->pInitList);

    m_objectList.add(pObj);
    ASM_OBJECT_TYP* pObjPrev = m_hashObjects.put(pObj, path.ids.psz, false);
    assert(pObjPrev == NULL);

    if(bitOffs == ASM_INVALID_BITOFF)
    {
        TO_MAPWRITE5(_T("ObjNr=%03ld  Seg=%d Size=%04d Off=0000.0 Obj=%-32s : %s\n"), 
            m_objectNr, segm, pObj->byteSize, path.ids.psz, pType->name.psz);
    }
    else
    {
        TO_MAPWRITE7(_T("ObjNr=%03ld  Seg=%d Size=%04d Off=%04d.%d Obj=%-32s : %s\n"), 
            m_objectNr, segm, pObj->byteSize, (long)(bitOffs/8), (long)(bitOffs%8), path.ids.psz,pType->name.psz);
    }

    m_objectNr++;

    return m_objectNr;
}

void ASMGlobalContext_C::addProc(ASMPROC_TYP* pProc)
{
    const ASMPROC_TYP*          pProcPrev;

    if (pProc->isBuildIn())
    {
        pProc->assignClassID(-1);
    }
    else
    {
        assert(m_clsid_1st != -1);
        pProc->assignClassID(m_clsid_1st++);
    }

    if(pProc->getPouTyp()==ASM_FUN)
    {
        m_funList.add(pProc);
        pProcPrev =  m_hashFuns.put(pProc, pProc->getName(), false);
    }
    else
    {
        m_prgFBList.add(pProc);
        pProcPrev =  m_hashPrgFB.put(pProc, pProc->getName(), false);
    }

    if(pProcPrev)
    {
        TCHAR buffer[EDP_MAX_EDP_STR_LEN];
        ST_EdiPosToString(pProcPrev->getEdp(), buffer);
        ST_DiagMsg2(pProc->getEdp(), E_4CCG_ERR_POU_REDEF, pProc->getName(), buffer);
    }
}

void ASMGlobalContext_C::registerClassName(ST_IDENT_TYP ClassName, long ClsId)
{
    TCHAR buf[255];
    _ltoa(ClsId, buf, 10);
    LPCTSTR pPrev = m_hashClasses.put(ClassName.psz, buf, false);
    assert(pPrev == NULL);

//    fprintf(m_pClsIdFile, _T("%s %s\n"), ClassName.psz, buf);
}

LPCTSTR ASMGlobalContext_C::getClassName(long ClsId)const
{
    TCHAR buf[255];
    LPCTSTR pClassName = m_hashClasses.get(_ltoa(ClsId, buf, 10));
    // TODO: activate assertion after detroying hack for mlib FBs
    // assert(pClassName != NULL);
    return pClassName;
}

void ASMGlobalContext_C::addTaskProperties(const ST_EdiPos*       pEdp,
                                           ST_IDENT_TYP           TaskIdent,
                                           const ASM_LITERAL_TYP& TaskId,
                                           const ASM_LITERAL_TYP& autoStart,
                                           const ASM_LITERAL_TYP& intTime,
                                           const ASM_LITERAL_TYP& Prio,
                                           const ASM_LITERAL_TYP& Loaded,
                                           const ASM_LITERAL_TYP& Events,
                                           const ASM_LITERAL_TYP& WatchDog)
{
    ASM_TASK_TYP* taskTyp = new ASM_TASK_TYP;
    ASM_InitTaskTyp(taskTyp);

    m_taskList.add(taskTyp);

    const ASM_TASK_TYP* pPrev = m_hashTasks.put(taskTyp, TaskIdent.psz, false);
    assert(pPrev == NULL);

    taskTyp->name = TaskIdent;
    taskTyp->taskId = (int) TaskId.any64;
    taskTyp->autoStart = (int) autoStart.any64;

    if (Events.type == ASM_TYPE_UNDEF)
	{
		taskTyp->bEventSet	= FALSE;
        taskTyp->Events		= 0;
	}
    else
	{
		taskTyp->bEventSet	= TRUE;
        taskTyp->Events		= Events.any64;
	}

	/* A cyclic task wins (overrules event definition)!
	 */
    if (intTime.type == ASM_TYPE_UNDEF)
	{
		taskTyp->bCycleSet		= FALSE;
        taskTyp->intervalTime	= 0;
	}
    else
	{
        taskTyp->intervalTime	= intTime.any64;

		if (taskTyp->intervalTime == 0x7ffffffeul)
		{
			// Hack: A value of 0x7ffffffeul signals a cycle time of 0!
			taskTyp->intervalTime	= 0;
			taskTyp->bCycleSet		= TRUE;
			taskTyp->bEventSet		= FALSE;
		}
		else if (taskTyp->intervalTime == 0x7ffffffful)
		{
			// Hack: A value of 0x7ffffffful signals an event driven task!
			taskTyp->intervalTime	= 0;
			taskTyp->bCycleSet		= FALSE;
			taskTyp->bEventSet		= TRUE;
		}
		else
		{
			// Hack: Any other value defines a cyclic task
			taskTyp->bCycleSet		= TRUE;
			taskTyp->bEventSet		= FALSE;
		}
	}
    
    if (Prio.type == ASM_TYPE_UNDEF)
        taskTyp->Prio = 5;							// TODO: define a correct default PRIO
    else
        taskTyp->Prio = Prio.any64;

    if (Loaded.type == ASM_TYPE_UNDEF)
        taskTyp->Loaded = 1;
    else
        taskTyp->Loaded = (int) Loaded.any64;

    if (WatchDog.type == ASM_TYPE_UNDEF)
        taskTyp->WatchDog = 0;
    else
        taskTyp->WatchDog = WatchDog.any64;
}

void ASMGlobalContext_C::setActualTask(const ST_EdiPos* pEdp,
                                       ST_IDENT_TYP*    name)
{
    m_curTask = m_hashTasks.get(name->psz);
    if (m_curTask == NULL)
    {
        ST_DiagMsg1(pEdp, E_4CCG_ERR_TASK_UNDEFINED, name->psz);
    }
}

void ASMGlobalContext_C::addTaskPrginstAssignment(
    const ST_EdiPos* pEdp,
    ST_IDENT_TYP     prgInst
)
{
    const ASM_GLOB_COMP* pGComp;
    if (!m_curTask)
    {
        assert(ASM_GetTotalErrors()>0);
        return;
    }
    
    pGComp = lookUpPrgInst(prgInst);
    if(!pGComp)
    {
        ST_DiagMsg1(pEdp, E_4CCG_ERR_UNDEF_VAR, prgInst.psz);
    }
    else if(pGComp->pType->type != ASM_PRGI)
    {
        ST_DiagMsg3(pEdp, E_4CCG_ERR_INVALID_POU_TYPE, _T(".prg"), prgInst.psz, 
        ASM_TypeEnumToString(pGComp->pType->type));
    }
    else
    {
        m_curTask->prgInstList.add(prgInst);
        m_hashPrgInstTasks.insert(prgInst.psz, m_curTask);
    }
}

const ASM_TASK_TYP* ASMGlobalContext_C::getTaskOfPrgInst(const TCHAR* pszPrgInstName)
{
    int                 i;
    int                 j;
    int                 nTask; 
    int                 nPInst; 
    const ASM_TASK_TYP* pTask;

    nTask = m_taskList.getMaxUsed();
    for(i=0; i<nTask; i++)
    {
        pTask =  m_taskList.get(i);
        nPInst = pTask->prgInstList.getMaxUsed();
        for(j=0; j<nPInst; j++)
        {
            if(!_tcscmp(pszPrgInstName, pTask->prgInstList.get(j).psz))
                return pTask;
        }
    }
    return NULL;
}
const ASM_GLOB_COMP* ASMGlobalContext_C::lookUpPrgInst(ST_IDENT_TYP prgInst)
{
    int                  i;
    const ASM_GLOB_COMP* pGComp;

    for(i=0; i<m_prgInstList.getMaxUsed();i++)
    {
        pGComp = m_prgInstList.get(i);
        if(ASM_IDENT_EQ(pGComp->path.ids, prgInst))
            return pGComp;
    }
    return NULL;
}


bool ASMGlobalContext_C::addGlobalDecl(
    const ST_EdiPos*            pEdp, 
    const ASM_DATA_TYP*         pType, 
    ST_IDENT_TYP                id,
    ASM_SEGM_TYP                segm,
    const ASM_DIRECT_TYP&       opt_addr,
    const ASM_INITIALISER_TYP&  opt_init
)
{
    ASM_GLOB_COMP*       pGComp; 

    if(pType->type==ASM_TYPE_UNDEF)
    {
        assert(ASM_GetTotalErrors()>0);
        return false;
    }
    

    pGComp = new ASM_GLOB_COMP;
    ASM_InitGlobComp(pGComp);

    pGComp->path  = registerInstPathPlusMember(NULL, id);
    pGComp->pType = pType;
    pGComp->segm  = segm;
    m_globComp.add(pGComp);
    if(m_hashGlobComp.put(pGComp, pGComp->path.ids.psz, false))
    {
        ST_DiagMsg2(pEdp, E_4CCG_ERR_REDEF_VAR, id.psz, _T("see above!"));
        return false;
    }

    if(pType->type==ASM_PRGI)
    {
        for(int i=0; i<m_prgInstList.getMaxUsed();i++)
        {
            if(ASM_IsInstPathEq(m_prgInstList.get(i)->path, pGComp->path))
            {
                ST_DiagMsg2(pEdp, E_4CCG_ERR_REDEF_VAR, id.psz, _T("see above!"));
                return false;
            }
        }
        // add to program instance list:
        m_prgInstList.add(pGComp);
    }

    

    if(opt_addr.loc!=ASM_SEGM_UNDEF)
    {
        if(ASM_CheckDirectAddUse(pEdp, opt_addr, pType, opt_init.initType != ASM_INIT_UNDEF))
        {
            pGComp->segm = opt_addr.loc;
            pGComp->bSwap = opt_addr.bSwap;
            pGComp->bitOff = opt_addr.bitoff;
            pGComp->bCGAddr = false;

            // // check for arrays on INPUT or OUTPUT
            // if (pGComp->segm == ASM_SEGM_INPUT || pGComp->segm == ASM_SEGM_OUTPUT)
            // {
            //     if (pType->bArray)
            //     {
            //         ST_DiagMsg1(pEdp, E_4CCG_ERR_NOT_SUPPORTED, _T("ARRAY or STRUCT AT %ID<fields> or %QD<fields>"));
            //         return false;
            //     }
            // }
        }
    }
    
    if (opt_init.initType == ASM_INIT_LIT)
    {
        if( ! ASM_AreTypesAssignable(opt_init.lit.type, pType->type))
            ST_DiagMsg2(pEdp, E_4CCG_ERR_INIT_TYPE, ASM_TypeEnumToString(opt_init.lit.type), pType->name.psz);
        else
            pGComp->initval = opt_init;
    }

    else if (opt_init.initType == ASM_INIT_LIST)
    {
        if( ! ASM_AreTypesAssignable(opt_init.pInitList->get(0).pLitList[0].type, pType->type))
            ST_DiagMsg2(pEdp, E_4CCG_ERR_INIT_TYPE, ASM_TypeEnumToString(opt_init.pInitList->get(0).pLitList[0].type), pType->name.psz);
        else
            pGComp->initval = opt_init;
    }

    return true;
}

void ASMGlobalContext_C::appendConfigData(const ST_EdiPos*           pEdp, 
                                          const ASM_INST_PATH_TYP*   pPath, 
                                          const ASM_INITIALISER_TYP& initVal)
{
    ASM_CONFIG_DATA cfg;
    cfg.edp     = *pEdp;
    cfg.path    = *pPath;
    cfg.initVal = initVal;
    
    m_ConfigData.add(cfg);
}

void ASMGlobalContext_C::addCopySection(CASMCopySection* pCpy)
{
    CASMCopySection* pCpyPrev;
    if(pCpy->getTaskName().psz)
        pCpyPrev = getCpyRegForTask(pCpy->getTaskName());
    else
        pCpyPrev = getCpyRegForRC();

    if(pCpyPrev)
        ST_DiagMsg1(NULL, E_ERR_SECTION_DUPLY, _T(".copy"));

    m_copySection.add(pCpy);
}
CASMCopySection* ASMGlobalContext_C::getCpyRegForRC()const
{
    CASMCopySection* pCpy;
    for(int i=0;i<m_copySection.getUsed();i++)
    {
        pCpy = m_copySection.get(i);
        if(pCpy->getTyp()==ASM_CPY_TYP_RC)
            return pCpy;
    }
    return NULL;
}
CASMCopySection* ASMGlobalContext_C::getCpyRegForTask(ST_IDENT_TYP taskName)const
{
    CASMCopySection* pCpy;
    for(int i=0;i<m_copySection.getUsed();i++)
    {
        pCpy = m_copySection.get(i);
        if(ASM_IDENT_EQ(pCpy->getTaskName(), taskName))
            return pCpy;
    }
    return NULL;
}


void ASMGlobalContext_C::addChannelAttribute(
    const ST_EdiPos*       pEdp, 
    const TCHAR*           pszAttr, 
    const ASM_LITERAL_TYP* pVal
)
{
    CChannelInfo* pCh = m_channelList.getLast();

    if(!pCh)
        ASM_INTERNAL_ERROR_STOP(pEdp);

    if(!_tcsicmp(pszAttr, "Ioffs"))
        pCh->m_ulIOffs = ASM_ForceDWORD(pEdp, pVal);
    else if(!_tcsicmp(pszAttr, "Isize"))
        pCh->m_ulISize = ASM_ForceDWORD(pEdp, pVal);
    else if(!_tcsicmp(pszAttr, "Qoffs"))
        pCh->m_ulQOffs = ASM_ForceDWORD(pEdp, pVal);
    else if(!_tcsicmp(pszAttr, "Qsize"))
        pCh->m_ulQSize = ASM_ForceDWORD(pEdp, pVal);
    else if(!_tcsicmp(pszAttr, "Moffs"))
        pCh->m_ulMOffs = ASM_ForceDWORD(pEdp, pVal);
    else if(!_tcsicmp(pszAttr, "Msize"))
        pCh->m_ulMSize = ASM_ForceDWORD(pEdp, pVal);
    else if(!_tcsicmp(pszAttr, "CHname"))
        ASM_ForceCString(pEdp, pVal, &pCh->m_szName);
    else
        ST_DiagMsg1(pEdp, E_4CCG_WARN_UNKNOWN_ATTRIB, pszAttr);
}



void ASMGlobalContext_C::parseAsmFile()
{
    bool            bOk;
    FileContext_C*  pfc;

    assert(m_asmFileName[0]);


    //input file:
    TO_MAPWRITE1(_T("Asm file : %s\n\n"), (LPCSTR) m_asmFileName);
    pfc = new FileContext_C(m_asmFileName);
    m_fileContext.add(pfc);
    bOk = ParseFile(pfc);
    assert(bOk || ASM_GetTotalErrors()>0);

}



long ASMGlobalContext_C::GetNumberOfGlobalBoolean(ASM_SEGM_TYP segm)
{
    long NumOfGlobBool = 0;

    int iMax = m_globComp.getMaxUsed();
    for(int i=0; i<iMax; i++)
    {
        ASM_GLOB_COMP* pGComp = m_globComp.get(i);
        if (pGComp->segm == segm) //TODO error should not count %M !!! only automatic vars !
        {
            if (pGComp->pType->type == ASM_BOOL && !pGComp->pType->bArray)
            {
                NumOfGlobBool++;
            }
        }
    }    

    return NumOfGlobBool;
}

void ASMGlobalContext_C::calcGlobObjectNumber(const ASM_GLOB_COMP* pGComp)
{
    const ASMPROC_TYP*     pInst;
    ASM_INST_PATH_TYP      path;

    if(pGComp->pType->bArray)
    {
        if(pGComp->pType->type == ASM_FBI || pGComp->pType->type == ASM_STRI)
        {
            // register the instance array itself
            pInst = pGComp->pType->pFBIDef;
            path = registerInstPathPlusMember(NULL, pGComp->path.ids);
            registerObject(path, pGComp->pType, NULL, pGComp->segm, pGComp->bitOff);
            //pInst->calcObjectNumOfNested(path, pGComp->pType, pGComp->segm);

            // register data type
            const ASM_DATA_TYP* pInstType = registerType(
                pGComp->pType->type, 
                pGComp->pType->pFBIDef->getTypeIdent(), 
                pGComp->pType->pFBIDef);

            // register every array element tself
            for(unsigned Elem=0; Elem < pGComp->pType->nElems; Elem++)
            {
                path = registerInstPathPlusArrayMember(NULL, pGComp->path.ids, Elem);
                registerObject(path, pInstType, NULL, pGComp->segm, ASM_INVALID_BITOFF);
                pInst->calcObjectNumOfNested(path, pInstType, pGComp->segm);
            }
        }
        else
        {
            registerObject(pGComp->path, 
                           pGComp->pType,
                           &pGComp->initval,
                           pGComp->segm,
                           pGComp->bitOff);
        }
    }
    else if (pGComp->pType->type == ASM_FBI ||  pGComp->pType->type == ASM_STRI)
    {
        pInst = pGComp->pType->pFBIDef;
        registerObject(pGComp->path, pGComp->pType, NULL, pGComp->segm, pGComp->bitOff);
        pInst->calcObjectNumOfNested(pGComp->path, pGComp->pType, pGComp->segm);
    }
}

void ASMGlobalContext_C::calcGlobalOffsets()
{
    const TCHAR*  pszSegName;
    ASM_BITOFF    ulBooleanBitOff_GM  = 0;
    ASM_BITOFF    ulBooleanBitOff_GMR = 0;
    ASM_BITSIZE   declBitSize;
    ASM_BITOFF*   pulOff;
    ASM_BITSIZE   declAlign;
    long          instAlign = getTrgAttr()->trg_instAlignBytes*8;

	

	if(m_trgAttr.trg_version < 21000)
	{
		// bits are packed at the beginning
		ulBooleanBitOff_GM = (__int64) m_trgAttr.trg_GmIecOffs * 8;
								// offset for IEC Variables relative
								// to the global Segment pointer

		// start address for non boolean members
		m_ulBitOff_GM  = ((GetNumberOfGlobalBoolean(ASM_SEGM_GLOBAL) + 15) / 16) * 16;
		m_ulBitOff_GM += (__int64) m_trgAttr.trg_GmIecOffs * 8;  // offset for IEC Variables relative
	}
	else
	{
		// start address for non boolean members
		m_ulBitOff_GM  = ((GetNumberOfGlobalBoolean(ASM_SEGM_GLOBAL) + 15) / 16) * 16;
	}
   
    // start address for non boolean retain members
    m_ulBitOff_GMR = ((GetNumberOfGlobalBoolean(ASM_SEGM_RETAIN) + 15) / 16) * 16;

	
    //starting points are the global simple:
    TO_MAPWRITE0(_T("\n.begin global offsets:\n"));
    int iMax = m_globComp.getMaxUsed();
    for(int i=0; i<iMax; i++)
    {
        ASM_GLOB_COMP* pGComp = m_globComp.get(i);

        if(!pGComp->bCGAddr)
            continue; // global var at USER addr
        if(pGComp->segm != ASM_SEGM_GLOBAL && pGComp->segm != ASM_SEGM_RETAIN)
            continue; // no prog or fb instances

        if(pGComp->pType->bArray || pGComp->pType->pFBIDef)
        {
            continue;// objects have no address
        }
        else if(pGComp->pType->type==ASM_STRING)
        {
            declBitSize = ASM_SimpleTypeBitSize(pGComp->pType);
            declAlign   = 8;
        }
        else if(pGComp->pType->type==ASM_BOOL)
        {
            declBitSize = 1;
            declAlign   = 1;
        }
        else
        {
            declBitSize = ASM_SimpleTypeBitSize(pGComp->pType);
            declAlign   = min(declBitSize, instAlign);
        }

        if(pGComp->segm==ASM_SEGM_GLOBAL)
        {
            pszSegName = _T("GLOBAL");
            pulOff = declBitSize==1 ? &ulBooleanBitOff_GM : &m_ulBitOff_GM;
        }
        else 
        {
            pszSegName = _T("RETAIN");
            pulOff = declBitSize==1 ? &ulBooleanBitOff_GMR : &m_ulBitOff_GMR;
        }

        assert(pGComp->bitOff==ASM_INVALID_BITOFF && pGComp->bitSize==0);

        pGComp->bitOff = *pulOff = ASM_AlignBitOffset(*pulOff, declAlign);
        pGComp->bitSize = declBitSize;
        *pulOff += declBitSize;

        if(declBitSize == 1)
        {
            TO_MAPWRITE6(_T("Seg: %s  %s %s Offs=%I64d[0x%08I64X].%I64d:\n"), 
                pszSegName,
                pGComp->pType->name.psz, 
                pGComp->path.ids.psz, 
                pGComp->bitOff / 8,
                pGComp->bitOff / 8,
                pGComp->bitOff % 8);
        }
        else
        {
            TO_MAPWRITE5(_T("Seg: %s  %s %s Offs=%I64d[0x%08I64X]:\n"), 
                pszSegName,
                pGComp->pType->name.psz, 
                pGComp->path.ids.psz, 
                pGComp->bitOff / 8,
                pGComp->bitOff / 8);
        }
    }
    TO_MAPWRITE0(_T(".end global offsets:\n"));
}


void ASMGlobalContext_C::calcObjectNumbers()
{
    const ASM_GLOB_COMP*   pGComp;
    const ASMPROC_TYP*     pPrg;

    if(m_trgAttr.trg_version >= 21000) {
	    m_objectNr += 2;
    }

    //starting points are the global retain instances,
	//reatin objechts must be first objs to make onlchange easier (this 
	//way retain objects will not change their obj numbers)
	//!!CAUTION!! the __taskinfo objects must have the lowest obj numbers!
	//the Control maps __taskinfo objects to task with this code:
	//pVM->Local.pState = (STaskState OS_DPTR *) (pVMM->Shared.pData[i + MAX_SEGMENTS + 2].pAdr);
	//where i is the i-th task (in order of declarations).
    TO_MAPWRITE0(_T("\n.begin task info objects:\n"));
    int iMax = m_globComp.getMaxUsed();
    for(int i=0; i<iMax; i++)
    {
        pGComp = m_globComp.get(i);
        if(_tcscmp(_T("__taskinfo"), pGComp->pType->name.psz)!=0)
            continue;

        calcGlobObjectNumber(pGComp);
    }    
    TO_MAPWRITE0(_T(".end task info objects:\n"));
 



    TO_MAPWRITE0(_T("\n.begin global retain objects:\n"));
    for(i=0; i<iMax; i++)
    {
        pGComp = m_globComp.get(i);
		
        if(pGComp->segm!=ASM_SEGM_RETAIN || pGComp->pType->type==ASM_PRGI)
            continue;
        
		//assert: __taskinfo objects are never retain.
		assert(_tcscmp(_T("__taskinfo"), pGComp->pType->name.psz) != 0);

        calcGlobObjectNumber(pGComp);
    }    
    TO_MAPWRITE0(_T(".end global retain objects:\n"));

    //second are the global instances:
    TO_MAPWRITE0(_T("\n.begin global objects:\n"));
    for(i=0; i<iMax; i++)
    {
        pGComp = m_globComp.get(i);
        if(pGComp->segm==ASM_SEGM_RETAIN || pGComp->pType->type==ASM_PRGI)
            continue;
        if(!_tcscmp(_T("__taskinfo"), pGComp->pType->name.psz))
            continue; //already done in first step

        calcGlobObjectNumber(pGComp);
    }    
    TO_MAPWRITE0(_T(".end global objects:\n"));


    //next are the program and fb instances:
    TO_MAPWRITE0(_T("\n.begin instance objects:\n"));
    for(i=0; i<m_prgInstList.getMaxUsed(); i++)
    {
        pGComp = m_prgInstList.get(i);
        assert(pGComp->pType->type==ASM_PRGI);

        pPrg = pGComp->pType->pFBIDef;
        assert(pPrg && pPrg->getPouTyp()==ASM_PRG);

        registerObject(pGComp->path, pGComp->pType, &pGComp->initval, pGComp->segm, ASM_INVALID_BITOFF);
        pPrg->calcObjectNumOfNested(pGComp->path, pGComp->pType, pGComp->segm);
    }
    TO_MAPWRITE0(_T(".end instance objects:\n"));
}

void ASMGlobalContext_C::calcOffsetLiterals()
{
    const ASMPROC_TYP*     pPrg;

    TO_MAPWRITE0(_T("\n.begin offset literals:\n"));

    // first start with functions
    for (int i=0; i<m_funList.getMaxUsed(); i++)
    {
        pPrg = *(m_funList.getAddrOf(i));
        if (    pPrg->getPouTyp() == ASM_FUN
            &&  pPrg->getOpCode() == -1)
        {
            pPrg->calcOffsetLiterals();
        }
    }

    //next are the program and fb instances:
    for (i=0; i<m_prgFBList.getMaxUsed(); i++)
    {
        pPrg = *(m_prgFBList.getAddrOf(i));
        if (    pPrg->getPouTyp() == ASM_PRG
            ||  (   pPrg->getPouTyp() == ASM_FB
                &&  pPrg->getOpCode() == -1))
        {
            pPrg->calcOffsetLiterals();
        }
    }
    TO_MAPWRITE0(_T(".end offset literals:\n"));
}

void ASMGlobalContext_C::calcConfigData()
{
    FC_CString          sPath(MAX_PATH);
    FC_CString          sMember(MAX_PATH);
    ASM_OBJECT_TYP*     pObj;
    const ASM_DECL_TYP* pDecl;

    int iMax = m_ConfigData.getUsed();
    for(int i=0; i<iMax; i++)
    {
        const ASM_CONFIG_DATA* pCD = m_ConfigData.getAddrOf(i);

        if(pCD->initVal.initType == ASM_INIT_LIT)
        {
            sPath.load(pCD->path.ids.psz).cutAtLast(_T("."));
            sMember.load(FC_StringGetFileExt(pCD->path.ids.psz));
            // look for object
            pObj = GetGC()->lookUpObjectUC(sPath);
            if(!pObj)
            {
                ST_DiagMsg1(&pCD->edp, E_4CCG_ERR_UNDEF_OBJ, sPath);
                continue;
            }
            // look for member
            pDecl = pObj->pType->pFBIDef ? pObj->pType->pFBIDef->lookUpLocalVar(sMember) : NULL;
            if(!pDecl)
            {
                ST_DiagMsg2(&pCD->edp, E_4CCG_ERR_UNDEF_MEMBER, sMember, sPath);
                continue;
            }
            if(pDecl->pType->bArray)
            {
                ST_DiagMsg2(&pCD->edp, E_4CCG_ERR_INIT_TYPE, 
                    ASM_TypeEnumToString(pCD->initVal.lit.type), _T("ARRAY"));
                continue;
            }
            if(pDecl->pType->type != pCD->initVal.lit.type)
            {
                ST_DiagMsg2(&pCD->edp, E_4CCG_ERR_INIT_TYPE, 
                    ASM_TypeEnumToString(pCD->initVal.lit.type),
                    ASM_TypeEnumToString(pDecl->pType->type)
                );
                continue;
            }
            ASM_MEMBER_INIT MInit;
            MInit.initVal = pCD->initVal;
            MInit.pDecl   = pDecl;
            pObj->listOfMemberInitVal.add(MInit);
        }
        else if(pCD->initVal.initType == ASM_INIT_LIST)
        {
            // if the init value is an array init we append it 
            // to the list of array init values
            // look for array object
            pObj = GetGC()->lookUpObjectUC(pCD->path.ids.psz);
            if(!pObj)
            {
                ST_DiagMsg1(&pCD->edp, E_4CCG_ERR_UNDEF_OBJ, pCD->path.ids.psz);
                continue;
            }
            else if(!pObj->pType->bArray || pObj->pType->pFBIDef)
            {
                ST_DiagMsg1(&pCD->edp, E_4CCG_ERR_WRONG_TYPE, _T("ARRAY OF ANY_SIMPLE"));
                continue;
            }
            pObj->ListOfListOfArrayInits.add(pCD->initVal.pInitList);
        }
        else
        {
            ASM_INTERNAL_ERROR_STOP(&pCD->edp);
        }
    }
}

void ASMGlobalContext_C::calcCopySections()
{
    int              i;
    CASMCopySection* pCpySec;
    
    assert(m_usedLocalRetainSize==-1 && m_usedGlobalRetainSize==-1 
           && m_lTotCopyRegionCount==0);

    m_ulBitOff_GMR = ASM_AlignBitOffset(m_ulBitOff_GMR, 64);//HACK when to assing the global size counters ??
    m_usedGlobalRetainSize = ASM_BITOFF_TO_BYTESIZE(m_ulBitOff_GMR);
    m_usedLocalRetainSize = 0;
 
    for(i=0;i<m_copySection.getUsed(); i++)
    {
        pCpySec = m_copySection.get(i);
        m_usedLocalRetainSize += pCpySec->calculate(this, m_lTotCopyRegionCount);
        m_lTotCopyRegionCount += pCpySec->getCount();
    }

    if(m_bCreateMapFile)
    {
        int idx = 0;
        TO_MAPWRITE0(_T("\n.begin copy sections\n"));
        for(i=0;i<m_copySection.getUsed(); i++)
           m_copySection.get(i)->dumpMapFile(NULL);

        TO_MAPWRITE0(_T("\nNr. : ObjNr Off  Size\n"));
        for(i=0;i<m_copySection.getUsed(); i++)
            m_copySection.get(i)->dumpMapFile(&idx);

        TO_MAPWRITE0(_T(".end copy sections:\n"));
    }
}

void ASMGlobalContext_C::calcInstructions()
{
    //starting points are functions:
    for (int i=0; i<m_funList.getMaxUsed(); i++)
    {
        ASMPROC_TYP* pPrg = *(m_funList.getAddrOf(i));
        if (    pPrg->getPouTyp() == ASM_FUN
            &&  pPrg->getOpCode() == -1)
        {
            pPrg->calcInstructions();
        }
    }

    // program and fb instances:
    for (i=0; i<m_prgFBList.getMaxUsed(); i++)
    {
        ASMPROC_TYP* pPrg = *(m_prgFBList.getAddrOf(i));
        if (    pPrg->getPouTyp() == ASM_PRG
            ||  (   pPrg->getPouTyp() == ASM_FB
                &&  pPrg->getOpCode() == -1))
        {
            pPrg->calcInstructions();
        }
    }
}

void ASMGlobalContext_C::checkSizes()
{
    TCHAR         buffer1[30];
    TCHAR         buffer2[30];
    unsigned long ulAllRet;


    ulAllRet = m_usedGlobalRetainSize+m_usedLocalRetainSize;

    sprintf(buffer1, "ip:%6u", m_ulTotalCodeBytes);
    GetGC()->writeMemInfoStr(_T("segsize"), _T("code"),  buffer1);
    GetGC()->writeMemInfoDW( _T("segsize"), _T("global"),  m_ulUsedGlobalSize);
    GetGC()->writeMemInfoDW( _T("segsize"), _T("globalsimple"),  m_ulUsedGlobalSimpleSize);
    GetGC()->writeMemInfoDW( _T("segsize"), _T("retain"),  ulAllRet);
    GetGC()->writeMemInfoDW( _T("segsize"), _T("local"),   m_ulLocalDataSize);
    GetGC()->writeMemInfoDW( _T("handels"), _T("objects"), m_objectNr);
    GetGC()->writeMemInfoDW( _T("handels"), _T("classes"), getNumberOfClasses());

    // Code segment
    if(m_ulTotalCodeBytes > m_trgAttr.trg_CodeSegSize)
    {
        ST_DiagMsg3(ASM_ModuleEdiPos(), E_4CCG_ERR_SEGM_EXCEEDS_MAX_SIZE,
            _T("code"),
            _ltot(m_ulTotalCodeBytes, buffer1, 10),
            _ultot(m_trgAttr.trg_CodeSegSize, buffer2, 10));
    }

    //max  objects
    if (m_objectNr > (long) m_trgAttr.trg_maxObjects)
    {
        ST_DiagMsg2(ASM_ModuleEdiPos(), E_4CCG_ERR_TOO_MANY_OBJECTS,
            _itot(m_trgAttr.trg_maxObjects - MAX_SEGMENTS, buffer1, 10),
            _itot(m_objectNr - MAX_SEGMENTS, buffer2, 10));
    }

    // LOCAL
    if((unsigned long)m_ulLocalDataSize > m_trgAttr.trg_LocDataSegSize)
    {
        ST_DiagMsg3(ASM_ModuleEdiPos(), E_4CCG_ERR_SEGM_EXCEEDS_MAX_SIZE,
            _T("local data"),  
            _ltot(m_ulLocalDataSize, buffer1, 10),
            _ultot(m_trgAttr.trg_LocDataSegSize, buffer2, 10));
    }


    // GLOBAL simples must not exceede 64kb
    if(m_ulUsedGlobalSimpleSize > 0xffff) //d-218
    {
        ST_DiagMsg3(ASM_ModuleEdiPos(), E_4CCG_ERR_SEGM_EXCEEDS_MAX_SIZE,
            _T("global.automatic (simple type)"),  
            _ltot(m_ulUsedGlobalSimpleSize, buffer1, 10),
            _ultot(0xffff, buffer2, 10));
    }
    //simple + complex limit:
    if(m_ulUsedGlobalSize > m_trgAttr.trg_GmIecSize)
    {
        ST_DiagMsg3(ASM_ModuleEdiPos(), E_4CCG_ERR_SEGM_EXCEEDS_MAX_SIZE,
            _T("global.automatic"),  
            _ltot(m_ulUsedGlobalSize, buffer1, 10),
            _ultot(m_trgAttr.trg_GmIecSize, buffer2, 10));
    }
    // GLOBAL RETAIN
    if(ulAllRet > m_trgAttr.trg_GMRSegSize)
    {
        FC_CString ins1(MAX_PATH);
        ins1<<_T("VAR_GLOBAL: ")<<m_usedGlobalRetainSize<<_T("and LOCAL: ")<<m_usedLocalRetainSize;
        ST_DiagMsg3(ASM_ModuleEdiPos(), E_4CCG_ERR_SEGM_EXCEEDS_MAX_SIZE,
            _T("retain"), ins1.psz(),  _ultot(m_trgAttr.trg_GMRSegSize, buffer2, 10));
    }

    if(m_trgAttr.trg_dumpinfo)
    {
        _tprintf("4asm: used: .code=%u of %uk .objects=%u of %u .global=%u of %ubytes .retain=%u of %ubytes\n",
            (m_ulTotalCodeBytes+1023)/1024, m_trgAttr.trg_CodeSegSize/1024,
            m_objectList.getMaxUsed(),  m_trgAttr.trg_maxObjects,
            m_ulUsedGlobalSize, m_trgAttr.trg_GmIecSize,
            (unsigned long)(m_usedGlobalRetainSize+m_usedLocalRetainSize), m_trgAttr.trg_GMRSegSize
        );
    }
}

static void ASMDumpMapLine(
    FILE*               pMFile,
    const TCHAR*        pszTask,
    const TCHAR*        pszInstPath, //NULL if glob simple var
    const TCHAR*        pszItem,
    const ASM_DATA_TYP* pType,
    long                index,
    ASM_BITOFF          bitOff
)
{
    TCHAR          buffer[256];
    ASM_TYPE_ENUM  type;
    unsigned long  ulBytes;
    long           nBits;
    const TCHAR*   psz;


    type    = pType->type; 
    ulBytes = (unsigned long)(bitOff/8);
    nBits   =          (long)(bitOff%8);

    assert(type<=ASM_LAST_BASIC_TYPE);

    if(pszTask)
    {
        fputs(pszTask, pMFile);
        fputc(_T('.'), pMFile);
    }
    if(pszInstPath)
    {
        fputs(pszInstPath, pMFile);
        fputc(_T('.'), pMFile);
    }
    fputs(pszItem, pMFile);
    fputc(_T(';'), pMFile);

    if(type == ASM_STRING)
    {
        assert(pType->lStrlen>=0);
        wsprintf(buffer, _T("STRING[%d]"), pType->lStrlen);
        fputs(buffer, pMFile);
    }
    else
    {
        for(psz = ASM_TypeEnumToString(type); *psz; psz++)
            fputc(toupper(*psz), pMFile);        
    }

    fputc(_T(';'), pMFile);

    if(type==ASM_BOOL)
    {
        wsprintf(buffer, _T("%d.%u.%d\n"), index, ulBytes, nBits);
    }
    else
    {
        assert(nBits==0);
        wsprintf(buffer, _T("%d.%u\n"), index, ulBytes);
    }

    fputs(buffer, pMFile);
}


void ASMGlobalContext_C::dumpMapFile()
{
    FILE*                       pMFile;
    ST_String_C                 FileName;
    int                         i;
    int                         j;
    const ASM_OBJECT_TYP*       pObj;
    const ASM_GLOB_COMP*        pGComp;
    int                         nUsed;
    int                         nDecls;
    const ASM_DECL_TYP*         pDecl;
    const ASM_DATA_TYP*         pType;
    ASM_TYPE_ENUM               type;
    const TCHAR*                pszTyp;
    const TCHAR*                pszId;
    const GArray<ASM_DECL_TYP*>*pDecls;
    TCHAR                       itoabuf[64];
    const ASM_TASK_TYP*         pCurrTask;
    const TCHAR*                pszTask;
    const TCHAR*                pszInstPath;
    const TCHAR*                pszCurrPrgInst; //only for assertion

    
    FileName << m_GenPath <<_T("\\") << FILE_MAPFILE;

    pMFile = fopen(FileName, "wt");
    if(!pMFile)
    {
        ST_DiagMsg1(ASM_ModuleEdiPos(), E_4CCG_ERR_FILE_OPEN, FileName);
        return;
    }


    //the global simple first:
    nUsed = m_globComp.getMaxUsed();
    for(i=0; i<nUsed; i++)
    {
        pGComp = m_globComp.get(i);
        pType  = pGComp->pType;
        type   = pType->type;
        pszId  = pGComp->path.ids.psz;

        if(type>ASM_LAST_BASIC_TYPE || pType->bArray)
            continue;

        assert(pGComp->path.depth==1);

        //skip all special globs e.f. __state:
        if(pszId[0]==_T('_') && pszId[1]==_T('_'))
            continue;

        ASMDumpMapLine(pMFile, NULL, NULL, pGComp->path.ids.psz,
            pType, ASM_GetObjIdxFromSeg(pGComp->segm), pGComp->bitOff);
    }


    //all other instances:
    pszTask = NULL;
    pszCurrPrgInst = NULL;
    nUsed = m_objectList.getMaxUsed();
    for(i=0; i<nUsed; i++)
    {
        pObj        = m_objectList.get(i);
        pszInstPath = pObj->path.ids.psz; 
        pType       = pObj->pType;
        pszTyp      = pType->name.psz;
        type        = pType->type;

        //skip special instances e.g. __task  type:
        if(type>ASM_LAST_BASIC_TYPE && pszTyp[0]==_T('_') && pszTyp[1]==_T('_'))
            continue;


        //assert: I assume that the order of instances in the array is sorted in natural order:
        if(type==ASM_PRGI)
        {
            assert(pObj->path.depth==1);
            pCurrTask = getTaskOfPrgInst(pszInstPath);
            assert(pCurrTask);
            pszTask = pCurrTask->name.psz;
            pszCurrPrgInst = pszInstPath;
        }
        assert(!pszCurrPrgInst || !_tcsncmp(pszInstPath, pszCurrPrgInst, _tcslen(pszCurrPrgInst)));



        if(pType->bArray)
        {
            if(type<=ASM_LAST_BASIC_TYPE)
            {
                assert(((long)pType->nElems)>=0);//2GB array elems is enough.
                
                ASM_BITSIZE  elemBitSize;
                ASM_BITOFF32 firstElemBitOff = ASM_GetFirstArrayElemBitOff(pType, &elemBitSize);

                for(j=0; j<(long)pType->nElems; j++)
                {
                    itoa(j, itoabuf, 10);
                    ASMDumpMapLine(pMFile, pszTask, pszInstPath, itoabuf,
                        pType, pObj->Index, firstElemBitOff + j*elemBitSize);
                }//else: array of struct shall not appear in map file...
            }
        }
        else 
        {
            assert(type>ASM_LAST_BASIC_TYPE);

            pDecls = pType->pFBIDef->getDecls();
            nDecls = pDecls->getMaxUsed();
            
            for(j=0; j<nDecls; j++)
            {
                pDecl = pDecls->get(j);
                pType = pDecl->pType;
                type  = pType->type;
                
                if(type<=ASM_LAST_BASIC_TYPE && !pType->bArray)
                {
                    ASMDumpMapLine(pMFile, pszTask, pszInstPath, pDecl->id.psz,
                        pType, pObj->Index, pDecl->declBitOff);
                }
            }
        }
    }

    fclose(pMFile);
}


void ASMGlobalContext_C::dumpDataAndCodeFiles()
{
    // open download domain file
    FC_CString DLdomainFileName(MAX_PATH);
    DLdomainFileName<<m_GenPath<<FILE_NAME_DOMAIN_LIST;
    m_DldFile = fopen(DLdomainFileName, "w");

    // common header file name
    fprintf(m_DldFile, _T("%s;%s\n"),
        DL_FILE_SECTION_CONFIG,
        FILE_NAME_COMMON_HDR);

    // domain file list name
    fprintf(m_DldFile, _T("%s;%s\n"),
        DL_FILE_SECTION_DEBUG,
        FILE_NAME_DOMAIN_LIST);

    // global data      ("__globals.def")
    dumpGlobalDefinitions();
    dumpObjectHash();

    // class data       ("*.def")
    dumpClassDefinitions();

    // array data       ("dint[11].def")
    dumpArrayDefinitions();

    // post compile relevant data
    dumpClassIds();

    // map              ("*.map")
    dumpCodeIntoMapFile();

    // code             ("*.bin")
    dumpCodeFiles();

    // für unsere abdeckungstests
    dumpAllUsedOpcodes();

    // debug info files ("*.dbi")
    dumpDebugInfo();

    // initial values   ("*.init")
    dumpInitialValues();

    // task data        ("__task.list")
    dumpTaskData();

    // common download header
    dumpCmnHeader();

    // call stack       ("__callstack.bin")
    dumpCallStack();

    // close __domain.list
    fclose(m_DldFile);

    // map file:
    dumpMapFile();

}

void ASMGlobalContext_C::dumpGlobalDefinitions()
{
    FILE*					pFile;
	const ASM_GLOB_COMP*	pGComp;
	int						i;
	int						nMax;
    FC_CString				fileName(MAX_PATH);


    fileName.load(m_GenPath)<<FILE_NAME_GLOBDATA_DEF FILE_EXT_DATA_DEF;
    
    pFile = fopen(fileName, "w");
    fprintf(m_DldFile, _T("%s;%s\n"),   // common header file
        DL_FILE_SECTION_DEBUG,
        FILE_NAME_GLOBDATA_DEF FILE_EXT_DATA_DEF);

	nMax = m_prgInstList.getMaxUsed();
    for (i=0; i<nMax; i++)
    {
        pGComp = m_prgInstList.get(i);
		dumpGlobalDef2Line(pFile, pGComp);
    }

	nMax = m_globComp.getMaxUsed();
    for (i=0; i<nMax; i++)
    {
        pGComp = m_globComp.get(i);
        if (pGComp->path.depth == 1)
        {
            if (   pGComp->segm == ASM_SEGM_INPUT  || pGComp->segm == ASM_SEGM_OUTPUT 
                || pGComp->segm == ASM_SEGM_GLOBAL || pGComp->segm == ASM_SEGM_RETAIN)
            {
                dumpGlobalDef2Line(pFile, pGComp);
            }
        }
    }

    fclose(pFile);
}


//meaning of the 8 fields of a decl in a def file:
//name;typechar;segm;typeName;byteoff;bitoff;bitsize;strlen
// 1      2       3     4        5       6      7      8
//#define CSV_IDX_NAME         1
//#define CSV_IDX_TYPECHAR     2           /* S,N,G,S*,N*  (* == pointer)                                         */
//#define CSV_IDX_SEGMENT      3           /* Segment if appropriate, else nix                              */
//#define CSV_IDX_DATA_TYPE    4           /* BOOL, LREAL, MyType, ...                                            */
//#define CSV_IDX_BYTE_OFFSET  5           /* Offset in byte of member in parent structure/segment */
//#define CSV_IDX_BIT_OFFSET   6           /* Offset in bit (0-7) for BOOL variables, else nix            */    
//#define CSV_IDX_BIT_SIZE     7           /* Size of variable, index or pointer in bit               */
//#define CSV_IDX_STRLEN       8           /* Maximum number of characters if STRING[], else nix   */
void ASMGlobalContext_C::dumpGlobalDef2Line(
	FILE*				 pFile,
	const ASM_GLOB_COMP* pGComp
)
{
	const ASM_DATA_TYP* pType;
	ASM_TYPE_ENUM		type;
	BOOL				bIsObj;
    FC_CString			defDecl(256);

	
	pType  = pGComp->pType;
	type   = pType->type;
	bIsObj = ASM_IsObject(pType);
	
	//name
	defDecl.load(pGComp->path.ids.psz)<<';';

	//typechar
	defDecl<<(bIsObj ? 'S' : 'G')<<';';

	//segm
	if(!bIsObj)
		defDecl<<(pGComp->bCGAddr?ASM_GetObjIdxFromSeg(pGComp->segm):pGComp->segm);
	//else: nix
	defDecl<<';';
	
	//typeName
    ASM_AppendDefFileTypeName(defDecl, pType)<<';';
	
	//byteoff
	if(!bIsObj)
		defDecl<<(long)(pGComp->bitOff / 8);
	//else: nix
	defDecl<<';';
	
	//bitoff
	if(type==ASM_BOOL && !bIsObj)
		defDecl<<(long)(pGComp->bitOff % 8);
	//else: nix
	defDecl<<';';
	
	//bitsize
	if(!bIsObj)
		defDecl<<pGComp->bitSize;
	//else: nix
	defDecl<<';';

	//strlen
	if(type==ASM_STRING && !bIsObj)
		defDecl<<pType->lStrlen;
	//else: nix
	defDecl<<'\n';
	
	fwrite(defDecl.psz(), defDecl.len(), 1, pFile);
}

void ASMGlobalContext_C::dumpObjectHash()
{
    FC_CString typeName(MAX_PATH);
    FC_CString FileName(MAX_PATH);
    FC_CString ObjName(MAX_PATH);
    FileName.load(m_GenPath);
    FileName << FILE_NAME_OBJECT_HASH;
    
    FILE* pFile;
    pFile = fopen(FileName, "w");

    fprintf(m_DldFile, _T("%s;%s\n"),   // common header file
        DL_FILE_SECTION_DEBUG,
        FILE_NAME_OBJECT_HASH);

    int iMax = m_objectList.getMaxUsed();
    for (int i=0; i<iMax; i++)
    {
        ASM_OBJECT_TYP* pObj = m_objectList.get(i);

        long clsID;
        if (pObj->pType->type == ASM_FBI || pObj->pType->type == ASM_PRGI)
        {
            ASMPROC_TYP* pClass = m_hashPrgFB.get(pObj->pType->pFBIDef->getName());
            clsID = pClass->getClassId();
        }
        else
        {
            clsID = CLSID_NONE;
        }

        ObjName.clear().appendUpTo(pObj->path.ids.psz, _T("."));

        long lTaskId = 0;
        const ASM_TASK_TYP* pTask = findTaskOfPrgInst(ObjName);
        if (pTask != NULL)  // global objects have no task assignment
            lTaskId = pTask->taskId;
		typeName.clear();
		ASM_AppendDefFileTypeName(typeName, pObj->pType);

        fprintf(pFile, _T("%s;%ld;%lu;%ld;%ld;%ld;%ld;%ld;%ld;%s\n"),
            pObj->path.ids.psz,
            lTaskId,
            pObj->Index,
            0,
            pObj->byteSize,
            -1,     // TODO RETAIN OBJID
            -1,     // TODO RETAIN OFFSET
            -1,     // TODO RETAIN OFFSET
            clsID,
            typeName.psz()
		);
    }

    fclose(pFile);
}

void ASMGlobalContext_C::dumpClassDefinitions()
{
    FILE*		pFile;
	int			i;
    FC_CString	fileName(MAX_PATH);

    //functions don't have def2 files, maybe in future...:
    //for (int i=0; i<m_funList.getMaxUsed(); i++)

    // then PRGs and FBs
    for (i=0; i<m_prgFBList.getMaxUsed(); i++)
    {
        const ASMPROC_TYP* pPrg = m_prgFBList.get(i);
        
        if (m_hashTypes.get(pPrg->getName()) == NULL)
            continue;   // type not used

		fileName.load(m_GenPath)<<pPrg->getName()<<FILE_EXT_DATA_DEF;
        pFile = fopen(fileName, "w");
        pPrg->dumpPrgFBClassDefinition(pFile);
        fclose(pFile);

        fprintf(m_DldFile, _T("%s;%s%s\n"),   // common header file
            DL_FILE_SECTION_DEBUG,
            pPrg->getName(), FILE_EXT_DATA_DEF);
    }
}


void ASMGlobalContext_C::dumpArrayDefinitions()
{
    FILE* pFile;
    LPCTSTR psz = NULL;

    int iMax = m_registeredTypes.getMaxUsed();
    for (int i=0; i<iMax; i++)
    {
        const ASM_DATA_TYP* pType = m_registeredTypes.get(i);
        if (pType->bArray)
        {
            FC_CString FileName(80);
            FileName.load(m_GenPath);
			ASM_AppendDefFileTypeName(FileName, pType);
            FileName << FILE_EXT_DATA_DEF;
            
            pFile = fopen(FileName, "w");
            dumpArrayDefinition(pFile, pType);
            fclose(pFile);
            
            fprintf(m_DldFile, _T("%s;%s\n"),   // common header file
                DL_FILE_SECTION_DEBUG,
                FC_StringGetFilePart(FileName)
			);
        }
    }
}


//meaning of the 8 fields of a decl in a def file:
//name;typechar;segm;typeName;byteoff;bitoff;bitsize;strlen
// 1      2       3     4        5       6      7      8
void ASMGlobalContext_C::dumpArrayDefinition(FILE*               pFile,
                                             const ASM_DATA_TYP* pType)
{
	unsigned			ui;
    FC_CString			defDecl(256);
    ASM_BITSIZE			elemBitSize;
	ASM_TYPE_ENUM		type = pType->type;
	BOOL				bIsObj = type==ASM_STRI || type==ASM_FBI;
    const TCHAR*		pszTypeName = bIsObj ? pType->pFBIDef->getName() : ASM_TypeEnumToString(type);
    ASM_BITOFF32		firstElemBitOff = ASM_GetFirstArrayElemBitOff(pType, &elemBitSize);
    
	assert(pType->bArray);

    for (ui=0; ui<pType->nElems; ui++)
	{
		//name
		defDecl.clear()<<ui<<';';

		//typechar
		defDecl<<(bIsObj ? 'S' : 'N')<<';';

		//segm
		defDecl<</*nix*/';';
		
		//typeName
		defDecl<<pszTypeName<<';';
		
		//byteoff
        defDecl<<(firstElemBitOff + ui * (unsigned long)elemBitSize) / 8<<';';

		//bitoff
		if(type==ASM_BOOL)
			defDecl<< ui % 8;
		//else: nix
		defDecl<<';';
		
		//bitsize
		defDecl<<elemBitSize<<';';

		//strlen
		if(type==ASM_STRING)
			defDecl<<pType->lStrlen;
		//else: nix
		defDecl<<'\n';

		fwrite(defDecl.psz(), defDecl.len(), 1, pFile);
	}
}

void ASMGlobalContext_C::dumpClassIds()
{
    FILE* pFile;

    FC_CString FileName(80);
    FileName.load(m_GenPath);
    FileName << FILE_NAME_CLASSID_TXT;
    pFile = fopen(FileName, "w");
    
    //starting points are functions:
    for (int i=0; i<m_funList.getMaxUsed(); i++)
    {
        const ASMPROC_TYP* pPrg = m_funList.get(i);
        if (    pPrg->getPouTyp() == ASM_FUN
            &&  pPrg->getOpCode() == -1)
        {
            fprintf (pFile, "%s;%ld\n", pPrg->getName(), pPrg->getClassId());
        }
    }

    //program and fb instances:
    for (i=0; i<m_prgFBList.getMaxUsed(); i++)
    {
        const ASMPROC_TYP* pPrg = m_prgFBList.get(i);
        if (    pPrg->getPouTyp() == ASM_PRG
            ||  (   pPrg->getPouTyp() == ASM_FB
                &&  pPrg->getOpCode() == -1))
        {
            fprintf (pFile, "%s;%ld\n", pPrg->getName(), pPrg->getClassId());
        }
    }

    fclose(pFile);
}

void ASMGlobalContext_C::dumpCodeIntoMapFile()
{
    if ( ! m_bCreateMapFile)
        return; // nothing to do

    //starting points are functions:
    for (int i=0; i<m_funList.getMaxUsed(); i++)
    {
        const ASMPROC_TYP* pPrg = m_funList.get(i);
        if (    pPrg->getPouTyp() == ASM_FUN
            &&  pPrg->getOpCode() == -1)
        {
            TO_MAPWRITE2(_T("\n.code %s (classid=%ld) begin\n"), pPrg->getName(), pPrg->getClassId());

            pPrg->dumpCodeIntoMapFile();
            
            TO_MAPWRITE1(_T(".code %s end\n"), pPrg->getName());
        }
    }

    //program and fb instances:
    for (i=0; i<m_prgFBList.getMaxUsed(); i++)
    {
        const ASMPROC_TYP* pPrg = m_prgFBList.get(i);
        if (    pPrg->getPouTyp() == ASM_PRG
            ||  (   pPrg->getPouTyp() == ASM_FB
                &&  pPrg->getOpCode() == -1))
        {
            TO_MAPWRITE2(_T("\n\n\n.code %s (classid=%ld) begin\n"), pPrg->getName(), pPrg->getClassId());

            pPrg->dumpCodeIntoMapFile();
            
            TO_MAPWRITE1(_T(".code %s end\n"), pPrg->getName());
        }
    }

}

void ASMGlobalContext_C::dumpCodeFiles()
{
    FC_CString          fileName(MAX_PATH);
    const ASMPROC_TYP*  pProc;
    FILE*               pBinFile;

    m_ulTotalCodeBytes  = 0;

    fileName<<m_GenPath<<'\\'<<FILE_NAME_ALL_CLASSES FILE_EXT_BINARY;
    pBinFile = fopen(fileName, "wb");
    if(!pBinFile)
    {
        ST_DiagMsg1(ASM_ModuleEdiPos(), E_4CCG_ERR_FILE_OPEN, fileName);
        return;
    }

    //starting points are functions:
    for (int i=0; i<m_funList.getMaxUsed(); i++)
    {
        pProc = m_funList.get(i);
        if(pProc->isBuildIn())
            continue;
        pProc->dumpCode(pBinFile);
        m_ulTotalCodeBytes += pProc->getCodeSize();
    }

    //program and fb instances:
    for (i=0; i<m_prgFBList.getMaxUsed(); i++)
    {
        pProc = m_prgFBList.get(i);
        if(pProc->isBuildIn() || pProc->getPouTyp()==ASM_STRUCT)
            continue;
        pProc->dumpCode(pBinFile);
        m_ulTotalCodeBytes += pProc->getCodeSize();
    }

    fprintf(m_DldFile, _T("%s;%s%s\n"),
        DL_FILE_SECTION_CODE,
        FILE_NAME_ALL_CLASSES,FILE_EXT_BINARY);
    
    fclose(pBinFile);
}

void ASMGlobalContext_C::dumpAllUsedOpcodes()
{
    FILE* pFile;

    ST_String_C FileName;
    ST_String_C Path(m_GenPath);
    FileName << FILE_NAME_USED_OPCODES;
    Path << (LPCTSTR) FileName;
    
    pFile = fopen(Path, "w");

    //starting points are functions:
    for (int i=0; i<m_funList.getMaxUsed(); i++)
    {
        const ASMPROC_TYP* pPrg = m_funList.get(i);
        if (    pPrg->getPouTyp() == ASM_FUN
            &&  pPrg->getOpCode() == -1)
        {
            pPrg->dumpUsedOpcodes(m_opcodes);
        }
    }

    //program and fb instances:
    for (i=0; i<m_prgFBList.getMaxUsed(); i++)
    {
        const ASMPROC_TYP* pPrg = m_prgFBList.get(i);
        if (    pPrg->getPouTyp() == ASM_PRG
            ||  (   pPrg->getPouTyp() == ASM_FB
                &&  pPrg->getOpCode() == -1))
        {
            pPrg->dumpUsedOpcodes(m_opcodes);
        }
    }

    for (i=0; i<1024*64; i++)
    {
        if (m_opcodes[i] != NULL)
        {
            fprintf(pFile, "%s,%04x\n", m_opcodes[i], i);
        }
    }
    fclose(pFile);
}

void ASMGlobalContext_C::dumpDebugInfo()const
{
    FILE* pFile;

    //starting points are the functions:
    for (int i=0; i<m_funList.getMaxUsed(); i++)
    {
        const ASMPROC_TYP* pPrg = m_funList.get(i);
        if (    pPrg->getPouTyp() == ASM_FUN
            &&  pPrg->getOpCode() == -1)
        {
            ST_String_C FileName(m_GenPath);
            FileName << pPrg->getName();
            FileName << FILE_EXT_DEBUGINFO;
            
            pFile = fopen(FileName, "w");
            pPrg->dumpDebugInfo(pFile);
            fclose(pFile);

            fprintf(m_DldFile, _T("%s;%s%s\n"),
                DL_FILE_SECTION_DEBUG,
                pPrg->getName(),FILE_EXT_DEBUGINFO);
        }
    }

    //program and fb instances:
    for (i=0; i<m_prgFBList.getMaxUsed(); i++)
    {
        const ASMPROC_TYP* pPrg = m_prgFBList.get(i);
        if (    pPrg->getPouTyp() == ASM_PRG
            ||  (   pPrg->getPouTyp() == ASM_FB
                &&  pPrg->getOpCode() == -1))
        {
            ST_String_C FileName(m_GenPath);
            FileName << pPrg->getName();
            FileName << FILE_EXT_DEBUGINFO;
            
            pFile = fopen(FileName, "w");
            pPrg->dumpDebugInfo(pFile);
            fclose(pFile);

            fprintf(m_DldFile, _T("%s;%s%s\n"),
                DL_FILE_SECTION_DEBUG,
                pPrg->getName(),FILE_EXT_DEBUGINFO);
        }
    }
}

long ASMGlobalContext_C::getNumberOfClasses()const
{
    long NumberOfClasses = 0;
    for (int i=0; i<m_funList.getMaxUsed(); i++)    // FUNs
    {
        const ASMPROC_TYP* pPrg = m_funList.get(i);
        if (   pPrg->getPouTyp() == ASM_FUN
            && pPrg->getOpCode() == -1)
        {
            NumberOfClasses++;
        }
    }
    for (i=0; i<m_prgFBList.getMaxUsed(); i++)      // PRGs / FBs
    {
        const ASMPROC_TYP* pPrg = m_prgFBList.get(i);
        if (    pPrg->getPouTyp() == ASM_PRG
            ||  (   pPrg->getPouTyp() == ASM_FB
                &&  pPrg->getOpCode() == -1))
        {
            NumberOfClasses++;
        }
    }

    return NumberOfClasses;
}

unsigned long ASMGlobalContext_C::dumpInitialArrayValues(
    FILE*                 pFile,
    const ASM_OBJECT_TYP* pObj
)
{
    ASM_LITERAL_TYP* pInit = NULL;
    ASM_INIT_REPEAT_VAL_TYP* pRptVal = NULL;
    FC_CGArray<ASM_INIT_REPEAT_VAL_TYP>* pListOfArrayInits = NULL;

    DumpObjblockHeader(pFile, pObj);
    unsigned long ulSize = pObj->byteSize;
    BYTE* pMem = new BYTE[ulSize];
    memset(pMem, 0, ulSize);

    //get element size and alignment info:
    ASM_BITSIZE  elemBitSize;
    ASM_BITOFF32 firstElemBitOff = ASM_GetFirstArrayElemBitOff(pObj->pType, &elemBitSize);

    int iListCnt = pObj->ListOfListOfArrayInits.getUsed();
    for (int iLC=0; iLC<iListCnt || iLC == 0; iLC++)    // iLC == 0: loop at least one time
    {
        if (iListCnt != 0)
            pListOfArrayInits = pObj->ListOfListOfArrayInits.get(iLC);

        if (pListOfArrayInits)
            pRptVal = pListOfArrayInits->getAddrOf(0); // get first elem
        
        // fill alignemtn gap between VM_ARRAY_LIMIT_SIZE and first element with 0:
        ASM_ValToMemBlock(pMem, 0, 0, firstElemBitOff, ulSize);
        // dump array size
        ASM_ValToMemBlock(pMem, pObj->pType->nElems, 0, VM_ARRAY_LIMIT_SIZE, ulSize);
        
        int iAI = 0;
        int iRP = 0;
        int iLI = 0;
        for (int i=0; i<(int)pObj->pType->nElems; i++)
        {
            pInit = NULL;
            if (pListOfArrayInits)
            {
                if (iAI < pListOfArrayInits->getUsed())
                {
                    if (iRP == pRptVal->nRepeat)
                    {
                        iRP = 0;
                        iLI = 0;
                        pRptVal = pListOfArrayInits->getAddrOf(iAI);   // get next elem
                    }
                    
                    if (pRptVal->nListSize == 0 && pRptVal->pLitList == NULL)
                    {
                        // there is syntax like "ident : type := [...,n(),...]"
                        // ==> leave the current init values
                        pInit = NULL;
                    }
                    else
                    {
                        pInit = &pRptVal->pLitList[iLI];
                    }
                    iLI++;
                    
                    if (iLI == pRptVal->nListSize || pRptVal->nListSize == 0) 
                    {
                        // if (pRptVal->nListSize == 0)
                        // ==> leave the current inti value (see above)
                        iLI = 0;
                        iRP++;
                    }
                    
                    if (iRP == pRptVal->nRepeat)    // last elem from repeat-val
                        iAI++;
                }
            }
            
            // if ther is no init given we dump nothing
            // caution: array of complex: we dump the object idx 
            if (pObj->pType->type != ASM_STRI)
            {
                // according to ASM syntax: 'path := [..., n(val,(),(),()), ...]'
                // "literals" that are '()' are indicated by ASM_VOID
                if (   pInit == NULL
                    || pInit->type == ASM_VOID)
                {
                    // ==> leave the current init values (see above)
                    // for strings we look whether there is already an init
                    // if not we dump an empty string (init of max len is important)
                    if (pObj->pType->type == ASM_STRING)
                    {
                        long Offs = firstElemBitOff + elemBitSize * i;
                        Offs += ASM_BasicTypeBitSize(ASM_BYTE);
                        // max len is at the second byte position  
                        ASM_ValToMemBlock(
                            pMem, 
                            pObj->pType->lStrlen, 
                            Offs,
                            ASM_BasicTypeBitSize(ASM_BYTE), 
                            ulSize);
                    }
                    
                    continue;
                }
            }
            
            if (pObj->pType->type == ASM_STRING)
            {
                // STRING
                ASM_LITERAL_TYP lit0;
                lit0.type = ASM_STRING;
                lit0.pszIECStr = "''";
                
                long Offs = firstElemBitOff + elemBitSize * i;
                ASM_BinStringToMemBlock(
                    pMem, 
                    pObj->pType, 
                    Offs, 
                    pInit ? pInit : &lit0,
                    ulSize);
            }
            else if (ASM_IsSimpleBasicType(pObj->pType->type))
            {
                // SIMPLE
                long Offs = firstElemBitOff + elemBitSize * i;
                ASM_ValToMemBlock(
                    pMem, 
                    pInit ? pInit->any64 : 0, 
                    Offs,
                    ASM_BitSizeOfArrayElem(pObj->pType), 
                    ulSize);
            }
            else
            {
                // OBJECTs
                long Offs = firstElemBitOff + elemBitSize * i;
                const ASM_OBJECT_TYP* pSubObj;
                long ObjIdx = ASM_GetObjectID(pObj->path.ids, NULL, &pSubObj, i);
                ASM_ValToMemBlock(
                    pMem, 
                    ObjIdx, 
                    Offs, 
                    VM_OBJ_INDEX_SIZE, 
                    ulSize);
            }
        }
    }
    fwrite(pMem, sizeof(char), ulSize, pFile);

    ((ASM_OBJECT_TYP*) pObj)->bInit = true;
    delete[] pMem;

    return ulSize;
}

void ASMGlobalContext_C::dumpInitialValues()
{
    FILE*                   pFile;
    FC_CString              fileName(MAX_PATH);
    unsigned long           ulSize;
    BYTE*                   pInit;
    __int64                 IecOffset;
    const ASM_GLOB_COMP*    pGComp;
	

    // globals
    fileName << m_GenPath<< FILE_NAME_ALL_OBJECTS FILE_EXT_INITDATA;
    pFile = fopen(fileName, "wb");


	// IEC VARIABLES

	if(m_trgAttr.trg_version >= 21000)
	{
		IecOffset = 0;
		ulSize = (unsigned long)m_ulBitOff_GM;
	}
	else
	{
		IecOffset = (__int64)m_trgAttr.trg_GmIecOffs * 8;
		ulSize = (unsigned long)(m_ulBitOff_GM - IecOffset);
	}

	ulSize = (unsigned long)ASM_AlignBitOffset(ulSize, 64) / 8; // align to 8 byte
	pInit = new BYTE[ulSize==0 ? 1 : ulSize];
	memset(pInit, 0, ulSize);
    
    m_ulUsedGlobalSimpleSize = ulSize;
	m_ulUsedGlobalSize = ulSize;

	// simple global variables (NON RETAIN)
	int iMax = m_globComp.getMaxUsed();
	for (int i=0; i<iMax && ulSize; i++)
	{
		pGComp = m_globComp.get(i);
		if (ASM_IsObject(pGComp->pType))
			continue;// global objects have no index in global memory, index is already in code

		if(!pGComp->bCGAddr)
		{
			// simples at USER will be initialised later add to m_directSimpleVars:
			assert(pGComp->segm!=ASM_SEGM_RETAIN);
            if(pGComp->pType->type==ASM_STRING)
            {
        		m_directSimpleVars.add(pGComp);
            }
			else if(pGComp->initval.lit.type != ASM_TYPE_UNDEF)
            {
				//if(pGComp->initval.lit.any64 != 0i64) <-OSAI: remove this if to get an init also for 0-values 
				
				m_directSimpleVars.add(pGComp);
            }
			continue;
		}

		if(pGComp->segm==ASM_SEGM_RETAIN)
			continue;// will be dumped afterwards

		dumpGCompInitValue(pGComp, IecOffset, pInit, ulSize);
	}
	// now goes into the file
	DumpGlobalSegHdr2(pFile, ASM_SEGM_GLOBAL, ulSize);
	fwrite(pInit, sizeof(char), ulSize, pFile);
	delete[] pInit;




	// simple global variables (RETAIN)
	// --------------------------------
	ulSize = (unsigned long) m_ulBitOff_GMR;
	ulSize = (unsigned long) ASM_AlignBitOffset(ulSize, 64) / 8; // align to 8 byte
	pInit = new BYTE[ulSize==0 ? 1 : ulSize];
	memset(pInit, 0, ulSize);

	//assert: m_usedGlobalRetainSize must have been calculated already in this->calcCopySections
	assert(m_usedGlobalRetainSize >=0 && m_usedGlobalRetainSize == (ASM_BYTESIZE)ulSize);
	m_usedGlobalRetainSize = ulSize;

	iMax = m_globComp.getMaxUsed();
	for(i=0; i<iMax && ulSize; i++)
	{
		pGComp = m_globComp.get(i);
		if(ASM_IsObject(pGComp->pType) || pGComp->segm!=ASM_SEGM_RETAIN)
			continue;

		assert(pGComp->bCGAddr);//asset: retain are never %-Addresses
		dumpGCompInitValue(pGComp, 0, pInit, ulSize);
	}
	// now goes into the file
	DumpGlobalSegHdr2(pFile, ASM_SEGM_RETAIN, ulSize);
	fwrite(pInit, sizeof(char), ulSize, pFile);
	delete[] pInit;


	
    // instance data
	long ulRetObjAlign = m_trgAttr.trg_instAlignBytes;
    iMax = m_objectList.getMaxUsed();
    unsigned long ulObjSize;
    for (i=0; i<iMax; i++)
    {
        ASM_OBJECT_TYP* pObj = m_objectList.get(i);
        if(pObj->bInit)
            continue;   // already initialised

        if(pObj->pType->bArray)
        {
            ulObjSize = dumpInitialArrayValues(pFile, pObj);
        }
        else
        {
            //program, struct and fb instances:
            ulObjSize = pObj->pType->pFBIDef->dumpInitialValues(pFile, pObj);
        }

        if(pObj->bitOffs == ASM_INVALID_BITOFF)
        {
            switch(pObj->segm)
            {
            case ASM_SEGM_GLOBAL: m_ulUsedGlobalSize += ulObjSize; break;//TODO align to 16 bit ??
            case ASM_SEGM_RETAIN: 
				//Consider alignment gaps as done in the VM 
				//since the VM does not know the alignmet of structs it assumes
				//worstcase alignment, this is trg_instAlignBytes:
				m_usedGlobalRetainSize = ((m_usedGlobalRetainSize+ulRetObjAlign-1)/ulRetObjAlign)*ulRetObjAlign;
				m_usedGlobalRetainSize += ulObjSize; 
				m_usedGlobalRetainSize = ((m_usedGlobalRetainSize+ulRetObjAlign-1)/ulRetObjAlign)*ulRetObjAlign; //VM does it like this, we must do too
			break;
            case ASM_SEGM_INST: break;
            default:
                ASM_INTERNAL_ERROR_STOP(NULL);
            }
        }
        //else we count not objects which have a direct addr (struct AT %)
    }



    // 
    // simple vars with direct address
    // -------------------------------
    //
    iMax = m_directSimpleVars.getUsed();
    unsigned char* pMem = new unsigned char[8];
    for (i=0; i<iMax; i++)
    {
        const ASM_GLOB_COMP* pGComp = m_directSimpleVars.get(i);
        WORD wSize;
        VM_STRING vmStr;

        if(pGComp->pType->type==ASM_STRING)
        {
            ASM_BinStringToMemBlock((BYTE*)&vmStr,
                                    pGComp->pType,
                                    0,
                                    &pGComp->initval.lit,
                                    sizeof(vmStr));
            wSize = (2*sizeof(BYTE) + vmStr.CurLen)*8;
        }
        else
        {
            wSize = (WORD) ASM_BasicTypeBitSize(pGComp->pType->type);
        }

        XDownDirect sDownDirect;
        sDownDirect.bySegment = pGComp->segm;
        sDownDirect.byBit     = wSize == 1 ? 1 << (BYTE) (pGComp->bitOff & 0x07) : 0;
        sDownDirect.uSize     = TRANSLATE_WORD(wSize);
        sDownDirect.ulOffset  = TRANSLATE_DWORD((DWORD) (pGComp->bitOff / 8));

        fwrite(&sDownDirect, sizeof(char), sizeof(XDownDirect), pFile);

        if(pGComp->pType->type==ASM_STRING)
        {
            fwrite(&vmStr, sizeof(char), 2*sizeof(BYTE) + vmStr.CurLen, pFile);
        }
        else
        {
            if (wSize == 1)
            {
                // set pMem to zero
                *pMem = 0;
            }
            ASM_ValToMemBlock(pMem, pGComp->initval.lit.any64, 0, wSize, 8);
            fwrite(pMem, sizeof(char), wSize == 1 ? wSize : wSize / 8, pFile);
        }
    }
    delete pMem;

    fprintf(m_DldFile, _T("%s;%s%s\n"),
        DL_FILE_SECTION_INIT,
        FILE_NAME_ALL_OBJECTS, FILE_EXT_INITDATA);
    fclose(pFile);
}


void ASMGlobalContext_C::dumpGCompInitValue(const ASM_GLOB_COMP* pGComp,
                                            __int64              ChnOffset,
                                            BYTE*                pInit,
                                            unsigned long        ulSize)const
{
    if (pGComp->pType->type == ASM_STRING && !pGComp->pType->bArray)
    {
        // STRING
        ASM_InitBinStringToMemBlock(
            pInit, 
            pGComp->pType,
            pGComp->bitOff - ChnOffset,
            &pGComp->initval,
            ulSize);
    }
    else if (ASM_IsSimpleType(pGComp->pType))
    {
        // SIMPLE
        ASM_InitValToMemBlock(
            pInit, 
            &pGComp->initval,
            pGComp->bitOff - ChnOffset, 
            ASM_SimpleTypeBitSize(pGComp->pType), 
            ulSize);
    }
    else if (ASM_IsObject(pGComp->pType))
    {
        // OBJECTs
        long ObjIdx = ASM_GetObjectID(pGComp->path.ids, NULL, NULL);
        ASM_ValToMemBlock(
            pInit, 
            ObjIdx,
            pGComp->bitOff - ChnOffset, 
            VM_OBJ_INDEX_SIZE, 
            ulSize);
    }
}


void ASMGlobalContext_C::DumpGlobalSegHdr2(
    FILE*        pFile,
    ASM_SEGM_TYP segm,
    long         Size
)
{
    XDownHeader cbHeader;

	int idx = ASM_GetObjIdxFromSeg(segm);
	if(idx < MAX_SEGMENTS)
		idx = -1;

	cbHeader.uIndex   = TRANSLATE_WORD((IEC_UINT)idx);
    cbHeader.uSegment = TRANSLATE_WORD((WORD) segm);
    cbHeader.ulSize   = TRANSLATE_DWORD((DWORD) Size);
    cbHeader.ulOffset = 0;
    cbHeader.byFixed  = 0;
    cbHeader.byType   = VMM_OT_UNKNOWN;    

    fwrite(&cbHeader, sizeof(char), sizeof(XDownHeader), pFile);
}
void ASMGlobalContext_C::DumpObjblockHeader(
    FILE*                 pFile,
    const ASM_OBJECT_TYP* pObj
)
{
    XDownHeader cbHeader;

    if(pObj->segm==ASM_SEGM_INST)
    {
        m_ulLocalDataSize += pObj->byteSize;
    }

    if(ASM_GetVMStoreByteSize(pObj->pType) != pObj->byteSize)
        ASM_INTERNAL_ERROR_STOP(NULL);

    assert(pObj->segm >= 0 && pObj->segm <= 4);
    cbHeader.uIndex   = TRANSLATE_WORD((IEC_UINT) pObj->Index);
    cbHeader.uSegment = TRANSLATE_WORD((WORD)pObj->segm);
    cbHeader.ulSize   = TRANSLATE_DWORD(pObj->byteSize);
    cbHeader.ulOffset = pObj->bitOffs == ASM_INVALID_BITOFF ? 
        TRANSLATE_DWORD(0):
        TRANSLATE_DWORD((unsigned long)(pObj->bitOffs >> 3));
    cbHeader.byFixed  = pObj->bitOffs == ASM_INVALID_BITOFF ? 0 : 1;
	if(pObj->pType->bArray)
		cbHeader.byType = VMM_OT_ARRAY;
	else if(!pObj->pType->pFBIDef)
		cbHeader.byType = VMM_OT_SIMPLE;
	else 
	{
		switch(pObj->pType->pFBIDef->getPouTyp())
		{
		case ASM_FUN: 
			cbHeader.byType = VMM_OT_FUN; break;
		case ASM_FB:
			cbHeader.byType = VMM_OT_FB; break;
		case ASM_PRG:
			cbHeader.byType = VMM_OT_PROG; break;
		case ASM_STRUCT:
			cbHeader.byType = VMM_OT_STRUCT; break;
		default:
			cbHeader.byType = VMM_OT_UNKNOWN; break;
		}
	}

    // printf ("idx: %d  seg:%d  size:%ld  offs; %lu\n", cbHeader.uIndex,cbHeader.uSegment,cbHeader.ulSize,cbHeader.ulOffs);

    fwrite(&cbHeader, sizeof(char), sizeof(XDownHeader), pFile);
}



void ASMGlobalContext_C::dumpTaskData()
{
    FILE* pFile;
    ST_String_C FileName;

    // globals
    FileName << m_GenPath;
    FileName << FILE_NAME_TASK_LIST;
    pFile = fopen(FileName, "w");

    fprintf(m_DldFile, _T("%s;%s\n"),   // common header file
        DL_FILE_SECTION_DEBUG,
        FILE_NAME_TASK_LIST);

    // dump task IDs

    int iMax = m_taskList.getMaxUsed();
    for (int i=0; i<iMax; i++)
    {
        const ASM_TASK_TYP* pTask = m_taskList.get(i);
        
		if (m_trgAttr.trg_version >= 20700)
		{
			if(m_trgAttr.trg_version>=20900 && !_tcsicmp(m_trgAttr.trg_type, _T("4cosai")))
			{
				//FC_CString str;
				if (pTask->bCycleSet == TRUE)
				{
					fprintf (pFile, _T("%s;%ld;%s"), pTask->name.psz, pTask->taskId, pTask->Loaded ? _T("L") : _T("U"));
					fprintf (pFile, _T(";CYCLIC;%ld"), pTask->intervalTime);
					fprintf (pFile, _T(";%ld\n"), pTask->Prio);
				}
				else if (pTask->bEventSet == TRUE)
				{
					IEC_UDINT Events = TRANSLATE_DWORD((IEC_UDINT)pTask->Events);
					int iEventIndex = 0;
					fprintf (pFile, _T("%s;%ld;%s;EVENT;%ld"), pTask->name.psz, pTask->taskId, pTask->Loaded ? _T("L") : _T("U"), 
						pTask->Prio);
					while (Events != 0)
					{
						if ((Events & 0x0001) == 1 )
						{
							fprintf (pFile, _T(";%d"), iEventIndex+1);
						}
						Events >>= 1;
						iEventIndex++;
					}
					Events = TRANSLATE_DWORD((IEC_UDINT)(pTask->Events>>32));
					iEventIndex = 32;
					while (Events != 0)
					{
						if ((Events & 0x0001) == 1 )
						{
							fprintf (pFile, _T(";%d"), iEventIndex+1);
						}
						Events >>= 1;
						iEventIndex++;
					}
					fprintf (pFile, _T("\n"));
				}
				else
				{
					fprintf (pFile, _T("%s;%ld;%s\n"), pTask->name.psz, pTask->taskId, pTask->Loaded ? _T("L") : _T("U"));
				}
			}
			else
			{

				fprintf (pFile, _T("%s;%ld;%s\n"), pTask->name.psz, pTask->taskId, pTask->Loaded ? _T("L") : _T("U"));
			}
		}
		else
		{
			fprintf (pFile, _T("%s;%ld\n"), pTask->name.psz, pTask->taskId);
		}
    }

    fclose(pFile);
}



void ASMGlobalContext_C::WriteUsedMemRegions(FILE*               pCommonHdr,
                                             ASM_SEGM_TYP        segm,
                                             const ASM_TASK_TYP* pTask)
{
    const FC_CGArray<ASM_MEM_REGION>*   pMemRgns;
    const ASM_MEM_REGION* pAsmMemRgn;
    XMemRegion SMemRgn;
    int i;

    // WRITE
    switch(segm)
    {
    case ASM_SEGM_INPUT:
        pMemRgns = &pTask->MemRegions.ISegMemRgnWR;
        SMemRgn.bySegment = ASM_SEGM_INPUT;

        if (pMemRgns->getUsed() != 0)
        {
            assert( ! "INPUT segment cannot has WRITE regions");
            return;
        }
        break;
    case ASM_SEGM_OUTPUT:
        pMemRgns = &pTask->MemRegions.OSegMemRgnWR;
        SMemRgn.bySegment = ASM_SEGM_OUTPUT;
        break;
    case ASM_SEGM_GLOBAL:
        ASM_INTERNAL_ERROR_STOP(NULL);//old code should not reached any more
        pMemRgns = &pTask->MemRegions.GSegMemRgnWR;
        SMemRgn.bySegment = ASM_SEGM_GLOBAL;
        break;
    case ASM_SEGM_RETAIN:
        ASM_INTERNAL_ERROR_STOP(NULL);//old code should not reached any more
        pMemRgns = &pTask->MemRegions.GRSegMemRgnWR;
        SMemRgn.bySegment = ASM_SEGM_RETAIN;
        break;
    default:
        assert( ! "invalid segment used");
        return;
    }
    
    SMemRgn.bRead = 0;
    for (i=0; i<pMemRgns->getUsed(); i++)
    {
        pAsmMemRgn = pMemRgns->getAddrOf(i);
        SMemRgn.ulOffset = TRANSLATE_DWORD(pAsmMemRgn->ulOffset);
        SMemRgn.uSize    = TRANSLATE_WORD(pAsmMemRgn->uSize);
        fwrite(&SMemRgn, sizeof(char), sizeof(XMemRegion), pCommonHdr);
    }

    // READ
    switch(segm)
    {
    case ASM_SEGM_INPUT:
        pMemRgns = &pTask->MemRegions.ISegMemRgnRD;
        SMemRgn.bySegment = ASM_SEGM_INPUT;
        break;
    case ASM_SEGM_OUTPUT:
        pMemRgns = &pTask->MemRegions.OSegMemRgnRD;
        SMemRgn.bySegment = ASM_SEGM_OUTPUT;
        break;
    case ASM_SEGM_GLOBAL:
        ASM_INTERNAL_ERROR_STOP(NULL);//old code should not reached any more
        pMemRgns = &pTask->MemRegions.GSegMemRgnRD;
        SMemRgn.bySegment = ASM_SEGM_GLOBAL;
        break;
    case ASM_SEGM_RETAIN:
        ASM_INTERNAL_ERROR_STOP(NULL);//old code should not reached any more
        pMemRgns = &pTask->MemRegions.GRSegMemRgnRD;
        SMemRgn.bySegment = ASM_SEGM_RETAIN;
        break;
    default:
        assert( ! "invalid segment used");
        return;
    }

    SMemRgn.bRead = 1;
    for (i=0; i<pMemRgns->getUsed(); i++)
    {
        pAsmMemRgn = pMemRgns->getAddrOf(i);
        SMemRgn.ulOffset = TRANSLATE_DWORD(pAsmMemRgn->ulOffset);
        SMemRgn.uSize    = TRANSLATE_WORD(pAsmMemRgn->uSize);
        fwrite(&SMemRgn, sizeof(char), sizeof(XMemRegion), pCommonHdr);
    }
  
}

bool ASMGlobalContext_C::WriteGlobSegmentDesc(FILE* pCommonHdr)
{
    XObject SegDesc;

    DWORD minVal = 0x04;
    DWORD dwSize = 0;

    // GLOBAL INPUT
    SegDesc.pAdr   = (BYTE*) TRANSLATE_DWORD(m_trgAttr.trg_ISegStart);
    SegDesc.ulSize = TRANSLATE_DWORD(m_trgAttr.trg_ISegSize);
    fwrite(&SegDesc, sizeof(char), sizeof(SegDesc), pCommonHdr);

    // GLOBAL OUTPUT
    SegDesc.pAdr   = (BYTE*) TRANSLATE_DWORD(m_trgAttr.trg_OSegStart);
    SegDesc.ulSize = TRANSLATE_DWORD(m_trgAttr.trg_OSegSize);
    fwrite(&SegDesc, sizeof(char), sizeof(SegDesc), pCommonHdr);

    // GLOBAL SEGMENT
    SegDesc.pAdr   = (BYTE*) TRANSLATE_DWORD(m_trgAttr.trg_GMSegStart);
    SegDesc.ulSize = TRANSLATE_DWORD(m_trgAttr.trg_GMSegSize);
    fwrite(&SegDesc, sizeof(char), sizeof(SegDesc), pCommonHdr);

    // GLOBAL RETAIN SEGMENT
    SegDesc.pAdr   = (BYTE*) TRANSLATE_DWORD(m_trgAttr.trg_GMRSegStart);
    SegDesc.ulSize = TRANSLATE_DWORD(m_trgAttr.trg_GMRSegSize);
    fwrite(&SegDesc, sizeof(char), sizeof(SegDesc), pCommonHdr);

    // INSTANCE SEGMENT
    SegDesc.pAdr   = (BYTE*) TRANSLATE_DWORD(m_trgAttr.trg_LocDataSegStart);
    SegDesc.ulSize = TRANSLATE_DWORD(m_trgAttr.trg_LocDataSegSize);
    fwrite(&SegDesc, sizeof(char), sizeof(SegDesc), pCommonHdr);
    
    // CODE SEGMENT
    SegDesc.pAdr   = (BYTE*) TRANSLATE_DWORD(m_trgAttr.trg_CodeSegStart);
    SegDesc.ulSize = TRANSLATE_DWORD(m_trgAttr.trg_CodeSegSize);
    fwrite(&SegDesc, sizeof(char), sizeof(SegDesc), pCommonHdr);

    return true;
}



void ASMGlobalContext_C::dumpXProject_213(FILE* pFile)
{
    ST_IDENT_TYP         id = registerId(_T("__state"));
	const ASM_GLOB_COMP* pGComp = lookUpGlobalVar(id);
    unsigned long        ulStateVarByteOff;    

	if(pGComp == NULL)
		ASM_INTERNAL_ERROR_STOP(NULL);

	ulStateVarByteOff =  (unsigned long)(pGComp->bitOff / 8);

    
    XProject_213 sProject;

    sProject.ulBinDLVersion = TRANSLATE_DWORD(m_trgAttr.trg_version);
    sProject.ulStateVarOffs = TRANSLATE_DWORD(ulStateVarByteOff);
    sProject.uTasks         = TRANSLATE_WORD((IEC_UINT) m_taskList.getMaxUsed());
    sProject.uCodeObjects   = TRANSLATE_WORD((IEC_UINT) getNumberOfClasses());
    if(m_trgAttr.trg_version >= 21000) 	{
        // add global and global retain objects
        sProject.uInstObjects   = TRANSLATE_WORD((WORD) m_objectList.getMaxUsed() + 2);
    } else {
        sProject.uInstObjects   = TRANSLATE_WORD((WORD) m_objectList.getMaxUsed());
    }
    sProject.uDirectVars    = TRANSLATE_WORD((WORD)m_directSimpleVars.getUsed());
    sProject.ulIECOffset    = TRANSLATE_DWORD(m_trgAttr.trg_GmIecOffs);
    sProject.ulIECSize      = TRANSLATE_DWORD(m_trgAttr.trg_GmIecSize);
    sProject.uDebugCode     = TRANSLATE_WORD((IEC_UINT) m_trgAttr.trg_dgbSetting);
	sProject.uCpyRegions    = TRANSLATE_WORD((IEC_UINT) m_lTotCopyRegionCount);
    
	CASMCopySection* pCpy = getCpyRegForRC();
    assert(!pCpy || sProject.uCpyRegions>0);
    if(pCpy)
    {
	    sProject.uCpyRegConst = TRANSLATE_WORD((IEC_UINT)pCpy->getCount());
        sProject.uCpyRegCOff  = TRANSLATE_WORD((IEC_UINT)pCpy->getFinalOffset());
    }
    else
    {
        sProject.uCpyRegConst = 0;
        sProject.uCpyRegCOff  = 0;
    }

	sProject.uProjInfo		= TRANSLATE_WORD((IEC_UINT)1);
    sProject.uIOLayer       = TRANSLATE_WORD((WORD) m_channelList.getUsed());

    fwrite(&sProject, sizeof(char), sizeof(sProject), pFile);
}

void ASMGlobalContext_C::dumpXIOLayer(FILE* pFile)
{
    int      i;
    XIOLayer xio;  


    for(i=0; i<m_channelList.getUsed(); i++)
    {
        const CChannelInfo* pCh = m_channelList.get(i);
        xio.ulIOffs = TRANSLATE_DWORD(pCh->m_ulIOffs);
        xio.ulISize = TRANSLATE_DWORD(pCh->m_ulISize);

        xio.ulQOffs = TRANSLATE_DWORD(pCh->m_ulQOffs);
        xio.ulQSize = TRANSLATE_DWORD(pCh->m_ulQSize);

        xio.ulMOffs = TRANSLATE_DWORD(pCh->m_ulMOffs);
        xio.ulMSize = TRANSLATE_DWORD(pCh->m_ulMSize);

        xio.usChannel = pCh->m_usChannel;
        strncpy((char*)xio.szName, pCh->m_szName, FC_ARRAY_LEN(xio.szName));
        xio.szName[FC_ARRAY_LEN(xio.szName)-1] = 0;

        fwrite(&xio, sizeof(char), sizeof(xio), pFile);
    }
}

void ASMGlobalContext_C::dumpXProject_207(FILE* pFile)
{
    ST_IDENT_TYP         id = registerId(_T("__state"));
	const ASM_GLOB_COMP* pGComp = lookUpGlobalVar(id);
    unsigned long        ulStateVarByteOff;    

	if(pGComp == NULL)
		ASM_INTERNAL_ERROR_STOP(NULL);

	ulStateVarByteOff =  (unsigned long)(pGComp->bitOff / 8);

    
    XProject_207 sProject;

    sProject.ulBinDLVersion = TRANSLATE_DWORD(m_trgAttr.trg_version);
    sProject.ulStateVarOffs = TRANSLATE_DWORD(ulStateVarByteOff);
    sProject.uTasks         = TRANSLATE_WORD((IEC_UINT) m_taskList.getMaxUsed());
    sProject.uCodeObjects   = TRANSLATE_WORD((IEC_UINT) getNumberOfClasses());
    if(m_trgAttr.trg_version >= 21000) 	{
        // add global and global retain objects
        sProject.uInstObjects   = TRANSLATE_WORD((WORD) m_objectList.getMaxUsed() + 2);
    } else {
        sProject.uInstObjects   = TRANSLATE_WORD((WORD) m_objectList.getMaxUsed());
    }
    sProject.uDirectVars    = TRANSLATE_WORD((WORD)m_directSimpleVars.getUsed());
    sProject.ulIECOffset    = TRANSLATE_DWORD(m_trgAttr.trg_GmIecOffs);
    sProject.ulIECSize      = TRANSLATE_DWORD(m_trgAttr.trg_GmIecSize);
    sProject.uDebugCode     = TRANSLATE_WORD((IEC_UINT) m_trgAttr.trg_dgbSetting);
	sProject.uCpyRegions    = TRANSLATE_WORD((IEC_UINT) m_lTotCopyRegionCount);
    
	CASMCopySection* pCpy = getCpyRegForRC();
    assert(!pCpy || sProject.uCpyRegions>0);
    if(pCpy)
    {
	    sProject.uCpyRegConst = TRANSLATE_WORD((IEC_UINT)pCpy->getCount());
        sProject.uCpyRegCOff  = TRANSLATE_WORD((IEC_UINT)pCpy->getFinalOffset());
    }
    else
    {
        sProject.uCpyRegConst = 0;
        sProject.uCpyRegCOff  = 0;
    }

	sProject.uProjInfo		= TRANSLATE_WORD((IEC_UINT)1);

    fwrite(&sProject, sizeof(char), sizeof(sProject), pFile);
}

void ASMGlobalContext_C::dumpXProject_205(FILE* pFile)
{
    ST_IDENT_TYP         id = registerId(_T("__state"));
	const ASM_GLOB_COMP* pGComp = lookUpGlobalVar(id);
    unsigned long        ulStateVarByteOff;    

	if(pGComp == NULL)
		ASM_INTERNAL_ERROR_STOP(NULL);

	ulStateVarByteOff =  (unsigned long)(pGComp->bitOff / 8);

    XProject_205 sProject;

    sProject.ulBinDLVersion = TRANSLATE_DWORD(m_trgAttr.trg_version);
    sProject.ulStateVarOffs = TRANSLATE_DWORD(ulStateVarByteOff);
    sProject.uTasks         = TRANSLATE_WORD((IEC_UINT) m_taskList.getMaxUsed());
    sProject.uCodeObjects   = TRANSLATE_WORD((IEC_UINT) getNumberOfClasses());
    sProject.uInstObjects   = TRANSLATE_WORD((WORD) m_objectList.getMaxUsed());
    sProject.uDirectVars    = TRANSLATE_WORD((WORD)m_directSimpleVars.getUsed());
    sProject.ulIECOffset    = TRANSLATE_DWORD(m_trgAttr.trg_GmIecOffs);
    sProject.ulIECSize      = TRANSLATE_DWORD(m_trgAttr.trg_GmIecSize);
    sProject.uDebugCode     = TRANSLATE_WORD((IEC_UINT) m_trgAttr.trg_dgbSetting);
	sProject.uCpyRegions    = TRANSLATE_WORD((IEC_UINT) m_lTotCopyRegionCount);
    
	CASMCopySection* pCpy = getCpyRegForRC();
    assert(!pCpy || sProject.uCpyRegions>0);
    if(pCpy)
    {
	    sProject.uCpyRegConst = TRANSLATE_WORD((IEC_UINT)pCpy->getCount());
        sProject.uCpyRegCOff  = TRANSLATE_WORD((IEC_UINT)pCpy->getFinalOffset());
    }
    else
    {
        sProject.uCpyRegConst = 0;
        sProject.uCpyRegCOff  = 0;
    }
    fwrite(&sProject, sizeof(char), sizeof(sProject), pFile);
}

void ASMGlobalContext_C::dumpXProject_200(FILE* pFile)
{
    ST_IDENT_TYP         id = registerId(_T("__state"));
	const ASM_GLOB_COMP* pGComp = lookUpGlobalVar(id);
    unsigned long        ulStateVarByteOff;    

	if(pGComp == NULL)
		ASM_INTERNAL_ERROR_STOP(NULL);

	ulStateVarByteOff =  (unsigned long)(pGComp->bitOff / 8);

    XProject_200 sProject;

    if(m_trgAttr.trg_version <= 20300)
	{
	    // Version control prior to version V2.05. 
        sProject.ulBinDLVersion = TRANSLATE_DWORD(0x23072307ul);
	}
    else
	{
		// Starting with version V2.05 the 
	    // corresponding firmware version must be used!
        sProject.ulBinDLVersion = TRANSLATE_DWORD(m_trgAttr.trg_version);
	}

    sProject.ulStateVarOffs = TRANSLATE_DWORD(ulStateVarByteOff);
    sProject.uTasks         = TRANSLATE_WORD((IEC_UINT) m_taskList.getMaxUsed());
    sProject.uDebugCode     = TRANSLATE_WORD((IEC_UINT) m_trgAttr.trg_dgbSetting);
    sProject.uCodeObjects   = TRANSLATE_WORD((IEC_UINT) getNumberOfClasses());
    sProject.uInstObjects   = TRANSLATE_WORD((WORD) m_objectList.getMaxUsed());
    sProject.ulIECOffset    = TRANSLATE_DWORD(m_trgAttr.trg_GmIecOffs);
    sProject.ulIECSize      = TRANSLATE_DWORD(m_trgAttr.trg_GmIecSize);
    sProject.uDirectVars    = TRANSLATE_WORD((WORD)m_directSimpleVars.getUsed());
    assert(m_copySection.getUsed()==0);
    fwrite(&sProject, sizeof(char), sizeof(sProject), pFile);
}

void ASMGlobalContext_C::dumpProjInfo(FILE* pFile)
{
	XProjInfo sProjInfo;
	memset(&sProjInfo, 0x00, sizeof(sProjInfo));

	strncpy((char*)sProjInfo.szProjName,  m_trgAttr.trg_piName,     MAX_INFO_SHORT);		
    ((char*)sProjInfo.szProjName)[MAX_INFO_SHORT-1] = 0;
	strncpy((char*)sProjInfo.szProjVers,  m_trgAttr.trg_piVersion,  MAX_INFO_SHORT);
    ((char*)sProjInfo.szProjVers)[MAX_INFO_SHORT-1] = 0;

	strncpy((char*)sProjInfo.szCreated,	 m_trgAttr.trg_piCreated,   MAX_INFO_LONG);
    ((char*)sProjInfo.szCreated)[MAX_INFO_LONG-1] = 0;
	strncpy((char*)sProjInfo.szModified,  m_trgAttr.trg_piModified, MAX_INFO_LONG);
    ((char*)sProjInfo.szModified)[MAX_INFO_LONG-1] = 0;
	strncpy((char*)sProjInfo.szOwner,	 m_trgAttr.trg_piOwner,     MAX_INFO_SHORT);
    ((char*)sProjInfo.szOwner)[MAX_INFO_SHORT-1] = 0;

	strncpy((char*)sProjInfo.szComment1,  m_trgAttr.trg_piComment1, MAX_INFO_LONG);
    ((char*)sProjInfo.szComment1)[MAX_INFO_LONG-1] = 0;
	strncpy((char*)sProjInfo.szComment2,  m_trgAttr.trg_piComment2, MAX_INFO_LONG);
    ((char*)sProjInfo.szComment2)[MAX_INFO_LONG-1] = 0;
	strncpy((char*)sProjInfo.szComment3,  m_trgAttr.trg_piComment3, MAX_INFO_LONG);
    ((char*)sProjInfo.szComment3)[MAX_INFO_LONG-1] = 0;

    fwrite(&sProjInfo, sizeof(char), sizeof(sProjInfo), pFile);
}

bool ASMGlobalContext_C::dumpCmnHeader()
{
    int                  i;
    FILE*                pFile;
    FC_CString           fileName(MAX_PATH);


    // FILE "__common.hdr"
    //
    fileName << m_GenPath << FILE_NAME_COMMON_HDR;
    pFile = fopen(fileName, "wb");
    if(!pFile)
    {
        ST_DiagMsg1(NULL, E_4CCG_ERR_WRITE_FILE_FAILED, fileName);
        return false;
    }

    // version dependent project definition:
	if(m_trgAttr.trg_version >= 21030)
	{
        dumpXProject_213(pFile);
        dumpProjInfo(pFile);
	} 
	else if(m_trgAttr.trg_version >= 20700)
	{
        dumpXProject_207(pFile);
        dumpProjInfo(pFile);
	} 
    else if(m_trgAttr.trg_version >= 20500)
    {
        dumpXProject_205(pFile);
    }
    else
    {
        dumpXProject_200(pFile);
    }




    // Task definition
    for (i=0; i<m_taskList.getMaxUsed(); i++)
    {
        const ASM_TASK_TYP* pTask = m_taskList.get(i);

        WORD uRegionsWr =   pTask->MemRegions.ISegMemRgnWR.getUsed()
                           + pTask->MemRegions.OSegMemRgnWR.getUsed()
                           + pTask->MemRegions.GSegMemRgnWR.getUsed()
                           + pTask->MemRegions.GRSegMemRgnWR.getUsed();

        WORD uRegionsRd =   pTask->MemRegions.ISegMemRgnRD.getUsed()
                           + pTask->MemRegions.OSegMemRgnRD.getUsed()
                           + pTask->MemRegions.GSegMemRgnRD.getUsed()
                           + pTask->MemRegions.GRSegMemRgnRD.getUsed();

        if(pTask->MemRegions.GRSegMemRgnRD.getUsed()!=0 || 
           pTask->MemRegions.GRSegMemRgnWR.getUsed()!=0 ||
           pTask->MemRegions.GSegMemRgnWR.getUsed()!=0 ||
           pTask->MemRegions.GSegMemRgnRD.getUsed()!=0
          )
        {
           DumpUserText(_T("fatal error: unexpected global or retain memory region in this version\n"));
           ASM_INTERNAL_ERROR_STOP(NULL);
        }
        //compatibillity for 2.0: always gen a retain copy list:
        if(m_trgAttr.trg_version<=20500 && !_tcsicmp(m_trgAttr.trg_type, _T("4cec")))
        {
            if(m_usedGlobalRetainSize>0)
            {
                uRegionsWr++;
                uRegionsRd++;
            }
        }



        if (strlen(pTask->name.psz) > 32)
        {
            // TODO: error msg
            assert( ! "task name longer than 32 chars");
        }
        if(m_trgAttr.trg_version >= 21001)
        {
            XTask_21001 TaskDef;

			TaskDef.usAttrib	=  0;
			TaskDef.usAttrib	|= pTask->autoStart		? VMM_TASK_ATTR_AUTOSTART	: 0;
			TaskDef.usAttrib	|= pTask->bCycleSet		? VMM_TASK_ATTR_CYCLIC		: 0;
			TaskDef.usAttrib	|= pTask->bEventSet		? VMM_TASK_ATTR_EVENT_DRIVEN: 0;
			TaskDef.usAttrib	|= ! pTask->Loaded		? VMM_TASK_ATTR_UNLOADED	: 0;

			if(TaskDef.usAttrib & VMM_TASK_ATTR_CYCLIC)
			{
				TaskDef.ulPara1	= TRANSLATE_DWORD((DWORD)pTask->intervalTime);
				TaskDef.ulPara2	= 0;
			}
			else if (TaskDef.usAttrib & VMM_TASK_ATTR_EVENT_DRIVEN)
			{
				TaskDef.ulPara1	= TRANSLATE_DWORD((IEC_UDINT)pTask->Events);
				TaskDef.ulPara2	= TRANSLATE_DWORD((IEC_UDINT)(pTask->Events>>32));
			}
			else
			{
				TaskDef.ulPara1	= 0;
				TaskDef.ulPara2	= 0;
			}
            
            TaskDef.ulWatchDogMS= TRANSLATE_DWORD((DWORD)pTask->WatchDog);//for version >=2.10.01
            TaskDef.usPrograms	= (IEC_USINT)pTask->prgInstList.getMaxUsed();
            TaskDef.uRegionsWr	= TRANSLATE_WORD(uRegionsWr);
            TaskDef.uRegionsRd	= TRANSLATE_WORD(uRegionsRd);

            CASMCopySection* pCpy = getCpyRegForTask(pTask->name);
            if(pCpy)
            {
	            TaskDef.uCpyRegions= TRANSLATE_WORD((IEC_UINT)pCpy->getCount());
                TaskDef.uCpyRegOff = TRANSLATE_WORD((IEC_UINT)pCpy->getFinalOffset());
            }
            else
            {
	            TaskDef.uCpyRegions= 0;
                TaskDef.uCpyRegOff = 0;
            }

            memset(TaskDef.szName, 0, MAX_TASKNAME);
            strcpy((char *)TaskDef.szName, pTask->name.psz);
			_strupr((char *)TaskDef.szName);
			TaskDef.szName[MAX_TASKNAME - 1] = 0;

            TaskDef.usPriority  = (IEC_USINT) pTask->Prio;

            fwrite(&TaskDef, sizeof(char), sizeof(TaskDef), pFile);
        }
        else if(m_trgAttr.trg_version >= 20700)
        {
            XTask_207 TaskDef;
			
			TaskDef.usAttrib	=  0;
			TaskDef.usAttrib	|= pTask->autoStart		? VMM_TASK_ATTR_AUTOSTART	: 0;
			TaskDef.usAttrib	|= pTask->bCycleSet		? VMM_TASK_ATTR_CYCLIC		: 0;
			TaskDef.usAttrib	|= pTask->bEventSet		? VMM_TASK_ATTR_EVENT_DRIVEN: 0;
			TaskDef.usAttrib	|= ! pTask->Loaded		? VMM_TASK_ATTR_UNLOADED	: 0;

			if(TaskDef.usAttrib & VMM_TASK_ATTR_CYCLIC)
			{
				TaskDef.ulPara1	= TRANSLATE_DWORD((DWORD)pTask->intervalTime);
				TaskDef.ulPara2	= 0;
			}
			else if (TaskDef.usAttrib & VMM_TASK_ATTR_EVENT_DRIVEN)
			{
				TaskDef.ulPara1	= TRANSLATE_DWORD((IEC_UDINT)pTask->Events);
				TaskDef.ulPara2	= TRANSLATE_DWORD((IEC_UDINT)(pTask->Events>>32));
			}
			else
			{
				TaskDef.ulPara1	= 0;
				TaskDef.ulPara2	= 0;
			}

            TaskDef.usPrograms	= (IEC_USINT)pTask->prgInstList.getMaxUsed();
            TaskDef.uRegionsWr	= TRANSLATE_WORD(uRegionsWr);
            TaskDef.uRegionsRd	= TRANSLATE_WORD(uRegionsRd);

            CASMCopySection* pCpy = getCpyRegForTask(pTask->name);
            if(pCpy)
            {
	            TaskDef.uCpyRegions= TRANSLATE_WORD((IEC_UINT)pCpy->getCount());
                TaskDef.uCpyRegOff = TRANSLATE_WORD((IEC_UINT)pCpy->getFinalOffset());
            }
            else
            {
	            TaskDef.uCpyRegions= 0;
                TaskDef.uCpyRegOff = 0;
            }

            memset(TaskDef.szName, 0, MAX_TASKNAME);
            strcpy((char *)TaskDef.szName, pTask->name.psz);
			_strupr((char *)TaskDef.szName);
			TaskDef.szName[MAX_TASKNAME - 1] = 0;

            TaskDef.usPriority  = (IEC_USINT) pTask->Prio;

            fwrite(&TaskDef, sizeof(char), sizeof(TaskDef), pFile);
        }
        else if(m_trgAttr.trg_version >= 20500)
        {
            XTask_205 TaskDef;
            TaskDef.byStart    = pTask->autoStart;
            TaskDef.ulInterval = pTask->bCycleSet ? TRANSLATE_DWORD((DWORD)pTask->intervalTime) : 0;
            TaskDef.uPriority  = TRANSLATE_WORD((IEC_UINT) pTask->Prio);
            TaskDef.byPrograms = (IEC_BYTE) pTask->prgInstList.getMaxUsed();
            TaskDef.uRegionsWr = TRANSLATE_WORD(uRegionsWr);
            TaskDef.uRegionsRd = TRANSLATE_WORD(uRegionsRd);
            CASMCopySection* pCpy = getCpyRegForTask(pTask->name);
            if(pCpy)
            {
	            TaskDef.uCpyRegions= TRANSLATE_WORD((IEC_UINT)pCpy->getCount());
                TaskDef.uCpyRegOff = TRANSLATE_WORD((IEC_UINT)pCpy->getFinalOffset());
            }
            else
            {
	            TaskDef.uCpyRegions= 0;
                TaskDef.uCpyRegOff = 0;
            }

            memset((char*) &TaskDef.szName[0], 0, MAX_TASKNAME);
            strcpy((char*) &TaskDef.szName[0], pTask->name.psz);
            _strupr((char*) &TaskDef.szName[0]);
            TaskDef.byDummy = 0xff;
            fwrite(&TaskDef, sizeof(char), sizeof(TaskDef), pFile);
        }
        else
        {
            XTask_200 TaskDef;
            TaskDef.byStart    = pTask->autoStart;
            TaskDef.ulInterval = pTask->bCycleSet ? TRANSLATE_DWORD((DWORD)pTask->intervalTime) : 0;
            TaskDef.uPriority  = TRANSLATE_WORD((IEC_UINT) pTask->Prio);
            TaskDef.byPrograms = (IEC_BYTE) pTask->prgInstList.getMaxUsed();
            TaskDef.uRegionsWr = TRANSLATE_WORD(uRegionsWr);
            TaskDef.uRegionsRd = TRANSLATE_WORD(uRegionsRd);
            memset((char*) &TaskDef.szName[0], 0, MAX_TASKNAME);
            strcpy((char*) &TaskDef.szName[0], pTask->name.psz);
            _strupr((char*) &TaskDef.szName[0]);
            TaskDef.byDummy = 0xff;
            fwrite(&TaskDef, sizeof(char), sizeof(TaskDef), pFile);
        }

        // program instance definition
        for (int o=0; o<pTask->prgInstList.getMaxUsed(); o++)
        {
            ST_IDENT_TYP         PrgInst;
            ST_IDENT_TYP         PrgType;
            const ASM_GLOB_COMP* pGComp; 
            const ASMPROC_TYP*   pPrgFb;
            const ASM_OBJECT_TYP*pObj;


            PrgInst = pTask->prgInstList.get(o);
            pGComp  = lookUpGlobalVar(PrgInst);
            PrgType = pGComp->pType->pFBIDef->getTypeIdent();
            pPrgFb  = m_hashPrgFB.get(PrgType.psz);
            assert(pPrgFb != NULL);
            
            pObj = m_hashObjects.get(PrgInst.psz);
            assert(pObj != NULL);

            // prog inst
            XIndex Index;
            Index.uCode = TRANSLATE_WORD((IEC_UINT) pPrgFb->getClassId());
            Index.uInst = TRANSLATE_WORD((IEC_UINT) pObj->Index);
            fwrite(&Index, sizeof(char), sizeof(XIndex), pFile);
        }

        // memory region to copy in the VM
        WriteUsedMemRegions(pFile, ASM_SEGM_INPUT, pTask);
        WriteUsedMemRegions(pFile, ASM_SEGM_OUTPUT, pTask);

        //old code: WriteUsedMemRegions(pFile, ASM_SEGM_GLOBAL, pTask);
        //old code: WriteUsedMemRegions(pFile, ASM_SEGM_RETAIN, pTask);
        //compatibillity for 2.0: always gen a retain copy list:
        if(m_trgAttr.trg_version<=20500 && !_tcsicmp(m_trgAttr.trg_type, _T("4cec")))
        {
            if(m_usedGlobalRetainSize>0)
            {
                XMemRegion SMemRgn;
                SMemRgn.ulOffset = TRANSLATE_DWORD(0);
                SMemRgn.uSize    = TRANSLATE_WORD((unsigned short)m_usedGlobalRetainSize);
                SMemRgn.bySegment = ASM_SEGM_RETAIN;
                SMemRgn.bRead = 0;
                fwrite(&SMemRgn, sizeof(char), sizeof(XMemRegion), pFile);
                SMemRgn.bRead = 1;
                fwrite(&SMemRgn, sizeof(char), sizeof(XMemRegion), pFile);
            }
        }

    }

    // segment definition
    WriteGlobSegmentDesc(pFile);


    if(m_copySection.getUsed()>0)
    {
        for(i=0;i<m_copySection.getUsed(); i++)
        {
            CASMCopySection* pCpySec = m_copySection.get(i);
            if(pCpySec->getTyp()==ASM_CPY_TYP_RW)
                pCpySec->dumpToBinFile(pFile);
        }
        for(i=0;i<m_copySection.getUsed(); i++)
        {
            CASMCopySection* pCpySec = m_copySection.get(i);
            if(pCpySec->getTyp()==ASM_CPY_TYP_RC)
                pCpySec->dumpToBinFile(pFile);
        }
    }

 
	if(m_trgAttr.trg_version >= 21030)
    {
        dumpXIOLayer(pFile);
    }

    fclose(pFile);

    return true;
}

bool ASMGlobalContext_C::dumpCallStack()
{
    FILE* pFile;
    ST_String_C FileName;
    FC_CHashPtr<long, long> listOfCalled;
    const FC_CHashPtr<long, long>::Pair* pPair;

    fprintf(m_DldFile, _T("%s;%s\n"),
        DL_FILE_SECTION_DEBUG,
        FILE_NAME_CALL_STACK);

    // call stack
    //
    // FILE "__common.hdr"
    //
    FileName << m_GenPath;
    FileName << FILE_NAME_CALL_STACK;
    pFile = fopen(FileName, "wb");

    if (pFile == NULL)
        return false;

    int iMax = m_taskList.getMaxUsed();
    fwrite(&iMax, sizeof(char), sizeof(int), pFile);   // num of tasks

    for (int i=0; i<iMax; i++)
    {
        const ASM_TASK_TYP* pTask = m_taskList.get(i);

        listOfCalled.clear();
        int iPrgs = pTask->prgInstList.getMaxUsed();
        for (int o=0; o<iPrgs; o++)
        {
            ST_IDENT_TYP prgName = pTask->prgInstList.get(o);
            const ASM_GLOB_COMP* pPrg = lookUpGlobalVar(prgName);
            assert(pPrg);
            pPrg->pType->pFBIDef->buildCallStack(&listOfCalled);
        }

        long lNumOfClasses = listOfCalled.countPairs();
        fwrite(&lNumOfClasses, sizeof(char), sizeof(long), pFile);   // num of classes

        for (pPair = listOfCalled.firstPair(); pPair; pPair = pPair->pNext)
        {
            long ClsId = pPair->pKey;
            fwrite(&ClsId, sizeof(char), sizeof(long), pFile);   // classes id
        }
    }

    fclose(pFile);

    return true;
}




struct ASM_OPCDEF g_asmOPCfield[] = 
{
//PUPs
	{ "PSHD_GI_1",      0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF1} },
	{ "PSHD_GI_8",      0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },
	{ "PSHD_GI_16",     0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },
	{ "PSHD_GI_32",     0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },
	{ "PSHD_GI_64",     0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },

    { "PSHD_GO_1",      0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF1} },
	{ "PSHD_GO_8",      0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },
	{ "PSHD_GO_16",     0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },
	{ "PSHD_GO_32",     0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },
	{ "PSHD_GO_64",     0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },

    { "PSHD_GM_1",      0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF1} },
	{ "PSHD_GM_8",      0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },
	{ "PSHD_GM_16",     0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },
	{ "PSHD_GM_32",     0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },
	{ "PSHD_GM_64",     0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },

    { "PSHD_GR_1",      0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF1} },
	{ "PSHD_GR_8",      0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },
	{ "PSHD_GR_16",     0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },
	{ "PSHD_GR_32",     0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },
	{ "PSHD_GR_64",     0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },

    { "PSHD_IN_1",       0,  ASM_MAX_PRFIX, 1, {ASM_PAR_L_OFF1} },
	{ "PSHD_IN_8",       0,  ASM_MAX_PRFIX, 1, {ASM_PAR_L_OFF8} },
	{ "PSHD_IN_16",      0,  ASM_MAX_PRFIX, 1, {ASM_PAR_L_OFF8} },
	{ "PSHD_IN_32",      0,  ASM_MAX_PRFIX, 1, {ASM_PAR_L_OFF8} },
	{ "PSHD_IN_64",      0,  ASM_MAX_PRFIX, 1, {ASM_PAR_L_OFF8} },

    { "PSHI_IN_1",    0,  ASM_MAX_PRFIX, 1, {ASM_PAR_L_OFF8} },
    { "PSHI_IN_8",    0,  ASM_MAX_PRFIX, 1, {ASM_PAR_L_OFF8} },
    { "PSHI_IN_16",   0,  ASM_MAX_PRFIX, 1, {ASM_PAR_L_OFF8} },
    { "PSHI_IN_32",   0,  ASM_MAX_PRFIX, 1, {ASM_PAR_L_OFF8} },
    { "PSHI_IN_64",   0,  ASM_MAX_PRFIX, 1, {ASM_PAR_L_OFF8} },

    { "PSHO_ID_1",    0,  ASM_MAX_PRFIX, 2, {ASM_PAR_OBJID, ASM_PAR_OFFSETOF1} },
    { "PSHO_ID_8",    0,  ASM_MAX_PRFIX, 2, {ASM_PAR_OBJID, ASM_PAR_OFFSETOF8} },
    { "PSHO_ID_16",   0,  ASM_MAX_PRFIX, 2, {ASM_PAR_OBJID, ASM_PAR_OFFSETOF8} },
    { "PSHO_ID_32",   0,  ASM_MAX_PRFIX, 2, {ASM_PAR_OBJID, ASM_PAR_OFFSETOF8} },
    { "PSHO_ID_64",   0,  ASM_MAX_PRFIX, 2, {ASM_PAR_OBJID, ASM_PAR_OFFSETOF8} },

    { "PSHO_IN_1",  0,  ASM_MAX_PRFIX, 2, {ASM_PAR_L_OFF8, ASM_PAR_OFFSETOF1} },
    { "PSHO_IN_8",  0,  ASM_MAX_PRFIX, 2, {ASM_PAR_L_OFF8, ASM_PAR_OFFSETOF8} },
    { "PSHO_IN_16", 0,  ASM_MAX_PRFIX, 2, {ASM_PAR_L_OFF8, ASM_PAR_OFFSETOF8} },
    { "PSHO_IN_32", 0,  ASM_MAX_PRFIX, 2, {ASM_PAR_L_OFF8, ASM_PAR_OFFSETOF8} },
    { "PSHO_IN_64", 0,  ASM_MAX_PRFIX, 2, {ASM_PAR_L_OFF8, ASM_PAR_OFFSETOF8} },

    { "PSHO_ST_1",    0,  ASM_MAX_PRFIX, 1, {ASM_PAR_OFFSETOF1} },
    { "PSHO_ST_8",    0,  ASM_MAX_PRFIX, 1, {ASM_PAR_OFFSETOF8} },
    { "PSHO_ST_16",   0,  ASM_MAX_PRFIX, 1, {ASM_PAR_OFFSETOF8} },
    { "PSHO_ST_32",   0,  ASM_MAX_PRFIX, 1, {ASM_PAR_OFFSETOF8} },
    { "PSHO_ST_64",   0,  ASM_MAX_PRFIX, 1, {ASM_PAR_OFFSETOF8} },

    //Immediate:
    { "PSHC_VV_8",    0,  ASM_NO_PRFIX,  1, {ASM_PAR_LIT8 } },
    { "PSHC_VV_16",   0,  ASM_NO_PRFIX,  1, {ASM_PAR_LIT16} },
    { "PSHC_VV_32",   0,  ASM_NO_PRFIX,  1, {ASM_PAR_LIT32} },
    { "PSHC_VV_64",   0,  ASM_NO_PRFIX,  1, {ASM_PAR_LIT64} },
    { "PSHC_TX",      0,  ASM_NO_PRFIX,  1, {ASM_PAR_LITS8} },


    //POPs:
	{ "POPD_GI_1",      0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF1} },
	{ "POPD_GI_8",      0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },
	{ "POPD_GI_16",     0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },
	{ "POPD_GI_32",     0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },
	{ "POPD_GI_64",     0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },

    { "POPD_GO_1",      0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF1} },
	{ "POPD_GO_8",      0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },
	{ "POPD_GO_16",     0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },
	{ "POPD_GO_32",     0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },
	{ "POPD_GO_64",     0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },

    { "POPD_GX_1",     0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF1} },
	{ "POPD_GX_8",     0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },
	{ "POPD_GX_16",    0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },
    { "POPD_GX_32",    0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },
	{ "POPD_GX_64",    0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },

    { "POPD_GM_1",      0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF1} },
	{ "POPD_GM_8",      0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },
	{ "POPD_GM_16",     0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },
	{ "POPD_GM_32",     0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },
	{ "POPD_GM_64",     0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },

    { "POPD_GR_1",      0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF1} },
	{ "POPD_GR_8",      0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },
	{ "POPD_GR_16",     0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },
	{ "POPD_GR_32",     0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },
	{ "POPD_GR_64",     0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },

    { "POPD_IN_1",       0,  ASM_MAX_PRFIX, 1, {ASM_PAR_L_OFF1} },
	{ "POPD_IN_8",       0,  ASM_MAX_PRFIX, 1, {ASM_PAR_L_OFF8} },
	{ "POPD_IN_16",      0,  ASM_MAX_PRFIX, 1, {ASM_PAR_L_OFF8} },
	{ "POPD_IN_32",      0,  ASM_MAX_PRFIX, 1, {ASM_PAR_L_OFF8} },
	{ "POPD_IN_64",      0,  ASM_MAX_PRFIX, 1, {ASM_PAR_L_OFF8} },

    { "POPI_IN_1",    0,  ASM_MAX_PRFIX, 1, {ASM_PAR_L_OFF8} },
    { "POPI_IN_8",    0,  ASM_MAX_PRFIX, 1, {ASM_PAR_L_OFF8} },
    { "POPI_IN_16",   0,  ASM_MAX_PRFIX, 1, {ASM_PAR_L_OFF8} },
    { "POPI_IN_32",   0,  ASM_MAX_PRFIX, 1, {ASM_PAR_L_OFF8} },
    { "POPI_IN_64",   0,  ASM_MAX_PRFIX, 1, {ASM_PAR_L_OFF8} },

    { "POPO_ID_1",    0,  ASM_MAX_PRFIX, 2, {ASM_PAR_OBJID, ASM_PAR_OFFSETOF1} },
    { "POPO_ID_8",    0,  ASM_MAX_PRFIX, 2, {ASM_PAR_OBJID, ASM_PAR_OFFSETOF8} },
    { "POPO_ID_16",   0,  ASM_MAX_PRFIX, 2, {ASM_PAR_OBJID, ASM_PAR_OFFSETOF8} },
    { "POPO_ID_32",   0,  ASM_MAX_PRFIX, 2, {ASM_PAR_OBJID, ASM_PAR_OFFSETOF8} },
    { "POPO_ID_64",   0,  ASM_MAX_PRFIX, 2, {ASM_PAR_OBJID, ASM_PAR_OFFSETOF8} },

    { "POPO_IN_1",  0,  ASM_MAX_PRFIX, 2, {ASM_PAR_L_OFF8, ASM_PAR_OFFSETOF1} },
    { "POPO_IN_8",  0,  ASM_MAX_PRFIX, 2, {ASM_PAR_L_OFF8, ASM_PAR_OFFSETOF8} },
    { "POPO_IN_16", 0,  ASM_MAX_PRFIX, 2, {ASM_PAR_L_OFF8, ASM_PAR_OFFSETOF8} },
    { "POPO_IN_32", 0,  ASM_MAX_PRFIX, 2, {ASM_PAR_L_OFF8, ASM_PAR_OFFSETOF8} },
    { "POPO_IN_64", 0,  ASM_MAX_PRFIX, 2, {ASM_PAR_L_OFF8, ASM_PAR_OFFSETOF8} },

    { "POPO_ST_1",    0,  ASM_MAX_PRFIX, 1, {ASM_PAR_OFFSETOF1} },
    { "POPO_ST_8",    0,  ASM_MAX_PRFIX, 1, {ASM_PAR_OFFSETOF8} },
    { "POPO_ST_16",   0,  ASM_MAX_PRFIX, 1, {ASM_PAR_OFFSETOF8} },
    { "POPO_ST_32",   0,  ASM_MAX_PRFIX, 1, {ASM_PAR_OFFSETOF8} },
    { "POPO_ST_64",   0,  ASM_MAX_PRFIX, 1, {ASM_PAR_OFFSETOF8} },


//LEAs
    { "LEAD_GI",       0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },
    { "LEAD_GO",       0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },
	{ "LEAD_GX",      0,  ASM_MAX_PRFIX, 2, {ASM_PAR_G_OFF8, ASM_PAR_SIZE8} },
    { "LEAD_GM",       0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },
    { "LEAD_GR",       0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF8} },
    { "LEAD_IN",        0,  ASM_MAX_PRFIX, 1, {ASM_PAR_L_OFF8} },

    { "LEAD_GI_1",      0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF1} },
	{ "LEAD_GX_1",     0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF1} },
    { "LEAD_GM_1",      0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF1} },
    { "LEAD_GR_1",      0,  ASM_MAX_PRFIX, 1, {ASM_PAR_G_OFF1} },
    { "LEAD_IN_1",       0,  ASM_MAX_PRFIX, 1, {ASM_PAR_L_OFF1} },

    { "LEAO_XD",      0,  ASM_MAX_PRFIX, 1, {ASM_PAR_OBJID} },
    { "LEAO_ID",     0,  ASM_MAX_PRFIX, 2, {ASM_PAR_OBJID, ASM_PAR_OFFSETOF8} },
    { "LEAO_XN",    0,  ASM_MAX_PRFIX, 1, {ASM_PAR_L_OFF8} },
    { "LEAO_IN",   0,  ASM_MAX_PRFIX, 2, {ASM_PAR_L_OFF8, ASM_PAR_OFFSETOF8} },
    { "LEAI_ID",     0,  ASM_MAX_PRFIX, 1, {ASM_PAR_OFFSETOF8} },
    { "LEAO_ST",     0,  ASM_MAX_PRFIX, 1, {ASM_PAR_OFFSETOF8} },

    { "LEAO_ID_1",    0,  ASM_MAX_PRFIX, 2, {ASM_PAR_OBJID, ASM_PAR_OFFSETOF1} },
    { "LEAO_IN_1",  0,  ASM_MAX_PRFIX, 2, {ASM_PAR_L_OFF8, ASM_PAR_OFFSETOF1} },
    { "LEAO_ST_1",    0,  ASM_MAX_PRFIX, 1, {ASM_PAR_OFFSETOF1} },

    { "CLST",        0,  ASM_MAX_PRFIX, 1, {ASM_PAR_SIZE8} },
    { "RETN",          0,  ASM_MAX_PRFIX, 0, {ASM_PAR_UNDEF} },

    { "CALF",         0,  ASM_MAX_PRFIX, 2, {ASM_PAR_CLSID, ASM_PAR_SIZE8} },
    { "CALF_PR",        0,  ASM_MAX_PRFIX, 3, {ASM_PAR_CLSID, ASM_PAR_SIZE8, ASM_PAR_SIZE8} },

    { "CALB_PI",        0,  ASM_MAX_PRFIX, 2, {ASM_PAR_CLSID, ASM_PAR_OBJID} },
    { "CALB_PN",      0,  ASM_MAX_PRFIX, 2, {ASM_PAR_CLSID, ASM_PAR_L_OFF8} },

    { "CALF_SL",      0,  ASM_MAX_PRFIX, 2, {ASM_PAR_FUNID, ASM_PAR_SIZE8} },
    { "CALF_CL",        0,  ASM_MAX_PRFIX, 2, {ASM_PAR_FUNID, ASM_PAR_SIZE8} },

    { "CALB_SI",     0,  ASM_MAX_PRFIX, 2, {ASM_PAR_FBID, ASM_PAR_OBJID} },
    { "CALB_SN",   0,  ASM_MAX_PRFIX, 2, {ASM_PAR_FBID, ASM_PAR_L_OFF8} },
    { "CALB_CI",       0,  ASM_MAX_PRFIX, 2, {ASM_PAR_FBID, ASM_PAR_OBJID} },
    { "CALB_CN",     0,  ASM_MAX_PRFIX, 2, {ASM_PAR_FBID, ASM_PAR_L_OFF8} },



//PUP/POP/LEAs vector:
    { "PSHV_ID_1",        0,  ASM_MAX_PRFIX, 2, {ASM_PAR_OBJID, ASM_PAR_AIDX} },
    { "PSHV_ID_8",        0,  ASM_MAX_PRFIX, 2, {ASM_PAR_OBJID, ASM_PAR_AIDX} },
    { "PSHV_ID_16",       0,  ASM_MAX_PRFIX, 2, {ASM_PAR_OBJID, ASM_PAR_AIDX} },
    { "PSHV_ID_32",       0,  ASM_MAX_PRFIX, 2, {ASM_PAR_OBJID, ASM_PAR_AIDX} },
    { "PSHV_ID_64",       0,  ASM_MAX_PRFIX, 2, {ASM_PAR_OBJID, ASM_PAR_AIDX} },

    { "POPV_ID_1",         0,  ASM_MAX_PRFIX, 2, {ASM_PAR_OBJID, ASM_PAR_AIDX} },
    { "POPV_ID_8",         0,  ASM_MAX_PRFIX, 2, {ASM_PAR_OBJID, ASM_PAR_AIDX} },
    { "POPV_ID_16",        0,  ASM_MAX_PRFIX, 2, {ASM_PAR_OBJID, ASM_PAR_AIDX} },
    { "POPV_ID_32",        0,  ASM_MAX_PRFIX, 2, {ASM_PAR_OBJID, ASM_PAR_AIDX} },
    { "POPV_ID_64",        0,  ASM_MAX_PRFIX, 2, {ASM_PAR_OBJID, ASM_PAR_AIDX} },

    { "PSHV_IN_1",      0,  ASM_MAX_PRFIX, 2, {ASM_PAR_L_OFF8, ASM_PAR_AIDX} },
    { "PSHV_IN_8",      0,  ASM_MAX_PRFIX, 2, {ASM_PAR_L_OFF8, ASM_PAR_AIDX} },
    { "PSHV_IN_16",     0,  ASM_MAX_PRFIX, 2, {ASM_PAR_L_OFF8, ASM_PAR_AIDX} },
    { "PSHV_IN_32",     0,  ASM_MAX_PRFIX, 2, {ASM_PAR_L_OFF8, ASM_PAR_AIDX} },
    { "PSHV_IN_64",     0,  ASM_MAX_PRFIX, 2, {ASM_PAR_L_OFF8, ASM_PAR_AIDX} },

    { "POPV_IN_1",       0,  ASM_MAX_PRFIX, 2, {ASM_PAR_L_OFF8, ASM_PAR_AIDX} },
    { "POPV_IN_8",       0,  ASM_MAX_PRFIX, 2, {ASM_PAR_L_OFF8, ASM_PAR_AIDX} },
    { "POPV_IN_16",      0,  ASM_MAX_PRFIX, 2, {ASM_PAR_L_OFF8, ASM_PAR_AIDX} },
    { "POPV_IN_32",      0,  ASM_MAX_PRFIX, 2, {ASM_PAR_L_OFF8, ASM_PAR_AIDX} },
    { "POPV_IN_64",      0,  ASM_MAX_PRFIX, 2, {ASM_PAR_L_OFF8, ASM_PAR_AIDX} },

    { "PSHV_ST_1",    0,  ASM_MAX_PRFIX, 1, {ASM_PAR_AIDX} },
    { "PSHV_ST_8",    0,  ASM_MAX_PRFIX, 1, {ASM_PAR_AIDX} },
    { "PSHV_ST_16",   0,  ASM_MAX_PRFIX, 1, {ASM_PAR_AIDX} },
    { "PSHV_ST_32",   0,  ASM_MAX_PRFIX, 1, {ASM_PAR_AIDX} },
    { "PSHV_ST_64",   0,  ASM_MAX_PRFIX, 1, {ASM_PAR_AIDX} },
    
    { "POPV_ST_1",     0,  ASM_MAX_PRFIX, 1, {ASM_PAR_AIDX} },
    { "POPV_ST_8",     0,  ASM_MAX_PRFIX, 1, {ASM_PAR_AIDX} },
    { "POPV_ST_16",    0,  ASM_MAX_PRFIX, 1, {ASM_PAR_AIDX} },
    { "POPV_ST_32",    0,  ASM_MAX_PRFIX, 1, {ASM_PAR_AIDX} },
    { "POPV_ST_64",    0,  ASM_MAX_PRFIX, 1, {ASM_PAR_AIDX} },

    { "LEAV_ID",          0,  ASM_MAX_PRFIX, 2, {ASM_PAR_OBJID, ASM_PAR_AIDX} },
    { "LEAV_IN",        0,  ASM_MAX_PRFIX, 2, {ASM_PAR_L_OFF8, ASM_PAR_AIDX} },
    { "LEAV_ST",         0,  ASM_MAX_PRFIX, 2, {ASM_PAR_SIZE1, ASM_PAR_AIDX} },
    { "LEAV_SS",     0,  ASM_MAX_PRFIX, 1, {ASM_PAR_AIDX} },
    { "LEAV_TX",      0,  ASM_MAX_PRFIX, 1, {ASM_PAR_AIDX} },

    { "PSHX____16",   0,  ASM_MAX_PRFIX, 2, {ASM_PAR_L_OFF8, ASM_PAR_AIDX} },
    { "POPX____16",    0,  ASM_MAX_PRFIX, 2, {ASM_PAR_L_OFF8, ASM_PAR_AIDX} },

    { "MCPY",        0,  ASM_MAX_PRFIX, 1, {ASM_PAR_SIZE8} },
    { "INIT_STRING",   0,  ASM_NO_PRFIX,  1, {ASM_PAR_LIT8} },
};


void ASMGlobalContext_C::initOpCodes()
{
    int iMax = ASM_getNumberOfOpcodes();

    for (int i=0; i<iMax; i++)
    {
        ASM_CodeTableEntry* pEntry = ASM_getOpcode(i);
        //printf("%s\n", pEntry->OpcodeString);
        bool bNew = m_hashOpCodes.insert(pEntry->OpcodeString, pEntry->OpCode);
        assert(bNew);
    }

    for(i=0; i<FC_ARRAY_LEN(g_asmOPCfield); i++)
    {
        g_asmOPCfield[i].uscod = (unsigned short)m_hashOpCodes.get(g_asmOPCfield[i].pszId);
        if(g_asmOPCfield[i].uscod ==0)
            ASM_INTERNAL_ERROR_STOP(NULL);
        m_hashOPC.insert(g_asmOPCfield[i].pszId, &g_asmOPCfield[i]);
    }

}


void ASMGlobalContext_C::calcMemberOffsets()
{
    int          i;
    ASMPROC_TYP* pProc;

    TO_MAPWRITE0(_T("\n.begin data offsets:"));
    for(i=0; i<m_funList.getMaxUsed(); i++)
    {
        pProc = m_funList.get(i);
        pProc->calcFunMemberOffsets();
    }

    for(i=0; i<m_prgFBList.getMaxUsed(); i++)
    {
        pProc = m_prgFBList.get(i);
        pProc->calcMemberOffsets();
    }
    TO_MAPWRITE0(_T(".end data offsets:\n"));
}

/* ---------------------------------------------------------------------------- */

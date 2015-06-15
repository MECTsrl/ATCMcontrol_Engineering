/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/globctx.h 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: globctx.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/globctx.h $
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

#ifndef _GLOBCTX_H_
#define _GLOBCTX_H_

#include <stdio.h>

#include "defines.h"
#include "fc_todebug\fc_assert.h"
#include "fc_tools\fc_cgarray.h"
#include "fc_tools\fc_cstring.h"
#include "fc_tools\fc_hash.h"
#include "fc_tools\fc_system.h"
#include "cgt_misc.h"
#include "cgt_tools.h"
#include "CG_IECFront.h"
#include "cgt_CDiagMsg.h"
#include "parseKAD.h"
#include "license.h"

//internal error with context info:
#define VMASSERT(cond, pEdp)   if(!(cond))CGlobCtx::vminterr(pEdp, __FILE__, __LINE__, #cond)
#define VMFATAL(resason, pEdp) CGlobCtx::vminterr(pEdp, __FILE__, __LINE__, resason)

class CTarget;
class CCodeInfo;


//Command line and 4cp's [compiler.<targettype] options:
//All file names are as found in argv[] of main or 4cp
//they are not converted to full pathes here.
struct CMDL_OPT_TYP
{
    FC_CStr     sz4CP;              //4cp file          
    FC_CStrList targets;            //list of targets to build, if empty ->build all
    FC_CStr     szCheckFile;        //the file to check
    FC_CStr     szCurTrgTyp;        //the current target type, 
                                    //multiple targets must be of the same type
    FC_CStr     szLibTrgType;       //build library for this target type
    bool        bIncrementelly;     //-i option
    FC_CStr     szMakeLib;          //the lib to make/append
    int         nGenAsmMapFile;     //=0: no map file
                                    //=1: map file
                                    //=2: map file with ST source code annotation
    bool        bCallAsm;           //undocumented feature:
                                    //do not call the external assembler (default : true)
    bool        bCallPostCompiler;  //undocumented feature:
                                    //perform the post compile step or not
                                    //[post:1|post:0]
    int         nVerboseMode;       //undocumented feature:
                                    //output call of asm, ncc with or without full
                                    //command line, value 0,1,2, default is 1
    FC_CStr     sPostCompileOpt;    //options for the post copmiler
};

enum BP_OPTION
{
    BP_OPT0 =0,         //no BP at all
    BP_OPT1 =1,         //begin/end POU only
    BP_OPT2 =2,         //whereever possible
    BP_OPT_INTERNAL = 3,// internal option no breakpoint will be dumped even in asm file
    BP_OPT_INVAL = 4
};

// hash table for IDs of library FUNs and FBs
struct LibFunInfo
{
    long  nNumInControl; //ordinal number inside controller firmware
    long  nMinVersion;   //minimun controller Version required (0 is all versions)
    long  nMaxVersion;   //maximun controller Version allowed (99999 is all versions)
};

typedef FC_CHashStr<LibFunInfo*> LibFunIdFile;

struct MEM_ARRAY
{
    BYTE*         pAdr;
    unsigned long ulSize;
};

struct ADDR_MAPS
{
    ADDR_MAPS() :
        pGIMemWR(NULL),   pGIMemRD(NULL),
        pGOMemWR(NULL),   pGOMemRD(NULL)
    {
    }

    MEM_ARRAY* pGIMemWR;       // array of memory array    INPUT, WRITE
    MEM_ARRAY* pGIMemRD;       //                          INPUT, READ
    MEM_ARRAY* pGOMemWR;       //                          OUTPUT, WRITE
    MEM_ARRAY* pGOMemRD;       //                          OUTPUT, READ
};
struct MEM_RGNS
{
    FC_CGArray<VM_MemRegion*> GIMemRgnsWR;  // mem regions for INPUT, WRITE
    FC_CGArray<VM_MemRegion*> GIMemRgnsRD;  //                 INPUT, READ
    FC_CGArray<VM_MemRegion*> GOMemRgnsWR;  //                 OUTPUT, WRITE
    FC_CGArray<VM_MemRegion*> GOMemRgnsRD;  //                 OUTPUT, READ
};

inline VM_ADR_OFF VM_AlignBitOffset(VM_ADR_OFF bitOff, unsigned long ula)
{
    return ((bitOff+ula-1)/ula)*ula;
}


class CGlobCtx
{
    SL_4CVMLicData          m_licData;
    CMDL_OPT_TYP            m_pCmdlOpts;
    FC_CGArray<CTarget*>    m_targets;
    FC_CString              m_4C_Version;

    //set by 4cp's [compiler.4cpc] opt=bp0/1/2 setting
    BP_OPTION               m_bpopt; 
    bool                    m_bNccCalled;//TODO should be in CTarget??
    // for the 4VM target every body has to be assigned to a unique
    // class ID.
    long                    m_NextFreeClassId;
    
    
    FC_CHashStr<const CG_Resource*> m_resources;//loaded .res

    FC_CHashPtr<const CG_Symtab*, CCodeInfo*> m_POUcode;

    FC_CHashPtr<const CG_LIBContext*, LibFunIdFile*> m_LibFunIdFiles;

    const CG_SymEntry** m_pAutoExternals;    //auto extern decls

    FC_CGArray<const CG_SymEntry*>  m_ComplexGlobals;

    //counts asm files to compile with 4asm.
    int                     m_AsmFilesCreated; 

    //directory info set by initPathInfo:
    FC_CStr   m_TrgTypeGenPath;// full path of gen\.<trg type>

    //complex initialisations
    FC_CGArray<const CG_SymEntry*>  m_initializedEntries;

    // address calculation
    long                                            m_lResTasks;
    long                                            m_maxChanelArraySizeAllocated;
    ADDR_MAPS*                                      m_pAddrMaps;
    FC_CHashPtr<const CG_Task*, ADDR_MAPS*>         m_TaskAddrMaps;
    FC_CHashPtr<const CG_Task*, MEM_RGNS*>          m_TaskAllMemRgns;
    FC_CHashPtr<const CG_SymEntry*, VM_Address*>    m_DirectAddrEntries;


    //must be called very early,but the front end must have
    //the 4CP loaded also:
    void initPathInfo();
    void cleanGenDir();


    //load source file attracted by targtes
    //loadConfig->loadTargets->loadResources
    //loadResources also calls code gen routines for the bodies
    void loadConfig();     
    void loadTargets();
    void loadResources();
    
    //check task properties constraints: ranges and valid combinations
    void checkTaskProps(const CG_Task* pTask);

    //dumps the target's DBI:
    void dumpTargetDBIFile(
        CTarget*            pTrg,
        const CG_Resource*  pRes
    );
    //dumps the instance view of a target:
    void dumpInstView(
        CTarget*            pTrg,
        const CG_Resource*  pRes
    );
    //dumps a loader.list for a target:
    void dumpTrgLoaderList(
        CTarget*                pTrg,
        const CG_Symtab*const*  pzpRefSym
    );

    // calculate used READ and WRITE memory regions for every task
    void       calcMemRegions(const BYTE*                pMem,
                              int                        iSize,
                              unsigned long              ulOffset,
                              FC_CGArray<VM_MemRegion*>* pMemRgns,
                              int                        iMaxMemRgnsPerSegm);

    // dumps the *.4asm file for the external assembler
    void buildAsmFile(CTarget* pTrg);
    // helper to build ASM file
    void dumpTargetInfo(CGT_CDmp& dmp, const CTarget* pTrg);
    void dumpGlobalSimpleVars(
        const CG_Symtab* pGVL, 
        CTarget*         pTrg, 
        LPCTSTR fileName
    );
    void dumpGlobalVarsOfType(
        const CG_Symtab* pGVL, 
        const CG_Symtab* pSym, 
        CTarget*         pTrg, 
        LPCTSTR fileName
    );
    void dumpProgInstances(
        const CG_Symtab* pGVL, 
        CTarget*         pTrg, 
        LPCTSTR fileName
    );
    void dumpResource(const CG_Resource* pRes, LPCTSTR fileName);
    void dumpTasks(const CG_Resource* pRes, LPCTSTR fileName);
    void dumpMemRegion(CGT_CDmp&                  dmp,
                       const FC_CGArray<VM_MemRegion*>* pMemRgns,
                       VM_BITSET                  bsCurSegm,
                       LPCTSTR                    pszSegmSize,
                       LPCTSTR                    pszListType,
                       LPCTSTR                    pszSegm);
    void dumpConfigSect(const CG_Resource* pRes, LPCTSTR fileName);


    // get the access information for every symentry
    void calcVarExtAccessInfo(CTarget* pTrg);
    void InsertIntoUsedMap(ADDR_MAPS* pAdrMap, CGT_AccInfo*  pAccInfo);
    
    // direct addresses will be converted to the VM format
    void calcDirectAddresses(CTarget* ptrg);
    
    

    void compilePOU(
        const CG_Resource*   pRes, //may be NULL if building a library
        const CG_Symtab*     pSym
    );

    //compile4CPPOU helper function to build libs.
    void compile4CPPOU(
        const CG_POU4CPInfo* pInf //a 4cp POU info
    );


    void callExternalCompiler(const CTarget* pTrg);

    void callPostCompile(const CTarget* pTrg);
    void checkMeminfo(const CTarget* pTrg);

    void buildDLFile(const CTarget* pTrg);
    void appendFile(FILE*         pDestFile,
                    const TCHAR*  pPath,
                    const TCHAR*  pFileToAppend);

    bool loadIdLibFunFb(const CG_LIBContext* libCtx,
                        const CG_Edipos*     pEdp,
                        LibFunIdFile*        pLibFunFbIds);
    
    bool fillIdHash(const TCHAR*  pszFunFBidFile, 
                    LibFunIdFile* pLibFunFbIds, 
                    LPCTSTR       pCharBuf);


    static CGT_CDiagMsg*     m_gpdm;//only used for old code tobe removed
public:

    CGlobCtx(
        ICG_IECFront* pFront
    );
    ~CGlobCtx();
    CMDL_OPT_TYP* getWrtOpts(){return &m_pCmdlOpts;}//get writable pointer

    static void vminterr(const CG_Edipos* pEdp, const TCHAR* pszFile, long nLine, const TCHAR* pszReason=NULL);

    //main operations:
    void rebuildAll();

    //only check, do not destrtoy old build result:
    void checkFile();

    //if called to build a library:
    void dumpPOULinkFiles();

    //------------------------------------------------------------------*
    /**
     * GetOptionsFrom4CP: loads any target type specific compiler options
     *                    from the 4CP file. The 4CP file must be 
     *                    succesfully loaded before calling this function.
     *
     * Diagnostics:
     * 1) All errors/warnings during the parse process are submitted, 
     *
     * @param         pOpts: options structure containing the available
     *                       options for this target 
     *
     * @return        false: if option is required and not found
     *
     * @exception       -
     *
     * @see           (CMDL_OPT_TYP defined in glbctx.h)
     *
     */
    bool getOptionsFrom4CP();

    // licence stuff
    void CGlobCtx::initLicenseInfo(
        const CG_LIBContext*const* pzLIBs,
        const TCHAR*               pszTrgType
    );
    const SL_4CVMLicData* getLicData()const{return &m_licData;}
    void checkSourceProtection();
    
    // LIBRARY FUNs and FBs
    // return -1 if not succesful: fatal error
    long getIdOfLibFunFb(
        const CG_LIBContext* libCtx,
        const CG_Edipos*     pEdpMac,
        const TCHAR*         pszNameOrLit,
        long*                pMinVersion, //output: min control version, 0 if all versions are ok                
        long*                pMaxVersion  //output: max control version, 99999 if all versions are ok
    );


    //public and short names for convenience:
    CKADInfo                 m_kad;
    ICG_IECFront* const      m_pIF; //front end IF
    CGT_CDiagMsg             m_dm;  //diag msg handler
    const CMDL_OPT_TYP*const m_popt;//cmdl option set
    ICG_Config*              m_pConfig;//may be NULL if no config
                                       //possible for lib projects.
                                       //set in loadConfig()

    // get the class id for a POU
    long getNextFreeClassId(){return m_NextFreeClassId++;}

    // get a target
    CTarget* getTarget()
    {
        // we have only one target at the moment, may be 0 if build libs:
        assert(m_targets.getUsed()<=1);
        return m_targets.getUsed()>0 ?  m_targets.get(0) : NULL;//TODO more tgrs??
    }

    // get debug option
    BP_OPTION getBPOption()const      { return m_bpopt; }
    const CMDL_OPT_TYP* getOpts()const{return &m_pCmdlOpts;}

    //the gen path for DBI file:
    const TCHAR* getTrgDBIPath()const;
    //the gen path:
    const TCHAR* getGenPath()const
    {
        return m_pIF->FCPgetGenPath();
    }
    // full path of gen\.<trg type>
    const TCHAR* getTrgTypeGenPath()const
    {
        assert(!m_TrgTypeGenPath.isEmpty());
        return m_TrgTypeGenPath;
    }

    // intitialized complex instances
    FC_CGArray<const CG_SymEntry*>* getInitEntries() { return &m_initializedEntries; }

    const CG_POU4CPInfo** getProcessed4CPFiles()const;

    const TCHAR* get4CCopyRightSourceHeader(LPCTSTR trg);

    CCodeInfo* getPOUInfoBySym(const CG_Symtab* pSym)const { return m_POUcode.get(pSym); }

    const FC_CHashPtr<const CG_SymEntry*, VM_Address*>* getDirectVars()const
	{
		return &m_DirectAddrEntries;
	}
};

// OSAI Hack >>
bool gen4GHFile(const TCHAR* pszProjPath, const TCHAR* pszProjGuid);
// OSAI Hack <<


#endif

/* ---------------------------------------------------------------------------- */

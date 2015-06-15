/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/asm/globalContext.h 2     12.04.07 19:53 Ef $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: globalContext.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/asm/globalContext.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / CCG
 *
 * =COMPONENT			ASM
 *
 * =CURRENT 	 $Date: 12.04.07 19:53 $
 *			 $Revision: 2 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _GLOBALCONTEXT_H_
#define _GLOBALCONTEXT_H_



#include "asmtools.h"
#include "asmstmt.h"
#include "asmerr.h"

class CASMCopySection;

class FileContext_C
{
    ST_String_C  m_fileName;
    int          m_errorInFile;
public:
    FileContext_C(const TCHAR* pszFile);

    const TCHAR* getFileName()const{return m_fileName;}
    const TCHAR* getErrMsgFileName()const{return m_fileName;}
    void setOnOpenLastWriteFileTime(__int64 ft){}
    int getErrorsInFile()const{return m_errorInFile;}
    void incrementErrorsInFile(){m_errorInFile++;}
};


struct ASM_TARGET_ATTRIBUTES
{
    FC_CStr       trg_type;            //4cec,4cvm,...
    unsigned long trg_version;         //firmware version 
    unsigned long trg_maxObjects;      //max number of instance objects
    bool          trg_dumpinfo;

    unsigned long trg_ISegStart;       //start address of input segment
    unsigned long trg_ISegSize;        //size of input segment
    unsigned long trg_OSegStart;       //start address of output segment
    unsigned long trg_OSegSize;        //size of output segment
    unsigned long trg_GMSegStart;      //start address of GLOBAL segment
    unsigned long trg_GMSegSize;       //size of GLOBAL segment
    unsigned long trg_GMRSegStart;     //start address of RETAIN segment
    unsigned long trg_GMRSegSize;      //sizof RETAIN segment
    unsigned long trg_CodeSegStart;    //start address of CODE segment
    unsigned long trg_CodeSegSize;     //size of CODE segment
    unsigned long trg_LocDataSegStart; //start address of INSTANCE segment
    unsigned long trg_LocDataSegSize;  //size of INSTANCE segment
    unsigned long trg_GmIecOffs;       //offset of global IEC variables relative to the GDP (trg_GMSegStart)
    unsigned long trg_GmIecSize;       //size of global IEC variables 
    unsigned long trg_dgbSetting;      //breakpoint setting in Engineering

    long          trg_instAlignBytes;  //1,2,4,8: maximum alignment for fb,prg and auto globals.
                                       //If set to 8 all data types will be aligned to their size
                                       //simple BOOLs are not affected, BOOLs in fb,prg and auto global
                                       //are bit packed. In generall data types with size <trg_instAlignBytes 
                                       //are not affected these types are always aligned to their size.
    long          trg_stackAlignBytes; //1,2,4,8: minimum alignment for stack based offsets
                                       //all pushes are assumed to push at least this number of bits, 
                                       //even a push BOOL will do so.
    int           trg_bigEndian;       //0 little endian, 1 big,  -1 not set
    long          trg_minOperandSize;  // 8, 16 or 32 bit

    FC_CStr       trg_piVersion;        // Project infos
    FC_CStr       trg_piName;
    FC_CStr       trg_piModified;
    FC_CStr       trg_piCreated;
    FC_CStr       trg_piOwner;
    FC_CStr       trg_piComment1;
    FC_CStr       trg_piComment2;
    FC_CStr       trg_piComment3;

    ASM_TARGET_ATTRIBUTES()
    {
        trg_version = 0x0;
        trg_maxObjects= 0x0;
        trg_dumpinfo = false;
        trg_ISegStart = 0xffffffff;
        trg_ISegSize = 0xffffffff;
        trg_OSegStart = 0xffffffff;
        trg_OSegSize = 0xffffffff;
        trg_GMSegStart = 0xffffffff;
        trg_GMSegSize = 0xffffffff;
        trg_GMRSegStart = 0xffffffff;
        trg_GMRSegSize = 0xffffffff;
        trg_CodeSegStart = 0xffffffff;
        trg_CodeSegSize = 0xffffffff;
        trg_LocDataSegStart = 0xffffffff;
        trg_LocDataSegSize = 0xffffffff;
        trg_GmIecOffs = 0xffffffff;
        trg_GmIecSize = 0xffffffff;
        trg_dgbSetting = 0xffffffff;
        trg_instAlignBytes = -1;
        trg_stackAlignBytes = -1;
        trg_bigEndian = -1;    // default is false (little endian)
        trg_minOperandSize = -1;
    }
};


typedef enum ASM_PAR_TYP
{
    ASM_PAR_UNDEF,
    ASM_PAR_L_OFF1,        //bit offset of FUN,FB,Prg local variable
    ASM_PAR_L_OFF8,        //byte offset of FUN,FB,Prg local variable

    ASM_PAR_G_OFF1,        //bit offset in %I,%Q,%M or global segment
    ASM_PAR_G_OFF8,        //byte offset in %I,%Q,%M or global segment

    ASM_PAR_OFFSETOF1,     //bit offset offsetof(struct,member)
    ASM_PAR_OFFSETOF8,     //byte offset offsetof(struct,member)
    
    ASM_PAR_SIZE1,         //a size in bits
    ASM_PAR_SIZE8,         //a size in bytes
    ASM_PAR_OBJID,         //obj index 
    ASM_PAR_FUNID,         //lib function id
    ASM_PAR_FBID,          //lib function block id
    ASM_PAR_CLSID,         //classid of FB,prg or fun
    ASM_PAR_LIT8,          //literal 8 
    ASM_PAR_LIT16,         //literal 16
    ASM_PAR_LIT32,         //literal 32
    ASM_PAR_LIT64,         //literal 64
    ASM_PAR_LITS8,         //literal string8

    //no binary params, needed as extra info:
    ASM_PAR_AIDX,          //for vector opcodes this is the index bit size
                           //the with prefix must be exactly this!
};



#define ASM_NO_PRFIX   0x0   //no prefix calculation, use as found in intOpcds.h
#define ASM_MAX_PRFIX  0x1   //prefix is max. size of all parametres

struct ASM_OPCDEF
{
    const char*    pszId;      //asm symbolic name of opcode
    unsigned short uscod;      //opcode without prefix
    unsigned short usFlags;    //processing flags 
    int            nPar;       //number of parameters
    ASM_PAR_TYP    parTyp[3];  //parameter type flags
};


class ASMGlobalContext_C
{
    static ASMGlobalContext_C* pTheGC;
    friend ASMGlobalContext_C* GetGC();

    FC_CStr     m_asmFileName;//as specified at command line (may or may not have a (rel)path)
    FC_CStr     m_memInfoFileName;
    FC_CStr     m_GenPath; //path for the generated files
    
    //values from .target section:
    ASM_TARGET_ATTRIBUTES m_trgAttr;
    long                  m_clsid_1st;        // first free class id 

    // list of all assembler opcodes
    FC_CHashStr<long>                   m_hashOpCodes;//TODO remove !
    FC_CHashStr<const ASM_OPCDEF*>      m_hashOPC;

    //list of parsed files:
    GArray<FileContext_C*> m_fileContext;

    //result of parseFile:
    ST_IdentBag                          m_identifiers;//store identifiers only once
    
    GArray<ASM_DATA_TYP*>                m_registeredTypes;
    ST_HashPointer<const ASM_DATA_TYP>   m_hashTypes;
    
    GArray<ASMPROC_TYP*>                 m_funList;
    ST_HashPointer<const ASMPROC_TYP>    m_hashFuns;

    GArray<ASMPROC_TYP*>                 m_prgFBList;
    ST_HashPointer<ASMPROC_TYP>          m_hashPrgFB;

    long                                 m_objectNr;
    GArray<ASM_OBJECT_TYP*>              m_objectList;  // list of all instances and arrays
    ST_HashPointer<ASM_OBJECT_TYP>       m_hashObjects;

    GArray<ASM_TASK_TYP*>                m_taskList;    // list of all tasks
    ST_HashPointer<ASM_TASK_TYP>         m_hashTasks;   // hash of all task key is task name
    FC_CHashStr<ASM_TASK_TYP*>           m_hashPrgInstTasks;   // hash of all prog instances in all tasks
    ASM_TASK_TYP*                        m_curTask;     // cur task being processed

    GArray<ASM_GLOB_COMP*>               m_prgInstList; //list of prog instances (order important!)
    GArray<ASM_GLOB_COMP*>               m_globComp;    //global,data & const variables (FBs are expanded)
    ST_HashPointer<ASM_GLOB_COMP>        m_hashGlobComp;

    ST_HashPointer<const TCHAR>          m_hashClasses; // map of class names
    FILE*                                m_pClsIdFile;  // file contains class id's

    FC_CGArray<ASM_CONFIG_DATA>          m_ConfigData;
    FC_CGArray<CASMCopySection*>         m_copySection;         

    // for initializing the VM
    FC_CGArray<const ASM_GLOB_COMP*>     m_iecGlobs;    // all CG global vars
    FC_CGArray<const ASM_GLOB_COMP*>     m_directSimpleVars; //simple variables with address

    FC_CGArray<CChannelInfo*>            m_channelList; //all .channel info (since V20213)


    // file containing all names of files to download
    FILE*                   m_DldFile;
    // COMMAND LINE OPTIONS
    bool                    m_bCreateMapFile; // create a map file. default: false
    
    ASM_BITOFF   m_ulBitOff_GM; //remove this var ...
    ASM_BITOFF   m_ulBitOff_GMR;//remove this var ...
    ASM_BYTESIZE m_usedGlobalRetainSize; // includes 16 bit allignment 
    ASM_BYTESIZE m_usedLocalRetainSize; // includes 16 bit allignment 
    ASM_BYTESIZE m_ulLocalDataSize;    
    unsigned long m_ulUsedGlobalSize;   //byte size of all global vars (simple type & struct types)
    unsigned long m_ulUsedGlobalSimpleSize;//byte size of all global simple vars (must be <=64k!)
    unsigned long m_ulTotalCodeBytes;
    long         m_lTotCopyRegionCount;

    //private functions:
    void dumpInitValues(char* pBinFile, unsigned long totalSize);
    void dumpCodeSegment(char* pBinFile, unsigned long totalSize);

    void dumpGlobalDefinitions();
    void dumpObjectHash();
    void dumpClassDefinitions();
    void dumpGlobalDef2Line(FILE* pFile, const ASM_GLOB_COMP* pGComp);
    void dumpArrayDefinitions();
    void dumpClassIds();
    void dumpArrayDefinition(FILE* pFile, const ASM_DATA_TYP* pType);
    void dumpCodeFiles();
    void dumpCodeIntoMapFile();
    void dumpDebugInfo()const;
    void dumpInitialValues();
    unsigned long dumpInitialArrayValues(
        FILE* pFile, 
        const ASM_OBJECT_TYP* pObj 
    );
    void dumpTaskData();
    bool dumpCmnHeader();
private:
    //helper functions for version dependet output format handling:
    void dumpXProject_213(FILE* pFile);
    void dumpXIOLayer(FILE* pFile);
    void dumpXProject_207(FILE* pFile);
    void dumpXProject_205(FILE* pFile);
    void dumpXProject_200(FILE* pFile);
    void dumpProjInfo(FILE* pFile);
public:
    bool dumpCallStack();
    // für unsere abdeckungstests
    void dumpAllUsedOpcodes();

    long GetNumberOfGlobalBoolean(ASM_SEGM_TYP segm);
    long getNumberOfClasses()const;
    
    void WriteUsedMemRegions(FILE*               pCommonHdr,
                             ASM_SEGM_TYP        segm,
                             const ASM_TASK_TYP* pTask);
    bool WriteGlobSegmentDesc(FILE* pCommonHdr);

    void dumpGCompInitValue(const ASM_GLOB_COMP* pGComp,
                            __int64              ChnOffset,
                            BYTE*                pInit,
                            unsigned long        ulSize)const;

    void DumpGlobalSegHdr2(
        FILE*        pFile,
        ASM_SEGM_TYP segm,
        long         Size
    );

    void buildDLFile();

public:
    // just to delete all newed init lists and literal lists
    FC_CGArray<FC_CGArray<ASM_INIT_REPEAT_VAL_TYP>*> m_ListOfInitLists;
    FC_CGArray<ASM_LITERAL_TYP*>                     m_ListOfLitLists;
    FC_CGArray<ST_IDENT_TYP*>                        m_listOfIdLists;
public:
    ~ASMGlobalContext_C();
    ASMGlobalContext_C();

    //commandline options:
    void parseCommandLine(int argc_in, TCHAR** argv_in);
    bool isMapFileCreated()const { return m_bCreateMapFile; }

    void DumpObjblockHeader(
        FILE*                 pFile,
        const ASM_OBJECT_TYP* pObj
    );


    void addTargetAttribute(const ST_EdiPos* pEdp, ST_IDENT_TYP key, const ASM_LITERAL_TYP& val);

    ST_IDENT_TYP registerId(const TCHAR* pszId){return m_identifiers.insert(pszId);}
    const TCHAR* registerStringLit(const TCHAR* pszId){return m_identifiers.insertString(pszId);}
    ASM_INST_PATH_TYP registerInstPathPlusMember(const ASM_INST_PATH_TYP* pPath,
                                                 ST_IDENT_TYP             member);
    ASM_INST_PATH_TYP registerInstArrayPath(const ASM_INST_PATH_TYP& path,
                                            const ASM_LITERAL_TYP*   pArrayIdx);

    ASM_INST_PATH_TYP registerInstPathPlusArrayMember(const ASM_INST_PATH_TYP* pPath,
                                                      ST_IDENT_TYP             member,
                                                      int                      Index);
    const ASM_DATA_TYP* registerBasicType(ASM_TYPE_ENUM type)
    {
        return m_registeredTypes.get(type);
    }

    const ASM_DATA_TYP* registerType(ASM_TYPE_ENUM type,
                                     ST_IDENT_TYP name, 
                                     const ASMPROC_TYP* pProcDef);
    const ASM_DATA_TYP* registerType(ASM_TYPE_ENUM type, 
                                     ST_IDENT_TYP name, 
                                     const ASMPROC_TYP* pProcDef,
                                     bool bArray,
                                     const ASM_LITERAL_TYP* pArrayDim, 
                                     const ASM_LITERAL_TYP* pStrLen);
    const ASM_GLOB_COMP* registerLiteral(const ASM_LITERAL_TYP& lit);

    long registerObject(const ASM_INST_PATH_TYP&    path,
                        const ASM_DATA_TYP*         pType,
                        const ASM_INITIALISER_TYP*  pInit,
                        ASM_SEGM_TYP                segm,
                        ASM_BITOFF                  bitOffs);


    void registerClassName(ST_IDENT_TYP ClassName, long ClsId);
    LPCTSTR getClassName(long ClsId)const;

    // PROCS
    void addProc(ASMPROC_TYP* pProc);
    
    // TASKS
    void addTaskProperties(const ST_EdiPos*       pEdp,
                           ST_IDENT_TYP           TaskIdent,
                           const ASM_LITERAL_TYP& TaskId,
                           const ASM_LITERAL_TYP& autoStart,
                           const ASM_LITERAL_TYP& intTime,
                           const ASM_LITERAL_TYP& Prio,
                           const ASM_LITERAL_TYP& Loaded,
                           const ASM_LITERAL_TYP& Events,
                           const ASM_LITERAL_TYP& WatchDog
    );
    void setActualTask(
        const ST_EdiPos* pEdp, 
        ST_IDENT_TYP*    name
    );
    void addTaskPrginstAssignment(
        const ST_EdiPos*  pEdp,
        ST_IDENT_TYP      prgInst
    );
    const ASM_TASK_TYP* getTaskByName(ST_IDENT_TYP taskName)
    {
        return m_hashTasks.get(taskName.psz);
    }
    ASM_TASK_TYP* getTaskByNameUC(ST_IDENT_TYP taskName)
    {
        return m_hashTasks.get(taskName.psz);
    }
    const ASM_TASK_TYP* getTaskOfPrgInst(const TCHAR* pszPrgInstName);
    const ASM_GLOB_COMP* lookUpPrgInst(ST_IDENT_TYP prgInst);
    
    // DECLS
    bool addGlobalDecl(
        const ST_EdiPos*           pEdp, 
        const ASM_DATA_TYP*        pType, 
        ST_IDENT_TYP               id,
        ASM_SEGM_TYP               segm,
        const ASM_DIRECT_TYP&      opt_addr,
        const ASM_INITIALISER_TYP& opt_init
    );
  
    void appendConfigData(const ST_EdiPos*           pEdp, 
                          const ASM_INST_PATH_TYP*   pPath, 
                          const ASM_INITIALISER_TYP& initVal);

    void addCopySection(CASMCopySection* pCpy);
    CASMCopySection* getCpyRegForRC()const;
    CASMCopySection* getCpyRegForTask(ST_IDENT_TYP taskName)const;

    //channel info (since 20213)
    void addChannel(unsigned long ulChannelNum)
    {
        CChannelInfo* pCh = new CChannelInfo(ulChannelNum);
        m_channelList.add(pCh);
    }
    void addChannelAttribute(
        const ST_EdiPos*       pEdp, 
        const TCHAR*           pszAttr, 
        const ASM_LITERAL_TYP* pVal
    );
    


    const ASMPROC_TYP* lookUpFunction(ST_IDENT_TYP name)const
    {
        return m_hashFuns.get(name.psz);    
    }
    const ASMPROC_TYP* lookUpPrgFB(ST_IDENT_TYP name)const
    {
        return m_hashPrgFB.get(name.psz);    
    }
    const ASM_GLOB_COMP* lookUpGlobalVar(ST_IDENT_TYP name)
    {
        return m_hashGlobComp.get(name.psz);
    }
    const ASM_GLOB_COMP* lookUpGlobalVar(const TCHAR* pszName)
    {
        return m_hashGlobComp.get(pszName);
    }
    const ASM_GLOB_COMP* lookUpGlobalComp(const ASM_INST_PATH_TYP& path)
    {
        return m_hashGlobComp.get(path.ids.psz);
    }
    const ASM_OBJECT_TYP* lookUpObject(ST_IDENT_TYP name)
    {
        return m_hashObjects.get(name.psz);
    }
    const ASM_OBJECT_TYP* lookUpObject(const TCHAR* pszName)//HACK remove
    {
        return m_hashObjects.get(pszName);
    }
    const ASM_OBJECT_TYP* lookUpObject(const ASM_INST_PATH_TYP& path)
    {
        return m_hashObjects.get(path.ids.psz);
    }
    ASM_OBJECT_TYP* lookUpObjectUC(const TCHAR* pszName) ////HACK remove unconst version of lookUpObject()
    {
        return m_hashObjects.get(pszName);
    }
    const ASM_TASK_TYP* lookUpTask(LPCTSTR name)
    {
        return m_hashTasks.get(name);
    }
    ASM_TASK_TYP* lookUpTaskUC(LPCTSTR name)    // unconst version of lookUpTask()
    {
        return m_hashTasks.get(name);
    }
    const ASM_TASK_TYP* findTaskOfPrgInst(LPCTSTR instName)
    {
        return m_hashPrgInstTasks.get(instName);
    }
    long getOpcode(LPCTSTR sOpc)
    {
        return m_hashOpCodes.get(sOpc);
    }
    const ASM_OPCDEF* getOpc(const TCHAR* pszId)
    {
        return m_hashOPC.get(pszId);
    }

    const ASM_TARGET_ATTRIBUTES* getTrgAttr()const { return &m_trgAttr; }

    void calcGlobObjectNumber(const ASM_GLOB_COMP* pGComp);

    void initOpCodes();
    void parseAsmFile();
    void insertSysGlobals();
    void calcGlobalOffsets();
    void calcMemberOffsets();
    void calcObjectNumbers();
    void calcOffsetLiterals();
    void calcConfigData();
    void calcCopySections();
    void calcInstructions();
    void checkSizes();
    void dumpDataAndCodeFiles();
    
    void dumpMapFile();

    void writeMemInfoDW(const TCHAR* pszSec, const TCHAR* pszKey, unsigned long dw)
    {
        TCHAR buffer[64];
        _ultot(dw, buffer, 10);
        WritePrivateProfileString(pszSec, pszKey, buffer, m_memInfoFileName);
    }
    void writeMemInfoStr(const TCHAR* pszSec, const TCHAR* pszKey, const TCHAR* pszText)
    {
        WritePrivateProfileString(pszSec, pszKey, pszText, m_memInfoFileName);
    }

    //last pass (if you like):
    void debugDumpHeader(FILE* pout)const;
    void dumpGCompList(FILE* pout, ASM_SEGM_TYP segmWanted)const;
    void debugDumpDataSegments(FILE* pout)const;
    void debugDumpCodeSegment(FILE* pout)const;
    void debugDumpAll(FILE* pout)const;

    // für unseren abdeckungstest
    const char* m_opcodes[1024*64];
};
ASMGlobalContext_C* GetGC();


// defines for output files:
#define GEN_DIR                     _T("gen\\")
#define FILE_EXT_MAPFILE            _T(".map")
#define FILE_MAPFILE                _T("mapfile.txt")
#define FILE_EXT_DATA_DEF           _T(".def2")
#define FILE_EXT_BINARY             _T(".bin")
#define FILE_EXT_DEBUGINFO          _T(".dbp")
#define FILE_EXT_INITDATA           _T(".init")
#define FILE_NAME_GLOBDATA_DEF      _T("__globals")
#define FILE_NAME_OBJECT_HASH       _T("__objects.hash")
#define FILE_NAME_DOMAIN_LIST       _T("__domain.list")
#define FILE_NAME_TASK_LIST         _T("__task.list")
#define FILE_NAME_LOADER_LIST       _T("loader.list")
#define FILE_NAME_COMMON_HDR        _T("__common.hdr")
#define FILE_NAME_CALL_STACK        _T("__callstack.bin")
#define FILE_NAME_ALL_CLASSES       _T("__classes")
#define FILE_NAME_ALL_OBJECTS       _T("__objects")
#define FILE_NAME_CLASSID_TXT       _T("__classid.txt")
#define FILE_NAME_USED_OPCODES      _T("__usedOpcodes.txt")

#define DL_FILE_SECTION_CODE        _T("code")
#define DL_FILE_SECTION_DEBUG       _T("debug")
#define DL_FILE_SECTION_CONFIG      _T("config")
#define DL_FILE_SECTION_INIT        _T("init")

#define TAG_TARGETTYPE              _T("targettype")
#define TAG_FIRMWAREVERS            _T("firmwareversion")

#define TAG_DBGSETTING              _T("dbgsetting")
#define TAG_ISEGSTART               _T("inputstart")
#define TAG_ISEGSIZE                _T("inputsize")
#define TAG_OSEGSTART               _T("ouputstart")
#define TAG_OSEGSIZE                _T("ouputsize")
#define TAG_GMSEGSTART              _T("gmsegstart")
#define TAG_GMSEGSIZE               _T("gmsegsize")
#define TAG_GMRSEGSTART             _T("gmrsegstart")
#define TAG_GMRSEGSIZE              _T("gmrsegsize")
#define TAG_CODESEGSTART            _T("codesegstart")
#define TAG_CODESEGSIZE             _T("codesegsize")
#define TAG_LOCDATASEGSTART         _T("locdatasegstart")
#define TAG_LOCDATASEGSIZE          _T("locdatasegsize")
#define TAG_GMIECOFFS               _T("gmiecoffs")
#define TAG_GMIECSIZE               _T("gmiecsize")

#define TAG_PI_VERSION              _T("projversion")
#define TAG_PI_NAME                 _T("projname")
#define TAG_PI_MODIFIED             _T("projmodified")
#define TAG_PI_CREATED              _T("projcreated")
#define TAG_PI_OWNER                _T("projowner")
#define TAG_PI_COMMENT1             _T("projcomment1")
#define TAG_PI_COMMENT2             _T("projcomment2")
#define TAG_PI_COMMENT3             _T("projcomment3")

#define CLSID_NONE                  -1

#endif

/* ---------------------------------------------------------------------------- */

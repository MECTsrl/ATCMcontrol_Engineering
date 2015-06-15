/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/asm/asmstmt.h 2     12.04.07 19:53 Ef $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: asmstmt.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/asm/asmstmt.h $
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

#ifndef _ASMSTMT_H_
#define _ASMSTMT_H_
 
#include "asmtools.h"
#include "asmtype.h"


enum ASM_INSTR_ENUM
{
    ASM_LABDEF,     //Label definition, does no operation
    ASM_VMOPC,
    ASM_BPOP,       //breakpoint op code, does BP instrumentation code
    ASM_STMTBEG,    //begin of a stmt code position, does no operation (for NCC only)
    ASM_ST_INFO,    //for debug only: contains the ST source code
    ASM_CALOP,      //call an intrinsic operator
    ASM_JMP,        //jmp
    ASM_JMPC,       //jmp if true
    ASM_JMPCN,      //jmp if false
    ASM_NOINSTR,    //no valid instr
};

enum ASM_PRIMARY_ENUM
{
    ASM_PRM_NONE,        //empty  
    ASM_PRM_LIT,         //literal argument: dint#45;
    ASM_PRM_SIZEOF1,     //bit size of any data type;
    ASM_PRM_SIZEOF8,     //byte size of any data type;
    ASM_PRM_AIDX,        //vector index type
    ASM_PRM_CLSID,       //clsid(<pouname>)
    ASM_PRM_FBID,        //libid(<sys/mlibfb>)
    ASM_PRM_FUNLOCBYTES, //funlocbytes(<funname>)
    ASM_PRM_LEAVEBYTES,  //leavebytes(<funname>)
    ASM_PRM_LOCALCOMP,   //symbolic local variable reference: aBool or aFB.Member
    ASM_PRM_GLOBCOMP,    //symbolic global variable refernce: prginst.aFB
    ASM_PRM_LABEL,       //label name
};

#pragma pack(1)
typedef struct
{
	BYTE CurLen;
	BYTE MaxLen;
	char Contents[256]; // maxlen = 255
} VM_STRING;
#pragma pack()


enum ASM_MEMRGN_TYP
{
    ASM_LIST_NONE = -1,
    ASM_WRITE_LIST,
    ASM_READ_LIST
};
struct ASM_MEM_REGION
{
    unsigned long     ulOffset;
    unsigned short    uSize;
};


#define ASM_MAX_DIRADDR_STRLEN 256
struct ASM_DIRECT_TYP
{
    ASM_SEGM_TYP   loc;      //in,out,status or undef if no direct address  
    char           bitsize;  //1,8,16,32,64
    bool           bSwap;    //true if must be swapped
    ASM_BITOFF     bitoff;   //the bit offset  
};
inline void ASM_InitDirectAddr(ASM_DIRECT_TYP* pAddr)
{
    pAddr->loc     = ASM_SEGM_UNDEF;
    pAddr->bitsize = 0;
    pAddr->bSwap   = false;
    pAddr->bitoff  = 0;
}
//parse the following: %[IQM][XBWDL]<bytes|bits>
bool ASM_ParseDirectAddr(const TCHAR* pszAddr, ASM_DIRECT_TYP* pResult);
const TCHAR* ASM_AddrToString(TCHAR buffer[ASM_MAX_DIRADDR_STRLEN], const ASM_DIRECT_TYP& addr);
bool ASM_CheckDirectAddUse(const ST_EdiPos*     pEdp, 
                           const ASM_DIRECT_TYP addr,
                           const ASM_DATA_TYP*  pType,
                           bool                 bHasInitVal);


struct ASM_LITERAL_TYP
{
    ASM_TYPE_ENUM type;      //ASM_TYPE_UNDEF if no literal 
    union
    {
        //if type is ASM_BOOl,BYTE,...LREAL:
        __int64           any64;        

        //if type is ASM_STRING:
        const char*       pszIECStr;    //pointer to null terminated string ASM_STRNG in IEC format
        
        //if type is ASM_OFFSET_LITERAL:
        struct                          
        {
            ST_IDENT_TYP  idStruct;     //NULL if offsetof(localvar) or <typename>
            ST_IDENT_TYP  idMember;     //never NULL offsetof(<typename>, <member>) 
        };
        
        //if type is ASM_OBJID_LITERAL:
        ST_IDENT_TYP      idObj;        //name of global struct/array, no '.' inside
    };
};
FC_CString& ASM_LitToString(FC_CString& text, const ASM_LITERAL_TYP* pLit);
long ASM_ForceDINT(const ST_EdiPos* pEdp, const ASM_LITERAL_TYP* pLit);
unsigned long ASM_ForceDWORD(const ST_EdiPos* pEdp, const ASM_LITERAL_TYP* pLit);
void ASM_ForceCString(const ST_EdiPos* pEdp, const ASM_LITERAL_TYP* pLit, FC_CString* pCstr);
FC_CString& ASM_AppendDefFileTypeName(FC_CString& dmp, const ASM_DATA_TYP* pType);


enum ASM_INIT_TYP
{
    ASM_INIT_LIT,           // init value is a simple literal
    ASM_INIT_LIST,          // list of INIT_VAL
    ASM_INIT_UNDEF = -1,    // invalid
};

struct ASM_INIT_REPEAT_VAL_TYP
{
    int                          nRepeat;   // according to ST: "... := [ nRepeat(lit[,lit]) ]"
    int                          nListSize; // always > 0
    ASM_LITERAL_TYP*             pLitList;  // pLitList has nListSize member
};

struct ASM_INITIALISER_TYP
{
    ASM_INIT_TYP initType;
    union
    {
        ASM_LITERAL_TYP                      lit;
        FC_CGArray<ASM_INIT_REPEAT_VAL_TYP>* pInitList;
    };
};


struct ASM_DECL_TYP
{
    ST_EdiPos           edp;        //line info
    ST_IDENT_TYP        id;         //name of variable
    ASM_DECL_SCOPE      scope;      //var, var_input,...
    const ASM_DATA_TYP* pType;      //data type of decl
    ASM_BITSIZE         declBitSize;//the size in bits allocated in the VM
                                    //this has nothing todo with the data type bit size!
                                    //Example if align is 32 bit a BOOL has 
    long                align;      //-1 or >0 the user defined alignment for this decl
    ASM_BITOFF          declBitOff; //ASM_INVALID_BITOFF or bit offset 
                                    //of component relative to struct begin
    ASM_INITIALISER_TYP initval;    //optional init value
    ASM_DIRECT_TYP      direct;     //optional direct address
    long                nCount;     //0..N ordinal number of decl in POU
};

inline void ASM_InitDeclTyp(ASM_DECL_TYP* pDecl)
{
    memset(pDecl, 0, sizeof(ASM_DECL_TYP));
}


typedef struct ASM_ALL_REGIONS_TYP
{
    FC_CGArray<ASM_MEM_REGION>   ISegMemRgnWR;
    FC_CGArray<ASM_MEM_REGION>   OSegMemRgnWR;
    FC_CGArray<ASM_MEM_REGION>   GSegMemRgnWR;
    FC_CGArray<ASM_MEM_REGION>   GRSegMemRgnWR;
    FC_CGArray<ASM_MEM_REGION>   ISegMemRgnRD;
    FC_CGArray<ASM_MEM_REGION>   OSegMemRgnRD;
    FC_CGArray<ASM_MEM_REGION>   GSegMemRgnRD;
    FC_CGArray<ASM_MEM_REGION>   GRSegMemRgnRD;
} ASM_ALL_REGIONS;

struct ASM_TASK_TYP
{
    ST_IDENT_TYP            name;          // name of task
    int                     taskId;        // id of task (id is downloaded to 4cvm
    int                     autoStart;     // id of task (id is downloaded to 4cvm
    __int64                 Prio;          // priority of task
    __int64                 intervalTime;  // interval time
    int                     Loaded;        // specify if the task is loaded or not
    __int64                 Events;        // specify events that could fire the task 
                                           //   it is incompatible with intervalTime
	bool					bCycleSet;	   // Cycle time specified for task
	bool					bEventSet;	   // Events defined for task

    __int64                 WatchDog;      // watchdog in ms compatible with all other flags
    GArray<ST_IDENT_TYP>    prgInstList;   // list of assigned prg instances
    ASM_ALL_REGIONS         MemRegions;    // list of all mem regions used 
};
inline void ASM_InitTaskTyp(ASM_TASK_TYP* pTask)
{
    pTask->name.psz = NULL;
    pTask->taskId = -1;
    pTask->Prio = -1;
    pTask->intervalTime = -1;
    pTask->autoStart = 0;       // is this default correct
    pTask->Loaded = 1;          
    pTask->Events = 0;          
    pTask->WatchDog = 0;

	pTask->bEventSet = FALSE;
	pTask->bCycleSet = FALSE;
}




// initialising of member variables
struct ASM_MEMBER_INIT
{
    const ASM_DECL_TYP* pDecl;
    ASM_INITIALISER_TYP initVal;
};

struct ASM_CONFIG_DATA
{
    ST_EdiPos           edp;       // line info
    ASM_INST_PATH_TYP   path;      // instance info
    ASM_INITIALISER_TYP initVal;   // init value
};

// ASM_GLOB_COMP represents a glob var or a prg.myFB.member1
struct ASM_GLOB_COMP
{
    ASM_SEGM_TYP            segm;
    ASM_INST_PATH_TYP       path;         //full path
    const ASM_DATA_TYP*     pType;        //data type
    ASM_INITIALISER_TYP     initval;      //optional init val
    ASM_BITOFF              bitOff;       //offset in segm (in bits!) or offset of instance 
                                          //data relative to segm
                                          //Note:
                                          //If this is a struct at %MB... then bitOff
                                          //will be also the final runtime offset in segm.
    unsigned long           bitSize;      //size in bits
                                          //Note:
                                          //If is Object then size is 16 bits
                                          //size of object is in ASM_PROC_TYP
    bool                    bSwap;        //if word|dword is !swap 
    bool                    bCGAddr;      //true if no user address
};
inline ASM_InitGlobComp(ASM_GLOB_COMP* pGlobComp)
{
    memset(pGlobComp, 0, sizeof(ASM_GLOB_COMP));
    pGlobComp->bitOff = ASM_INVALID_BITOFF;
    pGlobComp->bitSize = 0;
    pGlobComp->bCGAddr = true;
}

struct ASM_PRIMARY_TYP
{
    ASM_PRIMARY_ENUM     argkind;
    unsigned             argSize; //in bytes: 1,2,4 or 8
    union
    {
        ASM_LITERAL_TYP          lit;           //if argkind ASM_PRM_LIT
        ST_IDENT_TYP             label;         //if argkind ASM_PRM_LABEL
        struct                                  //if argkind ASM_RPM_LOCALCOMP
        {
            union
            {
                const ASM_GLOB_COMP* pGComp;    //if argkind ASM_RPM_GLOBCOMP
                const ASM_DECL_TYP*  pLocal;    //the decl of the last hierachy of path
            };
            ASM_INST_PATH_TYP    path;          //can be: "myTP.in" HACK remove !!
        };
        const ASM_DATA_TYP*      pType;         //ASM_PRM_SIZEOF1, ASM_PRM_SIZEOF8, ASM_PRM_AIDX
                                                //alsi if ASM_PRM_CLSID, then pType is a Fun,FB,Prg
        long                     lId;           //ASM_PRM_FBID
    };
};
bool ASM_IsSimpleType(const ASM_DATA_TYP* pType);
bool ASM_IsSimpleBasicType(ASM_TYPE_ENUM type);
bool ASM_IsObject(const ASM_DATA_TYP* pType);

struct ASM_OBJECT_TYP
{
    long                    Index;        //object index
    ASM_INST_PATH_TYP       path;         //full path
    const ASM_DATA_TYP*     pType;        //data type
    ASM_SEGM_TYP            segm;         //dest segm of obj.   
    ASM_BITSIZE             byteSize;     //size of object in bits (%8 should be 0!)
    bool                    bInit;        //set when it is initialised
    ASM_BITOFF              bitOffs;      //-1 if no USER address given for the object
                                          //else the bit offset relative to segm pointer
    FC_CGArray<ASM_MEMBER_INIT>          listOfMemberInitVal;
    FC_CGArray<
        FC_CGArray<ASM_INIT_REPEAT_VAL_TYP>*> ListOfListOfArrayInits;
};


long ASM_GetObjectID(ST_IDENT_TYP Ident, const ASM_INST_PATH_TYP* Path, const ASM_OBJECT_TYP** pObj, int ArrIdx = -1);
void ASM_ValToFile(BYTE* pMem, __int64 val, ASM_BITOFF Offs, size_t size);
void ASM_ValToMemBlock(BYTE*          pMem,
                       __int64        val,
                       ASM_BITOFF     Offs,
                       size_t         size,
                       __int64        limit);
void ASM_InitValToMemBlock(BYTE*                      pMem,
                           const ASM_INITIALISER_TYP* pInit,
                           ASM_BITOFF                 Offs,
                           size_t                     size,
                           __int64                    limit);
void ASM_BinStringToMemBlock(BYTE*                  pMem,
                             const ASM_DATA_TYP*    pType,
                             ASM_BITOFF             Offs,
                             const ASM_LITERAL_TYP* lit,
                             __int64                limit);
void ASM_InitBinStringToMemBlock(BYTE*                      pMem,
                                 const ASM_DATA_TYP*        pType,
                                 ASM_BITOFF                 Offs,
                                 const ASM_INITIALISER_TYP* pInit,
                                 __int64                    limit);

struct ASM_OPCDEF;
//defines a instruction e.g. jmp, calfb, calf
class ASM_INSTR_TYP
{
    ST_EdiPos               m_edp;     //line info
    ASM_INSTR_ENUM          m_instr;   //call, jmp...
    const ASM_OPCDEF*       m_popc;    //generic vm opc definition
    ASMPROC_TYP*            m_pProcDef;//definition of FB or function
    GArray<ASM_PRIMARY_TYP> m_args;    //list of arguments, if any. 
                                       //If ASM_CALL first arg is the FBI 
                                       //If ASM_LABDEF first arg is label name
    GArray<__int64>         m_binArgs; //list of binary arguments, if any. 
                                       //If ASM_CALL first arg is the FBI 
                                       //If ASM_LABDEF first arg is label name
    ASM_BYTEOFF             m_finalByteOff; //final offset in bytes
                                            //relative to binary file begin
    unsigned short          m_Size;     //size in bytes of the complete instruction line
    unsigned                m_Opcode;   //binary opcode

    void processVMOPC();
    void ProcessCalop();
    void ProcessBreakpointOPCode();

public:

    ASM_INSTR_TYP*          m_pNext;   //next instruction

    ASM_INSTR_TYP(
        const ST_EdiPos*       pEdp, 
        ASM_INSTR_ENUM         instr, 
        ASMPROC_TYP*           pProcDef,
        const ASM_OPCDEF*      popc,
        int                    nArgs,
        ASM_PRIMARY_TYP**      pArgs
     );
    ~ASM_INSTR_TYP();

    ASM_INSTR_ENUM      getKind()const{return m_instr;}
    const ST_IDENT_TYP* getLabel()const{return m_instr==ASM_LABDEF ? &m_args.get(0).label : NULL;}
    const ST_EdiPos*    getEdp()const{return &m_edp;}
    unsigned short      getSize()const{return m_Size;}
    const ASMPROC_TYP*  getProcDef()const{return m_pProcDef;}
    const TCHAR*        getOpCodeName();
    void resolveLabel(const ASMPROC_TYP* pMyProc);
    
    void calcOpCodeSize1();
    void calcOpCodeSize2();

    unsigned short getInstrByteSize()const { return m_Size; }
    void setFinalByteOffset(ASM_BYTEOFF off){assert(off>=0);m_finalByteOff = off;}
    ASM_BYTEOFF getFinalByteOffset()const{assert(m_finalByteOff>=0);return m_finalByteOff;}

    void dumpCode(char* pData, unsigned long byteDataSize)const;
    void DumpBinary(FILE* pFile);
    void dumpIntoMapFile(ASM_BYTEOFF offset);
    void DumpDebugInfo(FILE* pFile);

    // für unsere abdeckungstests
    void MarkUsedOpcode(const char** pArray);
};



//defines a build in or user defined .fun .fb .prg .struct
class ASMPROC_TYP
{
    ST_EdiPos                   m_edp;    //edipos of definition
    ASM_PROC_ENUM               m_kind;   //fun, fb or prg
    ST_IDENT_TYP                m_name;   //name of FB or function or Program
    long                        m_xopcode; //if -1 it must be a user defined proc

    GArray<ASM_DECL_TYP*>        m_decls; 
    ST_HashPointer<ASM_DECL_TYP> m_hashDecls; // hash of local decls

    ASM_INSTR_TYP*              m_firstInstr; 
    ASM_INSTR_TYP*              m_lastInstr;  
    unsigned long               m_CodeSize; // size of the binary code
    long                        m_ClassID;  // id of class


    ASM_BYTESIZE                m_nSizeBytes; //-1 if not already calculated
                                              //if fun: m_byteSize==m_nFunParamBytes+m_nFunLocalBytes  
    //for user functions only, 
    //Note: the fun m_nFun*Bytes sizes include all necessary alignment bytes.
    ASM_BYTESIZE                m_nFunRetValBytes;//ret val bytes that should be left on stack
    ASM_BYTESIZE                m_nFunParamBytes; //number of bytes of parameters including ret val
    ASM_BYTESIZE                m_nFunLocalBytes; //bytes for local vars

    FC_CHashStr<
        const ASM_INSTR_TYP*>   m_hashLabelInstr; // hash of labeled instr

    GArray<ASM_INSTR_TYP*>      m_BpPosList;// list of breakpoint postions

    FC_CHashPtr
        <const ASMPROC_TYP*, long> m_Called;   // list of all called classes

public:
    
    ASMPROC_TYP(const ST_EdiPos* pEdp, ASM_PROC_ENUM kind, ST_IDENT_TYP name, long opcode);
    ~ASMPROC_TYP();

    void addDecl(const ST_EdiPos*           pEdp, 
                 ST_IDENT_TYP               id, 
                 ASM_DECL_SCOPE             scope, 
                 const ASM_DATA_TYP*        pType,
                 const ASM_DIRECT_TYP&      opt_addr,
                 const ASM_INITIALISER_TYP& opt_init
    );

    const ASM_DECL_TYP* addTmpVar(const ST_EdiPos*  pEdp, 
                                  ST_IDENT_TYP      id,
                                  int               bitSize,
                                  bool              bIsString);

    void addInst(ASM_INSTR_TYP* pInst);
    void insertInst(ASM_INSTR_TYP* pPred, ASM_INSTR_TYP* pInst);

    const TCHAR* getName()const{return m_name.psz;}
    ST_IDENT_TYP getTypeIdent() const {return m_name;}
    ASM_PROC_ENUM getPouTyp()const{return m_kind;} 
    const TCHAR* getPouTypeString()const;
    const GArray<ASM_DECL_TYP*>* getDecls()const{return &m_decls;}
    const long getClassId() const { return m_ClassID; }
    const ST_EdiPos* getEdp()const{return &m_edp;}
    const ASM_DECL_TYP* lookUpLocalVar(ST_IDENT_TYP varName)const
    {
        return m_hashDecls.get(varName.psz);
    }
    const ASM_DECL_TYP* lookUpLocalVar(const TCHAR* pszVarName)const
    {
        return m_hashDecls.get(pszVarName);
    }
    const ASM_DECL_TYP* getDeclByIndex(int idx)const
    {
        if(idx>=0 && idx<m_decls.getMaxUsed())
            return m_decls.get(idx);
        return NULL;
    }
    void insertCalledClass(const ASMPROC_TYP* pClass)
    {
        m_Called.insert(pClass, 0);
    }
    void buildCallStack(FC_CHashPtr<long, long>* listOfCalled)const;

    bool isBuildIn()const{return m_xopcode>=0;};
    long getOpCode()const{return m_xopcode;}

    ASM_BYTESIZE getByteSize()const
    {
        assert(m_nSizeBytes>=0); 
        return m_nSizeBytes;
    }
    ASM_BYTESIZE getFunLocalBytes()const
    {
        assert(m_nFunLocalBytes>=0);
        return m_nFunLocalBytes;
    }
    ASM_BYTESIZE getFunRetValBytes()const
    {
        assert(m_nFunRetValBytes>=0);
        return m_nFunRetValBytes;
    }
    ASM_BYTESIZE getFunParamBytes()const
    {
        assert(m_nFunParamBytes>=0);
        return m_nFunParamBytes;
    }
    ASM_BYTESIZE getFunLeaveBytes()const
    {
        assert(m_nFunLocalBytes>=0 && m_nFunParamBytes>=0 && m_nFunRetValBytes>=0);
        assert(m_nFunLocalBytes + m_nFunParamBytes == m_nSizeBytes);
        return m_nFunLocalBytes + m_nFunParamBytes - m_nFunRetValBytes;
    }

    unsigned long getCodeSize()const{ return m_CodeSize; }

    const ASM_INSTR_TYP* getLabelInstr(ST_IDENT_TYP label)const
    {
        return m_hashLabelInstr.get(label.psz);
    }
    bool insertLabel(const ASM_INSTR_TYP* pLabelInst);


    void insertBpPos(ASM_INSTR_TYP* curInst)
    {
        m_BpPosList.add(curInst);
    }

    void calcObjectNumOfNested(
        const ASM_INST_PATH_TYP&   InstPath,
        const ASM_DATA_TYP*        pType,
        ASM_SEGM_TYP               segm
    )const;
    void calcMemberOffsets();
    void calcFunMemberOffsets();
    void calcOffsetLiterals()const;
    void calcInstructions();
    void dumpPrgFBClassDefinition(FILE* pFile)const;
    void dumpCode(FILE* pFile)const;
    void dumpCodeIntoMapFile()const;
    void dumpDebugInfo(FILE* pFile)const;
    unsigned long dumpInitialValues(
        FILE*                 pFile,
        const ASM_OBJECT_TYP* pObj
    )const;
    void ProcessDeclInitValue(const ASM_DECL_TYP*        pDecl,
                              const ASM_INITIALISER_TYP* pInitval,
                              const ASM_INST_PATH_TYP*   pObjPath,
                              BYTE*                      pInit,
                              unsigned long              ulSize)const;

    void assignClassID(long ClassID) { m_ClassID = ClassID; }

    long m_alignNextDecl; //HACK but simply public.
                          //gets set to -1 in every call to addDecl

    // für unsere abdeckungstests
    void dumpUsedOpcodes(const char** pArray)const;

private:
    void DumpCodeblockHeader(FILE* pFile)const;
};


class CChannelInfo
{
public:
    unsigned char m_usChannel; /* Corresponding Channel number     */
    FC_CString    m_szName;    /* IO layer type (fcdb, bcbac, ...) */
    unsigned long m_ulIOffs;   /* Offset of input (%I) channel     */
    unsigned long m_ulISize;   /* Size of input (%I) channel       */
    unsigned long m_ulQOffs;   /* Offset of output (%Q) channel    */
    unsigned long m_ulQSize;   /* Size of output (%Q) channel      */
    unsigned long m_ulMOffs;   /* Offset of memory (%M) channel    */
    unsigned long m_ulMSize;   /* Size of memory (%M) channel      */

    CChannelInfo(unsigned long usChannel)
    {
        m_usChannel = (unsigned char)usChannel; //TODO check range
        m_ulIOffs = m_ulISize = 0;
        m_ulQOffs = m_ulQSize = 0;
        m_ulMOffs = m_ulMSize = 0;
    }
   
};

#endif

/* ---------------------------------------------------------------------------- */

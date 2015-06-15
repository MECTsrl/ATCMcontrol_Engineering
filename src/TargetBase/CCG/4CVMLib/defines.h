/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/defines.h 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: defines.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/defines.h $
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

#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <tchar.h>


#define MODULE_NAME             _T("4cvm")   //name of this back end exe file
#define CHECK_MEMORY            /* delete new char */

#define PC_TRG_LOADER_LIST      _T("loader.list")
#define PC_RETAIN_ITEM_FILE     _T("__retain.list")
#define PC_TARGET_INIT_FILE     _T("__.init")
#define PC_TASK_FILE_PRFIX      _T("__")

#define FILE_NAME_DOMAIN_LIST   _T("__domain.list")
#define FILE_NAME_CUSTOM_BIN    _T("__custdl.bin")
#define DL_SECTION_CUSTOM       _T("custom")

#define CG_VM_ASM_TEXT_EXT      _T(".4asm")

#define FILE_NAME_OSAI_DL_FILE_FRGR_TASK    _T("fore.bin")
#define FILE_NAME_COMMON_HDR    _T("__common.hdr")
#define FILE_NAME_ALL_CLASSES   _T("__classes")
#define FILE_NAME_ALL_OBJECTS   _T("__objects")
#define FILE_NAME_CALL_STACK    _T("__callstack.bin")
#define FILE_EXT_BINARY         _T(".bin")
#define FILE_EXT_INITDATA       _T(".init")


// SFC
#define SFC_DOTRANSACT          _T("__dotransact")



//code of Libs can be reused if Major and minor version is same
//a patch or service pack is not treated as incompatible
//In general the binary code version string is hard coded here
//and not related to the build index.
#define BIN_CODE_VERSION _T("1.20")  //TODO update to 2.00, für Messe erst mal alt 1.20

// options in the 4CP file
#define CG_4CP_ATTR_OPT     _T("opt")

// options in the class files
#define CG_VM_CLASS_OPT_BP  _T("bpopt")
// task attributes
#define TASK_ATTR_INTERVAL   _T("INTERVAL")
#define TASK_ATTR_AUTOSTART  _T("AUTOSTART")
#define TASK_ATTR_PRIO       _T("PRIORITY")
#define TASK_ATTR_LOADED     _T("LOADED")
#define TASK_ATTR_EVENTS     _T("EVENT")
#define TASK_ATTR_WATCHDOG   _T("WATCHDOG")

#define CRS                         _T("\n")

// jump label prefixes:
#define CG_VM_LABEL_L               _T("@L")
#define CG_VM_LABEL_LOOP            _T("@loop")
#define CG_VM_LABEL_READY           _T("@ready")
// special jump label used once inside body:
#define CG_VM_LABEL_POUEND          _T("@__end")
#define VM_LABEL_ACTION_RETURN      _T("@__action_return") 


#define CG_VM_MAX_VM_ADDR_LEN       256

// used in library definitions
#define CG_VM_VMOP                  _T("VMOP")
#define CG_VM_NOOP                  _T("NOOP")
#define CG_VM_EXTLIB                _T("EXTLIB")
#define CG_VM_STDLIB                _T("STDLIB")
#define CG_VM_LIBFUNFB_IDFILE       _T("FunFbId.txt")


// tags for cg-asm interface
#define CG_VM_ASM_TARGETTYPE        _T("targettype       ")
#define CG_VM_ASM_FIRMWAREVERS      _T("firmwareversion  ")
#define CG_VM_ASM_DGBSETTING        _T("dbgsetting       ")
#define CG_VM_ASM_GMIECOFFS         _T("gmiecoffs        ")
#define CG_VM_ASM_GMIECSIZE         _T("gmiecsize        ")
#define CG_VM_ASM_ISEGSTART         _T("inputstart       ")
#define CG_VM_ASM_ISEGSIZE          _T("inputsize        ")
#define CG_VM_ASM_OSEGSTART         _T("ouputstart       ")
#define CG_VM_ASM_OSEGSIZE          _T("ouputsize        ")
#define CG_VM_ASM_GMSEGSTART        _T("gmsegstart       ")
#define CG_VM_ASM_GMSEGSIZE         _T("gmsegsize        ")
#define CG_VM_ASM_GMRSEGSTART       _T("gmrsegstart      ")
#define CG_VM_ASM_GMRSEGSIZE        _T("gmrsegsize       ")
#define CG_VM_ASM_CODESEGSTART      _T("codesegstart     ")
#define CG_VM_ASM_CODESEGSIZE       _T("codesegsize      ")
#define CG_VM_ASM_LOCDATASEGSTART   _T("locdatasegstart  ")
#define CG_VM_ASM_LOCDATASEGSIZE    _T("locdatasegsize   ")
#define CG_VM_ASM_IECTIMESIZE       _T("iectimesize      ")
#define CG_VM_ASM_MAXOBJECTS        _T("maxobjects       ")
#define CG_VM_ASM_GMICHOFFSET       _T("gmichanneloffs")
#define CG_VM_ASM_GMOCHOFFSET       _T("gmochanneloffs")
#define CG_VM_ASM_GMCHOFFSET        _T("gmchanneloffs")
#define CG_VM_ASM_GMRCHOFFSET       _T("gmrchanneloffs")
#define CG_VM_ASM_GMICHSIZE         _T("gmichannelsize")
#define CG_VM_ASM_GMOCHSIZE         _T("gmochannelsize")
#define CG_VM_ASM_GMCHSIZE          _T("gmchannelsize")
#define CG_VM_ASM_GMRCHSIZE         _T("gmrchannelsize")
#define CG_VM_ASM_GMICHINIT         _T("gmichannelinit")
#define CG_VM_ASM_GMOCHINIT         _T("gmochannelinit")
#define CG_VM_ASM_GMCHINIT          _T("gmchannelinit")
#define CG_VM_ASM_GMRCHINIT         _T("gmrchannelinit")
#define CG_VM_ASM_PI_VERSION        _T("projversion     ")
#define CG_VM_ASM_PI_NAME           _T("projname        ")
#define CG_VM_ASM_PI_MODIFIED       _T("projmodified    ")
#define CG_VM_ASM_PI_CREATED        _T("projcreated     ")
#define CG_VM_ASM_PI_OWNER          _T("projowner       ")
#define CG_VM_ASM_PI_COMMENT1       _T("projcomment1    ")
#define CG_VM_ASM_PI_COMMENT2       _T("projcomment2    ")
#define CG_VM_ASM_PI_COMMENT3       _T("projcomment3    ")




enum VM_SEGM_TYP
{
    VM_SEGM_UNDEF   =-1,
    VM_SEGM_INPUT   = 0, //in VM  SEG_INPUT
    VM_SEGM_OUTPUT  = 1, //in VM  SEG_OUTPUT
    VM_SEGM_GLOBAL  = 2, //in VM  SEG_GLOBAL
    VM_SEGM_RETAIN  = 3, //in VM  SEG_RETAIN
    VM_SEGM_INST    = 4, //in VM  SEG_LOCAL
};

typedef struct _CG_SymEntry      CG_SymEntry;
class VM_Address
{
public:
    const CG_SymEntry*  m_pEntry;
    unsigned long       ulByteOffs;//byteoffset in VM segment including any channel offset
    unsigned char       ucBitPos;  //bit position inside byte <segment base ptr>[ulByteOffs]
    char                chSize;    //X,B,W,D,L,S
    int                 bitSize;
    VM_SEGM_TYP         vmSegEnum; //segment enum
    unsigned long       ulChOff;   //channel offset in segment.
    long                lChannel;  //channel of the VM
    VM_Address(const CG_SymEntry* pEntry)
    {
        m_pEntry = pEntry;
        ulByteOffs=0;
        ucBitPos  =0;
        chSize    =0;
        bitSize   =0;
        vmSegEnum =VM_SEGM_UNDEF;
        ulChOff   =0;
        lChannel  =0;
    }
};

struct VM_MemRegion
{
    unsigned long   ulOffset;   // offset of the mem region
    unsigned long   ulSize;     // byte size of the mem region
};

// type of address offset
#define INVAL_OFFSET  0xffffffff
typedef unsigned long VM_ADR_OFF;

// bitset for VM segments
typedef unsigned long VM_BITSET;
#define VM_BS_SEGM_UNDEF    0x00000000ul
#define VM_BS_SEGM_INPUT    0x00000001ul
#define VM_BS_SEGM_OUTPUT   0x00000002ul
#define VM_BS_SEGM_GLOBAL   0x00000004ul
#define VM_BS_SEGM_RETAIN   0x00000008ul

// name of the VM segments
#define VM_SEGNAME_INPUT    _T("INPUT")
#define VM_SEGNAME_OUTPUT   _T("OUTPUT")
#define VM_SEGNAME_GLOBAL   _T("GLOBAL")
#define VM_SEGNAME_RETAIN   _T("RETAIN")

#endif

/* ---------------------------------------------------------------------------- */

/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/parseKAD.h 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: parseKAD.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/parseKAD.h $
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

#ifndef _PARSEKAD_H_
#define _PARSEKAD_H_


//----  Aggregate Includes:   --------------------------------------*
#include <limits.h>
#include "fc_todebug\fc_assert.h"
#include "fc_tools\fc_cgarray.h"
#include "fc_tools\fc_cstring.h"
#include "CG_IECFront.h"
#include "defines.h"

class CGT_CDiagMsg;

struct VM_SETTINGS
{
    int         bigEndian;          //0 little, 1 big, -1 not set.
    int         timeSizeRes;        // be 32 or 64 bit
    int         instAlignBytes;     //max instance data alignment
    int         stackAlignBytes;    //minimum stack align
    int         opMinSize;          //min. operand size of the target in bytes

    unsigned long ulMaxObjects;     // max number of instance objects (include ARRAY of STRUCT)
    unsigned long ulMaxCodeObjects; // max code objects


    FC_CString  szISegStart;        // start addr of input segment
    FC_CString  szOSegStart;        // start addr of output segment
    FC_CString  szGMSegStart;       // start addr of global segment
    FC_CString  szGMRSegStart;      // start addr of retain segment
    FC_CString  szCodeSegStart;     // start addr of code segment
    FC_CString  szLocDataSegStart;  // start addr of local data segment

    FC_CString    szISegSize;       // size of input segment
    unsigned long ulISegSize;       // for faster access

    FC_CString    szOSegSize;       // size of output segment
    unsigned long ulOSegSize;       // for faster access

    FC_CString    szGMSegSize;      // size of %M segment
    unsigned long ulGMSegSize;      // for faster access

    FC_CString  szGMRSegSize;       // size of retain segment

    FC_CString  szCodeSegSize;      // size of code segment
    FC_CString  szLocDataSegSize;   // size of local data segment
    FC_CString  szGMiecSize;        // size of global IEC variables

    FC_CString  szGMiecOffs;        // offset of global IEC variables relative to szGMSegStart
    unsigned long ulGMiecOffset;    // for faster access
};
struct CG_OPTIONS
{
    FC_CString  szPostCompileExe;
    FC_CString  szPostCall;
    FC_CString  szAutoDeclIniFile;
    GUID        clsidAutoDecl;
    bool        bMemInfo;            //dumps mem info to stdout
	CG_OPTIONS()
	{
		memset(&clsidAutoDecl, 0, sizeof(clsidAutoDecl));
		bMemInfo = false;
	}
};




//CHANNEL_TYP holds the KAD info of a <CHANNEL> definition:
struct CHANNEL_TYP
{
    //lChannelNum used if !bFBUS:
    long          lChannelNum;         // channel num: 
                                       // -1 if defined by user (only if also bUseConfigurator==true)
                                       // -2 if not used at all (only if also bUseConfigurator==true and at most one channel)
    bool          bInitialise;         // channel data will be initialised

    FC_CString    szExt;               // extension of the field bus file or "IO type name" if no configurator
    bool          bUseConfigurator;    // TRUE if CLSID given
    GUID          clsidFBUS;           // classid of the ICompile interface if any


    //all XXOff & XXSize members are set to 0 if not specified in KAD, 0 means "no I Q or M" 
    unsigned long ulISegOffs;          // offset for INPUT variables
    unsigned long ulIChnSize;          // mem size for INPUT variables

    unsigned long ulOSegOffs;          // offset for OUTPUT variables
    unsigned long ulOChnSize;          // mem size for OUTPUT variables

    unsigned long ulGMSegOffs;         // offset for global variables
    unsigned long ulGMChnSize;         // mem size for global variables

    CHANNEL_TYP()
    {
        lChannelNum = -1;
        bInitialise = false;
        bUseConfigurator = false;
        memset(&clsidFBUS, 0, sizeof(clsidFBUS));
        ulISegOffs = ulOSegOffs = ulGMSegOffs = 0;
        ulIChnSize = ulOChnSize = ulGMChnSize = 0;
    }


    //usefull helper:
    unsigned long getOff(char cIQM)const
    {
       if(cIQM=='I') return ulISegOffs;
       if(cIQM=='Q') return ulOSegOffs;
       if(cIQM=='M') return ulGMSegOffs;
       assert(!"bad cIQM");
       return 0x0;
    }
    unsigned long getSize(char cIQM)const
    {
       if(cIQM=='I') return ulIChnSize;
       if(cIQM=='Q') return ulOChnSize;
       if(cIQM=='M') return ulGMChnSize;
       assert(!"bad cIQM");
       return 0x0;
    }
};


class CKADInfo
{
    FC_CStr                  m_szKADFile;   //full path and file name of 4cg.kad
    CG_Edipos                m_KADEdp;      //full path and file name of 4cg.kad line=1, col=0
    FC_CGArray<CHANNEL_TYP*> m_channels;    //list of <FBUS> and <CHANNEL> definitions
    VM_SETTINGS              m_VmSettings;  //settings of the 4CVM
    CG_OPTIONS               m_CgOptions;   //options for the code generator

    // type and language info
    CG_BITSET                m_AllowedIECtypes;
    CG_BITSET                m_AllowedArrayIndexIECtypes;
    FC_CString               m_AllowedIEClanguages;
    bool                     m_CheckAddressSpace;    //if true check overlapping %-addr use
    VM_BITSET                m_useTIforSegm;         // TI: TaskImage
    VM_BITSET                m_optimizeTIforSegm;    // TI: TaskImage
    long                     m_lMaxRegionsWR[VM_SEGM_INST+1];
    long                     m_lMaxRegionsRD[VM_SEGM_INST+1];
    
    long                     m_lMaxTasks;    // maximum number of tasks suported by the target
                                             // -1 if no maximum
    bool                     m_bLocalRetain; //bool Attr LOCAL_RETAIN (default=false)

    FC_CString               m_listOfFBUSTypes;//for better diags "fcdp, fcbc, fcmodbus"

    void clear();
public:
    CKADInfo() {clear();}
    ~CKADInfo(){clear();}

    bool load(
        CGT_CDiagMsg* pdm,
        const TCHAR*  pszKADFileAndPath,
    	const TCHAR*  pszTargetType,
        unsigned long ulTrgVer
    );

    //access funs:
    const VM_SETTINGS* getVmSettings()        { return &m_VmSettings; }
    const CG_OPTIONS*  getCgOptions()const    { return &m_CgOptions;  }
    CG_OPTIONS*        getCgOptionsWrt()      { return &m_CgOptions;  }
    void               disableProcessImage()  { m_useTIforSegm &= ~(VM_BS_SEGM_INPUT|VM_BS_SEGM_OUTPUT);}

    // type, segment and language info:
    CG_BITSET   getIECTypes()const            { return m_AllowedIECtypes; }
    bool        getCheckAddressSpace()const   { return m_CheckAddressSpace; }
    CG_BITSET   getArrIdxIECTypes()const      { return m_AllowedArrayIndexIECtypes; }
    LPCTSTR     getIECLanguages()const        { return m_AllowedIEClanguages; }
    VM_BITSET   getUseTIforSegm()const        { return m_useTIforSegm; } // TI: TaskImage
    VM_BITSET   getOptimizeTIforSegmBS()const { return m_optimizeTIforSegm; } // BS: BitSet
    const long* getMaxMemRgnsWR()const        { return m_lMaxRegionsWR; }
    const long* getMaxMemRgnsRD()const        { return m_lMaxRegionsRD; }
    long        getMaxTasks()const            { return m_lMaxTasks; }
    bool        getLocalRetain()const         { return m_bLocalRetain; }
    bool        isLoaded()const               {return !m_szKADFile.isEmpty();}
    const TCHAR* getFile(void)const           {return m_szKADFile;}
    const       FC_CGArray<CHANNEL_TYP*>* getChannels()const{ return &m_channels;}
    const CHANNEL_TYP* getChannelConfiguratorByExt(const TCHAR* pszExt, bool* pbMultible)const;
    const CHANNEL_TYP* getChannelInfoByChannelNum(int nChNum)const;
    const TCHAR* getListOfFBUSTypes(); 
    FC_CStr getChannelNumberForExt(const TCHAR* pszExt)const;
};

#endif

/* ---------------------------------------------------------------------------- */


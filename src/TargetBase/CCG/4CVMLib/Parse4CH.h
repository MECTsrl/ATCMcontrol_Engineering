/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/Parse4CH.h 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: Parse4CH.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/Parse4CH.h $
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

#ifndef _PARSE4CH_H_
#define _PARSE4CH_H_

#include "fc_tools\fc_cgarray.h"
#include "fc_tools\fc_cstring.h"
#include "fc_tools\fc_hash.h"

#include "io_4cvm_config.h"
#include "parseKAD.h"

#include "defines.h"


class CTarget;
class CGlobCtx;

class CChannelBase
{
public:
    //all these members are set in derived classes:
    long          m_channel;            //the first digit of %-Addr
    FC_CStr       m_ext;                //from KAD
                                        //string is the channel number
    bool          m_bInitialise;        // channel data will be initialised

    unsigned long m_ulISegOffs;         // offset for INPUT variables
    unsigned long m_ulIChnSize;         // mem size for INPUT variables

    unsigned long m_ulOSegOffs;         // offset for OUTPUT variables
    unsigned long m_ulOChnSize;         // mem size for OUTPUT variables

    unsigned long m_ulGMSegOffs;        // offset for global variables
    unsigned long m_ulGMChnSize;        // mem size for global variables


    CChannelBase()
    {
        m_channel = -1;
        m_bInitialise = false;
        m_ulISegOffs = m_ulOSegOffs = m_ulGMSegOffs = 0;
        m_ulIChnSize = m_ulOChnSize = m_ulGMChnSize = 0;
    }
    virtual ~CChannelBase(){}

    virtual bool checkAddr(
        CGlobCtx*		 pgctx,
        CGT_CDiagMsg*    pdm,
        const CG_Edipos* pEdp,
        const TCHAR*     pszIECAddr,
        const CG_DType*  pType,
        const TCHAR*     pszInstPath,
        VM_Address*      pVMAddr
    )const = 0;


    //usefull helper:
    unsigned long getOff(char cIQM)const
    {
       if(cIQM=='I') return m_ulISegOffs;
       if(cIQM=='Q') return m_ulOSegOffs;
       if(cIQM=='M') return m_ulGMSegOffs;
       assert(!"bad cIQM");
       return 0x0;
    }
    unsigned long getSize(char cIQM)const
    {
       if(cIQM=='I') return m_ulIChnSize;
       if(cIQM=='Q') return m_ulOChnSize;
       if(cIQM=='M') return m_ulGMChnSize;
       assert(!"bad cIQM");
       return 0x0;
    }
    VM_SEGM_TYP getVMSegm(char cIQM)const
    {
       if(cIQM=='I') return VM_SEGM_INPUT;
       if(cIQM=='Q') return VM_SEGM_OUTPUT;
       if(cIQM=='M') return VM_SEGM_GLOBAL;
       assert(!"bad cIQM");
       return VM_SEGM_UNDEF;
    }


};



class CChannelConfig : public CChannelBase
{
    FC_CStr                m_masterFile;//project relative FBUS data file
    FC_CStr                m_masterName;//file base name no path no ext
    const TCHAR*           m_pszIOTyp;  //points to the file ext. of m_masterFile 
    CG_Edipos              m_edp;       //edp of m_masterFile line=1, col=0
    //next members set in CIOComponent::compile():
    I4CVMCompile2*         m_pICompile; 
public:

    CChannelConfig(const TCHAR* pszMasterFile);
    virtual ~CChannelConfig();

    //compile() must be called once if this channel has a configurator
    bool compile( 
        CGlobCtx*      pgctx,
        const CTarget* pTrg
    );
    virtual bool checkAddr(
        CGlobCtx*		 pgctx,
        CGT_CDiagMsg*    pdm,
        const CG_Edipos* pEdp,
        const TCHAR*     pszIECAddr,
        const CG_DType*  pType,
        const TCHAR*     pszInstPath,
        VM_Address*      pVMAddr
    )const;

    const char* getMasterFile()const{return m_masterFile;}
};

class CChannelNoConfig : public CChannelBase
{
public:
    CChannelNoConfig(CHANNEL_TYP* pChannel)
    {
        assert(!pChannel->bUseConfigurator);
        //only copy the values from KAD to members of the base class:
        m_channel         = pChannel->lChannelNum;
        m_ext.load(pChannel->szExt);

        m_bInitialise     = pChannel->bInitialise;

        m_ulISegOffs      = pChannel->ulISegOffs;
        m_ulIChnSize      = pChannel->ulIChnSize;

        m_ulOSegOffs      = pChannel->ulOSegOffs;
        m_ulOChnSize      = pChannel->ulOChnSize;

        m_ulGMSegOffs     = pChannel->ulGMSegOffs;
        m_ulGMChnSize     = pChannel->ulGMChnSize;
    }

    virtual bool checkAddr(
        CGlobCtx*		 pgctx,
        CGT_CDiagMsg*    pdm,
        const CG_Edipos* pEdp,
        const TCHAR*     pszIECAddr,
        const CG_DType*  pType,
        const TCHAR*     pszInstPath,
        VM_Address*      pVMAddr
    )const;
};



class CTarget
{
    //initialised by loadTarget:
    CG_Edipos       m_edp;     //edipos to submitt 4ch located diags (just the file name, no line info)
    FC_CStr         m_trgFile; //project relative target definition file name
    FC_CStr         m_trgName; //name of target (the target definition file base name)
    FC_CStr         m_trgType; //target type (always lowercase)
    unsigned long   m_ulTrgVer;//target firmware version from connect string (always lowercase)
    FC_CStr         m_resName; //opt. assigned resource
    FC_CStr         m_control; //opt. control's address
    FC_CStr         m_console; //opt. control's address

    // for project info
    FC_CStr         m_strPIVersion;  // version of the compiler used
    FC_CStr         m_strPIModified; // Modified date of the target file
    FC_CStr         m_strPICreated;  // Creation date of the target file
    FC_CStr         m_strPIOwner;    // Owner of the target file
    FC_CStr         m_strPIComment1; // User text of the target file
    FC_CStr         m_strPIComment2;
    FC_CStr         m_strPIComment3;
    
    FC_CStrList     m_FBUSConfigFiles; //list of all <FBUS ..> nodes
                                       //of the target definition file. This are 
                                       //the IOComponents instatated in this target.
    FC_CStr         m_CDownloadDir;    // Download directory for custom download (full or prj dir relative)
    FC_CStr         m_trgGenPath;      //full path to gen\<target name>
    
    //initialised by  CGlobCtx::loadResources():
    const CG_Resource*  m_pres; //may be NULL after error.
    
    long              m_channelArraySize;//set during compile(), the highest channel number
    CChannelBase**    m_ppChannelArray;//set during CTarget::compile(), 
                                       //contains all <FBUS> from m_FBUSmasters
                                       //and all <CHANNELS> from KAD. Size is 
                                       //m_channelArraySize

    // license stuff: we count the number of I/Os : starter KIT allows 
    // only 6 boolean I/O
    int       m_iNumIOUsed;
    bool      m_bIOErrMsgAlreadyDsp;
    bool      m_bDEMOErrMsgAlreadyDsp;

public:
    FC_CString m_paramList;
    CTarget();
    ~CTarget();
    
    /////////////////////////////////////////////////////////////////////
    /**
     * loadTarget reads a target definition files's basic info, but does not 
     * call the compile methods of the contained IO-masters.
     *
     * @param pgctx       the global context
     * @param pszTrgFile  the target definition file, full 
     *                    or prj dir relative path
     * @return            true if no errors
     */
    bool loadTarget(
        CGlobCtx*    pgctx,
        const TCHAR* pszTrgFile 
    );

    bool compile(
        CGT_CDiagMsg* pdm,
        CGlobCtx*     pgctx
    );

    
    //get basic info:
    const CG_Edipos* getEdp()const{return &m_edp;}
    const TCHAR* getTrgFile()const{return m_trgFile;}
    const TCHAR* getTrgName()const{return m_trgName;}
    const TCHAR* getTrgType()const{return m_trgType;}
    unsigned long getTrgVer()const{return m_ulTrgVer;}

    bool         hasResName()const{return !m_resName.isEmpty();}
    bool         hasControl()const{return !m_control.isEmpty();}
    bool         hasConsole()const{return !m_console.isEmpty();}
    
    const TCHAR* getResName()const{return m_resName;}
    const TCHAR* getControl()const{return m_control;}
    const TCHAR* getConsole()const{return m_console;}

    const TCHAR* getTrgGenPath()const   {return m_trgGenPath;}

    const TCHAR* getCDownloadDir()const {return m_CDownloadDir;}

    const TCHAR* getPIVersion()const  {return m_strPIVersion;}
    const TCHAR* getPIModified()const {return m_strPIModified;}
    const TCHAR* getPICreated()const  {return m_strPICreated;}
    const TCHAR* getPIOwner()const    {return m_strPIOwner;}
    const TCHAR* getPIComment1()const {return m_strPIComment1;}
    const TCHAR* getPIComment2()const {return m_strPIComment2;}
    const TCHAR* getPIComment3()const {return m_strPIComment3;}

    //called after all attracted POU bodies compiled with no errors:
    void  setCGRes(const CG_Resource* pres)
    {
        assert(pres->bNoError);//don't set if errors.
        assert(!lstrcmpi(m_resName, pres->pszId));
        m_pres = pres;
    }
    //returns NULL if any errors in resource and attracted POUs
    const CG_Resource* getCGRes()const{return m_pres;}

    void incNumIOBits(int num) { m_iNumIOUsed += num; }
    int  getNumIOBits()const { return m_iNumIOUsed; }

    void setIOerrMsgDisplayed() { m_bIOErrMsgAlreadyDsp = true; }
    bool isIOErrMsgDisplayed()const { return m_bIOErrMsgAlreadyDsp; }
    void setDEMOerrMsgDisplayed() { m_bDEMOErrMsgAlreadyDsp = true; }
    bool isDEMOErrMsgDisplayed()const { return m_bDEMOErrMsgAlreadyDsp; }

    bool checkAddr(
        CGlobCtx*          pgctx,
        const CG_SymEntry* pEntry,
        VM_Address*        pvmAddr
    )const;

    //get the corresponging CChannelBase object that is responsible for 
    //address checking, this is determinded by the first number of the %-addr:
    const CChannelBase* getIOCompByAddr(
        const TCHAR* pszAddr, 
        int*         pnChannelNum
    )const;
    
    int getChannelArraySize()const{return m_channelArraySize;}
    const CChannelBase* getIOCompByChannel(
        int nChannelNum
    )const
    {
        return (nChannelNum>=0 && nChannelNum<m_channelArraySize)
                   ? m_ppChannelArray[nChannelNum] : NULL;
    }
    
    void dumpChannelInfo(CGT_CDiagMsg* pdm, const TCHAR* pszAsmFile);



    void dumpLoaderList(
        CGlobCtx* pgctx
    );

};

#endif

/* ---------------------------------------------------------------------------- */

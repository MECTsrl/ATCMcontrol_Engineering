
#ifndef __PARSE4CH_H_
#define __PARSE4CH_H_

//----  Aggregate Includes:   --------------------------------------*
#include "fc_tools\fc_cgarray.h"
#include "fc_tools\fc_cstring.h"
#include "fc_tools\fc_hash.h"

//----  Forward Class Definitions:   -------------------------------*
class CGlobCtx;
class CKADInfo;
interface ICompile;
interface IDPCompile;

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


//used to pass parametres to checkAddr
struct IOCOMP_CHK_ADDR
{
    //input:
    const CG_Edipos* pEdp; 
    const TCHAR*     pszAddr;
    const TCHAR*     pszType;
    const TCHAR*     pszInst;
    //output:
    FC_CString       javaClass;
    FC_CString       initString;
};



class PC_CIOComp
{
    FC_CStr                m_masterFile;//project relative FBUS data file
    const TCHAR*           m_pszIOTyp;  //points to the file ext. of m_masterFile 

    //next members set in PC_CIOComp::compile():
    ICompile*              m_pICompile; 
	IDPCompile*            m_pDPCompile;
    FC_CStrList            m_domainList;//list of bin files to download, GEN_DIR relative path 
    long                   m_channel;  //the first digit of %-Addr 
                                       //string is the channel number
    FC_CStr                m_IOLayerClsid;
public:
    FC_CStr                m_masterName;//file base name no path no ext
    CG_Edipos              m_edp;      //points to the XML <FBUS link>

    PC_CIOComp(
        const TCHAR* pszTrgFile,   //pointer valid longer than this
        const TCHAR* pszMasterFile //the <FBUS LINK="master.typ">
    );
    ~PC_CIOComp();

    /////////////////////////////////////////////////////////////////////
    /**
     * compile: create a COM object with classid selected by m_pszIOTyp
     * (using KAD info). Call the load method and get the IO-Components domain 
     * list.
     * @param pdm           err msg sink
     * @param pkad          used to get the classid from the IO-components file ext.
     * @param pszPrjPath    full path of project
     * @param pszTrgGenPath full path for target domain destination e.g. ..\gen\4cpc
     * @return        true if no errors
     */
    bool compile(
        CGT_CDiagMsg*   pdm,
        const CKADInfo* pkad,
        const TCHAR*    pszPrjPath,
        const TCHAR*    pszTrgGenPath
    );
    bool checkAddr(
        CGT_CDiagMsg*    pdm,
        IOCOMP_CHK_ADDR* pData
    )const;
    
    const FC_CStrList* getDomainList()const{return &m_domainList;}

    long getChannel()const{return m_channel;}
    const TCHAR* getIOLayerClsid()const{return m_IOLayerClsid;}

    //next methods specific for RTCE:
	inline bool isDP()const{return m_pDPCompile != NULL;}
    long getDPMasterNum()const; 
    bool getDPAddrByteOffset(const TCHAR* pszAddr, long* plByteOffset, long* plBitOffset)const;
    bool getDPIOSize(long* plInBytes, long* plOutBytes, long* plStatusBytes)const;
};



class CTarget
{
    //initialised by loadTarget:
    CG_Edipos m_edp;     //edipos to submitt 4ch located diags
    FC_CStr   m_trgFile; //project relative 4ch file name
    FC_CStr   m_trgName; //name of target (the 4ch file base name)
    FC_CStr   m_resName; //opt. assigned resource
    long      m_lVersion;//20000 for old and 22000 for 'new' controls
    FC_CStr   m_control; //opt. control's address
    FC_CStr   m_console; //opt. control's address
    //list of all <FBUS ..> nodes in 4ch:
    FC_CGArray<PC_CIOComp*> m_masters;
    FC_CStr  m_trgGenPath; //full path to gen\<target name>
    
    //initialised by  CGlobCtx::loadResources():
    const CG_Resource*   m_pres; //may be NULL after error.

    
    //domain list: do a shas to avoid duplicate domains
    //in loader.list (this is a old problem from the bad way
    //who init files are produced)
    FC_CHashStr<int>    m_domList;
    bool                m_bDidAddrSimuWarn; //set true if a warning IO is mem was given once 
    bool                m_bDemoVersion;     //if set true in loadTarget 
    long                m_NumberOfIOPoints; //used for license check and mem info
    long                m_nRetainItems;     //used for mem info
public:
    
    CTarget();
    ~CTarget();
    
    /////////////////////////////////////////////////////////////////////
    /**
     * loadTarget reads a 4CH File's basic info, but does not 
     * call the compile methods of the contained masters.
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

    /////////////////////////////////////////////////////////////////////
    /**
     * compile: calls the PC_CIOComp::compile method for all 
     * IO-components linked to the target, simply a loop over all
     * PC_CIOComp* in m_masters. 
     *
     * @param pdm        err msg sink
     * @param pkad       used to get the classid from the IO-components file ext.
     * @param pszPrjPath full path of project
     * @return        true if no errors
     */
    bool compile(
        CGT_CDiagMsg*   pdm,
        const CKADInfo* pkad,
        const TCHAR*    pszPrjPath
    );

    
    //get basic info:
    const CG_Edipos* getEdp()const{return &m_edp;}
    const TCHAR* getTrgFile()const{return m_trgFile;}
    const TCHAR* getTrgName()const{return m_trgName;}

    bool         hasResName()const{return !m_resName.isEmpty();}
    bool         hasControl()const{return !m_control.isEmpty();}
    bool         hasConsole()const{return !m_console.isEmpty();}
    long         getVersion()const{return m_lVersion;}

    const TCHAR* getResName()const{return m_resName;}
    const TCHAR* getControl()const{return m_control;}
    const TCHAR* getConsole()const{return m_console;}

    const TCHAR* getTrgGenPath()const{return m_trgGenPath;}
    const FC_CGArray<PC_CIOComp*>* getMasters()const{return &m_masters;}

    //called after all attracted POU bodies compiled with no errors:
    void  setCGRes(const CG_Resource* pres)
    {
        assert(pres->bNoError);//don't set if errors.
        assert(!lstrcmpi(m_resName, pres->pszId));
        m_pres = pres;
    }
    //returns NULL if any errors in resource and attracted POUs
    const CG_Resource* getCGRes()const{return m_pres;}

    //isIOEnabled may return fals if e.g demo version 
    //or no fbus masters declared, then all %-add are treated as memory
    //for desk top simulation.
    bool isIOEnabled()const{return !m_bDemoVersion && m_masters.getUsed()!=0;}
    void warnDisableIO(
        CGT_CDiagMsg* pdm, 
        const CG_Edipos* pEdp
    );
    bool checkAddr(
        CGT_CDiagMsg*    pdm,
        IOCOMP_CHK_ADDR* pData
    );
    void setUsedRetain(long nRetainItems)
    { 
        m_nRetainItems = nRetainItems;
    }
    long getUsedRetainItems()
    {
        assert(m_nRetainItems>=0);
        return m_nRetainItems;
    }
    long getUsedIOPoints()
    {
        return m_NumberOfIOPoints;
    }


    //get the corresponging master for an %-address or directly
    //by channel number (the first number of %-addr.)
    const PC_CIOComp* getFBMasterByAddr(
        const TCHAR* pszAddr, 
        int*         pnChannelNum
    )const;
    
    const PC_CIOComp* getFBMasterByChannel(
        int nChannelNum
    )const;


    void addDomain(
        const TCHAR* pszGenRelDomFile
    );

    void dumpLoaderList(
        CGlobCtx*           pgctx,
        const FC_CStrList&  usrClassDom,  
        const FC_CStrList&  cmnClassDom,  
        const FC_CString&   cmnLibDomains,  //from 4cl __control
        const FC_CString&   anyOtherDomains //from 4cp [compiler.4cpc]'s bin=
    );

};


#endif // __PARSE4CH_H_



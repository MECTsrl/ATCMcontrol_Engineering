#ifndef GLOBCTX_H_
#define GLOBCTX_H_

#include "defines.h"
#include "fc_todebug\fc_assert.h"
#include "fc_tools\fc_cgarray.h"
#include "fc_tools\fc_cstring.h"
#include "fc_tools\fc_hash.h"
#include "fc_tools\fc_system.h"
#include "CG_IECFront.h"
#include "cgt_CDiagMsg.h"
#include "parseKAD.h"
#include "license.h"


class CTarget;
class CCodeInfo;


//command line options
//all file names are as found in argv[] of main
//they are not converted to full pathes here.
struct CMDL_OPT_TYP
{
    FC_CStr     sz4CP;          //4cp file          
    FC_CStrList targets;        //list of targets to build, if empty ->build all
    FC_CStr     szCheckFile;    //the file to check
    bool        bIncrementelly; //-i option
    FC_CStr     szMakeLib;      //the lib to make/append
    bool        bAppendLib;     //if append to lib
};

enum BP_OPTION
{
    BP_OPT0, //no BP at all
    BP_OPT1, //begin/end POU only
    BP_OPT2  //whereever possible
};





class CGlobCtx
{
    SL_4CPCLicData          m_licData;
    CMDL_OPT_TYP            m_pCmdlOpts;
    FC_CGArray<CTarget*>    m_targets;
    
    
    FC_CHashStr<const CG_Resource*> m_resources;//loaded .res

    FC_CHashPtr<const CG_Symtab*, CCodeInfo*> m_POUcode;
    //true if at least one java file has 'jitmenot' attr.
    bool                    m_jitmenot;         
    //counts java files to compile with jvc.
    int                     m_CodeFilesCreated; 
    //set by 4cp's [compiler.4cpc] opt=bp0/1/2 setting
    BP_OPTION               m_bpopt; 
    //set by the 4cpc file VERSION= attribute, 
    //Note: all multible targtes must be of same version!
    int                     m_controlVer;

    //set by 4cp's [compiler.4cpc] opt=jvc_gen:<string> jvc_ext:<string>
    FC_CString              m_optJVC_EXT;     
    FC_CString              m_optJVC_GEN;
    FC_CStrList             m_extJavaFiles;
    FC_CGArray<CG_Edipos>   m_extJavaFilesEdp;
    //bin=myDll.dll [, destfolder]
    FC_CGArray<CG_Edipos>   m_extBinFilesEdp;
    FC_CStrList             m_extBinFiles;  //full path or relative to prj
    FC_CStrList             m_extBinDestDir;//relative to control\bin

    //data=myFile.txt [, destfolder]
    FC_CGArray<CG_Edipos>   m_extDataFilesEdp;
    FC_CStrList             m_extDataFiles;  //full path or relative to prj
    FC_CStrList             m_extDataDestDir;//relative to control\bin

    //set by 4cp's [compiler.4cpc] opt=verbose:[0|1|2]
    int                     m_optVerbose;

    //all user classes (java files of project) created in gen\.4cpc\.classes, 
    //must be downloaded & 4cl'ed:
    FC_CStrList             m_usrClassFiles;//including package path
    
    //set in getControlLibDomains: contains a 'loader.list piece' that
    //downloads all __control\*.* of ALL 4cls of the 4cp (not only the IEC-calltree 
    //attracted 4cls, as done for __console\*.*).
    FC_CString              m_cmnLoaderlist;
    FC_CStrList             m_cmnClassFiles;//including package path


    //directory info set by initPathInfo:
    FC_CStr   m_TrgTypeGenPath;// full path of gen\.<trg type>
    FC_CStr   m_clsGenPath;    // full path of gen\.<trg type>\.classes
    FC_CStr   m_lnkGenPath;    // full path of gen\.<trg type>\.link


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

    //dumps all init files of a target:
    void dumpTrgInitFiles(
        CTarget* pTrg
    );
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


    void compilePrgFBSdt(
        const CG_Symtab*     pSym
    );
    void getRecurseCalledFuns(
        const CG_Symtab*     pCaller, 
        FC_CHashPtr<
          const CG_Symtab*, 
          const CCodeInfo*>* pCalled
    );
    //compile4CPPOU helper function to build libs.
    void compile4CPPOU(
        const CG_POU4CPInfo* pInf //a 4cp POU info
    );

    void getControlLibDomains(); //check out all __control files of all libs 
    void callJvcExt();//compile user ext'ra' java files into gen\.4cpc\.classes
    void callJvcGen();//compile generated java files into gen\.4cpc\.classes


public:
    CGlobCtx(
        ICG_IECFront* pFront
    );
    ~CGlobCtx();

    void initLicenseInfo(const CG_LIBContext*const* pzLIBs);

    CMDL_OPT_TYP* getWrtOpts(){return &m_pCmdlOpts;}//get writable pointer

    //must be called after load4CP to read all opt= form [compiler.4cpc] section:
    void getOptionsFrom4CP();
    
    //main operations:
    void rebuildAll();

    //call if there is a console to build:
    void buildConsoleForTrg(const CTarget* pTrg);
    void buildConsoleForLib();


    //only check, do not destrtoy old build result:
    void checkFile();

    //if called to build a library:
    void dumpPOULinkFiles();



    const SL_4CPCLicData* getLicData()const{return &m_licData;}
    
    //public and short names for convenience:
    CKADInfo                 m_kad;
    ICG_IECFront* const      m_pIF; //front end IF
    CGT_CDiagMsg             m_dm;  //diag msg handler
    const CMDL_OPT_TYP*const m_popt;//cmdl option set
    ICG_Config*              m_pConfig;//may be NULL if no config
                                       //possible for lib projects.
                                       //set in loadConfig()
    BP_OPTION getBPOption()const{return m_bpopt;}
    int getControlVer()const{return m_controlVer;}




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
    // full path of gen\.<trg type>\.classes for generated java file's classes
    const TCHAR* getClsGenPath()const
    {
        assert(!m_clsGenPath.isEmpty());
        return m_clsGenPath;
    }
    // full path of gen\.<trg type>\.link for user java file's classes
    const TCHAR* getLnkGenPath()const
    {
        assert(!m_lnkGenPath.isEmpty());
        return m_lnkGenPath;
    }


    const CG_POU4CPInfo** getProcessed4CPFiles()const;

};



#endif
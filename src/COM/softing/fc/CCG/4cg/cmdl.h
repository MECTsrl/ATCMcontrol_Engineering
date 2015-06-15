

#ifndef CMDL_H_
#define CMDL_H_

#include "helper.h"


//command line options
//all file names are as found in argv[] of main
//they are not converted to full pathes here.
enum LIB_ACTION
{
    LIB_ACT_UNDEF,
    LIB_MAKE,
    LIB_MAKE_STUB,
    LIB_APPEND,
    LIB_REMOVE,
    LIB_LIST,
    LIB_LISTLONG,
    LIB_EXTRACT
};
const TCHAR* LIBOptName(LIB_ACTION o, bool bMinus=false);

class CMDL_OPTIONSTYP
{
public:
    CMDL_OPTIONSTYP()
    {
        bIncrementelly = false;
        bRecursive     = false;
        libAct         = LIB_ACT_UNDEF;
        bXREF          = true;
        bConsoleInfo   = false;
        nVerbose       = 1;
    }
    FC_CStr        sz4CP;          //4cp file          
    FC_CStr        sz4CPFullPath;  //4cp file's full path         
    FC_CStrList    targets;        //list of targets to build, if empty ->build all
    FC_CStr        szCheckFile;    //the file to check
    FC_CStr        szCheckTrgType; //empty or trg type for check file.
	FC_CStr        szCheckTrgVers; //empty or trg version number
    bool           bXREF;          //build xref
    bool           bConsoleInfo;   //build console information
    bool           bIncrementelly; //-i option
    bool           bRecursive;     //-r recursive invocation (for auto build lib)

    LIB_ACTION     libAct;         //lib action (make/append/list/remove)
    FC_CStr        szMakeLib;      //the lib name to act on (is the dest if -al) 
    FC_CStr        szSourceLib;    //the source lib for the -al "append one lib to another" feature
    FC_CStr        szLibTargetType;//the specific target type for the lib action
                                   //if "0" create stub only
    int            nVerbose;       //0,1,2: 
                                   //0: no progress/action info, 
                                   //1: default: start up message lib check ins, ...
                                   //2: 1 + include backends calls prre/post build actions.
                                    
    FC_CStrList    libSrcDirs;     //if a .4cl does not exist, try to get source from this dir
    FC_CGArray<const CG_Edipos*>libSrcDirsEdp;


    //return true if rebuild all must be done, then e.g. clean up the gen dir:
    //if the command line has individual targets that is not a rebuild all!
    bool isRebuildProject()
    {
        if(!szCheckFile.isEmpty())
            return false;
        return libAct==LIB_MAKE || libAct==LIB_MAKE_STUB || 
            (libAct==LIB_ACT_UNDEF && targets.getUsed()==0 && !bIncrementelly);
    }
};



extern "C" bool DumpLogoAndHelp(
    int           argc, 
    TCHAR**       argv
);

extern "C" bool ParseCommandLine(
    CGT_CDiagMsg*    pdm,
    ICG_IECFront*    pIFront, 
    CMDL_OPTIONSTYP* pOpts,
    int              argc, 
    TCHAR**          argv
);











#endif



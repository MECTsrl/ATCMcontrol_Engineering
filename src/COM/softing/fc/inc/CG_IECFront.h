/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/CG_IECFront.h 2     14.05.07 16:37 Ef $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CG_IECFront.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/CG_IECFront.h $
 *
 * =PROJECT             CAK1020  ATCMControl V2.0
 *
 * =SWKE                4CCG
 *
 * =COMPONENT           CG_IECFront
 *
 * =CURRENT             $Date: 14.05.07 16:37 $
 *                      $Revision: 2 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *

 



 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *== ATCMControl IEC Compiler front COM-Interface definition. This header file
 *   contains the Class and Interface definitions.
 *   The uuid definitions can be found in CG_IECFront_i.c
 *   This header file has currently no 'C' COM Interface definitions, 
 *   you must use it from C++.
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  05.01.01  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/

#ifndef __CG_IECFRONT_H_
#define __CG_IECFRONT_H_
#include <tchar.h>
#include <unknwn.h>

#include "CG_4CLAccess.h"

#ifdef UNICODE
#error "CG_IECFront.h cannot be used with unicode, using TCHAR is only for future versions"
#endif
//warning C4200: nonstandard extension used : zero-sized array in struct/union
#pragma warning( disable : 4200) 


//coclass supports the IID_ICG_IECFront interface ID:
EXTERN_C const GUID CLSID_CG_IECFront;





//----  Forward Class Definitions:   -------------------------------*
// Some  enums,unions, structures must be foreward defined:
typedef enum   _CG_SOURCETYPES  CG_SOURCETYPES;
typedef enum   _CG_DTYPES       CG_DTYPES;
typedef enum   _CG_DECLSCOPE    CG_DECLSCOPE;
typedef enum   _CG_POUTYP       CG_POUTYP;
typedef enum   _CG_STMTTYP      CG_STMTTYP;
typedef enum   _CG_EXPRTYP      CG_EXPRTYP;

typedef struct _CG_EdpSymtab     CG_EdpSymtab;
typedef struct _CG_Edipos        CG_Edipos;
typedef struct _CG_DType         CG_DType;
typedef struct _CG_Subrange      CG_Subrange;
typedef struct _CG_Literal       CG_Literal;
typedef struct _CG_Initialiser   CG_Initialiser;
typedef struct _CG_KeyValPair    CG_KeyValPair;
typedef struct _CG_POU4CPInfo    CG_POU4CPInfo;
typedef struct _CG_LIBContext    CG_LIBContext;
typedef struct _CG_AttribList    CG_AttribList;
typedef struct _CG_SymEntry      CG_SymEntry;
typedef struct _CG_Symtab        CG_Symtab;
typedef struct _CG_MacroBody     CG_MacroBody;
typedef struct _CG_ExprTree      CG_ExprTree;
typedef struct _CG_ArgExpr       CG_ArgExpr;
typedef struct _CG_STStmt        CG_STStmt;
typedef struct _CG_SFCStruct     CG_SFCStruct;
typedef struct _CG_TaskInit      CG_TaskInit;
typedef struct _CG_Task          CG_Task;
typedef struct _CG_Resource      CG_Resource;
typedef struct _CG_ResImpInfo    CG_ResImpInfo;
typedef struct _CG_Features      CG_Features;

interface ICG_Body;
interface ICG_Expr;
interface ICG_Config;
interface ICG_ExtBodyInfo;

//Display an internal error. The error number C0000 is descriped in the 
//standard 4C core online help and gives the user at least a good feeling...
#define CG_FATAL_MSGFMT _T("4cg : fatal error C0000: internal compiler")\
                        _T(" error: compiler file '%s' line %d.\n")\
                        _T("Assumed reason: %s\n")

//version info of front end:
#define CG_FRONT_END_VERSION _T("2.0")

//the project file extension:
#define CG_4CP_EXTENSION           _T("4cp")

//the project format supported by the front end:
#define CG_VAL_4CP_FORMAT          _T("V2.0")

//The following section and key names are predefined:
//Only these predefined sections and their content are
//validated by the front end and 4cg.exe:
#define CG_SEC_4CP_ATTRIBUTES      _T("Attributes")
#define CG_SEC_4CP_FORMAT          _T("Format") 
#define CG_SEC_4CP_CLASSES_BEG     _T("Classes.")
#define CG_SEC_4CP_TARGETS         _T("Targets")
#define CG_SEC_4CP_LIBS            _T("Libs")
#define CG_SEC_4CP_COMPILER_BEG    _T("Compiler.")
#define CG_SEC_4CP_COMPILER        _T("Compiler")
// all files of section [console] are downloaded to all consoles of all resources
#define CG_SEC_4CP_CONSOLE         _T("Console")
#define CG_SEC_4CP_CONSOLE_BEG     _T("Console.")


//predefined classes: 
#define CG_SEC_4CP_STDCLS_CON      _T("classes.Configurations")
#define CG_SEC_4CP_STDCLS_PRG      _T("classes.Programms")
#define CG_SEC_4CP_STDCLS_FB       _T("classes.Functionblocks")
#define CG_SEC_4CP_STDCLS_FUN      _T("classes.Functions")
#define CG_SEC_4CP_STDCLS_SDT      _T("classes.Structures")
#define CG_SEC_4CP_STDCLS_CST      _T("classes.Constants")
#define CG_SEC_4CP_STDCLS_GVL      _T("classes.GVLs")

//predefined keys of predefined sections: 
//[Format]
#define CG_KEY_4CP_VERSION         _T("version")
#define CG_KEY_4CP_NEEDVER         _T("needver") //needver=[$(insert)]<file>.[dll|exe|4cl] [=|>=|<=] Build<BNr>

//[classes.*] 
#define CG_KEY_4CP_FILE            _T("file")

//predefined source file extensions: 
#define CG_PREDEF_SOURCE_HDR  _T("hdr") //.hdr only for internal use in libs
#define CG_PREDEF_SOURCE_ST   _T("st")
#define CG_PREDEF_SOURCE_IL   _T("il")
#define CG_PREDEF_SOURCE_4GR  _T("4gr")
#define CG_PREDEF_SOURCE_SDT  _T("sdt")
#define CG_PREDEF_SOURCE_CST  _T("cst")
#define CG_PREDEF_SOURCE_GVL  _T("gvl")
#define CG_PREDEF_SOURCE_RES  _T("res")
#define CG_PREDEF_SOURCE_CON  _T("con")
#define CG_PREDEF_SOURCE_EXTS _T("st,il,4gr,sdt,cst,gvl,res,con")
#define CG_PREDEF_LANG_EXTS   _T("st,il,4gr,sdt")

//predefined library file extensions: 
#define CG_PREDEF_LIB_EXT     _T("4cl")


//TODO make list complete !
//predefined attributes for POUs comments:
#define CG_ATTR_VERSION    _T("version")   //value:  "<any user defined text>"
#define CG_ATTR_DEEPCOPY   _T("deepcopy")  //value:  "warn"|"disable"

//predefined attributes for POU and decl comments:
#define CG_ATTR_DEPRECATED    _T("deprecated")   //value: text for deprecated warning msg.
#define CG_ATTR_FORCECONTEXT  _T("forcecontext") //value: comma separated list of required calling context 
                                                 //fCurrently there are:
                                                 //   forcecontext="acyclictask"
                

#define CG_MAX_ID_LENGTH     32 //the maximun allowed length in TCHARS of an identifier
#define CG_MAX_INST_PATH_LEN 32 //the maximun allowed instance path length including
                                //target address and resource name. Examples:
                                //  "{w-ef}.Resource1.Program1.myFB.aInput"  has lenght 5
                                //  "{w-ef}.Resource1.aGlob"                 has lenght 3
                                //Note: whenever instance pathes must be stored/created this can
                                //used as static dimension, no instance path will be longer 
                                //once the project passed successfully the front end.



//predefined $(inserts):                expands to:   recognized in all file= 
#define CG_INSERT_INSDIR  _T("insdir")  //installation directoy: c:\4c2   
#define CG_INSERT_LIBDIR  _T("libdir")  //standard lib path: c:\4c2\engineering\lib
#define CG_INSERT_BINDIR  _T("bindir")  //binary dir         c:\4c2\engineering\bin
#define CG_INSERT_SYSDIR  _T("sysdir")  //windows GetSystemDirectory
#define CG_INSERT_WINDIR  _T("windir")  //windows GetWindowsDirectory 
//if a 4cp is loaded also the following:
#define CG_INSERT_PRJNAME _T("prjname") //project name     
#define CG_INSERT_PRJDIR  _T("prjdir")  //project path     
#define CG_INSERT_GENDIR  _T("gendir")  //gen path of current project


//predefined custom build step/opt= commands of [compiler] section:
                                           //lib.* executed before any post build commands.
#define CG_BUILDCMD_LIBPUT _T("lib.put")   //lib.put= [path\]file(s)  >   __CONSOLE\4cpc
#define CG_BUILDCMD_LIBTXT _T("lib.txt")   //lib.txt= <append this text > __CONSOLE\4cpc
                                           

#define CG_BUILDCMD_LIBLIC _T("lib.license")//lib.license= <any string>


#define CG_BUILDCMD_PRE    _T("pre")        //pre build step before any other action,
                                            //stops compilation if process returns !=0                  
#define CG_BUILDCMD_PRECD  _T("pre.cd")     //change current working directory for next commands
#define CG_BUILDCMD_PREI   _T("pre.ignore") //pre build step before any other action, 
                                            //but ignore process return value.
#define CG_BUILDCMD_PREP   _T("pre.print")  //only print pre.print=<pszVal> with $(...) expanded 
                                            //before any other action, does not execute <pszVal>.

#define CG_BUILDCMD_POST   _T("post")       //post build step after all other actions (even after 
                                            //build lib), stops compilation if process returns !=0.
#define CG_BUILDCMD_POSTCD _T("post.cd")    //change current working directory for next commands
#define CG_BUILDCMD_POSTI  _T("post.ignore")//post build step aftzer all other actions, 
                                            //but ignore process return value.
#define CG_BUILDCMD_POSTP  _T("post.print") //only print post.print=<pszVal> with $(...) expanded
                                            //after all other actions, does not execute <pszVal>.
//build options:
#define CG_BUILD_OPT            _T("opt")       //opt= for all other options
#define CG_BUILD_OPT_XREF       _T("xref:")     //opt=xref:0,1  
#define CG_BUILD_OPT_CONSOLEINF _T("console:")  //opt=console:0,1  
#define CG_BUILD_OPT_VERBOSE    _T("verbose:")  // opt=verbose:0,1,2  

//the following can be used as an instant mini help after errors:
#define CG_COMPSEC_HELP    _T("opt, (pre|post)[.cd|.print|.ignore], lib.put, lib.txt, lib.license")
#define CG_BUILD_OPT_HELP  _T("xref:(0|1), verbose:(0|1|2), console(0|1)")

//predefined special POU names in libraries:
#define CG_LIB_POU_LICENSE  _T("__LICENSE")
#define CG_LIB_POU_VERSION  _T("__VERSION")
#define CG_LIB_POU_CONST    _T("CONST")
#define CG_LIB_POU_CONTROL  _T("__CONTROL")
#define CG_LIB_POU_CONSOLE  _T("__CONSOLE")
#define CG_LIB_POU_HELP     _T("__HELP")


//#define predefined gen output folders and files extensions:
//they can but should not be used for target types or languages.
//for XREF:
#define CG_XREF_GEN_DIR     _T("__4xr")  //<gen>\__4xr
#define CG_XREF_POU_EXT     _T("4xr")    //<gen>\__4xr\<pouname>.<pouext>.4xr
#define CG_XREF_LST_EXT     _T("4xl")    //<gen>\__4xr\<resource>.4xl
 

//predefined link file extension:
#define CG_LINK_FILE       _T(".link") //used for library creation
//Backends create:
//<gen>\.<target type>\<source file>.<ext>.link 
//  containing all required and the put=<fullpath>\<domain file> commands 
//
//Inside the 4CL there is the target independent IEC interface description:
//    <POU name>\.link
//containing all required but the put=<fullpath>\<domain file> and link=<mangeled name>
//commands are removed, version=<front end version>,  trgtyp="" and source="".
//
//Inside the 4CL there is the target type specific information:
//    <POU name>\<target type>\.link
//containing all required but the put=<fullpath>\<domain file> commands are removed,
//version=<back end version> and trgtyp=<target type>.

//Link file commands/keys: 
//They must appear in exactly this order in any .link file. 
//Key names are only for documentation purpose.
#define CG_LINK_LIB_VERSION  _T("version") //required, front or back end version
#define CG_LINK_LIB_IECIF    _T("iecif")   //required, mangeled name see createIECMangeledName
#define CG_LINK_LIB_POUCLASS _T("pouclass")//required, engineering's KAD POU class name
#define CG_LINK_LIB_TRGTYP   _T("trgtyp")  //required, empty for POU lib template
#define CG_LINK_LIB_HEADER   _T("header")  //required, file name inside lib including the display portion
#define CG_LINK_LIB_SOURCE   _T("source")  //required, empty if precompiled code or POU lib template 
#define CG_LINK_LIB_POUATTR  _T("pouattr") //zero or more, attributes of the POU's (*[[..]]*)
#define CG_LINK_LIB_LINK     _T("link")    //zero or more, called functions and used CONST's mangeld names  
#define CG_LINK_LIB_PUT      _T("put")     //zero or more, files to check in (with full path, destination 
                                           //inside lib is always the <POU name>\<target type> folder.


/* Example link file:
version;2.0
iecif;FB$TemperatureControl(Q#BOOL#STATE,Q#BOOL#ERROR,Q#DINT#ERROR_NR,Q#WSTRING#ERROR_STRING)
pouclass;FUNCTION_BLOCK
trgtyp;4cpc
header;TemperatureControl.hdr;727
source;
pouattr;Version;12
pouattr;UserURL;4cfile:TemperatureControl.htm
link;FUN$OutputDebugString(Q#VOID#OutputDebugString,I#WSTRING#STR)
put;C:\4c2\COM\softing\fc\bak\CL\4C_System\eng\gen\.4cpc\.link\TemperatureControl.hdr
put;C:\4c2\COM\softing\fc\bak\CL\4C_System\eng\gen\.4cpc\.classes\temperaturecontrol.class
put;C:\4c2\COM\softing\fc\bak\CL\4C_System\eng\TemperatureControl.asp
*/


//flags for ICG_IECFront::loadBody:
#define CG_LOADBDY_TMPVAR_OPT1   0x00001  //if set: perform basic  LD/FBD tmp var optimisation
                                          //if not set absolutely no optimisation, allows full 
                                          //breakpoints inside networks.
                                         
                                          
#define CG_SFC_EXCEPTIONACTION _T("ExceptionAction") //an action with this name
                                                     //is an sfc exception action:
                                                     //this action will be executed every sfc cycle
                                                     //before the sfc interpreter is called
                                                     //a return in this action will prevent that the
                                                     //sfc interpreter is called at all


//+-------------------------------------------------------------------------
EXTERN_C const GUID FAR  IID_ICG_IECFront;
class ICG_IECFront : public IUnknown
{
public:

//--- module initialisation:
    //Normal way: 
    //loadKAD->loadFeatures->load4CP->load4CLs->loadCSTs
    //When using only e.g. parseExprFromString() loadXXX method can be ommitted.


    virtual boolean STDMETHODCALLTYPE loadKAD(
        /*[string][in]*/const TCHAR* pszKADFileAndPath
    ) = 0;
    
    //call in order to restrict features pFeatureSet can be NULL 
    //to enable all implemented features.
    virtual boolean STDMETHODCALLTYPE loadFeatures(
        /*[in]*/ const CG_Features* pFeatureSet
    ) = 0;
    virtual boolean STDMETHODCALLTYPE load4CP(
        /*[string][in]*/const TCHAR* psz4CPFile
    ) = 0;


//--- KAD info service, requires a prior call to loadKAD:
    //KADgetFile can be used for  diag msg on KAD problems, and
    //it simply returns the KAD file name and full path.
    //Or "" if no KAD sucessfully loaded (and intenal error)
    virtual const TCHAR* STDMETHODCALLTYPE KADgetFile(void) =0;

    //KADgetTargetTypeExts can be used for e.g. error messages
    //returns e.g. "4cpc,pam,sam" or "" (and an internal error) if no KAD loaded.
    virtual const TCHAR* STDMETHODCALLTYPE KADgetTargetTypeExts(void) =0;
    
    //returns the number of target types found in KAD
    virtual int STDMETHODCALLTYPE KADCountTargetTypes(void) =0;

    /**
     * KADgetTargetByExt searches for the back end by file extension.
     *
     * @param    iTargetType    target type number: 
     *                          valid range 1..KADCountTargetTypes() 
     * @param    ppsz           if not NULL: receives a pointer to 
     *                          full path and file of backend exe.
     * @param    puuidTrgInfo   if not NULL: receives the clsid of the  
     *                          corresponding KAD's GETTARGETINFO clsid.
     * @param    pbMultipleTrgs if not NULL: set TRUE if backend supports 
     *                          multible trgs on the command line or set FALSE
     *                          if does not support.
     * @return   the ordinal number of target type in KAD starting at 1
     *           or if target type is not defined returns 0.
     *
     * @see      KADgetTargetTypeByNum
     */
    virtual int STDMETHODCALLTYPE KADgetTargetTypeByExt(
        const TCHAR*  pszExt,
        const TCHAR** ppsz,
        GUID*         puuidTrgInfo,
        boolean*      pbMultipleTrgs
    ) =0;

    //------------------------------------------------------------------*
    /**
     * KADgetTargetTypeByNum gets the KAD's target type info by number.
     * @param    iTargetType    target type number: 
     *                          valid range 1..KADCountTargetTypes() 
     * @param    ppsz           if not NULL: receives a pointer to 
     *                          full path and file of backend exe.
     * @param    puuidTrgInfo   if not NULL: receives the clsid of the  
     *                          corresponding KAD's GETTARGETINFO clsid.
     * @param    pbMultipleTrgs if not NULL: set TRUE if backend supports 
     *                          multible trgs on the command line or set FALSE
     *                          if does not support.
     * @return   the target type extension if succesfull or NULL if any error
     * @see      KADgetTargetByExt
     */
    virtual const TCHAR* STDMETHODCALLTYPE  KADgetTargetTypeByNum(
        int           iTargetType,
        const TCHAR** ppsz,
        GUID*         puuidTrgInfo,
        boolean*      pbMultipleTrgs
    ) =0;

    //KADgetSourceExts can be used for e.g. error messages
    //returns e.g. "st,4gr,sdt,pn" or "" (and internal error) if no KAD loaded.
    virtual const TCHAR* STDMETHODCALLTYPE KADgetSourceExts(void) = 0;
    
    //KADgetSourceTypeByExt searches for the source file type in a list of 
    //the predefined and also in the KAD's list of IEC filters.
    //Note: if a filter is defined for one of the predefined 
    //source types the return value is not CG_SRC_TYPE_ADDON_FILTER
    //it's the corresponding CG_SRC_TYPE_XXX instead.
    //return CG_SRC_TYPE_UNDEF if not unknown source type.
    virtual CG_SOURCETYPES STDMETHODCALLTYPE KADgetSourceTypeByExt(
        const TCHAR* pszExt      //the file extension to be searched for e.g. "st"
    ) =0;
    //KADgetFilterByExt searches for the source file input filter by file 
    //extension. Note: if a filter is defined for one of the predefined 
    //extensions the function returns this filter.
    //returns true if a filter found false otherwise
    virtual boolean STDMETHODCALLTYPE KADgetFilterByExt(
        const TCHAR* pszExt,    //source file extension to search for
        GUID*        puuidFilter//if not NULL receives the clsid of the 
                                //corresponding KAD's IECFILTER entry.
    ) =0;


//--- 4cp file's info service, requires prior call to load4CP:
    //project file name for error messages,
    //exactly the same as was passed to load4CP:
    virtual const TCHAR* STDMETHODCALLTYPE FCPgetDiagFileName(void) = 0;
    //Note: multiple occurence of the same section is merged
    //getSectionNames: returns list of all section names found
    virtual const TCHAR*const* STDMETHODCALLTYPE FCPgetSectionNames(void) = 0;
    //getSectionContent: returns NULL if section not found (empty section returns !=NULL)
    //The '[' ']' are not part of section name. 
    //Example: FCPgetSection("compiler.4CPC")
    virtual const CG_AttribList* STDMETHODCALLTYPE FCPgetSection(
        /*[string][in]*/ const TCHAR* pszSec
    ) = 0;

    
    //fast hash access to compiler specific sections:
    //pszTAR and pszPOU are the symbolic IEC names
    //without any extension and path, returns the corresponding
    //4CP info or NULL if not found.
    virtual const CG_POU4CPInfo* STDMETHODCALLTYPE FCPgetCONinfo(
        /*[string][in]*/ const TCHAR* pszCON  //can be NULL to get the one and only con
    ) = 0;
    virtual const CG_POU4CPInfo* STDMETHODCALLTYPE FCPgetTARinfo(
        /*[string][in]*/const TCHAR* pszTAR
    ) = 0;
    virtual const CG_POU4CPInfo* STDMETHODCALLTYPE FCPgetPOUinfo(
        /*[string][in]*/const TCHAR* pszPOU
    ) = 0;

    //------------------------------------------------------------------*
    /**
     * Searches the 4cp for a named library and returns a lib context info.
     * @param    pszLIB library identifer, no  path but extension 4cl
     *           e.g. 4C_IEC1131_3.4cl
     *           Note: all libs attached to the project must be unique 
     *           in this way.
     * @return   NULL if no such lib in the [libs] section or a 
     *           context info struct.
     * @see      get4CLLibIF, CG_LIBContext
     */
    virtual const CG_LIBContext* STDMETHODCALLTYPE FCPgetLIBinfo(
        /*[string][in]*/const TCHAR* pszLIB
    ) = 0;

    //------------------------------------------------------------------*
    /**
     * Get a list of all LIBs in the [libs] section.
     * @return   a NULL terminated array of CG_LIBContext structs.
     */
    virtual const CG_LIBContext*const*
         STDMETHODCALLTYPE FCPgetAllLIBs(void) = 0;

    /**
     * Get a list of all .cst files in all [classes.*] sections.
     * Note: a .cst in any other classes section than [classes.Constants]
     * is only a warning not an error.
     * @return   a NULL terminated array of CG_POU4CPInfo structs.
     */
    virtual const CG_POU4CPInfo*const* 
         STDMETHODCALLTYPE FCPgetAllCSTs(void) = 0;

    //------------------------------------------------------------------*
    /**
     * Get a list of all POUs in all [classes.*] sections that are not 
     * .cst and not .con. All files not .cst,.con are considered to be 
     * in the POU name space, their extension must be either a predef 
     * language extension (see #define CG_PREDEF_LANG_EXTS) or one of
     * the KAD defined filter extension.
     *
     * @return        a NULL terminated array of CG_POU4CPInfo structs.
     */
    virtual const CG_POU4CPInfo*const* 
         STDMETHODCALLTYPE FCPgetAllPOUs(void) = 0;

    /**
     * Seraches the <project>.4cp [classes.*] sections and all libraries 
     * in [libs] section for a POU by name. Libraries are searched in the 
     * order of the 4CP's lib=<libfile> section entries.
     * Duply defined POUs cause a warning (if bWarnDuply is true) and the 
     * first found definition is used.
     * Searching in libraries is done without useing the target type, only
     * if a lib definition found and pszTrgType!=NULL the 'supported 
     * target type' is checked (this makes POU existence independent from 
     * any current target type scope). 
     * The function does not validate if the file physically exists.
     *
     * The result of the search is a unique locator of the source file
     * that contains the POU definition, this may be also be a .hdr
     * file if POU is lib defined and has precompiled body as .obj .class
     * .bin or the like. 
     * In other words: If the locator's extension is .hdr there is either 
     * no target type scope given or no source code for the body (see 
     * source code libs below). .hdr is only used together with a library 
     * definition.
     *
     * The function can be called with a certain target type the POU must
     * support. If the POU is project defined the pszTrgType param is 
     * ignored. If the POU is library defined the <POU name>\<pszTrgType>
     * folder must exist too, otherwise the function returns 0 (but does 
     * not submit a diagnostic). 
     *
     * Source code library support: 
     * If the POU is library defined and there is a target type scope given
     * (param pszTrgType!=NULL) and the corresponding .link file in 
     * <libid>::4cl\<pouname>\<trgtyp>\.link contains a
     *   "source;<pouname>.[sdt|st|4gr|<filter ext>"  entry the returned locator 
     * will point to this source file. A struct is always source code.
     * 
     * Syntax of returned locator is:
     * a) Project defined:
     *    [path\]<pouname>.[sdt|st|4gr|<filter ext>]
     *
     * b) Library defined and no target type specified or not 
     *    supported for that target type:
     *     <libid>.4cl::<pouname>.[sdt|hdr]
     *
     * c) Library defined and target type given and supported by POU and source code lib:
     * [<libid>.4cl::]<pouname>[\<trgtype>]\<pouname>.[sdt|st|4gr|<filter ext>]
     *  
     * d) Library defined and target type given and supported by POU and precompiled:
     * [<libid>.4cl::]<pouname>[\<trgtype>]\<pouname>.[hdr]
     *
     * e) POU not defined at all:
     * <empty string>
     *
     * <libid> is the library file base name without path e.g 4C_IEC1131_3
     * Note: libs attached to the project must be unique in respect to their file 
     * base name.
     * 
     * @param     pszPOUName   IEC POU name (a FUN,STRUCT, FB, PRG or add on language POU).
     * @param     pszTrgType   NULL or the target type to support.
     * @param     szBuffer     Output: will be filled with an unique locator
     * @param     nBuffer      the size of the result buffer in TCHARs. 2*MAX_PATH
     *                         will always be enough.
     * @param     bNoWarnings  if true duply defined POU warning is suppressed.
     *
     * @return    -1:  buffer too short, then szBuffer is unchanged.
     *             0:  POU not defined at all (szBuffer is now set empty)
     *             1:  POU defined and if pszTrgType was not NULL does also 
     *                 support pszTrgType.
     *             2:  POU defined but does not support pszTrgType.
     *                 In this case szBuffer receives the interface file
     *                 in <libname>.4cl::<pouname>\<pouname>.[sdt|hdr]
     *                 
     * @see       loadHeader,loadBody       
    */
    virtual long STDMETHODCALLTYPE FCPgetPOUDefFileLocator(
        /*[in, string]*/ const TCHAR* pszPOUName,
        /*[in, string]*/ const TCHAR* pszTrgType,
        /*[out,string]*/ TCHAR        szBuffer[/*nBuffer*/],
        /*[in]        */ long         nBuffer,
        /*[in]        */ boolean      bWarnDuply
    ) = 0;




//--- directory info functions, available after load4CP:
    //------------------------------------------------------------------*
    /**
     * FCPgetGenPath: get the gen path for compiler output
     *                Note: don't assume 'gen' is always underneath prjdir.
     * @return        the full path the 'gen' directory.
     */
    virtual const TCHAR* STDMETHODCALLTYPE FCPgetGenPath( void) = 0;

    //------------------------------------------------------------------*
    /**
     * FCPgetPrjPath: get the project path of the loaded 4CP.
     * @return        the full path the directory where the 4CP is located.
     */
    virtual const TCHAR* STDMETHODCALLTYPE FCPgetPrjPath( void) = 0;
    
    //------------------------------------------------------------------*
    /**
     * FCPgetPrjName: get the 4CP file basename without extension and path.
     *                Note: the project name is not necessarily an 
     *                identifer, can even contain blanks.
     * @return  the name of the project
     */
    virtual const TCHAR* STDMETHODCALLTYPE FCPgetPrjName( void) = 0;

    //------------------------------------------------------------------*
    /**
     * FCPgetStandardLibPath: get the standard libraray path.
     * @return  standard libraray path
     */
    virtual const TCHAR* STDMETHODCALLTYPE FCPgetStandardLibPath( void) = 0;

    //------------------------------------------------------------------*
    /**
     * FCPgetInstallationPath: get the front end module path, this must be 
     * also the installation path e.g. "c:\Programms\4CONTROL\engineering\bin"
     * @return  4C-core-system's installation path, including the '\bin'
     */
    virtual const TCHAR* STDMETHODCALLTYPE FCPgetInstallationPath( void) = 0;






//--- load source files:
    //------------------------------------------------------------------*
    /**
     *loadLIBs: loads all .4cl of the 4cp's [libs] section. Requires a 
     * prior call to load4CP.
     * loadLIBs appends the 4cl's POU-names to the internal POU-name list
     * and loads the 4cl's .cst files to the global const table.
     * Loading a library does not load the POUs containd immediately,
     * except the CST POUs.
     *
     * Diagnostics: 
     * All errors/warnings during the load lib process are submitted.
     * If any errors, backends should stop the build process.
     * A POU name clash during loadLIBs is not treated as an error
     * or warning. Later when a duply defined POU is actually used a 
     * warning is submitted, unused duply defined POUs are not 
     * diagnosed at all. This is different for a CONST name clash, since
     * the .cst are loaded immediately a corresponding diag is submitted
     * from loadLIBs. Currently a global CONST name clash is treated as an 
     * error.
     * 
     * Note: The library's binary format version is checked and if not ok
     * submit an error, but the contained obj. code is not version checked, 
     * this must be done by back ends for every used POU individually.
     * 
     * @return   true if no error, false otherwise. 
     * @see      load4CP
     */
    virtual boolean STDMETHODCALLTYPE loadLIBs(void) = 0;

    //------------------------------------------------------------------*
    /**
     *loadCSTs: loads all .cst of the 4cp's [classes.constants] section 
     * to the global const table. Requires a prior call to load4CP.
     * Diagnostics: 
     * All errors/warnings during the parse process are submitted.
     * If any errors, backends should stop the build process.
     * 
     * @return   true if no error, false otherwise. 
     * @see      load4CP,loadCST
     */
    virtual boolean STDMETHODCALLTYPE loadCSTs(void) = 0;


    //------------------------------------------------------------------*
    /**
     * loadConfig: loads a con file, always load no look up.
     * Load loadConfig must be called before loadResource. The returned 
     * pointer is valid until the ICG_IECFront is finally released. 
     * The configuration is identifed by its CG_POU4CPInf struct. 
     * This struct can be obtained by FCPgetCONinfo(NULL), there is only 
     * one or no .con file per project.
     * Note 1: Having no config is normal when builing a library.
     * Note 2: loadConfig neither loads the contained resources 
     *         nor checks that any #import "<file>.res>" does exist.
     *         Also VAR_CONFIG, VAR_ACCESS sections are not validated, 
     *         this is up to back ends.
     * 
     * Diagnostics:
     * 1) All errors/warnings during the parse process are submitted, 
     * 2) If the con file is not found loadConfig returns NULL, NO error 
     *    message is submitted and the error count is not incremented.
     *    The caller is responsible to handle this sort of error.
     *
     * @param    pCON   the config's CG_POU4CPInfo struct to load.
     *                  
     * @return   NULL if file not found or any errors, otherwise a 
     *           ICG_Config interface, must be released by caller. 
     *
     * @see      FCPgetCONinfo, ICG_Config       
     */
    virtual ICG_Config* STDMETHODCALLTYPE loadConfig(
        /*[in]*/ const CG_POU4CPInfo*  pCON
    ) = 0;




    //------------------------------------------------------------------*
    /**
     * loadHeader: load symbol table of a FB,Function,Program or Struct. 
     * If already loaded pure lookup and return the previously loaded 
     * symtab. The returned pointer is valid until the ICG_IECFront 
     * is finally released. 
     * The POU is searched by IEC name, not a file name. 
     * loadHeader uses the following search strategy (see also FCPgetPOUDefFileLocator):
     * 1) the POU file must be referenced in one of the 4cp's 
     *    classes.* sections as a file named [opt_path\]<POUName>.<ext>
     * 2) searches the libraries for POU name in the order as they
     *    have been loaded. 
     *    If there was a target type specified in loadFeatures:
     *       first the library folder <libname>.4cl::<pouname>\<target type> 
     *       is searched for the POU definition, if that folder does not exists
     *       the folder <libname>.4cl::<pouname> is used to get the 'IEC interface only'
     *       definition. 
     *    
     * If the POU was found this way, then the POU is called 'defined'.
     * If the POU is defined it's header portion is loaded if 
     * <ext> is  .st, .sdt or .4rg.
     * if <ext> is a KAD defined filter ext the filter is called and 
     * if succeeded the header portion of the file produced by the 
     * filter is loaded.
     * Note: A filter called via loadHeader must return an IEC object 
     * as .st, .sdt or .4gr file that defines a struct, function, 
     * function block or program.
     *
     * Diagnostics:
     * 1) If the POU is loaded the first time the search descriped above
     *    will be continued to check if the POU is possibly defined twice.
     *    If so, a warning is submitted and the first found definition 
     *    is used.
     * 2) All errors/warnings during the parse process are submitted, 
     *    including file not found errors.
     * 3) If the POU is not defined loadHeader returns NULL, NO error 
     *    message is submitted and the error count is not incremented.
     *    The caller is responsible to handle this sort of error.
     * 4) If a target type specified in loadFeatures and the POU is 
     *    library defined but there is no implementaion for that target type
     *    NO error message is submitted and the error count is not incremented.
     *    The caller is responsible to handle this sort of error (to detect this 
     *    situation check if the CG_STAB_LIBIF_ONLY bit is set in CG_Symtab::flags)
     *
     * Special situations:
     *  1) the POU is defined but the file has hard syntax errors even 
     *     before the FUNCTION, FUNCTION_BLOCK, PROGRAM or STRUCT keyword 
     *     was reached (or can just not be opened because the file does 
     *     physically not exist or is not readable).
     *  2) The POU name inside the file differs from the file base name.
     *
     *  In both cases the CG_Symtab::pouType member is set to 
     *  CG_POUTYP_UNDEF and the symtab's error flag is set, corresponding 
     *  error messages are submitted and loadHeader returns not NULL.
     *
     * @param    pszPOUName  the IEC POU-name to load or lookup 
     * @return   NULL only if POU was not defined, otherwise a symtab 
     *           that may still contain errors see CG_Symtab::flags bits.
     *
     * @see      CG_Symtab, FCPgetPOUDefFileLocator       
     */
    virtual const CG_Symtab* STDMETHODCALLTYPE   loadHeader(
        /*[in,string]*/ const TCHAR*  pszPOUName //IEC POU name
    ) = 0;


    //------------------------------------------------------------------*
    /**
     * loadGVL: load the gvl's symbol table or pure lookup if already 
     * loaded and returns a symtab.
     * The returned pointer is valid until the ICG_IECFront is finally 
     * released. The GVL is searched by file name, optionally containig 
     * a full or project relative path. The extension must be .gvl,
     * otherwise an internal compiler error is submitted.
     * A GVL need not be referenced in the 4CP, pure file existence is
     * enough to define a gvl. 
     * Note: The name of the symtab CG_Symtab::pszId will be the canonical
     *       (relative path) file name. e.g. "C:\myProject\./mygvl.gvl" will 
     *       be stored as "c:\myProject\mygvl.gvl". loadGVL can be used 
     *       with any form of the file name, it does not produce a second
     *       'alias' symtab.
     * 
     * Diagnostics: 
     * 1) All errors/warnings during the parse process are submitted.
     * 2) If the GVL file does not exist NO error message is submitted 
     *    and loadGVL returns NULL. The error count is not incremented. 
     *    The caller is responsible to handle this sort of error.
     * 
     * @param    pszGVLFile  the GVL file to load: [path\]<file>.gvl 
     * @return   NULL only if GVL file does not exist, otherwise a symtab 
     *           that may still contain errors see CG_Symtab::flags bits.
     *
     * @see      CG_Symtab       
     */
    virtual const CG_Symtab* STDMETHODCALLTYPE   loadGVL(
        /*[in,string]*/ const TCHAR*     pszGVLFile
    ) = 0;


    //------------------------------------------------------------------*
    /**
     *loadCST: loads a .cst file or look up if already is loaded. 
     * The .cst may or may not be part of the 4cp's [classes.constants] section.
     * 
     * Diagnostics:
     * All errors/warnings during the parse process are submitted, except if
     * file not found error, caller must handle this error.
     * 
     * @param    pszCSTFile       project relative or full file and path
     *                            must have the .cst extension.
     * @param    bAddToGlobCSTs   If true (and no errors on load) all CONST decls
     *                            of the cst are added to the global CONST table,
     *                            this also forces the CONST decls to be unique.
     *                            
     *                            If false: the CONST decls are not checked to be 
     *                            unique within the global table only inside the cst.
     * 
     * @return   NULL if file not, or a CG_Symtab struct that may still 
     *           contain errors (see loadHeader)
     * @see      loadCSTs, loadHeader
     */
    virtual const CG_Symtab* STDMETHODCALLTYPE loadCST(
        /*[in,string]*/ const TCHAR* pszCSTFile,
        /*[in]       */ boolean      bAddToGlobCSTs
    ) = 0;


    //------------------------------------------------------------------*
    /**
     * loadResource: load a resource description or pure lookup if already 
     * loaded and returns a const CG_Resource*.
     * The returned pointer is valid until the ICG_IECFront is finally 
     * released. The resource is searched by file name on disk.
     * All POU headers, not their bodies, of used gvls, programs, FBs and
     * structs are loaded automatically too, this is a recursive process.
     * Functions called from POUs that have a body are not loaded because
     * no program or FB body is loaded. 
     * If any attracted POU has a header with errors, the resource inherits
     * this error condition and loadResource returns a CG_Resource* with the 
     * CG_Resource::bNoError set to false.
     *
     * Diagnostics: 
     * 1) All errors/warnings during the parse process are submitted.
     * 2) All errors/warnings of attracted POU headers are also submitted
     *    if these POUs are loaded first.
     * 3) If the resource file is not found NO error message is submitted. 
     *    and the error count is not incremented.
     *    The caller is responsible to handle this sort of error.     
     *
     * @param    pszResFile:  the resource file name to load, usually 
     *                        obtained from ICG_Config::getResImpByName
     * @return   NULL  only if resource does not exist otherwise a const 
     *           CG_Resource* that may still have errros, see 
     *           CG_Resource::bNoError member.
     *
     * @see      CG_Resource, ICG_Config
     */
    virtual const CG_Resource* STDMETHODCALLTYPE loadResource(
        /*[in,string]*/ const TCHAR* pszResFile
    ) =0;
    
    //------------------------------------------------------------------*
    /**
     *loadBody: loads the body of a POU if any and also the corresponding 
     * symbol table too if not already loaded. If either the body or the
     * header is currently loaded is loadBody performs only a look up.
     * If POU's body was loaded but released is is reparsed are reloaded.
     * 
     * The returned symtab pointer is valid until the ICG_IECFront is 
     * finally released. The returnd symtab is the same object as if 
     * loadHeader would be called. Also the error handling and POU search
     * is the same as descriped in loadHeader. 
     * The returned ICG_Body is valid until is finally released, then
     * the memory of the body is freed, but not the symtab.
     * 
     * Special situation:
     * 1)If the POU has no body because it's a struct or the body consists
     *   only of inline{<text>} macros the out param ppBody is set to NULL,
     *   and only the symtab is returned. This is not considered as an error
     *   or assertion. If a symtab is already loaded for the POU check the
     *   CG_Symtab::bodyLang/::pouTyp members to avoid unnecessary and in 
     *   fact presumably incorrect calls to loadBody.
     * 2)If the POU source file has been modified since it was first 
     *   seen by either a loadHeader or loadBody an error message is
     *   submitted and the old symtab is returned but no body.
     * 
     * @param    pszPOUName  the POU to load or look up header body.
     * @param    codeGenOpt  bitset of CG_LOADBDY_* flags, controls
     *                       special code gen options e.g. tmp var optimisation
     *                       Warning: TODO if the POU body is currently loaded
     *                       but the flags was different no reload happens.
     *                        
     * @param    pBody       out: NULL if no body or any error in 
     *                       header or body.
     * @return   NULL only if POU not defined, otherwise a symtab that may 
     *           still contain errors. If the POU has a body and 
     *           *ppBody is not NULL the symtab is of cource error free.
     *
     * @see      loadHeader, ICG_Body
     */
    virtual const CG_Symtab* STDMETHODCALLTYPE loadBody(
        /*[in,string]*/ const TCHAR*     pszPOUName,
        /*[in]       */ long             codeGenOpt,
        /*[out]      */ ICG_Body**       ppBody
    )  = 0;
    

    //------------------------------------------------------------------*
    /**
     *getMacroBody: returns a description of a non-polymorph inline{} body,
     *              one that has no 'overload' syntax.
     *              NOTE: call this function only if:
     *              (pSym->flags & CG_STAB_POLYMORH) == 0
     *              (pSym->flags & CG_STAB_MACRO)    == 1
     *              (pSym->flags & CG_STAB_HDR_NOERR)== 1
     *
     * @param    pSym  the symbol table of the function or FB with inline body.
     *                        
     * @return   NULL (and assert) if the pSym->flags bits not set as described 
     *           above, otherwise a pointer to a CG_MacroBody struct that 
     *           describes the non poly inline body. The returned pointer has 
     *           the same life time as a CG_Symtab*.
     *
     * @see      CG_MacroBody, CG_Symtab
     */
    virtual const CG_MacroBody* STDMETHODCALLTYPE getNonPolyMacroBody(
        /*[in]*/ const CG_Symtab* pSym
    )  = 0;



    //------------------------------------------------------------------*
    /** TODO is this a good function ??? ICG_Body::getExtBodyInfo() is enough??
     *loadExtBodyInfo: looks up or (re)loads the extended body information 
     * for POU identified by pSym. 
     * look up:  If currently an extended body info for this POU is loaded, that 
     *           object is returned
     * (re)load: If currently no extended body info for this POU is loaded, 
     *           the ICG_IECFront::loadBody is called to create the info
     *           and that possibly newly created body is released. 
     *
     * Note: For performance it is better to load the body first, call
     * the ICG_Body::getExtBodyInfo() method and hold a reference to that
     * body info, this hold reference will allow loadExtBodyInfo always to 
     * look up not a reload.
     * This method is mainly for e.g. for console code generation, where
     * only the extended info is needed. If the body has errors the 
     * return value is NULL.
     * It is not an error to call this function for symtabs that have
     * no body, if so, the return value is NULL.
     * 
     * @param    pSym   a symtab that should have a body, 
     *                  see CG_Symtab::flags and CG_STAB_HAS_BDY
     * @return   An interface where to get e.g. variable body usage info
     *           and called functions. Or NULL if there is no body 
     *           (TODO lib 'virtual' body info?) or the body or hader is 
     *           in error anyway.
     *
     * @see      ICG_ExtBodyInfo, loadBody, ICG_Body
     */
    virtual ICG_ExtBodyInfo* STDMETHODCALLTYPE loadExtBodyInfo(
        /*[in]*/ const CG_Symtab*   pSym
    )  = 0;




//--- parse expressions on the fly:
    //parseExprFromString: usefull for add-on language filters (e.g. PipeNetwork compiler)
    //returns NULL if any error in pszExpr otherwise an ICG_Expr interface.
    virtual ICG_Expr* STDMETHODCALLTYPE parseExprFromString( 
      /*[in]*/ const CG_Edipos*   pEdp,     //must not be NULL
      /*[in]*/ boolean            bIncCol,  //pEdp's line,column info is updated for every diag msg
      /*[in,string]*/const TCHAR* pszExpr,  //expression as string, should not have newlines.
      /*[in,ptr]*/const TCHAR*    pszPOUCtx,//may be NULL, if only constant expr. should be evaluated.
                                            //note: global CONST tables are searched by default.
                                            //if not NULL the corresponding POU's symtab is (loaded)
                                            //if not already done and it's variables can be used
                                            //in the expression.
      /*[in]*/ boolean            bNoWarns  //if true no warnings, usefull for reparse same expr. 
    ) = 0;




//--- symbol table methods: 
    //------------------------------------------------------------------*
    /**
     * get4CLLibIF returns the preloaded library interface
     * for the library identified by a const CG_LIBContext* 
     * The load4CP and loadLIBs must be called before, otherwise raise
     * an internal error. 
     * 
     * @param     pInf     a lib context info for an attached library
     *                     can be retrieved via FCPgetLIBinfo.
     *
     * @return    NULL if the pInf is not one of the 4CP libs (internal 
     *            error diag already submitted)
     *            Or a ready to use library READ ONLY access interface.
     *            loadLIBs opens only for reading. A write operation
     *            on the returned interface will fail.
     *            The caller must Release() if the IF no longer used.
     * @see       loadLIBs, getLIBinfo, I4CLAccess in CG_4CLAccess.h
     */
    virtual I4CLAccess* STDMETHODCALLTYPE get4CLLibIF(const CG_LIBContext* pInf) = 0;


    //------------------------------------------------------------------*
    /**
     * lookUpEntry searches any symtab for an entry by name, the function 
     * does NOT search any #import gvls.
     * 
     * @param     pSym     pointer to symtab where to look up the decl.
     * @param     pszId    name of the decl to lookup
     * @return    if the decl found the corresponding CG_SymEntry 
     *            struct or NULL if not found.
    */
    virtual const CG_SymEntry* STDMETHODCALLTYPE lookUpEntry( 
        /*[in,ref]   */ const CG_Symtab* pSym,
        /*[in,string]*/ const TCHAR*     pszId
    ) = 0;

    
    //------------------------------------------------------------------*
    /**
     * lookUpGlobVar searches a resource's global table for a glob var by
     * name, the function recursively searches also all #import gvls.
     * 
     * @param     pRes     pointer to resouce where to look up the glob.
     * @param     pszId    name of the glob to lookup
     * @return    if the VAR_GLOBAL decl found the corresponding CG_SymEntry 
     *            struct or NULL if not found.
    */
    virtual const CG_SymEntry* STDMETHODCALLTYPE lookUpGlobVar( 
        /*[in,ref]   */ const CG_Resource* pRes,
        /*[in,string]*/ const TCHAR*       pszId
    ) = 0;
    
    //------------------------------------------------------------------*
    /**
     * lookUpCONST searches the global CONST table for an entry by name.
     * 
     * @param     pszId    the name of the CONST to find.
     * @return    if the CONST decl found the corresponding CG_SymEntry 
     *            struct or NULL if not found.
    */
    virtual const CG_SymEntry* STDMETHODCALLTYPE lookUpCONST( 
        /*[in,string]*/ const TCHAR*  pszId
    ) = 0;

    
    //------------------------------------------------------------------*
    /**
     * getNumPrjCST gets the total number of currently loaded cst symtabs.
     *              includes project and all (loaded) library defined cst files.
     * @return    number of cst symtabs loaded.
    */
    virtual long STDMETHODCALLTYPE getNumPrjCST(void) = 0;
    
    //------------------------------------------------------------------*
    /**
     * getPrjCSTbyNum a cst symtab by number
     * @param     n   range: 0..getNumPrjCST()-1
     * @return    the n-th cst symtab
    */
    virtual const CG_Symtab* STDMETHODCALLTYPE getPrjCSTbyNum(long n) = 0;



    //------------------------------------------------------------------*
    /**
     * lookUpTaskInit can be used to find a task init spec by name.
     * Note: this function searches the CG_Task::pInits array and not the 
     * CG_Task::pAttribs.
     * 
     * @param     pTask    task where to lookup the init spec
     * @param     pszInit  the name of the init e.g PRIORITY
     * @return    if the init found the corresponding CG_TaskInit struct
     *            or NULL if not found.
    */
    virtual const CG_TaskInit* STDMETHODCALLTYPE lookUpTaskInit(
        /*[in,prt]*/    const CG_Task* pTask, 
        /*[in,string]*/ const TCHAR*   pszInit
    ) = 0;
    
    //------------------------------------------------------------------*
    /**
     * lookUpAttrib searches for an attribute by name in a CG_AttribList
     * structure.
     * 
     * @param     pAttrlist the list where to search can be NULL too
     * @param     pszKey    the key name to search for
     * @return    if key found the corresponding CG_KeyValPair struct
     *            or NULL if not found.
    */
    virtual const CG_KeyValPair* STDMETHODCALLTYPE lookUpAttrib( 
        /*[in,ptr]   */  const CG_AttribList* pAttrlist,
        /*[in,string]*/  const TCHAR*         pszKey
    ) = 0;


    //------------------------------------------------------------------*
    /**
     * createIECMangeledName build a mangeled name from a symtab. The symtab
     * must be a FUN,FB,PRG or STRUCT. The mangeled name contains the full
     * IEC interface but no initial values and no %IX*.9 incomplete 
     * address info. TODO exmaple of syntax here
     * 
     * @param     pSym    the POU symtab to build the mangeled name from
     * @return            the mangeled name as BSTR or NULL if 
     *                    - symtab is not a FUN,FB,PRG or STRUCT
     *                    - the symtab has error in header
     * @return    the interfce text as BSTR or NULL if 
     *            - symtab in error 
     *            - POU isn't a FUN,FB,PRG or STRUCT
     *            Both errors cause a fatal internal error.
    */
    virtual BSTR STDMETHODCALLTYPE createIECMangeledName( 
        /*[in,ref]*/ const CG_Symtab* pSym
    ) = 0;


    //------------------------------------------------------------------*
    /**
     * getIECHeaderText build a BSTR containing the textual header 
     * portion of a POU. The POU can also be a .cst or .gvl file, in this 
     * simply the whole file as it is will be loaded. If the POU was 
     * previously processed by an IEC-filter the IEC header portion of
     * the file produced by the filter is loaded. TODO: if gvl/cst comments after 
     * END_* are truncated ?? or not ??
     * 
     * @param     pSym    the POU symtab to get the header text from.
     * @param     nBuffer size in TCHARs of supplied buffer, must be 
     *            > pSym->ulHdrBytes/sizeof(TCHAR) + 1.
     * @param     pBuffer receives the header text as 0-terminated string. 
     *
     * @return    false if: 
     *            - symtab in error (raise fatal internal error)
     *            - file IO error 
     *            - source file time changed since it was first seen.
     *            - buffer to short (raise fatal internal error)
     *            All errors are submitted as diags.
    */
    virtual boolean STDMETHODCALLTYPE getIECHeaderText( 
        /*[in,ref]*/ const CG_Symtab* pSym,
        /*[in]    */ long             nBuffer,
        /*[out]   */ TCHAR*           pszBuffer
    ) = 0;




 //--- error handling:
    //------------------------------------------------------------------*
    /**
     * getPredefMsgFmt: backends and filters can resue certain standard 
     * diagnostic messages from CG_Message.h. Typically all diags releated
     * to file-IO, XML parse errors, invalid 4CP class section name, ...
     * can be resused. Reusing a diag means also reusing the online help
     * for this message.
     *
     * @param     hrmsg one of the predefind messages.
     * @return    if hrmsg was found in the resource table the raw format
     *            containing possibly %1 ... %9 inlines. The inlines are 
     *            always text not a printf like %d. See the CG_Message.h
     *            what inlines are needed by a certain diag msg.
     * @see       CG_Message.h     
    */
    virtual BSTR STDMETHODCALLTYPE getPredefMsgFmt(
        /*[in] */HRESULT hrmsg
    ) = 0;


    //------------------------------------------------------------------*
    /**
     * edpToStr: converts a CG_Edipos to a string that is understood 
     * by the 4C engineering and editors. The format of the produced 
     * string is:  edp= <file>'('<line>[,col]')'        for text files or 
     *         or  edp= <file>'('<letter>[,x[,y]...]')' for graphics.
     * 
     * This function is mainly used for error location info previously
     * returned by the front end, but it's also possible to pass a 
     * CG_Edipos structure newly created, in this case you must know
     * the details of CG_Edipos members.
     *
     * @param     pEdp a properly initialised CG_Edipos struct.
     * @param     nBuf length in TCHARs of pszEdp, MAX_PATH is alwasy enough
     * @return    false only if buffer to short.
     * @see       CG_Edipos     
    */
    virtual boolean STDMETHODCALLTYPE edpToStr( 
        /*[ptr][in]*/   const CG_Edipos* pEdp, 
        /*[out]    */   TCHAR*           pszEdp,
        /*[in]     */   long             nBuf
    ) = 0;


    //------------------------------------------------------------------*
    /**
     * getDisplayedErrors: get current number of displayed errors.
     * The front end error count contains all errors directly produced
     * by itself and errors that have been submitted by any language
     * filter called so far. Every loadXXX method may increase the
     * current total error count. But if there are more than e.g 100 errors
     * they will not be displayed any more and getDisplayedErrors()
     * returns always 100.
     * @return     the current sum of displayed errors.
     * @see        getTotalErrors
    */
    virtual long STDMETHODCALLTYPE getDisplayedErrors( void) = 0;

    //------------------------------------------------------------------*
    /**
     * getDisplayedWarnings: get current warnings displayed by the front end.
     * @return     the current total sum of warnings.
     * @see        getTotalErrors, getDisplayedErrors
    */
    virtual long STDMETHODCALLTYPE getDisplayedWarnings( void) =0;

    //------------------------------------------------------------------*
    /**
     * getTotalErrors: get current number of displayed and non displayed 
     * errors, can be used mainly for assertions.
     * @return     the current sum of displayed and non displayed errors.
     * @see        getDisplayedErrors
    */
    virtual long STDMETHODCALLTYPE getTotalErrors( void) = 0;

    //------------------------------------------------------------------*
    /**
     * userMsg: simply print message to the user, userMsg neither 
     * interprets the message text nor increments the error/warning 
     * counters. This function is mainly used by language filters for 
     * convenience only (because they are not processes). Backends 
     * (which are always processes) typically call other tools like make, 
     * C-compiler and can use printf. 
     *
     * @param     pszText: text to dump, new line must be explicitely 
     *            supplied (text is flushed immediately)
     * @return    void 
     */
    virtual void STDMETHODCALLTYPE userMsg(const TCHAR* pszText) = 0;




//--- miscellaneous helper methods:
    //------------------------------------------------------------------*
    /**
     * checkIdentifier
     * Checks for a valid IEC1131-3 Identifier, this means 
     * a) Syntax  identifier := (letter | ('_' (letter|digit))) {['_'] (letter|digit)}
     * b) identifier must not be one of the keywords (ignoring the case). This function 
     *    checks also for all ATCMControl specific keywords, e.g. NATIVE,SYNCHRONIZED,... .
     * c) the maximum length is checked, see the define CG_MAX_ID_LENGTH.
     *
     * @param     pszId   string to be checked for valid IEC1131-3 Identifier
     * @return    One of the following:      meaning
     *            S_OK                   valid identifier
     *            CG_E_IDENT_SYNTAX      the pure syntax is incorrect
     *            CG_E_ID_IS_KEWORD      syntax is ok, but pszId is a keyword
     *            CG_E_ID_TOO_LONG       syntax is ok, but pszId is too long
     * @see  CG_MAX_ID_LENGTH
     */
    virtual HRESULT STDMETHODCALLTYPE checkIdentifier(const TCHAR* pszId)=0;


    //------------------------------------------------------------------*
    /**
     * getDefaultInit can be used to retrieve a CG_Literal struct filled 
     * with IEC default initial value, the one to use if no explicit init val
     * given. Default initial values are defined as 0 for all integral types
     * 0.0 for all float and empty string for string types. The date and time
     * data types the following values are used:
     *   dt#0001-01-01-00:00:00.000    (binary int64 is 0)
     *   tod#00:00:00.000              (binary long  is 0) 
     *   d#0001-01-01                  (binary long  is 0)
     *   t#0ms                         (binary int64 is 0) 
     *
     * @param     elementary the type enum of one of the elementary type
     *                       must not be a struct, fb, array and no QVT_*
                             or ANY_*
     * @return    !=NULL  if elementary is elementary
     *            NULL    if there is no default init val (type is not elementary)
     * @see      CG_Literal, CG_IsElementary, 
     *           ATCMControl documentaion: 'date and time data types'
     */
    virtual const CG_Literal* STDMETHODCALLTYPE getDefaultInit(CG_DTYPES elementary)=0;


    //------------------------------------------------------------------*
    /**
     * getEnumFromTypeKW convets an IEC type keyword to a CG_DTYPES enum
     * representation. 
     * @param    pszSimple   IEC type keyword.
     * @return   A CG_DTYPES enum if is an elementary type or QVT_* 
     *           or one of the ANY_* keywords.
     *           Or CG_DTYPES_UNDEF if pszSimple was not elementary,QVT_*, ANY_*
     * @see      CG_DTYPES
     */
    virtual CG_DTYPES STDMETHODCALLTYPE getEnumFromTypeKW(const TCHAR* pszSimple) = 0;




    //------------------------------------------------------------------*
    /**
     * dtypeFullEqual checks if two CG_DType* data types are 100% equal
     * includes also string length and an R/F_Edge bool is not the same 
     * as a normal bool 
     * @param     pT1    type one 
     * @param     pT2    type two
     * @return    true only if both types are 100% equal
     * @see      CG_DType
     */
    virtual boolean STDMETHODCALLTYPE dtypeFullEqual(
        const CG_DType* pT1, 
        const CG_DType* pT2
    ) = 0;

    //------------------------------------------------------------------*
    /**
     * getArrayBaseType retrieves the array base type of pArrType in form of 
     * a const DG_DType*. This function can only be used if 
     * pArrType->bIsArray==true.
     *
     * @param     pArrType    any array type.
     * @return    if pArrType is an array type: the base type as const DG_DType* 
     *            representation.
     *            if pArrType is not an array returns pArrType itself and 
     *            an assertion.
     * @see      CG_DType
     */
    virtual const CG_DType* STDMETHODCALLTYPE getArrayBaseType(
        const CG_DType* pArrType 
    ) = 0;



    //------------------------------------------------------------------*
    /**
     * checkSourceFile: can be used to simple IEC check a source file, 
     * a source file is one of the predefined st,4gr,gvl,res,.. file types 
     * or there is a filter defined for it in the KAD. The file to be 
     * checked must not necessarily be one of the 4cp's [classes.*] files. 
     * It's searched by file name on disk. All error messages use the project 
     * relative form of pszFile (if possible) as diag file name.
     * checkSourceFile does not check a 4CP file because this must have 
     * been loaded already, it retuns 0 if attempted.
     * The POU name is not checked for multiple definitions. 
     * Note: 
     *    - KAD and 4CP must be loaded first.
     *    - Featureset, libs and CSTs should be loaded first.
     *
     * returns the number of errors for this file 0 means all ok.
     *
     * returns -1 if the file extension is not known, in this case the caller
     * must produce a diag message, e.g using a code like this:
     *
     * CG_InitCMDLEdipos(&edp);
     * CGT_CDiagMsg::msg3(CG_E_BAD_CMDL_FILETYPE, &edp, _T("c"), szCheckFile, pIFront->KADgetSourceExts());
     *
     * Produces a diag:
     *   command line : error C1002: option '-c': argument 'file.txt' must have 
     *       file extension 'st,4gr,sdt,cst,gvl,res,con,pn'
     *
     * @param    pszFile  the source file to check, if not absolute
     *                    it's searched relative to the project directory
     * @return   >=0:     number of error found 
     *           -1 :     file cannot be checked, has unknown file extension
     */
    virtual long STDMETHODCALLTYPE checkSourceFile(
        const TCHAR* pszFile
    ) = 0;
};




//+-------------------------------------------------------------------------
// Interface to get the con file content
EXTERN_C const GUID FAR  IID_ICG_Config;
struct ICG_Config : public IUnknown
{
public:
    //TODO add VAR_CONFIG ACCESS_PATH con info here.
    //TODO comment 
    virtual const TCHAR* STDMETHODCALLTYPE         getName(void)    = 0;
    virtual const CG_Edipos*  STDMETHODCALLTYPE    getEdp(void)     = 0;
    virtual const __int64 STDMETHODCALLTYPE        getFileTime(void)= 0;
    virtual const CG_AttribList* STDMETHODCALLTYPE getAttribs(void) = 0;


    //------------------------------------------------------------------*
    /**
     * getResImpByName: searches the corresponding #import "<resource>.res"
     * by IEC name of resource. The function does not produce any diag msg.
     * 
     * @return  : pointer to CG_ResImpInfo or NULL if resource is undefined
    */
    virtual const CG_ResImpInfo* STDMETHODCALLTYPE getResImpByName(
        const TCHAR* pszResName
    ) = 0;
};



//+-------------------------------------------------------------------------
EXTERN_C const GUID FAR  IID_ICG_Body;
struct ICG_Body : public IUnknown
{
public:
    //get header for convenience...
    virtual const CG_Symtab* STDMETHODCALLTYPE getHeader( void) = 0;
    
    virtual boolean STDMETHODCALLTYPE isSFC( void) = 0;



    //------------------------------------------------------------------*
    /**
     * edpBeg: used for e.g. for special breakpoints and diag msgs.
     * if st: the edp of the fist statement if 4gr: a special edp info
     * @return  : the begin of body edipos
    */
    virtual const CG_Edipos* STDMETHODCALLTYPE edpBeg(void) = 0;
    //------------------------------------------------------------------*
    /**
     * edpEnd: used for e.g. for special breakpoints and diag msgs.
     * if st: the end_* keyword if 4gr: a special end body edp info
     * @return  : the end of body edipos
    */
    virtual const CG_Edipos* STDMETHODCALLTYPE edpEnd(void) = 0;


    //getSTStmts() if not sfc:
    //returns not NULL if empty body, at least one CG_STStmt with
    //CG_STStmt::stype==CG_EMPTY is forced by IEC syntax.
    //returns NULL only if is SFC body.
    virtual const CG_STStmt* STDMETHODCALLTYPE getSTStmts(void) = 0;
    
    //if sfc:
    virtual const CG_SFCStruct* STDMETHODCALLTYPE getSFCStruct(void) = 0;
    //return NULL (and assert) if non SFC or index out of bounds:
    virtual const CG_STStmt*    STDMETHODCALLTYPE getAction(long nAction) = 0;
    //return NULL (and assert) if non SFC or index out of bounds:
    virtual const CG_STStmt*    STDMETHODCALLTYPE getTransi(long nTransi) = 0;
    // returns the body of the sfc exception action, if none exists returns NULL
    virtual const CG_STStmt*    STDMETHODCALLTYPE getExceptionAction() = 0;

    
    //returns >=0 the number of trigger helper variables used in whole body
    //if N is returned, then __trig0..N-1 BOOL variables must be 
    //allocated by the backend.
    virtual long STDMETHODCALLTYPE getUsedTriggers(void) = 0;

    virtual ICG_ExtBodyInfo* STDMETHODCALLTYPE getExtBodyInfo(void) = 0;
};



//+-------------------------------------------------------------------------
// Interface to get extended variable usage info
EXTERN_C const GUID FAR  IID_ICG_ExtBodyInfo;
struct ICG_ExtBodyInfo : public IUnknown
{
public:

    virtual long STDMETHODCALLTYPE getVarExternalWrites(
        const CG_SymEntry* pEntry, 
        const CG_Edipos**  ppEdpArray
    ) = 0;



    //------------------------------------------------------------------*
    /**
     * getSimpleMemberWriteInfo: give summary info which members of a struct
     * or FB instance are written in the body. It does not count any access
     * inside native end_native and any array [] access is stripped (same as 
     * done for XREF). Typically this function can be used for ATCMControl 
     * console code generation.
     * Note that called function block outputs are NOT treatet
     * as written by this function only because the FB was called.
     * Example body:
     *   aFB.a := 4;
     *   x     := aFB.a;
     *   aFB   := aFB2;
     *   aFB3.a.y := x;
     *   aFB3.a.x := x;
     *
     *Call for   : returns:   
     *  aFB      : true
     *  aFB.u    : true    Note:  aFB.u was not assigned but base was written
     *  aFB2     : false 
     *  aFB.a    : true
     *  aFB3     : false
     *  aFB3.a   : false   Note:  aFB3.a was not itself fully written
     *  aFB3.a.y : true
     *  aFB3.a.x : true
     *  aFB3.a.u : false   Note:  aFB3.a.u was not assigned but base was written
     *
     * @param    nPath   >=1: length of entry path.
     * @param    pPath   Entry list that form x.y.z, x must be form this symtab
     *                   and x must be a FB/Struct or array of.
     * @return   true:   The specified pPath gets fully written inside the body.
     *                   If pPath 'x.y.z' and 'x' is a complex type then fully 
     *                   written means:
     *                   a) deep copy to x.y.z
     *                   b) x.y.x passed as var_in_out or function output param
     *                   In other word return false means that there are some
     *                   members not written
     *           false:  path was not assigned in this body,
    */
    virtual boolean STDMETHODCALLTYPE getSimpleMemberWriteInfo(
        long                     nPath,
        const CG_SymEntry*const* pPath/*[nPath]*/ 
    ) = 0;
};



//+-------------------------------------------------------------------------
// Interface for a single on the fly parsed expression, release will 
// free the memory
EXTERN_C const GUID FAR  IID_ICG_Expr;
struct ICG_Expr : public IUnknown
{
public:
    virtual const CG_ExprTree* STDMETHODCALLTYPE getExpr(CG_DTYPES want) = 0;
    virtual boolean            STDMETHODCALLTYPE isLvalue(void)= 0;
};






//+-------------------------------------------------------------------------
//CG_Edipos: use CG_IECFront::edpToStr to convert a CG_Edipos into a string.
//To encode a 'command line' error location init a CG_Edipos with    
//CG_INitCMDLEdipos and pass to the diagMsg functions.
//To display 
typedef struct _CG_GREDP{
    WORD x;  //4gr box  x-pos
    WORD y;  //4gr box  y-pos
    WORD l;  //(l;c): st-line  or second 4gr point
    WORD c;  
} CG_GREDP;
typedef struct _CG_STEDP{
    long l;  //always > line
    long c;  //if >0 colum
} CG_STEDP;
typedef enum _CG_EDPTYP {
    CG_EDP_FILE
   ,CG_EDP_ST
   ,CG_EDP_4GR_BOX
   ,CG_EDP_4GR_STBOX
   ,CG_EDP_4GR_SIGNAL
   ,CG_EDP_4GR_REGION
   ,CG_EDP_4GR_LINE
   ,CG_EDP_4GR_END
   ,CG_EDP_4GR_BEG
   ,CG_EDP_TEXT
} CG_EDPTYP;

struct _CG_Edipos {
    CG_EDPTYP typ; 
    //'typ' determines the usage of pszFile and the union in the following way:
    //
    //typ:            union: format:
    //CG_EDP_FILE       -    if pszFile != NULL
    //                         sprintf(buf,"%s",                pszFile);
    //                       else
    //                         sprintf(buf,"command line");
    //CG_EDP_ST         lc   lc.c==0: 
    //                         sprintf(buf,"%s(%d)",            pszFile,lc.l);
    //                       lc.c >0: 
    //                         sprintf(buf,"%s(%d,%d)",         pszFile,lc.l,lc.c);
    //CG_EDP_4GR_BOX    gr     sprintf(buf,"%s(b,%d,%d)",       pszFile,gr.x,gr.y);
    //CG_EDP_4GR_STBOX  gr     sprintf(buf,"%s(b,%d,%d,%d,%d)", pszFile,gr.x,gr.y,gr.l,gr.c);
    //CG_EDP_4GR_SIGNAL gr     sprintf(buf,"%s(s,%d,%d)",       pszFile,gr.x,gr.y);
    //CG_EDP_4GR_LINE   gr     sprintf(buf,"%s(l,%d,%d,%d,%d)", pszFile,gr.x,gr.y,gr.l,gr.c);
    //CG_EDP_4GR_REGION gr     sprintf(buf,"%s(r,%d,%d,%d,%d)", pszFile,gr.x,gr.y,gr.l,gr.c);
    //CG_EDP_4GR_BEG    -      sprintf(buf,"%s(begin)",         pszFile);
    //CG_EDP_4GR_END    -      sprintf(buf,"%s(end)",           pszFile);
    //
    //CG_EDP_TEXT       psz    sprintf(buf,"%s%s",              pszFile,psz);

    const TCHAR* pszFile; //source file name e.g.  'program1.st', 
                          //'c:\4c\lib\IEC_1131_3.4cl::TON.hdr'"
                          //pszFile may only be NULL if typ == CG_EDP_FILE
                          //then use error location 'command line'
    union /* switch(typ) */
    {
        CG_STEDP     lc; //ST line column
        CG_GREDP     gr; //graphics location
        const TCHAR* psz;//contains all info without delimiters '(' ')'
    };
};




//lists for simple strings and symbol tables, sym entries with edipos:
struct _CG_EdpSymtab {
    CG_Edipos        edp;    //usually source location of the #import
    const CG_Symtab* pSym;   //pointer to symtab, loaded when seen
};
typedef struct _CG_EdpStr
{
    CG_Edipos    edp;       //source location of text
    const TCHAR* psz;       //text data
} CG_EdpStr;

typedef struct _CG_EdpEntry
{
    CG_Edipos          edp;    //source location of sym entry reference
    const CG_SymEntry* pEntry; //pointer to entry that is referenced at edp
} CG_EdpEntry;

struct _CG_POU4CPInfo
{
    CG_Edipos      edp;      //edipos of the file=<value>
    const TCHAR*   pszClass; //The [classes.<class>] e.g. "Programms" 
                             //or for libraries it's "libs"
    const TCHAR*   pszId;    //The POU name as constructed from the file name.
                             //For CST files it's the same as pszFile. 
    const TCHAR*   pszFile;  //The file name as found in 4CP, but converted to
                             //canonical form, all \/..\  are resolved.
                             //May have a full or still project relative path.
};

//Library context info:
struct _CG_LIBContext
{
    CG_Edipos      edp;      //edipos of the 4CP's lib=<libname>.4cl
    const TCHAR*   pszId;    //The lib file name without path but with extension 4cl.
                             //All libraries of the project must be unique by their Ids.
    const TCHAR*   pszFile;  //A possible $(libdir) is expanded
                             //if there was no $(libdir) pszFile may still 
                             //be project relative or has any other full path.
};





//CG_SOURCETYPES: enum for predefined buildin source file types
//the last value CG_SRC_TYPE_ADDON_FILTER is used if a file type
//matches one of the KAD defined input filters:
enum _CG_SOURCETYPES {
    CG_SRC_TYPE_UNDEF = 0
   ,CG_SRC_TYPE_4CP
   ,CG_SRC_TYPE_HDR
   ,CG_SRC_TYPE_CST
   ,CG_SRC_TYPE_SDT
   ,CG_SRC_TYPE_ST
   ,CG_SRC_TYPE_IL
   ,CG_SRC_TYPE_4GR
   ,CG_SRC_TYPE_GVL
   ,CG_SRC_TYPE_RES
   ,CG_SRC_TYPE_CON
   ,CG_SRC_TYPE_ADDON_FILTER
};


//CG_DType: generall data type describtion:
//Note: last enum is used to size arrays in the CG_Features struct
enum _CG_DTYPES {            //mangeled name char
     CG_DTYPES_UNDEF =0
//void is only used for functions with no return value (4C-extension):
    ,CG_VOID                 //0
//bool types the edge  detecting bool is treated as 'basic type'
    ,CG_BOOL_R_EDGE          //a
    ,CG_BOOL_F_EDGE          //a
    ,CG_BOOL                 //a
//the bitstring types:
    ,CG_BYTE                 //b
    ,CG_WORD                 //c
    ,CG_DWORD                //d
    ,CG_LWORD                //e
//the floats:
    ,CG_REAL                 //f
    ,CG_LREAL                //g
//the int types:
    ,CG_SINT                 //h
    ,CG_INT                  //i
    ,CG_DINT                 //j
    ,CG_LINT                 //k
    ,CG_USINT                //l
    ,CG_UINT                 //m
    ,CG_UDINT                //n
    ,CG_ULINT                //o
//time stuff: 
    ,CG_DT                   //p
//  ,CG_DT32                 //q  TODO       
    ,CG_TOD                  //r
    ,CG_DATE                 //s
    ,CG_TIME                 //t
//  ,CG_TIME32               //u  TODO
//string types 8 and 16 bit wide:
    ,CG_STRING               //v
    ,CG_WSTRING              //w

//user defined:
    ,CG_INSTANCE             //1
    ,CG_STRUCT               //2
    ,CG_PROGRAM              //3
//  ,CG_ENUM                 //4  TODO

//QVT for all simple except for the edge detecting bool (4C extension):
    ,CG_QVT_BOOL             //A
    ,CG_QVT_BYTE             //B
    ,CG_QVT_WORD             //C
    ,CG_QVT_DWORD            //D
    ,CG_QVT_LWORD            //E
    ,CG_QVT_REAL             //F
    ,CG_QVT_LREAL            //G
    ,CG_QVT_SINT             //H
    ,CG_QVT_INT              //I
    ,CG_QVT_DINT             //J
    ,CG_QVT_LINT             //K
    ,CG_QVT_USINT            //L
    ,CG_QVT_UINT             //M
    ,CG_QVT_UDINT            //N
    ,CG_QVT_ULINT            //O
    ,CG_QVT_DT               //P
//  ,CG_QVT_DT32             //Q
    ,CG_QVT_TOD              //R
    ,CG_QVT_DATE             //S
    ,CG_QVT_TIME             //T
//  ,CG_QVT_TIME32           //U
    ,CG_QVT_STRING           //V
    ,CG_QVT_WSTRING          //W

    //polymorphic types:
    ,CG_ANY
    ,CG_ANY_NUM
    ,CG_ANY_REAL
    ,CG_ANY_INT
    ,CG_ANY_BIT
    ,CG_ANY_STRING
    ,CG_ANY_DATE
    ,CG_ANY_ELEMENTARY
    ,CG_ANY_MAGNITUDE
    ,CG_ANY_DERIVED
    ,CG_ANY_WAITABLE //##AC extended buildin type: BOOL or a FB with exec-done interface
    ,CG_POLYMORPHMIX //##AC user defined poly mix like TIME|LINT
    ,CG_GOTOLABEL    //##AC extended buildin type GOTOLABEL
};



//alternative basic type representation using
//a Type Bit Set GC_TBS_*  defines:
#define CG_TBS_EMPTY            0x0000000000000000i64
#define CG_TBS_VOID             0x0000000000000001i64
#define CG_TBS_BOOL             0x0000000000000002i64
#define CG_TBS_BYTE             0x0000000000000004i64
#define CG_TBS_WORD             0x0000000000000008i64
#define CG_TBS_DWORD            0x0000000000000010i64
#define CG_TBS_LWORD            0x0000000000000020i64 
#define CG_TBS_REAL             0x0000000000000040i64 
#define CG_TBS_LREAL            0x0000000000000080i64 
#define CG_TBS_SINT             0x0000000000000100i64 
#define CG_TBS_INT              0x0000000000000200i64 
#define CG_TBS_DINT             0x0000000000000400i64 
#define CG_TBS_LINT             0x0000000000000800i64 
#define CG_TBS_USINT            0x0000000000001000i64 
#define CG_TBS_UINT             0x0000000000002000i64 
#define CG_TBS_UDINT            0x0000000000004000i64 
#define CG_TBS_ULINT            0x0000000000008000i64 

#define CG_TBS_DT               0x0000000000010000i64 
#define CG_TBS_TOD              0x0000000000020000i64 
#define CG_TBS_DATE             0x0000000000040000i64 
#define CG_TBS_TIME             0x0000000000080000i64 

#define CG_TBS_STRING           0x0000000001000000i64 
#define CG_TBS_WSTRING          0x0000000002000000i64 

#define CG_TBS_QVT_BOOL         0x0000000010000000i64 
#define CG_TBS_QVT_BYTE         0x0000000020000000i64 
#define CG_TBS_QVT_WORD         0x0000000040000000i64 
#define CG_TBS_QVT_DWORD        0x0000000080000000i64 
#define CG_TBS_QVT_LWORD        0x0000000100000000i64 
#define CG_TBS_QVT_REAL         0x0000000200000000i64 
#define CG_TBS_QVT_LREAL        0x0000000400000000i64 
#define CG_TBS_QVT_SINT         0x0000000800000000i64 
#define CG_TBS_QVT_INT          0x0000001000000000i64 
#define CG_TBS_QVT_DINT         0x0000002000000000i64 
#define CG_TBS_QVT_LINT         0x0000004000000000i64 
#define CG_TBS_QVT_USINT        0x0000008000000000i64 
#define CG_TBS_QVT_UINT         0x0000010000000000i64 
#define CG_TBS_QVT_UDINT        0x0000020000000000i64 
#define CG_TBS_QVT_ULINT        0x0000040000000000i64 

#define CG_TBS_QVT_DT           0x0000100000000000i64 
#define CG_TBS_QVT_TOD          0x0000200000000000i64 
#define CG_TBS_QVT_DATE         0x0000400000000000i64 
#define CG_TBS_QVT_TIME         0x0000800000000000i64 

#define CG_TBS_QVT_STRING       0x0001000000000000i64 
#define CG_TBS_QVT_WSTRING      0x0002000000000000i64 

#define CG_TBS_INSTANCE         0x0010000000000000i64 
#define CG_TBS_STRUCT           0x0020000000000000i64 
#define CG_TBS_PROGRAM          0x0040000000000000i64 
#define CG_TBS_ARRAY            0x0080000000000000i64
#define CG_TBS_ENUM             0x0100000000000000i64  //future use
//specials:
#define CG_TBS_GOTOLABEL        0x1000000000000000i64  //##AC goto label:

//predefined sets, some correspond also to IEC keywords.
typedef unsigned __int64 CG_BITSET;
//IEC Keyword: 
const CG_BITSET CG_TBS_ANY_REAL   =(CG_TBS_REAL
                                  | CG_TBS_LREAL);

const CG_BITSET CG_TBS_ANY_SIGN_INT=(CG_TBS_SINT
                                   |  CG_TBS_INT
                                   |  CG_TBS_DINT
                                   |  CG_TBS_LINT);

const CG_BITSET CG_TBS_ANY_USIGN_INT=(CG_TBS_USINT
                                  | CG_TBS_UINT
                                  | CG_TBS_UDINT
                                  | CG_TBS_ULINT);
//IEC Keyword: 
const CG_BITSET CG_TBS_ANY_INT      =(CG_TBS_ANY_SIGN_INT
                                   |CG_TBS_ANY_USIGN_INT);

//IEC Keyword: 
const CG_BITSET CG_TBS_ANY_NUM    =(CG_TBS_ANY_REAL
                                  | CG_TBS_ANY_INT);
//IEC Keyword: 
const CG_BITSET CG_TBS_ANY_BIT    =(CG_TBS_LWORD
                                  | CG_TBS_DWORD
                                  | CG_TBS_WORD
                                  | CG_TBS_BYTE
                                  | CG_TBS_BOOL);
//IEC Keyword: 
const CG_BITSET CG_TBS_ANY_DATE   =(CG_TBS_DT
                                  | CG_TBS_DATE
                                  | CG_TBS_TOD);

const CG_BITSET CG_TBS_ANY_SIGNED =(CG_TBS_ANY_REAL
                                  | CG_TBS_ANY_SIGN_INT
                                  | CG_TBS_TIME);
//IEC Keyword: 
const CG_BITSET CG_TBS_ANY_STRING =(CG_TBS_STRING
                                  | CG_TBS_WSTRING);
//IEC Keyword: 
const CG_BITSET CG_TBS_ANY_ELEMENTARY=(CG_TBS_ANY_NUM
                                  | CG_TBS_ANY_BIT
                                  | CG_TBS_ANY_STRING
                                  | CG_TBS_ANY_DATE
                                  | CG_TBS_TIME);
//IEC Keyword: 
const CG_BITSET CG_TBS_ANY_DERIVED=(CG_TBS_ARRAY
                                  | CG_TBS_INSTANCE
                                  | CG_TBS_STRUCT);
//IEC Keyword: 
const CG_BITSET CG_TBS_ANY_MAGNITUDE=(CG_TBS_ANY_NUM
                                  | CG_TBS_TIME);

const CG_BITSET CG_TBS_ANY_QVT    =(CG_TBS_QVT_BOOL
                                  | CG_TBS_QVT_BYTE
                                  | CG_TBS_QVT_WORD
                                  | CG_TBS_QVT_DWORD
                                  | CG_TBS_QVT_LWORD
                                  | CG_TBS_QVT_REAL
                                  | CG_TBS_QVT_LREAL
                                  | CG_TBS_QVT_SINT
                                  | CG_TBS_QVT_INT
                                  | CG_TBS_QVT_DINT
                                  | CG_TBS_QVT_LINT
                                  | CG_TBS_QVT_USINT
                                  | CG_TBS_QVT_UINT
                                  | CG_TBS_QVT_UDINT
                                  | CG_TBS_QVT_ULINT
                                  | CG_TBS_QVT_DT
                                  | CG_TBS_QVT_TOD
                                  | CG_TBS_QVT_DATE
                                  | CG_TBS_QVT_TIME
                                  | CG_TBS_QVT_STRING
                                  | CG_TBS_QVT_WSTRING);
//IEC Keyword: 
const CG_BITSET CG_TBS_ANY_SIMPLE =(CG_TBS_ANY_ELEMENTARY
                                  | CG_TBS_ANY_QVT); 

//IEC Keyword: 
const CG_BITSET CG_TBS_ANY        = (CG_TBS_ANY_SIMPLE|CG_TBS_ANY_QVT|CG_TBS_ANY_DERIVED|CG_TBS_ENUM);//##AC

const CG_BITSET CG_TBS_ALLBITS    = 0xffffffffffffffffi64;  //##AC



struct _CG_Subrange {
    long          lRangeStart;  // first element of range
    unsigned long ulRangeLength;// number of elements in range - 1
                                //Examples:
                                // [-1..2]: lRangeStart   == -1
                                // [-1..2]: ulRangeLength == +3
                                // [0..0] : lRangeStart   == 0
                                // [0..0] : ulRangeLength == 0
};




#define CG_MAX_ARRAY_DIM  1
struct _CG_DType {
    CG_DTYPES type; //type enum, if bIsArray==true it is the array base type
    CG_BITSET tbs;  //alternate type representation as bitset.
                    //The bitset always is the same information as the type enum,
                    //except for the R/F_EDGE, this is not contained in the bitset.
                    //The CG_TBS_ARRAY is never set here, it's always the array base type.

    //pSym always points to the Program,FB or Struct symtab if
    //type==CG_[PROGRAM|INSTANCE|STRUCT]
    const CG_Symtab* pSym; 
    //Note: the CG_DType::pSym->flags  CG_STAB_HDR_NOERR bit is always set.
    //If a POU header has errors no CG_DType is created from that POU.

    //lStrLen only used if type==CG_[QVT_][W]STRING (-1 otherwise)
    long             lStrLen;  //-1: means no [len] specified on type decl
                               //>0: otherwise, string[0] decl is treated as an error.
                               //=0: is a possible value, but not from a type decl
                               //    it's needed only for an empty "" string literal
                               //    expression tree node type.
    
    boolean bIsArray;          //true only if type has arrayness.

    //next five members only set if bIsArray==true (set to 0 otherwise):
    long             nElems;   //Total number of elements of the array even if more
                               //dimensional array.
                               //If the array is assumed size nElems is 0.
    long             nDim;     //0<=nDim<CG_MAX_ARRAY_DIM number of dimensions
                               //nDim is 0 for assumed size one dimensional arrays
                               //Note: don't use nDim==0 to test for assumed size array 
                               //use nElems==0 instead.
                               //Reason: future versions may support fortran like 
                               //assumed size arrays like ARRAY[2..6,1..*] in this 
                               //case nDim will be 2.
    CG_Subrange      range[CG_MAX_ARRAY_DIM];  //array of subranges size_is nDim

    

    //for convenience a type string representaion
    const TCHAR*     pszName; 
    //it's based on IEC format with all keywords uppercase
    //and user defined types are spelled exactly (including the case) 
    //as pSym->pszId. Examples:
    //"DINT", "STRING", "STRING[23]", "ANY_INT"
    //"ARRAY OF BOOL"  (assumed size array)
    //"ARRAY[-2..8,0..3] OF myStruct"
    //"ARRAY[-2..8] OF WSTRING[12]"
};






//expression description:
// Notes: 
//last enum is used to size arrays in the CG_Features struct
//CG_NOOP: no operation is used for '(<expr>)' convenient
//         to dump high level language like C, java.
//CG_MOVE: only used for for-loop embeeded ':=' 
//         all other assigns are a CG_STStmt 
enum _CG_EXPRTYP
{
     CG_EXPRTYP_UNDEF =0
    //unary ops:
    ,CG_NOT ,CG_UMINUS ,CG_NOOP
    //binary ops:
    ,CG_OR ,CG_XOR, CG_AND
    ,CG_EQ ,CG_NE , CG_LT , CG_LE , CG_GT , CG_GE  
    ,CG_ADD,CG_SUB, CG_MUL, CG_DIV, CG_MOD, CG_POW //the ** 
    ,CG_MOVE      // :=
    //Primary:
    ,CG_LITERAL
    ,CG_VARREF
    ,CG_STRUCTREF
    ,CG_ARRAYREF
     //function call types:
    ,CG_FUNCALL
    ,CG_FUNMACRO
    ,CG_BUILDIN
    //tmp vars in the expr tree only used for LD/FBD
    ,CG_TMPVAR
    //presistent 'tmp vars' in the expr tree only used for LD contacts/coils
    ,CG_TRIGVAR
    //only for sfc context:
    ,CG_STEPACTION
    //user defined expr node
    ,CG_USEREXPR
    ,CG_FBCALLEXPR  //##AC
    ,CG_LABELREF    //##AC
};


typedef enum _CG_BUILDINS
{
    CG_BI_QUALITYOF, //DWORD(ANY_QVT)
    CG_BI_VALUEOF,   //XXX(QVT_XXX)
    CG_BI_QVT_XXX    //QVT_XXX(DOWRD,XXX,DT)  
                     //XXX=BOOL|BYTE|WORD|DWORD|LWORD|
                     //    REAL|LREAL
                     //    SINT|INT|DINT|LINT
                     //    USINT|UINT|UDINT|ULINT
                     //    DT|TOD|DATE|TIME|STRING|WSTRING
};



//The CG_[Var|Struct|Array|TmpVar]Expr::accFlags member has the 
//following access "used as" bits. The CG_SymEntry::accFlags will be 
//the bitwise-or of all accesses to the entry inside the body (after initial 
//loadBody). The CG_SymEntry::accFlags also contains the indirect accesses
//from all reference tmp vars that reference the entry, see CG_TmpVarExpr.  
#define CG_ACCESS_R  0x0001 // gets read
#define CG_ACCESS_r  0x0002 //'partially R'

#define CG_ACCESS_W  0x0004 //gets written
#define CG_ACCESS_w  0x0008 //'partially W'

#define CG_ACCESS_X  0x0010 //gets R and W, only set for expr. passed to Fun/FB inout params.
#define CG_ACCESS_x  0x0020 //'partially X'

#define CG_ACCESS_I  0x0040 //is passed as function (never FB) VAR_INPUT argument
#define CG_ACCESS_i  0x0080 //'partially I'

#define CG_ACCESS_O  0x0100 //is passed as function (never FB) VAR_OUTPUT argument
#define CG_ACCESS_o  0x0200 //'partially O'

#define CG_ACCESS_C  0x0400 //gets called, FB type expressions only
#define CG_ACCESS_c  0x0800 //'partially C'

#define CG_ACCESS_A  0x1000 //the address of an expr. was taken, this happens only
                            //in the case "<ref. tmp> := &<l-value>", currently LD/FBD only.
                            //the A-bit does not mean R,W,X,... , later when such 
                            //a ref. tmp var is used the actual access bits must be 
                            //added to the l-value expr. that is referenced (see CG_TmpVarExpr)
#define CG_ACCESS_a  0x2000 //'partially A'

#define CG_ACCESS_N  0x4000 //gets used literally inside a NATIVE END_NATIVE (scanned for ID-syntax) 
                            //no idea if R or W or ?. This Bit is only set
                            //in CG_SymEntry::accFlags not for any expression.
                            //see also the comments in CG_StmtNative struct.
#define CG_ACCESS_B  0x8000 //gets used as base of field selector "[]" or "."


//Any potential read/write access: 
//Note: 
// Use these bit sets to detect any potential R/W access.
// A FB call (Cc) may read/write the instance and the address of (Aa)
// is considered as a potential read/write too:
const WORD CG_ACCESS_ANY_W  = (CG_ACCESS_W|CG_ACCESS_w|
                               CG_ACCESS_O|CG_ACCESS_o|
                               CG_ACCESS_X|CG_ACCESS_x|
                               CG_ACCESS_C|CG_ACCESS_c|
                               CG_ACCESS_A|CG_ACCESS_a|
                               CG_ACCESS_N);
const WORD CG_ACCESS_ANY_R  = (CG_ACCESS_R|CG_ACCESS_r|
                               CG_ACCESS_I|CG_ACCESS_i|
                               CG_ACCESS_X|CG_ACCESS_x|
                               CG_ACCESS_C|CG_ACCESS_c|
                               CG_ACCESS_A|CG_ACCESS_a|
                               CG_ACCESS_N);


//Note: The R/W/X/I/O/C/A bits propagate down any struct or array selector and 
//      change then to r/w/x/i/o/c/a before stored in the base expression(s)'s
//      accFlags, except for arrays with non literal subscripts, then 
//      then R/W/X/I/O/C/A write through. 
//      If one of the 'potential' bits is set then the CG_ACCESS_B bit is 
//      always also set.
//
//Examples:
//a) a[i]:= 2:    
//   <SubExpr>        <SubExpr>->exTyp:    <SubExpr>->accFlag: 
//   a[i]             CG_ARRAYREF          CG_ACCESS_W
//   i                CG_VARREF            CG_ACCESS_R
//   a                CG_VARREF            CG_ACCESS_W|CG_ACCESS_B
//
//b) fun(a[45]):    
//   <SubExpr>        <SubExpr>->exTyp:    <SubExpr>->accFlag: 
//   a[45]            CG_ARRAYREF          CG_ACCESS_I
//   a                CG_VARREF            CG_ACCESS_i|CG_ACCESS_B
//
//c) a[34].member:= 2:    
//   <SubExpr>        <SubExpr>->exTyp:    <SubExpr>->accFlag: 
//   a[34].member     CG_STRUCTREF         CG_ACCESS_W
//   a[34]            CG_ARRAYREF          CG_ACCESS_w|CG_ACCESS_B
//   a                CG_VARREF            CG_ACCESS_w|CG_ACCESS_B
//
//d) a[i].member := 2:  
//   <SubExpr>        <SubExpr>->exTyp:    <SubExpr>->accFlag: 
//   a[i].member      CG_STRUCTREF         CG_ACCESS_W
//   i                CG_VARREF            CG_ACCESS_R
//   a[i]             CG_ARRAYREF          CG_ACCESS_w|CG_ACCESS_B
//   a                CG_VARREF            CG_ACCESS_w|CG_ACCESS_B
//
//Note:   
//   Once the 'X->x' convertion has been made for a base expr. it will not 
//   be changed from 'x->X', even not for arrays with non literal subscripts,
//   see example c). In other words: "a[i].member := 2" is considerd as only 
//   a potential write of the whole array since only one member of the 
//   struct was assigned. 
//
//e) FB(inout := x[4].y[s.m].z)
//   <SubExpr>        <SubExpr>->exTyp:    <SubExpr>->accFlag:
//   FB               CG_VARREF            CG_ACCESS_C (see Note)
//   x[4].y[s.m].z    CG_STRUCTREF         CG_ACCESS_X
//   x[4].y[s.m]      CG_ARRAYREF          CG_ACCESS_x|CG_ACCESS_B
//   s.m              CG_STRUCTREF         CG_ACCESS_R
//   s                CG_VARREF            CG_ACCESS_r|CG_ACCESS_B
//   x[4].y           CG_STRUCTREF         CG_ACCESS_x|CG_ACCESS_B
//   x[4]             CG_ARRAYREF          CG_ACCESS_x|CG_ACCESS_B
//   x                CG_VARREF            CG_ACCESS_x|CG_ACCESS_B
//
//Note:   
//   Here the FB instance was called like FB(), the inout param passing 
//   is assumed not to change the instance relevant data, therefore
//   the CG_StmtCallFB::pInst expr. gets only the CG_ACCESS_C flag
//   
//f) FB(in:=2, out:=var)
//   <SubExpr>        <SubExpr>->exTyp:    <SubExpr>->accFlag:
//   FB               CG_VARREF            CG_ACCESS_C|CG_ACCESS_w|CG_ACCESS_r
//   var              CG_VARREF            CG_ACCESS_W
//Note
//   FB.in and FB.out are not directly stored as expr. (see CG_StmtCallFB)




//types of <step name>.member and <action name>.member:
typedef enum _CG_SFCEXPR_TYP
{
    CG_SFCEXPR_TYP_UNDEF
   ,CG_STEP_X    //step.X 
   ,CG_STEP_T    //step.T
   ,CG_ACTION_A  //action.A 
   ,CG_ACTION_Q  //action.Q
} CG_SFCEXPR_TYP;

typedef struct _CG_LitExpr {
    const CG_Literal*  pLit;  //always !=NULL
    const CG_SymEntry* pConst;//!=NULL if lit was a CONST
} CG_LitExpr;

typedef struct _CG_VarExpr {
    WORD               accFlags;//access flags a set of CG_ACCESS_*
    const CG_SymEntry* pEntry;  //points to the entry of the body's symtab
} CG_VarExpr;

typedef struct _CG_StructExpr{
    WORD               accFlags;//access flags a set of CG_ACCESS_*
    const CG_ExprTree* pBase;//base expr., always of type FB or struct.
    const CG_SymEntry* pSelc;//entry of the field selector in the 
                             //the base expr.'s type symtab.
} CG_StructExpr;

typedef struct _CG_ArrayExpr{
    const CG_ExprTree* pBase; //base expression of type CG_ARRAY
    WORD               accFlags;//access flags a set of CG_ACCESS_*
    WORD               nSubs; //always >0
    const CG_ExprTree* pSubs; //the subscripts size is nDims.
} CG_ArrayExpr;

typedef  struct _CG_OpExpr {
    //all other are build in bin ops:
    const CG_ExprTree* pLhs; //always !=NULL
    const CG_ExprTree* pRhs; //NULL if unary: CG_NOT,CG_UMINUS,CG_NOOP
} CG_OpExpr;

struct _CG_MacroBody
{
    const CG_Symtab*   pSym;  //symbol table of called function
    CG_Edipos          edp;   //edipos of macro text
    const TCHAR*       pszMac;//the text of the macro
};

typedef struct _CG_FunExpr {
    const CG_Symtab*   pSym; //symbol table of called function
    long               nArgs;//>=0 number of arguments
    const CG_ArgExpr*  pArgs;//NULL if nArgs==0 or array of arguments
} CG_FunExpr;

typedef struct _CG_MacroExpr {
    const CG_MacroBody*pMac ;//describtion of function body (inline) macro
    long               nArgs;//>=0 number of arguments
    const CG_ArgExpr*  pArgs;//NULL if nArgs==0 or array of arguments
} CG_MacroExpr;

typedef struct _CG_BuildInExpr {
    const TCHAR*        pszId;  //name of buildin function e.g. QVT_BOOL,QualityOf
    long                nArgs;  //>=0 number of arguments
    const CG_ExprTree*  pArgs;  //NULL if nArgs==0 or array of arguments
} CG_BuildInExpr;

typedef struct _CG_TmpVarExpr {  //Tmp vars are only written once either via CG_ASSIGN
                                 //or as function/FB output. Tmp var numbers are
                                 //not be recycled inside networks but accross networks.
    WORD               accFlags; //access flags a set of CG_ACCESS_*
    long               lTmpNum;  //always >0 the number of the tmp
    const CG_ExprTree* pRefExpr; //!=NULL if the tmp var holds only a reference
                                 //of the l-value expression that gets assigned 
                                 //to this tmp var. Reference tmp vars are only 
                                 //initialised via CG_ASSIGN, never by function/
                                 //FB output or somehow else. And (as all tmp vars)
                                 //ref tmp vars never change the item they are 
                                 //referencing during their life time (=LD/FBD Network).
                                 
} CG_TmpVarExpr;

#define CG_TMP_R_TRIG  0x0001
#define CG_TMP_F_TRIG  0x0002
typedef struct _CG_TrigVarExpr {
    long    lTrigNum;       //always >=0 the number of the trig, trig vars are
                            //not recycled. lTrigNum increases from top
                            //to bottom of the body by +1. ICG_Body::getUsedTriggers()
                            //can be used to get the total number of triggers.
    WORD    trigTyp;        //a bitset of CG_TMP_* bits.
} CG_TrigVarExpr;

typedef struct _CG_SFCExpr
{
    CG_SFCEXPR_TYP   typ;    //the sfc struct typ expr kind
    long             iSANum; //>=0: the ordinal number of the Step or Action
                             //see e.g. ICG_Body::getAction
    const TCHAR*     pszSA;  //symbolic name of Step or Action
} CG_SFCExpr;

//advanced use the CG_Exprtree objects cretaed by backend (typically on stack):
typedef struct _CG_UserExpr {
    long               ldata;  //user defined long data
    union {
        const CG_ExprTree* pExpr1;
        const TCHAR*       psz1;
        void*              pvData1;
    };
    union {
        const CG_ExprTree* pExpr2;
        const TCHAR*       psz2;
        void*              pvData2;
    };

} CG_UserExpr;


typedef struct _CG_FBCallExpr { 
    const CG_ExprTree*  pInst;//strict IEC: always a CG_VARREF, future  //##AC
                              //version may support array of FBs        //##AC
    long                nArgs;//>=0 number of in/out assignments        //##AC
    const CG_ArgExpr*   pArgs;//NULL or array of arg expr.              //##AC
}  CG_FBCallExpr;                                                       //##AC

typedef struct _CG_LabelRefExpr {                                       //##AC
    long                nLblNum; //number of the label order            //##AC 
                                 //as seen in body >=0                  //##AC
    const TCHAR*        pszLbl;  //name of the label                    //##AC
} CG_LabelRefExpr;                                                      //##AC

struct _CG_ExprTree {
    CG_EXPRTYP      extyp;
    const CG_DType* pType;
    CG_Edipos       edp;

    union /*switch(extyp)*/ 
    { 
    /*case CG_LITERAL:   */  CG_LitExpr      lit;
    /*case CG_VARREF:    */  CG_VarExpr      var;
    /*case CG_STRUCTREF: */  CG_StructExpr   str;
    /*case CG_ARRAYREF:  */  CG_ArrayExpr    arr;
    /*case CG_FUNCALL:   */  CG_FunExpr      fun;
    /*case CG_FUNMACRO   */  CG_MacroExpr    mac;
    /*case CG_BUILDIN    */  CG_BuildInExpr  bin;
    /*case CG_TMPVAR:    */  CG_TmpVarExpr   tmp;
    /*case CG_TRIGVAR:   */  CG_TrigVarExpr  trg;
    /*case CG_STEPACTION */  CG_SFCExpr      sfc;
    /*case CG_USEREXPR:  */  CG_UserExpr     usr;
    /*case CG_FBCALLEXPR */  CG_FBCallExpr   fbc;  //##AC
    /*case CG_LABELREF   */  CG_LabelRefExpr lbl;  //##AC

    /*default:           */  CG_OpExpr       ops;
    }; //end union;
};


//param passing flags:
#define CG_EXPR_ARG_NEGOUT  0x0010  //used for LD/FBD outputs that are negated
                                    //will only be used for FB calls, neg. 
                                    //function outputs are handeled with tmp vars.
#define CG_EXPR_ARG_OUTASN 0x0001   //argument output syntax FBI(<formal> => <actual>) 
                                    //syntax was used or in LDFBD is on the right side.

typedef struct _CG_ArgExpr {
    const CG_SymEntry* pFormal; //pointer to formal param of FB/Fun's symtab
                                //all VAR_IN_EXT args point to the same sym entry
    WORD               argFlags;//CG_EXPR_ARG_NEGOUT
                                //CG_EXPR_ARG_OUTASN
    const CG_ExprTree* pActual; //actual value
} CG_ArgExpr;







// Note: last enum is used to size arrays in the CG_Features struct
enum _CG_STMTTYP {  //All ST statements
     CG_STMTTYP_UNDEF =0
    ,CG_ASSIGN
    ,CG_CALLVOIDFUN //used only for function call stmt. with no return value
    ,CG_CALLFB      //normal function block call     
    ,CG_CASE
    ,CG_IF
    ,CG_REPEAT
    ,CG_WHILE
    ,CG_FOR
    ,CG_GOTO
    ,CG_LABEL
    ,CG_EXIT
    ,CG_RETURN
    ,CG_EMPTY        //the pure ';' here only to allow a possible break point
    ,CG_NATIVE       //4C extension
    ,CG_SYNCHRONIZED //4C extension 
    ,CG_LDFBDNETWORK //devides a graphics body into networks, can be used for 
                     //optimisations and breakpoints.
}; 



typedef struct _CG_STCaseList {
    CG_Edipos             edp;
    const _CG_STCaseList* pNextCase;
    const CG_STStmt*      pStmts;

    long        nRanges; 
    CG_Subrange ranges[];
} CG_STCaseList;



typedef struct _CG_StmtAssign {
    const CG_ExprTree* pLhs;  //special case NULL: transition assignment in SFC
    const CG_ExprTree* pRhs;  //never NULL
} CG_StmtAssign;

typedef struct _CG_StmtCallFun {
    const CG_ExprTree* pExpr;
} CG_StmtCallFun;

typedef struct _CG_StmtCallFB { 
    const CG_ExprTree*  pInst;//strict IEC: always a CG_VARREF, future 
                              //version may support array of FBs
    long                nArgs;//>=0 number of in/out assignments
    const CG_ArgExpr*   pArgs;//NULL or array of arg expr.
    const CG_MacroBody* pMac; //NULL or the macro text of the 
                              //FB if it has an inline{...} body
} CG_StmtCallFB;

typedef struct _CG_StmtCase { 
    const CG_ExprTree*  pExpr;
    const CG_STCaseList*pCases;
    const CG_STStmt*    pElse;
} CG_StmtCase;

typedef struct _CG_StmtIf { 
    const CG_ExprTree*  pCond;
    const CG_STStmt*    pThen;
    const CG_STStmt*    pElse;
} CG_StmtIf;

typedef struct _CG_StmtRepeat { 
    const CG_STStmt*    pStmts;
    const CG_ExprTree*  pUntil;
} CG_StmtRepeat;

//case CG_WHILE:
typedef struct _CG_StmtWhile { 
    const CG_ExprTree*  pWhile;
    const CG_STStmt*    pStmts;
} CG_StmtWhile;

typedef struct _CG_StmtFor { 
    const CG_ExprTree*  pAssign;
    const CG_ExprTree*  pToExpr;
    const CG_ExprTree*  pByExpr;
    const CG_STStmt*    pStmts;
} CG_StmtFor;

typedef struct _CG_StmtGoto { 
    long                bUpward; //true if upward jump
    const CG_STStmt*    pLblStmt;//points to the correspondint label stmt
} CG_StmtGoto;

typedef struct _CG_StmtLabel { 
    long                bUsed;   //set true only if used by goto or label ref expression ##AC
    long                nLblNum; //number of the label order 
                                 //as seen in body >=0
    const TCHAR*        pszLbl;  //name of the label
} CG_StmtLabel;

typedef struct _CG_StmtNative { 
    const TCHAR*        pszNative;
    long                nRefEntries; //>=0: size of pRefEntries
    const CG_EdpEntry*  pRefEntries; //NULL or a list all entries that are
                                     //potentially used in this native block.
                                     //The block is scanned for the following 
                                     //syntax: '_'<IEC-identifier>, if such a 
                                     //string is also a sym entry (no #import gvls!)
                                     //the entry and its edipos it stored.
                                     //The entry get also the CG_ACCESS_N bit set
                                     //in its accFlags member.
} CG_StmtNative;

typedef struct _CG_StmtSync { 
    const CG_STStmt*    pStmts;
} CG_StmtSync;

typedef struct _CG_StmtNW {
    long                lang; //one of CG_STAB_BDY_IS_FBD or CG_STAB_BDY_IS_LD
    const CG_STStmt*    pStmts;
} CG_StmtNW;

struct _CG_STStmt {
    CG_STMTTYP       styp;
    CG_Edipos        edp;
    const CG_STStmt* pNext;
    
    union /*switch(stmt) */ 
    {
    /*case CG_ASSIGN:      */  CG_StmtAssign  asns;
    /*case CG_CALLVOIDFUN: */  CG_StmtCallFun funs;
    /*case CG_CALLFB:      */  CG_StmtCallFB  fbcs;
    /*case CG_CASE:        */  CG_StmtCase    cases;
    /*case CG_IF:          */  CG_StmtIf      ifs;
    /*case CG_REPEAT:      */  CG_StmtRepeat  reps;
    /*case CG_WHILE:       */  CG_StmtWhile   whls;
    /*case CG_FOR:         */  CG_StmtFor     fors;
    /*case CG_GOTO:        */  CG_StmtGoto    gots;
    /*case CG_LABEL:       */  CG_StmtLabel   lbls;
    /*case CG_EXIT:        */  //no special   data;
    /*case CG_RETURN:      */  //no special   data;
    /*case CG_EMPTY:       */  //no special   data;
    /*case CG_NATIVE:      */  CG_StmtNative  nats;
    /*case CG_SYNCHRONIZED:*/  CG_StmtSync    syns;
    /*case CG_LDFBDNETWORK:*/  CG_StmtNW      netw;
    };
};


typedef enum _CG_LIT_UT {
  //union type:   //IEC type:
    CG_LIT_NONE   //use psz only: STRING,WSTRING
   ,CG_LIT_I64    //ANY_INT, ANY_DATE, ANY_BIT
   ,CG_LIT_FLT    //REAL, LREAL
} CG_LIT_UT;

typedef struct _CG_Literal
{
    CG_DTYPES    type; //the (non polymorph) IEC type
    CG_LIT_UT    ut;   //redundant, but easier union switch enum
    const TCHAR* psz;  //normalised literal, see following description: 

    //if type is ANY_INT:
    //    format: [-]{digit}+   (no embedded '_')
    //    the psz can be directly passed to C-runtime _i64toa or _ui64toa
    //    The bin val union i64 is exactly the value from _i64toa or _ui64toa.
    //    if the type is unsigned use a (unsigned <type>)i64 cast.
    //
    //if type is REAL|LREAL:
    //    format: [+|-]34.45e2  (no embedded '_')
    //    The psz can be directly passed to C-runtime strtod. 
    //    The bin val union dbl is exactly this value form strtod.
    //
    //if type is STRING|WSTRING:
    //    original IEC string literal including the delimiters
    //
    //if type is BOOL:
    //    format:  true|false 
    //    The bin val union i64 is 0 or 1
    //
    //if type is TIME:
    //    format: #t[-]{digit}+ms, e.g. t#-45ms
    //    the time is represented in ms, currently no fraction
    //    ms is the smallest time unit that can be represented.
    //    The bin val union i64 is a signed time value in ms
    //
    //if type is TOD:
    //  format:  "tod#00:00:00.000"
    //  The bin val union i64 is >=0 the ms since midnight
    //
    //if type is DATE:
    //  format:  "d#0001-01-01"
    //  The bin val union i64 is >=0 the days since 1970,
    //  including leap year correction. 
    //
    //if type is DT:
    //  format  "dt#0001-01-01-00:00:00.000"
    //  The bin val i64 is >=0 UTC ms since 1970.
    //  Note: The converion from the user input literal to 
    //  psz and the bin val i64 depends on the local time zone 
    //  settings and also time of the compilation.

    //binary values:
    union//switch(ut)
    {
    //case CG_LIT_I64
        __int64  i64; 
    //case CG_LIT_FLT
        double   dbl;   //obtained by C-runtime function strtod
    };

} CG_Literal;

typedef struct _CG_SimpleInit {
    const CG_Literal*   pLit;   //never NULL, point to a lit struct
    const CG_SymEntry*  pConst; //!=NULL if lit was a CONST
} CG_SimpleInit;

typedef struct _CG_StructInit
{
    long                     nInits;
    //[size_is(ulInits)] 
    const CG_SymEntry*const* pEntries;
    //[size_is(ulInits)] 
    const CG_Initialiser*    pInits;        
} CG_StructInit; 

typedef struct _CG_ArrayInit
{
    long                  nInits;
    //[size_is(ulInits)] 
    const long*           pRepeats;
    //[size_is(ulInits)] 
    const CG_Initialiser* pInits;
} CG_ArrayInit;



typedef enum _GC_INITIALISER_UT {
    CG_INIT_SIMPLE
   ,CG_INIT_STRUCT
   ,CG_INIT_ARRAY
   ,CG_INIT_NONE   //used only for array init feature 4()
} GC_INITIALISER_UT;

struct _CG_Initialiser
{ 
    GC_INITIALISER_UT ut; //union type

    union /*switch(ut)*/ {
    /*case CG_INIT_SIMPLE:*/ CG_SimpleInit  sim;    
    /*case CG_INIT_STRUCT:*/ CG_StructInit  str; 
    /*case CG_INIT_ARRAY: */ CG_ArrayInit   arr;
    /*case CG_INIT_NONE   */              //nix
    };
};

//CG_Attributes: content of (*[[<tag>=<val>,...]]*) special comments
//4C defined tags:      version, UserText<num>, Alias, UserURL
//target defined tags: <targettype>.<id>, e.g. 'PC.javaclass'
struct _CG_KeyValPair {
    CG_Edipos    edpKey;  //source location of key
    const TCHAR* pszKey;  //attribute key name 

    CG_Edipos    edpVal;  //source location of Value
    const TCHAR* pszVal;  //attribute value; leading,trailing whitespace stripped
};
struct _CG_AttribList {
    long            nAttribs;
    //[size_is(nAttribs)]
    CG_KeyValPair   pairs[];
};



//CG_SymEntry: the records of a symbol table:
// defines for bitset CG_SymEntry::declMod 
//IEC related flags:
#define CG_DMOD_RETAIN       0x01
#define CG_DMOD_NON_RETAIN   0x02
#define CG_DMOD_CONSTANT     0x04
#define CG_DMOD_ANY_RETAIN   (CG_DMOD_RETAIN|CG_DMOD_NON_RETAIN)
//special flags:
#define CG_DMOD_SYS_VAR      0x10   //variable was defined using the IAutoDecl IF
#define CG_DMOD_OPTIONAL     0x20   //optional function/FB parameter, same asin C++ ##AC
#define CG_DMOD_SYS_ADDR     0x40   //forosai: TODO we need this bit only to change the 
                                    //frontend IF header in a binary compatible way,
                                    //only if this bit is set the pszSysAddr member may be accessed
                                    //older versions of _CG_SymEntry don't have this member!!
									

// Note: CG_VAR_NATIVE not used in CG_SymEntry::declScope
//       but sometimes convenient to have just a enum for it.
enum _CG_DECLSCOPE {//All variable declaration scopes  
     CG_DECLSCOPE_UNDEF =0
    ,CG_VAR
    ,CG_VAR_INPUT
    ,CG_VAR_OUTPUT
    ,CG_VAR_IN_OUT
    ,CG_VAR_EXTERNAL
    ,CG_VAR_GLOBAL
    ,CG_VAR_TEMP
    ,CG_VAR_IN_EXT
    ,CG_VAR_NATIVE  //4C extension 
    ,CG_CONST       //4C extension   
};
struct _CG_SymEntry 
{
    const TCHAR*          pszId;     //identifier
    CG_Edipos             edp;       //edipos of pszId
    const CG_DType*       pType;     //data type always set
    CG_DECLSCOPE          declScope; //VAR,VAR_INPUT,..
    const TCHAR*          pszAddr;   //NULL or pointer to direct address string
    CG_Edipos             edpAddr;   //memset to 0 or source location of address
    WORD                  declMod;   //bitset of CG_DMOD_* modifiers
    WORD                  accFlags;  //bitset of CG_ACCESS_*, only set if body
                                     //and after body was loaded, see details 
                                     //in comments & examples of CG_ACCESS_* bits.
    const CG_AttribList*  pAttribs;  //NULL or list of attributes
    const CG_Initialiser* pInit;     //NULL or the opt. initialiser
    const CG_SymEntry*    pNext;     //pointer to next entry or NULL
    long                  lNum;      //>=0: ordinal number of decl can be 
                                     //used to enum private data as a simple array.
    long                  lInstNum;  //-1 or >=0: ordinal number of "instance data relevant" decl
                                     //VAR_INPUT,VAR_OUTPUT,VAR   : textual order of decl (also if FUN)
                                     //VAR_GLOBAL,CONST,VAR_IN_EXT: always lInstNum==-1
                                     //VAR_IN_OUT                 : textual order of decl.
                                     //VAR_TEMP                   : textual order of decl.
                                     //Can be used to generate unique numbers for decls for 
                                     //the decl groups listed above
    long                  lParamNum; //>=0: this decl is part of the interface (in,out,outout,inext)
                                     // -1: this decl is not part of the interface.
                                     //lParamNum increases in the textual order of declarations
                                     //the function result entry has always lParamNum==0 even if
                                     //void, the last parameter has wParamNum+1==pParent->nParam
    const CG_Symtab*      pParent;   //for convenience only
	const TCHAR*          pszSysAddr;//forosai: should be placed after pszAddr TODO: access this
                                     //member only if CG_DMOD_SYS_ADDR bit is set, see comment above!								
};


//CG_Symtab: the symbol table, one for each struct, FB,
//function, program and *.gvl, *.sdt, *.cst file:
enum _CG_POUTYP { //All symbol table 'types'
     CG_POUTYP_UNDEF =0
    //IEC POU types:
    ,CG_POU_SDT
    ,CG_POU_FUN
    ,CG_POU_FB
    ,CG_POU_PRG
    //4C semi POU types:
    ,CG_POU_GVL   //for gvl files or VAR_GLOBAL(EXTERNAL)...END_VAR inside a resource.
    ,CG_POU_CST   //for cst files
};

// defines for bitset CG_Symtab::flags:
//------ error handling
#define CG_STAB_HDR_NOERR    0x000001  //set if no error in header
#define CG_STAB_LIBIF_ONLY   0x000002  //set if POU is 'only' the IEC interface portion 
                                       //of a <lib name>.4cl::<pou name>\<pou name>.[hdr|sdt].
                                       //This flag is only set for lib defined POUs
                                       //and signals that there is no implementation
                                       //for the active target type(=the one set in loadFeatures) 
                                       //see also ICG_IECFront::loadHeader.
                                       //This flag is only set if CG_Symtab::pLibCtx is != NULL.
#define CG_STAB_POLYMORH     0x000004  //set if the POU interface is polymorph, means
                                       //has any ANY_* or VAR_IN_EXT declarations (fun/FB only) 

//------ body kind: if none if these bits are set the POU is either a struct or a 
//       lib defined POU with precompiled obj. code (note the CG_STAB_LIBIF_ONLY special case)
#define CG_STAB_HAS_BDY      0x000010  //set if POU has a body, then loadBody is ok.
#define CG_STAB_MACRO        0x000020  //set if is macro inline
#define CG_STAB_OPERATOR     0x000080  //set if function is an operator alias e.g. ADD,MUL

//------ direct address usage
#define CG_STAB_HAS_ADDR     0x000100  //set if table contains any sort of %-addr.
#define CG_STAB_HAS_INCADDR  0x000200  //set if table contains incomplete addr. e.g. %IQ*.12


//------ miscellaneous:
#define CG_STAB_DIAG_ON_COPY 0x001000  //set if the deepcopy="disable|warn" attribute set.
#define CG_STAB_HAS_INST     0x002000  //set if symtab entries of type PRG,SDT,FB or array of
                                       //This bit may be set later during load body, if any 
                                       //auto extenal declaration is a FB or struct any is 
                                       //the first inst
#define CG_STAB_FORCEACTASK  0x004000  //set if the POU has the attribute forcecontext="acyclictask". ##AC


//------ body info: only set if has body and after loadBody:
#define CG_STAB_BDY_LOADED   0x010000  //set if body was at least once loaded 
#define CG_STAB_BDY_NOERR    0x020000  //set if body has no errors
#define CG_STAB_BDY_IS_ST    0x040000  //set if structured text body
#define CG_STAB_BDY_IS_LD    0x080000  //set if ladder body
#define CG_STAB_BDY_IS_FBD   0x100000  //set if function block diagram body
#define CG_STAB_BDY_IS_IL    0x200000  //set if instruction list body
#define CG_STAB_BDY_IS_SFC   0x400000  //set if sequential function chart body
                                       //The language bits ST,LD,FBD,IL,SFC are mutually
                                       //exclusive. In case of SFC the actions/transitions
                                       //can be of any language if source type is 4gr, to 
                                       //get the lang info of trans/actions see ICG_Body
                                       //and CG_STStmt::CG_StmtNW::lang
#define CG_STAB_BDY_NATIVE  0x1000000  //Body contains at least one NATIVE .. END_NATIVE block



struct _CG_Symtab {
    const TCHAR*          pszId;   //e.g. Program1,MyFun,Resource1.gvl,MyConst.cst
    CG_Edipos             edp;     //source location 'POU start' keyword
    CG_Edipos             edpEndHdr;//source location 'end of header' last END_VAR
    CG_POUTYP             pouTyp;  //what kind of POU
    CG_SOURCETYPES        srcTyp;  //enum of source file extension. If POU 
                                   //was processed by a filter srcTyp will be 
                                   //the type of the IEC interface definition 
                                   //(tmp) source file produced by the filter.
    long                  flags;   //bitset of CG_STAB_* defines
    const CG_LIBContext*  pLibCtx; //NULL or library context info struct,
                                   //if not NULL the POU is library defined.
    const CG_AttribList*  pAttribs;//NULL or list of attributes

    //Note on pFirst: Function return variable is treated as VAR_OUTPUT entry 
    //with CG_SymEntry::pszId=<FunctionName> and is always the 'pFirst'. 
	//A function returning void also has a first entry of type CG_VOID:
	//After call to ICG_IECfront::getBody(<thisPOU>) all entries from all 
    //#import *.gvl that are referenced in the body will be appended 
    //to the entry list.
    const CG_SymEntry*   pFirst; //NULL or first entry, loop starter.


    long                 nGVLImp;//0 or >0 the number of #imports
    const CG_EdpSymtab*  pGVLImp;//NULL or list of all #import *.gvl
                                 //size is nGVLImp, may be 0
    long              nVarNative;//0 or >0 the number of VAR_NATIVE blocks  
    const CG_EdpStr*  pVarNative;//NULL or list of all of VAR_NATIVE blocks
                                 //size is nVarNative, may be 0

    const TCHAR*     pszXREFFile;//NULL if no XREF or the file name e.g. 
                                 //program1.st.4xr, if the source has a path
                                 //then the path seperators \: are replaced by @.
                                 //All XREFs are stored flat in folder gen\__4xr.
                                 //library POUs are prefixed with <libname@>

	//header&interface byte offsets in source file (needed for .link files):
    unsigned long ulHdrBytes; //file pos of header end (last END_VAR,#import)
	unsigned long ulIF_Bytes; //file pos of interface end (displayed portion in 4cl) 
    __int64       ftLastWrite;//last write file time when file was first loaded

    //var_<scope> counters, 
    long nEntries; //(==nParam+nVar+nExternal+nTmp+nGlobals+nConst):
    long nGlobals; //VAR_GLOBAL
	long nInput;   //VAR_INPUT
	long nOutput;  //VAR_OUTPUT (also function return even if void)
    long nInOut;   //VAR_IN_OUT 
    long nTmp;     //VAR_TEMP
    long nVar;     //VAR
	long nExternal;//VAR_EXTERNALS, includes used(!) #import *.gvl entries
                   //may increase on loadBody
    long nConst;   //CONST
    long nParam;   //nIn+nOutput+nInOut+VAR_IN_EXT(<-only 1 allowed), nParam includes 
                   //the function return (even if void) therefore nParam>=1 for functions.
    const CG_SymEntry* pVarInExt; //NULL or the entry that is VAR_IN_EXT
                                  //if not NULL the function is extensible

    const CG_Symtab*const*   pzRefSyms;   //NULL:
                                          // POU has body but body not loaded yet
                                          //!=NULL:
                                          // Zero term. list of all instantiated 
                                          // FB,SDT (and if GVL also Program) types and 
                                          // called functions. The member is set even if
                                          // errors but the information may be incomplete.
                                          
    const CG_SymEntry*const* pzRefGlbCst; //NULL:
                                          // POU has body but body not loaded yet
                                          //!=NULL: 
                                          // Zero term. list of referenced global CONST.
                                          // The list contains all references found in header
                                          // and body. The member is set even if errors
                                          // but the information may be incomplete.
};






//CG_SFCStruct: the sfc network abstract description:")
//defines for bitset CG_SFCStruct::pStepActionBlockQualifier")
//All qualifiers with bitset 'greater' (means unsigned > op)")
//than CG_SFC_QUAL_TIME are time-qualifiers")
#define CG_SFC_QUAL_N     0x0001
#define CG_SFC_QUAL_R     0x0002
#define CG_SFC_QUAL_S     0x0004
#define CG_SFC_QUAL_P     0x0008
#define CG_SFC_QUAL_P0    0x0010
#define CG_SFC_QUAL_P1    0x0020
#define CG_SFC_QUAL_L     0x0040
#define CG_SFC_QUAL_D     0x0080
#define CG_SFC_QUAL_SD    0x0100
#define CG_SFC_QUAL_DS    0x0200
#define CG_SFC_QUAL_SL    0x0400
#define CG_SFC_QUAL_TIME  0x0040

struct _CG_SFCStruct {
    // these variables contain the logical structure of the SFC
    // for documentation see \\S_4C\4CONTROL\Projekt\SWE\4CC\v12\sfc.doc paragraph 2.4
    long nSteps;
    long nTrans;
    long nActions;
    long nNextSteps;
    long nPrevSteps;
    long nStepAction;
    long initStep;

    //symbolic info:
    /*[size_is(nSteps)]    */  const TCHAR** pStepNames;
    /*[size_is(nSteps)]    */  CG_Edipos*    pStepEdp;
    /*[size_is(nActions)]  */  const TCHAR** pActionNames;
    /*[size_is(nActions)]  */  CG_Edipos*    pActionEdp;
    /*[size_is(nTrans)]    */  const TCHAR** pTransNames;
    /*[size_is(nTrans)]    */  CG_Edipos*    pTransEdp;

    // transition-step structure
    /*[size_is(nSteps)]    */  long* pFollowTrans;
    /*[size_is(nTrans)]    */  long* pAltTrans;
    /*[size_is(nNextSteps)]*/  long* pNextSteps;
    /*[size_is(nTrans)]    */  long* pNextStepsOffset;
    /*[size_is(nPrevSteps)]*/  long* pPrevSteps;
    /*[size_is(nTrans)]    */  long* pPrevStepsOffset;

    // actions/actionblocks/step->actionblock
    /*[size_is(nStepAction)]*/ long* pStepActionBlocks;
    /*[size_is(nSteps)]     */ long* pStepActionBlockOffset;
    /*[size_is(nStepAction)]*/ long* pStepActionBlockQualifier;
    /*[size_is(nStepAction)]*/ long* pStepActionBlockTimeParameter;
};

//predefined reserverd task init attributes:
//IEC:
#define CG_TASK_SINGLE    _T("SINGLE")
#define CG_TASK_INTERVAL  _T("INTERVAL")
#define CG_TASK_PRIORITY  _T("PRIORITY")
//4C extension:
#define CG_TASK_AUTOSTART _T("AUTOSTART")
#define CG_TASK_IOSYNC    _T("IOSYNC")

//CG_Resource: content of a *.res file:
struct _CG_TaskInit{
    CG_Edipos     edp;    //source location of pszAttr name
    const TCHAR*  pszAttr;//Custom: any back end defined identifier
                          //reserved IEC: SINGLE|INTERVAL|PRIORITY
                          //reserved 4C:  AUTOSTART|IOSYNC
    CG_ExprTree   val;    //actual value: a literal or const ref
};


struct _CG_Task {
    CG_Edipos            edp;      //source location of task name
    const TCHAR*         pszId;    //task name
                                   //Note: task name space is the resource GVL
                                   //      a task cannot have the same name 
                                   //      as a glob var or a program instance
    const CG_AttribList* pAttribs; //NULL or list of attributes

    long                 nPrgs;    //>=0: number of programs assigned to task 
    const CG_SymEntry*const*
                         pPrgs;    //NULL if nPrgs==0 or program instance list 
                                   //size is nPrgs.
                                   //pPrgs[0..nPrgs-1] points to one of 
                                   //the resource symtab program GV entries.
                                   //Note: program instances are treated like
                                   //global FBs including the name space.
    
    long                 nInits;    //>=0 number of id:=<expr> task inits
    //[size_is(nInits)] 
    const CG_TaskInit*   pInits;    //NULL or array of id:=<expr> pairs
};



struct _CG_Resource {
    boolean               bNoError; //true only if no direct and no indirect error
                                    //from attracted POU headers (body errors are not included!)
    CG_Edipos             edp;      //source location of resource name
    const TCHAR*          pszId;    //resource name
    const TCHAR*          pszTyp;   //NULL or resource type. Type can be ommitted.
                                    //Interpretation of type is up to back end.
    const CG_AttribList*  pAttribs; //NULL or list of attributes, note: !<id> specials.
    long                  nTasks;   //number of tasks
    const CG_Task*        pTasks;   //NULL or array of task definitions, size is nTasks.
    const CG_Symtab*      pGVL;     //maybe NULL if bNoError==true, contains
                                    //all global variables of the resource:
                                    //The resource's #import are imports inside 
                                    //the pGVL (see pGVL->pGVLImp member)
                                    //and are NOT expanded as sym entries of pGVL.
                                    //a loop over all GVs requires a loop over all
                                    //imports too (but currently no nested imports). 
                                    //Program instances are stored as individual 
                                    //entries in pGVL (of type CG_PROGRAM)
                                    //Note: the unique name of pGVL->pszId is 
                                    //constructed as: gvl@<resource name>
    const CG_Symtab*const* pzGVLRefSyms;  //Never NULL: Zero term. list of types that are 
                                          //referenced in the GVL of the resource global
                                          //Structs,FBs and Program types 
};

typedef struct _CG_ResImpInfo
{
    CG_Edipos     edp;        //resource import's edp
    const TCHAR*  pszRes;     //resource IEC name
    const TCHAR*  pszImpFile; //project relative canonical file name
} CG_ResImpInfo;




//allow or restrict language features:
//Note: pointer to this struct is passed in CG_IECFront::loadFeatures
//It is allowed to call CG_IECFront::loadFeatures again to change the feature set
//Features must be explicitly enabled.

//CG_FRONT_END_TYPES: all types that are supported by the front end, 
//currently all except the unsigned int types and no SINT.
//The backend supported data types passed via the CG_Features struct 
//will be intersected with this type set.
const CG_BITSET CG_FRONT_END_TYPES = 
    (CG_TBS_ANY_REAL
  //|CG_TBS_SINT
    |CG_TBS_INT
    |CG_TBS_DINT
    |CG_TBS_LINT
  //|CG_TBS_USINT
    |CG_TBS_UINT
    |CG_TBS_UDINT
  //|CG_TBS_ULINT
    |CG_TBS_ANY_BIT
    |CG_TBS_ANY_DATE
    |CG_TBS_TIME
    |CG_TBS_ANY_STRING
    |CG_TBS_ARRAY
    |CG_TBS_INSTANCE
    |CG_TBS_STRUCT
    //the corresponding QVTs:
    |CG_TBS_QVT_BOOL
    |CG_TBS_QVT_BYTE
    |CG_TBS_QVT_WORD
    |CG_TBS_QVT_DWORD
    |CG_TBS_QVT_LWORD
    |CG_TBS_QVT_REAL
    |CG_TBS_QVT_LREAL
  //|CG_TBS_QVT_SINT
    |CG_TBS_QVT_INT
    |CG_TBS_QVT_DINT
    |CG_TBS_QVT_LINT
  //|CG_TBS_QVT_USINT
    |CG_TBS_QVT_UINT
    |CG_TBS_QVT_UDINT
  //|CG_TBS_QVT_ULINT
    |CG_TBS_QVT_DT
    |CG_TBS_QVT_TOD
    |CG_TBS_QVT_DATE
    |CG_TBS_QVT_TIME
    |CG_TBS_QVT_STRING
    |CG_TBS_QVT_WSTRING
);


struct _CG_Features {
    const TCHAR* pszTargetTyp;                //NULL or the target or back end type.
                                              //appears in the 'not supported' error msg if set
    //languages (ST is mandatory):
    boolean      bLadder;
    boolean      bFBD;
    boolean      bSFC;
  
    //declaration (header) features:
    boolean      bIncompleteAddr;              //allow %* and %+ syntax
    boolean      bAssumedSizeArrays;           //allow ARRAY OF <type> syntax
    long         declModifiers;                //bitset of CG_DMOD_* modifiers
    boolean      bMemoryDirectAddr;            //if true allow %M syntax


    //Type restrictions:
    CG_BITSET    allTypes;                     //generally supported basic data types
    CG_BITSET    arrayTypes;                   //supported array base data types 
    CG_BITSET    structMemberTypes;            //generally supported basic data types
    CG_BITSET    funRetTypes;                  //allowed function return types
    CG_BITSET    funInputTypes;                //allowed function input types
    CG_BITSET    funOutputTypes;               //allowed function output types
    CG_BITSET    funInOutTypes;                //allowed function in-out types  
    CG_BITSET    funLocalTypes;                //allowed function local types
    CG_BITSET    FBInOutTypes;                 //allowed function block in-out types  
    CG_BITSET    gvlTypes;                     //allowed types for global variables
    long         preferedInt;                  //16,32: the platform native integral
                                               //size of any integral. Used to fixType
                                               //expressions that are still polymorph
                                               //after type resolution. 
                                               //Example:  "aArray[1]" the literal '1' 
                                               //will be a signed 8,16 or 32 bit.
    long         preferedFloat;                //32,64: the platform native float type

    //allowed decl scopes:
    boolean bVarNativeFun;
    boolean bVarNativeFBPrg;
    boolean bVarInOutFun; 
    boolean bVarInOutFB; 
    boolean bVarTmpFun;
    boolean bVarTmpFBPrg;
    boolean bVarGlobPrg;
    boolean bVarExtFun;

    //task features:                  //Example:
    long               nTIPairs;      //=4
    const TCHAR*const* ppszTI;        //={"INTERVAL","PRIORITY","IOSYNC","AUTOSTART"}
    const CG_BITSET*   pTITypes;      //={"CG_TBS_ANY_INT,CG_TBS_ANY_INT,CG_TBS_ANY_INT,CG_TBS_BOOL}

    boolean  bMacroFB;   //if true allow inline{...} body for FBs.

    //SFC features:
    //TODO: action qualifiers


    //general limits nMax_* 
    //TODO e.g. max vars per POU, max struct nesting level, ....
    long         nMaxStrLen;        // 0 means no limit; otherwise represents the maximum number of characters
                                    // ex. 0 for 4cpc target
                                    //   255 for 4cwin target


    //if pAutodecl is not NULL, use this class as  auto decl server.
    //This class must implement the ICG_AutoDecl interface (see CG_AutoDecl.idl):
    const CLSID* pAutodecl;         //clsid or NULL
    const TCHAR* pszAutoDeclConfig; //must be set if pAutodecl is not NULL
                                    //config file name as passed to 
                                    //ICG_AutoDecl::init (see CG_AutoDecl.idl)
};
#pragma warning( default : 4200)
















//C++ inline helper functions:
//
//------------------------------------------------------------------*
/**
 * CG_Init[ST|CMDL|Text]Edipos can be used to initialise a CG_Edipos structure to 
 * be passed to diagMsg. 
 * 
 *  CG_Edipos cmdl;  <- allocate on stack.
 *  CG_InitCMDLEdipos(&cmdl)                   <-displays an command line error
 *  CG_InitSTEdipos(&cmdl, "myFile.txt")       <-displays an error on a file (no line,col)
 *  CG_InitSTEdipos(&cmdl, "myFile.txt", i)    <-displays a myFile.txt(67) (no col)
 *  CG_InitSTEdipos(&cmdl, "myFile.txt", i,j)  <-displays a myFile.txt(67,3)
 *  Note: you must not call CG_MakeEdiPos(&cmdl, NULL, 4,5) nor CG_MakeEdiPos(&cmdl, "myFile.txt", 0,5)
 *  
 * @param           pEdp    pointer to struct to be filled
 * @param           pszFile pointer to file name (live time of this pointer must be at 
 *                          least until diagMsg returns)
 * @param           nLine   >=0 the line number
 * @param           nCol    >=0 the column, if nCol>0 nLine must be >0 too
 * @return          void
*/
inline void CG_InitSTEdipos(
    CG_Edipos*   pEdp, 
    const TCHAR* pszFile, 
    long         nLine   = 0, 
    long         nCol    = 0
)
{
    assert(pszFile && nCol>=0 && nLine>=0 && !(nCol>0 && nLine==0));
    pEdp->typ     = CG_EDP_ST;
    pEdp->pszFile = pszFile;
    pEdp->lc.l    = nLine;
    pEdp->lc.c    = nCol;
}
inline void CG_InitCMDLEdipos(CG_Edipos* pEdp)
{
    pEdp->pszFile = NULL;
    pEdp->typ     = CG_EDP_FILE;
    pEdp->lc.l    = 0;
    pEdp->lc.c    = 0;
}
inline void CG_InitEdipos(CG_Edipos* pEdp)
{
    pEdp->pszFile = NULL;
    pEdp->typ     = CG_EDP_FILE;
    pEdp->lc.l    = 0;
    pEdp->lc.c    = 0;
}
//the psz must be valis as long a the dp is used:
inline void CG_InitTextEdipos(CG_Edipos* pEdp, const TCHAR* pszFile, const TCHAR* psz)
{
    pEdp->pszFile = pszFile;
    pEdp->typ     = CG_EDP_TEXT;
    pEdp->psz     = psz;
}


//------------------------------------------------------------------*
/**
 * CG_IncColEdipos be used to modify an edipos to give a more precise 
 * column information typically within a token.
 * The token's original CG_Edipos must be copied to a temporary 'edptmp' and
 * then call CG_IncColEdipos(&edptmp, <TCHAR offset>) if a better
 * column info for the diag msg can be given. The <TCHAR offset>
 * is the number of TCHARs that should be added to the tmpedp.
 * Note 1: tab chars are counted as one TCHAR.
 * Note 2: if the orginal edipos did not contain a column info 
 *         the function fails. A colums edp is present if 
 *         a) pEdpTmp->typ==CG_EDP_ST && pEdpTmp->lc.c>0
 *         or
 *         b) pEdpTmp->typ==CG_EDP_4GR_STBOX && pEdpTmp->gr.c>0
 * 
 * @param     pEdpTmp  pointer to struct to be changed
 *                     if the function failed pEdpTmp is not 
 *                     changed.
 * @param     incCol   >=0 the TCHAR offset in the token to be added.
 * @return    true if col info could be changed, false if not.
*/
inline bool CG_IncColEdipos(CG_Edipos* pEdpTmp, int incCol)
{
    assert(incCol>=0);
    if(pEdpTmp->pszFile)
    {
        if(pEdpTmp->typ==CG_EDP_ST && pEdpTmp->lc.c>0)
        {
            pEdpTmp->lc.c += incCol;
            return true;
        }
        if(pEdpTmp->typ==CG_EDP_4GR_STBOX && pEdpTmp->gr.c>0)
        {
            pEdpTmp->gr.c += incCol;
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------*
/**
 * CG_IncLineColEdipos be used to modify an edipos to give a more precise 
 * lind and column information typically within a text block.
 * The token's original CG_Edipos must be copied to a temporary 'edptmp' and
 * then call CG_IncLineColEdipos(&edptmp, <line offset>, <TCHAR offset>) 
 * if a better line/column info for the diag msg can be given. 
 * The TCHAR column info is the number of TCHARs that should set as tmpedp's
 * column info.
 * Note 1: tab chars are counted as one TCHAR.
 * Note 2: if the orginal edipos did not contain at least a line info 
 *         the function fails and does not change the pEdpTmp. 
 *         A line edp is present if 
 *         a) pEdpTmp->typ==CG_EDP_ST && pEdpTmp->lc.l
 *         or
 *         b) pEdpTmp->typ==CG_EDP_4GR_STBOX && pEdpTmp->gr.l
 *
 * @param     pEdpTmp  pointer to struct to be changed
 *                     if the function failed pEdpTmp is not 
 *                     changed.
 * @param     incLine  the line offset in the token to be added, can be <0 too.
 * @param     setCol   >=0 the TCHAR column in the token to be set.
 *                     a value of 0 means 'there is no column info'
 *
 * @return    true if line/col info could be changed, false if not.
*/
inline bool CG_IncLineColEdipos(CG_Edipos* pEdpTmp, int incLine, int setCol)
{
    assert(setCol>=0);
    if(pEdpTmp->pszFile)
    {
        if(pEdpTmp->typ==CG_EDP_ST && pEdpTmp->lc.l>0)
        {
            pEdpTmp->lc.c = setCol;
            pEdpTmp->lc.l += incLine;
            return true;
        }
        if(pEdpTmp->typ==CG_EDP_4GR_STBOX && pEdpTmp->gr.l>0)
        {
            pEdpTmp->gr.c = setCol;
            pEdpTmp->gr.l += incLine;
            return true;
        }
    }
    return false;
}



/**
 * CG_HasPOUTypBody can be used to classify a CG_POU_TYP enum. 
 * The function checks if the POU-type is a progrom,fb or 
 * function (this is the exact IEC definition of the term POU) 
 * Only 'IEC POUs' may have a body. When this functions returns 
 * true, that does not mean that a certain POU has a body in 
 * terms of ST++ statements:
 * Functions that have an inline{...} body: 
 *     ICG_IECFront::loadBody returns NULL
 *     ICG_IECFront::loadExtBodyInfo returns NULL
 * The inline code is part of the header and parsed 
 * in loadHeader.
 *   
 * Library defined POUs with precompiled code: 
 *     ICG_IECFront::loadBody returns NULL
 *     ICG_IECFront::loadExtBodyInfo returns NULL
 * 
 *
 * @param     t  enum to be classified. 
 * @return    true if POU has a body
 *
 * @see  CG_Symtab::flags:CG_STAB_HAS_BDY
*/
inline bool CG_HasPOUTypBody(CG_POUTYP t)
{
    return t==CG_POU_FB||t==CG_POU_FUN||t==CG_POU_PRG;
}




//------------------------------------------------------------------*
/**
 * CG_IsMonomorph can be used check if a basic type enum is a 
 * monomorph type (not one of the ANY_* types)
 * @param     t    the enum to classify
 * @return    true if not an ANY_* type
*/
inline bool CG_IsMonomorph(CG_DTYPES t)
{
    return t<CG_ANY;
}


//------------------------------------------------------------------*
/**
 * CG_IsUserTyp can be used to classify a CG_DTYES enum. The 
 * function checks is the type is a user defined type:
 * FB,PRG,STRUCT.
 * @param     t  enum to be classified. 
 * @return    true if a user defined type
*/
inline bool CG_IsUserTyp(CG_DTYPES t)
{
    return t==CG_INSTANCE||t==CG_STRUCT||t==CG_PROGRAM;
}

//------------------------------------------------------------------*
/**
 * CG_IsMultiElemType can be used to classify a CG_DType struct. The 
 * function checks if the type is any array or a 'struct' type that
 * has fields.
 * 
 * @param     p  enum to be classified. 
 * @return    true if a type is any array or fb,struct program
*/
inline bool CG_IsMultiElemType(const CG_DType* p)
{
    return p->bIsArray||p->type==CG_INSTANCE||p->type==CG_STRUCT||p->type==CG_PROGRAM;
}



//------------------------------------------------------------------*
/**
 * CG_HasExtTypeDescr can be used to classify a CG_DType struct. The 
 * function checks if the type has any extended type info:
 *   pSym, lStrlen or is an array.
 * If the function returns false, the type info of p
 * is identical with p->type enum without loss of data. 
 * @param     p    data type to be classified. 
 * @return    true if no extended type info
*/
inline bool CG_HasExtTypeDescr(const CG_DType* p)
{
    return p->lStrLen!=-1 || p->pSym || p->bIsArray;
}


//------------------------------------------------------------------*
/**
 * CG_DTYPES_IsElementary can be used to classify a CG_DTYES enum.
 * The function checks if the type is one of the basic types:
 * no QVT, no program, FB, struct or Array and no ANY-type
 * @param     t  enum to be classified. 
 * @return    true if type t is elementary.
*/
inline bool CG_IsElementary(CG_DTYPES t)
{
    assert(t!=CG_DTYPES_UNDEF);
    return t>=CG_BOOL && t<=CG_WSTRING;
}

inline bool CG_IsAnyBit(CG_DTYPES t)
{
    return CG_BOOL<=t && t<=CG_LWORD;
}

inline bool CG_IsAnyInt(CG_DTYPES t)
{
    return CG_SINT<=t && t<=CG_ULINT;
}

inline bool CG_IsAnyReal(CG_DTYPES t)
{
    return CG_REAL==t || t==CG_LREAL;
}

inline bool CG_IsAnyQVT(CG_DTYPES t)
{
    return CG_QVT_BOOL<=t && t<=CG_QVT_WSTRING;
}

inline bool CG_IsAnyEdge(CG_DTYPES t)
{
    return CG_BOOL_R_EDGE==t || CG_BOOL_F_EDGE==t;
}

inline bool CG_IsAssumedSizeArray(const CG_DType* p)
{
    return p->bIsArray && p->nElems==0;
}

inline bool CG_IsInterfaceDecl(const CG_DECLSCOPE ds)
{
    return ds==CG_VAR_INPUT||ds==CG_VAR_OUTPUT||ds==CG_VAR_IN_OUT||ds==CG_VAR_IN_EXT;
}




#endif // __CG_IECFRONT_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: CG_IECFront.h $
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 14.05.07   Time: 16:37
 * Updated in $/4CReleased/V2.20.00/COM/softing/fc/inc
 * osai feature: error message uses sys var name and not corresdon
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc
 * 
 * *****************  Version 7  *****************
 * User: Ef           Date: 5.10.06    Time: 9:33
 * Updated in $/4Control/COM/softing/fc/inc
 * added CG_STAB_BDY_NATIVE flag
 * 
 * *****************  Version 6  *****************
 * User: Ef           Date: 13.06.06   Time: 15:25
 * Updated in $/4Control/COM/softing/fc/inc
 * implemented UINT, UDINT
 * 
 * *****************  Version 5  *****************
 * User: Ef           Date: 6.06.06    Time: 16:41
 * Updated in $/4Control/COM/softing/fc/inc
 * changes from CA: console download is optional
 * 
 * *****************  Version 4  *****************
 * User: Ef           Date: 22.10.03   Time: 14:28
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 3  *****************
 * User: Wi           Date: 26.09.03   Time: 13:11
 * Updated in $/4Control/COM/softing/fc/inc
 * changes for acyclic tasks
 * 
 * *****************  Version 2  *****************
 * User: Ca           Date: 15.09.03   Time: 11:38
 * Updated in $/4Control/COM/softing/fc/inc
 * implement IL compiler
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 78  *****************
 * User: Jd           Date: 23.01.03   Time: 12:32
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 77  *****************
 * User: Ef           Date: 21.11.02   Time: 21:27
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 76  *****************
 * User: Ef           Date: 30.10.02   Time: 20:59
 * Updated in $/4Control/COM/softing/fc/inc
 * lib.txt command
 * 
 * *****************  Version 75  *****************
 * User: Ef           Date: 25.10.02   Time: 20:06
 * Updated in $/4Control/COM/softing/fc/inc
 * more $()-stuff and default 4cp keys values
 * 
 * *****************  Version 74  *****************
 * User: Ef           Date: 22.10.02   Time: 18:54
 * Updated in $/4Control/COM/softing/fc/inc
 * fixed bug in  CG_IncLineColEdipos
 * 
 * *****************  Version 73  *****************
 * User: Ef           Date: 1.08.02    Time: 20:09
 * Updated in $/4Control/COM/softing/fc/inc
 * new featue needver=   in [format]
 * 
 * *****************  Version 72  *****************
 * User: Jd           Date: 15.02.02   Time: 15:16
 * Updated in $/4Control/COM/softing/fc/inc
 * Added sfc exception action.
 * 
 * *****************  Version 71  *****************
 * User: Ef           Date: 4.12.01    Time: 22:28
 * Updated in $/4Control/COM/softing/fc/inc
 * new method: getSimpleMemberWriteInfo
 * 
 * *****************  Version 70  *****************
 * User: Ef           Date: 26.11.01   Time: 17:29
 * Updated in $/4Control/COM/softing/fc/inc
 * changes for CG_ACCESS_* bits and CG_TMPVAR
 * 
 * *****************  Version 69  *****************
 * User: Ef           Date: 26.10.01   Time: 20:14
 * Updated in $/4Control/COM/softing/fc/inc
 * added lib.license
 * 
 * *****************  Version 68  *****************
 * User: Die          Date: 24.10.01   Time: 20:48
 * Updated in $/4Control/COM/softing/fc/inc
 * more fields in CG_USEREXPR
 * 
 * *****************  Version 67  *****************
 * User: Ca           Date: 24.10.01   Time: 15:50
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 66  *****************
 * User: Ef           Date: 9.10.01    Time: 12:04
 * Updated in $/4Control/COM/softing/fc/inc
 * opt=xref:0|1
 * 
 * *****************  Version 65  *****************
 * User: Ca           Date: 12.09.01   Time: 14:26
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 64  *****************
 * User: Ca           Date: 6.09.01    Time: 16:29
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 63  *****************
 * User: Ef           Date: 6.09.01    Time: 9:58
 * Updated in $/4Control/COM/softing/fc/inc
 * added task init features
 * 
 * *****************  Version 62  *****************
 * User: Ef           Date: 21.08.01   Time: 16:15
 * Updated in $/4Control/COM/softing/fc/inc
 * kad:   BUILD_MULTIPLE
 * 
 * *****************  Version 61  *****************
 * User: Ca           Date: 21.08.01   Time: 11:53
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 60  *****************
 * User: Ef           Date: 2.08.01    Time: 14:21
 * Updated in $/4Control/COM/softing/fc/inc
 * corrected comments
 * 
 * *****************  Version 59  *****************
 * User: Ef           Date: 26.07.01   Time: 16:18
 * Updated in $/4Control/COM/softing/fc/inc
 * updated some comments
 * 
 * *****************  Version 58  *****************
 * User: Ef           Date: 24.07.01   Time: 11:04
 * Updated in $/4Control/COM/softing/fc/inc
 * changed called function handling, used type list in symtab
 * 
 * *****************  Version 57  *****************
 * User: Ef           Date: 19.07.01   Time: 23:02
 * Updated in $/4Control/COM/softing/fc/inc
 * xref done
 * 
 * *****************  Version 56  *****************
 * User: Ef           Date: 18.07.01   Time: 16:57
 * Updated in $/4Control/COM/softing/fc/inc
 * const ref in symtab
 * 
 * *****************  Version 55  *****************
 * User: Ef           Date: 16.07.01   Time: 23:41
 * Updated in $/4Control/COM/softing/fc/inc
 * started implem. Xref 
 * 
 * *****************  Version 54  *****************
 * User: Ef           Date: 6.07.01    Time: 17:17
 * Updated in $/4Control/COM/softing/fc/inc
 * changed error counter functions
 * 
 * *****************  Version 53  *****************
 * User: Ef           Date: 6.07.01    Time: 12:07
 * Updated in $/4Control/COM/softing/fc/inc
 * corrected comment
 * 
 * *****************  Version 52  *****************
 * User: Ef           Date: 5.07.01    Time: 22:54
 * Updated in $/4Control/COM/softing/fc/inc
 * implem. QVT
 * 
 * *****************  Version 51  *****************
 * User: Ef           Date: 3.07.01    Time: 22:12
 * Updated in $/4Control/COM/softing/fc/inc
 * language info in CG_LDFBDNETWORK
 * 
 * *****************  Version 50  *****************
 * User: Ef           Date: 28.06.01   Time: 20:45
 * Updated in $/4Control/COM/softing/fc/inc
 * more LD
 * 
 * *****************  Version 49  *****************
 * User: Ef           Date: 25.06.01   Time: 21:19
 * Updated in $/4Control/COM/softing/fc/inc
 * more LD
 * 
 * *****************  Version 48  *****************
 * User: Ef           Date: 25.06.01   Time: 16:19
 * Updated in $/4Control/COM/softing/fc/inc
 * [Classes.Config]   ->  [Classes.Configurations]
 * 
 * *****************  Version 47  *****************
 * User: Ef           Date: 22.06.01   Time: 10:15
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 46  *****************
 * User: Ef           Date: 19.06.01   Time: 19:06
 * Updated in $/4Control/COM/softing/fc/inc
 * updated comment
 * 
 * *****************  Version 45  *****************
 * User: Ef           Date: 18.06.01   Time: 22:13
 * Updated in $/4Control/COM/softing/fc/inc
 * more SFC
 * 
 * *****************  Version 44  *****************
 * User: Ef           Date: 8.06.01    Time: 16:26
 * Updated in $/4Control/COM/softing/fc/inc
 * more SFC
 * 
 * *****************  Version 43  *****************
 * User: Ef           Date: 7.06.01    Time: 21:42
 * Updated in $/4Control/COM/softing/fc/inc
 * started BPs
 * 
 * *****************  Version 42  *****************
 * User: Ef           Date: 18.05.01   Time: 20:18
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 41  *****************
 * User: Ef           Date: 18.05.01   Time: 10:20
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 40  *****************
 * User: Ef           Date: 17.05.01   Time: 18:31
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 39  *****************
 * User: Ef           Date: 16.05.01   Time: 19:38
 * Updated in $/4Control/COM/softing/fc/inc
 * autodecl
 * 
 * *****************  Version 38  *****************
 * User: Ef           Date: 15.05.01   Time: 19:43
 * Updated in $/4Control/COM/softing/fc/inc
 * changed edp for 4gr (more or less the old way)
 * 
 * *****************  Version 37  *****************
 * User: Ef           Date: 10.05.01   Time: 20:56
 * Updated in $/4Control/COM/softing/fc/inc
 * more 4gr
 * 
 * *****************  Version 36  *****************
 * User: Ef           Date: 4.05.01    Time: 21:10
 * Updated in $/4Control/COM/softing/fc/inc
 * started 4gr parsing
 * 
 * *****************  Version 35  *****************
 * User: Ef           Date: 3.05.01    Time: 22:47
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 34  *****************
 * User: Ef           Date: 2.05.01    Time: 18:07
 * Updated in $/4Control/COM/softing/fc/inc
 * more cg_features
 * 
 * *****************  Version 33  *****************
 * User: Ef           Date: 28.04.01   Time: 0:27
 * Updated in $/4Control/COM/softing/fc/inc
 * int,real,wstring
 * 
 * *****************  Version 32  *****************
 * User: Ef           Date: 27.04.01   Time: 18:46
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 31  *****************
 * User: Ef           Date: 26.04.01   Time: 20:29
 * Updated in $/4Control/COM/softing/fc/inc
 * type bitset is now 64 bit and also in interface
 * 
 * *****************  Version 30  *****************
 * User: Ef           Date: 19.04.01   Time: 15:43
 * Updated in $/4Control/COM/softing/fc/inc
 * R/W flag for FBI call entry both always set, new function to get the
 * array base type
 * 
 * *****************  Version 29  *****************
 * User: Ef           Date: 17.04.01   Time: 13:06
 * Updated in $/4Control/COM/softing/fc/inc
 * removed the CG_ARRAY enum
 * 
 * *****************  Version 28  *****************
 * User: Ef           Date: 11.04.01   Time: 12:12
 * Updated in $/4Control/COM/softing/fc/inc
 * changed lib=  to file= in [libs] section
 * 
 * *****************  Version 27  *****************
 * User: Ef           Date: 9.04.01    Time: 10:23
 * Updated in $/4Control/COM/softing/fc/inc
 * loadCST, more comments, corrected comments
 * 
 * *****************  Version 26  *****************
 * User: Ef           Date: 4.04.01    Time: 16:47
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 25  *****************
 * User: Ef           Date: 3.04.01    Time: 11:45
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 24  *****************
 * User: Ef           Date: 2.04.01    Time: 9:39
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 23  *****************
 * User: Ef           Date: 31.03.01   Time: 19:22
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 22  *****************
 * User: Ef           Date: 30.03.01   Time: 9:14
 * Updated in $/4Control/COM/softing/fc/inc
 * - changed loadLIBs,loadCSTs to have (void) argumet list, better have en
 * extar loadLIB/CST for non 4cp files (if we ever need this)
 * - for Lib POUs added a special CG_LIBContext better than resuse
 * CG_POU4CPinfo.
 * - new methods for Lib access, especially a generall file locator
 * syntax.
 * 
 * *****************  Version 21  *****************
 * User: Ef           Date: 28.03.01   Time: 17:26
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 20  *****************
 * User: Ef           Date: 27.03.01   Time: 21:09
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 19  *****************
 * User: Ef           Date: 26.03.01   Time: 17:44
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 18  *****************
 * User: Ef           Date: 26.03.01   Time: 11:51
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 17  *****************
 * User: Ef           Date: 23.03.01   Time: 10:14
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 16  *****************
 * User: Ef           Date: 20.03.01   Time: 11:53
 * Updated in $/4Control/COM/softing/fc/inc
 * more comments, chanded the CG_Literal to be more type safe added more
 * body ext info
 * 
 * *****************  Version 15  *****************
 * User: Ef           Date: 15.03.01   Time: 18:18
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 14  *****************
 * User: Ef           Date: 12.03.01   Time: 18:34
 * Updated in $/4Control/COM/softing/fc/inc
 * implemented config
 * 
 * *****************  Version 13  *****************
 * User: Ef           Date: 9.03.01    Time: 20:49
 * Updated in $/4Control/COM/softing/fc/inc
 * - removed CG_ArgList. 
 * - normal assign stmts are now a CG_STStmt not a CG_MOVE expr.
 * - reformatted the expr,stmt unions with named struct types,
 *   allows the visual studio statement completion and member tool tip to
 * work fine.
 * 
 * *****************  Version 12  *****************
 * User: Ef           Date: 6.03.01    Time: 22:01
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 11  *****************
 * User: Ef           Date: 6.03.01    Time: 10:19
 * Updated in $/4Control/COM/softing/fc/inc
 * small enhancements
 * 
 * *****************  Version 10  *****************
 * User: Ef           Date: 2.03.01    Time: 19:49
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 9  *****************
 * User: Ef           Date: 1.03.01    Time: 20:53
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 7  *****************
 * User: Ef           Date: 23.02.01   Time: 12:31
 * Updated in $/4Control/COM/softing/fc/inc
 * updated some comments
 * 
 * *****************  Version 6  *****************
 * User: Ef           Date: 22.02.01   Time: 14:32
 * Updated in $/4Control/COM/softing/fc/inc
 * all ST statements
 * 
 * *****************  Version 5  *****************
 * User: Ef           Date: 19.02.01   Time: 21:13
 * Updated in $/4Control/COM/softing/fc/inc
 * use _i.c file
 * 
 * *****************  Version 4  *****************
 * User: Ef           Date: 19.02.01   Time: 9:55
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 16.02.01   Time: 16:06
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 15.02.01   Time: 18:40
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 15.02.01   Time: 14:55
 * Created in $/4Control/COM/softing/fc/inc
 * initial check in
 *==
 *----------------------------------------------------------------------------*
*/

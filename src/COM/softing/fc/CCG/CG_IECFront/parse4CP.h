
#ifndef __PARSE4CP_H_
#define __PARSE4CP_H_


//----  Aggregate Includes:   --------------------------------------*
#include "fc_tools\fc_cstring.h"
#include "fc_tools\fc_hash.h"
#include "cgt_misc.h"
#include "fc_todebug\fc_assert.h"
#include "CG_IECFront.h"

//----  Forward Class Definitions:   -------------------------------*
class  CGT_CDiagMsg;
class  CKADInfo;
//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


class ST_Parse4CP
{
    FC_CHashStrTable            m_strtbl;
    const TCHAR**               m_pzpszSecNames;
    CG_POU4CPInfo**             m_pzpCSTInfo;
    CG_POU4CPInfo**             m_pzpGVLInfo;
    CG_LIBContext**             m_pzpLIBInfo;
    CG_POU4CPInfo**             m_pzpPOUInfo;
    FC_CHashStr<CG_AttribList*> m_4cp;

    FC_CHashStr<CG_POU4CPInfo*> m_TARinfo;
    FC_CHashStr<CG_POU4CPInfo*> m_CONinfo;
    FC_CHashStr<CG_POU4CPInfo*> m_POUinfo;
    FC_CHashStr<CG_POU4CPInfo*> m_CSTinfo;
    FC_CHashStr<CG_POU4CPInfo*> m_GVLinfo;
    FC_CHashStr<CG_LIBContext*> m_LIBinfo;


    //4CP stuff these members are initialised on load:
    //m_sz4CPFile: 
    //  file name exactly as passed in load4CP 
    //  may or may not have a full or relatve path
    //  it is what the user typed on the command line.
    //m_szFileFullPath: 
    //  after m_sz4CPFile as been made a full path assuming the 
    //  current working folder if m_sz4CPFile was relative.
    //m_szFileNameNoExt:
    //  no path nor extension, this is considered to be the 
    //  project name.
    //m_szPrjPath:
    //  full path to project
    FC_CStr   m_sz4CPFile;
    FC_CStr   m_sz4CPFileForDiag;
    FC_CStr   m_szFileFullPath;
    FC_CStr   m_szFileNameNoExt;
    FC_CStr   m_szPrjPath;
    FC_CStr   m_szGenPath;
    FC_CStr   m_szInstDir;
    CG_Edipos m_edp4CPBeg;

    bool               m_bDollarInsHelp;

    bool initFileNames(
        CGT_CDiagMsg* pdm,
        const TCHAR*  pszInstPath,
        const TCHAR*  psz4CPFile
    );

public:
    ST_Parse4CP();
    ~ST_Parse4CP(); 


//load or reload (clears old content):
    bool load(
        CGT_CDiagMsg* pdm,
        const TCHAR*  pszInstPath,
        const TCHAR*  psz4CPFile
    );

//create all hashes to enable getXXXinfo, getAllXXXs 
//todo so need the kad info. Also misplaced file (wrong e.g. 
//or unknown extensions are diagnosed. Return false if any fatal 
//error.
    bool buildPOUHashes(
        CGT_CDiagMsg*   pdm,
        const CKADInfo& kad
    );


    void clear();
    bool is4CPLoaded(void)const;

//4CP directory and file info service:
    const TCHAR*     getDiagFileName(void)const{return m_sz4CPFileForDiag;}
    const CG_Edipos* getEdpBeg()const{return &m_edp4CPBeg;}
    const TCHAR*     getFileAndPath(void)const;
    const TCHAR*     getPrjName(void)const;
    const TCHAR*     getPrjPath(void)const;
    const TCHAR*     getGenPath(void)const;

    bool expandFileInserts(
        CGT_CDiagMsg*    pdm,
        const CG_Edipos* pEdp, 
        FC_CString*      pVal
    );

//raw list like access to all sections:
//getSectionNames returns a NULL terminated array 
//of section names.
    const TCHAR*const*   getSectionNames();
    const CG_AttribList* getSection(const TCHAR* pszSec);

    const CG_Edipos* getEDPOfSection(const CG_AttribList* pSec)const
    {
        return pSec->nAttribs>0 ? &pSec->pairs[0].edpKey : &m_edp4CPBeg; //TODO edp of [section]?
    }

//fast hash access to compiler speciffc sections:
//pszTAR, pszRES and pszPOU are the symbolic IEC names
//without extension and path, youget on return the corresponding
//4CP info or NULL if not found.
    const CG_POU4CPInfo* getTARinfo(const TCHAR* pszTAR);
    const CG_POU4CPInfo* getCONinfo(const TCHAR* pszCON);
    const CG_POU4CPInfo* getPOUinfo(const TCHAR* pszPOU);
    const CG_LIBContext* getLIBinfo(const TCHAR* pszLIB);

    const CG_POU4CPInfo*const* getAllCSTs(void);
    const CG_POU4CPInfo*const* getAllGVLs(void);
    const CG_LIBContext*const* getAllLIBs(void);
    const CG_POU4CPInfo*const* getAllPOUs(void);
};


#endif // __PARSE4CP_H_



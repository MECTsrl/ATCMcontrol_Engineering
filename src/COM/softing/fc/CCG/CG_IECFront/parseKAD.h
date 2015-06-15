#ifndef __PARSEKAD_H_
#define __PARSEKAD_H_

//----  Aggregate Includes:   --------------------------------------*
#include "fc_todebug\fc_assert.h"
#include "fc_tools\fc_cgarray.h"
#include "fc_tools\fc_cstring.h"
#include "CG_IECFront.h"

//----  Forward Class Definitions:   -------------------------------*
class CGT_CDiagMsg;
//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


class CKADTarget{
public:
    FC_CStr     szExt;
    FC_CStr     szBackend;
    GUID        clsidTargetInfo;
    bool        bMultipleTrgs;
};

class CKADFilter{
public:
    FC_CStr     szExt;
    GUID        clsidFilter;
};


/* Example KAD:
<?xml version="1.0" encoding="UTF-8"?>
<CGKAD>
    <TARGETS>
        <TARGET EXT="pc" BACKEND="pc\4PCCode.exe" GETTARGETINFO="uuid1"/>
        <TARGET EXT="pam" BACKEND="pam\4PAMCode.exe" GETTARGETINFO="uuid2"/>
    </TARGETS>

    <LANGUAGES>
         <FILETYP EXT="pn" IECFILTER="uuid3" />
    </LANGUAGES>
</CGKAD >
*/
class CKADInfo
{
    //KAD stuff:
    FC_CStr         m_szKADFile;       //full path and file name of 4cg.kad
    FC_CString      m_szKADTargetExts; //comma seperated target type list e.g. "pc,pam,sam"
    FC_CString      m_szKADSourcesExts;//comma seperated source type list e.g. "4cp,st,4gr,gvl,pn"
    FC_CString      m_szKADLangExts;   //comma seperated language type list e.g. "st,4gr,pn"
    FC_CGArray<CKADTarget*> m_targets; //list of target type info structs
    FC_CGArray<CKADFilter*> m_filters; //list of IEC filter info structs

    void clear();

public:

    CKADInfo() : m_szKADSourcesExts(64),
                m_szKADLangExts(64),
                m_szKADTargetExts(64)
    {}
    ~CKADInfo()
    {
        clear();
    }


    bool load(
        CGT_CDiagMsg* pdm,
        const TCHAR*  pszKADFileAndPath
    );


    bool isLoaded()const{return !m_szKADFile.isEmpty();}
    const TCHAR* getFile(void)const;
    
    int countTargetTypes(void)const;
    
    const TCHAR* getTargetTypeExts(void)const;
    
    int getTargetTypeByExt(
        const TCHAR*  pszExt,
        const TCHAR** ppsz,
        GUID*         puuidTrgInfo,
        boolean*      pbMultipleTrgs
    )const;
    const TCHAR* getTargetTypeByNum(
        int           iTargetType,
        const TCHAR** ppsz,
        GUID*         puuidTrgInfo,
        boolean*      pbMultipleTrgs
    )const;
    
    const TCHAR* getSourceExts()const;
    
    const TCHAR* getLangExts()const;
    
    CG_SOURCETYPES getSourceTypeByExt(
        const TCHAR* pszExt
    )const;
    
    bool getFilterByExt(
        const TCHAR* pszExt, 
        GUID*        puuidFilter
    )const;
};




#endif // __PARSEKAD_H_



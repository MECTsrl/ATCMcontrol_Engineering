
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




/* Example KAD:
<?xml version="1.0" encoding="UTF-8"?>
<KAD4CPC> 
   <IOCOMPONENTS>
       <FILE EXT="pam"  CONFIGURATOR="<uuid>" />
       <FILE EXT="iodp" CONFIGURATOR="<uuid>" />
   </IOCOMPONENTS>
</KAD4CPC>
*/
struct IO_COMP_TYP
{
    FC_CStr  szExt;
    GUID     clsidFBUS;
};
class CKADInfo
{

    FC_CStr                  m_szKADFile; //full path and file name of 4cg.kad
    FC_CGArray<IO_COMP_TYP*> m_IOcomp;    //list of IO components 


    void clear();

public:

    CKADInfo(){}
    ~CKADInfo()
    {
        clear();
    }


    bool load(
        CGT_CDiagMsg* pdm,
        const TCHAR*  pszKADFileAndPath
    );


    //get information:
    bool isLoaded()const{return !m_szKADFile.isEmpty();}
    const TCHAR* getFile(void);
    bool getIOCompByExt(const TCHAR* pszExt, GUID* pClsid)const;
};




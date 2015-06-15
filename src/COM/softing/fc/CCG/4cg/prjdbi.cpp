

//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "fc_tools\fc_fileIO.h"
#include "cgt_misc.h"
#include "cgt_tools.h"
#include "prjdbi.h"

//----  Static Initializations:   ----------------------------------*



//want to dump:
// <?xml version="1.0" encoding="UTF-8"?>
// <POUS>  
// <PROJECT ID="Test1.4cp">
//    <TARGET ID="WhiskeyPlant" PCNAME="LocalPC" PCADDR="W-lue1">
//        <RESOURCEREF ID="Resource1" DBI="pc1\pc1.dbi"/>
//    </TARGET>
// </PROJECT>
// </POUS>

// 23.08.06 SIS >>
// sorting of targets for watch selection dialog
static int CompareTargets(const void *elem1, const void *elem2)
{
    const TargetAndType *nwe1 = (const TargetAndType*)elem1;
    const TargetAndType *nwe2 = (const TargetAndType*)elem2;
    
    return _tcsicmp(nwe1->pszTrg, nwe2->pszTrg);
}

extern "C" void SortTargetsByName(FC_CGArray<TargetAndType>*  pTargets)
{
    int     iElemCount = pTargets->getUsed();
    if(iElemCount > 0)
    {
        qsort(pTargets->getElements(), iElemCount, sizeof(pTargets->get(0)), CompareTargets);
    }
}
// 23.08.06 SIS <<

extern "C" void DumpProjectDBI(
    CGT_CDiagMsg*               pdm,
    ICG_IECFront*               pIFront,
    FC_CGArray<TargetAndType>*  pTargets
)
{
    int           i;
    CGT_CDmp      dmp;
    FC_CString    trg(MAX_PATH);
    FC_CString    trgTRG(MAX_PATH);
    const TCHAR*  pszGenPath;


    pszGenPath = pIFront->FCPgetGenPath();

    dmp<<CGT_XML_STANDARD_HDR _T("\n\n");


    dmp<<_T("<POUS>\n");
    dmp.incrIndent();

    dmp<<_T("<PROJECT ID="); dmp.xmlAttrVal(
         FC_StringGetFilePart(pIFront->FCPgetDiagFileName()))<<_T(">\n");
     
    for(i=0; i<pTargets->getUsed(); i++)
    {
        trg.load(pTargets->getAddrOf(i)->pszTrg);
        trg.changeFileExt(NULL);
        trgTRG.load(trg)<<_T("\\")<<trg<<_T(".trg");//HACK, we must rework the DBI backend of the 4CE

        if(!FC_FileExists(pszGenPath, trgTRG))
        {
            //at least one backend should have produced an error !!"
            //HACK may not be compiled we loop all targets of the 4cp !! assert(pdm->hasErr());
            continue;
        }
        TCHAR* pTxt;
        if(CGT_LoadTextFileToMem(pdm, NULL, pszGenPath, trgTRG, &pTxt, NULL, NULL))
        {
            dmp<<pTxt;
            delete(pTxt);
        }
    }
    dmp<<_T("</PROJECT>\n");

    dmp.decrIndent();
    dmp<<_T("</POUS>\n");

    CGT_DmpToFile(pdm, pszGenPath, _T("project.")CGT_EXT_DBI, dmp, false);
}



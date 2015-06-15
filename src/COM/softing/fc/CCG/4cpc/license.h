
#ifndef __LICENSE_H_
#define __LICENSE_H_

#include "helper.h"

//----  Prototypes:    ---------------------------------------------*

struct SL_4CPCLicData
{
    bool bDemo;        //if any missing license set to true
    
    //next filds are only set if !bDemo:
    bool bWB;          //true: workbench is licensed exists
    bool bPCEL;        //true: a 4cpc engineering license exists
    bool bIsBundle;    //true: workbench is bundle restricted
    long nIOs;         //nIOs only set if bundle, otherwise cannot check (remote!)
};


struct LIC_LIBS{
    const CG_LIBContext* pLIBCxt;
    long                 nLibProdNum;
};

void SL_Get4CPCLicData(
    CGT_CDiagMsg*         pdm, 
    SL_4CPCLicData*       pData,
    FC_CGArray<LIC_LIBS>* pLibs
);




#endif // __LICENSE_H_



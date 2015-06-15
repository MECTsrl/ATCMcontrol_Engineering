
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include <tchar.h>
#include "lic\fc_slbase.h"    	//lic acces IF
#include "lic\softingProd.h"
#include "lic\4CFeatures.h" 	//ATCM specific feature bit
#include "lic\LicName.h"
#include "license.h"
#include "cgt_cdiagmsg.h"
#include "CG_Messages.h"
//----  Static Initializations:   ----------------------------------*






void SL_Get4CPCLicData(
    CGT_CDiagMsg*         pdm, 
    SL_4CPCLicData*       pData,
    FC_CGArray<LIC_LIBS>* pLibs
)
{
    long                            lRet;
    long                            dwFeat;
    long                            lHLeft;
    long                            lMedia;
    long                            lProdNum;
    const TCHAR*                    pszProd;
    HMODULE                         hLicDll;
    SLIC_GetLicenseForProductProc   pGetLicenceForProdukt;
    
    
    memset(pData, 0, sizeof(*pData));


    //get the PC Engineering license first:
    lProdNum = SPROD_4C_21_PC_EL;
    pszProd  = SPROD_4C_21_PC_EL_NAME;
    lMedia   = SLIC_MEDIA_ANY;




    // load the licence DLL
    hLicDll = LoadLibrary(_T("fc_slbase20.dll"));
    if (hLicDll == NULL)
    {
        pdm->msg2(CG_W_LICENSE_INTERNAL, NULL, pszProd, _T("998"));
        pData->bDemo = true;
        return;
    }

    // get the function pointer
    pGetLicenceForProdukt = (SLIC_GetLicenseForProductProc) 
          GetProcAddress(hLicDll, _T("SLIC_GetLicenseForProduct"));
    if (pGetLicenceForProdukt == NULL)
    {
        pdm->msg2(CG_W_LICENSE_INTERNAL, NULL, pszProd, _T("999"));
        pData->bDemo = true;
        return;
    }









    
    lRet = pGetLicenceForProdukt(lProdNum, &dwFeat, &lHLeft, &lMedia);
    if(lRet==SLIC_OK)
    {
        pData->bPCEL = true;
        pData->bIsBundle = (dwFeat & PC_EL_FEAT_FULLVERSION)==0;
    }
    else if(lRet==SLIC_ERR_NO_LIC_FOUND || lRet==SLIC_ERR_NO_COMP_FOUND)
    {
        pdm->msg1(CG_W_LICENSE_DEMO_IO, NULL, pszProd);
        pData->bDemo = true;
    }
    else if(lRet==SLIC_ERR_TMPLIC_EXPIRED)
    {
        pdm->msg1(CG_W_LICENSE_TEMP_EX, NULL, pszProd);
        pData->bDemo = true;
    }
    else
    {
        pdm->msg2(CG_W_LICENSE_INTERNAL, NULL, pszProd, pdm->jot1()<<lRet);
        pData->bDemo = true;
    }

    if(pData->bDemo)
        return; //if already demo don't examine any further...

    //if the PC engineering is a bundle we need also a control:
    if(pData->bIsBundle)
    {
        //we need a control on this machine:
        lProdNum = SPROD_4C_21_CL;
        pszProd  = SPROD_4C_21_CL_NAME;
        lMedia   = SLIC_MEDIA_ANY;
        
        lRet = pGetLicenceForProdukt(lProdNum, &dwFeat, &lHLeft, &lMedia);
        if(lRet==SLIC_OK)
        {
            switch(dwFeat & 0xf)
            {
            case CL_FEAT_IOS_40:   pData->nIOs =   40; break;
            case CL_FEAT_IOS_100:  pData->nIOs =  100; break;
            case CL_FEAT_IOS_200:  pData->nIOs =  200; break;
            case CL_FEAT_IOS_500:  pData->nIOs =  500; break;
            case CL_FEAT_IOS_1K0:  pData->nIOs = 1000; break;
            case CL_FEAT_IOS_2K0:  pData->nIOs = 2000; break;
            case CL_FEAT_IOS_3K0:  pData->nIOs = 3000; break;
            case CL_FEAT_IOS_5K0:  pData->nIOs = 5000; break;
            case CL_FEAT_IOS_10K0:  pData->nIOs = 10000; break;
            default:
                pdm->msg2(CG_W_LICENSE_INTERNAL, NULL, pszProd, _T("999"));
                pData->bDemo = true;
            }
        }
        else if(lRet==SLIC_ERR_NO_LIC_FOUND || lRet==SLIC_ERR_NO_COMP_FOUND)
        {
            pdm->msg1(CG_W_LICENSE_DEMO_IO, NULL, pszProd);
            pData->bDemo = true;
        }
        else if(lRet==SLIC_ERR_TMPLIC_EXPIRED)
        {
            pdm->msg1(CG_W_LICENSE_TEMP_EX, NULL, pszProd);
            pData->bDemo = true;
        }
        else
        {
            pdm->msg2(CG_W_LICENSE_INTERNAL, NULL, pszProd, pdm->jot1()<<lRet);
            pData->bDemo = true;
        }
    }

    if(pData->bDemo)
        return; //if already demo don't examine any further...


    //check the workbench license if we need it.
    //we don't need a wb only and only if bundle with 40 IOs (strating edition)
    if(!pData->bIsBundle || (pData->bIsBundle && pData->nIOs!=40))
    {
        lProdNum = SPROD_4C_21_WB;
        pszProd  = SPROD_4C_21_WB_NAME;
        lMedia   = SLIC_MEDIA_ANY;
    
        lRet = pGetLicenceForProdukt(lProdNum, &dwFeat, &lHLeft, &lMedia);
        if(lRet==SLIC_OK)
        {
            //don't have features yet...
            assert(dwFeat==0);
            pData->bWB = true;
        }
        else if(lRet==SLIC_ERR_NO_LIC_FOUND || lRet==SLIC_ERR_NO_COMP_FOUND)
        {
            pdm->msg1(CG_W_LICENSE_DEMO_IO, NULL, pszProd);
            pData->bDemo = true;
        }
        else if(lRet==SLIC_ERR_TMPLIC_EXPIRED)
        {
            pdm->msg1(CG_W_LICENSE_TEMP_EX, NULL, pszProd);
            pData->bDemo = true;
        }
        else
        {
            pdm->msg2(CG_W_LICENSE_INTERNAL, NULL, pszProd, pdm->jot1()<<lRet);
            pData->bDemo = true;
        }
    }


    if(pLibs)
    {
        int       i;
        LIC_LIBS  lb;

        for(i=0; i<pLibs->getUsed(); i++)
        {
            lb = pLibs->get(i);
            lProdNum = lb.nLibProdNum;
            pszProd  = lb.pLIBCxt->pszId;
            lMedia   = SLIC_MEDIA_ANY;

            lRet = pGetLicenceForProdukt(lProdNum, &dwFeat, &lHLeft, &lMedia);
            
            //if dwFeat is not all bit 0 there is something wrong!
            //either the library is invalid (wrong prod number!) or the registry/dongle
            //treat this error as SLIC_ERR_REG_CORRUPTED
            if(lRet==SLIC_OK && dwFeat != 0)
                lRet = SLIC_ERR_REG_CORRUPTED;

            if(lRet==SLIC_OK)
            {
                //don't have features yet...
                assert(dwFeat==0);
                pData->bWB = true;
            }
            else if(lRet==SLIC_ERR_NO_LIC_FOUND || lRet==SLIC_ERR_NO_COMP_FOUND)
            {
                pdm->msg1(CG_W_LICENSE_DEMO_IO, &lb.pLIBCxt->edp, pszProd);
                pData->bDemo = true;
            }
            else if(lRet==SLIC_ERR_TMPLIC_EXPIRED)
            {
                pdm->msg1(CG_W_LICENSE_TEMP_EX, &lb.pLIBCxt->edp, pszProd);
                pData->bDemo = true;
            }
            else
            {
                pdm->msg2(CG_W_LICENSE_INTERNAL, &lb.pLIBCxt->edp, pszProd, pdm->jot1()<<lRet);
                pData->bDemo = true;
            }
        }
    }

}


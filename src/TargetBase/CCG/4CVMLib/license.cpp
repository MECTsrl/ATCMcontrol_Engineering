/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/license.cpp 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: license.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/license.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / CCG
 *
 * =COMPONENT			4CVMLib
 *
 * =CURRENT 	 $Date: 28.02.07 19:04 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 

/* ----  Includes:	 ---------------------------------------------------------- */

#include <tchar.h>
#include "lic\fc_slbase.h"    //lic acces IF
#include "lic\softingProd.h"  //wide product numbers
#include "lic\4CFeatures.h" //4C specific feature bit
#include "lic\LicName.h"
#include "license.h"
#include "cgt_cdiagmsg.h"
#include "CG_Messages.h"
#include "4cvmAdapt.h"
//----  Static Initializations:   ----------------------------------*





void SL_Get4CLicData(
    CGT_CDiagMsg*         pdm, 
    SL_4CVMLicData*       pData,
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
    long                            dwStartKitBitPos;
    
    memset(pData, 0, sizeof(*pData));
    
    pData->bConsiderLic = true;

    //get the VM Engineering addon license first:
    Adapt4CVM_GetLicensingInfo(lProdNum, pszProd, lMedia); 
    dwStartKitBitPos = 0; //EC_EL_FEAT_FULLVERSION TODO should be a parameter of Adapt4CVM_GetLicensingInfo

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
        pData->bVMEL = true;
        if(dwStartKitBitPos != 0)
            pData->bIsStartKit = (dwFeat & dwStartKitBitPos)==0;
        else
            pData->bIsStartKit = false; //d-2205: HACK for Osai,Cmz never assume a starter kit, this is only for 4cec!
                                        //see also in globctx.cpp where VM_STARTING_KIT_MAX_IO is used
                                        //we need to rework the way how starterkit bit is set here
                                        //it must be an AdaptXXX function !
    }
    else if(lRet==SLIC_ERR_NO_LIC_FOUND || lRet==SLIC_ERR_NO_COMP_FOUND)
    {
        pdm->msg1(CG_E_LICENSE_NOT_FOUND, NULL, pszProd);
        pData->bDemo = true;
    }
    else if(lRet==SLIC_ERR_TMPLIC_EXPIRED)
    {
        pdm->msg1(CG_W_LICENSE_TEMP_EX, NULL, pszProd);
        pdm->msg0(CG_I_TO_MANY_ERRORS, NULL); //d-218; //TODO make new msg id CG_E_LICENSE_TEMP_EX in front end mc file.
        exit(1);
        pData->bDemo = true;
    }
    else
    {
        pdm->msg2(CG_W_LICENSE_INTERNAL, NULL, pszProd, pdm->jot1()<<lRet);
        pdm->msg0(CG_I_TO_MANY_ERRORS, NULL); //d-218; //TODO make new msg id CG_E_LICENSE_INTERNAL in front end mc file.
        exit(1);
        pData->bDemo = true;
    }


    if(pData->bDemo)
        return; //if already demo don't examine any further...


    //check the workbench license if we need it.
    //we don't need a wb licence only and only if we have a starting edition
    //We need a WB license only if starting edition theory is enabled by dwStartKitBitPos!=0:
    if(dwStartKitBitPos!=0 && !pData->bIsStartKit)
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
            pdm->msg1(CG_E_LICENSE_NOT_FOUND, NULL, pszProd);
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
                pdm->msg1(CG_E_LICENSE_NOT_FOUND, &lb.pLIBCxt->edp, pszProd);
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








void SL_Get4CECLicData(
    CGT_CDiagMsg*         pdm, 
    SL_4CVMLicData*       pData,
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
    
    pData->bConsiderLic = true;

    //get the VM Engineering addon license first:
    Adapt4CVM_GetLicensingInfo(lProdNum, pszProd, lMedia);

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
        pData->bVMEL = true;
        pData->bIsStartKit = (dwFeat & PC_EL_FEAT_FULLVERSION)==0;
    }
    else if(lRet==SLIC_ERR_NO_LIC_FOUND || lRet==SLIC_ERR_NO_COMP_FOUND)
    {
        pdm->msg1(CG_E_LICENSE_NOT_FOUND, NULL, pszProd);
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


    //check the workbench license if we need it.
    //we don't need a wb licence only and only if we have a starting edition
    if( ! pData->bIsStartKit)
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
            pdm->msg1(CG_E_LICENSE_NOT_FOUND, NULL, pszProd);
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
                pdm->msg1(CG_E_LICENSE_NOT_FOUND, &lb.pLIBCxt->edp, pszProd);
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





void SL_GetCobanLicData( //HACK for Coban: beeter would be to call AddonHandler.dll export function
                         //because the AddonHandler.dll is changed for every customer anyway
                         //and the 4cvm component shall be unchanged.
    CGT_CDiagMsg*         pdm, 
    SL_4CVMLicData*       pData,
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


    //get the VM Engineering addon license first:
    lProdNum = 19;
    pszProd  = _T("Coban Control");
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
        pData->bVMEL = true;
        // we dont have a starter kit for Coban pData->bIsStartKit = (dwFeat & PC_EL_FEAT_FULLVERSION)==0;
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


    //check the workbench license if we need it.
    //we don't need a wb licence for Coban
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

/* ---------------------------------------------------------------------------- */

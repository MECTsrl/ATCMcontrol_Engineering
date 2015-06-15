/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/license.h 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: license.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/license.h $
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
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _LICENSE_H_
#define _LICENSE_H_

#include "helper.h"

//----  Prototypes:    ---------------------------------------------*

struct SL_4CVMLicData
{
    bool bDemo;        //if any missing license set to true
    
    //next filds are only set if !bDemo:
    bool bWB;          //true: workbench is licensed exists
    bool bVMEL;        //true: a 4CVM workbench addon license exists
    bool bIsStartKit;  //true: allows only 6 boolean I/O and max 2 targets
    bool bConsiderLic; //true: check for license restrictions
};


struct LIC_LIBS{
    const CG_LIBContext* pLIBCxt;
    long                 nLibProdNum;
};

void SL_Get4CLicData(
    CGT_CDiagMsg*         pdm, 
    SL_4CVMLicData*       pData,
    FC_CGArray<LIC_LIBS>* pLibs
);

void SL_Get4CECLicData(
    CGT_CDiagMsg*         pdm, 
    SL_4CVMLicData*       pData,
    FC_CGArray<LIC_LIBS>* pLibs
);

void SL_GetCobanLicData( //HACK for Coban: beeter would be to call AddonHandler.dll export function
                         //because the AddonHandler.dll is changed for every customer anyway
                         //and the 4cvm component shall be unchanged.
    CGT_CDiagMsg*         pdm, 
    SL_4CVMLicData*       pData,
    FC_CGArray<LIC_LIBS>* pLibs
);

#endif

/* ---------------------------------------------------------------------------- */

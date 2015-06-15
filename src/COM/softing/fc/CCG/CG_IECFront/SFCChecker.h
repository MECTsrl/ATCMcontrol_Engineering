
#ifndef __SFCCHECKER_H_
#define __SFCCHECKER_H_


//----  Aggregate Includes:   --------------------------------------*
#include "CGT_CDiagMsg.h"
#include "fc_tools\fc_cgarray.h"
#include "fc_tools\fc_cstring.h"

//----  Forward Class Definitions:   -------------------------------*
struct SFC_CHECKER;

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*




#ifdef __cplusplus
extern "C" {
#endif


SFC_CHECKER* ST_SFC_CreateSFCChecker(CGT_CDiagMsg* pdm);
void  ST_SFC_FreeSFCChecker(SFC_CHECKER* pHdl);


bool  ST_SFC_AddSFCName    (SFC_CHECKER* pHdl,
                            const TCHAR* pszName,
                            const CG_Edipos* ppos);
bool  ST_SFC_AddInitialStep(SFC_CHECKER* pHdl,
                            const TCHAR* pszName,
                            const CG_Edipos* ppos);
bool  ST_SFC_AddStep       (SFC_CHECKER* pHdl,
                            const TCHAR* pszName,
                            const CG_Edipos* ppos);
bool  ST_SFC_AddTransition (SFC_CHECKER* pHdl,
                            int prio, 
                            const FC_CStrList* fromList, 
                            const FC_CStrList* toList,
                            bool fromSim, 
                            bool toSim,
                            const CG_Edipos* ppos,
                            FC_CGArray<CG_Edipos> *pfromPos,
                            FC_CGArray<CG_Edipos> *ptoPos);
bool ST_SFC_AddTransitionNamed(SFC_CHECKER* pHdl,
                            const TCHAR* pszName,
                            int prio, 
                            const FC_CStrList* fromList, 
                            const FC_CStrList* toList,
                            bool fromSim, 
                            bool toSim,
                            const CG_Edipos* ppos,
                            FC_CGArray<CG_Edipos> *pfromPos,
                            FC_CGArray<CG_Edipos> *ptoPos);
bool  ST_SFC_AddActionBlock(SFC_CHECKER* pHdl,
                            const TCHAR* stepName, 
                            const TCHAR* actionName, 
                            const TCHAR* qualifier,
                            const long timeParam,
                            const CG_Edipos* ppos);
bool  ST_SFC_AddAction     (SFC_CHECKER* pHdl,
                            const TCHAR* pszName,
                            const CG_Edipos* ppos);
bool  ST_SFC_Check(
    SFC_CHECKER*     pHdl,
    const CG_Edipos* pEndOfFile
);
bool  ST_SFC_CheckDebug    (SFC_CHECKER* pHdl, CG_Edipos* pEndOfFile);

int   ST_SFC_GetAnzSteps   (SFC_CHECKER* pHdl);
int   ST_SFC_GetAnzTrans   (SFC_CHECKER* pHdl);
int   ST_SFC_GetAnzActions (SFC_CHECKER* pHdl);
int   ST_SFC_GetInitStep   (SFC_CHECKER* pHdl);

int   ST_SFC_GetAnzNextSteps (SFC_CHECKER* pHdl);
int   ST_SFC_GetAnzPrevSteps (SFC_CHECKER* pHdl);
int   ST_SFC_GetAnzStepAction(SFC_CHECKER* pHdl);

int   ST_SFC_GetFollowTrans                   (SFC_CHECKER* pHdl, int nr);
int   ST_SFC_GetAltTrans                      (SFC_CHECKER* pHdl, int nr);
int   ST_SFC_GetNextSteps                     (SFC_CHECKER* pHdl, int nr);
int   ST_SFC_GetNextStepsOffset               (SFC_CHECKER* pHdl, int nr);
int   ST_SFC_GetPrevSteps                     (SFC_CHECKER* pHdl, int nr);
int   ST_SFC_GetPrevStepsOffset               (SFC_CHECKER* pHdl, int nr);
int   ST_SFC_GetStepActionBlockOffset         (SFC_CHECKER* pHdl, int nr);
int   ST_SFC_GetStepActionBlocks              (SFC_CHECKER* pHdl, int nr);
int   ST_SFC_GetStepActionBlockQualifier      (SFC_CHECKER* pHdl, int nr);
int   ST_SFC_GetStepActionBlockTimeParameter  (SFC_CHECKER* pHdl, int nr);

//next functions return 0 if array to short:
int ST_SFC_GetFollowTransArray                 (SFC_CHECKER* pHdl, long* pArr/*[nElems]*/, int nElems);
int ST_SFC_GetAltTransArray                    (SFC_CHECKER* pHdl, long* pArr/*[nElems]*/, int nElems);
int ST_SFC_GetNextStepsArray                   (SFC_CHECKER* pHdl, long* pArr/*[nElems]*/, int nElems);
int ST_SFC_GetNextStepsOffsetArray             (SFC_CHECKER* pHdl, long* pArr/*[nElems]*/, int nElems);
int ST_SFC_GetPrevStepsArray                   (SFC_CHECKER* pHdl, long* pArr/*[nElems]*/, int nElems);
int ST_SFC_GetPrevStepsOffsetArray             (SFC_CHECKER* pHdl, long* pArr/*[nElems]*/, int nElems);
int ST_SFC_GetStepActionBlockOffsetArray       (SFC_CHECKER* pHdl, long* pArr/*[nElems]*/, int nElems);
int ST_SFC_GetStepActionBlocksArray            (SFC_CHECKER* pHdl, long* pArr/*[nElems]*/, int nElems);
int ST_SFC_GetStepActionBlockQualifierArray    (SFC_CHECKER* pHdl, long* pArr/*[nElems]*/, int nElems);
int ST_SFC_GetStepActionBlockTimeParameterArray(SFC_CHECKER* pHdl, long* pArr/*[nElems]*/, int nElems);


 

int   ST_SFC_IsStep                           (SFC_CHECKER* pHdl, const TCHAR* pName);
int   ST_SFC_IsAction                         (SFC_CHECKER* pHdl, const TCHAR* pName);

const TCHAR* ST_SFC_GetStepName               (SFC_CHECKER* pHdl, int nr);
const TCHAR* ST_SFC_GetActionName             (SFC_CHECKER* pHdl, int nr);
const TCHAR* ST_SFC_GetTransName              (SFC_CHECKER* pHdl, int nr);
const CG_Edipos* ST_SFC_GetStepEdiPos         (SFC_CHECKER* pHdl, int nr);
const CG_Edipos* ST_SFC_GetActionEdiPos       (SFC_CHECKER* pHdl, int nr);
const CG_Edipos* ST_SFC_GetTransEdiPos        (SFC_CHECKER* pHdl, int nr);


#ifdef __cplusplus
}
#endif






#endif // __SFCCHECKER_H_




#ifndef __FEATURESET_H_
#define __FEATURESET_H_



//----  Aggregate Includes:   --------------------------------------*
#include "CGT_CDiagMsg.h"
#include "CG_IECFront.h"

//----  Forward Class Definitions:   -------------------------------*
interface ICG_AutoDecl;
//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


class FeatureSet
{
public:
    FeatureSet();
    ~FeatureSet();



    FC_CStr      trgTyp;                       //NULL or the target or back end type.
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

    //taskInit features:
    long			      nTIPairs;            //=4
    FC_CStr*			  psTI;                //={"INTERVAL","PRIORITY","IOSYNC","AUTOSTART"}  
    CG_BITSET*			  pTITypes;            //={"CG_TBS_ANY_INT,CG_TBS_ANY_INT,CG_TBS_ANY_INT,CG_TBS_BOOL}

    boolean bMacroFB;

    long                  nMaxStrLen;          // 0 means no limit; otherwise represents the maximum number of characters
                                               // ex. 0 for 4cpc target
                                               //   255 for 4cwin target
 
    FC_CStr        autoDeclConfigFile;
    CLSID          autodecl;
    ICG_AutoDecl*  pIAutoDecl; //allocated only once.

    DWORD    magic;
};

class CFeatureSet
{
    FeatureSet*     m_pFeat;  //will be set by back end via loadFeatures()

public:
    CFeatureSet();
    ~CFeatureSet();

    void loadFeatures(
        CGT_CDiagMsg*      pdm,
        const CG_Features* pFeatureSet
    );
    
    //gets current features and set this to max. features:
    //caller must delete returned pointer or give back with
    //restoreFeatureSet:
    FeatureSet* getFeatureSet();//HACK
    void restoreFeatureSet(FeatureSet* pFeat);


    const FeatureSet*  m_pCFeat;
    const TCHAR* getTrgTyp()const{return m_pCFeat->trgTyp.isEmpty()?NULL:m_pCFeat->trgTyp.psz();}


    //check for supported features:
    bool checkBasicType(CG_BITSET btype)const
    {
        return ((m_pFeat->allTypes | CG_TBS_ANY_DERIVED) & btype) !=0;
    }

    //return NULL if no auto decl, caller MUST NOT release the IF when done.
    ICG_AutoDecl* getAutoDeclIF(CGT_CDiagMsg* pdm);

};


#endif // __FEATURESET_H_



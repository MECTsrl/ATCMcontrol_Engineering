/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/asm/asmcps.h 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: asmcps.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/asm/asmcps.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / CCG
 *
 * =COMPONENT			ASM
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

#ifndef _ASMCPS_H_
#define _ASMCPS_H_
 
//----  Aggregate Includes:   --------------------------------------*
#include "asmtools.h"
#include "asmtype.h"
#include "vmSharedDef.h"

//----  Forward Class Definitions:   -------------------------------*
class  ASMGlobalContext_C;
struct ASM_OBJECT_TYP;
struct ASM_DECL_TYP;
//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*
struct ASM_COPY_ITEM
{
    ASM_INST_PATH_TYP        inst;// example: program1.fb1.anarray 
    long                     nMem;// >=0:  number of individual members
    const ST_IDENT_TYP*      pMem;// pMem: array of members or NULL (global registered memory)
};





#define ASM_CPY_TYP_NO 0x0
#define ASM_CPY_TYP_RW 0x1
#define ASM_CPY_TYP_RC 0x2

class CASMCopySection
{
    ST_IDENT_TYP                m_typ;
    ST_IDENT_TYP                m_task;
    long                        m_nItems; 
    ASM_COPY_ITEM*              m_pItems;
    
    //set later in check and calculate:
    long                        m_typBits;
    FC_CGArray<XCopyRegion>     m_regions;
    long                        m_lFinalOffset;

    bool check(ASMGlobalContext_C* pCtx);
    bool addRegion(
        ASM_BYTEOFF*          pDestByteOff,
        const ASM_OBJECT_TYP* pObj,
        const ASM_DECL_TYP*   pDecl_first,
        const ASM_DECL_TYP*   pDecl_last
    );


public:
    CASMCopySection(
        ST_IDENT_TYP    typ, 
        ST_IDENT_TYP    task, 
        long            nItems, 
        ASM_COPY_ITEM*  pItems
    );
    ~CASMCopySection();


    ASM_BYTESIZE calculate(ASMGlobalContext_C* pCtx, long lFinalOffset);

    long getTyp()const              {return m_typBits;}
    const TCHAR* getTypName()const  {return m_typ.psz;}
    ST_IDENT_TYP getTaskName()const {return m_task;}
    long getCount()const            {return m_regions.getUsed();}
    long getFinalOffset()const      {return m_lFinalOffset;}

    void dumpMapFile(int* pidx)const;
    void dumpToBinFile(FILE* pFile);

};

#endif

/* ---------------------------------------------------------------------------- */

/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/asm/asmcps.cpp 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: asmcps.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/asm/asmcps.cpp $
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

/* ----  Includes:	 ---------------------------------------------------------- */

#include "vmSharedDef.h"
#include "asmcps.h"
#include "asmstmt.h"
#include "VmConstants.h"
#include "globalContext.h"



CASMCopySection::CASMCopySection(
    ST_IDENT_TYP   typ, 
    ST_IDENT_TYP   task, 
    long           nItems, 
    ASM_COPY_ITEM* pItems
)
{
    m_typ   = typ;
    m_task  = task;
    m_nItems= nItems; 
    m_pItems= pItems; 
    m_typBits  = ASM_CPY_TYP_NO;
    m_lFinalOffset = -1;
}
CASMCopySection::~CASMCopySection()
{
    delete(m_pItems);
}

bool CASMCopySection::check(ASMGlobalContext_C* pCtx)
{
    if(!_tcscmp(m_typ.psz, _T("RW")))
        m_typBits = ASM_CPY_TYP_RW;
    else if(!_tcscmp(m_typ.psz, _T("RC")))
        m_typBits = ASM_CPY_TYP_RC;
    else
    {
        ST_DiagMsg1(NULL, E_4CCG_ERR_UNEXPECTED, m_typ.psz);//TODO: edipos.
        printf("    in section .copy %s %s\n", m_typ.psz, m_task.psz?m_task.psz:"");
        return false;
    }


    if(m_task.psz)
    {
        if(!pCtx->getTaskByName(m_task))
        {
            ST_DiagMsg1(NULL, E_4CCG_ERR_TASK_UNDEFINED, m_task.psz);//TODO: edipos.
            printf("    in section .copy %s %s\n", m_typ.psz, m_task.psz?m_task.psz:"");
            return false;
        }
    }
    //TODO check only one RC and only one RW for a task...
    return true;
}

void CASMCopySection::dumpToBinFile(FILE* pFile)
{
    assert(m_regions.getUsed()>0);

    fwrite(m_regions.getCElements(), m_regions.getUsed(), sizeof(XCopyRegion), pFile);

    m_regions.setUsed(0);
    m_regions.sizeToFit();
}

void CASMCopySection::dumpMapFile(int* pidx)const
{
    int                i;
    const XCopyRegion* pr;

    if(!pidx)
    {
        TO_MAPWRITE4(_T("Typ=%3s task=%-16s:  count=%3d off=%3d\n"), 
                    m_typ.psz, m_task.psz?m_task.psz:"",
                    m_regions.getUsed(), m_lFinalOffset
        );
        return;
    }

    for(i=0;i<m_regions.getUsed();i++)
    {
        pr = m_regions.getAddrOf(i);
        TO_MAPWRITE4(_T("%-4d: %-5u %-4u %u\n"), *pidx, 
            TRANSLATE_WORD((IEC_UINT)pr->uiInst), //TODO: don't swap so early better do so in bin dump !!
            TRANSLATE_WORD((IEC_UINT)pr->uiOffSrc),
            TRANSLATE_WORD((IEC_UINT)pr->uiSize)
        );
        (*pidx)++;
    }
}

bool CASMCopySection::addRegion(
    ASM_BYTEOFF*          pDestByteOff,
    const ASM_OBJECT_TYP* pObj,
    const ASM_DECL_TYP*   pDecl_first,
    const ASM_DECL_TYP*   pDecl_last
)
{
    XCopyRegion         xReg;
    ASM_BITOFF          regBitSize;
    ASM_BYTESIZE        regByteSize;
    const ASM_DECL_TYP* pDecl_last_next;

    assert(pDecl_last->declBitSize>0);
    regBitSize = pDecl_last->declBitOff + pDecl_last->declBitSize - pDecl_first->declBitOff;
    regByteSize = ASM_BITOFF_TO_BYTESIZE(regBitSize);

    if((pDecl_first->declBitOff & 0x7))
    {
        ST_DiagMsg3(NULL, E_4CCG_ERR_CPY_SECTION, m_typ.psz, m_task.psz?m_task.psz:"", pDecl_first->id.psz);
        return false;
    }
    
    pDecl_last_next = pObj->pType->pFBIDef->getDeclByIndex(pDecl_last->nCount+1);
    if(pDecl_last_next && (pDecl_last_next->declBitOff & 0x7)!=0)
    {
        ST_DiagMsg3(NULL, E_4CCG_ERR_CPY_SECTION, m_typ.psz, m_task.psz?m_task.psz:"", pDecl_last->id.psz);
        return false;
    }

    (*pDestByteOff) += regByteSize; 
    xReg.uiInst   = TRANSLATE_WORD((IEC_UINT)pObj->Index);
    xReg.uiOffSrc = TRANSLATE_WORD((IEC_UINT)(pDecl_first->declBitOff/8));
    xReg.uiSize   = TRANSLATE_WORD((IEC_UINT)regByteSize);
    m_regions.add(xReg);
    return true;
}
ASM_BYTESIZE CASMCopySection::calculate(ASMGlobalContext_C* pCtx, long lFinalOffset)
{
    int                         i;
    int                         j;
    bool                        bOk;
    const ASM_OBJECT_TYP*       pObj;
    ASM_COPY_ITEM*              pItem;
    const ASMPROC_TYP*          pProc;
    const ASM_DECL_TYP*         pDecl;
    const ASM_DECL_TYP*         pDecl_first;
    const ASM_DECL_TYP*         pDecl_last;
    ASM_BYTEOFF                 destByteOff;//only used to get the size of the copy dest
                                            //this is only a rough test at compile time 
                                            //we cannot know additional allignment constraints
                                            //at runtime.

    if(!check(pCtx))
        return 0;

    assert(m_lFinalOffset==-1);
    m_lFinalOffset = lFinalOffset;

    destByteOff = 0;

    for(i=0; i<m_nItems; i++)
    {
        pItem = &m_pItems[i];
        pObj = pCtx->lookUpObject(pItem->inst);
        if(!pObj)
        {
            ST_DiagMsg1(NULL, E_4CCG_ERR_UNDEF_VAR, pItem->inst.ids.psz);//TODO: edipos.
            goto Error;
        }
        
        pProc = pObj->pType->pFBIDef;
        if(pItem->nMem>0)
        {
            pDecl_first = pDecl_last = NULL;
            for(j=0; j<pItem->nMem; j++)
            {
                pDecl = pProc ? pProc->lookUpLocalVar(pItem->pMem[j]) : NULL;
                if(!pDecl)
                {
                    ST_DiagMsg1(NULL, E_4CCG_ERR_UNDEF_VAR, pItem->pMem[j].psz);//TODO: edipos.
                    goto Error;
                }
                assert(pDecl->declBitSize>0 && pDecl->declBitOff!=ASM_INVALID_BITOFF && !ASM_IsObject(pDecl->pType));
                if(!pDecl_first)
                    pDecl_first = pDecl;

                bool bNewRegion = false;
                if(pDecl_last && pDecl_last->nCount+1 != pDecl->nCount)//HACK this algorithm works only if the decls are not sorted
                                                                      //and the pItem->pMem order is the same as the pDecl's order !
                {
                    for(int cd=pDecl_last->nCount+1; cd<pDecl->nCount; cd++) //HACK ugly
                    {
                        if(!ASM_IsObject(pProc->getDeclByIndex(cd)->pType))
                        {
                            bNewRegion = true;
                            break;//only a new region if non retain data between...
                                  //Object index may be copied anyway to retain segm.
                        }
                    }
                }
                
                if(bNewRegion)
                {
                    addRegion(&destByteOff, pObj, pDecl_first, pDecl_last);
                    pDecl_first = pDecl;
                    pDecl_last = NULL;
                }
                else
                {
                    pDecl_last = pDecl;
                }
            }
            
            if(pDecl_first)
                addRegion(&destByteOff, pObj, pDecl_first, pDecl_last);
        }
        else if(pObj->pType->bArray)
        {
            assert(!pProc && pObj->pType->nElems>0);
            if(!pProc && pObj->pType->nElems>0)
            {
                XCopyRegion   xReg;
                ASM_BYTESIZE  regByteSize;
                ASM_BITSIZE  elemBitSize;
                ASM_BITOFF32 firstElemBitOff = ASM_GetFirstArrayElemBitOff(pObj->pType, &elemBitSize);

                regByteSize = ASM_BITOFF_TO_BYTESIZE(elemBitSize * pObj->pType->nElems);
                destByteOff  += regByteSize; 
                xReg.uiInst   = TRANSLATE_WORD((IEC_UINT)pObj->Index);
                xReg.uiOffSrc = TRANSLATE_WORD((IEC_UINT)firstElemBitOff/8);
                xReg.uiSize   = TRANSLATE_WORD((IEC_UINT)regByteSize);
                m_regions.add(xReg);
            }
        }
        else if(pProc && pObj->pType->type==ASM_STRI)
        {
            const GArray<ASM_DECL_TYP*>* pDList = pProc->getDecls();
            int iFirst = 0;
            int iLast  = pDList->getMaxUsed()-1;

            while(iLast>=0 && ASM_IsObject(pDList->get(iLast)->pType))
                iLast--;
            while(iFirst<=iLast && ASM_IsObject(pDList->get(iFirst)->pType))
                iFirst++;

            if(iFirst<=iLast)
                addRegion(&destByteOff, pObj, pDList->get(iFirst), pDList->get(iLast));
        }
        else
        {
            ST_DiagMsg1(NULL, E_4CCG_ERR_WRONG_TYPE, _T("ANY STRUCT"));
            goto Error;
        }
    }

    bOk = true;
Error:
    m_nItems= 0; 
    delete(m_pItems);
    m_pItems = NULL;
    if(!bOk)
        printf("    in section .copy %s %s\n", m_typ.psz, m_task.psz?m_task.psz:"");
    return destByteOff;
}

/* ---------------------------------------------------------------------------- */

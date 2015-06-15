/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/asm/asm_ParseContext.h 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: asm_ParseContext.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/asm/asm_ParseContext.h $
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

#ifndef _ASM_PARSECONTEXT_H_
#define _ASM_PARSECONTEXT_H_

#include "asmtools.h"
#include "asmerr.h"
#include "asmstmt.h"
#include "asmcps.h"
#include "asm_lex.h"

struct ASM_OPCDEF;
class asm_ParseContext
{
    friend class asm_parse;
private:
    ST_EdiPos                    m_curredp;
    TCHAR*                       m_pszFileContext;
    unsigned long                m_ulFileLength;
    FC_CGArray<ST_IDENT_TYP>     m_tmpIdList;
    FC_CGArray<ASM_MEM_REGION>   m_tmpRegionList;
    FC_CGArray<ASM_COPY_ITEM>    m_tmpCopyItemList;

public:
    FileContext_C*                      m_pfc;
    ASM_ScannerStruct*                  m_pScanner;
    ST_LineCol                          m_currPos;
    bool                                m_bEnableInstr;    
    ASMPROC_TYP*                        m_curProc;//current proc parsed
    bool                                m_bIsRetain;
    ASM_TASK_TYP*                       m_pAktTask;
    FC_CGArray<ASM_INIT_REPEAT_VAL_TYP> m_InitList;
    FC_CGArray<ASM_LITERAL_TYP>         m_LitList;
    
    //con/destruction:
    asm_ParseContext(FileContext_C* pfc_in);
    ~asm_ParseContext();

    bool asm_LoadFile();


    //return pointer to current EdiPos of token last read:
    const ST_EdiPos* getEdiPos()
    {
        m_curredp.x = m_currPos.nColumn;
        m_curredp.y = m_currPos.nLine;
        return &m_curredp;
    }
    //update line,col info in current EdiPos:
    const ST_EdiPos* getEdiPos(ST_LineCol pos)
    {
        m_curredp.x = pos.nColumn;
        m_curredp.y = pos.nLine;
        return &m_curredp;
    }
    
    void tokenSyntax(const TCHAR* pszTok);

    void setActualTask(const ST_EdiPos* pEdp,
                       ST_IDENT_TYP*    name);

    const ASM_DATA_TYP* makeType(ST_IDENT_TYP typeName, 
                                 bool bArray,
                                 const ASM_LITERAL_TYP* pArrayDim, 
                                 const ASM_LITERAL_TYP* pStrLen);
    

    void makeBpPos(ST_LineCol             pos, 
                   const ASM_LITERAL_TYP* BpNum,
                   bool                   bInsertOPCode);

    void makeCalop(ST_LineCol   pos, 
                   ST_IDENT_TYP opName);
    
    void makeJmp(ST_LineCol             pos, 
                 ASM_INSTR_ENUM         instr, 
                 ST_IDENT_TYP           label);

    void makeLabel(ST_IDENT_TYP label);
    void checkVarReference(ASM_INST_PATH_TYP path, ASM_PRIMARY_TYP* pNewArg);
    void assignClssid(const ST_EdiPos* pEdp, const ASM_LITERAL_TYP& lit);
    void addProc(const ST_EdiPos*       pEdp, 
                 ST_IDENT_TYP           name, 
                 ASM_PROC_ENUM          kind, 
                 const ASM_LITERAL_TYP& lit);

    void appendRegionList(const ST_EdiPos* pEdp,
                         ST_IDENT_TYP     name,
                         ASM_MEMRGN_TYP   typ);

    void addCopyItem(ASM_INST_PATH_TYP path, FC_CGArray<ST_IDENT_TYP>* pMembers);


    FC_CGArray<ASM_INIT_REPEAT_VAL_TYP>* getInitList();

    ASM_LITERAL_TYP* makeLiteralList(const ASM_LITERAL_TYP& lit);
    ASM_LITERAL_TYP* getLiteralList();
};

bool ASM_LitTokenToLiteral(const ST_EdiPos* pEdp, const TCHAR* pText, ASM_LITERAL_TYP* pLit);
bool ASM_LitOffsetToLiteral(const TCHAR* pText, ASM_LITERAL_TYP* pLit);
bool ASM_LitObjidToLiteral(const TCHAR* pText, ASM_LITERAL_TYP* pLit);

#endif

/* ---------------------------------------------------------------------------- */

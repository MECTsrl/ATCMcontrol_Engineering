/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/asm/asmmain.cpp 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: asmmain.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/asm/asmmain.cpp $
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

static char szversioninfo[] = "$Header: /4CReleased/V2.20.00/TargetBase/CCG/asm/asmmain.cpp 1     28.02.07 19:04 Ln $";

#include "BuildNr.h"
#include "Product.h"
#include "resource.h"
#include "globalContext.h"


//memory allocation:
#define FC_LIB_USE_TODEBUG
#include "fc_tools\fc_libmem.h"



void *operator new(size_t s)
{
  void *p;
  FC_NewMemory(&p,s);
  return p;
}
void operator delete(void *p)
{
  if (p) 
    FC_FreeMemory(p);
}


static const TCHAR* GetCommandLineLogo()
{
    return _T(PROD_PRODUCTNAME) _T(" 4CVM Assembler ")
           _T(PROD_FULL_VERSION_STR) _T("\r\n")
           _T(PROD_COPYRIGHT) _T("\r\n\r\n");
}

static bool DumpLogoAndHelp(int argc, char** argv)
{
    int          i;
    bool         bNologo = false;
    bool         bHelp   = false;
    const TCHAR* pszLogo = GetCommandLineLogo();

    for(i=0;i<argc;i++)
    {
        if(argv[i][0]==_T('-') || argv[i][0]==_T('/'))
        {
            if(!_tcsicmp(argv[i]+1, _T("nologo")))
                bNologo = true;
            else if(!_tcsicmp(argv[i]+1, _T("?")) || !_tcsicmp(argv[i]+1, _T("help")))
                bHelp = true;
        }   
    }
    
    if(!bNologo && pszLogo)
    {
        //HACK: replace © by (C) for dos box ...
        TCHAR* pszBuffer = new char[_tcslen(pszLogo)+1];
        _tcscpy(pszBuffer, pszLogo);

        TCHAR* pszCopyRight = _tcsstr(pszBuffer, _T("©"));
        if(pszCopyRight)
        {
            *pszCopyRight = 0;

            DumpUserText(pszBuffer);
            DumpUserText(_T("(C)"));
            DumpUserText(pszCopyRight+1);
        }
        else
        {
            DumpUserText(pszBuffer);
        }
        
        delete(pszBuffer);

    }

    if(bHelp)
        ST_InfoMessage0(E_4CCG_INF_CMD_LINE_HELP);

    return bHelp;
}


int main(int argc, char** argv)
{
    ASMGlobalContext_C  gc;
#ifdef DEBUG
    FC_SetAllocBreak(0);
#endif
    ASM_InitErrors();

    if(DumpLogoAndHelp(argc, argv))
        return 0;

    gc.parseCommandLine(argc, argv);

    if(ASM_GetTotalErrors()==0)
        gc.initOpCodes();

    if(ASM_GetTotalErrors()==0)
        gc.parseAsmFile();

    if(ASM_GetTotalErrors()==0)
        gc.calcMemberOffsets();

    if(ASM_GetTotalErrors()==0)
        gc.calcObjectNumbers();


    if(ASM_GetTotalErrors()==0)
        gc.calcGlobalOffsets();

    if(ASM_GetTotalErrors()==0)
        gc.calcOffsetLiterals();

    if(ASM_GetTotalErrors()==0)
        gc.calcConfigData();

    if(ASM_GetTotalErrors()==0)
        gc.calcInstructions();

    if(ASM_GetTotalErrors()==0)
        gc.calcCopySections();

    if(ASM_GetTotalErrors()==0)
        gc.dumpDataAndCodeFiles();

    if(ASM_GetTotalErrors()==0)
        gc.checkSizes();

    //if(ASM_GetTotalErrors() || ASM_GetTotalWarnings())
    // ???? no err sum because called from 4ccg       ST_InfoMessage2(E_4CCG_FINAL_ERR_SUM, Tostr_C(ASM_GetTotalErrors()), Tostr_C(ASM_GetTotalWarnings())); 

    TO_CloseMapFile();

    return ASM_GetTotalErrors();
}

/* ---------------------------------------------------------------------------- */

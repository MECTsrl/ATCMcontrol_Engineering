


#define ST_IS_NAME_EQUAL(x,y) (!lstrcmpi(x,y))


//----  Includes:   -------------------------------------------*
#include <stdio.h>

#include "helper.h"
#include "BuildNr.h"
#include "Product.h"
#include "defines.h"
#include "fc_tools\fc_registry.h"
#include "fc_tools\fc_system.h"




const TCHAR* Get4CCopyRightSourceHeader()
{
    return _T("/* ") _T(PROD_PRODUCTNAME) _T(" ") TARGET_EXT _T("-back end ")
           _T(PROD_FULL_VERSION_STR) _T("\n * ")
           _T(PROD_COPYRIGHT) _T("\n */\n\n");
}
const TCHAR* GetGeneratorVersionString()
{
    const TCHAR* psz =  TARGET_EXT _T(PROD_PRODUCTNAME) _T(" ") _T(PROD_FULL_VERSION_STR);
    return psz;
}



void SetFeatures(ICG_IECFront* pIFront)
{
    CG_Features  feat;
    
    memset(&feat, 0, sizeof(CG_Features));

    //TODO make _CG_Features complete !!!! 
    feat.pszTargetTyp = TARGET_EXT;

    //languages (ST is mandatory):
    feat.bLadder = true;
    feat.bFBD    = true;
    feat.bSFC    = true;
  
    //declaration (header) features:
    feat.bIncompleteAddr    = 1;      //allow %* and %+ syntax
    feat.bAssumedSizeArrays = 1;      //allow ARRAY OF <type> syntax
    feat.declModifiers      = CG_DMOD_ANY_RETAIN|CG_DMOD_CONSTANT;
    feat.bMemoryDirectAddr  = 1;      //for example: CANopen %MS4.3!emcy

    feat.allTypes          = SUPPORTED_BASIC_TYPES;
    feat.arrayTypes        = (SUPPORTED_BASIC_TYPES|CG_TBS_STRUCT);
    feat.structMemberTypes = SUPPORTED_BASIC_TYPES|CG_TBS_ARRAY|CG_TBS_STRUCT;
    feat.funRetTypes       = SUPPORTED_BASIC_TYPES; //TODO: |CG_TBS_ARRAY|CG_TBS_STRCUT;
    feat.funInputTypes     = SUPPORTED_BASIC_TYPES|CG_TBS_ARRAY|CG_TBS_STRUCT; 
    feat.funOutputTypes    = SUPPORTED_BASIC_TYPES|CG_TBS_ARRAY|CG_TBS_STRUCT;
    feat.funInOutTypes     = SUPPORTED_BASIC_TYPES|CG_TBS_ARRAY|CG_TBS_STRUCT;
    feat.funLocalTypes     = SUPPORTED_BASIC_TYPES|CG_TBS_ARRAY|CG_TBS_STRUCT;
    feat.FBInOutTypes      = SUPPORTED_BASIC_TYPES|CG_TBS_ARRAY|CG_TBS_STRUCT;
    feat.gvlTypes          = SUPPORTED_BASIC_TYPES|CG_TBS_ARRAY|CG_TBS_STRUCT;
 
    feat.preferedInt       = 32;
    feat.preferedFloat     = 64;


    //allowed decl scopes:
    feat.bVarNativeFun  = 1;
    feat.bVarNativeFBPrg= 1;
    feat.bVarInOutFun   = 1; 
    feat.bVarInOutFB    = 1; 
    feat.bVarTmpFun     = 0; 
    feat.bVarTmpFBPrg   = 0; 
    feat.bVarGlobPrg    = 0; 
    feat.bVarExtFun     = 0; 


    //task features:
    TCHAR* ppszTI[]            = { _T("INTERVAL"), _T("PRIORITY"), _T("IOSYNC"),   _T("AUTOSTART") }; 
    const CG_BITSET pTITypes[] = { CG_TBS_TIME,    CG_TBS_ANY_INT, CG_TBS_ANY_INT, CG_TBS_BOOL}; 

    feat.nTIPairs = 4; 
    feat.ppszTI   = ppszTI;	
    feat.pTITypes = pTITypes; 
    
    //SFC features:
    //TODO: action qualifiers

    //general limits nMax_* 
    //TODO e.g. max vars per POU, max struct nesting level, ....
    feat.nMaxStrLen = 0; // no string length limit
    
    feat.pAutodecl = NULL;


     pIFront->loadFeatures(&feat);
}

bool checkTaskSettings(
    CGT_CDiagMsg*              pdm, 
	ICG_IECFront*			   pIF,
	const CG_Resource*		   pRes, 
	const CTarget*			   pTrg
)
{
	const CG_Task*	   pTask;
	const CG_TaskInit* pTaskInit;
	const CG_TaskInit* pTaskInitIOSYNC;
	const CG_TaskInit* pTaskInitInterval;
	const CG_TaskInit* pTaskInitPriority;
	const CG_TaskInit* pTaskInitAutostart;
	int				   IOSYNC_ChannelNum;
	long			   lErrors = pdm->getTotalErrors();
	__int64			   i64;

	for(long i = 0; i < pRes->nTasks; i++)
	{
		pTask = pRes->pTasks+i;
		
		pTaskInitIOSYNC    = NULL;
		pTaskInitInterval  = NULL;
		pTaskInitPriority  = NULL;
		pTaskInitAutostart = NULL;

		// check attributes semmantics
		for(long j = 0; j < pTask->nInits; j++)
		{
			pTaskInit = pTask->pInits+j;
  
			//toassert: if pAttribVal==NULL a parse error must have been happened
			assert(pTaskInit->val.pType && pTaskInit->val.extyp != CG_EXPRTYP_UNDEF);
    
			if(ST_IS_NAME_EQUAL(pTaskInit->pszAttr, _T("PRIORITY")))
			{
				i64 = pTaskInit->val.lit.pLit->i64;
				pTaskInitPriority = pTaskInit;

				if(i64<PC_MIN_TASK_PRIORITY || i64>PC_MAX_TASK_PRIORITY)
				{
					pdm->msg3(CG_E_BAD_TASK_PRIO, &pTaskInit->val.edp,
						pdm->jot1()<<i64,
					    pdm->jot2()<<PC_MIN_TASK_PRIORITY, 
					    pdm->jot3()<<PC_MAX_TASK_PRIORITY);
					const_cast<CG_Literal*>(pTaskInit->val.lit.pLit)->i64 = PC_MIN_TASK_PRIORITY;
				}
			}
			else if(ST_IS_NAME_EQUAL(pTaskInit->pszAttr, _T("INTERVAL")))
			{
				i64 = pTaskInit->val.lit.pLit->i64;
				pTaskInitInterval = pTaskInit;

				if(i64<PC_MIN_TASK_INTERVAL_MS || i64>PC_MAX_TASK_INTERVAL_MS)
				{
					pdm->msg3(CG_E_BAD_TASK_INTERVAL_RANGE, &pTaskInit->val.edp,
						pdm->jot1()<<i64,
					    pdm->jot2()<<PC_MIN_TASK_INTERVAL_MS, 
					    pdm->jot3()<<PC_MAX_TASK_INTERVAL_MS);
					const_cast<CG_Literal*>(pTaskInit->val.lit.pLit)->i64 = PC_MIN_TASK_INTERVAL_MS;
				}
			}
			else if(ST_IS_NAME_EQUAL(pTaskInit->pszAttr, _T("IOSYNC")))
			{
				i64 = pTaskInit->val.lit.pLit->i64;
				pTaskInitIOSYNC = pTaskInit;
				// the value validity will be checked later
			}
			else if(ST_IS_NAME_EQUAL(pTaskInit->pszAttr, _T("AUTOSTART")))
			{
				pTaskInitAutostart = pTaskInit;
			}
			else
				assert(!_T("unknown task attribut accepted by the frontend"));
		}		

		// check attributes compatibility
		if(pTaskInitIOSYNC)
		{       
			assert(pTaskInitIOSYNC->val.extyp==CG_LITERAL && pTaskInitIOSYNC->val.lit.pLit->ut==CG_LIT_I64);

			IOSYNC_ChannelNum = (long)(pTaskInitIOSYNC->val.lit.pLit->i64);

			if(pTaskInitPriority || pTaskInitInterval || pTaskInitAutostart)
			{
				pdm->msg1(CG_E_TASK_ATTR_COMBI, &pTaskInitIOSYNC->edp, pTask->pszId);
			}
			else if(pTrg)
			{
				const PC_CIOComp* pfbm = pTrg->getFBMasterByChannel(IOSYNC_ChannelNum);
				if(pfbm)
				{
					if(!pfbm->isDP())
					{
						pdm->msg3(CG_E_TASK_IOSYNC_NOT_DPFBM, &pTaskInitIOSYNC->val.edp,
							pTask->pszId, pdm->jot1()<<IOSYNC_ChannelNum, 
							pfbm->m_masterName);
					}
				}
				else
				{
                    FC_CString fbmName(32);
                    if(pTaskInitIOSYNC->val.lit.pConst)
                        fbmName<<pTaskInitIOSYNC->val.lit.pConst->pszId<<_T(" (=")<<IOSYNC_ChannelNum<<')';
                    else
                        fbmName<<IOSYNC_ChannelNum;

					pdm->msg3(CG_E_TASK_IOSYNC_BAD_CHANNEL, &pTaskInitIOSYNC->val.edp,
						pTask->pszId, fbmName, pTrg->getTrgName());
				}
			}
		}
		else if(!pTaskInitPriority)
		{
			pdm->msg0(CG_E_PRIO_OR_INTERVAL_MISSING, &pTask->edp);
		}
		else if(pTaskInitInterval && pTaskInitInterval->val.lit.pLit->i64 < PC_MIN_TASK_INTERVAL_NT_MS)
		{
			pdm->msg2(CG_W_TASK_INTERVAL_JITTER, &pTaskInitInterval->edp,
				pdm->jot1()<<pTaskInitInterval->val.lit.pLit->i64, pdm->jot2()<<PC_MIN_TASK_INTERVAL_NT_MS);
		}
	}

	if(lErrors != pdm->getTotalErrors())
		return false;

	return true;
}


void DumpResponseFile(
    CGT_CDiagMsg*              pdm, 
    const TCHAR*               pszTrgType,
    ICG_IECFront*              pIFront,
    const CG_POU4CPInfo*const* pzpProcessed
)
{
    CGT_CDmp                   dmp;
    const CG_POU4CPInfo*const* ppInf;
  
  
    dmp<<pdm->getDisplayedErrors()<<_T("\n");
    dmp<<pdm->getDisplayedWarnings()<<_T("\n");

    for(ppInf = pzpProcessed; *ppInf; ppInf++)
        dmp<<(*ppInf)->pszFile<<_T("\n");
    

    FC_CString resp(MAX_PATH);
    resp.load(pIFront->FCPgetGenPath())<<_T("\\.")<<pszTrgType<<_T("\\result.txt");
    CGT_DmpToFile(pdm, NULL, resp, dmp, false);
}




void ExecuteJvc(
    CGT_CDiagMsg* pdm,
    const TCHAR*  pszOpt,
    const TCHAR*  pszJavaFiles,
    int           nVerbose
)
{
    long            result;
    TCHAR           szDefExe[1028];
    const TCHAR*    pszKey1; 
    const TCHAR*    pszKey2; 
    const TCHAR*    pszVal1;
    const TCHAR*    pszVal2;
    FC_CString      fullcmdl(1028);
    bool            bNotFoundInReg;

    //if option string start with an option - or / 
    //assume that it contains also the java compiler exe:
    szDefExe[0] = 0;
    FC_StringSkipWhite(&pszOpt);
    if(pszOpt[0] && pszOpt[0] == '-' || pszOpt[0] == '/')
    {
        //HACK for sdk 4.0, the fuck sdk does not store it's
        //path in the registry on the old place! The only place we found
        //it in the regisrty is this one:
        bNotFoundInReg= false;
        pszKey1 = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\SDKJava40.exe");
        pszVal1 = _T("Path");

        pszKey2 = _T("SOFTWARE\\Microsoft\\SDKJava");
        pszVal2 = _T("Install Dir");

        result = FC_RegGetStringVal(
                 HKEY_LOCAL_MACHINE, 
                 pszKey1,
                 pszVal1,  
                 szDefExe, 
                 FC_ARRAY_LEN(szDefExe)
             );

        if(result != ERROR_SUCCESS)
        {
            result = FC_RegGetStringVal(
                     HKEY_LOCAL_MACHINE, 
                     pszKey2,
                     pszVal2,  
                     szDefExe, 
                     FC_ARRAY_LEN(szDefExe)
                 );
        }

        if(result == ERROR_SUCCESS)
        {
            _tcscat(szDefExe, _T("\\bin\\jvc.exe"));
            assert(_tcslen(szDefExe)<FC_ARRAY_LEN(szDefExe));
        }
        else
        {
            //if Java SDK 3.2 installed, this does not work.
            //but 4CCG will work if jvc.exe can be found in PATH...
            _tcscpy(szDefExe, _T("jvc.exe"));
            bNotFoundInReg = true;
        }
    }

    fullcmdl<<pszOpt<<' '<<pszJavaFiles;

    if(nVerbose==1)
    {
        if(szDefExe[0])
        { //TODO this is odd!
            pdm->userMsg(pdm->jot1().addpsz(FC_StringGetFilePart(szDefExe)).changeFileExt(NULL));
            pdm->userMsg(_T(" "));
            pdm->userMsg(pszJavaFiles);
        }
        else
        {
            pdm->userMsg(fullcmdl);
        }
        pdm->userMsg(_T("\n"));
    }
    else if(nVerbose==2)
    {
        if(szDefExe[0])
        {
            pdm->userMsg(szDefExe);
            pdm->userMsg(_T(" "));
        }
        pdm->userMsg(fullcmdl);
        pdm->userMsg(_T("\n"));
    }

    fflush(stdout);
    
    result = 1236984624;//<-only to be safe
    if(!FC_System(szDefExe[0]?szDefExe:NULL, fullcmdl, NULL, &result))
    {   
        long lastErr = GetLastError();
        if(szDefExe[0] && lastErr==ERROR_FILE_NOT_FOUND)
        {
            pdm->userMsg(           _T("\n    Note: the java SDK path was searched in the registry in the order:\n"));
            pdm->userMsg(pdm->jot1()<<_T("    Key='HKEY_LOCAL_MACHINE\\")<<pszKey1<<_T("' Value='")<<pszVal1<<_T("'\n"));
            pdm->userMsg(pdm->jot1()<<_T("    Key='HKEY_LOCAL_MACHINE\\")<<pszKey2<<_T("' Value='")<<pszVal2<<_T("'\n"));
        
            if(bNotFoundInReg)
               pdm->userMsg(_T("    but non of these registry keys exists (tried using %PATH%)\n\n"));
        }
        pdm->sysErr(GetLastError(), NULL, szDefExe[0]?szDefExe:fullcmdl.psz(), NULL);

        return;
    }

    if(result!=0)
    {    //bad return from jvc
        pdm->msg1(CG_E_BACKEND_ERROR, NULL, _T("java"));
    }
}


CG_OS_TYPES CG_GetOSVersion()
{
    OSVERSIONINFO VI;

    VI.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&VI);

	if (VI.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		switch (VI.dwMajorVersion)
		{
			case 3 : return E_WIN_NT35;
			case 4 : return E_WIN_NT40;
			case 5 :
				switch(VI.dwMinorVersion)
				{
					case 0: return E_WIN_2000;
					case 1: return E_WIN_XP;
					case 2: return E_WIN_2003_SERVER;
					default:return E_WIN_UNKNOWN;
				}
			break;
			case 6 :
				switch(VI.dwMinorVersion)
				{
					case 0: return E_WIN_VISTA;
					default:return E_WIN_UNKNOWN;
				}
			break;
			default:
			{
				return E_WIN_UNKNOWN;
			}

		} /* switch (VI.dwMajorVersion) */
	}
	else
	{
		return E_WIN_98_95_ME;
	}

    return E_WIN_UNKNOWN;
}


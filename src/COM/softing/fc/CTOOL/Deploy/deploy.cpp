/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/Deploy/deploy.cpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: deploy.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/Deploy/deploy.cpp $
 *
 * =PROJECT				CAK1020  ATCMControl V2.0
 *
 * =SWKE                4CDEPLOY
 *
 * =COMPONENT           Deploy
 *
 * =CURRENT      $Date: 28.02.07 19:01 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *

 



 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  09.07.2002  JD      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/





#include "stdafx.h"
#include "Deploy.h"

#include "ProjectPatcher.h"

#include "BuildNr.h"
#include "Product.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;





int  ParseCmdLine(int argc, TCHAR* argv[], CProjectPatcher& projPatch);
void OutputUsage();



//------------------------------------------------------------------*
/**
 * _tmain: main entry of the console application using MFC.
 *     
*/
int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
        HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
        if(FAILED(hr))
        {
            printf("Fatal Error: CoInitialize failed: hr=%x\n" , hr);
            return 1;
        }

	    CProjectPatcher projPatch;
        bool bSilent = false;
        int iAct = ParseCmdLine(argc, argv, projPatch); 
        if (iAct==0 || iAct==-1)
        {
            OutputUsage();
        }

        if (iAct<=0)
        {
            return iAct;
        }

        nRetCode = projPatch.DoAction();
	}

	return nRetCode;
}


//------------------------------------------------------------------*
/**
 * ParseCmdLine parse the command line options.
 *
 * @param   argc, argv: command line parameter
 * @param   projPather; the object doing the projetc patching work.
 * @return  0 : ShowHelp, exit
 *         -1 : cmd line error -> ShowHelp, exit
 *         -2 : cmd line error + -s -> exit without output
 *         >0 : correct cmd line -> do action
 */
int ParseCmdLine(int argc, TCHAR* argv[], CProjectPatcher& projPatch) 
{
    int iParamCount = 1;

    BOOL bSilent = FALSE;

    // 2 parameter necessary - first exe name
    if (argc<2)
    {
        return 0;
    }

    // check special parameter
    if (argc==2)
    {
        if (stricmp(argv[1], "-help")==0)
        {
            return 0;
        }
        else if (stricmp(argv[1], "-s")==0)
        {
            return -2;
        }
        else
        {
            return -1;
        }
    }

    // first parameter -s silent option
    if (stricmp(argv[1], "-s")==0)
    {
        bSilent = TRUE;
        iParamCount++;
    }

    projPatch.SetSilentMode(bSilent);

    // next comes the project directory
    projPatch.SetProjectDir(argv[iParamCount]);
    iParamCount++;


    if (stricmp(argv[iParamCount], "-show")==0)
    {
        projPatch.SetAction(ACT_SHOW);
        return 1; 
    }
    else if (stricmp(argv[iParamCount], "-patch")==0)
    {
        iParamCount++;
        while (iParamCount<argc)
        {
            if (!projPatch.AddPatchTargetInfo(argv[iParamCount]))
            {
                if (bSilent)
                {
                    return -2;
                }
                else
                {
                    return -1;
                }
            }
            iParamCount++;
        }

        projPatch.SetAction(ACT_PATCH);
        return 1;
    }
    else
    {
        if (bSilent)
        {
            return -2;
        }
        else
        {
            return -1;
        }
    }
    
    return 0;
}


//------------------------------------------------------------------*
/**
 * OutputUsage show a help screen.
 *
 */
void OutputUsage()
{
    CString strCopyRight(PROD_COPYRIGHT);
    strCopyRight.Replace("\251", "(C)");
    printf(PROD_PRODUCTNAME  " "  PROD_FULL_VERSION_STR " 4CDeploy: ATCMControl Deployment Tool\n");
    printf("%s\n\n", strCopyRight);
    
    printf("Usage:\n");
    
    printf("4CDeploy -?|-help\n");
    printf("     show this screen\n");
    
    printf("4CDeploy [-s] <project-file> -show\n");
    printf("     show all target with address info of a project\n");
    
    printf("4CDeploy [-s] <project-file> -patch <trg-name>:<trg-addr> <trg-name>:<trg-addr> ...\n");
    printf("     for all targets given, change the address information\n"
           "     and create the opc server entries\n"
           "     if the project contains only one target,\n"
           "     * can be given as target name for this target\n");

    printf(" -s : silent mode\n");
    printf("\n");
}



/*
 *----------------------------------------------------------------------------*
 * $History: deploy.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CTOOL/Deploy
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/CTOOL/Deploy
 * 
 * *****************  Version 2  *****************
 * User: Jd           Date: 17.09.02   Time: 13:30
 * Updated in $/4Control/COM/softing/fc/CTOOL/Deploy
 * 
 * *****************  Version 1  *****************
 * User: Jd           Date: 17.09.02   Time: 11:08
 * Created in $/4Control/COM/softing/fc/CTOOL/Deploy
 * Moved from Bruneck Target to product.
 * 
 *==
 *----------------------------------------------------------------------------*
*/

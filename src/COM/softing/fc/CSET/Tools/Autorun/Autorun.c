/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/Autorun/Autorun.c 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: Autorun.c $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/Autorun/Autorun.c $
 *
 * =PROJECT				CAK1020  ATCMControl V2.0
 *
 * =SWKE                CSET
 *
 * =COMPONENT           Autorun
 *
 * =CURRENT      $Date: 28.02.07 19:00 $
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
 *  13.12.2001  JD      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/






#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    static char szPath[MAX_PATH];
    static char szFile[MAX_PATH];
    int i;
    int len;

    // generiert den vollen Dateinnamen der INF-Datei
    if (!GetModuleFileName(NULL, szPath, sizeof(szPath)))
    {
        return 0;
    }

    len = lstrlen(szPath);
    for (i=len; i >= 0; i--)
    {
        if (szPath[i] == '\\')
        {
            szPath[i+1] = 0;
            break;
        }

    }

    if (i < 0)
    {
        return 0;
    }

    lstrcpy(szFile, szPath);
    lstrcat(szFile, "Autorun\\Default.htm");


    if ((HINSTANCE)32 >= ShellExecute(NULL, "open", szFile, NULL, NULL, SW_SHOWMAXIMIZED))
    {
        lstrcpy(szFile, szPath);
        lstrcat(szFile, "readme.txt");
        if ((HINSTANCE)32 >= ShellExecute(NULL, "open", "readme.txt", NULL, NULL, SW_SHOW))
        {
            MessageBox(NULL, "Please install MS Internet Explorer 4.0\nand load document Man\\Default.htm.",
                       "4Control", MB_ICONEXCLAMATION | MB_OK);
        }
    }

  return 0;
}


/*
 *----------------------------------------------------------------------------*
 *  $History: Autorun.c $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/Autorun
 * 
 * *****************  Version 2  *****************
 * User: Ln           Date: 24.11.04   Time: 10:52
 * Updated in $/4Control/COM/softing/fc/CSET/Tools/Autorun
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSET/Tools/autorun
 * 
 * *****************  Version 1  *****************
 * User: Jd           Date: 13.12.01   Time: 13:13
 * Created in $/4Control/COM/softing/fc/CSET/Tools/autorun
 * First checkin.
 *==
 *----------------------------------------------------------------------------*
*/

echo off
rem *H>> $Header: /4CReleased/V2.20.00/sign.bat 1     28.02.07 18:51 Ln $
rem *----------------------------------------------------------------------------*
rem *
rem * =FILENAME            $Workfile: sign.bat $
rem *                      $Logfile: /4CReleased/V2.20.00/sign.bat $
rem *
rem * =PROJECT				CAK1020  ATCMControl V2.0
rem *
rem * =SWKE                ATCMControl
rem *
rem * =COMPONENT           Signing Components
rem *
rem * =CURRENT      $Date: 28.02.07 18:51 $
rem *               $Revision: 1 $
rem *
rem * =REFERENCES
rem *
rem *----------------------------------------------------------------------------*
rem *
rem
rem 
rem
rem
rem
rem *==
rem *----------------------------------------------------------------------------*
rem * =DESCRIPTION
rem *    @DESCRIPTION@
rem *==
rem *----------------------------------------------------------------------------*
rem * =MODIFICATION
rem *  20.08.2001  JD      File created
rem *  see history at end of file !
rem *==
rem *******************************************************************************
rem H<<*


setlocal
if "%PROJECT_ROOT%" == "" set PROJECT_ROOT=\4Control_r
if not exist %PROJECT_ROOT%\nul goto bad_dir

set BUILD_ROOT=%PROJECT_ROOT%\COM\Softing\fc

if "%1" == "SIGN" goto production
if "%1" == "sign" goto production
if "%1" == "CHECK" goto check
if "%1" == "check" goto check
echo No parameter supplied, defaulting to SIGN !
pause
goto production

rem ******************************************************************
:production
echo *---- Do Official Signing of Controls and CABs:
echo.
echo ....Signing Navigator
cd %BUILD_ROOT%\CP\ActiveX\Navigator
call build_cab.bat SIGN

echo.
echo.
echo ....Signing BaseLib
cd %BUILD_ROOT%\CP
call Baselib_cab.bat SIGN

echo.
echo.
echo ....Signing CSC_Online
cd %BUILD_ROOT%\CP
call csc_online_cab.bat SIGN

echo. 
echo.
echo ....Signing 4CSetupV2
cd %BUILD_ROOT%\CSET
call signINST.bat SIGN

rem Falling thru to check trust !

:check
echo *---- Checking Trust of Controls and CABs:
echo ....Checking Navigator
cd %BUILD_ROOT%\CP\ActiveX\Navigator
call build_cab.bat CHECK

echo ....Checking BaseLib
cd %BUILD_ROOT%\CP
call Baselib_cab.bat CHECK

echo ....Checking CSC_Online
cd %BUILD_ROOT%\CP
call csc_online_cab.bat CHECK

echo ....Checking 4CSetupV2
cd %BUILD_ROOT%\CSET
call signINST.bat CHECK
goto end


:bad_dir
    echo Bad project root directoy: %PROJECT_ROOT%. Check your 4C project environment settings!
    goto end

:end
endlocal



rem *
rem *----------------------------------------------------------------------------*
rem *  $History: sign.bat $
rem * 
rem * *****************  Version 1  *****************
rem * User: Ln           Date: 28.02.07   Time: 18:51
rem * Created in $/4CReleased/V2.20.00
rem * 
rem * *****************  Version 1  *****************
rem * User: Ln           Date: 28.08.03   Time: 16:27
rem * Created in $/4Control
rem * 
rem * *****************  Version 3  *****************
rem * User: Jd           Date: 13.12.01   Time: 14:16
rem * Updated in $/4Control
rem * 
rem * *****************  Version 2  *****************
rem * User: Ef           Date: 20.08.01   Time: 15:19
rem * Updated in $/4Control
rem * 
rem * *****************  Version 1  *****************
rem * User: Jd           Date: 20.08.01   Time: 14:51
rem * Created in $/4Control
rem *==
rem *----------------------------------------------------------------------------*
rem *

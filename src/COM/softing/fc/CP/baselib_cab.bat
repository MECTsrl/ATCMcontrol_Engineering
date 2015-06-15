echo off
rem *H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/baselib_cab.bat 1     28.02.07 19:00 Ln $
rem *----------------------------------------------------------------------------*
rem *
rem * =FILENAME            $Workfile: baselib_cab.bat $
rem *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/baselib_cab.bat $
rem *
rem * =PROJECT             7VH102  ATCMControl V1
rem *
rem * =SWKE                4CSC
rem *
rem * =COMPONENT           CSC_Online, OPCProxy, CAB
rem *
rem * =CURRENT      $Date: 28.02.07 19:00 $
rem *           $Revision: 1 $
rem *
rem * =ENVIRONMENT         Win32:
rem *                      MS Windows NT 4.0
rem *                      MSVC++ V5.0 / MFC 4.2 / ATL 2.0 / DAO 3.5
rem *                      MS Java 2.0
rem *                      MS Visual SourceSafe 5.0
rem *
rem * =REFERENCES
rem *==
rem *----------------------------------------------------------------------------*
rem *
rem
rem 
rem
rem 
rem
rem *==
rem *
rem *----------------------------------------------------------------------------*
rem * =DESCRIPTION
rem * 
rem * batch to build and sign CSC_Online.cab
rem *==
rem *----------------------------------------------------------------------------*
rem * =MODIFICATION
rem *  1998-01-27  Je      File created
rem *  see history at end of file !
rem *==
rem ******************************************************************************
rem *H<<

setlocal
if "%PROJECT_ROOT%" == "" goto bad_dir
if not exist %PROJECT_ROOT%\nul goto bad_dir
if "%EXE_DIR%" == "" set EXE_DIR=%PROJECT_ROOT%
if "%PANEL_DIR%" == "" set PANEL_DIR=%EXE_DIR%\console
if not exist %PANEL_DIR%\nul md %PANEL_DIR%


if "%1" == "SIGN" goto production
if "%1" == "sign" goto production
if "%1" == "CHECK" goto check
if "%1" == "check" goto check
goto unsigned


rem ******************************************************************
:production
echo *---- Do Official Signing of Controls and CAB:
echo ... Signing OCX...
signcode -n "ATCM V2 BaseLib Control" -i http://www.4control.de -spc \\sfiler\IAD_4CONTROL\ProductGen\SetupGen\Zertifikate\VeriSign\CredVeriSign.spc -v \\sfiler\IAD_4CONTROL\ProductGen\SetupGen\Zertifikate\VeriSign\SoftingKey.pvk %EXE_DIR%\pub\fc_baseLib.dll
echo ... Timestamping OCX...
signcode -t http://timestamp.verisign.com/scripts/timstamp.dll -x %EXE_DIR%\pub\fc_baseLib.dll

echo ... Creating CAB...
echo BaseLib_cab.inf > cab.toc
echo %EXE_DIR%\pub\fc_baseLib.dll >> cab.toc
cabarc -s 6144 N %PANEL_DIR%\BaseLib.cab @cab.toc
del cab.toc

echo ... Signing CAB...
signcode -n "ATCM V2: BaseLib" -i http://www.4control.de -spc \\sfiler\IAD_4CONTROL\ProductGen\SetupGen\Zertifikate\VeriSign\CredVeriSign.spc -v \\sfiler\IAD_4CONTROL\ProductGen\SetupGen\Zertifikate\VeriSign\SoftingKey.pvk %PANEL_DIR%\BaseLib.cab
echo ... Timestamping CAB...
signcode -t http://timestamp.verisign.com/scripts/timstamp.dll -x %PANEL_DIR%\BaseLib.cab

echo ... Checking Trust of Control and CAB...
echo Checking Trust on BaseLib ActiveX Control......
chktrust -q %EXE_DIR%\pub\fc_baseLib.dll
echo Checking Trust on BaseLib CAB File......
chktrust -q %PANEL_DIR%\BaseLib.cab

goto end

rem ******************************************************************
:check
echo Checking Trust on BaseLib ActiveX Control......
chktrust %EXE_DIR%\pub\fc_baseLib.dll
echo Checking Trust on BaseLib CAB File......
chktrust %PANEL_DIR%\BaseLib.cab
goto end

rem ******************************************************************
:unsigned
echo *---- Creating certificate (test only!) ...
makecert -n "CN=ATCM ATCMControl (test)" -sv je.pvk BaseLib.cer
echo ... Converting certificate...
cert2spc BaseLib.cer CMN.spc

echo ... Creating CAB...
echo BaseLib_cab.inf > cab.toc
echo %EXE_DIR%\pub\fc_baseLib.dll >> cab.toc
cabarc -s 6144 N %PANEL_DIR%\BaseLib.cab @cab.toc
del cab.toc

echo ... Signing OCX...
signcode -n "ATCM V2 BaseLib Control" -i http://www.4control.de -spc CMN.spc -v je.pvk %EXE_DIR%\pub\fc_baseLib.dll

echo ... Signing CAB...
signcode -n "ATCM V2 BaseLib Control" -i http://www.4control.de -spc CMN.spc -v je.pvk %PANEL_DIR%\BaseLib.cab
del BaseLib.cer
del CMN.spc
goto end

:bad_dir
    echo Bad project root directoy: %PROJECT_ROOT%. Check your ATCM project environment settings!
    goto end

:end
endlocal

rem *----------------------------------------------------------------------------*
rem * =MODIFICATIONS
rem *  $History: baselib_cab.bat $
rem * 
rem * *****************  Version 1  *****************
rem * User: Ln           Date: 28.02.07   Time: 19:00
rem * Created in $/4CReleased/V2.20.00/COM/softing/fc/CP
rem * 
rem * *****************  Version 2  *****************
rem * User: GenATCM        Date: 7.04.04    Time: 10:17
rem * Updated in $/4Control/COM/softing/fc/CP
rem * adapted to new directory hierarchy
rem * 
rem * *****************  Version 1  *****************
rem * User: Ln           Date: 28.08.03   Time: 16:33
rem * Created in $/4Control/COM/softing/fc/CP
rem * 
rem * *****************  Version 2  *****************
rem * User: Ef           Date: 20.08.01   Time: 15:39
rem * Updated in $/4Control/COM/softing/fc/CP
rem * 
rem * *****************  Version 1  *****************
rem * User: Ef           Date: 8.08.01    Time: 21:04
rem * Created in $/4Control/COM/softing/fc/CP
rem * 
rem * *****************  Version 5  *****************
rem * User: Ed           Date: 23.09.99   Time: 13:46
rem * Updated in $/4Control/COM/softing/fc/CMN
rem * 
rem * *****************  Version 4  *****************
rem * User: Je           Date: 19.08.98   Time: 9:32
rem * Updated in $/4Control/COM/softing/fc/CMN
rem * * include timestamping, using correct URL!
rem * 
rem * *****************  Version 3  *****************
rem * User: Je           Date: 31.07.98   Time: 19:34
rem * Updated in $/4Control/COM/softing/fc/CMN
rem * * added production code signing with DigitalID from VeriSign
rem * 
rem * *****************  Version 2  *****************
rem * User: Je           Date: 11.07.98   Time: 18:16
rem * Updated in $/4Control/COM/softing/fc/CMN
rem * * buidl CAB for BaseLib
rem * 
rem * *****************  Version 1  *****************
rem * User: Je           Date: 8.07.98    Time: 12:04
rem * Created in $/4Control/COM/softing/fc/CMN
rem * * create CAB for baselib
rem *==
rem *----------------------------------------------------------------------------*
rem * 1997-11-25/Je  Generated by PERL5 script d:\bin\perl5/mkproto.bat Version 1.2
rem * ***************************  E O F  ******************************
rem * $Workfile: baselib_cab.bat $

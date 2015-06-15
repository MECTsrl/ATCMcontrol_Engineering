echo off
rem *H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/csc_online_cab.bat 1     28.02.07 19:00 Ln $
rem *----------------------------------------------------------------------------*
rem *
rem * =FILENAME            $Workfile: csc_online_cab.bat $
rem *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/csc_online_cab.bat $
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

echo ... Signing EXE...
signcode -n "ATCM Simple Online Control" -i http://www.4control.de -spc \\sfiler\IAD_4CONTROL\ProductGen\SetupGen\Zertifikate\VeriSign\CredVeriSign.spc -v \\sfiler\IAD_4CONTROL\ProductGen\SetupGen\Zertifikate\VeriSign\SoftingKey.pvk %EXE_DIR%\pub\AsyncOPC.exe
echo ... Timestamping EXE...
signcode -t http://timestamp.verisign.com/scripts/timstamp.dll -x %EXE_DIR%\pub\AsyncOPC.exe

echo ... Creating CAB...
echo CSC_Online_cab.inf > cab.toc
echo %EXE_DIR%\pub\AsyncOPC.exe >> cab.toc
echo %EXE_DIR%\pub\opcProxy.dll >> cab.toc
cabarc -s 6144 N %PANEL_DIR%\CSC_Online.cab @cab.toc
del cab.toc

echo ... Signing CAB...
signcode -n "ATCMControl Online Control" -i http://www.4control.de -spc \\sfiler\IAD_4CONTROL\ProductGen\SetupGen\Zertifikate\VeriSign\CredVeriSign.spc -v \\sfiler\IAD_4CONTROL\ProductGen\SetupGen\Zertifikate\VeriSign\SoftingKey.pvk %PANEL_DIR%\CSC_Online.cab
echo ... Timestamping CAB...
signcode -t http://timestamp.verisign.com/scripts/timstamp.dll -x %PANEL_DIR%\CSC_Online.cab

echo ... Checking Trust of Control and CAB...
echo Checking Trust on Simple CSC_Online ActiveX Control......
chktrust -q %EXE_DIR%\pub\AsyncOPC.exe
echo Checking Trust on CSC_Online CAB File......
chktrust -q %PANEL_DIR%\CSC_Online.cab

goto end

rem ******************************************************************
:check
echo Checking Trust on Simple CSC_Online ActiveX Control......
chktrust %EXE_DIR%\pub\AsyncOPC.exe
echo Checking Trust on CSC_Online CAB File......
chktrust %PANEL_DIR%\CSC_Online.cab
goto end

rem ******************************************************************
:unsigned
echo *---- Creating certificate (test only!) ...
makecert -n "CN=ATCM (test)" -sv je.pvk CSC_Online.cer
echo ... Converting certificate...
cert2spc CSC_Online.cer CSC.spc

echo ... Signing OCX...
signcode -n "ATCM Simple Online Control" -i http://www.4control.de -spc CSC.spc -v je.pvk %EXE_DIR%\pub\AsyncOPC.exe

echo ... Creating CAB...
echo CSC_Online_cab.inf > cab.toc
echo %EXE_DIR%\pub\AsyncOPC.exe >> cab.toc
echo %EXE_DIR%\pub\opcProxy.dll >> cab.toc
cabarc -s 6144 N %PANEL_DIR%\CSC_Online.cab @cab.toc
del cab.toc

echo ... Signing CAB...
signcode -n "ATCMControl Online Control" -i http://www.4control.de -spc CSC.spc -v je.pvk %PANEL_DIR%\CSC_Online.cab
del CSC_Online.cer
del CSC.spc
goto end

:bad_dir
    echo Bad project root directoy: %PROJECT_ROOT%. Check your ATCM project environment settings!
    goto end

:end
endlocal

rem *----------------------------------------------------------------------------*
rem * =MODIFICATIONS
rem *  $History: csc_online_cab.bat $
rem * 
rem * *****************  Version 1  *****************
rem * User: Ln           Date: 28.02.07   Time: 19:00
rem * Created in $/4CReleased/V2.20.00/COM/softing/fc/CP
rem * 
rem * *****************  Version 2  *****************
rem * User: GenATCM        Date: 7.04.04    Time: 10:19
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
rem * *****************  Version 15  *****************
rem * User: Ed           Date: 16.02.00   Time: 14:52
rem * Updated in $/4Control/COM/softing/fc/CSC
rem * AsyncOPC created
rem * 
rem * *****************  Version 14  *****************
rem * User: Ed           Date: 26.10.99   Time: 9:35
rem * Updated in $/4Control/COM/softing/fc/CSC
rem * remove CSC_Online.ocx from CAB download, finally.
rem * 
rem * *****************  Version 13  *****************
rem * User: Ed           Date: 3.10.99    Time: 18:21
rem * Updated in $/4Control/COM/softing/fc/CSC
rem * undo...
rem * 
rem * *****************  Version 12  *****************
rem * User: Ed           Date: 23.09.99   Time: 13:54
rem * Updated in $/4Control/COM/softing/fc/CSC
rem * 
rem * *****************  Version 11  *****************
rem * User: So           Date: 27.08.99   Time: 16:19
rem * Updated in $/4Control/COM/softing/fc/CSC
rem * target directory for CSC_simpleOPC changed to "pub"
rem * 
rem * *****************  Version 10  *****************
rem * User: So           Date: 25.06.99   Time: 10:29
rem * Updated in $/4Control/COM/softing/fc/CSC
rem * 
rem * *****************  Version 9  *****************
rem * User: So           Date: 24.06.99   Time: 14:52
rem * Updated in $/4Control/COM/softing/fc/CSC
rem * added: csc_simpleOPC.dll to CAB
rem * 
rem * *****************  Version 8  *****************
rem * User: Vo           Date: 9.06.99    Time: 8:07
rem * Updated in $/4Control/COM/softing/fc/CSC
rem * CSC_Online.ocx is in pub directory
rem * 
rem * *****************  Version 7  *****************
rem * User: Je           Date: 19.08.98   Time: 11:10
rem * Updated in $/4Control/COM/softing/fc/CSC
rem * * include timestamping, using correct URL!
rem * 
rem * *****************  Version 6  *****************
rem * User: Je           Date: 19.08.98   Time: 9:32
rem * Updated in $/4Control/COM/softing/fc/CSC
rem * * include timestamping, using correct URL!
rem * 
rem * *****************  Version 5  *****************
rem * User: Je           Date: 31.07.98   Time: 17:18
rem * Updated in $/4Control/COM/softing/fc/CSC
rem * * added production code signing with DigitalID from VeriSign
rem * 
rem * *****************  Version 4  *****************
rem * User: Ed           Date: 25.02.98   Time: 14:27
rem * Updated in $/4Control/COM/softing/fc/CSC
rem * panel heisst jetzt console
rem *
rem ******************  Version 2  *****************
rem *User: Je           Date: 27.01.98   Time: 13:56
rem *Updated in $/4Control/COM/softing/fc/CSC
rem *
rem ******************  Version 1  *****************
rem *User: Je           Date: 27.01.98   Time: 11:08
rem *Created in $/4Control/COM/softing/fc/CSC
rem *==
rem *----------------------------------------------------------------------------*
rem * 1997-11-25/Je  Generated by PERL5 script d:\bin\perl5/mkproto.bat Version 1.2
rem * ***************************  E O F  ******************************
rem * $Workfile: csc_online_cab.bat $

@echo off

REM === initializing variables
setlocal

if "%PROJECT_ROOT%" == "" goto bad_dir


if "%PANEL_DIR%" == "" set PANEL_DIR=%PROJECT_ROOT%\console
if not exist %PANEL_DIR%\nul md %PANEL_DIR%

set PUB_DIR=%PROJECT_ROOT%\pub
if not exist %PROJECT_ROOT%\nul md %PUB_DIR%


set NAVIGATOR_CAB=%PANEL_DIR%\CPNavigator.cab
set NAVIGATOR_OCX=%PUB_DIR%\CPNavigator.ocx
set NAVIGATOR_INF=CPNavigator_cab.inf

REM === branch on arguments

if "%1" == "SIGN" goto production
if "%1" == "sign" goto production

if "%1" == "CHECK" goto check
if "%1" == "check" goto check

goto unsigned 

REM ================================================================================
:production
REM ================================================================================


REM=== signing navigator ocx
set NAME=ATCM Navigator Control
set INFO=http://www.4control.de
set SPC=\\sfiler\IAD_4CONTROL\ProductGen\SetupGen\Zertifikate\VeriSign\CredVeriSign.spc
set PVK=\\sfiler\IAD_4CONTROL\ProductGen\SetupGen\Zertifikate\VeriSign\SoftingKey.pvk
signcode -n "%NAME%" -i "%INFO%" -spc "%SPC%" -v "%PVK%" "%NAVIGATOR_OCX%"
echo ... Timestamping OCX...
signcode -t http://timestamp.verisign.com/scripts/timstamp.dll -x "%NAVIGATOR_OCX%"

REM === creating navigator cab file
cabarc -s 6144 N "%NAVIGATOR_CAB%" "%NAVIGATOR_OCX%" "%NAVIGATOR_INF%"

REM === official signing of navigator cab file
set NAME=4CONTROLV2 CONSOLE(NAVIGATOR)
set INFO=http://www.4control.de
set SPC=\\sfiler\IAD_4CONTROL\ProductGen\SetupGen\Zertifikate\VeriSign\CredVeriSign.spc
set PVK=\\sfiler\IAD_4CONTROL\ProductGen\SetupGen\Zertifikate\VeriSign\SoftingKey.pvk
signcode -n "%NAME%" -i "%INFO%" -spc "%SPC%" -v "%PVK%" "%NAVIGATOR_CAB%"
echo ... Timestamping CAB...
signcode -t http://timestamp.verisign.com/scripts/timstamp.dll -x "%NAVIGATOR_CAB%"

goto end

REM ================================================================================
:check
REM ================================================================================

REM === checking trust on navigator ocx
chktrust "%NAVIGATOR_OCX%"

REM === checking trust on navigator cab file
chktrust "%NAVIGATOR_CAB%"

goto end

REM ================================================================================
:unsigned
REM ================================================================================

REM === creating certificate (test only!)
set NAME=CN=ATCM (mkr)
set PVK=mkr.pvk
echo on
makecert -n "%NAME%" -sv "%PVK%" CPNav.cer
@echo off

REM === converting certificate
echo on
cert2spc CPNav.cer CPNav.spc
@echo off

REM === signing navigator ocx
set NAME=ATCM Navigator Control
set INFO=http://www.4control.de
set SPC=CPNav.spc
set PVK=mkr.pvk
echo on
rem TODO signcode -n "%NAME%" -i "%INFO%" -spc "%SPC%" -v "%PVK%" "%NAVIGATOR_OCX%"
@echo off

REM === creating navigator cab
echo on
cabarc -s 6144 N "%NAVIGATOR_CAB%" "%NAVIGATOR_OCX%" "%NAVIGATOR_INF%"
@echo off

REM === signing navigator cab
set NAME=ATCMControl CONSOLE(NAVIGATOR) V1
set INFO=http://www.4control.de
set SPC=CPNav.spc
set PVK=mkr.pvk
echo on
rem TODO signcode -n "%NAME%" -i "%INFO%" -spc "%SPC%" -v "%PVK%" "%NAVIGATOR_CAB%"
@echo off

REM === remove temporary files
del CPNav.cer
del CPNav.spc

goto end

REM ================================================================================
:bad_dir
REM ================================================================================
    echo Bad project root directory: %PROJECT_ROOT%. Check your ATCM project environment settings!
    goto end

REM ================================================================================
:end
REM ================================================================================

endlocal


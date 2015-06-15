

REM === initializing variables
setlocal

if "%PROJECT_ROOT%" == "" set PROJECT_ROOT=\4Control
if not exist %PROJECT_ROOT%\nul goto bad_dir

if "%EXE_DIR%" == "" set EXE_DIR=%PROJECT_ROOT%\

if "%PANEL_DIR%" == "" set PANEL_DIR=%EXE_DIR%\console
if not exist %PANEL_DIR%\nul md %PANEL_DIR%

set PUB_DIR=%EXE_DIR%\pub
if not exist %PUB_DIR%\nul md %PUB_DIR%

set CPCLASSES_CAB=%PANEL_DIR%\CPClasses.cab

REM === branch on arguments

if "%1" == "SIGN" goto production
if "%1" == "sign" goto production

if "%1" == "CHECK" goto check
if "%1" == "check" goto check

goto unsigned 

REM ================================================================================
:production
REM ================================================================================

REM === official signing of cpclasses cab file
set NAME=ATCMControl CONSOLE(CLASSES) V2
set INFO=http://www.4control.de
set SPC=\\sfiler\IAD_4CONTROL\ProductGen\SetupGen\Zertifikate\VeriSign\CredVeriSign.spc
set PVK=\\sfiler\IAD_4CONTROL\ProductGen\SetupGen\Zertifikate\VeriSign\SoftingKey.pvk
echo ... Signing CAB...
signcode -n "%NAME%" -i "%INFO%" -spc "%SPC%" -v "%PVK%" "%CPCLASSES_CAB%"
REM echo ... Timestamping CAB...
signcode -t http://timestamp.verisign.com/scripts/timstamp.dll -x "%CPCLASSES_CAB%"

goto end

REM ================================================================================
:check
REM ================================================================================

REM === checking trust on navigator cab file
chktrust "%CPCLASSES_CAB%"
chkjava "%CPCLASSES_CAB%"

goto end

REM ================================================================================
:unsigned
REM ================================================================================

REM === creating certificate (test only!)
set NAME=CN=ATCM (so)
set PVK=so.pvk
echo on
makecert -n "%NAME%" -sv "%PVK%" CPClasses.cer
@echo off

REM === converting certificate
echo on
cert2spc CPClasses.cer CPClasses.spc
@echo off

REM === signing navigator cab
set NAME=ATCMControl CONSOLE(CPCLASSES) V1
set INFO=http://www.4control.de
set SPC=CPClasses.spc
set PVK=so.pvk
echo on
signcode -j javasign.dll -jp javasec.ini -n "%NAME%" -i "%INFO%" -spc "%SPC%" -v "%PVK%" "%CPCLASSES_CAB%"
@echo off

REM === remove temporary files
del CPClasses.cer
del CPClasses.spc

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


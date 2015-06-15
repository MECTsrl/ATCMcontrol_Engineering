@ECHO OFF

setlocal


@ECHO.
@ECHO FullGen.bat
@ECHO -------------------------------------------------------------------------

set LFG_BRANCH=%1
set LFG_BUILD=%2

if "%LFG_BRANCH%" == "" goto error
if "%LFG_BUILD%" == "" goto error

set LFG_BRANCH_DIR=

if "%LFG_BRANCH%" == "DEV" goto skipsetbranch

set LFG_BRANCH_DIR=_%LFG_BRANCH%

:skipsetbranch

@ECHO LFG_BRANCH = %LFG_BRANCH%
@ECHO LFG_BUILD  = %LFG_BUILD%
@ECHO.

echo call GenKernel %LFG_BRANCH% %LFG_BUILD%
     call GenKernel %LFG_BRANCH% %LFG_BUILD%
if errorlevel 1 goto error

echo call GenInitEnv.bat
     call GenInitEnv.bat
if errorlevel 1 goto error


@ECHO.
@ECHO Create Error File
@ECHO -------------------------------------------------------------------------

if "%LFG_BUILD%" == "TEST" goto set_test_build_kernel

echo call %SG_TARGET%\SysgenV2\BuildNr%LFG_BRANCH_DIR%.bat
     call %SG_TARGET%\SysgenV2\BuildNr%LFG_BRANCH_DIR%.bat
if errorlevel 1 goto error

set LFG_KERN_BLD=%BUILDNUMBER%

goto set_test_build_kernel_continue

: set_test_build_kernel

set LFG_KERN_BLD=TEST

: set_test_build_kernel_continue

echo call %SG_GENDIR%\4Control_R\COM\softing\fc\res\Version.bat
     call %SG_GENDIR%\4Control_R\COM\softing\fc\res\Version.bat
if errorlevel 1 goto error

@ECHO FC_VERSION   = %FC_VERSION%
@ECHO LFG_KERN_BLD = %LFG_KERN_BLD%

set LFG_ERRFILE=%SG_TARGET%\SysGenV2\%FC_VERSION%.%LFG_KERN_BLD%\FullGen.err


@ECHO.
@ECHO Build Error File - Kernel
@ECHO -------------------------------------------------------------------------
ECHO LFG_ERRFILE = %LFG_ERRFILE%

echo.                                                                                        > %LFG_ERRFILE%
echo ** Kernel - Release                                                                    >> %LFG_ERRFILE%
echo *******************                                                                    >> %LFG_ERRFILE%
type %SG_TARGET%\SysGenV2\%FC_VERSION%.%LFG_KERN_BLD%\4Control_R\COM\build.err              >> %LFG_ERRFILE%
echo.                                                                                       >> %LFG_ERRFILE%
echo.                                                                                       >> %LFG_ERRFILE%
echo ** Kernel - Debug                                                                      >> %LFG_ERRFILE%
echo *****************                                                                      >> %LFG_ERRFILE%
type %SG_TARGET%\SysGenV2\%FC_VERSION%.%LFG_KERN_BLD%\4Control\COM\build.err                >> %LFG_ERRFILE%
echo.                                                                                       >> %LFG_ERRFILE%
echo.                                                                                       >> %LFG_ERRFILE%
echo ** TargetBase - Release                                                                >> %LFG_ERRFILE%
echo ***********************                                                                >> %LFG_ERRFILE%
type %SG_TARGET%\SysGenV2\%FC_VERSION%.%LFG_KERN_BLD%\4Control_R\TargetBase\build.err       >> %LFG_ERRFILE%
echo.                                                                                       >> %LFG_ERRFILE%
echo.                                                                                       >> %LFG_ERRFILE%
echo ** TargetBase - Debug                                                                  >> %LFG_ERRFILE%
echo *********************                                                                  >> %LFG_ERRFILE%
type %SG_TARGET%\SysGenV2\%FC_VERSION%.%LFG_KERN_BLD%\4Control\TargetBase\build.err         >> %LFG_ERRFILE%
echo.                                                                                       >> %LFG_ERRFILE%


@ECHO.
@ECHO Build 4CWin
@ECHO -------------------------------------------------------------------------

echo call GenTarget 4CWin %FC_VERSION%.%LFG_KERN_BLD% %LFG_BRANCH% %LFG_BUILD%
     call GenTarget 4CWin %FC_VERSION%.%LFG_KERN_BLD% %LFG_BRANCH% %LFG_BUILD%
if errorlevel 1 goto error

if "%LFG_BUILD%" == "TEST" goto set_test_build_win

echo call %SG_TARGET%\SysgenV2\4CWin\BuildNr%LFG_BRANCH_DIR%.bat
     call %SG_TARGET%\SysgenV2\4CWin\BuildNr%LFG_BRANCH_DIR%.bat
if errorlevel 1 goto error

set LFG_WIN_BLD=%BUILDNUMBER%

goto set_test_build_kernel_continue

: set_test_build_win

set  LFG_WIN_BLD=TEST

: set_test_build_kernel_continue

echo LFG_WIN_BLD = %LFG_WIN_BLD%

echo xcopy %SG_TARGET%\SetupV2\4CWin\B%LFG_WIN_BLD%.%FC_VERSION%.%LFG_KERN_BLD%\*.* %SG_TARGET%\SetupV2\%FC_VERSION%.%LFG_KERN_BLD%\*.* /S /Y /K /R
     xcopy %SG_TARGET%\SetupV2\4CWin\B%LFG_WIN_BLD%.%FC_VERSION%.%LFG_KERN_BLD%\*.* %SG_TARGET%\SetupV2\%FC_VERSION%.%LFG_KERN_BLD%\*.* /S /Y /K /R > nul 2>&1
if errorlevel 1 goto error


@ECHO.
@ECHO Build Error File - 4CWin
@ECHO -------------------------------------------------------------------------
ECHO LFG_ERRFILE = %LFG_ERRFILE%

echo.                                                                                       >> %LFG_ERRFILE%
echo.                                                                                       >> %LFG_ERRFILE%
echo ** 4CWin - Release                                                                     >> %LFG_ERRFILE%
echo ******************                                                                     >> %LFG_ERRFILE%
type %SG_TARGET%\SysGenV2\4CWin\B%LFG_WIN_BLD%.%FC_VERSION%.%LFG_KERN_BLD%\4Control_R\Target\4CWin\build.err     >> %LFG_ERRFILE%
echo.                                                                                       >> %LFG_ERRFILE%
echo.                                                                                       >> %LFG_ERRFILE%
echo ** 4CWin - Debug                                                                       >> %LFG_ERRFILE%
echo ****************                                                                       >> %LFG_ERRFILE%
type %SG_TARGET%\SysGenV2\4CWin\B%LFG_WIN_BLD%.%FC_VERSION%.%LFG_KERN_BLD%\4Control\Target\4CWin\build.err       >> %LFG_ERRFILE%
echo.                                                                                       >> %LFG_ERRFILE%


echo.
echo.
echo -------  D o n e !  -------

goto end

:error

echo.
echo.
echo -------  E R R O R  -------
echo.
echo FullGen [Branch] [Build]
echo [Branch]: DEV  V2.06  V2.10.01  ...
echo [Build]:  NEW  TEST  3139  3238  ...
echo.

:end

endlocal

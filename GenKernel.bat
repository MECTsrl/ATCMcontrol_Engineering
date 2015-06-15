@ECHO OFF

setlocal

set CWD=%~d0%~p0 
cd /D %CWD%

rem FOR /F "delims=" %i IN ('date /t') DO set INIT_DATE=%i
rem FOR /F "delims=" %i IN ('time /t') DO set INIT_TIME=%i

set SDK_ROOT=C:\ATCMControl_SDK
set PROJECT_ROOT=%SDK_ROOT%\GenWork\GenDir.tmp\4Control_R
set ISDIR=%SDK_ROOT%\GenWork\GenDir.tmp
set SG_SETUP=%SDK_ROOT%\GenWork\SetupDir
set SG_TARGET=%SDK_ROOT%\Installer
set SG_LATEST=%SDK_ROOT%\Installer\RELEASE
set LIBS_DIR=%SDK_ROOT%\LIBS
set SKELETON_DIR=%SDK_ROOT%\Libs_Skeleton
set LOG_ROOT=%SDK_ROOT%\Logs
set LSG_LOGFILE=%LOG_ROOT%\GenKernel.log


@ECHO.
@ECHO GenKernel.bat
@ECHO -------------------------------------------------------------------------

if "%CD%"=="%SDK_ROOT%" goto skiprepocopy

echo.
echo Copying sources to %SDK_ROOT%
echo.

if exist %SDK_ROOT% rd /S /Q %SDK_ROOT%

md %PROJECT_ROOT%
md %SG_SETUP%
md %SG_TARGET%
md %LIBS_DIR%
md %SKELETON_DIR%
md %LOG_ROOT%

echo GenKernel.bat started at > %LSG_LOGFILE%
rem echo %INIT_DATE% >> %LSG_LOGFILE%
rem echo %INIT_TIME% >> %LSG_LOGFILE%
date /t >> %LSG_LOGFILE%
time /t >> %LSG_LOGFILE%

xcopy src\* %PROJECT_ROOT% /S /Y /Q /K /R /E >nul 2>>%LSG_LOGFILE%
xcopy Installer\* %SG_TARGET% /S /Y /Q /K /R /E >nul 2>>%LSG_LOGFILE%
xcopy LIBS\* %LIBS_DIR% /S /Y /Q /K /R /E >nul 2>>%LSG_LOGFILE%
xcopy Libs_Skeleton\* %SKELETON_DIR% /S /Y /Q /K /R /E >nul 2>>%LSG_LOGFILE%
xcopy *.bat %SDK_ROOT% /Y /Q /K >nul 2>>%LSG_LOGFILE%

cd /D %SDK_ROOT% 

:skiprepocopy

@ECHO.
@ECHO Set-VCVARS.bat
@ECHO -------------------------------------------------------------------------

@call "%programfiles%\Microsoft Visual Studio\VC98\bin\VCVARS32" >nul 2>>%LSG_LOGFILE%

CALL GenInitEnv.bat
CALL checkinstallation

@ECHO.
@ECHO Generate Release Version
@ECHO -------------------------------------------------------------------------
@ECHO.                                                                                          >> %LSG_LOGFILE%
@ECHO Generate Release Version                                                                  >> %LSG_LOGFILE%
@ECHO -------------------------------------------------------------------------                 >> %LSG_LOGFILE%

set OFFICIAL_BUILD=1
set PROJECT_PARENT=%CD%

@ECHO PROJECT_ROOT = %PROJECT_ROOT%
@ECHO PROJECT_ROOT = %PROJECT_ROOT%                                                             >> %LSG_LOGFILE%

cd /D %PROJECT_ROOT%\COM

set NO_MAKE_BROWSE=1		
set NO_MAKE_DEPEND=1
set NO_MAKE_QUICK=1

echo perl ..\GenTools\make.pl Kernel_Prima                                                      >> %LSG_LOGFILE%
     perl ..\GenTools\make.pl Kernel_Prima                                              >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo nmake -nologo -i -f Kernel_Prima.mak makemake "DEBUG=0" all 2>&1 | perl ..\GenTools\LogFile.pl Build.log
echo nmake -nologo -i -f Kernel_Prima.mak makemake "DEBUG=0" all 2>&1 | perl ..\GenTools\LogFile.pl Build.log          >> %LSG_LOGFILE%
     nmake -nologo -i -f Kernel_Prima.mak makemake "DEBUG=0" all 2>&1 | perl ..\GenTools\LogFile.pl Build.log  >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo find /I "warning " build.log | perl ..\GenTools\LogFile.pl Build.wrn
echo find /I "warning " build.log | perl ..\GenTools\LogFile.pl Build.wrn                       >> %LSG_LOGFILE%
     find /I "warning " build.log | perl ..\GenTools\LogFile.pl Build.wrn               >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo find /I "error " build.log | perl ..\GenTools\LogFile.pl Build.err
echo find /I "error " build.log | perl ..\GenTools\LogFile.pl Build.err                         >> %LSG_LOGFILE%
     find /I "error " build.log | perl ..\GenTools\LogFile.pl Build.err                 >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

cd /d "%PROJECT_PARENT%"
@call GenTargetBase
if errorlevel 1 goto error

cd /d "%PROJECT_PARENT%"
@call CreateSetup
if errorlevel 1 goto error

cd /d "%PROJECT_PARENT%"
@call RegistryDlls
if errorlevel 1 goto error

echo GenKernel.bat finished at                                                                  >> %LSG_LOGFILE%
date /t                                                                                         >> %LSG_LOGFILE%
time /t                                                                                         >> %LSG_LOGFILE%

goto success

:error
echo.
echo ERROR on GenKernel. See log file for more informations.
echo -------------------------------------------------------------------------
echo.
goto end

:success
echo.
echo SUCCESS on GenKernel. All files were generated.
echo -------------------------------------------------------------------------
echo.                                                                                         >> %LSG_LOGFILE%
echo SUCCESS on GenKernel. All files were generated.										  >> %LSG_LOGFILE%
echo -------------------------------------------------------------------------                >> %LSG_LOGFILE%
goto end

:end
endlocal
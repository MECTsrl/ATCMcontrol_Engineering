@ECHO OFF

setlocal

set CWD=%~d0%~p0 
cd /D %CWD%

if "%1"=="" goto notargeterror

if /I "%CD%" NEQ "C:\ATCMControl_SDK" goto directoryerror

REM Setting Target and Version-------------------------------------------------
SET LSG_TARGET=%1
SET VERSION=RELEASE

@ECHO.
@ECHO GenTarget_%LSG_TARGET%.bat
@ECHO -------------------------------------------------------------------------

@ECHO.
@ECHO Set-VCVARS.bat
@ECHO -------------------------------------------------------------------------

@call "%programfiles%\Microsoft Visual Studio\VC98\bin\VCVARS32" >nul 2>&1

@CALL GenInitEnv.bat
@CALL CheckInstallation

set SDK_ROOT=%CD%
set LOG_ROOT=%SDK_ROOT%\Logs
set LSG_LOGFILE=%LOG_ROOT%\GenTarget_%LSG_TARGET%.log
@ECHO LSG_LOGFILE    = %LSG_LOGFILE%

echo GenTarget_%LSG_TARGET%.bat started at                                                       > %LSG_LOGFILE%
date /t                                                                                         >> %LSG_LOGFILE%
time /t                                                                                         >> %LSG_LOGFILE%

@ECHO.
@ECHO Setting Variables
@ECHO -------------------------------------------------------------------------
@ECHO.                                                                                          >> %LSG_LOGFILE%
@ECHO Setting Variables			                                                                >> %LSG_LOGFILE%
@ECHO -------------------------------------------------------------------------                 >> %LSG_LOGFILE%

set OFFICIAL_BUILD=1
set PROJECT_PARENT=%CD%
set PROJECT_ROOT=%PROJECT_PARENT%\GenWork\GenDir.tmp\4Control_R
@ECHO PROJECT_ROOT = %PROJECT_ROOT%
@ECHO PROJECT_ROOT = %PROJECT_ROOT%                                                             >> %LSG_LOGFILE%
set ISDIR=%PROJECT_PARENT%\GenWork\GenDir.tmp
set SG_SETUP=%PROJECT_PARENT%\GenWork\SetupDir
set SG_TARGET=%PROJECT_PARENT%\Installer
set SG_LATEST=%PROJECT_PARENT%\Installer\%VERSION%
set SG_SOFTLM=%PROJECT_PARENT%\License_Manager\V3.02.0059\AllLanguages
set SG_GENDIR=%PROJECT_PARENT%\GenWork\GenDir.tmp
set TARGET_BASE=%PROJECT_ROOT%\TargetBase
set DSTDIR=%PROJECT_ROOT%\Target\%LSG_TARGET%

if exist %PROJECT_ROOT%\Engineering\bin\%LSG_TARGET% goto deltarget
goto dontdeltarget

:deltarget
rd /S /Q %PROJECT_ROOT%\Engineering\bin\%LSG_TARGET%
:dontdeltarget

@ECHO.
@ECHO Connect drives
@ECHO -------------------------------------------------------------------------
@ECHO.                                                                                          >> %LSG_LOGFILE%
@ECHO Connect drives                                                                            >> %LSG_LOGFILE%
@ECHO -------------------------------------------------------------------------                 >> %LSG_LOGFILE%

@REM  I: Build directory    ------------------------------

subst I: /d                                                                             >> nul 2>&1

echo subst I: %SG_GENDIR%
echo subst I: %SG_GENDIR%                                                                       >> %LSG_LOGFILE%
     subst I: %SG_GENDIR%                                                               >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

@REM  J: Build directory    ------------------------------

subst J: /d                                                                             >> nul 2>&1

echo subst J: %SG_GENDIR%
echo subst J: %SG_GENDIR%                                                                       >> %LSG_LOGFILE%
     subst J: %SG_GENDIR%                                                               >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

@REM  K: Setup directory    ------------------------------

subst K: /d                                                                             >> nul 2>&1

echo subst K: %SG_SETUP%
echo subst K: %SG_SETUP%                                                                        >> %LSG_LOGFILE%
     subst K: %SG_SETUP%                                                                >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error


@ECHO.
@ECHO Build Release Version
@ECHO -------------------------------------------------------------------------
@ECHO.                                                                                          >> %LSG_LOGFILE%
@ECHO Build Release Version                                                                     >> %LSG_LOGFILE%
@ECHO -------------------------------------------------------------------------                 >> %LSG_LOGFILE%

J:
set OFFICIAL_BUILD=1
cd %PROJECT_ROOT%

@ECHO Destination = %DSTDIR%
@ECHO Destination = %DSTDIR%                                          							>> %LSG_LOGFILE%

@ECHO.
@ECHO Generate Release Version
@ECHO -------------------------------------------------------------------------
@ECHO.                                                                                          >> %LSG_LOGFILE%
@ECHO Generate Release Version                                                                  >> %LSG_LOGFILE%
@ECHO -------------------------------------------------------------------------                 >> %LSG_LOGFILE%


cd /D %PROJECT_ROOT%\Target\%LSG_TARGET%

set NO_MAKE_BROWSE=1
set NO_MAKE_DEPEND=1
set NO_MAKE_QUICK=1

echo perl ..\..\GenTools\make.pl %LSG_TARGET%
echo perl ..\..\GenTools\make.pl %LSG_TARGET%                                                             >> %LSG_LOGFILE%
     perl ..\..\GenTools\make.pl %LSG_TARGET%                                                     >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo nmake -nologo -i -f %LSG_TARGET%.mak makemake "DEBUG=0" all 2>&1 | perl ..\..\GenTools\LogFile.pl Build.log
echo nmake -nologo -i -f %LSG_TARGET%.mak makemake "DEBUG=0" all 2>&1 | perl ..\..\GenTools\LogFile.pl Build.log          >> %LSG_LOGFILE%
     nmake -nologo -i -f %LSG_TARGET%.mak makemake "DEBUG=0" all 2>&1 | perl ..\..\GenTools\LogFile.pl Build.log  >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo find /I "warning " build.log | perl ..\..\GenTools\LogFile.pl Build.wrn
echo find /I "warning " build.log | perl ..\..\GenTools\LogFile.pl Build.wrn                    >> %LSG_LOGFILE%
     find /I "warning " build.log | perl ..\..\GenTools\LogFile.pl Build.wrn            >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo find /I "error " build.log | perl ..\..\GenTools\LogFile.pl Build.err
echo find /I "error " build.log | perl ..\..\GenTools\LogFile.pl Build.err                      >> %LSG_LOGFILE%
     find /I "error " build.log | perl ..\..\GenTools\LogFile.pl Build.err              >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error



@ECHO.
@ECHO Create Setup
@ECHO -------------------------------------------------------------------------
@ECHO.                                                                                          >> %LSG_LOGFILE%
@ECHO Create Setup                                                                              >> %LSG_LOGFILE%
@ECHO -------------------------------------------------------------------------                 >> %LSG_LOGFILE%


cd /D %PROJECT_ROOT%\Target\%LSG_TARGET%\Setup

attrib -R *.ism

echo call IsCmdBld -i Build.ini
echo call IsCmdBld -i Build.ini                                                                 >> %LSG_LOGFILE%
     call IsCmdBld -i Build.ini                                                         >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

cd ..

echo xcopy %SG_SETUP%\%LSG_TARGET%AddOn\Configuration1\Release1\DiskImages\DISK1\*.*    %SG_LATEST%\Setup\%LSG_TARGET%AddOn\*.* /S /Y /Q /K /R /E
echo xcopy %SG_SETUP%\%LSG_TARGET%AddOn\Configuration1\Release1\DiskImages\DISK1\*.*    %SG_LATEST%\Setup\%LSG_TARGET%AddOn\*.* /S /Y /Q /K /R /E				>> %LSG_LOGFILE%
     xcopy %SG_SETUP%\%LSG_TARGET%AddOn\Configuration1\Release1\DiskImages\DISK1\*.*    %SG_LATEST%\Setup\%LSG_TARGET%AddOn\*.* /S /Y /Q /K /R /E		>> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error



@REM Uncomment the following line to stop the generation for Install Shield
@REM operations with the correct paths.
@REM --------------------------------------------------------------------------

@REM goto error

@ECHO.
@ECHO Deleting temporary files ...
@ECHO -------------------------------------------------------------------------
@ECHO.                                                                                          >> %LSG_LOGFILE%
@ECHO Deleting temporary files ...                                                              >> %LSG_LOGFILE%
@ECHO -------------------------------------------------------------------------                 >> %LSG_LOGFILE%

%SG_DRIVE%
cd %SG_DRIVE%\%SG_DIR%

@del /s /F J:\*.obj                                                                      >> nul 2>> nul
@del /s /F J:\*.res                                                                      >> nul 2>> nul
@del /s /F J:\*.sbr                                                                      >> nul 2>> nul
@del /s /F J:\vc60.pdb                                                                   >> nul 2>> nul

@ECHO.
@ECHO Clean Up
@ECHO -------------------------------------------------------------------------
@ECHO.                                                                                          >> %LSG_LOGFILE%
@ECHO Clean Up                                                                                  >> %LSG_LOGFILE%
@ECHO -------------------------------------------------------------------------                 >> %LSG_LOGFILE%

cd /D %PROJECT_PARENT%

subst I: /d                                                                             >> nul 2>&1
subst J: /d                                                                             >> nul 2>&1
subst K: /d         																	>> nul 2>&1

@call CreateSetup
if errorlevel 1 goto error

cd /d "%PROJECT_PARENT%"
@call RegistryDlls
if errorlevel 1 goto error

echo GenTarget_%LSG_TARGET%.bat finished at                                                     >> %LSG_LOGFILE%
date /t                                                                                         >> %LSG_LOGFILE%
time /t                                                                                         >> %LSG_LOGFILE%
goto success

@REM --------------------------------------------------------------------------


:error
echo.
echo ERROR on GenTarget_%LSG_TARGET%. See Log file for more informations.
echo --------------------------------------------------------------------------
echo.
exit /B 1
goto end

:directoryerror
echo.
echo ERROR. GenTarget_%LSG_TARGET%.bat cannot be called from repository folder.
echo Call it from SDK folder instead.
echo --------------------------------------------------------------------------
echo.
exit /B 1
goto end

:success
echo.
echo SUCCESS on generation. Target %LSG_TARGET% generated with success.
echo --------------------------------------------------------------------------
echo.
goto end

:notargeterror
echo.
echo ERROR no Target Specified. You should specify a target as paremeter.
echo -------------------------------------------------------------------------
echo.
exit /B 1
goto end

:end
endlocal

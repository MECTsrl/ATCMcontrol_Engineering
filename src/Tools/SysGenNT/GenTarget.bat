@ECHO OFF

setlocal

@ECHO.
@ECHO GenTarget.bat
@ECHO -------------------------------------------------------------------------

set LSG_TARGET=%1
set LSG_KERNEL=%2
set LSG_BRANCH=%3
set LSG_BUILD=%4

if "%LSG_TARGET%" == "" goto error
if "%LSG_KERNEL%" == "" goto error
if "%LSG_BRANCH%" == "" goto error
if "%LSG_BUILD%" == "" goto error


set LSG_BRANCH_DIR=

if "%LSG_BRANCH%" == "DEV" goto skipsetbranch

set LSG_BRANCH_DIR=_%LSG_BRANCH%

:skipsetbranch


set BUILDNUMBER=

if "%LSG_BUILD%" == "TEST" goto skipsetbuildnr
if "%LSG_BUILD%" == "NEW" goto skipsetbuildnr

set BUILDNUMBER=%LSG_BUILD%

:skipsetbuildnr

@ECHO LSG_BRANCH_DIR = %LSG_BRANCH_DIR%
@ECHO BUILDNUMBER    = %BUILDNUMBER%


CALL GenInitEnv.bat

%SG_DRIVE%
cd %SG_DRIVE%\%SG_DIR%

if "%LSG_LOGFILE%" == "" set LSG_LOGFILE=%SG_DRIVE%\%SG_DIR%\Log\%LSG_TARGET%.log
@ECHO LSG_LOGFILE    = %LSG_LOGFILE%

echo GenTarget.bat started at                                                                    > %LSG_LOGFILE%
date /t                                                                                         >> %LSG_LOGFILE%
time /t                                                                                         >> %LSG_LOGFILE%




@ECHO.
@ECHO Set SS Source
@ECHO -------------------------------------------------------------------------
@ECHO.                                                                                          >> %LSG_LOGFILE%
@ECHO Set SS Source                                                                             >> %LSG_LOGFILE%
@ECHO -------------------------------------------------------------------------                 >> %LSG_LOGFILE%

if "%LSG_BRANCH%" == "DEV" goto setdevssdir

set LSG_SSDIR=$/AOReleased/%LSG_TARGET%%LSG_BRANCH_DIR%

if "%LSG_TARGET%" == "4CWin" goto setsstowinBRANCH
if "%LSG_TARGET%" == "4CWIN" goto setsstowinBRANCH

goto setsscontinue

: setsstowinBRANCH

set LSG_SSDIR=$/4CReleased/%LSG_BRANCH%/Target/%LSG_TARGET%


goto setsscontinue

@REM --------------------------------------------------------------------------

:setdevssdir

set LSG_SSDIR=$/Adapt/%LSG_TARGET%%LSG_BRANCH_DIR%

if "%LSG_TARGET%" == "4CWin" goto setsstowin
if "%LSG_TARGET%" == "4CWIN" goto setsstowin

goto setsscontinue

: setsstowin

set LSG_SSDIR=$/4Control%LSG_BRANCH_DIR%/Target/%LSG_TARGET%


: setsscontinue

@ECHO LSG_SSDIR     = %LSG_SSDIR%
@ECHO LSG_SSDIR     = %LSG_SSDIR%                                                               >> %LSG_LOGFILE%



@ECHO.
@ECHO Connect drives
@ECHO -------------------------------------------------------------------------
@ECHO.                                                                                          >> %LSG_LOGFILE%
@ECHO Connect drives                                                                            >> %LSG_LOGFILE%
@ECHO -------------------------------------------------------------------------                 >> %LSG_LOGFILE%

@REM  I: Build directory    ------------------------------

subst I: /d                                                                             >> nul

rd /q /s %SG_GENDIR%                                                                    >> nul
md %SG_GENDIR%                                                                          >> nul

echo subst I: %SG_GENDIR%
echo subst I: %SG_GENDIR%                                                                       >> %LSG_LOGFILE%
     subst I: %SG_GENDIR%                                                               >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

@REM  J: Build directory    ------------------------------

subst J: /d                                                                             >> nul

echo subst J: %SG_GENDIR%
echo subst J: %SG_GENDIR%                                                                       >> %LSG_LOGFILE%
     subst J: %SG_GENDIR%                                                               >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

@REM  K: Setup directory    ------------------------------

subst K: /d                                                                             >> nul

rd /q /s %SG_SETUP%                                                                     >> nul
md %SG_SETUP%                                                                           >> nul

echo subst K: %SG_SETUP%
echo subst K: %SG_SETUP%                                                                        >> %LSG_LOGFILE%
     subst K: %SG_SETUP%                                                                >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error



@ECHO.
@ECHO Increment build number
@ECHO -------------------------------------------------------------------------
@ECHO.                                                                                          >> %LSG_LOGFILE%
@ECHO Increment build number                                                                    >> %LSG_LOGFILE%
@ECHO -------------------------------------------------------------------------                 >> %LSG_LOGFILE%

echo mkdir %SG_TARGET%\SysgenV2\%LSG_TARGET%
echo mkdir %SG_TARGET%\SysgenV2\%LSG_TARGET%                                                    >> %LSG_LOGFILE%
     mkdir %SG_TARGET%\SysgenV2\%LSG_TARGET%                                            >> nul 2>> nul

del /F BuildNr.h                                                                        >> nul

if     "%LSG_BUILD%" == "TEST"  goto skipincrementbuildnr
if not "%LSG_BUILD%" == "NEW"   goto skipincrementbuildnr

echo ss checkout %LSG_SSDIR%/res/BuildNr.h -I- -GF-
echo ss checkout %LSG_SSDIR%/res/BuildNr.h -I- -GF-                                             >> %LSG_LOGFILE%
     ss checkout %LSG_SSDIR%/res/BuildNr.h -I- -GF-                                     >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

del /F BuildNr.bat 2>nul
echo perl BuildNr.pl
echo perl BuildNr.pl                                                                            >> %LSG_LOGFILE%
perl BuildNr.pl                                                                         >> nul 2>> %LSG_LOGFILE%

call BuildNr.bat
if "%BUILDNUMBER%" == "" goto error
@ECHO BUILDNUMBER = %BUILDNUMBER%


echo ss checkin %LSG_SSDIR%/res/BuildNr.h -I- -GF-
echo ss checkin %LSG_SSDIR%/res/BuildNr.h -I- -GF-                                              >> %LSG_LOGFILE%
     ss checkin %LSG_SSDIR%/res/BuildNr.h -I- -GF-                                      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo ss label %LSG_SSDIR%/ -LB%BUILDNUMBER% -I-
echo ss label %LSG_SSDIR%/ -LB%BUILDNUMBER% -I-                                                 >> %LSG_LOGFILE%
     ss label %LSG_SSDIR%/ -LB%BUILDNUMBER% -I-                                         >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

set LSG_BUILD=%BUILDNUMBER%
@ECHO LSG_BUILD = %LSG_BUILD%

goto continuebuildnr


:skipincrementbuildnr

echo ss get %LSG_SSDIR%/res/BuildNr.h -I- -GF-
echo ss get %LSG_SSDIR%/res/BuildNr.h -I- -GF-                                                  >> %LSG_LOGFILE%
     ss get %LSG_SSDIR%/res/BuildNr.h -I- -GF-                                          >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

del /F BuildNr.bat 2>nul
echo perl BuildNr2.pl
echo perl BuildNr2.pl                                                                           >> %LSG_LOGFILE%
     perl BuildNr2.pl                                                                           >> %LSG_LOGFILE%


:continuebuildnr

echo copy BuildNr.bat %SG_TARGET%\SysgenV2\%LSG_TARGET%\BuildNr%LSG_BRANCH_DIR%.bat
echo copy BuildNr.bat %SG_TARGET%\SysgenV2\%LSG_TARGET%\BuildNr%LSG_BRANCH_DIR%.bat             >> %LSG_LOGFILE%
     copy BuildNr.bat %SG_TARGET%\SysgenV2\%LSG_TARGET%\BuildNr%LSG_BRANCH_DIR%.bat     >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error



@ECHO.
@ECHO Build Release Version
@ECHO -------------------------------------------------------------------------
@ECHO.                                                                                          >> %LSG_LOGFILE%
@ECHO Build Release Version                                                                     >> %LSG_LOGFILE%
@ECHO -------------------------------------------------------------------------                 >> %LSG_LOGFILE%

J:
set OFFICIAL_BUILD=1
set PROJECT_ROOT=J:\4Control_R
set TARGET_BASE=J:\4Control_R\TargetBase
md %PROJECT_ROOT%
cd %PROJECT_ROOT%
md %PROJECT_ROOT%\Target\%LSG_TARGET%
md %PROJECT_ROOT%\GenTools
md %PROJECT_ROOT%\SDK

@ECHO Destination = %PROJECT_ROOT%\Target\%LSG_TARGET%
@ECHO Destination = %PROJECT_ROOT%\Target\%LSG_TARGET%                                          >> %LSG_LOGFILE%

set SRCDIR=%SG_SOURCE%\SysgenV2\%LSG_KERNEL%

@ECHO Source = %SG_SOURCE%\SysgenV2\%LSG_KERNEL%
@ECHO Source = %SG_SOURCE%\SysgenV2\%LSG_KERNEL%                                                >> %LSG_LOGFILE%


@ECHO.
@ECHO Copy Base Version
@ECHO -------------------------------------------------------------------------
@ECHO.                                                                                          >> %LSG_LOGFILE%
@ECHO Copy Base Version                                                                         >> %LSG_LOGFILE%
@ECHO -------------------------------------------------------------------------                 >> %LSG_LOGFILE%

echo xcopy %SRCDIR%\4Control_R\TargetBase\*.* %PROJECT_ROOT%\TargetBase\*.* /S /Y /Q /K /R /E
echo xcopy %SRCDIR%\4Control_R\TargetBase\*.* %PROJECT_ROOT%\TargetBase\*.* /S /Y /Q /K /R /E            >> %LSG_LOGFILE%
     xcopy %SRCDIR%\4Control_R\TargetBase\*.* %PROJECT_ROOT%\TargetBase\*.* /S /Y /Q /K /R /E    >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy %SRCDIR%\4Control_R\Engineering\*.* %PROJECT_ROOT%\Engineering\*.* /S /Y /Q /K /R /E
echo xcopy %SRCDIR%\4Control_R\Engineering\*.* %PROJECT_ROOT%\Engineering\*.* /S /Y /Q /K /R /E          >> %LSG_LOGFILE%
     xcopy %SRCDIR%\4Control_R\Engineering\*.* %PROJECT_ROOT%\Engineering\*.* /S /Y /Q /K /R /E  >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy %SRCDIR%\4Control_R\Pub\*.* %PROJECT_ROOT%\Pub\*.* /S /Y /Q /K /R /E
echo xcopy %SRCDIR%\4Control_R\Pub\*.* %PROJECT_ROOT%\Pub\*.* /S /Y /Q /K /R /E                          >> %LSG_LOGFILE%
     xcopy %SRCDIR%\4Control_R\Pub\*.* %PROJECT_ROOT%\Pub\*.* /S /Y /Q /K /R /E                  >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy %SRCDIR%\4Control_R\SetupGen\*.* %PROJECT_ROOT%\SetupGen\*.* /S /Y /Q /K /R /E
echo xcopy %SRCDIR%\4Control_R\SetupGen\*.* %PROJECT_ROOT%\SetupGen\*.* /S /Y /Q /K /R /E                >> %LSG_LOGFILE%
     xcopy %SRCDIR%\4Control_R\SetupGen\*.* %PROJECT_ROOT%\SetupGen\*.* /S /Y /Q /K /R /E        >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy %SRCDIR%\4Control_R\4C_Inst3.dll %PROJECT_ROOT%\*.* /S /Y /Q /K /R /E
echo xcopy %SRCDIR%\4Control_R\4C_Inst3.dll %PROJECT_ROOT%\*.* /S /Y /Q /K /R /E                         >> %LSG_LOGFILE%
     xcopy %SRCDIR%\4Control_R\4C_Inst3.dll %PROJECT_ROOT%\*.* /S /Y /Q /K /R /E                 >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy %SRCDIR%\4Control_R\KADmerge.dll %PROJECT_ROOT%\*.* /S /Y /Q /K /R /E
echo xcopy %SRCDIR%\4Control_R\KADmerge.dll %PROJECT_ROOT%\*.* /S /Y /Q /K /R /E                        >> %LSG_LOGFILE%
     xcopy %SRCDIR%\4Control_R\KADMerge.dll %PROJECT_ROOT%\*.* /S /Y /Q /K /R /E                 >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy %SRCDIR%\4Control_R\KADMerger.exe %PROJECT_ROOT%\*.* /S /Y /Q /K /R /E
echo xcopy %SRCDIR%\4Control_R\KADMerger.exe %PROJECT_ROOT%\*.* /S /Y /Q /K /R /E                        >> %LSG_LOGFILE%
     xcopy %SRCDIR%\4Control_R\KADMerger.exe %PROJECT_ROOT%\*.* /S /Y /Q /K /R /E                >> nul 2>> %LSG_LOGFILE%

echo xcopy %SRCDIR%\MergeModules\*.* %SG_SETUP%\*.* /S /Y /Q /K /R /E
echo xcopy %SRCDIR%\MergeModules\*.* %SG_SETUP%\*.* /S /Y /Q /K /R /E                                    >> %LSG_LOGFILE%
     xcopy %SRCDIR%\MergeModules\*.* %SG_SETUP%\*.* /S /Y /Q /K /R /E                            >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy %SRCDIR%\4Control_R\GenTools\*.* %PROJECT_ROOT%\GenTools\*.* /S /Y /Q /K /R /E
echo xcopy %SRCDIR%\4Control_R\GenTools\*.* %PROJECT_ROOT%\GenTools\*.* /S /Y /Q /K /R /E                >> %LSG_LOGFILE%
     xcopy %SRCDIR%\4Control_R\GenTools\*.* %PROJECT_ROOT%\GenTools\*.* /S /Y /Q /K /R /E        >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error



@ECHO.
@ECHO Get Version (SS)
@ECHO -------------------------------------------------------------------------
@ECHO.                                                                                          >> %LSG_LOGFILE%
@ECHO Get Version (SS)                                                                          >> %LSG_LOGFILE%
@ECHO -------------------------------------------------------------------------                 >> %LSG_LOGFILE%

if "%LSG_BUILD%" == "TEST" goto getactualversion_release

echo ss get %LSG_SSDIR% -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\Target\%LSG_TARGET%
echo ss get %LSG_SSDIR% -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\Target\%LSG_TARGET%         >> %LSG_LOGFILE%
     ss get %LSG_SSDIR% -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\Target\%LSG_TARGET% >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

goto startreleasegen


:getactualversion_release

echo ss get %LSG_SSDIR%  -R -I- -GTM -GL%PROJECT_ROOT%\Target\%LSG_TARGET%
echo ss get %LSG_SSDIR%  -R -I- -GTM -GL%PROJECT_ROOT%\Target\%LSG_TARGET%                      >> %LSG_LOGFILE%
     ss get %LSG_SSDIR%  -R -I- -GTM -GL%PROJECT_ROOT%\Target\%LSG_TARGET%              >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error



:startreleasegen

@ECHO.
@ECHO Generate Release Version
@ECHO -------------------------------------------------------------------------
@ECHO.                                                                                          >> %LSG_LOGFILE%
@ECHO Generate Release Version                                                                  >> %LSG_LOGFILE%
@ECHO -------------------------------------------------------------------------                 >> %LSG_LOGFILE%

J:
cd %PROJECT_ROOT%\Target\%LSG_TARGET%

set NO_MAKE_BROWSE=1
set NO_MAKE_DEPEND=1
set NO_MAKE_QUICK=1

echo perl ..\..\GenTools\make.pl %1
echo perl ..\..\GenTools\make.pl %1                                                             >> %LSG_LOGFILE%
     perl ..\..\GenTools\make.pl %1                                                     >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo nmake -nologo -i -f %1.mak makemake "DEBUG=0" all 2>&1 | perl ..\..\GenTools\LogFile.pl Build.log
echo nmake -nologo -i -f %1.mak makemake "DEBUG=0" all 2>&1 | perl ..\..\GenTools\LogFile.pl Build.log          >> %LSG_LOGFILE%
     nmake -nologo -i -f %1.mak makemake "DEBUG=0" all 2>&1 | perl ..\..\GenTools\LogFile.pl Build.log  >> nul 2>> %LSG_LOGFILE%
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

J:
cd %PROJECT_ROOT%
cd Target\%LSG_TARGET%

cd setup

attrib -R *.ism

echo call IsCmdBld -i Build.ini
echo call IsCmdBld -i Build.ini                                                                 >> %LSG_LOGFILE%
     call IsCmdBld -i Build.ini                                                         >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

cd ..

J:
cd %PROJECT_ROOT%
cd Target\%LSG_TARGET%

echo xcopy %SG_SETUP%\%LSG_TARGET%AddOn\Configuration1\Release1\*.*    %SG_TARGET%\SetupV2\%LSG_TARGET%\B%LSG_BUILD%.%LSG_KERNEL%\%LSG_TARGET%AddOn\*.* /S /Y /Q /K /R /E
echo xcopy %SG_SETUP%\%LSG_TARGET%AddOn\Configuration1\Release1\*.*    %SG_TARGET%\SetupV2\%LSG_TARGET%\B%LSG_BUILD%.%LSG_KERNEL%\%LSG_TARGET%AddOn\*.* /S /Y /Q /K /R /E            >> %LSG_LOGFILE%
     xcopy %SG_SETUP%\%LSG_TARGET%AddOn\Configuration1\Release1\*.*    %SG_TARGET%\SetupV2\%LSG_TARGET%\B%LSG_BUILD%.%LSG_KERNEL%\%LSG_TARGET%AddOn\*.* /S /Y /Q /K /R /E    >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy %SG_SETUP%\%LSG_TARGET%AddOn\Configuration1\Release1\DiskImages\DISK1\*.*    %SG_LATEST%\Setup\%LSG_TARGET%AddOn\*.* /S /Y /Q /K /R /E
echo xcopy %SG_SETUP%\%LSG_TARGET%AddOn\Configuration1\Release1\DiskImages\DISK1\*.*    %SG_LATEST%\Setup\%LSG_TARGET%AddOn\*.* /S /Y /Q /K /R /E				>> %LSG_LOGFILE%
     xcopy %SG_SETUP%\%LSG_TARGET%AddOn\Configuration1\Release1\DiskImages\DISK1\*.*    %SG_LATEST%\Setup\%LSG_TARGET%AddOn\*.* /S /Y /Q /K /R /E		>> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error



@REM Uncomment the following line to stop the generation for Install Shield
@REM operations with the correct paths.
@REM --------------------------------------------------------------------------

@REM goto error



@ECHO.
@ECHO Build Debug Version
@ECHO -------------------------------------------------------------------------
@ECHO.                                                                                          >> %LSG_LOGFILE%
@ECHO Build Debug Version                                                                       >> %LSG_LOGFILE%
@ECHO -------------------------------------------------------------------------                 >> %LSG_LOGFILE%

J:
cd \
set PROJECT_ROOT=J:\4Control
set TARGET_BASE=J:\4Control\TargetBase
md %PROJECT_ROOT%
cd %PROJECT_ROOT%
md %PROJECT_ROOT%\Target\%LSG_TARGET%
md %PROJECT_ROOT%\GenTools
md %PROJECT_ROOT%\SDK

@ECHO Destination = %PROJECT_ROOT%\Target\%LSG_TARGET%
@ECHO Destination = %PROJECT_ROOT%\Target\%LSG_TARGET%                                          >> %LSG_LOGFILE%

set SRCDIR=%SG_SOURCE%\SysgenV2\%LSG_KERNEL%

@ECHO Source = %SG_SOURCE%\SysgenV2\%LSG_KERNEL%
@ECHO Source = %SG_SOURCE%\SysgenV2\%LSG_KERNEL%                                                >> %LSG_LOGFILE%



@ECHO.
@ECHO Copy Base Version
@ECHO -------------------------------------------------------------------------
@ECHO.                                                                                          >> %LSG_LOGFILE%
@ECHO Copy Base Version                                                                         >> %LSG_LOGFILE%
@ECHO -------------------------------------------------------------------------                 >> %LSG_LOGFILE%

echo xcopy %SRCDIR%\4Control\TargetBase\*.* %PROJECT_ROOT%\TargetBase\*.* /S /Y /Q /K /R /E
echo xcopy %SRCDIR%\4Control\TargetBase\*.* %PROJECT_ROOT%\TargetBase\*.* /S /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy %SRCDIR%\4Control\TargetBase\*.* %PROJECT_ROOT%\TargetBase\*.* /S /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy %SRCDIR%\4Control\Engineering\*.* %PROJECT_ROOT%\Engineering\*.* /S /Y /Q /K /R /E
echo xcopy %SRCDIR%\4Control\Engineering\*.* %PROJECT_ROOT%\Engineering\*.* /S /Y /Q /K /R /E            >> %LSG_LOGFILE%
     xcopy %SRCDIR%\4Control\Engineering\*.* %PROJECT_ROOT%\Engineering\*.* /S /Y /Q /K /R /E    >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy %SRCDIR%\4Control\Pub\*.* %PROJECT_ROOT%\Pub\*.* /S /Y /Q /K /R /E
echo xcopy %SRCDIR%\4Control\Pub\*.* %PROJECT_ROOT%\Pub\*.* /S /Y /Q /K /R /E                            >> %LSG_LOGFILE%
     xcopy %SRCDIR%\4Control\Pub\*.* %PROJECT_ROOT%\Pub\*.* /S /Y /Q /K /R /E                    >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy %SRCDIR%\4Control\4C_Inst3.dll %PROJECT_ROOT%\*.* /S /Y /Q /K /R /E
echo xcopy %SRCDIR%\4Control\4C_Inst3.dll %PROJECT_ROOT%\*.* /S /Y /Q /K /R /E                           >> %LSG_LOGFILE%
     xcopy %SRCDIR%\4Control\4C_Inst3.dll %PROJECT_ROOT%\*.* /S /Y /Q /K /R /E                   >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy %SRCDIR%\4Control\KADmerge.dll %PROJECT_ROOT%\*.* /S /Y /Q /K /R /E
echo xcopy %SRCDIR%\4Control\KADmerge.dll %PROJECT_ROOT%\*.* /S /Y /Q /K /R /E                           >> %LSG_LOGFILE%
     xcopy %SRCDIR%\4Control\KADMerge.dll %PROJECT_ROOT%\*.* /S /Y /Q /K /R /E                   >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy %SRCDIR%\4Control_R\KADMerger.exe %PROJECT_ROOT%\*.* /S /Y /Q /K /R /E
echo xcopy %SRCDIR%\4Control_R\KADMerger.exe %PROJECT_ROOT%\*.* /S /Y /Q /K /R /E                        >> %LSG_LOGFILE%
     xcopy %SRCDIR%\4Control_R\KADMerger.exe %PROJECT_ROOT%\*.* /S /Y /Q /K /R /E                >> nul 2>> %LSG_LOGFILE%

echo xcopy %SRCDIR%\MergeModules\*.* %SG_SETUP%\*.* /S /Y /Q /K /R /E
echo xcopy %SRCDIR%\MergeModules\*.* %SG_SETUP%\*.* /S /Y /Q /K /R /E                                    >> %LSG_LOGFILE%
     xcopy %SRCDIR%\MergeModules\*.* %SG_SETUP%\*.* /S /Y /Q /K /R /E                            >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy %SRCDIR%\4Control_R\GenTools\*.* %PROJECT_ROOT%\GenTools\*.* /S /Y /Q /K /R /E
echo xcopy %SRCDIR%\4Control_R\GenTools\*.* %PROJECT_ROOT%\GenTools\*.* /S /Y /Q /K /R /E                >> %LSG_LOGFILE%
     xcopy %SRCDIR%\4Control_R\GenTools\*.* %PROJECT_ROOT%\GenTools\*.* /S /Y /Q /K /R /E        >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error



@ECHO.
@ECHO Get Version (SS)
@ECHO -------------------------------------------------------------------------
@ECHO.                                                                                          >> %LSG_LOGFILE%
@ECHO Get Version (SS)                                                                          >> %LSG_LOGFILE%
@ECHO -------------------------------------------------------------------------                 >> %LSG_LOGFILE%

if "%LSG_BUILD%" == "TEST" goto getactualversion_debug

echo ss get %LSG_SSDIR% -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\Target\%LSG_TARGET%
echo ss get %LSG_SSDIR% -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\Target\%LSG_TARGET%         >> %LSG_LOGFILE%
     ss get %LSG_SSDIR% -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\Target\%LSG_TARGET% >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

goto startdebuggen


:getactualversion_debug

echo ss get %LSG_SSDIR%  -R -I- -GTM -GL%PROJECT_ROOT%\Target\%LSG_TARGET%
echo ss get %LSG_SSDIR%  -R -I- -GTM -GL%PROJECT_ROOT%\Target\%LSG_TARGET%                      >> %LSG_LOGFILE%
     ss get %LSG_SSDIR%  -R -I- -GTM -GL%PROJECT_ROOT%\Target\%LSG_TARGET%              >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error



:startdebuggen

@ECHO.
@ECHO Generate Debug Version
@ECHO -------------------------------------------------------------------------
@ECHO.                                                                                          >> %LSG_LOGFILE%
@ECHO Generate Debug Version                                                                    >> %LSG_LOGFILE%
@ECHO -------------------------------------------------------------------------                 >> %LSG_LOGFILE%

J:
cd %PROJECT_ROOT%\Target\%LSG_TARGET%

set NO_MAKE_BROWSE=1
set NO_MAKE_DEPEND=1
set NO_MAKE_QUICK=1

echo perl ..\..\GenTools\make.pl %1
echo perl ..\..\GenTools\make.pl %1                                                             >> %LSG_LOGFILE%
     perl ..\..\GenTools\make.pl %1                                                     >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo nmake -nologo -i -f %1.mak makemake "DEBUG=1" all 2>&1 | perl ..\..\GenTools\LogFile.pl Build.log
echo nmake -nologo -i -f %1.mak makemake "DEBUG=1" all 2>&1 | perl ..\..\GenTools\LogFile.pl Build.log          >> %LSG_LOGFILE%
     nmake -nologo -i -f %1.mak makemake "DEBUG=1" all 2>&1 | perl ..\..\GenTools\LogFile.pl Build.log  >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo find /I "warning " build.log | perl ..\..\GenTools\LogFile.pl Build.wrn
echo find /I "warning " build.log | perl ..\..\GenTools\LogFile.pl Build.wrn                    >> %LSG_LOGFILE%
     find /I "warning " build.log | perl ..\..\GenTools\LogFile.pl Build.wrn            >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo find /I "error " build.log | perl ..\..\GenTools\LogFile.pl Build.err
echo find /I "error " build.log | perl ..\..\GenTools\LogFile.pl Build.err                      >> %LSG_LOGFILE%
     find /I "error " build.log | perl ..\..\GenTools\LogFile.pl Build.err              >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error


set PROJECT_ROOT=
set OFFICIAL_BUILD=

set NO_MAKE_BROWSE=
set NO_MAKE_DEPEND=
set NO_MAKE_QUICK=



@ECHO.
@ECHO Copy SysGen
@ECHO -------------------------------------------------------------------------
@ECHO.                                                                                          >> %LSG_LOGFILE%
@ECHO Copy SysGen                                                                               >> %LSG_LOGFILE%
@ECHO -------------------------------------------------------------------------                 >> %LSG_LOGFILE%

%SG_DRIVE%
cd %SG_DRIVE%\%SG_DIR%

echo Deleting temporary files ...
echo Deleting temporary files ...                                                               >> %LSG_LOGFILE%

del /s /F J:\*.obj                                                                      >> nul
del /s /F J:\*.res                                                                      >> nul
del /s /F J:\*.sbr                                                                      >> nul
del /s /F J:\vc60.pdb                                                                   >> nul

J:
cd \

echo xcopy J:\*.* %SG_TARGET%\SysGenV2\%LSG_TARGET%\B%LSG_BUILD%.%LSG_KERNEL%\*.* /S /Y /Q /K /R /E
echo xcopy J:\*.* %SG_TARGET%\SysGenV2\%LSG_TARGET%\B%LSG_BUILD%.%LSG_KERNEL%\*.* /S /Y /Q /K /R /E          >> %LSG_LOGFILE%
     xcopy J:\*.* %SG_TARGET%\SysGenV2\%LSG_TARGET%\B%LSG_BUILD%.%LSG_KERNEL%\*.* /S /Y /Q /K /R /E  >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error



@ECHO.
@ECHO Clean Up
@ECHO -------------------------------------------------------------------------
@ECHO.                                                                                          >> %LSG_LOGFILE%
@ECHO Clean Up                                                                                  >> %LSG_LOGFILE%
@ECHO -------------------------------------------------------------------------                 >> %LSG_LOGFILE%

%SG_DRIVE%
cd %SG_DRIVE%\%SG_DIR%

subst I: /d                                                                             >> nul
subst J: /d                                                                             >> nul
subst K: /d                                                                             >> nul

echo GenTarget.bat finished at                                                                  >> %LSG_LOGFILE%
date /t                                                                                         >> %LSG_LOGFILE%
time /t                                                                                         >> %LSG_LOGFILE%


echo copy %LSG_LOGFILE% %SG_TARGET%\SysGenV2\%LSG_TARGET%\B%LSG_BUILD%.%LSG_KERNEL%\GenAdapt.log
     copy %LSG_LOGFILE% %SG_TARGET%\SysGenV2\%LSG_TARGET%\B%LSG_BUILD%.%LSG_KERNEL%\GenAdapt.log

goto end

@REM --------------------------------------------------------------------------


:error

echo.
echo -------  E R R O R  -------
echo.
echo GenTarget [Target] [KernelDir] [Branch] [Build]
echo [Target]:    4CWin 4COsai 4CFC  ...
echo [KernelDir]: V2.10.02.3315.x  V2.10.02.TEST ...
echo [Branch]:    DEV  V2.06  V2.10.01  ...
echo [Build]:     NEW  TEST  3139  3238  ...
echo.

:end

%SG_DRIVE%
cd %SG_DRIVE%\%SG_DIR%

endlocal

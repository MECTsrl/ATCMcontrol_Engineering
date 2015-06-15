@ECHO OFF


setlocal

@ECHO.
@ECHO GenKernel.bat
@ECHO -------------------------------------------------------------------------

set LSG_BRANCH=%1
set LSG_BUILD=%2

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

if "%LSG_LOGFILE%" == "" set LSG_LOGFILE=%SG_DRIVE%\%SG_DIR%\Log\GenKernel.log
@ECHO LSG_LOGFILE    = %LSG_LOGFILE%

echo GenKernel.bat started at                                                                    > %LSG_LOGFILE%
date /t                                                                                         >> %LSG_LOGFILE%
time /t                                                                                         >> %LSG_LOGFILE%




@ECHO.
@ECHO Set SS Source
@ECHO -------------------------------------------------------------------------
@ECHO.                                                                                          >> %LSG_LOGFILE%
@ECHO Set SS Source                                                                             >> %LSG_LOGFILE%
@ECHO -------------------------------------------------------------------------                 >> %LSG_LOGFILE%

if "%LSG_BRANCH%" == "DEV" goto setdevssdir

set LSG_SSDIR=$/4CReleased/%LSG_BRANCH%
goto aftersetssdir

:setdevssdir

set LSG_SSDIR=$/4Control%LSG_BRANCH_DIR%

:aftersetssdir


@ECHO LSG_SSDIR    = %LSG_SSDIR%
@ECHO LSG_SSDIR    = %LSG_SSDIR%                                                                >> %LSG_LOGFILE%



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

del /F BuildNr.h                                                                        >> nul

if     "%LSG_BUILD%" == "TEST"  goto skipincrementbuildnr
if not "%LSG_BUILD%" == "NEW"   goto skipincrementbuildnr

echo ss checkout %LSG_SSDIR%/COM/softing/fc/res/BuildNr.h -I- -GF-
echo ss checkout %LSG_SSDIR%/COM/softing/fc/res/BuildNr.h -I- -GF-                              >> %LSG_LOGFILE%
     ss checkout %LSG_SSDIR%/COM/softing/fc/res/BuildNr.h -I- -GF-                      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

del /F BuildNr.bat 2>nul
echo perl BuildNr.pl
echo perl BuildNr.pl                                                                            >> %LSG_LOGFILE%
     perl BuildNr.pl                                                                    >> nul 2>> %LSG_LOGFILE%

call BuildNr.bat
if "%BUILDNUMBER%" == "" goto error
@ECHO BUILDNUMBER = %BUILDNUMBER%


echo ss checkin %LSG_SSDIR%/COM/softing/fc/res/BuildNr.h -I- -GF-
echo ss checkin %LSG_SSDIR%/COM/softing/fc/res/BuildNr.h -I- -GF-                               >> %LSG_LOGFILE%
     ss checkin %LSG_SSDIR%/COM/softing/fc/res/BuildNr.h -I- -GF-                       >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error


@REM Set Labels

echo ss label %LSG_SSDIR%/COM -LB%BUILDNUMBER% -I-
echo ss label %LSG_SSDIR%/COM -LB%BUILDNUMBER% -I-                                              >> %LSG_LOGFILE%
     ss label %LSG_SSDIR%/COM -LB%BUILDNUMBER% -I-                                      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo ss label %LSG_SSDIR%/TargetBase -LB%BUILDNUMBER% -I-
echo ss label %LSG_SSDIR%/TargetBase -LB%BUILDNUMBER% -I-                                       >> %LSG_LOGFILE%
     ss label %LSG_SSDIR%/TargetBase -LB%BUILDNUMBER% -I-                               >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo ss label %LSG_SSDIR%/GenTools -LB%BUILDNUMBER% -I-
echo ss label %LSG_SSDIR%/GenTools -LB%BUILDNUMBER% -I-                                         >> %LSG_LOGFILE%
     ss label %LSG_SSDIR%/GenTools -LB%BUILDNUMBER% -I-                                 >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo ss label %LSG_SSDIR%/Target/Templates -LB%BUILDNUMBER% -I-
echo ss label %LSG_SSDIR%/Target/Templates -LB%BUILDNUMBER% -I-                                 >> %LSG_LOGFILE%
     ss label %LSG_SSDIR%/Target/Templates -LB%BUILDNUMBER% -I-                         >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo ss label %LSG_SSDIR%/Tools -LB%BUILDNUMBER% -I-
echo ss label %LSG_SSDIR%/Tools -LB%BUILDNUMBER% -I-                                            >> %LSG_LOGFILE%
     ss label %LSG_SSDIR%/Tools -LB%BUILDNUMBER% -I-                                    >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo ss label %LSG_SSDIR%/SetupGen -LB%BUILDNUMBER% -I-
echo ss label %LSG_SSDIR%/SetupGen -LB%BUILDNUMBER% -I-                                         >> %LSG_LOGFILE%
     ss label %LSG_SSDIR%/SetupGen -LB%BUILDNUMBER% -I-                                 >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

set LSG_BUILD=%BUILDNUMBER%
@ECHO LSG_BUILD = %LSG_BUILD%

goto continuebuildnr


:skipincrementbuildnr

echo ss get %LSG_SSDIR%/COM/softing/fc/res/BuildNr.h -I- -GF-
echo ss get %LSG_SSDIR%/COM/softing/fc/res/BuildNr.h -I- -GF-                                   >> %LSG_LOGFILE%
     ss get %LSG_SSDIR%/COM/softing/fc/res/BuildNr.h -I- -GF-                           >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

del /F BuildNr.bat 2>nul
echo perl BuildNr2.pl
echo perl BuildNr2.pl                                                                           >> %LSG_LOGFILE%
     perl BuildNr2.pl                                                                           >> %LSG_LOGFILE%


:continuebuildnr

echo copy BuildNr.bat %SG_TARGET%\SysgenV2\BuildNr%LSG_BRANCH_DIR%.bat
echo copy BuildNr.bat %SG_TARGET%\SysgenV2\BuildNr%LSG_BRANCH_DIR%.bat                          >> %LSG_LOGFILE%
     copy BuildNr.bat %SG_TARGET%\SysgenV2\BuildNr%LSG_BRANCH_DIR%.bat                  >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error



@ECHO.
@ECHO Build Release Version
@ECHO -------------------------------------------------------------------------
@ECHO.                                                                                          >> %LSG_LOGFILE%
@ECHO Build Release Version                                                                     >> %LSG_LOGFILE%
@ECHO -------------------------------------------------------------------------                 >> %LSG_LOGFILE%

I:
set OFFICIAL_BUILD=1
set NO_MAKE_BROWSE=1
set PROJECT_ROOT=I:\4Control_R
md %PROJECT_ROOT%                                                                               >> nul
cd %PROJECT_ROOT%                                                                               >> nul
md COM                                                                                          >> nul
md TargetBase                                                                                   >> nul
md GenTools                                                                                     >> nul
md Target                                                                                       >> nul
md Tools                                                                                        >> nul
md SetupGen                                                                                     >> nul
cd Target                                                                                       >> nul
md Templates                                                                                    >> nul
cd ..                                                                                           >> nul



@ECHO.
@ECHO Get Version (SS)
@ECHO -------------------------------------------------------------------------
@ECHO.                                                                                          >> %LSG_LOGFILE%
@ECHO Get Version (SS)                                                                          >> %LSG_LOGFILE%
@ECHO -------------------------------------------------------------------------                 >> %LSG_LOGFILE%

if "%LSG_BUILD%" == "TEST" goto getactualversion_release

echo ss get %LSG_SSDIR%/GenTools -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\GenTools
echo ss get %LSG_SSDIR%/GenTools -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\GenTools               >> %LSG_LOGFILE%
     ss get %LSG_SSDIR%/GenTools -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\GenTools       >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo ss get %LSG_SSDIR%/COM -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\COM
echo ss get %LSG_SSDIR%/COM -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\COM                         >> %LSG_LOGFILE%
     ss get %LSG_SSDIR%/COM -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\COM                 >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo ss get %LSG_SSDIR%/TargetBase -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\TargetBase
echo ss get %LSG_SSDIR%/TargetBase -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\TargetBase           >> %LSG_LOGFILE%
     ss get %LSG_SSDIR%/TargetBase -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\TargetBase   >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo ss get %LSG_SSDIR%/Target\Templates -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\Target\Templates
echo ss get %LSG_SSDIR%/Target\Templates -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\Target\Templates           >> %LSG_LOGFILE%
     ss get %LSG_SSDIR%/Target\Templates -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\Target\Templates   >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo ss get %LSG_SSDIR%/Tools -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\Tools
echo ss get %LSG_SSDIR%/Tools -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\Tools             >> %LSG_LOGFILE%
     ss get %LSG_SSDIR%/Tools -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\Tools     >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo ss get %LSG_SSDIR%/SetupGen -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\SetupGen
echo ss get %LSG_SSDIR%/SetupGen -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\SetupGen             >> %LSG_LOGFILE%
     ss get %LSG_SSDIR%/SetupGen -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\SetupGen     >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error


goto startreleasegen


:getactualversion_release

echo ss get %LSG_SSDIR%/GenTools  -R -I- -GTM -GL%PROJECT_ROOT%\GenTools      
echo ss get %LSG_SSDIR%/GenTools  -R -I- -GTM -GL%PROJECT_ROOT%\GenTools                        >> %LSG_LOGFILE%     
     ss get %LSG_SSDIR%/GenTools  -R -I- -GTM -GL%PROJECT_ROOT%\GenTools                >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo ss get %LSG_SSDIR%/COM -R -I- -GTM -GL%PROJECT_ROOT%\COM
echo ss get %LSG_SSDIR%/COM -R -I- -GTM -GL%PROJECT_ROOT%\COM                                   >> %LSG_LOGFILE%
     ss get %LSG_SSDIR%/COM -R -I- -GTM -GL%PROJECT_ROOT%\COM                           >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo ss get %LSG_SSDIR%/TargetBase -R -I- -GTM -GL%PROJECT_ROOT%\TargetBase
echo ss get %LSG_SSDIR%/TargetBase -R -I- -GTM -GL%PROJECT_ROOT%\TargetBase                     >> %LSG_LOGFILE%
     ss get %LSG_SSDIR%/TargetBase -R -I- -GTM -GL%PROJECT_ROOT%\TargetBase             >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo ss get %LSG_SSDIR%/Target\Templates -R -I- -GTM -GL%PROJECT_ROOT%\Target\Templates
echo ss get %LSG_SSDIR%/Target\Templates -R -I- -GTM -GL%PROJECT_ROOT%\Target\Templates             >> %LSG_LOGFILE%
     ss get %LSG_SSDIR%/Target\Templates -R -I- -GTM -GL%PROJECT_ROOT%\Target\Templates     >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo ss get %LSG_SSDIR%/Tools -R -I- -GTM -GL%PROJECT_ROOT%\Tools
echo ss get %LSG_SSDIR%/Tools -R -I- -GTM -GL%PROJECT_ROOT%\Tools                               >> %LSG_LOGFILE%
     ss get %LSG_SSDIR%/Tools -R -I- -GTM -GL%PROJECT_ROOT%\Tools                       >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo ss get %LSG_SSDIR%/SetupGen -R -I- -GTM -GL%PROJECT_ROOT%\SetupGen
echo ss get %LSG_SSDIR%/SetupGen -R -I- -GTM -GL%PROJECT_ROOT%\SetupGen                         >> %LSG_LOGFILE%
     ss get %LSG_SSDIR%/SetupGen -R -I- -GTM -GL%PROJECT_ROOT%\SetupGen                 >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error


:startreleasegen

@ECHO.
@ECHO Generate Release Version
@ECHO -------------------------------------------------------------------------
@ECHO.                                                                                          >> %LSG_LOGFILE%
@ECHO Generate Release Version                                                                  >> %LSG_LOGFILE%
@ECHO -------------------------------------------------------------------------                 >> %LSG_LOGFILE%

echo call %PROJECT_ROOT%\COM\softing\fc\res\Version.bat                             
echo call %PROJECT_ROOT%\COM\softing\fc\res\Version.bat                                         >> %LSG_LOGFILE%
     call %PROJECT_ROOT%\COM\softing\fc\res\Version.bat                                 >> nul 2>> %LSG_LOGFILE%                            
if errorlevel 1 goto error

@ECHO PROJECT_ROOT = %PROJECT_ROOT%
@ECHO PROJECT_ROOT = %PROJECT_ROOT%                                                             >> %LSG_LOGFILE%

@ECHO Destination  = %SG_TARGET%\SetupV2\%FC_VERSION%.%LSG_BUILD%
@ECHO Destination  = %SG_TARGET%\SetupV2\%FC_VERSION%.%LSG_BUILD%                               >> %LSG_LOGFILE%

I:
cd %PROJECT_ROOT%\COM

set NO_MAKE_BROWSE=1
set NO_MAKE_DEPEND=1
set NO_MAKE_QUICK=1

echo perl ..\GenTools\make.pl Kernel_Ext
echo perl ..\GenTools\make.pl Kernel_Ext                                                      >> %LSG_LOGFILE%
     perl ..\GenTools\make.pl Kernel_Ext                                              >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo nmake -nologo -i -f Kernel_Ext.mak makemake "DEBUG=0" all 2>&1 | perl ..\GenTools\LogFile.pl Build.log
echo nmake -nologo -i -f Kernel_Ext.mak makemake "DEBUG=0" all 2>&1 | perl ..\GenTools\LogFile.pl Build.log          >> %LSG_LOGFILE%
     nmake -nologo -i -f Kernel_Ext.mak makemake "DEBUG=0" all 2>&1 | perl ..\GenTools\LogFile.pl Build.log  >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo find /I "warning " build.log | perl ..\GenTools\LogFile.pl Build.wrn
echo find /I "warning " build.log | perl ..\GenTools\LogFile.pl Build.wrn                       >> %LSG_LOGFILE%
     find /I "warning " build.log | perl ..\GenTools\LogFile.pl Build.wrn               >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo find /I "error " build.log | perl ..\GenTools\LogFile.pl Build.err
echo find /I "error " build.log | perl ..\GenTools\LogFile.pl Build.err                         >> %LSG_LOGFILE%
     find /I "error " build.log | perl ..\GenTools\LogFile.pl Build.err                 >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error



@ECHO.
@ECHO Generate TargetBase
@ECHO -------------------------------------------------------------------------
@ECHO.                                                                                          >> %LSG_LOGFILE%
@ECHO Generate TargetBase                                                                       >> %LSG_LOGFILE%
@ECHO -------------------------------------------------------------------------                 >> %LSG_LOGFILE%

set TARGET_BASE=%PROJECT_ROOT%\TargetBase
@ECHO TARGET_BASE = %TARGET_BASE%

echo md %PROJECT_ROOT%\TargetBase\Kernel
echo md %PROJECT_ROOT%\TargetBase\Kernel                                                        >> %LSG_LOGFILE%
     md %PROJECT_ROOT%\TargetBase\Kernel                                                >> nul 2>> %LSG_LOGFILE%

echo attrib -R /S BaseBuildNr.h
echo attrib -R /S BaseBuildNr.h                                                                 >> %LSG_LOGFILE%
     attrib -R /S BaseBuildNr.h                                                         >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo perl BaseBuildNr.pl
echo perl BaseBuildNr.pl                                                                        >> %LSG_LOGFILE%
     perl BaseBuildNr.pl                                                                >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\inc\*.*   %TARGET_BASE%\Kernel\inc\*.* /S /Y /Q /K /R /E 
echo xcopy softing\fc\inc\*.*   %TARGET_BASE%\Kernel\inc\*.* /S /Y /Q /K /R /E                           >> %LSG_LOGFILE% 
     xcopy softing\fc\inc\*.*   %TARGET_BASE%\Kernel\inc\*.* /S /Y /Q /K /R /E                   >> nul 2>> %LSG_LOGFILE% 
if errorlevel 1 goto error

echo xcopy softing\fc\lib\*.*   %TARGET_BASE%\Kernel\lib\*.* /S /Y /Q /K /R /E
echo xcopy softing\fc\lib\*.*   %TARGET_BASE%\Kernel\lib\*.* /S /Y /Q /K /R /E                           >> %LSG_LOGFILE%
     xcopy softing\fc\lib\*.*   %TARGET_BASE%\Kernel\lib\*.* /S /Y /Q /K /R /E                   >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\EXT\*.*   %TARGET_BASE%\Kernel\EXT\*.* /S /Y /Q /K /R /E
echo xcopy softing\fc\EXT\*.*   %TARGET_BASE%\Kernel\EXT\*.* /S /Y /Q /K /R /E                           >> %LSG_LOGFILE%
     xcopy softing\fc\EXT\*.*   %TARGET_BASE%\Kernel\EXT\*.* /S /Y /Q /K /R /E                   >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\res\*.*   %TARGET_BASE%\Kernel\res\*.* /S /Y /Q /K /R /E
echo xcopy softing\fc\res\*.*   %TARGET_BASE%\Kernel\res\*.* /S /Y /Q /K /R /E                           >> %LSG_LOGFILE%
     xcopy softing\fc\res\*.*   %TARGET_BASE%\Kernel\res\*.* /S /Y /Q /K /R /E                   >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\Stingray\*.*        %TARGET_BASE%\Kernel\CE\inc\Stringray\*.* /S /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\Stingray\*.*        %TARGET_BASE%\Kernel\CE\inc\Stringray\*.* /S /Y /Q /K /R /E             >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\Stingray\*.*        %TARGET_BASE%\Kernel\CE\inc\Stringray\*.* /S /Y /Q /K /R /E     >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\XMLHelper\*.*       %TARGET_BASE%\Kernel\CE\inc\XMLHelper\*.* /S /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\XMLHelper\*.*       %TARGET_BASE%\Kernel\CE\inc\XMLHelper\*.* /S /Y /Q /K /R /E             >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\XMLHelper\*.*       %TARGET_BASE%\Kernel\CE\inc\XMLHelper\*.* /S /Y /Q /K /R /E     >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\CEAddon*.*          %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\CEAddon*.*          %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\CEAddon*.*          %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\CEBackparse*.*      %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\CEBackparse*.*      %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\CEBackparse*.*      %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\CEContainer*.*      %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\CEContainer*.*      %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\CEContainer*.*      %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\CEDragDrop*.*       %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\CEDragDrop*.*       %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\CEDragDrop*.*       %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\CEProjInfo*.*       %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\CEProjInfo*.*       %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\CEProjInfo*.*       %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\CEProjWiz*.*        %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\CEProjWiz*.*        %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\CEProjWiz*.*        %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\CESysDef.h          %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\CESysDef.h          %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\CESysDef.h          %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\CEVarDecl*.*        %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\CEVarDecl*.*        %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\CEVarDecl*.*        %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\ExeDelegator*.*     %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\ExeDelegator*.*     %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\ExeDelegator*.*     %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\FindInFiles*.*      %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\FindInFiles*.*      %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\FindInFiles*.*      %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\PrgEditor*.*        %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\PrgEditor*.*        %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\PrgEditor*.*        %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\SBL*.*              %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\SBL*.*              %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\SBL*.*              %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\SEC*.*              %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\SEC*.*              %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\SEC*.*              %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\StdEditor*.*        %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\StdEditor*.*        %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\StdEditor*.*        %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\key*.*              %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\key*.*              %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\key*.*              %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\cecompmanev*.*      %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\cecompmanev*.*      %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\cecompmanev*.*      %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\DirectoryDlg*.*     %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\DirectoryDlg*.*     %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\DirectoryDlg*.*     %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\CEIECOperation*.*   %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\CEIECOperation*.*   %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\CEIECOperation*.*   %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\lib\Stingray\*.*        %TARGET_BASE%\Kernel\CE\lib\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\lib\Stingray\*.*        %TARGET_BASE%\Kernel\CE\lib\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\lib\Stingray\*.*        %TARGET_BASE%\Kernel\CE\lib\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\lib\CEUtil.lib          %TARGET_BASE%\Kernel\CE\lib\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\lib\CEUtil.lib          %TARGET_BASE%\Kernel\CE\lib\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\lib\CEUtil.lib          %TARGET_BASE%\Kernel\CE\lib\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\lib\CEUtil_XML.lib      %TARGET_BASE%\Kernel\CE\lib\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\lib\CEUtil_XML.lib      %TARGET_BASE%\Kernel\CE\lib\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\lib\CEUtil_XML.lib      %TARGET_BASE%\Kernel\CE\lib\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\lib\OE.lib              %TARGET_BASE%\Kernel\CE\lib\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\lib\OE.lib              %TARGET_BASE%\Kernel\CE\lib\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\lib\OE.lib              %TARGET_BASE%\Kernel\CE\lib\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\lib\PrgEditor.lib       %TARGET_BASE%\Kernel\CE\lib\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\lib\PrgEditor.lib       %TARGET_BASE%\Kernel\CE\lib\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\lib\PrgEditor.lib       %TARGET_BASE%\Kernel\CE\lib\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\lib\StdEditor.lib       %TARGET_BASE%\Kernel\CE\lib\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\lib\StdEditor.lib       %TARGET_BASE%\Kernel\CE\lib\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\lib\StdEditor.lib       %TARGET_BASE%\Kernel\CE\lib\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CCG\inc\*.*                %TARGET_BASE%\Kernel\CCG\inc\*.*  /Y /Q /K /R /E
echo xcopy softing\fc\CCG\inc\*.*                %TARGET_BASE%\Kernel\CCG\inc\*.*  /Y /Q /K /R /E                >> %LSG_LOGFILE%
     xcopy softing\fc\CCG\inc\*.*                %TARGET_BASE%\Kernel\CCG\inc\*.*  /Y /Q /K /R /E        >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CSC\opcEngine\include\*.*      %TARGET_BASE%\Kernel\CSC\opcEngine\include\*.*  /Y /Q /K /R /E
echo xcopy softing\fc\CSC\opcEngine\include\*.*      %TARGET_BASE%\Kernel\CSC\opcEngine\include\*.*  /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CSC\opcEngine\include\*.*      %TARGET_BASE%\Kernel\CSC\opcEngine\include\*.*  /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CSC\opcEngine\engine\opcerror.h %TARGET_BASE%\Kernel\CSC\opcEngine\include\*.*  /Y /Q /K /R /E
echo xcopy softing\fc\CSC\opcEngine\engine\opcerror.h %TARGET_BASE%\Kernel\CSC\opcEngine\include\*.*  /Y /Q /K /R /E             >> %LSG_LOGFILE%
     xcopy softing\fc\CSC\opcEngine\engine\opcerror.h %TARGET_BASE%\Kernel\CSC\opcEngine\include\*.*  /Y /Q /K /R /E     >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\ce\cebackparser\backparserdll\*.h  %TARGET_BASE%\Kernel\CE\cebackparser\backparserdll\*.h  /Y /Q /K /R /E
echo xcopy softing\fc\ce\cebackparser\backparserdll\*.h  %TARGET_BASE%\Kernel\CE\cebackparser\backparserdll\*.h  /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\ce\cebackparser\backparserdll\*.h  %TARGET_BASE%\Kernel\CE\cebackparser\backparserdll\*.h  /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\ce\lib\BackparserDLL.lib           %TARGET_BASE%\Kernel\CE\lib\*.*  /Y /Q /K /R /E
echo xcopy softing\fc\ce\lib\BackparserDLL.lib           %TARGET_BASE%\Kernel\CE\lib\*.*  /Y /Q /K /R /E                     >> %LSG_LOGFILE%
     xcopy softing\fc\ce\lib\BackparserDLL.lib           %TARGET_BASE%\Kernel\CE\lib\*.*  /Y /Q /K /R /E             >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\ce\cewiz\inc\*.*                   %TARGET_BASE%\Kernel\CE\cewiz\inc\*.*  /Y /Q /K /R /E
echo xcopy softing\fc\ce\cewiz\inc\*.*                   %TARGET_BASE%\Kernel\CE\cewiz\inc\*.*  /Y /Q /K /R /E               >> %LSG_LOGFILE%
     xcopy softing\fc\ce\cewiz\inc\*.*                   %TARGET_BASE%\Kernel\CE\cewiz\inc\*.*  /Y /Q /K /R /E       >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\ce\cewiz\lib\ProjectWizardCore.lib %TARGET_BASE%\Kernel\CE\lib\*.*  /Y /Q /K /R /E
echo xcopy softing\fc\ce\cewiz\lib\ProjectWizardCore.lib %TARGET_BASE%\Kernel\CE\lib\*.*  /Y /Q /K /R /E                     >> %LSG_LOGFILE%
     xcopy softing\fc\ce\cewiz\lib\ProjectWizardCore.lib %TARGET_BASE%\Kernel\CE\lib\*.*  /Y /Q /K /R /E             >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error


I:
cd %PROJECT_ROOT%\TargetBase

set NO_MAKE_BROWSE=1
set NO_MAKE_DEPEND=1
set NO_MAKE_QUICK=1

echo perl ..\GenTools\make.pl TargetBase_Ext
echo perl ..\GenTools\make.pl TargetBase_Ext                                                        >> %LSG_LOGFILE%
     perl ..\GenTools\make.pl TargetBase_Ext                                                >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo nmake -nologo -i -f TargetBase_Ext.mak makemake "DEBUG=0" all 2>&1 | perl ..\GenTools\LogFile.pl Build.log
echo nmake -nologo -i -f TargetBase_Ext.mak makemake "DEBUG=0" all 2>&1 | perl ..\GenTools\LogFile.pl Build.log          >> %LSG_LOGFILE%
     nmake -nologo -i -f TargetBase_Ext.mak makemake "DEBUG=0" all 2>&1 | perl ..\GenTools\LogFile.pl Build.log  >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo find /I "warning " build.log | perl ..\GenTools\LogFile.pl Build.wrn
echo find /I "warning " build.log | perl ..\GenTools\LogFile.pl Build.wrn                       >> %LSG_LOGFILE%
     find /I "warning " build.log | perl ..\GenTools\LogFile.pl Build.wrn               >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo find /I "error " build.log | perl ..\GenTools\LogFile.pl Build.err
echo find /I "error " build.log | perl ..\GenTools\LogFile.pl Build.err                         >> %LSG_LOGFILE%
     find /I "error " build.log | perl ..\GenTools\LogFile.pl Build.err                 >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error



@ECHO.
@ECHO Create Setup
@ECHO -------------------------------------------------------------------------
@ECHO.                                                                                          >> %LSG_LOGFILE%
@ECHO Create Setup                                                                              >> %LSG_LOGFILE%
@ECHO -------------------------------------------------------------------------                 >> %LSG_LOGFILE%

I:
cd %PROJECT_ROOT%
cd COM\softing\fc\CSET

attrib -R /S *.ism

cd MergeModules

cd SharedDLLs
echo call IsCmdBld -i SharedDLLs\Build.ini
echo call IsCmdBld -i SharedDLLs\Build.ini                                                      >> %LSG_LOGFILE%
     call IsCmdBld -i Build.ini                                                         >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

cd ..\SharedTLBs
echo call IsCmdBld -i SharedTLBs\Build.ini
echo call IsCmdBld -i SharedTLBs\Build.ini                                                      >> %LSG_LOGFILE%
     call IsCmdBld -i Build.ini                                                         >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

cd ..\opcEngine
echo call IsCmdBld -i opcEngine\Build.ini
echo call IsCmdBld -i opcEngine\Build.ini                                                       >> %LSG_LOGFILE%
     call IsCmdBld -i Build.ini                                                         >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

cd ..\ParseGSD
echo call IsCmdBld -i ParseGSD\Build.ini
echo call IsCmdBld -i ParseGSD\Build.ini                                                        >> %LSG_LOGFILE%
     call IsCmdBld -i Build.ini                                                         >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

cd ..\OpcEnum
echo call IsCmdBld -i OpcEnum\Build.ini
echo call IsCmdBld -i OpcEnum\Build.ini                                                         >> %LSG_LOGFILE%
     call IsCmdBld -i Build.ini                                                         >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

cd ..\Hardlock
echo call IsCmdBld -i Hardlock\Build.ini
echo call IsCmdBld -i Hardlock\Build.ini                                                        >> %LSG_LOGFILE%
     call IsCmdBld -i Build.ini                                                         >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

cd ..\FC_Uninstall
echo call IsCmdBld -i FC_Uninstall\Build.ini
echo call IsCmdBld -i FC_Uninstall\Build.ini                                                    >> %LSG_LOGFILE%
     call IsCmdBld -i Build.ini                                                         >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

cd ..\GSD
echo call IsCmdBld -i GSD\Build.ini
echo call IsCmdBld -i GSD\Build.ini                                                             >> %LSG_LOGFILE%
     call IsCmdBld -i Build.ini                                                         >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

@REM cd ..\FBC_ProfiDP
@REM echo call IsCmdBld -i FBC_ProfiDP\Build.ini
@REM echo call IsCmdBld -i FBC_ProfiDP\Build.ini                                                     >> %LSG_LOGFILE%
@REM      call IsCmdBld -i Build.ini                                                         >> nul 2>> %LSG_LOGFILE%
@REM if errorlevel 1 goto error

@REM cd ..\FBC_BACnet
@REM echo call IsCmdBld -i FBC_BACnet\Build.ini
@REM echo call IsCmdBld -i FBC_BACnet\Build.ini                                                     >> %LSG_LOGFILE%
@REM      call IsCmdBld -i Build.ini                                                         >> nul 2>> %LSG_LOGFILE%
@REM if errorlevel 1 goto error

cd ..\PMTool
echo call IsCmdBld -i PMTool\Build.ini
echo call IsCmdBld -i PMTool\Build.ini                                                          >> %LSG_LOGFILE%
     call IsCmdBld -i Build.ini                                                         >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error


cd ..\..

cd Setups

cd 4CKernel
echo call IsCmdBld -i 4CKernel\Build.ini
echo call IsCmdBld -i 4CKernel\Build.ini                                                        >> %LSG_LOGFILE%
     call IsCmdBld -i Build.ini                                                         >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

@REM cd ..\4CPCAddOn
@REM echo call IsCmdBld -i 4CPCAddOn\Build.ini
@REM echo call IsCmdBld -i 4CPCAddOn\Build.ini                                                       >> %LSG_LOGFILE%
@REM      call IsCmdBld -i Build.ini                                                         >> nul 2>> %LSG_LOGFILE%
@REM if errorlevel 1 goto error

@REM cd ..\4CPCCtrl
@REM echo call IsCmdBld -i 4CPCCtrl\Build.ini
@REM echo call IsCmdBld -i 4CPCCtrl\Build.ini                                                        >> %LSG_LOGFILE%
@REM      call IsCmdBld -i Build.ini                                                         >> nul 2>> %LSG_LOGFILE%
@REM if errorlevel 1 goto error

@REM cd ..\4CDatabase
@REM echo call IsCmdBld -i 4CDatabase\Build.ini
@REM echo call IsCmdBld -i 4CDatabase\Build.ini                                                      >> %LSG_LOGFILE%
@REM      call IsCmdBld -i Build.ini                                                         >> nul 2>> %LSG_LOGFILE%
@REM if errorlevel 1 goto error

@REM cd ..\4CControlLoop
@REM echo call IsCmdBld -i 4CControlLoop\Build.ini
@REM echo call IsCmdBld -i 4CControlLoop\Build.ini                                                   >> %LSG_LOGFILE%
@REM      call IsCmdBld -i Build.ini                                                         >> nul 2>> %LSG_LOGFILE%
@REM if errorlevel 1 goto error

cd ..\..


@REM Uncomment the following line to stop the generation for Install Shield
@REM operations with the correct paths.
@REM --------------------------------------------------------------------------

@REM goto error



@ECHO.
@ECHO Copy Setups
@ECHO -------------------------------------------------------------------------
@ECHO.                                                                                          >> %LSG_LOGFILE%
@ECHO Copy Setups                                                                               >> %LSG_LOGFILE%
@ECHO -------------------------------------------------------------------------                 >> %LSG_LOGFILE%

echo md I:\MergeModules                                                                 >> nul
     md I:\MergeModules

echo copy %SG_SETUP%\*.msm I:\MergeModules\*.msm /Y
echo copy %SG_SETUP%\*.msm I:\MergeModules\*.msm /Y                                             >> %LSG_LOGFILE%
     copy %SG_SETUP%\*.msm I:\MergeModules\*.msm /Y                                     >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error


echo xcopy %SG_SETUP%\4CKernel\Configuration1\Release1\*.*      %SG_TARGET%\SetupV2\%FC_VERSION%.%LSG_BUILD%\4CKernel\*.*  /S /Y /Q /K /R /E
echo xcopy %SG_SETUP%\4CKernel\Configuration1\Release1\*.*      %SG_TARGET%\SetupV2\%FC_VERSION%.%LSG_BUILD%\4CKernel\*.*  /S /Y /Q /K /R /E                >> %LSG_LOGFILE%
     xcopy %SG_SETUP%\4CKernel\Configuration1\Release1\*.*      %SG_TARGET%\SetupV2\%FC_VERSION%.%LSG_BUILD%\4CKernel\*.*  /S /Y /Q /K /R /E        >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

@REM echo xcopy %SG_SETUP%\4CPCAddOn\Configuration1\Release1\*.*     %SG_TARGET%\SetupV2\%FC_VERSION%.%LSG_BUILD%\4CPCAddOn\*.* /S /Y /Q /K /R /E 
@REM echo xcopy %SG_SETUP%\4CPCAddOn\Configuration1\Release1\*.*     %SG_TARGET%\SetupV2\%FC_VERSION%.%LSG_BUILD%\4CPCAddOn\*.* /S /Y /Q /K /R /E                >> %LSG_LOGFILE%
@REM      xcopy %SG_SETUP%\4CPCAddOn\Configuration1\Release1\*.*     %SG_TARGET%\SetupV2\%FC_VERSION%.%LSG_BUILD%\4CPCAddOn\*.* /S /Y /Q /K /R /E        >> nul 2>> %LSG_LOGFILE%
@REM if errorlevel 1 goto error

@REM echo xcopy %SG_SETUP%\4CPCControl\Configuration1\Release1\*.*   %SG_TARGET%\SetupV2\%FC_VERSION%.%LSG_BUILD%\4CPCControl\*.* /S /Y /Q /K /R /E 
@REM echo xcopy %SG_SETUP%\4CPCControl\Configuration1\Release1\*.*   %SG_TARGET%\SetupV2\%FC_VERSION%.%LSG_BUILD%\4CPCControl\*.* /S /Y /Q /K /R /E              >> %LSG_LOGFILE%
@REM      xcopy %SG_SETUP%\4CPCControl\Configuration1\Release1\*.*   %SG_TARGET%\SetupV2\%FC_VERSION%.%LSG_BUILD%\4CPCControl\*.* /S /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
@REM if errorlevel 1 goto error

@REM echo xcopy %SG_SETUP%\4CDatabase\Configuration1\Release1\*.*    %SG_TARGET%\SetupV2\%FC_VERSION%.%LSG_BUILD%\4CDatabase\*.* /S /Y /Q /K /R /E 
@REM echo xcopy %SG_SETUP%\4CDatabase\Configuration1\Release1\*.*    %SG_TARGET%\SetupV2\%FC_VERSION%.%LSG_BUILD%\4CDatabase\*.* /S /Y /Q /K /R /E               >> %LSG_LOGFILE%
@REM      xcopy %SG_SETUP%\4CDatabase\Configuration1\Release1\*.*    %SG_TARGET%\SetupV2\%FC_VERSION%.%LSG_BUILD%\4CDatabase\*.* /S /Y /Q /K /R /E       >> nul 2>> %LSG_LOGFILE%
@REM if errorlevel 1 goto error

@REM echo xcopy %SG_SETUP%\4CControlloop\Configuration1\Release1\*.* %SG_TARGET%\SetupV2\%FC_VERSION%.%LSG_BUILD%\4CControlLoop\*.* /S /Y /Q /K /R /E 
@REM echo xcopy %SG_SETUP%\4CControlloop\Configuration1\Release1\*.* %SG_TARGET%\SetupV2\%FC_VERSION%.%LSG_BUILD%\4CControlLoop\*.* /S /Y /Q /K /R /E            >> %LSG_LOGFILE%
@REM      xcopy %SG_SETUP%\4CControlloop\Configuration1\Release1\*.* %SG_TARGET%\SetupV2\%FC_VERSION%.%LSG_BUILD%\4CControlLoop\*.* /S /Y /Q /K /R /E    >> nul 2>> %LSG_LOGFILE%
@REM if errorlevel 1 goto error

echo xcopy %SG_SOFTLM%\*.* %SG_TARGET%\SetupV2\%FC_VERSION%.%LSG_BUILD%\LM\*.* /S /Y /Q /K /R /E 
echo xcopy %SG_SOFTLM%\*.* %SG_TARGET%\SetupV2\%FC_VERSION%.%LSG_BUILD%\LM\*.* /S /Y /Q /K /R /E                >> %LSG_LOGFILE%
     xcopy %SG_SOFTLM%\*.* %SG_TARGET%\SetupV2\%FC_VERSION%.%LSG_BUILD%\LM\*.* /S /Y /Q /K /R /E        >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error



echo xcopy %SG_SETUP%\4CKernel\Configuration1\Release1\DiskImages\DISK1\*.*      %SG_LATEST%\Setup\4CKernel\*.*  /S /Y /Q /K /R /E
echo xcopy %SG_SETUP%\4CKernel\Configuration1\Release1\DiskImages\DISK1\*.*      %SG_LATEST%\Setup\4CKernel\*.*  /S /Y /Q /K /R /E                  >> %LSG_LOGFILE%
     xcopy %SG_SETUP%\4CKernel\Configuration1\Release1\DiskImages\DISK1\*.*      %SG_LATEST%\Setup\4CKernel\*.*  /S /Y /Q /K /R /E          >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

@REM echo xcopy %SG_SETUP%\4CPCAddOn\Configuration1\Release1\DiskImages\DISK1\*.*     %SG_LATEST%\Setup\4CPCAddOn\*.* /S /Y /Q /K /R /E 
@REM echo xcopy %SG_SETUP%\4CPCAddOn\Configuration1\Release1\DiskImages\DISK1\*.*     %SG_LATEST%\Setup\4CPCAddOn\*.* /S /Y /Q /K /R /E                  >> %LSG_LOGFILE%
@REM      xcopy %SG_SETUP%\4CPCAddOn\Configuration1\Release1\DiskImages\DISK1\*.*     %SG_LATEST%\Setup\4CPCAddOn\*.* /S /Y /Q /K /R /E          >> nul 2>> %LSG_LOGFILE%
@REM if errorlevel 1 goto error

@REM echo xcopy %SG_SETUP%\4CPCControl\Configuration1\Release1\DiskImages\DISK1\*.*   %SG_LATEST%\Setup\4CPCControl\*.* /S /Y /Q /K /R /E 
@REM echo xcopy %SG_SETUP%\4CPCControl\Configuration1\Release1\DiskImages\DISK1\*.*   %SG_LATEST%\Setup\4CPCControl\*.* /S /Y /Q /K /R /E                >> %LSG_LOGFILE%
@REM      xcopy %SG_SETUP%\4CPCControl\Configuration1\Release1\DiskImages\DISK1\*.*   %SG_LATEST%\Setup\4CPCControl\*.* /S /Y /Q /K /R /E        >> nul 2>> %LSG_LOGFILE%
@REM if errorlevel 1 goto error

@REM echo xcopy %SG_SETUP%\4CDatabase\Configuration1\Release1\DiskImages\DISK1\*.*    %SG_LATEST%\Setup\4CDatabase\*.* /S /Y /Q /K /R /E 
@REM echo xcopy %SG_SETUP%\4CDatabase\Configuration1\Release1\DiskImages\DISK1\*.*    %SG_LATEST%\Setup\4CDatabase\*.* /S /Y /Q /K /R /E                 >> %LSG_LOGFILE%
@REM      xcopy %SG_SETUP%\4CDatabase\Configuration1\Release1\DiskImages\DISK1\*.*    %SG_LATEST%\Setup\4CDatabase\*.* /S /Y /Q /K /R /E         >> nul 2>> %LSG_LOGFILE%
@REM if errorlevel 1 goto error

@REM echo xcopy %SG_SETUP%\4CControlloop\Configuration1\Release1\DiskImages\DISK1\*.* %SG_LATEST%\Setup\4CControlLoop\*.* /S /Y /Q /K /R /E 
@REM echo xcopy %SG_SETUP%\4CControlloop\Configuration1\Release1\DiskImages\DISK1\*.* %SG_LATEST%\Setup\4CControlLoop\*.* /S /Y /Q /K /R /E              >> %LSG_LOGFILE%
@REM      xcopy %SG_SETUP%\4CControlloop\Configuration1\Release1\DiskImages\DISK1\*.* %SG_LATEST%\Setup\4CControlLoop\*.* /S /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
@REM if errorlevel 1 goto error


echo xcopy %SG_SOFTLM%\*.* %SG_LATEST%\Setup\LM\*.* /S /Y /Q /K /R /E 
echo xcopy %SG_SOFTLM%\*.* %SG_LATEST%\Setup\LM\*.* /S /Y /Q /K /R /E                                       >> %LSG_LOGFILE%
     xcopy %SG_SOFTLM%\*.* %SG_LATEST%\Setup\LM\*.* /S /Y /Q /K /R /E                               >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error


echo copy %PROJECT_ROOT%\Setup\Autorun.exe                 %SG_LATEST%\Autorun.exe /Y
echo copy %PROJECT_ROOT%\Setup\Autorun.exe                 %SG_LATEST%\Autorun.exe /Y           >> %LSG_LOGFILE%
     copy %PROJECT_ROOT%\Setup\Autorun.exe                 %SG_LATEST%\Autorun.exe /Y   >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo copy %PROJECT_ROOT%\Setup\Docu\PC-Based\Autorun.ico   %SG_LATEST%\Autorun.ico /Y
echo copy %PROJECT_ROOT%\Setup\Docu\PC-Based\Autorun.ico   %SG_LATEST%\Autorun.ico /Y           >> %LSG_LOGFILE%
     copy %PROJECT_ROOT%\Setup\Docu\PC-Based\Autorun.ico   %SG_LATEST%\Autorun.ico /Y   >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo copy %PROJECT_ROOT%\Setup\Docu\PC-Based\Autorun.inf   %SG_LATEST%\Autorun.inf /Y
echo copy %PROJECT_ROOT%\Setup\Docu\PC-Based\Autorun.inf   %SG_LATEST%\Autorun.inf /Y           >> %LSG_LOGFILE%
     copy %PROJECT_ROOT%\Setup\Docu\PC-Based\Autorun.inf   %SG_LATEST%\Autorun.inf /Y   >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy %PROJECT_ROOT%\Setup\Docu\PC-Based\Autorun\*.*  %SG_LATEST%\Autorun\*.* /S /Y /Q /K /R /E
echo xcopy %PROJECT_ROOT%\Setup\Docu\PC-Based\Autorun\*.*  %SG_LATEST%\Autorun\*.* /S /Y /Q /K /R /E            >> %LSG_LOGFILE%
     xcopy %PROJECT_ROOT%\Setup\Docu\PC-Based\Autorun\*.*  %SG_LATEST%\Autorun\*.* /S /Y /Q /K /R /E    >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo copy %PROJECT_ROOT%\Setup\Setup.exe                   %SG_LATEST%\Setup.exe /Y
echo copy %PROJECT_ROOT%\Setup\Setup.exe                   %SG_LATEST%\Setup.exe /Y             >> %LSG_LOGFILE%
     copy %PROJECT_ROOT%\Setup\Setup.exe                   %SG_LATEST%\Setup.exe /Y     >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo copy %PROJECT_ROOT%\Setup\*.ini                       %SG_LATEST%\*.ini /Y
echo copy %PROJECT_ROOT%\Setup\*.ini                       %SG_LATEST%\*.ini /Y                 >> %LSG_LOGFILE%
     copy %PROJECT_ROOT%\Setup\*.ini                       %SG_LATEST%\*.ini /Y         >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo copy %PROJECT_ROOT%\Setup\de\*.ini                    %SG_LATEST%\*.ini /Y
echo copy %PROJECT_ROOT%\Setup\de\*.ini                    %SG_LATEST%\*.ini /Y                 >> %LSG_LOGFILE%
     copy %PROJECT_ROOT%\Setup\de\*.ini                    %SG_LATEST%\*.ini /Y         >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo copy %PROJECT_ROOT%\Setup\*.bat                       %SG_LATEST%\*.bat /Y
echo copy %PROJECT_ROOT%\Setup\*.bat                       %SG_LATEST%\*.bat /Y                 >> %LSG_LOGFILE%
     copy %PROJECT_ROOT%\Setup\*.bat                       %SG_LATEST%\*.bat /Y         >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo copy %PROJECT_ROOT%\Setup\Setup\License.rtf           %SG_LATEST%\Setup\License.rtf /Y
echo copy %PROJECT_ROOT%\Setup\Setup\License.rtf           %SG_LATEST%\Setup\License.rtf /Y         >> %LSG_LOGFILE%
     copy %PROJECT_ROOT%\Setup\Setup\License.rtf           %SG_LATEST%\Setup\License.rtf /Y >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy %PROJECT_ROOT%\Setup\iis\*.htm                  %SG_LATEST%\Redist\*.htm /Y /Q /K /R
echo xcopy %PROJECT_ROOT%\Setup\iis\*.htm                  %SG_LATEST%\Redist\*.htm /Y /Q /K /R             >> %LSG_LOGFILE%
     xcopy %PROJECT_ROOT%\Setup\iis\*.htm                  %SG_LATEST%\Redist\*.htm /Y /Q /K /R     >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error



@ECHO.
@ECHO Build Debug Version
@ECHO -------------------------------------------------------------------------
@ECHO.                                                                                          >> %LSG_LOGFILE%
@ECHO Build Debug Version                                                                       >> %LSG_LOGFILE%
@ECHO -------------------------------------------------------------------------                 >> %LSG_LOGFILE%

I:
cd \
set NO_MAKE_BROWSE=0
set PROJECT_ROOT=I:\4Control
md %PROJECT_ROOT%                                                                               >> nul
cd %PROJECT_ROOT%                                                                               >> nul
md COM                                                                                          >> nul
md TargetBase                                                                                   >> nul
md gentools                                                                                     >> nul
md Target                                                                                       >> nul
md Tools                                                                                        >> nul
md SetupGen                                                                                     >> nul
cd Target                                                                                       >> nul
md Templates                                                                                    >> nul
cd ..                                                                                           >> nul

@ECHO Destination = %SG_TARGET%\SetupV2\%FC_VERSION%.%LSG_BUILD%
@ECHO Destination = %SG_TARGET%\SetupV2\%FC_VERSION%.%LSG_BUILD%                                >> %LSG_LOGFILE%



@ECHO.
@ECHO Get Version (SS)
@ECHO -------------------------------------------------------------------------
@ECHO.                                                                                          >> %LSG_LOGFILE%
@ECHO Get Version (SS)                                                                          >> %LSG_LOGFILE%
@ECHO -------------------------------------------------------------------------                 >> %LSG_LOGFILE%

if "%LSG_BUILD%" == "TEST" goto getactualversion_debug

echo ss get %LSG_SSDIR%/GenTools -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\GenTools
echo ss get %LSG_SSDIR%/GenTools -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\GenTools               >> %LSG_LOGFILE%
     ss get %LSG_SSDIR%/GenTools -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\GenTools       >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo ss get %LSG_SSDIR%/COM -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\COM
echo ss get %LSG_SSDIR%/COM -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\COM                         >> %LSG_LOGFILE%
     ss get %LSG_SSDIR%/COM -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\COM                 >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo ss get %LSG_SSDIR%/TargetBase -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\TargetBase
echo ss get %LSG_SSDIR%/TargetBase -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\TargetBase           >> %LSG_LOGFILE%
     ss get %LSG_SSDIR%/TargetBase -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\TargetBase   >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo ss get %LSG_SSDIR%/Target\Templates -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\Target\Templates
echo ss get %LSG_SSDIR%/Target\Templates -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\Target\Templates           >> %LSG_LOGFILE%
     ss get %LSG_SSDIR%/Target\Templates -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\Target\Templates   >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo ss get %LSG_SSDIR%/Tools -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\Tools
echo ss get %LSG_SSDIR%/Tools -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\Tools                     >> %LSG_LOGFILE%
     ss get %LSG_SSDIR%/Tools -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\Tools             >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo ss get %LSG_SSDIR%/SetupGen -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\SetupGen
echo ss get %LSG_SSDIR%/SetupGen -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\SetupGen              >> %LSG_LOGFILE%
     ss get %LSG_SSDIR%/SetupGen -VLB%BUILDNUMBER% -R -I- -GTM -GL%PROJECT_ROOT%\SetupGen      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

goto startdebuggen


:getactualversion_debug

echo ss get %LSG_SSDIR%/GenTools  -R -I- -GTM -GL%PROJECT_ROOT%\GenTools      
echo ss get %LSG_SSDIR%/GenTools  -R -I- -GTM -GL%PROJECT_ROOT%\GenTools                        >> %LSG_LOGFILE%     
     ss get %LSG_SSDIR%/GenTools  -R -I- -GTM -GL%PROJECT_ROOT%\GenTools                >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo ss get %LSG_SSDIR%/COM -R -I- -GTM -GL%PROJECT_ROOT%\COM
echo ss get %LSG_SSDIR%/COM -R -I- -GTM -GL%PROJECT_ROOT%\COM                                   >> %LSG_LOGFILE%
     ss get %LSG_SSDIR%/COM -R -I- -GTM -GL%PROJECT_ROOT%\COM                           >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo ss get %LSG_SSDIR%/TargetBase -R -I- -GTM -GL%PROJECT_ROOT%\TargetBase
echo ss get %LSG_SSDIR%/TargetBase -R -I- -GTM -GL%PROJECT_ROOT%\TargetBase                     >> %LSG_LOGFILE%
     ss get %LSG_SSDIR%/TargetBase -R -I- -GTM -GL%PROJECT_ROOT%\TargetBase             >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo ss get %LSG_SSDIR%/Target\Templates -R -I- -GTM -GL%PROJECT_ROOT%\Target\Templates
echo ss get %LSG_SSDIR%/Target\Templates -R -I- -GTM -GL%PROJECT_ROOT%\Target\Templates             >> %LSG_LOGFILE%
     ss get %LSG_SSDIR%/Target\Templates -R -I- -GTM -GL%PROJECT_ROOT%\Target\Templates     >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo ss get %LSG_SSDIR%/Tools -R -I- -GTM -GL%PROJECT_ROOT%\Tools
echo ss get %LSG_SSDIR%/Tools -R -I- -GTM -GL%PROJECT_ROOT%\Tools                               >> %LSG_LOGFILE%
     ss get %LSG_SSDIR%/Tools -R -I- -GTM -GL%PROJECT_ROOT%\Tools                       >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo ss get %LSG_SSDIR%/SetupGen -R -I- -GTM -GL%PROJECT_ROOT%\SetupGen
echo ss get %LSG_SSDIR%/SetupGen -R -I- -GTM -GL%PROJECT_ROOT%\SetupGen                        >> %LSG_LOGFILE%
     ss get %LSG_SSDIR%/SetupGen -R -I- -GTM -GL%PROJECT_ROOT%\SetupGen                >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error


:startdebuggen

@ECHO.
@ECHO Generate Debug Version
@ECHO -------------------------------------------------------------------------
@ECHO.                                                                                          >> %LSG_LOGFILE%
@ECHO Generate Debug Version                                                                    >> %LSG_LOGFILE%
@ECHO -------------------------------------------------------------------------                 >> %LSG_LOGFILE%

I:
cd %PROJECT_ROOT%\COM

set NO_MAKE_BROWSE=1
set NO_MAKE_DEPEND=1
set NO_MAKE_QUICK=1

echo perl ..\GenTools\make.pl Kernel_Ext
echo perl ..\GenTools\make.pl Kernel_Ext                                                            >> %LSG_LOGFILE%
     perl ..\GenTools\make.pl Kernel_Ext                                                    >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo nmake -nologo -i -f Kernel_Ext.mak makemake "DEBUG=1" all 2>&1 | perl ..\GenTools\LogFile.pl Build.log
echo nmake -nologo -i -f Kernel_Ext.mak makemake "DEBUG=1" all 2>&1 | perl ..\GenTools\LogFile.pl Build.log          >> %LSG_LOGFILE%
     nmake -nologo -i -f Kernel_Ext.mak makemake "DEBUG=1" all 2>&1 | perl ..\GenTools\LogFile.pl Build.log  >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo find /I "warning " build.log | perl ..\GenTools\LogFile.pl Build.wrn
echo find /I "warning " build.log | perl ..\GenTools\LogFile.pl Build.wrn                       >> %LSG_LOGFILE%
     find /I "warning " build.log | perl ..\GenTools\LogFile.pl Build.wrn               >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo find /I "error " build.log | perl ..\GenTools\LogFile.pl Build.err
echo find /I "error " build.log | perl ..\GenTools\LogFile.pl Build.err                         >> %LSG_LOGFILE%
     find /I "error " build.log | perl ..\GenTools\LogFile.pl Build.err                 >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error



@ECHO.
@ECHO Generate TargetBase
@ECHO -------------------------------------------------------------------------
@ECHO.                                                                                          >> %LSG_LOGFILE%
@ECHO Generate TargetBase                                                                       >> %LSG_LOGFILE%
@ECHO -------------------------------------------------------------------------                 >> %LSG_LOGFILE%

set TARGET_BASE=%PROJECT_ROOT%\TargetBase
@ECHO TARGET_BASE = %TARGET_BASE%

echo md %PROJECT_ROOT%\TargetBase\Kernel
echo md %PROJECT_ROOT%\TargetBase\Kernel                                                        >> %LSG_LOGFILE%
     md %PROJECT_ROOT%\TargetBase\Kernel                                                >> nul 2>> %LSG_LOGFILE%

echo attrib -R /S BaseBuildNr.h
echo attrib -R /S BaseBuildNr.h                                                                 >> %LSG_LOGFILE%
     attrib -R /S BaseBuildNr.h                                                         >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo perl BaseBuildNr.pl
echo perl BaseBuildNr.pl                                                                        >> %LSG_LOGFILE%
     perl BaseBuildNr.pl                                                                >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\inc\*.*   %TARGET_BASE%\Kernel\inc\*.* /S /Y /Q /K /R /E 
echo xcopy softing\fc\inc\*.*   %TARGET_BASE%\Kernel\inc\*.* /S /Y /Q /K /R /E                           >> %LSG_LOGFILE% 
     xcopy softing\fc\inc\*.*   %TARGET_BASE%\Kernel\inc\*.* /S /Y /Q /K /R /E                   >> nul 2>> %LSG_LOGFILE% 
if errorlevel 1 goto error

echo xcopy softing\fc\lib\*.*   %TARGET_BASE%\Kernel\lib\*.* /S /Y /Q /K /R /E
echo xcopy softing\fc\lib\*.*   %TARGET_BASE%\Kernel\lib\*.* /S /Y /Q /K /R /E                           >> %LSG_LOGFILE%
     xcopy softing\fc\lib\*.*   %TARGET_BASE%\Kernel\lib\*.* /S /Y /Q /K /R /E                   >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\EXT\*.*   %TARGET_BASE%\Kernel\EXT\*.* /S /Y /Q /K /R /E
echo xcopy softing\fc\EXT\*.*   %TARGET_BASE%\Kernel\EXT\*.* /S /Y /Q /K /R /E                           >> %LSG_LOGFILE%
     xcopy softing\fc\EXT\*.*   %TARGET_BASE%\Kernel\EXT\*.* /S /Y /Q /K /R /E                   >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\res\*.*   %TARGET_BASE%\Kernel\res\*.* /S /Y /Q /K /R /E
echo xcopy softing\fc\res\*.*   %TARGET_BASE%\Kernel\res\*.* /S /Y /Q /K /R /E                           >> %LSG_LOGFILE%
     xcopy softing\fc\res\*.*   %TARGET_BASE%\Kernel\res\*.* /S /Y /Q /K /R /E                   >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\Stingray\*.*        %TARGET_BASE%\Kernel\CE\inc\Stringray\*.* /S /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\Stingray\*.*        %TARGET_BASE%\Kernel\CE\inc\Stringray\*.* /S /Y /Q /K /R /E             >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\Stingray\*.*        %TARGET_BASE%\Kernel\CE\inc\Stringray\*.* /S /Y /Q /K /R /E     >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\XMLHelper\*.*       %TARGET_BASE%\Kernel\CE\inc\XMLHelper\*.* /S /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\XMLHelper\*.*       %TARGET_BASE%\Kernel\CE\inc\XMLHelper\*.* /S /Y /Q /K /R /E             >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\XMLHelper\*.*       %TARGET_BASE%\Kernel\CE\inc\XMLHelper\*.* /S /Y /Q /K /R /E     >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\CEAddon*.*          %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\CEAddon*.*          %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\CEAddon*.*          %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\CEBackparse*.*      %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\CEBackparse*.*      %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\CEBackparse*.*      %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\CEContainer*.*      %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\CEContainer*.*      %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\CEContainer*.*      %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\CEDragDrop*.*       %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\CEDragDrop*.*       %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\CEDragDrop*.*       %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\CEProjInfo*.*       %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\CEProjInfo*.*       %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\CEProjInfo*.*       %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\CEProjWiz*.*        %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\CEProjWiz*.*        %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\CEProjWiz*.*        %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\CESysDef.h          %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\CESysDef.h          %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\CESysDef.h          %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\CEVarDecl*.*        %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\CEVarDecl*.*        %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\CEVarDecl*.*        %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\ExeDelegator*.*     %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\ExeDelegator*.*     %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\ExeDelegator*.*     %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\FindInFiles*.*      %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\FindInFiles*.*      %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\FindInFiles*.*      %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\PrgEditor*.*        %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\PrgEditor*.*        %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\PrgEditor*.*        %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\SBL*.*              %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\SBL*.*              %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\SBL*.*              %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\SEC*.*              %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\SEC*.*              %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\SEC*.*              %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\StdEditor*.*        %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\StdEditor*.*        %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\StdEditor*.*        %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\key*.*              %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\key*.*              %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\key*.*              %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\cecompmanev*.*      %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\cecompmanev*.*      %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\cecompmanev*.*      %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\DirectoryDlg*.*     %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\DirectoryDlg*.*     %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\DirectoryDlg*.*     %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\inc\CEIECOperation*.*   %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\inc\CEIECOperation*.*   %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\inc\CEIECOperation*.*   %TARGET_BASE%\Kernel\CE\inc\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\lib\Stingray\*.*        %TARGET_BASE%\Kernel\CE\lib\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\lib\Stingray\*.*        %TARGET_BASE%\Kernel\CE\lib\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\lib\Stingray\*.*        %TARGET_BASE%\Kernel\CE\lib\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\lib\CEUtil.lib          %TARGET_BASE%\Kernel\CE\lib\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\lib\CEUtil.lib          %TARGET_BASE%\Kernel\CE\lib\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\lib\CEUtil.lib          %TARGET_BASE%\Kernel\CE\lib\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\lib\CEUtil_XML.lib      %TARGET_BASE%\Kernel\CE\lib\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\lib\CEUtil_XML.lib      %TARGET_BASE%\Kernel\CE\lib\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\lib\CEUtil_XML.lib      %TARGET_BASE%\Kernel\CE\lib\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\lib\OE.lib              %TARGET_BASE%\Kernel\CE\lib\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\lib\OE.lib              %TARGET_BASE%\Kernel\CE\lib\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\lib\OE.lib              %TARGET_BASE%\Kernel\CE\lib\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\lib\PrgEditor.lib       %TARGET_BASE%\Kernel\CE\lib\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\lib\PrgEditor.lib       %TARGET_BASE%\Kernel\CE\lib\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\lib\PrgEditor.lib       %TARGET_BASE%\Kernel\CE\lib\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CE\lib\StdEditor.lib       %TARGET_BASE%\Kernel\CE\lib\*.* /Y /Q /K /R /E
echo xcopy softing\fc\CE\lib\StdEditor.lib       %TARGET_BASE%\Kernel\CE\lib\*.* /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CE\lib\StdEditor.lib       %TARGET_BASE%\Kernel\CE\lib\*.* /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CCG\inc\*.*                %TARGET_BASE%\Kernel\CCG\inc\*.*  /Y /Q /K /R /E
echo xcopy softing\fc\CCG\inc\*.*                %TARGET_BASE%\Kernel\CCG\inc\*.*  /Y /Q /K /R /E            >> %LSG_LOGFILE%
     xcopy softing\fc\CCG\inc\*.*                %TARGET_BASE%\Kernel\CCG\inc\*.*  /Y /Q /K /R /E    >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CSC\opcEngine\include\*.*      %TARGET_BASE%\Kernel\CSC\opcEngine\include\*.*  /Y /Q /K /R /E
echo xcopy softing\fc\CSC\opcEngine\include\*.*      %TARGET_BASE%\Kernel\CSC\opcEngine\include\*.*  /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\CSC\opcEngine\include\*.*      %TARGET_BASE%\Kernel\CSC\opcEngine\include\*.*  /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\CSC\opcEngine\engine\opcerror.h %TARGET_BASE%\Kernel\CSC\opcEngine\include\*.*  /Y /Q /K /R /E
echo xcopy softing\fc\CSC\opcEngine\engine\opcerror.h %TARGET_BASE%\Kernel\CSC\opcEngine\include\*.*  /Y /Q /K /R /E             >> %LSG_LOGFILE%
     xcopy softing\fc\CSC\opcEngine\engine\opcerror.h %TARGET_BASE%\Kernel\CSC\opcEngine\include\*.*  /Y /Q /K /R /E     >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\ce\cebackparser\backparserdll\*.h  %TARGET_BASE%\Kernel\CE\cebackparser\backparserdll\*.h  /Y /Q /K /R /E
echo xcopy softing\fc\ce\cebackparser\backparserdll\*.h  %TARGET_BASE%\Kernel\CE\cebackparser\backparserdll\*.h  /Y /Q /K /R /E              >> %LSG_LOGFILE%
     xcopy softing\fc\ce\cebackparser\backparserdll\*.h  %TARGET_BASE%\Kernel\CE\cebackparser\backparserdll\*.h  /Y /Q /K /R /E      >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\ce\lib\BackparserDLL.lib           %TARGET_BASE%\Kernel\CE\lib\*.*  /Y /Q /K /R /E
echo xcopy softing\fc\ce\lib\BackparserDLL.lib           %TARGET_BASE%\Kernel\CE\lib\*.*  /Y /Q /K /R /E                     >> %LSG_LOGFILE%
     xcopy softing\fc\ce\lib\BackparserDLL.lib           %TARGET_BASE%\Kernel\CE\lib\*.*  /Y /Q /K /R /E             >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\ce\cewiz\inc\*.*                   %TARGET_BASE%\Kernel\CE\cewiz\inc\*.*  /Y /Q /K /R /E
echo xcopy softing\fc\ce\cewiz\inc\*.*                   %TARGET_BASE%\Kernel\CE\cewiz\inc\*.*  /Y /Q /K /R /E               >> %LSG_LOGFILE%
     xcopy softing\fc\ce\cewiz\inc\*.*                   %TARGET_BASE%\Kernel\CE\cewiz\inc\*.*  /Y /Q /K /R /E       >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy softing\fc\ce\cewiz\lib\ProjectWizardCore.lib %TARGET_BASE%\Kernel\CE\lib\*.*  /Y /Q /K /R /E
echo xcopy softing\fc\ce\cewiz\lib\ProjectWizardCore.lib %TARGET_BASE%\Kernel\CE\lib\*.*  /Y /Q /K /R /E                     >> %LSG_LOGFILE%
     xcopy softing\fc\ce\cewiz\lib\ProjectWizardCore.lib %TARGET_BASE%\Kernel\CE\lib\*.*  /Y /Q /K /R /E             >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error


I:
cd %PROJECT_ROOT%\TargetBase

set NO_MAKE_BROWSE=1
set NO_MAKE_DEPEND=1
set NO_MAKE_QUICK=1

echo perl ..\GenTools\make.pl TargetBase_Ext
echo perl ..\GenTools\make.pl TargetBase_Ext                                                        >> %LSG_LOGFILE%
     perl ..\GenTools\make.pl TargetBase_Ext                                                >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo nmake -nologo -i -f TargetBase_Ext.mak makemake "DEBUG=1" all 2>&1 | perl ..\GenTools\LogFile.pl Build.log
echo nmake -nologo -i -f TargetBase_Ext.mak makemake "DEBUG=1" all 2>&1 | perl ..\GenTools\LogFile.pl Build.log          >> %LSG_LOGFILE%
     nmake -nologo -i -f TargetBase_Ext.mak makemake "DEBUG=1" all 2>&1 | perl ..\GenTools\LogFile.pl Build.log  >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo find /I "warning " build.log | perl ..\GenTools\LogFile.pl Build.wrn
echo find /I "warning " build.log | perl ..\GenTools\LogFile.pl Build.wrn                       >> %LSG_LOGFILE%
     find /I "warning " build.log | perl ..\GenTools\LogFile.pl Build.wrn               >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo find /I "error " build.log | perl ..\GenTools\LogFile.pl Build.err
echo find /I "error " build.log | perl ..\GenTools\LogFile.pl Build.err                         >> %LSG_LOGFILE%
     find /I "error " build.log | perl ..\GenTools\LogFile.pl Build.err                 >> nul 2>> %LSG_LOGFILE%
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

del /s /F I:\*.obj                                                                      >> nul
del /s /F I:\*.res                                                                      >> nul
del /s /F I:\*.sbr                                                                      >> nul
del /s /F I:\vc60.pdb                                                                   >> nul

I:
cd \

echo xcopy I:\*.* %SG_TARGET%\SysGenV2\%FC_VERSION%.%LSG_BUILD%\*.* /S /Y /Q /K /R /E
echo xcopy I:\*.* %SG_TARGET%\SysGenV2\%FC_VERSION%.%LSG_BUILD%\*.* /S /Y /Q /K /R /E                    >> %LSG_LOGFILE%
     xcopy I:\*.* %SG_TARGET%\SysGenV2\%FC_VERSION%.%LSG_BUILD%\*.* /S /Y /Q /K /R /E            >> nul 2>> %LSG_LOGFILE%
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
subst K: /d                                                                             >> nul

echo GenKernel.bat finished at                                                                  >> %LSG_LOGFILE%
date /t                                                                                         >> %LSG_LOGFILE%
time /t                                                                                         >> %LSG_LOGFILE%


echo copy %LSG_LOGFILE% %SG_TARGET%\SysGenV2\%FC_VERSION%.%LSG_BUILD%\GenKernel.log
     copy %LSG_LOGFILE% %SG_TARGET%\SysGenV2\%FC_VERSION%.%LSG_BUILD%\GenKernel.log

goto end

@REM --------------------------------------------------------------------------


:error

echo.
echo -------  E R R O R  -------
echo.
echo GenKernel [Branch] [Build]
echo [Branch]: DEV  V2.06  V2.10.01  ...
echo [Build]:  NEW  TEST  3139  3238  ...
echo.

:end

%SG_DRIVE%
cd %SG_DRIVE%\%SG_DIR%

endlocal

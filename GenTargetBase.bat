@ECHO OFF

setlocal

set CWD=%~d0%~p0 
cd /D %CWD%

@ECHO.
@ECHO Set-VCVARS.bat
@ECHO -------------------------------------------------------------------------

@call "%programfiles%\Microsoft Visual Studio\VC98\bin\VCVARS32" >nul 2>&1

CALL GenInitEnv.bat
CALL CheckInstallation

if "%LSG_LOGFILE%"=="" set LSG_LOGFILE=GenTargetBase.log

@ECHO.
@ECHO Generate TargetBase
@ECHO -------------------------------------------------------------------------
@ECHO.                                                                                          >> %LSG_LOGFILE%
@ECHO Generate TargetBase                                                                       >> %LSG_LOGFILE%
@ECHO -------------------------------------------------------------------------                 >> %LSG_LOGFILE%

cd /D %PROJECT_ROOT%

echo md %PROJECT_ROOT%\TargetBase\Kernel
echo md %PROJECT_ROOT%\TargetBase\Kernel                                                        >> %LSG_LOGFILE%
     md %PROJECT_ROOT%\TargetBase\Kernel                                                >> nul 2>> %LSG_LOGFILE%

set TARGET_BASE=%PROJECT_ROOT%\TargetBase
@ECHO TARGET_BASE = %TARGET_BASE%

cd /D %PROJECT_ROOT%\COM
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

cd /D %PROJECT_ROOT%\TargetBase

set NO_MAKE_BROWSE=1
set NO_MAKE_DEPEND=1
set NO_MAKE_QUICK=1

echo perl ..\GenTools\make.pl TargetBase_Prima
echo perl ..\GenTools\make.pl TargetBase_Prima                                                        >> %LSG_LOGFILE%
     perl ..\GenTools\make.pl TargetBase_Prima                                                >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo nmake -nologo -i -f TargetBase_Prima.mak makemake "DEBUG=0" all 2>&1 | perl ..\GenTools\LogFile.pl Build.log
echo nmake -nologo -i -f TargetBase_Prima.mak makemake "DEBUG=0" all 2>&1 | perl ..\GenTools\LogFile.pl Build.log          >> %LSG_LOGFILE%
     nmake -nologo -i -f TargetBase_Prima.mak makemake "DEBUG=0" all 2>&1 | perl ..\GenTools\LogFile.pl Build.log  >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo find /I "warning " build.log | perl ..\GenTools\LogFile.pl Build.wrn
echo find /I "warning " build.log | perl ..\GenTools\LogFile.pl Build.wrn                       >> %LSG_LOGFILE%
     find /I "warning " build.log | perl ..\GenTools\LogFile.pl Build.wrn               >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo find /I "error " build.log | perl ..\GenTools\LogFile.pl Build.err
echo find /I "error " build.log | perl ..\GenTools\LogFile.pl Build.err                         >> %LSG_LOGFILE%
     find /I "error " build.log | perl ..\GenTools\LogFile.pl Build.err                 >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error
goto end

:error
echo.
echo ERROR on GenTargetBase. See Log file for more informations.
echo -------------------------------------------------------------------------
echo.
exit /B 1
goto end

:end
endlocal

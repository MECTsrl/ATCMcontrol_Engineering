@Echo off

setlocal

set CWD=%~d0%~p0 
cd /D %CWD%

if "%LSG_LOGFILE%"=="" set LSG_LOGFILE=GenKernel.log
if "%PROJECT_ROOT%"=="" set PROJECT_ROOT=%CD%\GenWork\GenDir.tmp\4Control_R
@ECHO PROJECT_ROOT = %PROJECT_ROOT%
@ECHO PROJECT_ROOT = %PROJECT_ROOT%                                                             >> %LSG_LOGFILE%
if "%PROJECT_PARENT%"=="" set PROJECT_PARENT=%CD%
if "%ISDIR%"=="" set ISDIR=%CD%\GenWork\GenDir.tmp
if "%SG_SETUP%"=="" set SG_SETUP=%CD%\GenWork\SetupDir
if "%SG_TARGET%"=="" set SG_TARGET=%CD%\Installer
if "%SG_LATEST%"=="" set SG_LATEST=%CD%\Installer\RELEASE


@ECHO.
@ECHO Create Setup
@ECHO -------------------------------------------------------------------------
@ECHO.                                                                                          >> %LSG_LOGFILE%
@ECHO Create Setup                                                                              >> %LSG_LOGFILE%
@ECHO -------------------------------------------------------------------------                 >> %LSG_LOGFILE%

subst I: /d                                                                             >> nul 2>&1

echo subst I: %ISDIR%
echo subst I: %ISDIR%                                                                       	>> %LSG_LOGFILE%
     subst I: %ISDIR%                                                               	>> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

subst  K: /d																			>> nul 2>&1
rd /q /s %SG_SETUP%                                                                     >> nul
md %SG_SETUP%    


echo subst K: %SG_SETUP%
echo subst K: %SG_SETUP%                                                                        >> %LSG_LOGFILE%
     subst K: %SG_SETUP%                                                                >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error


cd /D %PROJECT_ROOT%
cd /D COM\softing\fc\CSET

echo attrib -R /S *.ism
attrib -R /S *.ism

cd /D MergeModules

cd /D SharedDLLs
echo call IsCmdBld -i SharedDLLs\Build.ini
echo call IsCmdBld -i SharedDLLs\Build.ini                                                      >> %LSG_LOGFILE%
     call IsCmdBld -i Build.ini                                                         >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

cd /D ..\SharedTLBs
echo call IsCmdBld -i SharedTLBs\Build.ini
echo call IsCmdBld -i SharedTLBs\Build.ini                                                      >> %LSG_LOGFILE%
     call IsCmdBld -i Build.ini                                                         >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

cd /D ..\opcEngine
echo call IsCmdBld -i opcEngine\Build.ini
echo call IsCmdBld -i opcEngine\Build.ini                                                       >> %LSG_LOGFILE%
     call IsCmdBld -i Build.ini                                                         >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

cd /D ..\ParseGSD
echo call IsCmdBld -i ParseGSD\Build.ini
echo call IsCmdBld -i ParseGSD\Build.ini                                                        >> %LSG_LOGFILE%
     call IsCmdBld -i Build.ini                                                         >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

cd /D ..\OpcEnum
echo call IsCmdBld -i OpcEnum\Build.ini
echo call IsCmdBld -i OpcEnum\Build.ini                                                         >> %LSG_LOGFILE%
     call IsCmdBld -i Build.ini                                                         >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

cd /D ..\Hardlock
echo call IsCmdBld -i Hardlock\Build.ini
echo call IsCmdBld -i Hardlock\Build.ini                                                        >> %LSG_LOGFILE%
     call IsCmdBld -i Build.ini                                                         >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

cd /D ..\FC_Uninstall
echo call IsCmdBld -i FC_Uninstall\Build.ini
echo call IsCmdBld -i FC_Uninstall\Build.ini                                                    >> %LSG_LOGFILE%
     call IsCmdBld -i Build.ini                                                         >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

cd /D ..\GSD
echo call IsCmdBld -i GSD\Build.ini
echo call IsCmdBld -i GSD\Build.ini                                                             >> %LSG_LOGFILE%
     call IsCmdBld -i Build.ini                                                         >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error


cd /D ..\PMTool
echo call IsCmdBld -i PMTool\Build.ini
echo call IsCmdBld -i PMTool\Build.ini                                                          >> %LSG_LOGFILE%
     call IsCmdBld -i Build.ini                                                         >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

cd /D ..\..

cd /D Setups

cd /D ATCMControl
echo call IsCmdBld -i ATCMControl\Build.ini
echo call IsCmdBld -i ATCMControl\Build.ini                                                     >> %LSG_LOGFILE%
     call IsCmdBld -i Build.ini                                                         >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

cd /D ..\..

@ECHO.
@ECHO Copy Setups
@ECHO -------------------------------------------------------------------------
@ECHO.                                                                                          >> %LSG_LOGFILE%
@ECHO Copy Setups                                                                               >> %LSG_LOGFILE%
@ECHO -------------------------------------------------------------------------                 >> %LSG_LOGFILE%



echo md I:\MergeModules                                                                 >> nul
  md I:\MergeModules

echo xcopy %SG_SETUP%\*.msm I:\MergeModules\ /sy
echo xcopy %SG_SETUP%\*.msm I:\MergeModules\ /sy        	                                >> %LSG_LOGFILE%
xcopy %SG_SETUP%\*.msm I:\MergeModules\ /sy            		                        >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo xcopy %SG_SETUP%\ATCMControl\Configuration1\Release1\DiskImages\DISK1\*.*      %SG_LATEST%\Setup\ATCMControl\*.*  /S /Y /Q /K /R /E
echo xcopy %SG_SETUP%\ATCMControl\Configuration1\Release1\DiskImages\DISK1\*.*      %SG_LATEST%\Setup\ATCMControl\*.*  /S /Y /Q /K /R /E                  >> %LSG_LOGFILE%
     xcopy %SG_SETUP%\ATCMControl\Configuration1\Release1\DiskImages\DISK1\*.*      %SG_LATEST%\Setup\ATCMControl\*.*  /S /Y /Q /K /R /E          >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error


echo copy %PROJECT_ROOT%\Setup\Docu\PC-Based\Autorun.ico   %SG_LATEST%\Autorun.ico /Y
echo copy %PROJECT_ROOT%\Setup\Docu\PC-Based\Autorun.ico   %SG_LATEST%\Autorun.ico /Y           >> %LSG_LOGFILE%
     copy %PROJECT_ROOT%\Setup\Docu\PC-Based\Autorun.ico   %SG_LATEST%\Autorun.ico /Y   >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo copy %PROJECT_ROOT%\Setup\Docu\PC-Based\Autorun.inf   %SG_LATEST%\Autorun.inf /Y
echo copy %PROJECT_ROOT%\Setup\Docu\PC-Based\Autorun.inf   %SG_LATEST%\Autorun.inf /Y           >> %LSG_LOGFILE%
     copy %PROJECT_ROOT%\Setup\Docu\PC-Based\Autorun.inf   %SG_LATEST%\Autorun.inf /Y   >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo copy %PROJECT_ROOT%\Setup\Setup.exe                   %SG_LATEST%\Setup.exe /Y
echo copy %PROJECT_ROOT%\Setup\Setup.exe                   %SG_LATEST%\Setup.exe /Y             >> %LSG_LOGFILE%
     copy %PROJECT_ROOT%\Setup\Setup.exe                   %SG_LATEST%\Setup.exe /Y     >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo copy %PROJECT_ROOT%\Setup\*.ini                       %SG_LATEST%\*.ini /Y
echo copy %PROJECT_ROOT%\Setup\*.ini                       %SG_LATEST%\*.ini /Y                 >> %LSG_LOGFILE%
     copy %PROJECT_ROOT%\Setup\*.ini                       %SG_LATEST%\*.ini /Y         >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

echo copy %PROJECT_ROOT%\Setup\Setup\License.rtf           %SG_LATEST%\Setup\License.rtf /Y
echo copy %PROJECT_ROOT%\Setup\Setup\License.rtf           %SG_LATEST%\Setup\License.rtf /Y     >> %LSG_LOGFILE%
     copy %PROJECT_ROOT%\Setup\Setup\License.rtf           %SG_LATEST%\Setup\License.rtf /Y >> nul 2>> %LSG_LOGFILE%
if errorlevel 1 goto error

@del /s /F I:\*.obj                                                                      >> nul 2>>nul
@del /s /F I:\*.res                                                                      >> nul 2>>nul
@del /s /F I:\*.sbr                                                                      >> nul 2>>nul
@del /s /F I:\vc60.pdb                                                                   >> nul 2>>nul

goto end

                                                                       
:error
echo.
echo ERROR on CreateSetup. See Log file for more informations.
echo -------------------------------------------------------------------------
echo.
exit /B 1
goto end

:end
subst I: /d >> nul 2>>nul
subst K: /d >> nul 2>>nul
endlocal
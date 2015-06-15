@echo off
if "%STINGRAY_DIR%" == "" goto stingray_root_missing
if "%PROJECT_ROOT%" == "" goto project_root_missing

echo copy stringray pdb files to %STINGRAY_DIR%\ObjectiveStudio\Lib to %PROJECT_ROOT%\COM\softing\fc\CE\lib\Stingray
xcopy /Q /C /R /S /Y %STINGRAY_DIR%\ObjectiveStudio\Lib\ot701*.pdb %PROJECT_ROOT%\COM\softing\fc\CE\lib\Stingray\*.*
xcopy /Q /C /R /S /Y %STINGRAY_DIR%\ObjectiveStudio\Lib\sfl103*.pdb %PROJECT_ROOT%\COM\softing\fc\CE\lib\Stingray\*.*

goto finish

:stingray_root_missing
echo error: your stingray root system variable (STINGRAY_DIR) is not set! -> stop.
goto finish

:project_root_missing
echo error: your project root system variable (PROJECT_ROOT) is not set! -> stop.

:finish
echo done.
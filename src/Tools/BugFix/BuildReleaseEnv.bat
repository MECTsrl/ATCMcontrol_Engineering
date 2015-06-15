@echo off
if "%PROJECT_ROOT%" == "" goto error

set GENBASE=I:\4Control_R

if exist I:\GenKernel.log goto aisokay

REM echo set build number
REM call buildnr.bat

REM echo net use I: /DELETE
REM net use I: /DELETE
REM echo net use I: \\S21\4Control\SysgenV2\Build%BUILDNUMBER%
REM net use I: \\S21\4Control\SysgenV2\Build%BUILDNUMBER%

if not exist I:\fullgen.err goto error
if not exist I:\genall.log goto error
echo I: not available or incomplete build, stop.
goto finish

:error
echo log, err file not exist, stop.
goto finish


:aisokay
call Batch\stopservices.bat

echo copy console
xcopy /Q /C /R /S /Y %GENBASE%\Console\*.* %PROJECT_ROOT%\Console\*.*
echo copy control
xcopy /Q /C /R /S /Y %GENBASE%\Control\*.* %PROJECT_ROOT%\Control\*.*
echo copy engineering
xcopy /Q /C /R /S /Y %GENBASE%\Engineering\bin\*.* %PROJECT_ROOT%\Engineering\bin\*.*
echo copy target base
xcopy /Q /C /R /S /Y %GENBASE%\TargetBase\Kernel\*.* %TARGET_BASE%\Kernel\*.*
echo copy target base libs
xcopy /Q /C /R /S /Y %GENBASE%\TargetBase\lib\*.* %TARGET_BASE%\lib\*.*
echo copy target base includes
xcopy /Q /C /R /S /Y %GENBASE%\TargetBase\inc\*.* %TARGET_BASE%\inc\*.*
echo copy gentools
xcopy /Q /C /R /S /Y %GENBASE%\gentools\*.* %PROJECT_ROOT%\gentools\*.*
echo copy includes
xcopy /Q /C /R /S /Y %GENBASE%\COM\softing\fc\inc\*.* %PROJECT_ROOT%\COM\softing\fc\inc\*.*
xcopy /Q /C /R /S /Y %GENBASE%\COM\softing\fc\res\*.* %PROJECT_ROOT%\COM\softing\fc\res\*.*
xcopy /Q /C /R /S /Y %GENBASE%\COM\softing\fc\CE\inc\*.* %PROJECT_ROOT%\COM\softing\fc\CE\inc\*.*
xcopy /Q /C /R /S /Y %GENBASE%\COM\softing\fc\CE\res\*.* %PROJECT_ROOT%\COM\softing\fc\CE\res\*.*
xcopy /Q /C /R /S /Y %GENBASE%\COM\softing\fc\EXT\*.* %PROJECT_ROOT%\COM\softing\fc\EXT\*.*
echo copy libraries
xcopy /Q /C /R /S /Y %GENBASE%\COM\softing\fc\lib\*.* %PROJECT_ROOT%\COM\softing\fc\lib\*.*
xcopy /Q /C /R /S /Y %GENBASE%\COM\softing\fc\CE\lib\*.* %PROJECT_ROOT%\COM\softing\fc\CE\lib\*.*
echo copy fc-dlls
xcopy /Q /C /R /S /Y %GENBASE%\Pub\fc_*.dll %windir%\system32\*.*
echo copy opcengine.dll
xcopy /Q /C /R /S /Y %GENBASE%\Pub\opcengine.* %windir%\system32\*.*

REM echo get sources from source safe
REM call Batch\getsourcesfromss.bat

call Batch\copypdb.bat
call Batch\startservices.bat
echo done.

:finish

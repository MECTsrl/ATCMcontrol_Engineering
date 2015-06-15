@ECHO OFF

setlocal

set CWD=%~d0%~p0 
cd /D %CWD%

if not "%CD%"=="C:\ATCMControl_SDK" goto directoryerror

SET ERROR_VAR=
SET CONTAINS_LIB=
set PROJECT_ROOT=%CD%\GenWork\GenDir.tmp\4Control_R


cd LIBS
for /d %%d in (*) do call :apply_lib %%d
if "%ERROR_VAR%"=="1" goto end
if not "%CONTAINS_LIB%"=="1" goto nolibfound
goto perform_build


:apply_lib
if exist %PROJECT_ROOT%\TargetBase\Libs\%1 goto libexisterror
SET CONTAINS_LIB=1
md %PROJECT_ROOT%\TargetBase\Libs\%1

REM Create blank dsp file
@echo. > %1\%1.dsp

REM Create inf file to generate mak
@echo. > %1\%1.inf
@echo USER=libname=%1 >> %1\%1.inf
@echo USER=make_lib: force >> %1\%1.inf
@echo USER= $(4CG) $(MAKEDIR)\$(libname).4cp  -ml:0 $(4CL_DIR)\$(libname).4cl >> %1\%1.inf
@echo. >> %1\%1.inf

REM Create build files in Target FCTPAC020_01
md %PROJECT_ROOT%\Target\FCTPAC020_01\Libs\%1
@echo. > %PROJECT_ROOT%\Target\FCTPAC020_01\Libs\%1\%1.dsp
@echo. > %PROJECT_ROOT%\Target\FCTPAC020_01\Libs\%1\%1.inf

REM Fill inf file with properly values
@echo USER=libname=%1 >> %PROJECT_ROOT%\Target\FCTPAC020_01\Libs\%1\%1.inf
@echo USER=targettype=FCTPAC020_01 >> %PROJECT_ROOT%\Target\FCTPAC020_01\Libs\%1\%1.inf
@echo USER=makefirst: force >> %PROJECT_ROOT%\Target\FCTPAC020_01\Libs\%1\%1.inf
@echo USER= xcopy $(TARGET_BASE)\Libs\$(libname)\*.st   $(MAKEDIR)\*.st   /Y >> %PROJECT_ROOT%\Target\FCTPAC020_01\Libs\%1\%1.inf
@echo USER= xcopy $(TARGET_BASE)\Libs\$(libname)\*.cst  $(MAKEDIR)\*.cst  /Y >> %PROJECT_ROOT%\Target\FCTPAC020_01\Libs\%1\%1.inf
@echo USER= xcopy $(TARGET_BASE)\Libs\$(libname)\*.sdt  $(MAKEDIR)\*.sdt  /Y >> %PROJECT_ROOT%\Target\FCTPAC020_01\Libs\%1\%1.inf
@echo USER= xcopy $(TARGET_BASE)\Libs\$(libname)\*.htm*  $(MAKEDIR)\*.htm*  /Y >> %PROJECT_ROOT%\Target\FCTPAC020_01\Libs\%1\%1.inf
@echo USER= copy  $(TARGET_BASE)\Libs\$(libname)\FunFbId.txt     $(MAKEDIR)\FunFbId.txt     /Y >> %PROJECT_ROOT%\Target\FCTPAC020_01\Libs\%1\%1.inf
@echo USER=make_lib: force >> %PROJECT_ROOT%\Target\FCTPAC020_01\Libs\%1\%1.inf
@echo USER= $(4CG) $(MAKEDIR)\$(libname).4cp  -ml:$(targettype) $(ENGBIN_DIR)\$(targettype)\libs\$(libname).$(targettype).4cl >> %PROJECT_ROOT%\Target\FCTPAC020_01\Libs\%1\%1.inf
@echo USER= $(4CG) -al $(ENGBIN_DIR)\$(targettype)\libs\$(libname).$(targettype).4cl  $(4CL_DIR)\$(libname).4cl >> %PROJECT_ROOT%\Target\FCTPAC020_01\Libs\%1\%1.inf
@echo. >> %PROJECT_ROOT%\Target\FCTPAC020_01\Libs\%1\%1.inf

REM Copy 4cp file to FCTPAC020_01 target
xcopy %1\%1.4cp %PROJECT_ROOT%\Target\FCTPAC020_01\Libs\%1\

REM Add actual lib to TARGET libs make
@echo. >> %PROJECT_ROOT%\Target\FCTPAC020_01\Libs\Libs.inf
@echo SUBPROJECT=%1\%1.mak >> %PROJECT_ROOT%\Target\FCTPAC020_01\Libs\Libs.inf
@echo USER= cd $(MAKEDIR)\%1 >> %PROJECT_ROOT%\Target\FCTPAC020_01\Libs\Libs.inf
@echo USER= $(MAKE) -$(MAKEFLAGS) -f %1.mak     make_lib DEBUG=$(DEBUG) >> %PROJECT_ROOT%\Target\FCTPAC020_01\Libs\Libs.inf

REM Copy all files To make build
xcopy %1 %PROJECT_ROOT%\TargetBase\Libs\%1

REM Add actual lib to general libs make
@echo. >> %PROJECT_ROOT%\TargetBase\Libs\Libs.inf
@echo SUBPROJECT=%1\%1.mak >> %PROJECT_ROOT%\TargetBase\Libs\Libs.inf
@echo USER= cd $(MAKEDIR)\%1 >> %PROJECT_ROOT%\TargetBase\Libs\Libs.inf
@echo USER= $(MAKE) -$(MAKEFLAGS) -f %1.mak     make_lib DEBUG=$(DEBUG) >> %PROJECT_ROOT%\TargetBase\Libs\Libs.inf
goto end

:prebuilderror
echo.
echo ERROR. GenKernel for complete build should run first once.
echo -------------------------------------------------------------------------
echo.
goto end

:libexisterror
set ERROR_VAR=1
echo.
echo WARNING. Some libs were not integrated.
echo -------------------------------------------------------------------------
echo.
goto end

:nolibfound
echo.
echo ERROR. There are no libs in LIBS path.
echo -------------------------------------------------------------------------
echo.
cd ..
exit /B 1
goto end

:perform_build
cd ..
echo Building New Libs
@call checkinstallation
@call gentargetbase
if errorlevel 1 goto end
@call GenTarget_FCTPAC020_01
if errorlevel 1 goto end
goto success

:directoryerror
echo.
echo ERROR. GenLib cannot be called from repository folder. Call it from SDK
echo folder instead.
echo -------------------------------------------------------------------------
echo.
exit /B 1
goto end

:success
echo.
echo SUCCESS on generation. Libs integrated with success.
echo -------------------------------------------------------------------------
goto end

:end
endlocal
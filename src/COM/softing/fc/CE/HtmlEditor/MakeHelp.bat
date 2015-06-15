@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by FCHTMLED.HPJ. >"hlp\FcHtmlEd.hm"
echo. >>"hlp\FcHtmlEd.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\FcHtmlEd.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\FcHtmlEd.hm"
echo. >>"hlp\FcHtmlEd.hm"
echo // Prompts (IDP_*) >>"hlp\FcHtmlEd.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\FcHtmlEd.hm"
echo. >>"hlp\FcHtmlEd.hm"
echo // Resources (IDR_*) >>"hlp\FcHtmlEd.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\FcHtmlEd.hm"
echo. >>"hlp\FcHtmlEd.hm"
echo // Dialogs (IDD_*) >>"hlp\FcHtmlEd.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\FcHtmlEd.hm"
echo. >>"hlp\FcHtmlEd.hm"
echo // Frame Controls (IDW_*) >>"hlp\FcHtmlEd.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\FcHtmlEd.hm"
REM -- Make help for Project FCHTMLED


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\FcHtmlEd.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\FcHtmlEd.hlp" goto :Error
if not exist "hlp\FcHtmlEd.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\FcHtmlEd.hlp" Debug
if exist Debug\nul copy "hlp\FcHtmlEd.cnt" Debug
if exist Release\nul copy "hlp\FcHtmlEd.hlp" Release
if exist Release\nul copy "hlp\FcHtmlEd.cnt" Release
echo.
goto :done

:Error
echo hlp\FcHtmlEd.hpj(1) : error: Problem encountered creating help file

:done
echo.

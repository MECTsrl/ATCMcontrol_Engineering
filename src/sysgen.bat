@ECHO OFF
REM possible parameters: sysgen [clean] ["DEBUG=0"]
REM
REM set NO_MAKE_xxx=0 to reverse default settings
REM
REM ===============================================
REM $Archive: /4CReleased/V2.20.00/sysgen.bat $ $Author: Ln $
REM $Header: /4CReleased/V2.20.00/sysgen.bat 1     28.02.07 18:51 Ln $
REM $Date: 28.02.07 18:51 $ $Modtime: 30.08.04 21:41 $
REM $Revision: 1 $ $Workfile: sysgen.bat $
REM ===============================================

SETLOCAL
IF "%NO_MAKE_BROWSE%" == "" SET NO_MAKE_BROWSE=1
IF "%NO_MAKE_DEPEND%" == "" SET NO_MAKE_DEPEND=1
IF "%NO_MAKE_QUICK%" == "" SET NO_MAKE_QUICK=1
perl gentools\make.pl 4Control
nmake -nologo -i -f 4Control.mak makemake %1 %2 %3 all 2>&1 | perl gentools\logfile.pl build.log
ENDLOCAL
find /I "warning " build.log | perl gentools\logfile.pl build.wrn
find /I "error " build.log | perl gentools\logfile.pl build.err

# $Header: /4CReleased/V2.20.00/GenTools/global.inc 3     24.02.08 18:18 Ln $
# ------------------------------------------------------------------------------
#
# =FILENAME             $Workfile: global.inc $
#                        $Logfile: /4CReleased/V2.20.00/GenTools/global.inc $
#
# =PROJECT              ATCMControl V2.x
#
# =SWKE                 GenTools
#
# =COMPONENT            GenTools
#
# =CURRENT      $Date: 24.02.08 18:18 $
#           $Revision: 3 $
#
# ------------------------------------------------------------------------------
#

.SUFFIXES:

all: premake buildall postmake
buildall: makefirst oleclasses javaclasses precompile compile prelink link makelast
premake:
postmake:

makefirst:                    # first build-target (for USER-Statements)
oleclasses:                   # midl, mktyplib
javaclasses:                  # jvc
precompile:                   # copy, yacc, lex, msjavah
compile:                      # cl, rc
prelink:                      # lib
link:                         # link
makelast:                     # last build-target (for USER-Statements)
doxygen:		              # generate documentation
makemake:                     # recursive make
force:                        # force unconditional execution of rules
clean::                       # clean-up

SUB           	= 1
DEBUG         	= 1
EXE_DIR       	= $(PROJECT_ROOT)
ENGBIN_DIR    	= $(PROJECT_ROOT)\Engineering\bin
BUILT_LIB_DIR	= $(PROJECT_ROOT)\Engineering\Lib
CLASSES_ROOT  	= $(PROJECT_ROOT)\Control\classes
INCLUDE_DIR   	= $(PROJECT_ROOT)\COM\softing\fc\inc
RESOURCE_DIR  	= $(PROJECT_ROOT)\COM\softing\fc\res
LIB_DIR       	= $(PROJECT_ROOT)\COM\softing\fc\lib
SYSTEM_LIBS_DIR = $(PROJECT_ROOT)\COM\softing\fc\CL
CC            	= cl -nologo
CXX           	= cl -nologo
RC            	= rc


#setting for 4cl library generation make_lib build step:
4CG           = $(EXE_DIR)\Engineering\bin\4cg -nologo
4CL_DIR       = $(EXE_DIR)\Engineering\lib



!if $(STATIC_RUNTIME)
R=T
!else
R=D
!endif

!if $(DEBUG)
CFLAGS        = -W3 -Od -Zi -GR -GX -M$Rd -DDEBUG -DWIN32 -D_WIN32_WINNT=0x0400 -FI$(PROJECT_ROOT)\gentools\globprag.h
CXXFLAGS      = -W3 -Od -Zi -GR -GX -M$Rd -DDEBUG -DWIN32 -D_WIN32_WINNT=0x0400 -FI$(PROJECT_ROOT)\gentools\globprag.h
RCFLAGS       = -r -d DEBUG
!else
CFLAGS        = -W3 -Os -Zi -GR -GX -M$R -DNDEBUG -DWIN32 -D_WIN32_WINNT=0x0400 -FI$(PROJECT_ROOT)\gentools\globprag.h
CXXFLAGS      = -W3 -Os -Zi -GR -GX -M$R -DNDEBUG -DWIN32 -D_WIN32_WINNT=0x0400 -FI$(PROJECT_ROOT)\gentools\globprag.h
RCFLAGS       = -r -d NDEBUG
!endif

LNK           = link -nologo
!if $(DEBUG)
LFLAGS        = -DEBUG -FIXED:NO
!else
LFLAGS        = -DEBUG
!endif
AR            = lib -nologo
BSCMAKE       = bscmake -nologo
JAVAC         = jvc -nologo
JFLAGS        = -g -w4 -d $(CLASSES_ROOT) -cp $(CLASSES_ROOT);$(PROJECT_ROOT);$(WINDIR)\Java\Classes\classes.zip
JAVAH         = msjavah
JAVAHFLAGS    = -classpath $(CLASSES_ROOT) -helpers -ctors -nowritebarrier -noclinit
JACTIVEX      = jactivex
JACTIVEXFLAGS = -javatlb -e -d $(CLASSES_ROOT)
LEX           = lex
LEXFLAGS      = -alTv -LC
YACC          = yacc
YACCFLAGS     = -lmv -LC
MIDL          = midl
MIDLFLAGS     = -Os
MKTYPLIB      = mktyplib
MKTYPLIBFLAGS =
MC            = mc
MCFLAGS       =
RM            = del
CP            = copy
CAT           = type

!if $(DEBUG)
OBJ           = objd
!else
OBJ           = objp
!endif

$(OBJ):
        - md $@


!ifdef OFFICIAL_BUILD
CFLAGS        = $(CFLAGS) -D_OFFICIAL_BUILD
CXXFLAGS      = $(CXXFLAGS) -D_OFFICIAL_BUILD
RCFLAGS       = $(RCFLAGS) -d _OFFICIAL_BUILD
!endif

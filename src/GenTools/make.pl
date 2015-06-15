#H>> $Header: /4CReleased/V2.20.00/GENTOOLS/make.pl 1     28.02.07 18:53 Ln $
#----------------------------------------------------------------------------*
#
# =FILENAME            $Workfile: make.pl $
#                      $Logfile: /4CReleased/V2.20.00/GENTOOLS/make.pl $
#
# =PROJECT             CAK1020  ATCMControl V2.0
#
# =SWKE                GENTOOLS
#
# =COMPONENT           GENTOOLS
#
# =CURRENT             $Date: 28.02.07 18:53 $
#                      $Revision: 1 $
#
# =ENVIRONMENT         MS Windows NT 4.0 / MS Windows 2000
#                      MSVC++ V6.0
#                      MS Java 3.2
#                      MS Visual SourceSafe 6.0
#                      Perl5
#
# =REFERENCES
#==
#----------------------------------------------------------------------------*
#





#==
#
#----------------------------------------------------------------------------*
# =DESCRIPTION
#== ATCMControl makefile generator, usage:  perl make.pl <componentname>
#----------------------------------------------------------------------------*
# =MODIFICATIONS
# $History: make.pl $
# 
# *****************  Version 1  *****************
# User: Ln           Date: 28.02.07   Time: 18:53
# Created in $/4CReleased/V2.20.00/GENTOOLS
# 
# *****************  Version 3  *****************
# User: Wi           Date: 10.09.03   Time: 9:10
# Updated in $/4Control/GenTools
# error fixed (unicode dlls)
# 
# *****************  Version 2  *****************
# User: Wi           Date: 9.09.03    Time: 10:11
# Updated in $/4Control/GenTools
# adaptation for unicode gui applications (used for 4CAlmEvtTrd, no bad
# effect for  other builds)
# 
# *****************  Version 1  *****************
# User: Ln           Date: 28.08.03   Time: 16:34
# Created in $/4Control/GENTOOLS
# 
# *****************  Version 6  *****************
# User: Ed           Date: 5/16/01    Time: 1:40p
# Updated in $/4Control/gentools
# add error keyword to all error messages
# 
# *****************  Version 5  *****************
# User: Jd           Date: 30.04.01   Time: 14:26
# Updated in $/4Control/gentools
# Added doxygen support in make.pl
# 
# *****************  Version 4  *****************
# User: Ef           Date: 15.02.01   Time: 15:03
# Updated in $/4Control/gentools
# allow ommitting the component's name and use the current working dir
# name instead
# 
# *****************  Version 3  *****************
# User: Ef           Date: 13.12.00   Time: 18:43
# Updated in $/4Control/gentools
# added too many param error message (used make.pl *.inf and had two
# inf files in the directory, hard to find bug!)
# 
# *****************  Version 2  *****************
# User: Ef           Date: 12.12.00   Time: 15:55
# Updated in $/4Control/gentools
# * to # from vss comment, and some changes:
# - check that PROJECT_ROOT is set correctly (no default anymore)
# - added minimnal command line help 
# -  the command line parameter can be now <compname>.inf too
# 
# *****************  Version 1  *****************
# User: Ef           Date: 11.12.00   Time: 18:14
# Created in $/4Control/gentools
# gentools: taken from V1.3, new file headers
#==
#*****************************************************************************
#H<<


$VERSION = '$Revision: 1 $';
$|=1; # flush stdout if redirected...
print "$0: Version=$VERSION\n";

$make_depend=1;
$make_depend=0 if defined $ENV{NO_MAKE_DEPEND} and $ENV{NO_MAKE_DEPEND};
$make_browse=1;
$make_browse=0 if defined $ENV{NO_MAKE_BROWSE} and $ENV{NO_MAKE_BROWSE};
$make_quick=1;
$make_quick=0 if defined $ENV{NO_MAKE_QUICK} and $ENV{NO_MAKE_QUICK};

$isdll=0;         # from .inf: TARGETTYPE=DLL|EXE
$islib=0;         # from .inf: TARGETTYPE=LIB
$isgui=1;         # from .inf: TARGETTYPE=GUI|CON
$isunicode=0;     # from .inf: UNICODE=1|ON|0|OFF
$odl_to_h=0;      # from .inf: ODL_TO_H=1|ON|0|OFF
$odl_to_java=0;   # from .inf: ODL_TO_JAVA=1|ON|0|OFF
$yl_cg_style=0;   # from .inf: YACC_LEX_CG_STYLE=0|1|ON|OFF
$idl_to_java=0;   # from .inf: IDL_TO_JAVA=1|ON|0|OFF
$target="";       # from .inf: TARGET=x
$rawtarget="";    # from .inf: $target minus extension
$implib="";       # from .inf: IMPLIB=x.lib
$deffile="";      # from .dsp: SOURCE=x.def
@cppsources=();   # from .dsp: SOURCE=x.cpp
@csources=();     # from .dsp: SOURCE=x.c
@ysources=();     # from .dsp: SOURCE=x.y
@lsources=();     # from .dsp: SOURCE=x.l
$rcsource="";     # from .dsp: SOURCE=x.rc
$use_master_rc=0; # from .inf: MASTER_RC=x.rc
@mcsources=();    # from .dsp: SOURCE=x.mc
@odlsources=();   # from .dsp: SOURCE=x.odl
@idlsources=();   # from .dsp: SOURCE=x.idl
@javasources=();  # from .dsp: SOURCE=x.java
@libraries=();    # from .dsp: SOURCE=x.lib or .inf LIBRARY=x.lib
@objsources=();   # from .dsp: SOURCE=x.obj
@sbrsources=();
@deflibs=(        # from .inf: DEFLIB=x.lib
    "kernel32",
    "user32",
    "gdi32",
    "advapi32",
);
@defobjs=();      # from .inf: OBJECT=x.obj
@incpath=();      # from .inf: INCLUDE=x
@defines=();      # from .inf: DEFINE=x
@libpath=();      # from .inf: LIBPATH=x
@copyfrom=();     # from .inf: COPY=source target
@copyto=();
@latecopyfrom=(); # from .inf: LATECOPY=source target
@latecopyto=();
$pchfile="";      # from .inf: PCHFILE=x (w/o .h .cpp or .pch)
$package="";      # from .inf: PACKAGE=x (like COM.softing.fc.CC.xxx)
$package_dir="";  # from like package with '\' instead of '.'
@subprojects=();  # from .inf: SUBPROJECT=x.mak
@usercmds=();     # from .inf: USER=x
@javahcmds=();    # from .inf: JAVAH=x
$static_runtime=0;# from .inf: STATIC_RUNTIME=1|ON|0|OFF
$parent="";       # from .inf: PARENT=x.mak
$component="";    # the input component name without any extension from commandline param



#check command line parameter:
if($ARGV[0] eq "")
{
   chomp($cwd = `cd`);     # is there any other way to learn my current working directory???
   $component = getname($cwd);
}
else
{
   if( $ARGV[1] ne "" or 
     (getExt($ARGV[0]) ne "" and getExt($ARGV[0]) ne "inf") or
       $ARGV[0]=~m{^[-/]}
     )
   {
       print "command line error:\n" if($ARGV[0] ne "-?" && $ARGV[0] ne "/?");
       print "too many parameters\n" if($ARGV[1] ne "");
       print "\n";
       print "\n Usage: make.pl [<component>[.inf]]\n";
       print "\n        make.pl -?   for help\n";
       print "\n if <component> name is ommitted the current direcorty name is assumed\n";
       print " make.pl create make file from the <component>.inf and <component>.dsp.\n";
       die   " The current working directory must be the components directory\n";
   }
   $component = getFileNameNoExt($ARGV[0]);
}

#check environment:
die "make.pl: error: environment variable PROJECT_ROOT not defined\n" if(!defined $ENV{PROJECT_ROOT});
if(!-e "$ENV{PROJECT_ROOT}\\gentools\\global.inc")
{
    die "make.pl: error: '$ENV{PROJECT_ROOT}\\gentools\\global.inc' does not exist (PROJECT_ROOT correct?)\n";
}


open (INF, "< $component.inf") or die "make.pl: error: can't open $component.inf in the folder " . $cwd;
while (<INF>) {
    chomp;
    s/[ \t]+$//;
    # $_ = lc $_;
    if (/^TARGET=(.+)$/i) {
      print "$component.inf($.) : error : more than one TARGET\n" if $target ne "";
      $target=$1;
      $rawtarget = $target;
      $rawtarget =~ s!\.[^\\/.]+$!!;
    }
    elsif (/^IMPLIB=(.+)\.lib$/i) {
      print "$component.inf($.) : error : more than one IMPLIB\n" if $implib ne "";
      $implib = $1;
      $isdll=1;
      $islib=0;
    }
    elsif (/^TARGETTYPE=DLL$/i) {
      $isdll=1;
      $islib=0;
    }
    elsif (/^TARGETTYPE=EXE$/i) {
      $isdll=0;
      $islib=0;
    }
    elsif (/^TARGETTYPE=LIB$/i) {
      $isdll=0;
      $islib=1;
    }
    elsif (/^TARGETTYPE=GUI$/i) {
      $isgui=1;
    }
    elsif (/^TARGETTYPE=CON$/i) {
      $isgui=0;
    }
    elsif (/^UNICODE=(1|ON)$/i) {
      $isunicode=1;
    }
    elsif (/^UNICODE=(0|OFF)$/i) {
      $isunicode=0;
    }
    elsif (/^YACC_LEX_CG_STYLE=(1|ON)$/i) {
      $yl_cg_style=1;
    }
    elsif (/^YACC_LEX_CG_STYLE=(0|OFF)$/i) {
      $yl_cg_style=0;
    }
    elsif (/^ODL_TO_H=(1|ON)$/i) {
      $odl_to_h=1;
    }
    elsif (/^ODL_TO_H=(0|OFF)$/i) {
      $odl_to_h=0;
    }
    elsif (/^ODL_TO_JAVA=(1|ON)$/i) {
      $odl_to_java=1;
    }
    elsif (/^ODL_TO_JAVA=(0|OFF)$/i) {
      $odl_to_java=0;
    }
    elsif (/^IDL_TO_JAVA=(1|ON)$/i) {
      $idl_to_java=1;
    }
    elsif (/^IDL_TO_JAVA=(0|OFF)$/i) {
      $idl_to_java=0;
    }
    elsif (/^STATIC_RUNTIME=(1|ON)$/i) {
      $static_runtime=1;
    }
    elsif (/^STATIC_RUNTIME=(0|OFF)$/i) {
      $static_runtime=0;
    }
    elsif (/^INCLUDE=(.+)$/i) {
      $incpath[$#incpath+1] = $1;
    }
    elsif (/^DEFINE=(.+)$/i) {
      $defines[$#defines+1] = $1;
    }
    elsif (/^LIBPATH=(.+)$/i) {
      $libpath[$#libpath+1] = $1;
    }
    elsif (/^DEFLIB=(.+)\.lib$/i) {
      $deflibs[$#deflibs+1] = $1;
    }
    elsif (/^LIBRARY=(.+)\.lib$/i) {
      $libraries[$#libraries+1] = $1;
    }
    elsif (/^OBJECT=(.+)\.obj$/i) {
      $defobjs[$#defobjs+1] = $1;
    }
    elsif (/^MASTER_RC=(.+)\.rc$/i) {
      print "$component.inf($.) : error : more than one .rc file\n" if $rcsource ne "";
      $rcsource = $1;
      $use_master_rc = 1;
    }
    elsif (/^COPY=([^ \t]+)[ \t]+([^ \t]+)$/i) {
      $i = $#copyfrom+1;
      $copyfrom[$i] = $1;
      $copyto[$i] = $2;
    }
    elsif (/^LATECOPY=([^ \t]+)[ \t]+([^ \t]+)$/i) {
      $i = $#latecopyfrom+1;
      $latecopyfrom[$i] = $1;
      $latecopyto[$i] = $2;
    }
    elsif (/^PCHFILE=(.+)$/i) {
      print "$component.inf($.) : error : more than one PCHFILE\n" if $pchfile ne "";
      $pchfile = $1;
    }
    elsif (/^PACKAGE=(.+)$/i) {
      print "$component.inf($.) : error : more than one PACKAGE\n" if $package ne "";
      $package = $1;
      $package_dir = $package;
      $package_dir =~ s/\./\\/g;
    }
    elsif (/^JAVAH=(.+)$/i) {
      $javahcmds[$#javahcmds+1] = $1;
    }
    elsif (/^SUBPROJECT=(.+)\.mak$/i) {
      $subprojects[$#subprojects+1] = $1;
    }
    elsif (/^PARENT=(.+)\.mak$/i) {
      print "$component.inf($.) : error : more than one PARENT\n" if $parent ne "";
      $parent = $1;
    }
    elsif (/^USER=(.+)$/i) {
      $usercmds[$#usercmds+1] = $1;
    }
    elsif (/^#/ or $_ eq "") {
      # comment - ignored
    }
    else {
      print "$component.inf($.) : error : unknown command\n";
    }
}
close INF;

open (INF, "< $component.dsp") or warn "make.pl: warning: can't open $component.dsp";
while (<INF>) {
    chomp;
    s/[ \t]+$//;
    # $_ = lc $_;
    if (/^SOURCE=(\.\\)?(.+)\.cpp$/i) {
      $cppsources[$#cppsources+1] = $2;
    }
    elsif (/^SOURCE=(\.\\)?(.+)\.c$/i) {
      $csources[$#csources+1] = $2;
    }
    elsif (/^SOURCE=(\.\\)?(.+)\.y$/i) {
      $ysources[$#ysources+1] = $2;
    }
    elsif (/^SOURCE=(\.\\)?(.+)\.l$/i) {
      $lsources[$#lsources+1] = $2;
    }
    elsif (!$use_master_rc && /^SOURCE=(\.\\)?(.+)\.rc$/i) {
      print "$component.dsp($.) : error : more than one .rc file\n" if $rcsource ne "";
      $rcsource = $2;
    }
    elsif (/^SOURCE=(\.\\)?(.+)\.mc$/i) {
      $mcsources[$#mcsources+1] = $2;
    }
    elsif (/^SOURCE=(\.\\)?(.+)\.odl$/i) {
      $odlsources[$#odlsources+1] = $2;
    }
    elsif (/^SOURCE=(\.\\)?(.+)\.idl$/i) {
      $idlsources[$#idlsources+1] = $2;
    }
    elsif (/^SOURCE=(\.\\)?(.+)\.java$/i) {
      $javasources[$#javasources+1] = $2;
    }
    elsif (/^SOURCE=(\.\\)?(.+)\.lib$/i) {
      $libraries[$#libraries+1] = $2;
    }
    elsif (/^SOURCE=(\.\\)?(.+)\.obj$/i) {
      $objsources[$#objsources+1] = $2;
    }
    elsif (/^SOURCE=(\.\\)?(.+)\.def$/i) {
      print "$component.dsp($.) : error : more than one .def file\n" if $deffile ne "";
      $deffile = $2;
    }
}
close INF;

open (MAK, "> $component.mak") or die "make.pl: error: can't open $component.mak";

print MAK "#automatically generated by $0: Version=$VERSION\n\n";

print MAK "!ifndef PROJECT_ROOT\n";
print MAK "!MESSAGE check environmemt: environment variable PROJECT_ROOT not defined\n" ;
print MAK "!ERROR PROJECT_ROOT must point to the project base directory\n";
print MAK "!endif\n\n";

print MAK "STATIC_RUNTIME = $static_runtime\n";
print MAK "!include <\$(PROJECT_ROOT)\\gentools\\global.inc>\n";

foreach $t (@usercmds) {
  print MAK "$t\n";
}

if ($make_quick) {
  print MAK "premake: $component.mak\n";
  print MAK "$component.mak: $component.dsp $component.inf\n";
  print MAK "\tperl \$(PROJECT_ROOT)\\gentools\\make.pl $component\n";
  print MAK "\t\$(MAKE) -\$(MAKEFLAGS) -f $component.mak buildall SUB=\$(SUB) DEBUG=\$(DEBUG)\n";

  if ($parent ne "") {
    my $i = getpath($parent);
    my $n = getname($parent);
    print MAK "postmake: $parent.mak\n";
    print MAK "$parent.mak: force\n";
    print MAK "\tcd $i\n" if $i ne ".";
    print MAK "\t\$(MAKE) -\$(MAKEFLAGS) -f $n.mak buildall SUB=0 DEBUG=\$(DEBUG)\n";
    print MAK "\tcd \$(MAKEDIR)\n" if $i ne ".";
  }
}

if (scalar @subprojects != 0) {
  print MAK "!if \$(SUB)!=0\n";
  print MAK "makemake:\n";
  foreach $t (@subprojects) {
    my $i = getpath($t);
    my $n = getname($t);
    print MAK "\tcd $i\n" if $i ne ".";
    print MAK "\tperl \$(PROJECT_ROOT)\\gentools\\make.pl $n\n";
    print MAK "\t\$(MAKE) -\$(MAKEFLAGS) -f $n.mak makemake SUB=1\n";
    print MAK "\tcd \$(MAKEDIR)\n" if $i ne ".";
  }
  print MAK "makefirst oleclasses javaclasses precompile compile prelink link makelast:\n";
  foreach $t (@subprojects) {
    my $i = getpath($t);
    my $n = getname($t);
    print MAK "\tcd $i\n" if $i ne ".";
    print MAK "\t\$(MAKE) -\$(MAKEFLAGS) -f $n.mak \$@ SUB=1 DEBUG=\$(DEBUG)\n";
    print MAK "\tcd \$(MAKEDIR)\n" if $i ne ".";
  }
  print MAK "clean::\n";
  foreach $t (@subprojects) {
    my $i = getpath($t);
    my $n = getname($t);
    print MAK "\tcd $i\n" if $i ne ".";
    print MAK "\t\$(MAKE) -\$(MAKEFLAGS) -f $n.mak clean SUB=1 DEBUG=\$(DEBUG)\n";
    print MAK "\tcd \$(MAKEDIR)\n" if $i ne ".";
  }
  print MAK "!endif\n";
}

# doxygen
print MAK "doxygen: $component.doxy\n";
print MAK "\t\$(CP) \$(PROJECT_ROOT)\\gentools\\4C_doxy.css .\\4C_doxy.css\n";
print MAK "\tdoxygen $component.doxy\n";
print MAK "$component.doxy:\n";
print MAK "\t\$(CP) \$(PROJECT_ROOT)\\gentools\\Sample.doxy .\\$component.doxy\n";


# doxygen: CEProjMan.doxy
#	$(CP) $(PROJECT_ROOT)\gentools\4C_doxy.css  .\4C_doxy.css
#	doxygen CEProjMan.doxy
# CEProjMan.doxy:
#	$(CP) $(PROJECT_ROOT)\gentools\Sample.doxy  .\CEProjMan.doxy



foreach $t (@idlsources){
  my $i;
  my $n = getname($t);
  print MAK "oleclasses: $n.h\n"; # force this before compile...
  print MAK "$n.tlb: $n.h\n" if $idl_to_java; # just a guess...
  print MAK "$n.h: $t.idl\n"; # I don't know for sure if midl produces $t.tlb or $t_[cspi].c
  print MAK "\t\$(MIDL) \$(MIDLFLAGS)";
  foreach $i (@incpath) {
    print MAK " -I $i";
  }
  foreach $i (@defines) {
    print MAK " -D $i";
  }
  print MAK " -h $n.h $t.idl\n";
  if ($idl_to_java) {
    print MAK "\t\$(JACTIVEX) \$(JACTIVEXFLAGS) -p $package $n.tlb\n";
  }
  print MAK "clean::\n";
  print MAK "\t-\$(RM) $n.h\n";
}

foreach $t (@odlsources){
  my $i;
  my $n = getname($t);
  print MAK "oleclasses: $n.tlb\n"; # force this before compile...
  print MAK "$n.h: $n.tlb\n" if $odl_to_h; # prevent nmake from calling the following rule twice...
  print MAK "$n.tlb: $t.odl\n";
  print MAK "\t\$(MKTYPLIB) \$(MKTYPLIBFLAGS)";
  foreach $i (@incpath) {
    print MAK " -I $i";
  }
  foreach $i (@defines) {
    print MAK " -D$i";
  }
  print MAK " -h $n.h" if $odl_to_h;
  print MAK " -tlb $n.tlb $t.odl\n";
  if ($odl_to_java) {
    print MAK "\t\$(JACTIVEX) \$(JACTIVEXFLAGS) -p $package $n.tlb\n";
  }
  print MAK "clean::\n";
  print MAK "\t-\$(RM) $n.h\n" if $odl_to_h;
  print MAK "\t-\$(RM) $n.tlb\n";
}

foreach $t (@javasources) {
  print MAK "javaclasses: \$(CLASSES_ROOT)\\$package_dir\\$t.class\n"; # execute this early
  print MAK "\$(CLASSES_ROOT)\\$package_dir\\$t.class: $t.java\n";
  if ($make_quick) {
    print MAK "!ifdef _JAVAC_FORCE_REMOVE\n";
    print MAK "\t\@if exist \$@ \$(RM) \$@\n";
    print MAK "!else\n";
    print MAK "\t@\$(MAKE) -nologo -f $component.mak javaclasses SUB=0 _JAVAC_FORCE_REMOVE=1\n";
    print MAK "\t\$(JAVAC) \$(JFLAGS) $t.java\n";
    print MAK "!endif\n";
  }
  else {
    print MAK "\t\$(JAVAC) \$(JFLAGS) $t.java\n";
  }
  print MAK "clean::\n";
  print MAK "\t-\$(RM) \$(CLASSES_ROOT)\\$package_dir\\$t.class\n";
}

foreach $t (@javahcmds) {
  my $i = $t;
  $i =~ s/\./_/g;
  my $j = $t;
  $j =~ s/\./\\/g;
  print MAK "precompile: $i.h\n";
  print MAK "$i.h: \$(CLASSES_ROOT)\\$j.class\n";
  print MAK "\t\@if exist $i.h \$(RM) $i.h\n"; # force msjavah to create a new h-file with a new date!
  print MAK "\t\$(JAVAH) \$(JAVAHFLAGS) -o $i.h $t\n";
  print MAK "clean::\n";
  print MAK "\t-\$(RM) $i.h\n";
}

foreach $t (@lsources){
  my $i;
  for ($i = 0; $i <= $#cppsources; $i++) {
    if (uc($cppsources[$i]) eq uc($t)) {
      splice @cppsources, $i, 1; # remove this target from @cppsources
      last;
    }
  }

  print MAK "precompile: $t.cpp\n"; # force this before compile...
#  if($yl_cg_style)
#  {
#    print MAK "$t.cpp: $t.l\n";
#    print MAK "\t\$(LEX) \$(LEXFLAGS) -P \$(PROJECT_ROOT)\\gentools\\yylex.cpp -o $t.cpp~ -D $t.h~ $t.l\n";
#    print MAK "\tperl \$(PROJECT_ROOT)\\gentools\\yyreplace.pl $t l\n";
#    print MAK "clean::\n";
#    print MAK "\t-\$(RM) $t.cpp\n";
#  }
#  else
#  {
#    print MAK "$t.h: $t.cpp\n"; # prevent nmake from calling the following rule twice...
#    print MAK "$t.cpp: $t.l\n";
#    print MAK "\t\$(LEX) \$(LEXFLAGS) -o $t.cpp -D $t.h $t.l\n";
#    print MAK "clean::\n";
#    print MAK "\t-\$(RM) $t.cpp $t.h\n";
#  }  
  $cppsources[$#cppsources+1] = $t;
}

foreach $t (@ysources){
  my $i;
  for ($i = 0; $i <= $#cppsources; $i++) {
    if (uc($cppsources[$i]) eq uc($t)) {
      splice @cppsources, $i, 1; # remove this target from @cppsources
      last;
    }
  }

  print MAK "precompile: $t.cpp\n"; # force this before compile...
#  if($yl_cg_style)
#  {
#    print MAK "$t.cpp: $t.y\n";
#    print MAK "\t\$(YACC) \$(YACCFLAGS) -P \$(PROJECT_ROOT)\\gentools\\yyparse.cpp -o $t.cpp~ -D $t.h~ $t.y\n";
#    print MAK "\tperl \$(PROJECT_ROOT)\\gentools\\yyreplace.pl $t y\n";
#    print MAK "clean::\n";
#    print MAK "\t-\$(RM) $t.cpp\n";
#  }
#  else
#  {
#    print MAK "$t.h: $t.cpp\n"; # prevent nmake from calling the following rule twice...
#    print MAK "$t.cpp: $t.y\n";
#    print MAK "\t\$(YACC) \$(YACCFLAGS) -o $t.cpp -D $t.h $t.y\n";
#    print MAK "clean::\n";
#    print MAK "\t-\$(RM) $t.cpp $t.h\n";
#  }
  $cppsources[$#cppsources+1] = $t;
}

foreach $t (@mcsources){
  my $n = getname($t);
  print MAK "precompile: $n.h\n";
  print MAK "$n.h: $t.mc\n";
  print MAK "\t\$(MC) \$(MCFLAGS) $t.mc\n";
  print MAK "clean::\n";
  print MAK "\t-\$(RM) $n.h\n";
}

for ($i=0; $i <= $#copyfrom; $i++) {
  print MAK "precompile: $copyto[$i]\n";
  print MAK "$copyto[$i]: $copyfrom[$i]\n";
  print MAK "\t\$(CP) $copyfrom[$i] $copyto[$i]\n";
  print MAK "clean::\n";
  print MAK "\t-\$(RM) $copyto[$i]\n";
}

foreach $t (@cppsources){
  my $i;
  my $n = getname($t);
  print MAK "compile: \$(OBJ)\\$n.obj\n"; # execute this in the compile phase
  print MAK "\$(OBJ)\\$n.sbr: \$(OBJ)\\$n.obj\n" if $make_browse;
  print MAK "\$(OBJ)\\$n.obj: $t.cpp \$(OBJ)\n";
  print MAK "\t@\$(CXX) @<<\n";
  print MAK "\$(CXXFLAGS)";
  foreach $i (@incpath) {
    print MAK " -I$i";
  }
  foreach $i (@defines) {
    print MAK " -D$i";
  }
  print MAK " -DUNICODE -D_UNICODE" if $isunicode;
  print MAK " -Gi -Gm" if $make_quick;
  print MAK " -Yu$pchfile.h -Fp\$(OBJ)\\$pchfile.pch" if $make_quick and $pchfile ne "";
  print MAK " -Yc$pchfile.h" if $make_quick and uc($pchfile) eq uc($n);
  print MAK " -Fr\$(OBJ)\\$n.sbr" if $make_browse;
  print MAK " -Fo\$(OBJ)\\$n.obj -c \$(MAKEDIR)\\$t.cpp\n";
  print MAK "<<\n";
  if ($make_quick and $pchfile ne "") {
    if (uc($pchfile) eq uc($n)) {
      print MAK "\$(OBJ)\\$pchfile.pch: \$(OBJ)\\$n.obj\n";
    }
    else {
      print MAK "\$(OBJ)\\$n.obj: \$(OBJ)\\$pchfile.pch\n";
    }
  }
  print MAK "clean::\n";
  print MAK "\t-\$(RM) \$(OBJ)\\$n.obj\n";
  print MAK "\t-\$(RM) \$(OBJ)\\$n.sbr\n" if $make_browse;
  $objsources[$#objsources+1] = "\$(OBJ)\\".$n;
  $sbrsources[$#sbrsources+1] = "\$(OBJ)\\".$n if $make_browse;
}

if ($make_quick and $pchfile ne "") {
  print MAK "clean::\n";
  print MAK "\t-\$(RM) \$(OBJ)\\$pchfile.pch\n";
}

foreach $t (@csources){
  my $i;
  my $n = getname($t);
  print MAK "compile: \$(OBJ)\\$n.obj\n"; # execute this in the compile phase
  print MAK "\$(OBJ)\\$n.sbr: \$(OBJ)\\$n.obj\n" if $make_browse;
  print MAK "\$(OBJ)\\$n.obj: $t.c \$(OBJ)\n";
  print MAK "\t@\$(CC) @<<\n";
  print MAK "\$(CFLAGS)";
  foreach $i (@incpath) {
    print MAK " -I$i";
  }
  foreach $i (@defines) {
    print MAK " -D$i";
  }
  print MAK " -DUNICODE -D_UNICODE" if $isunicode;
  print MAK " -Gi -Gm" if $make_quick;
  print MAK " -Fr\$(OBJ)\\$n.sbr" if $make_browse;
  print MAK " -Fo\$(OBJ)\\$n.obj -c \$(MAKEDIR)\\$t.c\n";
  print MAK "<<\n";
  print MAK "clean::\n";
  print MAK "\t-\$(RM) \$(OBJ)\\$n.obj\n";
  print MAK "\t-\$(RM) \$(OBJ)\\$n.sbr\n" if $make_browse;
  $objsources[$#objsources+1] = "\$(OBJ)\\".$n;
  $sbrsources[$#sbrsources+1] = "\$(OBJ)\\".$n if $make_browse;
}

if (scalar @cppsources + scalar @csources != 0) {
  print MAK "clean::\n";
  print MAK "\t-\$(RM) vc60.idb vc60.pdb\n";
}

if ($rcsource ne "") {
  my $n = getname($rcsource);
  print MAK "compile: \$(OBJ)\\$n.res\n"; # execute this in the compile phase
  print MAK "\$(OBJ)\\$n.res: $rcsource.rc \$(OBJ)\n";
  print MAK "\t\$(RC) \$(RCFLAGS)";
  foreach $i (@incpath) {
    print MAK " -i $i";
  }
  foreach $i (@defines) {
    print MAK " -d $i";
  }
  print MAK " -fo\$(OBJ)\\$n.res $rcsource.rc\n";
  print MAK "clean::\n";
  print MAK "\t-\$(RM) \$(OBJ)\\$n.res\n";
}

if ($implib ne "") {
  print MAK "prelink: $implib.lib $implib.exp\n";
  print MAK "$implib.exp: $implib.lib\n"; # prevent nmake from calling the following rule twice...
  print MAK "$implib.lib: ".join(".obj ", @objsources, "")
                  .($deffile ne "" ? $deffile.".def" : "")."\n";
  print MAK "\t\$(AR) @<<\n";
  print MAK "-MACHINE:IX86 -NODEFAULTLIB -OUT:$implib.lib\n";
  if ($deffile ne "") {
    print MAK "-DEF:$deffile.def\n";
  }
  else {
    print MAK "-DEF:\n";
  }
  print MAK join(".obj ", @objsources, "")."\n";
  print MAK "<<\n";
  print MAK "clean::\n";
  print MAK "\t-\$(RM) $implib.lib $implib.exp\n";
}

if ($target ne "") {
  if ($islib) {
    print MAK "prelink: $target\n";
    print MAK "$target: ".join(".obj ", @objsources, "")."\n";
    print MAK "\t\$(AR) @<<\n";
    print MAK "-OUT:$target\n";
    print MAK join(".obj ", @objsources, "\n");
    print MAK "<<\n";
    print MAK "clean::\n";
    print MAK "\t-\$(RM) $target\n";
  }
  else {
    print MAK "link: $target\n";
    print MAK "$target: ".join(".obj ", @objsources, "")
                      .($rcsource ne "" ? "\$(OBJ)\\".getname($rcsource).".res " : "")
                      .join(".lib ", @libraries, "")
                      .($implib ne "" ? $implib.".exp" : $deffile ne "" ? $deffile.".def" : "")."\n";
    print MAK "\t\$(LNK) @<<\n";
    print MAK "\$(LFLAGS) -OUT:$target";
    if ($isgui) {
      print MAK " -SUBSYSTEM:WINDOWS";
	  if ($isunicode && !$isdll) {
        print MAK " /ENTRY:\"wWinMainCRTStartup\"";
	  }
    } else {
      print MAK " -SUBSYSTEM:CONSOLE";
    }
    if ($make_quick) {
      print MAK " -INCREMENTAL:YES";
    } else {
      print MAK " -INCREMENTAL:NO";
    }
    if ($isdll) {
    print MAK " -DLL";
    }
    print MAK "\n";

    if ($implib ne "") {
      print MAK "$implib.exp\n";
    }
    elsif ($deffile ne "") {
      print MAK "-DEF:$deffile.def\n";
    }

    print MAK join(".obj ", @objsources, @defobjs, "\n");
    print MAK "\$(OBJ)\\".getname($rcsource).".res\n" if $rcsource ne "";
    print MAK join(".lib ", @libraries, @deflibs, "\n");
    foreach $t (@libpath) {
      print MAK "-LIBPATH:$t\n";
    }
    print MAK "<<\n";
    if ($isdll and $implib eq "")
    {
      $t = $target;
      $t =~ s/\.[A-Za-z_0-9]+$//;
      print MAK "\t@\$(RM) $t.lib $t.exp\n"       # remove temporary .lib/.exp files
    }
    print MAK "clean::\n";
    print MAK "\t-\$(RM) $target\n";
    print MAK "\t-\$(RM) $rawtarget.pdb\n";
    print MAK "\t-\$(RM) $rawtarget.ilk\n" if $make_quick;
  }

} # end if ($target ne "")

if ($make_browse) {
  if ($make_quick) {
    if (scalar @sbrsources != 0) {
      print MAK "link: $component.bsc\n";
      print MAK "$component.bsc: ".join(".sbr ", @sbrsources, "")."\n";
      print MAK "\t\$(BSCMAKE) @<<\n";
      print MAK "-o $component.bsc\n";
      print MAK join(".sbr ", @sbrsources, "\n");
      print MAK "<<\n";
      print MAK "clean::\n";
      print MAK "\t-\$(RM) $component.bsc\n";
    }
  } else {
    print MAK "addbrowsefiles:\n";
    if (scalar @sbrsources != 0) {
      print MAK "\t\$(CAT) << >>\$(_BROWSE_LIST_FILE)\n";
      foreach $t (@sbrsources) {
        print MAK "\$(MAKEDIR)\\$t.sbr\n";
      }
      print MAK "<<\n";
    }
    if (scalar @subprojects != 0) {
      print MAK "!if \$(SUB) != 0\n";
      foreach $t (@subprojects) {
        my $i = getpath($t);
        my $n = getname($t);
        print MAK "\tcd $i\n" if $i ne ".";
        print MAK "\t\$(MAKE) -nologo -f $n.mak addbrowsefiles SUB=1 DEBUG=\$(DEBUG) _BROWSE_LIST_FILE=\$(_BROWSE_LIST_FILE)\n";
        print MAK "\tcd \$(MAKEDIR)\n" if $i ne ".";
      }
      print MAK "!endif\n";
    }
    print MAK "$component.lst: force\n";
    print MAK "\t\@if exist $component.lst \$(RM) $component.lst\n";
    print MAK "\t@\$(MAKE) -nologo -f $component.mak addbrowsefiles SUB=\$(SUB) DEBUG=\$(DEBUG) _BROWSE_LIST_FILE=\$(MAKEDIR)\\$component.lst\n";
    if (scalar @sbrsources != 0 or scalar @subprojects != 0) {
      print MAK "postmake: $component.bsc\n";
      print MAK "$component.bsc: $component.lst\n";
      print MAK "\tif exist $component.lst \$(BSCMAKE) -n -o $component.bsc \@$component.lst\n";
      print MAK "clean::\n";
      print MAK "\t-\$(RM) $component.bsc\n";
      print MAK "\t-\$(RM) $component.lst\n";
    }
  }
} # end if ($make_browse)

for ($i=0; $i <= $#latecopyfrom; $i++) {
  print MAK "link: $latecopyto[$i]\n";
  print MAK "$latecopyto[$i]: $latecopyfrom[$i]\n";
  print MAK "\t\$(CP) $latecopyfrom[$i] $latecopyto[$i]\n";
  print MAK "clean::\n";
  print MAK "\t-\$(RM) $latecopyto[$i]\n";
}

if ($make_depend) {

# make missing precompile targets
close MAK;
# typical build steps that generate .h files:
system "nmake -nologo -f $component.mak oleclasses precompile SUB=0";
open (MAK, ">> $component.mak") or die "make.pl: error: can't reopen $component.mak";

# make dependencies:
print "make dependencies...\n";

#set minimum environment
$ENV{PROJECT_ROOT} = "\\4Control" if !defined $ENV{PROJECT_ROOT};
$ENV{INCLUDE_DIR} = $ENV{PROJECT_ROOT}."\\COM\\softing\\fc\\inc";
$ENV{RESOURCE_DIR} = $ENV{PROJECT_ROOT}."\\COM\\softing\\fc\\res";
chomp($cwd = `cd`);     # is there any other way to learn my current working directory???
$ENV{MAKEDIR} = $cwd;

foreach $t (@cppsources) {
  my $n = getname($t);
  my @depend = find_depend($t.".cpp");
  print MAK "\$(OBJ)\\$n.obj: ".join("\\\n\t", @depend)."\n";
}

foreach $t (@csources) {
  my $n = getname($t);
  my @depend = find_depend($t.".c");
  print MAK "\$(OBJ)\\$n.obj: ".join("\\\n\t", @depend)."\n";
}

if ($rcsource ne "") {
  my $n = getname($rcsource);
  my @depend = find_rcdepend($rcsource.".rc");
  print MAK "\$(OBJ)\\$n.res: ".join("\\\n\t", @depend)."\n";
}

} #end if ($make_depend)

close MAK;
exit;
#end main

sub find_depend{
  my $file=$_[0];
  my %depend;
  my $opt = "-E -DWIN32 -D_WIN32_WINNT=0x0400 -D_MT"; # see global.inc
  my $x;
  $opt .= " -D_DLL" unless $static_runtime;
  $opt .= " -DUNICODE -D_UNICODE" if $isunicode;
  foreach $x (@defines) {
    $opt .= " -D$x";
  }
  foreach $x (@incpath) {
    $opt .= " -I$x";
  }
  # replace $(X) with environment var like nmake...
  $opt =~ s/\$\(([A-Za-z_][A-Za-z_0-9]*)\)|\$([A-Za-z_])/$ENV{$+}/g;

  open (CPPOUT, "cl -nologo $opt $file |") or die "make.pl: error: can't preprocess $file";
  while (<CPPOUT>) {
    if (/^#line[ \t]+[0-9]+[ \t]+"(.+)"/){
        my $filename = $1;
        $filename =~ s/\\\\/\\/g;
        $filename = '"'.$filename.'"' if $filename =~ / /;
        $depend{$filename}=1;
    }
  }
  close CPPOUT;
  return sort keys %depend;
}

sub find_rcdepend{
  my $file=$_[0];
  my $rc_syntax=1;
  my %depend;
  my $opt = "-E -DRC_INVOKED -I."; # preted to be compiled by rc...
  my $x;
  foreach $x (@defines) {
    $opt .= " -D$x";
  }
  foreach $x (@incpath) {
    $opt .= " -I$x";
  }
  # replace $(X) with environment var like nmake...
  $opt =~ s/\$\(([A-Za-z_][A-Za-z_0-9]*)\)|\$([A-Za-z_])/$ENV{$+}/g;

  open (CPPOUT, "cl -nologo $opt $file |") or die "make.pl: error: can't preprocess $file";
  while (<CPPOUT>) {
    if (/^#line[ \t]+[0-9]+[ \t]+"(.+)"/){
        my $filename = $1;
        $filename =~ s/\\\\/\\/g;
        $filename = '"'.$filename.'"' if $filename =~ / /;
        $depend{$filename}=1;
        $rc_syntax = $1 !~ /\.[hH]$/;
    }
    elsif ($rc_syntax &&
           /([^" \t\n]+)\.(bmp|ico|cur|tlb|rgs)[" \t\n]/i){
        my $filename = $1.".".$2;
        $filename =~ s/\\\\/\\/g;
        if (-e $filename) {
          $filename = '"'.$filename.'"' if $filename =~ / /;
          $depend{$filename}=1;
        }
    }
  }
  close CPPOUT;
  return sort keys %depend;
}

sub getpath{
  my $file=$_[0];
  if ($file =~ m!^(.*)[\\/]([^\\/]*)$!) {
    return "\\" if $1 eq "";
    return $1;
  }
  return ".";
}

sub getname{
  my $file=$_[0];
  if ($file =~ m!^(.*)[\\/]([^\\/]*)$!) {
    return $2;
  }
  return $file;
}

sub getExt
{
   my $file=$_[0];
   
   if($file =~ m{\.([^\\/.]*)$})
   {
      return lc($1);
   }

   return "";
}
sub getFileNameNoExt
{
   my $file= getname($_[0]);
   $file =~ s{\.([^\\/.]*)$}{};
   $file =~ s{\.$}{};
   return $file;
}


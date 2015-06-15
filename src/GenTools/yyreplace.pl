#H>> $Header: /4CReleased/V2.20.00/GENTOOLS/yyreplace.pl 1     28.02.07 18:53 Ln $
#----------------------------------------------------------------------------*
#
# =FILENAME            $Workfile: yyreplace.pl $
#                      $Logfile: /4CReleased/V2.20.00/GENTOOLS/yyreplace.pl $
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
#  Post process the xayy/lex output. Goals are: 
#  a) no 'global' variable
#  b) only one cpp file for the yacc and one for the lex, no h files
#  c) prepare the yacc to run without a lex (custom scanner)
#  This perl script will be used only if you set yacc_lex_cg_style=1 in our 
#  component's  inf file.
#==
#----------------------------------------------------------------------------*
# =MODIFICATIONS
# $History: yyreplace.pl $
# 
# *****************  Version 1  *****************
# User: Ln           Date: 28.02.07   Time: 18:53
# Created in $/4CReleased/V2.20.00/GENTOOLS
# 
# *****************  Version 1  *****************
# User: Ln           Date: 28.08.03   Time: 16:34
# Created in $/4Control/GENTOOLS
# 
# *****************  Version 2  *****************
# User: Ef           Date: 12.12.00   Time: 15:52
# Updated in $/4Control/gentools
# * to # from vss comment
# 
# *****************  Version 1  *****************
# User: Ef           Date: 11.12.00   Time: 18:14
# Created in $/4Control/gentools
# gentools: taken from V1.3, new file headers
#==
#*****************************************************************************
#H<<

# arguments:
#  ARGV[0]: xx
#  ARGV[1]: y|l

# this script does the following:
# 1) get all #include lines and lines starting with #yy_movetop 
#    from xx.y/xx.l and write them at top of xx.cpp 
# 2) replace all occurenes of yy_parse(or yy_scan) in xx.h~ with xx_parse(xx_scan)
#    and store result into xx.cpp (lines containing "extern YYSTYPE yylval;" are also removed).
# 3) replace all occurenes of yy_parse(or yy_scan) in xx.cpp~ with xx_parse(xx_scan)
#    and store result into xx.cpp (lines containing "extern YYSTYPE yylval;" are also removed).
# 4) del xx.h~  xx.cpp~



if(!defined($ARGV[0]) || !defined($ARGV[1]) || defined($ARGV[2]))
{
    error("invalid arguments for replace.pl");
}
if(($ARGV[1] ne "y") && ($ARGV[1] ne "l"))
{
    error("invalid arguments for replace.pl");
}






if($ARGV[1] eq "y")
{
  $search= "yy_parse";
  $replace= lc($ARGV[0])."_parse";
  $search2= "\\bParseContext\\b";
  $replace2= lc($ARGV[0])."_ParseContext";
}
if($ARGV[1] eq "l")
{
  $search= "yy_scan";
  $replace= lc($ARGV[0])."_scan";
  $search2= "ParseContext";
  $replace2= lc($ARGV[0])."_ParseContext";
}


$outFile = lc($ARGV[0]).".cpp";

open (OUTPUT, "> $outFile") or error("can't create $outFile");
print OUTPUT "/*!!!!! this file has been generated using yyreplace.pl !!!!!*/\n";
print OUTPUT "/* input files for yyreplace.pl: $ARGV[0].$ARGV[1], $ARGV[0].cpp~, $ARGV[0].h~*/\n\n";


# get all #include lines from xx.y on top of xx.cpp 
print OUTPUT "/*START yyreplace.pl: include files and #yy_movetop lines from $ARGV[0].$ARGV[1] */\n";

open (INPUT, "< $ARGV[0].$ARGV[1]") or error("can't open $ARGV[0].$ARGV[1]");
  while (<INPUT>) {
    if(/^[ \t]*#[ \t]*include.*/)
    {
      print OUTPUT;
    }
    if(/^#yy_movetop.*/)
    {
        s/^#yy_movetop//;
        print OUTPUT;
    }

  }
close INPUT;
print OUTPUT "/*END yyreplace.pl: include files from $ARGV[0].$ARGV[1] */\n\n\n";


# get content of xx.h~ and replace xx_scan/xx_parse
print OUTPUT "/*START yyreplace.pl: file $ARGV[0].h~ */\n";

open (INPUT, "< $ARGV[0].h~") or error("can't open $ARGV[0].h~");
  while (<INPUT>) {
    s/$search/$replace/go;
    s/$search2/$replace2/go;
    s/extern YYSTYPE yylval;//go;
    print OUTPUT;
  }
close INPUT;
unlink  "$ARGV[0].h~";
print OUTPUT "/*END yyreplace.pl: file $ARGV[0].h~ */\n";




# get content of xx.cpp~ and replace xx_scan/xx_parse
print OUTPUT "/*START yyreplace.pl: file $ARGV[0].cpp~ */\n";

open (INPUT, "< $ARGV[0].cpp~") or error("can't open $ARGV[0].cpp~");
  while (<INPUT>) {
    s/$search/$replace/go;
    s/$search2/$replace2/go;
    s/extern YYSTYPE yylval;//go;
    s{(^#yy_movetop.*)}{/*yyreplace.pl: moved on top: $1 */};
    print OUTPUT;
  }
close INPUT;
unlink  "$ARGV[0].cpp~";
print OUTPUT "/*END yyreplace.pl: file $ARGV[0].cpp~ */\n";


close OUTPUT;





sub error{
  my $arg = $_[0];
  print "error : $arg\n";
  exit 1;
}

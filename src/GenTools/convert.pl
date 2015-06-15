#H>> $Header: /4CReleased/V2.20.00/GENTOOLS/convert.pl 1     28.02.07 18:53 Ln $
#----------------------------------------------------------------------------*
#
# =FILENAME            $Workfile: convert.pl $
#                      $Logfile: /4CReleased/V2.20.00/GENTOOLS/convert.pl $
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
# convert AppStudio project to ATCMControl project
#==
#----------------------------------------------------------------------------*
# =MODIFICATIONS
# $History: convert.pl $
# 
# *****************  Version 1  *****************
# User: Ln           Date: 28.02.07   Time: 18:53
# Created in $/4CReleased/V2.20.00/GENTOOLS
# 
# *****************  Version 1  *****************
# User: Ln           Date: 28.08.03   Time: 16:34
# Created in $/4Control/GENTOOLS
# 
# *****************  Version 3  *****************
# User: Ef           Date: 13.12.00   Time: 18:01
# Updated in $/4Control/gentools
# allow also calling with .dsp extension, minimal commad line help
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





$VERSION = '$Revision: 1 $';

if($ARGV[0] eq "" or defined($ARGV[1]) or 
   (getExt($ARGV[0]) ne "" and getExt($ARGV[0]) ne "dsp")
   or $ARGV[0]=~m{^[-/]}
  )
{
    die "command line error, usage:\n     convert.pl <component>[.dsp]\n" 
}

$DSPNoExt = getFileNameNoExt($ARGV[0]);



print "$0 Version=$VERSION Project=$DSPNoExt\n";

!-e $DSPNoExt.".dsp~" or die "$DSPNoExt.dsp already converted";

rename ($DSPNoExt.".dsp", $DSPNoExt.".dsp~")
        or die "can't rename $DSPNoExt.dsp to $DSPNoExt.dsp~";


open (INPUT, "< $DSPNoExt.dsp~") or die "can't open $DSPNoExt.dsp~";
open (OUTPUT, "> $DSPNoExt.dsp") or die "can't create $DSPNoExt.dsp";
while (<INPUT>) {
  if (/"Win32 \(x86\)[^"\n]*"/ || /"Java Virtual Machine Java Project"/) {
    print OUTPUT $` . '"Win32 (x86) External Target"' . $';
  }
  elsif (/^CPP=cl.exe$/) {}
  elsif (/^MTL=midl.exe$/) {}
  elsif (/^RSC=rc.exe$/) {}
  elsif (/^# ADD /) {}
  elsif (/^# SUBTRACT /) {}
  elsif (/^BSC32=bscmake.exe$/) {}
  elsif (/^LINK32=link.exe$/) {}
  elsif (/^# PROP BASE Target_Dir/) {
    print OUTPUT <<"-end-";
# PROP BASE Cmd_Line "NMAKE /f $DSPNoExt.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "$DSPNoExt.exe"
# PROP BASE Bsc_Name "$DSPNoExt.bsc"
-end-
    print OUTPUT;
  }
  elsif (/^# PROP Target_Dir/) {
    print OUTPUT <<"-end-";
# PROP Cmd_Line "NMAKE /f $DSPNoExt.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "$DSPNoExt.exe"
# PROP Bsc_Name "$DSPNoExt.bsc"
-end-
    print OUTPUT;
  }
  else {
    print OUTPUT;
  }
}
close INPUT;
close OUTPUT;

print "$DSPNoExt.dsp was successfully converted (Old version is $DSPNoExt.dsp~).";




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



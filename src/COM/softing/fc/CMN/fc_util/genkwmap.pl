#H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_util/genkwmap.pl 1     28.02.07 19:00 Ln $
#----------------------------------------------------------------------------*
#
# =FILENAME            $Workfile: genkwmap.pl $
#                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_util/genkwmap.pl $
#
# =PROJECT             7VH102  ATCMControl
#
# =SWKE                CMN
#
# =COMPONENT           UTIL
#
# =CURRENT             $Date: 28.02.07 19:00 $
#                      $Revision: 1 $
#
# =ENVIRONMENT         perl5:
#                      MS Windows NT 4.0
#                      MSVC++ V5.0 / MFC 4.2 / ATL 2.0 / DAO 3.5
#                      MS Java 2.0
#                      MS Visual SourceSafe 5.0
#
# =REFERENCES
#==
#----------------------------------------------------------------------------*
#





#==
#
#----------------------------------------------------------------------------*
# =DESCRIPTION
#==
#----------------------------------------------------------------------------*
# =MODIFICATIONS
# $History: genkwmap.pl $
# 
# *****************  Version 1  *****************
# User: Ln           Date: 28.02.07   Time: 19:00
# Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_util
# 
# *****************  Version 1  *****************
# User: Ln           Date: 28.08.03   Time: 16:33
# Created in $/4Control/COM/softing/fc/CMN/fc_util
# 
# *****************  Version 2  *****************
# User: Ef           Date: 24.01.01   Time: 10:50
# Updated in $/4Control/COM/softing/fc/CMN/fc_util
# 
# *****************  Version 1  *****************
# User: Ef           Date: 23.01.01   Time: 17:10
# Created in $/4Control/COM/softing/fc/CMN/fc_util
# utils and baselib from V1.3
# 
# *****************  Version 4  *****************
# User: Ef           Date: 8.06.99    Time: 9:34
# Updated in $/4Control/COM/softing/fc/CMN/UTIL
# fixed bug
# 
# *****************  Version 3  *****************
# User: Ef           Date: 2.06.99    Time: 20:12
# Updated in $/4Control/COM/softing/fc/CMN/UTIL
# more st syntax
# 
# *****************  Version 2  *****************
# User: Ef           Date: 1.06.99    Time: 19:16
# Updated in $/4Control/COM/softing/fc/CMN/UTIL
# started to integrate 4ccg sts_scan to util.dll
# 
# *****************  Version 1  *****************
# User: Ef           Date: 31.05.99   Time: 15:10
# Created in $/4Control/COM/softing/fc/CMN/UTIL
#==
#*****************************************************************************
#H<<

# this script generates an enum and a keyword matching function 
# from the KW definition in $ARGV[0].
# Every line of the KW definition file should contain a keyword
# command line: 
#    genkwmap.pl <keywords>.txt <source>.cpp <header>.h <prefix>
#
# The enum definition will be dumped to <header>.h
# The map function (<prefix>_MapKW) and the enum to keyword
# function (<prefix>_GetKeyword) are declared static and 
# are dumped into <source>.cpp. All global identifiers
# are prefixed with <prefix>.



 #check command line parameter:
 if($#ARGV!=3 || getExt($ARGV[0]) ne "txt" || getExt($ARGV[1]) ne "cpp" 
    || getExt($ARGV[2]) ne "h" || getExt($ARGV[3]) ne "")
 {
    die "error: usage: genkwmap.pl <keywords>.txt <source>.cpp <header>.h <prefix>\n"; 
 }
 $kwDefFile = $ARGV[0];
 $enumFile = $ARGV[2];
 $cppFile = $ARGV[1];
 $preFix = $ARGV[3];


 #read kwdef file:
 open (INPUT, $kwDefFile) or die "error: genkwmap.pl: can't open '$kwDefFile'";
 @kws = <INPUT>;
 close(INPUT);

 #remove \n, remove empty lines and to 
 #upper case all keyword form the kw definition file 
 chomp(@kws);
 grep(s{\s*}{}g, @kws);
 @kws = grep(m{[^\s]}, @kws);
 $maxKWLen = 0;
 for($i=0; $i<=$#kws; $i++)
 {
    $kws[$i] = uc($kws[$i]);
    
    # get length of longest KW:
    $ilen= length($kws[$i]);
    $maxKWLen = $ilen if($ilen>$maxKWLen);
 }




 # dump keyword enum file:
 open (KWH, ">$enumFile") or die "error: genkwmap.pl: can't open '$enumFile'";
 print KWH "/* !!      do not edit this file     !!\n";
 print KWH " * automaticly generated by genkwmap.pl\n";
 print KWH " */\n\n";
 print KWH "#ifndef ".getFileNameNoExt($enumFile)."_H_\n";
 print KWH "#define ".getFileNameNoExt($enumFile)."_H_\n";
 print KWH "/* enum for all IEC kewords */\n";
 print KWH "enum ".$preFix."_KW_ENUM\n";
 print KWH "{\n";
 print KWH "    ".$preFix."_KW_NO_KEYWORD,\n";
 foreach $t (@kws)
 {
    print KWH "    ".$preFix."_KW_$t,\n";
 }
 print KWH "};\n\n";
 print KWH "#endif //".getFileNameNoExt($enumFile)."_H_\n";
 close(KWH);



 # dump the GetKeyword switch:
 #do not sort earlier!! the enum should be in same oder as in IECKeywords.txt:
 @kws = sort @kws;

 open (KWMAP, ">$cppFile") or die "error: genkwmap.pl: can't open '$cppFile'";
 print KWMAP "/* !!      do not edit this file     !!\n";
 print KWMAP " * automaticly generated by genkwmap.pl\n";
 print KWMAP " */\n\n";
 foreach $t (@kws)
 {
     print KWMAP "static const TCHAR* ".$preFix."_psz_$t = _T(\"$t\");\n";
 }
 print KWMAP "\n\n\nstatic LPCTSTR ".$preFix."_GetKeyword(".$preFix."_KW_ENUM kw)\n";
 print KWMAP "{\n";
 print KWMAP "    switch(kw)\n";
 print KWMAP "    {\n";
 foreach $t (@kws)
 {
    print KWMAP "    case ".$preFix."_KW_$t: return ".$preFix."_psz_$t;\n";
 }
 print KWMAP "    case ".$preFix."_KW_NO_KEYWORD: return \"\";\n";
 print KWMAP "    }\n";
 print KWMAP "    assert(!\"".$preFix."_GetKeyword: invalid enum\");\n";
 print KWMAP "    return \"\";\n";
 print KWMAP "}\n\n";
 print KWMAP "\n\n\nstatic ".$preFix."_KW_ENUM ".$preFix."_MapKW(const TCHAR* psz, int iLen)\n";
 print KWMAP "{\n\n";
 print KWMAP "    TCHAR buffer[$maxKWLen+1];\n";
 print KWMAP "\n";
 print KWMAP "    if(iLen>$maxKWLen)\n";
 print KWMAP "        return ".$preFix."_KW_NO_KEYWORD;\n\n";
 print KWMAP "\n";
 print KWMAP "    memcpy(buffer, psz, iLen);\n";
 print KWMAP "    buffer[iLen] = 0;\n";
 print KWMAP "    _tcsupr(buffer);\n";
 print KWMAP "\n";
 print KWMAP "    switch(buffer[0])\n";
 print KWMAP "    {\n";

 
 for($i=0; $i<=$#kws; )
 {
    $firstchar = substr($kws[$i], 0, 1);
    print KWMAP "    case '$firstchar':\n"; 

    for($i2=$i; $i2<=$#kws; $i2++)
    {
        last if(substr($kws[$i2], 0, 1) ne $firstchar);
    }

    for($i; $i<$i2; $i++)
    {
        print KWMAP "        if(iLen==".length($kws[$i]).
                    " && !_tcscmp(buffer, ".$preFix."_psz_$kws[$i])) return ".
                    $preFix."_KW_$kws[$i];\n";
    }
    print KWMAP "    break;\n";
 }

 
 print KWMAP "    }\n\n";
 print KWMAP "    return ".$preFix."_KW_NO_KEYWORD;\n";
 print KWMAP "}\n";
 close(KWMAP);




######################################################

sub getname{
  my $file=$_[0];
  if ($file =~ m!^(.*)[\\/]([^\\/]*)$!) {
    return $2;
  }
  return $file;
}
sub getExt()
{
   my $file=$_[0];
   
   if($file =~ m{\.([^\\/.]*)$})
   {
      return lc($1);
   }

   return "";
}
sub getFileNameNoExt()
{
   my $file= getname($_[0]);
   $file =~ s{\.([^\\/.]*)$}{};
   $file =~ s{\.$}{};
   return $file;
}

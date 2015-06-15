#H>> $Header: /4CReleased/V2.20.00/GENTOOLS/logfile.pl 1     28.02.07 18:53 Ln $
#----------------------------------------------------------------------------*
#
# =FILENAME            $Workfile: logfile.pl $
#                      $Logfile: /4CReleased/V2.20.00/GENTOOLS/logfile.pl $
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
#  Write
#==
#----------------------------------------------------------------------------*
# =MODIFICATIONS
# $History: logfile.pl $
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


open (LOGFILE, "> $ARGV[0]") or die "can't create $ARGV[0]";
while (<STDIN>) {
  print LOGFILE;
  print;
}
close LOGFILE;

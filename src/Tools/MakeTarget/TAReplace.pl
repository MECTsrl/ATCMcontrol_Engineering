#!perl

if($ARGV[0] eq "" || $ARGV[1] eq "")
{
    die "usage: TAReplace <ReplaceFile> <BaseDirectory>\n";
}

open(REPLACE, $ARGV[0]) || die "Can't open '$ARGV[0]'\n";

my $Line;
my $PH;
my $RT;
@PlaceHolders;
@ReplaceTexts;
my $Counter;

#standard replacements:
#source safe:

$Counter = 0;
$PlaceHolders[$Counter] = "SSHEADER";
$ReplaceTexts[$Counter] = "XHeader: X";
$ReplaceTexts[$Counter] =~ s/X/\$/g;
$Counter++;

$PlaceHolders[$Counter] = "SSARCHIVE";
$ReplaceTexts[$Counter] = "XArchive: X";
$ReplaceTexts[$Counter] =~ s/X/\$/g;
$Counter++;

$PlaceHolders[$Counter] = "SSWORKFILE";
$ReplaceTexts[$Counter] = "XWorkfile: X";
$ReplaceTexts[$Counter] =~ s/X/\$/g;
$Counter++;

$PlaceHolders[$Counter] = "SSLOGFILE";
$ReplaceTexts[$Counter] = "XLogfile: X";
$ReplaceTexts[$Counter] =~ s/X/\$/g;
$Counter++;

$PlaceHolders[$Counter] = "SSDATE";
$ReplaceTexts[$Counter] = "XDate: X";
$ReplaceTexts[$Counter] =~ s/X/\$/g;
$Counter++;

$PlaceHolders[$Counter] = "SSREVISION";
$ReplaceTexts[$Counter] = "XRevision: X";
$ReplaceTexts[$Counter] =~ s/X/\$/g;
$Counter++;

$PlaceHolders[$Counter] = "SSHISTORY";
$ReplaceTexts[$Counter] = "XHistory: X";
$ReplaceTexts[$Counter] =~ s/X/\$/g;
$Counter++;

while($Line = <REPLACE>)
{
    $Line =~ s/\n//;

    ($PH, $RT) = split(/=/, $Line);
    if($PH ne "")
    {
#        print $PH, $RT, "\n";
        $PlaceHolders[$Counter] = $PH;
        $ReplaceTexts[$Counter] = $RT;
        $Counter++;
    }
}
print "$Counter Replacements\n";
if($Counter > 0)
{
    ReplaceRecursive($ARGV[1]);
}

sub ReplaceRecursive
{
    local($basedir) = @_;
    my $i =1;
    my @dirs;
    my $file;
    my $newfile;

    opendir(xxDIR, $basedir);
    while($file = readdir(xxDIR)) 
    {
      if($file ne "." && $file ne "..")
      {
          if(-d "$basedir\\$file")
          {
             $dirs[$i++] = "$basedir\\$file";
          }
          else
          {
              DoReplacements("$basedir\\$file");
          }
      }
    }
    close xxDIR;

    for($i=1; $i<=$#dirs; $i++)
    {
        ReplaceRecursive($dirs[$i]);
    }
}

sub DoReplacements
{
    local($filename) = @_;
    my $content;
    my $ph;
    my $rt;
    
    open(SOURCE, $filename);
    sysread(SOURCE, $content, 2000000);
    close SOURCE;

    for($count = 0; $count <= $#PlaceHolders; ++$count)
    {
        $ph = $PlaceHolders[$count];
        $rt = $ReplaceTexts[$count];
        $content =~ s/\[\*$ph\]/$rt/ge;
    }

    if($content =~ /\[\*[A-Z]{2,4}\]/)
    {
        print "warning: dangling replacements in file '$filename'\n";
    }

    open(SOURCE,"> $filename");
    syswrite(SOURCE, $content, 2000000);
    close SOURCE;
}


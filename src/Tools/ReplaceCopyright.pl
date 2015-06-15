#!perl

if($ARGV[0] eq "" || $ARGV[1] eq "")
{
    die "usage: ReplaceCopyright <FileName> <CopyRightString>\n";
}

my $linecount = 0;
my $copyrightstring = $ARGV[1];
    
open(REPLACEFILE, $ARGV[0]);

while($_ = <REPLACEFILE>)
{
    $linecount++;
    if($_ !~ /Copyright \(c\) .*Softing/)
    {
        print;
    }
    else
    {
        if($ARGV[0] =~ /\.[Aa][Ss][Pp]$/)
        {
            print "<% //", $copyrightstring, " %>\n";
        }
        else
        {
            print "\(\* ", $copyrightstring, " \*\)\n";
        }
    }
}


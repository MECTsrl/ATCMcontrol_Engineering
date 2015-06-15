#!perl

if($ARGV[0] eq "")
{
    die "usage: TASetup <SetupFile>\n";
}
my $guid;

open(SETUPFILE, $ARGV[0]) || die "Could not open input file: '$ARGV[0]'\n";

while($_ = <SETUPFILE>)
{
    if($_ =~ /\[\*GUIDGEN\]/)
    {
        $guid = `uuidgen`;
        chop($guid);
        $guid = "\U$guid";
        $_ =~ s/\[\*GUIDGEN\]/$guid/e;
    }
    print;
}

close SETUPFILE;



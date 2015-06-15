#!perl

if($ARGV[0] eq "")
{
    die "usage: TAReplace <BaseDirectory>\n";
}

DeleteRecursive($ARGV[0]);

sub DeleteRecursive
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
                if($file eq "vssver.scc" || $file eq "mssccprj.scc")
                {
                    system("del $basedir\\$file");
                }
            }
        }
    }
    close xxDIR;

    for($i=1; $i<=$#dirs; $i++)
    {
        DeleteRecursive($dirs[$i]);
    }
}


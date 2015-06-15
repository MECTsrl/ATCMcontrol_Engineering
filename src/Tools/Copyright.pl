#!perl

if($ARGV[0] eq "" || $ARGV[1] eq "")
{
    die "usage: CopyRightReplace <BaseDirectory> <CopyRightString>\n";
}

ReplaceRecursive($ARGV[0], $ARGV[1], $ARGV[2]);

sub ReplaceRecursive
{
    local($basedir, $replacestring, $mod) = @_;
    my $i =1;
    my @dirs;
    my $file;

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
              if($file !~ /\.txt/ && $file !~ /\.scc/)
              {
                  DoReplacements("$basedir\\$file", $replacestring, $mod);
              }
          }
      }
    }
    close xxDIR;

    for($i=1; $i<=$#dirs; $i++)
    {
        ReplaceRecursive($dirs[$i], $replacestring, $mod)
    }
}


sub DoReplacements
{
    local($filename, $replacestring, $mod) = @_;
    
    my $count = 0;
    my $linecount = 0;
    my $tmpfile = "C:\\Tmp\\xxx.txt";


    if($filename =~ /\.[Ss][Tt]$/ ||
       $filename =~ /\.[Cc][Ss][Tt]$/ ||
       $filename =~ /\.[Ss][Dd][Tt]$/ ||
       $filename =~ /\.[Aa][Ss][Pp]$/)
    {
        print $filename, ":\n";
        if($mod eq "")
        {
            system("del $tmpfile");
            system("ReplaceCopyright.pl $filename \"$replacestring\" >> $tmpfile");
            system("copy $tmpfile $filename");
        }
        else
        {
            open(REPLACEFILE, $filename);
            while($_ = <REPLACEFILE>)
            {
                $linecount++;
                if($_ =~ /[Cc]opyright/)
                {
                    $count++;
                    if($mod ne "")
                    {
                        print " ", $linecount, ": ", $_;
                    }
                }
            }
            if($mod eq "-s")
            {
                print "  ", $count, " occurrences\n";
            }
        }
    }
}


#!perl

if($ARGV[0] ne "" && $ARGV[1] ne "" && $ARGV[2] ne "")
{
    RenameRecursive($ARGV[0], $ARGV[1], $ARGV[2]);
}
else
{
    print "usage: TARename <placeholder> <replace string> <base directory>\n";
}

sub RenameRecursive
{
    local($placeholder, $replace, $basedir) = @_;
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
              if($file =~ $placeholder)
              {
                  $newfile = $file;
                  $newfile =~ s/$placeholder/$replace/e;
                  rename "$basedir\\$file", "$basedir\\$newfile";
       	          print "renamed $file\n";
       	      }
          }
      }
    }
    close xxDIR;

    for($i=1; $i<=$#dirs; $i++)
    {
        RenameRecursive($placeholder, $replace, $dirs[$i]);
    }
}


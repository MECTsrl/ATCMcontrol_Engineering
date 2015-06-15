#!perl

if($ARGV[0] ne "")
{
    EmptySSFieldsRecursive($ARGV[0]);
}
else
{
    print "usage: TAEmptySSFields <base directory>\n";
}

sub EmptySSFieldsRecursive
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
              system("EmptySSFields $basedir\\$file");
          }
      }
    }
    close xxDIR;

    for($i=1; $i<=$#dirs; $i++)
    {
        EmptySSFieldsRecursive($placeholder, $replace, $dirs[$i]);
    }
}


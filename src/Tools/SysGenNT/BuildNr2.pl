# buildnr.pl
# increments PRODUCT_BUILD define

unlink "BuildNr.h~";
rename ("BuildNr.h", "buildnr.h~") or die "can't rename buildnr.h";
open (IN, "<BuildNr.h~") or die "can't open buildnr.h~";
open (OUT, ">BuildNr.h") or die "can't open buildnr.h";
open (BAT, ">BuildNr.bat") or die "can't open buildnr.bat";

while (<IN>) {
  if (/^\s*#\s*define\s+PRODUCT_BUILD\s+([0-9]+)\s*$/) {
    $buildnr = $1;
    print OUT "#define PRODUCT_BUILD $buildnr\n";
    print BAT "SET BUILDNUMBER=$buildnr\n";
    print "BUILDNUMBER=$buildnr\n";
  }
  else {
    print OUT;
  }
}

close IN;
close OUT;
close BAT;
unlink "BuildNr.h~";

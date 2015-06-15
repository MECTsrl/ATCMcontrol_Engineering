# basebuildnr.pl
# copies PRODUCT_BUILD define to BASE_BUILD_NR define


open (IN, "<softing\\fc\\res\\buildnr.h") or die "can't open buildnr.h";
open (OUT, ">softing\\fc\\inc\\basebuildnr.h") or die "can't open basebuildnr.h";


while (<IN>) {
  if (/^\s*#\s*define\s+PRODUCT_BUILD\s+([0-9]+)\s*$/) {
    $buildnr = $1;
    print OUT "#define BASE_BUILD_NR $buildnr\n";
  }
  else {
    print OUT;
  }
}

close IN;
close OUT;



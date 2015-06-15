#================================================================================
#================================================================================



require 5.003;

use strict;

error("too few args @ARGV") if  @ARGV < 1;

my $mode = shift(@ARGV);
error("first arg must be \"on\" or \"off\"") unless $mode eq "on" || $mode eq "off";

my @files = split("\n", `dir /b @ARGV`);

#print join("\n", (@files." FILES:", @files, ""));

my $pat_begin;
my $pat_end;

my $text_begin;
my $text_end;

if ( $mode eq "on" )
{
#	$pat_begin  = '\/\*_CPDEBUG_BEGIN(?!\*\/)';
#	$pat_end    = '(^|[^\/][^\*])_CPDEBUG_END\*\/';
#	$text_begin = '/*_CBDEBUG_BEGIN*/';
#	$text_end   = '/*_CBDEBUG_END*/';
}
else
{
	$pat_begin  = '\/\*_CPDEBUG_BEGIN\*\/';
	$pat_end    = '\/\*_CPDEBUG_END\*\/';
	$text_begin = '/*_CBDEBUG_BEGIN';
	$text_end   = '_CBDEBUG_END*/';
}

$| = 1;

foreach(sort(@files))
{
	my $file = $_;
	my $filetext = "";
        print "FILE: $file ...";
#	if ( open(FILE, "<$file") )
#	{
#		while(<FILE>)
#		{
#			my $line = $_;
#			$line =~ s/$pat_begin/$text_begin/g;
#			$line =~ s/$pat_end/$text_end/g;
#			$filetext .= $line;
#			print $line;
#		}
#		close FILE;
#		error("Could not write to file $file") unless open(FILE, ">$file");
#		print FILE $filetext;
#		close FILE;
#	    print "... OK\n";
#	}
#	else
#	{
#		print "could not open file $_";
#	}
}



sub error($)
{
	my $msg = shift;
	print "$msg\n";
	exit 1;
}

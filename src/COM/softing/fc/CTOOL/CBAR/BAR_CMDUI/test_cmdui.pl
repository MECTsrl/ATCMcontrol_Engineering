use strict;

$| = 1;

my $cmd = "4cbar";

my $test_log_dash  = do_test($cmd, "-");
my $test_log_slash = do_test($cmd, "/");

my $diffs = compare_test_logs($test_log_dash, $test_log_slash);

my $test_log_html = "test_bar_cmdui.html";
finish(1, "could not open file $test_log_html for writing") unless open(HTML, ">$test_log_html");
	    
print HTML "<HTML>\n";
print HTML "<HEAD><TITLE> Test BAR_CMDUI syntax errors </TITLE></HEAD>\n";
print HTML "<BODY>\n";

print HTML "<P>Error: calling option with dash(-) or slash(/) produced different output.<P>" if @$diffs >  0;
print HTML "<P>OK:    calling option with dash(-) or slash(/) produced same output.<P>"      if @$diffs <= 0;

print HTML "<TABLE BORDER=1>\n";

print HTML "<TR><TH>ID</TH><TH>Testcase</TH><TH>Output</TH></TR>\n";

my $i = 0;
for($i = 0; $i < @$test_log_dash; $i++)
{
    my ($args, $output) = @{$test_log_dash->[$i]};
    $output =~ s/</&lt;/g;
    $output =~ s/>/&gt;/g;
    $output =~ s/ /&nbsp;/g;
    $output = join("<BR>\n", split(/\n/, $output));
    print HTML "<TR>\n";
    print HTML "<TD VALIGN=TOP><CODE>$i</CODE></TD>\n";
    print HTML "<TD VALIGN=TOP><CODE>$args</CODE></TD>\n";
    print HTML "<TD VALIGN=TOP><CODE>$output<CODE></TD>\n";
    print HTML "</TR>\n";
}

print HTML "</TABLE>\n";

if ( @$diffs )
{
    print HTML "<P><TABLE BORDER=1>\n";
    print HTML "<TR><TH>Testcase</TH><TH>-option</TH><TH>/option</TH></TR>\n";
    $i = 0;
    for( $i = 0; $i < @$diffs; $i++)
    {
	my ($args, $dash_out, $slash_out) = @{$diffs->[$i]};
	$dash_out =~ s/</&lt;/g;
	$dash_out =~ s/>/&gt;/g;
	$dash_out =~ s/ /&nbsp;/g;
	$dash_out = join("<BR>\n", split(/\n/, $dash_out));
	$slash_out =~ s/</&lt;/g;
	$slash_out =~ s/>/&gt;/g;
	$slash_out =~ s/ /&nbsp;/g;
	$slash_out = join("<BR>\n", split(/\n/, $slash_out));
	print HTML "<TR>\n";
	print HTML "<TD VALIGN=TOP><CODE>$args</CODE></TD>\n";
	print HTML "<TD VALIGN=TOP><CODE>$dash_out<CODE></TD>\n";
	print HTML "<TD VALIGN=TOP><CODE>$slash_out</CODE></TD>\n";
	print HTML "</TR>\n";
    }
}
    print HTML "</TABLE>\n";

print HTML "</BODY>\n";
print HTML "</HTML>\n";

close HTML;
print "generated output file is: $test_log_html\n";

sub finish($$)
{
    my $foo = shift;
    my $msg = shift;
    print $msg;
    exit $foo;
}

#================================================================================
sub compare_test_logs($$)
{
    my $test_log_dash  = shift;
    my $test_log_slash = shift;

    my $differences = [];
    my $i = 0;
    for($i = 0; $i < @$test_log_dash; $i++)
    {
	my $args = $test_log_dash->[$i];
	my $dash_out  = $test_log_dash->[$i]->[1];
	my $slash_out = $test_log_slash->[$i]->[1];
	push(@$differences, [$args, $dash_out, $slash_out]) unless $dash_out eq $slash_out;
    }
    return $differences;
}

#================================================================================
sub do_test($$)
{
    my $cmd = shift;
    my $option_prefix = shift;

    my $test_log = [];

    my $test_cases = get_test_cases($option_prefix);
    foreach(@$test_cases)
    {
	my $args = $_;
	my $cmd_line = "$cmd $args";
	open(CBAR, "$cmd_line |");
	my @out = <CBAR>;
	push(@$test_log, [$args, join("", @out)]);
	close(CBAR);
	print ".";
    }
    print "\n";
    return $test_log;
}

#================================================================================
sub get_test_cases($)
{
    my $opt_prefix = shift;

    my $opt_h         = $opt_prefix . "?";
    my $opt_help      = $opt_prefix . "help";
    my $opt_config    = $opt_prefix . "config";
    my $opt_project   = $opt_prefix . "project";
    my $opt_backup    = $opt_prefix . "backup";
    my $opt_restore   = $opt_prefix . "restore";
    my $opt_control   = $opt_prefix . "control";
    my $opt_file      = $opt_prefix . "file";
    my $opt_nologo    = $opt_prefix . "nologo";
    my $opt_password  = $opt_prefix . "password";
    my $opt_overwrite = $opt_prefix . "overwrite";
    my $opt_invalid   = $opt_prefix . "invalidoption";

    return [
	    #=== branches that are not yet implemented
	    "-config w-ff -backup test_cfg.arc -password geheim",
	    "-config w-ff -restore test_cfg.arc -password geheim",
	    "-project c:\\programme\\4control\\demo\\intelligentlights -backup w-ff -control -password geheim",
	    "-project c:\\programme\\4control\\demo\\intelligentlights -backup test_cfg.arc -file -password geheim",
	    "-project c:\\programme\\4control\\demo\\intelligentlights -restore w-ff -control -password geheim",
	    "-project c:\\programme\\4control\\demo\\intelligentlights -restore test_cfg.arc -file -password geheim",

	    #=== printing help
	    "$opt_h",
	    "$opt_help",

	    "$opt_h $opt_config w-ff $opt_backup test.arc",
	    "$opt_h $opt_config w-ff $opt_restore test.arc",

	    "$opt_h $opt_project w-ff $opt_backup test.arc -file",
	    "$opt_h $opt_project w-ff $opt_restore test.arc -file",
	    #=== invalid options
	    "$opt_invalid",

	    #=== missing arguments or required options
	    "$opt_backup test.arc",
	    "$opt_restore test.arc",
	    "$opt_control",
	    "$opt_file",
	    "$opt_nologo",
	    "$opt_password topsecret",
	    "$opt_overwrite",

	    "$opt_config",
	    "$opt_config w-ff",
	    "$opt_config w-ff $opt_backup",
	    "$opt_config w-ff $opt_backup test.arc -password",
	    "$opt_config w-ff $opt_restore",
	    "$opt_config w-ff $opt_restore test.arc -password",

	    "$opt_project w-ff $opt_backup",
	    "$opt_project w-ff $opt_backup w-ff",
	    "$opt_project w-ff $opt_backup w-ff -control -password",
	    "$opt_project w-ff $opt_backup test.arc -file -password",
	    "$opt_project w-ff $opt_restore",
	    "$opt_project w-ff $opt_restore w-ff",
	    "$opt_project w-ff $opt_restore w-ff -control -password",
	    "$opt_project w-ff $opt_restore test.arc -file -password",

	    #=== unexpected arguments
	    "foo",

	    "foo $opt_config w-ff $opt_backup test.arc $opt_password topsecret $opt_overwrite $opt_nologo",
	    "$opt_config w-ff foo $opt_backup test.arc $opt_password topsecret $opt_overwrite $opt_nologo",
	    "$opt_config w-ff $opt_backup test.arc foo $opt_password topsecret $opt_overwrite $opt_nologo",
	    "$opt_config w-ff $opt_backup test.arc $opt_password topsecret foo $opt_overwrite $opt_nologo",
	    "$opt_config w-ff $opt_backup test.arc $opt_password topsecret $opt_overwrite foo $opt_nologo",
	    "$opt_config w-ff $opt_backup test.arc $opt_password topsecret $opt_overwrite $opt_nologo foo",

	    "foo $opt_config w-ff $opt_restore test.arc $opt_password topsecret $opt_overwrite $opt_nologo",
	    "$opt_config w-ff foo $opt_restore test.arc $opt_password topsecret $opt_overwrite $opt_nologo",
	    "$opt_config w-ff $opt_restore test.arc foo $opt_password topsecret $opt_overwrite $opt_nologo",
	    "$opt_config w-ff $opt_restore test.arc $opt_password topsecret foo $opt_overwrite $opt_nologo",
	    "$opt_config w-ff $opt_restore test.arc $opt_password topsecret $opt_overwrite foo $opt_nologo",
	    "$opt_config w-ff $opt_restore test.arc $opt_password topsecret $opt_overwrite $opt_nologo foo",

	    "foo $opt_project any\\dir $opt_backup test.arc $opt_file $opt_password topsecret $opt_overwrite $opt_nologo",
	    "$opt_project any\\dir foo $opt_backup test.arc $opt_file $opt_password topsecret $opt_overwrite $opt_nologo",
	    "$opt_project any\\dir $opt_backup test.arc foo $opt_file $opt_password topsecret $opt_overwrite $opt_nologo",
	    "$opt_project any\\dir $opt_backup test.arc $opt_file foo $opt_password topsecret $opt_overwrite $opt_nologo",
	    "$opt_project any\\dir $opt_backup test.arc $opt_file $opt_password topsecret foo $opt_overwrite $opt_nologo",
	    "$opt_project any\\dir $opt_backup test.arc $opt_file $opt_password topsecret $opt_overwrite foo $opt_nologo",
	    "$opt_project any\\dir $opt_backup test.arc $opt_file $opt_password topsecret $opt_overwrite $opt_nologo foo",

	    "foo $opt_project any\\dir $opt_restore test.arc $opt_file $opt_password topsecret $opt_overwrite $opt_nologo",
	    "$opt_project any\\dir foo $opt_restore test.arc $opt_file $opt_password topsecret $opt_overwrite $opt_nologo",
	    "$opt_project any\\dir $opt_restore test.arc foo $opt_file $opt_password topsecret $opt_overwrite $opt_nologo",
	    "$opt_project any\\dir $opt_restore test.arc $opt_file foo $opt_password topsecret $opt_overwrite $opt_nologo",
	    "$opt_project any\\dir $opt_restore test.arc $opt_file $opt_password topsecret foo $opt_overwrite $opt_nologo",
	    "$opt_project any\\dir $opt_restore test.arc $opt_file $opt_password topsecret $opt_overwrite foo $opt_nologo",
	    "$opt_project any\\dir $opt_restore test.arc $opt_file $opt_password topsecret $opt_overwrite $opt_nologo foo",

	    "foo $opt_project any\\dir $opt_backup w-ff $opt_control $opt_password topsecret $opt_overwrite $opt_nologo",
	    "$opt_project any\\dir foo $opt_backup w-ff $opt_control $opt_password topsecret $opt_overwrite $opt_nologo",
	    "$opt_project any\\dir $opt_backup w-ff foo $opt_control $opt_password topsecret $opt_overwrite $opt_nologo",
	    "$opt_project any\\dir $opt_backup w-ff $opt_control foo $opt_password topsecret $opt_overwrite $opt_nologo",
	    "$opt_project any\\dir $opt_backup w-ff $opt_control $opt_password topsecret foo $opt_overwrite $opt_nologo",
	    "$opt_project any\\dir $opt_backup w-ff $opt_control $opt_password topsecret $opt_overwrite foo $opt_nologo",
	    "$opt_project any\\dir $opt_backup w-ff $opt_control $opt_password topsecret $opt_overwrite $opt_nologo foo",

	    "foo $opt_project any\\dir $opt_restore w-ff $opt_control $opt_password topsecret $opt_overwrite $opt_nologo",
	    "$opt_project any\\dir foo $opt_restore w-ff $opt_control $opt_password topsecret $opt_overwrite $opt_nologo",
	    "$opt_project any\\dir $opt_restore w-ff foo $opt_control $opt_password topsecret $opt_overwrite $opt_nologo",
	    "$opt_project any\\dir $opt_restore w-ff $opt_control foo $opt_password topsecret $opt_overwrite $opt_nologo",
	    "$opt_project any\\dir $opt_restore w-ff $opt_control $opt_password topsecret foo $opt_overwrite $opt_nologo",
	    "$opt_project any\\dir $opt_restore w-ff $opt_control $opt_password topsecret $opt_overwrite foo $opt_nologo",
	    "$opt_project any\\dir $opt_restore w-ff $opt_control $opt_password topsecret $opt_overwrite $opt_nologo foo",

	    #=== invalid option combinations
	    "$opt_config w-ff $opt_project any\\dir $opt_backup test.arc",
	    "$opt_config w-ff $opt_backup test.arc $opt_restore test.arc",
	    "$opt_config w-ff $opt_backup test.arc $opt_control",
	    "$opt_config w-ff $opt_backup test.arc $opt_file",

	    "$opt_project any\\dir $opt_config w-ff $opt_backup test.arc $opt_file",
	    "$opt_project any\\dir $opt_backup test.arc $opt_restore test.arc $opt_file",
	    "$opt_project any\\dir $opt_backup test.arc $opt_file $opt_control",
	    ];
}
	    
	    


#!perl
sub mydie;

@text        = ();         # helper array, stores the 4ch file char by char, used in 
                           # sub RemoveSTComments, sub UnLinkStringLiterals and sub SubsSysSTComments

eval 'exec

  C:\Build\depot\main\Apps\ActivePerl\msi\data\ActivePerl\Perl\bin/perl -S $0

  ${1+"$@"}'
    if $running_under_some_shell;
			# this emulates #! processing on NIH machines.
			# (remove #! line above if indigestible)

eval '$'.$1.'$2;' while $ARGV[0] =~ /^([A-Za-z_0-9]+=)(.*)/ && shift;
			# process any FOO=bar switches

$[ = 1;			# set array base to 1
$, = ' ';		# set output field separator
$\ = "\n";		# set output record separator

#check command line parameter:
$ARGV[0] or mydie "4Convert: error: command line: need <project>.4cp as command parameter\n";
getExt($ARGV[0]) eq "4cp" or mydie "4Convert: error: command line: argument $ARGV[0] must have extension '.4cp'\n"; 

$projfile = getname($ARGV[0]);
$projfileNoExt = getFileNameNoExt($projfile);
$projpathrel = getpath($ARGV[0]); 

#print $ARGV[0];
#print $projfile;
#print $projfileNoExt;
#print $projpathrel;

#disable UNC pathes, resaon system command does not work at all!!!
if($projpathrel =~ /^\\\\/)
{
    mydie "4Convert: error: UNC pathes are not supported: '$projpathrel'\n";
}

if(!($projpathrel eq "."))
{
    chdir($projpathrel) or mydie "4Convert: error: can't change working directory to '$projpathrel'\n"; 
}

opendir(xxDIR, ".");
@files = readdir(xxDIR);
closedir(xxDIR);
foreach $t (@files)
{
    if(-e $t && !-w $t)
    {
        mydie "4Convert: error: file '$t' is not writable, please make all project files writable and try again\n";
    }
}

#back up directory including date and time info:
BackUpProject() or mydie;

#remove gen dir:
if(-e "gen" && -d "gen")
{
   RmdirRecursiv("gen");
}


&analyze4cp($projfileNoExt .'.4cp');
&analyze4ch($projfileNoExt .'.4ch');
&gen4cp($projfileNoExt .'.4cp');
if ( $configs > 0 ) {
	if( $projfileNoExt =~ /^([A-Za-z]|_[A-Za-z0-9])(_?[A-Za-z0-9])*$/ ) {#legal IEC-Identifier
		&genConfig($projfileNoExt);
		}
	else {
		&genConfig('Configuration1');
		}
	}
&gen4cpc();
unlink $projfileNoExt .'.4ch';
unlink 'CEPrjSettings.4cs';
print "conversion done";
#my $ans = <STDIN>;
#die "\n";    

sub analyze4cp {
    local($cpfile) = @_;
    $line_nr = 0;

LINE: while (($l = &Getline3($cpfile),$getline_ok) > 0) {
	#	print l
	$line_nr++;
	if ($l =~ /^\[.+\]/) {
		$section = $l;
		next LINE;
		}
	if ($l =~ /^file=/i) {
	    $s = 'file=', $l =~ s/$s//;
	    $fname = $l;

	    if ($section eq "[EXTFILES]") {
		$class4ext{$line_nr} = $fname;
	    }
	    elsif ($fname =~ /\.4ch/i) {
		next LINE;
	    }
	    elsif ($fname =~ /\.4cs/i) {
		$class4cs{$line_nr} = $fname;
	    }
	    elsif ($fname =~ /\.gvl/i) {
		$classgvl{$line_nr} = $fname;
	    }
	    elsif ($fname =~ /\.con/i) {
		$classcon{$line_nr} = $fname;
	    $configs++;
		}
	    elsif ($fname =~ /\.cst/i) {
		$classcst{$line_nr} = $fname;
	    }
	    elsif ($fname =~ /\.sdt/i) {
		$classsdt{$line_nr} = $fname;
	    }
	    elsif ($fname =~ /\.txt/i) {
		$classtxt{$line_nr} = $fname;
	    }
	    elsif ($fname =~ /download\.list/i) {
		$download_list = 1;
	    }
	    elsif ($fname =~ /\.st/i) {
			if (($whatclass = &whatst($fname)) eq 'prg') {
				$classprg{$line_nr} = $fname;
			}
			elsif ($whatclass eq 'fb') {
				$classfb{$line_nr} = $fname;
			}
			elsif ($whatclass eq 'fun') {
				$classfun{$line_nr} = $fname;
			}
			elsif ($whatclass eq 'ST-ERROR') {
				print 'ERROR : Cant determine type of "' . $fname . '"' ;
				print 'make shure that the keywords PROGRAM or FUNCTION_BLOCK or FUNCTION are at the begin of line.';
			}
	    }
	    elsif ($fname =~ /\.4gr/i) {
			if (($whatclass = &what4gr($fname)) eq 'prg') {
				$classprg{$line_nr} = $fname;
			}
			elsif ($whatclass eq 'fb') {
				$classfb{$line_nr} = $fname;
			}
			elsif ($whatclass eq 'fun') {
				$classfun{$line_nr} = $fname;
			}
			elsif ($whatclass eq 'GR-ERROR') {
				print 'ERROR : Cant determine type of "' . $fname . '"' ;
			}
	    }
		else {
			$otherFile{$line_nr} = $l;
		}

	}

	elsif ($l =~ /^GUID=/i) {
	    $cp_guid = $l;
	}
	elsif ($l =~ /^Alias=/i) {
	    $cp_Alias = $l;
	}
	elsif ($l =~ /^UserText1=/i) {
	    $cp_UserText1 = $l;
	}
	elsif ($l =~ /^UserText2=/i) {
	    $cp_UserText2 = $l;
	}
	elsif ($l =~ /^UserText3=/i) {
	    $cp_UserText3 = $l;
	}
	elsif ($l =~ /^UserURL=/i) {
	    $cp_UserURL = $l;
	}
	elsif ($l =~ /^version=/i) {
	    if ($l !~ /V1\.2/i) {
#			rollback($cpfile);
			mydie 'only Version 1.2 format is supported';
			last line;
	    }
	}
	elsif ($l =~ /^lib=/i) {
	    $s = 'lib=', $l =~ s/$s//;
		if($l =~ /[\/\\]+/ )
			{
			$class4cl{$line_nr} = $l;
			}
		else
		    {
			$class4cl{$line_nr} = "\$(libdir)\\".$l;
			}
		
	}
	elsif ($l =~ /^;/) {
	    $comments{$line_nr} = $l;
	}
	elsif ($section eq "[MACHINES]") {
	    if( $l =~ /=/ ) {
			@M = split(/=/, $l, 9);
			$machines{$M[1]} = $M[2];
			#e.g. machines[PC1]=Config
			}
	}
	elsif ($l !~ /^[ \t]*$/) {
		print "unrecognized:" . $l;
	    $otherFile{$line_nr} = $l;
	}
    }
    delete $opened{$cpfile} && close($cpfile);
}

sub gen4cp {
    local($cpFile) = @_;
    &Pick('>', $cpFile) &&
	(print $fh '[Attributes]');
    &Pick('>', $cpFile) &&
	(print $fh $cp_guid);
    if ($cp_Alias ne "" ) {
		&Pick('>', $cpFile) &&
		(print $fh $cp_Alias);
		}
    if ($cp_UserText1 ne "" ) {
		&Pick('>', $cpFile) &&
		(print $fh $cp_UserText1);
		}
    if ($cp_UserText2 ne "" ) {
		&Pick('>', $cpFile) &&
		(print $fh $cp_UserText2);
		}
    if ($cp_UserText3 ne "" ) {
		&Pick('>', $cpFile) &&
		(print $fh $cp_UserText3);
		}
    if ($cp_UserURL ne "" ) {
		&Pick('>', $cpFile) &&
		(print $fh $cp_UserURL);
		}

	
    &Pick('>', $cpFile) &&
	(print $fh "\n[Format]");
    &Pick('>', $cpFile) &&
	(print $fh 'version=V2.0');

    &Pick('>', $cpFile) &&
	(print $fh "\n[Classes.Configurations]");
	if( $configs != 0 ) {
 		if( $projfileNoExt =~ /^([A-Za-z]|_[A-Za-z0-9])(_?[A-Za-z0-9])*$/ ) {#legal IEC-Identifier
			&Pick('>', $cpFile) &&
			(print $fh 'file=' . $projfileNoExt . '.con');
			}
		else {
			&Pick('>', $cpFile) &&
			(print $fh 'file=Configuration1.con');
			}
	}

#    &Pick('>', $cpFile) &&
#	(print $fh "\n;[Classes.4cs]");
#   foreach $n (keys %class4cs) {
#	&Pick('>', $cpFile) &&
#	    (print $fh ';file=' . $class4cs{$n});
#    }
    &Pick('>', $cpFile) &&
	(print $fh "\n[Classes.GVLs]");
    foreach $n (keys %classgvl) {
	&Pick('>', $cpFile) &&
	    (print $fh 'file=' . $classgvl{$n});
    }
    &Pick('>', $cpFile) &&
	(print $fh "\n[Classes.Constants]");
    foreach $n (keys %classcst) {
	&Pick('>', $cpFile) &&
	    (print $fh 'file=' . $classcst{$n});
    }
    &Pick('>', $cpFile) &&
	(print $fh "\n[Classes.DataTypes]");
    foreach $n (keys %classsdt) {
	&Pick('>', $cpFile) &&
	    (print $fh 'file=' . $classsdt{$n});
    }
    &Pick('>', $cpFile) &&
	(print $fh "\n[Classes.Programs]");
    foreach $n (keys %classprg) {
	&Pick('>', $cpFile) &&
	    (print $fh 'file=' . $classprg{$n});
    }
    &Pick('>', $cpFile) &&
	(print $fh "\n[Classes.FunctionBlocks]");
    foreach $n (keys %classfb) {
	&Pick('>', $cpFile) &&
	    (print $fh 'file=' . $classfb{$n});
    }
    &Pick('>', $cpFile) &&
	(print $fh "\n[Classes.Functions]");
    foreach $n (keys %classfun) {
	&Pick('>', $cpFile) &&
	    (print $fh 'file=' . $classfun{$n});
    }
    &Pick('>', $cpFile) &&
	(print $fh "\n[Libs]");
    foreach $n (keys %class4cl) {
	&Pick('>', $cpFile) &&
	    (print $fh 'file=' . $class4cl{$n});
		if( $class4cl{$n} =~ /4C_System\.4cl/i ) {
			&Pick('>', $cpFile) &&
				(print $fh 'file=$(LIBDIR)\4CPC_System.4cl' );
			&Pick('>', $cpFile) &&
				(print $fh 'file=$(LIBDIR)\4CFPC_System.4cl' );
		}
		if( $class4cl{$n} =~ /4C_Communication\.4cl/i ) {
			&Pick('>', $cpFile) &&
				(print $fh 'file=$(LIBDIR)\4C_Utility.4cl' );
		}

    }
    &Pick('>', $cpFile) &&
	(print $fh "\n[User_Help]");
    foreach $n (keys %classtxt) {
	&Pick('>', $cpFile) &&
	    (print $fh 'file=' . $classtxt{$n});
    }
    &Pick('>', $cpFile) &&
	(print $fh "\n[Targets]");
    foreach $n (keys %machines) {
	&Pick('>', $cpFile) &&
	    (print $fh 'file=' . $n . '.4cpc');
    }
    &Pick('>', $cpFile) &&
	(print $fh "\n[EXTFILES]");
    foreach $n (keys %class4ext ) {  
	&Pick('>', $cpFile) &&
	    (print $fh 'file=' . $class4ext{$n});
    }
	if($download_list == 1 ) {
	    open (INPUT, "download.list") or mydie "4Convert: error: can't open download.list\n";
		read INPUT, $l, 20000;
		close INPUT;
		print $fh $l;
		unlink 'download.list';
	}
    &Pick('>', $cpFile) &&
	(print $fh "\n;Comments");
    foreach $n (sort keys %comments) {
	&Pick('>', $cpFile) &&
	    (print $fh $comments{$n});
    }
    &Pick('>', $cpFile) &&
	(print $fh "\n;OtherLines");
    foreach $n (sort keys %otherFile) {
	&Pick('>', $cpFile) &&
	    (print $fh 'file=' . $otherFile{$n});
    }

}

sub gen4cpc {
    foreach $apc (keys %machines) {
	$pcFile = $apc . '.4cpc';
	&Pick('>', $pcFile) &&
	    (print $fh "<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
	&Pick('>', $pcFile) &&
	    (print $fh "<PC CONTROL=\"" . $PC{$apc} . "\"");
	&Pick('>', $pcFile) &&
	    (print $fh "\tCONSOLE=\"" . $PC{$apc} . "\"");
	&Pick('>', $pcFile) &&
	    (print $fh "\tRESOURCE=\"" . $ConfRes{$machines{$apc}} . "\"");
	&Pick('>', $pcFile) &&
	    (print $fh "\tNAME=\"" . $apc . "\" >");
	foreach $ioname (keys %{ $IOcomps{$apc} }) {
		&Pick('>', $pcFile) &&
			(print $fh "<FBUS LINK=\"". $ioname . "." . $IOcomps{$apc}{$ioname} . "\"/>");
		}
	#	for( io in IOcomps{$apc} )
	#		print "<FBUS LINK=\""io"."IOcomps[apc][io]"\"/>" >pcFile
	&Pick('>', $pcFile) &&
	    (print $fh '</PC>');
	delete $opened{$pcFile} && close($pcFile);
    }
}

sub genConfig {
    local($confFileNoExt) = @_;
	local $res;
	local $confFile = $confFileNoExt .'.con';
    foreach $n (keys %classcon) {
	&analyzeCon($classcon{$n});
    }
	&Pick('>', $confFile) &&
	(print $fh 'CONFIGURATION ' . $confFileNoExt);
#	(print $fh 'CONFIGURATION Configuration1');
    foreach $res (keys %ConfRes) {
	&Pick('>', $confFile) &&
	    (print $fh "\t#IMPORT \"". $ConfRes{$res} . '.res"');
    }


#	so nicht! weil es auch unassigned ressources gibt :
#    foreach $pc (keys %machines) {
#	&Pick('>', $confFile) &&
#	    (print $fh "\t#IMPORT ", $ConfRes{$machines{$pc}} . '.res');
#    }
    &Pick('>', $confFile) &&
	(print $fh 'END_CONFIGURATION');
}

sub whatst {
    local($stfile) = @_;
    $whatstRet = 'ST-ERROR';
	@filename = split(/\./, $stfile, 10);
    open (INPUT, $stfile) or mydie "4Convert: error: can't open '$stfile'\n";
    read INPUT, $l, 2000000;
    close INPUT;

    @text = split //, $l;
    RemoveSTComments();
#    UnLinkStringLiterals($old4CHFile);
#    SubsSysSTComments($old4CHFile);
    $l = join('', @text);+
	$l =~ s/\n/ /g;
	$l =~ s/native.*?end_native/ /ig; #remove native blocks
#	if ($l =~ /;/) {
#		$l = $`;
		$l =~ s/\s+/ /g;

		if ($l =~ /PROGRAM $filename[1]/i) {
			$whatstRet = 'prg';
			}
		elsif ($l =~ /FUNCTION_BLOCK $filename[1]/i) {
			$whatstRet = 'fb';
			}
		elsif ($l =~ /FUNCTION $filename[1]/i) {
			$whatstRet = 'fun';
			}
#    }
    $whatstRet;
}

sub what4gr {
    local($grfile) = @_;
    $what4grRet = 'GR-ERROR';
	@filename = split(/\./, $grfile, 10);
    open (INPUT, $grfile) or mydie "4Convert: error: can't open '$grfile'\n";
    read INPUT, $l, 2000000;
    close INPUT;
    
    @text = split //, $l;
    RemoveSTComments();
#    UnLinkStringLiterals($old4CHFile);
#    SubsSysSTComments($old4CHFile);
    $l = join('', @text);
	$l =~ s/\n/ /g;
	if ($l =~ /<!\[CDATA.*\]\]>/i) {
		$l = $&;
		$l =~ s/&#[0-9][0-9];/ /g;
		$l =~ s/\s+/ /g;
#print $l;
		if ($l =~ /PROGRAM $filename[1]/i) {
			$what4grRet = 'prg';
			}
		elsif ($l =~ /FUNCTION_BLOCK $filename[1]/i) {
			$what4grRet = 'fb';
			}
		elsif ($l =~ /FUNCTION $filename[1]/i) {
			$what4grRet = 'fun';
			}
		}

    $what4grRet;
}

sub splitXML {
    local($l, *Keys) = @_;
    $s = '>', $l =~ s/$s//;
    @atmp = split(' ', $l, 9999);
    foreach $t ($[ .. $#atmp) {
	if ($atmp[$t] =~ '=') {
	    @b = split(/=/, $atmp[$t], 9999);
	    $s = "\"", $b['2'] =~ s/$s//g;
	    $Keys{$b['1']} = $b['2'];
	}
    }
}


sub analyze4ch {
    local($chfile) = @_;
    $copyflag = 0;
	$copied =0;
    while (($l = &Getline3($chfile),$getline_ok) > 0) {
	$l =~ s/[ \t]+/ /g;
	$l =~ s/[ \t]*=[ \t]*/=/g;
#print $l;
	if ($l =~ /<PC NAME/) {
	    &splitXML($l, *Keys);
	    $pcname = $Keys{'NAME'};
	    $pcaddr = $Keys{'ADDR'};
	    $PC{$pcname} = $pcaddr;
	}
	if ($l =~ /<FBUS NAME/) {
	    &splitXML($l, *Keys);
	    $fbusname = $Keys{'NAME'};
	    $fbustype = $Keys{'TYPE'};
	    $channel  = $Keys{'CHANNEL'};
		$IOcomps{$pcname}{$fbusname}="io" . $fbustype ;
	    $fbusfile = $fbusname . '.io' . $fbustype;
   	    &Pick('>', $fbusfile) &&
		(print $fh "<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
		&Pick('>', $fbusfile) &&
		(print $fh "<".uc($fbustype)."_CONFIG CHANNEL_NR=\"$channel\">");
	}
	if ($copyflag == 1) {
			&Pick('>', $fbusfile) &&
			(print $fh $l);
			$copied++;
	}
	if ($l =~ /<\w+_CONFIG>/) {
#  	    $l =~ s/>//;
		$copyflag = 1;
	}
	if ($l =~ /<\/.*_CONFIG>/) {
	    $copyflag = 0;
	    delete $opened{$fbusfile} && close($fbusfile);
	}
	if ( ($l =~ /<\/FBUSDATA>/) && ($copied == 0)) {
   		&Pick('>', $fbusfile) &&
		(print $fh "</".uc($fbustype)."_CONFIG>");
		$copyflag = 0;
		delete $opened{$fbusfile} && close($fbusfile);
	}

    }
    delete $opened{$chfile} && close($chfile);
}

sub analyzeCon {
    local($confile) = @_;
    while (($l = &Getline3($confile),$getline_ok) > 0) {
	#print l
	if ($l =~ /CONFIGURATION/i && $l !~ /END_/i) {
	    @S = split(' ', $l, 9999);
	    $conf = $S[2];
	}
	if ($l =~ /RESOURCE/i && $l !~ /END_/i) {
	    @S = split(' ', $l, 9999);
	    $ConfRes{$conf} = $S[2];
	    $resFile = $S[2] . '.res';
	    &Pick('>', $resFile) &&
		(print $fh $S[1].'	'.$S[2]);
	    while (($l = &Getline3($confile),$getline_ok) > 0) {
		if ($l !~ /END_CONFIGURATION/i) {
		    &Pick('>', $resFile) &&
			(print $fh $l);
		}
		else {
		    delete $opened{$resFile} && close($resFile);
		    last;
		}
	    }
	}
    }
    delete $opened{$confile} && close($confile);
	unlink $confile;
}

sub Getline3 {
    &Pick('',@_);
    local($_);
    if ($getline_ok = (($_ = <$fh>) ne '')) {
	;
    }
    chomp;
	$_;
}

sub Pick {
    local($mode,$name,$pipe) = @_;
    $fh = $name;
    open($name,$mode.$name.$pipe) unless $opened{$name}++ ;
#	if( $! ) {
#		 die "ERROR : Cant open \"".$name."\"";
#		 }
}


sub BackUpProject
{
    my $sourcDir;
    my $mybackUpDir;
    $backUpDir = $projfileNoExt." BACKUP ".localtime;
#	$backUpDir = $projfileNoExt."V1";
	$mybackUpDir = "..\\".$backUpDir;
    $mybackUpDir =~ s/:/-/g;
    $mybackUpDir =~ s/ +/_/g;

    if($projpathrel ne ".")
    {
        $backUpDirAndPath = "$projpathrel\\$mybackUpDir";
    }
    else
    {
        $backUpDirAndPath = $mybackUpDir;
    }
    $sourcDir = ".";
#print "BackUpProject: create backup in '$mybackUpDir'";

    !stat($mybackUpDir) or mydie "4Convert: error: backup directory '$mybackUpDir' already exists\n";

    MyXcopy($sourcDir, $mybackUpDir) or mydie  "4Convert: error: xcopy failed\n";
    return 1;
}



sub MyXcopy
{
    local($src,$dest) = @_;
    my $i =1;
    my @dirs;
    my $file;

    #be save:
    if(! -d $src) 
    {
        mydie "4Convert: Xcopy: '$src' is not a directory\n";
    }
    if(-e $dest && !-d $dest) 
    {
        mydie "4Convert: Xcopy: '$dest' is not a directory\n";
    }
    
    if(! -d $dest)
    {
        mkdir($dest, umask) or mydie "4Convert: error: faild to create directory '$dest': $!\n";
    }


    opendir(xxDIR, $src);
    while($file = readdir(xxDIR)) 
    {
      if($file ne "." && $file ne "..")
      {
          if(-d "$src\\$file")
          {
              $dirs[$i++] = $file;
          }
          else
          {
              copyFile("$src\\$file" , "$dest\\$file") or return 0;
          }
      }
    }
    close xxDIR;

    for($i=1; $i<=$#dirs; $i++)
    {
        MyXcopy("$src\\$dirs[$i]", "$dest\\$dirs[$i]") or return 0;
    }

    return 1;
}


sub copyFile
{
    local($src,$dest) = @_;
    my $content;
    my $size;

    ($dev,$ino,$mode,$nlink,$uid,$gid,$rdev,$size,
     $atime,$mtime,$ctime,$blksize,$blocks)  = stat($src);

#print "copy : $src  $dest\n";

    open(COPYFILE, $src) or mydie "4Convert: error: can't open '$src'\n";
    binmode COPYFILE;
    $size==sysread(COPYFILE, $content, $size) or mydie "4Convert: error: can't read file '$src'\n";
    close COPYFILE;

    open(COPYFILE, ">$dest") or mydie "4Convert: error: can't open '$dest' for writing\n";
    binmode COPYFILE;
    $size==syswrite(COPYFILE, $content, $size) or mydie "4Convert: error: can't write file '$dest'\n";
    close COPYFILE;
    
    return 1 if stat($dest);
    return 0;
}


sub RmdirRecursiv
{
    local($basedir) = @_;
    my $i =1;
    my @dirs;
    my $file;

    #be save:
#    if(!($basedir =~ m{^gen})) 
#    {
#        mydie "4Convert: internal error: illegal directory in RmdirRecursiv\n";
#    }
#print $basedir;
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
#            print "unlink: $basedir\\$file\n";
             unlink "$basedir\\$file";
          }
      }
    }
    close xxDIR;

    for($i=1; $i<=$#dirs; $i++)
    {
        RmdirRecursiv($dirs[$i]);
    }

#	print "rmdir: $basedir\n";
    rmdir($basedir);
}

#sub rollback {
#    local($file2close) = @_;
#
#	if ($file2close ne "") {
#		close($file2close);
#	}
#	chdir("..");
#	RmdirRecursiv(".\\$projfileNoExt");
#print $backUpDir;
#print $projfileNoExt;
#	rename(	$backUpDir , $projfileNoExt);
#
#}

sub RemoveSTComments
{
    my $i = 0;
    my $j = 0;
    my $Nest;
    
    while($i<=$#text)
    {
       if($text[$i]eq'(' && $text[$i+1]eq'*' && $text[$i+2]ne'[' && $text[$i+3]ne'[')
       {
            $Nest++;
            $i += 2;
            NEST: while($Nest>0)
            {
                for(;;)
                {
                    if($i>$#text)
                    {
                        mydie "4Convert: error: source file '$fileName' contains unterminated '(* ... *)' comments\n";
                    }
                    elsif($text[$i]eq'*' && $text[$i+1]eq')')
                    {
                       $i+=2;
                       last;
                    }
                    elsif($text[$i]eq'(' && $text[$i+1]eq'*')
                    {
                        $i += 2;
                        $Nest++;
                        #print "NEST $Nest\n";
                        next NEST;
                    }
                    else
                    {
                        $i++; 
                    }
                }

                $Nest--;
            }
       }
       else
       {
           $text[$j++] = $text[$i++];
       }
    }
    $#text = --$j;

}

sub getpath{
    local($file) = @_;
	if ($file =~ m!^(.*)[\\/]([^\\/]*)$!) {
    return "\\" if $1 eq "";
    return $1;
  }
  return ".";
}

sub getname{
    local($file) = @_;
  if ($file =~ m!^(.*)[\\/]([^\\/]*)$!) {
    return $2;
  }
  return $file;
}

sub getExt
{
    local($file) = @_;
   
   if($file =~ m{\.([^\\/.]*)$})
   {
      return lc($1);
   }

   return "";
}

sub getFileNameNoExt
{
    local($file) = @_;
#   my $file= getname($_[0]);
   $file =~ s{\.([^\\/.]*)$}{};
   $file =~ s{\.$}{};
   return $file;
}

sub mydie 
{
    print $_[1];
#    if($BackUpCreated)
#    {
        print "\n";
        print "The conversion failed due to an unrecoverable error";
        print "Do the following:";
        print " 1) if there is a backup directory delete the directory with the original projectname";
        print " 2) rename the backup directory to the original projectname";
        print " 3) copy the project to Your own backup directory";
        print " 4) try to repair the error in the project (see error messages above)";
        print " 5) retry the conversion \n";
        print "WARNING: It is strongly recommended to keep Your own backup copy!";
#    }
#    print "\npress <enter> to continue...";
#    my $ans = <STDIN>;
    die "\n";    
}

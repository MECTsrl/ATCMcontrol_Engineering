<!--ATCM. All rights reserved-->
<!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML//EN">
<html>
<head>
<META NAME="GENERATOR" Content="Microsoft Developer Studio">
<META HTTP-EQUIV="Content-Type" content="text/html; charset=iso-8859-1">
<TITLE>Browser Check</TITLE>
<script language="JavaScript">
<!--
// first check browser
var Version = navigator.appVersion;
var MainVersionNumberWithPoint = new String(Version.match(/^\d*\./));
var MainVersionNumber = MainVersionNumberWithPoint.replace(/\./, "");

if ((navigator.appName.indexOf("Microsoft Internet Explorer") >= 0) &&
    (MainVersionNumber >= 4))
{
	document.location = "CP_CheckDisplay.asp";
}
// -->
</script>
</head>

<body bgcolor='#E0E0E0'>
<p align='center'><font color='#FF0000' size='5'><strong>Check failed !</strong></font></p>
<p align='center'>
	<font size='3' face='MS Sans Serif'>
		4C Console runs only inside the Microsoft Internet Explorer 4.0 or higher with
		active scripting enabled !
	</font>
</p>
</body>

</html>
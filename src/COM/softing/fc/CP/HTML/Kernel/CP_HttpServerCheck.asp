<!-- ATCM. All rights reserved -->
<HTML>
<HEAD>
<META NAME="GENERATOR" Content="Microsoft Developer Studio">
<META HTTP-EQUIV="Content-Type" content="text/html; charset=iso-8859-1">
<meta HTTP-EQUIV="Pragma" content="no-cache">

<!--
<%@ LANGUAGE = JScript %>
<%
// This server check is executed after the browser check. Thus all properties of IE4.0 can be used to
// display the results of this check.
// If this check would be executed before the browser check, the results of this check must be displayable
// in all existing browsers.
//
// Asp-scripts are placed within html comments to enable the browser to display an error message without becomming
// confused by the asp-script code, if the asp-scripts are not executed.
//
// HtmlOpenComment and HtmlCloseComment are used to generate html comment delimiter whithout using the delimiter 
// strings directly. If the asp-scripts are not executed, the strings would be interpreted as html comments delimiter 
// by the browser. Thus, some parts of the asp-script would not be included in the html comments.  
var HtmlOpenComment = "<" + "!" + "-" + "-";
var HtmlCloseComment = "-" + "-" + ">";
%>
-->

<TITLE>4C Server Check</TITLE>

<script language="JavaScript">
// This error message is used if the asp-scripts are not executed.
str = "" + document.location;
var strServer = str.substring(7, str.indexOf("/", 7));
var ErrMsg = "'Active Server Page' - Extension (ASP) is not installed on '" + strServer + "'";

// This function (executed on the browser) writes the error message contained in ErrMsg
// into the DIV-element ErrorMessage.
function launchErrorMessage()
{
	setTimeout("setErrorMessage()", 5000);
}

function setErrorMessage()
{
	IdErrorText.innerText = ErrMsg;
	IdErrorTitle.innerText = "Sorry !";
}
</script>

<!--
<%=HtmlCloseComment%>

<script language="JavaScript">
<%
var ServerSoftware = Request.ServerVariables("SERVER_SOFTWARE") + "";
var MainVersionNumberWithPoint = new String(ServerSoftware.match(/\d*\./));
var MainVersionNumber = MainVersionNumberWithPoint.replace(/\./, "");
var MinMainVersionNumber = 3;

if (((ServerSoftware.indexOf("Microsoft-PWS/") >= 0) || (ServerSoftware.indexOf("Microsoft-IIS/") >= 0)) &&
	(MainVersionNumber >= MinMainVersionNumber))
{
%>
	// The server is ok. Let the browser start the console.
	document.location = "CP_Login.asp";
<%
}
else
{
%>
	// This error message is used (by the browser)
	// if asp-scripts are executed but the server version is wrong.
	ErrMsg = "Wrong Server Software installed on '" + strServer + "'. Use Microsoft Peer Web Server <%=MinMainVersionNumber%>.0 or higher " +
	         "or Microsoft Internet Information Server <%=MinMainVersionNumber%>.0 or higher.";
<%
}
%> 
</script>

<%=HtmlOpenComment%>
-->

</HEAD>

<BODY bgcolor='#E0E0E0' onload='launchErrorMessage()'>
<p><font color='#FF0000' size='5'><strong><div id='IdErrorTitle' align='center'></div></strong></font></p>
<p><font color='#000000' size='2'><div id='IdErrorText'  align='center'></div></font></p>
</BODY>
</HTML>

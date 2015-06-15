<%@ LANGUAGE = JScript %>
<% //ATCM. All rights reserved %>
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibAspParam.asp" -->
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibSecurity.asp" -->
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibHtml.asp" -->
<%
Response.Buffer = true;
%>
<!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML//EN">
<html>
<head>
<%GenerateStdHeader("Login")%>
<%GenerateStdIncludes()%> 
</head>
<%
var bCheck = false;
if (readParam("check") != null && readParam("check") == 1)
	bCheck = true;

var url = readParam("url");
if (url == null)
	url = "/4c/CP_Main.asp"

var Params = "&" + __FpParams.getFaceplateParamString();
if (Params.length == 1)
	Params = ""
if (readParam("AspFile") != null)
	Params += "&AspFile=" + readParam("AspFile");

// identification can be send either via "get" or post
// right now, there is no user concept in 4C -> use dummy user "unknown" instead
var UserName = "unknown";
var Password = readFormParam("password");
if (Password == null)
	Password = readParam("password");

// create session

var SessionId = null;
if (bCheck || Password != null || !CP_isSecurityEnabled())
	SessionId = CP_StartSession(UserName, Password);

// start application if session has been started successfully
if (SessionId != null)
{%>
<script language="JavaScript">
 document.location.href = "<%=url%>?SessionId=<%=SessionId%><%=Params%>";
</script>	
<%}
// show login screen
else
{%>

<script language="JavaScript">
function init()
{
<%	
if (SessionId == null && (Password != null || bCheck))
{%>
alert("The system could not log you on.\n\n Letters in passwords must be typed using the correct case.\n Make sure that Caps Lock is not accidentally on.");
<%}%>
LAYOUTFORM.IdPassword.focus();
}
</script>

<BODY  BACKGROUND="/4c/images/CP_LoginBackground.gif" LINK="#0000FF" VLINK="#990000" TEXT="#000000" TOPMARGIN=0 LEFTMARGIN=0 MARGINWIDTH=0 MARGINHEIGHT=0 onload="setTimeout('init()', 500)">
<CENTER>
<FORM NAME="LAYOUTFORM" ACTION="/4c/CP_Login.asp?url=<%=url%><%=Params%>&check=1" METHOD=POST>
  <TABLE BORDER=0 CELLSPACING=0 CELLPADDING=0 WIDTH=394>
   <TR VALIGN="top" ALIGN="left">
	<TD HEIGHT =87></TD>
	<TD WIDTH=30><IMG SRC="/4c/images/CP_LoginClearPixel.gif" WIDTH =30 HEIGHT=1 BORDER=0></TD>
	<TD></TD>
	<TD WIDTH=22><IMG SRC="/4c/images/CP_LoginClearPixel.gif" WIDTH =22 HEIGHT=1 BORDER=0></TD>
	<TD></TD>
   </TR>
   <TR VALIGN="top" ALIGN="left">
	<TD WIDTH=394 HEIGHT =292 COLSPAN=5 ALIGN="left" VALIGN="top"><IMG id="Picture3" HEIGHT=292 WIDTH=394 SRC="/4c/images/CP_Login.jpg"  BORDER=0  ALT="ATCMControl Console-Passwortscreen" ></TD>
   </TR>
   <TR VALIGN="top" ALIGN="left">
	<TD COLSPAN=5 HEIGHT =21></TD>
   </TR>
   <TR VALIGN="top" ALIGN="left">
	<TD WIDTH=224 HEIGHT =22><INPUT TYPE="password" id="IdPassword"   NAME="password" VALUE="" SIZE=28" MAXLENGTH=28 ></TD>
	<TD></TD>
	<TD WIDTH=63 ROWSPAN=2>  <INPUT TYPE="reset"    id="IdReset" NAME="reset" VALUE="Reset"  ></TD>
	<TD></TD>
	<TD WIDTH=55 ROWSPAN=2>  <INPUT TYPE="submit"   id="IdEnter" NAME="enter" VALUE="Enter" ></TD>
   </TR>
   <TR VALIGN="top" ALIGN="left">
	<TD COLSPAN=2 HEIGHT =2></TD>
	<TD></TD>
   </TR>
  </TABLE>
</FORM>
</BODY>
<%}%>
</html>

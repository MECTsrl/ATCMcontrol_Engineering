<% // ATCM. All rights reserved %>

<%
//----------------------------------------------------------------------------------------
// Buffer page output to aviod that the http header is written prematurely.
// Thus you can redirect the browser at any time (to write error messages).
Response.Buffer = true;
%>

<%
//----------------------------------------------------------------------------------------
// Function to write an error message.
function writeError(ClassName, ObjectName, ErrorText)
{
	Response.Redirect(getErrorUrl(ClassName, ObjectName, ErrorText));
}

function writeObjNotFound(ClassName, ObjectName)
{
	Response.Redirect(getObjNotFoundUrl(ClassName, ObjectName));
}
//----------------------------------------------------------------------------------------
%>

<%
//----------------------------------------------------------------------------------------
// Function to get an url to an error message page.
function getObjNotFoundUrl(ClassName, ObjectName)
{
	return "/4C/CP_ObjNotFound.asp?" +
		   "ClassName=" + escape(ClassName) + 
		   "&ObjName=" + escape(ObjectName);
}

function getErrorUrl(ClassName, ObjectName, ErrorText)
{
	return "/4C/CP_Error.asp?" +
		   "ClassName=" + escape(ClassName) + 
		   "&ObjName=" + escape(ObjectName) + 
		   "&Text=" + escape(ErrorText);
}
%>

<% function GetVersionString() { return "##VERSION##"; } %>

<%
function GenerateStdHeader(Title)
{%>
<META NAME="GENERATOR" Content="Microsoft Developer Studio">
<meta HTTP-EQUIV="Content-Type" content="text/html; charset=iso-8859-1">
<meta HTTP-EQUIV="Pragma" content="no-cache">
<title>4C <%=Title%></title>
<%}%>

<%
// communication control (ActiveX)
function GenerateProxyObj()
{%>
<object codebase="/4C/CSC_Online.cab#version=<%=GetVersionString()%>" classid="clsid:ACB9AA53-8B04-11d5-8796-00010209D2BC" border="0" width="0" height="0"> </object>
<script language="VBScript"> 
set CpProxy = CreateObject("4C_20_CSC.AsyncOPC.1")
</script>
<%}%>

<%
// control for IEC 1131-3 data formating(ActiveX)
function GenerateBaseLibObj()
{%>
<object id="Baselib" codebase="/4C/BaseLib.cab#version=<%=GetVersionString()%>" classid="CLSID:5E919C50-F2A2-11D4-A537-0008C779C600" border="0" width="0" height="0"> </object>
<%}%>

<%
// body attributes for faceplates
function GenerateFaceplateBodyTag() 
{%> 
class=Faceplate LINK="<%=getColorHyperLink()%>" ALINK="<%=getColorHyperLink()%>" VLINK="<%=getColorHyperLink()%>" onload="Init()" onunload="Exit()";
<%}%>

<%
// remains for compatibility reasons
function GenerateLinkToTuningDisplay(){}
%>

<%
// standard includes for faceplates
function GenerateStdIncludes()
{%>
<link rel=stylesheet href="/4C/CP_Style.css" type="text/css">
<script language="JavaScript" src="/4C/CP_Panel.js"></script>
<script language="VBSCRIPT" src="/4C/CP_FPLib.inc"></script>
<script language="JavaScript" src="/4C/CP_FPLib.js"></script>
<%}%>

<%
// colors
function getBgColorFacplate() { return "#E0E0E0"; }
function getBgColorTextOut() { return "#C0C0C0"; }
function getColorHyperLink() { return "#0000FF"; }
function getColorOk() { return "#00FF00"; }
function getColorWarning() { return "#FFFF00"; }
function getColorError() { return "#FF0000"; }
%>

<%
// height of single text line of the CpStdOut Applet/ActiveX
function getSingTextOutHeight() { return 26; }
%>

<%
// functions for CpStdOut Control
// generate control
function GenerateStdOutControl(Name, Lines)
{
	GenerateStdOutControl2(Name, Lines, 180)
}%>

<%
function GenerateStdOutControl2(Name, Lines, Width)
{
%>
<applet
    code=CPStdOut.class cabbase="/4C/cpclasses.cab#version=<%=GetVersionString()%>"
	MAYSCRIPT
    name=<%=Name%> 
	width=<%=Width%>   
	height=<%=Lines * getSingTextOutHeight()%>>
	<param name=BackgroundColor value="<%=getBgColorFacplate()%>">
	<param name=BackgroundTextColor value="<%=getBgColorTextOut()%>">
	<param name=HyperlinkColor value="<%=getColorHyperLink()%>">
</applet>
<%}%>

<%
// generate event handler entry for write value dialog
function GenerateStdOutEventHandler(Name)
{%>
<script language="JavaScript">
function handleEventWriteValue(VarPath, VarType)
{
	if (parent.frames[0] != null)
		parent.frames[0].CP_WriteValue(VarPath, VarType);
}
</script>
<%}%>

<%
// generate initialization of the control
// (including error checking)
function GenerateStdOutInitialization(Name)
{%>
	<%=Name%>.setEventWriteValue("handleEventWriteValue");
<%}%>

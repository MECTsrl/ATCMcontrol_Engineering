<%@ LANGUAGE = JScript %>
<% //ATCM. All rights reserved %>
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibFaceplate.asp" -->
<%
var AspFile = readParam("AspFile");

// get faceplate parameters and remove 'AspFile' - parameter
var params = "" + Request.ServerVariables("QUERY_STRING");
params = params.split(/AspFile=[^&]*&?/).join("");

var BodyUrl = "";
var HeaderUrl = "/4C/CP_FPHeader.asp" + "?" + params;
if (FileExists(AspFile))
{
	BodyUrl = AspFile + "?" + params;	
	HeaderUrl += "&MainUrl=" + AspFile;
}
else
{
	BodyUrl = getObjNotFoundUrl("", __FpParams.getObjName());
}
%>

<%
if (__FpParams.isStandAlone())
{%>
<FRAMESET ROWS="18,*,0" width="100%" FRAMEBORDER="NO" FRAMESPACING="0">
	<FRAME SRC="<%=HeaderUrl%>" NORESIZE WIDTH="100%" SCROLLING="NO" FRAMEBORDER="NO" MARGINHEIGHT="0" MARGINWIDTH="0" STYLE="border-bottom: groove 2px white">
	<FRAME SRC="<%=BodyUrl%>" NORESIZE WIDTH="100%" FRAMEBORDER="NO" MARGINHEIGHT="0" MARGINWIDTH="0">
	<FRAME SRC="CP_KeepAlive.asp?SessionId=<%=__FpParams.getSessionId()%>" name="FC_KeepAlive" noresize scrolling="no" marginheight="0">
</FRAMESET>
<%}
else
{%>
<FRAMESET ROWS="18,*" width="100%" FRAMEBORDER="NO" FRAMESPACING="0">
	<FRAME SRC="<%=HeaderUrl%>" NORESIZE WIDTH="100%" SCROLLING="NO" FRAMEBORDER="NO" MARGINHEIGHT="0" MARGINWIDTH="0" STYLE="border-bottom: groove 2px white">
	<FRAME SRC="<%=BodyUrl%>" NORESIZE WIDTH="100%" FRAMEBORDER="NO" MARGINHEIGHT="0" MARGINWIDTH="0">
</FRAMESET>
<%}%>

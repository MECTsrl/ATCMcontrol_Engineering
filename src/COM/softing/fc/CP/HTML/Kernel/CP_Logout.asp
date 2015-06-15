<%@ LANGUAGE = JScript %>
<% //ATCM. All rights reserved %>
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibAspParam.asp" -->
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibSecurity.asp" -->
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibHtml.asp" -->
<%
	CP_deleteSession(__FpParams.getSessionId());

	var showlogin = readParam("ShowLogin");
	if (showlogin != null && parseInt(showlogin) == 1)
	{
		Response.Buffer = true;
		var Params = "" + Request.ServerVariables("QUERY_STRING");
		var url = readParam("url");
		if (url == null)
			Params += "&url=/4c/CP_Main.asp";

		Response.Redirect("/4c/CP_Login.asp?" + Params);
	}
%>

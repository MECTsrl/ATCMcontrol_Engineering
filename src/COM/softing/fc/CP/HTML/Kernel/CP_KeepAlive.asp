<%@ LANGUAGE = JScript %>
<% //ATCM. All rights reserved %>
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibAspParam.asp" -->
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibSecurity.asp" -->
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibHtml.asp" -->
<%CP_keepAlive(__FpParams.getSessionId()); %>
<HTML>
<HEAD>
<%GenerateStdHeader("KeepAlive")%>
<script language="JavaScript">
top.m_KeepAliveRights = "<%=CP_getSessionRights(__FpParams.getSessionId())%>";
</script>
</HEAD>
<body>
My Rights : <%=CP_getSessionRights(__FpParams.getSessionId())%><br>
All Ids   : <%=Application("CP_SESSION_IDS")%><br>
All Rights: <%=Application("CP_SESSION_RIGHTS")%><br>
</body>
</HTML>
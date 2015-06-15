<%@ LANGUAGE = JScript %>
<% //ATCM. All rights reserved %>
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibFaceplate.asp" -->
<% CheckFaceplateSecurity(); %>

<html>
<head>
<% 
GenerateStdHeader("Message History");
GenerateStdIncludes();
%>  
<script language="JavaScript">
var m_BodyLoaded = false;
</script>
</head>

<FRAMESET ROWS="*,34" width="100%" FRAMEBORDER="NO" FRAMESPACING="0">
<FRAME SRC="CP_MsgHistoryBody.asp?SessionId=<%=__FpParams.getSessionId()%>" NORESIZE WIDTH="100%" FRAMEBORDER="NO" MARGINHEIGHT="0" MARGINWIDTH="0">
<FRAME SRC="CP_MsgHistoryCmd.asp?SessionId=<%=__FpParams.getSessionId()%>" NORESIZE WIDTH="100%" SCROLLING="NO" FRAMEBORDER="NO" MARGINHEIGHT="0" MARGINWIDTH="0" STYLE="border-top: groove 2px white">
</FRAMESET>

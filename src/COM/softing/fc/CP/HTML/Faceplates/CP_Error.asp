<%@ LANGUAGE = JScript %>
<% //ATCM. All rights reserved %>
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibFaceplate.asp" -->
<%
	var ErrorText = readParam("Text");
	var ObjName = readParam("ObjName");
	var ClassName = readParam("ClassName");
	if (ErrorText == null)
		writeError(null, null, "Error in call of CP_Error.asp");
%>
<HTML>
<HEAD>
<%GenerateStdHeader("Error")%>
<%GenerateStdIncludes()%>
</HEAD>
<body class=Faceplate>
<br>
<p align="center"><font color="#FF0000" size="5"><strong>Error !</strong></font></p>
<p align="center"><font size="3" face="MS Sans Serif"><%=ErrorText%></font></p>
</body>

</HTML>

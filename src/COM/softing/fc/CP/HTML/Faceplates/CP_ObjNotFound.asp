<%@ LANGUAGE = JScript %>
<% //ATCM. All rights reserved %>
<!-- #INCLUDE VIRTUAL = "/4C/CP_Inst.inc" -->
<!-- #INCLUDE VIRTUAL = "/4C/CP_FPLib.asp" -->
<%
	var ObjName = readParam("ObjName");
	var ClassName = readParam("ClassName");
%>
<HTML>
<HEAD>
<%GenerateStdHeader("Error")%>
<%GenerateStdIncludes()%>
</HEAD>
<body class=Faceplate>
<br>
<p align="center"><font size="3">The object</font></p>
<p align="center"><font size="3" color="#FF0000"><%=ObjName%></font></p>
<p align="center"><font size="3">has been removed !</font></p>
</body>
</HTML>

<%@ LANGUAGE = JScript %>
<% //ATCM. All rights reserved %>
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibFaceplate.asp" -->
<% CheckFaceplateSecurity(); %>
<html>
<head>
<%GenerateStdHeader("Tuning Display")%>
<%GenerateStdIncludes()%>

<%
var VarNameArray = GenerateStdCommunication2(null, true, true);
%>
<script language="JavaScript">
function InitFaceplate()
{
<% 
if (VarNameArray.length > 0) 
{
	for (var VarIndex = 0; VarIndex < VarNameArray.length; VarIndex++) 
	{
		var Name = VarNameArray[VarIndex];
%>	CPStdOut.AddLine(_CP_PATH + "<%=Name%>", _CP_TYPE_<%=Name%>, _CP_RW_<%=Name%>, 0, 0x4 | 0x20);
<%	}
	GenerateStdOutInitialization("CPStdOut");
}%>
}

function UpdateFaceplate()
{<% 
for (var VarIndex = 0; VarIndex < VarNameArray.length; VarIndex++) {
	var Name = VarNameArray[VarIndex];%>
	CPStdOut.SetValue(<%=VarIndex%>, false, 0, _CP_VAL_<%=Name%>, _CP_QUAL_<%=Name%>);<%}%>
}
</script>
<% 
if (VarNameArray.length > 0) 
{
	GenerateStdOutEventHandler("CPStdOut");
}
%>
</head>
<% 
if (VarNameArray.length > 0) 
{%>
<body <%=GenerateFaceplateBodyTag()%> onresize="CPStdOut.Refresh()">
<%
 	GenerateProxyObj(); 
	GenerateStdOutControl2("CPStdOut", VarNameArray.length, "100%");
%>
<hr>
<%}
else
{%>
<body <%=GenerateFaceplateBodyTag()%>>
<%}%>
</body>
</html>

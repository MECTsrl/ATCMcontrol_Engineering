<%@ LANGUAGE = VBScript %>
<!--
-->
<object RUNAT=Server SCOPE=PAGE id="opcClient"
   classid="CLSID:0E673EB2-74C0-11D2-88DF-006097E1BE2F">
</object>
-->


<html>
<head>
<META HTTP-EQUIV="Content-Type" content="text/html; charset=iso-8859-1">
<!-- <link rel=stylesheet href="/4C/CP_Style.css" type="text/css"> -->

<title>Test Simple OPC control as ASP</title>

</head>
<body >
<%
    dim var1, var2
    var1 = "[w-je].__srt0.__system.dload.dloadGUID"
    var2 = "[w-je].__srt0.__memused"
'    dim opc
'    set opc = CreateObject("4Control_CSC.simpleOPC")
'    on error resume next
    opcClient.Subscribe(var1)
    opcClient.Subscribe(var2)
%>

<table BORDER=1 cellpadding="2" cellspacing="0" WIDTH=80%>
<tr>
<thead>
<th>Var-Name</th>
<th>Value</th>
</tr>
<tbody>
<tr>
    <td><%=var1%></td>
    <td><%=opcClient.Value(var1)%></td>
</tr>
<tr>
    <td><%=var2%></td>
    <td><%=opcClient.Value(var2)%></td>
</tr>
</table>

<% 
''    opcClient.Unsubscribe(var1)
''    opcClient.Unsubscribe(var2)
''    set opcClient = nothing
%>
</body>
</html>

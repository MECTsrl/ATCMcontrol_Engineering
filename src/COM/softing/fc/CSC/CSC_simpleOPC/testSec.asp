<%@ LANGUAGE = JScript %>

<%
var __SEC_ConfigurationPath = "[" + Request.ServerVariables('SERVER_NAME') + "]";
var __SEC_VarPwd    = __SEC_ConfigurationPath + ".__system.security.Password";
var __SEC_VarEnable = __SEC_ConfigurationPath + ".__system.security.isEnabled";
var __SEC_VarRights = __SEC_ConfigurationPath + ".__system.security.Rights";

var __SEC_OPCClient = Server.CreateObject("4Control_CSC.simpleOPC");
var __SEC_Enabled = null;
%>


<HTML>
<HEAD>
<META NAME="GENERATOR" Content="Microsoft Developer Studio">
<META HTTP-EQUIV="Content-Type" content="text/html; charset=iso-8859-1">
<TITLE>Document Title</TITLE>

<script language="JavaScript">

</script>

</HEAD>

<BODY >
<%
		__SEC_OPCClient.Subscribe(__SEC_VarEnable);
		__SEC_Enabled = __SEC_OPCClient.Value(__SEC_VarEnable);
		__SEC_OPCClient.Unsubscribe(__SEC_VarEnable);
%>

path = (<%=__SEC_VarEnable%>)<br>
bEnable = (<%=__SEC_Enabled%>)<br>

<%
		__SEC_OPCClient.Subscribe(__SEC_VarPwd);
		__SEC_OPCClient.Value(__SEC_VarPwd) = "abcd";
		
        __SEC_OPCClient.Subscribe(__SEC_VarRights);
		var Rights = __SEC_OPCClient.Value(__SEC_VarRights);
		__SEC_OPCClient.Unsubscribe(__SEC_VarRights);
%>
Rights = (<%=Rights%>)<br>

</BODY>
</HTML>

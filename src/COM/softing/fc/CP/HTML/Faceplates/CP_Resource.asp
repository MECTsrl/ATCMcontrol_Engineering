<%@ LANGUAGE = JScript %>
<% //ATCM. All rights reserved %>
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibFaceplate.asp" -->
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibButton.asp" -->
<% CheckFaceplateSecurity(); %>
<html>
<head>
<%GenerateStdHeader("Resource")%>
<%GenerateStdIncludes()%>
<%GenerateStdCommunication()%>
<% 
var bCanExecute = CP_canSessionExecute(__FpParams.getSessionId());
if (bCanExecute)
{
	GenerateButtonFunctions();
}
%>  
<script language="JavaScript">
function InitFaceplate()
{
	InitAllButtons();

    CPStdOut1.AddLine("Running", "DINT", "r", 2, 1);
    CPStdOut1.AddLine("Stopped", "DINT", "r", 1, 1);
    CPStdOut1.AddLine("Loading", "DINT", "r", 1, 1);
    CPStdOut1.AddLine("Error"  , "DINT", "r", 0, 1);

    CPStdOut1.AddLine("Memory (kB)", _CP_TYPE___memused    , "r", 0, 4);
    CPStdOut1.AddLine("Project"   , _CP_TYPE___projectname, "r", 0, 4);

	<%GenerateStdOutInitialization("CPStdOut1");%>
		
	IdListLink.href = GetParentUrl();
}

function UpdateFaceplate()
{
	if (_CP_QUAL___state != 0)
		_CP_VAL___state = -2;  // all LEDs off

	CPStdOut1.SetValue(0, _CP_VAL_NATIVE___state == 2 , _CP_QUAL___state, "", _CP_QUAL___state);
	CPStdOut1.SetValue(1, _CP_VAL_NATIVE___state == 1 , _CP_QUAL___state, "", _CP_QUAL___state);
	CPStdOut1.SetValue(2, _CP_VAL_NATIVE___state == 0 , _CP_QUAL___state, "", _CP_QUAL___state);
	CPStdOut1.SetValue(3, _CP_VAL_NATIVE___state == -1, _CP_QUAL___state, "", _CP_QUAL___state);

	var mem_used = Math.round(_CP_VAL___memused / 1024);
	CPStdOut1.SetValue(4, false, 0, mem_used             , _CP_QUAL___memused);
	CPStdOut1.SetValue(5, false, 0, _CP_VAL___projectname, _CP_QUAL___projectname);

<%if (bCanExecute)
{%>
	if (_CP_VAL_NATIVE___state == 2 && _CP_QUAL___state == 0)	// running
	{
		Button0.enable(false);
		Button1.enable(true);
	}
	else if ((_CP_VAL_NATIVE___state == 1 || _CP_VAL_NATIVE___state == -1) && _CP_QUAL___state == 0) // stopped or in error state
	{
		Button0.enable(true);
		Button1.enable(false);
	}
	else										// status is something else
	{
		Button0.enable(false);
		Button1.enable(false);
	}
<%}%>
}

function GetParentUrl()
{
	return "/4C/CP_Loader.asp?AspFile=/4C/CP_ConfigurationList.asp&ConfFile=" + "<%=__FpParams.getConfFile()%>" + "&ObjName=Target List" + "&SessionId=" + "<%=__FpParams.getSessionId()%>" + "&StandAlone=" + "<%=__FpParams.isStandAlone()%>";
}

<%if (bCanExecute)
{%>
function StartResource()
{
	var path = _CP_PATH.substr(0, _CP_PATH.length-1);
	if (confirm("Do you really want to start the resource\n\n" + path + " ?"))
	{
		CpProxy.StartResource(path,"<%=CP_getSessionPwd(__FpParams.getSessionId())%>");
	}
}
function StopResource()
{
	var path = _CP_PATH.substr(0, _CP_PATH.length-1);
	if (confirm("Do you really want to stop the resource\n\n" + path + " ?"))
	{
		CpProxy.StopResource(path,"<%=CP_getSessionPwd(__FpParams.getSessionId())%>");
	}
}
<%}%>
</script>
<%GenerateStdOutEventHandler("CPStdOut")%>
</head>

<body <%=GenerateFaceplateBodyTag()%>>
<% 	GenerateProxyObj(); %>
<% 	GenerateStdOutControl("CPStdOut1", 6); %>

<%if (bCanExecute)
{%>
<table border="0">
<td>
	<% GenerateButton("", "images/CP_Start.gif", "images/CP_StartDisable.gif", "Start Resource", "normal", false, false, new Function('StartResource()'), 40);%>
</td>
<td>
	<% GenerateButton("", "images/CP_Stop.gif", "images/CP_StopDisable.gif", "Stop Resource", "normal", false, false, new Function('StopResource()'), 40);%>
</td>
</table>
<%}%>
<hr>
<a id="IdListLink" href="" onClick="LoadNewFaceplate()">Target List</a>

<% GenerateFunctionInitAllButtons() %>
</body>
</html>
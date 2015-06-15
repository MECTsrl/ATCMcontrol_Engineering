<%@ LANGUAGE = JScript %>
<% //ATCM. All rights reserved %>
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibFaceplate.asp" -->
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibButton.asp" -->
<% CheckFaceplateSecurity(); %>
<html>
<head>
<%GenerateStdHeader("Task")%>
<%GenerateStdIncludes()%>
<%GenerateStdCommunication();%>

<%
// status variable of parent resource
var ResourceStateVar = __InstanceData.getValuesStringByKey("PATH") + ".__state";
%>

<%
var bCanExecute = CP_canSessionExecute(__FpParams.getSessionId());
if (bCanExecute)
	GenerateButtonFunctions();
%>  
<script language="JavaScript">
function InitFaceplate()
{
	InitAllButtons();

    CPStdOut1.AddLine("Running", "DINT", "r", 2, 1);
    CPStdOut1.AddLine("Stopped", "DINT", "r", 1, 1);
    CPStdOut1.AddLine("Break"  , "DINT", "r", 1, 1);
    CPStdOut1.AddLine("Loading", "DINT", "r", 1, 1);
    CPStdOut1.AddLine("Error"  , "DINT", "r", 0, 1);

    CPStdOut1.AddLine("Cycle Time"  , _CP_TYPE___cycle, "r", 0, 4);
    CPStdOut1.AddLine("Elapsed Time", _CP_TYPE___etmax, "r", 0, 4);
    CPStdOut1.AddLine("Priority"    , _CP_TYPE___prio , "r", 0, 4);

	<%GenerateStdOutInitialization("CPStdOut1");%>

	IdListLink.href = GetParentUrl();

<% if (ResourceStateVar != null){%>
	SubscribeOPCList("<%=ResourceStateVar%>", ""); 
<% }%>
}

function UpdateFaceplate()
{
	var bResourceIsRunning = false;
	var ResValues = ReadOPCList("<%=ResourceStateVar%>", "").split("<%=getVarlistSeparator()%>");
	if (ResValues.length == 3)
	{
		var qRes = TranslateOPCQuality(ResValues[0]);
		var vRes = StringToNative(ResValues[1], "DINT");
		bResourceIsRunning = (qRes == 0) && (vRes == 2);
	}

	if (_CP_QUAL___state != 0)
		_CP_VAL___state = -2;  // all LEDs off

	CPStdOut1.SetValue(0, _CP_VAL_NATIVE___state == 2 , _CP_QUAL___state, "", _CP_QUAL___state);
	CPStdOut1.SetValue(1, _CP_VAL_NATIVE___state == 1 , _CP_QUAL___state, "", _CP_QUAL___state);
	CPStdOut1.SetValue(2, _CP_VAL_NATIVE___state == 3 , _CP_QUAL___state, "", _CP_QUAL___state);
	CPStdOut1.SetValue(3, _CP_VAL_NATIVE___state == 0 , _CP_QUAL___state, "", _CP_QUAL___state);
	CPStdOut1.SetValue(4, _CP_VAL_NATIVE___state == -1, _CP_QUAL___state, "", _CP_QUAL___state);

	CPStdOut1.SetValue(5, false, 0, _CP_VAL___cycle, _CP_QUAL___cycle);
	CPStdOut1.SetValue(6, false, 0, _CP_VAL___etmax, _CP_QUAL___etmax);
	CPStdOut1.SetValue(7, false, 0, _CP_VAL___prio , _CP_QUAL___prio);

<%if (bCanExecute)
{%>
	if (!bResourceIsRunning)              // to do anything with the task
	{                                     // the resource must be running 
		Button0.enable(false);
		Button1.enable(false);
	}
	else if (_CP_VAL_NATIVE___state == 2) // task is running
	{
		Button0.enable(false);
		Button1.enable(true);
	}
	else if (_CP_VAL_NATIVE___state == 1 || _CP_VAL_NATIVE___state == -1) // task is stopped or in error state
	{
		Button0.enable(true);
		Button1.enable(false);
	}
	else										// task status is something else
	{
		Button0.enable(false);
		Button1.enable(false);
	}
<%}%>

}

function GetParentUrl()
{
	return "/4C/CP_Loader.asp?AspFile=/4C/CP_TaskList.asp&ConfFile=" + "<%=__FpParams.getConfFile()%>" + "&ObjName=Task List" + "&SessionId=" + "<%=__FpParams.getSessionId()%>" + "&StandAlone=" + "<%=__FpParams.isStandAlone()%>";
}

<%if (bCanExecute)
{%>
function StartTask()
{
	var path = _CP_PATH.substr(0, _CP_PATH.length-1);
	if (confirm("Do you really want to start the task\n\n" + path + " ?"))
	{
		CpProxy.StartTask(path,"<%=CP_getSessionPwd(__FpParams.getSessionId())%>");
	}
}
function StopTask()
{
	var path = _CP_PATH.substr(0, _CP_PATH.length-1);
	if (confirm("Do you really want to stop the task\n\n" + path + " ?"))
	{
		CpProxy.StopTask(path,"<%=CP_getSessionPwd(__FpParams.getSessionId())%>");
	}
}
<%}%>

</script>
<%GenerateStdOutEventHandler("CPStdOut")%>
</head>

<body <%=GenerateFaceplateBodyTag()%>>
<% 	GenerateProxyObj(); %>
<% 	GenerateStdOutControl("CPStdOut1", 8); %>

<%if (bCanExecute)
{%>
<table border="0">
<td>
	<% GenerateButton("", "images/CP_Start.gif", "images/CP_StartDisable.gif", "Start Task", "normal", false, false, new Function('StartTask()'), 40);%>
</td>
<td>
	<% GenerateButton("", "images/CP_Stop.gif", "images/CP_StopDisable.gif", "Stop Task", "normal", false, false, new Function('StopTask()'), 40);%>
</td>
</table>
<%}%>
<hr>
<a id="IdListLink" href="" onClick="LoadNewFaceplate()">Task List</a>

<% GenerateFunctionInitAllButtons() %>
</body>
</html>
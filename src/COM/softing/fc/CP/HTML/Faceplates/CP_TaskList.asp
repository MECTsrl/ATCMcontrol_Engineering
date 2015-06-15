<%@ LANGUAGE = JScript %>
<% //ATCM. All rights reserved %>
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibFaceplate.asp" -->
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibButton.asp" -->
<% CheckFaceplateSecurity(); %>

<%
var bCanExecute = CP_canSessionExecute(__FpParams.getSessionId());
var ClassName = __FpParams.getClassName();
var ObjName = __FpParams.getObjName();
var ConfFile = __FpParams.getConfFile();

if (!openConfFile(ConfFile))
	writeError(ClassName, ObjName, "Could not load configuration data from file " + ConfFile);
ConfValues = Server.CreateObject("Scripting.Dictionary");

var TaskVarList;
var TaskNameList = new Array();
var count = 0;
var TaskName;
var Path = null;
var Class;
var NoTasks = 0;
do 
{
	TaskName = readConfObjectName();
	if (TaskName.length > 0)
	{
		readConfValues(ConfValues);
		Class = ConfValues.item("CLASS");			
		if (Class.length > 0 && (Class == "TASK") )
		{
			if (count == 0)
			{
				Path = ConfValues.item("PATH");
				TaskVarList =  TaskName + ".__state";
			}
			else
			{
				TaskVarList +=  getVarlistSeparator() + TaskName + ".__state";
			}
			TaskVarList +=  getVarlistSeparator() + TaskName + ".__prio";
			TaskVarList +=  getVarlistSeparator() + TaskName + ".__cycle";
			TaskVarList +=  getVarlistSeparator() + TaskName + ".__etmax";

			TaskNameList[count] = TaskName;
			count++;
		}
	}
} while (TaskName.length > 0) 
closeConfFile();
if  (count == 0)
    NoTasks = 1;

var nVarsPerTask = 4;

// status variable of parent resource
var MatchList = null;
var ResourceStateVar = null;
if (Path != null)
    MatchList = Path.match(/\[[^\[]+\](.)[^.]+/);
if (MatchList != null)
	ResourceStateVar = MatchList[0] + ".__state";
%>

<%
function GetTaskUrl(TaskIndex)
{
	return  "/4C/CP_Loader.asp?AspFile=/4C/CP_Task.asp&ConfFile=" + ConfFile + "&ObjName=" + TaskNameList[TaskIndex] + "&SessionId=" + __FpParams.getSessionId() + "&StandAlone=" + __FpParams.isStandAlone();
}
%>

<html>
<head>
<%GenerateStdHeader("Task List")%>
<%GenerateStdIncludes()%>

<%
if (bCanExecute)
{
	GenerateButtonFunctions();
}
%>

<% 
	GenerateLoginFunction(1);
%>

<script language="JavaScript">
var TaskVarList = "<%=TaskVarList%>";

function InitFaceplate()
{
	SubscribeOPCList(TaskVarList, "<%=Path%>."); 

<%if (NoTasks != 1)
{%>
	InitAllButtons();

	var ii;
	for (ii = 0; ii < <%=TaskNameList.length%>; ii++)
	{
		CPStdOutStateLed.AddLine("", "DINT"   , "r", 2, 0x03);
		CPStdOutState.AddLine   ("", "WString", "r", 0, 0x16);
		CPStdOutCycle.AddLine   ("", "DINT"   , "r", 0, 0x16);
		CPStdOutPrio.AddLine   ("", "DINT"   , "r", 0, 0x16);
	}

	<%GenerateStdOutInitialization("CPStdOutState");%>
	<%GenerateStdOutInitialization("CPStdOutStateLed");%>
	<%GenerateStdOutInitialization("CPStdOutCycle");%>
	<%GenerateStdOutInitialization("CPStdOutPrio");%>

<% if (ResourceStateVar != null){%>
	SubscribeOPCList("<%=ResourceStateVar%>", ""); 
<% }%>

<%
if ((Path != null) && (Path.length > 0)) 
{%>	
	setFaceplateToolTip("<%=Path%>.Task List");
<%}%>	

<%}%>
}

function ExitFaceplate()
{
	UnsubscribeOPCList(TaskVarList, ""); 
}

function UpdateFaceplate()
{
	<% GenerateLoginCall(Path, 0);%>

<%if (NoTasks != 1)
{%>
	var bResourceIsRunning = false;
	var ResValues = ReadOPCList("<%=ResourceStateVar%>", "").split("<%=getVarlistSeparator()%>");
	if (ResValues.length == 3)
	{
		var qRes = TranslateOPCQuality(ResValues[0]);
		var vRes = StringToNative(ResValues[1], "DINT");
		bResourceIsRunning = (qRes == 0) && (vRes == 2);
	}

	var values = ReadOPCList(TaskVarList, "<%=Path%>.").split("<%=getVarlistSeparator()%>");	
	if (values.length != <%=3 * nVarsPerTask * TaskNameList.length%>)
	{
		writeErrorMessage("proxy returned invalid number of variables");
	}
	else
	{	
		var State, Cycle, Prio, EtMax;  
		var qState, qCycle, qPrio, qEtMax;

		var bOneTaskRunning = false;
		var bOneTaskStopped = false;

		var ii;
		for (ii = 0; ii < <%=TaskNameList.length%>; ii++)
		{
			State	= values[<%=3 * nVarsPerTask%> * ii     + 1];
			Prio	= values[<%=3 * nVarsPerTask%> * ii + 3 + 1];
			Cycle	= values[<%=3 * nVarsPerTask%> * ii + 6 + 1];
			EtMax	= values[<%=3 * nVarsPerTask%> * ii + 9 + 1];

			qState  = TranslateOPCQuality(parseInt(values[<%=3 * nVarsPerTask%> * ii    ]));
			qPrio   = TranslateOPCQuality(parseInt(values[<%=3 * nVarsPerTask%> * ii + 3]));
			qCycle  = TranslateOPCQuality(parseInt(values[<%=3 * nVarsPerTask%> * ii + 6]));
			qEtMax  = TranslateOPCQuality(parseInt(values[<%=3 * nVarsPerTask%> * ii + 9]));

			if (State == 2)
			{
				CPStdOutStateLed.SetValue(ii, true, qState, "", qState, 2);
				CPStdOutState.SetValue(ii, false, qState, "Running", qState);
				bOneTaskRunning = true;
			}
			else if (State == 1)
			{
				CPStdOutStateLed.SetValue(ii, true, qState, "", qState, 1);
				CPStdOutState.SetValue(ii, false, qState, "Stopped", qState);
				bOneTaskStopped = true;
			}
			else if (State == 3)
			{
				CPStdOutStateLed.SetValue(ii, true, qState, "", qState, 1);
				CPStdOutState.SetValue(ii, false, qState, "Break", qState);
			}
			else if (State == 0)
			{
				CPStdOutStateLed.SetValue(ii, true, qState, "", qState, 1);
				CPStdOutState.SetValue(ii, false, qState, "Loading", qState);
			}
			else
			{
				CPStdOutStateLed.SetValue(ii, true, qState, "", qState, 0);
				CPStdOutState.SetValue(ii, false, qState, "Error", qState);
				bOneTaskStopped = true;
			}

			CPStdOutCycle.SetValue(ii, false, qCycle, Cycle, qCycle);
			CPStdOutPrio.SetValue(ii, false, qPrio, Prio, qPrio);
		}
<%if (bCanExecute)
{%>
		if (!bResourceIsRunning)              // to do anything with the task
		{                                     // the resource must be running 
			Button0.enable(false);
			Button1.enable(false);
		}
		else
		{
			Button0.enable(bOneTaskStopped);
			Button1.enable(bOneTaskRunning);
		}
<%}%>
	}
<%}%>
}
</script>

<%if (bCanExecute)
{%>
<script language="JavaScript">
function StartAllTasks()
{
	if (confirm("Do you really want to start all tasks of Resource\n\n" + "<%=Path%>" + " ?"))
	{
		CpProxy.StartAllTasks("<%=Path%>","<%=CP_getSessionPwd(__FpParams.getSessionId())%>");
	}
}
function StopAllTasks()
{
	if (confirm("Do you really want to stop all tasks of Resource\n\n" + "<%=Path%>" + " ?"))
	{
		CpProxy.StopAllTasks("<%=Path%>","<%=CP_getSessionPwd(__FpParams.getSessionId())%>");
	}
}
</script>
<%}%>

</head>

<body <%=GenerateFaceplateBodyTag()%> >
<% 	GenerateProxyObj(); %>

<table   class=Faceplate border="1" cellpadding="2" cellspacing="0" width="380">
<tr>
<td colspan=2 >  Status </td>
<td>  Name </td>
<td>  Cycle Time (ms) </td>
<td>  Priority  </td>
</tr>

<%if (NoTasks != 1)
{%>
<td width=20>
<%GenerateStdOutControl2("CPStdOutStateLed", TaskNameList.length, "100%")%>
</td>

<td width=60>
<%GenerateStdOutControl2("CPStdOutState", TaskNameList.length, "100%")%>
</td>

<td>
<table  height="<%=TaskNameList.length * getSingTextOutHeight()%>" class=Faceplate border="0" cellpadding="0" cellspacing="0" width="100%">
<%
for (ii = 0; ii < TaskNameList.length; ii++)
{
%>
<tr><td valign="top" nowrap> <a href="<%=GetTaskUrl(ii)%>" onClick="LoadNewFaceplate()"><%=TaskNameList[ii]%></a> </td></tr>
<% 
} %>
</table>
</td>

<td>
<%GenerateStdOutControl2("CPStdOutCycle", TaskNameList.length, "100%")%>
</td>
<td>
<%GenerateStdOutControl2("CPStdOutPrio", TaskNameList.length, "100%")%>
</td>
</table>

<%if (bCanExecute)
{%>
<table border="0">
<td>
	<% GenerateButton("", "images/CP_Start.gif", "images/CP_StartDisable.gif", "Start all Tasks", "normal", false, false, new Function('StartAllTasks()'), 40);%>
</td>
<td>
	<% GenerateButton("", "images/CP_Stop.gif", "images/CP_StopDisable.gif", "Stop all Tasks", "normal", false, false, new Function('StopAllTasks()'), 40);%>
</td>
</table>
<%}%>


<%}%>

<% GenerateFunctionInitAllButtons() %>

</body>
</html>


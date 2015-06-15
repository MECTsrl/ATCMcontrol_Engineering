<%@ LANGUAGE = JScript %>
<% //ATCM. All rights reserved %>
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibFaceplate.asp" -->
<% CheckFaceplateSecurity(); %>

<%
	var ClassName = __FpParams.getClassName();
	var ObjName = __FpParams.getObjName();
	var ConfFile = __FpParams.getConfFile();

	if (!openConfFile(ConfFile))
		writeError(ClassName, ObjName, "Could not load configuration data from file " + ConfFile);
	var ConfValues = Server.CreateObject("Scripting.Dictionary");

	var ResourceVarList;
	var ResourceNameList = new Array();
	var ResourcePathList = new Array();
	var ResourceName;
	var ResourcePath;
	var Class;
	var count = 0;

	do
	{
		ResourceName = readConfObjectName();
		if (ResourceName.length > 0)
		{
			readConfValues(ConfValues);
			Class = ConfValues.item("CLASS");
			if (Class.length > 0 && Class == "RESOURCE")
			{
				ResourceNameList[count] = ResourceName;
				ResourcePathList[count] = ConfValues.item("PATH");

				if (count == 0)
					ResourceVarList =  ResourcePathList[count] + "." + ResourceName + ".__state";
				else
					ResourceVarList +=  getVarlistSeparator() + ResourcePathList[count] + "." + ResourceName + ".__state";

				ResourceVarList +=  getVarlistSeparator() + ResourcePathList[count] + "." + ResourceName + ".__memused";
				ResourceVarList +=  getVarlistSeparator() + ResourcePathList[count] + "." + ResourceName + ".__projectname";

				count++;
			}
		}
	} while (ResourceName.length > 0);
	closeConfFile();

	var nVarsPerResource = 3;
%>

<%
function GetResourceUrl(Index)
{
	return  "/4C/CP_Loader.asp?AspFile=/4C/CP_Resource.asp&ConfFile=" + ConfFile + "&ObjName=" + ResourceNameList[Index] + "&SessionId=" + __FpParams.getSessionId() + "&StandAlone=" + __FpParams.isStandAlone();
}
%>

<html>
<head>
<%
GenerateStdHeader("Configuration List");
GenerateStdIncludes(); 
%>

<% 
	GenerateLoginFunction(ResourcePathList.length);
%>

<script language="JavaScript">

var ResourceVarList = "<%=ResourceVarList%>";

function InitFaceplate()
{
	SubscribeOPCList(ResourceVarList, ""); 

	var ii;
	for (ii = 0; ii < <%=ResourceNameList.length%>; ii++)
	{
		CPStdOutStateLed.AddLine("", "DINT"   , "r", 2, 0x03);
		CPStdOutState.AddLine   ("", "WString", "r", 0, 0x16);
		CPStdOutMem.AddLine     ("", "DINT"   , "r", 0, 0x16);
		CPStdOutProjName.AddLine("", "WString", "r", 0, 0x16);
	}

	<%GenerateStdOutInitialization("CPStdOutState");%>
	<%GenerateStdOutInitialization("CPStdOutStateLed");%>
	<%GenerateStdOutInitialization("CPStdOutMem");%>
	<%GenerateStdOutInitialization("CPStdOutProjName");%>
}

function ExitFaceplate()
{
	UnsubscribeOPCList(ResourceVarList, ""); 
}

function UpdateFaceplate()
{
<% // asychronous login call for all resources
for (var ii = 0; ii < ResourcePathList.length; ii++)
{
	GenerateLoginCall(ResourcePathList[ii], ii);
}%>
	var values = ReadOPCList(ResourceVarList, "").split("<%=getVarlistSeparator()%>");	
	if (values.length != <%=3 * nVarsPerResource * ResourceNameList.length%>)
	{
		writeErrorMessage("proxy returned invalid number of variables");
	}
	else
	{	
		var State, MemUsed, ProjectName;  
		var qState, qMemUsed, qProjectName;
		
		var ii;
		for (ii = 0; ii < <%=ResourceNameList.length%>; ii++)
		{
			State        = values[<%=3 * nVarsPerResource%> * ii     + 1];
			MemUsed      = values[<%=3 * nVarsPerResource%> * ii + 3 + 1];
			ProjectName  = values[<%=3 * nVarsPerResource%> * ii + 6 + 1];

			qState       = TranslateOPCQuality(parseInt(values[<%=3 * nVarsPerResource%> * ii    ]));
			qMemUsed     = TranslateOPCQuality(parseInt(values[<%=3 * nVarsPerResource%> * ii + 3]));
			qProjectName = TranslateOPCQuality(parseInt(values[<%=3 * nVarsPerResource%> * ii + 6]));

			if (State == 2)
			{
				CPStdOutStateLed.SetValue(ii, true, qState, "", qState, 2);
				CPStdOutState.SetValue(ii, false, qState, "Running", qState);
			}
			else if (State == 1)
			{
				CPStdOutStateLed.SetValue(ii, true, qState, "", qState, 1);
				CPStdOutState.SetValue(ii, false, qState, "Stopped", qState);
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
			}

			//== convert memused in bytes to kbytes 
			var iMemUsedRound = 0;
			if (qMemUsed == 0)
				iMemUsedRound = Math.round(MemUsed / 1024);

			CPStdOutMem.SetValue(ii, false, qMemUsed, iMemUsedRound, qMemUsed);
			CPStdOutProjName.SetValue(ii, false, qProjectName, ProjectName, qProjectName);
		}
	}
}
</script>
</head>

<body <%=GenerateFaceplateBodyTag()%>>
<% 	GenerateProxyObj(); %>

<table class=Faceplate border="1" cellpadding="2" cellspacing="0" width="380">
<tr>
<td colspan=2> Status </td>
<td> Maschine </td>
<td> Resource </td>
<td> Memory (kB) </td>
<td> Project</td>
</tr>

<td width=20>
<%GenerateStdOutControl2("CPStdOutStateLed", ResourceNameList.length, "100%")%>
</td>

<td width=60>
<%GenerateStdOutControl2("CPStdOutState", ResourceNameList.length, "100%")%>
</td>

<td>
<table  height="<%=ResourceNameList.length * getSingTextOutHeight()%>" class=Faceplate border="0" cellpadding="0" cellspacing="0" width="100%">
<%
for (ii = 0; ii < ResourceNameList.length; ii++)
{%>
<tr><td valign="top" nowrap><%=ResourcePathList[ii]%></td></tr>
<%}%>
</table>
</td>

<td>
<table  height="<%=ResourceNameList.length * getSingTextOutHeight()%>" class=Faceplate border="0" cellpadding="0" cellspacing="0" width="100%">
<%
for (ii = 0; ii < ResourceNameList.length; ii++)
{%>
<tr><td valign="top" nowrap> <a href="<%=GetResourceUrl(ii)%>" onClick="LoadNewFaceplate()"><%=ResourceNameList[ii]%></a> </td></tr>
<%}%>
</table>
</td>

<td>
<%GenerateStdOutControl2("CPStdOutMem", ResourceNameList.length, "100%")%>
</td>
<td width="100">
<%GenerateStdOutControl2("CPStdOutProjName", ResourceNameList.length, "100%")%>
</td>
</table>


</table>
</body>
</html>

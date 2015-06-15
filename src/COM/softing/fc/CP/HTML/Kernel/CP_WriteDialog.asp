<%@ LANGUAGE = JScript %>
<% //ATCM. All rights reserved %>

<!-- #INCLUDE VIRTUAL = "/4C/CP_LibFaceplate.asp" -->
<!-- #INCLUDE VIRTUAL = "/4C/CP_WriteDialogTitle.asp" -->

<HTML>
<HEAD>
<META NAME="GENERATOR" Content="Microsoft Developer Studio">
<META HTTP-EQUIV="Content-Type" content="text/html; charset=iso-8859-1">
<meta HTTP-EQUIV="Pragma" content="no-cache">
<link rel=stylesheet href="CP_Style.css" type="text/css">
<TITLE><%=getTitle()%></TITLE>

<script language="JavaScript" src="CP_Panel.js"></script>
<script language="VBSCRIPT" src="CP_FPLib.inc"></script>
<script language="JavaScript" src="CP_FPLib.js"></script>


<% 
	GenerateLoginFunction(1);
%>

<script language="JavaScript">
m_Path = null;
m_Type = null;

function initDialog()
{
	if (getDialogArgs() < 0)
		closeDialog();

	SubscribeOPCList(m_Path, "");
	
	IdWrite.disabled = true;	
	
	IdOutput.AddLine(m_Path, m_Type, "rw", 0, 0x2 | 0x4 | 0x10);
	<%GenerateStdOutInitialization("IdOutput");%>
	IdOutput.tabIndex = -1;

	IdInput.focus();

	checkInput();
	updateDialog();
}

function updateDialog()
{
	<%
	GenerateLoginCall(m_Path, 0);
	%>
	var values = ReadOPCListTyped(m_Type + "<%=getVarlistSeparator()%>" + m_Path, "").split("<%=getVarlistSeparator()%>");
	if (values.length != 3)
		writeErrorMessage("Proxy returned wrong number of values !");
	else
		IdOutput.SetValue(0, false, 0, values[1], TranslateOPCQuality (values[0]));
	
	setTimeout("updateDialog()", 1000);
}
	
function closeDialog()
{
	window.returnValue = -1; 
	window.close();
}


function getDialogArgs()
{
	m_Path = null;	
	m_Type = null;	
	if (window.dialogArguments == null)
	{
		alert("invalid dialog arguments");
		return -1;
	}

	if (window.dialogArguments != null)
	{
		m_Path = window.dialogArguments.Path;	
		m_Type = window.dialogArguments.Type;
		if (m_Path == null || m_Path == "")
		{
			alert("Undefined variable path")
			return -1;
		}

		if (m_Type == null || m_Type == "")
		{
			alert("Undefined data type")
			return -1;
		}
	}
	return 0;
}


function writeValue()
{
	CP_ErrorTextDialog.innerText = "";	
	if (CP_CheckIEC(m_Type, IdInput.value))
	{
		WriteOPCValue(m_Path, CP_IECToVariant(m_Type, IdInput.value));						
	}
}

function setErrorTextDialog(errorText)
{
	if (errorText != null && errorText.length > 0)
		CP_ErrorTextDialog.innerText = "Status: " + formatText(errorText, 80);
	else
		CP_ErrorTextDialog.innerText = "";
}

function formatText(errorText, maxLen)
{
	if(errorText.length > maxLen)
		return errorText.substr(0, maxLen) + "...";
	else
		return errorText
}


function checkInput()
{	
	IdWrite.disabled = !CP_CheckIEC(m_Type, IdInput.value);
}

function isReturnEvent()	{ return  (window.event.keyCode == 13); }
function isEscEvent()		{ return  (window.event.keyCode == 27); }

function onButtonKeyEvent()
{
	checkInput();
}

function onBodyKeyEvent()
{
	if (isEscEvent())
		closeDialog(); 
	else if (isReturnEvent() && !IdWrite.disabled)
		writeValue();
}

</script>

</HEAD>
<BODY class=DialogObject 
	onload="initDialog()" 
	onkeyup="onBodyKeyEvent()">
<% 	GenerateProxyObj(); %>
<% 	GenerateBaseLibObj(); %>

<table class=DialogObject border=0 width="100%">
	<tr>	
		<td width="10%">Current:</td>
        <td colspan=4 height=24>
			<%GenerateStdOutControl2("IdOutput", 1, "100%");%>
		</td>	
	</tr>
	<tr>
		<td width="10%">New:</td>
		<td colspan=4 height=24 >
			<input id=IdInput tabindex=1 type=text style="width:100%;height:100%;font-size:9pt"
			onkeyup="onButtonKeyEvent();">			
		</td>
	</tr>
	<tr>
		<td colspan=5 height=24>
			<font size="1"><div id="CP_ErrorTextDialog" style="color:#ff0000"></div></font> 
		</td>	
	</tr>
	<tr>
		<td></td>
		<td></td>
		<td></td>
		<td align="left" width="20%">
			<input type=button id="IdCancel" onclick="closeDialog()" value=Close style="width:100%;font-size:9pt" ></input>
		</td>
		<td align="right" width="20%">
			<input type=button id="IdWrite" onclick="writeValue()" value=Write style="width:100%;font-size:9pt"></input>
		</td>
	</tr>
</table>
</BODY>
</HTML>

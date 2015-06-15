<%@ LANGUAGE = JScript %>
<% //ATCM. All rights reserved %>

<!-- #INCLUDE VIRTUAL = "/4C/CP_LibFaceplate.asp" -->
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibSecurity.asp" -->
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

	RadioTrue.innerText = CP_VariantToIEC(m_Type, true);
	RadioFalse.innerText = CP_VariantToIEC(m_Type, false);				

	RadioTrueButton.focus();

	updateDialog();		
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

		switch(m_Type)
		{
			default :
				alert("Invalid data type");
				return -1;
				break;

			case "BOOL":
			case "QVT_BOOL":
				break;
		}
	}
	return 0;
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
	

function writeValue()
{
	CP_ErrorTextDialog.innerText = "";	

	if (RadioBool[0].checked)
	{
		WriteOPCValue(m_Path, true);
		RadioBool[0].checked = false;	
	}
	else if (RadioBool[1].checked)
	{
		WriteOPCValue(m_Path, false);	
		RadioBool[1].checked = false;
	}
	IdWrite.disabled = true;
}

function closeDialog()
{
	window.returnValue = -1; 
	window.close();
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

function isReturnEvent()	{ return  (window.event.keyCode == 13); }
function isEscEvent()		{ return  (window.event.keyCode == 27); }

function onButtonKeyEvent()
{
	if (isReturnEvent() && !IdWrite.disabled)
		writeValue();
}
function onBodyKeyEvent()
{
	if (isEscEvent())
		closeDialog(); 
}

function onRadioClick()
{
	IdWrite.disabled = !(RadioBool[0].checked | RadioBool[1].checked);
}




</script>

</HEAD>
<BODY class=DialogObject 
	onload="initDialog()" 
	onkeydown="onBodyKeyEvent()">
<% 	GenerateProxyObj(); %>
<% 	GenerateBaseLibObj(); %>
<table class=DialogObject border=0 width="100%">
	<tr>
		<td width="12%">Current:</td>
        <td width="68%"colspan=2 height=22>
			<%GenerateStdOutControl2("IdOutput", 1, "90%");%>
		</td>
		<td width=20% align="right">
			<input type=button id="IdWrite" onclick="writeValue()" value=Write style="width:100;font-size:9pt">
		</td>			
	</tr>
	<tr>
		<td width="12%">New:</td>		
		<td width="10%"><input type="radio" id="RadioTrueButton" onclick="onRadioClick()" onkeydown="onButtonKeyEvent()" name="RadioBool"></td>
		<td width="58%"><DIV ID=RadioTrue></DIV></td>	
		<td width=20% align="right">
			<input type=button id="IdCancel" onclick="closeDialog()" value=Close style="width:100;font-size:9pt">
		</td>					
	</tr>
	<tr>
		<td width="12%"></td>		
		<td width="10%"><input type="radio" id="RadioFalseButton" onclick="onRadioClick()" onkeydown="onButtonKeyEvent()" name="RadioBool"></td>
		<td width="58%"><DIV ID=RadioFalse></DIV></td>							
		<td width="20%"></td>		
	</tr>
	<tr>
		<td colspan=4 height=24>
			<font size="1"><div id="CP_ErrorTextDialog" style="color:#ff0000"></div></font> 
		</td>		
	</tr>
</table>
</BODY>
</HTML>

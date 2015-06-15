<%@ LANGUAGE = JScript %>
<% //ATCM. All rights reserved %>
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibAspParam.asp" -->
<HTML>
<HEAD>
<META NAME="GENERATOR" Content="Microsoft Developer Studio">
<META HTTP-EQUIV="Content-Type" content="text/html; charset=iso-8859-1">
<TITLE></TITLE>
<script language="JavaScript">

var m_ToolTip = "";
var m_Text = "";

function isHeaderScripletLoaded()
{
	return (document.CpHeader != null && document.CpHeader.setToolTip != null);
}

function setToolTip(tip)
{
	m_ToolTip = tip;
	if (isHeaderScripletLoaded())
		document.CpHeader.setToolTip(m_ToolTip);
}

function setText(text)
{
	m_Text = text;
	if (isHeaderScripletLoaded())
		document.CpHeader.setText(m_Text);
}

function InitCpHeader()
{
	if (!isHeaderScripletLoaded())
	{
		setTimeout("InitCpHeader()", 1000);
		return;
	}
<%
var ObjName = __FpParams.getObjName();
var ClassName = __FpParams.getClassName();
var Url = readParam("MainUrl");
%>
	document.CpHeader.setName("", "<%=ObjName%>");
	if (m_Text.length > 0)
		document.CpHeader.setText(m_Text);
	if (m_ToolTip.length > 0)
		document.CpHeader.setToolTip(m_ToolTip);
<%
if (Url != null && __FpParams.getConfFile() != null && __FpParams.getObjName() != null)
{
	var bNoSingleObject = 
		(Url.indexOf("CP_MsgHistory.asp") >= 0)          ||
		(Url.indexOf("CP_TaskList.asp") >= 0)            ||
		(Url.indexOf("CP_ConfigurationList.asp") >= 0)   ||
		(Url.indexOf("4c_profibus_io.4cl") >= 0);

	if (!bNoSingleObject)
	{
		if (Url.indexOf("CP_TuningDisplay.asp") >= 0)
		{
			var FacePlate = readParam("FacePlate");
			if (FacePlate != null)
			{%>
	var href = "/4C/CP_Loader.asp?AspFile=<%=FacePlate%>&ConfFile=<%=__FpParams.getConfFile()%>&Class=<%=__FpParams.getClassName()%>&ObjName=<%=__FpParams.getObjName()%>&SessionId=<%=__FpParams.getSessionId()%>&StandAlone=<%=__FpParams.isStandAlone()%>";
	document.CpHeader.setLink(href, "FP", "Goto Faceplate");
<%			}
		}
		else
		{%>
	var href = "/4C/CP_Loader.asp?AspFile=/4C/CP_TuningDisplay.asp&FacePlate=<%=Url%>&ConfFile=<%=__FpParams.getConfFile()%>&Class=<%=__FpParams.getClassName()%>&ObjName=<%=__FpParams.getObjName()%>&SessionId=<%=__FpParams.getSessionId()%>&StandAlone=<%=__FpParams.isStandAlone()%>";
	document.CpHeader.setLink(href, "TD", "Goto Tuning Display");
<%		}
	}
}
%>
}

function CP_WriteValue(ValuePath, ValueType)
{
	 setTimeout("CP_WriteValue2('" + ValuePath + "','" + ValueType + "')", 1);
}

function CP_WriteValue2(path, type)
{
	if (path != null && path.length > 0)
	{
		CP_WriteValueArgs.Path = path;
		CP_WriteValueArgs.Type = type;

		switch (type)
		{
			default:
				showModalDialog('/4C/CP_WriteDialog.asp?path=' + path + '&type=' + type + '&SessionId=<%=__FpParams.getSessionId()%>', CP_WriteValueArgs, "dialogWidth:450px;dialogHeight:160px;help:no;center:yes");
			break;

			case "BOOL":
			case "QVT_BOOL":
				showModalDialog('/4C/CP_WriteBoolDialog.asp?path=' + path + '&type=' + type + '&SessionId=<%=__FpParams.getSessionId()%>', CP_WriteValueArgs, "dialogWidth:450px;dialogHeight:150px;help:no;center:yes");
			break;
		}
	}
}

function CP_WriteValueArgs()
{
	var Path = "";
	var TypeId = "";
}

function onHeaderKeyDown()
{
	if (isHeaderScripletLoaded())
	{
		window.event.cancelBubble = true;
		if (32 == window.event.keyCode)
			document.CpHeader.setFocusFrame();
	}
}

function onHeaderFocus()
{
	if (isHeaderScripletLoaded())
		document.CpHeader.boldText(true);
}

function onHeaderBlur()
{
	if (isHeaderScripletLoaded())
		document.CpHeader.boldText(false);
}

function cancelDefaultAction()
{
	window.event.cancelBubble = true;
	window.event.returnValue = false;
}
</SCRIPT>
</HEAD>

<BODY onLoad="InitCpHeader()">
<a class="HeaderObject" href="" onclick="cancelDefaultAction()" onfocus="onHeaderFocus()" onblur="onHeaderBlur()" onkeydown="onHeaderKeyDown()">
	<object id="CpHeader" width="100%" height="100%" type="text/x-scriptlet" data="/4C/CP_FaceplateHeader.html"></object>
</a>
</BODY>
</HTML>

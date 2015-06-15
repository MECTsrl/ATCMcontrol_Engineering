<%@ LANGUAGE = JScript %>
<% //ATCM. All rights reserved %>
<OBJECT RUNAT=Server SCOPE=PAGE ID=IdMsg 
	CLASSID="Clsid:D8BA2C15-8B10-11d5-8798-00010209D2BC">
</OBJECT> 
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibFaceplate.asp" -->
<% CheckFaceplateSecurity(); %>

<% 
IdMsg.Connect(CP_getSessionPwd(__FpParams.getSessionId()));
var m_nLinesPerPage = 100;

var m_minID = IdMsg.MinHistoryID("");
var m_maxID = IdMsg.MaxHistoryID("");

var m_TotCount = (m_maxID - m_minID) + 1;
var	m_Count    = 0;

var m_Cmd = readParam("Cmd");

var m_StartID = parseInt(readParam("StartID"));
m_StartID = m_StartID < m_minID ? m_minID : m_StartID
m_StartID = m_StartID > m_maxID ? m_maxID : m_StartID


var m_EndID = parseInt(readParam("EndID"));
m_EndID = m_EndID > m_maxID ? m_maxID : m_EndID
m_EndID = m_EndID < m_StartID ? m_StartID : m_EndID


function getMessageType(text)
{
	if (text.indexOf(" : error ") >= 0)
		return 2;
	else if (text.indexOf(" : fatal error ") >= 0)
		return 2;
	else if (text.indexOf(" : warning ") >= 0)
		return 1;
	else
		return 0
}

function fill(ID)
{
	var id = ID;
	if (id < m_minID)
		id = m_minID;

	if (id > m_maxID)
		id = m_maxID;

	m_StartID = id;
	m_Count = 0;
	while (m_Count < m_nLinesPerPage && id <= m_maxID)
	{
		generateLine(id, IdMsg.MsgDate("", id), IdMsg.MsgText("", id), getMessageType(IdMsg.MsgText("", id)));
		m_Count++;
		id++;
	}
	m_EndID = m_StartID + m_Count - 1;
}

function generateLine(ID, date, text, type)
{%>
	<%if (type == 2){%><tr bgcolor="#ffc000"><%}
	else if (type == 1) {%><tr bgcolor="#ff8000"><%}
	else {%><tr><%}%>
<td Id="Id<%=ID%>" width= 50><%=ID%></td><td width=100><NOBR><%=date%></NOBR></td><td><%=text%></td></tr><%
}%>

<html>
<head>
<% 
GenerateStdHeader("Message History Body");
GenerateStdIncludes();
%>  
</head>
<body  class=Faceplate onload="init()" onunload="exit()">
<table class=Faceplate  border=0 width=700>
<%
if (m_Cmd == "Begin")
	fill(m_minID);
else if (m_Cmd == "End")
	fill(m_maxID - m_nLinesPerPage + 1);
else if (m_Cmd == "Forward")
	fill(m_EndID + 1);
else if (m_Cmd == "Back")
	fill(m_StartID - m_nLinesPerPage);
else if (m_Cmd == "Update")
	fill(m_StartID);
else
	fill(m_maxID - m_nLinesPerPage + 1);
%>
</table>
</body>

<script language="JavaScript">
function getStartID() { return <%=m_StartID%>; }
function getEndID() { return <%=m_EndID%>; }
function getMinID() { return <%=m_minID%>; }
function getMaxID() { return <%=m_maxID%>; }
function getCount() { return <%=m_Count%>; }

function init() 
{ 
<%
if (m_EndID == m_maxID) 
{%>
	Id<%=m_EndID%>.scrollIntoView(false);<% 
}
if (m_minID >= 0 && m_TotCount > 0)
{%>
	parent.setFaceplateText("(<%=m_StartID%> - <%=m_EndID%>)");<%
}%>
	parent.setFaceplateToolTip("[<%=Request.ServerVariables('SERVER_NAME')%>].Message Historie");

	parent.m_BodyLoaded = true;
}

function exit()
{
	parent.m_BodyLoaded = false;
}
</script>
</html>
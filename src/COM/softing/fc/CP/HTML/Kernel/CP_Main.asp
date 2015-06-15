<%@ LANGUAGE = JScript %>
<% //ATCM. All rights reserved %>
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibFaceplate.asp" -->
<!-- #INCLUDE VIRTUAL = "/4C/CP_View.asp" -->

<%CheckFaceplateSecurity()%>

<OBJECT RUNAT=Server SCOPE=PAGE ID=IdLayoutIo
CLASSID="Clsid:766287F2-9000-11d5-A550-0008C779C600">
</OBJECT> 
<% 
var m_SessionId  = __FpParams.getSessionId();

var m_View = new CP_View(0, 0);
IdLayoutIo.Init(Server.MapPath("/4C"), CP_getSessionUser(m_SessionId));

// Check rights by writing and reading test string
var bRightsOK = false;
var t = "" + new Date().getTime();
IdLayoutIo.Save("_tmp281260", t);
if (IdLayoutIo.Read("_tmp281260") == t)
	bRightsOK = true;
IdLayoutIo.Delete("_tmp281260");

if (!bRightsOK)
{%>
	<html>
	<head>
	<%GenerateStdHeader("No Rights")%>
	<link rel=stylesheet href="/4C/CP_Style.css" type="text/css">
	</head>
	<body class=Faceplate>
	<p><font color='#FF0000' size='5'><strong><div align='center'>Access denied !</div></strong></font></p>
	<p><font color='#000000' size='2'><div align='center'>Check permissions of directory "<%=Server.MapPath("/4C")%>/user" !</div></font></p>
	</BODY> 
	</body>
	</html>
<% }
else
{
	var layout = IdLayoutIo.Read("start")
	if (m_View.fromCookieString(layout))
	{
		IdLayoutIo.SetDefaultName("start");
	}
	else
	{
		//layout = IdLayoutIo.Read(IdLayoutIo.GetDefaultName());
		//if (!m_View.fromCookieString(layout))
		//{
			m_View.init(new Array(0,1,2,3, 0,1,2,3, 0,1,2,3, 0,1,2,3));
			IdLayoutIo.SetDefaultName("[Unsaved Layout]");
		//}
	}
	layout = m_View.toCookieString();
	IdLayoutIo.Save("_tmp281260", layout);
	%>

	<html>
	<head>
	<%GenerateStdHeader("Console")%>

	<script language="JavaScript" src="CP_Panel.js"></script>
	<script language="JavaScript">

	// indicates, which pages are loaded (see CP_Panel.js for more information)
	var m_LoadStatus = 0;

	var m_KeepAliveRights = "?";

	<%
	if (CP_isSecurityEnabled()) {%>
	function Logout() 
	{
		navigate("/4c/CP_Logout.asp?SessionId=<%=m_SessionId%>&ShowLogin=1");
	}

	<%}%>

	function deleteCurrentSession() 
	{
		FC_KeepAlive.navigate("/4c/CP_Logout.asp?SessionId=<%=m_SessionId%>&ShowLogin=0");
	}

	function init() 
	{
		m_KeepAliveTime = 0;
		setMainLoaded();
		setViewLoaded();
		init2();
	}

	function init2() 
	{
		if (isHeaderLoaded() && isNavigatorLoaded() && isStatusLoaded())
		{
			if (!isFrameLoaded())
				FC_Main.navigate("CP_FpContainer.asp?SessionId=<%=m_SessionId%>");

			keepAlive();
		}	
		else
			setTimeout("init2()", 500);
	}

	function exit() { m_LoadStatus = 0; }

	function getWEBServer() { return "<%=Request.ServerVariables('SERVER_NAME')%>"; }

	function setNavigatorWidth(iWidth)
	{
		if (iWidth == 0)
		{
			FS2.cols = "*,100%";
		}
		else
		{
			var w2 = 100 - iWidth;
			FS2.cols = iWidth.toString() + "%," + w2.toString() + "%";
		}

	}

	function keepAlive()
	{

		FC_KeepAlive.navigate("<%='http://' + Request.ServerVariables('SERVER_NAME') + '/4C/CP_KeepAlive.asp?SessionId=' + m_SessionId%>");
		setTimeout("keepAlive()", <%=CP_getTimeOut()/2%>);

		<%
		// m_KeepAliveTime and m_KeepAliveRights are set by CP_KeepAlive.asp
		// if any problem occurs, the login screen will be displayed 
		if (CP_isSecurityEnabled()) {%>
		if (m_KeepAliveRights != "?")
		{
			if (m_KeepAliveRights.length == 0)
			{
				alert("The session on the server <%=Request.ServerVariables('SERVER_NAME')%> has been removed !\nPlease login again.");
				Logout();
			}
		}
		<%}%>
	}
	</script>
	</head>

	<frameset bgcolor='#E0E0E0' rows="48,*,20,0" onload="init()" onunload="exit()">
		<frame src="CP_Header.asp?SessionId=<%=m_SessionId%>" name="FC_Header" noresize scrolling="no" marginheight="0">
		<frameset id=FS2 cols="<%=m_View.getNavigatorWidth()%>%,<%=100 - m_View.getNavigatorWidth()%>%" >
			<frame src="CP_Navigator.asp" name="FC_Navigator" scrolling="no">
			<frame src="CP_EmptyContainer.asp?SessionId=<%=m_SessionId%>" name="FC_Main">
		</frameset>
		<frame src="CP_StatusLine.asp?SessionId=<%=m_SessionId%>" name="FC_StatusLine" noresize scrolling="no" marginheight="0">
		<frame src="CP_KeepAlive.asp?SessionId=<%=m_SessionId%>" name="FC_KeepAlive" noresize scrolling="no" marginheight="0">
	</frameset>

	</html>
<%}%>

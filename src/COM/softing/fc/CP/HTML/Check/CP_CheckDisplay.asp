<%@ Language=JScript %>
<% //ATCM. All rights reserved %>
<%
var strPassed = "Passed";
var strFailed = "Failed";

function getServer()
{
	return "" + Request.ServerVariables("SERVER_SOFTWARE");
}

function checkServer()
{
	var ServerSoftware = getServer();
	var MainVersionNumberWithPoint = new String(ServerSoftware.match(/\d*\./));
	var MainVersionNumber = MainVersionNumberWithPoint.replace(/\./, "");
	var MinMainVersionNumber = 3;
	if (((ServerSoftware.indexOf("Microsoft-PWS/") >= 0) || (ServerSoftware.indexOf("Microsoft-IIS/") >= 0)) &&
		(MainVersionNumber >= MinMainVersionNumber))
	{
		return strPassed;
	}
	return strFailed;
}
%>

<HTML> 
<HEAD> 
<TITLE></TITLE> 
<script language="JavaScript">
var DoneBrowser = false;
var DoneJava = false;
var DoneActiveX = false;

function checkBrowser()
{
	if (!DoneBrowser)
	{
		DoneBrowser = true;

		var rv = "<%=strFailed%>";
		IdBowserName.innerText = navigator.appName + " " + navigator.appVersion;;

		var Version = navigator.appVersion;
		var MainVersionNumberWithPoint = new String(Version.match(/^\d*\./));
		var MainVersionNumber = MainVersionNumberWithPoint.replace(/\./, "");

		if ((navigator.appName.indexOf("Microsoft Internet Explorer") >= 0) &&
			(MainVersionNumber >= 4))
		{
			rv = "<%=strPassed%>";
		}
		IdBowserCheck.innerText = rv;
	}
}

function checkJava()
{
	if (!DoneJava)
	{
		DoneJava = true;

		var rv = "<%=strFailed%>";

		var temp_errortrap = onerror;
		onerror = errortrap_JavaFailed;

		if (navigator.javaEnabled() && document.IdObjJava.checkAppletExecution() == "ok")
			rv = "<%=strPassed%>";	
		
		onerror = temp_errortrap;
		IdJavaCheck.innerText = rv;
	}
}

function checkActiveX()
{
	if (!DoneActiveX)
	{
		DoneActiveX = true;

		var rv = "<%=strPassed%>";

		var temp_errortrap = onerror;
		onerror = errortrap_ActiveXFailed;

		document.IdObjActiveX.ShowObjectList(0, 0);
		
		onerror = temp_errortrap;
		IdActiveXCheck.innerText = rv;
	}
}

function errortrap_JavaFailed(msg,url,line)
{
	IdJavaCheck.innerText = "<%=strFailed%>";
	return true;
}

function errortrap_ActiveXFailed(msg,url,line)
{
	IdActiveXCheck.innerText = "<%=strFailed%>";
	return true;
}

function check()
{
	checkBrowser();
	checkJava();
	checkActiveX();
}
</script>

</HEAD> 
<body bgcolor='#E0E0E0' onload="check()">
<p align="center">4C Console Installation Check</p>

<TABLE border=1> 
	<TR> 
		<TD>Server</TD> 
		<TD> <%=getServer()%></TD> 
		<TD> <%=checkServer()%></TD> 
	</TR> 
	<TR> 
		<TD>Browser</TD> 
		<TD><div id="IdBowserName"></div></TD> 
		<TD><div id="IdBowserCheck"></div></TD> 
	</TR> 
	<TR> 
		<TD>Java</TD> 
		<TD><div id="IdJava"></div></TD> 
		<TD><div id="IdJavaCheck"></div></TD> 
	</TR> 
	<TR> 
		<TD>ActiveX</TD> 
		<TD><div id="IdActiveX"></div></TD> 
		<TD><div id="IdActiveXCheck"></div></TD> 
	</TR> 
</TABLE>


<!-- check activex -->
<object id="IdObjActiveX" classid="clsid:2CF26AE0-EE63-11d1-A512-00A024363DFC"
	codebase="CpNavigator.cab#version=##VERSION##" width="0" height="0">
</object>

<!-- check applet -->
<applet id="IdObjJava" code="CP_ASPStarter.class" height="0" width="0" > </applet>
</BODY> 
</HTML> 
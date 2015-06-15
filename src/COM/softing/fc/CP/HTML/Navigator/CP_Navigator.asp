<%@ LANGUAGE = JScript %>
<% //ATCM. All rights reserved %>
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibHtml.asp" -->
<html>
<head>
<%GenerateStdHeader("Navigator")%>
<%GenerateStdIncludes()%>

<script language="JavaScript">
function init() 
{
	setTimeout("init2()", 100);
}

function init2() 
{
	setNavigatorLoaded();
	if (isAllLoaded())
		LoadProject(); 
	else
		setTimeout("init2()", 500);
}

function exit() 
{
	resetNavigatorLoaded();
}


function LoadProject() 
{
	var iEoPath = location.href.lastIndexOf("CP_Navigator");
	var Path = location.href.substr(0, iEoPath);
	var URL = Path + "config/Objects.txt";
	document.FC_OCX_Navigator.LoadObjectList(URL); 
}

function ShowProject() 
{
	document.FC_OCX_Navigator.ShowObjectList(0, 0); 
	document.FC_OCX_Navigator.ShowObjectList(1, 64); 
}


function LoadSelectedURL(sUrl, bNewServer, bNewWindow) 
{
	if (isAllLoaded())
	{
		if (bNewServer == 0)
		{
			CP_getMainWindow().loadPage(sUrl, bNewWindow);
		}
		else
		{
			if (bNewWindow == 0)
				parent.navigate(sUrl);		
			else
				window.open(sUrl, null, "height=400,width=800,resizable=yes,status=yes,toolbar=yes,menubar=yes,location=yes");
		}
	}
}

</script>

<script
language="JavaScript" for="FC_OCX_Navigator" event="NewUrl(url, bNewServer, bNewWindow)">
	LoadSelectedURL(url, bNewServer, bNewWindow); 
</script>

<script
language="JavaScript" for="FC_OCX_Navigator" event="LoadingReady()">
	ShowProject(); 
</script>

</head>

<body bgcolor="#FFFFFF" topmargin="0" leftmargin="0" onLoad="init()" onUnload="exit()">

<object id="FC_OCX_Navigator"
	classid="clsid:2CF26AE0-EE63-11d1-A512-00A024363DFC"
	codebase="CpNavigator.cab#version=<%=GetVersionString()%>" 
	align="left" border="0"
	width="100%" height="100%" standby="Loading Navigator">
</object>

</body>
</html>

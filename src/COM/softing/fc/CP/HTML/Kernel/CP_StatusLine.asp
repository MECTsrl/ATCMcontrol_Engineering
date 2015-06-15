<%@ LANGUAGE = JScript %>
<% //ATCM. All rights reserved %>
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibFaceplate.asp" -->
<html>
<head>
<%GenerateStdHeader("Status Line")%>
<%GenerateStdIncludes()%>

<script language="JavaScript">

var m_VarList = "__system.license.DoesExpire|__system.license.Hours|__console.__system.Dload.DLoadGUID|__srt0.__system.Dload.DLoadGUID|__system.security.isEnabled";
var m_Path = "";

var m_bBlinkLicense = 0;
var m_bBlinkDownload = 0;
var m_OldDlGuid = "";

var m_qDoesExpire = 2;
var m_DoesExpire = 0;
var m_qHours = 2;
var m_Hours = 0;
var m_qDownloadGuid = 2;
var m_DownloadGuid = 0;

var m_qIsSecurityEnabled = 2;
var m_IsSecurityEnabled = 0;

function StatusInit() 
{
	m_bBlinkLicense = 0;
	m_bBlinkDownload = 0;
	m_OldDlGuid = "";
	
	setStatusLoaded();
	Init();
}

function StatusExit() 
{
	Exit();
	resetStatusLoaded();
}

function InitCommunication() 
{
	m_Path = "[" + CP_getPanelWindow().getWEBServer() + "].";
	SubscribeOPCList(m_VarList, m_Path); 
}	

function ExitCommunication() 
{
	UnsubscribeOPCList(m_VarList, m_Path); 
}	

function UpdateCommunication() 
{
//	IdError.innerText = ReadOPCList(m_VarList, m_Path);
	var Values = ReadOPCList(m_VarList, m_Path).split("<%=getVarlistSeparator()%>");
	if (Values.length == 15)
	{
		m_qDoesExpire = TranslateOPCQuality(parseInt(Values[0]));
		if (m_qDoesExpire == 0)
			m_DoesExpire = parseInt(Values[1]);
		m_qHours = TranslateOPCQuality(parseInt(Values[3]));
		if (m_qHours == 0)
			m_Hours = parseInt(Values[4]);

		var q1 = TranslateOPCQuality(parseInt(Values[6]));
		var q2 = TranslateOPCQuality(parseInt(Values[9]));
		m_qDownloadGuid = Math.max(q1, q2);
		if (m_qDownloadGuid == 0)
			m_DownloadGuid = Values[7] + Values[10];

		m_qIsSecurityEnabled = TranslateOPCQuality(parseInt(Values[12]));
		if (m_qIsSecurityEnabled == 0)
			m_IsSecurityEnabled = parseInt(Values[13]);
	}
}

function InitFaceplate()
{
}

function ExitFaceplate()
{
}

function UpdateFaceplate()
{
	if (m_qDownloadGuid == 0 && m_qIsSecurityEnabled == 0)
	{
		if (m_OldDlGuid == "")
		{
			m_OldDlGuid = m_DownloadGuid;
		}
		else
		{
			if (m_OldDlGuid != m_DownloadGuid)
			{
				IdDownload.innerText = "DOWNLOAD";
				startblinkDownload(true);
				m_OldDlGuid = m_DownloadGuid;
				if (m_IsSecurityEnabled)
					CP_getPanelWindow().deleteCurrentSession();
			}
		}
	}

//	DoesExpire = 1;
//	Hours = 0;
	if (m_qDoesExpire == 0 && m_qHours == 0)
	{
		if (m_DoesExpire)
		{
			var Days = parseInt(m_Hours / 24);
			var restHours = parseInt(m_Hours) % 24;

			if (m_Hours <= 0)
				IdLicense.innerText = "License has expired !";
			else if (m_Hours < 48)
				IdLicense.innerText = "License expires in " + m_Hours.toString() + " hours";
			else
				IdLicense.innerText = "License expires in " + Days.toString() + " days, " + restHours.toString() + " hours";

			startblinkLicense(m_Hours < 24);
			if (m_Hours < 72 && m_Hours >= 24)
				IdLicense.style.color = "#ff0000";
		}
		else
		{
			IdLicense.innerText = "";
		}
	}
}

function setErrorText(errorObject, errorText)
{
	if (errorText != null && errorText.length > 0)
	{
		var date = new Date();
		if (errorObject != null && errorObject.length > 0)
			IdError.innerText = "ERROR [" + date.toLocaleString() + "] > " + errorObject + " : " + errorText;
		else
			IdError.innerText = "ERROR [" + date.toLocaleString() + "] > " + errorText;
	}
	else
		IdError.innerText = "";
}

function startblinkLicense(bOn)
{
	if (bOn && m_bBlinkLicense == 0)
	{
		m_bBlinkLicense = 1;
		if (m_bBlinkDownload == 0)
			blink();		
	}
	else if (!bOn)
		m_bBlinkLicense = 0;
}

function startblinkDownload(bOn)
{
	if (bOn && m_bBlinkDownload == 0)
	{
		m_bBlinkDownload = 1;
		if (m_bBlinkLicense == 0)
			blink();		
	}
	else if (!bOn)
		m_bBlinkDownload = 0;
}

function blink()
{
	if (m_bBlinkLicense == 1)
	{
		IdLicense.style.color = "#ff0000";
		m_bBlinkLicense = 2;
	}
	else if (m_bBlinkLicense == 2)
	{
		IdLicense.style.color = "#000000";
		m_bBlinkLicense = 1;
	}

	if (m_bBlinkDownload == 1)
	{
		IdDownload.style.color = "#ff0000";
		m_bBlinkDownload = 2;
	}
	else if (m_bBlinkDownload == 2)
	{
		IdDownload.style.color = "#000000";
		m_bBlinkDownload = 1;
	}

	if (m_bBlinkLicense != 0 || m_bBlinkDownload != 0 )
		setTimeout("blink()", 500);
}

</script>
</head>

<body class="StatusLine" onload=StatusInit() onUnload="StatusExit()">
<% 	GenerateProxyObj(); %>
<table  class=StatusLine   border="0" cellpadding="0" cellspacing="0" width="100%" height=50%>
<tr> 
	<td width=2> </td> 
	<td>
		<table border="0"  cellpadding="0" cellspacing="0" width="100%" height="100%">
			<td CLASS=TextOutFieldSunken> <NOBR> <div id="IdError"  style="color:#ff0000">  &nbsp </NOBR> </div> </td> 
		</table> 
	</td>
	<td width=2> </td> 
	<td width=90>
		<table border="0"  cellpadding="0" cellspacing="0" width="100%" height="100%">
			<td CLASS=TextOutFieldSunken align=center> <NOBR> <div id="IdDownload"  style="color:#ff0000"> &nbsp </NOBR> </div> </td> 
		</table> 
	</td>
	<td width=2> </td> 
	<td width=200>
		<table border="0"  cellpadding="0" cellspacing="0" width="100%" height="100%">
			<td CLASS=TextOutFieldSunken align=center> <NOBR> <div id="IdLicense"  > &nbsp </NOBR> </div> </td> 
		</table> 
	</td>
	<td width=2> </td> 
</tr>
</table> 

</body>
</html>

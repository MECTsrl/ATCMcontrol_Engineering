<% //ATCM. All rights reserved %>
<%
var __SEC_ConfigurationPath = "[" + Request.ServerVariables('SERVER_NAME') + "]";
var __SEC_VarPwd    = __SEC_ConfigurationPath + ".__system.security.Password";
var __SEC_VarEnable = __SEC_ConfigurationPath + ".__system.security.isEnabled";
var __SEC_VarRights = __SEC_ConfigurationPath + ".__system.security.Rights";

var __SEC_Enabled = null;
 //__SEC_OPCClient is the 4C_20_CSC.simpleOPC.1:
%>
<OBJECT RUNAT=Server SCOPE=PAGE ID=__SEC_OPCClient
	CLASSID="Clsid:D8BA2C14-8B10-11d5-8798-00010209D2BC">
</OBJECT>
<%
//******************************************************************************
// Checks if the current SessionId (passed via url parameter to the ASP page) 
// belongs to a valid session stored in the application object. If not the 
// current page will be redirected to the login page.
// 
//******************************************************************************
function CheckFaceplateSecurity()
{
	// if no rights -> redirection to "login.asp"
	if (CP_getSessionRights(__FpParams.getSessionId()) == "")
	{
		Response.Buffer = true;
		var NewUrl ="/4c/CP_Login.asp?url=" + Request.ServerVariables("URL");
		
		var Params = "&" + __FpParams.getFaceplateParamString();
		if (Params.length == 1)
			Params = ""

		NewUrl += Params;
		Response.Redirect(NewUrl);
	}

	if (__FpParams.isStandAlone())
		CP_generateKeepAlive(__FpParams.getSessionId());
}
%>

<%
//******************************************************************************
// Creates a single session. 
// 
// Return :   0   OK
//          < 0   Error
//******************************************************************************
function CP_createSession(
	user,						// user name (must not be empty)
	password,					// password (can be empty)
	rights)						// "rwx": read/write/execute, 
								// "rw": read/write, 
								// "r" : read, 
								// ""  : removes session
//******************************************************************************
{
	if (rights == null || (rights != "" && rights != "r"  && rights != "rw" && rights != "rwx"))
		return null;
	if (user == null || user == "")
		return null;
	if (password == null)
		password = "";

	var SessionId   = CP_generateSessionId();
	var SessionTime = "" + new Date().getTime();
	var Addr = "" + Request.ServerVariables("REMOTE_ADDR");

	Application.Lock();
	var strIds = Application("CP_SESSION_IDS");
	if (strIds == null || strIds == "")
	{
		Application("CP_SESSION_IDS")    = SessionId;
		Application("CP_SESSION_RIGHTS") = rights;
		Application("CP_SESSION_TIMES")  = SessionTime;
		Application("CP_SESSION_USER")   = user;
		Application("CP_SESSION_PWD")    = password;
		Application("CP_SESSION_ADDR")   = Addr;
	}
	else
	{
		Application("CP_SESSION_IDS")    = Application("CP_SESSION_IDS")    + "|" + SessionId;
		Application("CP_SESSION_RIGHTS") = Application("CP_SESSION_RIGHTS") + "|" + rights;
		Application("CP_SESSION_TIMES")  = Application("CP_SESSION_TIMES")  + "|" + SessionTime;
		Application("CP_SESSION_USER")   = Application("CP_SESSION_USER")   + "|" + user;
		Application("CP_SESSION_PWD")    = Application("CP_SESSION_PWD")    + "|" + password;
		Application("CP_SESSION_ADDR")   = Application("CP_SESSION_ADDR")   + "|" + Addr;
	}
	Application.Unlock();

	return SessionId;
}
%>
<%
//******************************************************************************
// Deletes a single session
// 
// Return :   0   OK
//          < 0   Error
//******************************************************************************
function CP_deleteSession(
	SessionId)					// id which defines the session
//******************************************************************************
{
	Application.Lock();
	var strIds    = Application("CP_SESSION_IDS");
	var strRights = Application("CP_SESSION_RIGHTS");
	var strTimes  = Application("CP_SESSION_TIMES");
	var strUsers  = Application("CP_SESSION_USER");
	var strPwds   = Application("CP_SESSION_PWD");
	var strAddr   = Application("CP_SESSION_ADDR");
	if (strIds == null || strIds == "")
	{
		strIds    = "";
		strRights = "";
		strTimes  = "";
		strUsers  = "";
		strPwds   = "";
		strAddr   = "";
	}
	else
	{
		var IdArray    = strIds.split("|");
		var RightArray = strRights.split("|");
		var TimeArray  = strTimes.split("|");
		var UserArray  = strUsers.split("|");
		var PwdArray   = strPwds.split("|");
		var AddrArray  = strAddr.split("|");
		strIds    = "";
		strRights = "";
		strTimes  = "";
		strUsers  = "";
		strPwds   = "";
		strAddr   = "";

		var count = 0;
		for (var ii = 0; ii < IdArray.length; ii++)
		{
			if (SessionId != IdArray[ii])
			{
				if (count > 0)
				{
					strIds    += "|" + IdArray[ii];
					strRights += "|" + RightArray[ii];
					strTimes  += "|" + TimeArray[ii];
					strUsers  += "|" + UserArray[ii];
					strPwds   += "|" + PwdArray[ii];
					strAddr   += "|" + AddrArray[ii];
				}
				else
				{
					strIds    = IdArray[ii];
					strRights = RightArray[ii];
					strTimes  = TimeArray[ii];
					strUsers  = UserArray[ii];
					strPwds   = PwdArray[ii];
					strAddr   = AddrArray[ii];
				}
				count++;
			}
		}
	}

	Application("CP_SESSION_IDS")    = strIds;
	Application("CP_SESSION_RIGHTS") = strRights;
	Application("CP_SESSION_TIMES")  = strTimes;
	Application("CP_SESSION_USER")   = strUsers;
	Application("CP_SESSION_PWD")    = strPwds;
	Application("CP_SESSION_ADDR")   = strAddr;
	Application.Unlock();

	return 0;
}
%>

<%
//******************************************************************************
// Resets the session timeout of single session and removes all sessions, which
// have not been updated during the last 60 secs.
// 
// Return :   0   OK
//          < 0   Error
//******************************************************************************
function CP_keepAlive(
	SessionId)					// id which defines the session
//******************************************************************************
{
	Application.Lock();
	var strIds    = Application("CP_SESSION_IDS");
	var strRights = Application("CP_SESSION_RIGHTS");
	var strTimes  = Application("CP_SESSION_TIMES");
	var strUsers  = Application("CP_SESSION_USER");
	var strPwds   = Application("CP_SESSION_PWD");
	var strAddr   = Application("CP_SESSION_ADDR");
	if (strIds == null || strIds == "")
	{
		strIds    = "";
		strRights = "";
		strTimes  = "";
		strUsers  = "";
		strPwds   = "";
		strAddr   = "";
	}
	else
	{
		var IdArray    = strIds.split("|");
		var RightArray = strRights.split("|");
		var TimeArray  = strTimes.split("|");
		var UserArray  = strUsers.split("|");
		var PwdArray   = strPwds.split("|");
		var AddrArray  = strAddr.split("|");
		strIds    = "";
		strRights = "";
		strTimes  = "";
		strUsers  = "";
		strPwds   = "";
		strAddr   = "";

		var count = 0;
		var currTime = new Date().getTime();
		for (var ii = 0; ii < IdArray.length; ii++)
		{
			var age = currTime - parseInt(TimeArray[ii]);
			if (age < 2*CP_getTimeOut())
			{
				if (SessionId == IdArray[ii] || age < 0)
					TimeArray[ii] = "" + currTime;

				if (count > 0)
				{
					strIds    += "|" + IdArray[ii];
					strRights += "|" + RightArray[ii];
					strTimes  += "|" + TimeArray[ii];
					strUsers  += "|" + UserArray[ii];
					strPwds   += "|" + PwdArray[ii];
					strAddr   += "|" + AddrArray[ii];
				}
				else
				{
					strIds    = IdArray[ii];
					strRights = RightArray[ii];
					strTimes  = TimeArray[ii];
					strUsers  = UserArray[ii];
					strPwds   = PwdArray[ii];
					strAddr   = AddrArray[ii];
				}
				count++;
			}
		}
	}

	Application("CP_SESSION_IDS")    = strIds;
	Application("CP_SESSION_RIGHTS") = strRights;
	Application("CP_SESSION_TIMES")  = strTimes;
	Application("CP_SESSION_USER")   = strUsers;
	Application("CP_SESSION_PWD")    = strPwds;
	Application("CP_SESSION_ADDR")   = strAddr;
	Application.Unlock();

	return 0;
}
%>

<%
//******************************************************************************
// Returns the rights of a single session. If the session does not exist an
// empty string will be returned
// 
// Return : "rwx" read/write/execute
//          "rw"  read/write
//          "r"   read
//          ""    no rights -> session does not exsit
//******************************************************************************
function CP_getSessionRights(
	SessionId)					// id which defines the session
//******************************************************************************
{
	if (SessionId == null || SessionId.length < 1)
		return "";

	// get ids and rights
	Application.Lock();
	var strIds    = Application("CP_SESSION_IDS");
	var strRights = Application("CP_SESSION_RIGHTS");
	var strAddr   = Application("CP_SESSION_ADDR");
	var strTimes  = Application("CP_SESSION_TIMES");
	Application.Unlock();

	if (strIds == null || strIds.length == 0)
		return "";
	if (strRights == null || strRights.length == 0)
		return "";
	var IdArray    = strIds.split("|");

	// look for the id and return corresponding rights
	for (var ii = 0; ii < IdArray.length; ii++)
	{
		if (IdArray[ii] == SessionId)
		{
			var Addr = "" + Request.ServerVariables("REMOTE_ADDR");
			var AddrArray  = strAddr.split("|");
			if (Addr == AddrArray[ii])
			{
				var currTime = new Date().getTime();
				var TimeArray  = strTimes.split("|");
				if (currTime - parseInt(TimeArray[ii]) < 2*CP_getTimeOut())
				{
					var RightArray = strRights.split("|");
					return RightArray[ii];
				}
			}
			return "";
		}
	}
	return "";
}
%>

<%
//******************************************************************************
// Wrapper functions for session rights
//
//******************************************************************************
function CP_canSessionRead(SessionId)    { return CP_getSessionRights(SessionId).indexOf('r') >= 0; }
function CP_canSessionWrite(SessionId)   { return CP_getSessionRights(SessionId).indexOf('w') >= 0; }
function CP_canSessionExecute(SessionId) { return CP_getSessionRights(SessionId).indexOf('x') >= 0; }
%>

<%
//******************************************************************************
// Returns the user name of a single session. If the session does not exist an
// empty string will be returned
// 
// Return : user name
//******************************************************************************
function CP_getSessionUser(
	SessionId)					// id which defines the session
//******************************************************************************
{
	if (SessionId == null || SessionId.length < 1)
		return "";

	// get ids and users
	Application.Lock();
	var strIds   = Application("CP_SESSION_IDS");
	var strUsers = Application("CP_SESSION_USER");
	Application.Unlock();

	if (strIds == null || strIds.length == 0)
		return "";
	if (strUsers == null || strUsers.length == 0)
		return "";
	var IdArray   = strIds.split("|");
	var UserArray = strUsers.split("|");

	// look for the id and return corresponding user
	for (var ii = 0; ii < IdArray.length; ii++)
	{
		if (IdArray[ii] == SessionId)
			return UserArray[ii];
	}

	return "";
}
%>

<%
//******************************************************************************
// Returns the password of a single session. If the session does not exist an
// empty string will be returned
// 
// Return : password
//******************************************************************************
function CP_getSessionPwd(
	SessionId)					// id which defines the session
//******************************************************************************
{
	if (SessionId == null || SessionId.length < 1)
		return "";

	// get ids and rights
	Application.Lock();
	var strIds  = Application("CP_SESSION_IDS");
	var strPwds = Application("CP_SESSION_PWD");
	Application.Unlock();

	if (strIds == null || strIds.length == 0)
		return "";
	if (strPwds == null || strPwds.length == 0)
		return "";
	var IdArray  = strIds.split("|");
	var PwdArray = strPwds.split("|");

	// look for the id and return corresponding password
	for (var ii = 0; ii < IdArray.length; ii++)
	{
		if (IdArray[ii] == SessionId)
			return PwdArray[ii];
	}
	return "";
}
%>

<%
//******************************************************************************
// Creates a new session if the given user and password are valid.
// 
// Return : == null   login failed
//          != null   session id
//******************************************************************************
function CP_StartSession(
	strUser,					//
	strPassword)				//
//******************************************************************************
{
	if (strUser == null || strUser.length < 1)
		return null;

	var rights = ""
	if (CP_isSecurityEnabled())
	{
		if (strPassword == null)
			strPassword = "";

		__SEC_OPCClient.Subscribe(__SEC_VarPwd);
		__SEC_OPCClient.Value(__SEC_VarPwd) = strPassword;


		var iRights = 0;
        __SEC_OPCClient.Subscribe(__SEC_VarRights);
		for (var ii = 0; ii < 20 && iRights < 2; ii++)
		{
			iRights = __SEC_OPCClient.Value(__SEC_VarRights);
		}
		__SEC_OPCClient.Unsubscribe(__SEC_VarRights);
//		__SEC_OPCClient.Unsubscribe(__SEC_VarPwd);

		if (iRights != null)
		{
			if (iRights == 2) // read only rights
				rights = "r";
			else if (iRights == 3 || // normal rw-rights
					 iRights == 4)   // engineering rights
				rights = "rwx";		// right now, the "x"-right is mapped to the "w"-right
		}
	}
	else
	{
		rights = "rwx"
	}

	if (rights != "")
		return CP_createSession(strUser, strPassword, rights);

	return null;
}
%>

<%
function CP_generateSessionId() { return "" + new Date().getTime(); }
function CP_getTimeOut() { return 60000; }
%>


<%
//******************************************************************************
// reads from control if security is enabled
// 
// Return : true  : security is enabled
//			false : security is disabled
// 
//******************************************************************************
function CP_isSecurityEnabled() 
{ 
	if (__SEC_Enabled == null)
	{
		__SEC_OPCClient.Subscribe(__SEC_VarEnable)
		__SEC_Enabled = __SEC_OPCClient.Value(__SEC_VarEnable);
		__SEC_OPCClient.Unsubscribe(__SEC_VarEnable)
	}
	return __SEC_Enabled; 
}
%>


<%
//******************************************************************************
// 
// 
// 
// 
//******************************************************************************
function CP_generateKeepAlive(
	SessionId)					// id which defines the session
//******************************************************************************
{%>
<script language="JavaScript">
function keepAlive()
{
    top.FC_KeepAlive.navigate("<%='http://' + Request.ServerVariables('SERVER_NAME') + '/4C/CP_KeepAlive.asp?SessionId=' + SessionId%>");
	setTimeout("keepAlive()", <%=CP_getTimeOut()/2%>);
}
</script>
<%
}
%>


<%
function GenerateLoginFunction(MaxControls)
{
	if (CP_isSecurityEnabled()) {%>
<script language="JavaScript">
var _CP_LoginOk = new Array(<%=MaxControls%>);
var _CP_LoginInitDone = new Array(<%=MaxControls%>);
function doLogin(SecurityPath, index)
{
	if (_CP_LoginOk[index] == null || !_CP_LoginOk[index])
	{
		if (_CP_LoginInitDone[index] == null || !_CP_LoginInitDone[index])
		{
			SubscribeOPCList(SecurityPath + ".Rights", "");
			CpProxy.Subscribe(SecurityPath + ".Password");
			_CP_LoginInitDone[index] = true;
		}
		else
		{
			var values = ReadOPCList(SecurityPath + ".Rights", "").split("<%=getVarlistSeparator()%>");
			if (TranslateOPCQuality (values[0]) == 0)
			{
				var Rights = parseInt(values[1]);
				if (Rights != null)
				{
					CpProxy.Write(SecurityPath + ".Password", "<%=CP_getSessionPwd(__FpParams.getSessionId())%>");

					CpProxy.Unsubscribe(SecurityPath + ".Password");
					UnsubscribeOPCList(SecurityPath + ".Rights", "");
					_CP_LoginOk[index] = true;
				}
			}
		}
	}
}
</script>
<% 
	}
}
%>

<%
function GenerateLoginCall(ObjPath, index)
{
	if (CP_isSecurityEnabled()) 
	{
		var SecurityPath = ObjPath.match(/\[[^\[]+\]/);
		SecurityPath += ".__system.security";
%>
	doLogin("<%=SecurityPath%>", <%=index%>);
<%
	}
}
%>
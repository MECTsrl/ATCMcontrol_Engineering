<%@ LANGUAGE = JScript %>
<% //ATCM. All rights reserved %>

<% // This ASP stores layout files on the server and retrieves them. %>

<!-- #INCLUDE VIRTUAL = "/4C/CP_LibAspParam.asp" -->
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibSecurity.asp" -->

<OBJECT RUNAT=Server SCOPE=PAGE ID=IdLayoutIo
CLASSID="Clsid:766287F2-9000-11d5-A550-0008C779C600">
</OBJECT> 

<SCRIPT RUNAT=SERVER LANGUAGE=JScript>

function sendErrorMessage(Message)
{
	Response.Write("4CP_NOK: Error: " + Message);
}

function checkForNullAndWhitespace(value)
{
	if (value == null)
	{
		return false;
	}
	if (value.split(new RegExp("\\s", "g")) == "")
	{	
		// value contains only whitespace.
		return false;
	}
	return true;
}

function checkForIllegalCharacters(text)
{
    var textString = new String(text);
	if ((textString.lastIndexOf(";")  != -1) ||
	    (textString.lastIndexOf("\"") != -1) ||
	    (textString.lastIndexOf("\'") != -1) ||
	    (textString.lastIndexOf("\\") != -1) ||
	    (textString.lastIndexOf("?")  != -1) ||
	    (textString.lastIndexOf("&")  != -1) ||
	    (textString.lastIndexOf(">")  != -1) ||
	    (textString.lastIndexOf("<")  != -1) ||
	    (textString.lastIndexOf("|")  != -1) ||
	    (textString.lastIndexOf(",")  != -1) ||
	    (textString.lastIndexOf(".")  != -1) ||
	    (textString.lastIndexOf("/")  != -1) ||
	    (textString.lastIndexOf("*")  != -1) ||
	    (textString.lastIndexOf(":")  != -1))
	{
		return false;
	}
	else
	{
		// textString does not contain any illegal character.
		return true;
	}
}

function getLayoutname()
{
	var nameOfLayoutnameParam = "layoutname";
	var layoutname = readFormParam(nameOfLayoutnameParam);
	if (!checkForNullAndWhitespace(layoutname))
	{
		sendErrorMessage("missing " + nameOfLayoutnameParam);
		return null;
	}
	if (!checkForIllegalCharacters(layoutname))
	{
		sendErrorMessage("illegal " + nameOfLayoutnameParam);
		return null;
	}
	return layoutname;
}

function getTargetLayoutname()
{
	var nameOfLayoutnameParam = "targetlayoutname";
	var layoutname = readFormParam(nameOfLayoutnameParam);
	if (!checkForNullAndWhitespace(layoutname))
	{
		sendErrorMessage("missing " + nameOfLayoutnameParam);
		return null;
	}
	if (!checkForIllegalCharacters(layoutname))
	{
		sendErrorMessage("illegal " + nameOfLayoutnameParam);
		return null;
	}
	return layoutname;
}

function getLayout()
{
	var nameOfLayoutParam = "layout";
	var layout = readFormParam(nameOfLayoutParam);
	if (!checkForNullAndWhitespace(layout))
	{
		sendErrorMessage("missing " + nameOfLayoutParam);
		return null;
	}
	return layout;
}

function getUsername()
{
	var username = CP_getSessionUser(__FpParams.getSessionId());
	if ((!checkForIllegalCharacters(username)) || (username == ""))
	{
		sendErrorMessage("illegal user name" );
		return null;
	}
	return username;
}

function Save()
{
	var username = getUsername();
	if (username == null) return;

	var layoutname = getLayoutname();
	if (layoutname == null) return;

	var layout     = getLayout();
	if (layout == null) return;


    IdLayoutIo.Init(Server.MapPath("."), username);
    IdLayoutIo.Save(layoutname, layout);
    Response.Write("4CP__OK: ");
}

function Delete()
{
	var username = getUsername();
 	if (username == null) return;

	var layoutname = getLayoutname();
	if (layoutname == null) return;

    IdLayoutIo.Init(Server.MapPath("."), username);
    IdLayoutIo.Delete(layoutname);
    Response.Write("4CP__OK: ");
}

function Read()
{
	var username = getUsername();
	if (username == null) return;

	var layoutname = getLayoutname();
	if (layoutname == null) return;

    IdLayoutIo.Init(Server.MapPath("."), username);
    var layout = IdLayoutIo.Read(layoutname);
    Response.Write("4CP__OK: " + layout);
}

function Copy()
{
    var username = getUsername();
    if (username == null) return;

    var layoutname = getLayoutname();
    if (layoutname == null) return;

    var targetlayoutname = getTargetLayoutname();
    if (targetlayoutname == null) return;

    IdLayoutIo.Init(Server.MapPath("."), username);
    var layout = IdLayoutIo.Read(layoutname);

    IdLayoutIo.Save(targetlayoutname, layout);
    Response.Write("4CP__OK: ");
}

function GetNameList()
{
	var username = getUsername();
	if (username == null) return;

	IdLayoutIo.Init(Server.MapPath("."), username);
	var list = IdLayoutIo.GetNameList();
    Response.Write("4CP__OK: " + list);
}

function GetDefaultName()
{
	var username = getUsername();
	if (username == null) return;

	IdLayoutIo.Init(Server.MapPath("."), username);
	var defaultName = IdLayoutIo.GetDefaultName();
    Response.Write("4CP__OK: " + defaultName);
}

function SetDefaultName()
{
	var username = getUsername();
	if (username == null) return;

	var layoutname = getLayoutname();
	if (layoutname == null) return;

    IdLayoutIo.Init(Server.MapPath("."), username);
    IdLayoutIo.SetDefaultName(layoutname);
    Response.Write("4CP__OK: ");
}

function switchToAction()
{
	var nameOfActionParam = "action";
	var action = readFormParam(nameOfActionParam);
	switch (action)
	{
		case "Save": 
			Save();
			break;
		case "Delete": 
			Delete();
			break;
		case "Read": 
			Read();
			break;
		case "GetNameList": 
			GetNameList();
			break;
		case "GetDefaultName": 
			GetDefaultName();
			break;
		case "SetDefaultName": 
			SetDefaultName();
			break;
		case "Copy":
			Copy();
			break;
		default:
			sendErrorMessage("no or wrong " + nameOfActionParam + " specified");
	}
}

switchToAction();

</SCRIPT>

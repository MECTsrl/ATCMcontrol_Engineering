<%@ LANGUAGE = JScript %>
<% //ATCM. All rights reserved %>
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibAspParam.asp" -->
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibSecurity.asp" -->
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibHtml.asp" -->
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibButton.asp" -->
<html>
<head>
<%GenerateStdHeader("Header")%>
<%GenerateStdIncludes()%>
<%GenerateButtonFunctions()%>

<script language="JavaScript">


var m_bUnsavedSelection = false;

function init() 
{
	InitAllButtons();
	setHeaderLoaded();	
	
	setTimeout("init2()", 500);
}

function init2() 
{
	if (isAllLoaded())
	{
		initSelection(CP_getMainWindow().readViewList());
		setSelection(CP_getMainWindow().readViewName());
	}
	else
	{
		setTimeout("init2()", 500);
	}
}

function exit() 
{
	resetHeaderLoaded();
}

// event handler for "edit layout" button
function onEditLayout()
{
	if (isAllLoaded())	
		CP_getMainWindow().onEditLayout(); 
}

// event handler for "save layout" button
function onSaveLayout()
{
	if (isAllLoaded())	
	{
		showModalDialog('CP_ViewDialog.html', CP_getMainWindow(), "dialogWidth:600px;dialogHeight:280px;help:no;center:yes");
	}
}

// event handler for "toggle navigator" button
function onToggleNavigator()
{
	if (isAllLoaded())
	{
		var visible = !CP_getMainWindow().isNavigatorVisible();
		CP_getMainWindow().showNavigator(visible);
		setNavigatorButton(visible);
	}
}

function setNavigatorButton(bPress)
{
	if (isAllLoaded())
		Button2.setPressed(bPress);
}


// event handler for "help" button
function onHelp()
{
	window.open("help/docustart.htm", null, "scrollbars=1 resizable=1");
}

// event handler for "logout" button
<%if (CP_isSecurityEnabled()) {%>
function onLogout(id)
{
	if (isAllLoaded())	
		CP_getPanelWindow().Logout();
}
<%}%>

// event handler for "about" button
function onAbout()
{
	showModalDialog("/4C/CP_About.html", null, "dialogWidth:600px;dialogHeight:540px;help:no;center:yes");				
}

//============================================


function initSelection(NameList)
{
	m_bUnsavedSelection = false;
	IdSelect.selectedIndex = -1;
	if (NameList != null && NameList.length > 0) 
	{
		var NameArray = NameList.split(";");
		var ii;
		var OldLength = IdSelect.options.length;

		for (ii = OldLength-1; ii >= 0; ii--)
			IdSelect.remove(ii);

		for (ii = 0; ii < NameArray.length; ii++)
		{
			if (NameArray[ii].length > 0 && NameArray[ii] != "_tmp281260")
			{
				var element = document.createElement("OPTION");
				element.text = NameArray[ii];
				IdSelect.add(element);
			}
		}
	}
	if (IdSelect.options.length == 0)
		IdSelect.disabled = true;
	else
		IdSelect.disabled = false;
}

function setSelection(name)
{
	removeUnsavedSelection();
	var Length = IdSelect.options.length;
	for (ii = 0; ii < Length; ii++)
	{
		if (IdSelect.options[ii].text == name)
		{
			IdSelect.selectedIndex = ii;
			return;
		}
	}
	addUnsavedSelection();
}

function removeUnsavedSelection()
{
	if (IdSelect.options.length > 0 && m_bUnsavedSelection)
	{
		if (IdSelect.selectedIndex == 0)
			IdSelect.selectedIndex = -1;
		IdSelect.remove(0);
	}
	m_bUnsavedSelection = false;
}
function addUnsavedSelection()
{
	if (!m_bUnsavedSelection)
	{
		var element = document.createElement("OPTION");
		element.text = "[Unsaved layout]";
		IdSelect.add(element, 0);
		IdSelect.selectedIndex = 0;
	}
	
	if (IdSelect.options.length == 1)
		IdSelect.disabled = true;
	m_bUnsavedSelection = true;
}

function getSelection()
{
	if (m_bUnsavedSelection)
		return null;

	if (IdSelect.selectedIndex >= 0)
		return IdSelect.options[IdSelect.selectedIndex].text;
	else
		return null;		
}

function updateSelection()
{
	if (m_bUnsavedSelection && IdSelect.selectedIndex == 0) 
		return;

	removeUnsavedSelection();
	if (isMainLoaded())
		CP_getMainWindow().onSelectView(getSelection());
}
</script>
</head>

<body class="Header" onload=init() onUnload="exit()">

<table border="1" cellpadding="0" cellspacing="0" width="100%" height="47">
    <tr>
		<td width= 430> 
			<table border="0" cellpadding="0" cellspacing="0" width="100%" height=100%>        
				<tr>
					<td width=300>
						<table class=HeaderObject border="0" cellpadding="0" cellspacing="0" width="100%" height=100%>        
							<tr> 
								<td> Layout </td>
							</tr>
							<tr> 
								<td valign="top" width=100%> 
									<select class=ConsoleObject id=IdSelect size="1" style="width:298"  onchange="updateSelection()"> </select> 
								</td>        
							</tr>
						</table>
					</td>
					<td width=60>   
						<% GenerateButton("Disk", "images/CP_LayoutSave.gif", "null", "Save Layout", "flat", true, false, new Function('onSaveLayout()'));%>
					</td>
					<td width=60>		
						<% GenerateButton("Edit", "images/CP_LayoutEdit.gif", "null", "Edit Layout", "flat", true, false, new Function('onEditLayout()'));%> 		
					</td> 
				</tr>
			</table>
		</td>
		<td height=100%>
			<table border="0" cellpadding="0" cellspacing="0" width="100%" height=100%>        
				<tr>
					<td > &nbsp </td>
					<td width=60> 
						<% GenerateButton("Navigator", "images/CP_Navigator.gif", "null", "Hide/Show Navigator", "toggle", true, true, new Function('onToggleNavigator()'))%>
					</td>   
					<%if (CP_isSecurityEnabled()) {%>
					<td width= 60 > 
						<% GenerateButton("Log out", "images/CP_Logout.gif", "null", "Log out", "flat", true, false, new Function('onLogout()'))%>
					</td>        
					<%}%>
					<td width=60> 
						<% GenerateButton("Help", "images/CP_Help.gif", "null", "Open Help Screen", "flat", true, false, new Function('onHelp()'))%>
					</td>
				</tr>
			</table>
		</td>
		<td width=44> 
			<% GenerateButton("About", "images/FC_Logo.gif", "null", "About ATCMControl", "flat", true, false, new Function('onAbout()'))%>
		</td>
    </tr>
</table>

<% GenerateFunctionInitAllButtons() %>

<applet
    code=CP_ASPStarter.class
    name=CP_ASPStarter
    width=0
    height=0 VIEWASTEXT id=Applet2>
</applet>

</body>
</html>

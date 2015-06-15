// ATCM. All rights reserved

function CP_getProxy()           { return CpProxy; }
function CP_isPanelContext()     { return top.FC_Main != null ? true : false; }

function CP_getPanelWindow()     { return CP_isPanelContext() ? top               : null; }
function CP_getMainWindow()      { return CP_isPanelContext() ? top.FC_Main       : null; }
function CP_getNavigatorWindow() { return CP_isPanelContext() ? top.FC_Navigator  : null; }
function CP_getHeaderWindow()    { return CP_isPanelContext() ? top.FC_Header     : null; }
function CP_getStatusWindow()    { return CP_isPanelContext() ? top.FC_StatusLine : null; }

function setFrameLoaded()		{ setLoaded(1); }
function setHeaderLoaded()		{ setLoaded(2); }
function setNavigatorLoaded()	{ setLoaded(4); }
function setViewLoaded()		{ setLoaded(8); }
function setStatusLoaded()	    { setLoaded(16); }
function setMainLoaded()		{ setLoaded(32); }

function resetFrameLoaded()		{ resetLoaded(1); }
function resetHeaderLoaded()	{ resetLoaded(2); }
function resetNavigatorLoaded()	{ resetLoaded(4); }
function resetViewLoaded()		{ resetLoaded(8); }
function resetStatusLoaded()    { resetLoaded(16); }
function resetMainLoaded()		{ resetLoaded(32); }

function isFrameLoaded()     { return isLoaded(1); }
function isHeaderLoaded()    { return isLoaded(2); }
function isNavigatorLoaded() { return isLoaded(4); }
function isViewLoaded()      { return isLoaded(8); }
function isStatusLoaded()    { return isLoaded(16); }
function isMainLoaded()      { return isLoaded(32); }

function CP_getNavigator()
{
	if (CP_isPanelContext())
	{
		return top.FC_Navigator.FC_OCX_Navigator;
	}
	else
	{	
		return this.top.opener.top.FC_Navigator.FC_OCX_Navigator;
	}		 
}

function CP_getMainWindow2()
{
	if (CP_isPanelContext())
	{
		return top.FC_Main;
	}
	else
	{	
		return this.top.opener.top.FC_Main;
	}		 
}

function isAllLoaded() 
{
	if (CP_isPanelContext())
	{
		var w = CP_getPanelWindow();
		var wMain = CP_getMainWindow();

		if (w != null && wMain != null && wMain.frames != null)
			return ((w.m_LoadStatus & 63) == 63) && (wMain.frames.length > 0);
		else
			return false;
	}
	else
	{
		return true;
	}
}

function setLoaded(val) 
{ 
	var w = CP_getPanelWindow();
	if (w != null)
		w.m_LoadStatus |= val;
}

function isLoaded(val) 
{
	var w = CP_getPanelWindow();
	if (w != null)
		return (w.m_LoadStatus & val) != 0;
	else
		return false;
}

function resetLoaded(val) 
{ 
	var w = CP_getPanelWindow();
	if (w != null)
		w.m_LoadStatus &= ~val;
}

function errortrap_ActiveXNotLoaded(msg,url,line)
{
	document.open();
	document.writeln("<html> <body bgcolor='#E0E0E0'>");
	document.writeln("<p align='center'><font color='#FF0000' size='5'><strong>Fatal Error !</strong></font></p>");
	document.writeln("<p align='center'><font size='3' face='MS Sans Serif'>An ActiveX Control could not be Loaded !</font></p>");
	document.writeln("<p align='center'><font size='3' face='MS Sans Serif'>Check the Security Settings of your Browser for ActiveX Controls !</font></p>");
	document.writeln("</body> </html>");
	document.close();
	return true;
}

<%@ LANGUAGE = JScript %>
<% //ATCM. All rights reserved %>
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibFaceplate.asp" -->
<!-- #INCLUDE VIRTUAL = "/4C/CP_View.asp" -->

<OBJECT RUNAT=Server SCOPE=PAGE ID=IdLayoutIo
CLASSID="Clsid:766287F2-9000-11d5-A550-0008C779C600">
</OBJECT> 

<HTML>
<HEAD>
<%GenerateStdHeader("Faceplate Container")%>
<%
var m_SessionId  = __FpParams.getSessionId();

var m_View = new CP_View(0, 0);

IdLayoutIo.Init(Server.MapPath("/4C"), CP_getSessionUser(m_SessionId));
var layout = IdLayoutIo.Read("_tmp281260");
m_View.fromCookieString(layout);
%>

<script language="JavaScript" src="CP_Panel.js"></script>
<script language="JavaScript" src="CP_ViewMatrix.js"></script>
<script language="JavaScript" src="CP_View.js"></script>

<script language="JavaScript">

// the current view object
var m_View = new CP_View(0, 0);
if (!m_View.fromCookieString("<%=layout%>"))
	m_View.init(new Array(0,1,2,3, 0,1,2,3, 0,1,2,3, 0,1,2,3));		

// index of frame which has been selected
var m_FocusIndex = 0;

function init() 
{
	m_FocusIndex = 0;
	setFrameLoaded();
	setViewLoaded();

	showNavigator(isNavigatorVisible());
	CP_getHeaderWindow().setNavigatorButton(isNavigatorVisible());
}

function exit() 
{
	resetViewLoaded();
	resetFrameLoaded();
}

function beforeExit() 
{
}

function clearAllFrames() 
{
	if (isAllLoaded())
	{
		for (ii = 0; ii < frames.length; ii++)
			frames[ii].navigate("/4C/CP_Loader.asp?AspFile=/4C/CP_Empty.asp&ObjName=Empty");
	}
}

function clearFrame(frame) 
{
	if (isAllLoaded())
	{
		frame.navigate("/4C/CP_Loader.asp?AspFile=/4C/CP_Empty.asp&ObjName=Empty");
	}
}

function setFocusFrame(frame) 
{
	for (ii = 0; ii < frames.length; ii++)
	{
		if (frame == frames[ii])
			return setFocusIndex(ii);
	}
	return 0;
}

function getFocusFrame() 
{
	return frames[m_FocusIndex];
}

function setFocusIndex(index) 
{
	var bFound = 0;
	index = index % frames.length;
	for (ii = 0; ii < frames.length; ii++)
	{
		if (ii == index)
		{
			m_FocusIndex = ii;
			bFound = 1;
			if (frames[ii].frames[0].document.CpHeader != null && 
				frames[ii].frames[0].document.CpHeader.setFocus != null)
			{
				frames[ii].frames[0].document.CpHeader.setFocus(1);
			}
		}
		else
		{
			if (frames[ii].frames[0].document.CpHeader != null && 
				frames[ii].frames[0].document.CpHeader.setFocus != null)
			{
				frames[ii].frames[0].document.CpHeader.setFocus(0);
			}
		}
	}
	m_FocusIndex = index;
	return bFound;
}

function loadPage(URL, bStandAlone) 
{
	if (isAllLoaded())
	{
		if (frames.length >= m_FocusIndex && m_FocusIndex >= 0)
		{
			URL = URL.split("\\").join("/"); // replace "\" with "/"
			if (frames[m_FocusIndex] != null)
			{
				if (bStandAlone != 0)
				{
					var LoginUrl  = Url2LoginUrl(URL);
					if (LoginUrl != null)
						window.open(LoginUrl + "&StandAlone=1", null, "height=400,width=200,resizable=yes,status=no,toolbar=no,menubar=no,location=no");
				}
				else
				{
					var LoaderUrl = Url2LoaderUrl(URL);
					if (LoaderUrl != null)
					{
						frames[m_FocusIndex].navigate(LoaderUrl);
						setFocusIndex(m_FocusIndex + 1); 
					}
				}
			}
		}
	}
}

function Url2LoaderUrl(URL)
{
	var param = ""
	var html = URL;
	var ii = URL.indexOf("?");
	if (ii > 0)
	{
		param = "&" + URL.substr(ii + 1);     // url parameter
		html = URL.substr(0, ii);	// url without params
	}

	var path = URL.match(/http:\/\/[^/]+\/4c\//i); // e.g. http://w-mkr/4c/
	if (path != null && path.length > 0)
		html = html.substr(path[0].length);		// url without path and params
	return "/4C/CP_Loader.asp?AspFile=/4C/" + html + param + "&SessionId=<%=m_SessionId%>";
}

function Url2LoginUrl(URL)
{
	var param = ""
	var html = URL;
	var ii = URL.indexOf("?");
	if (ii > 0)
	{
		param = "&" + URL.substr(ii + 1);     // url parameter
		html = URL.substr(0, ii);
	}

	var path = URL.match(/http:\/\/[^/]+\/4c\//i); // e.g. http://w-mkr/4c/
	if (path != null && path.length > 0)
		html = html.substr(path[0].length);		// url without path and params

	return "/4C/CP_Login.asp?url=" + "/4C/CP_Loader.asp" + "&AspFile=/4C/" + html + param + "&password=<%=CP_getSessionPwd(m_SessionId)%>";
}

function updateView(strURL, strFrameName) 
{
	if (isAllLoaded())
	{
		if (strURL != null && strURL.length > 0 && strFrameName != null)
		{
			if (m_View != null && m_View.getSizeX() > 0)
			{
				var path = strURL.match(/http:\/\/[^/]+\//i); // e.g. http://w-mkr/
				if (path != null && path.length > 0)
					strURL = "/" + strURL.substr(path[0].length);

				var label = CPView_FrameNameToLabel(strFrameName);

				if (m_View.getPage(label) != CPView_RemoveSessionId(strURL))
					CP_getHeaderWindow().addUnsavedSelection();
				m_View.setPage(label, strURL);
			}
		}
	}
}

function merge(frame, direction)
{
	if (isAllLoaded())
	{
		var label = CPView_FrameNameToLabel(frame.name);
		m_View.merge(label, direction);		
		writeView("_tmp281260", m_View);
		location.reload();
		
		CP_getHeaderWindow().addUnsavedSelection();
	}
}

function split(frame, direction)
{
	if (isAllLoaded())
	{
		var label = CPView_FrameNameToLabel(frame.name);
		m_View.split(label, direction);
		writeView("_tmp281260", m_View);
		location.reload();

		CP_getHeaderWindow().addUnsavedSelection();
	}
}

function isNavigatorVisible()
{
	if (isAllLoaded())
		return m_View.isNavigatorVisible();
	else
		return false;
}

function showNavigator(bShow)
{
	if (isAllLoaded())
	{
		if (bShow)
		{
			CP_getPanelWindow().setNavigatorWidth(m_View.getNavigatorWidth());
		}
		else
		{
			if (m_View.isNavigatorVisible()) {
			var nw = CP_getNavigatorWindow().document.body.clientWidth;
			var mw = document.body.clientWidth;
			var iWidth = (100 * nw) / (nw + mw);
			m_View.setNavigatorWidth(parseInt(iWidth + 0.5));
			}

			CP_getPanelWindow().setNavigatorWidth(0);
		}

		if (bShow != m_View.isNavigatorVisible())
			CP_getHeaderWindow().addUnsavedSelection();

		m_View.setNavigatorVisible(bShow);
		
	}
}

function setNavigatorWidth(iWidth)
{
	if (isAllLoaded())
		m_View.setNavigatorWidth(iWidth);
}

function getView() 
{
	return m_View;
}

<%
var m_LayoutPage = "http://" + Request.ServerVariables('SERVER_NAME') + "/4C/CP_Layouts.asp?SessionId=" + m_SessionId;
%>
function readViewList() 
{
	CP_getHeaderWindow().CP_ASPStarter.startASP("<%=m_LayoutPage%>", "action", "GetNameList");

	var str = CP_getHeaderWindow().CP_ASPStarter.getASPOutput();
	if (str.indexOf("4CP__OK: ") == 0 && str.length > 9)
		return str.substr(9);
	else
		return "";
}

function readViewName() 
{
	CP_getHeaderWindow().CP_ASPStarter.startASP("<%=m_LayoutPage%>", "action", "GetDefaultName");
	var str = CP_getHeaderWindow().CP_ASPStarter.getASPOutput();
	if (str.indexOf("4CP__OK: ") == 0 && str.length > 9)
		return str.substr(9);
	else
		return "";
}

function writeViewName(ViewName) 
{
	CP_getHeaderWindow().CP_ASPStarter.startASP("<%=m_LayoutPage%>", "action", "SetDefaultName", "layoutname", ViewName);
}

function deleteView(ViewName) 
{
	CP_getHeaderWindow().CP_ASPStarter.startASP("<%=m_LayoutPage%>", "action", "Delete", "layoutname", ViewName);
}

function writeView(ViewName, View) 
{
	CP_getHeaderWindow().CP_ASPStarter.startASP("<%=m_LayoutPage%>", "action", "Save", "layoutname", ViewName, "layout", View.toCookieString());
}

function onSelectView(ViewName) 
{
	if (ViewName != null && isAllLoaded())
	{
		CP_getHeaderWindow().CP_ASPStarter.startASP("<%=m_LayoutPage%>", "action", "Copy", "layoutname", ViewName, "targetlayoutname", "_tmp281260");
		var str = CP_getHeaderWindow().CP_ASPStarter.getASPOutput(); 
		if (str.indexOf("4CP__OK: ") == 0)
		{
			writeViewName(ViewName);
			location.reload();
		}
	}
}

function onDeleteViews(ViewNameList) 
{
	if (isAllLoaded() && ViewNameList != null && ViewNameList.length > 0)
	{
		var NameArray = ViewNameList.split(";");
		for (ii = 0; ii < NameArray.length; ii++)
			deleteView(NameArray[ii]);

		var sel = CP_getHeaderWindow().getSelection();
		CP_getHeaderWindow().initSelection(readViewList());
		CP_getHeaderWindow().setSelection(sel);
	}
}


function onSaveView(ViewName) 
{
	if (isAllLoaded())
	{
		if (m_View.isNavigatorVisible())
		{
			var nw = CP_getNavigatorWindow().document.body.clientWidth;
			var mw = document.body.clientWidth;
			var iWidth = (100 * nw) / (nw + mw);

			m_View.setNavigatorWidth(parseInt(iWidth+0.5));
		}

		writeView(ViewName, m_View);
		writeViewName(ViewName);

		CP_getHeaderWindow().initSelection(readViewList());
		CP_getHeaderWindow().setSelection(ViewName);
	}
}



function onEditLayout()
{
	if (isAllLoaded())
	{
		var res = window.showModalDialog("CP_ViewEditor.htm", getViewDialogParams(), "dialogWidth:450px;dialogHeight:600px;help:no");
		if ( res != null )
		{
			m_View.m_LabelPattern = res;
			writeView("_tmp281260", m_View);
			location.reload();
			CP_getHeaderWindow().addUnsavedSelection();
		}
	}
}

function getViewDialogParams()
{
	// initialize list of tool tips
	var URLList = new Array(m_View.m_LabelUrl.length);
	for (ii = 0; ii < URLList.length; ii++)
	{
		var index1 = m_View.m_LabelUrl[ii].indexOf("&ObjName=");
		var index2 = -1;
		if (index1 >= 0)
		{
			index1 = m_View.m_LabelUrl[ii].indexOf("=", index1) + 1;
			var index2 = m_View.m_LabelUrl[ii].indexOf("&", index1);
			if (index2 < 0)
				index2 = m_View.m_LabelUrl[ii].length;
		}

		if (index1 > 0 && index2 > index1)
			URLList[ii] = m_View.m_LabelUrl[ii].substring(index1, index2);
		else
			URLList[ii] = "Empty";			
	}

	// initialize layout matrice
	var viewMatrix = new CP_ViewMatrix();
	viewMatrix.m_Matrix = m_View.m_LabelPattern;

	// set dialog params
	var ViewDialogParams = new Array(2);
	ViewDialogParams[0] = viewMatrix.copy();
	ViewDialogParams[1] = URLList;
	return ViewDialogParams;
}
</script>
</HEAD>

<frameset rows="100%" onload="init()" onunload="exit()" onbeforeunload="beforeExit()">
<%=m_View.toHtmlString(m_SessionId)%>
</frameset>
</HTML>

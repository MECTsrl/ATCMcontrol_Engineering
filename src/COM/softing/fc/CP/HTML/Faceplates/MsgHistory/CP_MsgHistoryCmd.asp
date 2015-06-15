<%@ LANGUAGE = JScript %>
<% //ATCM. All rights reserved %>
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibFaceplate.asp" -->

<html>
<head>
<% 
GenerateStdHeader("Message History Toolbar");
GenerateStdIncludes();
%>  
<script language="JavaScript">
var m_StartID = -1;
var m_EndID = -1;
var m_MinID = -1;
var m_MaxID = -1;
var m_Filter = 0;

function goBegin()   { refreshList("Begin");  }
function goEnd()     { refreshList("End"); }
function goBack()    { refreshList("Back"); }
function goForward() { refreshList("Forward"); }
function goCustom()  
{ 
	m_StartID = IdMsgId.value;
	refreshList("Update");
}

function setFilter(Filter)
{
	m_Filter = Filter;
	refreshList("End")
}

function refreshList(Cmd)
{
	parent.m_BodyLoaded = false;
	parent.frames[0].navigate("/4C/CP_MsgHistoryBody.asp?SessionId=<%=__FpParams.getSessionId()%>" + 
		"&Cmd=" + Cmd +  "&StartID=" + m_StartID + "&EndID=" + m_EndID  + "&Filter=" + m_Filter);
	update();
}

function enableButtons(bEnable)
{
	IdBegin.disabled = !bEnable;
	IdEnd.disabled = !bEnable;
	IdBack.disabled = !bEnable;	
	IdForward.disabled = !bEnable;
	IdCustom.disabled = !bEnable;	
}

function init()
{
	m_Filter = 0;
	update();
}

function exit()
{
}

function update()
{
	enableButtons(parent.m_BodyLoaded);
	if (parent.m_BodyLoaded)
	{
		m_StartID = parent.frames[0].getStartID();
		m_EndID = parent.frames[0].getEndID();
		m_MinID = parent.frames[0].getMinID();
		m_MaxID = parent.frames[0].getMaxID();

		IdMsgId.value = m_StartID;
	}
	else
	{
		setTimeout("update()", 500);
	}
}

function checkID()
{
	var s = IdMsgId.value.toString();
	var ll = s.length;
	var i0, i1, ii;

	// check for leading blanks
	for (i0 = 0; i0 < ll && s.charAt(i0) == ' '; i0++);
	for (i1 = ll-1; i1 >= 0 && s.charAt(i1) == ' '; i1--);

	if ((i1 < i0) ||								// no digits at all
	    ((i1 > i0) && (s.charAt(ii) == '0')) ||		// leading '0' is not valid
	    ((i1 - i0) > 6))							// id greater than 9999999
	{
		IdCustom.disabled = true;	
		return;
	}

	// check for integer digits
	for (ii = i0; ii <= i1; ii++)
	{
		if (s.charAt(ii) < '0' || s.charAt(ii) > '9')
		{
			IdCustom.disabled = true;	
			return;
		}
	}
	IdCustom.disabled = false;	
}

</script>

</head>

<body  class=Faceplate onload="init()" onunload="exit()">
<NOBR>

<table class=Faceplate border=0>
<tr>
<td><input type=button id="IdBegin"   title="Load first page"     onclick="goBegin()"   value="<<" style="width:50;font-size:9pt"></input></td> 
<td><input type=button id="IdBack"    title="Load previous page"  onclick="goBack()"    value="<"  style="width:50;font-size:9pt"></input></td> 
<td><input type=button id="IdForward" title="Load next page"      onclick="goForward()" value=">"  style="width:50;font-size:9pt"></input></td> 
<td><input type=button id="IdEnd"     title="Load last page"      onclick="goEnd()"     value=">>" style="width:50;font-size:9pt"></input></td> 
<td>&nbsp;</td>
<td> <input type=button id="IdCustom"  title="Go to custom message" onclick="goCustom()"  value="Go to" style="width:50;font-size:9pt"></input></td> 
<td> <input type=text   id="IdMsgId"   title="Custom message ID"    onkeyup="checkID()"   value="0"     style="width:50;font-size:9pt"></input></td>
</tr>

</table>



</body>
</html>

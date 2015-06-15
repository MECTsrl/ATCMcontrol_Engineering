<% //ATCM. All rights reserved %>
<%
var m_EnableTextColor = "#000000";
var m_DisableTextColor = "#505050";

var m_MaxId = -1;
var m_InitFunction = new Array();
%>

<%
function GenerateButton(text, ImageEnable, ImageDisable, tipText, Type, bEnable, bPressed, action, Width)
{
	m_MaxId++;

	if (Width == null || Width == "")
		Width = "60";

	var m_TextColor = m_EnableTextColor;
	if (!bEnable)
		m_TextColor = m_DisableTextColor;

	var ButtonFrameClass = "";
	if (Type == "toggle")
	{
		if (bPressed)
			ButtonFrameClass = "NegativeLightGreyButton";
		else
			ButtonFrameClass = "FlatButton";
	}
	else if (Type == "toggle2")
	{
		if (bPressed)
			ButtonFrameClass = "NegativeButton";
		else
			ButtonFrameClass = "PositiveButton";
	}
	else if (Type == "normal")
	{
		ButtonFrameClass = "PositiveButton";
	}
	else
	{
		ButtonFrameClass = "FlatButton";
	}

	m_InitFunction[m_MaxId] = "new Button('" + ImageEnable + "','" + ImageDisable + "','"  + Type + "'," +  bEnable + "," +  bPressed + ",'" + m_MaxId + "'," + action + ")";

	var ButtonAObj = "ButtonA" + m_MaxId;
	var ButtonTableObj = "ButtonTable" + m_MaxId;
	var ButtonFrameObj = "ButtonFrame" + m_MaxId;
	var ButtonImageObj = "ButtonImage" + m_MaxId;
	
	var ButtonImageSrc = "";
	if (bEnable && ImageEnable != null && ImageEnable.length > 0)
		ButtonImageSrc = ImageEnable;
	else if (!bEnable && ImageDisable != null && ImageDisable.length > 0)
		ButtonImageSrc = ImageDisable;

	ButtonTextObj  = "ButtonText"  + m_MaxId;
%>
<!-- Generated Button Entry -->
<a class="HeaderObject" href="" id=<%=ButtonAObj%> buttonid="<%=m_MaxId%>" 
	onclick="cancelDefaultAction()"	
	onfocus="onButtonFocus(this)"
	onblur="onButtonBlur(this)"
	onmousedown="onButtonMouseDown(this)"
	onmouseup="onButtonMouseUp(this)"
<%	if (Type != "normal" && Type != "toggle2") 
{%>
	onmouseover="onButtonMouseOver(this)"
	onmouseout="onButtonMouseOut(this)"
<%}%>
	onkeydown="onButtonKeyDown(this)">
	<table id=<%=ButtonTableObj%> buttonid="<%=m_MaxId%>" border=0 width="<%=Width%>" cellpadding="0" cellspacing="0" style="padding-top:2"  onclick="onButtonClick(this)">
		<td id=<%=ButtonFrameObj%> buttonid="<%=m_MaxId%>" CLASS=<%=ButtonFrameClass%> width="100%" title="<%=tipText%>"
			onfocus="onButtonFocus(this)" 
			onblur="onButtonBlur(this)">
			<img id=<%=ButtonImageObj%> buttonid="<%=m_MaxId%>" src="<%=ButtonImageSrc%>">
			<DIV id=<%=ButtonTextObj%> style="color:<%=m_TextColor%>" buttonid="<%=m_MaxId%>"><%=text%></DIV>
		</td>
	</table>
</a>
<!-- End of generated Button Entry -->
<%
	return m_MaxId;
}
%>

<%
function GenerateButtonFunctions()
{%>
<!-- Generated Button functions -->
<script language="JavaScript">
var __buttons = new Array();

function Button_setText(text)
{
	var ButtonTextObj = document.all['ButtonText' + this.id];
	ButtonTextObj.innerText = text;
}

function Button_setImage(ImageEnable, ImageDisable)
{
	this.ImageEnable = ImageEnable;
	this.ImageDisable = ImageDisable;

	this.enable(this.bEnabled);
}

function Button_enable(bEnable)
{
	var ButtonImageObj = document.all['ButtonImage' + this.id];
	var ButtonTextObj = document.all['ButtonText' + this.id];

	this.bEnabled = bEnable;
	if (this.bEnabled)
	{
		ButtonTextObj.style.color = "<%=m_EnableTextColor%>";
		if (this.ImageEnable != null && ButtonImageObj.src != this.ImageEnable)
			ButtonImageObj.src = this.ImageEnable;
	}
	else
	{
		ButtonTextObj.style.color = "<%=m_DisableTextColor%>";
		if (this.ImageDisable != null && ButtonImageObj.src != this.ImageDisable)
			ButtonImageObj.src = this.ImageDisable;
	}
}

function Button_setPressed(bPressed)
{
	if (bPressed != this.bPressed)
	{
		var ButtonFrameObj = document.all['ButtonFrame' + this.id];
		if (this.Type == "toggle")
		{
			if (bPressed)
				ButtonFrameObj.className = "NegativeLightGreyButton";
			else
				ButtonFrameObj.className = "FlatButton";
		}
		else if (this.Type == "toggle2")
		{
			if (bPressed)
				ButtonFrameObj.className = "NegativeButton";
			else
				ButtonFrameObj.className = "PositiveButton";
		}
	}

	this.bPressed = bPressed;
}

function Button_isEnabled() { return this.bEnabled; }
function Button_isPressed() { return this.bPressed; }

function Button(ImageEnable, ImageDisable, Type, bEnable, bPressed, id, action)
{
	this.id = id;
	this.Type = Type;
	this.action = action;

	this.setText = Button_setText;

	this.setImage = Button_setImage;
	this.ImageEnable = ImageEnable;
	this.ImageDisable = ImageDisable;

	this.isEnabled  = Button_isEnabled;
	this.enable = Button_enable;
	this.bEnabled = bEnable;
	
	this.isPressed  = Button_isPressed;
	this.setPressed = Button_setPressed;
	this.bPressed = bPressed;

	__buttons[id] = this;
}

function cancelDefaultAction()
{
	window.event.cancelBubble = true;
	window.event.returnValue = false;
}

function setClass(classname, obj)
{ 
	var item = __buttons[obj.buttonid];
	if (item != null)
	{
		var ButtonFrameObj = document.all['ButtonFrame' + item.id];
		if (item.bEnabled && !item.bPressed)
			ButtonFrameObj.className = classname;
	}
}

function doAction(obj)
{
	var item = __buttons[obj.buttonid];
	if (item != null && item.bEnabled && item.action != null)
	{
		if ((typeof item.action) == 'function') 
			item.action();
	}
}

function setFocused(obj, bFocused)
{
	var item = __buttons[obj.buttonid];
	if (item != null)
	{
		var ButtonTextObj = document.all['ButtonText' + item.id];
		if (bFocused && item.bEnabled)
			ButtonTextObj.style.fontWeight = "bold";	
//			ButtonTextObj.style.textDecorationUnderline = true;
		else if (!bFocused)
			ButtonTextObj.style.fontWeight = "normal";	
//			ButtonTextObj.style.textDecorationUnderline = false;
	}
}

function onButtonFocus(obj)
{
	setFocused(obj, true);
}

function onButtonBlur(obj)
{
	setFocused(obj, false);
}	

function onButtonClick(obj)
{
	window.event.cancelBubble = true;
	doAction(obj);
}

function onButtonMouseDown(obj)
{
	window.event.cancelBubble = true;
	setClass('NegativeButton', obj);
}

function onButtonMouseUp(obj)
{
	window.event.cancelBubble = true;
	var item = __buttons[obj.buttonid];
	if (item != null)
	{
		if (item.Type == "normal")
			setClass('PositiveButton', obj);	
		else if (item.Type == "flat")
			setClass('FlatButton', obj);	
	}
}

function onButtonMouseOver(obj)
{
	window.event.cancelBubble = true;
	if (!isInsideButton())
		setClass('PositiveButton', obj);
}

function onButtonMouseOut(obj)
{
	window.event.cancelBubble = true;
	if (!isInsideButton())
		setClass('FlatButton', obj);
}

function onButtonKeyDown(obj)
{
	window.event.cancelBubble = true;
	if (isKeyCode(13)) 
	{ 
		setClass('NegativeButton', obj); 
		doAction(obj);
	}
}

function isInsideButton()
{
	var fromElement = window.event.fromElement;
	var toElement = window.event.toElement;

	// If just moving around within the button, then return
	// This improves performance and avoids a flicker
	if ((fromElement != null) && (toElement != null))
	{
		if (fromElement.buttonid == toElement.buttonid) 
			return true;
	}
	return false;
} 

function isKeyCode(key)
{
	var keyCode = window.event.keyCode;
	if (key != null && keyCode != null)
	{
		if (key != keyCode) 
			return false;
	}
	return true;	
}
</script>
<!-- End of generated Button functions -->
<%}%>



<% 
function GenerateFunctionInitAllButtons()
{%>
<!-- Generated function for button initialization-->
<script language="JavaScript">
function InitAllButtons()
{<%
	for (var id = 0; id <= m_MaxId; id++) {%>
	Button<%=id%> = <%=m_InitFunction[id]%>;<%
	}%>	
}
</script>
<!-- End of generated Button function -->
<%}%>


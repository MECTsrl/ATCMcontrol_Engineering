// ATCM. All rights reserved

var __CpCycleTime = 1000;
var __CpLastTime = 0;
var __CpIgnoreFirst = true;

function Init() 
{
	Init2();
}

function Init2() 
{
	if (isAllLoaded())
	{
		if (this.InitCommunication != null)
			InitCommunication();
		if (this.InitFaceplate != null)
			InitFaceplate();

		__CpLastTime = new Date().getTime();
		setTimeout("Update()", __CpCycleTime);

		if (this.keepAlive != null)
			setTimeout("keepAlive()", 10000);
	}
	else
	{
		setTimeout("Init2()", 500);
	}
}

function Exit() 
{
	if (this.ExitFaceplate != null)
		ExitFaceplate();
	if (this.ExitCommunication != null)
		ExitCommunication();
}

function Update()
{
	if (isAllLoaded())
	{
		if (this.UpdateCommunication != null)
			UpdateCommunication();

		if (this.UpdateFaceplate != null)
			UpdateFaceplate();
		
		var CurrTime = new Date().getTime();		
		var ActualCycleTime =  CurrTime - __CpLastTime;
		var dt = parseInt(__CpCycleTime / 2);

		if (ActualCycleTime > (__CpCycleTime + dt))
		{
			if (!__CpIgnoreFirst)
			{
				__CpCycleTime = 2 * ActualCycleTime ;
				__CpCycleTime = Math.min(10000, __CpCycleTime);
			}
			__CpIgnoreFirst = false;
		}
		else
		{
			if (__CpCycleTime > 1000) 
			{
				__CpCycleTime = __CpCycleTime / 2;
				__CpCycleTime = Math.max(1000, __CpCycleTime);
			}
			__CpIgnoreFirst = true;
		}
		__CpLastTime = CurrTime;
		setTimeout("Update()", __CpCycleTime);
	}
	else
	{
		setTimeout("Update()", 2000);
	}
}

// A-tag onClick event handler
function LoadNewFaceplate()
{
	if (window.event.srcElement.tagName == "A")
	{
		if (window.event.srcElement.href.length > 0)
			parent.navigate(window.event.srcElement.href); 
        window.event.returnValue = false;
	}
}

function setFaceplateText(text)
{
	parent.frames[0].setText(text);
}

function setFaceplateToolTip(tip)
{
	if (parent.frames[0].setToolTip != null)
		parent.frames[0].setToolTip(tip);
}

function errortrap(msg,url,line)
{
	writeErrorMessage(msg);
	return true;
}

function writeErrorMessage(msg)
{
	if (CP_isPanelContext())
	{
		if (isAllLoaded())
		{
			if (parent.frames[0].document.CpHeader != null)
				CP_getStatusWindow().setErrorText(parent.frames[0].document.CpHeader.getName(), msg);
			else
				CP_getStatusWindow().setErrorText(null, msg);
		}
	}
	else if (top.CP_ErrorTextDialog != null)
	{
		top.setErrorTextDialog(msg);
	}
}

function CP_VariantToIEC(type, value)
{
	return document.Baselib.VariantToString(value, type, "", 2, 0);
}

function CP_IECToVariant(type, value)
{
	if (value == null)
		return null;

	if (value.length > 0 && type.indexOf("REAL") > 0 && value.indexOf(".") < 0)
		return document.Baselib.StringToVariant(value + ".0", type, "", 2);

	return document.Baselib.StringToVariant(value, type, "", 2);
}

function CP_CheckIEC(type, value)
{
	if (value == null)
		return false;

	if (value.length > 0 && type.indexOf("REAL") > 0 && value.indexOf(".") < 0)
		return document.Baselib.CheckString(value + ".0", type, "", 2) == 0;

	return document.Baselib.CheckString(value + "", type, "", 2) == 0
}

function TranslateOPCQuality (OPCQuality)
{
	switch (parseInt(OPCQuality) & 0xC0)
	{
	case 0xC0 :
		return 0;
	case 0x40 :
		return 1;
	default :
		return 2;
	}
}

function StringToNative(str, type)
{
	switch (type)
	{
	case "STRING" :
	case "WSTRING" :
	case "QVT_WSTRING" :
	case "QVT_STRING" :
		return str;

	case "REAL" :
	case "LREAL" :
	case "QVT_REAL" :
	case "QVT_LREAL" :
		return parseFloat(str);
	
	case "BOOL" :
	case "QVT_BOOL" :
		if (str.charAt(0) == 'F' || str.charAt(0) == 'f' || str.charAt(0) == '0')
			return false
		else
			return true;

	default :
                var aInt;
                aInt = parseInt(str);
                if(isNaN(aInt))
                    aInt = 0;
		return aInt;
	}
}


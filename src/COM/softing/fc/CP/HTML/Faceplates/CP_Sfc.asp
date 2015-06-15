<%@ LANGUAGE = JScript %>
<% //ATCM. All rights reserved %>
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibFaceplate.asp" -->
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibButton.asp" -->
<% CheckFaceplateSecurity(); %>
<html>
<head>
<% 
GenerateStdHeader("SFC");
GenerateStdIncludes();
GenerateStdCommunication2(null, true, false);

// get name of graphic data file
var DataFile = __InstanceData.ValuesByKeyDictionary.item("4GRV");
if (DataFile == null || DataFile.length == 0)
{
	writeError(__FpParams.getClassName(), __FpParams.getObjName(), 
		"Missing graphic file entry in configuration file \"" + __FpParams.getConfFile() + "\"");
}
var bCanExecute = CP_canSessionExecute(__FpParams.getSessionId());
%>


<script language="JavaScript">

var m_event  = "";
var m_param1 = "";
var m_param2 = "";
var m_iQual = 2;

function InitFaceplate()
{
}


function UpdateFaceplate()
{
	m_iQual = (
		_CP_QUAL___sfcvis | 
		_CP_QUAL___manualmode | 
		_CP_QUAL___forcedtransitions |
		_CP_QUAL___onceforcedtransitions |
		_CP_QUAL___blockedtransitions |
		_CP_QUAL___forcedactions |
		_CP_QUAL___onceforcedactions |
		_CP_QUAL___blockedactions);

	var bIsAutomatic = 	!StringToNative(_CP_VAL___manualmode, "BOOL");

	if (m_iQual == 0)
	{
		if (bIsAutomatic)
			document.body.className = "FaceplateSfcAuto";	
		else
			document.body.className = "FaceplateSfcManual";	
	}

	fbd.update(
		m_iQual,
		bIsAutomatic,
		_CP_VAL___sfcvis,
		fbd.joinIdLists(_CP_VAL___forcedtransitions, _CP_VAL___onceforcedtransitions, "add"),
		_CP_VAL___blockedtransitions,
		fbd.joinIdLists(_CP_VAL___forcedactions, _CP_VAL___onceforcedactions, "add"),
		_CP_VAL___blockedactions);
}

function handleEvent(event, param1, param2)
{
	m_event  = event;
	m_param1 = param1;
	m_param2 = param2;
	handleEvent2();
}

function handleEvent2()
{
	if (m_iQual != 0)
	{
		setTimeout("handleEvent2()", 1000);
	}
	else
	{
		switch (m_event)
		{
		case "ERROR" :
			writeErrorMessage(m_param1);
			break;

		case "doNext" :
			WriteOPCValue(_CP_PATH + _CP_NAME___donext, true);
			break;
			
		case "manualMode" :
			if (m_param2 == "0")
				WriteOPCValue(_CP_PATH + _CP_NAME___manualmode, false);
			else
				WriteOPCValue(_CP_PATH + _CP_NAME___manualmode, true);
			break;
		case "forceTransitionAll" :
			WriteOPCValue(_CP_PATH + _CP_NAME___blockedtransitions, "");							
			WriteOPCValue(_CP_PATH + _CP_NAME___forcedtransitions, m_param1);
			break;
		case "blockTransitionAll" :
			WriteOPCValue(_CP_PATH + _CP_NAME___forcedtransitions, "");							
			WriteOPCValue(_CP_PATH + _CP_NAME___blockedtransitions, m_param1);
			break;
		case "revertTransitionAll" :
			WriteOPCValue(_CP_PATH + _CP_NAME___forcedtransitions, "");							
			WriteOPCValue(_CP_PATH + _CP_NAME___blockedtransitions, "");
			break;							
		case "blockActionAll" :
			WriteOPCValue(_CP_PATH + _CP_NAME___forcedactions, "");
			WriteOPCValue(_CP_PATH + _CP_NAME___blockedactions, m_param1);
			break;
		case "forceActionAll" :
			WriteOPCValue(_CP_PATH + _CP_NAME___blockedactions, "");
			WriteOPCValue(_CP_PATH + _CP_NAME___forcedactions, m_param1);
			break;
		case "revertActionAll" :
			WriteOPCValue(_CP_PATH + _CP_NAME___blockedactions, "");
			WriteOPCValue(_CP_PATH + _CP_NAME___forcedactions, "");
			break;																	
		case "forceTransitionOnce" :
			WriteOPCValue(_CP_PATH + _CP_NAME___mancmd, "s;" + m_param1);
			break;
		case "forceTransition" :
			WriteOPCValue(_CP_PATH + _CP_NAME___forcedtransitions, fbd.joinIdLists(_CP_VAL___forcedtransitions, m_param1, m_param2));
			break;
		case "blockTransition" :
			WriteOPCValue(_CP_PATH + _CP_NAME___blockedtransitions, fbd.joinIdLists(_CP_VAL___blockedtransitions, m_param1, m_param2));
			break;
		case "forceActionOnce" :
			WriteOPCValue(_CP_PATH + _CP_NAME___onceforcedactions, fbd.joinIdLists(_CP_VAL___onceforcedactions, m_param1, m_param2));
			break;
		case "forceAction" :
			WriteOPCValue(_CP_PATH + _CP_NAME___forcedactions, fbd.joinIdLists(_CP_VAL___forcedactions, m_param1, m_param2));
			break;
		case "blockAction" :
			WriteOPCValue(_CP_PATH + _CP_NAME___blockedactions, fbd.joinIdLists(_CP_VAL___blockedactions, m_param1, m_param2));
			break;
		default :
			alert("Unknown event: " + m_event + ": " + m_param1 + ", " + m_param2);
			break;
		}

		m_event  = "";
		m_param1 = "";
		m_param2 = "";
	}
}

</script>

</head>

<body <%=GenerateFaceplateBodyTag()%>>
<% 	GenerateProxyObj(); %>
<APPLET id=fbd 
	code=FbdApplet.class cabbase="/4C/cpclasses.cab#version=<%=GetVersionString()%>"
	OPERATE="<%=bCanExecute ? 1 : 0%>"
	data="<%=DataFile%>" 
	height=100% width=100% 
	handler="handleEvent" 
	MAYSCRIPT>
</APPLET>

</body>
</html>
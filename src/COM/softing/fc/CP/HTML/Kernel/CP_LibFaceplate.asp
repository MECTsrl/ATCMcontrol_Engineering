<% //ATCM. All rights reserved %>

<!-- #INCLUDE VIRTUAL = "/4C/CP_Inst2.inc" -->
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibAspParam.asp" -->
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibSecurity.asp" -->
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibHtml.asp" -->

<% 
__InstanceData = null;
%>


<%
function GenerateStdCommunication()
{
	return GenerateStdCommunication2(null, false)
}

function GenerateStdCommunication2(NameList, bNoNativeVariables, bSkipHiddenVariables)
{
%>
<script language="JavaScript">
<%
	__InstanceData = new InstanceData(__FpParams.getConfFile(), __FpParams.getClassName(), __FpParams.getObjName());
	var ObjPath  = __InstanceData.getValuesStringByKey("PATH") + "." + __FpParams.getObjName();

	var VarNameArrayAll;
	if (NameList != null && NameList.length > 0)
		VarNameArrayAll = NameList.split(";");
	else
		VarNameArrayAll = __InstanceData.getAllParamNamesArray();
	
	var VarNameArray = new Array();

	var VarNameNativeList = ""; 
	var VarNameFormatList = "";
// generate variable declaration
	var IoCount = 0;
	for (var VarIndex in VarNameArrayAll)
	{ 
		var Name =   VarNameArrayAll[VarIndex];
		var Type =   __InstanceData.getParamTypeByName(Name);
		var Rights = __InstanceData.getParamRwByName(Name);
		if (!bSkipHiddenVariables || Rights.indexOf('h') < 0) // skip hidden IOs
		{
			if (IoCount != 0)
			{
				VarNameNativeList += getVarlistSeparator();
				VarNameFormatList += getVarlistSeparator();
			}
			VarNameNativeList += Name;
			VarNameFormatList += Type + getVarlistSeparator() + Name;
			VarNameArray[IoCount] = Name;
%>	
var _CP_NAME_<%=Name%>="<%=Name%>", _CP_VAL_<%=Name%>,<%if (!bNoNativeVariables) {%> _CP_VAL_NATIVE_<%=Name%>,<%}%> _CP_QUAL_<%=Name%>=2, _CP_TYPE_<%=Name%>="<%=Type%>", _CP_RW_<%=Name%>="<%=Rights%>", _CP_IO_<%=Name%>="<%=__InstanceData.getParamIoByName(Name)%>";<%
			IoCount++;
		}
	}%>
var _CP_NAME_NATIVE_LIST = "<%=VarNameNativeList%>";
var _CP_NAME_FORMAT_LIST = "<%=VarNameFormatList%>";
var _CP_PATH="<%=ObjPath%>.";
</script>

<% 
	GenerateLoginFunction(1);
%>

<% 
// function for variable subscription 
%>
<script language="JavaScript">
function InitCommunication() 
{
<%
if (IoCount > 0) 
{%>
	SubscribeOPCList(_CP_NAME_NATIVE_LIST, _CP_PATH); 
<%}%>
<%
if (ObjPath.length > 0) 
{%>	
	setFaceplateToolTip("<%=ObjPath%>");
<%}%>
}	
<% 
// function for variable unsubscription 
%>
function ExitCommunication() 
{<%
if (IoCount > 0) {%>
	UnsubscribeOPCList(_CP_NAME_NATIVE_LIST, _CP_PATH); <%}%>
}	

<% 
// function for variable update 
%>
function UpdateCommunication()
{<%
if (IoCount > 0) 
{

GenerateLoginCall(ObjPath, 0);

if (!bNoNativeVariables) {%>
	var ValuesFormat = ReadOPCListTypedAndRaw(_CP_NAME_FORMAT_LIST, _CP_PATH).split("<%=getVarlistSeparator()%>");
<%}
else {%>
	var ValuesFormat = ReadOPCListTyped(_CP_NAME_FORMAT_LIST, _CP_PATH).split("<%=getVarlistSeparator()%>");
<%}%>
	var ComError = false;
<%if (!bNoNativeVariables) {%>
	if (ValuesFormat.length != <%=4 * IoCount%>)
<%}
else {%>
	if (ValuesFormat.length != <%=3 * IoCount%>)
<%}%>
	{
		writeErrorMessage("Proxy returned wrong number of values !");
		ComError = true;
	}
	if (ComError)
	{<%
for (var VarIndex in VarNameArray)
{ 
	var Name = VarNameArray[VarIndex]; %>
		_CP_QUAL_<%=Name%> = 2;<%
}%>	
	}
	else
	{<%
for (var VarIndex in VarNameArray)
{ 
	var Name = VarNameArray[VarIndex];	%>
	<%if (!bNoNativeVariables) {%>
		_CP_VAL_<%=Name%> = ValuesFormat[<%=4 * VarIndex + 1%>];
		_CP_VAL_NATIVE_<%=Name%> = StringToNative(ValuesFormat[<%=4 * VarIndex + 2%>], _CP_TYPE_<%=Name%>);
		_CP_QUAL_<%=Name%> = TranslateOPCQuality(ValuesFormat[<%=4 * VarIndex%>]);<%
	}
	else
	{%>
		_CP_VAL_<%=Name%> = ValuesFormat[<%=3 * VarIndex + 1%>];
		_CP_QUAL_<%=Name%> = TranslateOPCQuality(ValuesFormat[<%=3 * VarIndex%>]);<%
	}
}%>
	}
<%}%>
}
</script>
<%
	return VarNameArray;
}


//----------------------------------------------------------------------------------------
// Read-interface to configuration files.

	// Class to implement automation objects.
	function InstanceData(ConfigFileName, ClassName, InstanceName)
	{
		// 'private' properties
		this.ConfigFileName = ConfigFileName;
		this.ClassName = ClassName;
		this.InstanceName = InstanceName;
		this.Delimiter = ";";

		// The following dictionaries contain arrays als values.
		this.ValuesByKeyDictionary = new ActiveXObject("Scripting.Dictionary");
		this.ParamValuesByNameDictionary = new ActiveXObject("Scripting.Dictionary");

		// 'private' methods
		this.GetValuesArrayFromValuesString = GetValuesArrayFromValuesString;
		this.fillDictionaries = fillDictionaries;

        // 'public' methods
		this.getValuesStringByKey = getValuesStringByKey;
		this.getParamValuesArrayByName = getParamValuesArrayByName;
		this.getParamKeyByName = getParamKeyByName;
		this.getParamTypeByName = getParamTypeByName;
		this.getParamIoByName = getParamIoByName;
		this.getParamRwByName = getParamRwByName;
		this.getAllParamNamesArray = getAllParamNamesArray;

		// Call the initialiser.
        this.fillDictionaries(ConfigFileName);
	}

// Member functions of class InstanceData.

	function GetValuesArrayFromValuesString(ValuesString)
	{
		var SplitExp = "([^" + this.Delimiter + "]+)";
		// SplitExp contais the string "([^;]+)";
		var SplitExpObj = new RegExp(SplitExp,"g");
		var ValuesArray = ValuesString.match(SplitExpObj);
		return ValuesArray;
	}

	function fillDictionaries()
	{
		// Read config data via VBScript functions.
		// TODO: Replace VBScript functions to read the config file by JScript methods
		//       which read the config data and write it directly into the dictionaries
		//       and not via the dictionary ReadConfValues;

		// Read config file.
		var ReadConfigValues = new ActiveXObject("Scripting.Dictionary");
		if(!getConfValues(this.ConfigFileName, this.InstanceName, ReadConfigValues))
		{
			writeObjNotFound(this.ClassName, this.InstanceName)
		}
		var KeysVBArray = ReadConfigValues.Keys();
		var KeysArray = KeysVBArray.toArray();

		// Fill ValuesByKeyDictionary and ParamValuesByNameDictonary.
		for (var KeyIndex in KeysArray)
		{
			var Key = KeysArray[KeyIndex];
			var ValuesString = ReadConfigValues.item(Key);
			var ValuesArray = this.GetValuesArrayFromValuesString(ValuesString);
			this.ValuesByKeyDictionary.Add(Key, ValuesArray);
			var ParamExpObj = /^((IN)|(OUT)|(INOUT))/;
			if (Key.match(ParamExpObj))
			{
				if (ValuesArray.length != 3)
				{
					writeError(this.ClassName, this.ObjName, 
						"Wrong number of values for parameter with tag \"" +
						Key + "\" of this object in configuration file \"" + this.ConfigFileName + "\"");
				}
				var ParamName = ValuesArray[0];
				if (this.ParamValuesByNameDictionary.Exists(ParamName) == true)
				{
					writeError(this.ClassName, this.ObjName, 
						"Parameter name \"" + ParamName + 
						"\" not unique for this object in configuration file \"" + this.ConfigFileName + "\"");
				}
 				var ValuesArrayWithoutNameWithKey = ValuesArray.slice(0);
				// Replace name by key.
				ValuesArrayWithoutNameWithKey[0] = Key;
				this.ParamValuesByNameDictionary.Add(ParamName, ValuesArrayWithoutNameWithKey);
			}
		}
	}

	function getValuesStringByKey(Key)
	{
		var ValuesArray = this.ValuesByKeyDictionary.item(Key);
		if (ValuesArray == null)
		{
			writeError(this.ClassName, this.ObjName, 
				"Missing tag \"" + Key + "\" in this object in configuration file \"" + this.ConfigFileName + "\"");
		}
		return ValuesArray.join(this.Delimiter);
	}


	function getParamValuesArrayByName(ParamName)
	{
		var ValuesArray = this.ParamValuesByNameDictionary.item(ParamName);
		if (ValuesArray == null)
		{
			writeError(this.ClassName, this.ObjName, 
				"Missing parameter name \"" + ParamName + "\" in this object in configuration file \"" + this.ConfigFileName + "\"");
		}
		return ValuesArray;
	}

	function getParamKeyByName(ParamName)
	{
		var ValuesArray = this.getParamValuesArrayByName(ParamName);
		return ValuesArray[0];
	}

	function getParamIoByName(ParamName)
	{
		var key = this.getParamKeyByName(ParamName);
		if (key.indexOf("INOUT", 0) == 0)
			return "INOUT";
		else if (key.indexOf("OUT", 0) == 0)
			return "OUT";
		else if (key.indexOf("IN", 0) == 0)
			return "IN";
		else
			return "";
	}

	function getParamTypeByName(ParamName)
	{
		var ValuesArray = this.getParamValuesArrayByName(ParamName);
		return ValuesArray[1];
	}

	function getParamRwByName(ParamName)
	{
		if (CP_canSessionWrite(__FpParams.getSessionId()))
		{
			var ValuesArray = this.getParamValuesArrayByName(ParamName);
			return ValuesArray[2];
		}
		else
		{
			return "r"
		}
	}

	function getAllParamNamesArray()
	{
		var ParamNamesVBArray = this.ParamValuesByNameDictionary.Keys();
		return ParamNamesVBArray.toArray();
	}
%>

<%
function GenerateNavigateIFrameFunction()
{
%>
<script language="JavaScript">
function NavigateIFrame(theFrame, thePath, withCaption)
{
	var navigator=CP_getNavigator();
	if (navigator == null)
		return;
	if ( ! navigator.IsObjectListLoaded() )
	{
		return;
	}
	else
	{
		var theUrl;
		if (withCaption==1)
			theUrl=navigator.GetLoaderUrl(thePath);
		else
			theUrl=navigator.GetObjectUrl(thePath);
		
		if (theUrl=="")
			return;
		var zp="&SessionId=<%=__FpParams.getSessionId()%>&StandAlone=<%=__FpParams.isStandAlone()%>";
		theUrl=theUrl+zp;
		document.all(theFrame).navigate(theUrl);
	}
}
</script>
<%
}%>
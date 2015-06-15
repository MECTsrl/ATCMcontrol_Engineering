<%
// ATCM. All rights reserved
%>

<%
function getVarlistSeparator() {  return "|"; }
%>


<%
// Functions for extracting parameters passed to the ASP page
%>

<%
// Constructor

function FpAspParams()
{
	// "private" methods
	this.readParam = readParam;

	// "private" properties
	this.ClassName = this.readParam("Class");
	this.ObjName = this.readParam("ObjName");
	this.ConfFile = this.readParam("ConfFile");
	this.SessionId = this.readParam("SessionId");

	if (this.readParam("StandAlone") == 1)
		this.StandAlone = 1;
	else
		this.StandAlone = 0;

	// "public" methods
	this.getClassName = getClassName;
	this.getObjName = getObjName;
	this.getConfFile = getConfFile;
	this.isStandAlone = isStandAlone;
	this.getSessionId = getSessionId;
	this.getFaceplateParamString = getFaceplateParamString;

}


// The function readParam can be used directly (without an instance of class FpAspParams).
function readParam(Name)
{  
	var Param = Request.QueryString(Name);
	var ConcatString = "X";
	var TestString = ConcatString + Param;
	if ((TestString != ConcatString) && (TestString != ConcatString + "undefined"))
	{
		return "" + Param;
	}
	else
	{
		// Param is empty or undefined i.e., it expands to "" or to "undefined".
		return null;
	}
}

function readFormParam(Name)
{  
	var Param = Request.Form(Name);
	var ConcatString = "X";
	var TestString = ConcatString + Param;
	if ((TestString != ConcatString) && (TestString != ConcatString + "undefined"))
	{
		return "" + Param;
	}
	else
	{
		// Param is empty or undefined i.e., it expands to "" or to "undefined".
		return null;
	}
}

function getFaceplateParamString()
{  
	Separator = "";
	var Params = "";
	if (this.ClassName != null)
	{
		Params += Separator + "ClassName=" + this.ClassName;
		Separator = "&";
	}
	if (this.ObjName != null)
	{
		Params += Separator + "ObjName=" + this.ObjName;
		Separator = "&";
	}
	if (this.ConfFile != null)
	{
		Params += Separator + "ConfFile=" + this.ConfFile;
		Separator = "&";
	}
	if (this.StandAlone == 1)
	{
		Params += Separator + "StandAlone=1";
		Separator = "&";
	}

	return Params;
}


function getClassName() { return this.ClassName; }
function getObjName() 	{ return this.ObjName; }
function getConfFile() 	{ return this.ConfFile; }
function isStandAlone() { return this.StandAlone; }
function getSessionId() { return this.SessionId; }
%>

<%
var	__FpParams = new FpAspParams();
%>

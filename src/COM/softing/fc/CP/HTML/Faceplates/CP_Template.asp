<%@ LANGUAGE = JScript %>
<% //ATCM. All rights reserved %>
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibFaceplate.asp" -->
<% 
// The faceplate security check. This function sends the login page to the client
// if the security check fails
CheckFaceplateSecurity(); 
%>	
<html>
<head>
<% 
GenerateStdHeader("");  // standard html header
GenerateStdIncludes();  // include files for client scrips, style sheets, ...

// Code generation for all io variables of a 4C object
// For every io variable of a 4C object the following script variables are generated:
//   _CP_NAME_<name>       : name of the variable
//   _CP_VAL_<name>        : value of the variable as formatted string
//   _CP_VAL_NATIVE_<name> : native value of the variable
//   _CP_QUAL_<name>       : value quality of the variable (0/1/2 : good/uncertain/bad)
//   _CP_TYPE_<name>       : IEC 1131 data types, e.g. "DINT"
//   _CP_RW_<name>         : rights, "r" or "rw"
//   _CP_IO_<name>         : "IN", "OUT", or "INOUT"
// The scope of script variables is global to this page. The value variables are
// updated automatically in every update cycle.
GenerateStdCommunication();		
%>

<script language="JavaScript">
// this function is called after the faceplate has been loaded and 
// the communication has been initialized
function InitFaceplate()
{
	// To do: enter code for faceplate specific initialisation
}

// this function is called every update cycle
function UpdateFaceplate()
{
	// To do: enter code for updating your html objects 
}

// this function is called before the faceplate is going to be unloaded
function ExitFaceplate()
{
	// To do: enter code for some sepecial clean up (in most cases there is nothing to do) 
}
</script>
</head>

<body <%=GenerateFaceplateBodyTag()%>>		<%// faceplate body styles and init function%>
<% 	GenerateProxyObj();						// communication client%>

To do: Enter HTML Code here <P>

</body>
</html>
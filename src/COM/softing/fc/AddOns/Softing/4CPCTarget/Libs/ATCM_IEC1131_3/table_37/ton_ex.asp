<%@ LANGUAGE = JScript %>
<% //ATCM. All rights reserved %>
<!-- #INCLUDE VIRTUAL = "/4C/CP_LibFaceplate.asp" -->
<% CheckFaceplateSecurity(); %>
<html>
<head>
<% 
GenerateStdHeader("");
GenerateStdIncludes();
GenerateStdCommunication();
%>
<%GenerateStdOutEventHandler("CPStdOut")%>
<script language="JavaScript">
function InitFaceplate()
{
	CPStdOut.AddLine("Delay"               , "TIME"     , "r"      , 0, 4);
	CPStdOut.AddLine(_CP_PATH + _CP_NAME_Q , _CP_TYPE_Q , _CP_RW_Q , 1, 4);
	CPStdOut.AddLine(_CP_PATH + _CP_NAME_IN, _CP_TYPE_IN, _CP_RW_IN, 0, 4);
	CPStdOut.AddLine(_CP_PATH + _CP_NAME_PT, _CP_TYPE_PT, _CP_RW_PT, 0, 4);
	<%GenerateStdOutInitialization("CPStdOut");%>
}

function UpdateFaceplate()
{
	CPStdOut.SetValue(1, 0, 0, _CP_VAL_Q , _CP_QUAL_Q);
	CPStdOut.SetValue(2, 0, 0, _CP_VAL_IN, _CP_QUAL_IN);
	CPStdOut.SetValue(3, 0, 0, _CP_VAL_PT, _CP_QUAL_PT);

	// take the worst quality of both values
	var qRD = Math.max(_CP_QUAL_PT, _CP_QUAL_ET);
	
	var valRD = 0;
	if (qRD < 2)
		valRD = parseInt(_CP_VAL_NATIVE_PT) - parseInt(_CP_VAL_NATIVE_ET);

	// set quality to "BAD" if range is not valid
	if (qRD < 2 && valRD < 0)
		qRD = 2;

	// "GOOD or "UNCERTAIN" quality
	if (qRD < 2)
	{
		CPStdOut.SetValue(0, 0, 0, CP_VariantToIEC("TIME", valRD), qRD);
		IdBg.setQuality(_CP_QUAL_PT);
		if (_CP_QUAL_PT < 2)
		{
			IdBg.setMin(_CP_VAL_NATIVE_SCALEMIN);
			IdBg.setMax(_CP_VAL_NATIVE_SCALEMAX);
		}
		IdBg.setQuality(1, qRD);
		IdBg.setValue(1, valRD);
	}
	// "BAD" quality
	else
	{
		CPStdOut.SetValue(0, 0, 0, valRD, qRD)
		IdBg.setQuality(2);	
	}
}
</script>

<title></title>
</head>

<body <%=GenerateFaceplateBodyTag()%>>
<% 	GenerateProxyObj(); %>
<% 	GenerateBaseLibObj(); %>

<table border="0" width="180" height="70" cellpadding="0" cellspacing="0">
  <tr>
    <td>
    <applet id="IdBg" width="100%" height="100%" code="CP_ApBarGrafH.class"
    cabbase="/4C/cpclasses.cab">
      <param name="Background" value="<%=getBgColorFacplate()%>">
    </applet>
    </td>
  </tr>
</table>

<hr>
<%GenerateStdOutControl("CPStdOut", 4);%>

<hr>
<% GenerateLinkToTuningDisplay(); %>
</body>
</html>

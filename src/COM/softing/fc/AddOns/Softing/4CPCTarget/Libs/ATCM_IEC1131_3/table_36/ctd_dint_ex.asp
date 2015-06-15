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
	CPStdOut.AddLine( _CP_PATH + _CP_NAME_CV, _CP_TYPE_CV, _CP_RW_CV, 0, 4);
	CPStdOut.AddLine( _CP_PATH + _CP_NAME_PV, _CP_TYPE_PV, _CP_RW_PV, 0, 4);
	<%GenerateStdOutInitialization("CPStdOut");%>
}

function UpdateFaceplate()
{
	CPStdOut.SetValue(0, 0, 0, _CP_VAL_CV, _CP_QUAL_CV);
	CPStdOut.SetValue(1, 0, 0, _CP_VAL_PV, _CP_QUAL_PV);

	IdLedCD.setValue(_CP_VAL_NATIVE_CD, _CP_QUAL_CD);

	// quality of scale has to be set first
	IdBg.setQuality(_CP_QUAL_SCALEMIN | _CP_QUAL_SCALEMAX);

	IdBg.setMin(_CP_VAL_NATIVE_SCALEMIN);
	IdBg.setMax(_CP_VAL_NATIVE_SCALEMAX);
	

	IdBg.setQuality(1,_CP_QUAL_CV);
	IdBg.setValue(1, _CP_VAL_NATIVE_CV);
	
	IdBg.setQualityLimit(1,0);
	IdBg.setValueLimit(1, 0);

	IdBg.setQualityLimit(3,_CP_QUAL_PV);
	IdBg.setValueLimit(3, _CP_VAL_NATIVE_PV);
}
</script>

<title></title>
</head>

<body <%=GenerateFaceplateBodyTag()%>>
<% 	GenerateProxyObj(); %>

<table border="0" width="180" height="200" cellpadding="0" cellspacing="0">
  <tr>
    <td width="16" height="100%"><table border="0" width="100%" height="100%" cellpadding="0"
    cellspacing="0">
      <tr>
        <td height="14"></td>
      </tr>
      <tr>
        <td></td>
      </tr>
      <tr>
        <td height="20">
        <applet code="CPApLed.class" cabbase="/4C/cpclasses.cab" name="IdLedCD" width="13"
        height="13">
          <param name="Type" value="ArrowSouthSunken">
          <param name="BgColor" value="<%=getBgColorFacplate()%>">
          <param name="ColorId" value="3">
        </applet>
        </td>
      </tr>
      <tr>
        <td height="15"></td>
      </tr>
    </table>
    </td>
    <td>
    <applet id="IdBg" height="100%" code="CP_ApBarGrafV.class" cabbase="/4C/cpclasses.cab"
    width="128">
      <param name="yOrigin" value="0.0">
      <param name="yMin" value="0.0">
      <param name="yMax" value="1000.0">
      <param name="limits" value="true">
      <param name="Background" value="<%=getBgColorFacplate()%>">
    </applet>
    </td>
  </tr>
</table>

<hr>
<%GenerateStdOutControl("CPStdOut", 2);%>

<hr>
<%GenerateLinkToTuningDisplay();%>
</body>
</html>

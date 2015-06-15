<%
// ATCM. All rights reserved

	var m_Title = "";
	var m_MaxPathLen = 40;

	var m_Path = readParam("path");
	var m_Type = readParam("type");

	formatTitle();

	function getPath() 	{ return(m_Path + ""); 	}
	function getTitle() { return m_Title; }
	function getType() 	{ return m_Type; }

	function formatTitle()
	{
		var sPath =	getPath();

		if(sPath != "" && sPath != null)
		{
			if (sPath.length > m_MaxPathLen)
			{
				var startIndex = sPath.length - m_MaxPathLen;
				var pointIndex = sPath.indexOf('.',startIndex);
				if (pointIndex >= 0)
					sPath = sPath.substr(pointIndex + 1); 
				else
				{
					pointIndex = sPath.lastIndexOf('.');
					if (pointIndex >= 0)
						sPath = sPath.substr(pointIndex + 1);
				}
			}
		}
		else
			sPath = "???";

		m_Title = "Write: " + sPath + " (" + getType() + ")"; 
	}

%>
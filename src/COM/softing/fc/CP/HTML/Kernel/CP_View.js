// ATCM. All rights reserved
function CP_View(SzX, SzY)
{
	this.m_Version = "1300";
	this.m_NavigatorWidth = 20;
	this.m_NavigatorVisible = 1;
	this.m_MaxSizeX = 4;
	this.m_MaxSizeY = 4;
	this.m_SzX = parseInt(SzX);
	this.m_SzY = parseInt(SzY);
	if (this.m_SzX < 1 || this.m_SzY < 1)
	{
		this.m_SzX = this.m_MaxSizeX;
		this.m_SzY = this.m_MaxSizeY;
	}
	else
	{
		this.m_SzX = Math.min(this.m_SzX, this.m_MaxSizeX); 
		this.m_SzY = Math.min(this.m_SzY, this.m_MaxSizeY); 
	}

	this.m_LabelPattern = new Array(this.m_SzX * this.m_SzY);
	this.m_LabelUrl = new Array(this.m_MaxSizeX * this.m_MaxSizeY);

	for (ii = 0; ii < this.m_LabelUrl.length; ii++)
		this.m_LabelUrl[ii] = "/4C/CP_Loader.asp?AspFile=/4C/CP_Empty.asp&ObjName=Empty";	

	this.getNavigatorWidth	= CPView_getNavigatorWidth;
	this.setNavigatorWidth	= CPView_setNavigatorWidth;

	this.isNavigatorVisible	 = CPView_isNavigatorVisible;
	this.setNavigatorVisible = CPView_setNavigatorVisible;

	this.getSizeX			= CPView_getSizeX;
	this.getSizeY			= CPView_getSizeY;
	this.setPage			= CPView_setPage;
	this.getPage			= CPView_getPage;
	this.setTileLabel		= CPView_setTileLabel;
	this.getTileLabel		= CPView_getTileLabel;
	this.init				= CPView_init;

	this.isElement			= CPView_isElement;
	this.getNewLabel		= CPView_getNewLabel;
	this.existLabel			= CPView_existLabel;
	this.getLabelPos		= CPView_getLabelPos;

	this.canMerge			= CPView_canMerge;
	this.merge				= CPView_merge;
	this.mergeRight			= CPView_mergeRight;
	this.mergeLeft			= CPView_mergeLeft;
	this.mergeDown			= CPView_mergeDown;
	this.mergeUp			= CPView_mergeUp;
		
	this.canSplit			= CPView_canSplit;
	this.split				= CPView_split;
	this.splitVertical		= CPView_splitVertical;
	this.splitHorizontal	= CPView_splitHorizontal;

	this.fromCookieString	= CPView_fromCookieString;
	this.toCookieString		= CPView_toCookieString;
	this.toHtmlString		= CPView_toHtmlString;

	// for internal use only
	this.toHtmlStringRecursive	= CPView_toHtmlStringRecursive;
	this.splitSubViewVert		= CPView_splitSubViewVert;
	this.splitSubViewHorz		= CPView_splitSubViewHorz;
	this.relabelTwins			= CPView_relabelTwins;
}

function CPView_FrameNameToLabel(Name)  { return Name.substr(1); }
function CPView_LabelToFrameName(Label) { return "F" + Label; }

function CPView_setNavigatorWidth(iWidth) 
{ 
	this.m_NavigatorWidth = Math.min(100, iWidth); 
	this.m_NavigatorWidth = Math.max(0, this.m_NavigatorWidth); 
}
function CPView_getNavigatorWidth() { return this.m_NavigatorWidth; }

function CPView_isNavigatorVisible() { return this.m_NavigatorVisible != 0; }
function CPView_setNavigatorVisible(bVisible) { this.m_NavigatorVisible = (bVisible != 0 ? 1 : 0); }

function CPView_RemoveSessionId(Url)
{
	// Remove SessionId from URL
	var Url2 = Url.split(/SessionId=[^&]*&?/).join("");
	// Remove a tangling '&'. (If 'SessionId' was the last parameter.)
	return Url2.split(/&$/).join("");
}


function CPView_setPage(Label, Url)
{
	var Url2 = CPView_RemoveSessionId(Url);

	iLabel = parseInt(Label);
	if (this.existLabel(iLabel))
		this.m_LabelUrl[iLabel] = Url2;
}

function CPView_getPage(Label)
{
	iLabel = parseInt(Label);
	if (iLabel >= 0 || iLabel < this.m_LabelUrl.length)
		return this.m_LabelUrl[iLabel];
	else
		return null;
}

function CPView_setTileLabel(x, y, Label)
{
	iLabel = parseInt(Label);
	if (iLabel >= 0 || iLabel < this.m_LabelUrl.length)
	{
		if (x >= 0 && x < this.m_SzX && 
			y >= 0 && y < this.m_SzY)
		{
			this.m_LabelPattern[y * this.m_SzX + x] = iLabel;
		}
	}
}

function CPView_getTileLabel(x, y)
{
	if (x >= 0 && x < this.m_SzX && y >= 0 && y < this.m_SzY)
	{
		return this.m_LabelPattern[y * this.m_SzX + x];
	}
	return null;
}


function CPView_init(LabelArray)
{
	if (LabelArray.length >= this.m_LabelPattern.length)
	{
		for (ii = 0; ii < this.m_LabelPattern.length; ii++)
			this.m_LabelPattern[ii] = parseInt(LabelArray[ii]);
	}
}

function CPView_getNewLabel(Url)
{
	var iLabel;
	for (iLabel = 0; iLabel < this.m_LabelUrl.length; iLabel++)
	{
		if (!this.existLabel(iLabel))
			break;
	}

	if (Url == null || Url == "")
		this.setPage(iLabel, "/4C/CP_Loader.asp?AspFile=/4C/CP_Empty.asp&ObjName=Empty")
	else
		this.setPage(iLabel, Url);		

	return iLabel;
}

function CPView_existLabel(Label)
{
	var iLabel = parseInt(Label);
	for (jj = 0; jj < this.m_LabelPattern.length; jj++)
	{
		if (this.m_LabelPattern[jj] == iLabel)
			return 1;
	}
	return 0;
}

function CPView_getSizeX() { return this.m_SzX; }
function CPView_getSizeY() { return this.m_SzY; }

function CPView_isElement()
{
	var ii;
	for (ii = 1; ii < this.m_LabelPattern.length; ii++)
	{
		if (this.m_LabelPattern[ii] != this.m_LabelPattern[0])
			return 0;
	}
	return 1;
}

function CPView_isInsideRect(xx, yy, rect)
{
	if (xx >= rect[0] && xx <= rect[2] && yy >= rect[1] && yy <= rect[3])
		return 1;
	else
		return 0;
}

function CPView_relabelTwins()
{
	var iLabel;
	for (iLabel = 0; iLabel < this.m_LabelUrl.length; iLabel++)
	{
		var XY = this.getLabelPos(iLabel);
		if (XY[0] >= 0)
		{
			var yy, xx;
			for (yy = XY[1]; yy < this.m_SzY; yy++)
			{
				for (xx = 0; xx < this.m_SzX; xx++)
				{
					if (!CPView_isInsideRect(xx, yy, XY) && iLabel == this.getTileLabel(xx, yy))
					{
						var yy2, xx2;
						var NewLabel = this.getNewLabel(this.getPage(iLabel));
						for (yy2 = XY[1]; yy2 <= XY[3]; yy2++)
						{
							for (xx2 = XY[0]; xx2 <= XY[2]; xx2++)
							{
								this.setTileLabel(xx2, yy2, NewLabel)
							}
						}
						return;
					}
				}
			}
		}		
	}
}

function CPView_getLabelPos(Label)
{
	var XY = new Array(-1, -1, -1, -1);
	iLabel = parseInt(Label);
	var ii, jj;
	for (ii = 0; ii < this.m_LabelPattern.length; ii++)
	{
		if (this.m_LabelPattern[ii] == iLabel)
		{
			XY[0] = parseInt(ii % this.m_SzX);
			XY[1] = parseInt(ii / this.m_SzX);
			jj = XY[0];
			while (jj < this.m_SzX && this.getTileLabel(jj, XY[1]) == iLabel)
				jj++;

			XY[2] = jj - 1;

			jj = XY[1];
			while (jj < this.m_SzY && this.getTileLabel(XY[0], jj) == iLabel)
				jj++;

			XY[3] = jj - 1;
			return XY;
		}
	}
	return XY;
}

function CPView_mergeLeft(Label)
{
	var iLabel = parseInt(Label);

	// position of label rectangle (xLeft, yTop, xRight, yBottom)
	var XY = this.getLabelPos(iLabel);
	if (XY[0] < 1)
		return;
	
	// fill with label until the common left border of all neightbors is reached
	var bContinue = 1;
	var yy, xx;
	for (xx = XY[0] - 1; xx >= 0 && bContinue == 1; xx--)
	{
		for (yy = XY[1]; yy <= XY[3]; yy++)
		{
			if (xx <= 0 || this.getTileLabel(xx, yy) != this.getTileLabel(xx-1, yy))
				bContinue = 0;
			this.setTileLabel(xx, yy, iLabel);
		}
	}
	this.relabelTwins();
}

function CPView_mergeRight(Label)
{
	var iLabel = parseInt(Label);

	// position of label rectangle (xLeft, yTop, xRight, yBottom)
	var XY = this.getLabelPos(iLabel);
	if (XY[0] < 0 || XY[2] >= (this.m_SzX-1))
		return;
			
	// fill with label until the common right border of all neightbors is reached
	var bContinue = 1;
	var yy, xx;
	for (xx = XY[2] + 1; xx < this.m_SzX && bContinue == 1; xx++)
	{
		for (yy = XY[1]; yy <= XY[3]; yy++)
		{
			if (xx >= (this.m_SzX - 1) || this.getTileLabel(xx, yy) != this.getTileLabel(xx+1, yy))
				bContinue = 0;
			this.setTileLabel(xx, yy, iLabel);
		}
	}
	this.relabelTwins();
}


function CPView_mergeUp(Label)
{
	var iLabel = parseInt(Label);

	// position of label rectangle (xLeft, yTop, xRight, yBottom)
	var XY = this.getLabelPos(iLabel);
	if (XY[1] < 1)
		return;
	
	// fill with label until the common left border of all neightbors is reached
	var bContinue = 1;
	var yy, xx;
	for (yy = XY[1] - 1; yy >= 0 && bContinue == 1; yy--)
	{
		for (xx = XY[0]; xx <= XY[2]; xx++)
		{
			if (yy <= 0 || this.getTileLabel(xx, yy) != this.getTileLabel(xx, yy-1))
				bContinue = 0;
			this.setTileLabel(xx, yy, iLabel);
		}
	}
	this.relabelTwins();
}

function CPView_mergeDown(Label)
{
	var iLabel = parseInt(Label);

	// position of label rectangle (xLeft, yTop, xRight, yBottom)
	var XY = this.getLabelPos(iLabel);
	if (XY[1] < 0 || XY[3] >= (this.m_SzY-1))
		return;
	
	// fill with label until the common left border of all neightbors is reached
	var bContinue = 1;
	var yy, xx;
	for (yy = XY[3] + 1; yy < this.m_SzY && bContinue == 1; yy++)
	{
		for (xx = XY[0]; xx <= XY[2]; xx++)
		{
			if (yy >= (this.m_SzY - 1) || this.getTileLabel(xx, yy) != this.getTileLabel(xx, yy+1))
				bContinue = 0;
			this.setTileLabel(xx, yy, iLabel);
		}
	}
	this.relabelTwins();
}

function CPView_merge(Label, Direction)
{
	var iDirection = parseInt(Direction);
	switch (iDirection)
	{
		case 0:
		this.mergeLeft(Label);
		break;

		case 1:
		this.mergeUp(Label);
		break;

		case 2:
		this.mergeRight(Label);
		break;

		case 3:
		this.mergeDown(Label);
		break;

		default :
		break;
	}
}

function CPView_canMerge(Label, Direction)
{
	// position of label rectgle (xLeft, yTop, xRight, yBottom)
	var XY = this.getLabelPos(Label);

	var iDirection = parseInt(Direction);
	switch (iDirection)
	{
		case 0:
		if (XY[0] < 1)
			return false;
		break;

		case 1:
		if (XY[1] < 1)
			return false;
		break;

		case 2:
		if (XY[0] < 0 || XY[2] >= this.m_SzX-1)
			return false;
		break;

		case 3:
		if (XY[1] < 0 || XY[3] >= this.m_SzY-1)
			return false;
		break;

		default :
		return false;
		break;
	}

	var tmpView = new CP_View(this.m_SzX, this.m_SzY);
	for (var ii = 0; ii < this.m_LabelPattern.length; ii++)
		tmpView.m_LabelPattern[ii] = this.m_LabelPattern[ii];

	tmpView.merge(Label, iDirection);

	var viewMatrix = new CP_ViewMatrix();
	viewMatrix.m_Matrix = tmpView.m_LabelPattern;
	return viewMatrix.check();
}


function CPView_splitVertical(Label, bKeepLeft)
{
	// position of label rectangle (xLeft, yTop, xRight, yBottom)
	var XY = this.getLabelPos(Label);

	// position of new label rectangle
	var size = XY[2] - XY[0] + 1;
	if (bKeepLeft)
		XY[0] = XY[2] - parseInt(size / 2 - 1);
	else
		XY[2] = XY[0] + parseInt(size / 2 - 1);

	// set new label to all tiles inside rectangle
	var NewLabel = this.getNewLabel("");
	var yy, xx;
	for (yy = XY[1]; yy <= XY[3]; yy++)
	{
		for (xx = XY[0]; xx <= XY[2]; xx++)
			this.setTileLabel(xx, yy, NewLabel);
	}
}

function CPView_splitHorizontal(Label, bKeepTop)
{
	// position of label rectangle (xLeft, yTop, xRight, yBottom)
	var XY = this.getLabelPos(Label);
	
	// position of new label rectangle
	var size = XY[3] - XY[1] + 1;
	if (bKeepTop)
		XY[1] = XY[3] - parseInt(size / 2 - 1);
	else
		XY[3] = XY[1] + parseInt(size / 2 - 1);

	// set new label to all tiles inside rectangle
	var NewLabel = this.getNewLabel("");
	var yy, xx;
	for (yy = XY[1]; yy <= XY[3]; yy++)
	{
		for (xx = XY[0]; xx <= XY[2]; xx++)
			this.setTileLabel(xx, yy, NewLabel);
	}
}

function CPView_split(Label, Direction)
{
	var iDirection = parseInt(Direction);
	switch (iDirection)
	{
		case 0:
		this.splitVertical(Label, false);
		break;

		case 1:
		this.splitHorizontal(Label, false);
		break;

		case 2:
		this.splitVertical(Label, true);
		break;

		case 3:
		this.splitHorizontal(Label, true);
		break;

		default :
		break;
	}
}

function CPView_canSplit(Label, Direction)
{
	// position of label rectgle (xLeft, yTop, xRight, yBottom)
	var XY = this.getLabelPos(Label);

	var iDirection = parseInt(Direction);
	switch (iDirection)
	{
		case 0:
		case 2:
		if (XY[0] < 0 || (XY[2] - XY[0]) < 1)
			return false;
		break;

		case 1:
		case 3:
		if (XY[0] < 0 || (XY[3] - XY[1]) < 1)
			return false;
		break;

		default :
		return false;
		break;
	}
	return true;
}


function CPView_splitSubViewVert(StartColumn)
{
	if (StartColumn < 0 || StartColumn >= this.m_SzX)
		return new CP_View(0, 0);

	var delta = 0;
	for (xx = StartColumn; xx < this.m_SzX-1 && delta == 0; xx++)
	{
		var count = 0;
		for (yy = 0; yy < this.m_SzY; yy++)
 		{
			if (this.getTileLabel(xx, yy) != this.getTileLabel(xx+1, yy))
				count++;
		}	
		if (count == this.m_SzY)
			delta = xx - StartColumn + 1;
	}
	if (delta == 0)
		delta = this.m_SzX - StartColumn;

	// create new subview
	var tf = new CP_View(delta, this.m_SzY);
	var yy, xx;
	for (xx = 0; xx < delta; xx++)
	{
		for (yy = 0; yy < this.m_SzY; yy++)
			tf.setTileLabel(xx, yy, this.getTileLabel(xx + StartColumn, yy));
	}
	for (ii = 0; ii < tf.m_LabelUrl.length; ii++)
		tf.m_LabelUrl[ii] = this.m_LabelUrl[ii];
	return tf;
}

function CPView_splitSubViewHorz(StartLine)
{
	if (StartLine < 0 || StartLine >= this.m_SzY)
		return new CP_View(0, 0);

	var delta = 0;
	var yy, xx;
	for (yy = StartLine; yy < this.m_SzY-1 && delta == 0; yy++)
	{
		var count = 0;
		for (xx = 0; xx < this.m_SzX; xx++)
 		{
			if (this.getTileLabel(xx, yy) != this.getTileLabel(xx, yy+1))
				count++;
		}	
		if (count == this.m_SzX)
			delta = yy - StartLine + 1;
	}
	if (delta == 0)
		delta = this.m_SzY - StartLine;

	// create new subview
	var tf = new CP_View(this.m_SzX, delta);
	for (yy = 0; yy < delta; yy++)
	{
		for (xx = 0; xx < this.m_SzX; xx++)		
			tf.setTileLabel(xx, yy, this.getTileLabel(xx, yy + StartLine));
	}
	for (ii = 0; ii < tf.m_LabelUrl.length; ii++)
		tf.m_LabelUrl[ii] = this.m_LabelUrl[ii];

	return tf;
}


function CPView_toHtmlString(SessionId)
{
	var strHtml = "";
	if (this.isElement())
	{
		strHtml += "<frameset cols='100%'" + ">\n";
		strHtml += this.toHtmlStringRecursive(0, SessionId);		
		strHtml += "</frameset>\n";
	}
	else
	{
		strHtml = this.toHtmlStringRecursive(0, SessionId);
	}
	return strHtml;
}

function CPView_toHtmlStringRecursive(bSplitVertical, SessionId)
{
	var strHtml = "";	
	if (this.isElement())
	{
		var strUrl = this.getPage(this.getTileLabel(0,0));
		strUrl += "&SessionId=" + SessionId;

		var strName = CPView_LabelToFrameName(this.getTileLabel(0,0));
		strHtml = "<frame src='" + strUrl  + "' name='" + strName + "' marginwidth=0 marginheight=0 noresize style=\"border: inset thin white\">\n";		
	}
	else
	{
		if (bSplitVertical != 0)
		{
			var xx1 = 0;
			var ssz = "";
			var isz = 0;
			while (xx1 < this.getSizeX())
			{
				var t1 = this.splitSubViewVert(xx1);
				isz = t1.getSizeX() * (100 / this.m_MaxSizeX);
				if (xx1 == 0)
					ssz = isz +"%"
				else
					ssz += "," + isz +"%"
				xx1 += t1.getSizeX();
			}
			strHtml += "<frameset cols='" + ssz + "'" + ">\n";

			xx1 = 0;
			while (xx1 < this.getSizeX())
			{
				var t1 = this.splitSubViewVert(xx1);
				strHtml += t1.toHtmlStringRecursive(0, SessionId);
				xx1 += t1.getSizeX();
			}
			strHtml += "</frameset>\n";
		}
		else
		{
			var yy1 = 0;
			var ssz = "";
			var isz = 0;
			while (yy1 < this.getSizeY())
			{
				var t1 = this.splitSubViewHorz(yy1);
				isz = t1.getSizeY() * (100 / this.m_MaxSizeY);
				if (yy1 == 0)
					ssz = isz +"%"
				else
					ssz += "," + isz +"%"

				yy1 += t1.getSizeY();
			}

			strHtml += "<frameset rows='" + ssz + "'" + ">\n";

			yy1 = 0;
			while (yy1 < this.getSizeY())
			{
				var t1 = this.splitSubViewHorz(yy1);
				strHtml += t1.toHtmlStringRecursive(1, SessionId);
				yy1 += t1.getSizeY();
			}
			strHtml += "</frameset>\n";
		}
	}
	return strHtml;
}

function CPView_toCookieString()
{
	var CookieString = escape(this.m_Version);			    // Version
	CookieString += ":" + escape(this.m_NavigatorWidth);      // width in percent of the Navigator
	CookieString += ":" + escape(this.m_NavigatorVisible);    // 1 if Navigator is visible
	CookieString += ":" + escape(this.getSizeX());		    // number of tiles in x - direction
	CookieString += ":" + escape(this.getSizeY());		    // number of tiles in y - direction

	for (ii = 0; ii < this.m_LabelPattern.length; ii++)
		CookieString += ":" + escape(this.m_LabelPattern[ii]);	// tile pattern

	for (ii = 0; ii < this.m_LabelUrl.length; ii++)
		CookieString += ":" + escape(this.m_LabelUrl[ii]);		// linked pages

	return CookieString;
}

function CPView_fromCookieString(strCookie)
{
	if (strCookie == null || strCookie.length == 0)
		return 0;

	var tokens = strCookie.split(':');
	var version = unescape(tokens[0]);
	if (version != this.m_Version && version != "1110")
		return 0;

	var jj = 1;
	if (version == this.m_Version)
	{
		this.m_NavigatorWidth   = parseInt(unescape(tokens[jj]));
		jj++;
		this.m_NavigatorVisible = parseInt(unescape(tokens[jj]));
		jj++;
	}

	this.m_SzX = parseInt(unescape(tokens[jj]));
	jj++;
	this.m_SzY = parseInt(unescape(tokens[jj]));
	jj++;

	this.m_LabelPattern = new Array(this.m_SzX * this.m_SzY);
	this.m_LabelUrl = new Array(this.m_MaxSizeX * this.m_MaxSizeY);

	for (ii = 0; ii < this.m_LabelPattern.length; ii++, jj++)
		this.m_LabelPattern[ii] = parseInt(unescape(tokens[jj]));	// tile pattern

	for (ii = 0; ii < this.m_LabelUrl.length; ii++, jj++)
		this.m_LabelUrl[ii] = unescape(tokens[jj]);	

	return 1;
}

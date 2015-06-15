// ATCM. All rights reserved
//============================================================
// This class describes a the layout of the view as a 4 x 4 matrix
// Each element carries an ID, that indicates which URL
// covers this tile.
//============================================================

function CP_ViewMatrix()
{
	this.m_Cols = 4;
	this.m_Rows = 4;
	
	var matrixSize   = this.m_Cols * this.m_Rows;
	this.m_Matrix       = new Array(matrixSize);
	for( var i = 0; i < this.m_Cols * this.m_Rows; i++ )
	{
		this.m_Matrix[i] = 0;
	}
	
	this.get	           = CP_ViewMatrix_get;
	this.set 	        = CP_ViewMatrix_set;
	this.fill 	        = CP_ViewMatrix_fill;
	this.copy 	        = CP_ViewMatrix_copy;
	this.check	        = CP_ViewMatrix_check;
	this.checkSubMatrix = CP_ViewMatrix_checkSubMatrix;
	this.isSimple       = CP_ViewMatrix_isSimple;
	this.isConnected    = CP_ViewMatrix_isConnected;
	this.findColSeps    = CP_ViewMatrix_findColSeps;
	this.findRowSeps    = CP_ViewMatrix_findRowSeps;
	this.findUpperLeft  = CP_ViewMatrix_findUpperLeft;
	this.findLowerRight = CP_ViewMatrix_findLowerRight;
}

function CP_ViewMatrix_copy()
{
	var matrix = new CP_ViewMatrix();
	for( var i = 0; i < matrix.m_Matrix.length; i++ )
		matrix.m_Matrix[i] = this.m_Matrix[i];
	return matrix;
}
	
function CP_ViewMatrix_fill(val)
{
	for( var i = 0; i < this.m_Cols * this.m_Rows; i++ )
	{
		this.m_Matrix[i] = val;
	}	
}

function CP_ViewMatrix_set(row, col, id)
{
	var oldID = this.get(row, col);
	this.m_Matrix[row * this.m_Rows + col] = id;
}

function CP_ViewMatrix_get(row, col)
{
	if ( row >= 0 && col >= 0 && row < this.m_Rows && col < this.m_Cols )
		return this.m_Matrix[row * this.m_Rows + col];
	else
		return -1;
}

function CP_ViewMatrix_check(matrix)
{
	var isConnected = this.isConnected();

	if ( ! isConnected )
		return false;

	var valid = this.checkSubMatrix(0, 0, this.m_Rows - 1, this.m_Cols - 1);
	return valid;
}

function CP_ViewMatrix_checkSubMatrix(minRow, minCol, maxRow, maxCol)
{
	//=== check if the submatrix is a single column or row
	//=== and contains only one id
	//=== this terminates the recursion
	if ( (minRow == maxRow || minCol == maxCol) && this.isSimple(minRow, minCol, maxRow, maxCol) )
	{
		return true;
	}

  	var colSeps = this.findColSeps(minRow, minCol, maxRow, maxCol);
  	var rowSeps = this.findRowSeps(minRow, minCol, maxRow, maxCol);		
	var subTreesValid = true;

	//==== split into columns if any found
   if ( colSeps.length > 0 )
   {
		var lastSep = minCol-1;
	   for( var i = 0; subTreesValid && i < colSeps.length; i++ )
      {
			var nextSep = colSeps[i];
			subTreesValid = this.checkSubMatrix(minRow, lastSep + 1, maxRow, nextSep);
			lastSep = nextSep;
	   }	
		return subTreesValid;
	}
	//=== if no columns were found, split into rows if any were found
   else if ( rowSeps.length > 0 )
   {
		var lastSep = minRow-1;
	   for( var i = 0; subTreesValid && i < rowSeps.length; i++ )
      {
			var nextSep = rowSeps[i];
			subTreesValid = this.checkSubMatrix(lastSep + 1, minCol, nextSep, maxCol);
			lastSep = nextSep;
	   }	
		return subTreesValid;
   }
  //=== if neither columns nor rows were found
   else
   {
		if ( this.isSimple(minRow, minCol, maxRow, maxCol) )
			return true;
		return false;
   }	
}
	
function CP_ViewMatrix_isSimple(minRow, minCol, maxRow, maxCol)
{
	// check if the submatrix is a simple one,
	// that means it is filled with only one id
	var simple = true;
	var previousID = null;
	for( var i = minRow; i <= maxRow; i++ )
	{
	  for( var j = minCol; simple && j <= maxCol; j++ )
		{
		   var newID = this.get(i,j);
			if ( previousID != null && newID !=previousID )
			{
				simple = false;
			}
			previousID = newID;
		}
	}
	return simple;
}

function CP_ViewMatrix_isConnected()
{
	var valid = true;
	for( var id = 0; id < this.m_Matrix.length; id++ )
	{
		var upperLeft  = this.findUpperLeft(id);
		var lowerRight = this.findLowerRight(id);
		if ( upperLeft == null )
			continue;
		var minRow = upperLeft[0];
		var minCol = upperLeft[1];
		var maxRow = lowerRight[0];
		var maxCol = lowerRight[1];
		if ( ! this.isSimple(minRow, minCol, maxRow, maxCol) )
			valid = false;
	}
	return valid;
}

function CP_ViewMatrix_findUpperLeft(id)
{
	var r = this.m_Matrix.length;
	var c = this.m_Matrix.length;
	for( var col = 0; col < this.m_Cols; col++ )
	{
			for( var row = 0; row < this.m_Rows; row++ )
			{
				if ( this.get(row, col) == id )
				{
					c = Math.min(c, col);
					r = Math.min(r, row);
				}
			}
	}
	var upperLeft = ( r<this.m_Matrix.length  && c<this.m_Matrix.length ? new Array(r, c) : null);
	return upperLeft;
}

function CP_ViewMatrix_findLowerRight(id)
{
	var r = -1;
	var c = -1;
	for( var col = 0; col < this.m_Cols; col++ )
	{
			for( var row = 0; row < this.m_Rows; row++ )
			{
				if ( this.get(row, col) == id )
				{
					c = Math.max(c, col);
					r = Math.max(r, row);
				}			
			}
	}
	var lowerRight = ( r>-1 && c>-1 ? new Array(r, c) : null);
	return lowerRight;
}

//============================================================
// Returns an array with column indices for the specified submatrix.
// A column j < maxCol is added to this array, if in every row i the
// ID at (i,j) is different from the ID at (i, j + 1).
// This means, that there exists a through borderline right of j , 
// from the top of the  matrix to the bottom, that allows to
// split this matrix into columns. 
// If any seperators were found, then maxCol is the last entry
//============================================================
function CP_ViewMatrix_findColSeps(minRow, minCol, maxRow, maxCol)
{
	//=== reserve an array with the maximal possible length
	var colSeps = new Array(maxCol - minCol + 1);

	//=== initialize counter for seperators
	var i = 0;
	//=== run through all columns of the submatrix except the last
	for( var col = minCol; col < maxCol; col++ )
	{
		//== at the beginning each border is assumed as through
		var through = true;
		//=== run through all rows of the submatrix in the actual column
		for( var row = minRow; through && row <= maxRow; row++ )
		{
			//=== if this matrix element and its right neighbour
			//=== have the same ID, then the line is not through
			if ( this.get(row, col) == this.get(row, col + 1) )
			{		
				through = false;
			}
		}
		//=== if this border is through
		if ( through )
		{
			//=== add the column to the
			//=== array of seperators
			colSeps[i] = col;
			//=== increase counter
			i++;
		}
	}
	
	if ( i == 0 )
	{
		return new Array(0);
	}
	else
	{
		if ( colSeps[i-1] != maxCol )
		{
			i++;
			colSeps[i-1] = maxCol;
		}
		return colSeps.slice(0, i);
	}
}

function CP_ViewMatrix_findRowSeps(minRow, minCol, maxRow, maxCol)
{
	//=== reserve an array with the maximal possible length
	var rowSeps = new Array(maxRow - minRow + 1);

	//=== initialize counter for seperators
	var i = 0;
	//=== run through all rows of the submatrix except the last
	for( var row = minRow; row < maxRow; row++ )
	{
		//== at the beginning each border is assumed as through
		var through = true;
		//=== run through all columns of the submatrix in the actual row
		for( var col = minCol; through && col <= maxCol; col++ )
		{
			//=== if this matrix element and its bottom neighbour
			//=== have the same ID, then the line is not through
			if ( this.get(row, col) == this.get(row + 1, col) )
			{		
				through = false;
			}
		}
		//=== if this border is through
		if ( through )
		{
			//=== add the row to the
			//=== array of seperators
			rowSeps[i] = row;
			//=== increase counter
			i++;
		}
	}
	
	if ( i == 0 )
	{
		return new Array(0);
	}
	else
	{
		if ( rowSeps[i-1] != maxRow )
		{
			i++;
			rowSeps[i-1] = maxRow;
		}
		return rowSeps.slice(0, i);
	}
}










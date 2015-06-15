/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpUtil/CP_GrColor.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CP_GrColor.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpUtil/CP_GrColor.java $
 *
 * =PROJECT             7VH102  ATCMControl
 *
 * =SWKE                CP
 *
 * =COMPONENT           graphic
 *
 * =CURRENT             $Date: 28.02.07 19:00 $
 *                      $Revision: 1 $
 *
 * =ENVIRONMENT         Win32:
 *                      MS Windows NT 4.0
 *                      MSVC++ V5.0 / MFC 4.2 / ATL 2.0 / DAO 3.5
 *                      MS Java 2.0
 *                      MS Visual SourceSafe 5.0
 *
 * =REFERENCES
 *==
 *----------------------------------------------------------------------------*
 *

 

 

 *==
 *
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATIONS
 * $History: CP_GrColor.java $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpUtil
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpUtil
 * 
 * *****************  Version 1  *****************
 * User: Scd          Date: 20.03.01   Time: 9:28
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpUtil
 * 
 * *****************  Version 1  *****************
 * User: Mkr          Date: 30.09.98   Time: 16:13
 * Created in $/fc/CP/Java/Graphic/CpUtil
 * 
 * *****************  Version 3  *****************
 * User: Ff           Date: 29.04.98   Time: 12:42
 * Updated in $/fc/CP/lib/Java/Graphic/CPUtil
 * 
 * *****************  Version 2  *****************
 * User: Ff           Date: 23.04.98   Time: 10:16
 * Updated in $/fc/CP/lib/Java/Graphic/CPUtil
 *==
 ******************************************************************************
H<<*/

import java.util.*;
import java.awt.*;

/**
	color class for ATCMControl Panel
*/
public class CP_GrColor
{
	/**
		get color by name or by "#<hexvalue>" 
	*/
	public static Color getColor(String p_colorName)
	{
		if (p_colorName.length()<=0) return m_defaultColor;
		if (p_colorName.charAt(0)=='#') 
			return getColorByValue(p_colorName);
		else
			return getColorByName(p_colorName);
	}

	protected static Color getColorByValue(String p_colorValue)
	{
		String value;
		int ll;
		int numval;

		//== cut of first character
		ll = p_colorValue.length();
		if (ll==7) {
			value = ""+p_colorValue.substring(1,ll);
			try 
			{
 				numval = Integer.parseInt(value,16);
			} 
			catch (NumberFormatException e1)
			{
				return m_defaultColor;
			}

			return new Color(numval);
		}
		
		return m_defaultColor;
	}

	protected static Color getColorByName(String p_colorName)
	{
		int left, right, actual, comp;
		
		//== bisectioning
		left = 0;
		right = m_numColors-1;

		for (;;)
		{
			actual = ( right + left ) / 2;
			comp = p_colorName.compareTo(m_colorNames[actual]);
			
			//== found ?
			if (comp == 0) {
				return new Color(m_colorValues[actual]);
			}

			//== end of iteration
			if (actual == left ) {
				if (p_colorName.compareTo(m_colorNames[right])==0) 
				{
					return new Color(m_colorValues[right]);
				}
				break;
			}
			
			//== new bounds
			if ( comp > 0 ) 
				left = actual;
			else
				right = actual;
		}

		return m_defaultColor;
	}

	//== default color if not found
	protected static Color m_defaultColor = Color.black;

	//== number of colors in table
	protected final static int m_numColors=140;

	//== color names, field has to be sorted
	protected static String m_colorNames[] = {
		"aliceblue",
		"antiquewhite",
		"aqua",
		"aquamarine",
		"azure",
		"beige",
		"bisque",
		"black",
		"blanchedalmond",
		"blue",
		"blueviolet",
		"brown",
		"burlywood",
		"cadetblue",
		"chartreuse",
		"chocolate",
		"coral",
		"cornflowerblue",
		"cornsilk",
		"crimson",
		"cyan",
		"darkblue",
		"darkcyan",
		"darkgoldenrod",
		"darkgray",
		"darkgreen",
		"darkkhaki",
		"darkmagenta",
		"darkolivegreen",
		"darkorange",
		"darkorchid",
		"darkred",
		"darksalmon",
		"darkseagreen",
		"darkslateblue",
		"darkslategray",
		"darkturquoise",
		"darkviolet",
		"deeppink",
		"deepskyblue",
		"dimgray",
		"dodgerblue",
		"firebrick",
		"floralwhite",
		"forestgreen",
		"fuchsia",
		"gainsboro",
		"ghostwhite",
		"gold",
		"goldenrod",
		"gray",
		"green",
		"greenyellow",
		"honeydew",
		"hotpink",
		"indianred",
		"indigo",
		"ivory",
		"khaki",
		"lavender",
		"lavenderblush",
		"lawngreen",
		"lemonchiffon",
		"lightblue",
		"lightcoral",
		"lightcyan",
		"lightgoldenrodyellow",
		"lightgreen",
		"lightgrey",
		"lightpink",
		"lightsalmon",
		"lightseagreen",
		"lightskyblue",
		"lightslategray",
		"lightsteelblue",
		"lightyellow",
		"lime",
		"limegreen",	
		"linen",
		"magenta",
		"maroon",
		"mediumaquamarine",
		"mediumblue",
		"mediumorchid",
		"mediumpurple",
		"mediumseagreen",
		"mediumslateblue",
		"mediumspringgreen",
		"mediumturquoise",
		"mediumvioletred",
		"midnightblue",
		"mintcream",
		"mistyrose",
		"moccasin",
		"navajowhite",
		"navy",
		"oldlace",
		"olive",
		"olivedrab",
		"orange",
		"orangered",
		"orchid",
		"palegoldenrod",
		"palegreen",
		"paleturquoise",
		"palevioletred",
		"papayawhip",
		"peachpuff",
		"peru",
		"pink",
		"plum",
		"powderblue",
		"purple",
		"red",
		"rosybrown",
		"royalblue",
		"saddlebrown",
		"salmon",
		"sandybrown",
		"seagreen",
		"seashell",
		"sienna",
		"silver",
		"skyblue",
		"slateblue",
		"slategray",
		"snow",
		"springgreen",
		"steelblue",
		"tan",
		"teal",
		"thistle",
		"tomato",
		"turquoise",
		"violet",
		"wheat",
		"white",
		"whitesmoke",
		"yellow",
		"yellowgreen"
	};

	//== color values
	protected static int m_colorValues[] = {
		0xF0F8FF,	// aliceblue
		0xFAEBD7,	// antiquewhite
		0x00FFFF,	// aqua
		0x7FFFD4,	// aquamarine
		0xF0FFFF,	// azure
		0xF5F5DC,	// beige
		0xFFE4C4,	// bisque
		0x000000,	// black
		0xFFEBCD,	// blanchedalmond
		0x0000FF,	// blue
		0x8A2BE2,	// blueviolet
		0xA52A2A,	// brown
		0xDEB887,	// burlywood
		0x5F9EA0,	// cadetblue
		0x7FFF00,	// chartreuse
		0xD2691E,	// chocolate
		0xFF7F50,	// coral
		0x6495ED,	// cornflowerblue
		0xFFF8DC,	// cornsilk
		0xDC143C,	// crimson
		0x00FFFF,	// cyan
		0x00008B,	// darkblue
		0x008B8B,	// darkcyan
		0xB8860B,	// darkgoldenrod
		0xA9A9A9,	// darkgray
		0x006400,	// darkgreen
		0xBDB76B,	// darkkhaki
		0x8B008B,	// darkmagenta
		0x556B2F,	// darkolivegreen
		0xFF8C00,	// darkorange
		0x9932CC,	// darkorchid
		0x8B0000,	// darkred
		0xE9967A,	// darksalmon
		0x8FBC8F,	// darkseagreen
		0x483D8B,	// darkslateblue
		0x2F4F4F,	// darkslategray
		0x00CED1,	// darkturquoise
		0x9400D3,	// darkviolet
		0xFF1493,	// deeppink
		0x00BFFF,	// deepskyblue
		0x696969,	// dimgray
		0x1E90FF,	// dodgerblue
		0xB22222,	// firebrick
		0xFFFAF0,	// floralwhite
		0x228B22,	// forestgreen
		0xFF00FF,	// fuchsia
		0xDCDCDC,	// gainsboro
		0xF8F8FF,	// ghostwhite
		0xFFD700,	// gold
		0xDAA520,	// goldenrod
		0x808080,	// gray
		0x00FF00,	// green
		0xADFF2F,	// greenyellow
		0xF0FFF0,	// honeydew
		0xFF69B4,	// hotpink
		0xCD5C5C,	// indianred
		0x4B0082,	// indigo
		0xFFFFF0,	// ivory
		0xF0E68C,	// khaki
		0xE6E6FA,	// lavender
		0xFFF0F5,	// lavenderblush
		0x7CFC00,	// lawngreen
		0xFFFACD,	// lemonchiffon
		0xADD8E6,	// lightblue
		0xF08080,	// lightcoral
		0xE0FFFF,	// ightcyan
		0xFAFAD2,	// lightgoldenrodyellow
		0x90EE90,	// lightgreen
		0xD3D3D3,	// lightgrey
		0xFFB6C1,	// lightpink
		0xFFA07A,	// lightsalmon
		0x20B2AA,	// lightseagreen
		0x87CEFA,	// lightskyblue
		0x778899,	// lightslategray
		0xB0C4DE,	// lightsteelblue
		0xFFFFE0,	// lightyellow
		0x00FF00,	// lime
		0x32CD32,	// limegreen	
		0xFAF0E6,	// linen
		0xFF00FF,	// magenta
		0x800000,	// maroon
		0x66CDAA,	// mediumaquamarine
		0x0000CD,	// mediumblue
		0xBA55D3,	// mediumorchid
		0x9370DB,	// mediumpurple
		0x3CB371,	// mediumseagreen
		0x7B68EE,	// mediumslateblue
		0x00FA9A,	// mediumspringgreen
		0x48D1CC,	// mediumturquoise
		0xC71585,	// mediumvioletred
		0x191970,	// midnightblue
		0xF5FFFA,	// mintcream
		0xFFE4E1,	// mistyrose
		0xFFE4B5,	// moccasin
		0xFFDEAD,	// navajowhite
		0x000080,	// navy
		0xFDF5E6,	// oldlace
		0x808000,	// olive
		0x6B8E23,	// olivedrab
		0xFFA500,	// orange
		0xFF4500,	// orangered
		0xDA70D6,	// orchid
		0xEEE8AA,	// palegoldenrod
		0x98FB98,	// palegreen
		0xAFEEEE,	// paleturquoise
		0xDB7093,	// palevioletred
		0xFFEFD5,	// papayawhip
		0xFFDAB9,	// peachpuff
		0xCD853F,	// peru
		0xFFC0CB,	// pink
		0xDDA0DD,	// plum
		0xB0E0E6,	// powderblue
		0x800080,	// purple
		0xFF0000,	// red
		0xBC8F8F,	// rosybrown
		0x4169E1,	// royalblue
		0x8B4513,	// saddlebrown
		0xFA8072,	// salmon
		0xF4A460,	// sandybrown
		0x2E8B57,	// seagreen
		0xFFF5EE,	// seashell
		0xA0522D,	// sienna
		0xC0C0C0,	// silver
		0x87CEEB,	// skyblue
		0x6A5ACD,	// slateblue
		0x708090,	// slategray
		0xFFFAFA,	// snow
		0x00FF7F,	// springgreen
		0x4682B4,	// steelblue
		0xD2B48C,	// tan
		0x008080,	// teal
		0xD8BFD8,	// thistle
		0xFF6347,	// tomato
		0x40E0D0,	// turquoise
		0xEE82EE,	// violet
		0xF5DEB3,	// wheat
		0xFFFFFF,	// white
		0xF5F5F5,	// whitesmoke
		0xFFFF00,	// yellow
		0x9ACD32	// yellowgreen
	};
}
/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpScale/CP_GrScaleModel.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CP_GrScaleModel.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpScale/CP_GrScaleModel.java $
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
 * $History: CP_GrScaleModel.java $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpScale
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpScale
 * 
 * *****************  Version 1  *****************
 * User: Scd          Date: 19.03.01   Time: 15:57
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpScale
 * 
 * *****************  Version 1  *****************
 * User: Mkr          Date: 30.09.98   Time: 16:13
 * Created in $/fc/CP/Java/Graphic/CpScale
 * 
 * *****************  Version 7  *****************
 * User: Ff           Date: 24.06.98   Time: 9:24
 * Updated in $/fc/CP/lib/Java/Graphic/CPScale
 * 
 * *****************  Version 6  *****************
 * User: Ff           Date: 17.06.98   Time: 16:57
 * Updated in $/fc/CP/lib/Java/Graphic/CPScale
 * 
 * *****************  Version 5  *****************
 * User: Ff           Date: 9.06.98    Time: 16:58
 * Updated in $/fc/CP/lib/Java/Graphic/CPScale
 * 
 * *****************  Version 4  *****************
 * User: Ff           Date: 29.04.98   Time: 12:42
 * Updated in $/fc/CP/lib/Java/Graphic/CPScale
 * 
 * *****************  Version 3  *****************
 * User: Ff           Date: 23.04.98   Time: 10:16
 * Updated in $/fc/CP/lib/Java/Graphic/CPScale
 *==
 ******************************************************************************
H<<*/

import java.lang.reflect.*;

//================================================================================
/**
  * This class encapsulates the screen independent parts of the scale compenent,
  * A scale is determined by the following parameters:
  *	<ul>
  * <li> the minimum of the scale domain: <tt>domainMin</tt>
  * <li> the maximum of the scale domain: <tt>domainMax</tt>
  * <li> the minimal allowed stepwidth: <tt>stepWidthMin</tt>
  * <li> a set of bases: <tt>bases</tt>
  * <li> a radix: <tt>radix</tt>
  * </ul>
  * The domain and the radix are fixed values, while the set of bases and the stepwidth
  * minimum are constraints, that determine a unique scale out of the set of
  * all scales.
  * A scale consists of all scalesteps that lie within the scale domain. 
  * Each scalestep is an integer multiple of the stepwidth of the scale, which is defined as:
  * <br><tt>stepwidth = base * radix<sup>exponent</sup></tt>, where
  * <ol>
  * <li><tt>base</tt> is from the set of allowed bases
  * <li><tt>radix</tt> is any integer greater than 1
  * <li><tt>exponent</tt> is any integer
  * </ol>
  * A scale for a given domain and radix is called better, if its stepwidth is smaller.
  * A optimal scale is the best scale for a given domain, radix and constraints for
  * stepwidth and bases.
  */
//================================================================================
class CP_GrScaleModel 
{
	//============================================================
	/** The debug output interface. */
	//============================================================
	/*_CPDEBUG_BEGIN
	static public CP_Debug m_debug;
_CPDEBUG_END*/

	//============================================================
	/** The minimum of the scale domain. */
	//============================================================
	protected double m_domainMin;

	//============================================================
	/** The maximum of the scale domain. */
	//============================================================
	protected double m_domainMax;

	//============================================================
	/** The minimal allowed step width. */
	//============================================================
	protected double m_stepWidthMin;

	//============================================================
	/** The set of allowed bases. */
	//============================================================
	protected long[] m_bases;
	
	//============================================================
	/** The radix, that shall be used. */
	//============================================================
	protected long m_radix;

	//============================================================
	/** The computed optimal stepwidth. */
	//============================================================
	protected double m_stepWidth;

	//============================================================
	/** The number of steps inside domain. */
	//============================================================
	protected long m_numberOfSteps;

	//============================================================
	/** Constructs a new scale model with default values.
	*/
	//============================================================
	CP_GrScaleModel()
	{		
		//=== set defaults
		setToDefaults();
	}

	//============================================================
	/** Computes the class invariant.
	* <tt> getStepWidth > 0</tt> implies
	* <ul>
	* <li><tt>domain is valid</tt>
	* <li><tt>radix is valid</tt>
	* <li><tt>base constraints are valid</tt>
	* <li><tt>stepwidth constraints are valid</tt>
	* <li><tt>m_stepWidthMin <= getStepWidth()</tt>
	* <li><tt>getNumberOfSteps() > 0</tt> implies
	*   <ul>
	*   <li><tt>(getNumberOfSteps() - 1)* getStepWidth() <= m_domainMax - m_domainMin</tt>
	*   <li><tt>getStep(0) <= getStep(getNumberOfSteps() - 1)</tt>
	*	<li><tt>getStep(0) >= m_domainMin</tt>
	*	<li><tt>getStep(getNumberOfSteps() - 1) <= m_domainMax</tt>
	*   <li><tt>getStep(0) - m_domainMin < getStepWidth()</tt>
	*   <li><tt>m_domainMax - getStep(getNumberOfSteps() - 1) < getStepWidth()</tt>
	*	</ul>
	* </ul>
	*/
	//============================================================
	protected boolean invariant()
	{
		double stepWidth = getStepWidth();
		long steps = getNumberOfSteps();
		if ( 
			stepWidth > 0 && 
			validDomain(m_domainMin, m_domainMax) &&
			validRadix(m_radix) &&
			validBaseConstraints(m_bases) &&
			validStepWidthConstraints(m_stepWidthMin) &&
			m_stepWidthMin <= stepWidth &&
			steps > 0
			)
		{
			double firstStep = getStep(0);
			double lastStep  = getStep(steps - 1);
			boolean b1 = (steps - 1) * stepWidth <= m_domainMax - m_domainMin;
			boolean b2 = firstStep <= lastStep;
			boolean b3 = firstStep >= m_domainMin;
			boolean b4 = lastStep <= m_domainMax;
			boolean b5 = firstStep - m_domainMin < stepWidth;
			boolean b6 = m_domainMax - lastStep < stepWidth;
			return b1 & b2 & b3 & b4 & b5 & b6;
		}
		return true;
	}
	
	//============================================================
	/** Checks if a domain is valid.
	*/
	//============================================================	
	protected boolean validDomain(double min, double max)
	{
		return min <= max;
	}

	//============================================================
	/** Checks if base constraints are valid.
	*/
	//============================================================	
	protected boolean validBaseConstraints(long[] bases)
	{
		boolean valid = bases.length > 0;
		for( int i = 0; i < bases.length; i++ )
		{
			valid &= bases[i] > 0;
		}
		return valid;
	}

	//============================================================
	/** Checks if radix is valid.
	*/
	//============================================================	
	protected boolean validRadix(long radix)
	{
		return radix > 1;
	}

	//============================================================
	/** Checks if stepwidthconstraints are valid.
	*/
	//============================================================	
	protected boolean validStepWidthConstraints(double stepWidthMin)
	{
		return stepWidthMin > 0;
	}

	//============================================================
	/** Sets all scale parameters and constraints to default values.
	* <ul>
	*    <li> domain minimum: 0
	*    <li> domain maximum: 100
	*    <li> stepWidth minimum:   2
	*    <li> bases: 1 2 25 5
	*    <li> radix: 10
    * </ul>
	*/
	//============================================================
	public void setToDefaults()
	{
		setDomain(0, 100);
		setStepWidthConstraints(2);
		long[] bases = { 1, 2, 25, 5 };
		setBaseConstraints(bases);
		setRadix(10);

		//=== assert class invariant
		/*_CPDEBUG_BEGIN
		m_debug.assertInv(invariant(), ""); 
_CPDEBUG_END*/
	}
	
	//============================================================
	/** Sets the scale domain.
	* @param min domain minimum
	* @param max domain maximum
	*/
	//============================================================
	public void setDomain(double min, double max) 
	{
		m_domainMin = min;
		m_domainMax = max;
		m_stepWidth = 0;
		
		//=== assert class invariant
		/*_CPDEBUG_BEGIN
		m_debug.assertInv(invariant(), ""); 
_CPDEBUG_END*/
	}

	//============================================================
	/** Sets the constraints for stepwidth.
	* @param stepWidthMin stepwidth minimum
	*/
	//============================================================
	public void setStepWidthConstraints(double min) {

		m_stepWidthMin   = min;
		m_stepWidth = 0;

		//=== assert class invariant
		/*_CPDEBUG_BEGIN
		m_debug.assertInv(invariant(), ""); 
_CPDEBUG_END*/
	}

	//============================================================
	/** Sets the constraints for bases.
	* @param bases array with allowed bases	
	*/
	//============================================================
	public void setBaseConstraints(long[] bases) 
	{

		m_bases = bases;
		m_stepWidth = 0;

		//=== assert class invariant
		/*_CPDEBUG_BEGIN
		m_debug.assertInv(invariant(), ""); 
_CPDEBUG_END*/
	}
	
	//============================================================
	/** Sets the radix.
	* @param radix
	*/
	//============================================================
	public void setRadix(long radix) 
	{
		m_radix = radix;
		m_stepWidth = 0;

		//=== assert class invariant
		/*_CPDEBUG_BEGIN
		m_debug.assertInv(invariant(), ""); 
_CPDEBUG_END*/
	}

	//============================================================
	/** Optimizes this scale model.
	* @see CP_GrScaleModel.computeStepWidthOpt
	* @see CP_GrScaleModel.computeNumberOfSteps
	*/
	//============================================================
	public void optimize() 
	{

		m_stepWidth = computeStepWidthOpt(m_bases, m_radix, m_stepWidthMin);
		m_numberOfSteps = computeNumberOfSteps(m_domainMin, m_domainMax, m_stepWidth);

		//=== assert class invariant
		/*_CPDEBUG_BEGIN
		m_debug.assertInv(invariant(), ""); 
_CPDEBUG_END*/
	}

	//============================================================
	/** Count number of scale steps inside domain.
	* If domain or stepwidth is invalid, a negative value is returned.
	* @param domainMin
	* @param domainMax
	* @param stepWidth
	* @return scaleSteps
	* Preconditions:
	* <ul>
	* <li><tt>domainMin <= domainMax</tt>
	* <li><tt>stepWidth > 0</tt>
	* </ul>
	* Postconditions:
	* <ul>
	* <li><tt>precondition</tt> implies <tt>steps * m_stepWidth <= m_domainMax - m_domainMin</tt>
	* <li><tt>!precondition</tt> implies <tt>steps < 0</tt>
	* </ul>
	*/
	//============================================================
	protected long computeNumberOfSteps(double domainMin, double domainMax, double stepWidth) 
	{			

/*_CPDEBUG_BEGIN
		//=== test precondition
		boolean precondition = 
			validDomain(domainMin, domainMax) &&
			stepWidth > 0;

		//===  check precondition
		m_debug.checkPre(precondition, getClass().getName() + ".computeNumberOfSteps");
_CPDEBUG_END*/
	
		//=== compute number of steps
		long steps = (long)Math.floor(domainMax/stepWidth) - (long)Math.ceil(domainMin/stepWidth) + 1;

/*_CPDEBUG_BEGIN
		//=== test postcondition
		boolean postcondition = 
			(steps - 1) * m_stepWidth <= m_domainMax - m_domainMin || !precondition;

		//=== assert postcondition
		m_debug.assertPost(postcondition, "");
_CPDEBUG_END*/

		//=== finished
		return steps;
	}
	
	//============================================================
	/** Get value of the i-th step.
	* If the indexparameter i is invalid <tt>NaN</tt> is returned.
	* @param i
	* @return stepValue
	*/
	//============================================================
	public double getStep(long i) 
	{
		if ( 0 <= i && i < m_numberOfSteps )
		{
			return ( (long)Math.ceil(m_domainMin/m_stepWidth) + i ) * m_stepWidth;
		}
		else 
		{
			return Double.NaN;
		}
	}

	//============================================================
	/** Computes an optimal stepwidth.
	* If the parameters are invalid or no scale fulfilles the constraints 
	* the value 0 is returned.
    * @param bases
	* @param radix
	* @param stepWidthMin
	* @return stepWidthOpt
    * Preconditions:
	* <ol>
	* <li><tt>bases.length > 0</tt>
	* <li><tt>bases[i] > 0</tt>, for all <tt>0 <= i < bases.length</tt>
	* <li><tt>radix > 1</tt>
	* <li><tt>stepWidthMin > 0</tt>
	* </ol>
	* Algorithm:<br>
	* If one of the preconditions fails, <tt>stepWidthOpt</tt> is set to 0 ( zero ).
	* Otherwise for every allowed base a minimal stepwidth is computed.
	* The optimal stepwidth is the minimum of all computed stepwidths.
	* <br>PostConditions:
	* <ol>
	* <li><tt>precondition</tt> implies 
	*    <ol>
	*     <li><tt>stepWidthMin <= stepWidthOpt</tt>
	*     <li><tt>stepWidthOpt = bases[i] * radix<sup>exponent</sup></tt>,
	*         for any <tt>int i</tt> with <tt>0 <= i <= bases.length</tt>
	*         and <tt>int exponent<tt>.
	*    </ol>
	* <li><tt>! precondition</tt> implies <tt>stepWidthOpt == 0</tt>
	* </ol>
	* @see CP_GrScaleModel.computeMinExponent
	*/
	//============================================================
	protected double computeStepWidthOpt(long[] bases, long radix, double stepWidthMin) 
	{
		//=== initialize optimal stepwidth
		double stepWidthOpt = 0;
		int optBaseIndex = -1;
		long optExponent = 0;

		//=== test precondition
		boolean precondition = 
			validRadix(radix) &&
			validBaseConstraints(bases) &&
			validStepWidthConstraints(stepWidthMin); 

		//=== check precondition
		/*_CPDEBUG_BEGIN
		m_debug.checkPre(precondition, getClass().getName() + ".computeStepWidthOpt");
_CPDEBUG_END*/

		//=== if precondition is fulfilled optimize
		if ( precondition )
		{
			//=== for every allowed base
			for(int i = 0; i < bases.length; i++ ) 
			{
				//=== compute the minimal stepwidth for this base
				//=== by computing the minimal exponent
				long exponent = computeMinExponent(bases[i], radix, stepWidthMin);
				double stepWidth = computeStepWidth(bases[i], radix, exponent);

				//=== if this minimal stepwidth is smaller than the actual optimal stepwidth ...
				//=== NOTE: if this is the first base considered, the actual optimal
				//=== stepwidth is still zero.
				if ( stepWidthOpt == 0 || stepWidth <= stepWidthOpt ) 
				{
					//=== ... then store this stepwidth as the new optimal stepwidth
					optBaseIndex = i;
					optExponent = exponent;
					stepWidthOpt = stepWidth;
				}
			}
		}
		//=== if precondition failed
		else
		{
			//=== do nothing and leave the optimal stepwidth at value zero.
		}

/*_CPDEBUG_BEGIN
		//=== test postcondition
		boolean postcondition = 
			( 
			stepWidthMin <= stepWidthOpt &&
			stepWidthOpt == computeStepWidth(bases[optBaseIndex], radix, optExponent)
			) || stepWidthOpt == 0;

		//=== assert postcondition
		m_debug.assertPost(postcondition, "");
_CPDEBUG_END*/

		//=== return optimal stepwidth
		//=== NOTE: this is zero if precondition failed
		return stepWidthOpt;
	}

	
	//============================================================
	/** Computes a minimal exponent for a given base, radix and a minimal step width.
	* @param base
	* @param radix
	* @param stepWidthMin
	* @return exponent
	* Preconditions:
	* <ol>
	* <li><tt>base > 0</tt>
	* <li><tt>radix > 1</tt>
	* <li><tt>stepWidthMin > 0</tt>
	* </ol>
	* Algorithm:<br>
	* <tt>exponent = ceiling(log<sub>radix</sub>(stepWidthMin/base))</tt>
	* <br>Postconditions:
	* <ol>
	* <li><tt>precondition</tt> implies <tt>base * radix<sup>exponent</sup> <= stepWidthMin</tt>
	* </ol>
	*/
	//============================================================
	protected long computeMinExponent(long base, long radix, double stepWidthMin) 
	{
		//=== test precondition
		boolean precondition = 
			base > 0 &&
			validRadix(radix) &&
			stepWidthMin > 0
		;

		//=== assert precondition
		/*_CPDEBUG_BEGIN
		m_debug.assertPre(precondition, "");
_CPDEBUG_END*/

		//=== initialize exponent
		long exponent = 0;

		//=== if precondition is fulfilled compute exponent
		if ( precondition ) {
			exponent = (long)Math.ceil( Math.log( stepWidthMin / base ) / Math.log(radix) );
		}
		
		/*_CPDEBUG_BEGIN
		//=== test postcondition
		boolean postcondition = 
			computeStepWidth(base, radix, exponent) >= stepWidthMin || !precondition;
			
		//=== assert postcondition
		m_debug.assertPost(postcondition, "");
_CPDEBUG_END*/

		return exponent;
	}

	//============================================================
	/** Computes the stepwidth for the given parameters.
	* <tt>stepWidth = base * radix<sup>exponent</sup></tt>
	* @param base
	* @param radix
	* @param exponent
	* @return stepWidth
	*/
	//============================================================
	protected double computeStepWidth(long base, long radix, long exponent)
	{
		return ( base * Math.pow(radix, exponent) );
	}

	//============================================================
	/** Returns the actual stepwidth.
	* @return stepWidth
	*/
	//============================================================
	public double getStepWidth() {
		return m_stepWidth;
	}

	//============================================================
	/** Returns the domain minimum.
	* @return minimum
	*/
	//============================================================
	public double getMin() {
		return m_domainMin;
	}

	//============================================================
	/** Returns the domain maximum.
	* @return maximum
	*/
	//============================================================
	public double getMax() {
		return m_domainMax;
	}

	//============================================================
	/** Returns the actual number of scalesteps in domain.
	* @return steps
	*/
	//============================================================
	public long getNumberOfSteps() {
		return m_numberOfSteps;
	}

	//============================================================
	/** Returns a string description of this object.
	* @return description
	*/
	//============================================================
	public String toString() {
		String description = "CP_GrScaleModel: ";
		description += " domain=[" + String.valueOf(m_domainMin) + "," + String.valueOf(m_domainMax) +  "]";
		description += ", radix=" + String.valueOf(m_radix);
		description += ", bases={";
		for(int i = 0; i < m_bases.length; i++)
		{
			if ( i > 0 )
			{
				description += " ";
			}
			description += String.valueOf(m_bases[i]);
		}
		description += "}";
		description += ", stepWidthMin=" + String.valueOf(m_stepWidthMin);
		description += ", stepWidth=" + String.valueOf(m_stepWidth);

		return description;
	}
}


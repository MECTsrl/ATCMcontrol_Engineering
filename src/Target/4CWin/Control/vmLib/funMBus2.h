/* $Header: /4CReleased/V2.20.00/Target/4CWin/CONTROL/vmLib/funMBus2.h 1     28.02.07 19:10 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: funMBus2.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/4CWin/CONTROL/vmLib/funMBus2.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				RunTime
 *
 * =COMPONENT			vmLib
 *
 * =CURRENT 	 $Date: 28.02.07 19:10 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#if defined(RTS_CFG_MBUS2_LIB)

									f_mbus2_read_volume 				,	/* 200 */
									f_mbus2_read_units_for_HCA			,	/* 201 */
									f_mbus2_read_time_point 			,	/* 202 */
									f_mbus2_read_temperature_difference ,	/* 203 */
									f_mbus2_read_return_temperature 	,	/* 204 */
									f_mbus2_read_pressure				,	/* 205 */
									f_mbus2_read_power					,	/* 206 */
									f_mbus2_read_operating_time 		,	/* 207 */
									f_mbus2_read_on_time				,	/* 208 */
									f_mbus2_read_mass_flow				,	/* 209 */
									f_mbus2_read_mass					,	/* 210 */
									f_mbus2_read_flow_temperature		,	/* 211 */
									f_mbus2_read_energy 				,	/* 212 */
									f_mbus2_read_averaging_duration 	,	/* 213 */
									f_mbus2_read_actuality_duration 	,	/* 214 */
									f_mbus2_read_volume_flow			,	/* 215 */
									f_mbus2_read_external_temperature	,	/* 216 */
									NULL								,	/* 217 */
									NULL								,	/* 218 */
									NULL								,	/* 219 */
									NULL								,	/* 220 */
									NULL								,	/* 221 */
									NULL								,	/* 222 */
									NULL								,	/* 223 */
									NULL								,	/* 224 */
									NULL								,	/* 225 */
									NULL								,	/* 226 */
									NULL								,	/* 227 */
									NULL								,	/* 228 */
									NULL								,	/* 229 */

#else	/* RTS_CFG_MBUS2_LIB */

									NULL								,	/* 200 */
									NULL								,	/* 201 */
									NULL								,	/* 202 */
									NULL								,	/* 203 */
									NULL								,	/* 204 */
									NULL								,	/* 205 */
									NULL								,	/* 206 */
									NULL								,	/* 207 */
									NULL								,	/* 208 */
									NULL								,	/* 209 */
									NULL								,	/* 210 */
									NULL								,	/* 211 */
									NULL								,	/* 212 */
									NULL								,	/* 213 */
									NULL								,	/* 214 */
									NULL								,	/* 215 */
									NULL								,	/* 216 */
									NULL								,	/* 217 */
									NULL								,	/* 218 */
									NULL								,	/* 219 */
									NULL								,	/* 220 */
									NULL								,	/* 221 */
									NULL								,	/* 222 */
									NULL								,	/* 223 */
									NULL								,	/* 224 */
									NULL								,	/* 225 */
									NULL								,	/* 226 */
									NULL								,	/* 227 */
									NULL								,	/* 228 */
									NULL								,	/* 229 */

#endif	/* RTS_CFG_MBUS2_LIB */

/* ---------------------------------------------------------------------------- */


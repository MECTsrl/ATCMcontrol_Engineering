/* $Header: /4CReleased/V2.20.00/TargetBase/PROJECTWIZARDBASE/ProjectWizardDefs.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: ProjectWizardDefs.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/PROJECTWIZARDBASE/ProjectWizardDefs.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase
 *
 * =COMPONENT			ProjectWizardBase
 *
 * =CURRENT 	 $Date: 28.02.07 19:06 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _PROJECTWIZARDDEFS_H_
#define _PROJECTWIZARDDEFS_H_

#define FILE_EXT_PROJECT          _T("4cp")
#define FILE_EXT_CONFIGURATION    _T("con")
#define FILE_EXT_RESOURCE         _T("res")
#define FILE_EXT_GLOBAL_VARIABLES _T("gvl")
#define FILE_EXT_CONSTANTS        _T("cst")
#define FILE_EXT_TARGETS          _T("4cpc")
#define FILE_EXT_LIBRARY          _T("4cl")
#define FILE_EXT_LATCOMM          _T("4clc")

#define CONFIGURATION_FILE_NAME   _T("Configuration1")

typedef int TSubSystems;

enum
{
    SUBSYSTEM_NONE = 0x0,
    SUBSYSTEM_ANY = 0x1,
    SUBSYSTEM_PROFIBUS = 0x2,
    SUBSYSTEM_OPC = 0x4,
    SUBSYSTEM_SERIAL_COMMUNICATION = 0x8,
    SUBSYSTEM_FC = 0x0010,
    SUBSYSTEM_BC = 0x0020,
    SUBSYSTEM_DC = 0x0040,
    SUBSYSTEM_GA = 0x0080
};


enum TIecLanguage
{
    IEC_LANGUAGE_ST,
    IEC_LANGUAGE_FBD,    
    IEC_LANGUAGE_LD,
    IEC_LANGUAGE_SFC,
    IEC_LANGUAGE_IL
};

#endif

/* ---------------------------------------------------------------------------- */

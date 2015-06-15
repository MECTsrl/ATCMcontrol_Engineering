/* ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: SetupDef.h $
 *						 $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/Setup/SetupDef.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				CSET
 *
 * =COMPONENT			Tools\Setup
 *
 * =CURRENT 	 $Date: 28.02.07 19:01 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _SETUPDEF_H_
#define _SETUPDEF_H_

#define SET_INI_FILE_COMMON			"Setup.ini"
#define SET_INI_FILE_PC				"PC-Based.ini"
#define SET_INI_FILE_LANGUAGE		"Language.ini"

#define SET_LIC_FILE				"Setup\\License.rtf"

#define SET_LOG_TRACE				"c:\\Setup.trc"
#define	SET_LOG_INSTALL				"c:\\Setup4.Inst.log"
#define	SET_LOG_UNINSTALL			"c:\\Setup4.UnInst.log"

#define SET_DEFDIR					"ATCMControlV2"

#define SET_HKEY_SETUP				"Software\\Softing\\Setup4C"
#define SET_HKEY_INSTPATH			"Software\\Softing\\4ControlV2\\2.0"
#define SET_HKEY_LM_INSTPATH		"Software\\Softing\\Products\\0"
#define SET_HKEY_CURRVERSION		"Software\\Microsoft\\Windows\\CurrentVersion"
#define SET_HKEY_RUNONCE			"Software\\Microsoft\\Windows\\CurrentVersion\\Run"

#define SET_HVAL_INSTPATH			"Path"
#define SET_HVAL_LM_INSTPATH		"VersionFile"
#define SET_HVAL_PROGDIR			"ProgramFilesDir"
#define SET_HVAL_RUNONCE			"Setup4C"
#define SET_HVAL_4C_COMPONENTS		"4C Components"
#define SET_HVAL_4CE_COMPONENTS		"4C %s Components"
#define SET_HVAL_LANGUAGE			"Language"		

#define SET_SCT_COMPONENTS			"Components"
#define SET_SCT_GENERAL				"General"
#define SET_SCT_CONVERSION			"Conversion"
#define SET_SCT_INSTALLPATH			"Installer"
#define SET_SCT_DESCRIPTION			"Description"
#define SET_SCT_PCODE				"Product Code"
#define SET_SCT_SUBDIR				"SubDir"
#define SET_SCT_MS_SETUP			"Microsoft Setup"
#define	SET_SCT_ICON				"Icon"
#define SET_SCT_LANGUAGE			"Languages"
#define SET_SCT_MINVER				"Min Version"
#define SET_SCT_SERVPACK			"Service Pack"

#define SET_KEY_CAPTION				"Caption"
#define SET_KEY_PNAME				"PName"
#define SET_KEY_TYPE				"Type"
#define SET_KEY_CUST				"Cust"

#define SET_TYPE_PC					"PC"
#define SET_TYPE_EMBEDDED			"EMB"

#define SET_INI_4CKERNEL			"4CKernel"
#define SET_INI_4CCONTROL			"4CControl"
#define SET_INI_4CADDON				"4CAddOn"
#define SET_INI_4CLM				"4CLM"
#define SET_INI_4CALL				"4CAddOnAll"
#define SET_INI_SYSTEM				"System"

#define SET_PRODKEY_SP				"SP"
#define SET_PRODKEY_IE				"IE"
#define SET_PRODKEY_MSI				"MSI"
#define SET_PRODKEY_JDK				"JDK"
#define SET_PRODKEY_JVM				"JVM"
#define SET_PRODKEY_IIS				"IIS"
#define SET_PRODKEY_KERNEL			"KRNL"
#define SET_PRODKEY_LM				"LM"
#define SET_PRODKEY_CTRL			"CTRL"
#define SET_PRODKEY_PCAO			"PCAO"
#define SET_PRODKEY_ECAO			"ECAO"
#define SET_PRODKEY_FCAO			"FCAO"
#define SET_PRODKEY_BCAO			"BCAO"
#define SET_PRODKEY_XCAO			"XCAO"
#define SET_PRODKEY_DCAO			"DCAO"
#define SET_PRODKEY_GAAO			"GAAO"
#define SET_PRODKEY_PCDB			"PCDB"
#define SET_PRODKEY_PCCL			"PCCL"
#define SET_PRODKEY_DEMO			"DEMO"
#define SET_PRODKEY_AO1				"AO1"
#define SET_PRODKEY_AO2				"AO2"
#define SET_PRODKEY_AO3				"AO3"
#define SET_PRODKEY_AO4				"AO4"

#define SET_PRODFEAT_CONS			"CONS"
#define SET_PRODFEAT_RTCE			"RTCE"
#define	SET_PRODFEAT_OPC			"OPC"

#define SET_SUFFIX_GER				"de"
#define SET_SUFFIX_ENG				"en"
#define SET_SUFFIX_ITL				"it"

#define SET_SUFFIX_NT				"NT"
#define SET_SUFFIX_2K				"2K"
#define SET_SUFFIX_XP				"XP"
#define SET_SUFFIX_9X				"9x"
#define SET_SUFFIX_2K3				"2K3"
#define SET_SUFFIX_VI				"VI"

#define SET_OS_VALID_OK				0
#define SET_OS_VALID_WARN			1
#define SET_OS_VALID_NOK			2

#define SET_DONT_CARE				"---"

// Key and install order for AddOn map
#define SET_CTRL_KEY_IIS			0u
#define SET_CTRL_KEY_SP				1u
#define SET_CTRL_KEY_IE				2u
#define SET_CTRL_KEY_MSI			3u
#define SET_CTRL_KEY_JDK			4u
#define SET_CTRL_KEY_JVM			5u
#define SET_CTRL_KEY_LM				8u
#define SET_CTRL_KEY_CTRL			9u			
#define SET_CTRL_KEY_KRNL			10u

#define SET_CTRL_LAST_MS			SET_CTRL_KEY_JVM
#define SET_CTRL_LAST_4C			SET_CTRL_KEY_KRNL

#define SET_HELPLANG_DE				"Deutsch"
#define	SET_HELPLANG_EN				"English"

#define SET_LANGID_DE				1031
#define SET_LANGID_EN				1033

#define SET_ICO_OS					0
#define SET_ICO_IE					1
#define SET_ICO_MSI					2
#define SET_ICO_IIS					3
#define SET_ICO_J					4

#define SET_ICO_LM					5

#define SET_ICO_4CKERN				6
#define SET_ICO_4CADDON				7

#define SET_ICO_C1					8
#define SET_ICO_C2					9
#define SET_ICO_C3					10
#define SET_ICO_C4					11
#define SET_ICO_C5					12


#define SET_FLAG_KERNEL				0x00000001u
#define SET_FLAG_CTRL				0x00000002u
#define SET_FLAG_PCAO				0x00000004u
#define SET_FLAG_ECAO				0x00000008u
#define SET_FLAG_FCAO				0x00000010u
#define SET_FLAG_PCDB				0x00000020u
#define SET_FLAG_PCCL				0x00000040u
#define SET_FLAG_BCAO				0x00000080u
#define SET_FLAG_XCAO				0x00000100u
#define SET_FLAG_GAAO				0x00000200u

#define SET_FLAG_DEMO				0x00000800u

#define SET_FLAG_AO1				0x00001000u
#define SET_FLAG_AO2				0x00002000u
#define SET_FLAG_AO3				0x00004000u
#define SET_FLAG_AO4				0x00005000u

#define SET_FLAG_CONS				0x01000000u
#define SET_FLAG_RTCE				0x02000000u
#define	SET_FLAG_EC_OPC				0x10000000u
#define	SET_FLAG_FC_OPC				0x20000000u

/* Features as defined in Install Shield projects
 */
#define SET_FEAT_CTRL				"Control"
#define SET_FEAT_RTCE				"RTCE"
#define SET_FEAT_CONS				"Console"

#endif	// _SETUPDEF_H_

/* ---------------------------------------------------------------------------- */

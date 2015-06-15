/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/inc/VmConstants.h 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: VmConstants.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/inc/VmConstants.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / CCG
 *
 * =COMPONENT			inc
 *
 * =CURRENT 	 $Date: 28.02.07 19:04 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _VMCONSTANTS_H_
#define _VMCONSTANTS_H_


/*                                                         */
/*                                                         */
/* defines for global properties of the custom specific VM */
/*                                                         */
/* begin                                                   */

// the internal register size of the target CPU
#define VM_REGISTER_SIZE        32

// the size of a member offset
#define VM_OFFSET_SIZE          16

// align members at the given bit number
// (e.g. 8=at byte boundaries ...)
#define VM_ALIGN_SIZE           16

// align bit members at the given bit number
// (e.g. 8=at byte boundaries ...)
#define VM_BIT_ALIGN_SIZE       1

// size of one bit (Type BOOL) located in instance data ( (1) 8, 16, 32)
#define VM_BOOL_BIT_SIZE        VM_BIT_ALIGN_SIZE

// size of one bit in a boolean array located in instance data ( (1) 8, 16, 32)
#define VM_BOOLARRAY_BIT_SIZE   32

// size of the index referencing on object in the object map (16, 32)
#define VM_OBJ_INDEX_SIZE       16

// size of a real address on the target (16, 32)
#define VM_ADDRESS_SIZE         32

// size of a the array boundary stored in the array itself (16, 32)
#define VM_ARRAY_LIMIT_SIZE     16

// size of the opcode (8, 16, 32)
#define VM_OPCODE_SIZE          16

// size of the class id (8, 16, 32)
#define VM_CLASS_ID_SIZE        16

// size of the class id (8, 16, 32)
#define VM_COPY_MEM_SIZE        16

// size of number of parameter (8, 16, 32)
#define VM_NUM_OF_PARAM_SIZE    16

// size bit address (8, 16, 32)
#define VM_BIT_ADDRESS_SIZE     32

#define VM_MAX_FBNESTING        25
#define VM_MAX_STRLEN           255

#define VM_SEGMENT_SIZE         (0xffff+1)

#endif

/* ---------------------------------------------------------------------------- */
